#include "stdafx.h"
#include "eLearning.h"
#include "resource.h"
#include "cmkapi.h"
#include "fileapi.h"
#include <io.h>
#include <fstream>
#include <direct.h>
#include "DlgTreeSelect.h"
#include <stdlib.h>

#include <atlbase.h>
#include <mshtml.h>
#include "Macro.h"
using   namespace   std;
int WINAPI GetBitsPerPixel()
{
	int nBitsPerPixel=::GetBitPerPixel(NULL);
	return nBitsPerPixel;
}

int	WINAPI GetBitPerPixel(HWND hWnd) 
{
    HDC hdc = ::GetDC(hWnd);
    int nBitPerPixel = ::GetDeviceCaps(hdc, BITSPIXEL);
    ::ReleaseDC(hWnd, hdc);
	return nBitPerPixel;
} 

int WINAPI GetLeftNumPart(CString strSrc)
{
	CString strNo = "";
	for (int i = 0; i < strSrc.GetLength(); i++)
	{
		if (strSrc[i] >= '0' || strSrc[i] <= '9')
			strNo += strSrc[i];
		else
			break;
	}
	return atoi(strNo);
}

int WINAPI GetFileCount(LPCTSTR szSrcPath, CStringArray* pExcludeFile)
{
	if (!::FileExists(szSrcPath))
		return FALSE;

	CString strSrcPath = szSrcPath;
	if (strSrcPath.Right(1) != "\\")
		strSrcPath = strSrcPath + "\\";
	
	CStringArray strArrayDir;
	GetDir(strSrcPath, strArrayDir);

	int nCount = 0;
	int i = 0;
	for (i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strSrcDir = strSrcPath + strArrayDir[i];
		nCount += GetFileCount(strSrcDir, pExcludeFile);
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArrayFile[i];
		BOOL bNoCount = FALSE;
		if (pExcludeFile)
		{
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.CompareNoCase(strSrcFile) == 0)
				{
					bNoCount = TRUE;
					break;
				}
			}
		}
		if (!bNoCount)
			nCount++;
	}

	return nCount;
}

BOOL WINAPI TryToMkDir(CString strPath, BOOL bMsg)
{

	_mkdir(strPath);
	if (!::FileExists(strPath))
	{
		if (bMsg)
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FAIL_TO_CREATE_DIR);
			strMsg.Format(strTmp, strPath);
			AfxMessageBox(strMsg, MB_ICONSTOP | MB_OK);
		}
		return FALSE;
	}
	return TRUE;
}

void WINAPI SortArray(CStringArray& strArray)
{
	int nSize = strArray.GetSize();
	for (int i = 0; i < nSize; i++)
		for (int j = i + 1; j < nSize; j++)
		{
			if (strArray[i].CompareNoCase(strArray[j]) > 0)
			{
				CString strTmp = strArray[i];
				strArray[i] = strArray[j];
				strArray[j] = strTmp;
			}

		}

}

BOOL WINAPI GetDir(CString strDir, CStringArray& strArray)
{
	strArray.RemoveAll();

	if (!FileExists(strDir))
		return FALSE;

	if (strDir.Right(1) != "\\")
		strDir = strDir + "\\";
	CString strFind;
	strFind = strDir + "*";

	WIN32_FIND_DATA FindInf;
	HANDLE hSearch = ::FindFirstFile(strFind, &FindInf);
	if (hSearch == INVALID_HANDLE_VALUE)
		return FALSE;  //Something Wrong??

	if ((FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		strcmp(FindInf.cFileName, ".") != 0 &&
		strcmp(FindInf.cFileName, "..") != 0)
	{
		strArray.Add(FindInf.cFileName);
	}
	
	while (::FindNextFile(hSearch, &FindInf))
	{
		if ((FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
			strcmp(FindInf.cFileName, ".") != 0 &&
			strcmp(FindInf.cFileName, "..") != 0)
		{
			strArray.Add(FindInf.cFileName);
		}
	}

	::FindClose(hSearch);
	return TRUE;
}

BOOL WINAPI ValidDir(LPCTSTR szPath)
{
    DWORD dwAttr = GetFileAttributes(szPath);
    if ((dwAttr == (DWORD) -1) || (! (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
		return FALSE;

	return TRUE;
}

BOOL WINAPI GetFiles(CString strDir, CString strExt, CStringArray& strArray)
{
	strArray.RemoveAll();

	if (!FileExists(strDir))
		return FALSE;

	if (strDir.Right(1) != "\\")
		strDir = strDir + "\\";
	
	CStringArray strArrayExt;
	if (strExt.Find(";") < 0)
		strArrayExt.Add(strExt);
	else
	{
		int nCount = 0;
		CString strTok;
		while(::GetTokStr(strExt, strTok, nCount, ";"))
			strArrayExt.Add(strTok);
	}

	for (int i = 0; i < strArrayExt.GetSize(); i++)
	{
		CString strFind;
		strFind = strDir + "*." + strArrayExt[i];

		WIN32_FIND_DATA FindInf;
		HANDLE hSearch = ::FindFirstFile(strFind, &FindInf);
		if (hSearch == INVALID_HANDLE_VALUE)
			continue; 

		if (!(FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			strArray.Add(FindInf.cFileName);
		
		while (::FindNextFile(hSearch, &FindInf))
		{
			if (!(FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				strArray.Add(FindInf.cFileName);
		}

		::FindClose(hSearch);
	}

	return TRUE;
}

BOOL WINAPI GetTokStr(const char* pSource, CString& StrDest, int& Start, CString Delimeter)
{
    int Len = strlen(pSource);
    
    if (Start >= Len || Start < 0)
    	return FALSE;

    int i;             
    int Count;
    BOOL LeadByteAppear = FALSE; 
    for (i = Start; i < Len; i++)
    {
        if (i != Len - 1)
        {
        	if (LeadByteAppear)
        	{
            	LeadByteAppear = FALSE;
            	continue;
        	}
        	else if (IsDBCSLeadByte(pSource[i]))
        	{
            	LeadByteAppear = TRUE;
            	continue;
        	}
        }
        
		BOOL bFound = (Delimeter.Find(pSource[i]) >= 0);
		if (i == Len - 1 || bFound)
     	{  	
           	if (bFound)
           		Count = i - Start;
           	else 
           	    Count = i - Start + 1;
           	    
           	char* pDest = new char[Count + 1];
			strncpy(pDest, pSource + Start, Count);
            pDest[Count] = '\0';
			StrDest = pDest;
			delete [] pDest;
            Start = i + 1;
            return TRUE;
        } 
    }                 
    return FALSE;
}

// Function    : FileExists
// Description : Determines if the specified file exists
// Input       : filename -- The name of the file to check for. 
//               If no path is specified for the file, the current directory is searched.
// Output      : TRUE -- file exists
//               FALSE -- file noexists
BOOL WINAPI FileExists(const char* filename)
{	// check the file exists ?

    if (_access(filename,0) == 0)   // Check for existence
		return TRUE;		// exist
    else
		return FALSE;	   // nonexist
}

CString WINAPI GetCoursePath(BOOL bConfig)
{
	BOOL bDefault = FALSE;
	CString strIniPath = MACRO_INI_PATH;
	if (bConfig)
	{
		char szBuf[256];
		::GetPrivateProfileString("CourseFolder", "Dir", "", szBuf, 256, strIniPath);
		CString strPath = szBuf;
		if (!::FileExists(strPath))
		{
			if (TryToMkDir(strPath, 0) == FALSE)
			{
				bDefault = TRUE;
			}
		}
		if (!bDefault)
			return strPath;
	}
	char* pszBuf = new char[2048];
    ::GetModuleFileName(AfxGetInstanceHandle(), pszBuf, 2048);
    char szDrive[5];
	char* pszPath = new char[2048];
    _splitpath(pszBuf, szDrive, pszPath, NULL, NULL);
    sprintf(pszBuf, "%s%s", szDrive, pszPath);
	CString StrPath = pszBuf;
	if (StrPath.Right(4).CompareNoCase("bin\\") == 0)
		StrPath = StrPath.Left(StrPath.GetLength() - 4);
	delete [] pszBuf;
	delete [] pszPath;
	if (bDefault)
		::WritePrivateProfileString("CourseFolder", "Dir", StrPath + "Courses\\", strIniPath);
	return StrPath + "Courses\\";
}

CString WINAPI GetSysPath()
{
	char* pszBuf = new char[2048];
    ::GetModuleFileName(AfxGetInstanceHandle(), pszBuf, 2048);
    char szDrive[5];
	char* pszPath = new char[2048];
    _splitpath(pszBuf, szDrive, pszPath, NULL, NULL);
    sprintf(pszBuf, "%s%s", szDrive, pszPath);
	CString StrPath = pszBuf;
	if (StrPath.Right(4).CompareNoCase("bin\\") == 0)
		StrPath = StrPath.Left(StrPath.GetLength() - 4);
	delete [] pszBuf;
	delete [] pszPath;
	return StrPath;
}

BOOL WINAPI LoadFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty)
{ 
	if (!::FileExists(strFileName))
		return FALSE;
	
	strArray.RemoveAll();

	char*  pBuffer = new char[65535];

	ifstream inFile(strFileName); 
	CString strtmp,strtmp2,strNameOnly;
	while (!inFile.eof())
	{
		inFile.getline(pBuffer, 65535);
		strtmp=pBuffer;
		if (pBuffer[0] == '\0' && bNoEmpty)
			continue;
		int iFind,iFind2,ifind3;
		iFind=strtmp.Find(",");
		if (iFind >= 0)
		{
           iFind2=strtmp.Find(",",iFind+1);
		   if (iFind2 >= 0)
		   {
			   strtmp2=strtmp.Mid(iFind+1,iFind2-iFind-1);
			   ifind3=strtmp2.Find("\\");
			   if (ifind3 >= 0) 
			   {
				 strNameOnly=GetFileName(strtmp2);
				 strtmp=strtmp.Left(iFind+1)+strNameOnly+strtmp.Mid(iFind2,strtmp.GetLength()-iFind2);
			   }
		   }
		}
	
	    strArray.Add(strtmp);
	};

	delete [] pBuffer;
	inFile.close();
	return TRUE;
};

BOOL WINAPI SaveFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty)
{ 
	CFile cf;
	if (!cf.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		if (bNoEmpty && str == "")
			continue;
		cf.Write(str + "\r\n", str.GetLength() + 2);
	}
	
	cf.Close();

	return TRUE;
};

void WINAPI Msg(char *szFormat, ...)
{
    TCHAR szBuffer[512];  // Large buffer for very long filenames (like with HTTP)

    va_list pArgs;
    va_start(pArgs, szFormat);
    vsprintf(szBuffer, szFormat, pArgs);
    va_end(pArgs);

    MessageBox(NULL, szBuffer, AfxGetAppName(), MB_OK);
}

void WINAPI DebugOutput(TCHAR *tszErr, ...)
{
    TCHAR tszErrOut[MAX_PATH + 256];

    va_list valist;

    va_start(valist,tszErr);
    wvsprintf(tszErrOut, tszErr, valist);
    OutputDebugString(tszErrOut);
    va_end (valist);
}

BOOL WINAPI FileExists(LPCTSTR strInputPath, LPCTSTR strFileExt)
{
	CString strPath = strInputPath;
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	CString strFindPath = strPath + CString("*.") + strFileExt;
	WIN32_FIND_DATA FindInf;
	HANDLE hSearch = ::FindFirstFile(strFindPath, &FindInf);
	if (hSearch == INVALID_HANDLE_VALUE)
		return FALSE;  //Something Wrong??

	::FindClose(hSearch);
	return TRUE;
}

BOOL WINAPI IsDirEmpty(LPCTSTR strInputPath)
{
	if (!::FileExists(strInputPath))
		return FALSE;

	CString strPath = strInputPath;
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	CString strFindPath = strPath + CString("*");
	WIN32_FIND_DATA FindInf;
	HANDLE hSearch = ::FindFirstFile(strFindPath, &FindInf);
	if (hSearch == INVALID_HANDLE_VALUE)
		return TRUE;  //Something Wrong??

	if ((FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		strcmp(FindInf.cFileName, ".") != 0 &&
		strcmp(FindInf.cFileName, "..") != 0)
	{
		::FindClose(hSearch);
		return FALSE;
	}

	if (!(FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		::FindClose(hSearch);
		return FALSE;
	}

	while (::FindNextFile(hSearch, &FindInf))
	{
		if ((FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
			strcmp(FindInf.cFileName, ".") != 0 &&
			strcmp(FindInf.cFileName, "..") != 0)
		{
			::FindClose(hSearch);
			return FALSE;
		}

		if (!(FindInf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			::FindClose(hSearch);
			return FALSE;
		}
	}

	::FindClose(hSearch);
	return TRUE;
}

BOOL WINAPI ClearDir(LPCTSTR szPath)
{
	DelTree(szPath);
	_mkdir(szPath);
	return TRUE;
}

BOOL WINAPI IsFileNameValid(LPCTSTR szFileName)
{
	CString str = szFileName;

	BOOL LeadByteAppear = FALSE; 
	int nLen = str.GetLength();
	for (int i = 0; i < nLen; i++)
    {
       	if (LeadByteAppear)
       	{
           	LeadByteAppear = FALSE;
           	continue;
       	}
       	else if (IsDBCSLeadByte(str[i]))
       	{
           	LeadByteAppear = TRUE;
           	continue;
       	}

		char ch = str[i];
		switch (ch)
		{
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				return FALSE;
		}
	}
	return TRUE;
}

BOOL WINAPI SureDeleteFile(LPCTSTR strPath)
{
	SetFileWritable(strPath);
	return ::DeleteFile(strPath);
}

void WINAPI SetFileWritable(LPCTSTR strPath)
{
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		::SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
}

CSize WINAPI GetBmpDim(UINT nID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);
	return CSize(b.bmWidth, b.bmHeight);
}

BOOL WINAPI CopyTree(LPCTSTR szSrcPath, LPCTSTR szDestPath, CStringArray* pExcludeFile)
{
	if (!::FileExists(szSrcPath))
		return FALSE;
	if (!::FileExists(szDestPath))
		_mkdir(szDestPath);

	CString strSrcPath = szSrcPath;
	if (strSrcPath.Right(1) != "\\")
		strSrcPath = strSrcPath + "\\";
	CString strDestPath = szDestPath;
	if (strDestPath.Right(1) != "\\")
		strDestPath = strDestPath + "\\";

	CStringArray strArrayDir;
	GetDir(strSrcPath, strArrayDir);
	int i = 0;
	for (i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strDir = strArrayDir[i];
		CString strSrcDir = strSrcPath + strDir;
		
		BOOL bNoCopy = FALSE;
		if (pExcludeFile)
		{
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.Right(1) == "\\")
				{
					strExFile = strExFile.Left(strExFile.GetLength() - 1);
					if (strExFile.CompareNoCase(strSrcDir) == 0)
					{
						bNoCopy = TRUE;
						break;
					}
				}
			}
		}

		if (!bNoCopy)
		{
			CString strDestDir = strDestPath + strDir;
			CopyTree(strSrcDir, strDestDir, pExcludeFile);
		}
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArrayFile[i];
		CString strDestFile = strDestPath + strArrayFile[i];
		BOOL bNoCopy = FALSE;
		if (pExcludeFile)
		{
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.Right(1) != "\\" && strExFile.CompareNoCase(strSrcFile) == 0)
				{
					bNoCopy = TRUE;
					break;
				}
			}
		}
		if (!bNoCopy)
			::CopyFile(strSrcFile, strDestFile, FALSE);
	}

	return TRUE;
}

BOOL WINAPI SetTreeWritable(LPCTSTR szSrcPath)
{
	if (!::FileExists(szSrcPath))
		return FALSE;

	AfxGetApp()->BeginWaitCursor();

	::SetFileWritable(szSrcPath);

	CString strSrcPath = szSrcPath;
	if (strSrcPath.Right(1) != "\\")
		strSrcPath = strSrcPath + "\\";

	CStringArray strArrayDir;
	GetDir(strSrcPath, strArrayDir);
	int i = 0;
	for (i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strSrcDir = strSrcPath + strArrayDir[i];
		SetTreeWritable(strSrcDir);
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArrayFile[i];
		::SetFileWritable(strSrcFile);
	}

	AfxGetApp()->EndWaitCursor();

	return TRUE;
}

BOOL WINAPI DelTree(LPCTSTR szPath, CStringArray* pExcludeFile)
{
	if (!::FileExists(szPath))
		return FALSE;

	CString strPath = szPath;
	if (strPath.Right(1) != "\\")
		strPath = strPath + "\\";
	
	CStringArray strArrayDir;
	GetDir(szPath, strArrayDir);
	int i = 0;
	for (i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strDir = strPath + strArrayDir[i];
		DelTree(strDir, pExcludeFile);
		_rmdir(strDir);
	}
	
	CStringArray strArrayFile;
	GetFiles(szPath, "*", strArrayFile);
	for (i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strFile = strPath + strArrayFile[i];
		BOOL bNoDel = FALSE;
		if (pExcludeFile)
		{
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.CompareNoCase(strFile) == 0)
				{
					bNoDel = TRUE;
					break;
				}
			}
		}
		if (!bNoDel)
			SureDeleteFile(strFile);
			//::DeleteFile(strFile);
	}

	_rmdir(szPath);

	return !::FileExists(szPath);
}

//LONGLONG WINAPI ConvertStrToTime(CString strTime)
//{
//	if (strTime.GetLength() != 10)
//		return 0;

//	LONGLONG n = atol(strTime.Left(2)) * 3600 + 
//			     atol(strTime.Mid(3, 2)) * 60 + 
//			     atol(strTime.Mid(6, 2));
//	long d = atol(strTime.Right(1));
//	n = 10 * n + d;
//	return 1000000 * n;
//}

long WINAPI ConvertStrToTime(CString strTime)
{
	if (strTime.GetLength() != 10)
		return 0;

	long n = atol(strTime.Left(2)) * 3600 + 
		     atol(strTime.Mid(3, 2)) * 60 + 
		     atol(strTime.Mid(6, 2));
	long d = atol(strTime.Right(1));
	n = 10 * n + d;
	return 100 * n;
}

CString WINAPI ConvertTimeToStr(long lTime)
{
    // Convert the LONGLONG duration into human-readable format
    int nMS = (lTime % 1000) / 100;
    int nSeconds = lTime / 1000;
    int nMinutes = nSeconds / 60;
	nSeconds %= 60;
	int nHour = nMinutes / 60;
	nMinutes %= 60;
 
    // Update the display
    TCHAR szDuration[24];
    wsprintf(szDuration, _T("%02d:%02d:%02d.%01d\0"), nHour, nMinutes, nSeconds, nMS);
    return szDuration;
}

//CString WINAPI ConvertTimeToStr(LONGLONG qwtime)
//{
    // Convert the LONGLONG duration into human-readable format
//    unsigned long nTotalMS = (unsigned long)(qwtime / 10000); // 100ns -> ms
//    int nMS = (nTotalMS % 1000) / 100;
//    int nSeconds = nTotalMS / 1000;
//    int nMinutes = nSeconds / 60;
//	nSeconds %= 60;
//	int nHour = nMinutes / 60;
//	nMinutes %= 60;
 
    // Update the display
//    TCHAR szDuration[24];
//    wsprintf(szDuration, _T("%02d:%02d:%02d.%01d\0"), nHour, nMinutes, nSeconds, nMS);
//    return szDuration;
//}

CString WINAPI ConvertSecToStr(long nSeconds)
{
    int nMinutes = nSeconds / 60;
	nSeconds %= 60;
	int nHour = nMinutes / 60;
	nMinutes %= 60;
 
    // Update the display
    TCHAR szDuration[24];
    wsprintf(szDuration, _T("%02d:%02d:%02d\0"), nHour, nMinutes, nSeconds);
    return szDuration;
}

CString WINAPI ConvertSecToMinStr(long nSeconds)
{
    int nMinutes = nSeconds / 60;
	nSeconds %= 60;
	int nHour = nMinutes / 60;
	nMinutes %= 60;
 
    // Update the display
    TCHAR szDuration[24];
    wsprintf(szDuration, _T("%02d:%02d\0"), nHour, nMinutes);
    return szDuration;
}

BOOL WINAPI IsDigit(CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		char ch = str[i];
		if (ch < '0' || ch > '9')
			return FALSE;
	}
	return TRUE;
}

BOOL WINAPI IsValidTime(CString strTime)
{
	strTime.TrimLeft();
	strTime.TrimRight();
	if (strTime.GetLength() != 10)
		return FALSE;
	if (strTime[2] != ':' || strTime[5] != ':' || strTime[8] != '.')
		return FALSE;
	if (!IsDigit(strTime.Mid(0, 2)) || !IsDigit(strTime.Mid(3, 2)) || 
		!IsDigit(strTime.Mid(6, 2)) || !IsDigit(strTime.Mid(9, 1)))
		return FALSE;
	if (!(atoi(strTime.Mid(3, 2)) < 60) || !(atoi(strTime.Mid(6, 2)) < 60))
		return FALSE;
	return TRUE;
}

BOOL WINAPI SelectImageFile(CString strTitle, CString strDefPath, CString& strSelPath)
{
	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = strDefPath;
	dlg.m_bSelectable = TRUE;
	dlg.m_strTitle = strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(TRUE);
	dlg.SetShowType(3);
	dlg.m_nWidth = 640;
	dlg.m_nHeight = 480;
	if(dlg.DoModal() == IDOK)
	{
		strSelPath = dlg.m_strSelectedFile;
		return TRUE;
	}

	return FALSE;
}

BOOL WINAPI SelectCtrlFile(CString strTitle, CString strDefPath, CString& strSelPath)
{
	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = strDefPath;
	dlg.m_bSelectable = FALSE;
	dlg.m_strTitle = strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(FALSE);
	dlg.SetShowType(2);
	dlg.m_nWidth = 560;
	dlg.m_nHeight = 480;
	if(dlg.DoModal() == IDOK)
	{
		strSelPath = dlg.m_strSelectedFile;
		return TRUE;
	}

	return FALSE;
}

CString WINAPI GetDir(LPCTSTR szPath)
{
	char szDrive[32];
	char* pDir = new char[4096]; 
	_splitpath(szPath, szDrive, pDir, NULL, NULL);
	CString strRet = CString(szDrive) + pDir;
	delete pDir;
	if (strRet.Right(1) != "\\")
		strRet = strRet + "\\";
	return strRet;
}

CString WINAPI GetFilePath(LPCTSTR szPath)
{
	char* pPath = new char[1024];
	char szDisk[128];
	_splitpath(szPath, szDisk, pPath, NULL, NULL);
	CString strRet = CString(szDisk) + pPath;
	delete pPath;
	return strRet;
}

CString WINAPI GetFileName(LPCTSTR szPath)
{
	char* pName = new char[1024]; 
	char szExt[128];
	_splitpath(szPath, NULL, NULL, pName, szExt);
	CString strRet = CString(pName) + szExt;
	delete pName;
	return strRet;
}
void WINAPI AnsitoUtf_8(char *TheAnsiString,char* Utf_8Code)
{
    BSTR b ;
	int lenA = lstrlenA(TheAnsiString);
	int lenW;
	//char Utf_8Code[512];
    lenW = ::MultiByteToWideChar(CP_ACP, 0, TheAnsiString, lenA,0, 0);

	if (lenW > 0)
	{
	// Check whether conversion was successful
		
		b = ::SysAllocStringLen(0, lenW);
		::MultiByteToWideChar(CP_ACP, 0, TheAnsiString, lenA, b, lenW);
        WideCharToMultiByte( CP_UTF8, 0,  b, -1,
        Utf_8Code, 512, NULL, NULL );
 	}

}
CString WINAPI GetFileExt(LPCTSTR szPath)
{
	char szExt[128];
	_splitpath(szPath, NULL, NULL, NULL, szExt);
	CString strRet = szExt;
	if (strRet.Left(1) == ".")
		strRet = strRet.Mid(1);
	return strRet;
}

BOOL WINAPI IsFullPath(LPCTSTR szPath)
{
	CString strPath = szPath;
	char szDrive[128];
	szDrive[0] = '\0';
	_splitpath(szPath, szDrive, NULL, NULL, NULL);
	if (szDrive[0] != '\0')
		return TRUE;
	return FALSE;
}

BOOL WINAPI IsValidNumber(CString strNum)
{
	if (strNum == "")
		return TRUE;
	
	int nMinus = 0;
	int nPoint = 0;
	for (int i = 0; i < strNum.GetLength(); i++)
	{
		char ch = strNum[i];
		if ((ch < '0' && ch > '9') && ch != '.' && ch != '-')
			return FALSE;
		if (ch == '-')
			nMinus++;
		if (ch == '.')
			nPoint++;
	}

	if (nMinus > 1 || nPoint > 1)
		return FALSE;

	int nMinusIndex = strNum.Find("-");
	if (nMinusIndex > 0)
		return FALSE;

	return TRUE;
}

BOOL WINAPI SafeCopyFile(CString strSrc, CString strDest, BOOL bFailIfExists) 
{
	if (::FileExists(strDest) && bFailIfExists)
		return TRUE;

	if (!::CopyFile(strSrc, strDest, FALSE))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_COPY);
		strMsg.Format(strTmp, strSrc, strDest);
		AfxMessageBox(strMsg, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL WINAPI CopyDir(CString strSrcPath, CString strDestPath) 
{
	if (strSrcPath.Right(1) != "\\")
		strSrcPath = strSrcPath + "\\";
	if (strDestPath.Right(1) != "\\")
		strDestPath = strDestPath + "\\";

	CStringArray strArray;
	::GetFiles(strSrcPath, "*", strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArray[i];
		CString strDestFile = strDestPath + strArray[i];
		if (!::SafeCopyFile(strSrcFile, strDestFile))
			return FALSE;			
	}

	return TRUE;
}

LONGLONG WINAPI GetVolFreeSpace(CString strVol) 
{
   double number_of_bytes_free_on_drive     = 0.0;
   double high_value                        = static_cast< double >(0xFFFFFFFF);
   high_value++;

   ULARGE_INTEGER number_of_free_bytes_on_disk;

   if (GetDiskFreeSpaceEx(strVol,
                          &number_of_free_bytes_on_disk,
                          NULL, NULL ) == FALSE )
   {
      return (LONGLONG)-1;
   }

   number_of_bytes_free_on_drive  = number_of_free_bytes_on_disk.LowPart;
   number_of_bytes_free_on_drive += static_cast< double >( static_cast< double >( number_of_free_bytes_on_disk.HighPart ) * high_value );

   return (LONGLONG)(number_of_bytes_free_on_drive);
}

BOOL WINAPI CopyFileToClipboard(CString strFile) 
{
	if (!::FileExists(strFile))
		return FALSE;
	
	CStringArray strArrayFile;
	::LoadFile(strFile, strArrayFile);
	long nSize = 0;
	for (int i = 0; i < strArrayFile.GetSize(); i++)
		nSize += strArrayFile[i].GetLength() + 2;

	if(::OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GHND, nSize + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		int nIndex = 0;
		CString strTmp;
		for (int i = 0; i < strArrayFile.GetSize(); i++)
		{
			strTmp = strArrayFile[i] + "\r\n";
			strcpy(buffer + nIndex, strTmp);
			nIndex += strTmp.GetLength();
		}

		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
		return true;
	}
	return FALSE;
}

BOOL WINAPI IsFileWritable(LPCTSTR strPath)
{
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		return FALSE;

	return TRUE;
}

BOOL WINAPI IsValidEmailAddr(CString elm)
{
	int Eof_elm = elm.GetLength() - 1;
 
	if (elm.Find("@") != -1 &&
		elm.Find(".") != -1 &&
		elm.Find("~") == -1 &&
		elm.Find("`") == -1 &&
		elm.Find("!") == -1 &&
		elm.Find("#") == -1 &&
		elm.Find("$") == -1 &&
		elm.Find("%") == -1 &&
		elm.Find("^") == -1 &&
		elm.Find("&") == -1 &&
		elm.Find("*") == -1 &&
		elm.Find("(") == -1 &&
		elm.Find(")") == -1 &&
		elm.Find("+") == -1 &&
		elm.Find("=") == -1 &&
		elm.Find("{") == -1 &&
		elm.Find("}") == -1 &&
		elm.Find("[") == -1 &&
		elm.Find("]") == -1 &&
		elm.Find(":") == -1 &&
		elm.Find(";") == -1 &&
		elm.Find("\"") == -1 &&
		elm.Find("'") == -1 &&
		elm.Find("<") == -1 &&
		elm.Find(">") == -1 &&
		elm.Find(",") == -1 &&
		elm.Find("?") == -1 &&
		elm.Find("\\") == -1 &&
		elm.Find("/") == -1 &&
		elm.Find("|") == -1 &&
		elm.Find("@") != Eof_elm &&
		elm.Find(".") != Eof_elm &&
		elm.Find("@") !=0 &&
		elm.Find(".") !=0 )
	{
		return TRUE;
	}

	return FALSE;
}

CString strTmpPath;
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			if (lpData)
			{
				strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
				SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
			}
			break;

		case BFFM_SELCHANGED: 
		{
			if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
			{
			  SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
			}
			break;
		}

		default:
			break;
	}
         
	return 0;
}

BOOL WINAPI GetFolder(CString* strSelectedFolder,
					  const char* lpszTitle,
				      const HWND hwndOwner, 
				      const char* strRootFolder, 
				      const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL)
	{
		bi.pidlRoot = NULL;
	}
	else
	{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[MAX_PATH];
	   OLECHAR       olePath[MAX_PATH];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy(szPath, (LPCTSTR)strRootFolder);
	   if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	   {
		   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
		   pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}

	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL)
	{
		bi.lParam = FALSE;
	}
	else
	{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL)
	{
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE)
		{
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}
	else
	{
		strSelectedFolder->Empty();
	}
	
	return FALSE;
}


BOOL WINAPI ConverSlashToBackslash(CString& strPath)
{
	for (int i = 0; i < strPath.GetLength(); i++)
	{
		char ch = strPath[i];
		if (::IsDBCSLeadByte(ch))
		{
			i++;
			continue;
		}
		if (ch == '/')
			strPath.SetAt(i, '\\');
	}

	return TRUE;
}

BOOL WINAPI SetFullPath(CString strSrcPath, CStringArray& strArrayFile)
{
	char szDrive[32];
	char* pszPath = new char[2048];
	_splitpath(strSrcPath, szDrive, pszPath, NULL, NULL);
	
	CString strPrefix = szDrive + CString(pszPath);
	CString strPath, strTmp;
	for (int i = 0; i < strArrayFile.GetSize(); i++)
	{
		strPath = strArrayFile[i];
		::ConverSlashToBackslash(strPath);

		char szDrv[32];
		_splitpath(strPath, szDrv, NULL, NULL, NULL);
		if (szDrv[0] == '\0')
		{
			if (strPath.Left(3) == "..\\")
			{
				strTmp = strPrefix.Left(strPrefix.GetLength() - 1);
				int nIndex = strTmp.ReverseFind('\\');
				if (nIndex >= 0)
				{
					strTmp = strTmp.Left(nIndex + 1);
					strArrayFile[i] = strTmp + strPath;
				}
			}
			else if (strPath.Left(2) == ".\\")
				strArrayFile[i] = strPrefix + strPath;
			else if (strPath.Left(1) == "\\")
				strArrayFile[i] = szDrive + strPath;
			else
				strArrayFile[i] = strPrefix + strPath;
		}
		else
			strArrayFile[i] = strPath;
	}

	delete [] pszPath;

	return TRUE;
}

void WINAPI CopyArray(CStringArray& strArraySrc, CStringArray& strArrayDest, BOOL bConv)
{
	for (int i = 0; i < strArraySrc.GetSize(); i++)
	{
		CString strPath = strArraySrc[i];
		if (bConv)
			::ConverSlashToBackslash(strPath);
		strArrayDest.Add(strPath);
	}
}

CString WINAPI GetAssociatedDir(CString strSrcPath)
{
	CString strFileName = ::GetFileName(strSrcPath);
	CString strPath = ::GetFilePath(strSrcPath);
	int nIndex = strFileName.ReverseFind('.');
	if (nIndex >= 0)
	{
		strPath = strPath + strFileName.Left(nIndex) + ".files\\";;
		return strPath;
	}

	return "";
}

CString WINAPI GetDotFiles(CString strSrcPath)
{
	CString strFileName = ::GetFileName(strSrcPath);
	int nIndex = strFileName.ReverseFind('.');
	if (nIndex >= 0)
		return strFileName.Left(nIndex) + ".files\\";;

	return "";
}

BOOL WINAPI CopyHTMLFile(CString strSrcPath, CString strDestPath, BOOL bCopyDotFiles)
{
	CString strDestDir = ::GetFilePath(strDestPath);
	if (::FileExists(strDestDir))
		::DelTree(strDestDir);
	CopyAllFilesNeed(strSrcPath, strDestPath, bCopyDotFiles);

	return TRUE;
}

BOOL WINAPI CopyAllFilesNeed(CString strSrcPath, CString strDestPath, BOOL bCopyDotFiles)
{
	CString strSrcDir = ::GetFilePath(strSrcPath);
	CString strDestDir = ::GetFilePath(strDestPath);
	CString strDotFiles = GetDotFiles(strSrcPath);

	_mkdir(strDestDir);
	::CopyFile(strSrcPath, strDestPath, FALSE);

	if (bCopyDotFiles)
	{
		CString strDotSrcFilePath = GetAssociatedDir(strSrcPath);
		CString strDotDestFilePath = strDestDir + strDotFiles;
		::CopyTree(strDotSrcFilePath, strDotDestFilePath);
	}

	CStringArray strArrayFile, strArrayOriginalFile;
	BOOL bFrame;
	GetHTMLEmbededFile(strSrcPath, strArrayFile, bFrame);
	if (bFrame)
	{
		for (int i = 0; i < strArrayFile.GetSize(); i++)
		{
			CString strPath = strArrayFile[i];
			::ConvertSlashToBackslash(strPath);
			if (!::IsFullPath(strPath))
			{
				CString strSubSrcFile = strSrcDir + strPath;
				CString strSubDestFile = strDestDir + strPath;
				CopyAllFilesNeed(strSubSrcFile, strSubDestFile, bCopyDotFiles);
			}
		}

		return TRUE;
	}


	CopyArray(strArrayFile, strArrayOriginalFile, FALSE);
	SetFullPath(strSrcPath, strArrayFile);

	//::SaveFile("c:\\filelist.txt", strArrayOriginalFile);

	CString strSrcFile, strDestFile, strMarkFile, strDir;
	int i = 0;
	for (i = 0; i < strArrayFile.GetSize(); i++)
	{
		strSrcFile = strArrayFile[i];

		if (::FileExists(strSrcFile))
		{
			int nLenSrc = strSrcDir.GetLength();
			if (strSrcFile.Left(nLenSrc).CompareNoCase(strSrcDir) == 0)
			{
				strDestFile = strDestDir + strSrcFile.Mid(nLenSrc);
				if (!::FileExists(strDestFile))
				{
					strDir = ::GetFilePath(strDestFile);
					if (!::FileExists(strDir))
						_mkdir(strDir);
					::CopyFile(strSrcFile, strDestFile, TRUE);
				}
			}
		}
	}

	int nFileIndex = 1;
	CString strNewName, strFileExt, strTmp;
	CStringArray strArrayTag, strArrayReplace;
	for (i = 0; i < strArrayOriginalFile.GetSize(); i++)
	{
		strSrcFile = strArrayOriginalFile[i];
		::ConvertSlashToBackslash(strSrcFile);
		if (::IsFullPath(strSrcFile) && ::FileExists(strSrcFile))
		{
			strFileExt = ::GetFileExt(strSrcFile);
			strNewName.Format("_%05d.%s", nFileIndex++, strFileExt);

			strDestFile = strDestDir + strDotFiles + strNewName;

			::CopyFile(strSrcFile, strDestFile, FALSE);

			strTmp = strDir.Left(strDir.GetLength() - 1);
			strTmp = ::GetFileName(strTmp);
			strMarkFile = strTmp + "\\" + strNewName;

			strArrayTag.Add(strArrayOriginalFile[i]);
			strArrayReplace.Add(strMarkFile);

			strArrayTag.Add(strSrcFile);
			strArrayReplace.Add(strMarkFile);
		}
	}

	::UpdateFileByTag(strDestDir, ::GetFileName(strDestPath), strArrayTag, strArrayReplace, FALSE);

	return TRUE;
}

BOOL WINAPI GetHTMLEmbededFile(CString strSrcPath, CStringArray& strArrayFile, BOOL& bFrame)
{
	if (!::FileExists(strSrcPath))
		return FALSE;

	CFile f;

	//let's open file and read it into CString (u can use any buffer to read though
	if (f.Open(strSrcPath, CFile::modeRead | CFile::shareDenyNone)) 
	{
		bFrame = FALSE;

		CString strWholeFile;
		int nLength = f.GetLength();
		char* pBuffer = new char[nLength + 1];
		f.Read(pBuffer, nLength);
		f.Close();

		IHTMLDocument2* pDoc;
		CComPtr<IHTMLElementCollection> pColl;
		CComPtr<IHTMLElement> pElement;

		HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
									  IID_IHTMLDocument2, (void**)&pDoc);
		
		//put the code into SAFEARRAY and write it into document
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		VARIANT *param;

		CComBSTR bstrData = pBuffer;

		hr = SafeArrayAccessData(psa, (LPVOID*)&param);
		param->vt = VT_BSTR;
		param->bstrVal = (BSTR)bstrData;
		
		hr = pDoc->write(psa);
		hr = pDoc->close();
	 	
		SafeArrayDestroy(psa);

		delete [] pBuffer;

		CComPtr<IDispatch> pDisp;
		//CStringArray strArrayFile;
		
	    if (SUCCEEDED(pDoc->get_all(&pColl)) && pColl)
		{
			long lCount;
			if (FAILED(pColl->get_length(&lCount)))
				return FALSE;

			for(long i = 0; i < lCount; i++)
			{
				CComVariant varIndex(i), varZero(0);
				if (FAILED(pColl->item(varIndex, varZero, &pDisp)))
					return FALSE;

				CComQIPtr<IHTMLElement> pElement(pDisp);
				if (!pElement)
					return FALSE;

				CComBSTR bstrName;
				if (SUCCEEDED(pElement->get_tagName(&bstrName)))
				{
					CString str = COLE2T(bstrName);
					BOOL bFound = FALSE;
					CComVariant vTag;
					CComBSTR bstrTmp;
					CString strRet;

					if (str.CompareNoCase("IMG") == 0 ||
						str.CompareNoCase("SCRIPT") == 0 ||
						str.CompareNoCase("INPUT") == 0 ||
						str.CompareNoCase("IFRAME") == 0 ||
						str.CompareNoCase("FRAME") == 0)
					{
						if (str.CompareNoCase("FRAME") == 0)
							bFrame = TRUE;

						bstrTmp = "SRC";
						if (SUCCEEDED(pElement->getAttribute(bstrTmp, 0, &vTag)))
						{
							if (vTag.vt != VT_NULL)
							{
								strRet = vTag.bstrVal;
								if (strRet != "")
									bFound = TRUE;
							}
						}
					}
					else if (str.CompareNoCase("BODY") == 0 || 
							 str.CompareNoCase("TD") == 0 ||
							 str.CompareNoCase("TR") == 0 || 
							 str.CompareNoCase("TABLE") == 0)
					{
						bstrTmp = "BACKGROUND";
						if (SUCCEEDED(pElement->getAttribute(bstrTmp, 0, &vTag)))
						{
							if (vTag.vt != VT_NULL)
							{
								strRet = vTag.bstrVal;
								if (strRet != "")
									bFound = TRUE;
							}
						}
					}
					else if (str.CompareNoCase("LINK") == 0 ||
						     str.CompareNoCase("A") == 0 ||
							 str.CompareNoCase("AREA") == 0)  
					{
						bstrTmp = "HREF";
						if (SUCCEEDED(pElement->getAttribute(bstrTmp, 0, &vTag)))
						{
							if (vTag.vt != VT_NULL)
							{
								strRet = vTag.bstrVal;
								if (strRet != "")
								{
									int nIndex = strRet.ReverseFind('.');
									if (nIndex >= 0)
									{
										CString strExt = strRet.Mid(nIndex + 1);
										if (strExt.CompareNoCase("htm") == 0 ||
											strExt.CompareNoCase("html") == 0 ||
											strExt.CompareNoCase("css") == 0)
										{
											bFound = TRUE;
										}
									}
								}
							}
						}
					}

					if (bFound)
					{
						BOOL bLocal = TRUE;
						if (strRet.Left(7).CompareNoCase("http://") == 0)
							bLocal = FALSE;

						if (strRet.Left(11) == "about:blank")
						{
							strRet = strRet.Mid(11);
							if (strRet == "")
								bLocal = FALSE;
						}
						
						if (bLocal)
						{
							if (strRet.Left(8).CompareNoCase("file:///") == 0)
								strRet = strRet.Mid(8);

							BOOL bNotFound = TRUE;
							for (int i = 0; i < strArrayFile.GetSize(); i++)
							{
								CString strFile = strArrayFile[i];
								if (strFile == strRet)
								{
									bNotFound = FALSE;
									break;
								}
							}

							if (bNotFound)
								strArrayFile.Add(strRet);
						}
					}
				}

				pDisp.Release();
			}			
		}

		pDoc->Release();
	}

	return TRUE;
}

BOOL WINAPI	GetHTMLTitle(CString strSrcPath, CString& strTitle)
{
	CFile f;

	//let's open file and read it into CString (u can use any buffer to read though
	if (f.Open(strSrcPath, CFile::modeRead | CFile::shareDenyNone)) 
	{
		CString strWholeFile;
		int nLength = f.GetLength();
		char* pBuffer = new char[nLength + 1];
		f.Read(pBuffer, nLength);
		f.Close();

		IHTMLDocument2* pDoc;
		CComPtr<IHTMLElementCollection> pColl;
		CComPtr<IHTMLElement> pElement;

		HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
									  IID_IHTMLDocument2, (void**)&pDoc);
		
		//put the code into SAFEARRAY and write it into document
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		VARIANT *param;

		CComBSTR bstrData = pBuffer;

		hr = SafeArrayAccessData(psa, (LPVOID*)&param);
		param->vt = VT_BSTR;
		param->bstrVal = (BSTR)bstrData;
		
		hr = pDoc->write(psa);
		hr = pDoc->close();
	 	
		SafeArrayDestroy(psa);

		delete [] pBuffer;

		BSTR bstrTitle = NULL;
		hr = pDoc->get_title(&bstrTitle);
		pDoc->Release();
		if (SUCCEEDED(hr))
		{
			strTitle = bstrTitle;
			SysFreeString(bstrTitle);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL WINAPI ConvertSlashToBackslash(CString& strPath)
{
	for (int i = 0; i < strPath.GetLength(); i++)
	{
		char ch = strPath[i];
		if (::IsDBCSLeadByte(ch))
		{
			i++;
			continue;
		}
		if (ch == '/')
			strPath.SetAt(i, '\\');
	}

	return TRUE;
}

HWND WINAPI GetChildWndByClassName(HWND hWnd, CString strClass)
{
	if (!hWnd)
		return NULL;

	HWND h = ::GetWindow(hWnd, GW_CHILD);
	if (!h)
		return NULL;
	
	HWND h2;
	while (h)
	{
		char szBuffer[128];
		::GetClassName(h, szBuffer, 128);
		if (strClass == szBuffer)
			return h;
		
		if ((h2 = ::GetChildWndByClassName(h, strClass)) != NULL)
			return h2;
		
		h = ::GetWindow(h, GW_HWNDNEXT);
	}

	return NULL;
}

void WINAPI DrawResizeBar(CRect& rect)
{
    HDC hdc;
	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
    HBRUSH hb;
	BITMAP bm;
    HBITMAP hbm;

	// See the KB Article Q68569 for information about how to draw the 
	// resizing rectangle.  That's where this pattern comes from.
    WORD aZigzag[] = { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA };

	// Fill out the bitmap structure for the PatBlt calls later
    bm.bmType = 0;
    bm.bmWidth = 8;
    bm.bmHeight = 8;
    bm.bmWidthBytes = 2;
    bm.bmPlanes = 1;
    bm.bmBitsPixel = 1;
    bm.bmBits = aZigzag;

    hbm = ::CreateBitmapIndirect(&bm);
    hb = ::CreatePatternBrush(hbm);   

    // By specifying NULL for the HWND in GetDC(), we get the DC for the
    // entire screen.
    hdc = ::GetDC(NULL);	 
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hb);

	::PatBlt(hdc, rect.left, rect.top, width, height, PATINVERT);

    ::SelectObject(hdc, hOldBrush);

	// Clean up
    ::ReleaseDC(NULL, hdc);
    ::DeleteObject(hb);
    ::DeleteObject(hbm);
}

AFX_STATIC_DATA HCURSOR _afx_hcurLast = NULL;
AFX_STATIC_DATA HCURSOR _afx_hcurDestroy = NULL;
AFX_STATIC_DATA UINT _afx_idcPrimaryLast = 0;
void WINAPI SetSplitCursor(int nType)
{
	UINT idcPrimary;        // app supplied cursor
	LPCTSTR idcSecondary;    // system supplied cursor (MAKEINTRESOURCE)

	if (nType == 1)
	{
		idcPrimary = AFX_IDC_HSPLITBAR;
		idcSecondary = IDC_SIZEWE;
	}
	else if (nType == 2)
	{
		idcPrimary = AFX_IDC_VSPLITBAR;
		idcSecondary = IDC_SIZENS;
	}

	if (idcPrimary != 0)
	{
		HCURSOR hcurToDestroy = NULL;
		if (idcPrimary != _afx_idcPrimaryLast)
		{
			HINSTANCE hInst = AfxFindResourceHandle(
				MAKEINTRESOURCE(idcPrimary), RT_GROUP_CURSOR);

			// load in another cursor
			hcurToDestroy = _afx_hcurDestroy;

			// Note: If this LoadCursor call fails, it is likely that
			//  _AFX_NO_SPLITTER_RESOURCES is defined in your .RC file.
			// To correct the situation, remove the following line from your
			//  resource script:
			//      #define _AFX_NO_SPLITTER_RESOURCES
			// This should be done using the Resource.Set Includes... command.

			if ((_afx_hcurDestroy = _afx_hcurLast =
			   ::LoadCursor(hInst, MAKEINTRESOURCE(idcPrimary))) == NULL)
			{
				// will not look as good
				TRACE0("Warning: Could not find splitter cursor - using system provided alternative.\n");

				ASSERT(_afx_hcurDestroy == NULL);    // will not get destroyed
				_afx_hcurLast = ::LoadCursor(NULL, idcSecondary);
				ASSERT(_afx_hcurLast != NULL);
			}
			_afx_idcPrimaryLast = idcPrimary;
		}
		ASSERT(_afx_hcurLast != NULL);
		::SetCursor(_afx_hcurLast);
		ASSERT(_afx_hcurLast != hcurToDestroy);
		if (hcurToDestroy != NULL)
			::DestroyCursor(hcurToDestroy); // destroy after being set
	}
}

int WINAPI GetIndex(CString strPath, CString strAppendix, CString strPrefix)
{
	CStringArray strArray;
	::GetFiles(strPath, strAppendix, strArray);
	int nIndex = 0;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int nLen = str.GetLength();
		if (nLen > strPrefix.GetLength() + 4)
		{
			str = str.Left(nLen - 4);
			int n = strPrefix.GetLength();
			if (str.Left(n).CompareNoCase(strPrefix) == 0)
			{
				int m = atoi(str.Right(str.GetLength() - n));
				if (m > nIndex)
					nIndex = m;
			}
		}
	}

	return nIndex;
}

void WINAPI DrawSunk(HDC hDC,LPRECT lpRect, BOOL bExclusive)
{   
	::SetROP2(hDC, R2_COPYPEN);
	
   	HPEN Pen1;
	Pen1 = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	HPEN OldPen;
	OldPen = (HPEN)::SelectObject(hDC, Pen1);

	::MoveToEx(hDC, lpRect->left, lpRect->bottom, NULL);
	::LineTo(hDC, lpRect->left, lpRect->top);
	::LineTo(hDC, lpRect->right, lpRect->top); 
	//(HPEN)::SelectObject(hDC, OldPen);
     
    HPEN Pen2;
	Pen2=::CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	(HPEN)::SelectObject(hDC, Pen2);
    //::MoveTo(hDC,lpRect->right, lpRect->top);
	::LineTo(hDC,lpRect->right, lpRect->bottom);
	::LineTo(hDC,lpRect->left, lpRect->bottom);

	::SelectObject(hDC, OldPen);

    ::DeleteObject(Pen1);
    ::DeleteObject(Pen2);
} 

CString WINAPI GetVideoPath(CString strCourse)
{
	CString strVideoPath = (GetCoursePath(1) + CString(strCourse) + "\\video\\default_repair.mp4");
	if (!FileExists(strVideoPath))
		strVideoPath = (GetCoursePath(1) + CString(strCourse) + "\\video\\default.mp4");
	return strVideoPath;
}

void WINAPI GetProcessTime(BOOL bStart)
{
	long static t1 = 0;
	long static t2 = 0;
	if (bStart)
		t1 = GetTickCount();
	else
	{
		t2 = GetTickCount();
		CString str;
		str.Format("time:%dms",t2 - t1);
		AfxMessageBox(str);
	}
}