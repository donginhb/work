// EasyCapture.cpp : CEasyCapture ‚ÌŽÀ‘•

#include "stdafx.h"
#include <dshow.h>
#include "EasyCapture.h"
#include "VideoCapture.h"

/////////////////////////////////////////////////////////////////////////////
// CEasyCapture

CEasyCapture::CEasyCapture()
{
	m_pVideoCapture = NULL;	
	m_eSwitch = ENUMSWITCH_NONE;
}

CEasyCapture::~CEasyCapture()
{
	_Shared_Close();
}

/////////////////////////////////////////////////////////////////////////////
// IEasyCapture Function 
STDMETHODIMP CEasyCapture::Open(long lReserved)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_Shared_Close();

	m_pVideoCapture = new CVideoCapture();	
	if((m_pVideoCapture==NULL)||!m_pVideoCapture->myOpen()) {
	 	_Shared_Close();
	 	return E_FAIL;
	}

	return S_OK;
}

void CEasyCapture::_Shared_Close()
{
	if(m_pVideoCapture!=NULL) {
		m_pVideoCapture->myDoStop();
		m_pVideoCapture->myClose();
		delete m_pVideoCapture;
		m_pVideoCapture = NULL;
	}
}

STDMETHODIMP CEasyCapture::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_Shared_Close();

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceTableUpdate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myDeviceTableUpdate()) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceTableGetCount(CAPTUREDEVICETYPE eType, long *plCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(plCount==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	*plCount = m_pVideoCapture->myDeviceTableGetCount((ENUMCAPTUREDEVICE)eType);

	if(*plCount<0) return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceTableGetName(CAPTUREDEVICETYPE eType, long lIndex, BSTR *pbstr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(pbstr==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	BSTR bstrName = m_pVideoCapture->myDeviceTableGetName((ENUMCAPTUREDEVICE)eType, lIndex);

	*pbstr = bstrName;
	if(bstrName==NULL) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceTableSearchName(CAPTUREDEVICETYPE eType, BSTR bstrName, long *plIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(plIndex==NULL) return FALSE;
	if(m_pVideoCapture==NULL) return E_FAIL;

	*plIndex = m_pVideoCapture->myDeviceTableSearchName((ENUMCAPTUREDEVICE)eType, bstrName);

	if(*plIndex<0) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceFormatTableGetCount(CAPTUREDEVICETYPE eType, long lDeviceIndex, long *plCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(plCount==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	*plCount = m_pVideoCapture->myDeviceFormatTableGetCount((ENUMCAPTUREDEVICE)eType, lDeviceIndex);

	if(*plCount<0) return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceFormatTableGetFormat(CAPTUREDEVICETYPE eType, long lDeviceIndex, long lFormatIndex, BSTR *pbstr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(pbstr==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	BSTR bstrFormat = m_pVideoCapture->myDeviceFormatTableGetFormat((ENUMCAPTUREDEVICE)eType, lDeviceIndex, lFormatIndex);

	*pbstr = bstrFormat;
	if(bstrFormat==NULL) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::VideoFrameRateTableGetCount(long lDeviceIndex, long *plCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(plCount==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	*plCount = m_pVideoCapture->myVideoFrameRateTableGetCount(lDeviceIndex);

	if(*plCount<0) return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CEasyCapture::VideoFrameRateTableGetFrameRate(long lDeviceIndex, long lFrameRateIndex, BSTR *pbstr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(pbstr==NULL) return E_FAIL;
	if(m_pVideoCapture==NULL) return E_FAIL;

	BSTR bstrFrameRate = m_pVideoCapture->myVideoFrameRateTableGetFrameRate(lDeviceIndex, lFrameRateIndex);

	*pbstr = bstrFrameRate;
	if(bstrFrameRate==NULL) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceOpen(CAPTURESESSION eSession, long lVideoIndex, long lAudioIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(m_pVideoCapture==NULL) return E_FAIL;

	if(!m_pVideoCapture->myDeviceOpen((ENUMSESSION)eSession, lVideoIndex, lAudioIndex))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::IPCameraOpen(BSTR bstrURL, BSTR bstrLogin, BSTR bstrPassword)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;

	if(!m_pVideoCapture->myIPCameraOpen(bstrURL, bstrLogin, bstrPassword))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DeviceClose()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	

	if(m_pVideoCapture==NULL) return E_FAIL;
	m_pVideoCapture->myDeviceClose();

	m_eSwitch = ENUMSWITCH_NONE;

	return S_OK;
}

STDMETHODIMP CEasyCapture::PreviewPrepare()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL)	return E_FAIL;
	if(!m_pVideoCapture->myPreviewPrepare()) 
		return E_FAIL;

	m_eSwitch = ENUMSWITCH_PREVIEW;
	return S_OK;
}

STDMETHODIMP CEasyCapture::CapturePrepare(BSTR bstrFile, BSTR bstrProfile, long lAudioTimeshift, VARIANT_BOOL vbSyncPreview)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myCapturePrepare(bstrFile, bstrProfile, lAudioTimeshift, (BOOL)vbSyncPreview)) 
		return E_FAIL;

	m_eSwitch = ENUMSWITCH_CAPTURE;
	return S_OK;
}

STDMETHODIMP CEasyCapture::DoStart()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())		

	if(m_pVideoCapture==NULL) return E_FAIL;

	m_pVideoCapture->myDoStop();
	
	if(!m_pVideoCapture->myDoStart()) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DoStop()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture!=NULL) 
		m_pVideoCapture->myDoStop();

	return S_OK;
}

STDMETHODIMP CEasyCapture::DoPause()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myDoPause()) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::DoResume()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myDoResume()) return E_FAIL;		

	return S_OK;
}

STDMETHODIMP CEasyCapture::DisplayOpen(long lwnd, long xx, long yy, long lWidth, long lHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(m_pVideoCapture==NULL)	return E_FAIL;

	if(!m_pVideoCapture->myDisplayOpen(xx, yy, lWidth, lHeight)) return E_FAIL;
	
	return S_OK;
}

STDMETHODIMP CEasyCapture::DisplayClose()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL)	return E_FAIL;

	m_pVideoCapture->myDisplayClose();
	
	return S_OK;
}

STDMETHODIMP CEasyCapture::SetupUserMsgHandler(long lWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->mySetupStatusHandler((HWND)lWnd))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::SendIDScript(BSTR bstrID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_eSwitch!=ENUMSWITCH_CAPTURE) return E_FAIL;

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->mySendScript(bstrID))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CEasyCapture::IsDigitalCamActive(VARIANT_BOOL *pvb)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pvb = OATRUE;

	return S_OK;
}

STDMETHODIMP CEasyCapture::GetRecordVolumeLevel(long *plLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myGetRecordVolumeLevel(plLevel))
		return E_FAIL;
		
	return S_OK;
}

STDMETHODIMP CEasyCapture::GetCurrentCount(long *plCurrentCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_pVideoCapture==NULL) return E_FAIL;
	if(!m_pVideoCapture->myGetCurrentCount(plCurrentCount))
		return E_FAIL;
		
	return S_OK;
}
