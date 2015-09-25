#include "StdAfx.h"
#include "VideoCapture.h"
#include "UserErrMsg.h"


CVideoCapture::CVideoCapture(void)
{
	m_pVideoCapture = NULL;
	m_hwndStatusReport = NULL;
	m_fCapturing = FALSE;	
}


CVideoCapture::~CVideoCapture(void)
{
	myClose();
}

BOOL CVideoCapture::myOpen()
{
	long lResult;
	HRESULT hval=0;

	myClose();

	hval = CoInitialize(NULL);
	if(FAILED(hval)) return FALSE;

	hval = CoCreateInstance(__uuidof(VFVideoCaptureClass), NULL, CLSCTX_INPROC_SERVER,
	                        __uuidof(IVFVideoCapture),
							reinterpret_cast<void**>(&m_pVideoCapture));
	if(FAILED(hval)) return FALSE;	

    m_pVideoCapture->Capture_Open(&lResult);
	if(lResult) return FALSE;

	m_pBSTRBuffer = (long)m_pCharArrayBuffer;
	m_lLimit = sizeof(m_pCharArrayBuffer);
	m_pBSTR = (BSTR)m_pCharArrayBuffer;
	return TRUE;
}

void CVideoCapture::myClose()
{
	long lResult;

	if(m_pVideoCapture!=NULL) {
		m_pVideoCapture->Capture_Close(&lResult);
		m_pVideoCapture = NULL;
		m_pBSTRBuffer = NULL;
		m_lLimit = 0;
		m_pBSTR = NULL;
		m_fCapturing = FALSE;
		CoUninitialize();
	}
}

BOOL CVideoCapture::myDoStart()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DoStart(&lResult);
	if(lResult) return FALSE;

	if(m_fCapturing) {
		if(IsWindow(m_hwndStatusReport))
			SendMessage(m_hwndStatusReport, WM_USER_CAPTURE_STATUS, MSGPARAM_CAPTURE_TRYSTART, 0);
	}

	return TRUE;
}

BOOL CVideoCapture::myDoStop()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	if(m_fCapturing) {
		m_pVideoCapture->Capture_DoStop(&lResult);
		if(lResult) return FALSE;
	}

	return TRUE;
}

BOOL CVideoCapture::myDoPause()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	if(m_fCapturing) {
		m_pVideoCapture->Capture_DoPause(&lResult);
		if(lResult) return FALSE;
	}

	return TRUE;
}

BOOL CVideoCapture::myDoResume()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	if(m_fCapturing) {
		m_pVideoCapture->Capture_DoResume(&lResult);
		if(lResult) return FALSE;
	}

	return TRUE;
}

BOOL CVideoCapture::myDeviceTableUpdate()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DeviceTableUpdate(&lResult);
	if(lResult) return FALSE;

	return TRUE;
}

long CVideoCapture::myDeviceTableGetCount(ENUMCAPTUREDEVICE eType)
{
	long count;

	if(m_pVideoCapture==NULL) return -1;

	m_pVideoCapture->Capture_DeviceTableGetCount(eType, &count);

	return count;
}

BSTR CVideoCapture::myDeviceTableGetName(ENUMCAPTUREDEVICE eType, long lIndex)
{
	long size;
	BSTR deviceName = NULL;

	if(lIndex<0) return NULL;
	if(m_pVideoCapture==NULL) return NULL;

	memset(m_pCharArrayBuffer, 0, sizeof(m_pCharArrayBuffer));
	m_pVideoCapture->Capture_DeviceTableGetName(eType, lIndex, m_pBSTRBuffer, m_lLimit, &size);

	if(size<=0) return NULL;
	deviceName = SysAllocString(m_pBSTR);

	return deviceName;
}

long CVideoCapture::myDeviceTableSearchName(ENUMCAPTUREDEVICE eType, BSTR bstrName)
{
	long index, size;

	if(bstrName==NULL) return -1;
	if(m_pVideoCapture==NULL) return -1;

	size = SysStringLen(bstrName);
	m_pVideoCapture->Capture_DeviceTableSearchName(eType, (long)bstrName, size, &index);

	return index;
}

long CVideoCapture::myDeviceFormatTableGetCount(ENUMCAPTUREDEVICE eType, long lDeviceIndex)
{
	long count;

	if(lDeviceIndex<0) return -1;
	if(m_pVideoCapture==NULL) return -1;

	m_pVideoCapture->Capture_DeviceFormatTableGetCount(eType, lDeviceIndex, &count);

	return count;
}

BSTR CVideoCapture::myDeviceFormatTableGetFormat(ENUMCAPTUREDEVICE eType, long lDeviceIndex, long lFormatIndex)
{
	long size;
	BSTR format = NULL;

	if(lDeviceIndex<0) return NULL;
	if(lFormatIndex<0) return NULL;
	if(m_pVideoCapture==NULL) return NULL;

	memset(m_pCharArrayBuffer, 0, sizeof(m_pCharArrayBuffer));
	m_pVideoCapture->Capture_DeviceFormatTableGetFormat(eType, lDeviceIndex, lFormatIndex, m_pBSTRBuffer, m_lLimit, &size);

	if(size<=0) return NULL;
	format = SysAllocString(m_pBSTR);

	return format;
}

long CVideoCapture::myVideoFrameRateTableGetCount(long lDeviceIndex)
{
	long count;

	if(lDeviceIndex<0) return -1;
	if(m_pVideoCapture==NULL) return -1;

	m_pVideoCapture->Capture_VideoFrameRateTableGetCount(lDeviceIndex, &count);

	return count;
}

BSTR CVideoCapture::myVideoFrameRateTableGetFrameRate(long lDeviceIndex, long lFrameRateIndex)
{
	long size;
	BSTR frameRate = NULL;

	if(lDeviceIndex<0) return NULL;
	if(lFrameRateIndex<0) return NULL;
	if(m_pVideoCapture==NULL) return NULL;

	memset(m_pCharArrayBuffer, 0, sizeof(m_pCharArrayBuffer));
	m_pVideoCapture->Capture_VideoFrameRateTableGetFrameRate(lDeviceIndex, lFrameRateIndex, m_pBSTRBuffer, m_lLimit, &size);

	if(size<=0) return NULL;
	frameRate = SysAllocString(m_pBSTR);

	return frameRate;
}

BOOL CVideoCapture::myDeviceOpen(ENUMSESSION eSession, long lIndexVideoDevice, long lIndexAudioDevice)
{
	long lResult;

	if(lIndexVideoDevice<0) return FALSE;
	if(lIndexAudioDevice<0) return FALSE;
	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DeviceOpen(eSession, lIndexVideoDevice, lIndexAudioDevice, &lResult);
	if(lResult) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::myIPCameraOpen(BSTR bstrURL, BSTR bstrLogin, BSTR bstrPassword)
{
	long lResult;
	long urlSize, loginSize, passwordSize;

	if(bstrURL==NULL) return FALSE;
	if(m_pVideoCapture==NULL) return FALSE;

	urlSize = SysStringLen(bstrURL);
	loginSize = SysStringLen(bstrLogin);
	passwordSize = SysStringLen(bstrPassword);
	m_pVideoCapture->Capture_IPCameraOpen((long)bstrURL, urlSize,
		                                       (long)bstrLogin, loginSize,
											   (long)bstrPassword, passwordSize, &lResult);
	if(lResult) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::myDeviceClose()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DeviceClose(&lResult);
	if(lResult) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::myPreviewPrepare()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_PreviewPrepare(&lResult);
	if(lResult) return FALSE;

	m_fCapturing = FALSE;

	return TRUE;
}

BOOL CVideoCapture::myCapturePrepare(BSTR bstrFile, BSTR bstrProfile, long lAudioTimeshift, BOOL fSyncView)
{
	long lResult;
	long fileSize, profileSize;

	if(bstrFile==NULL) return FALSE;
	if(bstrProfile==NULL) return FALSE;
	if(m_pVideoCapture==NULL) return FALSE;

	fileSize = SysStringLen(bstrFile);
	profileSize = SysStringLen(bstrProfile);
	m_pVideoCapture->Capture_CapturePrepare((long)bstrFile, fileSize,
		                                         (long)bstrProfile, profileSize, lAudioTimeshift,
												 (long)fSyncView, &lResult);
	if(lResult) return FALSE;

	m_fCapturing = TRUE;

	return TRUE;
}

BOOL CVideoCapture::myDisplayOpen(long xx, long yy, long lWidth, long lHeight)
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DisplayOpen(xx, yy, lWidth, lHeight, &lResult);
	if(lResult) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::myDisplayClose()
{
	long lResult;

	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_DeviceClose(&lResult);
	if(lResult) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::mySendScript(BSTR bstr)
{
	if(m_pVideoCapture==NULL) return FALSE;
	if(!m_fCapturing) return FALSE;

	return TRUE;
}

BOOL CVideoCapture::mySetupStatusHandler(HWND hwnd)
{
	m_hwndStatusReport = hwnd;

	return TRUE;
}

BOOL CVideoCapture::myGetRecordVolumeLevel(long *plLevel)
{
	long level;

	if(plLevel==NULL) return FALSE;
	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_GetRecordVolumeLevel(&level);
	*plLevel = level;

	return TRUE;
}

BOOL CVideoCapture::myGetCurrentCount(long *plCurrentCount)
{
	long currentCount;

	if(plCurrentCount==NULL) return FALSE;
	if(m_pVideoCapture==NULL) return FALSE;

	m_pVideoCapture->Capture_GetCount(&currentCount);
	*plCurrentCount = currentCount;

	return TRUE;
}
