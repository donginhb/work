// TWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TWnd.h"


// CTWnd

IMPLEMENT_DYNAMIC(CTWnd, CWnd)

CTWnd::CTWnd()
: panelWnd(NULL)
, isMouseDown(false)
, lastPoint(0)
{

}

CTWnd::~CTWnd()
{
}


BEGIN_MESSAGE_MAP(CTWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CTWnd message handlers




BOOL CTWnd::CreateEx(CCtrlPanelWnd* panelWnd, const RECT& rect)
{
	// TODO: Add your specialized code here and/or call the base class
	this->panelWnd = panelWnd;

	return CWnd::CreateEx(0, AfxRegisterWndClass(0), 0, WS_POPUP | WS_VISIBLE, rect, 0, 0);
}


int CTWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void CTWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isMouseDown = true;
	lastPoint = point;

	CWnd::OnLButtonDown(nFlags, point);
}


void CTWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isMouseDown = false;

	CWnd::OnLButtonUp(nFlags, point);
}


void CTWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(isMouseDown)
	{
#if 0
		CClientDC dc = CClientDC(this);
		CPen pen = CPen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen* oldPen = dc.SelectObject(&pen);
		dc.MoveTo(lastPoint);
		dc.LineTo(point);
		dc.SelectObject(oldPen);
#else
		HDC hDC = ::GetDC(NULL);
		//::SetROP2(hDC, (bEx) ? R2_XORPEN : R2_COPYPEN);

   		HPEN Pen1;                
		Pen1 = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HPEN OldPen;
		OldPen = (HPEN)::SelectObject(hDC, Pen1);

		::MoveToEx(hDC, lastPoint.x, lastPoint.y, NULL);
		::LineTo(hDC, point.x, point.y);
    
		::SelectObject(hDC, OldPen);

		::DeleteObject(Pen1);
		::ReleaseDC(NULL, hDC);
#endif
		lastPoint = point;
	}

	CWnd::OnMouseMove(nFlags, point);
}



void CTWnd::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	isMouseDown = false;

	CWnd::OnMouseLeave();
}
