#if !defined(AFX_NUMBEREDIT_H__73630BEB_E07A_4D38_B763_BB360E86CED1__INCLUDED_)
#define AFX_NUMBEREDIT_H__73630BEB_E07A_4D38_B763_BB360E86CED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumberEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumberEdit window

#define WM_ENTER_PRESSED (WM_APP + 138)

class CNumberEdit : public CEdit
{
// Construction
public:
	CNumberEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumberEdit)
	public:
		virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNumberEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNumberEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMBEREDIT_H__73630BEB_E07A_4D38_B763_BB360E86CED1__INCLUDED_)
