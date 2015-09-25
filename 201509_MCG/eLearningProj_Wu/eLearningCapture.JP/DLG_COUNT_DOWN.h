#if !defined(AFX_DLG_COUNT_DOWN_H__61E4CF4B_B41C_4E80_BBB1_7881E614AE67__INCLUDED_)
#define AFX_DLG_COUNT_DOWN_H__61E4CF4B_B41C_4E80_BBB1_7881E614AE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_COUNT_DOWN.h : header file
//
#define WM_USER_CountDown_TIMER			(WM_APP + 5656)
#include "GCCountdownCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDLG_COUNT_DOWN dialog

void TakeAction(UINT CtrlID);

class CDLG_COUNT_DOWN : public CDialog
{
// Construction
public:
	CDLG_COUNT_DOWN(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLG_COUNT_DOWN)
	enum { IDD = IDD_COUNT_DOWN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_COUNT_DOWN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_COUNT_DOWN)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnWM_USER_CountDown_TIMER(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CGCCountdownCtrl m_MyCtrl;
	void (*actfuncptr)(UINT CtrlID);
	UINT m_nTimerID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_COUNT_DOWN_H__61E4CF4B_B41C_4E80_BBB1_7881E614AE67__INCLUDED_)
