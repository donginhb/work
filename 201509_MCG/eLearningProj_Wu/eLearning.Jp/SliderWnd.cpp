// SliderWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "resource.h"
#include "SliderWnd.h"
#include "PointerWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderWnd

CString CSliderWnd::m_strClass = "";

CSliderWnd::CSliderWnd()
{
	m_crChannel = ::GetSysColor(COLOR_WINDOW);
	m_rcChannel.SetRectEmpty();
	m_pThumb = new CPointerWnd(IDB_THUMB);
	m_csThumb = GetBmpDim(IDB_THUMB);	
	m_csThumb.cx /= 4;
	m_nCurPos = 0;

	CBitmap bitmapHead;
	bitmapHead.LoadBitmap(IDB_THUMB_HEAD);

	BITMAP bHead;
	bitmapHead.GetBitmap(&bHead);
	int nBmpWidth = bHead.bmWidth;
	int nBmpHeight = bHead.bmHeight;
	m_nHeadWidth = nBmpWidth;
	m_imageList_head.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_head.Add(&bitmapHead, RGB(255, 0, 255));

	CBitmap bitmapBody;
	bitmapBody.LoadBitmap(IDB_THUMB_BODY);

	BITMAP bBody;
	bitmapBody.GetBitmap(&bBody);
	nBmpWidth = bBody.bmWidth;
	nBmpHeight = bBody.bmHeight;
	m_nBodyWidth = nBmpWidth;
	m_imageList_body.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_body.Add(&bitmapBody, RGB(255, 0, 255));

	CBitmap bitmapTail;
	bitmapTail.LoadBitmap(IDB_THUMB_TAIL);

	BITMAP bTail;
	bitmapTail.GetBitmap(&bTail);
	nBmpWidth = bTail.bmWidth;
	nBmpHeight = bTail.bmHeight;
	m_nTailWidth = nBmpWidth;
	m_imageList_tail.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_tail.Add(&bitmapTail, RGB(255, 0, 255));
}

CSliderWnd::~CSliderWnd()
{
	delete m_pThumb;
}


BEGIN_MESSAGE_MAP(CSliderWnd, CWnd)
	//{{AFX_MSG_MAP(CSliderWnd)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_MESSAGE(WM_POINTER_MOVE, OnPointerMove)
	ON_MESSAGE(WM_POINTER_UP, OnPointerUp)
	ON_MESSAGE(WM_POINTER_DOWN, OnPointerDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSliderWnd message handlers

BOOL CSliderWnd::OnEraseBkgnd(CDC* pDC) 
{
   	GetColors();
	CreatePens();

	CBrush backBrush(GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
	
	if (!m_rcChannel.IsRectEmpty())
	{
		DrawChannel(pDC, m_rcChannel);
	}

	DeletePens();

	return CWnd::OnEraseBkgnd(pDC);
}

BOOL CSliderWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
										 NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CSliderWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(NULL, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

void CSliderWnd::DrawChannel(CDC *pDC, const CRect &rect)
{
	if (rect.IsRectEmpty())
		return;

	CRect rc(rect);

/*
	// If the slider has a selection, shrink the rect a little.
	int nRangeMin = GetRangeMin();
	int nRangeMax = GetRangeMax();
	//if (nRangeMin || nRangeMax)
		rc.InflateRect(0, -1, 0, -1);

	// Another minor adjustment for the rect.
		rc.InflateRect(-1, 0, -1, 1);

	CPen *pOldPen = pDC->SelectObject(&m_penDkShadow);
	CBrush br(m_crChannel);
	CBrush *pOldBrush = pDC->SelectObject(&br);

	// If the height or width is > 3, draw the inner round rect.
	//if ((bHorz && rc.Height() > 3) || (!bHorz && rc.Width() > 3))
		pDC->RoundRect(rc, CPoint(2, 2));

	rc.InflateRect(1, 1, 1, 0);

	// Give it a sunken appearance.
	pDC->SelectObject(&m_penShadow);
	pDC->MoveTo(rc.left, rc.bottom - 2);
	pDC->LineTo(rc.left, rc.top + 2);
	pDC->LineTo(rc.left + 2, rc.top);
	pDC->LineTo(rc.right - 2, rc.top);

	pDC->SelectObject(&m_penHilight);
	pDC->MoveTo(rc.left + 1, rc.bottom - 1);
	pDC->LineTo(rc.left + 2, rc.bottom);
	pDC->LineTo(rc.right - 2, rc.bottom);
	pDC->LineTo(rc.right - 2, rc.bottom - 1);
	pDC->LineTo(rc.right - 1, rc.bottom - 1);
	pDC->LineTo(rc.right - 1, rc.top + 2);
	pDC->LineTo(rc.right - 3, rc.top);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
*/
	m_imageList_head.Draw(pDC, 0, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
	int nStart = rc.left + m_nHeadWidth;
	for (int i = 0;; i++)
	{
		if (nStart + m_nBodyWidth > rect.right)
			break;
		m_imageList_body.Draw(pDC, 0, CPoint(nStart, rc.top), ILD_TRANSPARENT);
		nStart += m_nBodyWidth;
	}
	m_imageList_tail.Draw(pDC, 0, CPoint(nStart, rc.top), ILD_TRANSPARENT);
}

void CSliderWnd::CreatePens()
{
	DeletePens();

	m_penDkShadow.CreatePen(PS_SOLID, 1, m_crDkShadow);
	m_penShadow.CreatePen(PS_SOLID, 1, m_crShadow);
	m_penHilight.CreatePen(PS_SOLID, 1, m_crHilight);
}

void CSliderWnd::DeletePens()
{
	if (m_penDkShadow.m_hObject)
		m_penDkShadow.DeleteObject();
	if (m_penShadow.m_hObject)
		m_penShadow.DeleteObject();
	if (m_penHilight.m_hObject)
		m_penHilight.DeleteObject();
}

void CSliderWnd::GetColors()
{
	m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);

	// Get a color halfway between COLOR_3DSHADOW AND COLOR_3DFACE,
	// and COLOR_3DDKSHADOW and COLOR_3DSHADOW.
	COLORREF crFace = ::GetSysColor(COLOR_3DFACE);
	COLORREF crShadow = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
	BYTE byRedFace = GetRValue(crFace);
	BYTE byRedShadow = GetRValue(crShadow);
	BYTE byRedDkShadow = GetRValue(crDkShadow);
	BYTE byGreenFace = GetGValue(crFace);
	BYTE byGreenShadow = GetGValue(crShadow);
	BYTE byGreenDkShadow = GetGValue(crDkShadow);
	BYTE byBlueFace = GetBValue(crFace);
	BYTE byBlueShadow = GetBValue(crShadow);
	BYTE byBlueDkShadow= GetBValue(crDkShadow);

	m_crShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
							byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
							byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

	m_crDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
							byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
							byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
}	// GetColors

void CSliderWnd::SetChannelColor(COLORREF crColor)
{
	m_crChannel = crColor;
}

void CSliderWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rcClient;
	GetClientRect(rcClient);
	m_rcChannel.left = rcClient.left + 8;
	m_rcChannel.top = rcClient.top + 7;
	m_rcChannel.right = rcClient.right - 8;
	m_rcChannel.bottom = m_rcChannel.top + 9;
}

void CSliderWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pThumb->DestroyWindow();	
}

int CSliderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	//rect.SetRectEmpty();
	rect.top = 7;
	rect.bottom = rect.top + m_csThumb.cy;
	rect.left = 8 - m_csThumb.cx / 2;
	rect.right = rect.left + m_csThumb.cx;
	m_pThumb->Create(WS_CHILD | WS_VISIBLE, rect, this, 0);
	
	return 0;
}

CSize CSliderWnd::GetBmpDim(UINT nID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);
	return CSize(b.bmWidth, b.bmHeight);
}

LRESULT CSliderWnd::OnPointerDown(WPARAM wParam, LPARAM lParam)
{
	if (m_nMin == 0 && m_nMax == 0)
		return 0L;

	long nIndex = GetIndexFromRect((CPointerWnd*)lParam);
	GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_THUMBTRACK, (LPARAM)this);
	return 0L;
}

long CSliderWnd::GetIndexFromRect(CPointerWnd* pWnd)
{
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	long lPos = rc.left + m_csThumb.cx / 2;
	int nWidth = m_rcChannel.Width(); 
	if (nWidth > m_nMax)
		lPos++; 
	return GetIndexFromPos(lPos);
}

long CSliderWnd::GetIndexFromPos(long nPtx)
{
	if (m_rcChannel.IsRectEmpty())
		return 0;

	if (m_nMax == 0)
		return 0;

	int nWidth = m_rcChannel.Width();
	int nGap = nWidth / m_nMax;
	nPtx += nGap / 2;
	long n = (nPtx - m_rcChannel.left) * m_nMax / nWidth;
	if (n < 0)
		n = 0;
	else if (n > m_nMax)
		n = m_nMax;
	return n;
}

long CSliderWnd::GetPosFromIndex(long nPos)
{
	if (m_rcChannel.IsRectEmpty())
		return -1;

	if (m_nMax == 0)
		return -1;

	int nWidth = m_rcChannel.Width();
	long n = m_rcChannel.left + nPos * nWidth / m_nMax;

	//int nSize = m_csPointer.cx / 2;
	//int nGap = nWidth / (m_nMax + 1);
	//long n = m_rcChannel.left + nPos * nWidth / (m_nMax + 1) - nSize + nGap / 2;
	return n;
}

LRESULT CSliderWnd::OnPointerUp(WPARAM wParam, LPARAM lParam)
{
	if (m_nMin == 0 && m_nMax == 0)
		return 0L;

	CPointerWnd* pWnd = (CPointerWnd*)lParam;
	long nMark = GetIndexFromRect(pWnd);
	
	GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_THUMBPOSITION, (LPARAM)this);
	GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_ENDSCROLL, (LPARAM)this);

	return 0L;
}

LRESULT CSliderWnd::OnPointerMove(WPARAM wParam, LPARAM lParam)
{
	if (m_nMin == 0 && m_nMax == 0)
		return 0L;

	CPoint point;
	point.x = (int)LOWORD(wParam);
	point.y = (int)HIWORD(wParam);

	ScreenToClient(&point);

	CPointerWnd* pWnd = (CPointerWnd*)lParam;

	CRect rcThumb;
	pWnd->GetWindowRect(rcThumb);
	ScreenToClient(rcThumb);

	long nIndex = GetIndexFromPos(point.x);
	if (m_nCurPos == nIndex)
		return 0L;

	int nSize = m_csThumb.cx / 2;
	int nWidth = m_rcChannel.Width(); 
	long nptx;
	if (nIndex == 0)
		nptx = m_rcChannel.left;
	else if (nIndex == m_nMax)
		nptx = m_rcChannel.right;
	else if (nWidth > m_nMax)
		nptx = GetPosFromIndex(nIndex);
	else
		nptx = point.x;
	nptx = nptx - nSize;// + nGap / 2; 

	int nTop = GetPointerTop();
	
	pWnd->SetWindowPos(NULL, nptx, nTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pWnd->RedrawWindow();
	m_nCurPos = nIndex;

	GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_THUMBTRACK, (LPARAM)this);

	return 0L;
}

void CSliderWnd::SetPos(long nPos, BOOL bForce)
{
	if (nPos == m_nCurPos && !bForce)
		return;

	int nSize = m_csThumb.cx / 2;
	int nWidth = m_rcChannel.Width(); 
	long nptx;
	if (nPos == 0)
		nptx = m_rcChannel.left;
	else if (nPos == m_nMax)
		nptx = m_rcChannel.right;
	else 
		nptx = GetPosFromIndex(nPos);
	nptx = nptx - nSize;// + nGap / 2; 

	int nTop = GetPointerTop();

	m_pThumb->SetWindowPos(NULL, nptx, nTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_nCurPos = nPos;

	m_pThumb->RedrawWindow();

	//GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_THUMBPOSITION, (LPARAM)this);
}


void CSliderWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcChannel = m_rcChannel;
	rcChannel.top--;
	rcChannel.bottom += 2;
	if (rcChannel.PtInRect(point))
	{
		long nIndex = GetIndexFromPos(point.x);	
		if (nIndex != m_nCurPos)
		{
			SetPos(nIndex);
			GetParent()->SendMessage(WM_SLIDER_HSCROLL, SB_THUMBPOSITION, (LPARAM)this);
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CSliderWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	//::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	CWnd::OnMouseMove(nFlags, point);
}
