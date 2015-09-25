#if !defined(AFX_DLGSELTYPE_H__817C631A_CF4F_4EF0_A300_3C94EF985268__INCLUDED_)
#define AFX_DLGSELTYPE_H__817C631A_CF4F_4EF0_A300_3C94EF985268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelType.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelType dialog

class CDlgSelType : public CDialog
{
// Construction
public:
	CDlgSelType(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelType)
	enum { IDD = IDD_DIALOG_TYPE_SELECTING };
	CCheckListBox   m_clType;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelType)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSelectall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray*	m_pstrArrayType;
	CStringArray	m_strArrayHTML;

protected:
	CString GetLinkText();
	int GetNumberPart(CString str);
	CString GetTableHTML(CString strLabel, CString strLink, CString strURL);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELTYPE_H__817C631A_CF4F_4EF0_A300_3C94EF985268__INCLUDED_)
