#if !defined(AFX_DLGNOTEEDIT_H__09C282D7_BF6E_4826_8823_1709A38290CC__INCLUDED_)
#define AFX_DLGNOTEEDIT_H__09C282D7_BF6E_4826_8823_1709A38290CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNoteEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNoteEdit dialog

class CDlgNoteEdit : public CDialog
{
// Construction
public:
	CDlgNoteEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNoteEdit)
	enum { IDD = IDD_DIALOG_NOTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNoteEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNoteEdit)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strNote;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNOTEEDIT_H__09C282D7_BF6E_4826_8823_1709A38290CC__INCLUDED_)
