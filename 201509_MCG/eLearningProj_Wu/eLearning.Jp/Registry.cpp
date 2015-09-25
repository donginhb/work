#include "stdafx.h"
#include "Registry.h"

BOOL WINAPI RegGetKey(CString StrKey, int& nStrValue)
{
	CString StrValue;
	if (RegGetKey(StrKey, StrValue))
	{
		nStrValue = atoi(StrValue);
		return TRUE;
	}
	
	return FALSE;
}

BOOL WINAPI RegKeyExists(CString StrKey)
{
	int nFirstIndex = StrKey.Find('\\');
	if (nFirstIndex < 0)
		return FALSE;
	CString StrFirstKey = StrKey.Left(nFirstIndex);
	CString StrSec = StrKey.Mid(nFirstIndex + 1);
	HKEY hKey;
	if (StrFirstKey == "HKEY_CLASSES_ROOT")
		hKey = HKEY_CLASSES_ROOT;
	else if (StrFirstKey == "HKEY_CURRENT_USER")
		hKey = HKEY_CURRENT_USER;
	else if (StrFirstKey == "HKEY_LOCAL_MACHINE")
		hKey = HKEY_LOCAL_MACHINE;
	else if (StrFirstKey == "HKEY_USERS")
		hKey = HKEY_USERS;
	else if (StrFirstKey == "HKEY_CURRENT_CONFIG")
		hKey = HKEY_CURRENT_CONFIG;
	else
		return FALSE;

	if (RegOpenKeyEx(hKey,  _T(StrSec),	0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL WINAPI RegGetKey(CString StrKey, CString& StrValue)
{
	int nFirstIndex = StrKey.Find('\\');
	int nLastIndex = StrKey.ReverseFind('\\');
	if (nFirstIndex < 0)
		return FALSE;
	CString StrFirstKey = StrKey.Left(nFirstIndex);
	CString StrLastKey = StrKey.Mid(nLastIndex + 1);
	CString StrSec = StrKey.Mid(nFirstIndex + 1, nLastIndex - nFirstIndex - 1);
	HKEY hKey;
	if (StrFirstKey == "HKEY_CLASSES_ROOT")
		hKey = HKEY_CLASSES_ROOT;
	else if (StrFirstKey == "HKEY_CURRENT_USER")
		hKey = HKEY_CURRENT_USER;
	else if (StrFirstKey == "HKEY_LOCAL_MACHINE")
		hKey = HKEY_LOCAL_MACHINE;
	else if (StrFirstKey == "HKEY_USERS")
		hKey = HKEY_USERS;
	else if (StrFirstKey == "HKEY_CURRENT_CONFIG")
		hKey = HKEY_CURRENT_CONFIG;
	else
		return FALSE;

	if (RegOpenKeyEx(hKey,  _T(StrSec),	0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		TCHAR* pszData = new TCHAR[2048];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = 2048;

		if (RegQueryValueEx(hKey, _T(StrLastKey), NULL, &dwKeyDataType,
				(LPBYTE) pszData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			StrValue = pszData;
		}
		RegCloseKey(hKey);
		delete [] pszData;
		return TRUE;
	}

	return FALSE;
}

BOOL WINAPI RegSetKey(CString StrKey, int nStrValue)
{
	char szBuffer[32];
	itoa(nStrValue, szBuffer, 10);
	return RegSetKey(StrKey, szBuffer);
}

BOOL WINAPI RegSetKey(CString StrKey, CString StrValue)
{
	int nFirstIndex = StrKey.Find('\\');
	int nLastIndex = StrKey.ReverseFind('\\');
	if (nFirstIndex < 0)
		return FALSE;
	CString StrFirstKey = StrKey.Left(nFirstIndex);
	CString StrLastKey = StrKey.Mid(nLastIndex + 1);
	CString StrSec = StrKey.Mid(nFirstIndex + 1, nLastIndex - nFirstIndex - 1);
	HKEY hKey;
	if (StrFirstKey == "HKEY_CLASSES_ROOT")
		hKey = HKEY_CLASSES_ROOT;
	else if (StrFirstKey == "HKEY_CURRENT_USER")
		hKey = HKEY_CURRENT_USER;
	else if (StrFirstKey == "HKEY_LOCAL_MACHINE")
		hKey = HKEY_LOCAL_MACHINE;
	else if (StrFirstKey == "HKEY_USERS")
		hKey = HKEY_USERS;
	else if (StrFirstKey == "HKEY_CURRENT_CONFIG")
		hKey = HKEY_CURRENT_CONFIG;
	else
		return FALSE;

	DWORD Reserved = 0;
	DWORD OpenStatus;
	HKEY hNext;
	long nRet = ::RegCreateKeyEx(hKey, StrSec, Reserved, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hNext, &OpenStatus);
	if (nRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	unsigned char* p = new unsigned char[2048];
	strcpy((char*)p, (const char *)StrValue);
	nRet = ::RegSetValueEx(hNext, StrLastKey, NULL, REG_SZ, p, StrValue.GetLength());
	if (nRet != ERROR_SUCCESS)
	{
		delete [] p;
		::RegCloseKey(hNext);
		return FALSE;
	}
	delete [] p;

	::RegCloseKey(hNext);

	return TRUE;
}
