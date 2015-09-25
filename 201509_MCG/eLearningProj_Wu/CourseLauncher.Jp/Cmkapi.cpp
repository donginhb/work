#include "stdafx.h"
#include "CourseLoader.h"
#include "cmkapi.h"
#include <io.h>
#include <fstream>
#include <direct.h>
#include <stdlib.h>
using namespace std;

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

	char*  pBuffer = new char[2048];

	ifstream inFile(strFileName); 
	while (!inFile.eof())
	{
		inFile.getline(pBuffer, 2048);
		if (pBuffer[0] == '\0' && bNoEmpty)
			continue;
		strArray.Add(pBuffer);
	};

	delete [] pBuffer;

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
        if (i != nLen - 1)
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
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		::SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
	return ::DeleteFile(strPath);
}

CSize WINAPI GetBmpDim(UINT nID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);
	return CSize(b.bmWidth, b.bmHeight);
}

BOOL WINAPI TryToMakeDir(CString strPath, CWnd* pWnd)
{
	if (::FileExists(strPath))
		return TRUE;

	_mkdir(strPath);

	if (!::FileExists(strPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_CREATE_DIR);
		strMsg.Format(strTmp, strPath);
		W_SHOW_ERROR_STR(strMsg, pWnd);
		return FALSE;
	}

	return TRUE;
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
	for (int i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strSrcDir = strSrcPath + strArrayDir[i];
		nCount += GetFileCount(strSrcDir, pExcludeFile);
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (int i = 0; i < strArrayFile.GetSize(); i++)
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

BOOL WINAPI CopyTree(LPCTSTR szSrcPath, LPCTSTR szDestPath, CStringArray* pExcludeFile, BOOL bSetWritable)
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
	for (int i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strSrcDir = strSrcPath + strArrayDir[i];
		CString strDestDir = strDestPath + strArrayDir[i];
		if (pExcludeFile)
		{
			BOOL bBreak = FALSE;
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.Right(1) == "\\")
				{
					strExFile = strExFile.Left(strExFile.GetLength() - 1);
					if (strSrcDir.CompareNoCase(strExFile) == 0)
					{
						bBreak = TRUE;
						break;
					}
				}
			}
			if (bBreak)
				continue;
		}
		CopyTree(strSrcDir, strDestDir, pExcludeFile);
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (int i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArrayFile[i];
		CString strDestFile = strDestPath + strArrayFile[i];
		BOOL bNoCopy = FALSE;
		if (pExcludeFile)
		{
			for (int j = 0; j < pExcludeFile->GetSize(); j++)
			{
				CString strExFile = pExcludeFile->GetAt(j);
				if (strExFile.CompareNoCase(strSrcFile) == 0)
				{
					bNoCopy = TRUE;
					break;
				}
			}
		}
		if (!bNoCopy)
		{
			::CopyFile(strSrcFile, strDestFile, FALSE);
			if (bSetWritable)
				SetFileWritable(strDestFile);
		}
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
	for (int i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strSrcDir = strSrcPath + strArrayDir[i];
		SetTreeWritable(strSrcDir);
	}

	CStringArray strArrayFile;
	GetFiles(szSrcPath, "*", strArrayFile);
	for (int i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strSrcFile = strSrcPath + strArrayFile[i];
		::SetFileWritable(strSrcFile);
	}

	AfxGetApp()->EndWaitCursor();

	return TRUE;
}

BOOL WINAPI IsFileWritable(LPCTSTR strPath)
{
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		return FALSE;

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
	for (int i = 0; i < strArrayDir.GetSize(); i++)
	{
		CString strDir = strPath + strArrayDir[i];
		DelTree(strDir, pExcludeFile);
		_rmdir(strDir);
	}
	
	CStringArray strArrayFile;
	GetFiles(szPath, "*", strArrayFile);
	for (int i = 0; i < strArrayFile.GetSize(); i++)
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
			::SureDeleteFile(strFile);
	}

	_rmdir(szPath);

	return !::FileExists(szPath);
}

LONGLONG WINAPI ConvertStrToTime(CString strTime)
{
	if (strTime.GetLength() != 10)
		return 0;

	LONGLONG n = atol(strTime.Left(2)) * 3600 + 
			     atol(strTime.Mid(3, 2)) * 60 + 
			     atol(strTime.Mid(6, 2));
	long d = atol(strTime.Right(1));
	n = 10 * n + d;
	return 1000000 * n;
}

CString WINAPI ConvertTimeToStr(LONGLONG qwtime)
{
    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long)(qwtime / 10000); // 100ns -> ms
    int nMS = (nTotalMS % 1000) / 100;
    int nSeconds = nTotalMS / 1000;
    int nMinutes = nSeconds / 60;
	nSeconds %= 60;
	int nHour = nMinutes / 60;
	nMinutes %= 60;
 
    // Update the display
    TCHAR szDuration[24];
    wsprintf(szDuration, _T("%02d:%02d:%02d.%01d\0"), nHour, nMinutes, nSeconds, nMS);
    return szDuration;
}

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

CString WINAPI GetFileName(LPCTSTR szPath)
{
	char* pName = new char[1024]; 
	char szExt[128];
	_splitpath(szPath, NULL, NULL, pName, szExt);
	CString strRet = CString(pName) + szExt;
	delete pName;
	return strRet;
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

void WINAPI SetFileWritable(LPCTSTR strPath)
{
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		::SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
}

BOOL WINAPI UpdateFileByTag(CString strPath, CString strFile, 
			 				CStringArray& strsTag, CStringArray& strsReplace, 
							BOOL bReplaceToLast)
{
	BOOL bRet = TRUE;

	CString strSrcPath = strPath + "\\" + strFile;
	CString strDestPath = strPath + "\\" + "tmp";
	CString strTmpPath = strPath + "\\" + "tmp2";

	FILE *stream_in;
	if ((stream_in = fopen((LPCTSTR)strSrcPath, "r" )) == NULL)
		return FALSE;

	FILE *stream_out;
	if ((stream_out = fopen((LPCTSTR)strDestPath, "w+" )) == NULL)
	{
		fclose(stream_in);
		return FALSE;
	}

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		fclose(stream_out);
		return FALSE;
	}

	//int n = strlen(pline) - 1;
	//if (pline[n] == '\r' || pline[n] == '\n')
	//	pline[n] = '\0';

	BOOL bUpdate = FALSE;
	CString strBuf = pline;// + CString("\r\n");
	while (TRUE)
	{
		for (int i = 0; i < strsTag.GetSize(); i++)
		{
			CString strFind = strsTag[i];
			CString strReplace = strsReplace[i];
			int nIndex = strBuf.Find(strFind);
			if (nIndex >= 0)
			{
				CString strRemain = strBuf.Mid(nIndex + strFind.GetLength());
				strBuf = strBuf.Left(nIndex) + strReplace;
				if (!bReplaceToLast) 
					strBuf += strRemain;
				else
					strBuf += "\n";

				bUpdate = TRUE;
			}
		}

		if (fputs(strBuf, stream_out) == EOF)
		{
			bRet = FALSE;
			break;
		}
		
		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		//int n = strlen(pline) - 1;
		//if (pline[n] == '\r' || pline[n] == '\n')
		//	pline[n] = '\0';
		strBuf = pline;// + CString("\r\n");
	}

	fclose(stream_in);
	fclose(stream_out);
	delete [] pline;

	if (!bUpdate)
	{
		::SureDeleteFile(strDestPath);
		return TRUE;
	}

	if (bRet)
	{
		::SureDeleteFile(strTmpPath);
		if (rename(strSrcPath, strTmpPath) == 0)
		{
			if (rename(strDestPath, strSrcPath) != 0)
			{
				bRet = FALSE;
				rename(strTmpPath, strSrcPath);
			}
			else
				::SureDeleteFile(strTmpPath);
		}
		else
			bRet = FALSE;
	}

	return bRet;
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

CString WINAPI GetFilePath(LPCTSTR szPath)
{
	char* pPath = new char[1024];
	char szDisk[128];
	_splitpath(szPath, szDisk, pPath, NULL, NULL);
	CString strRet = CString(szDisk) + pPath;
	delete pPath;
	return strRet;
}

