#if !defined(AFX_DLGRESTORECOURSE_H__76491631_5BAF_4B2A_8A09_E95856072205__INCLUDED_)
#define AFX_DLGRESTORECOURSE_H__76491631_5BAF_4B2A_8A09_E95856072205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRestoreCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRestoreCourse dialog
#include "BasicCheckListBox.h"
class CDlgRestoreCourse : public CDialog
{
// Construction
public:
	CDlgRestoreCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRestoreCourse)
	enum { IDD = IDD_DIALOG_SEL_RESTORE };
	CBasicCheckListBox   m_clCourse;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRestoreCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRestoreCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckDel();
	afx_msg void OnSelectall();
	afx_msg void OnSelchangeComboDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray*	m_pstrArraySelCourse;
	BOOL			m_bDel;
	//add by buxiangyi
	BOOL            m_over;
	CString			m_strPath;

protected:
	void LoadBackup();
	
protected:
	CStringArray	m_strArrayDir;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESTORECOURSE_H__76491631_5BAF_4B2A_8A09_E95856072205__INCLUDED_)
