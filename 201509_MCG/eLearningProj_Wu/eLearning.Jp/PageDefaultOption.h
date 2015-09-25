#if !defined(AFX_PAGEDEFAULTOPTION_H__D2F8B705_81CB_4F94_BB85_21C7BCC85CC9__INCLUDED_)
#define AFX_PAGEDEFAULTOPTION_H__D2F8B705_81CB_4F94_BB85_21C7BCC85CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOption dialog
#include "BasicCheckListBox.h"
class CPageDefaultOption : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDefaultOption)

// Construction
public:
	CPageDefaultOption();
	~CPageDefaultOption();

// Dialog Data
	//{{AFX_DATA(CPageDefaultOption)
	enum { IDD = IDD_DIALOG_DEFAULT_OPTION };
	CBasicCheckListBox   m_clCourse;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDefaultOption)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDefaultOption)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCheckChange(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SetCheckItem(UINT nID, int nCheck);
	void WriteOptionFile(CString strPath);
	BOOL StrEqual(CString strInput, UINT nID);
	void ClearCheckItem();
	void SetOption(CString strOptionPath);

public:
	BOOL	m_bCanGetActive;
	BOOL    m_PageDefaultOptionFlag;

	//add 2014/7/30 kagata BugNo.133
	int chksave[20];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEFAULTOPTION_H__D2F8B705_81CB_4F94_BB85_21C7BCC85CC9__INCLUDED_)
