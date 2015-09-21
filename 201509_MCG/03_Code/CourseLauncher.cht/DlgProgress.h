#if !defined(AFX_DLGPROGRESS_H__5E6AAE5E_D2AB_4A54_BEF2_C3322AE04A39__INCLUDED_)
#define AFX_DLGPROGRESS_H__5E6AAE5E_D2AB_4A54_BEF2_C3322AE04A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
#include <afxcmn.h>
class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CAnimateCtrl	m_avi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void PeekAndPump();
	BOOL IsCanceled() { return m_bCancelled; };
	void Show();
	void Hide();
	void EnableCancel(BOOL b);

protected:
	void Stop();
	void Play();

public:
	CString m_strAVIPath;

protected:
	BOOL m_bCancelled;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__5E6AAE5E_D2AB_4A54_BEF2_C3322AE04A39__INCLUDED_)
