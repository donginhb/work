#if !defined(AFX_DLGVIEWSELECT_H__1459219A_21E5_4D84_8BB8_6D4A4EE8F46F__INCLUDED_)
#define AFX_DLGVIEWSELECT_H__1459219A_21E5_4D84_8BB8_6D4A4EE8F46F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgViewSelect dialog

class CDlgViewSelect : public CDialog
{
// Construction
public:
	CDlgViewSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewSelect)
	enum { IDD = IDD_DIALOG_VIEW_SEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray* m_pstrArrayCourse;

protected:
	BOOL GetCourseURL(CString strNewCourse, CString& strURL);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWSELECT_H__1459219A_21E5_4D84_8BB8_6D4A4EE8F46F__INCLUDED_)
