#if !defined(AFX_DLGDEFAULTPROFILESET_H__2364CF47_8E24_403C_9E41_6E8402C0AD19__INCLUDED_)
#define AFX_DLGDEFAULTPROFILESET_H__2364CF47_8E24_403C_9E41_6E8402C0AD19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefaultProfileSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultProfileSet dialog
class CDataBrowseWnd;
class CDlgDefaultProfileSet : public CDialog
{
// Construction
public:
	CDlgDefaultProfileSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDefaultProfileSet)
	enum { IDD = IDD_DIALOG_PROFILE_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefaultProfileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefaultProfileSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonDel();
	afx_msg void OnButtonRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDataBrowseWnd*		m_pAttribSelGrid;

public:
	CString m_strIniPath;
	BOOL m_bNoVideo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFAULTPROFILESET_H__2364CF47_8E24_403C_9E41_6E8402C0AD19__INCLUDED_)
