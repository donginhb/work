#if !defined(AFX_BMPSTATIC_H__F8DE22E2_9EB9_4CF5_BDE2_4A81BD88F25A__INCLUDED_)
#define AFX_BMPSTATIC_H__F8DE22E2_9EB9_4CF5_BDE2_4A81BD88F25A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic window

class CBmpStatic : public CWnd
{
// Construction
public:
	CBmpStatic(UINT nID);
	CBmpStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpStatic)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

public:
	void SetBorder(BOOL bWithBorder) {m_bBorder = bWithBorder;}
	void SetImage(CString strPath);

protected:
	static CString m_strClass;
	CImageList*	m_pimageList;
	int			m_nWidth;
	int			m_nHeight;
	BOOL		m_bBorder;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPSTATIC_H__F8DE22E2_9EB9_4CF5_BDE2_4A81BD88F25A__INCLUDED_)
