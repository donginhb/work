// EffectManageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "EffectManageWnd.h"
#include "MemDC.h"
#include "grdapi.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CEffectManageWnd::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CEffectManageWnd

CEffectManageWnd::CEffectManageWnd()
{
	m_lTimeStart = 0;
	m_lLength = 0;
	m_pObBreakPoint = new CObArray;
	m_nCtrlEntered = -1;
	m_bLeaveTrack = FALSE;
	m_bCanRMenu = TRUE;
	m_pToolTip = new CToolTipCtrl;
}

CEffectManageWnd::~CEffectManageWnd()
{
	::ReleaseObArray(m_pObBreakPoint);
	delete m_pObBreakPoint;

	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CEffectManageWnd, CWnd)
	//{{AFX_MSG_MAP(CEffectManageWnd)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEffectManageWnd message handlers

BOOL CEffectManageWnd::OnEraseBkgnd(CDC* pOrigDC) 
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

	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSegTmp = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		DrawMarkAtTime(pDC, i);
	}

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

BOOL CEffectManageWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CEffectManageWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

int CEffectManageWnd::TimeToPos(long lTime)
{
	return (lTime - m_lTimeStart) * m_nPixelUnit / m_nUnit;
}

long CEffectManageWnd::PosToTime(int xPos)
{
	long lRet = m_lTimeStart + m_nUnit * xPos / m_nPixelUnit;
	return lRet;
}

void CEffectManageWnd::SetPixelUnit(int lTime)
{
	m_nPixelUnit = lTime;
}

void CEffectManageWnd::SetTimeStart(long lTime)
{
	m_lTimeStart = lTime;
}

void CEffectManageWnd::SetTimeUnit(int nTimeUnit)
{
	m_nUnit = nTimeUnit;
}

void CEffectManageWnd::Initialize()
{
}

BOOL CEffectManageWnd::IsTimeVisible(long lTime)
{
	CRect rcClient;
	GetClientRect(rcClient);

	long lTimeStart = PosToTime(0);
	long lTimeEnd = PosToTime(rcClient.right);
	if (lTime >= lTimeStart && lTime <= lTimeEnd)
		return TRUE;

	return FALSE;
}

void CEffectManageWnd::DrawMarkAtTime(CDC* pDC, int nIndex)
{
	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nIndex);
	long lTime = pSeg->m_lTime;
	if (IsTimeVisible(lTime))
	{
		long nX = TimeToPos(lTime);

		CRect rcPos;
		GetClientRect(rcPos);
	
		rcPos.left = nX - 1;
		if (lTime == m_lLength && pSeg->m_nType == 0)
			rcPos.left--;
		rcPos.right = nX + 4;
		rcPos.bottom -= 3;
		if (lTime == m_lLength)
			rcPos.left--;

		CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
		CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
		CBrush brWhite(RGB(255, 255, 255));
		CBrush brYellow(RGB(255, 255, 0));
		CPen* pOldPen = (CPen*)pDC->SelectObject(&penDkGray);

		CBrush* pOldBrush;
		if (pSeg->m_nStatus == constBtnOver)
			pOldBrush = (CBrush*)pDC->SelectObject(&brYellow);
		else
			pOldBrush = (CBrush*)pDC->SelectObject(&brWhite);

		pDC->Rectangle(rcPos);

		int nType = pSeg->m_nType;
		CPoint pt;
		if (nType == 1 || nType == 3)
		{
			pDC->SelectObject(penBlue);
			pt.x = rcPos.right + 1;
			pt.y = (rcPos.Height() - 5) / 2;
			pDC->MoveTo(pt.x, pt.y);
			pDC->LineTo(pt.x, pt.y + 5);
			pDC->MoveTo(pt.x + 1, pt.y + 1);
			pDC->LineTo(pt.x + 1, pt.y + 4);
			pDC->MoveTo(pt.x + 2, pt.y + 2);
			pDC->LineTo(pt.x + 2, pt.y + 3);
		}
		if (nType == 2 || nType == 3)
		{
			pDC->SelectObject(penBlue);
			pt.x = rcPos.left - 3;
			pt.y = (rcPos.Height() - 5) / 2;
			pDC->MoveTo(pt.x, pt.y);
			pDC->LineTo(pt.x, pt.y + 5);
			pDC->MoveTo(pt.x + 1, pt.y + 1);
			pDC->LineTo(pt.x + 1, pt.y + 4);
			pDC->MoveTo(pt.x + 2, pt.y + 2);
			pDC->LineTo(pt.x + 2, pt.y + 3);
		}
		if (nType == 4 && pSeg->m_nStatus != constBtnOver)
		{
			CPen penLeft(PS_SOLID, 1, RGB(255, 255, 192));
			CPen penMiddle(PS_SOLID, 1, RGB(255, 207, 0));
			CPen penRight(PS_SOLID, 1, RGB(255, 158, 0));
			pDC->SelectObject(penLeft);
			pDC->MoveTo(rcPos.left + 1, rcPos.top + 1);
			pDC->LineTo(rcPos.left + 1, rcPos.bottom - 1);

			pDC->SelectObject(penMiddle);
			pDC->MoveTo(rcPos.left + 2, rcPos.top + 1);
			pDC->LineTo(rcPos.left + 2, rcPos.bottom - 1);
			
			pDC->SelectObject(penRight);
			pDC->MoveTo(rcPos.left + 3, rcPos.top + 1);
			pDC->LineTo(rcPos.left + 3, rcPos.bottom - 1);
		}
		
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

void CEffectManageWnd::SetBreakPoint(CDWordArray& dwArrayBreakPoint, long lTimeStart, long lTimeEnd)
{
	int i = 0;
	for (i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		if (pSeg->m_nType == 0 || pSeg->m_lTime >= lTimeStart && pSeg->m_lTime <= lTimeEnd)
		{
			m_pObBreakPoint->RemoveAt(i);	
			delete pSeg;
			i--;
		}
	}

	for (i = 0; i < dwArrayBreakPoint.GetSize(); i++)
		AddSegment(dwArrayBreakPoint[i], 0, 0);

	AddSegment(0, 0, 0);
	AddSegment(m_lLength, 0, 0);

	ResetTooltip();	
}

BOOL CEffectManageWnd::AddSegment(long lTime, long lDuration, int nType, CString strParam, CString strTip)
{
	int nIndex = -1;
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSegTmp = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		if (pSegTmp->m_lTime == lTime)
		{
			if (nType != 0 || pSegTmp->m_nType == 0)
			{
				pSegTmp->m_lDuration = lDuration;
				pSegTmp->m_strParam = strParam;
				pSegTmp->m_nType = nType;
				pSegTmp->m_nStatus = constBtnNormal;
				pSegTmp->m_strTip = strTip;
			}
			return TRUE;
		}
		else if (pSegTmp->m_lTime > lTime)
		{
			nIndex = i;
			break;
		}
	}

	CEffectPoint* pSeg = new CEffectPoint;
	pSeg->m_lTime = lTime;
	pSeg->m_lDuration = lDuration;
	pSeg->m_strParam = strParam;
	pSeg->m_nType = nType;
	pSeg->m_nStatus = constBtnNormal;
	pSeg->m_strTip = strTip;

	if (nIndex < 0)
		m_pObBreakPoint->Add(pSeg);
	else
		m_pObBreakPoint->InsertAt(nIndex, pSeg);

	return TRUE;
}

void CEffectManageWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pObBreakPoint->GetSize() == 0)
		return;

	LeaveTrack();

	int nCtrl = CtrlEntered(point);
	if (nCtrl != m_nCtrlEntered)
		LeaveCtrl(m_nCtrlEntered);
	if (nCtrl < 0)
	{
	   	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return;
	}
	::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));

	EnterCtrl(nCtrl);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CEffectManageWnd::LeaveTrack() 
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

CRect CEffectManageWnd::GetSegRect(int nIndex) 
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcRet;
	rcRet.SetRectEmpty();

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nIndex);
	long lTime = pSeg->m_lTime;
	int nPosStart = TimeToPos(lTime) - 1;
	int nPosEnd = TimeToPos(lTime) + 4;

	if (nPosStart < rcClient.right && nPosEnd > rcClient.left)
	{
		rcRet.top = rcClient.top;
		rcRet.bottom = rcClient.bottom - 3;
		rcRet.left = nPosStart;
		rcRet.right = nPosEnd;
		if (pSeg->m_nType == 1 || pSeg->m_nType == 3)
			rcRet.right += 6;
		if (pSeg->m_nType == 2 || pSeg->m_nType == 3)
			rcRet.left -= 5;
	}

	return rcRet;
}

LRESULT CEffectManageWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		if (pSeg->m_nStatus == constBtnOver) //Button
		{
			pSeg->m_nStatus = constBtnNormal;
			RedrawWindow(GetSegRect(i));
		}
	}
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return 0;
}

void CEffectManageWnd::EnterCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nCtrl);
	if (pSeg->m_nStatus == constBtnNormal)
	{
		pSeg->m_nStatus = constBtnOver;
		RedrawWindow(GetSegRect(nCtrl));
	}

	m_nCtrlEntered = nCtrl;
}

void CEffectManageWnd::LeaveCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	if (nCtrl >= m_pObBreakPoint->GetSize())
		return;

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nCtrl);

	if (pSeg->m_nStatus == constBtnOver)
	{
		pSeg->m_nStatus = constBtnNormal;
		RedrawWindow(GetSegRect(nCtrl));
	}
}

int CEffectManageWnd::CtrlEntered(CPoint point)
{
	for (int i = m_pObBreakPoint->GetSize() - 1; i >= 0; i--)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		CRect rcSeg = GetSegRect(i);
		if (rcSeg.PtInRect(point))
			return i;
	}
	return -1;
}

void CEffectManageWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_nClickCtrl < 0)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(m_nClickCtrl);
	int nCurCtrl = CtrlEntered(point);
	BOOL bIn = (nCurCtrl == m_nClickCtrl);
	if (bIn)
		pSeg->m_nStatus = constBtnOver;
	else if (!bIn)
		pSeg->m_nStatus = constBtnNormal;

	RedrawWindow();

	if (bIn)
		GetParent()->PostMessage(WM_EFFECT_PRESSED, m_nClickCtrl, (LPARAM)pSeg->m_lTime);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CEffectManageWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nCtrl = CtrlEntered(point);
	if (nCtrl < 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		m_nClickCtrl = -1;
		return;
	}

	m_nClickCtrl = nCtrl;
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CEffectManageWnd::SetLength(long lTimeLength)
{ 
	m_lLength = lTimeLength;
}

void CEffectManageWnd::OnRButtonUp(UINT nFlags, CPoint point) 
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

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nCtrl);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_EFFECT));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (nCtrl == 0)
	{
		pPopup->DeleteMenu(ID_FADE_OUT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_TRANSIT_EFFECT, MF_BYCOMMAND);
		if (pSeg->m_nType == 1)
			pPopup->CheckMenuItem(ID_FADE_IN, MF_BYCOMMAND | MF_CHECKED);
	}
	else if (nCtrl == m_pObBreakPoint->GetSize() - 1)
	{
		pPopup->DeleteMenu(ID_FADE_IN, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_TRANSIT_EFFECT, MF_BYCOMMAND);
		if (pSeg->m_nType == 2)
			pPopup->CheckMenuItem(ID_FADE_OUT, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		pPopup->DeleteMenu(ID_FADE_IN, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_FADE_OUT, MF_BYCOMMAND);
	}

	CPoint pts(point);
	ClientToScreen(&pts);
	UINT nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RIGHTBUTTON | TPM_RETURNCMD, pts.x, pts.y,this);

	CRect rc = GetSegRect(nCtrl);
	CRect rc2, rc3;
	if (nCmd == ID_FADE_IN)
	{
		GetParent()->SendMessage(WM_FADE_IN_EFFECT, pSeg->m_lTime, nCtrl);
		pSeg->m_nType = 1 - pSeg->m_nType;
		rc2 = GetSegRect(nCtrl);
		rc3.UnionRect(rc, rc2);
		RedrawWindow(rc3);
	}
	else if (nCmd == ID_FADE_OUT)
	{
		GetParent()->SendMessage(WM_FADE_OUT_EFFECT, pSeg->m_lTime, nCtrl);
		pSeg->m_nType = 2 - pSeg->m_nType;
		rc2 = GetSegRect(nCtrl);
		rc3.UnionRect(rc, rc2);
		RedrawWindow(rc3);
	}
	else if (nCmd == ID_TRANSIT_EFFECT)
		GetParent()->PostMessage(WM_TRANSIT_EFFECT, pSeg->m_lTime, nCtrl);
	else if (nCmd == ID_DEL_OBJ)
	{
		GetParent()->PostMessage(WM_DELETE_EFFECT, pSeg->m_lTime, nCtrl);
	}

	CWnd::OnRButtonUp(nFlags, point);
}

void CEffectManageWnd::DeletePeriod(long lTimeStart, long lTimeEnd)
{
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		long lSegTime = pSeg->m_lTime;
		if (lSegTime < lTimeEnd && lSegTime > lTimeStart)
		{
			m_pObBreakPoint->RemoveAt(i);
			i--;
			delete pSeg;
		}
		else if (lSegTime >= lTimeEnd)
		{
			long lDuration = lTimeEnd - lTimeStart;
			pSeg->m_lTime -= lDuration;
		}
	}
	ResetTooltip();	
}

BOOL CEffectManageWnd::IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd)
{
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		long lSegTime = pSeg->m_lTime;
		if (lSegTime < lTimeEnd && lSegTime > lTimeStart)
			return TRUE;	
	}

	return FALSE;
}

void CEffectManageWnd::InsertPeriod(long lTimeInsert, long lDuration)
{
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		long lTime = pSeg->m_lTime;
		if (lTime >= lTimeInsert)
			pSeg->m_lTime = lTime + lDuration;
	}

	m_lLength += lDuration;
	ResetTooltip();	
}

void CEffectManageWnd::ResetTooltip()
{
	AddToolTip();
}

void CEffectManageWnd::AddToolTip()
{
	delete m_pToolTip;
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	CString strTitle;
	CString strPreMark;
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		long lSegTime = pSeg->m_lTime;
		CRect rcPos = GetSegRect(i);
		if (!rcPos.IsRectEmpty())
		{
			CString strTime = GetTimeStr(lSegTime);
			CString strTip = "<" + strTime + ">";
			m_pToolTip->AddTool(this, strTip, rcPos, 1);
		}
	}
	m_pToolTip->Activate(TRUE);
}


LRESULT CEffectManageWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

int CEffectManageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create(this);

	return 0;
}

CString CEffectManageWnd::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

void CEffectManageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ResetTooltip();	
}

void CEffectManageWnd::GetStartEndTime(long lTime, long lDuration, int nType, 
									   long& lTimeStart, long& lTimeEnd)
{
	lDuration /= 2;
	if (nType == 1)
	{
		lTimeStart = lTime;
		lTimeEnd = lTime + lDuration;
	}
	else if (nType == 2)
	{
		lTimeStart = lTime - lDuration;
		lTimeEnd = lTime;
	}
	else
	{
		lTimeStart = lTime - lDuration;
		lTimeEnd = lTime + lDuration;
	}
}

BOOL CEffectManageWnd::TimeOverlap(long lTime, long lDuration, int nIndexNotCare)
{
	long lTimeStart, lTimeEnd;
	GetStartEndTime(lTime, lDuration, 4, lTimeStart, lTimeEnd);
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		if (i == nIndexNotCare)
			continue;

		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);
		if (pSeg->m_nType == 0)
			continue;

		long lTimeRangeStart, lTimeRangeEnd;
		GetStartEndTime(pSeg->m_lTime, pSeg->m_lDuration, pSeg->m_nType, lTimeRangeStart, lTimeRangeEnd);

		if (lTimeRangeStart < lTimeEnd && lTimeRangeEnd > lTimeStart)
			return TRUE;
	}

	return FALSE;
}

void CEffectManageWnd::SetSegArray(CStringArray& strArrayVideo)
{
	::ReleaseObArray(m_pObBreakPoint);

	GetParent()->SendMessage(WM_SET_BREAKPOINT);

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

		int nType;
		long lTime;
		if (lTimeStart == 0)
		{
			nType = 1;
			lTime = 0;
		}
		else if (lTimeStart + lTimeLength >= m_lLength)
		{
			nType = 2;
			lTime = m_lLength;
		}
		else
		{
			nType = 4;
			lTime = lTimeStart + lTimeLength / 2;
		}
		
		AddSegment(lTime, lTimeLength, nType, strPath);
	}

	ResetTooltip();	
}

void CEffectManageWnd::GetSegString(CString& str)
{
	str = "";
	for (int i = 0; i < m_pObBreakPoint->GetSize(); i++)
	{
		CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(i);

		if (pSeg->m_nType == 0)
			continue;

		long lTimeStart, lTimeEnd;
		GetStartEndTime(pSeg->m_lTime, pSeg->m_lDuration, pSeg->m_nType, lTimeStart, lTimeEnd);
		CString strFile = pSeg->m_strParam;

		CString strSeg;
		strSeg.Format("%ld,%s,0,%ld;", lTimeStart, strFile, lTimeEnd - lTimeStart);
		str += strSeg;
	}
}

void CEffectManageWnd::DeletePeriodByIndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_pObBreakPoint->GetSize())
		return;

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nIndex);
	if (nIndex == 0 || nIndex == m_pObBreakPoint->GetSize() - 1)
		pSeg->m_nType = 0;
	else
	{
		m_pObBreakPoint->RemoveAt(nIndex);
		delete pSeg;
	}

	ResetTooltip();	

	RedrawWindow();
}

void CEffectManageWnd::SetEmptyByndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_pObBreakPoint->GetSize())
		return;

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nIndex);
	pSeg->m_nType = 0;

	ResetTooltip();	

	RedrawWindow();
}

void CEffectManageWnd::GetSegInfo(int nIndex, long& lTime, long& lDuration, 
								  CString& strPath, int& nType) 
{
	if (nIndex < 0 || nIndex >= m_pObBreakPoint->GetSize())
		return;

	CEffectPoint* pSeg = (CEffectPoint*)m_pObBreakPoint->GetAt(nIndex);
	lTime = pSeg->m_lTime;
	lDuration = pSeg->m_lDuration;
	strPath = pSeg->m_strParam;
	nType = pSeg->m_nType;
}
