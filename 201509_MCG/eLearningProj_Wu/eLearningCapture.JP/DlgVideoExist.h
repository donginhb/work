#if !defined(AFX_DLGVIDEOEXIST_H__4079F650_EA14_4F22_8C49_72511C9774C1__INCLUDED_)
#define AFX_DLGVIDEOEXIST_H__4079F650_EA14_4F22_8C49_72511C9774C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVideoExist.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoExist dialog

#define ID_OVERWRITE 128	
#define ID_APPEND    129	

class CDlgVideoExist : public CDialog
{
// Construction
public:
	CDlgVideoExist(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVideoExist)
	enum { IDD = IDD_VIDEO_EXIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVideoExist)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVideoExist)
	afx_msg void OnButtonAppend();
	afx_msg void OnButtonOverwrite();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEOEXIST_H__4079F650_EA14_4F22_8C49_72511C9774C1__INCLUDED_)
