#if !defined(AFX_DLGINPUTCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_)
#define AFX_DLGINPUTCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCourse dialog

class CDlgInputCourse : public CDialog
{
// Construction
public:
	CDlgInputCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputCourse)
	enum { IDD = IDD_DIALOG_INPUT_COURSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputCourse)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRec();
	virtual void OnCancel();
	afx_msg void OnKillfocusEditCourse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCourseName;
	CString m_strCaption;

protected:
	CString m_strPreCourse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_)
