#if !defined(AFX_DLGVERSIONOUTOFDATE_H__C1A90219_5A09_413A_8D73_362DD6E710A1__INCLUDED_)
#define AFX_DLGVERSIONOUTOFDATE_H__C1A90219_5A09_413A_8D73_362DD6E710A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVersionOutOfDate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVersionOutOfDate dialog

class CDlgVersionOutOfDate : public CDialog
{
// Construction
public:
	CDlgVersionOutOfDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVersionOutOfDate)
	enum { IDD = IDD_DATE_OUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVersionOutOfDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVersionOutOfDate)
	afx_msg void OnReg();
	afx_msg void OnRegister();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVERSIONOUTOFDATE_H__C1A90219_5A09_413A_8D73_362DD6E710A1__INCLUDED_)
