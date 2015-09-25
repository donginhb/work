// BmpBtn.cpp : implementation file
//

#include "stdafx.h"
//#include "eLearningCapture.h"
#include "BmpBtn.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CBmpBtn

CString CBmpBtn::m_strClass = "";

CBmpBtn::CBmpBtn(UINT nBmpID, UINT nType)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nBmpID);

	BITMAP b;
	bitmap.GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;
	int nCount = 4;
	UINT nWidth = nBmpWidth / nCount;
	ASSERT(nCount >= 2);

	m_imageList.Create(nWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, nCount, 1);
	m_imageList.Add(&bitmap, RGB(255, 0, 255));
	
	m_bTracking = FALSE;
	m_nType = nType;
	m_nStatus = constBtnNormal;
	m_bLeaveTrack = FALSE;
	m_nHeight = nBmpHeight;
	m_nWidth = nWidth;
	m_nBorderType = 0;
}

CBmpBtn::~CBmpBtn()
{
}


BEGIN_MESSAGE_MAP(CBmpBtn, CWnd)
	//{{AFX_MSG_MAP(CBmpBtn)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBmpBtn message handlers

BOOL CBmpBtn::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
					 	AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL);

	CRect rc = rect;
	if (rc.right < 0)
		rc.right = rc.left + m_nWidth;
	
	if (rc.bottom < 0)
		rc.bottom = rc.top + m_nHeight;

	return CWnd::Create(m_strClass, NULL, dwStyle, rc, pParentWnd, nID);
}


void CBmpBtn::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
}

void CBmpBtn::LeaveTrack() 
{
	if (!m_bLeaveTrack)
	{
		m_bLeaveTrack = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
	}
}

LRESULT CBmpBtn::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	if (m_nStatus == constBtnOver) //Button
	{
		m_nStatus = constBtnNormal;
		RedrawWindow();
	}

	return 0;
}

void CBmpBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	if ((m_nType == BTN_SUNK_BUTTON && m_nStatus == constBtnDown) ||
		(m_nStatus == constBtnDisable))
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	GetParent()->PostMessage(WM_BUTTON_DOWN, GetDlgCtrlID(), 0);

	m_bTracking = TRUE;
	SetCapture();
	SetFocus();

	if (m_nType == BTN_TOGGLE_BUTTON && m_nStatus == constBtnDown)
		m_nStatus = constBtnOver;
	else
		m_nStatus = constBtnDown;

	RedrawWindow();

	m_ptPrePos = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CBmpBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	m_bTracking = FALSE;

	ReleaseCapture();

	CRect rcClient;
	GetClientRect(rcClient);
	BOOL bIn = rcClient.PtInRect(point);

	if (bIn && m_nType == BTN_PUSH_BUTTON)
		m_nStatus = constBtnOver;
	else if (!bIn && m_nType == BTN_PUSH_BUTTON)
		m_nStatus = constBtnNormal;
	else if (bIn && m_nType == BTN_SUNK_BUTTON)
		m_nStatus = constBtnDown;
	else if (!bIn && m_nType == BTN_SUNK_BUTTON)
		m_nStatus = constBtnNormal;

	RedrawWindow();

	if (bIn)
		GetParent()->PostMessage(WM_BUTTON_UP, GetDlgCtrlID(), 0);

	CWnd::OnLButtonUp(nFlags, point);
}

void CBmpBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
	MouseMove(point);
}

void CBmpBtn::MouseMove(CPoint point) 
{
	if (m_bTracking)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		BOOL bCurIn = rcClient.PtInRect(point);
		BOOL bPreIn = rcClient.PtInRect(m_ptPrePos);

		if (m_nType == BTN_SUNK_BUTTON || m_nType == BTN_PUSH_BUTTON)
		{
			if (bCurIn && !bPreIn)
				m_nStatus = constBtnDown;
			else if (!bCurIn && bPreIn)
				m_nStatus = constBtnNormal;
		}
		else if (m_nType == BTN_TOGGLE_BUTTON)
		{
			if (bCurIn && !bPreIn)
			{
				if (m_nStatus == constBtnNormal)
					m_nStatus = constBtnDown;
				else if (m_nStatus == constBtnDown)
					m_nStatus = constBtnOver;
			}
			else if (!bCurIn && bPreIn)
			{
				if (m_nStatus == constBtnOver)
					m_nStatus = constBtnDown;
				else if (m_nStatus == constBtnDown)
					m_nStatus = constBtnNormal;
			}
		}
		if ((bCurIn && !bPreIn) || (!bCurIn && bPreIn))
			RedrawWindow();
	
		m_ptPrePos = point;
	}
	else
	{
		LeaveTrack();

		if (m_nStatus == constBtnNormal)
		{
			m_nStatus = constBtnOver;
			RedrawWindow();
		}
	}
}

BOOL CBmpBtn::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_TRANSPARENT;
	
	return CWnd::PreCreateWindow(cs);
}

BOOL CBmpBtn::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);

	CWnd*  pWndParent;       // handle of our parent window
	POINT  pt;         
	pWndParent = GetParent();         
	pt.x       = 0;
	pt.y       = 0;         
	MapWindowPoints(pWndParent, &pt, 1);
	OffsetWindowOrgEx(dc.m_hDC, pt.x, pt.y, &pt);
	::SendMessage(pWndParent->m_hWnd, WM_ERASEBKGND, (WPARAM)dc.m_hDC, 0);
	SetWindowOrgEx(dc.m_hDC, pt.x, pt.y, NULL);

	m_imageList.Draw(&dc, m_nStatus, CPoint(0, 0), ILD_TRANSPARENT);

	COLORREF crDkGray = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF crWhite = ::GetSysColor(COLOR_3DHILIGHT);
	COLORREF crBlack = ::GetSysColor(COLOR_3DDKSHADOW);
	CPen penWhite(PS_SOLID, 1, crWhite);
	CPen penDkGray(PS_SOLID, 1, crDkGray);
	CPen penBlack(PS_SOLID, 1, crBlack);
	CPen* pOldPen = dc.SelectObject(&penWhite);

	CRect rcClient;
	GetClientRect(rcClient);
	if (m_nBorderType == 1 || m_nBorderType == 2)
	{
		if ((m_nStatus == 0 && m_nBorderType == 2) || 
				 m_nStatus == 1 || m_nStatus == 3)
		{
			dc.MoveTo(0, rcClient.bottom - 2);
			dc.LineTo(0, 0);
			dc.LineTo(rcClient.right - 1, 0);
			
			dc.SelectObject(&penDkGray);
			dc.MoveTo(rcClient.right - 1, 0);
			dc.LineTo(rcClient.right - 1, rcClient.bottom - 1);
			dc.LineTo(-1, rcClient.bottom - 1);
		}
		else if (m_nStatus == 2)
		{
			dc.SelectObject(&penDkGray);
			dc.MoveTo(0, rcClient.bottom - 2);
			dc.LineTo(0, 0);
			dc.LineTo(rcClient.right - 1, 0);
			
			dc.SelectObject(&penWhite);
			dc.MoveTo(rcClient.right - 1, 0);
			dc.LineTo(rcClient.right - 1, rcClient.bottom - 1);
			dc.LineTo(-1, rcClient.bottom - 1);
		}
	}

	dc.SelectObject(pOldPen);
	
	return TRUE;
}

int CBmpBtn::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ToolTip.Create(this);
	
	return 0;
}

LRESULT CBmpBtn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			{
				MSG msg;
				msg.hwnd = m_hWnd;
				msg.message = message;
				msg.wParam = wParam;
				msg.lParam = lParam;

				m_ToolTip.RelayEvent(&msg);
			}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CBmpBtn::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx > 0 && cy > 0)
	{
		CString Str;
		Str.LoadString(GetDlgCtrlID());
		CRect rc;
		rc.SetRect(0, 0, cx - 1, cy - 1);
		m_ToolTip.AddTool(this, Str, rc, 1);
		m_ToolTip.Activate(TRUE);
	}
}

void CBmpBtn::SetButtonDown(BOOL b)
{
	m_nStatus = (b) ? constBtnDown : constBtnNormal;
	RedrawWindow();
}

void CBmpBtn::EnableButton(BOOL b)
{
	m_nStatus = (b) ? constBtnNormal: constBtnDisable;
	RedrawWindow();
	EnableWindow(b);
}



void CBmpBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}

	if ((m_nType == BTN_SUNK_BUTTON && m_nStatus == constBtnDown) ||
		(m_nStatus == constBtnDisable))
	{
		CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}

	GetParent()->PostMessage(WM_BUTTON_DOWN, GetDlgCtrlID(), 0);

	m_bTracking = TRUE;
	SetCapture();
	//SetFocus();

	if (m_nType == BTN_TOGGLE_BUTTON && m_nStatus == constBtnDown)
		m_nStatus = constBtnOver;
	else
		m_nStatus = constBtnDown;

	RedrawWindow();

	m_ptPrePos = point;
	
	CWnd::OnLButtonDblClk(nFlags, point);
}
