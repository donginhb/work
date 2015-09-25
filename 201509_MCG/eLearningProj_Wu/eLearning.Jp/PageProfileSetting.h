#if !defined(AFX_PROFILESETTING_H__E42F4904_8C20_4ACD_A851_016A7A48ADAA__INCLUDED_)
#define AFX_PROFILESETTING_H__E42F4904_8C20_4ACD_A851_016A7A48ADAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageProfileSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageProfileSetting dialog

class CPageProfileSetting : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageProfileSetting)

// Construction
public:
	CPageProfileSetting();
	~CPageProfileSetting();

// Dialog Data
	//{{AFX_DATA(CPageProfileSetting)
	enum { IDD = IDD_DIALOG_PROFILE_SETTING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageProfileSetting)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageProfileSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAudioQuality();
	afx_msg void OnSelchangeComboDim();
	afx_msg void OnSelchangeComboFps();
	afx_msg void OnSelchangeComboVideoQuality();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bCanGetActive;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILESETTING_H__E42F4904_8C20_4ACD_A851_016A7A48ADAA__INCLUDED_)
