#if !defined(AFX_PAGEDEFAULTOUTLOOK_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_)
#define AFX_PAGEDEFAULTOUTLOOK_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultOutlook.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOutlook dialog

class CPageDefaultOutlook : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDefaultOutlook)

// Construction
public:
	CPageDefaultOutlook();
	~CPageDefaultOutlook();

// Dialog Data
	//{{AFX_DATA(CPageDefaultOutlook)
	enum { IDD = IDD_DIALOG_DEFAULT_OUTLOOK };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDefaultOutlook)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDefaultOutlook)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListOutlook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bCanGetActive;
	BOOL    m_PageDefaultOutLookFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEFAULTOUTLOOK_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_)
