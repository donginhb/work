// ColorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "ColorWnd.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorWnd

CColorWnd::CColorWnd()
{
	m_crFill = RGB(255, 255, 128);
}

CColorWnd::~CColorWnd()
{
}


BEGIN_MESSAGE_MAP(CColorWnd, CWnd)
	//{{AFX_MSG_MAP(CColorWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColorWnd message handlers

void CColorWnd::OnPaint() 
{
	CPaintDC ptdc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(rcClient);

	CMemDC_MCG dc(&ptdc);
	CBrush br(m_crFill);
	
	dc.FillRect(rcClient, &br);
}

BOOL CColorWnd::Create(CRect rect, CWnd* pParentWnd) 
{
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, 0, NULL);
}


void CColorWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint pt;
	GetCursorPos(&pt);   
	long l = MAKELONG((WORD)pt.x, (WORD)pt.y);

	GetParent()->SendMessage(WM_COLORWND_MOUSE_MOVE, (WPARAM)l, (LPARAM)this);
		
	CWnd::OnMouseMove(nFlags, point);
}
