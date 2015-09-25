#if !defined(AFX_IMAGECTRL_H__AD073382_FD23_11D4_B46F_0020AF3085DB__INCLUDED_)
#define AFX_IMAGECTRL_H__AD073382_FD23_11D4_B46F_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageCtrl window

//#include "ltwrappr.h"

class CImageCtrl : public CWnd
{
// Construction
public:
	CImageCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageCtrl)
	public:
	virtual BOOL Create(DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void DrawPictureFrame(CDC* pDC, CRect rc);

public:
	BOOL LoadFile(LPCTSTR strFileName, BOOL bForce = FALSE);

protected:
	//LBitmapWindow m_LBitmapWnd;
	CBitmap*			m_pBMP;
	CString				m_strFileName;
	static CString		m_strClass;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECTRL_H__AD073382_FD23_11D4_B46F_0020AF3085DB__INCLUDED_)
