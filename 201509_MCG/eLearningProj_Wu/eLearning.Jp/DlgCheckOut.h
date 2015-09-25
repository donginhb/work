#if !defined(AFX_DLGCHECKOUT_H__D770E67E_E6E1_41D4_96FA_4DA4C73619E2__INCLUDED_)
#define AFX_DLGCHECKOUT_H__D770E67E_E6E1_41D4_96FA_4DA4C73619E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCheckOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckOut dialog

class CDlgCheckOut : public CDialog
{
// Construction
public:
	CDlgCheckOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCheckOut)
	enum { IDD = IDD_CHCEK_OUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCheckOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCheckOut)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHECKOUT_H__D770E67E_E6E1_41D4_96FA_4DA4C73619E2__INCLUDED_)
