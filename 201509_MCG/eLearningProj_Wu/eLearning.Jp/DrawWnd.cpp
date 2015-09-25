// DrawWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DrawWnd.h"
#include "MemDC.h"
#include "grdapi.h"
#include "cmkapi.h"
#include "ToolTipWnd.h"
#include "macro.h"
#include "VideoEffectWnd.h"
#include "eLearningView.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CDrawWnd::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd

CDrawWnd::CDrawWnd()
{
	m_lTimeStart = 0;
	m_lLength = 0;
	m_pSegArray = new CObArray;
	m_bTracking = FALSE;
	m_bLeaveTrack = FALSE;
	m_nCtrlEntered = -1;
	m_bCanRMenu = TRUE;
	m_bCanRDelete = TRUE;
	m_bCanREdit = FALSE;
	m_bCanRView = FALSE;
	m_crNormal = RGB(0, 0, 192);
	m_crHot = RGB(255, 255, 0);
	m_pToolTip = new CToolTipCtrl;
	m_pTooltipWnd = new CToolTipWnd;
	m_bShowParamAsTip = FALSE;
}

CDrawWnd::~CDrawWnd()
{
	::ReleaseObArray(m_pSegArray);
	delete m_pSegArray;

	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
	//{{AFX_MSG_MAP(CDrawWnd)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrawWnd message handlers

void CDrawWnd::DrawPen(CDC* pDC)
{
	CStringArray *strArrayDrawTime = MACRO_VIEW->GetDrawTime();
	RECT panelRc, drawRc;
	this->GetClientRect(&panelRc);
	for (int i = 0; i < strArrayDrawTime->GetCount(); i++)
	{
		int drawTime = atoi(strArrayDrawTime->GetAt(i));
		if (drawTime > m_lLength)
		{
			break;
		}

		CImage penImage;
		penImage.Load(::GetSysPath() + "Pencil_2.bmp");
		if (!penImage.IsNull())
		{
			int imageHeight = penImage.GetHeight();
			int imageWidth = penImage.GetWidth();
			drawRc = panelRc;
			drawRc.left = TimeToPos(drawTime);
			int nGap = (drawRc.bottom - imageHeight) / 2;
			if (nGap > 0)
			{
				drawRc.top = nGap;
				drawRc.bottom = imageHeight + drawRc.top;
				drawRc.right = imageWidth + drawRc.left;
			}
			else
				drawRc.right = drawRc.bottom * imageWidth / imageHeight + drawRc.left;
			if (drawRc.left > panelRc.left && drawRc.right < panelRc.right)
			{
				penImage.SetTransparentColor(RGB(255, 0, 255));
				penImage.TransparentBlt(pDC->m_hDC, drawRc, CRect(0, 0, imageWidth, imageHeight));
			}//.Draw(pDC->m_hDC, drawRc, Gdiplus::InterpolationModeDefault);
		}
	}
}

BOOL CDrawWnd::OnEraseBkgnd(CDC* pOrigDC) 
{
	CMemDC_MCG* pDC = new CMemDC_MCG(pOrigDC);

	CRect rcClient;
	GetClientRect(rcClient);

	CPen penBlack(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));
	CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));

	CBrush br(RGB(132, 130, 132));//::GetSysColor(COLOR_3DFACE));
	pDC->FillRect(&rcClient, &br);
	
	CPen* pOldPen = pDC->SelectObject(&penWhite);
	
	pDC->MoveTo(0, rcClient.bottom - 3);
	//pDC->LineTo(rcClient.right - 1, rcClient.bottom - 3);
	
	pDC->SelectObject(&penDkGray);
	pDC->MoveTo(0, rcClient.bottom - 1);
	//pDC->LineTo(rcClient.right - 1, rcClient.bottom - 1);

	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		long lTimeStart = pSeg->m_lTimeStart;
		long lTimeEnd = pSeg->m_lTimeEnd;
		int nXStart = TimeToPos(lTimeStart);
		int nXEnd = TimeToPos(lTimeEnd);
		if (nXStart > rcClient.right || nXEnd < rcClient.left)
			continue;

		CBrush brBlue(pSeg->m_crNormal);
		CBrush brRed(pSeg->m_crHot);

		CRect rc = rcClient;
		rc.left = nXStart + 1;
		rc.right = nXEnd + 1;
		rc.bottom = rcClient.bottom - 3;
		if (pSeg->m_nStatus == constBtnNormal)
			pDC->FillRect(&rc, &brBlue);
		else if (pSeg->m_nStatus == constBtnOver || pSeg->m_nStatus == constBtnDown)
			pDC->FillRect(&rc, &brRed);

		CRect rcPos = rc;		
		pDC->SelectObject(&penDkGray);
		if (IsTimeVisible(lTimeStart))
		{
			rcPos.left = nXStart - 1;
			rcPos.right = nXStart + 4;
			if (lTimeStart == m_lLength)
				rcPos.left--;
			pDC->Rectangle(rcPos);
		}

		if (IsTimeVisible(lTimeEnd))
		{
			rcPos.left = nXEnd - 1;
			rcPos.right = nXEnd + 4;
			if (lTimeEnd == m_lLength)
				rcPos.left--;
			pDC->Rectangle(rcPos);
		}
	}

	DrawPen(pDC);

	if (PosToTime(rcClient.right) > m_lLength)
	{
		int nPos = TimeToPos(m_lLength);
		pDC->SelectObject(&penBlack);
		pDC->MoveTo(nPos + 1, 0);
		//pDC->LineTo(nPos + 1, rcClient.bottom);

		pDC->SelectObject(&penDkGray);
		pDC->MoveTo(nPos, 0);
		//pDC->LineTo(nPos, rcClient.bottom);

		CBrush bkBrush(RGB(132, 130, 132));
		CRect rc = rcClient;
		rc.left = nPos + 2;
		pDC->SelectObject(&bkBrush);
	    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
	}

	pDC->SelectObject(pOldPen);

	delete pDC;

	return TRUE;
}

BOOL CDrawWnd::IsTimeVisible(long lTime)
{
	CRect rcClient;
	GetClientRect(rcClient);

	long lTimeStart = PosToTime(0);
	long lTimeEnd = PosToTime(rcClient.right);
	if (lTime >= lTimeStart && lTime <= lTimeEnd)
		return TRUE;

	return FALSE;
}

BOOL CDrawWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CDrawWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

int CDrawWnd::TimeToPos(long lTime)
{
	return (lTime - m_lTimeStart) * m_nPixelUnit / m_nUnit;
}

long CDrawWnd::PosToTime(int xPos)
{
	long lRet = m_lTimeStart + m_nUnit * xPos / m_nPixelUnit;
	return lRet;
}

void CDrawWnd::SetPixelUnit(int lTime)
{
	m_nPixelUnit = lTime;
}

void CDrawWnd::SetTimeStart(long lTime)
{
	m_lTimeStart = lTime;
}

void CDrawWnd::SetTimeUnit(int nTimeUnit)
{
	m_nUnit = nTimeUnit;
}

void CDrawWnd::Initialize()
{
}

BOOL CDrawWnd::AddSegment(long lTimeStart, long lTimeEnd, CString strParam, 
								 int nType, CString strTip, COLORREF crNormal, COLORREF crHot)
{
	if (TimeOverlap(lTimeStart, lTimeEnd, -1))
		return FALSE;

	CVideoEffectSeg* pSeg = new CVideoEffectSeg;
	pSeg->m_lTimeStart = lTimeStart;
	pSeg->m_lTimeEnd = lTimeEnd;
	pSeg->m_strParam = strParam;
	pSeg->m_nType = nType;
	pSeg->m_nStatus = constBtnNormal;
	pSeg->m_strTip = strTip;
	pSeg->m_crNormal = crNormal;
	pSeg->m_crHot = crHot;

	m_pSegArray->Add(pSeg);

	return TRUE;
}

void CDrawWnd::GetSegString(CString& str)
{
	str = "";
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		long lTimeStart = pSeg->m_lTimeStart;
		long lTimeEnd = pSeg->m_lTimeEnd;
		//CString strFile = pSeg->m_strParam;
		//GetFileName
		//wangwang
        CString strFile = GetFileName(pSeg->m_strParam);

		CString strSeg;
		strSeg.Format("%ld,%s,0,%ld;", lTimeStart, strFile, lTimeEnd - lTimeStart);
		str += strSeg;
	}
}

void CDrawWnd::SetSegArray(CStringArray& strArrayVideo, int nType)
{
	//if (m_pSegArray.GetSize()!=NULL)
	  ::ReleaseObArray(m_pSegArray);
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[255];
	::GetPrivateProfileString("General", "CourseName", "", szBuf, 255, strSysIniPath);
	CString VideoPath=szBuf;//::WritePrivateProfileString("General", "CourseName", strCourse, strIniPath);

	for (int i = 0; i < strArrayVideo.GetSize(); i++)
	{
		CString strSeg = strArrayVideo[i];

		long lTimeStart, lTimeLength;
		CString strPath;

		CString strTok;
		int nCount = 0;
		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;
		lTimeStart = atol(strTok);

		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;
		strPath = strTok;

		if (!::GetTokStr(strSeg, strTok, nCount, ","))
			continue;

		if (!::GetTokStr(strSeg, strTok, nCount, ";"))
			continue;
		lTimeLength = atol(strTok);

		CVideoEffectSeg* pSeg = new CVideoEffectSeg;
		pSeg->m_lTimeStart = lTimeStart;
		pSeg->m_lTimeEnd = lTimeStart + lTimeLength;
		pSeg->m_strParam = MACRO_VIDEO_DIR(VideoPath)+strPath;
		//wangwangtext
		pSeg->m_strParam = strPath;
		pSeg->m_nType = nType;
		pSeg->m_nStatus = constBtnNormal;
		pSeg->m_crHot = m_crHot;
		pSeg->m_crNormal = m_crNormal;
		m_pSegArray->Add(pSeg);
	}

	ResetTooltip();	
}

void CDrawWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	/*if (m_bTracking)
	{
		m_ptPrePos = point;
		
		CRect rcClient;
		GetClientRect(rcClient);

		int nLengthPos = TimeToPos(m_lLength);
		if (m_nDragType == 1)
		{
			int nMaxPos = min(rcClient.right, nLengthPos);

			int nWidth = m_rectTracker.Width();
			if (point.x - m_ptOffset.x + 1 <= 0)
				point.x = m_ptOffset.x - 1;
			else if (point.x - m_ptOffset.x + nWidth - 3 >= nMaxPos)
				point.x = nMaxPos - nWidth + 3 + m_ptOffset.x;

			if (m_rectTracker.left + m_ptOffset.x != point.x)
			{
				if (m_bFirstMove)
					OnInvertTracker(m_rectTracker);
				
				m_rectTracker.left = point.x - m_ptOffset.x;
				m_rectTracker.right = point.x - m_ptOffset.x + nWidth;

				OnInvertTracker(m_rectTracker);

				long lTimeStart = PosToTime(m_rectTracker.left + 1);
				long lTimeEnd = PosToTime(m_rectTracker.right - 4);
				ShowToolTip(lTimeStart, lTimeEnd);

				m_bFirstMove = TRUE;
			}
		}
		else if (m_nDragType == 0 || m_nDragType == 2)
		{
			CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(m_nClickCtrl);
			long lTimeStart = pSeg->m_lTimeStart;
			long lTimeEnd = pSeg->m_lTimeEnd;
			
			int nRightPos = TimeToPos(lTimeEnd);
			int nLeftPos = TimeToPos(lTimeStart);
			int nOrigWidth = nRightPos - nLeftPos + 4;
			int nMaxPos = min(rcClient.right, nLengthPos);
			int nMinPos = max(rcClient.left, nLeftPos);
			int nWidth = m_rectTracker.Width();

			if (m_nDragType == 0)
			{
				int nMax = min(nMaxPos, nRightPos);
				if (point.x - m_ptOffset.x + 1 <= 0)
					point.x = m_ptOffset.x - 1;
				else if (point.x - m_ptOffset.x - 3 >= nMax - 7)
					point.x = nMax + 3 + m_ptOffset.x - 7;
				if (m_rectTracker.left + m_ptOffset.x != point.x)
				{
					if (m_bFirstMove)
						OnInvertTracker(m_rectTracker);
					m_rectTracker.left = point.x - m_ptOffset.x;
					OnInvertTracker(m_rectTracker);

					m_bFirstMove = TRUE;
				}
			}
			else if (m_nDragType == 2)
			{
				if (point.x - m_ptOffset.x + 1 + nOrigWidth <= nMinPos + 8)
					point.x = nMinPos + m_ptOffset.x - 1 + 8 - nOrigWidth;
				else if (point.x - m_ptOffset.x - 3  + nOrigWidth >= nMaxPos)
					point.x = nMaxPos + 3 + m_ptOffset.x - nOrigWidth;
				if (m_rectTracker.right + m_ptOffset.x - nOrigWidth != point.x)
				{
					if (m_bFirstMove)
						OnInvertTracker(m_rectTracker);
					m_rectTracker.right = point.x - m_ptOffset.x + nOrigWidth;
					OnInvertTracker(m_rectTracker);

					m_bFirstMove = TRUE;
				}
			}
		}
	}
	else
	{
		if (m_pSegArray->GetSize() == 0)
			return;

		LeaveTrack();

		int nType;
		int nCtrl = CtrlEntered(point, &nType);
		if (nCtrl != m_nCtrlEntered)
			LeaveCtrl(m_nCtrlEntered);
		if (nCtrl < 0)
		{
	   		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			return;
		}
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nCtrl);
		if (pSeg->m_nType == 0)
			::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
		else
		{
			if (nType == 0 || nType == 2)
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			else
				::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
		}

		EnterCtrl(nCtrl);
	}*/
	
	CWnd::OnMouseMove(nFlags, point);
}

CRect CDrawWnd::GetSegRect(int nIndex) 
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcRet;
	rcRet.SetRectEmpty();

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nIndex);
	long lTimeStart = pSeg->m_lTimeStart;
	long lTimeEnd = pSeg->m_lTimeEnd;
	int nPosStart = TimeToPos(lTimeStart) - 1;
	int nPosEnd = TimeToPos(lTimeEnd) + 4;

	if (nPosStart < rcClient.right && nPosEnd > rcClient.left)
	{
		rcRet.top = rcClient.top;
		rcRet.bottom = rcClient.bottom - 3;
		rcRet.left = nPosStart;
		rcRet.right = nPosEnd;
	}

	return rcRet;
}

void CDrawWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	int nType;
	int nCtrl = CtrlEntered(point, &nType);
	if (nCtrl < 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	/*CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnDisable)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}*/

	m_nDragType = nType;
	//m_rectTracker = GetSegRect(nCtrl);
	//m_ptOffset.x = point.x - m_rectTracker.left;
	//m_ptOffset.y = point.y - m_rectTracker.top;

	SetCapture();
	SetFocus();

	//pSeg->m_nStatus = constBtnDown;

	RedrawWindow(m_rectTracker);

	m_bTracking = TRUE;
	//long lTimeStart = PosToTime(m_rectTracker.left + 1);
	//long lTimeEnd = PosToTime(m_rectTracker.right - 4);
	//ShowToolTip(lTimeStart, lTimeEnd);

	m_nClickCtrl = nCtrl;
	m_ptPrePos = point;
	m_bFirstMove = FALSE;

	//GetParent()->SendMessage(WM_PERIOD_MOVING);

	CWnd::OnLButtonDown(nFlags, point);
}

void CDrawWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	m_bTracking = FALSE;
	m_pTooltipWnd->ShowWindow(SW_HIDE);

	ReleaseCapture();

	//CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(m_nClickCtrl);

	BOOL bMoved = FALSE;
	if (m_bFirstMove)
	{
		/*OnInvertTracker(m_rectTracker);

		m_bFirstMove = FALSE;
		bMoved = TRUE;

		long lTimeStart = PosToTime(m_rectTracker.left + 1);
		long lTimeEnd;
		if (m_nDragType == 1)
			lTimeEnd = lTimeStart + pSeg->m_lTimeEnd - pSeg->m_lTimeStart;
		else if (m_nDragType == 0 || m_nDragType == 2)
			lTimeEnd = PosToTime(m_rectTracker.right - 3);

		if (!TimeOverlap(lTimeStart, lTimeEnd, m_nClickCtrl))
		{
			GetParent()->SendMessage(WM_PERIOD_TOBE_MOVED, m_nClickCtrl, GetDlgCtrlID());

			pSeg->m_lTimeStart = lTimeStart;
			pSeg->m_lTimeEnd = lTimeEnd;

			GetParent()->SendMessage(WM_PERIOD_MOVED, m_nClickCtrl, GetDlgCtrlID());

			ResetTooltip();
		}
		else
		{
			W_SHOW_ERROR(IDS_OVERLAP_PERIOD, this);
		}*/
	}
	else
	{
		//GetParent()->PostMessage(WM_CLICK_VIDEOEFFECT, pSeg->m_lTimeStart, pSeg->m_lTimeEnd);
	}

	int nCurCtrl = CtrlEntered(point);
	/*BOOL bIn = (nCurCtrl == m_nClickCtrl);
	if (bIn)
		pSeg->m_nStatus = constBtnOver;
	else if (!bIn)
		pSeg->m_nStatus = constBtnNormal;*/
	if (nCurCtrl < 0)
	{
		CStringArray *strArrayDrawTime = MACRO_VIEW->GetDrawTime();
		CString strNewTime;
		strNewTime.Format("%ld", PosToTime(point.x));
		strArrayDrawTime->RemoveAt(m_nClickCtrl);
		strArrayDrawTime->InsertAt(m_nClickCtrl, strNewTime);
	}
	else
	{
		W_SHOW_ERROR(IDS_OVERLAP_PERIOD, this);
	}

	RedrawWindow();

	CWnd::OnLButtonUp(nFlags, point);
}

void CDrawWnd::LeaveTrack() 
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

LRESULT CDrawWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		if (pSeg->m_nStatus == constBtnOver) //Button
		{
			pSeg->m_nStatus = constBtnNormal;
			RedrawWindow(GetSegRect(i));
		}
	}
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return 0;
}

void CDrawWnd::EnterCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnNormal)
	{
		pSeg->m_nStatus = constBtnOver;
		RedrawWindow(GetSegRect(nCtrl));
	}

	m_nCtrlEntered = nCtrl;
}

void CDrawWnd::LeaveCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	if (nCtrl >= m_pSegArray->GetSize())
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nCtrl);

	if (pSeg->m_nStatus == constBtnOver)
	{
		pSeg->m_nStatus = constBtnNormal;
		RedrawWindow(GetSegRect(nCtrl));
	}
}

int CDrawWnd::CtrlEntered(CPoint point, int* pnType)
{

	CStringArray *strArrayDrawTime = MACRO_VIEW->GetDrawTime();
	BOOL findStart = FALSE;
	for (int i = 0; i < strArrayDrawTime->GetCount(); i++)
	{
		int drawTime = atoi(strArrayDrawTime->GetAt(i));
		if (point.x < TimeToPos(drawTime))
			continue;
		else if (point.x - TimeToPos(drawTime) < 10)
		{
			return i;
		}
	}
	return -1;

}

void CDrawWnd::OnInvertTracker(const CRect& rect)
{
	CRect rc = rect;
	ClientToScreen(rc);
	::DrawResizeBar(rc);
}

void CDrawWnd::DeletePeriodByIndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_pSegArray->GetSize())
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nIndex);
	m_pSegArray->RemoveAt(nIndex);
	//::DeleteFile(pSeg->m_strParam);
	delete pSeg;

	ResetTooltip();	

	RedrawWindow();
}

BOOL CDrawWnd::TimeOverlap(long lTimeStart, long lTimeEnd, int nIndexNotCare)
{
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		if (i == nIndexNotCare)
			continue;

		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		if (pSeg->m_lTimeStart < lTimeEnd && pSeg->m_lTimeEnd > lTimeStart)
			return TRUE;
	}

	return FALSE;
}

void CDrawWnd::SetSegTip(int nIndex, CString strTip) 
{
	if (nIndex < 0 || nIndex >= m_pSegArray->GetSize())
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nIndex);
	pSeg->m_strTip = strTip;
}

void CDrawWnd::SetSegColor(int nIndex, COLORREF crNormal, COLORREF crHot) 
{
	if (nIndex < 0 || nIndex >= m_pSegArray->GetSize())
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nIndex);
	pSeg->m_crHot = crHot;
	pSeg->m_crNormal = crNormal;
}

void CDrawWnd::GetSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
								 CString& strPath, int& nType) 
{
	if (nIndex < 0 || nIndex >= m_pSegArray->GetSize())
		return;

	CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nIndex);
	lTimeStart = pSeg->m_lTimeStart;
	lTimeEnd = pSeg->m_lTimeEnd;
	strPath = pSeg->m_strParam;
	nType = pSeg->m_nType;
}

void CDrawWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
#ifdef _BUNDLE
	CWnd::OnRButtonUp(nFlags, point);
	return;
#endif

	int nCtrl = CtrlEntered(point);
	if (nCtrl < 0)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	m_nCtrlEntered = -1;

	if (!m_bCanRMenu)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	//CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(nCtrl);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_CP));
	CMenu* pPopup = menu.GetSubMenu(0);
	
	/*if (!m_bCanRDelete)
		pPopup->DeleteMenu(ID_DEL_OBJ, MF_BYCOMMAND);
	if (!m_bCanRView)
		pPopup->DeleteMenu(ID_VIEW_OBJ, MF_BYCOMMAND);
	if (!m_bCanREdit)
		pPopup->DeleteMenu(ID_EDIT_CONTENT, MF_BYCOMMAND);*/
	pPopup->DeleteMenu(ID_DEMO_CAPTURE, MF_BYCOMMAND);
	pPopup->DeleteMenu(ID_IMPORT_DEMO_FILE, MF_BYCOMMAND);
	pPopup->DeleteMenu(ID_DESIGN, MF_BYCOMMAND);
	pPopup->DeleteMenu(ID_VIEW_OBJ, MF_BYCOMMAND);

	ASSERT(pPopup != NULL);
	CPoint pts(point);
	ClientToScreen(&pts);
	UINT nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RIGHTBUTTON | TPM_RETURNCMD, pts.x, pts.y,this);

	/*if (nCmd == ID_DEL_OBJ)
		GetParent()->PostMessage(WM_PERIOD_DELETE, nCtrl, GetDlgCtrlID());
	else if (nCmd == ID_VIEW_OBJ)
		GetParent()->PostMessage(WM_PERIOD_VIEW, nCtrl, GetDlgCtrlID());
	else if (nCmd == ID_EDIT_CONTENT)
		GetParent()->PostMessage(WM_PERIOD_EDIT, nCtrl, GetDlgCtrlID());*/
	if (nCmd == ID_DELETE_CP)
		MACRO_VIEW->DeleteDrawPoint(nCtrl);
	else if (nCmd == ID_DELETE_ALL_CP)
	{
		CStringArray *strArrayDrawTime = MACRO_VIEW->GetDrawTime();
		while (0 < strArrayDrawTime->GetCount())
			MACRO_VIEW->DeleteDrawPoint(0);
	}
	else if(nCmd == ID_EDIT_CP)
	{
		MACRO_VIEW->EditDrawMark();
	}

	ResetTooltip();	

	RedrawWindow();
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CDrawWnd::DeletePeriod(long lTimeStart, long lTimeEnd)
{

	CStringArray *strArrayDrawTime = MACRO_VIEW->GetDrawTime();
	for (int i = 0; i < strArrayDrawTime->GetSize(); i++)
	{
		long nDrawTime = atoi(strArrayDrawTime->GetAt(i));
		if(nDrawTime < lTimeStart)
			continue;
		else if (nDrawTime >= lTimeStart && nDrawTime <= lTimeEnd)
		{
			strArrayDrawTime->RemoveAt(i);
			i--;
		}
		else if (nDrawTime > lTimeEnd)
		{
			CString strNewTime;
			strNewTime.Format("%ld", nDrawTime - (lTimeEnd - lTimeStart));
			strArrayDrawTime->RemoveAt(i);
			strArrayDrawTime->InsertAt(i, strNewTime);
		}
	}

	ResetTooltip();	
}

BOOL CDrawWnd::IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd)
{
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		long lSegTimeStart = pSeg->m_lTimeStart;
		long lSegTimeEnd = pSeg->m_lTimeEnd;
		if (lSegTimeStart < lTimeEnd && lSegTimeEnd > lTimeStart)
			return TRUE;	
	}

	return FALSE;
}

void CDrawWnd::InsertPeriod(long lTimeInsert, long lDuration)
{
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		long lTimeStart = pSeg->m_lTimeStart;
		long lTimeEnd = pSeg->m_lTimeEnd;
		if (lTimeStart >= lTimeInsert)
		{
			pSeg->m_lTimeStart = lTimeStart + lDuration;
			pSeg->m_lTimeEnd = lTimeEnd + lDuration;
		}
	}

	m_lLength += lDuration;
	ResetTooltip();	
}

CString CDrawWnd::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

void CDrawWnd::AddToolTip()
{
	if (m_pToolTip )
	  delete m_pToolTip;
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	CString strTitle;
	CString strPreMark;
	for (int i = 0; i < m_pSegArray->GetSize(); i++)
	{
		CVideoEffectSeg* pSeg = (CVideoEffectSeg*)m_pSegArray->GetAt(i);
		long lSegTimeStart = pSeg->m_lTimeStart;
		long lSegTimeEnd = pSeg->m_lTimeEnd;
		CString strTip = pSeg->m_strTip;
		CRect rcPos = GetSegRect(i);
		if (!rcPos.IsRectEmpty())
		{
			CString strTime1 = GetTimeStr(lSegTimeStart);
			CString strTime2 = GetTimeStr(lSegTimeEnd);
			if (m_bShowParamAsTip)
				strTip = strTip + " <" + strTime1 + " - " + strTime2 + ">";
			else
				strTip = "<" + strTime1 + " - " + strTime2 + ">";
			m_pToolTip->AddTool(this, strTip, rcPos, 1);
		}
	}
	m_pToolTip->Activate(TRUE);
}


LRESULT CDrawWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

				if (m_pToolTip)
					m_pToolTip->RelayEvent(&msg);
			}
    }

	return CWnd::WindowProc(message, wParam, lParam);
}

int CDrawWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create(this);
	
	m_pTooltipWnd->Create(this);
	m_pTooltipWnd->SetDrawFont(MACRO_SYS_FONT);

	return 0;
}

void CDrawWnd::ResetTooltip()
{
	AddToolTip();
}

void CDrawWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ResetTooltip();	
}

void CDrawWnd::ShowToolTip(long lTimeStart, long lTimeEnd)
{
	CPoint pt;
	GetCursorPos(&pt);
	int npx = pt.x + 16;
	CRect rcClient;
	GetClientRect(rcClient);
	pt.y = rcClient.bottom + 3; 
	ClientToScreen(&pt);
	pt.x = npx;

	CString strTime1 = GetTimeStr(lTimeStart);
	CString strTime2 = GetTimeStr(lTimeEnd);
	CString strTip = "<" + strTime1 + " - " + strTime2 + ">";
	m_pTooltipWnd->ShowText(pt, strTip);
}

void CDrawWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pTooltipWnd->DestroyWindow();
	delete m_pTooltipWnd;
}
