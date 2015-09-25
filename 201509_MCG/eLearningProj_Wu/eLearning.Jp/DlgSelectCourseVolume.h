#if !defined(AFX_DLGSELECTCOURSEVOLUME_H__06CCA5B0_D688_482F_AF01_6F31D40ACBEB__INCLUDED_)
#define AFX_DLGSELECTCOURSEVOLUME_H__06CCA5B0_D688_482F_AF01_6F31D40ACBEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectCourseVolume.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourseVolume dialog
#include "BasicCheckListBox.h"
class CDlgSelectCourseVolume : public CDialog
{
// Construction
public:
	CDlgSelectCourseVolume(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectCourseVolume)
	enum { IDD = IDD_DIALOG_SEL_COURSE_VOLUME };
	CBasicCheckListBox   m_clCourse;
	CSliderCtrl	m_sliderVolume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectCourseVolume)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectCourseVolume)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectall();
	afx_msg void OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray*	m_pstrArraySelCourse;
	int m_nVolumeValue;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTCOURSEVOLUME_H__06CCA5B0_D688_482F_AF01_6F31D40ACBEB__INCLUDED_)
