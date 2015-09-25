#define DEV_NOT_SET		2
#define DEV_NOT_MATCH	1
#define DEV_ERROR		-1
#define DEV_MATCH		0
#define USER_ABORT		3
#define AUDIO_ONLY		4

#include "..\ComCapture\ComCapture\ComCapture_i.h"
void WINAPI GetDevice(CString& strType, CString& strParam1, CString& strParam2, CString strCourse ="");
int WINAPI CheckDeviceValid(CString strType, CString strParam1, CString strParam2);
int WINAPI IsDeviceValid(CString strType, CString strParam1, CString strParam2);
BOOL WINAPI EnumDevice(IEasyCapture* pEasyCapture, 
					   CAPTUREDEVICETYPE eType, 
					   CStringArray& strArray);
BOOL WINAPI EnumDeviceFrame(IEasyCapture* pEasyCapture, 
	CAPTUREDEVICETYPE eType, 
	long index,
	CStringArray& strArray);
int WINAPI CheckAndInputDev(CString strType, CString strParam1, CString strParam2);
BOOL WINAPI EnumDeviceCamera(IEasyCapture* pEasyCapture,  
	long m_CameraIndex,
	CString& strArray);
int WINAPI FindDeviceValid(CAPTUREDEVICETYPE eType, CString DeviceName);
int WINAPI FindDeviceFrameValid(CAPTUREDEVICETYPE eType,int DeviceNumber, CString DeviceNameFrame, CStringArray& strArray);
int WINAPI CheckAndInputDevSCreen(CString strType, CString strParam1, CString strParam2);
int WINAPI CheckAndInputDevValid(CString strType, CString strParam1, CString strParam2);