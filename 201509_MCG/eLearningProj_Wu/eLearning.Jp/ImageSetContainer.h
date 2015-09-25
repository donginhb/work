#if !defined(AFX_CIMAGESETCONTAINER_H__DD770C4F_7EAC_4237_A9D4_447854A5D67C__INCLUDED_)
#define AFX_CIMAGESETCONTAINER_H__DD770C4F_7EAC_4237_A9D4_447854A5D67C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CImageSetContainer.h : header file
//

#define WM_PICTURE_SET_SELECTED (WM_APP + 3438)

#define constShowNormal			0
#define constShowFit			1
#define constShowByRate			2 
#define constShowOneEachRow		3 

/////////////////////////////////////////////////////////////////////////////
// CImageSetContainer window

class CImageSetContainer : public CWnd
{
// Construction
public:
	CImageSetContainer();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageSetContainer)
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd); 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageSetContainer();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageSetContainer)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString m_strClass;
	CObArray*		m_pDrawArray;
	int				m_nTotalRow;
	int				m_nVisibleY;
	int				m_nVisibleX;
	int				m_nBMPWidth;
	int				m_nBMPHeight;
	int				m_nFocus;
	BOOL			m_bScroll;
	CString			m_strPath;
	CStringArray	m_strArrayFile;
	CFont*			m_pFont;
	int				m_nInitBMPHeight;
	CSize			m_csImage;
	CWordArray		m_wdArrayCurrentIndex;
	int				m_nID;

protected:
	void	ReCalcIndex();
	BOOL	CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	
	int		IndexFromPt(CPoint pt);
	void	DrawSelRect(CDC* pDC, CRect rc);
	CRect	GetRectFromIndex(int nIndex);
	void	GetDrawCount();
	void	DrawPicture(CDC* pDC);
	void	DrawPictureFrame(CDC* pDC, CRect rc);
	void	DrawInnerFrame(CDC* pDC, CRect rc);
	void	SetDrawArray(CStringArray* pFileArray = NULL, CString StrPath = "");
	void	DrawFileName(CDC* pDC, CRect rcDC, int nIndex);
	void	CalcFrameSize();
	BOOL	GetFirstFileDim(CSize& cs);

public:
	void	SetPath(CString strPath);
	BOOL	GetSelectedFile(CString& strPath);
	void	SetTextFont(CFont* pFont) { m_pFont = pFont; };
	void	SetFrameSize(int nWidth, int nHeight);

public:
	BOOL	m_bShowFileName;
	BOOL	m_bShowFileExt;
	BOOL	m_bSelectable;
	CString m_strSelectedFile;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIMAGESETCONTAINER_H__DD770C4F_7EAC_4237_A9D4_447854A5D67C__INCLUDED_)
