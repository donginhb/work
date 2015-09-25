#if !defined(AFX_COLORWND_H__040F07F5_1285_46B5_B58B_17A1899900D1__INCLUDED_)
#define AFX_COLORWND_H__040F07F5_1285_46B5_B58B_17A1899900D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorWnd window
#define WM_COLORWND_MOUSE_MOVE (WM_APP + 2371)
class CColorWnd : public CWnd
{
// Construction
public:
	CColorWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorWnd)
	public:
	virtual BOOL Create(CRect rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_crFill;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORWND_H__040F07F5_1285_46B5_B58B_17A1899900D1__INCLUDED_)
