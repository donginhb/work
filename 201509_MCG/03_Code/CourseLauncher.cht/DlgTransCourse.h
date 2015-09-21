#if !defined(AFX_DLGTRANSCOURSE_H__CA2D3E70_005E_4047_9990_6B0C341CE521__INCLUDED_)
#define AFX_DLGTRANSCOURSE_H__CA2D3E70_005E_4047_9990_6B0C341CE521__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTransCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTransCourse dialog

class CDlgTransCourse : public CDialog
{
// Construction
public:
	CDlgTransCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTransCourse)
	enum { IDD = IDD_DIALOG_TRANS_COURSE };
	CComboBox	m_cbCourse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTransCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTransCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCourse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRANSCOURSE_H__CA2D3E70_005E_4047_9990_6B0C341CE521__INCLUDED_)
