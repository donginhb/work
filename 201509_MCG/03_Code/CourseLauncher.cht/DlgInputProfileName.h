#if !defined(AFX_DLGINPUTPROFILENAME_H__D37817F2_78DD_4BBF_9856_19C108E571DC__INCLUDED_)
#define AFX_DLGINPUTPROFILENAME_H__D37817F2_78DD_4BBF_9856_19C108E571DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputProfileName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputProfileName dialog

class CDlgInputProfileName : public CDialog
{
// Construction
public:
	CDlgInputProfileName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputProfileName)
	enum { IDD = IDD_DIALOG_INPUT_PROFILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputProfileName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputProfileName)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString			m_strProfileName;
	CStringArray*	m_pstrArrayProfile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTPROFILENAME_H__D37817F2_78DD_4BBF_9856_19C108E571DC__INCLUDED_)
