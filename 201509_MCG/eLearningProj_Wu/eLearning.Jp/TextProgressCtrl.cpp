// TextProgressCtrl.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright 1998.
//
// Modified : 26/05/98 Jeremy Davis, jmd@jvf.co.uk
//				Added colour routines
//
// TextProgressCtrl is a drop-in replacement for the standard 
// CProgressCtrl that displays text in a progress control.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to
// the me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

#include "stdafx.h"
#include "TextProgressCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

CTextProgressCtrl::CTextProgressCtrl()
{
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CTextProgressCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTextProgressCtrl::OnPaint() 
{
	int nMin, nMax;
	GetRange(nMin, nMax);
    if (nMin >= nMax) 
        return;

	int nPos = GetPos();

	COLORREF colFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	COLORREF colBk			= ::GetSysColor(COLOR_WINDOW);
	COLORREF colTextFore	= ::GetSysColor(COLOR_HIGHLIGHT);
	COLORREF colTextBk		= ::GetSysColor(COLOR_WINDOW);

    CRect LeftRect, RightRect, ClientRect;
    GetClientRect(ClientRect);

    double Fraction = ((double)(nPos - nMin)) / ((double)(nMax - nMin));
    double dPerc = ((double)(nPos - nMin) * 100.0) / ((double)(nMax - nMin));

	CPaintDC PaintDC(this); // device context for painting
    CMemDC_MCG dc(&PaintDC);
	//CPaintDC dc(this);    // device context for painting (if not double buffering)

    LeftRect = RightRect = ClientRect;

    LeftRect.right = LeftRect.left + (int)((LeftRect.right - LeftRect.left)*Fraction);
    dc.FillSolidRect(LeftRect, colFore);

    RightRect.left = LeftRect.right;
    dc.FillSolidRect(RightRect, colBk);

    CString str;
	int n = (int)dPerc;
    str.Format("%d%%", n);

    dc.SetBkMode(TRANSPARENT);

    CRgn rgn;
    rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
    dc.SelectClipRgn(&rgn);
    dc.SetTextColor(colTextBk);

    dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rgn.DeleteObject();
    rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
    dc.SelectClipRgn(&rgn);
    dc.SetTextColor(colTextFore);

    dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    rgn.DeleteObject();
}

