#if !defined(AFX_TOOLPALETEVIDEOEDIT_H__DBE5E46D_117E_471B_99B5_29840F45CBBF__INCLUDED_)
#define AFX_TOOLPALETEVIDEOEDIT_H__DBE5E46D_117E_471B_99B5_29840F45CBBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolPaleteVideoEdit.h : header file
//

#define WM_PALETE_BTN_DOWN (WM_APP + 2692)
/////////////////////////////////////////////////////////////////////////////
// CToolPaleteVideoEdit window
class CBmpBtn;

class CToolPaleteVideoEdit : public CWnd
{
// Construction
public:
	CToolPaleteVideoEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolPaleteVideoEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolPaleteVideoEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolPaleteVideoEdit)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSize m_csBtn;
	CBmpBtn* m_pBtnMark;
	CBmpBtn* m_pBtnCut;
	CBmpBtn* m_pBtnInsert;
	CBmpBtn* m_pBtnEditMark;
	CBmpBtn* m_pBtnAddEvent;
	CBmpBtn* m_pBtnEditSave;
	CBmpBtn* m_pBtnAddWave;
	CBmpBtn* m_pBtnAddText;
	CBmpBtn* m_pBtnAddEffect;
	CBmpBtn* m_pBtnMute;
	CBmpBtn* m_pBtnSetPoints;
	CBmpBtn* m_pBtnAdjust;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLPALETEVIDEOEDIT_H__DBE5E46D_117E_471B_99B5_29840F45CBBF__INCLUDED_)
