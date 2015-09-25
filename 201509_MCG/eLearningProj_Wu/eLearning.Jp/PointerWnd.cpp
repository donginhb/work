// PointerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PointerWnd.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointerWnd

CString CPointerWnd::m_strClass = "";

CPointerWnd::CPointerWnd(UINT nBmpID)
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
	m_nStatus = constPointerNormal;
	m_bLeaveTrack = FALSE;
	m_nHeight = nBmpHeight;
	m_nWidth = nWidth;
	m_pToolTip = new CToolTipCtrl;
	m_pFont = NULL;
}

CPointerWnd::~CPointerWnd()
{
	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CPointerWnd, CWnd)
	//{{AFX_MSG_MAP(CPointerWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPointerWnd message handlers

BOOL CPointerWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadCursor(MCG_IDC_HAND), NULL);

	CRect rc = rect;
	if (rc.right < 0)
		rc.right = rc.left + m_nWidth;
	
	if (rc.bottom < 0)
		rc.bottom = rc.top + m_nHeight;

	return CWnd::Create(m_strClass, NULL, dwStyle, rc, pParentWnd, nID);
}


void CPointerWnd::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
}

void CPointerWnd::LeaveTrack() 
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

LRESULT CPointerWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	if (m_nStatus == constPointerOver) //Button
	{
		m_nStatus = constPointerNormal;
		RedrawWindow();
	}

	return 0;
}

void CPointerWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	if (m_nStatus == constPointerDisable)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	CPoint pt = point;
	ClientToScreen(&pt);
	WPARAM wParam = MAKELONG(pt.x, pt.y);
	GetParent()->PostMessage(WM_POINTER_DOWN, wParam, (LPARAM)this);

	m_bTracking = TRUE;
	SetCapture();

	m_nStatus = constPointerDown;

	RedrawWindow();

	m_ptPrePos = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CPointerWnd::OnLButtonUp(UINT nFlags, CPoint point) 
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

	if (bIn)
		m_nStatus = constPointerOver;
	else if (!bIn)
		m_nStatus = constPointerNormal;

	RedrawWindow();

	CPoint pt;
	GetCursorPos(&pt);   
	long l = MAKELONG((WORD)pt.x, (WORD)pt.y);

	GetParent()->PostMessage(WM_POINTER_UP, l, (LPARAM)this);

	CWnd::OnLButtonUp(nFlags, point);
}

void CPointerWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	CPoint pt;
	GetCursorPos(&pt);   
	long l = MAKELONG((WORD)pt.x, (WORD)pt.y);
	if (m_bTracking)
	{
		if (m_ptPrePos == point)
			return;

		GetParent()->SendMessage(WM_POINTER_MOVE, (WPARAM)l, (LPARAM)this);
	}
	else
	{
		LeaveTrack();

		if (m_nStatus == constPointerNormal)
		{
			m_nStatus = constPointerOver;
			RedrawWindow();
		}

		GetParent()->SendMessage(WM_POINTER_MOUSE_MOVE, (WPARAM)l, (LPARAM)nFlags);
	}
}

BOOL CPointerWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_TRANSPARENT;
	
	return CWnd::PreCreateWindow(cs);
}

BOOL CPointerWnd::OnEraseBkgnd(CDC* pDC) 
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

	return TRUE;
}

int CPointerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create(this);
	m_pToolTip->SetFont(m_pFont);

	return 0;
}

void CPointerWnd::ResetToolTip(CString strTip)
{
	delete m_pToolTip;
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);
	m_pToolTip->SetFont(m_pFont);

	CRect rc;
	GetClientRect(rc);
	m_pToolTip->AddTool(this, strTip, rc, 1);
	m_pToolTip->Activate(TRUE);
}

LRESULT CPointerWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

				m_pToolTip->RelayEvent(&msg);
			}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CPointerWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx > 0 && cy > 0)
	{
		CString Str;
		Str.LoadString(GetDlgCtrlID());
		CRect rc;
		rc.SetRect(0, 0, cx - 1, cy - 1);
		m_pToolTip->AddTool(this, Str, rc, 1);
		m_pToolTip->Activate(TRUE);
	}
}

void CPointerWnd::SetButtonDown(BOOL b)
{
	m_nStatus = (b) ? constPointerDown : constPointerNormal;
	RedrawWindow();
}

void CPointerWnd::EnableButton(BOOL b)
{
	m_nStatus = (b) ? constPointerNormal: constPointerDisable;
	RedrawWindow();
	EnableWindow(b);
}

