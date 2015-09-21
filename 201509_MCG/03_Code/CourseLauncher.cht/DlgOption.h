#if !defined(AFX_DLGOPTION_H__3C2EB42C_B3B2_459E_8AD1_EE21FDD43620__INCLUDED_)
#define AFX_DLGOPTION_H__3C2EB42C_B3B2_459E_8AD1_EE21FDD43620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOption dialog

class CDlgOption : public CDialog
{
// Construction
public:
	CDlgOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOption)
	enum { IDD = IDD_DIALOG_OPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOption)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTION_H__3C2EB42C_B3B2_459E_8AD1_EE21FDD43620__INCLUDED_)
