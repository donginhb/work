// MediaConvert.cpp : Implementation of CMediaConvert
#include "stdafx.h"
#include "MediaFile.h"
#include "MediaConvert.h"
#include <streams.h>
#include <Dshowasf.h>
/////////////////////////////////////////////////////////////////////////////
// CMediaConvert
#include "HelperDShow.h"
#include "Media2WMX.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#define AUDIO_MIN_KILOSPACE		5
#define VIDEO_MIN_KILOSPACE		10

STDMETHODIMP CMediaConvert::SetupProgressHandler(long lwnd, long lMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_hWndProgress = (HWND)lwnd;
	if((m_hWndProgress!=NULL)&&!IsWindow(m_hWndProgress))
		m_hWndProgress = NULL;

	m_lMsgProgress = lMsg;

	return S_OK;
}

STDMETHODIMP CMediaConvert::SetupQueryAbort(long lWndAbortActive, long *lpbAbort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_hWndAbort = (HWND)lWndAbortActive;
	m_plAbort = lpbAbort;
	if((m_hWndAbort!=NULL)&&!IsWindow(m_hWndAbort)) {
		m_hWndAbort = NULL;
		m_plAbort = NULL;
	}

	return S_OK;
}

STDMETHODIMP CMediaConvert::MediaGetDuration(BSTR bstrMedia, double *pdmsDuration)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	double dLength=0;
	HRESULT hr = MediaFileGetDuration(bstrMedia, &dLength);
	if(FAILED(hr)) return hr;

	*pdmsDuration = dLength * 1000.0;

	return S_OK;
}

STDMETHODIMP CMediaConvert::Media2WMV_Param(BSTR bstrMedia, long lAudioKiloBPS, long lVideoKiloBPS, BSTR bstrOutWMX, BSTR bstrOutProfile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(lAudioKiloBPS<AUDIO_MIN_KILOSPACE) lAudioKiloBPS = AUDIO_MIN_KILOSPACE;
	if(lVideoKiloBPS<VIDEO_MIN_KILOSPACE) lVideoKiloBPS = VIDEO_MIN_KILOSPACE;

	CMedia2WMX	Media2WMX;
	if(IsWindow(m_hWndProgress)) Media2WMX.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(IsWindow(m_hWndAbort)) Media2WMX.SetupQueryAbort(m_hWndAbort, m_plAbort);

	if(!Media2WMX.Media2WMV_Param(bstrMedia, lAudioKiloBPS * 1000, lVideoKiloBPS * 1024, bstrOutWMX, bstrOutProfile)) 
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CMediaConvert::Media2WMV(BSTR bstrMedia, BSTR bstrProfile, BSTR bstrOutWMX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMedia2WMX	Media2WMX;
	if(IsWindow(m_hWndProgress)) Media2WMX.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(IsWindow(m_hWndAbort)) Media2WMX.SetupQueryAbort(m_hWndAbort, m_plAbort);

	if(!Media2WMX.Media2WMV(bstrMedia, bstrProfile, bstrOutWMX))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CMediaConvert::Media2WMA(BSTR bstrMedia, BSTR bstrProfile, BSTR bstrOutWMA)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMedia2WMX	Media2WMX;
	if(IsWindow(m_hWndProgress)) Media2WMX.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(IsWindow(m_hWndAbort)) Media2WMX.SetupQueryAbort(m_hWndAbort, m_plAbort);

	if(!Media2WMX.Media2WMA(bstrMedia, bstrProfile, bstrOutWMA))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CMediaConvert::Media2WMA_Param(BSTR bstrMedia, long lKiloBPS, BSTR bstrOutWMA, BSTR bstrOutProfile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMedia2WMX	Media2WMX;
	if(IsWindow(m_hWndProgress)) Media2WMX.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(IsWindow(m_hWndAbort)) Media2WMX.SetupQueryAbort(m_hWndAbort, m_plAbort);

	if(!Media2WMX.Media2WMA_Param(bstrMedia, lKiloBPS * 1000, bstrOutWMA, bstrOutProfile))
		return E_FAIL;

	return S_OK;
}

#include "HelperProfile.h"
#include "HelperKit.h"
BOOL GetMediaFileParameter(WCHAR *wchFile, BOOL *pfAudio, AUDIO_PARAMS &apm, BOOL *pfVideo, VIDEO_PARAMS &vpm);
STDMETHODIMP CMediaConvert::MediaGetParams(BSTR bstrMedia, MF_AUDIOPARAMS *pam, MF_VIDEOPARAMS *pvm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BOOL fAudio=FALSE, fVideo=FALSE;
	if(!GetMediaFileParameter(bstrMedia, &fAudio, *(AUDIO_PARAMS*)pam, &fVideo, *(VIDEO_PARAMS*)pvm))
		return E_FAIL;

	if(!fAudio) memset(pam, 0, sizeof(MF_AUDIOPARAMS));
	if(!fVideo) memset(pvm, 0, sizeof(MF_VIDEOPARAMS));

	return S_OK;
}

STDMETHODIMP CMediaConvert::WMV2WMV_MBR(BSTR bstrWMX, BSTR bstrMBRProfile, BSTR bstrOutput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMedia2WMX	Media2WMX;
	if(IsWindow(m_hWndProgress)) Media2WMX.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
	if(IsWindow(m_hWndAbort)) Media2WMX.SetupQueryAbort(m_hWndAbort, m_plAbort);
	
	if(!Media2WMX.WMV2WMV_MBR(bstrWMX, bstrMBRProfile, bstrOutput))
		return E_FAIL;
	
	return S_OK;
}
