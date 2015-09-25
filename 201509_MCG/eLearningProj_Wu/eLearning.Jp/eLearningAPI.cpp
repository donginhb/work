#include "stdafx.h"
#include "resource.h"
#include "eLearningAPI.h"
#include "FileApi.h"
#include "cmkapi.h"
#include "Macro.h"
#include "wmsdk.h"

void WINAPI SetAdvisor(CString strCourse, CString strAdvisor) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strsTag, strsReplace;
	strsTag.Add("var g_strAdvisor=");

	strsReplace.Add("var g_strAdvisor=" + CString("\"") + strAdvisor + "\";");
	::UpdateFileByTag(strPath, "_info.js", strsTag, strsReplace, TRUE);
}

void WINAPI SetMessage(CString strCourse, CString strMSG) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strsTag, strsReplace;
	strsTag.Add("var g_strMessage=");

	strsReplace.Add("var g_strMessage=" + CString("\"") + strMSG + "\";");
	::UpdateFileByTag(strPath, "_info.js", strsTag, strsReplace, TRUE);
}

void WINAPI SetCourseName(CString strCourse, CString strCourseName) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strsTag, strsReplace;
	strsTag.Add("var g_strCourseName=");

	strsReplace.Add("var g_strCourseName=" + CString("\"") + strCourseName + "\";");
	::UpdateFileByTag(strPath, "_info.js", strsTag, strsReplace, TRUE);
}
//add by buxaingyi
void WINAPI SetJSCourseName(CString strCourse, CString strCourseName) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strsTag, strsReplace;
	strsTag.Add("var TitleName = ");

	strsReplace.Add("var TitleName = " + CString("\"") + strCourseName + "\";");
	::UpdateFileByTag(strPath, "config.js", strsTag, strsReplace, TRUE);
}
/////////////////////////////////////////////////////////////////////////
BOOL WINAPI GetFileDuration(CString strPath, long& lDuration)
{
	lDuration = 0;

	IWMHeaderInfo *pHeaderInfo;
	WCHAR         wszFilename[_MAX_PATH];
	if (!::FileExists(strPath))
		return FALSE;
	
	::MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS, strPath, -1, wszFilename, _MAX_PATH);
	  
//    CoInitialize(NULL);

	IWMMetadataEditor *pEditor;
	HRESULT hr = WMCreateEditor(&pEditor);
	if (FAILED(hr))
	{
//	   CoUninitialize();
		return FALSE;
	}

	hr = pEditor->Open(wszFilename);
	if (FAILED(hr))
	{
		pEditor->Release();
//	    CoUninitialize();
		return FALSE;
	}

	hr = pEditor->QueryInterface(IID_IWMHeaderInfo, ( void ** )&pHeaderInfo);
	WORD cbLength = sizeof(QWORD) ;

	QWORD qwFirstTime;
	WORD wStreamNum = 0;
	WMT_ATTR_DATATYPE Type;
	hr = pHeaderInfo->GetAttributeByName(&wStreamNum, L"Duration", 
										 &Type, (BYTE *)&qwFirstTime, &cbLength ) ;

	if (FAILED(hr))
	{
//	    CoUninitialize();
		pHeaderInfo->Release();
		pEditor->Release();
		return FALSE;
	};

	lDuration = (long)(qwFirstTime / 10000);

	pHeaderInfo->Release();
	pEditor->Release();
//    CoUninitialize();

	return(hr);
}