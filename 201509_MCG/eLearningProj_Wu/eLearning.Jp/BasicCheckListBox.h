#if !defined(AFX_BASICCHECKLISTBOX_H__C6FC32CD_1052_4D35_B383_FE5BEB042649__INCLUDED_)
#define AFX_BASICCHECKLISTBOX_H__C6FC32CD_1052_4D35_B383_FE5BEB042649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasicCheckListBox.h : header file
//

#define WM_CHECK_CHANGE (WM_APP + 981)

/////////////////////////////////////////////////////////////////////////////
// CBasicCheckListBox window

class CBasicCheckListBox : public CCheckListBox
{
// Construction
public:
	CBasicCheckListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicCheckListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasicCheckListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBasicCheckListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICCHECKLISTBOX_H__C6FC32CD_1052_4D35_B383_FE5BEB042649__INCLUDED_)
