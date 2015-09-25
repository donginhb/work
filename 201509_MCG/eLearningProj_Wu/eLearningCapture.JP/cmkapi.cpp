#include "stdafx.h"
#include "cmkapi.h"
#include <io.h>
#include <fstream>
using namespace std;

CString WINAPI GetFilePath(LPCTSTR szPath)
{
	char* pPath = new char[1024];
	char szDisk[128];
	_splitpath(szPath, szDisk, pPath, NULL, NULL);
	CString strRet = CString(szDisk) + pPath;
	delete pPath;
	return strRet;
}

BOOL WINAPI DelFiles(CString strPath, CString strExt)
{
	if (!::FileExists(strPath))
		return TRUE;
	CStringArray strArrayFiles;
	GetFiles(strPath, strExt, strArrayFiles);
	if (strPath.Right(1) != "\\")
		strPath = strPath + "\\";
	for (int i = 0; i < strArrayFiles.GetSize(); i++)
	{
		CString strFile = strArrayFiles[i];
		if (!SureDeleteFile(strPath + strFile))
			return FALSE;
	}
	return TRUE;
}

BOOL WINAPI FileExists(const char* filename)
{	// check the file exists ?

    if (_access(filename,0) == 0)   // Check for existence
		return TRUE;		// exist
    else
		return FALSE;	   // nonexist
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

BOOL WINAPI SureDeleteFile(LPCTSTR strPath)
{
	DWORD dw = ::GetFileAttributes(strPath);
	if (dw | FILE_ATTRIBUTE_READONLY)
		::SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
	return ::DeleteFile(strPath);
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

CString	 WINAPI GetCoursePath(BOOL bConfig)
{
	return "";
}