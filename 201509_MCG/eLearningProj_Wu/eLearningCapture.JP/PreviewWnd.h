#if !defined(AFX_PREVIEWWND_H__1D4F4065_01BB_439A_9557_3E5F55ED5BCC__INCLUDED_)
#define AFX_PREVIEWWND_H__1D4F4065_01BB_439A_9557_3E5F55ED5BCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd window

#define WM_CLOSE_PREVIEW		(WM_APP + 199)
#define WM_SET_AUDIO_PREVIEW	(WM_APP + 200)
#define WM_GET_AUDIO_PREVIEW	(WM_APP + 201)

class CPreviewWnd : public CWnd
{
// Construction
public:
	CPreviewWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewWnd)
	virtual	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreviewWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPreviewVoice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString	m_strClass;
	CWnd*			m_pMSGWnd;
	CMenu*			m_pMenu;

public:
	BOOL m_bPreviewAudioMute;

protected:	
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

public:
	void SetMSGWnd(CWnd* pWnd) { m_pMSGWnd = pWnd; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWWND_H__1D4F4065_01BB_439A_9557_3E5F55ED5BCC__INCLUDED_)
