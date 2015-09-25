
#include "stdafx.h"
#include "eLearning.h"
#include "MediaUtils.h"
#include "cmkapi.h"
#include "Mainfrm.h"
#include "ProgressWnd.h"
#include "Macro.h"
#include "fileapi.h"
#include <TCHAR.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <STDDEF.H >
#include <wchar.h>
#include <fstream>
#include <afx.h>

using namespace std;
//#include "..\MediaUtility\MediaUtility.h"

CMediaUtils::CMediaUtils()
{
	m_pEditor = NULL;
	m_pHeaderInfo = NULL;
	m_bOpened = FALSE;
	m_bCreated = FALSE;
	m_strFileName = "";
}

BOOL CMediaUtils::Create()
{
	HRESULT hr;

//	if(FAILED(CoInitialize(NULL))) 
//	{
//		SHOW_ERROR(IDS_COM_INIT_ERR);
//		return -1;
//	}

	hr = WMCreateEditor(&m_pEditor);
	if FAILED(hr)
	{
		m_strErr.LoadString(IDS_ERR_META_DATA);
		return FALSE;
	};

	hr = m_pEditor->QueryInterface(IID_IWMHeaderInfo,(void**)&m_pHeaderInfo);
	if FAILED(hr)
	{
		m_strErr.LoadString(IDS_ERR_HEAD_INFO);
		return FALSE;
	};

	m_bCreated = TRUE;
	return TRUE;
};

CMediaUtils::~CMediaUtils()
{
	if (m_pEditor)
		m_pEditor->Close();
	SAFE_RELEASE(m_pEditor);
    SAFE_RELEASE(m_pHeaderInfo);
//	CoUninitialize();
};

IWMMetadataEditor* CMediaUtils::GetMediaEditor()
{
	return m_pEditor;
};

IWMHeaderInfo* CMediaUtils::GetHeaderInfo()
{
	return m_pHeaderInfo;
};

BOOL CMediaUtils::Open(LPCTSTR FileName)
{  
	ASSERT(m_bCreated);

	WCHAR* pwFileName = new WCHAR[_MAX_PATH];
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, FileName, -1, pwFileName, _MAX_PATH);

	if (m_pEditor != NULL)
	{
		HRESULT hr = m_pEditor->Open(pwFileName); 
		if (SUCCEEDED(hr))
		{
			m_bOpened = TRUE;
			m_strFileName = FileName;
			delete [] pwFileName;
			return TRUE;
		}
		else 
		{   
			m_strErr.LoadString(IDS_ERR_EDITOR_OPEN);
			delete [] pwFileName;
			return FALSE; 
		}
	} 

	delete [] pwFileName;
	return FALSE;
};

BOOL CMediaUtils::AddScript(LPCTSTR lpstrType, LPCTSTR lpstrCmd, 
							 int hours, int minutes, double secs)
{ 
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WCHAR* wType = new WCHAR[_MAX_PATH];
	WCHAR* wCmd = new WCHAR[_MAX_PATH];
	QWORD NanoSeconds;
	BOOL br = TRUE;
	HRESULT hr;

	int r = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpstrType, -1, wType, _MAX_PATH); 
	if (r == 0)
	{
		br = FALSE;
		goto finish;
	};

	r = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpstrCmd, -1, wCmd, _MAX_PATH); 
	if (r == 0)
	{
		br = FALSE;
		goto finish;
	};

	NanoSeconds = (QWORD)((hours * 3600 + minutes * 60 + secs) * 10000000);
	if (m_pHeaderInfo != NULL)
	{
		hr = m_pHeaderInfo->AddScript(wType, wCmd, NanoSeconds);
		if (FAILED(hr))
			br = FALSE;
	} 
	else 
	   br = FALSE;

finish:
	delete [] wType;
	delete [] wCmd;
	return br;
};

BOOL CMediaUtils::AddScript(LPCTSTR lpstrType, LPCTSTR lpstrCmd, LONGLONG llTime)
{ 
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WCHAR* wType = new WCHAR[_MAX_PATH];
	WCHAR* wCmd = new WCHAR[_MAX_PATH];
	BOOL br = TRUE;
	HRESULT hr;

	int r = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpstrType, -1, wType, _MAX_PATH); 
	if (r == 0)
	{
		br = FALSE;
		goto finish;
	};

	r = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpstrCmd, -1, wCmd, _MAX_PATH); 
	if (r == 0)
	{
		br = FALSE;
		goto finish;
	};

	if (m_pHeaderInfo != NULL)
	{
		hr = m_pHeaderInfo->AddScript(wType, wCmd, llTime);
		if (FAILED(hr))
			br = FALSE;
	} 
	else 
	   br = FALSE;

finish:
	delete [] wType;
	delete [] wCmd;
	return br;
};

BOOL CMediaUtils::RemoveScript(WORD idx)
{ 
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD lIdx;
	BOOL br = TRUE;

	HRESULT hr = m_pHeaderInfo->GetScriptCount(&lIdx);
	if (SUCCEEDED(hr)) 
	{
		if ((idx >= 0) && (idx <= lIdx - 1))
		{
			hr = m_pHeaderInfo->RemoveScript(idx);
			if (SUCCEEDED(hr))
				br = Flush();
			else
				br = FALSE;
		} 
		else 
			br = FALSE;
	} 
	else 
		br = FALSE;   

	return br;
};

BOOL CMediaUtils::AddMarker(LPCTSTR lpcMarkerName, LONGLONG llTime)
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WCHAR*  pwMarkerName = new WCHAR[_MAX_PATH];
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpcMarkerName,
						  -1, pwMarkerName, _MAX_PATH);

	BOOL  br = FALSE;
	HRESULT hr = m_pHeaderInfo->AddMarker(pwMarkerName, llTime);
	if (SUCCEEDED(hr))
		br = TRUE;

	delete [] pwMarkerName;

	return br;
}

BOOL CMediaUtils::AddMarker(LPCTSTR lpcMarkerName, int hours, int minutes, double secs)
{  
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WCHAR*  pwMarkerName = new WCHAR[_MAX_PATH];
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpcMarkerName,
						  -1, pwMarkerName, _MAX_PATH);
	QWORD NanoSeconds = (QWORD)((hours * 3600 + minutes * 60 + secs) * 10000000);
	
	BOOL  br = FALSE;
	HRESULT hr = m_pHeaderInfo->AddMarker(pwMarkerName, NanoSeconds);
	if (SUCCEEDED(hr))
		br = TRUE;

	delete [] pwMarkerName;

	return br;
};

BOOL CMediaUtils::RemoveMarker(WORD wIndex)
{ 
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD pcMarker;
	BOOL br = TRUE;
	HRESULT hr = m_pHeaderInfo->GetMarkerCount(&pcMarker);
	if (SUCCEEDED(hr))
	{
		if ((wIndex >= 0) && (wIndex <= pcMarker))
		{
			hr = m_pHeaderInfo->RemoveMarker(wIndex);
			if (SUCCEEDED(hr))
				br = Flush();
		} 
		else
			br = FALSE;
	} 
	else 
		br = FALSE;

	return br;
   

};

BOOL CMediaUtils::RemoveAllMarkers(BOOL bFlush)
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD pcMarkers;

	HRESULT hr = m_pHeaderInfo->GetMarkerCount(&pcMarkers);
	if (SUCCEEDED(hr)) 
	{
		for (int i = pcMarkers - 1; i >= 0; i--)
		{
			hr = m_pHeaderInfo->RemoveMarker(i);
			if (FAILED(hr))
				return FALSE;
		};
	} 
	else 
		return FALSE;

	if (bFlush)
		return Flush();

	return TRUE;
};

BOOL CMediaUtils::RemoveAllScripts(BOOL bFlush)
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD pcMarkers;

	HRESULT hr = m_pHeaderInfo->GetScriptCount(&pcMarkers);
	if (SUCCEEDED(hr)) 
	{
		for (int i = pcMarkers - 1; i >= 0; i--)
		{
			hr = m_pHeaderInfo->RemoveScript((WORD)i);
			if (FAILED(hr))
				return FALSE;
		};
	} 
	else 
		return FALSE;

	if (bFlush)
		return Flush();

	return TRUE;
};

BOOL CMediaUtils::GetScriptNames(CStringArray* pstrArrayType, 
								  CStringArray* pstrArrayCmd, 
								  CStringArray* pstrArrayTime)
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	HRESULT hr = S_OK ;
	
 	WORD cScript = 0 ;
	WCHAR *pwszType = NULL ;
	WORD cchTypeLen = 0 ;
	WCHAR *pwszCommand = NULL ;
	WORD cchCommandLen = 0 ;
	QWORD cnsScriptTime = 0 ;
	
	hr = m_pHeaderInfo->GetScriptCount(&cScript);
	if (FAILED(hr))
	{
		DebugOutput(_T("GetScriptCount failed (hr=0x%08x).\n" ), hr) ;
		return hr;
	}

	if (pstrArrayType)
		pstrArrayType->RemoveAll();

	if (pstrArrayCmd)
		pstrArrayCmd->RemoveAll();
	
	if (pstrArrayTime)
		pstrArrayTime->RemoveAll();

	CString strUndefined = "";
	//strUndefined.LoadString(IDS_UNDEFINED);
	for (WORD j = 0; j < cScript; j++)
	{
		if (pstrArrayType)
			pstrArrayType->Add(strUndefined);

		if (pstrArrayCmd)
			pstrArrayCmd->Add(strUndefined);

		if (pstrArrayTime)
			pstrArrayTime->Add(strUndefined);
	}

	for( int i = 0 ; i < cScript ; i++)
	{
		//
		// Get the memory reqd for this script
		//
		hr = m_pHeaderInfo->GetScript(i,
									  NULL,
									  &cchTypeLen,
									  NULL,
									  &cchCommandLen,
									  &cnsScriptTime) ;
		if (FAILED(hr))
		{
			DebugOutput( _T("GetScript failed for Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}
		
		pwszType = new WCHAR[cchTypeLen];
		pwszCommand = new WCHAR[cchCommandLen];
		if (pwszType == NULL || pwszCommand == NULL)
		{
			hr = E_OUTOFMEMORY ;
			break ;
		}
		//
		// Now, get the script
		//
		hr = m_pHeaderInfo->GetScript(i,
									  pwszType,
									  &cchTypeLen,
									  pwszCommand,
									  &cchCommandLen,
									  &cnsScriptTime);
		if (FAILED(hr))
		{
			DebugOutput( _T("GetScript failed for Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}
		
		char szScript[256];
		int nRet;
		if (pstrArrayType)
		{
			nRet = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pwszType, cchTypeLen,
									   szScript, 256, NULL, NULL);
			if (nRet)
				pstrArrayType->SetAt(i, szScript);
		}

		if (pstrArrayCmd)
		{
			nRet = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pwszCommand, cchCommandLen,
									   szScript, 256, NULL, NULL);
			if (nRet)
				pstrArrayCmd->SetAt(i, szScript);
		}

		if (pstrArrayTime)
			pstrArrayTime->SetAt(i, ConvertLLTimeToStr(cnsScriptTime));

		delete[] pwszType ;
		delete[] pwszCommand ;
		pwszType = pwszCommand = NULL ;
		
		cchTypeLen = 0;
		cchCommandLen = 0 ;
	}
	
	delete[] pwszType ;
	delete[] pwszCommand ;
	pwszType = pwszCommand = NULL ;
	
	return SUCCEEDED(hr);
}

BOOL CMediaUtils::GetMarkerNames(CStringArray* pstrArrayMarkers, 
								 CStringArray* pstrArrayTime)

{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD cMarker = 0 ;
	HRESULT hr = m_pHeaderInfo->GetMarkerCount(&cMarker);

	CString strUndefined = "";
	//strUndefined.LoadString(IDS_UNDEFINED);
	
	if (SUCCEEDED(hr))
	{
		if (pstrArrayMarkers)
			pstrArrayMarkers->RemoveAll();
		if (pstrArrayTime)
			pstrArrayTime->RemoveAll();
		for (WORD j = 0; j < cMarker; j++)
		{
			if (pstrArrayMarkers)
				pstrArrayMarkers->Add(strUndefined);
			if (pstrArrayTime)
				pstrArrayTime->Add(strUndefined);
		}

		WCHAR *pwszMarkerName = NULL ;
		WORD cchMarkerNameLen = 0 ;
		QWORD cnsMarkerTime = 0;
		for( int i = 0 ; i < cMarker ; i++)
		{
			//
			// Get the memory reqd for this Marker
			//
			hr = m_pHeaderInfo->GetMarker( i ,
										   NULL ,
										   &cchMarkerNameLen ,
										   &cnsMarkerTime) ;
			if (FAILED(hr))
			{
				::DebugOutput(_T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}
			
			pwszMarkerName = new WCHAR[cchMarkerNameLen] ;
			if( pwszMarkerName == NULL )
			{
				hr = E_OUTOFMEMORY ;
				break ;
			}

			hr = m_pHeaderInfo->GetMarker( i ,
										   pwszMarkerName ,
										   &cchMarkerNameLen ,
										   &cnsMarkerTime) ;
			if (FAILED(hr))
			{
				::DebugOutput( _T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}

			if (pstrArrayMarkers)
			{
				char szMarkName[256];
				int nRet = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pwszMarkerName, 
									cchMarkerNameLen, szMarkName, 256, NULL, NULL);
				if (nRet)
					pstrArrayMarkers->SetAt(i, szMarkName);
			}

			if (pstrArrayTime)
				pstrArrayTime->SetAt(i, ConvertLLTimeToStr(cnsMarkerTime));

			delete[] pwszMarkerName ;
			pwszMarkerName = NULL ;
			cchMarkerNameLen = 0 ;
		}
		delete[] pwszMarkerName ;
		pwszMarkerName = NULL ;

		if(FAILED(hr))
			return FALSE;
	};
 
	return TRUE;
}

BOOL CMediaUtils::GetMarkerTimeByName(CString strMarkerName, QWORD& nPos)

{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	WORD cMarker = 0 ;
	HRESULT hr = m_pHeaderInfo->GetMarkerCount(&cMarker);
	if (SUCCEEDED(hr))
	{
		WCHAR *pwszMarkerName = NULL ;
		WORD cchMarkerNameLen = 0 ;
		QWORD cnsMarkerTime = 0;
		for( int i = 0 ; i < cMarker ; i++)
		{
			//
			// Get the memory reqd for this Marker
			//
			hr = m_pHeaderInfo->GetMarker( i ,
										   NULL ,
										   &cchMarkerNameLen ,
										   &cnsMarkerTime) ;
			if (FAILED(hr))
			{
				::DebugOutput(_T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}
			
			pwszMarkerName = new WCHAR[cchMarkerNameLen] ;
			if( pwszMarkerName == NULL )
			{
				hr = E_OUTOFMEMORY ;
				break ;
			}

			hr = m_pHeaderInfo->GetMarker( i ,
										   pwszMarkerName ,
										   &cchMarkerNameLen ,
										   &cnsMarkerTime) ;
			if (FAILED(hr))
			{
				::DebugOutput( _T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}

			char szMarkName[256];
			int nRet = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pwszMarkerName, 
								cchMarkerNameLen, szMarkName, 256, NULL, NULL);
			if (nRet)
			{
				if (strcmp(szMarkName, strMarkerName) == 0)
				{
					nPos = cnsMarkerTime;
					delete[] pwszMarkerName ;
					pwszMarkerName = NULL ;
					return TRUE;
				}
			}

			delete[] pwszMarkerName ;
			pwszMarkerName = NULL ;
			cchMarkerNameLen = 0 ;
		}
		delete[] pwszMarkerName ;
		pwszMarkerName = NULL ;
	};
 
	return FALSE;
}

BOOL CMediaUtils::IsMarked()
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	ASSERT(m_pHeaderInfo);
	
	WORD   wMarkerCount;
	HRESULT hr = m_pHeaderInfo->GetMarkerCount(&wMarkerCount);
	if (SUCCEEDED(hr))
		return wMarkerCount != 0;
	return FALSE;
}

BOOL CMediaUtils::IsScripted()
{
	ASSERT(m_bCreated);
	ASSERT(m_bOpened);

	ASSERT(m_pHeaderInfo);
	
	WORD   wScriptCount;
	HRESULT hr = m_pHeaderInfo->GetScriptCount(&wScriptCount);
	if (SUCCEEDED(hr))
		return wScriptCount != 0;
	return FALSE;
}

BOOL CMediaUtils::MergeOutline(CStringArray& strArrayOutLine, CStringArray& strArrayStep)
{
	CStringArray strArrayTime;
	CStringArray strArraySlide;
	CStringArray strArrayOutlineCopy;
	int i = 0;
	for (i = 0; i < strArrayStep.GetSize(); i++)
	{
		CString str = strArrayStep[i];

		int nIdx = str.Find(' ');
		if (nIdx < 0)
			continue;

		CString strTime = str.Left(nIdx);

		CString strRemain = str.Mid(nIdx + 1);
		int nIdx2 = strRemain.Find(' ');
		if (nIdx2 < 0)
			continue;
		CString strCmd = strRemain.Left(nIdx2);
		strCmd.TrimLeft();
		strCmd.TrimRight();
		if (strCmd != "flipslide" && strCmd != "flippage")
			continue;

		CString strSlide = strRemain.Mid(nIdx2);
		strSlide.TrimLeft();
		strSlide.TrimRight();

		if (strSlide.Find(".") < 0)
		{
			strArrayTime.Add(strTime);
			strArraySlide.Add(strSlide);
		}
	}

	for (i = 0; i < strArrayOutLine.GetSize(); i++)
		strArrayOutlineCopy.Add(strArrayOutLine[i]);

//	CString strUndefined;
//	strUndefined.LoadString(IDS_UNDEFINED);
	strArrayOutLine.RemoveAll();
	for (i = 0; i < strArraySlide.GetSize(); i++)
	{
		int nSlideSeq = atoi(strArraySlide[i]) - 1;
		int nOutlineNo = strArrayOutlineCopy.GetSize();
//		CString strOutline = strUndefined;
		CString strOutline = "";
		if (nSlideSeq >= 0 && nSlideSeq < nOutlineNo) 
			strOutline = strArrayOutlineCopy[nSlideSeq];
		strArrayOutLine.Add(strArrayTime[i] + " " + strOutline);
	}

	return TRUE;
}

BOOL CMediaUtils::TagASFBothWithTime(CStringArray& strArrayOutline, 
									 CStringArray& strArrayStep,
									 BOOL bFlush)
{
	CString strTitle;
	strTitle.LoadString(IDS_TAG_ASF);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	BOOL bMarked = IsMarked();

	if (bMarked)
	{
		BOOL b = RemoveAllMarkers(FALSE);
		b = RemoveAllScripts(TRUE);
	}

	int hh;
	int mm;
	double ss;
	CString strValue;
	double pre_ss = -1.0;
	int i = 0; 
	for (i = 0; i < strArrayOutline.GetSize(); i++)
	{
		CString strOutline = strArrayOutline[i];
		if (strOutline != "")
		{
			SeparateTimeStr(strOutline, hh, mm, ss, strValue);
			if (pre_ss == ss)
				ss = ss + 0.1;
			AddMarker(strValue, hh, mm, ss);
			pre_ss = ss;
		}
	}

	CString strCmd;
	pre_ss = -1.0;
	for (i = 0; i < strArrayStep.GetSize(); i++)
	{
		SeparateTimeStr2(strArrayStep[i], hh, mm, ss, strValue, strCmd);
		if (pre_ss == ss)
			ss = ss + 0.1;
		AddScript(strCmd, strValue, hh, mm, ss);
		pre_ss = ss;

		CString strTmp = strArrayStep[i];
		int nIdx = strTmp.Find(' ');
		if (nIdx >= 0)
		{
			CString strParam = strTmp.Mid(nIdx + 1);
			CString strTime;
			strTime.Format("%02d:%02d:%02d.%01d ", hh, mm, (int)ss, ((int)(ss * 10)) % 10);
			strArrayStep[i] = strTime + strParam;
		}
	}

	WriteTimeScript(strArrayStep);

	if (bFlush)
	{
		if (!Flush())
		{
			MACRO_MAINFRM->HideProgress();
			return FALSE;
		}
	}

	MACRO_MAINFRM->HideProgress();
	return TRUE;
}

BOOL CMediaUtils::WriteTimeScript()
{
	CStringArray strArrayType, strArrayCmd, strArrayTime;	
	GetScriptNames(&strArrayType, &strArrayCmd, &strArrayTime);
	CString strTmp;
	CStringArray strArrayScript;
	for (int i = 0; i < strArrayTime.GetSize(); i++)
	{
		strTmp = strArrayTime[i] + " " + strArrayType[i] + " " + strArrayCmd[i];
		strArrayScript.Add(strTmp);
	}

	return WriteTimeScript(strArrayScript);
}

BOOL CMediaUtils::WriteTimeScript(CStringArray& strArrayStep)
{
	CStringArray strArrayScript;
	int nSize = strArrayStep.GetSize();
	double* pdArrayTime = new double[nSize];
	CString strCmd;
	int nSlide = 0, nMarker;
	int hh;
	int mm;
	double ss;
	CString strValue;
	for (int i = 0; i < nSize; i++)
	{
		SeparateTimeStr2(strArrayStep[i], hh, mm, ss, strValue, strCmd);

		double fSec = hh * 3600 + mm * 60 + ss;

		pdArrayTime[i] = fSec;

		nMarker = -1;
		if ((strCmd == "flipslide" && strValue.Find(".") < 0) ||
			(strCmd == "flippage" && strValue.Find(".") < 0))
			nMarker = nSlide++;

		CString str;
		str.Format("new hScriptList(%.1f,\"%s\", \"%s\", %d),\n", fSec, strValue, strCmd, nMarker);
		strArrayScript.Add(str);
	}

	SortArray(pdArrayTime, nSize, strArrayScript);

	delete [] pdArrayTime;

	if (strArrayScript.GetSize() > 0)
	{
		CString strTmp = strArrayScript[strArrayScript.GetSize() - 1];
		if (strTmp.Right(2) == ",\n")
			strTmp = strTmp.Left(strTmp.GetLength() - 2) + "\n" ;
		strArrayScript[strArrayScript.GetSize() - 1] = strTmp;
	}
	strArrayScript.Add(");\n");
	strArrayScript.InsertAt(0, "var gScriptTable = new Array(\n");

	CString strFilePath = ::GetFilePath(m_strFileName);
	int nLength = strFilePath.GetLength();
	if (strFilePath.Right(13).CompareNoCase("\\html\\movie\\") == 0)
		strFilePath = strFilePath.Left(nLength - 13) + "\\html";
	else if (strFilePath.Right(7).CompareNoCase("\\video\\") == 0)
		strFilePath = strFilePath.Left(nLength - 7) + "\\html";
	if (::FileExists(strFilePath + "\\_scriptfunc.js"))
	{
		::UpdateFileByStartEnd(strFilePath, "_scriptfunc.js", "//@@@StartPos", "//@@@EndPos", 
							   strArrayScript);
	}

	return TRUE;
}


void CMediaUtils::SortArray(double* pdArrayTime, int nSize, CStringArray& strArray)
{
	for (int i = 0; i < nSize; i++)
	{
		for (int j = i + 1; j < nSize; j++)
		{
			if (pdArrayTime[i] > pdArrayTime[j])
			{
				double dTmp = pdArrayTime[i];
				pdArrayTime[i] = pdArrayTime[j];
				pdArrayTime[j] = dTmp;
				
				CString strTmp = strArray[i];
				strArray[i] = strArray[j];
				strArray[j] = strTmp;
			}

		}
	}
}

BOOL CMediaUtils::SeparateTimeStr(CString str, 
								  int& hh, int& mm, double& ss, 
								  CString& strParam)
{
	str.TrimLeft();
	str.TrimRight();
	if (str == "")
		return FALSE;

	int nIdx = str.Find(' ');
	if (nIdx < 0)
		return FALSE;

	strParam = str.Mid(nIdx + 1);
	strParam.TrimLeft();
	strParam.TrimRight();

	CString strTime = str.Left(nIdx);
	ConvTime(strTime, hh, mm, ss);

	return TRUE;
}

BOOL CMediaUtils::SeparateTimeStr2(CString str, int& hh, int& mm, double& ss, 
								   CString& strParam, CString& strParam2)
{
	str.TrimLeft();
	str.TrimRight();
	if (str == "")
		return FALSE;

	int nIdx = str.Find(' ');
	if (nIdx < 0)
		return FALSE;

	strParam = str.Mid(nIdx + 1);
	strParam.TrimLeft();
	strParam.TrimRight();

	CString strTime = str.Left(nIdx);
	ConvTime(strTime, hh, mm, ss);

	nIdx = strParam.Find(' ');
	if (nIdx < 0)
		return FALSE;

	strParam2 = strParam.Left(nIdx);
	strParam = strParam.Mid(nIdx + 1);

	return TRUE;
}

void CMediaUtils::ConvTime(CString strTime, int& hh, int& mm, double& ss)
{
	CString strTok;
	int nCount = 0;
	::GetTokStr(strTime, strTok, nCount, ":");
	hh = atoi(strTok);

	::GetTokStr(strTime, strTok, nCount, ":");
	mm = atoi(strTok);

	::GetTokStr(strTime, strTok, nCount, ":");
	ss = atof(strTok);
}

BOOL CMediaUtils::Flush()
{ 
	if (!m_pEditor)
		return FALSE;

	HRESULT hr;
	hr = m_pEditor->Flush();
	
	return SUCCEEDED(hr);
}

CString CMediaUtils::ConvertLLTimeToStr(LONGLONG qwtime)
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
