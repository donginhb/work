#if !defined(AFX_DLGSELVIDEOSRC_H__6A834E85_1127_4392_80F1_EBD6438065C4__INCLUDED_)
#define AFX_DLGSELVIDEOSRC_H__6A834E85_1127_4392_80F1_EBD6438065C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelVideoSrc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelVideoSrc dialog
class CBmpBtn;
class CDlgSelVideoSrc : public CDialog
{
// Construction
public:
	CDlgSelVideoSrc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelVideoSrc)
	enum { IDD = IDD_DIALOG_INS_VIDEO_SRC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelVideoSrc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelVideoSrc)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioExistingFile();
	afx_msg void OnRadioRecord();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnKillfocusEditFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bCaptureNow;
	CString m_strPath;

protected:
	CBmpBtn* m_pBtnOpen;

protected:
	void SelASFFile();
	void EnableFileSel(BOOL b); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELVIDEOSRC_H__6A834E85_1127_4392_80F1_EBD6438065C4__INCLUDED_)
