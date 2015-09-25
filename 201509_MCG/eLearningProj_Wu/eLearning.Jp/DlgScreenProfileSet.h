#if !defined(AFX_DLGSCREENPROFILESET_H__E6D21C9A_6A1E_47DC_8A18_8168ADC00E3F__INCLUDED_)
#define AFX_DLGSCREENPROFILESET_H__E6D21C9A_6A1E_47DC_8A18_8168ADC00E3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScreenProfileSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgScreenProfileSet dialog
class CDataBrowseWnd;
class CDlgScreenProfileSet : public CDialog
{
// Construction
public:
	CDlgScreenProfileSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgScreenProfileSet)
	enum { IDD = IDD_DIALOG_PROFILE_SET_S };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScreenProfileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScreenProfileSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonDel();
	afx_msg void OnButtonRename();
	afx_msg void OnSelchangeComboCodec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDataBrowseWnd*		m_pAttribSelGrid;

public:
	CString m_strIniPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCREENPROFILESET_H__E6D21C9A_6A1E_47DC_8A18_8168ADC00E3F__INCLUDED_)
