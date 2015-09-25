#if !defined(AFX_PAGEMSRV_H__1EEE6B3B_0A46_42A2_83C4_2F1BD957024C__INCLUDED_)
#define AFX_PAGEMSRV_H__1EEE6B3B_0A46_42A2_83C4_2F1BD957024C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMSrv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageMSrv dialog

class CPageMSrv : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMSrv)

// Construction
public:
	CPageMSrv();
	~CPageMSrv();

// Dialog Data
	//{{AFX_DATA(CPageMSrv)
	enum { IDD = IDD_DIALOG_MEDIA_SERVER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMSrv)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMSrv)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditIp();
	afx_msg void OnKillfocusEditLoginName();
	afx_msg void OnKillfocusEditPassword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bCanGetActive;

protected:
	CString	m_strPreLogin;
	CString m_strPreIP;
	CString m_strPrePWD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMSRV_H__1EEE6B3B_0A46_42A2_83C4_2F1BD957024C__INCLUDED_)
