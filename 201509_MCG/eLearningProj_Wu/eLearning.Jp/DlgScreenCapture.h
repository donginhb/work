#if !defined(AFX_DLGSCREENCAPTURE_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_)
#define AFX_DLGSCREENCAPTURE_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScreenCapture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgScreenCapture dialog
class CBmpBtn;
class CDataBrowseWnd;
class CDlgScreenCapture : public CDialog
{
// Construction
public:
	CDlgScreenCapture(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgScreenCapture)
	enum { IDD = IDD_DIALOG_CAPTURE_SCREEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScreenCapture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScreenCapture)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//add by buxiangyi
	/*afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComboCodec();*/
	///////////////////////////////////////////////////////////////
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

#endif // !defined(AFX_DLGSCREENCAPTURE_H__01027B86_C04D_4B39_B90E_08F52B49DB90__INCLUDED_)
