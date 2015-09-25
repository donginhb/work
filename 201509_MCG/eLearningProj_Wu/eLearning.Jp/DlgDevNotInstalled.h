#if !defined(AFX_DLGDEVNOTINSTALLED_H__FBB8382F_FDC3_4800_A12A_FDCCE5131589__INCLUDED_)
#define AFX_DLGDEVNOTINSTALLED_H__FBB8382F_FDC3_4800_A12A_FDCCE5131589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevNotInstalled.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNotInstalled dialog

class CDlgDevNotInstalled : public CDialog
{
// Construction
public:
	CDlgDevNotInstalled(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevNotInstalled)
	enum { IDD = IDD_DIALOG_ASK_DEV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevNotInstalled)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevNotInstalled)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDev();
	afx_msg void OnRetry();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strDevName;
	CString m_strVideoDev;
	CString m_strAudioDev;
	BOOL m_Screenflag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEVNOTINSTALLED_H__FBB8382F_FDC3_4800_A12A_FDCCE5131589__INCLUDED_)
