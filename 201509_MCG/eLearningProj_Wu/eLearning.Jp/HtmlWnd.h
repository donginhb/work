#if !defined(AFX_HTMLWND_H__CD280538_F3B1_11D4_BE14_0020AF3085DB__INCLUDED_)
#define AFX_HTMLWND_H__CD280538_F3B1_11D4_BE14_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd window

class CHtmlWnd : public CWnd
{
// Construction
public:
	CHtmlWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHtmlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHtmlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString GetLocationURL() const;
	void Navigate2(LPCTSTR lpszURL, BOOL bReCreate = FALSE,
				   DWORD dwFlags = 0,
				   LPCTSTR lpszTargetFrameName = NULL,
				   LPCTSTR lpszHeaders = NULL,
				   LPVOID lpvPostData = NULL, 
				   DWORD dwPostDataLen = 0);

	void Refresh(){ ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Refresh(); };
	void Stop()	{ ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Stop(); };
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd); 
	void StartHook(int nHook);
	HWND GetIEWnd();

protected:
	BOOL CreateHTMLWnd();
	void UnSetHook();
	BOOL SetHook();
	//static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

public:
	IWebBrowser2* m_pBrowserApp;
	CWnd* m_pwndBrowser;
	HWND m_hWndHTML;

protected:
	static HHOOK  m_hHook;
	static HINSTANCE m_hmodHook;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLWND_H__CD280538_F3B1_11D4_BE14_0020AF3085DB__INCLUDED_)
