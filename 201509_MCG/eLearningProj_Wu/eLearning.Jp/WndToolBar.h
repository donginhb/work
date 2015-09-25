#if !defined(AFX_WNDTOOLBAR_H__951C593C_74A1_4110_92C7_ED2775B88FBC__INCLUDED_)
#define AFX_WNDTOOLBAR_H__951C593C_74A1_4110_92C7_ED2775B88FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndToolBar window
class CBmpBtn;
class CHtmlWnd;
class CWndToolBar : public CWnd
{
// Construction
public:
	CWndToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndToolBar)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndToolBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	//CString FindJpegFilePrefix(LPCTSTR strCourse);

public:
	void ShowSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce);
	void ChangeSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce);
	void EnableButtons(BOOL b);
	void RefreshSlide();
	void setviewer(CString);

protected:
	CString			m_strPreSlide;
	static CString  m_strClass;

	CImageList		m_imageList_lefttop;
	CImageList		m_imageList_left;
	CImageList		m_imageList_leftbottom;
	CImageList		m_imageList_bottom;
	CImageList		m_imageList_rightbottom;
	CImageList		m_imageList_right;
	CImageList		m_imageList_righttop;
	CImageList		m_imageList_top;
	CSize			m_cslefttop;
	CSize			m_csleft;
	CSize			m_csleftbottom;
	CSize			m_csbottom;
	CSize			m_csrightbottom;
	CSize			m_csright;
	CSize			m_csrighttop;
	CSize			m_cstop;
	CHtmlWnd*		m_pSlideWnd;
	CString			m_strPreCmd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDTOOLBAR_H__951C593C_74A1_4110_92C7_ED2775B88FBC__INCLUDED_)
