#if !defined(AFX_PAGEDEFAULTLAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_)
#define AFX_PAGEDEFAULTLAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultLayout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultLayout dialog
class CImageCell;
class CPageDefaultLayout : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDefaultLayout)

// Construction
public:
	CPageDefaultLayout();
	~CPageDefaultLayout();

// Dialog Data
	//{{AFX_DATA(CPageDefaultLayout)
	enum { IDD = IDD_DIALOG_DEFAULT_LAYOUT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDefaultLayout)
	public:
	virtual BOOL OnSetActive();
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
	BOOL	m_bCanGetActive;
	CString         strOldName;
	BOOL            Layoutflag;
	CString         strPreName;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEFAULTLAYOUT_H__DDA83C2B_9483_4725_A545_50C56AAD3199__INCLUDED_)
