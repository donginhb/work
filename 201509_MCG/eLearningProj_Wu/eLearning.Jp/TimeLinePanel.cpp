// TimeLinePanel.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "TimeLinePanel.h"
#include "MemDC.h"
#include "TimeLineCtrl.h"
#include "SliderWnd.h"
#include "ToolTipWnd.h"
#include "eLearningView.h"
#include "macro.h"
#include "cmkapi.h"
#include "BmpBtn.h"
#include "ToolPaleteVideoEdit.h"
#include "KeyFramePanel.h"
#include "VideoEffectWnd.h"
#include "VideoWnd.h"
#include "Mainfrm.h"
#include "DlgSelVideoSrc.h"
#include "eLearningAPI.h"
#include "DlgEditMark.h"
#include "EffectManageWnd.h"
#include "grdapi.h"
#include "DlgSelDevice.h"
#include "CaptureDevApi.h"
#include <dshow.h>
#include "FileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define cLeftGap	75
#define cLeftGapEn	105
#define cRightGap	74
#define cBottomGap	25
#define cScrollUnit 10
/////////////////////////////////////////////////////////////////////////////
// CTimeLinePanel

CTimeLinePanel::CTimeLinePanel()
{
	m_csBtn = ::GetBmpDim(IDB_PREV);
	m_csBtn.cx /= 4;

	m_csSmallBtn = ::GetBmpDim(IDB_ZOOM_IN_SCALE);
	m_csSmallBtn.cx /= 4;
}

CTimeLinePanel::~CTimeLinePanel()
{
}


BEGIN_MESSAGE_MAP(CTimeLinePanel, CWnd)
	//{{AFX_MSG_MAP(CTimeLinePanel)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_SLIDER_HSCROLL, OnSliderHScroll)
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_MESSAGE(WM_SHIFT_LEFT, OnShiftLeft)
	ON_MESSAGE(WM_SHIFT_RIGHT, OnShiftRight)
	ON_MESSAGE(WM_TIME_POS_CHANGED, OnTimePosChanged)
	ON_MESSAGE(WM_CHANGE_SLIDER, OnChangeSlider)
	ON_MESSAGE(WM_REGEN_TAG, OnRegenTag)
	ON_MESSAGE(WM_KEY_FRAME_MOVED, OnKeyMoved)
	ON_MESSAGE(WM_KEY_FRAME_TOBE_MOVED, OnKeyTobeMoved)
	ON_MESSAGE(WM_KEY_FRAME_PRESSING, OnKeyPressing)
	ON_MESSAGE(WM_PALETE_BTN_DOWN, OnPaleteBtn)
	ON_MESSAGE(WM_SET_LENGTH, OnSetLength)
	ON_MESSAGE(WM_CP_ACTION, OnCPAction)
	ON_MESSAGE(WM_PERIOD_TOBE_MOVED, OnPeriodTobeMoved)
	ON_MESSAGE(WM_PERIOD_MOVED, OnPeriodMoved)
	ON_MESSAGE(WM_PERIOD_DELETE, OnPeriodDelete)
	ON_MESSAGE(WM_PERIOD_VIEW, OnPeriodView)
	ON_MESSAGE(WM_PERIOD_EDIT, OnPeriodEdit)
	ON_MESSAGE(WM_PERIOD_MOVING, OnPeriodMoving)
	ON_MESSAGE(WM_PERIOD_UNINSERT, OnPeriodUnInsert)
	ON_MESSAGE(WM_PERIOD_UNDELETE, OnPeriodUnDelete)
	ON_MESSAGE(WM_FADE_IN_EFFECT, OnFadeInEffect)
	ON_MESSAGE(WM_FADE_OUT_EFFECT, OnFadeOutEffect)
	ON_MESSAGE(WM_TRANSIT_EFFECT, OnTransitEffect)
	ON_MESSAGE(WM_DELETE_EFFECT, OnDeleteEffect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTimeLinePanel::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	//if (m_strClass == "")
	//	m_strClass = AfxRegisterWndClass(CS_DBLCLKS, 0, NULL);
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CTimeLinePanel::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTimeLinePanel message handlers

BOOL CTimeLinePanel::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();

	CFont* pOldFont = (CFont*)dc.SelectObject(MACRO_SYS_FONT);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	CRect rcText = m_pTimeCtrl->GetCtrlRect(0);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	rcText.top++;
	rcText.bottom++;

	CString strText;
	strText.LoadString(IDS_VIDEO);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(6);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	rcText.top++;
	rcText.bottom++;

	strText.LoadString(IDS_AUDIO_TRACK);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	
	rcText = m_pTimeCtrl->GetCtrlRect(1);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	
	strText.LoadString(IDS_SLIDES);//IDS_SYNC_POINT);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(2);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	//rcText.top -= 4;
	//rcText.bottom -= 4;
	
	strText.LoadString(IDS_AUDIO_REPLACE);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(3);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	//rcText.top -= 3;
	//rcText.bottom -= 3;
	
	strText.LoadString(IDS_AUDIO_MIX);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(4);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	//rcText.top -= 2;
	//rcText.bottom -= 2;
	
	strText.LoadString(IDS_CAPTIONS);//IDS_VIDEO_TEXT);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(5);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	//rcText.top -= 1;
	//rcText.bottom -= 1;
	
	strText.LoadString(IDS_VIDEO_EFFECT);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	rcText = m_pTimeCtrl->GetCtrlRect(7);
	ScreenToClient(rcText);
	rcText.right = rcText.left - 10;
	rcText.left = 10;
	
	strText.LoadString(IDS_HANDDRAW);
	dc.DrawText(strText, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(pOldFont);

	return TRUE;
}

int CTimeLinePanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	rect.SetRect(0, 0, 0, 0);
	m_pTimeCtrl = new CTimeLineCtrl;
	m_pTimeCtrl->Create(WS_BORDER | WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, rect, this);

	m_pSliderWnd = new CSliderWnd;
	m_pSliderWnd->Create(WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_HORZ, rect, this, ID_SLIDER);
	m_pSliderWnd->SetRange(0, 0);
	m_pSliderWnd->SetChannelColor(RGB(128, 128, 196));

	m_pToolTip = new CToolTipWnd;
	m_pToolTip->Create(this);
	m_pToolTip->SetDrawFont(MACRO_SYS_FONT2);

	m_pBtnPrev = new CBmpBtn(IDB_PREV, BTN_PUSH_BUTTON);
	m_pBtnPrev->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PREV);

	m_pBtnNext = new CBmpBtn(IDB_NEXT, BTN_PUSH_BUTTON);
	m_pBtnNext->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_NEXT);

	m_pBtnPrevPage = new CBmpBtn(IDB_PREV_PAGE, BTN_PUSH_BUTTON);
	m_pBtnPrevPage->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PREV_PAGE);

	m_pBtnNextPage = new CBmpBtn(IDB_NEXT_PAGE, BTN_PUSH_BUTTON);
	m_pBtnNextPage->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_NEXT_PAGE);

	m_pBtnZoomin = new CBmpBtn(IDB_ZOOM_IN_SCALE, BTN_PUSH_BUTTON);
	m_pBtnZoomin->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_ZOOM_IN_SCALE);

	m_pBtnZoomout = new CBmpBtn(IDB_ZOOM_OUT_SCALE, BTN_PUSH_BUTTON);
	m_pBtnZoomout->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_ZOOM_OUT_SCALE);

	m_pToolPalete = new CToolPaleteVideoEdit;
	m_pToolPalete->Create(NULL, NULL, WS_CHILD, rect, this, NULL);

//	HRESULT	hval = CoCreateInstance(CLSID_EasyUtility, NULL, CLSCTX_INPROC_SERVER, 
//									IID_IEasyUtility, (void**)&m_pEasyUtility);
//	if (FAILED(hval))
//	{
//		W_SHOW_ERROR(IDS_ERROR_COM_EASY_UTIL, this);
//		m_pEasyUtility = NULL;
//	}
//	m_pEasyUtility->SetupProgressHandler((long)(MACRO_MAINFRM->m_hWnd));

//	hval = CoCreateInstance(CLSID_EasyConvert, NULL, CLSCTX_INPROC_SERVER, 
//							IID_IEasyConvert, (void**)&m_pConvert);
//	if (FAILED(hval))
//	{
//		SHOW_ERROR(IDS_ERROR_COM_EASY_CONVERT);
//		return -1;
//	}

	return 0;
}

void CTimeLinePanel::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pTimeCtrl->DestroyWindow();
	delete m_pTimeCtrl;

	m_pSliderWnd->DestroyWindow();
	delete m_pSliderWnd;

	m_pToolTip->DestroyWindow();
	delete m_pToolTip;

	m_pBtnPrev->DestroyWindow();
	delete m_pBtnPrev;

	m_pBtnNext->DestroyWindow();
	delete m_pBtnNext;

	m_pBtnPrevPage->DestroyWindow();
	delete m_pBtnPrevPage;

	m_pBtnNextPage->DestroyWindow();
	delete m_pBtnNextPage;

	m_pBtnZoomin->DestroyWindow();
	delete m_pBtnZoomin;

	m_pBtnZoomout->DestroyWindow();
	delete m_pBtnZoomout;

	m_pToolPalete->DestroyWindow();
	delete m_pToolPalete;

//	if (m_pEasyUtility != NULL) 
//	{
//		m_pEasyUtility->Release(); 
//		m_pEasyUtility = NULL;
//	}	

//	if (m_pConvert)
//	{
//		m_pConvert->Release();
//		m_pConvert = NULL;
//	}
}

void CTimeLinePanel::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	int nLeftGap;

#ifdef _JAPANESE
	nLeftGap = cLeftGapEn;
#else
	nLeftGap = cLeftGap;
#endif

#ifdef _EN
	nLeftGap = cLeftGapEn;
#endif

	CRect rcCtrl;
	rcCtrl.left = nLeftGap;
	rcCtrl.right = cx - cRightGap;
	rcCtrl.top = 5;
	rcCtrl.bottom = cy - cBottomGap;
	m_pTimeCtrl->MoveWindow(rcCtrl);

	CRect rcPalete = rcCtrl;
	rcPalete.left = rcCtrl.right + 3;
	rcPalete.right = cx - 5;
	m_pToolPalete->MoveWindow(rcPalete);

	CRect rcSlider = rcCtrl;
	rcSlider.left -= 5;
	rcSlider.right += 5;
	rcSlider.top = rcCtrl.bottom;
	rcSlider.bottom = rcSlider.top + 25;
	m_pSliderWnd->MoveWindow(rcSlider);

	CRect rcBtn;
	rcBtn.right = rcSlider.left - 5;
	rcBtn.left = rcBtn.right - m_csBtn.cx;
	int nGap = (m_csBtn.cy - rcSlider.Height()) / 2;
	rcBtn.top = rcSlider.top - nGap;
	rcBtn.bottom = rcBtn.top + m_csBtn.cy;
	m_pBtnPrev->MoveWindow(rcBtn);

	rcBtn.right = rcBtn.left - 5;
	rcBtn.left = rcBtn.right - m_csSmallBtn.cx;
	m_pBtnPrevPage->MoveWindow(rcBtn);

	rcBtn.left = rcSlider.right + 5;
	rcBtn.right = rcBtn.left + m_csBtn.cx;;
	m_pBtnNext->MoveWindow(rcBtn);

	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + m_csSmallBtn.cx;
	m_pBtnNextPage->MoveWindow(rcBtn);

	rcBtn.right = rcSlider.left - 5;
	rcBtn.left = rcBtn.right - m_csSmallBtn.cx;
	rcBtn.top = rcCtrl.top - 1;
	rcBtn.bottom = rcBtn.top + m_csSmallBtn.cy;
	m_pBtnZoomout->MoveWindow(rcBtn);

	rcBtn.right = rcBtn.left - 5;
	rcBtn.left = rcBtn.right - m_csSmallBtn.cx;
	m_pBtnZoomin->MoveWindow(rcBtn);
}

void CTimeLinePanel::Initialize()
{
	m_pTimeCtrl->SetTimeUnit(100);
	m_pTimeCtrl->SetPixelUnit(1);
	m_pTimeCtrl->Initialize();
}

void CTimeLinePanel::SetLength(long lDuration)
{
	m_lLength = lDuration;
	m_pTimeCtrl->SetLength(lDuration);

	m_pBtnZoomin->EnableButton(!m_pTimeCtrl->IsMinScale());

	SetSliderRange();
}

void CTimeLinePanel::SetSliderRange()
{
	if (m_lLength == 0)
		m_pSliderWnd->SetPos(0);

	CRect rcClient;
	m_pTimeCtrl->GetClientRect(rcClient);
	int nWidth = (int)(rcClient.Width() * 0.9);
	long lUnit = m_pTimeCtrl->GetTimeUnit();
	long lPixelUnit = m_pTimeCtrl->GetPixelUnit();
	long lWindowTime = nWidth * lUnit / lPixelUnit;
	long lOffsetTime = max(0, m_lLength - lWindowTime);
	long lScrollAmount = lOffsetTime / lUnit / cScrollUnit;
	m_pSliderWnd->SetRange(0, lScrollAmount);

	long lCurPos = m_pTimeCtrl->GetTimeStart() / lUnit / cScrollUnit;
	if (lCurPos > lScrollAmount)
		lCurPos = lScrollAmount;

	m_pSliderWnd->SetPos(lCurPos, TRUE);

	long lTime = (long)lCurPos * lUnit * cScrollUnit;
	m_pTimeCtrl->SetTimeStart(lTime);
}

void CTimeLinePanel::ShowToolTip(long lTime)
{
	CPoint pt;
	GetCursorPos(&pt);
	pt.x += 6;
	pt.y += 20;

	CString strTime = GetTimeStr(lTime);
	m_pToolTip->ShowText(pt, strTime);
}

CString CTimeLinePanel::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

LRESULT CTimeLinePanel::OnSliderHScroll(WPARAM wParam, LPARAM lParam) 
{
	int nID = m_pSliderWnd->GetDlgCtrlID();
	if (nID == ID_SLIDER)
	{
		int nPos = m_pSliderWnd->GetPos();
		switch (wParam)
		{
			case SB_THUMBTRACK:
			{	
				long lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->ShowTimeAt(lTime);
			}
			break;

			case SB_ENDSCROLL:
			{	
				m_pToolTip->ShowWindow(SW_HIDE);
			}
			break;

			case SB_THUMBPOSITION:
			{
				if (nPos < 0)
					return 0L;

				long lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->ShowTimeAt(lTime);
				m_pTimeCtrl->OnEventPosChanged();
				break;
			}
		}
	}

	return 0L;
}

LRESULT CTimeLinePanel::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;

	int nPos = m_pSliderWnd->GetPos();
	int nMin, nMax;
	m_pSliderWnd->GetRange(nMin, nMax);

	CRect rcClient;
	m_pTimeCtrl->GetClientRect(rcClient);
	int nWidth = (int)(rcClient.Width() * 0.9);
	switch (nID)
	{
		case ID_BTN_PREV:
		{
			if (nPos > 0)
			{	
				nPos--;
				m_pSliderWnd->SetPos(nPos);
				long lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->SetTimeStart(lTime);
				m_pTimeCtrl->ShiftWindow(10);
				m_pTimeCtrl->OnEventPosChanged();
			}
			break;
		}

		case ID_BTN_PREV_PAGE:
		{
			if (nPos > 0)
			{	
				int nPixelUnit = m_pTimeCtrl->GetPixelUnit();
				int nDelta = nWidth / cScrollUnit / nPixelUnit;
				nPos -= nDelta;
				if (nPos < 0)
					nPos = 0;
				m_pSliderWnd->SetPos(nPos);
				int lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->ShowTimeAt(lTime);
				m_pTimeCtrl->OnEventPosChanged();
			}
			break;
		}

		case ID_BTN_NEXT:
		{
			if (nPos < nMax)
			{
				nPos++;
				m_pSliderWnd->SetPos(nPos);
				long lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->SetTimeStart(lTime);
				m_pTimeCtrl->ShiftWindow(-10);
				m_pTimeCtrl->OnEventPosChanged();
			}
			break;
		}

		case ID_BTN_NEXT_PAGE:
		{
			if (nPos < nMax)
			{
				int nPixelUnit = m_pTimeCtrl->GetPixelUnit();
				int nDelta = nWidth / cScrollUnit / nPixelUnit;
				nPos += nDelta;
				if (nPos > nMax)
					nPos = nMax;
				m_pSliderWnd->SetPos(nPos);
				long lUnit = m_pTimeCtrl->GetTimeUnit();
				long lTime = (long)nPos * lUnit * cScrollUnit;
				m_pTimeCtrl->ShowTimeAt(lTime);
				m_pTimeCtrl->OnEventPosChanged();
			}
			break;
		}

		case ID_ZOOM_IN_SCALE:
		{
			m_pBtnZoomout->EnableButton(TRUE);

			long nPos; 
			long lPointerTime;
			if (m_pTimeCtrl->IsPointerVisible())
			{
				lPointerTime = m_pTimeCtrl->GetPointerTime();
				nPos = m_pTimeCtrl->TimeToPos(lPointerTime);
			}
			else
			{
				nPos = rcClient.Width() / 2;
				lPointerTime = m_pTimeCtrl->PosToTime(nPos);
			}

			int nUnit, nPixelUnit;
			m_pTimeCtrl->GetZoomIn(nUnit, nPixelUnit);
			if (nPixelUnit == 4)
				m_pBtnZoomin->EnableButton(FALSE);
			m_pTimeCtrl->SetTimeUnit(nUnit);
			m_pTimeCtrl->SetPixelUnit(nPixelUnit);

			long lOffset = m_pTimeCtrl->GetTimeUnit() * nPos / m_pTimeCtrl->GetPixelUnit();
			long lNewStartTime = max(0, lPointerTime - lOffset);
			m_pTimeCtrl->SetTimeStart(lNewStartTime);
			
			SetSliderRange();
			m_pTimeCtrl->RedrawWindow();
			m_pTimeCtrl->SetPointerPos();
			m_pTimeCtrl->OnEventPosChanged();
			break;
		}

		case ID_ZOOM_OUT_SCALE:
		{	
			m_pBtnZoomin->EnableButton(TRUE);

			long nPos; 
			long lPointerTime;
			if (m_pTimeCtrl->IsPointerVisible())
			{
				lPointerTime = m_pTimeCtrl->GetPointerTime();
				nPos = m_pTimeCtrl->TimeToPos(lPointerTime);
			}
			else
			{
				nPos = rcClient.Width() / 2;
				lPointerTime = m_pTimeCtrl->PosToTime(nPos);
			}

			int nUnit, nPixelUnit;
			m_pTimeCtrl->GetZoomOut(nUnit, nPixelUnit);
			if (nUnit == 20000)
				m_pBtnZoomout->EnableButton(FALSE);
			m_pTimeCtrl->SetTimeUnit(nUnit);
			m_pTimeCtrl->SetPixelUnit(nPixelUnit);

			long lOffset = m_pTimeCtrl->GetTimeUnit() * nPos / m_pTimeCtrl->GetPixelUnit();
			long lNewStartTime = max(0, lPointerTime - lOffset);
			m_pTimeCtrl->SetTimeStart(lNewStartTime);

			SetSliderRange();
			m_pTimeCtrl->RedrawWindow();
			m_pTimeCtrl->SetPointerPos();
			m_pTimeCtrl->OnEventPosChanged();
			break;
		}
	}

	return 0L;
}

LRESULT CTimeLinePanel::OnShiftRight(WPARAM wParam, LPARAM lParam)
{
	int nPos = m_pSliderWnd->GetPos();
	int nMin, nMax;
	m_pSliderWnd->GetRange(nMin, nMax);
	if (nPos > 0)
	{	
		nPos--;
		m_pSliderWnd->SetPos(nPos);
	}

	return 0L;
}

LRESULT CTimeLinePanel::OnShiftLeft(WPARAM wParam, LPARAM lParam)
{
	int nPos = m_pSliderWnd->GetPos();
	int nMin, nMax;
	m_pSliderWnd->GetRange(nMin, nMax);
	if (nPos < nMax)
	{
		nPos++;
		m_pSliderWnd->SetPos(nPos);
	}

	return 0L;
}

void CTimeLinePanel::SetKeyFrame(CStringArray& strMarkArray, CStringArray& strMarkTimeArray, 
								 CStringArray& strScriptArray, CStringArray& strCmdArray, 
								 CStringArray& strScriptTimeArray)
{
	m_pTimeCtrl->SetKeyFrame(strMarkArray, strMarkTimeArray, strScriptArray, strCmdArray, strScriptTimeArray);
}

void CTimeLinePanel::RemoveAllKeyFrame()
{
	m_pTimeCtrl->RemoveAllKeyFrame();
}

LRESULT CTimeLinePanel::OnTimePosChanged(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_TIME_POS_CHANGED, wParam, lParam);
	return 0L;
}

void CTimeLinePanel::SetPointerTimeAndShow(long lTime)
{
	m_pTimeCtrl->SetPointerTimeAndShow(lTime);
}

LRESULT CTimeLinePanel::OnChangeSlider(WPARAM wParam, LPARAM lParam)
{
	long lUnit = m_pTimeCtrl->GetTimeUnit();
	long nPos = (long)wParam / lUnit / cScrollUnit;
	m_pSliderWnd->SetPos(nPos);

	return 0L;
}

void CTimeLinePanel::GetTagFromUI(CStringArray& strArrayTimeMarker, 
								  CStringArray& strArrayMarker, 
								  CStringArray& strArrayTimeScript,
								  CStringArray& strArrayScript,
								  CStringArray& strArrayCmd)
{
	m_pTimeCtrl->GetTagFromUI(strArrayTimeMarker, strArrayMarker, strArrayTimeScript,
							  strArrayScript, strArrayCmd);
}

LRESULT CTimeLinePanel::OnKeyMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_KEY_FRAME_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnKeyTobeMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_KEY_FRAME_TOBE_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnRegenTag(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_REGEN_TAG, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnKeyPressing(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_KEY_FRAME_PRESSING, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPaleteBtn(WPARAM wParam, LPARAM lParam)
{
	long lID = wParam;

	GetParent()->SendMessage(WM_PALETTE_BTN_UP, wParam, lParam);   //Pause the video
/*
	switch (lID)
	{
		case ID_BTN_MARK:
			m_pTimeCtrl->SetMark();
		break;

		case ID_BTN_DELETE:
			if (m_pTimeCtrl->DeletePeriod())
				GetParent()->SendMessage(WM_MODIFY_VIDEO);
		break;

		case ID_BTN_MUTE:
		{
			long lTimeSelStart, lTimeSelEnd;
			m_pTimeCtrl->GetSelRange(lTimeSelStart, lTimeSelEnd);
			if (lTimeSelStart < 0)
			{
				W_SHOW_WARNING(IDS_NOT_MARK_COMPLETE_FOR_MUTE, this);
				return 0L;
			}

			GetParent()->PostMessage(WM_MUTE_PERIOD, lTimeSelStart, lTimeSelEnd);
		}
		break;

		case ID_BTN_REPLACE:
			ReplacePeriod();
		break;

		case ID_BTN_EDITMARK:
			EditMark(-1, -1);
		break;

		case ID_BTN_ADD_EVENT:
		{
			long lTime = m_pTimeCtrl->GetPointerTime();
			GetParent()->PostMessage(WM_ADD_EVENT, lTime, lParam);
		}
		break;

		case ID_BTN_SAVE_VIDEO:
			GetParent()->PostMessage(WM_SAVE_VIDEO);
		break;

		case ID_BTN_ADD_WAVE:
		{
			long lTime = m_pTimeCtrl->GetPointerTime();
			GetParent()->PostMessage(WM_ADD_WAVE, lTime, lParam);
			break;
		}

		case ID_BTN_ADD_TEXT:
		{
			long lTime = m_pTimeCtrl->GetPointerTime();
			GetParent()->PostMessage(WM_ADD_TEXT, lTime, lParam);
			break;
		}

		case ID_BTN_ADD_EFFECT:
		{
			long lTime = m_pTimeCtrl->GetPointerTime();
			GetParent()->PostMessage(WM_ADD_EFFECT, lTime, lParam);
			break;
		}

		case ID_BTN_SET_POINTS:
		{
			GetParent()->PostMessage(WM_SET_PPT_POINT, wParam, lParam);
			break;
		}
	}
*/
	return 0L;
}

int CTimeLinePanel::GetCurrentInsTmpIndex()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return -1;

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\video";

	CStringArray strArray;
	::GetFiles(strPath, "mp4", strArray);
	int nIndex = 0;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		str = str.Left(str.GetLength() - 4);
		if (str.Left(7).CompareNoCase("ins_tmp") == 0)
		{
			int n = atoi(str.Right(str.GetLength() - 7));
			if (n > nIndex)
				nIndex = n;
		}
	}

	return nIndex;
}

BOOL CTimeLinePanel::GetVideo(CString strCourse, CString& strVideoPath)
{
	CDlgSelVideoSrc dlg;
	if (dlg.DoModal() == IDCANCEL)
		return FALSE;
	MACRO_MAINFRM->UpdateWindow();	//add 2013/11/21 kagata BugNo.90

	if (dlg.m_bCaptureNow)
	{
		CString strSysIniPath = MACRO_INI_PATH;
	    char szBuf[256];
	    ::GetPrivateProfileString("Profile", "Param1", "", szBuf, 256, strSysIniPath);
		int nVideoFreq = atoi(szBuf);
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
	//	CString strDestASF = MACRO_COURSE_PATH + strCourse + "\\video\\default.mp4";
		PROFILEPARAMS param;
		CString strProfile = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
        MACRO_MAINFRM->GetPRXParams(strProfile, param);

//wangwang reload the devices
		CStringArray strArrayWebAudio, strArrayWebVideo;;
		
		//CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
		//::EnumDevice(m_pEasyCapture, eType, strArrayWebAudio);
		
		CAPTUREDEVICETYPE eType;
		HRESULT hval = 0;
		hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&m_pEasyCapture);
		if (hval != S_OK) 
		{
			MessageBox("Error Open: EasyCapture Fail");
			return FALSE;
		}

		hval = m_pEasyCapture->Open(0);
		if((hval != S_OK)) 
		{
			MessageBox("Error Open: EasyCapture Open Fail");
			return FALSE;
		}
		eType = CAPTUREDEVICETYPE_VIDEO;
		::EnumDevice(m_pEasyCapture, eType, strArrayWebVideo);

	    int WithCam = strArrayWebVideo.GetSize();
	    //m_bWithAudio = strArrayWebAudio.GetSize();

		if (szBuf[0] == '\0')//ini without video
		{
		 if (param.vinfo.dwBitrate!=0)
		 {
			 MACRO_VIEW->Stop();
             W_SHOW_ERROR(IDS_COURSE_WITHVIDEO, this);
			 CDlgSelDevice dlg;
			 dlg.DoModal();
           //MACRO_VIEW->SendMessage(ID_DEV_SETTING);
		 }

		}
		else //ini with video
		{
          if (param.vinfo.dwBitrate==0)
		  {
		     MACRO_VIEW->Stop();
			 W_SHOW_ERROR(IDS_COURSE_WITHOUTVIDEO, this);
			 CDlgSelDevice dlg;
			 dlg.DoModal();
		  }
		  else
		  {
			  if (WithCam <=0)
			  {

                W_SHOW_ERROR(IDS_INSERT_VIDEO_DEVICE, this);
				return false;
			  }
      
		  }  
		}
		int nIndex = GetCurrentInsTmpIndex();
		strVideoPath.Format("%s\\video\\ins_tmp%d.mp4", MACRO_COURSE_PATH + strCourse, nIndex + 1);
		RecordASFFile(strVideoPath);
		if (!FileExists(strVideoPath))
				return false;
	}

	else
	{
		strVideoPath = dlg.m_strPath;
    }

	//add 2013/11/05 kagata BugNo.23
	W_SHOW_MESSAGE(IDS_NEED_CONVERT, this);
	MACRO_MAINFRM->UpdateWindow();			//add 2013/11/21 kagata BugNo.90
	//wangwang chnaged here

		CString strSrc = MACRO_MP4_FILE(strCourse);
	//wangwang	if (!MACRO_MAINFRM->IsSameProfile(strSrc, strVideoPath))
		//{
			

			int nIndex = GetCurrentInsTmpIndex();
			CString strDest = strVideoPath;
			strDest.Format("%s\\video\\ins_tmp%d.mp4", MACRO_COURSE_PATH + strCourse, nIndex + 1);
		   
			CString strProfilePath = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
			//MACRO_MAINFRM->ConvertToSameProfile(strVideoPath, strDest, strProfilePath);
			//W_SHOW_MESSAGE(IDS_NEED_CONVERT, this);	//del 2013/11/05 kagata BugNo.23

			if (strVideoPath.Find("mp4") <= 0)
			{
				if (!MACRO_MAINFRM->ConvertFile(strVideoPath, strDest, 20, 128, TRUE))
				{
					CString strTmp, strMsg;
					strTmp.LoadString(IDS_CONVERT_ERROR);
					strMsg.Format(strTmp, strVideoPath, strDest);
					W_SHOW_ERROR_STR(strMsg, this);
					return FALSE;
				}
			}
			else
			{
				::SafeCopyFile(strVideoPath, strDest);
			}

			MACRO_MAINFRM->UpdateWindow();

			if (!MACRO_MAINFRM->CreateIndex(strDest))
			{
				W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
				return FALSE;
			}

			strVideoPath = strDest;
		//}
		//else
		//{
			/* wangwang begin
			int nIndex = GetCurrentInsTmpIndex();
			strVideoPath.Format("%s\\video\\ins_tmp%d.mp4", MACRO_COURSE_PATH + strCourse, nIndex + 1);
			if (!::SafeCopyFile(dlg.m_strPath, strVideoPath, FALSE))
				strVideoPath = dlg.m_strPath;
			wangwang end	*/
		//}
	//}

	return TRUE;
}

void CTimeLinePanel::RecordASFFile(CString strPath)
{
	CString strExePath = ::GetSysPath() + "bin\\RecordMP4.exe ";
	CString strParam = "/s \"" + strPath + "\"";
/*
	if (::WinExec(strExePath + strParam, SW_SHOWNORMAL) < 31)
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CANNOT_CAPTURE_EXE);
		strMsg.Format(strTmp, strExePath); 
		W_SHOW_ERROR_STR(strMsg, this);
		return;
	}
	else
	{
		MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
	}
*/
	  	    STARTUPINFO si;

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &m_pi, sizeof(m_pi) );
			//DWORD dwExit;
			strExePath=strExePath+strParam;
			if( !::CreateProcess(NULL, // No module name (use command line). 
				(LPSTR)(LPCSTR)strExePath,             // Command line. 
				NULL,             // Process handle not inheritable. 
				NULL,             // Thread handle not inheritable. 
				FALSE,            // Set handle inheritance to FALSE. 
				0,                // No creation flags. 
				NULL,             // Use parent's environment block. 
				NULL,             // Use parent's starting directory. 
				&si,              // Pointer to STARTUPINFO structure.
				&m_pi )             // Pointer to PROCESS_INFORMATION structure.
			) 
			{	
			  CloseHandle( m_pi.hProcess );
			  CloseHandle( m_pi.hThread );
			}
			else
				MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
			MACRO_MAINFRM->ShowWindow(SW_SHOWMINNOACTIVE);



            WaitForSingleObject(m_pi.hProcess, INFINITE);
            
			CloseHandle( m_pi.hProcess );
		    CloseHandle( m_pi.hThread );


			    	

			/*
			while (GetExitCodeProcess(m_pi.hProcess, &dwExit) == STILL_ACTIVE)
			{
				Sleep(100);
			}
			*/
}

void CTimeLinePanel::ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd)
{
	//long lTimeSelStart, lTimeSelEnd;
	//m_pTimeCtrl->GetReplacePeriod(lTimeSelStart, lTimeSelEnd);
	//if (lTimeSelStart < 0)
	//	return;
	//if (lTimeSelStart != lTimeStart || lTimeSelEnd != lTimeEnd)  //double check 
	//	return;

	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	long lDuration = MACRO_MAINFRM->GetMediaLength(strVideoPath);
	//add by buxiangyi
	CStringArray m_oldarray, array;
	CString m_time, strPath;
	m_time.Format("%d",lDuration/1000);
	m_oldarray.Add("var vduration =" );
	array.Add("var vduration = " + m_time + ";");
	strPath = strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

	CString strFileName;
	CString strTmp = MACRO_COURSE_PATH + strCourse + "\\video\\";
	if (strVideoPath.Left(strTmp.GetLength()).CompareNoCase(strTmp) == 0)
	{
		strVideoPath = strVideoPath.Mid(strTmp.GetLength());
	}
	
	if (m_pTimeCtrl->ReplacePeriod(lTimeStart, lTimeEnd, strVideoPath, lDuration))
	{
		GetParent()->SendMessage(WM_MODIFY_VIDEO);
		m_pTimeCtrl->RedrawWindow();
	}
}

BOOL CTimeLinePanel::ReplacePeriod()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	long lTimeStart, lTimeEnd;
	m_pTimeCtrl->GetReplacePeriod(lTimeStart, lTimeEnd);
	//if (lTimeStart < 0)
	//{
	//	W_SHOW_WARNING(IDS_NOT_MARK_FOR_REPLACE, this);
	//	return;
	//}

	if (lTimeStart < 0)
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_SURE_TO_INSERT_AT_CURSOR);

		long lPointerTime = m_pTimeCtrl->GetPointerTime();
		CString strTime = ::ConvertTimeToStr(lPointerTime);

		strMsg.Format(strTmp, strTime);
		
		if (W_ASK_YESNO_STR(strMsg, this) == IDNO)
			return FALSE;

		lTimeStart = lPointerTime;
	}

	CString strVideoPath;
	if (GetVideo(strCourse, strVideoPath))
	{
		if (::FileExists(strVideoPath))
		{
			long lDuration = MACRO_MAINFRM->GetMediaLength(strVideoPath);
			//add by buxiangyi
			CStringArray m_oldarray, array;
			CString m_time, strPath;
			m_time.Format("%d",lDuration/1000);
			m_oldarray.Add("var vduration =" );
			array.Add("var vduration = " + m_time + ";");
			strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
			UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

			CString strFileName;
			CString strTmp = MACRO_COURSE_PATH + strCourse + "\\video\\";
			if (strVideoPath.Left(strTmp.GetLength()).CompareNoCase(strTmp) == 0)
			{
				strVideoPath = strVideoPath.Mid(strTmp.GetLength());
			}

			if (m_pTimeCtrl->ReplacePeriod(lTimeStart, lTimeEnd, strVideoPath, lDuration))
			{
				GetParent()->SendMessage(WM_MODIFY_VIDEO);
				return TRUE;
			}
		}
		else    //UnRecorded
		{
			CString strPeriod;
			strPeriod.Format("%ld %ld", lTimeStart, lTimeEnd);
			GetParent()->SendMessage(WM_WAIT_FOR_FILE, 
								    (WPARAM)(const char*)strPeriod, (LPARAM)(const char*)strVideoPath);
			return TRUE;   //???
		}
	}
	return FALSE;
}

BOOL CTimeLinePanel::GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName)
{
	return m_pTimeCtrl->GetVideoPosAtTime(lTime, lVideoPos, strFileName);
}

BOOL CTimeLinePanel::GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName)
{
	return m_pTimeCtrl->GetVideoPosNextToTime(lTime, lVideoPos, strFileName);
}

BOOL CTimeLinePanel::GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName)
{
	return m_pTimeCtrl->GetTimeByVideoPos(lTime, lVideoPos, strFileName);
}

LRESULT CTimeLinePanel::OnSetLength(WPARAM wParam, LPARAM lParam)
{
	SetLength(wParam);
	m_pTimeCtrl->RedrawWindow();

	GetParent()->SendMessage(WM_SET_LENGTH, wParam);

	return 0L;
}

void CTimeLinePanel::GetVideoSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath)
{
	m_pTimeCtrl->GetVideoSegString(str, strDefault, bConvDefault, strSubPath);
}

void CTimeLinePanel::GetAudioReplaceSegString(CString& str)
{
	m_pTimeCtrl->GetAudioReplaceSegString(str);
}

void CTimeLinePanel::GetAudioMixSegString(CString& str)
{
	m_pTimeCtrl->GetAudioMixSegString(str);
}

void CTimeLinePanel::GetVideoTextSegString(CString& str)
{
	m_pTimeCtrl->GetVideoTextSegString(str);
}

void CTimeLinePanel::GetVideoEffectSegString(CString& str)
{
	m_pTimeCtrl->GetVideoEffectSegString(str);
}

BOOL CTimeLinePanel::EditMark(int nPrePage, int nPreCell)
{
	CDlgEditMark dlg;
	dlg.m_nPrePage = nPrePage;
	dlg.m_nPreCell = nPreCell;
	CStringArray strArrayTime, strArrayMarker, strArrayScript, strArrayCmd;
	m_pTimeCtrl->GetTagFromUI(strArrayTime, strArrayMarker, strArrayScript, strArrayCmd);
	
	CStringArray strArrayOrigMark, strArrayOrigStep;
	CStringArray strArrayMark, strArrayStep;
	int i = 0;
	for (i = 0; i < strArrayTime.GetSize(); i++)
	{
		CString strTime = strArrayTime[i];
		CString strCmd = strArrayCmd[i];
		CString str = strArrayScript[i];
		str.TrimLeft();
		str.TrimRight();
		strArrayOrigStep.Add(strTime + " " + strCmd + " " + str);

		str = strArrayMarker[i];
		str.TrimLeft();
		str.TrimRight();
		if (str != "")
			strArrayOrigMark.Add(strTime + " " + str);
	}
	
	dlg.m_strTimeMax = ::ConvertTimeToStr(m_lLength);
	dlg.SetArrayPtr(&strArrayTime, &strArrayMarker, &strArrayScript, &strArrayCmd);
	if (dlg.DoModal() == IDCANCEL)
		return FALSE;

	MACRO_MAINFRM->UpdateWindow();

	for (i = 0; i < strArrayMarker.GetSize(); i++)
	{
		CString strTime = strArrayTime[i];
		CString str = strArrayMarker[i];
		str.TrimLeft();
		if (str != "")
			strArrayMark.Add(strArrayTime[i] + " " + str);
	}

	for (i = 0; i < strArrayScript.GetSize(); i++)
	{
		CString strTime = strArrayTime[i];
		CString str = strArrayScript[i];
		str.TrimLeft();
		CString strCmd = strArrayCmd[i];
		strArrayStep.Add(strTime + " " + strCmd + " " + str);
	}

	if (IsTagChanged(strArrayMark, strArrayStep, strArrayOrigMark, strArrayOrigStep))
	{
		m_pTimeCtrl->SetKeyFrame(strArrayTime, strArrayMarker, strArrayScript, strArrayCmd);
		m_pTimeCtrl->RedrawWindow();
		GetParent()->SendMessage(WM_REGEN_TAG);
	}

	m_pTimeCtrl->RedrawWindow();

	return TRUE;
}

void CTimeLinePanel::CutAndConv(CStringArray& strArrayInput, 
				 		       CStringArray& strArrayOutputTime,
							   CStringArray& strArrayOutputValue)
{
	for (int i = 0; i < strArrayInput.GetSize(); i++)
	{
		CString strInput = strArrayInput[i];
		int nFind = strInput.Find(' ');
		if (nFind >= 0)
		{
			CString strTime = strInput.Left(nFind);
			CString strValue = strInput.Mid(nFind + 1);
			strArrayOutputValue.Add(strValue);
			strArrayOutputTime.Add(strTime);
		}
	}
}

BOOL CTimeLinePanel::IsArrayDifferent(CStringArray& strArrayInputTime1, 
				 					 CStringArray& strArrayInputValue1,
							         CStringArray& strArrayInputTime2, 
				 					 CStringArray& strArrayInputValue2)
{
	for (int i = 0; i < strArrayInputValue1.GetSize(); i++)
	{
		CString strValue = strArrayInputValue1[i];
		CString strTime = strArrayInputTime1[i];
		strValue.TrimLeft();
		strValue.TrimRight();
		strTime.TrimLeft();
		strTime.TrimRight();
		BOOL bFound = FALSE;
		for (int j = 0; j < strArrayInputValue2.GetSize(); j++)
		{
			CString strValue2 = strArrayInputValue2[j];
			CString strTime2 = strArrayInputTime2[j];
			strValue2.TrimLeft();
			strValue2.TrimRight();
			strTime2.TrimLeft();
			strTime2.TrimRight();
			if (strValue == strValue2 && strTime == strTime2)
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound)
			return TRUE;
	}

	return FALSE;
}

BOOL CTimeLinePanel::IsTagChanged(CStringArray& strArrayOutline, 
				 		         CStringArray& strArrayStep,
								 CStringArray& strArrayOldOutline, 
				 		         CStringArray& strArrayOldStep)
{
	CStringArray strArrayMarkers, strArrayMarkTime;
	CStringArray strArrayCmd, strArrayCmdTime;

	CStringArray strArrayOrigMarkers, strArrayOrigMarkTime;
	CStringArray strArrayOrigCmd, strArrayOrigCmdTime;

	CutAndConv(strArrayOutline, strArrayMarkTime, strArrayMarkers);
	CutAndConv(strArrayStep, strArrayCmdTime, strArrayCmd);
	CutAndConv(strArrayOldOutline, strArrayOrigMarkTime, strArrayOrigMarkers);
	CutAndConv(strArrayOldStep, strArrayOrigCmdTime, strArrayOrigCmd);

	if (strArrayCmd.GetSize() != strArrayOrigCmd.GetSize())
		return TRUE;

	if (strArrayMarkers.GetSize() != strArrayOrigMarkers.GetSize())
		return TRUE;

	if (IsArrayDifferent(strArrayOrigMarkTime, strArrayOrigMarkers,
						 strArrayMarkTime, strArrayMarkers))
		return TRUE;

	if (IsArrayDifferent(strArrayOrigCmdTime, strArrayOrigCmd,
						 strArrayCmdTime, strArrayCmd))
		return TRUE;

	return FALSE;
}

LRESULT CTimeLinePanel::OnCPAction(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PAUSE_VIDEOPLAY, wParam, lParam);   //Pause the video

	CObArray* pObj = new CObArray;
	GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj, 0); 

	BOOL bResult = TRUE;
	if (lParam == ID_EDIT_CP)
		bResult = OnEditCP(wParam); 
	else if (lParam == ID_DELETE_CP)
		OnDeleteCP(wParam); 
	else if (lParam == ID_DELETE_ALL_CP)
		OnDeleteAllCP(); 
	else if (lParam == ID_DEMO_CAPTURE)
		bResult = OnCaptureCP(wParam); 
	else if (lParam == ID_IMPORT_DEMO_FILE)
		OnImportDemoFile(wParam); 
	else if (lParam == ID_VIEW_OBJ)
		bResult = OnViewCP(wParam);

	if (bResult)
	{
		CObArray* pObj2 = new CObArray;
		GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj2, (LPARAM)pObj); 
		MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(GetParent(), "video_op", pObj, pObj2);
	}
	else
	{
		::ReleaseObArray(pObj);
		delete pObj;
	}

	return 0L;
}

BOOL CTimeLinePanel::OnEditCP(int nIndex) 
{
	long lTime;
	CString strTitle, strCmd, strStep; 
	m_pTimeCtrl->GetFrameInfo(nIndex, strTitle, lTime, strCmd, strStep);
	CString strTime = ::ConvertTimeToStr(lTime);

	int nPrePage = 0;
	int nPreCell = -1;
	if (strCmd == "zoomin" || strCmd == "zoomout")
		nPrePage = 1;
	else if (strCmd == "demo")
		nPrePage = 2;
	else if (strCmd == "drawline" || strCmd == "drawrect" || strCmd == "drawmark" || strCmd == "freehand")
		nPrePage = 3;

	int nSCount = 0;
	int nZCount = 0;
	int nDCount = 0;
	int nTCount = 0;
	for (int i = 0; i < (int)nIndex; i++)
	{
		long lTime2;
		CString strTitle2, strCmd2, strStep2; 
		m_pTimeCtrl->GetFrameInfo(i, strTitle2, lTime2, strCmd2, strStep2);
		if (strCmd2 == "zoomin" || strCmd2 == "zoomout")
			nZCount++;
		else if (strCmd2 == "demo")
			nDCount++;
		else if (strCmd2 == "drawline" || strCmd2 == "drawrect" || strCmd2 == "drawmark" || strCmd2 == "freehand")
			nTCount++;
		else
			nSCount++;
	}

	if (nPrePage == 0)
		nPreCell = nSCount;
	else if (nPrePage == 1)
		nPreCell = nZCount;
	else if (nPrePage == 2)
		nPreCell = nDCount;
	else if (nPrePage == 3)
		nPreCell = nTCount;

	return EditMark(nPrePage, nPreCell);
}

void CTimeLinePanel::OnDeleteCP(int nIndex) 
{
	m_pTimeCtrl->RemoveFrameObj(nIndex);
}

void CTimeLinePanel::OnDeleteAllCP() 
{
	m_pTimeCtrl->RemoveFrameObj(-1);
}

BOOL CTimeLinePanel::OnCaptureCP(int nIndex) 
{
	long lTime;
	CString strTitle, strCmd, strStep; 
	m_pTimeCtrl->GetFrameInfo(nIndex, strTitle, lTime, strCmd, strStep);
	if (strCmd != "demo")
		return FALSE;

	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\movie\\demo" + strStep + ".mp4";
	if (::FileExists(strPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_DEMO_EXIST);
		strMsg.Format(strTmp, strStep);
		if (W_ASK_YESNO_STR(strMsg, this) == IDNO)
			return FALSE;
		::DeleteFile(strPath);
	}

	CString strExePath;
	/*
	if (EV_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_eval.exe /screen ";
	else if (PROMOTE_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_promote.exe /screen ";
	else
		strExePath = ::GetSysPath() + "bin\\MCGCapture.exe /screen ";
	*/
	if (EV_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_eval.exe /screen ";
	else if (PROMOTE_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_promote.exe /screen ";
	else if (BUNDLE_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_bundle.exe /screen ";
	else
		strExePath = ::GetSysPath() + "bin\\MCGCapture.exe /screen ";

	//CString strParam = "\"" + strPath + "\"";
	CString strParam = "\"" + strPath + "\" /profilepath_xxx \"" + MACRO_INI_FILE(strCourse) + "\"";
	if (::WinExec(strExePath + strParam, SW_SHOWNORMAL) < 31)
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CANNOT_EXEC_EXE);
		strMsg.Format(strTmp, strExePath); 
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}
	else
	{
		MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
	}

	return TRUE;
}

BOOL CTimeLinePanel::OnImportDemoFile(int nIndex) 
{
	long lTime;
	CString strTitle, strCmd, strStep; 
	m_pTimeCtrl->GetFrameInfo(nIndex, strTitle, lTime, strCmd, strStep);
	if (strCmd != "demo")
		return FALSE;

	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\movie\\demo" + strStep + ".mp4";
	if (::FileExists(strPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_DEMO_EXIST);
		strMsg.Format(strTmp, strStep);
		if (W_ASK_YESNO_STR(strMsg, this) == IDNO)
			return FALSE;
	}

	CString StrTemp;
	StrTemp.LoadString(IDS_VIDEO_FILE);//IDS_MEDIA_FILE);
 	CFileDialog* pDlg = new CFileDialog(TRUE, "mp4", NULL, 
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);
	if (pDlg->DoModal() != IDOK)
	{
		delete pDlg;
		return FALSE;
	}
	
	CString strSrcPath = pDlg->GetPathName();
	delete pDlg;

	::SafeCopyFile(strSrcPath, strPath);

	return TRUE;
}

BOOL CTimeLinePanel::OnViewCP(int nIndex) 
{
	long lTime;
	CString strTitle, strCmd, strStep; 
	m_pTimeCtrl->GetFrameInfo(nIndex, strTitle, lTime, strCmd, strStep);
	if (strCmd != "demo")
		return FALSE;

	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	if (strCmd == "demo")
	{
		CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\movie\\demo" + strStep + ".mp4";
		if (!::FileExists(strPath))
			return FALSE;

		::ShellExecute(m_hWnd, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
	}

	return TRUE;
}

void CTimeLinePanel::SetVideoSegArray(CStringArray& strArrayVideo)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	m_pTimeCtrl->SetVideoSegArray(strArrayVideo, MACRO_COURSE_PATH + strCourse + "\\video\\");
}

void CTimeLinePanel::SetAudioReplaceSegArray(CStringArray& strArrayVideo)
{
	m_pTimeCtrl->SetAudioReplaceSegArray(strArrayVideo);
	for (int i = 0; i < strArrayVideo.GetSize(); i++)
	{
		long lTimeStart, lTimeEnd;
		int nType;
		CString strPath;
		m_pTimeCtrl->GetAudioReplaceSegInfo(i, lTimeStart, lTimeEnd, strPath, nType);
		
		CString strFile = ::GetFileName(strPath);
		if (strFile.Left(8) == "mute_tmp")
			m_pTimeCtrl->SetAudioReplaceSegColor(i, RGB(255, 0, 0), RGB(255, 255, 0));	
	}
}

void CTimeLinePanel::SetAudioMixSegArray(CStringArray& strArrayVideo)
{
	m_pTimeCtrl->SetAudioMixSegArray(strArrayVideo);
}

void CTimeLinePanel::SetVideoTextSegArray(CStringArray& strArrayVideo)
{
	m_pTimeCtrl->SetVideoTextSegArray(strArrayVideo);
}

void CTimeLinePanel::SetVideoEffectSegArray(CStringArray& strArrayVideo)
{
	m_pTimeCtrl->SetVideoEffectSegArray(strArrayVideo);
}

void CTimeLinePanel::SetNewSegArray()
{
	m_pTimeCtrl->SetNewSegArray();
}

BOOL CTimeLinePanel::AddAudioMix(long lTime, long lTimeEnd, CString strDestPath)
{
	return m_pTimeCtrl->AddAudioMix(lTime, lTimeEnd, strDestPath);
}

BOOL CTimeLinePanel::AddAudioReplace(long lTime, long lTimeEnd, CString strDestPath)
{
	CString strFileName = ::GetFileName(strDestPath);
	COLORREF crNormal, crHot;
	if (strFileName.Left(8) == "mute_tmp")
	{
		crNormal = RGB(255, 0, 0);
		crHot = RGB(255, 255, 0);
	}
	else
	{
		crNormal = RGB(0, 0, 192);
		crHot = RGB(255, 255, 0);
	}

	BOOL b = m_pTimeCtrl->AddAudioReplace(lTime, lTimeEnd, strDestPath, crNormal, crHot);
	if (b)
		m_pTimeCtrl->SetNoSelectAndRepaint();
	return b;
}

BOOL CTimeLinePanel::AddVideoText(long lTime, long lTimeEnd, CString strDestPath, CString strTip)
{
	return m_pTimeCtrl->AddVideoText(lTime, lTimeEnd, strDestPath, strTip);
}

BOOL CTimeLinePanel::AddVideoEffect(long lTime, long lDuration, int nType, CString strDestPath)
{
	return m_pTimeCtrl->AddVideoEffect(lTime, lDuration, nType, strDestPath);
}

LRESULT CTimeLinePanel::OnPeriodMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_TOBE_MOVED, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodMoving(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_PERIOD_MOVING, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodDelete(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_DELETE, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodView(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_VIEW, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodEdit(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_PERIOD_EDIT, wParam, lParam);
	return 0L;
}

void CTimeLinePanel::DeleteAudioReplaceByIndex(int nIndex)
{
	m_pTimeCtrl->DeleteAudioReplaceByIndex(nIndex);
}

void CTimeLinePanel::DeleteAudioMixByIndex(int nIndex)
{
	m_pTimeCtrl->DeleteAudioMixByIndex(nIndex);
}

void CTimeLinePanel::DeleteVideoTextByIndex(int nIndex)
{
	m_pTimeCtrl->DeleteVideoTextByIndex(nIndex);
}

void CTimeLinePanel::DeleteVideoEffectByIndex(int nIndex)
{
	m_pTimeCtrl->DeleteVideoEffectByIndex(nIndex);
}

void CTimeLinePanel::GetAudioReplaceSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
										   CString& strPath, int& nType)
{
	m_pTimeCtrl->GetAudioReplaceSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLinePanel::GetAudioMixSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
									   CString& strPath, int& nType)
{
	m_pTimeCtrl->GetAudioMixSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLinePanel::GetVideoTextSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
								 	    CString& strPath, int& nType)
{
	m_pTimeCtrl->GetVideoTextSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

void CTimeLinePanel::GetVideoEffectSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, 
									      CString& strPath, int& nType)
{
	m_pTimeCtrl->GetVideoEffectSegInfo(nIndex, lTimeStart, lTimeEnd, strPath, nType);
}

BOOL CTimeLinePanel::IsVideoTextOverlap(long lTime, long lTimeEnd)
{
	return m_pTimeCtrl->IsVideoTextOverlap(lTime, lTimeEnd);
}

BOOL CTimeLinePanel::IsVideoEffectOverlap(long lTime, long lDuration)
{
	return m_pTimeCtrl->IsVideoEffectOverlap(lTime, lDuration);
}

BOOL CTimeLinePanel::IsAudioReplaceOverlap(long lTime, long lTimeEnd)
{
	return m_pTimeCtrl->IsAudioReplaceOverlap(lTime, lTimeEnd);
}

BOOL CTimeLinePanel::IsAudioMixOverlap(long lTime, long lTimeEnd)
{
	return m_pTimeCtrl->IsAudioMixOverlap(lTime, lTimeEnd);
}

void CTimeLinePanel::SetVideoTextSegTip(int nIndex, CString strTip) 
{
	m_pTimeCtrl->SetVideoTextSegTip(nIndex, strTip);
}

int CTimeLinePanel::GetVideoTextSegCount()
{
	return m_pTimeCtrl->GetVideoTextSegCount();
}

void CTimeLinePanel::ResetVideoTextTooltip()
{
	m_pTimeCtrl->ResetVideoTextTooltip();
}

void CTimeLinePanel::ResetVideoEffectTooltip()
{
	m_pTimeCtrl->ResetVideoEffectTooltip();
}

void CTimeLinePanel::ResetAudioReplaceTooltip()
{
	m_pTimeCtrl->ResetAudioReplaceTooltip();
}

void CTimeLinePanel::ResetAudioMixTooltip()
{
	m_pTimeCtrl->ResetAudioMixTooltip();
}

void CTimeLinePanel::SetMark()
{
	m_pTimeCtrl->SetMark();
}

BOOL CTimeLinePanel::DeletePeriod()
{
	return m_pTimeCtrl->DeletePeriod();
}

void CTimeLinePanel::GetSelRange(long& lTimeSelStart, long& lTimeSelEnd)
{
	m_pTimeCtrl->GetSelRange(lTimeSelStart, lTimeSelEnd);
}

long CTimeLinePanel::GetPointerTime()
{
	return m_pTimeCtrl->GetPointerTime();
}

LRESULT CTimeLinePanel::OnPeriodUnInsert(WPARAM wParam, LPARAM lParam)
{
	CString strTmp;
	strTmp.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTmp, strTmp);
	MACRO_MAINFRM->ShowProgress();
	CObArray* pObj = new CObArray;
	GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj, 0); 

	if (m_pTimeCtrl->DeletePeriod(wParam, lParam))
	{
		GetParent()->SendMessage(WM_MODIFY_VIDEO);

		CObArray* pObj2 = new CObArray;
		GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj2, (LPARAM)pObj); 
		MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(GetParent(), "video_op", pObj, pObj2);
	}
	else
	{
		::ReleaseObArray(pObj);
		delete pObj;
	}
	MACRO_MAINFRM->HideProgress();

	return 0L;
}

LRESULT CTimeLinePanel::OnPeriodUnDelete(WPARAM wParam, LPARAM lParam)
{
	CObArray* pObj = new CObArray;
	GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj, 0); 

	m_pTimeCtrl->UnDeletePeriod(wParam);
	GetParent()->SendMessage(WM_MODIFY_VIDEO);

	CObArray* pObj2 = new CObArray;
	GetParent()->SendMessage(WM_RETRIEVE_VIDEO_DATA, (WPARAM)pObj2, (LPARAM)pObj); 
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(GetParent(), "video_op", pObj, pObj2);

	return 0L;
}

LRESULT CTimeLinePanel::OnFadeInEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_FADE_IN_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnFadeOutEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_FADE_OUT_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnDeleteEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_DELETE_EFFECT, wParam, lParam);
	return 0L;
}

LRESULT CTimeLinePanel::OnTransitEffect(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_TRANSIT_EFFECT, wParam, lParam);
	return 0L;
}

BOOL CTimeLinePanel::GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, 
									  CString& strPathNext, long& lTimeNext)
{
	return m_pTimeCtrl->GetBoundaryFiles(lTime, strPathPrev, lTimePrev, strPathNext, lTimeNext);
}

void CTimeLinePanel::SetBreakPoint()
{
	m_pTimeCtrl->SetBreakPoint();
}

void CTimeLinePanel::RedrawPanel()
{
	m_pTimeCtrl->RedrawWindow();
}

void CTimeLinePanel::SetSlidePath(CString slidePath)
{
	m_pTimeCtrl->SetSlidePath(slidePath);
}
