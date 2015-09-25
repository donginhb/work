#if !defined(AFX_IMAGECELL_H__CEE022B3_378B_4BE9_BCE0_B196D77332CE__INCLUDED_)
#define AFX_IMAGECELL_H__CEE022B3_378B_4BE9_BCE0_B196D77332CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageCell.h : header file
//

#define WM_IMAGE_CELL_UP			(WM_APP + 790)
#define WM_IMAGE_CELL_DBL_CLICK		(WM_APP + 791)
#define WM_CELL_CMD					(WM_APP + 799)

#ifndef constBtnNormal
#define constBtnNormal		0
#endif

#ifndef constBtnOver
#define constBtnOver		1
#endif

#ifndef constBtnDown
#define constBtnDown		2
#endif

#ifndef constBtnDisable
#define constBtnDisable		3
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageCell window

class CImageCell : public CWnd
{
// Construction
public:
	CImageCell();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageCell)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID); 
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageCell();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageCell)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString m_strClass;
	CString			m_strPath;
	CBitmap*		m_pBMP;
	BOOL			m_bTracking;
	UINT			m_nStatus;
	CPoint			m_ptPrePos;
	CToolTipCtrl	m_ToolTip;
	CString			m_strFile;
	CString			m_strSize;
	CString			m_strTitle;

public:
	CFont*			m_pFont;
	BOOL			m_bWithFrame;
	UINT			m_nMenuID;
	BOOL			m_bShowSize;
	COLORREF		m_rgbTitleBg;
	COLORREF		m_rgbTitleFg;

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void DrawPicture(CDC* pDC);
	void DrawPictureFrame(CDC* pDC, CRect rc);
	void DrawInnerFrame(CDC* pDC, CRect rc);
	void DrawFileInfo(CDC* pDC, CRect rcDC, CString strWrite);

public:
	void SetPath(CString strPath);
	void SetTitle(CString strTitle) {m_strTitle = strTitle;};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECELL_H__CEE022B3_378B_4BE9_BCE0_B196D77332CE__INCLUDED_)
