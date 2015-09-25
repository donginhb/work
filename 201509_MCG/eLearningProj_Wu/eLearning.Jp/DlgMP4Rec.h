#if !defined(AFX_DLGWMVREC_H__CE06E268_0BA6_41D9_B327_EEF7974A86FA__INCLUDED_)
#define AFX_DLGWMVREC_H__CE06E268_0BA6_41D9_B327_EEF7974A86FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMP4Rec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Rec dialog
class CBmpBtn;
class CDataBrowseWnd;
class CDlgMP4Rec : public CDialog
{
// Construction
public:
	CDlgMP4Rec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMP4Rec)
	enum { IDD = IDD_DIALOG_REC_WMV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMP4Rec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMP4Rec)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//add by buxiangyi
	//afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	/////////////////////////////////////////////////////////
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBmpBtn*			m_pBtnOpen;
	CDataBrowseWnd*		m_pAttribSelGrid;
	CString				m_strIniPath;

public:
	CString				m_strOutIniPath;
	CString				m_strVideoPath;

protected:
	void				OnSelectFile(); 
public:
	afx_msg void OnClickedDeviceSet();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWMVREC_H__CE06E268_0BA6_41D9_B327_EEF7974A86FA__INCLUDED_)
