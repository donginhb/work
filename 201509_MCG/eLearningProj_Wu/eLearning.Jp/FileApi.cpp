#include "stdafx.h"
#include "FileApi.h"
#include "cmkapi.h"
#include "Macro.h"
#include <stdio.h>

BOOL WINAPI WriteTemplateFile(CString strFileName, CString strCourse, 
			 				  CStringArray& strsTag, CStringArray& strsReplace)
{
	BOOL bRet = TRUE;

	CString strSrcPath = MACRO_TEMPLATE_PATH + strFileName;
	CString strDestPath = MACRO_COURSE_PATH + strCourse + "\\html\\" + strFileName;

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
				strBuf = strBuf.Left(nIndex) + strReplace + 
						 strBuf.Mid(nIndex + strFind.GetLength());
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

	return bRet;
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

BOOL WINAPI FileContainString(CString strSrcPath, CString strMatch)
{
	BOOL bRet = FALSE;

	FILE *stream_in;
	if ((stream_in = fopen((LPCTSTR)strSrcPath, "r" )) == NULL)
		return FALSE;

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		return FALSE;
	}

	CString strBuf = pline;// + CString("\r\n");
	while (TRUE)
	{
		int nIndex = strBuf.Find(strMatch);
		if (nIndex >= 0)
		{	
			bRet = TRUE;
			break;
		}

		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		strBuf = pline;// + CString("\r\n");
	}

	fclose(stream_in);
	delete [] pline;

	return bRet;
}

BOOL WINAPI UpdateFileByStartEnd(CString strPath, CString strFile, 
			 					 CString strStart, CString strEnd, 
								 CStringArray& strsReplace)
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
	while (bRet)
	{
		if (strBuf.Left(strStart.GetLength()) == strStart)
		{
			if (fputs(strBuf, stream_out) == EOF)
			{
				bRet = FALSE;
				break;
			}

			while (fgets(pline, 4096, stream_in) != NULL)
			{
				strBuf = pline;
				if (strBuf.Left(strEnd.GetLength()) == strEnd)
				{
					for (int i = 0; i < strsReplace.GetSize(); i++)
					{
						strBuf = strsReplace[i];
						if (fputs(strBuf, stream_out) == EOF)
						{
							bRet = FALSE;
							break;
						}
					}
					if (fputs(strEnd + "\n", stream_out) == EOF)
					{
						bRet = FALSE;
						break;
					}
					
					bUpdate = TRUE;
					
					break;
				}
			}
		}
		else
		{
			if (fputs(strBuf, stream_out) == EOF)
			{
				bRet = FALSE;
				break;
			}
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

CString WINAPI GetMarkSeq(CString strCourse)
{
	CString strOutlinePath = MACRO_OUTLINE_FILE(strCourse);
	if (!::FileExists(strOutlinePath))
		return "";
 
	CStringArray strArray;
	if (!::LoadFile(strOutlinePath, strArray, TRUE))
		return "";
	strArray.RemoveAt(0);

	CString strRet = "";
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		char szBuf[32];
		strRet += "new markList(\"" + strArray[i] + "\"," + itoa(i + 1, szBuf, 10) + "),\n";
	}
	if (strRet != "")
		strRet = strRet.Left(strRet.GetLength() - 2);
	return strRet;
}

CString WINAPI GetFirstHTMLFile(CString strCourse)
{
//	CString strIniPath = MACRO_COURSE_PATH + strCourse + "\\Course.ini";
//	char szBuf[33];
//	::GetPrivateProfileString("General", "FirstFile", "", szBuf, 32, strIniPath);

	CString strFileName = "";
	CString strBuf = ""; //szBuf;
	if (strBuf == "")
	{
		CString strJSPath = MACRO_COURSE_PATH + strCourse + "\\html\\script.js";
		if (!::FileExists(strJSPath))
			return "";

		FILE *stream;
		if ((stream = fopen((LPCTSTR)strJSPath, "r" )) == NULL )
			return "";
		char* pline = new char[4096];
		if (fgets(pline, 4096, stream) == NULL)
		{
			delete [] pline;
			fclose(stream);
			return "";
		}
	
		strBuf = pline;
		CString strFind = "new hrefList(\"";
		while (TRUE)
		{
			int nIndex = strBuf.Find(strFind);
			if (nIndex >= 0)
			{
				nIndex += strFind.GetLength();
				CString strRemain = strBuf.Mid(nIndex);
				nIndex = strRemain.Find("\"");
				strFileName = strRemain.Mid(0, nIndex);
				break;
			}

			if (fgets(pline, 4096, stream) == NULL)
				break;
			strBuf = pline;
		}
	
		fclose(stream);
		delete [] pline;
	}
	else
		return strBuf;

	//if (strFileName != "")
	//	::WritePrivateProfileString("General", "FirstFile", strFileName, strIniPath);
	
	return strFileName;
}

BOOL WINAPI ParseForVideo(CString strSrcPath, 
						  CStringArray& strArrayFile, 
						  CStringArray& strArrayOldLine, 
						  CStringArray& strArrayNewLine,
						  CStringArray& strArrayTypes)
{
	FILE *stream_in;
	if ((stream_in = fopen((LPCTSTR)strSrcPath, "r" )) == NULL)
		return FALSE;

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		return FALSE;
	}

	CString strBuf = pline;
	while (TRUE)
	{
		CString strLower = strBuf;
		strLower.MakeLower();
		BOOL bType1 = (strLower.Find("<param name=") >= 0 && strLower.Find("value=") >= 0);
		BOOL bType2 = (strLower.Find("<oa:video src=\"") >= 0);
		if (bType1 || bType2)
		{
			BOOL bFound = FALSE;
			CString strType;
			for (int i = 0; i < strArrayTypes.GetSize(); i++)
			{
				strType = strArrayTypes.GetAt(i);
				if (strLower.Find("." + strType) >= 0)
				{
					bFound = TRUE;
					break;
				}
			}

			if (bFound)
			{	
				int nV, nAVI;
				CString strFileName;
				if (bType1)
				{
					nV = strLower.Find("value=");
					nAVI = strLower.Find("." + strType);
					strFileName = strBuf.Mid(nV + 6, nAVI - nV - 6);
				}
				else
				{
					nV = strLower.Find("<oa:video src=\"");
					nAVI = strLower.Find("." + strType + "\"");
					strFileName = strBuf.Mid(nV + 15, nAVI - nV - 15);
				}

				BOOL bF = FALSE;
				CString strPath = strFileName + "." + strType;
				for (int k = 0; k < strArrayFile.GetSize(); k++)
				{
					CString strFile = strArrayFile[k];
					if (strPath.CompareNoCase(strFile) == 0)
					{
						bF = TRUE;
						break;
					}
				}
				if (!bF)
					strArrayFile.Add(strPath);

				CString strFilePart = strBuf.Left(nAVI);
				//strFilePart.MakeLower();
				CString strNewLine = strFilePart + ".mp4" + 
									 strBuf.Mid(nAVI + strType.GetLength() + 1);
				strNewLine.TrimLeft();
				strNewLine.TrimRight();
				strArrayNewLine.Add(strNewLine);

				CString strOldLine = strBuf;
				strOldLine.TrimLeft();
				strOldLine.TrimRight();
				strArrayOldLine.Add(strOldLine);
			}
		}

		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		strBuf = pline;
	}

	fclose(stream_in);
	delete [] pline;

	return TRUE;
}

BOOL WINAPI ParseForAudio(CString strSrcPath, 
						  CStringArray& strArrayFile, 
						  CStringArray& strArrayOldLine, 
						  CStringArray& strArrayNewLine,
						  CStringArray& strArrayTypes)
{
	FILE *stream_in;
	if ((stream_in = fopen((LPCTSTR)strSrcPath, "r" )) == NULL)
		return FALSE;

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		return FALSE;
	}

	CString strBuf = pline;
	while (TRUE)
	{
		CString strLower = strBuf;
		strLower.MakeLower();

		BOOL bFound = FALSE;
		CString strType;
		for (int i = 0; i < strArrayTypes.GetSize(); i++)
		{
			strType = strArrayTypes.GetAt(i);
			if (strLower.Find("." + strType + "'") >= 0)
			{
				bFound = TRUE;
				break;
			}
		}

		if (bFound)
		{	
			CString strTag = "." + strType + "'";
			int nV = strLower.Find(strTag);
			int nIndex = 0;
			for (int i = nV - 1; i >= 0; i--)
			{
				char ch = strLower[i];
				if (!((ch >= '0' && ch <= '9') ||
					 (ch >= 'a' && ch <= 'z') || 
					 (ch >= 'A' && ch <= 'Z')))
				{
					nIndex = i + 1;
					break;
				}
			}
			
			CString strFileName = strBuf.Mid(nIndex, nV - nIndex);
			BOOL bF = FALSE;
			CString strPath = strFileName + "." + strType;
			for (int k = 0; k < strArrayFile.GetSize(); k++)
			{
				CString strFile = strArrayFile[k];
				if (strPath.CompareNoCase(strFile) == 0)
				{
					bF = TRUE;
					break;
				}
			}
			if (!bF)
				strArrayFile.Add(strPath);


			CString strFilePart = strBuf.Left(nIndex);
			//strFilePart.MakeLower();
			CString strNewLine = strFilePart + strFileName + ".mp4" + 
				                 strBuf.Mid(nV + strType.GetLength() + 1);
			strNewLine.TrimLeft();
			strNewLine.TrimRight();
			strArrayNewLine.Add(strNewLine);

			CString strOldLine = strBuf;
			strOldLine.TrimLeft();
			strOldLine.TrimRight();
			strArrayOldLine.Add(strOldLine);
		}

		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		strBuf = pline;
	}

	fclose(stream_in);
	delete [] pline;

	return TRUE;
}

CString WINAPI GetFileCreateTime(CString strFileName)
{
	CFileFind cFind;

	cFind.FindFile(strFileName);
	cFind.FindNextFile();

	CTime fTime;
	cFind.GetCreationTime(fTime);
	
	cFind.Close();

	int nYear = fTime.GetYear();
	int nMonth = fTime.GetMonth();
	int nDay = fTime.GetDay();
	CString strRet;
	strRet.Format("%d-%02d-%02d", nYear, nMonth, nDay);

	return strRet;
}

//add by buxaingyi 

void WINAPI ConvertCodePage(CString &str,UINT srcCodePage,UINT destCodePage)
{
	int nLen = ::MultiByteToWideChar(srcCodePage, MB_ERR_INVALID_CHARS, (LPCTSTR)str, -1, NULL, 0);

	wchar_t *wszUnicode = new wchar_t[nLen + 1];

	memset(wszUnicode, 0, nLen * 2 + 2);

	nLen = MultiByteToWideChar(srcCodePage, 0, (LPCTSTR)str, -1, wszUnicode, nLen);

	nLen = WideCharToMultiByte(destCodePage, 0, wszUnicode, -1, NULL, 0, NULL, NULL);

	char *szMultiByte = new char[nLen + 1];

	memset(szMultiByte, 0, nLen + 1);

	WideCharToMultiByte (destCodePage, 0, wszUnicode, -1, szMultiByte, nLen, NULL, NULL);

	str = szMultiByte;

	delete wszUnicode;

	delete szMultiByte;
}
