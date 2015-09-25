// ImageCell.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "ImageCell.h"
#include "MemDC.h"
#include "bmpapi.h"
#include "jpgapi.h"
#include "cmkapi.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CImageCell::m_strClass = "";

#define cSizeTextHeight 22

/////////////////////////////////////////////////////////////////////////////
// CImageCell

CImageCell::CImageCell()
{
	m_pFont = NULL;
	m_pBMP = NULL;
	m_bTracking = FALSE;
	m_nStatus = constBtnNormal;
	m_bWithFrame = FALSE;
	m_nMenuID = 0;
	m_bShowSize = TRUE;
	m_strFile = "";
	m_strSize = "";
	m_strTitle = "!@#$%";
	m_rgbTitleBg =RGB(255, 255, 214);
	m_rgbTitleFg = RGB(0, 0, 128);
}

CImageCell::~CImageCell()
{
	if (m_pBMP)
	{
		m_pBMP->DeleteObject();
		delete m_pBMP;
	}
}


BEGIN_MESSAGE_MAP(CImageCell, CWnd)
	//{{AFX_MSG_MAP(CImageCell)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageCell message handlers

void CImageCell::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!::IsWindow(m_hWnd))
		return;

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

BOOL CImageCell::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);
	return bRet;
}

BOOL CImageCell::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						  UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(0, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

void CImageCell::SetPath(CString strPath)
{
    if (strPath == "" || _access(strPath, 0) != 0)   // Check for existence
	{
		if (m_pBMP)
		{
			m_pBMP->DeleteObject();
			delete m_pBMP;
		}
		m_pBMP = NULL;
		m_strFile = "";
		m_strSize = "";
		RedrawWindow();
		return;
	}

	//CBitmap* pBMP = LoadDDB(strPath);
	CBitmap* pBMP = LoadFile(strPath);
	//CBitmap* pBMP = CreateRectBMP(RGB(255,0,0), 100, 100);
	if (pBMP)
	{
		if (m_pBMP)
		{
			m_pBMP->DeleteObject();
			delete m_pBMP;
		}
		m_pBMP = pBMP;
		m_strPath = strPath;
		m_strFile = ::GetFileName(strPath);

		BITMAP b;
		m_pBMP->GetBitmap(&b);
		CString strColor;
		if (b.bmBitsPixel == 1)
			strColor = "2";
		else if (b.bmBitsPixel == 2)
			strColor = "4";
		else if (b.bmBitsPixel == 4)
			strColor = "16";
		else if (b.bmBitsPixel == 8)
			strColor = "256";
		else if (b.bmBitsPixel == 16)
			strColor = "16b";
		else if (b.bmBitsPixel == 24)
			strColor = "24b";
		else if (b.bmBitsPixel == 32)
			strColor = "32b";

		m_strSize.Format("%dx%d", b.bmWidth, b.bmHeight);
		RedrawWindow();	
	}
}

BOOL CImageCell::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	DrawPicture(&dc);

	return TRUE;
}

void CImageCell::DrawFileInfo(CDC* pDC, CRect rcDC, CString strWrite)
{
	if (m_pFont == NULL)
		m_pFont = GetFont();

	CFont* pOldFont = pDC->SelectObject(m_pFont);

	pDC->SetTextColor(m_rgbTitleFg);
	pDC->SetBkMode(TRANSPARENT);
	
	CRect rcDraw = rcDC;
	rcDraw.top = rcDC.bottom - cSizeTextHeight + 1;
	rcDraw.left += 2;
	rcDraw.right -= 2;
	pDC->DrawText(strWrite, rcDraw, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CImageCell::DrawPicture(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);
	
	CRect rcDraw = rcClient;
	rcDraw.bottom -= ((m_bShowSize) ? cSizeTextHeight : 0);
	if (m_bWithFrame)
	{
		DrawPictureFrame(pDC, rcClient);
		rcDraw.InflateRect(-10, -10);
	}
	else
		rcDraw.InflateRect(-1, -1);
	int nHeight = rcDraw.Height();
	int nWidth = rcDraw.Width();

	if (m_bShowSize)
	{
		if (!m_bWithFrame)
		{
			CRect rcSq = rcDraw;
			rcSq.top = rcSq.bottom + 3;
			rcSq.bottom = rcDraw.bottom + cSizeTextHeight;
			
			CBrush br(m_rgbTitleBg);
			pDC->FillRect(rcSq, &br);
			
			CPen penLight, penDark;
			penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
			penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

			CPen* pOldPen = pDC->SelectObject(&penDark);
			pDC->MoveTo(rcSq.left, rcSq.bottom);
			pDC->LineTo(rcSq.left, rcSq.top);
			pDC->LineTo(rcSq.right, rcSq.top);
			pDC->SelectObject(&penLight);
			pDC->LineTo(rcSq.right, rcSq.bottom);
			pDC->LineTo(rcSq.left, rcSq.bottom);
			
			pDC->SelectObject(pOldPen);
			penLight.DeleteObject();
			penDark.DeleteObject();
		}

		if (m_strTitle != "!@#$%")
			DrawFileInfo(pDC, rcClient, m_strTitle);
		else
		{
			if (m_strFile != "" && m_strSize != "")
				DrawFileInfo(pDC, rcClient, m_strFile + "  (" + m_strSize + ")");
		}
	}

	if (m_pBMP != NULL)
	{
		BITMAP bm;
		m_pBMP->GetBitmap(&bm);
		int nBMPHeight = bm.bmHeight;
		int nBMPWidth = bm.bmWidth;
		if (nBMPHeight < nHeight && nBMPWidth < nWidth)
		{
			rcDraw.left += (nWidth - nBMPWidth) / 2;
			rcDraw.right = rcDraw.left + nBMPWidth;
			rcDraw.top += (nHeight - nBMPHeight) / 2;
			rcDraw.bottom = rcDraw.top + nBMPHeight;
		}
		else 
		{
			double dH = (double) nBMPHeight / nHeight;
			double dW = (double) nBMPWidth / nWidth;
			if (dH > dW)
			{
				int nNewWidth = nHeight * nBMPWidth / nBMPHeight;
				rcDraw.left += (nWidth - nNewWidth) / 2;
				rcDraw.right = rcDraw.left + nNewWidth;
			}
			else 
			{
				int nNewHeight = nWidth * nBMPHeight / nBMPWidth;
				rcDraw.top += (nHeight - nNewHeight) / 2;
				rcDraw.bottom = rcDraw.top + nNewHeight;
			}
		}

		if (m_nStatus == constBtnDown)
			rcDraw.OffsetRect(1, 1);
		DrawInnerFrame(pDC, rcDraw);

		if (m_pBMP)
		{
			CRect rcBMP;
			rcBMP.left = 0;
			rcBMP.top = 0;
			rcBMP.right = rcBMP.left + nBMPWidth;
			rcBMP.bottom = rcBMP.top + nBMPHeight;
			::DDBPaint(pDC->m_hDC, rcDraw, (HBITMAP)m_pBMP->m_hObject, rcBMP, NULL);
		}
		else
		{
			CBrush br(RGB(255, 255, 255));
			pDC->FillRect(rcDraw, &br);
		}
	}
}

void CImageCell::DrawPictureFrame(CDC* pDC, CRect rc)
{
	rc.right--;
	rc.bottom--;

	CPen penLight, penDark;
	penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	CRect rcSq = rc;
	rcSq.bottom -= ((m_bShowSize) ? cSizeTextHeight : 0);

	CPen* pOldPen;
	if (m_nStatus == constBtnNormal)
		pOldPen = pDC->SelectObject(&penLight);
	else
		pOldPen = pDC->SelectObject(&penDark);
	pDC->MoveTo(rcSq.left, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.top);
	pDC->LineTo(rcSq.right, rcSq.top);
	if (m_nStatus == constBtnNormal)
		pDC->SelectObject(&penDark);
	else
		pDC->SelectObject(&penLight);
	pDC->LineTo(rcSq.right, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.bottom);

	if (m_bShowSize)
	{
		rcSq.top = rcSq.bottom + 3;
		rcSq.bottom = rc.bottom;
		
		CBrush br(m_rgbTitleBg);
		pDC->FillRect(rcSq, &br);
		
		pDC->SelectObject(&penDark);
		pDC->MoveTo(rcSq.left, rcSq.bottom);
		pDC->LineTo(rcSq.left, rcSq.top);
		pDC->LineTo(rcSq.right, rcSq.top);
		pDC->SelectObject(&penLight);
		pDC->LineTo(rcSq.right, rcSq.bottom);
		pDC->LineTo(rcSq.left, rcSq.bottom);	
	}

	pDC->SelectObject(pOldPen);
	penLight.DeleteObject();
	penDark.DeleteObject();
}

void CImageCell::DrawInnerFrame(CDC* pDC, CRect rc)
{
	CPen penLight, penDark;
	penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	rc.left--;
	rc.top--;

	CPen* pOldPen = pDC->SelectObject(&penDark);
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);
	pDC->SelectObject(&penLight);
	pDC->LineTo(rc.right, rc.bottom);
	pDC->LineTo(rc.left, rc.bottom);

	pDC->SelectObject(pOldPen);
	penLight.DeleteObject();
	penDark.DeleteObject();
}

void CImageCell::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	m_bTracking = TRUE;
	SetCapture();

	m_nStatus = constBtnDown;
	RedrawWindow();

	m_ptPrePos = point;
*/	
	CWnd::OnLButtonDown(nFlags, point);
}

void CImageCell::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*
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

	m_nStatus = constBtnNormal;
	RedrawWindow();

	if (bIn)
		GetParent()->PostMessage(WM_IMAGE_CELL_UP, GetDlgCtrlID(), 0);
*/
	CWnd::OnLButtonUp(nFlags, point);
}

void CImageCell::OnMouseMove(UINT nFlags, CPoint point) 
{
/*
	if (m_bTracking)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		BOOL bCurIn = rcClient.PtInRect(point);
		BOOL bPreIn = rcClient.PtInRect(m_ptPrePos);

		if (bCurIn && !bPreIn)
			m_nStatus = constBtnDown;
		else if (!bCurIn && bPreIn)
			m_nStatus = constBtnNormal;

		if ((bCurIn && !bPreIn) || (!bCurIn && bPreIn))
			RedrawWindow();
	
		m_ptPrePos = point;
	}
*/	
	CWnd::OnMouseMove(nFlags, point);
}

void CImageCell::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	GetParent()->PostMessage(WM_IMAGE_CELL_DBL_CLICK, GetDlgCtrlID(), 0);
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

int CImageCell::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ToolTip.Create(this);
	
	return 0;
}

LRESULT CImageCell::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

void CImageCell::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_nMenuID == 0)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(m_nMenuID));
	CMenu* pPopup = menu.GetSubMenu(0);
	
	ASSERT(pPopup != NULL);
	CPoint pts(point);
	ClientToScreen(&pts);
	UINT nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RIGHTBUTTON | TPM_RETURNCMD, pts.x, pts.y,this);
	if (nCmd != 0)
		GetParent()->SendMessage(WM_CELL_CMD, GetDlgCtrlID(), nCmd);

	CWnd::OnRButtonUp(nFlags, point);
}
