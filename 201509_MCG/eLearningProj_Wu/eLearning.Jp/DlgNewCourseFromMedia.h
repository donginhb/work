#if !defined(AFX_DLGNEWCOURSEFROMMEDIA_H__4F90536B_E545_4FC2_9DDC_42E975F36E0B__INCLUDED_)
#define AFX_DLGNEWCOURSEFROMMEDIA_H__4F90536B_E545_4FC2_9DDC_42E975F36E0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewCourseFromMedia.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourseFromMedia dialog
class CBmpBtn;
class CDlgNewCourseFromMedia : public CDialog
{
// Construction
public:
	CDlgNewCourseFromMedia(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewCourseFromMedia)
	enum { IDD = IDD_DIALOG_NEW_COURSE_FROM_MEDIA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewCourseFromMedia)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewCourseFromMedia)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioHtml();
	afx_msg void OnRadioPpt();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComboPresent();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBmpBtn* m_pBtnOpenPPT;
	CBmpBtn* m_pBtnOpenMedia;

protected:
	void OnSelectPPT();
	void OnSelectMedia(); 
	BOOL Commit();
	
public:
	CString m_strCourseName;
	CString m_strPresentSrc;
	CString m_strMediaPath;
	BOOL	m_bPPT;
	BOOL	m_bOverwrite;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWCOURSEFROMMEDIA_H__4F90536B_E545_4FC2_9DDC_42E975F36E0B__INCLUDED_)
