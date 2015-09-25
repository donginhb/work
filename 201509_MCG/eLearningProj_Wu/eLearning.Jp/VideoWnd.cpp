// VideoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "VideoWnd.h"
#include "MemDc.h"
#include "PointerWnd.h"
#include "cmkapi.h"
#include "macro.h"
#include "TooltipWnd.h"
#include "grdapi.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CVideoWnd::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CVideoWnd

CVideoWnd::CVideoWnd()
{
	m_csPointer = ::GetBmpDim(IDB_BOUNDARY);	
	m_csPointer.cx /= 4;
	m_pToolTip = NULL;
	m_lTimePointer = -1;
	m_lTimeSelStart = -1;
	m_lTimeSelEnd = -1;
	m_pPointer = NULL;
	m_pTimeSeq = new CObArray;
	m_pFileSeq = new CObArray;
	m_pDelSeq = new CObArray;
	m_bLeaveTrack = FALSE;
	m_nCtrlEntered = -1;
	m_nDelEntered = -1;
}

CVideoWnd::~CVideoWnd()
{
	::ReleaseObArray(m_pTimeSeq);
	delete m_pTimeSeq;

	::ReleaseObArray(m_pFileSeq);
	delete m_pFileSeq;

	::ReleaseObArray(m_pDelSeq);
	delete m_pDelSeq;
}


BEGIN_MESSAGE_MAP(CVideoWnd, CWnd)
	//{{AFX_MSG_MAP(CVideoWnd)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_MESSAGE(WM_POINTER_DOWN, OnPointerDown)
	ON_MESSAGE(WM_POINTER_UP, OnPointerUp)
	ON_MESSAGE(WM_POINTER_MOVE, OnPointerMove)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVideoWnd message handlers

void CVideoWnd::DrawThumbnails(CDC *pDC)
{
	RECT panelRc, drawRc;
	this->GetClientRect(&panelRc);
	int nInterval = 10000;
	long lRight = PosToTime(panelRc.right);
	long lLeft = PosToTime(panelRc.left);
	int nPanelLength = lRight - lLeft;
	int nStep = nPanelLength / nInterval / 20 + 1;
	CString strThumbDir = MACRO_HTML_DIR(MACRO_MAINFRM->GetCourse()) + "THUMB2";
	int nPos = TimeToPos(m_lLength);
	if (panelRc.right > nPos)
		panelRc.right = nPos;
	for (int i = 0; i * nInterval <= m_lLength; i = i + nStep)
	{
		CString strThumb = strThumbDir + "\\VideoThumb";
		strThumb.AppendFormat("%d", i);
		strThumb += ".bmp";
		CImage thumbImage;
		thumbImage.Load(strThumb);
		if (!thumbImage.IsNull())
		{
			int imageHeight = thumbImage.GetHeight();
			int imageWidth = thumbImage.GetWidth();
			drawRc = panelRc;
			drawRc.left = TimeToPos(i * nInterval);
			drawRc.right = drawRc.bottom * imageWidth / imageHeight + drawRc.left;
			if (drawRc.left >= panelRc.left && drawRc.right < panelRc.right)
				thumbImage.Draw(pDC->m_hDC, drawRc, Gdiplus::InterpolationModeDefault);
		}
	}
}

BOOL CVideoWnd::OnEraseBkgnd(CDC* pOrigDC) 
{
	//CMemDC_MCG dc(pOrigDC);
	//CDC* pDC = &dc;

	CMemDC_MCG* pDC = new CMemDC_MCG(pOrigDC);

	CRect rcClient;
	GetClientRect(rcClient);

	CBrush br(RGB(144, 144, 144));//RGB(132, 130, 132));//RGB(156, 207, 206));
	pDC->FillRect(&rcClient, &br);

	CBrush brRed(RGB(255, 255, 0));
	CBrush brGreen(RGB(0, 128, 0));
	CBrush brBlue(RGB(255, 0, 0));
	int i = 0;
	for (i = 0; i < m_pFileSeq->GetSize(); i++)
	{
		CRect rc = GetFileSeqRect(i);
		if (!rc.IsRectEmpty())
		{
			CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(i);
			if (pSeg->m_nStatus == constBtnOver || pSeg->m_nStatus == constBtnDown)
				pDC->FillRect(&rc, &brRed);
			else if (pSeg->m_nStatus == constBtnNormal)
				pDC->FillRect(&rc, &brGreen);
		}
	}

	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));
	CPen penDkGray(PS_SOLID, 1, RGB(144, 144, 144));
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	
	CBrush brWhite(RGB(255, 255, 255));
	CBrush brBlack(RGB(96, 96, 96));

	CBrush* pOldBrush = pDC->SelectObject(&brWhite);
	CPen* pOldPen = pDC->SelectObject(&penWhite);

	if (m_lTimeStart == 0)
	{
		pDC->MoveTo(1, rcClient.bottom - 2);
		//pDC->LineTo(1, 0);
		//pDC->LineTo(rcClient.right, 0);

		pDC->SelectObject(&penDkGray);
		pDC->MoveTo(1, rcClient.bottom - 2);
		//pDC->LineTo(rcClient.right, rcClient.bottom - 2);
	}
	else
	{
		pDC->MoveTo(1, 0);
		//pDC->LineTo(rcClient.right, 0);

		pDC->SelectObject(&penDkGray);
		pDC->MoveTo(1, rcClient.bottom - 2);
		//pDC->LineTo(rcClient.right, rcClient.bottom - 2);
	}
	
	pDC->SelectObject(&penBlack);
	pDC->MoveTo(0, rcClient.bottom - 1);
	//pDC->LineTo(rcClient.right, rcClient.bottom - 1);

	int nDelIndex = -1;
	for (i = 0; i < m_pTimeSeq->GetSize() - 1; i++)
	{
		CRect rcDraw = GetBoundaryRect(i);
		BOOL bDel = IsDel(i);
		if (bDel)
			nDelIndex++;
		if (!rcDraw.IsRectEmpty())
		{
			if (!bDel)
			{
				pDC->SelectObject(&penDkGray);
				pDC->SelectObject(&brWhite);
			}
			else
			{
				CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(nDelIndex);
				if (pSeg->m_nStatus == constBtnOver)
					pDC->SelectObject(&brBlue);
				else
					pDC->SelectObject(&brBlack);

				pDC->SelectObject(&penWhite);
			}
			pDC->Rectangle(rcDraw);
		}
	}

	DrawThumbnails(pDC);

	if (PosToTime(rcClient.right) > m_lLength)
	{
		int nPos = TimeToPos(m_lLength);

		pDC->SelectObject(&penBlack);
		pDC->MoveTo(nPos + 1, 0);
		//pDC->LineTo(nPos + 1, rcClient.bottom - 1);

		pDC->SelectObject(&penDkGray);
		pDC->MoveTo(nPos, 1);
		//pDC->LineTo(nPos, rcClient.bottom - 2);

		CBrush bkBrush(RGB(132, 130, 132));
		CRect rc = rcClient;
		rc.left = nPos + 2;
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&bkBrush);
	    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(pOldBrush);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pDC;

	if (m_lTimeSelStart >= 0)													//直接化在��EC, 否則重畫時 Pattern
	{																			//會糊掉
		CRect rect = rcClient;
		rect.top = 1;
		rect.bottom = rcClient.bottom - 2;
		
		rect.left = max(0, TimeToPos(m_lTimeSelStart) + 1);
		rect.right = min(rcClient.right, TimeToPos(m_lTimeSelEnd) + 1);
		if (rect.right >= 0 && rect.left <= rcClient.right)
			DrawSelRect(rect, pOrigDC);
	}

	return TRUE;
}

int CVideoWnd::TimeToPos(long lTime)
{
	return (lTime - m_lTimeStart) * m_nPixelUnit / m_nUnit;
}

long CVideoWnd::PosToTime(int xPos)
{
	long lRet = m_lTimeStart + m_nUnit * xPos / m_nPixelUnit;
	return lRet;
}

void CVideoWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pPointer->DestroyWindow();
	delete m_pPointer;

	m_pToolTip->DestroyWindow();
	delete m_pToolTip;
}

BOOL CVideoWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CVideoWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

int CVideoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	rect.SetRectEmpty();
	m_pPointer = new CPointerWnd(IDB_BOUNDARY);
	m_pPointer->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_POINTER_START);
	m_pPointer->SetTipFont(MACRO_SYS_FONT2);

	m_pToolTip = new CToolTipWnd;
	m_pToolTip->Create(this);
	m_pToolTip->SetDrawFont(MACRO_SYS_FONT2);

	return 0;
}

void CVideoWnd::HidePointer()
{
	m_pPointer->ShowWindow(SW_HIDE);
	m_lTimePointer = -1;
}

void CVideoWnd::SetPointerTime(long lTime)
{
	if (m_lTimeSelStart >= 0)
	{
		m_lTimeSelStart = -1;
		m_lTimeSelEnd = -1;
		RedrawWindow();
	}
	else
	{
		if (m_lTimePointer < 0)
		{
			m_lTimePointer = lTime;
			SetPointerPos();
		}
		else
		{
			GetParent()->SendMessage(WM_GET_POINTER_TIME);
			m_lTimeSelStart = min(m_lTimeCurrent, m_lTimePointer);
			m_lTimeSelEnd = max(m_lTimeCurrent, m_lTimePointer);
			if (m_lTimeSelStart == m_lTimeSelEnd)
				m_lTimeSelStart = m_lTimeSelEnd = -1;
			HidePointer();
			RedrawWindow();
		}
	}
}

BOOL CVideoWnd::IsTimeVisible(long lTime)
{
	CRect rcClient;
	GetClientRect(rcClient);

	long lTimeStart = PosToTime(0);
	long lTimeEnd = PosToTime(rcClient.right);
	if (lTime >= lTimeStart && lTime <= lTimeEnd)
		return TRUE;

	return FALSE;
}

BOOL CVideoWnd::IsPointerVisible()
{
	if (m_lTimePointer < 0)
		return FALSE;

	CRect rcClient;
	GetClientRect(rcClient);

	long lTimeStart = PosToTime(-m_csPointer.cx);
	long lTimeEnd = PosToTime(rcClient.right + m_csPointer.cx);
	if (m_lTimePointer >= lTimeStart && m_lTimePointer <= lTimeEnd)
		return TRUE;

	return FALSE;
}

void CVideoWnd::SetPointerPos()
{
	if (IsPointerVisible())
	{
		int nX = TimeToPos(m_lTimePointer);
		int nptx = nX - m_csPointer.cx / 2 + 2;
		int nTop = 1;
		m_pPointer->SetWindowPos(&wndTop, nptx, nTop, m_csPointer.cx, m_csPointer.cy, SWP_SHOWWINDOW);
		m_pPointer->RedrawWindow();
	}
	else
	{
		if (m_pPointer)
			m_pPointer->ShowWindow(SW_HIDE);
	}
}

void CVideoWnd::SetPixelUnit(int lTime)
{
	m_nPixelUnit = lTime;
	SetPointerPos();
}

void CVideoWnd::SetTimeStart(long lTime)
{
	m_lTimeStart = lTime;
	SetPointerPos();
}

void CVideoWnd::SetTimeUnit(int nTimeUnit)
{
	m_nUnit = nTimeUnit;
	SetPointerPos();
}

LRESULT CVideoWnd::OnPointerDown(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	point.x = (int)LOWORD(wParam);
	point.y = (int)HIWORD(wParam);

	CPointerWnd* pWnd = (CPointerWnd*)lParam;

	CRect rcThumb;
	pWnd->GetWindowRect(rcThumb);

	m_ptOffset.x = rcThumb.left - point.x;
	m_ptOffset.y = rcThumb.top - point.y;
	
	ShowToolTip(m_lTimePointer);

	return 0L;
}

LRESULT CVideoWnd::OnPointerUp(WPARAM wParam, LPARAM lParam)
{
	CRect rcPtr;
	m_pPointer->GetWindowRect(rcPtr);
	ScreenToClient(rcPtr);
	int nPtrWidth = m_csPointer.cx / 2 - 2;

	m_lTimePointer = PosToTime(rcPtr.left + nPtrWidth);
	SetPointerPos();	

	m_pToolTip->ShowWindow(SW_HIDE);

	return 0L;
}

LRESULT CVideoWnd::OnPointerMove(WPARAM wParam, LPARAM lParam)
{
	MovePointer((int)LOWORD(wParam), (int)HIWORD(wParam));

	return 0L;
}

void CVideoWnd::MovePointer(int nX, int nY)
{
	CPoint point;
	point.x = nX;
	point.y = nY;

	ScreenToClient(&point);

	CRect rcThumb;
	m_pPointer->GetWindowRect(rcThumb);
	ScreenToClient(rcThumb);

	int nPtrWidth = m_csPointer.cx / 2 - 2;

	CRect rcClient;
	GetClientRect(rcClient);

	int nptx = point.x + m_ptOffset.x;
	nptx = TimeToPos(PosToTime(nptx + nPtrWidth)) - nPtrWidth;

	m_pPointer->SetWindowPos(NULL, nptx, 1, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_pPointer->RedrawWindow();

	ShowToolTip(PosToTime(nptx + nPtrWidth));
}

void CVideoWnd::ShowToolTip(long nPos)
{
	CPoint pt;
	GetCursorPos(&pt);
	pt.x += 6;
	pt.y += 20;

	CString strTime = GetTimeStr(nPos);
	m_pToolTip->ShowText(pt, strTime);
}

CString CVideoWnd::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

HBITMAP CVideoWnd::CreateDitherBitmap()
{
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD      bmiColors[16];
	} bmi;
	memset(&bmi, 0, sizeof(bmi));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 8;
	bmi.bmiHeader.biHeight = 8;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	COLORREF clr = ::GetSysColor(COLOR_BTNFACE);

	bmi.bmiColors[0].rgbBlue = GetBValue(clr);
	bmi.bmiColors[0].rgbGreen = GetGValue(clr);
	bmi.bmiColors[0].rgbRed = GetRValue(clr);

	clr = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	bmi.bmiColors[1].rgbBlue = GetBValue(clr);
	bmi.bmiColors[1].rgbGreen = GetGValue(clr);
	bmi.bmiColors[1].rgbRed = GetRValue(clr);

	// initialize the brushes
	long patGray[8];
	for (int i = 0; i < 8; i++)
	   patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;

	HDC hDC = ::GetDC(NULL);
	HBITMAP hbm = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT,
		(LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
	::ReleaseDC(NULL, hDC);

	return hbm;
}

void CVideoWnd::DrawSelRect(CRect& rect, CDC* pDC)
{
	static WORD _dotPatternBmp[8] = { 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055};

	HBITMAP hbm;
	HBRUSH  hbr, hbrushOld;

	hbm = CreateBitmap(8, 8, 1, 1, _dotPatternBmp);
	hbr = CreatePatternBrush(hbm);
	
	SetBrushOrgEx(pDC->m_hDC, rect.left, rect.top, 0);
	hbrushOld = (HBRUSH)SelectObject(pDC->m_hDC, hbr);
	
	PatBlt(pDC->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	
	SelectObject(pDC->m_hDC, hbrushOld);
	
	DeleteObject(hbr);
	DeleteObject(hbm);
}

/*
void CVideoWnd::DrawSelRect(CRect& rect, CDC* pDC)
{
	//CBrush br(RGB(255, 0, 0));
	//pDC->FillRect(rect, &br);
	//return;

	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
    HBRUSH hb;
	BITMAP bm;
    HBITMAP hbm;

	// See the KB Article Q68569 for information about how to draw the 
	// resizing rectangle.  That's where this pattern comes from.
    WORD aZigzag[] = { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA };

	// Fill out the bitmap structure for the PatBlt calls later
    bm.bmType = 0;
    bm.bmWidth = 8;
    bm.bmHeight = 8;
    bm.bmWidthBytes = 2;
    bm.bmPlanes = 1;
    bm.bmBitsPixel = 1;
    bm.bmBits = aZigzag;

    //hbm = ::CreateBitmapIndirect(&bm);
    //hbm = CreateDitherBitmap();

	CBitmap bmp;
	bmp.LoadBitmap(IDB_PAT);

	CBrush br;
	br.CreatePatternBrush(&bmp);
	bmp.DeleteObject();

	//hb = ::CreatePatternBrush(hbm);   

    // By specifying NULL for the HWND in GetDC(), we get the DC for the
    // entire screen.
//    ::SelectObject((HDC)pDC->m_hDC, hb);

	pDC->FillRect(&rect, &br);

	//::PatBlt((HDC)pDC->m_hDC, rect.left, rect.top, width, height, PATCOPY);

    // Clean up
    //::DeleteObject(hb);
    //::DeleteObject(hbm);
}
*/

void CVideoWnd::SetLength(long lTimeLength)
{ 
	m_lLength = lTimeLength;
}

void CVideoWnd::Initialize()
{
	m_lTimeSelStart = -1;
	m_lTimeSelEnd = -1;
	HidePointer();
}

BOOL CVideoWnd::UnDeletePeriod(long lTime, long& lLenAdded)
{
	int nIndex = -1;
	int i = 0;
	for (i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		if (pSeg->m_lLogicalTimeEnd == lTime && i < m_pTimeSeq->GetSize() - 1)
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex < 0)
		return FALSE;

	CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(nIndex);
	CTimeSeg* pNextSeg = (CTimeSeg*)m_pTimeSeq->GetAt(nIndex + 1);

	lLenAdded = pNextSeg->m_lPhysicalTimeStart - pSeg->m_lPhysicalTimeEnd;

	long lLen = pNextSeg->m_lPhysicalTimeEnd - pSeg->m_lPhysicalTimeStart;
	pSeg->m_lPhysicalTimeEnd = pNextSeg->m_lPhysicalTimeEnd;
	pSeg->m_lLogicalTimeEnd = pSeg->m_lLogicalTimeStart + lLen;
	
	m_pTimeSeq->RemoveAt(nIndex + 1);
	delete pNextSeg;

	for (i = nIndex + 1; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		pSeg->m_lLogicalTimeStart += lLenAdded;
		pSeg->m_lLogicalTimeEnd += lLenAdded;
	}

	ReCalcFileSeq();

	return TRUE;
}

void CVideoWnd::ReplacePeriod(long lTimeSelStart, long lTimeSelEnd, CString strFileName, long lDuration)
{
	if (lTimeSelStart < 0)
		return;

	if (lTimeSelStart >= 0 && lTimeSelEnd < 0)
	{
		ArrangeNodeInsert(lTimeSelStart, lDuration, strFileName);   //insert
		ReCalcFileSeq();
	}
	else if (lTimeSelStart >= 0 && lTimeSelEnd >= 0)                //replace
	{
		ArrangeNodeDelete(lTimeSelStart, lTimeSelEnd);	
		ArrangeNodeInsert(lTimeSelStart, lDuration, strFileName); 
		ReCalcFileSeq();
	}

	SetNoSelect();
	RedrawWindow();
}

void CVideoWnd::SetNoSelectAndRepaint()
{
	SetNoSelect();
	RedrawWindow();
}

void CVideoWnd::DeletePeriod(long lTimeSelStart, long lTimeSelEnd)
{
	if (lTimeSelStart < 0)
		return;
	
	ArrangeNodeDelete(lTimeSelStart, lTimeSelEnd);	
	ReCalcFileSeq();

	SetNoSelect();
	RedrawWindow();
}

void CVideoWnd::ArrangeNodeInsert(long lTimeInsert, long lDuration, CString strFilePath)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;

		if (lTimeInsert > lLogicalTimeStart && lTimeInsert < lLogicalTimeEnd)
		{
			CTimeSeg* pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lTimeInsert;
			pSegNew->m_lLogicalTimeEnd = lTimeInsert + lDuration;
			pSegNew->m_lPhysicalTimeStart = 0;
			pSegNew->m_lPhysicalTimeEnd = lDuration;
			pSegNew->m_strFileName = strFilePath;
			m_pTimeSeq->InsertAt(i, pSegNew);

			pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lLogicalTimeStart;
			pSegNew->m_lLogicalTimeEnd = lTimeInsert;
			pSegNew->m_lPhysicalTimeStart = lPysicalTimeStart;
			pSegNew->m_lPhysicalTimeEnd = lPysicalTimeStart + lTimeInsert - lLogicalTimeStart;
			pSegNew->m_strFileName = pSeg->m_strFileName;
			m_pTimeSeq->InsertAt(i, pSegNew);

			pSeg->m_lLogicalTimeStart = lTimeInsert + lDuration;
			pSeg->m_lLogicalTimeEnd = lLogicalTimeEnd + lDuration;
			pSeg->m_lPhysicalTimeStart = lPysicalTimeEnd - (lLogicalTimeEnd - lTimeInsert);
			pSeg->m_lPhysicalTimeEnd = lPysicalTimeEnd;

			i += 2;
		}
		else if (lTimeInsert < lLogicalTimeStart)
		{
			pSeg->m_lLogicalTimeStart = lLogicalTimeStart + lDuration;
			pSeg->m_lLogicalTimeEnd = lLogicalTimeEnd + lDuration;
		}
		else if (lTimeInsert == lLogicalTimeStart)
		{
			CTimeSeg* pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lTimeInsert;
			pSegNew->m_lLogicalTimeEnd = lTimeInsert + lDuration;
			pSegNew->m_lPhysicalTimeStart = 0;
			pSegNew->m_lPhysicalTimeEnd = lDuration;
			pSegNew->m_strFileName = strFilePath;
			m_pTimeSeq->InsertAt(i, pSegNew);

			pSeg->m_lLogicalTimeStart = lLogicalTimeStart + lDuration;
			pSeg->m_lLogicalTimeEnd = lLogicalTimeEnd + lDuration;

			i++;
		}
	}

	if (m_lLength == lTimeInsert)
	{
		CTimeSeg* pSegNew = new CTimeSeg;
		pSegNew->m_lLogicalTimeStart = lTimeInsert;
		pSegNew->m_lLogicalTimeEnd = lTimeInsert + lDuration;
		pSegNew->m_lPhysicalTimeStart = 0;
		pSegNew->m_lPhysicalTimeEnd = lDuration;
		pSegNew->m_strFileName = strFilePath;
		m_pTimeSeq->Add(pSegNew);
	}

	m_lLength += lDuration;
}

void CVideoWnd::ReCalcFileSeq()
{
	::ReleaseObArray(m_pFileSeq);
	int i = 0;
	for (i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		CString strFileName = pSeg->m_strFileName;
		if (strFileName != "*")
		{
			for (int j = i + 1; j < m_pTimeSeq->GetSize(); j++)
			{
				CTimeSeg* pNextSeg = (CTimeSeg*)m_pTimeSeq->GetAt(j);
				if (strFileName == pNextSeg->m_strFileName)
				{
					lLogicalTimeEnd = pNextSeg->m_lLogicalTimeEnd;
					i = j;
				}
				else
					break;
			}

			CTimeSeg* pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lLogicalTimeStart;
			pSegNew->m_lLogicalTimeEnd = lLogicalTimeEnd;
			pSegNew->m_strFileName = strFileName;
			pSegNew->m_nStatus = constBtnNormal;
			m_pFileSeq->Add(pSegNew);
		}
	}

	::ReleaseObArray(m_pDelSeq);
	for (i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		if (IsDel(i))
		{
			CTimeSeg* pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lLogicalTimeEnd;
			pSegNew->m_nStatus = constBtnNormal;
			m_pDelSeq->Add(pSegNew);
		}
	}
}

CRect CVideoWnd::GetFileSeqRect(int nIndex)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(nIndex);
	long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
	long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
	int nXStart = TimeToPos(lLogicalTimeStart);
	int nXEnd = TimeToPos(lLogicalTimeEnd);
	if (nXStart > rcClient.right || nXEnd < rcClient.left)
		return CRect(0, 0, 0, 0);

	CRect rc = rcClient;
	rc.left = nXStart;
	rc.right = nXEnd + 4;

	return rc;
}

void CVideoWnd::MergeNode()
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;
		CString strFileName = pSeg->m_strFileName;

		if (i < m_pTimeSeq->GetSize() - 1)
		{
			CTimeSeg* pSegNext = (CTimeSeg*)m_pTimeSeq->GetAt(i + 1);
			long lNextLogicalTimeStart = pSegNext->m_lLogicalTimeStart;
			long lNextLogicalTimeEnd = pSegNext->m_lLogicalTimeEnd;
			long lNextPysicalTimeStart = pSegNext->m_lPhysicalTimeStart;
			long lNextPysicalTimeEnd = pSegNext->m_lPhysicalTimeEnd;
			CString strNextFileName = pSegNext->m_strFileName;

			if (lLogicalTimeEnd == lNextLogicalTimeStart && 
				lPysicalTimeEnd == lNextPysicalTimeStart && 
				strFileName == strNextFileName)
			{
				pSeg->m_lLogicalTimeEnd = pSegNext->m_lLogicalTimeEnd;
				pSeg->m_lPhysicalTimeEnd = pSegNext->m_lPhysicalTimeEnd;
				
				m_pTimeSeq->RemoveAt(i + 1);
				delete pSegNext;

				i--;
			}
		}
	}
}

void CVideoWnd::ArrangeNodeDelete(long lTimeSelStart, long lTimeSelEnd)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;

		if (lLogicalTimeStart >= lTimeSelEnd)
		{
			pSeg->m_lLogicalTimeStart = lLogicalTimeStart - (lTimeSelEnd - lTimeSelStart);
			pSeg->m_lLogicalTimeEnd = lLogicalTimeEnd - (lTimeSelEnd - lTimeSelStart);
		}
		else if (lTimeSelStart >= lLogicalTimeEnd)
		{
			//Do nothing
		}
		else if (lTimeSelStart > lLogicalTimeStart && lLogicalTimeEnd > lTimeSelEnd)
		{
			CTimeSeg* pSegNew = new CTimeSeg;
			pSegNew->m_lLogicalTimeStart = lLogicalTimeStart;
			pSegNew->m_lLogicalTimeEnd = lTimeSelStart;
			pSegNew->m_lPhysicalTimeStart = lPysicalTimeStart;
			pSegNew->m_lPhysicalTimeEnd = lPysicalTimeStart + lTimeSelStart - lLogicalTimeStart;
			pSegNew->m_strFileName = pSeg->m_strFileName;
			m_pTimeSeq->InsertAt(i, pSegNew);

			pSeg->m_lLogicalTimeStart = lTimeSelStart;
			pSeg->m_lLogicalTimeEnd = lLogicalTimeEnd - (lTimeSelEnd - lTimeSelStart);
			pSeg->m_lPhysicalTimeStart = lPysicalTimeEnd - (lLogicalTimeEnd - lTimeSelEnd);
			pSeg->m_lPhysicalTimeEnd = lPysicalTimeEnd;

			i++;
		}
		else if (lLogicalTimeStart >= lTimeSelStart && lTimeSelEnd >= lLogicalTimeEnd)
		{
			m_pTimeSeq->RemoveAt(i);
			i--;
			delete pSeg;
		}
		else if (lTimeSelEnd > lLogicalTimeStart && lTimeSelEnd < lLogicalTimeEnd && 
				 lTimeSelStart <= lLogicalTimeStart)
		{
			pSeg->m_lLogicalTimeStart = lTimeSelStart;
			pSeg->m_lLogicalTimeEnd = lTimeSelStart + lLogicalTimeEnd - lTimeSelEnd;
			pSeg->m_lPhysicalTimeStart = lPysicalTimeEnd - (lLogicalTimeEnd - lTimeSelEnd);
		}
		else if (lTimeSelStart > lLogicalTimeStart && lTimeSelStart < lLogicalTimeEnd && 
				 lTimeSelEnd >= lLogicalTimeEnd)
		{
			pSeg->m_lLogicalTimeEnd = lTimeSelStart;
			pSeg->m_lPhysicalTimeEnd = lPysicalTimeStart + lTimeSelStart - lLogicalTimeStart;
		}
	}

	//for (i = 0; i < m_dwArrayDel.GetSize(); i++)
	//{
	//	long lTimeDel = m_dwArrayDel[i];
	//	if (lTimeDel >= lTimeSelStart && lTimeDel <= lTimeSelEnd)
	//	{
	//		m_dwArrayDel.RemoveAt(i);
	//		i--;
	//	}
	//	else if (lTimeDel > lTimeSelEnd)
	//	{
	//		lTimeDel -= (lTimeSelEnd - lTimeSelStart);
	//		m_dwArrayDel[i] = lTimeDel;
	//	}
	//}
	//m_dwArrayDel.Add(lTimeSelStart);

	m_lLength -= (lTimeSelEnd - lTimeSelStart);

	MergeNode();
}

void CVideoWnd::SetNoSelect()
{
	m_lTimeSelStart = -1;
	m_lTimeSelEnd = -1;
	HidePointer();
}

BOOL CVideoWnd::IsDel(int nIndex)
{
	if (nIndex >= m_pTimeSeq->GetSize() - 1)
		return FALSE;

	CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(nIndex);
	CString strFile = pSeg->m_strFileName;

	CTimeSeg* pSegNext = (CTimeSeg*)m_pTimeSeq->GetAt(nIndex + 1);
	CString strNextFile = pSegNext->m_strFileName;

	return (strFile == strNextFile);
}

CRect CVideoWnd::GetBoundaryRect(int nIndex)
{
	CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(nIndex);
	long lTime = pSeg->m_lLogicalTimeEnd;
	return GetTimeRect(lTime);
}

CRect CVideoWnd::GetDelRect(int nIndex)
{
	CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(nIndex);
	long lTime = pSeg->m_lLogicalTimeStart;
	return GetTimeRect(lTime);
}

CRect CVideoWnd::GetTimeRect(long lTime)
{	
	CRect rcPos;
	rcPos.SetRectEmpty();

	CRect rcClient;
	GetClientRect(rcClient);

	if (IsTimeVisible(lTime))
	{
		int nPos = TimeToPos(lTime);
		rcPos.top = 1;
		rcPos.bottom = rcClient.bottom - 2;
		rcPos.left = nPos - 1;
		rcPos.right = nPos + 4;
		if (lTime == m_lLength)
			rcPos.left--;
	}
	return rcPos;
}

void CVideoWnd::LeaveTrack() 
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

LRESULT CVideoWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	int i = 0;
	for (i = 0; i < m_pFileSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(i);
		if (pSeg->m_nStatus == constBtnOver) //Button
		{
			pSeg->m_nStatus = constBtnNormal;
			RedrawWindow(GetFileSeqRect(i));
		}
	}

	for (i = 0; i < m_pDelSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(i);
		if (pSeg->m_nStatus == constBtnOver) //Button
		{
			pSeg->m_nStatus = constBtnNormal;
			RedrawWindow(GetDelRect(i));
		}
	}

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return 0;
}

void CVideoWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_pTimeSeq->GetSize() == 1)
		return;

	LeaveTrack();

	int nCtrl = CtrlEntered(point);
	if (nCtrl != m_nCtrlEntered)
		LeaveCtrl(m_nCtrlEntered);

	int nDelCtrl = DelEntered(point);
	if (nDelCtrl != m_nDelEntered)
		LeaveDel(m_nDelEntered);

	if (nCtrl < 0 && nDelCtrl < 0)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return;
	}

   	::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
	
	if (nCtrl >= 0)
		EnterCtrl(nCtrl);

	if (nDelCtrl >= 0)
		EnterDel(nDelCtrl);
}

void CVideoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nClickedIndex = CtrlEntered(point);
	m_nClickedDel = DelEntered(point);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CVideoWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	int nClickedIndex = CtrlEntered(point);
	int nClickedDel = DelEntered(point);
	if (m_nClickedDel >= 0 && nClickedDel == m_nClickedDel)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(m_nClickedDel);
		long lTime = pSeg->m_lLogicalTimeStart;
		GetParent()->PostMessage(WM_PERIOD_CLICKED, lTime, lTime);
	}
	else if (m_nClickedIndex >= 0 && m_nClickedIndex == nClickedIndex)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(m_nClickedIndex);
		long lTimeStart = pSeg->m_lLogicalTimeStart;
		long lTimeEnd = pSeg->m_lLogicalTimeEnd;
		GetParent()->PostMessage(WM_PERIOD_CLICKED, lTimeStart, lTimeEnd);
	}

	m_nClickedIndex = -1;
	m_nClickedDel = -1;
}

BOOL CVideoWnd::GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;

		if (lTime >= lLogicalTimeStart && lTime <= lLogicalTimeEnd)
		{
			lVideoPos = (lTime - lLogicalTimeStart) + lPysicalTimeStart;
			strFileName = pSeg->m_strFileName;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CVideoWnd::GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;

		if (lTime >= lLogicalTimeStart && lTime <= lLogicalTimeEnd)
		{
			if (i !=  m_pTimeSeq->GetSize() - 1)
			{
				pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i + 1);
				lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
				lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
				lVideoPos = (lTime - lLogicalTimeStart) + lPysicalTimeStart;
				strFileName = pSeg->m_strFileName;
				return TRUE;
			}
			else
			{
				lVideoPos = -1;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CVideoWnd::GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;
		CString strFile = pSeg->m_strFileName;

		if (strFileName == strFile && lVideoPos >= lPysicalTimeStart && lVideoPos <= lPysicalTimeEnd)
		{
			lTime = (lVideoPos - lPysicalTimeStart) + lLogicalTimeStart;
			return TRUE;
		}
	}

	return FALSE;
}

void CVideoWnd::GetSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath)
{
	str = "";
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
		long lPysicalTimeStart = pSeg->m_lPhysicalTimeStart;
		long lPysicalTimeEnd = pSeg->m_lPhysicalTimeEnd;
		CString strFile = pSeg->m_strFileName;
		if (strFile == "*")
		{
			if (bConvDefault)
				strFile = strDefault;
		}
		else
		{
			if (!::IsFullPath(strFile))
				strFile = strSubPath + strFile;
		}

		CString strSeg;
		strSeg.Format("%ld,%s,%ld,%ld;", lLogicalTimeStart, strFile, lPysicalTimeStart, lPysicalTimeEnd);
		str += strSeg;
	}
}

/*
BOOL CVideoWnd::IsVideoModified()
{
	if (m_pTimeSeq->GetSize() != 1)
		return FALSE;

	CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(0);
	long lLogicalTimeStart = pSeg->m_lLogicalTimeStart;
	long lLogicalTimeEnd = pSeg->m_lLogicalTimeEnd;
	if (lLogicalTimeStart != 0 || lLogicalTimeEnd != m_lLength)
		return TRUE;

	return FALSE;
}
*/

void CVideoWnd::SetNewSegArray()
{
	::ReleaseObArray(m_pTimeSeq);
	::ReleaseObArray(m_pFileSeq);
	if (m_lLength > 0)
	{
		CTimeSeg* pSeg = new CTimeSeg;
		pSeg->m_lLogicalTimeStart = 0;
		pSeg->m_lLogicalTimeEnd = m_lLength;
		pSeg->m_lPhysicalTimeStart = 0;
		pSeg->m_lPhysicalTimeEnd = m_lLength;
		pSeg->m_strFileName = "*";
		m_pTimeSeq->Add(pSeg);
	}
}

void CVideoWnd::SetSegArray(CStringArray& strArrayVideo, CString strSubPath)
{
	::ReleaseObArray(m_pTimeSeq);
	::ReleaseObArray(m_pFileSeq);
	for (int i = 0; i < strArrayVideo.GetSize(); i++)
	{
		CString strSeg = strArrayVideo[i];

		long lTimeStart, lPhysicalStart, lPhysicalEnd;
		CString strPath;

		CString strTok;
		int nCount = 0;
		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;
		lTimeStart = atol(strTok);

		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;
		strPath = strTok;
		if (!::IsFullPath(strPath) && strPath != "*")
			strPath = strSubPath + strPath;

		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;
		lPhysicalStart = atol(strTok);

		if (!::GetTokStr(strSeg, strTok, nCount, ";"))
			continue;
		lPhysicalEnd = atol(strTok);

		CTimeSeg* pSeg = new CTimeSeg;
		pSeg->m_lLogicalTimeStart = lTimeStart;
		pSeg->m_lLogicalTimeEnd = lTimeStart + lPhysicalEnd - lPhysicalStart;
		pSeg->m_lPhysicalTimeStart = lPhysicalStart;
		pSeg->m_lPhysicalTimeEnd = lPhysicalEnd;
		pSeg->m_strFileName = strPath;
		m_pTimeSeq->Add(pSeg);
	}
	ReCalcFileSeq();
}

int CVideoWnd::CtrlEntered(CPoint point)
{
	for (int i = 0; i < m_pFileSeq->GetSize(); i++)
	{
		CRect rc = GetFileSeqRect(i);
		if (rc.PtInRect(point))
			return i;
	}
	return -1;
}

int CVideoWnd::DelEntered(CPoint point)
{
	for (int i = 0; i < m_pDelSeq->GetSize(); i++)
	{
		CRect rc = GetDelRect(i);
		if (rc.PtInRect(point))
			return i;
	}
	return -1;
}

void CVideoWnd::EnterCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnNormal)
	{
		pSeg->m_nStatus = constBtnOver;
		RedrawWindow(GetFileSeqRect(nCtrl));
	}

	m_nCtrlEntered = nCtrl;
}

void CVideoWnd::EnterDel(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnNormal)
	{
		pSeg->m_nStatus = constBtnOver;
		RedrawWindow(GetDelRect(nCtrl));
	}

	m_nDelEntered = nCtrl;
}

void CVideoWnd::LeaveCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	if (nCtrl >= m_pFileSeq->GetSize())
		return;

	CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnOver)
	{
		pSeg->m_nStatus = constBtnNormal;
		RedrawWindow(GetFileSeqRect(nCtrl));
	}
}

void CVideoWnd::LeaveDel(int nCtrl)
{
	if (nCtrl < 0)
		return;

	if (nCtrl >= m_pDelSeq->GetSize())
		return;

	CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnOver)
	{
		pSeg->m_nStatus = constBtnNormal;
		RedrawWindow(GetDelRect(nCtrl));
	}
}

void CVideoWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
#ifdef _BUNDLE
	CWnd::OnRButtonUp(nFlags, point);
	return;
#endif

	int nClickedIndex = CtrlEntered(point);
	int nClickedDel = DelEntered(point);
	if (nClickedIndex < 0 && nClickedDel < 0)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_VIDEO_EDIT));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	long lTimeStart, lTimeEnd;
	if (nClickedDel >= 0)
	{
		pPopup->DeleteMenu(ID_VIDEO_UNINSERT, MF_BYCOMMAND);
		CTimeSeg* pSeg = (CTimeSeg*)m_pDelSeq->GetAt(nClickedDel);
		lTimeStart = pSeg->m_lLogicalTimeStart;
	}
	else
	{
		pPopup->DeleteMenu(ID_VIDEO_UNDELETE, MF_BYCOMMAND);
		CTimeSeg* pSeg = (CTimeSeg*)m_pFileSeq->GetAt(nClickedIndex);
		lTimeStart = pSeg->m_lLogicalTimeStart;
		lTimeEnd = pSeg->m_lLogicalTimeEnd;
	}

	CPoint pts(point);
	ClientToScreen(&pts);
	UINT nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | 
									   TPM_RIGHTBUTTON | TPM_RETURNCMD, pts.x, pts.y,this);

	if (nCmd == ID_VIDEO_UNINSERT)
		GetParent()->PostMessage(WM_PERIOD_UNINSERT, lTimeStart, lTimeEnd);
	else if (nCmd == ID_VIDEO_UNDELETE)
		GetParent()->PostMessage(WM_PERIOD_UNDELETE, lTimeStart, lTimeStart);
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CVideoWnd::GetBreakPoint(CDWordArray& dwArrayBreakPoint)
{
	for (int i = 0; i < m_pTimeSeq->GetSize() - 1; i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lTime = pSeg->m_lLogicalTimeEnd;
		dwArrayBreakPoint.Add(lTime);
	}
}

BOOL CVideoWnd::GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, 
								 CString& strPathNext, long& lTimeNext)
{
	for (int i = 0; i < m_pTimeSeq->GetSize(); i++)
	{
		CTimeSeg* pSeg = (CTimeSeg*)m_pTimeSeq->GetAt(i);
		long lTimeStart = pSeg->m_lLogicalTimeStart;
		long lTimeEnd = pSeg->m_lLogicalTimeEnd;
		if (lTime > lTimeStart && lTime < lTimeEnd)
		{
			strPathPrev = pSeg->m_strFileName;
			lTimePrev = pSeg->m_lPhysicalTimeStart + lTime - lTimeStart;
			strPathNext = strPathPrev;
			lTimeNext = lTimePrev;
			return TRUE;
		}
		else if (lTime == lTimeEnd)
		{
			strPathPrev = pSeg->m_strFileName;
			lTimePrev = pSeg->m_lPhysicalTimeStart + lTime - lTimeStart;
			if (i != m_pTimeSeq->GetSize() - 1)		
			{
				CTimeSeg* pSegNext = (CTimeSeg*)m_pTimeSeq->GetAt(i + 1);
				strPathNext = pSegNext->m_strFileName;
				lTimeNext = pSegNext->m_lPhysicalTimeStart;
			}
			else
				return FALSE;

			return TRUE;
		}
	}

	return TRUE;
}

