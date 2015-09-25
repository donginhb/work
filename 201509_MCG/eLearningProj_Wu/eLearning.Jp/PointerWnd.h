#if !defined(AFX_POINTERWND_H__2C559A7A_5CA2_484A_9095_404E6662BC18__INCLUDED_)
#define AFX_POINTERWND_H__2C559A7A_5CA2_484A_9095_404E6662BC18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPointerWnd window

#define constPointerNormal		0
#define constPointerOver		1
#define constPointerDown		2
#define constPointerDisable		3

#define WM_POINTER_UP				(WM_APP + 190)
#define WM_POINTER_DOWN				(WM_APP + 191)
#define WM_POINTER_MOVE				(WM_APP + 192)
#define WM_POINTER_MOUSE_MOVE		(WM_APP + 193)

class CPointerWnd : public CWnd
{
// Construction
public:
	CPointerWnd(UINT nBmpID);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPointerWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPointerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPointerWnd)
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString m_strClass;

	BOOL			m_bTracking;
	BOOL			m_bLeaveTrack;
	UINT			m_nStatus;
	CPoint			m_ptPrePos;
	CImageList		m_imageList;
	CToolTipCtrl*	m_pToolTip;
	int				m_nWidth;
	int				m_nHeight;
	CFont*			m_pFont;

protected:
	void LeaveTrack(); 

public:
	void SetButtonDown(BOOL b);
	void EnableButton(BOOL b);
	CPoint GetClickPoint() {return m_ptPrePos;}
	void ResetToolTip(CString strTip);
	void SetTipFont(CFont* pFont){ m_pFont = pFont; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POINTERWND_H__2C559A7A_5CA2_484A_9095_404E6662BC18__INCLUDED_)
