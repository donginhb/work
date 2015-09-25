// TimeLineCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "TimeLineCtrl.h"
#include "MemDC.h"
#include "cmkapi.h"
#include "macro.h"
#include "PointerWnd.h"
#include "BmpBtn.h"
#include "ToolTipWnd.h"
#include "ColorWnd.h"
#include "VideoWnd.h"
#include "AudioWnd.h"
#include "DrawWnd.h"
//#include "ControlPanel.h"
#include "KeyFramePanel.h"
#include "VideoEffectWnd.h"
#include "EffectManageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define cRuleHeight 15
#define cVideoHeight 18
#define cKeyFrameHeight 31
#define cVideoEffectHeight 16

CTimeLineCtrl::CTimeLineCtrl()
{
	m_pKeyFrame = new CKeyFramePanel;
	m_pLocateLine = new CColorWnd;
	m_pToolTip = new CToolTipWnd;
	m_pVideoWnd = new CVideoWnd;
	m_pAudioWnd = new CAudioWnd;
	m_pDrawWnd = new CDrawWnd;
	
	m_pAudioReplaceWnd = new CVideoEffectWnd;
	m_pAudioMixWnd = new CVideoEffectWnd;
	m_pVideoEffectWnd = new CEffectManageWnd;
	m_pVideoTextWnd = new CVideoEffectWnd;
	m_pAudioReplaceWnd->m_bCanRView = TRUE;
	m_pAudioMixWnd->m_bCanRView = TRUE;
	m_pVideoTextWnd->m_bCanRView = FALSE;
	m_pVideoTextWnd->m_bCanREdit = TRUE;
	m_pVideoTextWnd->m_bShowParamAsTip = TRUE;

	m_pPointer = new CPointerWnd(IDB_POINTER);
	m_pTmpLocateLine = new CColorWnd;

	SetTimeUnit(100);
	SetPixelUnit(1);
	m_lTimeStart = 0;
	m_csPointer = ::GetBmpDim(IDB_POINTER);	
	m_csPointer.cx /= 4;
	m_lTimePointer = 0;
	m_nID = 0;
	m_nID2 = 0;
}

CTimeLineCtrl::~CTimeLineCtrl()
{
}


BEGIN_MESSAGE_MAP(CTimeLineCtrl, CWnd)
	//{{AFX_MSG_MAP(CTimeLineCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_MESSAGE(WM_POINTER_MOVE, OnPointerMove)
	ON_MESSAGE(WM_POINTER_UP, OnPointerUp)
	ON_MESSAGE(WM_POINTER_DOWN, OnPointerDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_POINTER_MOUSE_MOVE, OnPointerMouseMove)
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_COLORWND_MOUSE_MOVE, OnColorWndMouseMove)
	ON_MESSAGE(WM_EFFECT_PRESSED, OnEffectPressed)
	ON_MESSAGE(WM_KEY_FRAME_PRESSED, OnKeyFramePressed)
	ON_MESSAGE(WM_KEY_FRAME_MOVED, OnKeyFrameMoved)
	ON_MESSAGE(WM_KEY_FRAME_TOBE_MOVED, OnKeyFrameTobeMoved)
	ON_MESSAGE(WM_REGEN_TAG, OnRegenTag)
	ON_MESSAGE(WM_KEY_FRAME_PRESSING, OnKeyFramePressing)
	ON_MESSAGE(WM_GET_POINTER_TIME, OnGetPointerTime)
	ON_MESSAGE(WM_PERIOD_CLICKED, OnPeriodClicked)
	ON_MESSAGE(WM_CP_ACTION, OnCPAction)
	ON_MESSAGE(WM_PERIOD_MOVED, OnPeriodMoved)
	ON_MESSAGE(WM_PERIOD_TOBE_MOVED, OnPeriodTobeMoved)
	ON_MESSAGE(WM_PERIOD_DELETE, OnPeriodDelete)
	ON_MESSAGE(WM_PERIOD_VIEW, OnPeriodView)
	ON_MESSAGE(WM_PERIOD_EDIT, OnPeriodEdit)
	ON_MESSAGE(WM_CLICK_VIDEOEFFECT, OnClickVideoEffect)
	ON_MESSAGE(WM_PERIOD_MOVING, OnPeriodMoving)
	ON_MESSAGE(WM_PERIOD_UNINSERT, OnPeriodUnInsert)
	ON_MESSAGE(WM_PERIOD_UNDELETE, OnPeriodUnDelete)
	ON_MESSAGE(WM_FADE_IN_EFFECT, OnFadeInEffect)
	ON_MESSAGE(WM_FADE_OUT_EFFECT, OnFadeOutEffect)
	ON_MESSAGE(WM_TRANSIT_EFFECT, OnTransitEffect)
	ON_MESSAGE(WM_DELETE_EFFECT, OnDeleteEffect)
	ON_MESSAGE(WM_SET_BREAKPOINT, OnSetBreakPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTimeLineCtrl message handlers

BOOL CTimeLineCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	//if (m_strClass == "")
	//	m_strClass = AfxRegisterWndClass(CS_DBLCLKS, 0, NULL);
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CTimeLineCtrl::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTimeLineCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(RGB(132, 130, 132));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	brBg.CreateSolidBrush(RGB(255, 255, 255));
	rc.bottom = rc.top + cRuleHeight;
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();

	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&penBlack);
	dc.MoveTo(0, cRuleHeight);
	dc.LineTo(rc.right, cRuleHeight);

	//CPen penDkGray1(PS_SOLID, 1, RGB(144, 144, 144));
	//dc.SelectObject(&penDkGray1);
	//dc.MoveTo(0, cRuleHeight + 1);
	//dc.LineTo(rc.right, cRuleHeight + 1);

	dc.SelectObject(&penBlack);

	CFont* pOldFont = dc.SelectObject(MACRO_SYS_FONT);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));

	long lTime = m_lTimeStart;
	long lLongUnit = m_nUnit * 100;
	long lSmallUnit = m_nUnit * 10;
	for (int i = -60; i < rc.right + 60; i++)
	{
		if (i % m_nPixelUnit == 0)
		{
			lTime = m_lTimeStart + i * m_nUnit / m_nPixelUnit;
			if (lTime % lLongUnit == 0)
			{
				dc.MoveTo(i, cRuleHeight - 1);
				dc.LineTo(i, cRuleHeight - 4);

				CString str;
				CRect rc;
				rc.top = 0;
				rc.bottom = cRuleHeight - 2;
				str = ::ConvertTimeToStr(lTime);
				rc.left = i - 53;
				rc.right = i + 50;
				dc.DrawText(str, str.GetLength(), rc, DT_SINGLELINE | DT_CENTER | DT_NOCLIP | DT_VCENTER);
			}
			else if (lTime % lSmallUnit == 0)
				dc.SetPixel(i, cRuleHeight - 1, RGB(0, 0, 0));
		}
	}

	
	CRect rcClient;
	GetClientRect(rcClient);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));
	CPen penDkGray(PS_SOLID, 1, RGB(112, 112, 112));
	CPen penGray(PS_SOLID, 1, RGB(192, 192, 192));
	CPen penLtGray(PS_SOLID, 1, RGB(224, 224, 224));
	
	//dc.SelectObject(&penLtGray);
	//int nLineHeight = cRuleHeight + cVideoHeight + 2;
	//dc.MoveTo(rcClient.left - 1, nLineHeight);
	//dc.LineTo(rcClient.right + 1, nLineHeight);

	//dc.SelectObject(&penGray);
	//nLineHeight = cRuleHeight + cVideoHeight + 3;
	//dc.MoveTo(rcClient.left - 1, nLineHeight);
	//dc.LineTo(rcClient.right + 1, nLineHeight);

	//dc.SelectObject(&penDkGray);
	//nLineHeight = cRuleHeight + cVideoHeight + 3;
	//dc.MoveTo(rcClient.left - 1, nLineHeight);
	//dc.LineTo(rcClient.right + 1, nLineHeight);

	//nLineHeight = cRuleHeight + cVideoHeight + cKeyFrameHeight + 2;
	//dc.MoveTo(rcClient.left - 1, nLineHeight);
	//dc.LineTo(rcClient.right + 1, nLineHeight);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);

	return CWnd::OnEraseBkgnd(pDC);
}

void CTimeLineCtrl::Initialize()
{
	m_pKeyFrame->Initialize();
	m_pVideoWnd->Initialize();

	SetTimeStart(0);
	m_lTimePointer = 0;
	SetPointerPos();
}

void CTimeLineCtrl::SetLength(long lLength)
{
	m_lLength = lLength;
	m_pKeyFrame->SetLength(lLength);
	m_pVideoWnd->SetLength(lLength);
	m_pAudioWnd->SetLength(lLength);
	m_pDrawWnd->SetLength(lLength);
	m_pAudioMixWnd->SetLength(lLength);
	m_pAudioReplaceWnd->SetLength(lLength);
	m_pVideoTextWnd->SetLength(lLength);
	m_pVideoEffectWnd->SetLength(lLength);
	
	//SetPixelUnit(1);
	//SetTimeUnit(100);
	if (m_lLength > 0)
	{
		int nPos = TimeToPos(m_lLength);
		CRect rcClient;
		GetClientRect(rcClient);
		while (nPos < (int)(rcClient.right * 3 / 4))
		{
			if (IsMinScale())
				break;

			int nTimeUnit, nPixelUnit;
			GetZoomIn(nTimeUnit, nPixelUnit);
			SetPixelUnit(nPixelUnit);
			SetTimeUnit(nTimeUnit);
			nPos = TimeToPos(m_lLength);
		}
	}

	m_pKeyFrame->RedrawWindow();
	m_pVideoWnd->RedrawWindow();
	m_pAudioWnd->RedrawWindow();
	m_pDrawWnd->RedrawWindow();
	m_pAudioReplaceWnd->RedrawWindow();
	m_pAudioMixWnd->RedrawWindow();
	m_pVideoTextWnd->RedrawWindow();
	m_pVideoEffectWnd->RedrawWindow();
}

void CTimeLineCtrl::ShowTimeAt(long lTime)
{
	SetTimeStart(lTime);
	SetPointerPos();
	
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.InflateRect(2, 2);
	rcClient.bottom = cRuleHeight;
	RedrawWindow(rcClient);

	m_pKeyFrame->RedrawWindow();
	m_pVideoWnd->RedrawWindow();
	m_pAudioWnd->RedrawWindow();
	m_pDrawWnd->RedrawWindow();
	m_pAudioReplaceWnd->RedrawWindow();
	m_pAudioMixWnd->RedrawWindow();
	m_pVideoTextWnd->RedrawWindow();
	m_pVideoEffectWnd->RedrawWindow();
}

void CTimeLineCtrl::OnEventPosChanged()
{
	m_pKeyFrame->ResetTooltip();
	m_pAudioReplaceWnd->ResetTooltip();
	m_pAudioMixWnd->ResetTooltip();
	m_pVideoTextWnd->ResetTooltip();
	m_pVideoEffectWnd->ResetTooltip();
}

void CTimeLineCtrl::SetPixelUnit(int nTime)
{
	m_nPixelUnit = nTime;
	m_pKeyFrame->SetPixelUnit(nTime);
	m_pVideoWnd->SetPixelUnit(nTime);
	m_pAudioWnd->SetPixelUnit(nTime);
	m_pDrawWnd->SetPixelUnit(nTime);
	m_pAudioReplaceWnd->SetPixelUnit(nTime);
	m_pAudioMixWnd->SetPixelUnit(nTime);
	m_pVideoTextWnd->SetPixelUnit(nTime);
	m_pVideoEffectWnd->SetPixelUnit(nTime);
}

void CTimeLineCtrl::SetTimeStart(long lTime)
{
	m_lTimeStart = lTime;
	m_pKeyFrame->SetTimeStart(lTime);
	m_pVideoWnd->SetTimeStart(lTime);
	m_pAudioWnd->SetTimeStart(lTime);
	m_pDrawWnd->SetTimeStart(lTime);
	m_pAudioReplaceWnd->SetTimeStart(lTime);
	m_pAudioMixWnd->SetTimeStart(lTime);
	m_pVideoTextWnd->SetTimeStart(lTime);
	m_pVideoEffectWnd->SetTimeStart(lTime);
}

void CTimeLineCtrl::SetTimeUnit(int nTimeUnit)
{
	m_nUnit = nTimeUnit;
	m_pKeyFrame->SetTimeUnit(nTimeUnit);
	m_pVideoWnd->SetTimeUnit(nTimeUnit);
	m_pAudioWnd->SetTimeUnit(nTimeUnit);
	m_pDrawWnd->SetTimeUnit(nTimeUnit);
	m_pAudioReplaceWnd->SetTimeUnit(nTimeUnit);
	m_pAudioMixWnd->SetTimeUnit(nTimeUnit);
	m_pVideoTextWnd->SetTimeUnit(nTimeUnit);
	m_pVideoEffectWnd->SetTimeUnit(nTimeUnit);
}

void CTimeLineCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rc;
	m_pPointer->GetWindowRect(rc);
	if (rc.IsRectEmpty())
	{
		rc.bottom = cRuleHeight - 1;
		rc.top = rc.bottom - m_csPointer.cy;
		rc.left = -m_csPointer.cx / 2;
		rc.right = rc.left + m_csPointer.cx;
		m_pPointer->MoveWindow(rc);
	}

	CRect rcAll;
	this->GetClientRect(rcAll);
	int cAllHeight = rcAll.bottom - rcAll.top - cRuleHeight - 1;

	CRect rcVideo;
	rcVideo.top = cRuleHeight + 1;
	rcVideo.bottom = rcVideo.top + cAllHeight / 5;//cVideoHeight;
	rcVideo.left = -1;
	rcVideo.right = cx;
	/*m_pVideoWnd*/m_pKeyFrame->MoveWindow(rcVideo);

	rcVideo.top = rcVideo.bottom;
	rcVideo.bottom = rcVideo.top + cAllHeight / 5;//cKeyFrameHeight;
	/*m_pKeyFrame*/m_pVideoWnd->MoveWindow(rcVideo);

	rcVideo.top = rcVideo.bottom;
	rcVideo.bottom = rcVideo.top + cAllHeight / 5;//cKeyFrameHeight;
	/*m_pKeyFrame*/m_pAudioWnd->MoveWindow(rcVideo);


	rcVideo.top = rcVideo.bottom;
	rcVideo.bottom = rcVideo.top + cAllHeight / 10;//cVideoEffectHeight;
	m_pVideoTextWnd->MoveWindow(rcVideo);

	rcVideo.top = rcVideo.bottom;
	rcVideo.bottom = rcVideo.top + cAllHeight / 10;//rcVideo.top + cAllHeight / 8;//cVideoEffectHeight;
	m_pVideoEffectWnd->MoveWindow(rcVideo);

	rcVideo.top = rcVideo.bottom;
	rcVideo.bottom = rcAll.bottom;//rcVideo.top + cAllHeight / 8;//cVideoEffectHeight;
	m_pDrawWnd->MoveWindow(rcVideo);

	SetLinePos();
}

void CTimeLineCtrl::SetLinePos() 
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc;
	m_pPointer->GetWindowRect(rc);
	ScreenToClient(rc);
	int nptx = rc.left + m_csPointer.cx / 2;
	m_pLocateLine->SetWindowPos(NULL, nptx, cRuleHeight - 1, 1, 
								rcClient.bottom + 1 - cRuleHeight, SWP_SHOWWINDOW);
	m_pLocateLine->RedrawWindow();
	UpdateWindow(); 
}

void CTimeLineCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pPointer->DestroyWindow();
	delete m_pPointer;

	m_pToolTip->DestroyWindow();
	delete m_pToolTip;

	m_pLocateLine->DestroyWindow();
	delete m_pLocateLine;

	m_pTmpLocateLine->DestroyWindow();
	delete m_pTmpLocateLine;

	m_pVideoWnd->DestroyWindow();
	m_pAudioWnd->DestroyWindow();
	m_pDrawWnd->DestroyWindow();
	delete m_pVideoWnd;
	delete m_pAudioWnd;
	delete m_pDrawWnd;

	m_pAudioReplaceWnd->DestroyWindow();
	delete m_pAudioReplaceWnd;

	m_pAudioMixWnd->DestroyWindow();
	delete m_pAudioMixWnd;

	m_pVideoTextWnd->DestroyWindow();
	delete m_pVideoTextWnd;

	m_pVideoEffectWnd->DestroyWindow();
	delete m_pVideoEffectWnd;

	m_pKeyFrame->DestroyWindow();
	delete m_pKeyFrame;

	if (m_nID > 0)
		KillTimer(m_nID);

	if (m_nID2 > 0)
		KillTimer(m_nID2);
}

int CTimeLineCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	rect.SetRectEmpty();
	m_pPointer->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_POINTER_START);

	m_pToolTip->Create(this);
	m_pToolTip->SetDrawFont(MACRO_SYS_FONT2);

	m_pVideoWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, 0);
	m_pAudioWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, 0);
	m_pDrawWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, 0);
	m_pKeyFrame->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, this, ID_KEY_FRAME);
	m_pAudioReplaceWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, ID_AUDIO_REPLACE);
	m_pAudioMixWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, ID_AUDIO_MIX);
	m_pVideoTextWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, ID_VIDEO_TEXT);
	m_pVideoEffectWnd->Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect, this, ID_VIDEO_EFFECT);

	m_pLocateLine->Create(rect, this);

	m_pTmpLocateLine->Create(rect, this);
	m_pTmpLocateLine->m_crFill = RGB(0, 0, 0);

	return 0;
}

void CTimeLineCtrl::ShowTmpLocateLine(BOOL bShow)
{
	if (bShow)
	{
		m_pTmpLocateLine->ShowWindow(SW_SHOW);
		if (m_nID2 == 0)
			m_nID2 = SetTimer(2, 100, NULL);
	}
	else
	{
		m_pTmpLocateLine->ShowWindow(SW_HIDE);
		m_pToolTip->ShowWindow(SW_HIDE);
		if (m_nID2 > 0)
		{
			KillTimer(m_nID2);
			m_nID2 = 0;
		}
	}
}

LRESULT CTimeLineCtrl::OnPointerDown(WPARAM wParam, LPARAM lParam)
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

	ShowTmpLocateLine(FALSE);

	return 0L;
}

LRESULT CTimeLineCtrl::OnPointerUp(WPARAM wParam, LPARAM lParam)
{
	if (m_nID > 0)
	{
		KillTimer(m_nID);
		m_nID = 0;
	}

	CRect rcPtr;
	m_pPointer->GetWindowRect(rcPtr);
	ScreenToClient(rcPtr);
	int nPtrWidth = m_csPointer.cx / 2;

	SetPointerTime(PosToTime(rcPtr.left + nPtrWidth));	

	m_pToolTip->ShowWindow(SW_HIDE);

	OnEventPosChanged();

	return 0L;
}

LRESULT CTimeLineCtrl::OnPointerMouseMove(WPARAM wParam, LPARAM lParam)
{
	UINT nFlag = (UINT)lParam;
	if (MK_LBUTTON & nFlag)
		return 0L;

	CPoint point;
	point.x = LOWORD(wParam);
	point.y = HIWORD(wParam);

	ScreenToClient(&point);
	if (point.y < cRuleHeight - 1)
	{	
		SetTmpLinePos(point);
		m_pPointer->RedrawWindow();
	}

	return 0L;
}

LRESULT CTimeLineCtrl::OnPointerMove(WPARAM wParam, LPARAM lParam)
{
	MovePointer((int)LOWORD(wParam), (int)HIWORD(wParam));

	return 0L;
}

void CTimeLineCtrl::MovePointer(int nX, int nY)
{
	CPoint point;
	point.x = nX;
	point.y = nY;

	ScreenToClient(&point);

	CRect rcThumb;
	m_pPointer->GetWindowRect(rcThumb);
	ScreenToClient(rcThumb);

	int nPtrWidth = m_csPointer.cx / 2;

	CRect rcClient;
	GetClientRect(rcClient);

	m_nLeftShift = 0;
	m_nRightShift = 0;
	int nptx = point.x + m_ptOffset.x;
	if (nptx < -nPtrWidth)
	{
		nptx = -nPtrWidth;
		m_nRightShift = 1;
	}
	else if (nptx > rcClient.right - nPtrWidth - 1)
	{
		nptx = rcClient.right - nPtrWidth - 1;
		m_nLeftShift = 1;
	}
	else
		nptx = TimeToPos(PosToTime(nptx + nPtrWidth)) - nPtrWidth;

	int nTop = cRuleHeight - 1 - m_csPointer.cy;

	m_pPointer->SetWindowPos(NULL, nptx, nTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_pPointer->RedrawWindow();

	SetLinePos();
	
	ShowToolTip(PosToTime(nptx + nPtrWidth));
	
	if (m_nLeftShift || m_nRightShift)
	{
		m_nID = SetTimer(1, 100, NULL);
	}
}

void CTimeLineCtrl::ShiftWindow(int nShiftAmount, BOOL bAdjustPos)
{
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.bottom = cRuleHeight;

	m_pPointer->ShowWindow(SW_HIDE);
	m_pLocateLine->ShowWindow(SW_HIDE);

	//ScrollWindow(nShiftAmount * m_nPixelUnit, 0, rcClient, rcClient);
	RedrawWindow(rcClient);

	CRect rcFrame;
	//m_pKeyFrame->GetClientRect(rcFrame);
	//m_pKeyFrame->ScrollWindow(nShiftAmount * m_nPixelUnit, 0, rcFrame, rcFrame);
	m_pKeyFrame->RedrawWindow();

	//m_pVideoWnd->GetClientRect(rcFrame);
	//m_pVideoWnd->ScrollWindow(nShiftAmount * m_nPixelUnit, 0, rcFrame, rcFrame);
	m_pVideoWnd->RedrawWindow();
	m_pAudioWnd->RedrawWindow();
	m_pDrawWnd->RedrawWindow();

	m_pAudioReplaceWnd->RedrawWindow();
	m_pAudioMixWnd->RedrawWindow();
	m_pVideoTextWnd->RedrawWindow();
	m_pVideoEffectWnd->RedrawWindow();

	if (bAdjustPos)
		SetPointerPos();
	else
	{
		m_pPointer->ShowWindow(SW_SHOW);
		m_pPointer->RedrawWindow();
		m_pLocateLine->ShowWindow(SW_SHOW);
		m_pLocateLine->RedrawWindow();
	}
}

BOOL CTimeLineCtrl::IsPointerVisible()
{
	CRect rcClient;
	GetClientRect(rcClient);

	long lTimeStart = PosToTime(-m_csPointer.cx);
	long lTimeEnd = PosToTime(rcClient.right + m_csPointer.cx);
	if (m_lTimePointer >= lTimeStart && m_lTimePointer <= lTimeEnd)
		return TRUE;

	return FALSE;
}

void CTimeLineCtrl::SetPointerPos()
{
	if (IsPointerVisible())
	{
		int nX = TimeToPos(m_lTimePointer);
		int nptx = nX - m_csPointer.cx / 2;
		int nTop = cRuleHeight - 1 - m_csPointer.cy;
		m_pPointer->SetWindowPos(NULL, nptx, nTop, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		m_pPointer->RedrawWindow();
		m_pLocateLine->ShowWindow(SW_SHOW);
		SetLinePos();
	}
	else
	{
		m_pPointer->ShowWindow(SW_HIDE);
		m_pLocateLine->ShowWindow(SW_HIDE);
	}
}

long CTimeLineCtrl::PosToTime(int xPos)
{
	long lRet = m_lTimeStart + m_nUnit * xPos / m_nPixelUnit;
	if (lRet < 0)
		lRet = 0;
	if (lRet > m_lLength)
		lRet = m_lLength;

	return lRet;
}

int CTimeLineCtrl::TimeToPos(long lTime)
{
	return (lTime - m_lTimeStart) * m_nPixelUnit / m_nUnit;
}

void CTimeLineCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ShowTmpLocateLine(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CTimeLineCtrl::ShowToolTip(long nPos)
{
	CPoint pt;
	GetCursorPos(&pt);
	pt.x += 6;
	pt.y += 20;

	CString strTime = GetTimeStr(nPos);
	m_pToolTip->ShowText(pt, strTime);
}

CString CTimeLineCtrl::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

void CTimeLineCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CTimeLineCtrl::OnTimer(UINT nIDEvent) 
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if (nIDEvent == m_nID)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		if ((pt.x < rcClient.right && m_nLeftShift) || 
			(pt.x > rcClient.left && m_nRightShift))
		{
			m_nRightShift = m_nLeftShift = 0;
			KillTimer(m_nID);
			m_nID = 0;
		}

		if (m_nRightShift)
		{
			GetParent()->PostMessage(WM_SHIFT_RIGHT);
			if (m_lTimeStart - 10 * m_nUnit >= 0)
			{
				m_lTimeStart -= 10 * m_nUnit;
				m_pKeyFrame->SetTimeStart(m_lTimeStart);
				m_pVideoWnd->SetTimeStart(m_lTimeStart);
				m_pAudioWnd->SetTimeStart(m_lTimeStart);
				m_pDrawWnd->SetTimeStart(m_lTimeStart);
				m_pAudioReplaceWnd->SetTimeStart(m_lTimeStart);
				m_pAudioMixWnd->SetTimeStart(m_lTimeStart);
				m_pVideoTextWnd->SetTimeStart(m_lTimeStart);
				m_pVideoEffectWnd->SetTimeStart(m_lTimeStart);
				ShiftWindow(10, FALSE);
			}
		}
		else if (m_nLeftShift)
		{
			GetParent()->PostMessage(WM_SHIFT_LEFT);
			if (m_lTimeStart + 10 * m_nUnit < m_lLength)
			{
				m_lTimeStart += 10 * m_nUnit;
				m_pKeyFrame->SetTimeStart(m_lTimeStart);
				m_pVideoWnd->SetTimeStart(m_lTimeStart);
				m_pAudioWnd->SetTimeStart(m_lTimeStart);
				m_pDrawWnd->SetTimeStart(m_lTimeStart);
				m_pAudioReplaceWnd->SetTimeStart(m_lTimeStart);
				m_pAudioMixWnd->SetTimeStart(m_lTimeStart);
				m_pVideoTextWnd->SetTimeStart(m_lTimeStart);
				m_pVideoEffectWnd->SetTimeStart(m_lTimeStart);
				ShiftWindow(-10, FALSE);
			}
		}
	}
	else if (nIDEvent == m_nID2)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.bottom = cRuleHeight - 1;
		if (!rcClient.PtInRect(pt))
		{
			ShowTmpLocateLine(FALSE);
			m_pPointer->RedrawWindow();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CTimeLineCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (point.y < cRuleHeight - 1)
		SetTmpLinePos(point);
	else
		ShowTmpLocateLine(FALSE);

	CWnd::OnMouseMove(nFlags, point);
}

void CTimeLineCtrl::SetPointerTime(long lTime)
{
	m_lTimePointer = lTime;
	SetPointerPos();
	GetParent()->PostMessage(WM_TIME_POS_CHANGED, m_lTimePointer, 0);
}

void CTimeLineCtrl::SetPointerTimeAndShow(long lTime)
{
	if (m_lTimePointer == lTime)
		return;

	m_lTimePointer = lTime;
	SetPointerPos();

	CRect rcClient;
	GetClientRect(rcClient);

	BOOL bEdge = FALSE;
	long lTimeStart = PosToTime(0);
	long lTimeEnd = PosToTime(rcClient.right - m_csPointer.cx);
	if (m_lTimePointer >= lTimeStart && m_lTimePointer <= lTimeEnd)
		bEdge = TRUE;

	if (!bEdge)
	{
		long lNewStart = PosToTime(TimeToPos(m_lTimePointer) - rcClient.Width() / 2);
		lNewStart -= lNewStart % 100;
		ShowTimeAt(lNewStart);
		OnEventPosChanged();

		GetParent()->PostMessage(WM_CHANGE_SLIDER, m_lTimeStart);
	}
}

void CTimeLineCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (point.y >= 0 && point.y < cRuleHeight - 1)
		SetPointerTime(PosToTime(point.x));
	
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CTimeLineCtrl::OnColorWndMouseMove(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	point.x = LOWORD(wParam);
	point.y = HIWORD(wParam);

	ScreenToClient(&point);

	if (point.y > cRuleHeight)
		return 0L;

	SetTmpLinePos(point);

	return 0L;
}

void CTimeLineCtrl::SetTmpLinePos(CPoint point)
{
	CRect rcClient;
	GetClientRect(rcClient);
	int nHeight = rcClient.Height();

	m_pTmpLocateLine->SetWindowPos(NULL, point.x, point.y + 1, 1, nHeight, SWP_SHOWWINDOW);
	ShowTmpLocateLine(TRUE);
	m_pTmpLocateLine->RedrawWindow();
	UpdateWindow(); 

	ShowToolTip(m_lTimeStart + m_nUnit * point.x / m_nPixelUnit);
}

void CTimeLineCtrl::SetKeyFrame(CStringArray& strArrayTime, CStringArray& strArrayMark,  
								CStringArray& strArrayScript, CStringArray& strArrayCmd)
{
	m_pKeyFrame->SetKeyFrame(strArrayMark, strArrayScript, strArrayCmd, strArrayTime);
}

void CTimeLineCtrl::SetKeyFrame(CStringArray& strMarkArray, CStringArray& strMarkTimeArray, 
								CStringArray& strScriptArray, CStringArray& strCmdArray, 
								CStringArray& strScriptTimeArray)
{
	m_pKeyFrame->RemoveAllKeyFrame();
	if (strMarkArray.GetSize() == 0)
	{
		m_pKeyFrame->RedrawWindow();
		return;
	}

	CStringArray strArrayTime;
	int i = 0;
	for (i = 0; i < strMarkArray.GetSize(); i++)
		AddstrArrayNoDup(strMarkTimeArray[i], strArrayTime);
	for (i = 0; i < strScriptArray.GetSize(); i++)
		AddstrArrayNoDup(strScriptTimeArray[i], strArrayTime);
	
	CStringArray strArrayMark, strArrayScript, strArrayCmd;
	for (i = 0; i < strArrayTime.GetSize(); i++)
	{
		strArrayMark.Add("");
		strArrayScript.Add("");
		strArrayCmd.Add("");
		CString strTime = strArrayTime[i];
	}

	for (i = 0; i < strMarkArray.GetSize(); i++)
	{
		CString strMarkTime = strMarkTimeArray[i];
		CString strMark = strMarkArray[i];
		for (int j = 0; j < strArrayTime.GetSize(); j++)
		{
			if (strMarkTime == strArrayTime[j])
			{
				strArrayMark[j] = strMark;
				break;
			}
		}
	};

	for (i = 0; i < strScriptArray.GetSize(); i++)
	{
		CString strScript = strScriptArray[i];
		CString strCmd = strCmdArray[i];
		CString strScriptTime = strScriptTimeArray[i];
		for (int j = 0; j < strArrayTime.GetSize(); j++)
		{
			if (strScriptTime == strArrayTime[j])
			{
				strArrayScript[j] = strScript;
				strArrayCmd[j] = strCmd;
				break;
			}
		}
	};

	m_pKeyFrame->SetKeyFrame(strArrayMark, strArrayScript, strArrayCmd, strArrayTime);
}

void CTimeLineCtrl::AddstrArrayNoDup(CString str, CStringArray& strArray)
{
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		if (strArray[i] > str)
		{
			strArray.InsertAt(i, str);
			return;
		}
		else if (strArray[i] == str)
			return;
	}
	strArray.Add(str);
}

void CTimeLineCtrl::RemoveAllKeyFrame()
{
	m_pKeyFrame->RemoveAllKeyFrame();
	m_pKeyFrame->RedrawWindow();
}

LRESULT CTimeLineCtrl::OnKeyFramePressed(WPARAM wParam, LPARAM lParam)
{
	SetPointerTime(lParam);

	return 0L;
}

LRESULT CTimeLineCtrl::OnEffectPressed(WPARAM wParam, LPARAM lParam)
{
	SetPointerTime(lParam);

	return 0L;
}

LRESULT CTimeLineCtrl::OnKeyFrameMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_KEY_FRAME_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnKeyFrameTobeMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_KEY_FRAME_TOBE_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnRegenTag(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_REGEN_TAG, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnKeyFramePressing(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_KEY_FRAME_PRESSING, wParam, lParam);

	return 0L;
}

void CTimeLineCtrl::GetTagFromUI(CStringArray& strArrayTimeMarker, 
								 CStringArray& strArrayMarker, 
								 CStringArray& strArrayTimeScript,
								 CStringArray& strArrayScript,
								 CStringArray& strArrayCmd)
{
	m_pKeyFrame->GetKeyFrameData(strArrayTimeMarker, strArrayMarker, strArrayTimeScript,
								 strArrayScript, strArrayCmd);
}

void CTimeLineCtrl::GetTagFromUI(CStringArray& strArrayTime, 
								 CStringArray& strArrayMarker, 
								 CStringArray& strArrayScript,
								 CStringArray& strArrayCmd)
{
	m_pKeyFrame->GetKeyFrameData(strArrayTime, strArrayMarker, strArrayScript, strArrayCmd);
}

void CTimeLineCtrl::SetMark()
{
	m_pVideoWnd->SetPointerTime(m_lTimePointer);
}

LRESULT CTimeLineCtrl::OnGetPointerTime(WPARAM wParam, LPARAM lParam)
{
	m_pVideoWnd->m_lTimeCurrent = m_lTimePointer;

	return 0L;
}

BOOL CTimeLineCtrl::DeletePeriod()
{
	long lTimeSelStart, lTimeSelEnd;
	m_pVideoWnd->GetSelRange(lTimeSelStart, lTimeSelEnd);
	if (lTimeSelStart < 0)
	{
		W_SHOW_WARNING(IDS_NOT_MARK_COMPLETE_FOR_CUT, this);
		return FALSE;
	}
	return DeletePeriod(lTimeSelStart, lTimeSelEnd);
}

BOOL CTimeLineCtrl::DeletePeriod(long lTimeSelStart, long lTimeSelEnd)
{
	if (IsAnyPeriodDeleted(lTimeSelStart, lTimeSelEnd))
	{
		if (W_ASK_YESNO(IDS_EFFECT_ALL_DELETED, this) == IDNO)
			return FALSE;
	}

	m_pKeyFrame->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pKeyFrame->TagChanged();
	
	m_pAudioReplaceWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pAudioReplaceWnd->ResetTooltip();

	m_pAudioMixWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pAudioMixWnd->ResetTooltip();

	m_pVideoEffectWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pVideoEffectWnd->ResetTooltip();

	m_pVideoTextWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pVideoTextWnd->ResetTooltip();

	m_pAudioWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pAudioWnd->ResetTooltip();

	m_pDrawWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);
	m_pDrawWnd->ResetTooltip();

	m_pVideoWnd->DeletePeriod(lTimeSelStart, lTimeSelEnd);

	GetParent()->SendMessage(WM_SET_LENGTH, m_lLength - (lTimeSelEnd - lTimeSelStart));

	SetPointerTime(lTimeSelStart);

	SetBreakPoint(lTimeSelStart, lTimeSelEnd);
	m_pVideoEffectWnd->RedrawWindow();

	return TRUE;
}

BOOL CTimeLineCtrl::IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd)
{
	BOOL bRet = m_pAudioReplaceWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd) ||
				m_pAudioMixWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd) ||
//				m_pVideoEffectWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd) ||
				m_pVideoTextWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd) ||
				m_pAudioWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd) ||
				m_pDrawWnd->IsAnyPeriodDeleted(lTimeStart, lTimeEnd);

	return bRet;
}

BOOL CTimeLineCtrl::ReplacePeriod(long lTimeStart, long lTimeEnd, CString strVideoPath, long lDuration)
{
	if (lTimeEnd >= 0 && IsAnyPeriodDeleted(lTimeStart, lTimeEnd))
	{
		if (W_ASK_YESNO(IDS_EFFECT_ALL_DELETED, this) == IDNO)
			return FALSE;
	}

	m_pVideoWnd->ReplacePeriod(lTimeStart, lTimeEnd, strVideoPath, lDuration);

	if (lTimeEnd >= 0)
	{
		m_pKeyFrame->DeletePeriod(lTimeStart, lTimeEnd);
		m_pKeyFrame->InsertPeriod(lTimeStart, lDuration);
		m_pKeyFrame->TagChanged();

		m_pAudioReplaceWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pAudioReplaceWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioReplaceWnd->ResetTooltip();

		m_pAudioMixWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pAudioMixWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioMixWnd->ResetTooltip();

		m_pVideoEffectWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pVideoEffectWnd->InsertPeriod(lTimeStart, lDuration);
		m_pVideoEffectWnd->ResetTooltip();

		m_pVideoTextWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pVideoTextWnd->InsertPeriod(lTimeStart, lDuration);
		m_pVideoTextWnd->ResetTooltip();

		m_pAudioWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pAudioWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioWnd->ResetTooltip();

		m_pDrawWnd->DeletePeriod(lTimeStart, lTimeEnd);
		m_pDrawWnd->InsertPeriod(lTimeStart, lDuration);
		m_pDrawWnd->ResetTooltip();

		GetParent()->SendMessage(WM_SET_LENGTH, m_lLength - (lTimeEnd - lTimeStart) + lDuration);
	}
	else if (lTimeEnd < 0)
	{
		m_pKeyFrame->InsertPeriod(lTimeStart, lDuration);
		m_pKeyFrame->TagChanged();

		m_pAudioReplaceWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioReplaceWnd->ResetTooltip();

		m_pAudioMixWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioMixWnd->ResetTooltip();

		m_pVideoEffectWnd->InsertPeriod(lTimeStart, lDuration);
		m_pVideoEffectWnd->ResetTooltip();

		m_pVideoTextWnd->InsertPeriod(lTimeStart, lDuration);
		m_pVideoTextWnd->ResetTooltip();

		m_pAudioWnd->InsertPeriod(lTimeStart, lDuration);
		m_pAudioWnd->ResetTooltip();

		m_pDrawWnd->InsertPeriod(lTimeStart, lDuration);
		m_pDrawWnd->ResetTooltip();

		GetParent()->SendMessage(WM_SET_LENGTH, m_lLength + lDuration);
	}

	SetPointerTime(lTimeStart);

	SetBreakPoint();
	m_pVideoEffectWnd->RedrawWindow();

	return TRUE;
}

void CTimeLineCtrl::UnDeletePeriod(long lTime)
{
	long lDuration;
	m_pVideoWnd->UnDeletePeriod(lTime, lDuration);

	m_pKeyFrame->InsertPeriod(lTime, lDuration);
	m_pKeyFrame->TagChanged();

	m_pAudioReplaceWnd->InsertPeriod(lTime, lDuration);
	m_pAudioReplaceWnd->ResetTooltip();

	m_pAudioMixWnd->InsertPeriod(lTime, lDuration);
	m_pAudioMixWnd->ResetTooltip();

	m_pVideoEffectWnd->InsertPeriod(lTime, lDuration);
	m_pVideoEffectWnd->ResetTooltip();
	SetBreakPoint(lTime + lDuration, lTime + lDuration);
	m_pVideoEffectWnd->RedrawWindow();

	m_pVideoTextWnd->InsertPeriod(lTime, lDuration);
	m_pVideoTextWnd->ResetTooltip();

	GetParent()->SendMessage(WM_SET_LENGTH, m_lLength + lDuration);

	SetPointerTime(lTime);
}

void CTimeLineCtrl::GetReplacePeriod(long& lTimeStart, long& lTimeEnd)
{
	m_pVideoWnd->GetSelRange(lTimeStart, lTimeEnd);
	if (lTimeStart < 0)
		lTimeStart = m_pVideoWnd->GetPointerTime();
}

LRESULT CTimeLineCtrl::OnPeriodClicked(WPARAM wParam, LPARAM lParam)
{
	long lTimeStart = wParam;
	long lTimeEnd = lParam;
	if (lTimeStart != lTimeEnd)
	{
		if (m_lTimePointer == lTimeStart)
			SetPointerTime(lTimeEnd);
		else
			SetPointerTime(lTimeStart);
	}
	else
	{
		if (m_lTimePointer != lTimeStart)
			SetPointerTime(lTimeStart);
	}

	return 0L;
}

LRESULT CTimeLineCtrl::OnClickVideoEffect(WPARAM wParam, LPARAM lParam)
{
	if (m_lTimePointer == (long)wParam)
		SetPointerTime(lParam);
	else
		SetPointerTime(wParam);

	return 0L;
}

BOOL CTimeLineCtrl::GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName)
{
	return m_pVideoWnd->GetVideoPosAtTime(lTime, lVideoPos, strFileName);
}

BOOL CTimeLineCtrl::GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName)
{
	return m_pVideoWnd->GetVideoPosNextToTime(lTime, lVideoPos, strFileName);
}

BOOL CTimeLineCtrl::GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName)
{
	return m_pVideoWnd->GetTimeByVideoPos(lTime, lVideoPos, strFileName);
}

void CTimeLineCtrl::GetVideoSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath)
{
	m_pVideoWnd->GetSegString(str, strDefault, bConvDefault, strSubPath);
}

void CTimeLineCtrl::GetAudioReplaceSegString(CString& str)
{
	m_pAudioReplaceWnd->GetSegString(str);
}

void CTimeLineCtrl::GetAudioMixSegString(CString& str)
{
	m_pAudioMixWnd->GetSegString(str);
}

void CTimeLineCtrl::GetVideoTextSegString(CString& str)
{
	m_pVideoTextWnd->GetSegString(str);
}

void CTimeLineCtrl::GetVideoEffectSegString(CString& str)
{
	m_pVideoEffectWnd->GetSegString(str);
}

LRESULT CTimeLineCtrl::OnCPAction(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_CP_ACTION, wParam, lParam);
	return 0L;
}

void CTimeLineCtrl::GetFrameInfo(int nIndex, CString& strTitle, long& lTime, 
								 CString& strCmd, CString& strStep)
{
	m_pKeyFrame->GetFrameInfo(nIndex, strTitle, lTime, strCmd, strStep);
}

void CTimeLineCtrl::RemoveFrameObj(int nIndex)
{
	m_pKeyFrame->RemoveFrameObj(nIndex);
	m_pKeyFrame->TagChanged();
}

void CTimeLineCtrl::GetZoomIn(int& nRetTimeUnit, int& nRetPixelUnit)
{
	int nUnit = GetTimeUnit();
	int nPixelUnit = GetPixelUnit();
	nRetTimeUnit = nUnit;
	nRetPixelUnit = nPixelUnit;
	if (nUnit == 100 && nPixelUnit == 2)
		nRetPixelUnit = 4;
	else if (nUnit == 100 && nPixelUnit == 1)
		nRetPixelUnit = 2;
	else if (nUnit == 200)
		nRetTimeUnit = 100;
	else if (nUnit == 400)
		nRetTimeUnit = 200;
	else if (nUnit == 600)
		nRetTimeUnit = 400;
	else if (nUnit == 800)
		nRetTimeUnit = 600;
	else if (nUnit == 1000)
		nRetTimeUnit = 800;
	else if (nUnit == 2000)
		nRetTimeUnit = 1000;
	else if (nUnit == 4000)
		nRetTimeUnit = 2000;
	else if (nUnit == 6000)
		nRetTimeUnit = 4000;
	else if (nUnit == 8000)
		nRetTimeUnit = 6000;
	else if (nUnit == 10000)
		nRetTimeUnit = 8000;
	else if (nUnit == 20000)
		nRetTimeUnit = 10000;
}

BOOL CTimeLineCtrl::IsMinScale()
{
	int nUnit = GetTimeUnit();
	int nPixelUnit = GetPixelUnit();
	return nUnit == 100 && nPixelUnit == 4;
}

void CTimeLineCtrl::GetZoomOut(int& nRetTimeUnit, int& nRetPixelUnit)
{
	int nUnit = GetTimeUnit();
	int nPixelUnit = GetPixelUnit();
	nRetTimeUnit = nUnit;
	nRetPixelUnit = nPixelUnit;
	if (nUnit == 100 && nPixelUnit == 4)
		nRetPixelUnit = 2;
	else if (nUnit == 100 && nPixelUnit == 2)
		nRetPixelUnit = 1;
	else if (nUnit == 100 && nPixelUnit == 1)
		nRetTimeUnit = 200;
	else if (nUnit == 200)
		nRetTimeUnit = 400;
	else if (nUnit == 400)
		nRetTimeUnit = 600;
	else if (nUnit == 600)
		nRetTimeUnit = 800;
	else if (nUnit == 800)
		nRetTimeUnit = 1000;
	else if (nUnit == 1000)
		nRetTimeUnit = 2000;
	else if (nUnit == 2000)
		nRetTimeUnit = 4000;
	else if (nUnit == 4000)
		nRetTimeUnit = 6000;
	else if (nUnit == 6000)
		nRetTimeUnit = 8000;
	else if (nUnit == 8000)
		nRetTimeUnit = 10000;
	else if (nUnit == 10000)
		nRetTimeUnit = 20000;
}

CRect CTimeLineCtrl::GetCtrlRect(int nIndex)
{
	CRect rc;
	if (nIndex == 0)
		m_pVideoWnd->GetWindowRect(rc);
	else if (nIndex == 1)
		m_pKeyFrame->GetWindowRect(rc);
	else if (nIndex == 2)
		m_pAudioReplaceWnd->GetWindowRect(rc);
	else if (nIndex == 3)
		m_pAudioMixWnd->GetWindowRect(rc);
	else if (nIndex == 4)
		m_pVideoTextWnd->GetWindowRect(rc);
	else if (nIndex == 5)
		m_pVideoEffectWnd->GetWindowRect(rc);
	else if (nIndex == 6)
		m_pAudioWnd->GetWindowRect(rc);
	else if (nIndex == 7)
		m_pDrawWnd->GetWindowRect(rc);

	return rc;
}

/*
BOOL CTimeLineCtrl::IsVideoModified()
{
	return m_pVideoWnd->IsVideoModified();
}
*/

void CTimeLineCtrl::SetVideoSegArray(CStringArray& strArrayVideo, CString strSubPath)
{
	m_pVideoWnd->SetSegArray(strArrayVideo, strSubPath);
}

void CTimeLineCtrl::SetAudioReplaceSegArray(CStringArray& strArrayVideo)
{
	m_pAudioReplaceWnd->SetSegArray(strArrayVideo, 0);
	m_pAudioWnd->SetSegArray(strArrayVideo, 1);
}

void CTimeLineCtrl::SetAudioMixSegArray(CStringArray& strArrayVideo)
{
	m_pAudioMixWnd->SetSegArray(strArrayVideo, 0);
	m_pAudioWnd->SetSegArray(strArrayVideo, 2);
}

void CTimeLineCtrl::SetVideoTextSegArray(CStringArray& strArrayVideo)
{
	m_pVideoTextWnd->SetSegArray(strArrayVideo, 1);
}

void CTimeLineCtrl::SetVideoEffectSegArray(CStringArray& strArrayVideo)
{
	m_pVideoEffectWnd->SetSegArray(strArrayVideo);
}

void CTimeLineCtrl::SetNewSegArray()
{
	m_pVideoWnd->SetNewSegArray();
}

BOOL CTimeLineCtrl::AddAudioReplace(long lTime, long lTimeEnd, CString strDestPath, 
									COLORREF crNormal, COLORREF crHot)
{
	if (!m_pAudioReplaceWnd->AddSegment(lTime, lTimeEnd, strDestPath, 0, "", crNormal, crHot))
		return FALSE;

	if (!m_pAudioWnd->AddSegment(lTime, lTimeEnd, strDestPath, 1, "", crNormal, crHot))
		return FALSE;

	/*m_pAudioReplaceWnd*/m_pAudioWnd->RedrawWindow();
	return TRUE;
}

void CTimeLineCtrl::SetNoSelectAndRepaint()
{
	m_pVideoWnd->SetNoSelectAndRepaint();
}

BOOL CTimeLineCtrl::AddAudioMix(long lTime, long lTimeEnd, CString strDestPath)
{
	if (!m_pAudioMixWnd->AddSegment(lTime, lTimeEnd, strDestPath, 0))
		return FALSE;

	if (!m_pAudioWnd->AddSegment(lTime, lTimeEnd, strDestPath, 2))
		return FALSE;

	/*m_pAudioMixWnd*/m_pAudioWnd->RedrawWindow();
	return TRUE;
}

BOOL CTimeLineCtrl::AddVideoText(long lTime, long lTimeEnd, CString strDestPath, CString strTip)
{
	if (!m_pVideoTextWnd->AddSegment(lTime, lTimeEnd, strDestPath, 1, strTip))
		return FALSE;

	m_pVideoTextWnd->RedrawWindow();
	return TRUE;
}

BOOL CTimeLineCtrl::AddVideoEffect(long lTime, long lDuration, int nType, CString strDestPath)
{
	if (!m_pVideoEffectWnd->AddSegment(lTime, lDuration, nType, strDestPath, ""))
		return FALSE;

	m_pVideoEffectWnd->RedrawWindow();
	return TRUE;
}

LRESULT CTimeLineCtrl::OnPeriodMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_TOBE_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodMoving(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_MOVING, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodDelete(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_DELETE, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodView(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_VIEW, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodEdit(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_EDIT, wParam, lParam);
	return 0L;
}

void CTimeLineCtrl::DeleteAudioReplaceByIndex(int nIndex)
{
	m_pAudioReplaceWnd->DeletePeriodByIndex(nIndex);
	m_pAudioReplaceWnd->ResetTooltip();
}

void CTimeLineCtrl::DeleteAudioMixByIndex(int nIndex)
{
	m_pAudioMixWnd->DeletePeriodByIndex(nIndex);
	m_pAudioMixWnd->ResetTooltip();
}

void CTimeLineCtrl::DeleteVideoTextByIndex(int nIndex)
{
	m_pVideoTextWnd->DeletePeriodByIndex(nIndex);
	m_pVideoTextWnd->ResetTooltip();
}

void CTimeLineCtrl::DeleteVideoEffectByIndex(int nIndex)
{
	long lTime, lDuration;
	CString strPath;
	int nType;
	m_pVideoEffectWnd->GetSegInfo(nIndex, lTime, lDuration, strPath, nType); 
	
	CDWordArray dwArrayBreakPoint;
	m_pVideoWnd->GetBreakPoint(dwArrayBreakPoint);
	BOOL bFound = FALSE;
	for (int i = 0; i < dwArrayBreakPoint.GetSize(); i++)
	{
		if ((long)dwArrayBreakPoint[i] == lTime)
		{
			bFound = TRUE;
			break;
		}
	}
	if (bFound)
		m_pVideoEffectWnd->SetEmptyByndex(nIndex);
	else
		m_pVideoEffectWnd->DeletePeriodByIndex(nIndex);

	m_pVideoEffectWnd->ResetTooltip();
}

void CTimeLineCtrl::GetAudioReplaceSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
										   CString& strPath, int& nType)
{
	m_pAudioReplaceWnd->GetSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLineCtrl::SetAudioReplaceSegColor(int nIndex, COLORREF crNormal, COLORREF crHot)
{
	m_pAudioReplaceWnd->SetSegColor(nIndex, crNormal, crHot);
}

void CTimeLineCtrl::GetAudioMixSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
									   CString& strPath, int& nType)
{
	m_pAudioMixWnd->GetSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLineCtrl::GetVideoTextSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
								 	    CString& strPath, int& nType)
{
	m_pVideoTextWnd->GetSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLineCtrl::GetVideoEffectSegInfo(int nIndex, long& lTime, long& lDuration, 
									      CString& strPath, int& nType)
{
	m_pVideoEffectWnd->GetSegInfo(nIndex, lTime, lDuration, strPath, nType);
}

BOOL CTimeLineCtrl::IsVideoTextOverlap(long lTime, long lTimeEnd)
{
	return m_pVideoTextWnd->TimeOverlap(lTime, lTimeEnd, -1);
}

BOOL CTimeLineCtrl::IsVideoEffectOverlap(long lTime, long lDuration)
{
	return m_pVideoEffectWnd->TimeOverlap(lTime, lDuration, -1);
}

BOOL CTimeLineCtrl::IsAudioReplaceOverlap(long lTime, long lTimeEnd)
{
	return m_pAudioReplaceWnd->TimeOverlap(lTime, lTimeEnd, -1);
}

BOOL CTimeLineCtrl::IsAudioMixOverlap(long lTime, long lTimeEnd)
{
	return m_pAudioMixWnd->TimeOverlap(lTime, lTimeEnd, -1);
}

void CTimeLineCtrl::SetVideoTextSegTip(int nIndex, CString strTip) 
{
	m_pVideoTextWnd->SetSegTip(nIndex, strTip);
}

int CTimeLineCtrl::GetVideoTextSegCount()
{
	return m_pVideoTextWnd->GetSegCount();
}

void CTimeLineCtrl::ResetVideoTextTooltip()
{
	m_pVideoTextWnd->ResetTooltip();
}

void CTimeLineCtrl::ResetVideoEffectTooltip()
{
	m_pVideoEffectWnd->ResetTooltip();
}

void CTimeLineCtrl::ResetAudioReplaceTooltip()
{
	m_pAudioReplaceWnd->ResetTooltip();
}

void CTimeLineCtrl::ResetAudioMixTooltip()
{
	m_pAudioMixWnd->ResetTooltip();
}

LRESULT CTimeLineCtrl::OnPeriodUnInsert(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_UNINSERT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnPeriodUnDelete(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_UNDELETE, wParam, lParam);
	return 0L;
}

void CTimeLineCtrl::GetSelRange(long& lTimeSelStart, long& lTimeSelEnd)
{
	m_pVideoWnd->GetSelRange(lTimeSelStart, lTimeSelEnd);
}

LRESULT CTimeLineCtrl::OnFadeInEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_FADE_IN_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnFadeOutEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_FADE_OUT_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnDeleteEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_DELETE_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnTransitEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_TRANSIT_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLineCtrl::OnSetBreakPoint(WPARAM wParam, LPARAM lParam)
{
	SetBreakPoint();
	return 0L;
}

void CTimeLineCtrl::SetBreakPoint(long lTimeStart, long lTimeEnd)
{
	CDWordArray dwArrayBreakPoint;
	m_pVideoWnd->GetBreakPoint(dwArrayBreakPoint);
	m_pVideoEffectWnd->SetBreakPoint(dwArrayBreakPoint, lTimeStart, lTimeEnd);
}

BOOL CTimeLineCtrl::GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, 
								     CString& strPathNext, long& lTimeNext)
{
	return m_pVideoWnd->GetBoundaryFiles(lTime, strPathPrev, lTimePrev, strPathNext, lTimeNext);
}

void CTimeLineCtrl::SetSlidePath(CString slidePath)
{
	m_pKeyFrame->SetSlidePath(slidePath);
}
