#if !defined(AFX_DLGNEWCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_)
#define AFX_DLGNEWCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourse dialog

class CBmpBtn;
class CDlgNewCourse : public CDialog
{
// Construction
public:
	CDlgNewCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewCourse)
	enum { IDD = IDD_DIALOG_NEW_COURSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewCourse)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRec();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnKillfocusEditCourse();
	afx_msg void OnRadioHtml();
	afx_msg void OnRadioPpt();
	afx_msg void OnSelchangeComboPresent();
	afx_msg void OnDefaultProfile();
	afx_msg void OnScreenProfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCourseName;
	CString m_strSrc;
	BOOL	m_bPPT;
	BOOL	m_bRec;
	BOOL    m_bNoRecord;
	BOOL	m_bOverwrite;

protected:
	CBmpBtn* m_pBtnOpenPPT;

protected:
	void OnSelectPPT(); 
	BOOL Commit(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWCOURSE_H__42CC16A0_F505_11D4_B46F_0020AF3085DB__INCLUDED_)
