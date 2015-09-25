#if !defined(AFX_DLGASKTYPE_H__F64BB002_DEA5_4FA8_97EF_88D63E046370__INCLUDED_)
#define AFX_DLGASKTYPE_H__F64BB002_DEA5_4FA8_97EF_88D63E046370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAskType.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAskType dialog

class CDlgAskType : public CDialog
{
// Construction
public:
	CDlgAskType(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAskType)
	enum { IDD = IDD_DIALOG_ASK_TYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAskType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAskType)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strType;
	CStringArray m_strArrayTypes;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGASKTYPE_H__F64BB002_DEA5_4FA8_97EF_88D63E046370__INCLUDED_)
