#if !defined(AFX_DLGSELECTCOURSE_H__B90B4CFF_78CB_4895_85D8_71CEA7BE9293__INCLUDED_)
#define AFX_DLGSELECTCOURSE_H__B90B4CFF_78CB_4895_85D8_71CEA7BE9293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourse dialog
#include "BasicCheckListBox.h"
class CDlgSelectCourse : public CDialog
{
// Construction
public:
	CDlgSelectCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectCourse)
	enum { IDD = IDD_DIALOG_SEL_COURSE };
	CBasicCheckListBox   m_clCourse;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString	m_strTitle;
	CString m_strWndText;
	CStringArray*	m_pstrArraySelCourse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTCOURSE_H__B90B4CFF_78CB_4895_85D8_71CEA7BE9293__INCLUDED_)
