#if !defined(AFX_PPTSYNCWND_H__0FBD28E6_B166_433D_B771_110DCA4F331B__INCLUDED_)
#define AFX_PPTSYNCWND_H__0FBD28E6_B166_433D_B771_110DCA4F331B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPTSyncWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPTSyncWnd window
#define WM_SYNC_WND_CLOSED (WM_APP + 1739)
#define WM_ADD_PPT_POINT  (WM_APP + 1740)	
class CPPTWnd;
class CPPTSyncWnd : public CWnd
{
// Construction
public:
	CPPTSyncWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPTSyncWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPPTSyncWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPPTSyncWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnChangeSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddPPTRecord(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPPTWnd*		m_pPPTWnd;
	BOOL			m_bSamePage;
	int				m_nPreSlideNo;
	int				m_nPreAnimaNo;
    HICON           m_hIcon; //leu

public:
	CString			m_strPPTPath;
	CWnd*			m_pMSGWnd;
	CStringArray	m_strArrayTitle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTSYNCWND_H__0FBD28E6_B166_433D_B771_110DCA4F331B__INCLUDED_)
