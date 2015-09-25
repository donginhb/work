#if !defined(AFX_COURSELAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_)
#define AFX_COURSELAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultLayout.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPageDefaultLayout dialog
class CImageCell;
class CourseLayout : public CDialog
{
	DECLARE_DYNAMIC(CourseLayout)

public:
	CourseLayout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CourseLayout();

// Dialog Data
	enum { IDD = IDD_DIALOG_COURSE_LAYOUT };

public:
	virtual void OnOK();
	virtual void OnCancel();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDefaultLayout)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnCellCmd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellDblClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL SelectLayout();

protected:
	CImageCell* m_pLayoutCell;

public:	
	BOOL			m_bCanGetActive;
	char			m_OldName[256];
	CStringArray	m_CourseLayoutNumber;
	int				m_RowIndex;
	CString			LayoutName;
	CButton         *pBtn;
	CString			strName;
	CString         strOldName;
	BOOL            Layoutflag;
	CString         strPreName;
	CString         layoutNo;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COURSELAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_)
