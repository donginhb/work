#if !defined(AFX_CDlgCourseFolder_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_)
#define AFX_CDlgCourseFolder_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScreenCapture.h : header file
//
#include "resource.h"
// CDlgCourseFolder dialog
class CBmpBtn;
class CDlgCourseFolder : public CDialog
{

public:
	CDlgCourseFolder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCourseFolder();

// Dialog Data
	enum { IDD = IDD_DIALOG_COURSE_FOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScreenCapture)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//add by buxiangyi
	/*afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComboCodec();*/
	///////////////////////////////////////////////////////////////
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBmpBtn*			m_pBtnOpen;
	CString				m_strIniPath;

protected:
	void				OnSelectFile(); 
};
#endif // !defined(AFX_CDlgCourseFolder_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_)