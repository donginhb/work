#if !defined(AFX_DLGBACKUPMAINTAIN_H__C90C20D5_43CD_41F9_B068_97D2F62813D0__INCLUDED_)
#define AFX_DLGBACKUPMAINTAIN_H__C90C20D5_43CD_41F9_B068_97D2F62813D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBackupMaintain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupMaintain dialog
#include "BasicCheckListBox.h"
class CDlgBackupMaintain : public CDialog
{
// Construction
public:
	CDlgBackupMaintain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBackupMaintain)
	enum { IDD = IDD_DIALOG_BACKUP_MAINTAIN };
	CBasicCheckListBox   m_clCourse;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBackupMaintain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBackupMaintain)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelCourse();
	afx_msg void OnRenameCourse();
	virtual void OnCancel();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectall();
	afx_msg void OnSelchangeComboDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:	
	BOOL GetSelCourse(CStringArray& strArraySel); 
	void LoadBackup();

protected:
	CStringArray m_strArrayDir;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBACKUPMAINTAIN_H__C90C20D5_43CD_41F9_B068_97D2F62813D0__INCLUDED_)
