#include "stdafx.h"
#include "elearning.h"
#include "macro.h"
#include "cmkapi.h"

#include "CaptureDevApi.h"
#include "Device.h"
#include "DlgDevNotInstalled.h"
#include <dshow.h>

extern "C" {
#include "..\ComCapture\ComCapture\ComCapture_i.c"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifndef OATRUE
//#define OATRUE (-1)
//#endif // OATRUE
//#ifndef OAFALSE
//#define OAFALSE (0)
//#endif // OAFALSE

#ifndef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
#endif

void WINAPI GetDevice(CString& strType, CString& strParam1, CString& strParam2, CString strCourse )
{
	CString strIniPath;
	if (strCourse != "")
		strIniPath = MACRO_INI_FILE(strCourse);
	else
		strIniPath = MACRO_INI_PATH;

	char szValue[256];
	::GetPrivateProfileString("Device", "Type", "", szValue, 255, strIniPath);
	strType = szValue;
	if (strType == "DV")
	{
		::GetPrivateProfileString("Device", "Video", "", szValue, 255, strIniPath);
		strParam1 = szValue;
	}
	else if (strType == "CAM")
	{
		::GetPrivateProfileString("Device", "Video", "", szValue, 255, strIniPath);
		strParam1 = szValue;
	}

	::GetPrivateProfileString("Device", "Audio", "", szValue, 255, strIniPath);
	strParam2 = szValue;
}

//return -1 if Error; 
//return 0 if Success; 
//return 1 if Device not set correctly;
//return 2 if Not set before;
int WINAPI CheckDeviceValid(CString strType, CString strParam1, CString strParam2)							
{
//	CString strType, strParam1, strParam2;
//	GetDevice(strType, strParam1, strParam2);
	if (strType == "")
		return DEV_NOT_SET;
	
	return IsDeviceValid(strType, strParam1, strParam2);
}

int WINAPI IsDeviceValid(CString strType, CString strParam1, CString strParam2)
{
   	strType.TrimLeft();
   	strType.TrimRight();
	if (strType == "")
		return DEV_ERROR;

	IEasyCapture* pEasyCapture = NULL;

	HRESULT hval = 0;
	hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&pEasyCapture);
    if (hval != S_OK) 
	{
		SHOW_FUNC_ERROR_STR("CoCreateInstance(CLSID_EasyCapture,...)");
		return DEV_ERROR;
	}

	hval = pEasyCapture->Open(0);
    if((hval != S_OK)) 
	{
		RELEASE(pEasyCapture);
		SHOW_FUNC_ERROR_STR("IEasyCapture::Open");
		return DEV_ERROR;
	}

	CStringArray strArrayWebAudio, strArrayWebVideo, strArrayDV;
	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	if (!EnumDevice(pEasyCapture, eType, strArrayWebAudio))
	{
		if (pEasyCapture != NULL) 
		{
			pEasyCapture->Close();
			RELEASE(pEasyCapture);
		}
		return DEV_ERROR;
	}

	eType = CAPTUREDEVICETYPE_VIDEO;
	if (!EnumDevice(pEasyCapture, eType, strArrayWebVideo))
	{
		if (pEasyCapture != NULL) 
		{
			pEasyCapture->Close();
			RELEASE(pEasyCapture);
		}
		return DEV_ERROR;
	}

	if (pEasyCapture != NULL) 
	{
		pEasyCapture->Close();
		RELEASE(pEasyCapture);
	}

	if (strType == "DV")
	{
		//for (int i = 0; i < strArrayDV.GetSize(); i++)
		//{
		//	if (strParam1 == strArrayDV[i])
		//		return DEV_MATCH;
		//}
		//return DEV_NOT_MATCH;

		BOOL bFound = FALSE;
		int i = 0;
		for (i = 0; i < strArrayDV.GetSize(); i++)
		{
			if (strParam1 == strArrayDV[i])
			{
				bFound = TRUE;
				break;
			}
		}
		
		if (!bFound)
			return DEV_NOT_MATCH;
		
		for (i = 0; i < strArrayWebAudio.GetSize(); i++)
		{
			if (strParam2 == strArrayWebAudio[i])
				return DEV_MATCH;
		}

		return DEV_NOT_MATCH;
	}
	else if (strType == "CAM")
	{
		BOOL bFound = FALSE;
		int i = 0;
		for (i = 0; i < strArrayWebVideo.GetSize(); i++)
		{
			if (strParam1 == strArrayWebVideo[i])
			{
				bFound = TRUE;
				break;
			}
		}
		
		if (!bFound)
			return DEV_NOT_MATCH;
		
		for (i = 0; i < strArrayWebAudio.GetSize(); i++)
		{
			if (strParam2 == strArrayWebAudio[i])
				return DEV_MATCH;
		}

		return DEV_NOT_MATCH;
	}
	else 
		return DEV_MATCH;

	return DEV_MATCH;  
}

BOOL WINAPI EnumDevice(IEasyCapture* pEasyCapture, 
					   CAPTUREDEVICETYPE eType, 
					   CStringArray& strArray)
{
	if (pEasyCapture == NULL) 
		return FALSE;

	strArray.RemoveAll();

	long lCount=0;

	HRESULT hval=0;
	hval = pEasyCapture->DeviceTableUpdate();
	if (FAILED(hval)) 
	{
		//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
		return FALSE;
	}

	hval = pEasyCapture->DeviceTableGetCount(eType, &lCount);
	if (FAILED(hval)) 
	{
		//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
		return FALSE;
	}

	CString	cst;
	BSTR bstr = NULL;
	long ii = 0;
	for (ii = 0; ii < lCount; ii++) 
	{		
		hval = pEasyCapture->DeviceTableGetName(eType, ii, &bstr);
		if (FAILED(hval)) 
		{
			//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
			return FALSE;
		}
		cst = bstr;
		SysFreeString(bstr);
		strArray.Add(cst);
	}

	return TRUE;
}

//add by buxiangyi
BOOL WINAPI EnumDeviceFrame(IEasyCapture* pEasyCapture, 
	CAPTUREDEVICETYPE eType, 
	long index,
	CStringArray& strArray)
{
	if (pEasyCapture == NULL) 
		return FALSE;

	strArray.RemoveAll();

	long lCount=0;

	HRESULT hval=0;
	hval = pEasyCapture->DeviceTableUpdate();
	if (FAILED(hval)) 
	{
		//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
		return FALSE;
	}

	hval = pEasyCapture->DeviceFormatTableGetCount(eType,index, &lCount);
	if (FAILED(hval)) 
	{
		//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
		return FALSE;
	}

	CString	cst;
	BSTR bstr = NULL;
	long ii = 0;
	for (ii = 0; ii < lCount; ii++) 
	{		
		hval = pEasyCapture->DeviceFormatTableGetFormat(eType, index, ii, &bstr);
		if (FAILED(hval)) 
		{
			//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
			return FALSE;
		}
		cst = bstr;
		SysFreeString(bstr);
		strArray.Add(cst);
	}

	return TRUE;
}
BOOL WINAPI EnumDeviceCamera(IEasyCapture* pEasyCapture,  
	long m_CameraIndex,
	CString& strArray)
{
		if (pEasyCapture == NULL) 
			return FALSE;

		long lCount=0;

		HRESULT hval=0;
		hval = pEasyCapture->DeviceTableUpdate();
		if (FAILED(hval)) 
		{
			//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
			return FALSE;
		}

		if (m_CameraIndex == -1)
		{
			return FALSE;
		}

		hval = pEasyCapture->VideoFrameRateTableGetCount(m_CameraIndex, &lCount);
		if (FAILED(hval)) 
		{
			//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
			return FALSE;
		}

		CString	cst;
		BSTR bstr = NULL;
		long ii = 0;
		for (ii = 0; ii < lCount; ii++) 
		{		
			hval = pEasyCapture->VideoFrameRateTableGetFrameRate(m_CameraIndex, ii, &bstr);
			if (FAILED(hval)) 
			{
				//SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE);
				return FALSE;
			}
			cst = bstr;
			SysFreeString(bstr);
			strArray += cst + "\\";
		}
}


int WINAPI FindDeviceValid(CAPTUREDEVICETYPE eType, CString DeviceName)
{

	IEasyCapture* pEasyCapture = NULL;

	HRESULT hval = 0;
	hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&pEasyCapture);
	if (hval != S_OK) 
	{
		SHOW_FUNC_ERROR_STR("CoCreateInstance(CLSID_EasyCapture,...)");
		return DEV_ERROR;
	}

	hval = pEasyCapture->Open(0);
	if((hval != S_OK)) 
	{
		RELEASE(pEasyCapture);
		SHOW_FUNC_ERROR_STR("IEasyCapture::Open");
		return DEV_ERROR;
	}
	CStringArray strArray;
	if (!EnumDevice(pEasyCapture, eType, strArray))
	{
		if (pEasyCapture != NULL) 
		{
			pEasyCapture->Close();
			RELEASE(pEasyCapture);
		}
		return DEV_ERROR;
	}

	if (pEasyCapture != NULL) 
	{
		pEasyCapture->Close();
		RELEASE(pEasyCapture);
	}

	
		BOOL bFound = FALSE;
		int i = 0;
		for (i = 0; i < strArray.GetSize(); i++)
		{
			if (DeviceName== strArray[i])
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound)
			return DEV_ERROR;
	

	return i;  
}

int WINAPI FindDeviceFrameValid(CAPTUREDEVICETYPE eType,int DeviceNumber, CString DeviceNameFrame, CStringArray& strArray)
{

	IEasyCapture* pEasyCapture = NULL;

	HRESULT hval = 0;
	hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&pEasyCapture);
	if (hval != S_OK) 
	{
		SHOW_FUNC_ERROR_STR("CoCreateInstance(CLSID_EasyCapture,...)");
		return DEV_ERROR;
	}

	hval = pEasyCapture->Open(0);
	if((hval != S_OK)) 
	{
		RELEASE(pEasyCapture);
		SHOW_FUNC_ERROR_STR("IEasyCapture::Open");
		return DEV_ERROR;
	}

	if (!EnumDeviceFrame(pEasyCapture, eType, DeviceNumber, strArray))
	{
		if (pEasyCapture != NULL) 
		{
			pEasyCapture->Close();
			RELEASE(pEasyCapture);
		}
		return DEV_ERROR;
	}

	if (pEasyCapture != NULL) 
	{
		pEasyCapture->Close();
		RELEASE(pEasyCapture);
	}


	BOOL bFound = FALSE;
	int i = 0;
	for (i = 0; i < strArray.GetSize(); i++)
	{
		if (DeviceNameFrame== strArray[i])
		{
			bFound = TRUE;
			break;
		}
	}

	if (!bFound)
		return DEV_ERROR;


	return i;  
}
////////////////////////////////////////////////////////////////////////////////////

int WINAPI CheckAndInputDev(CString strType, CString strParam1, CString strParam2)
{
	int n;
	while ((n = ::CheckDeviceValid(strType, strParam1, strParam2)) != DEV_MATCH)
	{
		//CString strType, strParam1, strParam2;
			::GetDevice(strType, strParam1, strParam2);		//replace 2013/9/18 kagata PPT2013
		if (n == DEV_NOT_MATCH)
		{
			CDlgDevNotInstalled dlg;
			dlg.m_strDevName = (strType == "DV") ? "DV1394" : "Web Camera";
			dlg.m_strVideoDev = strParam1;
			dlg.m_strAudioDev = strParam2;
			UINT nRet = dlg.DoModal();
			if (nRet == ID_RETRY)
				continue;
			else if (nRet == ID_CHANGE_DEV)
			{
				CDevice dlg;
				dlg.SetNoAsk(TRUE);
				dlg.DoModal();
				::GetDevice(strType, strParam1, strParam2);			//add 2013/9/18 kagata PPT2013
			}
			else if (nRet == IDCANCEL)
				return USER_ABORT;
		}
		else if (n == DEV_NOT_SET)
		{
			if (strParam2 == "")
			{
				if (ASK_YESNO(IDS_NO_SEL_BEFORE) == IDYES)
				{
					CDevice dlg;
					dlg.DoModal();
				}
				else
					return USER_ABORT;
			}
			else
				return AUDIO_ONLY;
		}
		else if (n == DEV_ERROR)
			return DEV_ERROR;
	}
	return DEV_MATCH;
}

//add by buxiangyi
int WINAPI CheckAndInputDevSCreen(CString strType, CString strParam1, CString strParam2)
{
	int n;
	while ((n = ::CheckDeviceValid(strType, strParam1, strParam2)) != DEV_MATCH)
	{
		//CString strType, strParam1, strParam2;
		::GetDevice(strType, strParam1, strParam2);		//replace 2013/9/18 kagata PPT2013
		if (n == DEV_NOT_MATCH)
		{
			CDlgDevNotInstalled dlg;
			dlg.m_strDevName = (strType == "DV") ? "DV1394" : "Screen Capture";
			dlg.m_strAudioDev = strParam2;
			dlg.m_Screenflag = TRUE;
			UINT nRet = dlg.DoModal();
			if (nRet == ID_RETRY)
				continue;
			else if (nRet == ID_CHANGE_DEV)
			{
				CDevice dlg;
				dlg.SetNoAsk(TRUE);
				dlg.DoModal();
				::GetDevice(strType, strParam1, strParam2);			//add 2013/9/18 kagata PPT2013
			}
			else if (nRet == IDCANCEL)
				return USER_ABORT;
		}
		else if (n == DEV_NOT_SET)
		{
			if (strParam2 == "")
			{
				if (ASK_YESNO(IDS_NO_SEL_BEFORE) == IDYES)
				{
					CDevice dlg;
					dlg.DoModal();
				}
				else
					return USER_ABORT;
			}
			else
				return AUDIO_ONLY;
		}
		else if (n == DEV_ERROR)
			return DEV_ERROR;
	}
	return DEV_MATCH;
}

int WINAPI CheckAndInputDevValid(CString strType, CString strParam1, CString strParam2)
{
	int n;
	if((n = ::CheckDeviceValid(strType, strParam1, strParam2)) != DEV_MATCH)
	{
		return DEV_NOT_MATCH;
	}
	else
	{
		return DEV_MATCH;
	}
}



///////////////////////////////////////////////////////////////
