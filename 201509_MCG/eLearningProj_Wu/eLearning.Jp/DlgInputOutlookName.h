#if !defined(AFX_DLGINPUTOUTLOOKNAME_H__B00122CA_65F8_4E29_9534_7E80CA6AE24D__INCLUDED_)
#define AFX_DLGINPUTOUTLOOKNAME_H__B00122CA_65F8_4E29_9534_7E80CA6AE24D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputOutlookName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutlookName dialog

class CDlgInputOutlookName : public CDialog
{
// Construction
public:
	CDlgInputOutlookName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputOutlookName)
	enum { IDD = IDD_DIALOG_INPUT_OUTLOOK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputOutlookName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputOutlookName)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillfocusComboOutlook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString	m_strOutlook;
	CString m_strPreOutlook;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTOUTLOOKNAME_H__B00122CA_65F8_4E29_9534_7E80CA6AE24D__INCLUDED_)
