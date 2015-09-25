#if !defined(AFX_PAGEINPUTNAME_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_PAGEINPUTNAME_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPageInputName dialog
class CPageInputName : public CNewWizPage
{
// Construction
public:
	CPageInputName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageInputName)
	enum { IDD = IDW_INPUT_NAME };
	//}}AFX_DATA
	
	CFont m_Font;
	CFont m_Font2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageInputName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();
	void OnSetActive();
	BOOL OnKillActive();
	LRESULT OnWizardBack();

	// Generated message map functions
	//{{AFX_MSG(CPageInputName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEINPUTNAME_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
