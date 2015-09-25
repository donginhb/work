#if !defined(AFX_PAGESELTV_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_PAGESELTV_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPage.h : header file
//

#include "NewWizPage.h"
class CImageCell;
/////////////////////////////////////////////////////////////////////////////
// CPageSelTV dialog
class CPageSelTV : public CNewWizPage
{
// Construction
public:
	CPageSelTV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSelTV)
	enum { IDD = IDW_SELECT_TV };
	//}}AFX_DATA
	
	CFont m_Font;
	CFont m_Font2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSelTV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();
	LRESULT OnWizardBack();
	BOOL OnWizardFinish();

	void OnSetActive();
	BOOL OnKillActive();

	// Generated message map functions
	//{{AFX_MSG(CPageSelTV)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCellDblClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void OnSelectLayout(); 
    void OnDestroyPage();
	BOOL SelectLayout();
	void CollectLayout(CString strSel); 

protected:
	CImageCell*		m_pLayoutCell;
	CString			m_strPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESELTV_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
