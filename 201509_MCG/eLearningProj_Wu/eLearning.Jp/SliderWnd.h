#if !defined(AFX_SLIDERWND_H__ED37A409_FF19_4D1F_A668_649F33A868DA__INCLUDED_)
#define AFX_SLIDERWND_H__ED37A409_FF19_4D1F_A668_649F33A868DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SliderWnd.h : header file
//

#define WM_SLIDER_HSCROLL (WM_APP + 585)

/////////////////////////////////////////////////////////////////////////////
// CSliderWnd window
class CPointerWnd;
class CSliderWnd : public CWnd
{
// Construction
public:
	CSliderWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderWnd)
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSliderWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSliderWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg	LRESULT OnPointerMove(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPointerUp(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPointerDown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString m_strClass;
	CPen			m_penDkShadow;
	CPen			m_penShadow;
	CPen			m_penHilight;
	COLORREF		m_crChannel;
	COLORREF		m_crDkShadow;
	COLORREF		m_crShadow;
	COLORREF		m_crHilight;
	int				m_nMin;
	int				m_nMax;
	CRect			m_rcChannel;
	CPointerWnd*	m_pThumb;
	CSize			m_csThumb;
	long			m_nCurPos;
	CImageList		m_imageList_head;
	CImageList		m_imageList_body;
	CImageList		m_imageList_tail;
	int				m_nHeadWidth;
	int				m_nBodyWidth;
	int				m_nTailWidth;

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void CreatePens();
	void DeletePens();
	void DrawChannel(CDC *pDC, const CRect &rect);
	int GetRangeMin() { return m_nMin;};
	int GetRangeMax() { return m_nMax;};
	void GetColors();
	CSize GetBmpDim(UINT nID);
	long GetIndexFromPos(long nPos);
	long GetIndexFromRect(CPointerWnd* pWnd);
	long GetPosFromIndex(long nPos);
	int GetPointerTop()	{ return m_rcChannel.top;}

public:
	void SetChannelColor(COLORREF crColor);
	void SetRange(int nMin, int nMax) { m_nMin = nMin; m_nMax = nMax; };
	long GetPos() {return m_nCurPos;}
	void SetPos(long nPos, BOOL bForce = FALSE);
	void GetRange(int& nMin, int& nMax) { nMin = m_nMin; nMax = m_nMax; };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERWND_H__ED37A409_FF19_4D1F_A668_649F33A868DA__INCLUDED_)
