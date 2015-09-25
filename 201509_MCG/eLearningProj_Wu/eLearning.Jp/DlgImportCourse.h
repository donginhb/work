#if !defined(AFX_DLGIMPORTCOURSE_H__4040E891_093F_4B8F_A699_AF7066DA492A__INCLUDED_)
#define AFX_DLGIMPORTCOURSE_H__4040E891_093F_4B8F_A699_AF7066DA492A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImportCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImportCourse dialog

#include "BasicCheckListBox.h"
class CDlgImportCourse : public CDialog
{
// Construction
public:
	CDlgImportCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImportCourse)
	enum { IDD = IDD_DIALOG_IMPORT_COURSE };
	CBasicCheckListBox   m_clCourse;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImportCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImportCourse)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboVersion();
	afx_msg void OnSelectall();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CStringArray m_strArrayPath;

public:	
	CStringArray* m_pstrArrayImportCourse;
	CString m_strPath;

protected:
	void LoadCourse(CString strPath); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORTCOURSE_H__4040E891_093F_4B8F_A699_AF7066DA492A__INCLUDED_)
