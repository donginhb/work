#if !defined(AFX_VOLUMEBAR_H__E1590F98_D01B_4642_A36C_F39FB42FE1B8__INCLUDED_)
#define AFX_VOLUMEBAR_H__E1590F98_D01B_4642_A36C_F39FB42FE1B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// VolumeBar.h : header file
//
// Written by Cristina Cañero (cristina@iti.gr)
// Copyright (c) 2003.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.


/////////////////////////////////////////////////////////////////////////////
// CVolumeBar window
#define MAX_VOLUME 100

class CVolumeBar : public CStatic
{
	bool isInteracting;
	bool isMouseOnThumb;
	bool isVertical;

	unsigned long volume;
	
	CRect thumbRect;

	
// Construction
public:
	CVolumeBar();
	virtual ~CVolumeBar();

// Attributes
public:
	COLORREF bkColor;
	COLORREF barColor;

// Operations
public:

	void SetVolume(unsigned long v);
	unsigned long GetVolume();

	bool GetOrientation();			// true : vertical, false : horizontal
	void SetOrientation(bool vertical = true);
	
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CVolumeBar)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void UpdateVolume(CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEBAR_H__E1590F98_D01B_4642_A36C_F39FB42FE1B8__INCLUDED_)
