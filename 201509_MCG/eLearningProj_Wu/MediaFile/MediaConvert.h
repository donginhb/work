// MediaConvert.h : Declaration of the CMediaConvert

#ifndef __MEDIACONVERT_H_
#define __MEDIACONVERT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMediaConvert
class ATL_NO_VTABLE CMediaConvert : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMediaConvert, &CLSID_MediaConvert>,
	public IDispatchImpl<IMediaConvert, &IID_IMediaConvert, &LIBID_MEDIAFILELib>
{
private:
	long			*m_plAbort;
	HWND			m_hWndProgress, m_hWndAbort;
	long			m_lMsgProgress;
public:
	CMediaConvert()
	{
		m_hWndProgress = NULL;
		m_hWndAbort = NULL;
		m_plAbort = NULL;
		m_lMsgProgress = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MEDIACONVERT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMediaConvert)
	COM_INTERFACE_ENTRY(IMediaConvert)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMediaConvert
public:
	STDMETHOD(WMV2WMV_MBR)(/*[in]*/BSTR bstrMedia, /*[in]*/BSTR bstrMBRProfile,  /*[in]*/BSTR bstrOutput);
	STDMETHOD(MediaGetParams)(/*[in]*/BSTR bstrMedia, /*[out]*/MF_AUDIOPARAMS *pam, /*[out]*/MF_VIDEOPARAMS *pvm);
	STDMETHOD(Media2WMA_Param)(/*[in]*/BSTR bstrMedia, /*[in]*/long lKiloBPS, /*[in]*/BSTR bstrOutWMA, /*[in]*/BSTR bstrOutProfile);
	STDMETHOD(Media2WMA)(/*[in]*/BSTR bstrMedia, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutWMA);
	STDMETHOD(Media2WMV)(/*[in]*/BSTR bstrMedia, /*[in]*/BSTR bstrProfile, /*[in]*/BSTR bstrOutWMX);
	STDMETHOD(Media2WMV_Param)(/*[in]*/BSTR bstrMedia, /*[in]*/long lAudioKiloBPS, /*[in]*/long lVideoKiloBPS, /*[in]*/BSTR bstrOutWMX, /*[in]*/BSTR bstrOutProfile);
	STDMETHOD(MediaGetDuration)(/*[in]*/BSTR bstrMedia, /*[out]*/double *pdmsDuration);
	STDMETHOD(SetupQueryAbort)(/*[in]*/long lWndAbortActive, /*[in]*/long *lpbAbort);
	STDMETHOD(SetupProgressHandler)(/*[in]*/long lwnd, /*[in]*/long lMsg);
};

#endif //__MEDIACONVERT_H_
