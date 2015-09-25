#if !defined(AFX_WNDLOGO_H__1B34A360_1D0A_11D4_BE12_0020AF3085DB__INCLUDED_)
#define AFX_WNDLOGO_H__1B34A360_1D0A_11D4_BE12_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndLogo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndLogo window

class CWndLogo : public CWnd
{
// Construction
public:
	CWndLogo(CString StrLogoPath);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndLogo)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndLogo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndLogo)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap*	m_pBitmap;
	int			m_nBmpWidth;
	int			m_nBmpHeight;

public:
	BOOL Create();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDLOGO_H__1B34A360_1D0A_11D4_BE12_0020AF3085DB__INCLUDED_)
