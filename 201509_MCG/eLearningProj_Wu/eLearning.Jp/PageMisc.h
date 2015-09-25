#if !defined(AFX_PAGEMISC_H__B8A2C272_F322_43FD_B5D5_E3895B24D66E__INCLUDED_)
#define AFX_PAGEMISC_H__B8A2C272_F322_43FD_B5D5_E3895B24D66E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMisc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageMisc dialog
#include "..\ComCapture\ComCapture\ComCapture_i.h"
class CPageMisc : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMisc)

// Construction
public:
	CPageMisc();
	~CPageMisc();

// Dialog Data
	//{{AFX_DATA(CPageMisc)
	enum { IDD = IDD_DIALOG_MISC };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMisc)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMisc)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioHigh();
	afx_msg void OnRadioLow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bCanGetActive;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMISC_H__B8A2C272_F322_43FD_B5D5_E3895B24D66E__INCLUDED_)
