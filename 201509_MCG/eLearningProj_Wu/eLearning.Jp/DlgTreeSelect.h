#if !defined(AFX_DLGTREESELECT_H__C9E23DA3_1A32_11D2_876C_444553540000__INCLUDED_)
#define AFX_DLGTREESELECT_H__C9E23DA3_1A32_11D2_876C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//
#include "ShellTree.h"

/////////////////////////////////////////////////////////////////////////////
// DlgTreeSelect dialog
class CImageContainer;
class CDlgTreeSelect : public CDialog
{
// Construction
public:
	CDlgTreeSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTreeSelect)
	enum { IDD = IDD_BROWSE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTreeSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTreeSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandingDirtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedDirtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemDirtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnButtonDefault();
	virtual void OnOK();
	afx_msg LRESULT OnPictureSelected(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CImageContainer*	m_pImageContainer;
	int					m_ncxGap;
	int					m_ncyGap;
	int					m_ncyBetween;
	BOOL				m_bTracking;
	CRect				m_rectLimit;
	CRect				m_rectTracker;
	BOOL				m_bTunneling;
	BOOL				m_bShowFileName;
	BOOL				m_bShowFileExt;
	int					m_nShowType;

protected:
	void StopTracking(BOOL bAccept);
	void StartTracking(int ht);
	void GetHitRect(CRect& rectHit);
	int HitTest(CPoint pt);
	void OnInvertTracker(const CRect& rect);
	void GetInsideRect(CRect& rect); 
	void SetResizeStyle();
	void SelchangedDirtree(CString strPath);
	void ResetSize();

public:
	CString m_strDefaultPath;
	CString m_strSelectedFile;
	BOOL	m_bSelectable;
	BOOL	m_bStretch;
	BOOL	m_bResize;
	CString m_strTitle;
	CString m_strShow;
	int		m_nFrameWidth;
	int		m_nFrameHeight;
	int		m_nWidth;
	int		m_nHeight;
	//add by buxiangyi
	BOOL	m_pPageLayout;
	BOOL	m_pPageImageLogo;
	BOOL	m_pPageImageBackGround;
	BOOL	m_pPageImagePhoto;

	

	
	

public:
	void ShowFileName(BOOL b); 
	void ShowFileExt(BOOL b);
	void SetShowType(int n);
	void SetFrameSize(int nWidth, int nHeight);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTREESELECT_H__C9E23DA3_1A32_11D2_876C_444553540000__INCLUDED_)
