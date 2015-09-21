#if !defined(AFX_DLGASKCOURSEDUP_H__942C6838_D091_4C3B_9C91_DE14BAE9D217__INCLUDED_)
#define AFX_DLGASKCOURSEDUP_H__942C6838_D091_4C3B_9C91_DE14BAE9D217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAskCourseDup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAskCourseDup dialog

class CDlgAskCourseDup : public CDialog
{
// Construction
public:
	CDlgAskCourseDup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAskCourseDup)
	enum { IDD = IDD_DIALOG_COURSE_DUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAskCourseDup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAskCourseDup)
	afx_msg void OnRadioNewName();
	afx_msg void OnRadioOverwrite();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strRadioOverwrite;
	CString m_strRadioNewName;
	CString m_strPath;
	CString m_strCourse;
	BOOL	m_bOverWrite;

protected:
	CString m_strPreCourse;

protected:
	int GetCurrentCourseIndex();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGASKCOURSEDUP_H__942C6838_D091_4C3B_9C91_DE14BAE9D217__INCLUDED_)
