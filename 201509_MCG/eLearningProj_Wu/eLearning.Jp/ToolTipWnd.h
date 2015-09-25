#if !defined(AFX_TOOLTIPWND_H__542B74C2_63E5_4F42_B6CB_3C06B5C87D95__INCLUDED_)
#define AFX_TOOLTIPWND_H__542B74C2_63E5_4F42_B6CB_3C06B5C87D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd window

class CToolTipWnd : public CWnd
{
// Construction
public:
	CToolTipWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipWnd)
	virtual BOOL Create(CWnd* pParentWnd); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

protected:
	static CString m_strClass; 
	CString m_strText;
	CFont*	m_pFont;

public:
	void ShowText(CPoint pt, CString strText);
	void SetDrawFont(CFont* pf) {m_pFont = pf;};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPWND_H__542B74C2_63E5_4F42_B6CB_3C06B5C87D95__INCLUDED_)
