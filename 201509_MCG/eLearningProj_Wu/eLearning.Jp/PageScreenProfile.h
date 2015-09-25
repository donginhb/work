#if !defined(AFX_PAGESCREENPROFILE_H__28EB3C0D_6D1D_4C10_BF03_8D014173E626__INCLUDED_)
#define AFX_PAGESCREENPROFILE_H__28EB3C0D_6D1D_4C10_BF03_8D014173E626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageScreenProfile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageScreenProfile dialog

class CPageScreenProfile : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageScreenProfile)

// Construction
public:
	CPageScreenProfile();
	~CPageScreenProfile();

// Dialog Data
	//{{AFX_DATA(CPageScreenProfile)
	enum { IDD = IDD_DIALOG_PROFILE_SETTING_S };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageScreenProfile)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageScreenProfile)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAudioQualityS();
	afx_msg void OnSelchangeComboFpsS();
	afx_msg void OnSelchangeComboVideoQualityS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bCanGetActive;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCREENPROFILE_H__28EB3C0D_6D1D_4C10_BF03_8D014173E626__INCLUDED_)
