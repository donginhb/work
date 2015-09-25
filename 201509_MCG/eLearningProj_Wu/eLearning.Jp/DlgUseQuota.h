#if !defined(AFX_DLGUSEQUOTA_H__34D5D153_9A03_43E2_B8A5_E7793895AE31__INCLUDED_)
#define AFX_DLGUSEQUOTA_H__34D5D153_9A03_43E2_B8A5_E7793895AE31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUseQuota.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUseQuota dialog

class CDlgUseQuota : public CDialog
{
// Construction
public:
	CDlgUseQuota(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUseQuota)
	enum { IDD = IDD_USE_QUOTA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUseQuota)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUseQuota)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSEQUOTA_H__34D5D153_9A03_43E2_B8A5_E7793895AE31__INCLUDED_)
