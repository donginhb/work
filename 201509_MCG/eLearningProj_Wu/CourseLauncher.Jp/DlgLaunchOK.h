#if !defined(AFX_DLGLAUNCHOK_H__1622DA60_A002_4454_8C2A_F83B4BDE1FCE__INCLUDED_)
#define AFX_DLGLAUNCHOK_H__1622DA60_A002_4454_8C2A_F83B4BDE1FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLaunchOK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLaunchOK dialog

class CDlgLaunchOK : public CDialog
{
// Construction
public:
	CDlgLaunchOK(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLaunchOK)
	enum { IDD = IDD_DIALOG_LAUNCH_OK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLaunchOK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLaunchOK)
	afx_msg void OnViewResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLAUNCHOK_H__1622DA60_A002_4454_8C2A_F83B4BDE1FCE__INCLUDED_)
