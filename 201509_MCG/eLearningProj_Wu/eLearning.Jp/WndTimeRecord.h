#if !defined(AFX_WNDTIMERECORD_H__C01C150C_C042_442C_BA36_4DBCB52D6F44__INCLUDED_)
#define AFX_WNDTIMERECORD_H__C01C150C_C042_442C_BA36_4DBCB52D6F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndTimeRecord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndTimeRecord window

#define WM_TIME_LEFT (WM_APP + 209)
#define WM_TIME_OVER (WM_APP + 210)




class CWndTimeRecord : public CWnd
{
// Construction
public:
	CWndTimeRecord(CString strRegPath);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndTimeRecord)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndTimeRecord();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndTimeRecord)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	UINT	m_nTimerID;
	CString m_strRegPath;
	CString m_strFormerRegPath;

	long	m_nTimeLeft;
	DWORD   m_dwTickCount;
	int		m_nMin;
	CString m_strKey;
	CString m_strFormerKey;
	BOOL	m_bKeyExist;

public:
	void Activate();
	long GetTimeLeft() {return m_nTimeLeft;};
	void SetTimeLeft(long nTimeLeft) {m_nTimeLeft = nTimeLeft;};
	void SetRegPath(CString strReg){m_strRegPath = strReg;};
	BOOL GetKey(CString& strInstallKey); 
	BOOL GetFormerKey(CString& strInstallKey); 
	void SetstrFormerRegPath(CString strFormerRegPath);

	BOOL KeyDecode(CString strKey, CString& strSN);
	void GetKeyInfo(CString& strType, long& nTime) 
			{ strType = m_strKey.Left(2);  nTime = m_nTimeLeft;};
	BOOL KeyExist() {return m_bKeyExist;};
	BOOL PromoteToFormal();
	void StopTimer();
	void RestartTimer();
	void TimerAction(); 
	void WriteTimeLeft(); 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDTIMERECORD_H__C01C150C_C042_442C_BA36_4DBCB52D6F44__INCLUDED_)
