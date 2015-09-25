#include "stdafx.h"
#include "jpgapi.h"   
#include "bmpapi.h"
#include "l_bitmap.h"
#include "l_error.h"
#include <io.h>

BOOL WINAPI ConvertImageToJPG(CString strSrcFile, CString strDestFile)
{
	if (strDestFile.Right(4).CompareNoCase(".jpg") != 0)
		return FALSE;

    if (_access(strSrcFile,0) != 0)   // Check for existence
		return FALSE;

	int nRet;
	BITMAPHANDLE bmpHandle;	

	if ((nRet = L_LoadBitmap((LPTSTR)(LPCTSTR)strSrcFile, &bmpHandle, 0, ORDER_BGR, NULL)) < 1)
		return FALSE;

	if ((nRet = L_SaveBitmap((LPTSTR)(LPCTSTR)strDestFile, &bmpHandle, FILE_JFIF, 24, 128, SAVE_OVERWRITE)) < 1)
		return FALSE;

	return TRUE;
}

BOOL WINAPI ConvertToJPG(CString strSrcFile, CString strDestFile)
{
	if (strDestFile.Right(4).CompareNoCase(".jpg") != 0)
		return FALSE;

    if (_access(strSrcFile,0) != 0)   // Check for existence
		return FALSE;

	BOOL bRet = FALSE;
	CBitmap* pBMP = ::LoadFile(strSrcFile);
	if (pBMP)
	{
		bRet =  SaveDDBToJPG(pBMP, strDestFile);
		pBMP->DeleteObject();
		delete pBMP;
	}

	//if (!bRet)
	//{
	//	CString strTmp, strMsg;
	//	strTmp.LoadString(IDS_CONVERT_ERROR);
	//	strMsg.Format(strTmp, strSrcFile, strDestFile);
	//	SHOW_ERROR_STR(strMsg);
	//}

	return bRet;
}

BOOL WINAPI SaveDDBToJPG(CBitmap* pBMP, CString sPath)
{
	if (sPath.Right(4).CompareNoCase(".jpg") != 0)
		return FALSE;

	BITMAP b;
	pBMP->GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;

	int nRet;
	BITMAPHANDLE bmpHandle;
	if ((nRet = L_InitBitmap(&bmpHandle, 0, 0, 0)) < 1)
		return FALSE;

	HDC hDC = ::GetDC(0);
	if ((nRet = L_ConvertFromDDB(hDC, &bmpHandle, (HBITMAP)pBMP->m_hObject, NULL)) < 1)
	{
		::ReleaseDC(0, hDC);
		L_FreeBitmap(&bmpHandle);
		return FALSE;
	}

	::ReleaseDC(0, hDC);

	if ((nRet = L_SaveBitmap((LPTSTR)(LPCTSTR)sPath, &bmpHandle, FILE_JFIF, 24, 2, SAVE_OVERWRITE)) < 1)
	{
		L_FreeBitmap(&bmpHandle);
		return FALSE;
	}

	L_FreeBitmap(&bmpHandle);

	return TRUE;
}

