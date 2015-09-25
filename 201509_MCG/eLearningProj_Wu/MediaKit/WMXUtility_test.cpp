// WMXUtility.cpp : Implementation of CWMXUtility
#include "stdafx.h"
#include "MediaKit.h"
#include "WMXUtility.h"

/////////////////////////////////////////////////////////////////////////////
// CWMXUtility
#include <wmsdk.h>
#include "ReaderBase.h"
#include "WriterBase.h"
#include "WMXNormalize.h"
#include "WMXDumper.h"
#include "WMXStreamReplace.h"
#include "WMXConvert.h"
#include "WMXCopy.h"
#include "InfoReader.h"
#include "HelperProfile.h"
#include "HelperKit.h"
#include "WavWriterGuids.h"

#include <wmsdkidl.h>
#include <initguid.h>

#include "..\VFXManager\VFXMan.h"
#include "..\VFXManager\VFXMan_i.c"

#ifndef OATRUE
#define OATRUE (-1)
#endif // OATRUE
#ifndef OAFALSE
#define OAFALSE (0)
#endif // OAFALSE

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

STDMETHODIMP CWMXUtility::SetupProgressHandler(long lWnd, long lMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(IsWindow((HWND)lWnd)) m_hWndProgress = (HWND)lWnd;
	else m_hWndProgress = NULL;

	m_lMsgProgress = lMsg;

	return S_OK;
}

STDMETHODIMP CWMXUtility::SetupQueryAbort(long lWndAbortActive, long *lpbAbort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_hWndAbort = (HWND)lWndAbortActive;
	m_pbAbort = lpbAbort;

	return S_OK;
}

STDMETHODIMP CWMXUtility::GetWMXAudioPeak(BSTR bstrWMX, long *plPeakMin, long *plPeakMax, long *plMin, long *plMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXNormalize	WMXNormalize;
	if(IsWindow(m_hWndProgress)) WMXNormalize.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXNormalize.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	
	BOOL fOK = WMXNormalize.AudioGetPeak(bstrWMX, plPeakMin, plPeakMax, plMin, plMax);

	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXAudioMultiply(BSTR bstrWMX, long lRatio_100, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXNormalize	WMXNormalize;
	if(IsWindow(m_hWndProgress)) WMXNormalize.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXNormalize.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	
	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);
	BOOL fOK = WMXNormalize.AudioMultiply(bstrWMX, bstrOutput, bstrProfile, lRatio_100, fCopyHeader, fCopyScript, fCopyMarker);

	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXAudioNormalize(BSTR bstrWMX, long lPercentage_100, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXNormalize	WMXNormalize;
	if(IsWindow(m_hWndProgress)) WMXNormalize.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXNormalize.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	
	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);
	BOOL fOK = WMXNormalize.AudioNormalize(bstrWMX, bstrOutput, bstrProfile, lPercentage_100, fCopyHeader, fCopyScript, fCopyMarker);

	if(!fOK) return E_FAIL;
	return S_OK;
}

BOOL MakeListArray(BSTR bstrList, CDWordArray &StartTimes, CDWordArray &StopTimes, CStringArray &Files)
{
	StartTimes.RemoveAll();
	StopTimes.RemoveAll();
	Files.RemoveAll();

	CString cst(bstrList);
	if(cst.Right(1)!=";") cst += ";";

	CString cstFile;
	long lmsStart=0, lmsStop=0;
	
	int nPtr=0, nTmpPtr=0;
	do {
		nTmpPtr = cst.Find(';', nPtr);
		if(nTmpPtr<nPtr+1) break;
		lmsStart = atol(cst.Mid(nPtr, nTmpPtr-nPtr));
		nPtr = nTmpPtr + 1;
	
		nTmpPtr = cst.Find(';', nPtr);
		if(nTmpPtr<nPtr+1) break;
		lmsStop = atol(cst.Mid(nPtr, nTmpPtr-nPtr));
		nPtr = nTmpPtr + 1;

		nTmpPtr = cst.Find(';', nPtr);
		if(nTmpPtr<nPtr+1) break;
		cstFile = cst.Mid(nPtr, nTmpPtr-nPtr);
		nPtr = nTmpPtr + 1;

		if(lmsStop<=lmsStart)
			return FALSE;

		StartTimes.Add(lmsStart);
		StopTimes.Add(lmsStop);
		Files.Add(cstFile);
	} while(TRUE);

	int ii=0;
	for(ii=0; ii<StartTimes.GetSize()-1; ii++) {
		if(StartTimes.GetAt(ii+1)<StopTimes.GetAt(ii))
			return FALSE;
	}

	return TRUE;
}

STDMETHODIMP CWMXUtility::WMXAudioReplace(BSTR bstrFile, BSTR bstrReplace, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXStreamReplace WMXStreamReplace;

	if(IsWindow(m_hWndProgress)) WMXStreamReplace.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXStreamReplace.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);
	if(!WMXStreamReplace.MakeReplace(bstrFile, bstrReplace, TRUE, FALSE, FALSE, fCopyHeader, fCopyScript, fCopyMarker, bstrOutput))
		return E_FAIL;

	return S_OK;
}

#include "WMX2Media.h"
STDMETHODIMP CWMXUtility::WavFromWMX(BSTR bstrWMX, long lBitsPerSample, long lChannels, long lSampleRate, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMX2Media WMX2Media;
	if(IsWindow(m_hWndProgress)) WMX2Media.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMX2Media.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	if(!WMX2Media.MakeWave(bstrWMX, (WORD)lBitsPerSample, (WORD)lChannels, (DWORD)lSampleRate, bstrOutput))
		return E_FAIL;

	return S_OK;
}

#include "WMXFromMedia.h"
STDMETHODIMP CWMXUtility::WMXAudioFromWav(BSTR bstrWMX, BSTR bstrWav, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXFromMedia WMXFromMedia;
	if(IsWindow(m_hWndProgress)) WMXFromMedia.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXFromMedia.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);
	if(!WMXFromMedia.WMXFromWav(bstrWMX, bstrProfile, bstrWav, fCopyHeader, fCopyScript, fCopyMarker, bstrOutput))
		return E_FAIL;

	return S_OK;
}

BOOL ParseTable(BSTR bstrTable, CStringArray &cstArray)
{
	cstArray.RemoveAll();

	CString cst(bstrTable);
	if(cst.Right(1)!=";") cst += ";";

	CString cstName;
	BSTR bstrName=NULL;
	int nPtr=0, nTmpPtr=0;
	do {
		nTmpPtr = cst.Find(';', nPtr);
		if(nTmpPtr<nPtr+1) break;
		cstName = cst.Mid(nPtr, nTmpPtr-nPtr);
		bstrName = cstName.AllocSysString();
		nPtr = nTmpPtr + 1;
	
		cstArray.Add(cstName);
	} while(TRUE);

	return TRUE;
}

BOOL ParseList(BSTR bstrLists, CDWordArray &TimeArray, CStringArray &cstArray, CDWordArray &StartArray, CDWordArray &StopArray)
{
	TimeArray.RemoveAll();
	cstArray.RemoveAll();
	StartArray.RemoveAll();
	StopArray.RemoveAll();

	CString cst(bstrLists);
	if(cst.Right(1)!=";") cst += ";";

	CString cstFile;
	BSTR bstrFile=NULL;
	long lmsTime=0, lmsStart=0, lmsStop=0;
	int nPtr=0, nTmpPtr=0;
	do {
		nTmpPtr = cst.Find(',', nPtr);
		if(nTmpPtr<nPtr+1) break;
		lmsTime = atol(cst.Mid(nPtr, nTmpPtr-nPtr));
		nPtr = nTmpPtr + 1;

		nTmpPtr = cst.Find(',', nPtr);
		if(nTmpPtr<nPtr+1) break;
		cstFile = cst.Mid(nPtr, nTmpPtr-nPtr);
		bstrFile = cstFile.AllocSysString();
		nPtr = nTmpPtr + 1;
	
		nTmpPtr = cst.Find(',', nPtr);
		if(nTmpPtr<nPtr+1) break;
		lmsStart = atol(cst.Mid(nPtr, nTmpPtr-nPtr));
		nPtr = nTmpPtr + 1;

		nTmpPtr = cst.Find(';', nPtr);
		if(nTmpPtr<nPtr+1) break;
		lmsStop = atol(cst.Mid(nPtr, nTmpPtr-nPtr));
		nPtr = nTmpPtr + 1;

		TimeArray.Add(lmsTime);
		cstArray.Add(cstFile);
		StartArray.Add(lmsStart);
		StopArray.Add(lmsStop);
	} while(TRUE);

	return TRUE;
}

#include "WaveLib\HelperWav.h"
#include "HelperWavSpec.h"
#include "MultiWMXAudio2Wav.h"
#include "MultiWMXWav2Audio.h"
#include "MultiWMXCopy.h"
#include "MultiWMXCopyWav.h"
#include "InfoReader.h"
/*
STDMETHODIMP CWMXUtility::MultiWMXAudio2Wav(BSTR bstrAudioList, IUnknown *pManager, BSTR bstrMixerTimers, long lBitsPerSample, long lChannels, long lSampleRate, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrAudioList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	CMultiWMXAudio2Wav	MultiWMXAudio2Wav;

	if(IsWindow(m_hWndProgress)) MultiWMXAudio2Wav.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) MultiWMXAudio2Wav.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	int		nIndex = TimeArray.GetSize()-1;
	DWORD	dwmsLen = TimeArray.GetAt(nIndex) + StopArray.GetAt(nIndex) - StartArray.GetAt(nIndex);
	MultiWMXAudio2Wav.SetupTotalDuration((QWORD)TIMEUNIT / 1000 * dwmsLen);

	CString cstOutput(bstrOutput);
	CString cstTemp(cstOutput + ".tmp");
	DeleteFile(cstOutput);
	DeleteFile(cstTemp);
	
	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, (WORD)lBitsPerSample, (WORD)lChannels, (DWORD)lSampleRate, (LPSTR)(LPCSTR)cstTemp))
		return E_FAIL;

	WavWriterSetEasyManager(&WavWriter, pManager);

	BOOL fError=TRUE;
	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavWriterClose(&WavWriter);
		return E_FAIL;
	}

	fError = FALSE;
	MultiWMXAudio2Wav.SetWriter(&WavWriter);

	UINT umsTime=0;
	for(long ii=0; ii<CstArray.GetSize(); ii++) {
		if(umsTime<TimeArray.GetAt(ii)) {
			if(!WavWriterAppendMute(&WavWriter, TimeArray.GetAt(ii)-umsTime)) {
				fError = TRUE;
				break;
			}
			umsTime += TimeArray.GetAt(ii)-umsTime;
		}

		DWORD dwStart = StartArray.GetAt(ii);
		DWORD dwStop = StopArray.GetAt(ii);

		CString cst = CstArray.GetAt(ii).Right(4);
		cst.MakeUpper();
		if(cst==".WAV") {
			UINT uStart = (UINT)(dwStart / 1000.0 * WavWriter.pwfx->nAvgBytesPerSec);
			UINT uStop = (UINT)(dwStop / 1000.0 * WavWriter.pwfx->nAvgBytesPerSec);
			uStart = uStart / WavWriter.pwfx->nBlockAlign * WavWriter.pwfx->nBlockAlign;
			uStop = uStop / WavWriter.pwfx->nBlockAlign * WavWriter.pwfx->nBlockAlign;

			if(!WavWriterAppendFile(&WavWriter, uStart, uStop, (LPSTR)(LPCSTR)CstArray.GetAt(ii))) {
				fError = TRUE;
				break;
			}
		}
		else {
			CComBSTR cbstrFile(CstArray.GetAt(ii));
			if(dwStop>dwStart) {
				if(!MultiWMXAudio2Wav.AddWMXAudio(cbstrFile, dwStart, dwStop)) {
					fError = TRUE;
					break;
				}
			}
		}

		umsTime = TimeArray.GetAt(ii) + dwStop - dwStart;
	}

	WavWriterClose(&WavWriter);

	if(fError) return E_FAIL;

	if(wcslen(bstrMixerTimers)>0) {
		CComBSTR cbstr(cstTemp), cbstrOut(cstOutput);
		if(FAILED(MultiWMXMixer2Wav(cbstr, bstrMixerTimers, cbstrOut)))
			return E_FAIL;

		DeleteFile(cstTemp);
	}
	else MoveFile(cstTemp, cstOutput);

	return S_OK;
}
*/

/*
STDMETHODIMP CWMXUtility::MultiWMXAudio2Wav(BSTR bstrAudioList, IUnknown *pManager, BSTR bstrMixerTimers, long lBitsPerSample, long lChannels, long lSampleRate, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrAudioList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;
			
	CString cstOutput(bstrOutput);
	CString cstTemp(cstOutput + ".tmp");
	DeleteFile(cstOutput);
	DeleteFile(cstTemp);
	
	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, (WORD)lBitsPerSample, (WORD)lChannels, (DWORD)lSampleRate, (LPSTR)(LPCSTR)cstTemp))
		return E_FAIL;
	
	WavWriterSetEasyManager(&WavWriter, pManager);
	
	BOOL fError=FALSE;
	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavWriterClose(&WavWriter);
		return E_FAIL;
	}

	long ii=0, lmsTotal=0;
	for(ii=0; ii<CstArray.GetSize(); ii++) {
		long lmsStart = StartArray.GetAt(ii);
		long lmsStop = StopArray.GetAt(ii);
		lmsTotal += lmsStop - lmsStart;
	}
		
	long lmsCurrent=0;
	for(ii=0; ii<CstArray.GetSize(); ii++) {
		long lmsStart = StartArray.GetAt(ii);
		long lmsStop = StopArray.GetAt(ii);
		ASSERT(lmsStop>lmsStart);
		
		CComBSTR cbstrFile(CstArray.GetAt(ii));

		if(!WMX2Wav(cbstrFile, lmsStart, lmsStop, &WavWriter)) {
			fError = TRUE;
			break;
		}
		if(IsWindow(m_hWndProgress)) {
			lmsCurrent += lmsStop - lmsStart;
			WORD wPercentage = (WORD)(lmsCurrent * 100 / lmsTotal);
			SendMessage(m_hWndProgress, m_lMsgProgress, 0, MAKELONG(wPercentage, 100));
		}
	}
	
	WavWriterClose(&WavWriter);
	
	if(fError) return E_FAIL;
	
	if(wcslen(bstrMixerTimers)>0) {
		CComBSTR cbstr(cstTemp), cbstrOut(cstOutput);
		if(FAILED(MultiWMXMixer2Wav(cbstr, bstrMixerTimers, cbstrOut)))
			return E_FAIL;
		
		DeleteFile(cstTemp);
	}
	else MoveFile(cstTemp, cstOutput);
	
	return S_OK;
}
*/

STDMETHODIMP CWMXUtility::MultiWMXAudio2Wav(BSTR bstrAudioList, IUnknown *pManager, BSTR bstrMixerTimers, long lBitsPerSample, long lChannels, long lSampleRate, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrAudioList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	long ii=0, lmsTotal=0;
	for(ii=0; ii<CstArray.GetSize(); ii++) {
		long lmsStart = StartArray.GetAt(ii);
		long lmsStop = StopArray.GetAt(ii);
		lmsTotal += lmsStop - lmsStart;
	}
	
	CString cstOutput(bstrOutput);
	CString cstTemp(cstOutput + ".tmp");
	DeleteFile(cstOutput);
	DeleteFile(cstTemp);
	
	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, (WORD)lBitsPerSample, (WORD)lChannels, (DWORD)lSampleRate, (LPSTR)(LPCSTR)cstTemp))
		return E_FAIL;
	
	WavWriterSetEasyManager(&WavWriter, pManager);
	
	BOOL fError=FALSE;
	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavWriterClose(&WavWriter);
		return E_FAIL;
	}
	
	long lmsCurrent=0;
	for(ii=0; ii<CstArray.GetSize(); ii++) {
		CString cstLoop(cstTemp);
		cstLoop += ".wav";

		DeleteFile(cstLoop);
		
		WAVWRITER loopWavWriter;
		memset(&loopWavWriter, 0, sizeof(loopWavWriter));
		if(!WavWriterCreate_PCM(&loopWavWriter, (WORD)lBitsPerSample, (WORD)lChannels, (DWORD)lSampleRate, (LPSTR)(LPCSTR)cstLoop))
			return E_FAIL;
				
		BOOL fError=FALSE;
		if(!WavWriterStartDataWrite(&loopWavWriter)) {
			WavWriterClose(&loopWavWriter);
			return E_FAIL;
		}
		
		long lmsStart = StartArray.GetAt(ii);
		long lmsStop = StopArray.GetAt(ii);
		ASSERT(lmsStop>lmsStart);
		
		CComBSTR cbstrFile(CstArray.GetAt(ii));
		
		if(!WMX2Wav(cbstrFile, lmsStart, lmsStop, &loopWavWriter)) {
			fError = TRUE;
			break;
		}
		if(IsWindow(m_hWndProgress)) {
			lmsCurrent += lmsStop - lmsStart;
			WORD wPercentage = (WORD)(lmsCurrent * 100 / lmsTotal);
			SendMessage(m_hWndProgress, m_lMsgProgress, 0, MAKELONG(wPercentage, 100));
		}
		WavWriterClose(&loopWavWriter);

		WAVREADER loopWavReader;
		if(!WavReaderOpen(&loopWavReader, (LPSTR)(LPCSTR)cstLoop))
			return E_FAIL;
		
		if(!WavReaderStartDataRead(&loopWavReader)) {
			WavReaderClose(&loopWavReader);
			return E_FAIL;
		}
		
		UINT uSize = (UINT)((double)(lmsStop - lmsStart) * loopWavReader.pwfx->nAvgBytesPerSec / 1000.0);
		LPBYTE pdata = (LPBYTE)malloc(uSize);
		if(pdata) {
			WavReaderReadSerialData(&loopWavReader, uSize, pdata, &uSize);
			WavWriterWriteData(&WavWriter, pdata, uSize);
		}

		WavReaderClose(&loopWavReader);
		
		if(fError) return E_FAIL;
	}	

	WavWriterClose(&WavWriter);
	
	if(fError) return E_FAIL;
	
	if(wcslen(bstrMixerTimers)>0) {
		CComBSTR cbstr(cstTemp), cbstrOut(cstOutput);
		if(FAILED(MultiWMXMixer2Wav(cbstr, bstrMixerTimers, cbstrOut)))
			return E_FAIL;
		
		DeleteFile(cstTemp);
	}
	else MoveFile(cstTemp, cstOutput);
	
	return S_OK;
}

STDMETHODIMP CWMXUtility::MultiWMXAudioFromWav(BSTR bstrWMXList, IUnknown *pVFXManager, BSTR bstrWave, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrWMXList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	CMultiWMXWav2Audio	MultiWMXWav2Audio;

	if(IsWindow(m_hWndProgress)) MultiWMXWav2Audio.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) MultiWMXWav2Audio.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	CString cstWav(bstrWave);
	WAVREADER WavReader;
	if(!WavReaderOpen(&WavReader, (LPSTR)(LPCSTR)cstWav))
		return E_FAIL;

	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	MultiWMXWav2Audio.SetupTotalDuration((QWORD)WavReader.ckData.cksize*TIMEUNIT/WavReader.pwfx->nAvgBytesPerSec);

	CWriterBase cwriterbase;
	if(!cwriterbase.Create(bstrOutput, bstrProfile)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	cwriterbase.ConfigAudioInputSimpleUncompressed(WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nChannels, WavReader.pwfx->nSamplesPerSec);
	cwriterbase.ConfigVideoInputSimpleUncompressed(16, 0);

	if(!cwriterbase.BeginWriting()) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	BOOL fError = FALSE;
	DWORD dwmsTime=0;
	UINT uTemp, uPosWave=0;	
	IEasyManager *pEasyManager=(IEasyManager *)pVFXManager;
	if(pEasyManager!=NULL) {
//		HRESULT hval = pVFXManager->QueryInterface(IID_IEasyManager, (void**)&pEasyManager);
//		if(FAILED(hval)||(pEasyManager==NULL))
//			return E_FAIL;
		pEasyManager->SetupAudioInfo(WavReader.pwfx->nChannels, WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nSamplesPerSec, 0);
		MultiWMXWav2Audio.SetEasyManager(pEasyManager);
	}
	MultiWMXWav2Audio.SetWriterBase(&cwriterbase);
	for(int ii=0; ii<TimeArray.GetSize(); ii++) {
// This version does not allow empty period!!
//		uTemp = (UINT)(TimeArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
//		uTemp = uTemp / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;		
//		if(uPosWave < uTemp) {
//			if(!MultiWMXWav2Audio.AppendWavReader(dwmsTime<TimeArray.GetAt(ii), uPosWave, uTemp - uPosWave, &WavReader)) {
//				fError = TRUE;
//				break;
//			}
//		}
		CComBSTR cbstr(CstArray.GetAt(ii));
		if(!MultiWMXWav2Audio.AddWMXFile(cbstr, StartArray.GetAt(ii), StopArray.GetAt(ii), &WavReader, uPosWave)) {
				fError = TRUE;
				break;
		}
		dwmsTime = TimeArray.GetAt(ii) + StopArray.GetAt(ii) - StartArray.GetAt(ii);
	}

// This version does not allow empty period!!
//	if(!fError) {
//		if(uPosWave < WavReader.ckData.cksize) {
//			fError = MultiWMXWav2Audio.AppendWavReader(TRUE, uPosWave, WavReader.ckData.cksize - uPosWave, &WavReader);				
//		}
//	}

	WavReaderClose(&WavReader);
	MultiWMXWav2Audio.SetEasyManager(NULL);

	cwriterbase.EndWriting();
	cwriterbase.Close();

	pEasyManager = NULL;
//	RELEASE(pEasyManager);

	return S_OK;
}

STDMETHODIMP CWMXUtility::MultiWMXDirectCopy(BSTR bstrList, VARIANT_BOOL vbVideo, VARIANT_BOOL vbScript, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	if(TimeArray.GetSize()<=0) 
		return E_FAIL;

	CInfoReader cinforeader;
	CComBSTR	cbstrSource(CstArray.GetAt(0));
	if(!cinforeader.Open(cbstrSource)) return E_FAIL;
	QWORD qwAlign=0;
	if(!cinforeader.GetAudioAlignTime(&qwAlign)||(qwAlign==0))
		qwAlign = 1;

	CWriterBase cwriterbase;
	BOOL fOK = cwriterbase.Create(bstrOutput, cinforeader.m_pProfile);
	cinforeader.Close();
	if(!fOK) return E_FAIL;

	cwriterbase.ConfigAudioInputCodecNoNeeded();
	cwriterbase.ConfigVideoInputCodecNoNeeded();
	cwriterbase.ConfigScriptInputCodecNoNeeded();

	if(!cwriterbase.BeginWriting()) return E_FAIL;

	CMultiWMXCopy	ccopy;
	
	if(IsWindow(m_hWndProgress)) ccopy.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) ccopy.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	
	BOOL fError=FALSE;
	DWORD dwmsTime = 0;	

	int		nIndex = TimeArray.GetSize()-1;
	DWORD	dwmsLen = TimeArray.GetAt(nIndex) + StopArray.GetAt(nIndex) - StartArray.GetAt(nIndex);
	ccopy.SetupTotalDuration((QWORD)TIMEUNIT / 1000 * dwmsLen);	
	ccopy.SetAlignTime(qwAlign);
	ccopy.SetWriterBase(&cwriterbase);
	for(int ii=0; ii<TimeArray.GetSize(); ii++) {
// This version does not allow empty period!!
//		if(dwmsTime<TimeArray.GetAt(ii)) { 
//			// append mute compressed audio, how??
//		}
		CComBSTR cbstrWMX(CstArray.GetAt(ii));
		if(!ccopy.AddWMX(cbstrWMX, StartArray.GetAt(ii), StopArray.GetAt(ii), 
			vbVideo==OATRUE, vbScript==OATRUE)) {
			fError = TRUE;
			break;
		}
	}	
	
	cwriterbase.EndWriting();
	cwriterbase.Close();

	if(fError) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::MultiWMXCopyWav(BSTR bstrList, BSTR bstrWave, BSTR bstrOrgProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrList, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	CMultiWMXCopyWav ccopy;

	if(IsWindow(m_hWndProgress)) ccopy.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) ccopy.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	int		nIndex = TimeArray.GetSize()-1;
	DWORD	dwmsLen = TimeArray.GetAt(nIndex) + StopArray.GetAt(nIndex) - StartArray.GetAt(nIndex);
	ccopy.SetupTotalDuration((QWORD)TIMEUNIT / 1000 * dwmsLen);	

	CString cstWav(bstrWave);
	WAVREADER WavReader;
	if(!WavReaderOpen(&WavReader, (LPSTR)(LPCSTR)cstWav))
		return E_FAIL;

	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	CWriterBase cwriterbase;
	if(!cwriterbase.Create(bstrOutput, bstrOrgProfile)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	cwriterbase.ConfigAudioInputSimpleUncompressed(WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nChannels, WavReader.pwfx->nSamplesPerSec);
	cwriterbase.ConfigVideoInputCodecNoNeeded();

	if(!cwriterbase.BeginWriting()) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	BOOL fError = FALSE;
	DWORD dwmsTime=0;
	UINT uTemp, uPosWave=0;	
	ccopy.SetWriterBase(&cwriterbase);
	for(int ii=0; ii<TimeArray.GetSize(); ii++) {
//		uTemp = (UINT)(TimeArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
//		uTemp = uTemp / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;		
//		if(uPosWave < uTemp) {
//			if(!ccopy.AppendWavReader(dwmsTime<TimeArray.GetAt(ii), uPosWave, uTemp - uPosWave, &WavReader)) {
//				fError = TRUE;
//				break;
//			}
//		}
		CComBSTR cbstr(CstArray.GetAt(ii));
		if(!ccopy.AddWMXFile(cbstr, StartArray.GetAt(ii), StopArray.GetAt(ii), &WavReader, uPosWave)) {
			fError = TRUE;
			break;
		}
		dwmsTime = TimeArray.GetAt(ii) + StopArray.GetAt(ii) - StartArray.GetAt(ii);
	}

//	if(!fError) {
//		if(uPosWave < WavReader.ckData.cksize) {
//			fError = ccopy.AppendWavReader(TRUE, uPosWave, WavReader.ckData.cksize - uPosWave, &WavReader);				
//		}
//	}

	WavReaderClose(&WavReader);

	cwriterbase.EndWriting();
	cwriterbase.Close();

	return S_OK;
}

#include "MultiWMXMixer2Wav.h"
STDMETHODIMP CWMXUtility::MultiWMXMixer2Wav(BSTR bstrWav, BSTR bstrLists, BSTR bstrOutWav)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CDWordArray TimeArray, StartArray, StopArray;
	CStringArray CstArray;
	if(!ParseList(bstrLists, TimeArray, CstArray, StartArray, StopArray))
		return E_FAIL;

	CMultiWMXMixer2Wav cmixer;

	if(IsWindow(m_hWndProgress)) cmixer.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) cmixer.SetupQueryAbort(m_hWndAbort, m_pbAbort);	

	CString cstWav(bstrWav), cstOutWav(bstrOutWav);
	WAVREADER WavReader;
	if(!WavReaderOpen(&WavReader, (LPSTR)(LPCSTR)cstWav))
		return E_FAIL;

	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	cmixer.SetupTotalDuration((QWORD)TIMEUNIT * WavReader.ckData.cksize / WavReader.pwfx->nAvgBytesPerSec);	

	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nChannels, WavReader.pwfx->nSamplesPerSec, (LPSTR)(LPCSTR)cstOutWav)) {
		WavReaderClose(&WavReader);
		return E_FAIL;
	}

	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavReaderClose(&WavReader);
		WavWriterClose(&WavWriter);
		return E_FAIL;
	}

	cmixer.SetWriter(&WavWriter);

	BOOL fError = FALSE;
	DWORD dwmsTime=0;
	UINT uTemp, uPosWave=0;	
	for(int ii=0; ii<TimeArray.GetSize(); ii++) {
//		uTemp = (UINT)(TimeArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
//		uTemp = uTemp / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;		
//		if(uPosWave < uTemp) {
//			if(!WavWriterAppendFile(&WavWriter, uPosWave, uTemp, (LPSTR)(LPCSTR)cstWav)) {
//				fError = TRUE;
//				break;
//			}
//			uPosWave = uTemp;
//
//			cmixer.ReportMultiProgress(cmixer.m_hWndProgress, cmixer.m_lMsgProgress, (QWORD)TIMEUNIT * uPosWave / WavReader.pwfx->nAvgBytesPerSec);
//		}		
		CComBSTR cbstr(CstArray.GetAt(ii));
		if(!cmixer.MixerWMXAudio(cbstr, StartArray.GetAt(ii), StopArray.GetAt(ii), &WavReader, uPosWave)) {
			fError = TRUE;
			break;
		}
		dwmsTime = TimeArray.GetAt(ii) + StopArray.GetAt(ii) - StartArray.GetAt(ii);

		cmixer.ReportMultiProgress(cmixer.m_hWndProgress, cmixer.m_lMsgProgress, (QWORD)TIMEUNIT * uPosWave / WavReader.pwfx->nAvgBytesPerSec);
	}

//	if(!fError) {
//		if(uPosWave < WavReader.ckData.cksize) {
//			if(WavWriterAppendFile(&WavWriter, uPosWave, WavReader.ckData.cksize, (LPSTR)(LPCSTR)cstWav))
//				cmixer.ReportMultiProgress(cmixer.m_hWndProgress, cmixer.m_lMsgProgress, (QWORD)TIMEUNIT * WavReader.ckData.cksize / WavReader.pwfx->nAvgBytesPerSec);
//		}
//	}

	WavReaderClose(&WavReader);
	WavWriterClose(&WavWriter);

	cmixer.Close();

	if(fError) return E_FAIL;

	return S_OK;
}

#include "MultiTimerBase.h"
STDMETHODIMP CWMXUtility::Wav2WMX(BSTR bstrWav, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMultiTimerBase MultiTimerBase;

	CString	cstWave(bstrWav);
	WAVREADER wavreader;
	if(!WavReaderOpen(&wavreader, (LPSTR)(LPCSTR)cstWave))
		return E_FAIL;
	if(!WavReaderStartDataRead(&wavreader)) {
		WavReaderClose(&wavreader);
		return E_FAIL;
	}

	MultiTimerBase.SetupTotalDuration((QWORD)TIMEUNIT*(wavreader.ckData.cksize/wavreader.pwfx->nAvgBytesPerSec));

	CWriterBase cwriterbase;
	if(!cwriterbase.Create(bstrOutput, bstrProfile)) {
		WavReaderClose(&wavreader);
		return E_FAIL;
	}

	if(!cwriterbase.ConfigAudioInputSimpleUncompressed(wavreader.pwfx->wBitsPerSample, wavreader.pwfx->nChannels, wavreader.pwfx->nSamplesPerSec)) {		
		WavReaderClose(&wavreader);
		cwriterbase.Close();
		return E_FAIL;
	}

	if(!cwriterbase.BeginWriting()) {
		WavReaderClose(&wavreader);
		cwriterbase.Close();
		return E_FAIL;
	}

	BOOL fError = FALSE;
	QWORD qwTime = 0;
	DWORD dwWavPos=0, dwCopySize=0;
	while(dwWavPos<wavreader.ckData.cksize) {
		dwCopySize = wavreader.ckData.cksize - dwWavPos;
		if(dwCopySize>wavreader.pwfx->nAvgBytesPerSec) dwCopySize = wavreader.pwfx->nAvgBytesPerSec;

		INSSBuffer *pNSSBuffer=NULL;		
		HRESULT hr = cwriterbase.m_pWriter->AllocateSample(dwCopySize, &pNSSBuffer);
		if(FAILED(hr)) {
			fError = TRUE;
			break;
		}

		LPBYTE	pBuffer=NULL;
		hr = pNSSBuffer->GetBuffer(&pBuffer);
		if(FAILED(hr)) {
			RELEASE(pNSSBuffer);
			fError = TRUE;
			break;
		}
		
		UINT uRetSize=0;
		if(!WavReaderReadDirectData(&wavreader, dwWavPos, dwCopySize, pBuffer, &uRetSize)) {
			RELEASE(pNSSBuffer);
			fError = TRUE;
			break;
		}

		hr = cwriterbase.m_pWriter->WriteSample(cwriterbase.m_dwInputAudio, qwTime, 0, pNSSBuffer);
		RELEASE(pNSSBuffer);
		if(FAILED(hr)) {			
			fError = TRUE;
			break;
		}

		qwTime += (QWORD)dwCopySize * TIMEUNIT / wavreader.pwfx->nAvgBytesPerSec;
		dwWavPos += dwCopySize;

		MultiTimerBase.ReportMultiProgress(m_hWndProgress, m_lMsgProgress, qwTime);
	}

	WavReaderClose(&wavreader);

	cwriterbase.EndWriting();
	cwriterbase.Close();

	if(fError) return E_FAIL;

	return S_OK;
}

BOOL SameMediaType(WM_MEDIA_TYPE *pMedia1, WM_MEDIA_TYPE *pMedia2)
{
    if( pMedia1->majortype != pMedia2->majortype )
        return FALSE ;

    if( pMedia1->subtype != pMedia2->subtype )
        return FALSE ;

    if( pMedia1->bFixedSizeSamples != pMedia2->bFixedSizeSamples )
        return FALSE ;
    
    if( pMedia1->bTemporalCompression != pMedia2->bTemporalCompression )
        return FALSE ;

    if( pMedia1->lSampleSize != pMedia2->lSampleSize )
        return FALSE ;

    if( pMedia1->formattype != pMedia2->formattype )
        return FALSE ;

    if( pMedia1->cbFormat != pMedia2->cbFormat )
        return FALSE ;

    if( 0 != memcmp( pMedia1->pbFormat, pMedia2->pbFormat, pMedia1->cbFormat ) )
       return FALSE ;

    return TRUE ;
}

BOOL SameStreamProps(IWMMediaProps *pProps1, IWMMediaProps *pProps2)
{
	if((pProps1==NULL)||(pProps2==NULL))
		return FALSE;

	HRESULT			hr=S_OK;
	DWORD			cbType1=0, cbType2=0;
	WM_MEDIA_TYPE	*pfmt1=NULL, *pfmt2=NULL;
	do {
		hr = pProps1->GetMediaType(NULL, &cbType1);
		if(FAILED(hr)) break;
		pfmt1 = (WM_MEDIA_TYPE*)malloc(cbType1);
		if(pfmt1==NULL) {
			hr = E_FAIL;
			break;
		}
		hr = pProps1->GetMediaType(pfmt1, &cbType1);
		if(FAILED(hr)) break;

		hr = pProps2->GetMediaType(NULL, &cbType2);
		if(FAILED(hr)) break;
		pfmt2 = (WM_MEDIA_TYPE*)malloc(cbType2);
		if(pfmt2==NULL) {
			hr = E_FAIL;
			break;
		}
		hr = pProps2->GetMediaType(pfmt2, &cbType2);
		if(FAILED(hr)) break;

		if(!SameMediaType(pfmt1, pfmt2)) {
			hr = E_FAIL;
			break;
		}
	}
	while(FALSE);

	if(pfmt1!=NULL) free(pfmt1);
	if(pfmt2!=NULL) free(pfmt2);

	return SUCCEEDED(hr);
}

BOOL SameProfileStream(IWMProfile *pProfile1, WORD wStream1, IWMProfile *pProfile2, WORD wStream2)
{
	if((pProfile1==NULL)||(pProfile2==NULL)) 
		return FALSE;

	IWMStreamConfig *pStream1=NULL, *pStream2=NULL;
	IWMMediaProps	*pProps1=NULL, *pProps2=NULL;

	HRESULT hr=S_OK;
	do {
		hr = pProfile1->GetStreamByNumber(wStream1, &pStream1);
		if(FAILED(hr)) break;
		hr = pProfile2->GetStreamByNumber(wStream2, &pStream2);
		if(FAILED(hr)) break;
		hr = pStream1->QueryInterface(IID_IWMMediaProps, (VOID**)&pProps1);
		if(FAILED(hr)) break;
		hr = pStream2->QueryInterface(IID_IWMMediaProps, (VOID**)&pProps2);
		if(FAILED(hr)) break;

		if(!SameStreamProps(pProps1, pProps2)) {
			hr = E_FAIL;
			break;
		}
	}
	while(FALSE);
	
	RELEASE(pStream1);
	RELEASE(pStream2);
	RELEASE(pProps1);
	RELEASE(pProps2);

	return SUCCEEDED(hr);
}

STDMETHODIMP CWMXUtility::IsWMXProtected(BSTR bstrWMX, VARIANT_BOOL *pvbProtected)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    IWMMetadataEditor* pEditor     = NULL ;
    IWMHeaderInfo*     pHeaderInfo = NULL ;

    HRESULT hr = S_OK ;	
    //
    // Open the files using metadata editor.
    // Opening it using IWMReader would decrement the usage count,
    // if its a protected file
    // Use False Loop to Avoid Goto!!
	*pvbProtected = OAFALSE;
    do
    {
        hr = WMCreateEditor( &pEditor );
	    if( FAILED( hr ) ) break;

	    hr = pEditor->Open( bstrWMX ) ;
	    if( FAILED ( hr ) ) break;

	    hr = pEditor->QueryInterface( IID_IWMHeaderInfo, ( void ** ) &pHeaderInfo );
	    if( FAILED( hr ) ) break;

        WORD wStreamNum = 0 ;
		WMT_ATTR_DATATYPE type ;
		BYTE value[4] ;
		WORD cbLength = 4 ;
        //
        // Check the protected attribute of the header
        //
		hr = pHeaderInfo->GetAttributeByName( &wStreamNum,  g_wszWMProtected, &type, value, &cbLength ) ;
		if( FAILED( hr ) ) break;

        if( value[0] ) {
			*pvbProtected = OATRUE;
			break ;
		}
    }
    while( FALSE ) ;
    
    RELEASE( pHeaderInfo ) ;
    RELEASE( pEditor ) ;

    return hr ;
}

STDMETHODIMP CWMXUtility::IsWMXProfileStreamCompatible(BSTR bstrWMX1, BSTR bstrWMX2, VARIANT_BOOL *pvbAudio, VARIANT_BOOL *pvbVideo, VARIANT_BOOL *pvbScript)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=0;
	VARIANT_BOOL vbProtected=OAFALSE;
	hr = IsWMXProtected(bstrWMX1, &vbProtected);
	if(FAILED(hr)||(vbProtected==OATRUE)) return E_FAIL;
	hr = IsWMXProtected(bstrWMX2, &vbProtected);
	if(FAILED(hr)||(vbProtected==OATRUE)) return E_FAIL;

	CInfoReader cInfoReader1, cInfoReader2;

	if(!cInfoReader1.Open(bstrWMX1)) 
		return E_FAIL;
	if(!cInfoReader2.Open(bstrWMX2)) {
		cInfoReader1.Close();
		return E_FAIL;
	}

	*pvbAudio = OATRUE; *pvbVideo = OATRUE; *pvbScript = OATRUE;
	if(pvbAudio!=NULL) {
		*pvbAudio = SameProfileStream(cInfoReader1.m_pProfile, cInfoReader1.m_wStreamAudio,
			cInfoReader2.m_pProfile, cInfoReader2.m_wStreamAudio)?OATRUE:OAFALSE;
	}
	if(pvbVideo!=NULL) {	
		if(cInfoReader1.m_fVideoExisted&&cInfoReader2.m_fVideoExisted) {
			*pvbVideo = SameProfileStream(cInfoReader1.m_pProfile, cInfoReader1.m_wStreamVideo,
				cInfoReader2.m_pProfile, cInfoReader2.m_wStreamVideo)?OATRUE:OAFALSE;
		}
	}

	if(pvbScript!=NULL) {
		if(cInfoReader1.m_fScriptExisted&&cInfoReader2.m_fScriptExisted) {
			*pvbScript = SameProfileStream(cInfoReader1.m_pProfile, cInfoReader1.m_wStreamScript,
				cInfoReader2.m_pProfile, cInfoReader2.m_wStreamScript)?OATRUE:OAFALSE;
		}
	}

	return S_OK;
}

STDMETHODIMP CWMXUtility::IsWMXProfileCompatible(BSTR bstrWMX1, BSTR bstrWMX2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VARIANT_BOOL vbAudio=OAFALSE, vbVideo=OAFALSE, vbScript=OAFALSE;
	HRESULT hr = IsWMXProfileStreamCompatible(bstrWMX1, bstrWMX2, &vbAudio, &vbVideo, &vbScript);
	if(FAILED(hr)) return hr;

	if((vbAudio!=OATRUE)||(vbVideo!=OATRUE)||(vbScript!=OATRUE))
		return E_FAIL;

	return S_OK;
}

#include "Index.h"
STDMETHODIMP CWMXUtility::WMXMakeIndex(BSTR bstrWMX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CIndex	cindex;

	if(IsWindow(m_hWndProgress)) 
		cindex.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);

	if(!cindex.MakeIndex(bstrWMX))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::IsWMXIndexExisted(BSTR bstrWMX, VARIANT_BOOL *pvbExisted)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CIndex	cindex;

	*pvbExisted = cindex.IndexExisted(bstrWMX)?OATRUE:OAFALSE;

	return S_OK;
}

STDMETHODIMP CWMXUtility::GetWMXDuration(BSTR bstrWMX, double *pdmsLength)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pdmsLength==NULL) 
		return E_FAIL;

	CInfoReader cInfoReader;

	if(!cInfoReader.Open(bstrWMX))
		return E_FAIL;
	
	QWORD qwDuration=0;
	if(!cInfoReader.GetDuration(&qwDuration)) {
		cInfoReader.Close();
		return E_FAIL;
	}

	*pdmsLength = (double)((LONGLONG)qwDuration / 10000.0); // mini seconds

	cInfoReader.Close();

	return S_OK;
}

BOOL WriteScriptTable(LPSTR szFile, CStringArray &Scripts, CDWordArray &Times)
{
	CFile		cf;
	char		szlf[3] = { 13, 10, 0 };
	if(!cf.Open(szFile, CFile::modeCreate|CFile::modeWrite))
		return E_FAIL;

	for(int ii=0; ii<Scripts.GetSize(); ii++) {
		CString	cst;
		cst.Format("%s %lu %s", (LPCSTR)Scripts.GetAt(ii), Times.GetAt(ii), szlf);

		try {
			cf.WriteHuge((LPCSTR)cst, cst.GetLength());
		}
		catch( CFileException* theException ) {
			if(theException!=NULL) theException->Delete();
			return FALSE;
		}
	}

	cf.Close();

	return TRUE;
}

#include "WMXScriptDump.h"
STDMETHODIMP CWMXUtility::SpecWMXDumpScript(BSTR bstrWMX, BSTR bstrOutTable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXScriptDump WMXScriptDump;
	if(IsWindow(m_hWndProgress)) WMXScriptDump.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXScriptDump.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	CStringArray CmdArray, TextArray;
	CDWordArray	 TimeArray;
	if(!WMXScriptDump.Script2Array(bstrWMX, CmdArray, TextArray, TimeArray))
		return E_FAIL;

	if(!WriteScriptTable((LPSTR)(LPCSTR)CString(bstrOutTable), TextArray, TimeArray))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::PRXSubtract(BSTR bstrPRX1, VARIANT_BOOL vbVideo, VARIANT_BOOL vbScript, BSTR bstrPRX2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=0;
	IWMProfileManager *pWMPM0=NULL;
	hr = WMCreateProfileManager(&pWMPM0);
	if ( FAILED( hr ) ) return hr;

	IWMProfileManager2 *pWMPM=NULL;
	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if ( FAILED( hr ) ) return hr;

	hr = E_FAIL;
	IWMProfile *pProfile=NULL, *pProfileNew=NULL;
	pProfile = PF_LoadFromPRX(pWMPM, bstrPRX1);
	if(pProfile!=NULL) {		
		pProfileNew = PF_Subtract(pWMPM, pProfile, (vbVideo==OATRUE), (vbScript==OATRUE));
		if(pProfileNew!=NULL) {
			hr = PF_SavePRX(pWMPM, pProfileNew, bstrPRX2);
			RELEASE(pProfileNew);
		}
		RELEASE(pProfile);
	}
	RELEASE(pWMPM);

	return S_OK;
}

BOOL LowerPRX(BSTR bstrPRX, VARIANT_BOOL vbAudio, long lAudioSampleRate, long lAudioChannels, long lAudioKiloBPS, VARIANT_BOOL vbVideo, long lVideoSecPerKey, long lVideoQuality, long lVideoFPS, long lVideoKiloBPS, VARIANT_BOOL vbScript, long lScriptKiloBPS, BSTR bstrOutPRX)
{
	IWMProfileManager	*pwmpm0=NULL;
	IWMProfileManager2	*pwmpm=NULL;
	HRESULT hval = WMCreateProfileManager(&pwmpm0);
	if ( FAILED( hval ) ) return FALSE;

	hval = pwmpm0->QueryInterface(IID_IWMProfileManager2, (void**)&pwmpm);
	RELEASE(pwmpm0);
	if(FAILED(hval)) return FALSE;
		
	hval = E_FAIL;
	IWMProfile *pProfile = PF_LoadFromPRX(pwmpm, bstrPRX);
	if(pProfile!=NULL) {
		if(lAudioChannels<1) lAudioChannels = 1;
		if(lAudioChannels>2) lAudioChannels = 2;
		if(lAudioKiloBPS<0) lAudioKiloBPS = 0;
		if(lVideoSecPerKey<=0) lVideoSecPerKey = 1;
		if(lVideoFPS<=0) lVideoFPS = 10; // min default is 10 fps
		if(lVideoKiloBPS<0) lVideoKiloBPS = 0;
		if(lScriptKiloBPS<0) lScriptKiloBPS = 0;
		IWMProfile *pNewProfile=NULL;
		pNewProfile = PF_Lower(pwmpm, pProfile, 
			(vbAudio==OATRUE), lAudioSampleRate, lAudioChannels, lAudioKiloBPS * 1000, 
			(vbVideo==OATRUE), (DWORD)lVideoSecPerKey, (DWORD)lVideoQuality, (WORD)lVideoFPS, (DWORD)lVideoKiloBPS * 1024, 
			(vbScript==OATRUE), lScriptKiloBPS * 1024);
		RELEASE(pProfile);
		hval = PF_SavePRX(pwmpm, pNewProfile, bstrOutPRX);
		RELEASE(pNewProfile);
	}
	RELEASE(pwmpm);

	return TRUE;
}

STDMETHODIMP CWMXUtility::PRXLower(BSTR bstrPRX, VARIANT_BOOL vbAudioLower, long lAudioSampleRate, long lAudioChannels, long lAudioKiloBPS, VARIANT_BOOL vbVideoLower, long lVideoSecPerKey, long lVideoQuality, long lVideoFPS, long lVideoKiloBPS, VARIANT_BOOL vbScriptLower, long lScriptKiloBPS, BSTR bstrOutPRX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(!LowerPRX(bstrPRX, vbAudioLower, lAudioSampleRate, lAudioChannels, lAudioKiloBPS, vbVideoLower, lVideoSecPerKey, lVideoQuality, lVideoFPS, lVideoKiloBPS, vbScriptLower, lScriptKiloBPS, bstrOutPRX))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::PRXGetParams(BSTR bstrPRX, PROFILEPARAMS *pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pParam==NULL)
		return E_FAIL;

	IWMProfileManager	*pwmpm0=NULL;
	IWMProfileManager2	*pwmpm=NULL;
	HRESULT hval = WMCreateProfileManager(&pwmpm0);
	if ( FAILED( hval ) ) return FALSE;

	hval = pwmpm0->QueryInterface(IID_IWMProfileManager2, (void**)&pwmpm);
	RELEASE(pwmpm0);
	if(FAILED(hval)) return FALSE;

	IWMProfile *pProfile=NULL;
	pProfile = PF_LoadFromPRX(pwmpm, bstrPRX);
	RELEASE(pwmpm);
	if(pProfile==NULL) return FALSE;

	AUDIO_PARAMS apm;
	VIDEO_PARAMS vpm;
	SCRIPT_PARAMS spm;
	hval = PF_GetParameter(pProfile, apm, vpm, spm);
	RELEASE(pProfile);

	memcpy(&pParam->ainfo, &apm, sizeof(apm));
	memcpy(&pParam->vinfo, &vpm, sizeof(vpm));
	memcpy(&pParam->sinfo, &spm, sizeof(spm));

	return S_OK;
}

BOOL WMXGetProfileParameter(IWMProfileManager2 *pwmpm, BSTR bstrWMX, AUDIO_PARAMS &apm, VIDEO_PARAMS &vpm, SCRIPT_PARAMS &spm)
{
	CInfoReader InfoReader;

	if(!InfoReader.Open(bstrWMX))
		return FALSE;

	BOOL fOK = PF_GetParameter(InfoReader.m_pProfile, apm, vpm, spm);

	InfoReader.Close();

	return fOK;
}

STDMETHODIMP CWMXUtility::PRXFromWMX(BSTR bstrWMX, VARIANT_BOOL vbVideo, long lFPS, VARIANT_BOOL vbScript, BSTR bstrOutPRX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=0;
	IWMProfileManager *pWMPM0=NULL;
	hr = WMCreateProfileManager(&pWMPM0);
	if ( FAILED( hr ) ) return hr;

	IWMProfileManager2 *pWMPM=NULL;
	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if ( FAILED( hr ) ) return hr;

	AUDIO_PARAMS	apm;
	VIDEO_PARAMS	vpm;
	SCRIPT_PARAMS	spm;

	if(!WMXGetProfileParameter(pWMPM, bstrWMX, apm, vpm, spm)) {
		RELEASE(pWMPM);
		return E_FAIL;
	}
	
	BOOL fUseScript = (vbScript==OATRUE)&&(spm.dwBitrate>0);
	BOOL fUseVideo = (vbVideo==OATRUE)&&(vpm.dwBitrate>0);

	PF_CorrectAPM(apm);

	if(fUseVideo) {
		vpm.dwFPS = (lFPS>0)?lFPS:1;
		PF_CorrectVPM(vpm);
	}

	if(fUseScript) PF_CorrectSPM(spm);

	IWMProfile *pProfile = PF_Generate(pWMPM, &apm, fUseScript?&spm:NULL, 
		fUseVideo?1:0, fUseVideo?&vpm:NULL);			
	if(pProfile==NULL) {
		RELEASE(pWMPM);
		return E_FAIL;
	}

	hr = PF_SavePRX(pWMPM, pProfile, bstrOutPRX);
	RELEASE(pProfile);
	RELEASE(pWMPM);	

	return S_OK;
}

STDMETHODIMP CWMXUtility::PRXGen(PROFILEPARAMS *pParam, BSTR bstrGenPRX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(pParam==NULL)
		return E_FAIL;

//	PF_CorrectAPM(*((AUDIO_PARAMS*)&pParam->ainfo));
//	PF_CorrectVPM(*((VIDEO_PARAMS*)&pParam->vinfo));
//	PF_CorrectSPM(*((SCRIPT_PARAMS*)&pParam->sinfo));

	HRESULT hr=0;
	IWMProfileManager *pWMPM0=NULL;
	hr = WMCreateProfileManager(&pWMPM0);
	if ( FAILED( hr ) ) return hr;

	IWMProfileManager2 *pWMPM=NULL;
	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if ( FAILED( hr ) ) return hr;

	IWMProfile *pProfile = PF_Generate(pWMPM, (AUDIO_PARAMS*)&pParam->ainfo, 
		(pParam->sinfo.dwBitrate>0)?(SCRIPT_PARAMS*)&pParam->sinfo:NULL,
		(pParam->vinfo.dwBitrate>0)?1:0,
		(pParam->vinfo.dwBitrate>0)?(VIDEO_PARAMS*)&pParam->vinfo:NULL);

	hr = E_FAIL;
	if(pProfile!=NULL) {
		hr = PF_SavePRX(pWMPM, pProfile, bstrGenPRX);
	}
	RELEASE(pWMPM);
	RELEASE(pProfile);
	
	return hr;
}

STDMETHODIMP CWMXUtility::GetWMXProfileInfo(BSTR bstrWMX, AUDIOPARAM *painfo, VIDEOPARAM *pvinfo, SCRIPTPARAM *psinfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=0;
	IWMProfileManager *pWMPM0=NULL;
	hr = WMCreateProfileManager(&pWMPM0);
	if ( FAILED( hr ) ) return hr;

	IWMProfileManager2 *pWMPM=NULL;
	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if ( FAILED( hr ) ) return hr;

	AUDIO_PARAMS	apm;
	VIDEO_PARAMS	vpm;
	SCRIPT_PARAMS	spm;

	BOOL fOK = WMXGetProfileParameter(pWMPM, bstrWMX, apm, vpm, spm);
	RELEASE(pWMPM);

	if(!fOK) return E_FAIL;
	
	if(painfo!=NULL) memcpy(painfo, &apm, sizeof(AUDIOPARAM));
	if(pvinfo!=NULL) memcpy(pvinfo, &vpm, sizeof(VIDEOPARAM));
	if(psinfo!=NULL) memcpy(psinfo, &spm, sizeof(SCRIPTPARAM));

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXConvert(BSTR bstrWMX, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXConvert	WMXConvert;
	if(IsWindow(m_hWndProgress)) WMXConvert.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXConvert.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);
	if(!WMXConvert.MakeConvert(bstrWMX, bstrProfile, fCopyHeader, fCopyScript, fCopyMarker, bstrOutput))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXCopy(BSTR bstrWMX, VARIANT_BOOL vbVideo, VARIANT_BOOL vbScript, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXCopy	WMXCopy;
	if(IsWindow(m_hWndProgress)) WMXCopy.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXCopy.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	BOOL fCopyHeader=(vbCopyHeader==OATRUE);
	BOOL fCopyScript=(vbCopyScript==OATRUE);
	BOOL fCopyMarker=(vbCopyMarker==OATRUE);	
	if(!WMXCopy.MakeCopy(bstrWMX, (vbVideo==OATRUE), (vbScript==OATRUE), fCopyHeader, fCopyScript, fCopyMarker, bstrOutput))
		return E_FAIL;

	return S_OK;
}

#include "WMXAVScript.h"
STDMETHODIMP CWMXUtility::SpecWMXMakeAVScript(BSTR bstrWMX, VARIANT_BOOL vbCopyHeader, VARIANT_BOOL vbCopyScript, VARIANT_BOOL vbCopyMarker, BSTR bstrOutWMX, BSTR bstrOutTable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWMXAVScript WMXAVScript;
	if(IsWindow(m_hWndProgress)) WMXAVScript.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(m_pbAbort!=NULL) WMXAVScript.SetupQueryAbort(m_hWndAbort, m_pbAbort);

	CStringArray CmdArray, TextArray;
	CDWordArray	 TimeArray;
	if(!WMXAVScript.MakeAVScript(bstrWMX, (vbCopyHeader==OATRUE), (vbCopyScript==OATRUE), (vbCopyMarker==OATRUE), bstrOutWMX, CmdArray, TextArray, TimeArray))
		return E_FAIL;

	if(!WriteScriptTable((LPSTR)(LPCSTR)CString(bstrOutTable), TextArray, TimeArray))
		return E_FAIL;	

	return S_OK;
}

#include <l_bitmap.h>
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))
STDMETHODIMP CWMXUtility::WMXFromImage(BSTR bstrImage, long lmsDuration, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT				hr=S_OK;
	BOOL				fOK=FALSE;
	WAVEFORMATEX		wfx;
	BITMAPINFOHEADER	bmpheader;
	BITMAPHANDLE		bmh;
	CString				cstImage(bstrImage);
	INSSBuffer			*pNssBuffer=NULL;
	BYTE				*pBuffer=NULL;
	WM_MEDIA_TYPE		*pwmt=NULL;
	CWriterBase			WriterBase;	

	if(!WriterBase.Create(bstrOutput, bstrProfile))
		return E_FAIL;

	pwmt = WriterBase.GetVideoInputFormat();
	if(pwmt==NULL) goto ERROR_CLOSE;

	memcpy(&bmpheader, &(((WMVIDEOINFOHEADER*)(pwmt->pbFormat))->bmiHeader), __min(pwmt->cbFormat, sizeof(bmpheader)));
	bmpheader.biSize = sizeof(bmpheader);
	bmpheader.biBitCount = 24;
	bmpheader.biPlanes = 1;
	bmpheader.biCompression = 0;
	bmpheader.biSizeImage = DIBSIZE(bmpheader);

	WriterBase.FreeFormatAllocated(pwmt);

	hr = L_LoadBitmap((LPSTR)(LPCSTR)cstImage, &bmh, bmpheader.biBitCount, ORDER_BGR, NULL);
	if(hr<1) goto ERROR_CLOSE;

	if((bmh.Width!=bmpheader.biWidth)||(bmh.Height!=bmpheader.biHeight)) {
		hr = L_SizeBitmap(&bmh, bmpheader.biWidth, bmpheader.biHeight, SIZE_RESAMPLE);
		if(hr<1) goto ERROR_FREEBMH;
	}

	if(!WriterBase.m_fVideoExisted)
		goto ERROR_FREEBMH;

	if(!WriterBase.ConfigAudioInputSimpleUncompressed(16, 1, 16000))
		goto ERROR_FREEBMH;

	pwmt = WriterBase.GetAudioInputFormat();
	if(pwmt==NULL) goto ERROR_FREEBMH;

	memcpy(&wfx, pwmt->pbFormat, __min(pwmt->cbFormat, sizeof(wfx)));
	ASSERT(wfx.wFormatTag==WAVE_FORMAT_PCM);
	
	WriterBase.FreeFormatAllocated(pwmt);

	if(!WriterBase.ConfigVideoInputSimpleUncompressed(bmpheader.biBitCount, 0))
		goto ERROR_FREEBMH;

	if(!WriterBase.BeginWriting())
		goto ERROR_FREEBMH;

	{
		for(long lmsTime=0; lmsTime<lmsDuration; lmsTime+=250) {		
			hr = WriterBase.m_pWriter->AllocateSample(bmh.Size, &pNssBuffer);
			if(pNssBuffer==NULL)
				goto ERROR_WRITING;

			pNssBuffer->GetBuffer(&pBuffer);
			UINT uOffset=0;
			for(UINT ii=0; ii<(UINT)bmh.Height; ii++) {
				if(bmh.ViewPerspective==BOTTOM_LEFT)
					hr = L_GetBitmapRow(&bmh, pBuffer + uOffset, ii, bmh.BytesPerLine);
				else
					hr = L_GetBitmapRow(&bmh, pBuffer + uOffset, bmh.Height-ii-1, bmh.BytesPerLine);				
				if(hr<1) {
					RELEASE(pNssBuffer);
					goto ERROR_WRITING;
				}
				uOffset += bmh.BytesPerLine;
			}

			hr = WriterBase.m_pWriter->WriteSample(WriterBase.m_dwInputVideo, (QWORD)lmsTime * 10000, (lmsTime==0)?WM_SF_CLEANPOINT|WM_SF_DISCONTINUITY:0, pNssBuffer);
			RELEASE(pNssBuffer);
			if(FAILED(hr))
				goto ERROR_WRITING;
		}

		UINT uCopySize=0, uTotalSize = lmsDuration * wfx.nAvgBytesPerSec / 1000;
		for(UINT ii=0; ii<uTotalSize; ii+=wfx.nAvgBytesPerSec) {
			uCopySize = uTotalSize - ii;
			if(uCopySize>wfx.nAvgBytesPerSec) uCopySize = wfx.nAvgBytesPerSec;
			uCopySize = uCopySize / wfx.nBlockAlign * wfx.nBlockAlign;

			hr = WriterBase.m_pWriter->AllocateSample(uCopySize, &pNssBuffer);
			if(pNssBuffer==NULL)
				goto ERROR_WRITING;

			pNssBuffer->GetBuffer(&pBuffer);
			if(wfx.wBitsPerSample==8) memset(pBuffer, 0, uCopySize);
			else memset(pBuffer, 0xFF, uCopySize);

			hr = WriterBase.m_pWriter->WriteSample(WriterBase.m_dwInputAudio, (QWORD)10000000L*ii/wfx.nAvgBytesPerSec, WM_SF_CLEANPOINT, pNssBuffer);
			RELEASE(pNssBuffer);
			if(FAILED(hr))
				goto ERROR_WRITING;			
		}
	}

	fOK = TRUE;
	
ERROR_WRITING:	
	WriterBase.EndWriting();
ERROR_FREEBMH:
	L_FreeBitmap(&bmh);
ERROR_CLOSE:
	WriterBase.Close();
		
	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXFromMuteAudio(long lmsDuration, BSTR bstrProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CWriterBase WriterBase;
	
	if(!WriterBase.Create(bstrOutput, bstrProfile))
		return E_FAIL;

	long ii=0;
	if(!WriterBase.ConfigAudioInputSimpleUncompressed(16, 1, 16000))
		goto ERROR_MUTEAUDIO;

	if(!WriterBase.BeginWriting())
		goto ERROR_MUTEAUDIO;

	for(ii=0; ii<lmsDuration; ii+=1000) {
		UINT uCopySize=0;

		uCopySize = (lmsDuration - ii) * 32000 / 1000;
		if(uCopySize > 32000) uCopySize = 32000;
		uCopySize = uCopySize / 2 * 2;
		
		INSSBuffer *pNssBuffer=NULL;		
		HRESULT hr = WriterBase.m_pWriter->AllocateSample(uCopySize, &pNssBuffer);
		if(SUCCEEDED(hr)) {
			LPBYTE pBuffer=NULL;
			hr = pNssBuffer->GetBuffer(&pBuffer);
			if(SUCCEEDED(hr)) {
				memset(pBuffer, 0x0, uCopySize);
				hr = WriterBase.m_pWriter->WriteSample(WriterBase.m_dwInputAudio, (QWORD)ii*10000, WM_SF_CLEANPOINT, pNssBuffer);
			}
			RELEASE(pNssBuffer);
		}
		if(FAILED(hr)) goto ERROR_MUTEWRITE;
	}

ERROR_MUTEWRITE:
	WriterBase.EndWriting();
ERROR_MUTEAUDIO:
	WriterBase.Close();
	return S_OK;
}

long GetMappingFormat(ENUMIMAGEFORMAT_KIT eFormat)
{
	long lLTFormat=FILE_BMP;
	switch(eFormat) {
	case IMAGEFORMAT_KIT_BMP: lLTFormat = FILE_BMP; break;
	case IMAGEFORMAT_KIT_JPG: lLTFormat = FILE_JFIF; break;
	case IMAGEFORMAT_KIT_PCX: lLTFormat = FILE_PCX; break;
	case IMAGEFORMAT_KIT_TIF: lLTFormat = FILE_TIF; break;
	case IMAGEFORMAT_KIT_PNG: lLTFormat = FILE_PNG; break;
	}

	return lLTFormat;
}

STDMETHODIMP CWMXUtility::WMXGetCoveredImages(BSTR bstrWMX, ENUMIMAGEFORMAT_KIT eFormat, long lmsTime, BSTR bstrPrevImage, BSTR bstrNextImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	CInfoReader cinforeader;
	if(!cinforeader.Open(bstrWMX)) return E_FAIL;

	long lLTFormat=GetMappingFormat(eFormat);

	CString cstPrev(bstrPrevImage), cstNext(bstrNextImage);
	BOOL fOK = cinforeader.GetCoveredImages(lLTFormat, lmsTime, (LPSTR)(LPCSTR)cstPrev, (LPSTR)(LPCSTR)cstNext);
	cinforeader.Close();
	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXGetNextImage(BSTR bstrWMX, ENUMIMAGEFORMAT_KIT eFormat, long lmsTime, BSTR bstrOutImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CInfoReader cinforeader;
	if(!cinforeader.Open(bstrWMX)) return E_FAIL;

	long lLTFormat=GetMappingFormat(eFormat);

	CString cst(bstrOutImage);
	BOOL fOK = cinforeader.GetNextImage(lLTFormat, lmsTime, (LPSTR)(LPCSTR)cst);
	cinforeader.Close();
	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMXGetPrevImage(BSTR bstrWMX, ENUMIMAGEFORMAT_KIT eFormat, long lmsTime, BSTR bstrOutImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CInfoReader cinforeader;
	if(!cinforeader.Open(bstrWMX)) return E_FAIL;

	long lLTFormat=GetMappingFormat(eFormat);

	CString cst(bstrOutImage);
	BOOL fOK = cinforeader.GetPrevImage(lLTFormat, lmsTime, (LPSTR)(LPCSTR)cst);
	cinforeader.Close();
	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::WMX_DBG_GetSampleCount(BSTR bstrWMX, long *plAudio, long *plVideo, long *plScript)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CInfoReader cInfoReader;

	if(!cInfoReader.Open(bstrWMX))
		return E_FAIL;

	BOOL fOK = cInfoReader.GetSampleCount(plAudio, plVideo, plScript);

	cInfoReader.Close();

	if(!fOK) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWMXUtility::MultiWMXDirectAppend(BSTR bstrWMXNameTable, BSTR bstrOutWMX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CStringArray CstArray;
	if(!ParseTable(bstrWMXNameTable, CstArray))
		return E_FAIL;

	HRESULT hr = 0;
	QWORD	qwOffset=0;

	CWriterBase cwriterbase;	
	
	for(int ii=0; ii<CstArray.GetSize(); ii++) {	
		hr = E_FAIL;

		CWMXDumper	cwmxdumper;
		CComBSTR	cbstrSource(CstArray.GetAt(ii));
		if(!cwmxdumper.Open(cbstrSource)) 
			break;

		cwmxdumper.SetSelectedStreams(TRUE, TRUE, TRUE);

		cwmxdumper.ConfigAudioOutputCompressed();
		cwmxdumper.ConfigVideoOutputCompressed();
		cwmxdumper.ConfigScriptOutputCompressed();
		
		if(ii==0) {
			if(!cwriterbase.Create(bstrOutWMX, cwmxdumper.m_pProfile))				
				break;

			cwriterbase.ConfigAudioInputCodecNoNeeded();
			cwriterbase.ConfigVideoInputCodecNoNeeded();
			cwriterbase.ConfigScriptInputCodecNoNeeded();

			if(!cwriterbase.BeginWriting())				
				break;
		}
		
		CComBSTR cbstrType("TEXT");
		CString cstFile = CstArray.GetAt(ii);
		int nPtr = cstFile.ReverseFind('\\');
		if(nPtr<0) {
			int nPtr = cstFile.ReverseFind(':');
			if(nPtr<0) nPtr = -1;
		}
		nPtr ++;
		cstFile = cstFile.Mid(nPtr, cstFile.GetLength()-nPtr);
		CComBSTR cbstrFile(cstFile);
		cwmxdumper.SetInitialScript(TRUE, cbstrType, cbstrFile);
		cwmxdumper.SetWriterBase(&cwriterbase);
		cwmxdumper.SetOffset(qwOffset);
		if(!cwmxdumper.Run_AutoMode(0, 0)) 
			break;

		QWORD qwAlignTime = cwmxdumper.GetAudioDefaultDuration();
		QWORD qwTime = cwmxdumper.GetTotalDeliverTime();
		if(qwAlignTime>0)
			qwTime = (qwTime + qwAlignTime - 1)/ qwAlignTime * qwAlignTime;
		qwOffset += qwTime;

		cwmxdumper.Close();

		hr = S_OK;
	}	
	
	cwriterbase.EndWriting();
	cwriterbase.Close();

	if(FAILED(hr)) return hr;

	return S_OK;
}
