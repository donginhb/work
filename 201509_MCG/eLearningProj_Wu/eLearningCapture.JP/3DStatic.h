#if !defined(AFX_3DSTATIC_H__D0980C7E_1EDD_4928_A13B_10C27D065F9F__INCLUDED_)
#define AFX_3DSTATIC_H__D0980C7E_1EDD_4928_A13B_10C27D065F9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DStatic window

class C3DStatic : public CWnd
{
// Construction
public:
	C3DStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DStatic)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C3DStatic();
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

	// Generated message map functions
protected:
	//{{AFX_MSG(C3DStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	

protected:
	static CString m_strClass;
	CFont*	m_pFont;
	CString	m_strText;

public:
	void SetDrawFont(CFont* pFont) { m_pFont = pFont; }
	void SetText(CString strText, BOOL b = TRUE) { m_strText = strText; if (b) RedrawWindow();}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DSTATIC_H__D0980C7E_1EDD_4928_A13B_10C27D065F9F__INCLUDED_)
