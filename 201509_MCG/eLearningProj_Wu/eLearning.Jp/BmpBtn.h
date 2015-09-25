#if !defined(AFX_BMPBTN_H__69AB13FE_039F_4D37_A198_DC1E8738F99C__INCLUDED_)
#define AFX_BMPBTN_H__69AB13FE_039F_4D37_A198_DC1E8738F99C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpBtn window
#define constBtnNormal		0
#define constBtnOver		1
#define constBtnDown		2
#define constBtnDisable		3

typedef enum tagBTN_TYPE
{
	BTN_SUNK_BUTTON = 0,
	BTN_PUSH_BUTTON = 1,
	BTN_TOGGLE_BUTTON = 2,
}
BTN_TYPE;

#define WM_BUTTON_UP	(WM_APP + 170)
#define WM_BUTTON_DOWN	(WM_APP + 171)

class CBmpBtn : public CWnd
{
// Construction
public:
	CBmpBtn(UINT nBmpID, UINT nType);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpBtn)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpBtn)
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString m_strClass;

	BOOL			m_bTracking;
	BOOL			m_bLeaveTrack;
	UINT			m_nStatus;
	UINT			m_nType;
	CPoint			m_ptPrePos;
	CImageList		m_imageList;
	CToolTipCtrl	m_ToolTip;
	int				m_nWidth;
	int				m_nHeight;
	int				m_nBorderType;

protected:
	void LeaveTrack(); 
	void MouseMove(CPoint point); 

public:
	void SetButtonDown(BOOL b);
	void EnableButton(BOOL b);
	UINT GetButtonStatus(){return m_nStatus;};
	BOOL IsButtonDown() {return m_nStatus == constBtnDown;};
	void SetBorderType(int nType) {m_nBorderType = nType;};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBTN_H__69AB13FE_039F_4D37_A198_DC1E8738F99C__INCLUDED_)
