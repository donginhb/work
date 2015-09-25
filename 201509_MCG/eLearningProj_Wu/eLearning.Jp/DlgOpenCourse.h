#if !defined(AFX_DLGOPENCOURSE_H__5B8BF111_33BC_4686_A08F_912617138DF9__INCLUDED_)
#define AFX_DLGOPENCOURSE_H__5B8BF111_33BC_4686_A08F_912617138DF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOpenCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenCourse dialog

class CDlgOpenCourse : public CDialog
{
// Construction
public:
	CDlgOpenCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOpenCourse)
	enum { IDD = IDD_OPEN_COURSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOpenCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOpenCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListCourse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCourse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPENCOURSE_H__5B8BF111_33BC_4686_A08F_912617138DF9__INCLUDED_)
