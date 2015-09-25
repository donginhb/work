#include <streams.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <initguid.h>
#include "WavWriter.h"
#include "resource.h"

// Setup data
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
//
// CreateInstance
//
// This goes in the factory template table to create new instances
//
CUnknown * WINAPI CWavWriterFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CWavWriterFilter(pUnk, phr);

} // CreateInstance

STDMETHODIMP CWavWriterFilter::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    if (riid == IID_IWavWriterConfig) 
        return GetInterface((IWavWriterConfig *)this, ppv);
    return CBaseRenderer::NonDelegatingQueryInterface(riid,ppv);

} // NonDelegatingQueryInterface

//
// Constructor
//
// Create the filter, WavWriter window, and input pin
//
#pragma warning(disable:4355)
//
CWavWriterFilter::CWavWriterFilter(LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseRenderer(CLSID_WavWriter, NAME("WavWriter"), pUnk, phr)
{
	m_pWavWriter = NULL;
	memset(&m_wfx, 0, sizeof(m_wfx));
	m_lCurrSize = m_lSizeBound = 0;
	m_llStartTime = 0;
} // (Constructor)


//
// Destructor
//
CWavWriterFilter::~CWavWriterFilter()
{
} // (Destructor)

//
// CheckMediaType
//
// Check that we can support a given proposed type
//
HRESULT CWavWriterFilter::CheckMediaType(const CMediaType *pmt)
{
    WAVEFORMATEX *pwfx = (WAVEFORMATEX *) pmt->Format();

    if (pwfx == NULL)
        return E_INVALIDARG;

    // Reject non-PCM Audio type

    if (pmt->majortype != MEDIATYPE_Audio) {
        return E_INVALIDARG;
    }

    if (pmt->formattype != FORMAT_WaveFormatEx) {
        return E_INVALIDARG;
    }

    if (pwfx->wFormatTag != WAVE_FORMAT_PCM) {
        return E_INVALIDARG;
    }

	if (pwfx->wBitsPerSample != m_wfx.wBitsPerSample) {
		return E_INVALIDARG;
	}

	if (pwfx->nChannels != m_wfx.nChannels) {
		return E_INVALIDARG;
	}

	if (pwfx->nSamplesPerSec != m_wfx.nSamplesPerSec) {
		return E_INVALIDARG;
	}

	if (pwfx->nBlockAlign != m_wfx.nBlockAlign) {
		return E_INVALIDARG;
	}

	if (pwfx->nAvgBytesPerSec != m_wfx.nAvgBytesPerSec) {
		return E_INVALIDARG;
	}

    return NOERROR;

} // CheckMediaType

HRESULT CWavWriterFilter::SetMediaType(const CMediaType *pmt)
{
	CAutoLock lock(this);

	if(pmt==NULL) return E_FAIL;
	if(!pmt->IsValid()) return E_FAIL;

	if(MEDIATYPE_Audio!=*(pmt->Type())) return E_FAIL;
	if(MEDIASUBTYPE_PCM!=*(pmt->Subtype())) return E_FAIL;
	if(FORMAT_WaveFormatEx!=*(pmt->FormatType())) return E_FAIL;
	if(sizeof(m_wfx)>pmt->FormatLength()) return E_FAIL;

	memcpy(&m_wfx, pmt->Format(), sizeof(m_wfx));
	
	return CBaseRenderer::SetMediaType(pmt);
}

HRESULT CWavWriterFilter::DoRenderSample(IMediaSample *pMediaSample)
{
    CAutoLock lock(this);

	if(m_pWavWriter==NULL)
		return  E_FAIL;
	
	LPBYTE pBuffer=NULL;
	HRESULT hr = pMediaSample->GetPointer(&pBuffer);
	if(FAILED(hr)) return E_FAIL;

	long lSize = pMediaSample->GetActualDataLength();
	if(m_lSizeBound>0) {
		if(m_lCurrSize>=m_lSizeBound)
			lSize = 0;
		else if((m_lCurrSize+lSize)>=m_lSizeBound)
			lSize = m_lSizeBound - m_lCurrSize;
	}
	if(lSize>0) {		
		LONGLONG llStart=0, llStop=0;
		pMediaSample->GetTime(&llStart, &llStop);
		LONG lAlignSize = (LONG)((llStart-m_llStartTime) * m_pWavWriter->pwfx->nSamplesPerSec * m_pWavWriter->pwfx->nChannels * (m_pWavWriter->pwfx->wBitsPerSample/8) / 1000 / 10000);
		lAlignSize = lAlignSize / m_pWavWriter->pwfx->nBlockAlign * m_pWavWriter->pwfx->nBlockAlign;
		if(lAlignSize>m_lCurrSize) {
			static BYTE szBuffer[1024];
			memset(szBuffer, 0, sizeof(szBuffer));
			
			LONG lFillSize=m_lCurrSize;
			while(lFillSize<lAlignSize) {
				LONG lRealSize= lAlignSize-lFillSize;
				if(lRealSize>1024) lRealSize = 1024;
				if(0>WavWriterWriteData(m_pWavWriter, szBuffer, lRealSize))
					return E_FAIL;
				lFillSize += lRealSize;
			}
			m_lCurrSize = lAlignSize;
		}

		UINT cbRetSize=0;
		if(0>WavWriterWriteData(m_pWavWriter, pBuffer, (DWORD)lSize))
			return E_FAIL;
		m_lCurrSize += lSize;
	}

	return S_OK;
}

STDMETHODIMP CWavWriterFilter::SetupSizeBound(long lBound)
{
	m_lSizeBound = lBound;

	return S_OK;
}

STDMETHODIMP CWavWriterFilter::SetupAudioFormat(long lBitsPerSample, long lChannels, long lSampleRate)
{
	if(m_State!=State_Stopped)
		return E_FAIL;
	if((m_pInputPin!=NULL)&&m_pInputPin->IsConnected())
		return E_FAIL;

	memset(&m_wfx, 0, sizeof(m_wfx));

	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.wBitsPerSample = (WORD)lBitsPerSample;
	m_wfx.nChannels = (UINT)lChannels;
	m_wfx.nSamplesPerSec = lSampleRate;
	m_wfx.nBlockAlign = (m_wfx.nChannels * m_wfx.wBitsPerSample)/8;
	m_wfx.nAvgBytesPerSec = (m_wfx.nSamplesPerSec * m_wfx.nBlockAlign);

	return TRUE;
}

#include <atlbase.h>
#include <dshowasf.h>
#include <dmodshow.h>
#include <qedit.h>
#include "HelperDShow.h"

// this function only is suitable for MCG generated wmv/wma!!
BOOL WMX2Wav(BSTR bstrWMX, long lmsStart, long lmsStop, LPWAVWRITER pWavWriter)
{
	HRESULT hr=0;
	CWavWriterFilter *pWriteFilter = new CWavWriterFilter(NULL, &hr);
	if(FAILED(hr)||(pWriteFilter==NULL))
		return FALSE;
	pWriteFilter->AddRef(); // to act as COM!!
	pWriteFilter->SetupWavWriter(pWavWriter);
	pWriteFilter->SetupAudioFormat((WORD)pWavWriter->pwfx->wBitsPerSample, 
		(WORD)pWavWriter->pwfx->nChannels,
		(DWORD)pWavWriter->pwfx->nSamplesPerSec);
	long lSizeBound = (long)((double)(lmsStop-lmsStart) * pWavWriter->pwfx->nAvgBytesPerSec /1000.0);
	lSizeBound = lSizeBound / pWavWriter->pwfx->nBlockAlign * pWavWriter->pwfx->nBlockAlign;
	//pWriteFilter->SetupSizeBound(lSizeBound);
	pWriteFilter->SetupSizeBound(0); //DIRTY Fix audio short problem.

	CComPtr<IGraphBuilder>		pGB;
	CComPtr<IBaseFilter>		pAsfReader;
	CComPtr<IFileSourceFilter>	pFileSource;

	if (FAILED(pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return FALSE;

#ifdef _DEBUG
	DWORD dwRegGraph=0;
	AddToRot(pGB, &dwRegGraph);
#endif

	if(FAILED(pGB->AddFilter(pWriteFilter, L"WavWrite")))
		return FALSE;

	if(FAILED(pAsfReader.CoCreateInstance(CLSID_WMAsfReader, NULL, CLSCTX_INPROC)))
        return FALSE;

	if (FAILED(pGB->AddFilter(pAsfReader, L"AsfReader")))
		return FALSE;

	pAsfReader.QueryInterface(&pFileSource);
	if (FAILED(pFileSource->Load(bstrWMX, NULL)))
		return FALSE;

	CComPtr<IPin> pOutPin, pInPin;
	pOutPin = GetOutPin(pAsfReader, 0);
	if(pOutPin==NULL)
		return FALSE;

	pInPin = GetInPin(pWriteFilter, 0);
	if(pInPin==NULL)
		return FALSE;
	if(FAILED(pGB->Connect(pOutPin, pInPin)))
		return FALSE;
	pOutPin.Release();
	pInPin.Release();
	
/* To fix a bug when render video part and can not data transfer in graph, so mark this part.
	pOutPin = GetOutPin(pAsfReader, 1);
	if(pOutPin!=NULL) {
		CComPtr<IBaseFilter> pNullRenderer;
		pNullRenderer.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC);
		if(FAILED(pGB->AddFilter(pNullRenderer, L"NullRenderer")))
			return FALSE;
		pInPin = GetInPin(pNullRenderer, 0);
		if(FAILED(pGB->ConnectDirect(pOutPin, pInPin, NULL)))
			return FALSE;
		pOutPin.Release();
		pInPin.Release();
	}

	pOutPin = GetOutPin(pAsfReader, 2);
	if(pOutPin!=NULL) {
		CComPtr<IBaseFilter> pNullRenderer;
		pNullRenderer.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC);
		if(FAILED(pGB->AddFilter(pNullRenderer, L"NullRenderer")))
			return FALSE;
		pInPin = GetInPin(pNullRenderer, 0);
		if(FAILED(pGB->ConnectDirect(pOutPin, pInPin, NULL)))
			return FALSE;
		pOutPin.Release();
		pInPin.Release();
	}
*/	
	CComPtr<IMediaSeeking>		pSeek;	
	pGB->QueryInterface(&pSeek);

	LONGLONG llstart=(LONGLONG)lmsStart * 10000;
	LONGLONG llstop=(LONGLONG)lmsStop * 10000;
	LONGLONG llDuration=0;
	pSeek->GetDuration(&llDuration);
	if(llDuration>0) {
		if(llstop>llDuration) llstop = llDuration;
	}
	pWriteFilter->SetupStartTime(llstart);

	SetNoClock(pGB);
	hr = pSeek->SetPositions(&llstart, AM_SEEKING_AbsolutePositioning, &llstop, AM_SEEKING_AbsolutePositioning);
// DIRTY fix audio short
// It is trouble when render two pins (audio & video), and only can set start position. If only render audio pin, seems ok.
//	hr = pSeek->SetPositions(&llstart, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	if(FAILED(hr)) {
		ASSERT(0);
		return FALSE;
	}
	
	CComPtr<IMediaControl>		pMC;
	pGB->QueryInterface(&pMC);
	hr = pMC->Run();

	CComPtr<IMediaEventEx>		pEventEx;
	pGB->QueryInterface(&pEventEx);
	while(TRUE) {
		long lEvCode=0, lParam1=0, lParam2=0;
		hr = pEventEx->GetEvent(&lEvCode, &lParam1, &lParam2, 500);
		if(hr==S_OK) {
			pEventEx->FreeEventParams(lEvCode, lParam1, lParam2);
			if((lEvCode==EC_COMPLETE)||(lEvCode==EC_END_OF_SEGMENT))
				break;
			if(lEvCode==EC_ERRORABORT)
				return E_FAIL;
		}
		else {
			LONGLONG llCurr=0;
			hr = pSeek->GetCurrentPosition(&llCurr);
			if(llCurr>=(llstop-llstart))
				break;
		}
	}
	pMC->Stop();

	pGB->RemoveFilter(pWriteFilter);
	RELEASE(pWriteFilter);
		
#ifdef _DEBUG
	if(dwRegGraph!=0) {
		RemoveFromRot(dwRegGraph);
		dwRegGraph = 0;
	}
#endif
	return TRUE;
}