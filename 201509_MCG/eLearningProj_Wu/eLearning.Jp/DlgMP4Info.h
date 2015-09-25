#if !defined(AFX_DLGASFINFO_H__E351269C_4E38_44C2_8B5A_28D582114FC7__INCLUDED_)
#define AFX_DLGASFINFO_H__E351269C_4E38_44C2_8B5A_28D582114FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMP4Info.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Info dialog

class CDlgMP4Info : public CDialog
{
// Construction
public:
	CDlgMP4Info(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMP4Info)
	enum { IDD = IDD_DIALOG_WMV_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMP4Info)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMP4Info)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnRadioScreen();
	afx_msg void OnRadioDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void GetProfileData(CString strProfile);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGASFINFO_H__E351269C_4E38_44C2_8B5A_28D582114FC7__INCLUDED_)
