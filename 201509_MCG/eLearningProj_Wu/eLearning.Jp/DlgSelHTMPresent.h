#if !defined(AFX_DLGSELHTMPRESENT_H__95CED6FE_DBB1_4C7A_8EAD_C4D6375A6405__INCLUDED_)
#define AFX_DLGSELHTMPRESENT_H__95CED6FE_DBB1_4C7A_8EAD_C4D6375A6405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelHTMPresent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelHTMPresent dialog

class CDlgSelHTMPresent : public CDialog
{
// Construction
public:
	CDlgSelHTMPresent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelHTMPresent)
	enum { IDD = IDD_SEL_HTM_COURSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelHTMPresent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelHTMPresent)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strPresent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELHTMPRESENT_H__95CED6FE_DBB1_4C7A_8EAD_C4D6375A6405__INCLUDED_)
