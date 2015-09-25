// VisioForgeCapture.h : Declaration of the CVisioForgeCapture
#ifndef __VISIOFORGECAPTURE_H_
#define __VISIOFORGECAPTURE_H_

#pragma once

#include "Common.h"

#import ".\..\..\eLearningTitle.JP\Bin\VFVideoCapture.tlb" raw_interfaces_only

using namespace VFVideoCapture;

class CVideoCapture
{
private:
	IVFVideoCapture			*m_pVideoCapture;
	char					 m_pCharArrayBuffer[100];
	long					 m_pBSTRBuffer;
    long					 m_lLimit;
	BSTR					 m_pBSTR;
	HWND					 m_hwndStatusReport;
	BOOL					 m_fCapturing;
public:
	CVideoCapture(void);
	~CVideoCapture(void);
public:
	BOOL myOpen();
	void myClose();
	BOOL myDeviceTableUpdate();
	long myDeviceTableGetCount(ENUMCAPTUREDEVICE eType);
	BSTR myDeviceTableGetName(ENUMCAPTUREDEVICE eType, long lIndex);
	long myDeviceTableSearchName(ENUMCAPTUREDEVICE eType, BSTR bstrName);
	long myDeviceFormatTableGetCount(ENUMCAPTUREDEVICE eType, long lDeviceIndex);
	BSTR myDeviceFormatTableGetFormat(ENUMCAPTUREDEVICE eType, long lDeviceIndex, long lFormatIndex);
	long myVideoFrameRateTableGetCount(long lDeviceIndex);
	BSTR myVideoFrameRateTableGetFrameRate(long lDeviceIndex, long lFrameRateIndex);
	BOOL myDeviceOpen(ENUMSESSION eSession, long lIndexVideoDevice, long lIndexAudioDevice);
	BOOL myIPCameraOpen(BSTR bstrURL, BSTR bstrLogin, BSTR bstrPassword);
	BOOL myDeviceClose();
	BOOL myPreviewPrepare();
	BOOL myCapturePrepare(BSTR bstrFile, BSTR bstrProfile, long lAudioTimeshift, BOOL fSyncView);
	BOOL myDoStart();
	BOOL myDoStop();
	BOOL myDoPause();
	BOOL myDoResume();
	BOOL myDisplayOpen(long xx, long yy, long lWidth, long lHeight);
	BOOL myDisplayClose();
	BOOL mySetupStatusHandler(HWND hwnd);
	BOOL mySendScript(BSTR bstr);
	BOOL myGetRecordVolumeLevel(long *plLevel);
	BOOL myGetCurrentCount(long *plCurrentCount);
};

#endif //__VISIOFORGECAPTURE_H_
