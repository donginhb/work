#include "afxwin.h"
#include "wmxds.h"

#include <streams.h>
#include <atlbase.h>
#include <dshowasf.h>
#include <dmodshow.h>
#include <qedit.h>

#include <wmsysprf.h>

#include "HelperDShow.h"
#include "HelperProfile.h"
#include "HelperKit.h"

#include "WMXDSAudio.h"
#include "WMXDSVideo.h"

BOOL RenderToNull(IGraphBuilder *pGB, IPin *pOutPin)
{
	CComPtr<IBaseFilter> pNullRenderer;
	pNullRenderer.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC);
	if(FAILED(pGB->AddFilter(pNullRenderer, L"NullRenderer")))
		return FALSE;

	CComPtr<IPin> pInPin;
	pInPin = GetInPin(pNullRenderer, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;

	return TRUE;
}

BOOL WMXConvertMix(BSTR bstrWMX, LPVOID pAudioManager, LPVOID pVideoManager, CWriterBase *pWriterBase)
{
	HRESULT hr=0;
	CComPtr<IGraphBuilder>		pGB;
	CComPtr<IBaseFilter>		pAsfReader;
	CComPtr<IFileSourceFilter>	pFileSource;
	
	CComPtr<IMediaControl>		pMC;
	CComPtr<IMediaEventEx>		pEventEx;
	
	if (FAILED(pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return FALSE;
	
#ifdef _DEBUG
	DWORD dwRegGraph=0;
	AddToRot(pGB, &dwRegGraph);
#endif

	if(FAILED(pAsfReader.CoCreateInstance(CLSID_WMAsfReader, NULL, CLSCTX_INPROC)))
        return FALSE;
	
	if (FAILED(pGB->AddFilter(pAsfReader, L"AsfReader")))
		return FALSE;
	
	if(FAILED(pAsfReader.QueryInterface(&pFileSource)))
		return FALSE;

	if (FAILED(pFileSource->Load(bstrWMX, NULL)))
		return FALSE;

	// bug!!! must use 44100!!
	CWMXDSAudio *ptmpAudio = new CWMXDSAudio("DSAudio", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSAudio(ptmpAudio);
	
	if (FAILED(pGB->AddFilter(pWMXDSAudio, L"WMXDSAudio")))
		return FALSE;	

	CWMXDSVideo *ptmpVideo = new CWMXDSVideo("DSVideo", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSVideo(ptmpVideo);
	
	if (FAILED(pGB->AddFilter(pWMXDSVideo, L"WMXDSVideo")))
		return FALSE;	

	if(FAILED(pGB->RenderFile(bstrWMX, NULL)))
		return FALSE;
	
	CComPtr<IPin> pOutPin, pInPin;

	goto NEXT_RUN;

	pOutPin = GetOutPin(pAsfReader, 0);
	if(pOutPin==NULL)
		return FALSE;
			
	pInPin = GetInPin(pWMXDSAudio, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSAudio, 0);
	if(pOutPin==NULL)
		return FALSE;

	if(!RenderToNull(pGB, pOutPin))
		return FALSE;

	pOutPin.Release();
		
	pOutPin = GetOutPin(pAsfReader, 1);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pWMXDSVideo, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSVideo, 0);
	if(pOutPin==NULL)
		return FALSE;

	if(!RenderToNull(pGB, pOutPin))
		return FALSE;
	
	pOutPin.Release();
			
	pOutPin = GetOutPin(pAsfReader, 2);
	if(!RenderToNull(pGB, pOutPin))
		return FALSE;
	pOutPin.Release();
		
NEXT_RUN:
	SetNoClock(pGB);
	
	if(FAILED(pGB->QueryInterface(&pMC)))
		return FALSE;
	
	if(FAILED(pGB->QueryInterface(&pEventEx)))
		return FALSE;
	
	if(FAILED(hr=pMC->Run()))
		return FALSE;
	
	long lEvCode=0;
	while(TRUE) {
		hr = pEventEx->WaitForCompletion(500, &lEvCode);
		if(SUCCEEDED(hr)) 
			break;
		else if(hr==E_ABORT)
			continue; //return FALSE;
		else Sleep(1000);
	}
	pMC->Stop();
	
	pGB->RemoveFilter(pAsfReader);
#ifdef _DEBUG
	if(dwRegGraph!=0) {
		RemoveFromRot(dwRegGraph);
		dwRegGraph = 0;
	}
#endif
		
	return TRUE;
} 

BOOL WMXConvertByDS(BSTR bstrWMX, LPVOID pAudioManager, LPVOID pVideoManager, BSTR bstrProfile, BSTR bstrOutput)
{
	HRESULT hr=0;
	CComPtr<IGraphBuilder>		pGB;
	CComPtr<IBaseFilter>		pAsfReader;
	CComPtr<IBaseFilter>		pAsfWriter;
	CComPtr<IFileSourceFilter>	pFileSource;
	CComPtr<IFileSinkFilter2>	pFileSink;
	
	CComPtr<IMediaControl>		pMC;
	CComPtr<IMediaEventEx>		pEventEx;
	
	if (FAILED(pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return FALSE;
	
#ifdef _DEBUG
	DWORD dwRegGraph=0;
	AddToRot(pGB, &dwRegGraph);
#endif
	
	if(FAILED(pAsfWriter.CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC)))
        return FALSE;
	
	if(FAILED(pGB->AddFilter(pAsfWriter, L"AsfWriter")))
		return FALSE;

	if(FAILED(pAsfWriter.QueryInterface(&pFileSink)))
		return FALSE;

	pFileSink->SetMode(AM_FILE_OVERWRITE);
	
	if(FAILED(pFileSink->SetFileName(bstrOutput, NULL)))
		return FALSE;
	
	IWMProfileManager *pPM0=NULL;
    if(FAILED(WMCreateProfileManager(&pPM0)))
		return FALSE;

	CComPtr<IWMProfileManager2> pWMPM;
	hr = pPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	pPM0->Release();
	if(FAILED(hr)) return FALSE;
	
	CComPtr<IWMProfile> pTmpProfile = PF_LoadFromPRX(pWMPM, bstrProfile);
	if(pTmpProfile==NULL)
		return FALSE;
	
	CComPtr<IWMProfile> pProfile = PF_Subtract(pWMPM, pTmpProfile, TRUE, FALSE);
	if(pProfile==NULL)
		return FALSE;
	
	CComPtr<IConfigAsfWriter> pWriterConfig;
	if(FAILED(pAsfWriter.QueryInterface(&pWriterConfig)))
		return FALSE;

	if(FAILED(pWriterConfig->ConfigureFilterUsingProfile(pProfile)))
		return FALSE;
	
	if(FAILED(pWriterConfig->SetIndexMode(TRUE)))
		return FALSE;

	
	if(FAILED(pAsfReader.CoCreateInstance(CLSID_WMAsfReader, NULL, CLSCTX_INPROC)))
        return FALSE;
	
	if (FAILED(pGB->AddFilter(pAsfReader, L"AsfReader")))
		return FALSE;
	
	if(FAILED(pAsfReader.QueryInterface(&pFileSource)))
		return FALSE;

	if (FAILED(pFileSource->Load(bstrWMX, NULL)))
		return FALSE;

	CComPtr<IPin> pOutPin, pInPin;

	CWMXDSAudio *ptmpAudio = new CWMXDSAudio("DSAudio", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSAudio(ptmpAudio);
	
	if (FAILED(pGB->AddFilter(pWMXDSAudio, L"WMXDSAudio")))
		return FALSE;	

	CWMXDSVideo *ptmpVideo = new CWMXDSVideo("DSVideo", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSVideo(ptmpVideo);
	
	if (FAILED(pGB->AddFilter(pWMXDSVideo, L"WMXDSVideo")))
		return FALSE;	
	
	pOutPin = GetOutPin(pAsfReader, 0);
	if(pOutPin==NULL)
		return FALSE;
			
	pInPin = GetInPin(pWMXDSAudio, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSAudio, 0);
	if(pOutPin==NULL)
		return FALSE;

	pInPin = GetInPin(pAsfWriter, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
		
	pOutPin = GetOutPin(pAsfReader, 1);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pWMXDSVideo, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSVideo, 0);
	if(pOutPin==NULL)
		return FALSE;
	
	pInPin = GetInPin(pAsfWriter, 1);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
			
	pOutPin = GetOutPin(pAsfReader, 2);
	if(pOutPin!=NULL) {
	// can not join script output pin directly to writer, otherwise no file output even can run!!
		pInPin = GetInPin(pAsfWriter, 2);
		if((pInPin==NULL)||FAILED(pGB->Connect(pOutPin, pInPin))) {
			pInPin.Release();
			hr = pGB->Render(pOutPin);
			if(FAILED(hr)) return FALSE;
		}
		
		pOutPin.Release();
		pInPin.Release();
	}
	
	SetNoClock(pGB);
	
	if(FAILED(pGB->QueryInterface(&pMC)))
		return FALSE;
	
	if(FAILED(pGB->QueryInterface(&pEventEx)))
		return FALSE;
	
	if(FAILED(hr=pMC->Run()))
		return FALSE;
	
	long lEvCode=0;
	while(TRUE) {
		hr = pEventEx->WaitForCompletion(500, &lEvCode);
		if(SUCCEEDED(hr)) 
			break;
		else if(hr==E_ABORT) // only, time out
			continue;
		else if(hr==VFW_E_WRONG_STATE)
			return FALSE;
		else Sleep(1000);
	}
	pMC->Stop();
	
	pGB->RemoveFilter(pAsfReader);
	pGB->RemoveFilter(pAsfWriter);
	pAsfWriter.Release();	
#ifdef _DEBUG
	if(dwRegGraph!=0) {
		RemoveFromRot(dwRegGraph);
		dwRegGraph = 0;
	}
#endif
		
	return TRUE;
} 

BOOL WMXProcess(BSTR bstrWMX, BSTR bstrProfile, LPVOID pManager, DWORD dwmsTime, DWORD dwmsStart, DWORD dwmsStop, 
				CStringArray &ReplaceCstArray, CDWordArray &ReplaceTimeArray, CDWordArray &ReplaceStartArray, CDWordArray &ReplaceStopArray,
				CStringArray &MixCstArray, CDWordArray &MixTimeArray, CDWordArray &MixStartArray, CDWordArray &MixStopArray,
				BSTR bstrOutput)
{
	HRESULT hr=0;
	CComPtr<IGraphBuilder>		pGB;
	CComPtr<IBaseFilter>		pAsfReader;
	CComPtr<IBaseFilter>		pAsfWriter;
	CComPtr<IFileSourceFilter>	pFileSource;
	CComPtr<IFileSinkFilter2>	pFileSink;
	
	CComPtr<IMediaSeeking>		pSeek;	
	CComPtr<IMediaControl>		pMC;
	CComPtr<IMediaEventEx>		pEventEx;
	
	if (FAILED(pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return FALSE;
	
#ifdef _DEBUG
	DWORD dwRegGraph=0;
	AddToRot(pGB, &dwRegGraph);
#endif
	
	if(FAILED(pAsfWriter.CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC)))
        return FALSE;
	
	if(FAILED(pGB->AddFilter(pAsfWriter, L"AsfWriter")))
		return FALSE;

	if(FAILED(pAsfWriter.QueryInterface(&pFileSink)))
		return FALSE;

	pFileSink->SetMode(AM_FILE_OVERWRITE);
	
	if(FAILED(pFileSink->SetFileName(bstrOutput, NULL)))
		return FALSE;
	
	IWMProfileManager *pPM0=NULL;
    if(FAILED(WMCreateProfileManager(&pPM0)))
		return FALSE;

	CComPtr<IWMProfileManager2> pWMPM;
	hr = pPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	pPM0->Release();
	if(FAILED(hr)) return FALSE;
	
	CComPtr<IWMProfile> pTmpProfile = PF_LoadFromPRX(pWMPM, bstrProfile);
	if(pTmpProfile==NULL)
		return FALSE;

	CComPtr<IWMProfile> pProfile = PF_Subtract(pWMPM, pTmpProfile, TRUE, FALSE);
	if(pProfile==NULL)
		return FALSE;

	CComPtr<IConfigAsfWriter> pWriterConfig;
	if(FAILED(pAsfWriter.QueryInterface(&pWriterConfig)))
		return FALSE;

	if(FAILED(pWriterConfig->ConfigureFilterUsingProfile(pProfile)))
		return FALSE;
	
	if(FAILED(pWriterConfig->SetIndexMode(TRUE)))
		return FALSE;
	
	if(FAILED(pAsfReader.CoCreateInstance(CLSID_WMAsfReader, NULL, CLSCTX_INPROC)))
        return FALSE;
	
	if (FAILED(pGB->AddFilter(pAsfReader, L"AsfReader")))
		return FALSE;
	
	if(FAILED(pAsfReader.QueryInterface(&pFileSource)))
		return FALSE;

	if (FAILED(pFileSource->Load(bstrWMX, NULL)))
		return FALSE;
	
	CWMXDSAudio *ptmpAudio = new CWMXDSAudio("DSAudio", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSAudio(ptmpAudio);
	
	if (FAILED(pGB->AddFilter(pWMXDSAudio, L"WMXDSAudio")))
		return FALSE;	

	CWMXDSVideo *ptmpVideo = new CWMXDSVideo("DSVideo", NULL, &hr);
	CComPtr<IBaseFilter> pWMXDSVideo(ptmpVideo);
	
	if (FAILED(pGB->AddFilter(pWMXDSVideo, L"WMXDSVideo")))
		return FALSE;	
	
	CComPtr<IPin> pOutPin, pInPin;
	pOutPin = GetOutPin(pAsfReader, 0);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pWMXDSAudio, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSAudio, 0);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pAsfWriter, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
		
	pOutPin = GetOutPin(pAsfReader, 1);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pWMXDSVideo, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();

	pOutPin = GetOutPin(pWMXDSVideo, 0);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pAsfWriter, 1);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
		
	CComPtr<IBaseFilter> pNullRenderer;
	pNullRenderer.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC);
	if(FAILED(pGB->AddFilter(pNullRenderer, L"NullRenderer")))
		return FALSE;
	
	pOutPin = GetOutPin(pAsfReader, 2);
	if(pOutPin==NULL)
		return FALSE;
	pInPin = GetInPin(pNullRenderer, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
	
/*
		pOutPin = GetOutPin(pAsfReader, 0);
		if(pOutPin==NULL)
			return FALSE;
			
		if(FAILED(pOutPin->QueryInterface(&pSeek)))
			return FALSE;
	
		pOutPin.Release();*/
	
	
	if(FAILED(pGB->QueryInterface(&pSeek)))
		return FALSE;
	
	if(FAILED(pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME)))
		return FALSE;
	
	LONGLONG llstart=(LONGLONG)dwmsStart * 10000;
	LONGLONG llstop=(LONGLONG)dwmsStop * 10000;
	LONGLONG llDuration=0;
	if(FAILED(pSeek->GetDuration(&llDuration)))
		return FALSE;
	if(llDuration==0)
		return FALSE;

	if(llstop>llDuration) llstop = llDuration;
	SetNoClock(pGB);
	hr = pSeek->SetPositions(&llstart, AM_SEEKING_AbsolutePositioning, &llstop, AM_SEEKING_AbsolutePositioning);
	if(FAILED(hr)) {
		ASSERT(0);
		return FALSE;
	}
	
	if(FAILED(pGB->QueryInterface(&pMC)))
		return FALSE;
	
	if(FAILED(pGB->QueryInterface(&pEventEx)))
		return FALSE;
	
	if(FAILED(pMC->Run()))
		return FALSE;
	
	long lEvCode=0;
	while(TRUE) {
		hr = pEventEx->WaitForCompletion(250, &lEvCode);
		if(SUCCEEDED(hr))
			break;
		else if(hr==E_ABORT) 
			return FALSE;
	}
	pMC->Stop();
	
	pGB->RemoveFilter(pAsfReader);
	pGB->RemoveFilter(pAsfWriter);
	pAsfWriter.Release();	
#ifdef _DEBUG
	if(dwRegGraph!=0) {
		RemoveFromRot(dwRegGraph);
		dwRegGraph = 0;
	}
#endif
		
	return TRUE;
} 