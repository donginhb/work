#if !defined(AFX_DLGCOURSEDUP_H__680A73DC_3E7A_443F_BD82_656BF5AC22F7__INCLUDED_)
#define AFX_DLGCOURSEDUP_H__680A73DC_3E7A_443F_BD82_656BF5AC22F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCourseDup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCourseDup dialog

class CDlgCourseDup : public CDialog
{
// Construction
public:
	CDlgCourseDup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCourseDup)
	enum { IDD = IDD_DIALOG_COURSE_DUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCourseDup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCourseDup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioNewName();
	afx_msg void OnRadioOverwrite();
	virtual void OnCancel();
	afx_msg void OnKillfocusEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strStatic;
	CString m_strRadioOverwrite;
	CString m_strRadioNewName;
	CString m_strPath;
	CString m_strCourse;
	BOOL	m_bOverWrite;
	CString m_strTitle;

protected:
	int GetCurrentCourseIndex();

protected:
	CString m_strPreCourse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOURSEDUP_H__680A73DC_3E7A_443F_BD82_656BF5AC22F7__INCLUDED_)
