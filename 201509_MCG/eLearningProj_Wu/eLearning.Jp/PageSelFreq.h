#if !defined(AFX_PAGESELFREQ_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_PAGESELFREQ_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSelFreq dialog
class CPageSelFreq : public CNewWizPage
{
// Construction
public:
	CPageSelFreq(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSelFreq)
	enum { IDD = IDW_SELECT_FREQ };
	//}}AFX_DATA
	
	CFont m_Font;
	CFont m_Font2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSelFreq)
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
	//{{AFX_MSG(CPageSelFreq)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioOther();
	afx_msg void OnRadioFreq1();
	afx_msg void OnRadioFreq2();
	afx_msg void OnRadioFreq3();
	afx_msg void OnRadioFreq4();
	afx_msg void OnRadioFreq5();
	afx_msg void OnRadioFreq6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESELFREQ_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
