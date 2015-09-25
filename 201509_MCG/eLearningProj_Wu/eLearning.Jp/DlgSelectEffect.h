#if !defined(AFX_DLGSELECTEFFECT_H__8125802B_0F9B_4FD4_BAAC_7D1C87BE0A79__INCLUDED_)
#define AFX_DLGSELECTEFFECT_H__8125802B_0F9B_4FD4_BAAC_7D1C87BE0A79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectEffect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectEffect dialog
class CImageSetContainer;
class CDlgSelectEffect : public CDialog
{
// Construction
public:
	CDlgSelectEffect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectEffect)
	enum { IDD = IDD_DIALOG_SELECT_EFFECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectEffect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnPictureSetSelected(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CImageSetContainer*	m_pImageSet;

public:
	CString m_strEffectPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTEFFECT_H__8125802B_0F9B_4FD4_BAAC_7D1C87BE0A79__INCLUDED_)
