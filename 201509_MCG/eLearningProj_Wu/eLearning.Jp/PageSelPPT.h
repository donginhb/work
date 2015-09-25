#if !defined(AFX_PAGESELPPT_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_PAGESELPPT_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSelPPT dialog
class CBmpBtn;
class CPageSelPPT : public CNewWizPage
{
// Construction
public:
	CPageSelPPT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSelPPT)
	enum { IDD = IDW_SELECT_PPT };
	//}}AFX_DATA
	
	CFont m_Font;
	CFont m_Font2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSelPPT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();
	LRESULT OnWizardBack();
	void OnSetActive();
	BOOL OnKillActive();

	// Generated message map functions
	//{{AFX_MSG(CPageSelPPT)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBmpBtn* m_pBtnOpenPPT;

protected:
	void OnSelectPPT(); 
    void OnDestroyPage();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESELPPT_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
