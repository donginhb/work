#if !defined(AFX_DLGCLOSETODO_H__ACD5B999_1644_4286_BC22_2F7BB230FF7C__INCLUDED_)
#define AFX_DLGCLOSETODO_H__ACD5B999_1644_4286_BC22_2F7BB230FF7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCloseToDo.h : header file
//

#define ID_EXIT_ONLY 20
#define ID_EXIT_GEN  21


/////////////////////////////////////////////////////////////////////////////
// CDlgCloseToDo dialog

class CDlgCloseToDo : public CDialog
{
// Construction
public:
	CDlgCloseToDo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCloseToDo)
	enum { IDD = IDD_CLOSE_TODO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCloseToDo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCloseToDo)
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonExitGen();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCLOSETODO_H__ACD5B999_1644_4286_BC22_2F7BB230FF7C__INCLUDED_)
