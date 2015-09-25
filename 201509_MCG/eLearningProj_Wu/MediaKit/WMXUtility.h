// WMXUtility.h : Declaration of the CWMXUtility

#ifndef __WMXUTILITY_H_
#define __WMXUTILITY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWMXUtility
class ATL_NO_VTABLE CWMXUtility : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWMXUtility, &CLSID_WMXUtility>,
	public IDispatchImpl<IWMXUtility, &IID_IWMXUtility, &LIBID_MEDIAKITLib>
{
private:
	HWND	m_hWndProgress;
	HWND	m_hWndAbort;
	long	*m_pbAbort;
	long	m_lMsgProgress;
public:
	CWMXUtility()
	{
		m_hWndProgress = NULL;
		m_hWndAbort = NULL;
		m_pbAbort = NULL;
		m_lMsgProgress = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WMXUTILITY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWMXUtility)
	COM_INTERFACE_ENTRY(IWMXUtility)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWMXUtility
public:
	STDMETHOD(WMXAudioMixReplace2Wav)(/*[in]*/BSTR bstrWMX, /*[in]*/BSTR bstrReplaceList, /*[in]*/BSTR bstrMixList, /*[in]*/long lBitsPerSample, /*[in]*/long lChannels, /*[in]*/long lSampleRate, /*[in]*/BSTR  bstrOutputWav);
	STDMETHOD(WMXReplaceAudio_Wav)(/*[in]*/BSTR bstrFile, /*[in]*/IUnknown *pManager,  /*[in]*/BSTR bstrReplace, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXExport)(/*[in]*/BSTR bstrList, /*[in]*/IUnknown *pAudioManager,  /*[in]*/IUnknown *pVideoManager, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXConvertTrim)(/*[in]*/BSTR bstrWMX, /*[in]*/long lmsStart, /*[in]*/long lmsStop, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXDirectTrim)(/*[in]*/BSTR bstrWMX, /*[in]*/long lmsStart, /*[in]*/long lmsStop, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXAudio2Wav)(/*[in]*/BSTR bstrWMX, /*[in]*/long lmsStart, /*[in]*/long lmsStop, /*[in]*/long lBitsPerSample, /*[in]*/long lChannels, /*[in]*/long lSampleRate, /*[in]*/BSTR  bstrOutWav);
	STDMETHOD(MultiWMXOutput)(/*[in]*/BSTR bstrMain, /*[in]*/BSTR bstrAudioReplace, /*[in]*/BSTR bstrAudioMix,  /*[in]*/IUnknown *pManager,  /*[in]*/BSTR bstrProfile,  /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXDirectAppend)(/*[in]*/BSTR bstrWMXNameTable, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrOutWMX);
	STDMETHOD(WMX_DBG_GetSampleCount)(/*[in]*/BSTR bstrWMX, /*[out]*/long *plAudio, /*[out]*/long *plVideo, /*[out]*/long *plScript);
	STDMETHOD(WMXGetPrevImage)(/*[in]*/BSTR bstrWMX, /*[in]*/ENUMIMAGEFORMAT_KIT eFormat, /*[in]*/long lmsTime, /*[in]*/BSTR bstrOutImage);
	STDMETHOD(WMXGetNextImage)(/*[in]*/BSTR bstrWMX, /*[in]*/ENUMIMAGEFORMAT_KIT eFormat, /*[in]*/long lmsTime, /*[in]*/BSTR bstrOutImage);
	STDMETHOD(WMXGetCoveredImages)(/*[in]*/BSTR bstrWMX, /*[in]*/ENUMIMAGEFORMAT_KIT eFormat, /*[in]*/long lmsTime, /*[in]*/BSTR bstrPrevImage, /*[in]*/BSTR bstrNextImage);
	STDMETHOD(WMXFromMuteAudio)(/*[in]*/long lmsDuration, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXFromImage)(/*[in]*/BSTR bstrImage, /*[in]*/long lmsDuration, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXMixer2Wav)(/*[in]*/BSTR bstrWav, /*[in]*/BSTR bstrLists, /*[in]*/BSTR bstrOutWav);
	STDMETHOD(SpecWMXMakeAVScript)(/*[in]*/BSTR bstrWMX, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrOutWMX, /*[in]*/BSTR bstrOutTable);
	STDMETHOD(SpecWMXDumpScript)(/*[in]*/BSTR bstrWMX, /*[in]*/BSTR bstrOutTable);
	STDMETHOD(WMXCopy)(/*[in]*/BSTR bstrWMX, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXConvert)(/*[in]*/BSTR bstrWMX, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXAudioFromWav)(/*[in]*/BSTR bstrWMX, /*[in]*/BSTR bstrWav, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXAudioReplace)(/*[in]*/BSTR bstrFile, /*[in]*/BSTR bstrReplace, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXAudioNormalize)(/*[in]*/BSTR bstrWMX, /*[in]*/long lPercentage_100, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXAudioMultiply)(/*[in]*/BSTR bstrWMX, /*[in]*/long lRatio_100, /*[in]*/VARIANT_BOOL vbCopyHeader, /*[in]*/VARIANT_BOOL vbCopyScript, /*[in]*/VARIANT_BOOL vbCopyMarker, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(WMXMakeIndex)(/*[in]*/BSTR bstrWMX);
	STDMETHOD(PRXGen)(/*[in]*/PROFILEPARAMS *pParam, /*[in]*/BSTR bstrGenPRX);
	STDMETHOD(PRXGetParams)(/*[in]*/BSTR bstrPRX, /*[in]*/PROFILEPARAMS *pParam);
	STDMETHOD(PRXLower)(/*[in]*/BSTR bstrPRX, /*[in]*/VARIANT_BOOL vbAudioLower, /*[in]*/long lAudioSampleRate, /*[in]*/long lAudioChannels, /*[in]*/long lAudioKiloBPS, /*[in]*/VARIANT_BOOL vbVideoLower, /*[in]*/long lVideoSecPerKey, /*[in]*/long lVideoQuality, /*[in]*/long lVideoFPS, /*[in]*/long lVideoKiloBPS, /*[in]*/VARIANT_BOOL vbScriptLower, /*[in]*/long lScriptKiloBPS, /*[in]*/BSTR bstrOutPRX);
	STDMETHOD(PRXSubtract)(/*[in]*/BSTR bstrPRX1, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrPRX2);
	STDMETHOD(PRXFromWMX)(/*[in]*/BSTR bstrWMX, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/long lFPS, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrOutPrx);
	STDMETHOD(GetWMXProfileInfo)(/*[in]*/BSTR bstrWMX, /*[out]*/AUDIOPARAM *painfo, /*[out]*/VIDEOPARAM *pvinfo, /*[in]*/SCRIPTPARAM *psinfo);
	STDMETHOD(GetWMXDuration)(/*[in]*/BSTR bstrWMX, /*[out]*/double *pdmsLength);
	STDMETHOD(GetWMXAudioPeak)(/*[in]*/BSTR bstrWMX, /*[out]*/long *plPeakMin, /*[out]*/long *plPeakMax, /*[out]*/long *plMin, /*[out]*/long *plMax);
	STDMETHOD(IsWMXProfileCompatible)(/*[in]*/BSTR bstrWMX1, /*[in]*/BSTR bstrWMX2);
	STDMETHOD(IsWMXProfileStreamCompatible)(/*[in]*/BSTR bstrWMX1, /*[in]*/BSTR bstrWMX2, /*[out]*/VARIANT_BOOL *pvbAudio, /*[out]*/VARIANT_BOOL *pvbVideo, /*[out]*/VARIANT_BOOL *pvbScript);
	STDMETHOD(IsWMXIndexExisted)(/*[in]*/BSTR bstrWMX, /*[out]*/VARIANT_BOOL *pvbExisted);
	STDMETHOD(IsWMXProtected)(/*[in]*/BSTR bstrWMX, /*[out]*/VARIANT_BOOL *pvbProtected);
	STDMETHOD(WavFromWMX)(/*[in]*/BSTR bstrWMX, /*[in]*/long lBitsPerSample, /*[in]*/long lChannels, /*[in]*/long lSampleRate, /*[in]*/BSTR bstrOutput);
	STDMETHOD(Wav2WMX)(/*[in]*/BSTR bstrWav, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXCopyWav)(/*[in]*/BSTR bstrList, /*[in]*/BSTR bstrWav, /*[in]*/BSTR bstrOrgProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXDirectCopy)(/*[in]*/BSTR bstrList, /*[in]*/VARIANT_BOOL vbVideo, /*[in]*/VARIANT_BOOL vbScript, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXAudioFromWav)(/*[in]*/BSTR bstrWMXList, /*[in]*/IUnknown *pManager, /*[in]*/BSTR bstrWave, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutput);
	STDMETHOD(MultiWMXAudio2Wav)(/*[in]*/BSTR bstrAudioList, /*[in]*/IUnknown *pManager, /*[in]*/BSTR bstrMixerTimers, /*[in]*/long lBitsPerSample, /*[in]*/long lChannels, /*[in]*/long lSampleRate, /*[in]*/BSTR bstrOutput);
	STDMETHOD(SetupQueryAbort)(/*[in]*/long lWndAbortActive, /*[in]*/long *lpbAbort);
	STDMETHOD(SetupProgressHandler)(/*[in]*/long lWnd, /*[in]*/long lMsg);
	STDMETHOD(MBRPrxGen)(long lA1kbps, long lV1kbps, long lwV1, long lhV1, long lA2kbps, long lV2kbps, long lwV2, long lhV2, long lA3kbps, long lV3kbps, long lwV3, long lhV3, BSTR bstrPrxOut);
};

#endif //__WMXUTILITY_H_
