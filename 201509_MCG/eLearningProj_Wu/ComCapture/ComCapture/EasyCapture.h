// EasyCapture.h : CEasyCapture �̐錾

#ifndef __EASYCAPTURE_H_
#define __EASYCAPTURE_H_

#pragma once
#include "resource.h"       // ���C�� �V���{��



#include "ComCapture_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM �̊��S�T�|�[�g���܂�ł��Ȃ� Windows Mobile �v���b�g�t�H�[���̂悤�� Windows CE �v���b�g�t�H�[���ł́A�P��X���b�h COM �I�u�W�F�N�g�͐������T�|�[�g����Ă��܂���BATL ���P��X���b�h COM �I�u�W�F�N�g�̍쐬���T�|�[�g���邱�ƁA����т��̒P��X���b�h COM �I�u�W�F�N�g�̎����̎g�p�������邱�Ƃ���������ɂ́A_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ���`���Ă��������B���g�p�� rgs �t�@�C���̃X���b�h ���f���� 'Free' �ɐݒ肳��Ă���ADCOM Windows CE �ȊO�̃v���b�g�t�H�[���ŃT�|�[�g�����B��̃X���b�h ���f���Ɛݒ肳��Ă��܂����B"
#endif

using namespace ATL;


/////////////////////////////////////////////////////////////////////////////
// CEasyCapture

typedef enum {
	ENUMSWITCH_NONE=0,
	ENUMSWITCH_PREVIEW=1,
	ENUMSWITCH_CAPTURE=2
} ENUMSWITCH;

class CVideoCapture;

class ATL_NO_VTABLE CEasyCapture :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEasyCapture, &CLSID_EasyCapture>,
	public IDispatchImpl<IEasyCapture, &IID_IEasyCapture, &LIBID_ComCaptureLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	ENUMSWITCH			 m_eSwitch;
	CVideoCapture		*m_pVideoCapture;
public:
	CEasyCapture();
	~CEasyCapture();
private:
	void _Shared_Close();

public:
DECLARE_REGISTRY_RESOURCEID(IDR_EASYCAPTURE)


BEGIN_COM_MAP(CEasyCapture)
	COM_INTERFACE_ENTRY(IEasyCapture)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

// IEasyCapture Methods
public:
	STDMETHOD(GetCurrentCount)(/*[out]*/long *plCurrentCount);
	STDMETHOD(GetRecordVolumeLevel)(/*[out]*/long *plLevel);
	STDMETHOD(IsDigitalCamActive)(/*[out]*/VARIANT_BOOL *pvb);
	STDMETHOD(SendIDScript)(/*[in]*/BSTR bstrID);
	STDMETHOD(SetupUserMsgHandler)(/*[in]*/long lWnd);
	STDMETHOD(DisplayClose)();
	STDMETHOD(DisplayOpen)(/*[in]*/long lwnd, /*[in]*/long xx, /*[in]*/long yy, /*[in]*/long lWidth, /*[in]*/long lHeight);
	STDMETHOD(DoResume)();
	STDMETHOD(DoPause)();
	STDMETHOD(DoStop)();
	STDMETHOD(DoStart)();		
	STDMETHOD(CapturePrepare)(/*[in]*/BSTR bstrFile, /*[in]*/BSTR bstrProfile, /*[in]*/long lAudioTimeshift, /*[in]*/VARIANT_BOOL vbSyncPreview);
	STDMETHOD(PreviewPrepare)();
	STDMETHOD(DeviceTableSearchName)(/*[in]*/CAPTUREDEVICETYPE eType, /*[in]*/BSTR bstrName, /*[out]*/long *plIndex);
	STDMETHOD(DeviceClose)();
	STDMETHOD(DeviceOpen)(/*[in]*/CAPTURESESSION eSession, /*[in]*/long lVideoIndex, /*[in]*/long lAudioIndex);	
	STDMETHOD(IPCameraOpen)(/*[in]*/BSTR bstrURL, /*[in]*/BSTR bstrLogin, /*[in]*/BSTR bstrPassword);
	STDMETHOD(DeviceTableGetName)(/*[in]*/CAPTUREDEVICETYPE eType, /*[in]*/long lIndex, /*[out]*/BSTR *pbstr);
	STDMETHOD(DeviceTableGetCount)(/*[in]*/CAPTUREDEVICETYPE eType, /*[out]*/long *plCount);
	STDMETHOD(DeviceFormatTableGetCount)(/*[in]*/CAPTUREDEVICETYPE eType, /*[in]*/long lDeviceIndex, /*[out]*/long *plCount);
	STDMETHOD(DeviceFormatTableGetFormat)(/*[in]*/CAPTUREDEVICETYPE eType, /*[in]*/long lDeviceIndex, /*[in]*/long lFormatIndex, /*[out]*/BSTR *pbstr);
	STDMETHOD(VideoFrameRateTableGetCount)(/*[in]*/long lDeviceIndex, /*[out]*/long *plCount);
	STDMETHOD(VideoFrameRateTableGetFrameRate)(/*[in]*/long lDeviceIndex, /*[in]*/long lFrameRateIndex, /*[out]*/BSTR *pbstr);
	STDMETHOD(DeviceTableUpdate)();
	STDMETHOD(Close)();
	STDMETHOD(Open)(/*[in]*/long eMode);
};

OBJECT_ENTRY_AUTO(__uuidof(EasyCapture), CEasyCapture)

#endif //__EASYCAPTURE_H_
