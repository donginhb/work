// WndTimeRecord.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "WndTimeRecord.h"
#include "Registry.h"
#include "EncodeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ToUse "36000"
/////////////////////////////////////////////////////////////////////////////
// CWndTimeRecord

CWndTimeRecord::CWndTimeRecord(CString strRegPath)
{
	m_nTimerID = 0;
	m_strRegPath = strRegPath;
   

	m_nMin = 0;
	m_strKey = "";
	m_nTimeLeft = 0;
	//m_bKeyExist = FALSE;
	m_bKeyExist = TRUE;
	if (GetKey(m_strKey))
	{
		int nLen = m_strKey.GetLength();
		CString strID = m_strKey.Left(2);
		if (nLen == 22 && (strID == "pp" || strID == "ev" || strID == "bd" || strID == "pm"))
		{
			CString strTime = m_strKey.Mid(14, 8);
			m_nTimeLeft = atol(strTime);
			m_bKeyExist = TRUE;
		}
		else if (nLen == 14 && (strID == "pr" || strID == "po" || strID == "bf"))
		{
			m_bKeyExist = TRUE;
		}
		//WriteTimeLeft();
	}
    //GetFormerKey(m_strFormerKey);
	

	
}

CWndTimeRecord::~CWndTimeRecord()
{
}


BEGIN_MESSAGE_MAP(CWndTimeRecord, CWnd)
	//{{AFX_MSG_MAP(CWndTimeRecord)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndTimeRecord message handlers

int CWndTimeRecord::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_bKeyExist)
		return -1;

	return 0;
}

void CWndTimeRecord::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_nTimerID > 0)
		KillTimer(m_nTimerID);
}

void CWndTimeRecord::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimerID)
		TimerAction(); 

	CWnd::OnTimer(nIDEvent);
}

void CWndTimeRecord::TimerAction() 
{
	if (m_nTimeLeft == 0)
		return;

	DWORD dwTick = GetTickCount();
	int nSec = (dwTick - m_dwTickCount) / 1000;
	if (m_nTimeLeft - nSec <= 0)
	{
		m_nTimeLeft = 0;
		WriteTimeLeft();
		GetParent()->SendMessage(WM_TIME_OVER, 0, 0);
		return;
	}

	int nMin = nSec / 60;
	GetParent()->SendMessage(WM_TIME_LEFT, m_nTimeLeft - nSec, 0);

	if (m_nMin != nMin)
	{
		if (m_nTimeLeft - nSec > 0)
		{
			m_nTimeLeft -= nSec;
			m_dwTickCount = GetTickCount();
			WriteTimeLeft();
			m_nMin = nMin;
		}
	}
}

void CWndTimeRecord::Activate() 
{
	if (!::IsWindow(m_hWnd))
		return;

	if (!m_bKeyExist)
		return;

	CString strType = m_strKey.Left(2);
	if (strType != "pp" && strType != "ev" && strType != "bd" && strType != "pm")
		return;

	m_nTimerID = SetTimer(1, 30000, NULL);
	m_nMin = 0;
	m_dwTickCount = GetTickCount();

	TimerAction();
}

BOOL CWndTimeRecord::GetKey(CString& strInstallKey) 
{
	strInstallKey = "";

	CString strBuf;
	if (!::RegGetKey(m_strRegPath, strBuf))
	{
		return FALSE;
	}
	
	char szDate[256];
	::KeyDecode(strBuf, szDate);

	CString strDate = szDate;
	CString strID = strDate.Left(2);
	if (strID == "pp" || strID == "pr" || strID == "ev" || strID == "bd" || strID == "pm" || strID == "po" || strID == "bf")
		strInstallKey = szDate;
	else
		return FALSE;

	return TRUE;
}
BOOL CWndTimeRecord::GetFormerKey(CString& strInstallKey) 
{
	strInstallKey = "";

	CString strBuf;
	if (!::RegGetKey(m_strFormerRegPath, strBuf))
	{
		return FALSE;
	}
	
	char szDate[256];
	::KeyDecode(strBuf, szDate);

	CString strDate = szDate;
	CString strID = strDate.Left(2);
	if (strID == "pp" || strID == "pr" || strID == "ev" || strID == "bd" || strID == "pm" || strID == "po" || strID == "bf")
		strInstallKey = szDate;
	else
		return FALSE;

	return TRUE;
}
void CWndTimeRecord::SetstrFormerRegPath(CString strFormerRegPath)
{
    m_strFormerRegPath=strFormerRegPath;
  	if (GetFormerKey(m_strFormerKey))
	{
		int nLen = m_strFormerKey.GetLength();
		CString strID = m_strFormerKey.Left(2);
		if (nLen == 22 && (strID == "pp" || strID == "ev" || strID == "bd" || strID == "pm"))
		{
			CString strTime = m_strFormerKey.Mid(14, 8);
			m_nTimeLeft = atol(strTime);
			m_bKeyExist = TRUE;
		}
		else if (nLen == 14 && (strID == "pr" || strID == "po" || strID == "bf"))
		{
			m_bKeyExist = TRUE;
		}
		WriteTimeLeft();
	}
}


BOOL CWndTimeRecord::KeyDecode(CString strKey, CString& strSN)
{
	char szKey[128];
	::KeyDecode(strKey, szKey);
	strSN = szKey;

	return TRUE;
}

void CWndTimeRecord::WriteTimeLeft() 
{


	/*
	if (!m_strFormerRegPath)
		return;
    if (m_strKey=="")
	{
		CString strType = m_strFormerKey.Left(2);
		if (strType != "pp" && strType != "ev" && strType != "bd" && strType != "pm")
			return;
		
		//if (m_strRegPath == "")
		//	return;

		CString strValue;
		strValue.Format("%s%08d", m_strFormerKey.Left(14), 36000);

		char szKey[128];
		KeyEncode(strValue, szKey);
		CString strSN = szKey;
		if (strSN == "")
			return;
		
		::RegSetKey(m_strRegPath, strSN);
		return;

	}
    */
	if (!m_bKeyExist)
		return;
 
	CString strType = m_strKey.Left(2);
	if (strType != "pp" && strType != "ev" && strType != "bd" && strType != "pm")
		return;
	
	if (m_strRegPath == "")
		return;

	CString strValue;
	strValue.Format("%s%08d", m_strKey.Left(14), m_nTimeLeft);

	char szKey[128];
	KeyEncode(strValue, szKey);
	CString strSN = szKey;
	if (strSN == "")
		return;
	
	::RegSetKey(m_strRegPath, strSN);

  
}

BOOL CWndTimeRecord::PromoteToFormal()
{
	if (!m_bKeyExist)
		return FALSE;

	CString strKey = m_strKey.Left(2);
	if (strKey != "pp" && strKey != "pm" && strKey != "bd")
		return FALSE;
	
	if (m_strRegPath == "")
		return FALSE;

	CString strValue;
	if (::RegGetKey(m_strRegPath, strValue))
	{
		//return FALSE;
	}
	
	CTime ct = CTime::GetCurrentTime();
	if (strKey == "pp")
		strValue.Format("pr0100%d%02d%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay());
	else if (strKey == "pm")
		strValue.Format("po0100%d%02d%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay());
	else if (strKey == "bd")
		strValue.Format("bf0100%d%02d%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay());

	CString strSN = "";
	char szKey[128];
	KeyEncode(strValue, szKey);
	strSN = szKey;
	if (strSN == "")
		return FALSE;
	
	::RegSetKey(m_strRegPath, strSN);

	m_strKey = strValue;

	return TRUE;
}

void CWndTimeRecord::StopTimer()
{
	if ((int)m_nTimerID > 0)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = -1;
	}
}

void CWndTimeRecord::RestartTimer()
{
	if ((int)m_nTimerID < 0)
		m_nTimerID = SetTimer(1, 30000, NULL);
}
