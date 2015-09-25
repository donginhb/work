#if !defined(AFX_DLGBACKUPCOURSE_H__625EEA60_4B18_4CF2_990C_F2443E711E96__INCLUDED_)
#define AFX_DLGBACKUPCOURSE_H__625EEA60_4B18_4CF2_990C_F2443E711E96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBackupCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupCourse dialog
#include "BasicCheckListBox.h"
class CDlgBackupCourse : public CDialog
{
// Construction
public:
	CDlgBackupCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBackupCourse)
	enum { IDD = IDD_DIALOG_SEL_BKUP };
	CBasicCheckListBox   m_clCourse;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBackupCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBackupCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckDel();
	afx_msg void OnSelectall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray*	m_pstrArraySelCourse;
	BOOL			m_bDel;
	CString			m_strPath;

protected:
	CStringArray	m_strArrayDir;
public:
	afx_msg void OnClickedBackup();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBACKUPCOURSE_H__625EEA60_4B18_4CF2_990C_F2443E711E96__INCLUDED_)
