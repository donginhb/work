#if !defined(AFX_DLGBACKUPONECOURSE_H__DA5AEF2D_68DC_44E6_899C_A09E0F1FC73F__INCLUDED_)
#define AFX_DLGBACKUPONECOURSE_H__DA5AEF2D_68DC_44E6_899C_A09E0F1FC73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBackupOneCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupOneCourse dialog

class CDlgBackupOneCourse : public CDialog
{
// Construction
public:
	CDlgBackupOneCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBackupOneCourse)
	enum { IDD = IDD_DIALOG_SEL_BKUP_DIR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBackupOneCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBackupOneCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray	m_strArrayDir;

public:
	CString m_strCourse;
	BOOL	m_bDel;
	CString m_strPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBACKUPONECOURSE_H__DA5AEF2D_68DC_44E6_899C_A09E0F1FC73F__INCLUDED_)
