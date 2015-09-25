#if !defined(AFX_PPTWND_H__B156A438_F380_4DFB_AA73_6B0B1C1BFCD8__INCLUDED_)
#define AFX_PPTWND_H__B156A438_F380_4DFB_AA73_6B0B1C1BFCD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPTWnd.h : header file
//

#define WM_NEXT_SLIDE			(WM_APP + 634)
#define WM_PREVIOUS_SLIDE		(WM_APP + 635)
#define WM_FIRST_SLIDE			(WM_APP + 636)
#define WM_LAST_SLIDE			(WM_APP + 637)
#define WM_END_SLIDE			(WM_APP + 638)
#define WM_LBTN_DOWN			(WM_APP + 639)
#define WM_CHANGE_SLIDE			(WM_APP + 649)
#define WM_GET_TIME_STAMP		(WM_APP + 650)
#define WM_ADD_PPT_RECORD		(WM_APP + 651)

/////////////////////////////////////////////////////////////////////////////
// CPPTWnd window
class CPPTPanel;
class CBmpBtn;
class CNumberEdit;
class CBmpStatic;
class CSliderWnd;
class CToolTipWnd;
class CPPTWnd : public CWnd
{
// Construction
public:
	CPPTWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPTWnd)
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPPTWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPPTWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterPressed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNextSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPreviousSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFirstSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLastSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBtnDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderHScroll(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	BOOL SetPageNo();
	void UnSetHook();
	BOOL SetHook();
	void ShowToolTip(long nPos);
	void OnLocate();
	void OnLocatePrev();
	void OnLocateNext();
	void OnGotoTitle();

public:
	BOOL LoadPPT(CString strPath, int nWidth, int nHeight);
	void GetSliderTitle(CStringArray& strArray);
	int GetCurrentSlideNumber(); 
	int GetCurrentAnimaNumber();
	CStringArray* GetTitleArray() { return &m_strArrayTitle;};
	void OnGoto(int i);
	void OnLast();
	void OnNext();
	void OnPrev();
	void OnFirst();
	int ChkPPTver();	//add 2014/7/29 kagata BugNo.130

protected:
	static HHOOK  m_hHook;
	static HINSTANCE m_hmodHook;
	static CString	m_strClass;

	unsigned long	m_lTime;
	CPPTPanel*		m_pPPTPanel;
	CBmpBtn*		m_pBtnFirst;
	CBmpBtn*		m_pBtnPrev;
	CBmpBtn*		m_pBtnNext;
	CBmpBtn*		m_pBtnLast;
	CBmpBtn*		m_pBtnGoto;
	CBmpBtn*		m_pBtnLocate;
	CBmpBtn*		m_pBtnLocateNext;
	CBmpBtn*		m_pBtnLocatePrev;
	CBmpStatic*		m_pBmpLabel1;
	CBmpStatic*		m_pBmpLabel2;
	CBmpStatic*		m_pBmpLabel3;
	CSliderWnd*		m_pSliderWnd;
	CNumberEdit*	m_pPageNoEdit;
	CStringArray	m_strArrayTitle;
	CToolTipWnd*	m_pToolTip;

	static BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);		//add 2014/10/22 kagata
	static BOOL CALLBACK EnumChildProc(HWND, LPARAM);		//add 2014/10/22 kagata
	BOOL GetSlideWnd();		//add 2014/10/22 kagata

public:
	CString m_strOp;

	//add 2014/7/29 kagata BugNo.130
	int nVer;
	int nPageflag;
	int prev;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTWND_H__B156A438_F380_4DFB_AA73_6B0B1C1BFCD8__INCLUDED_)
