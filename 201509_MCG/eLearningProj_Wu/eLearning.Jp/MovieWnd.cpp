// MovieWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "mainfrm.h"
#include "resource.h"
#include "MovieWnd.h"
#include "MemDC.h"
#include "BmpStatic.h"
#include "cmkapi.h"
#include "DlgMP4Info.h"
#include "MediaUtils.h"
#include "TimeLinePanel.h"
#include "macro.h"
#include <io.h>
#include <atlbase.h>

#include "..\TimeLinePlayer\TimeLinePlayer\TimeLinePlayer_i.c"
#include "..\MediaKit\MediaKit.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#define NO_VOICE 0x80040216

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CMovieWnd::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CMovieWnd

CMovieWnd::CMovieWnd()
{
	m_bAudioOnly = FALSE;
	m_bClean = FALSE;
	m_pCtrlArray = new CObArray;
	m_nCtrlEntered = -1;
	m_bTracking = FALSE;
	m_bLeaveTrack = FALSE;
	m_bFit = FALSE;
	m_rcRedraw.SetRectEmpty();  
    m_lTimeLength = 0;
	m_rcStatusBar.SetRectEmpty();
	m_nTimerID = -1;
	m_pFont = NULL;

	m_pBmpSep = new CBmpStatic(IDB_SEP);
	m_pBmpSep->SetBorder(FALSE);

	m_pBmpStatus = new CBmpStatic(IDB_CUT_STATUS);
	m_pBmpStatus->SetBorder(FALSE);

	m_pBmpStatusRep = new CBmpStatic(IDB_REP_STATUS);
	m_pBmpStatusRep->SetBorder(FALSE);

    m_bWait = FALSE;
	m_bNoWait = FALSE;

	m_nLargeGap = constLargeGap;
	m_nSmallGap = constSmallGap;

	m_nPlayID = IDB_PLAY_VIDEO;
	m_nPauseID = IDB_PAUSE_VIDEO;
	m_nStopID = IDB_STOP_VIDEO;
	m_nProfileID = IDB_INFOVIDEO;
	m_nCaptureID = IDB_CAPTURE_VIDEO;

	m_bLoading = FALSE;
	m_nCurIndex = -1;
}

CMovieWnd::~CMovieWnd()
{
	if (!m_bClean)
	{
	}

    int nArraySize = m_pCtrlArray->GetSize();
    for (int i = 0; i < nArraySize; i++)
	{
		CObject* pObj = m_pCtrlArray->GetAt(0);
		m_pCtrlArray->RemoveAt(0);
	    delete pObj;                      
	}

	delete m_pCtrlArray;
	delete m_pBmpSep;
	delete m_pBmpStatus;
	delete m_pBmpStatusRep;
}


BEGIN_MESSAGE_MAP(CMovieWnd, CWnd)
	//{{AFX_MSG_MAP(CMovieWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMovieWnd message handlers

BOOL CMovieWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);

	CSize csBmp = GetBmpDim(m_nPlayID);
	csBmp.cx /= 4;
	m_nBtnHeight = csBmp.cy;

	CRect rcClient;
	GetClientRect(rcClient);

	CString strMsg;
	strMsg.LoadString(IDS_NO_FILE);

	CRect rc;
	rc.bottom = rcClient.bottom - m_nLargeGap;
	rc.top = rc.bottom - constStatusHeight;
	rc.left = rcClient.left + m_nLargeGap;
	rc.right = rcClient.right - m_nLargeGap;
	AddCtrl(IDC_STATUS_BAR, CTRL_STATUS_BAR, rc, 0, strMsg);
	m_rcStatusBar = rc;


	CString strTip;
	rc.bottom = rc.top - m_nSmallGap;
	rc.top = rc.bottom - csBmp.cy;
	rc.left = rcClient.left + m_nLargeGap;
	rc.right = rc.left + csBmp.cx;
	strTip.LoadString(IDS_PLAY);
	AddCtrl(IDC_PLAY, CTRL_SUNK_BUTTON, rc, m_nPlayID, strTip);

	rc.left = rc.right + constSmallGap;
	rc.right = rc.left + csBmp.cx;
	strTip.LoadString(IDS_STOP);
	AddCtrl(IDC_STOP, CTRL_SUNK_BUTTON, rc, m_nStopID, strTip);

	rc.left = rc.right + constSmallGap;
	rc.right = rc.left + csBmp.cx;
	strTip.LoadString(IDS_PAUSE);
	AddCtrl(IDC_PAUSE, CTRL_SUNK_BUTTON, rc, m_nPauseID, strTip);

	CSize csBmpSep = GetBmpDim(IDB_SEP);
	rc.left = rc.right + constSmallGap + 2;
	rc.right = rc.left + csBmpSep.cx;
	m_pBmpSep->Create(WS_CHILD | WS_VISIBLE, rc, this);

	rc.left = rc.right + constSmallGap + 2;
	rc.right = rc.left + csBmp.cx;
	strTip.LoadString(IDS_PROFILE);
	AddCtrl(IDC_PROFILE, CTRL_PUSH_BUTTON, rc, m_nProfileID, strTip);

	rc.left = rc.right + constSmallGap;
	rc.right = rc.left + csBmp.cx;
	strTip.LoadString(IDS_CAPTURE_VIDEO);
	AddCtrl(IDC_CAPTURE, CTRL_PUSH_BUTTON, rc, m_nCaptureID, strTip);

	m_imageList.Create(csBmp.cx, csBmp.cy, 
					   ILC_COLORDDB | ILC_MASK, 4 * 4, 1);
	LoadImageList();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_CUT_STATUS);
	BITMAP b;
	bitmap.GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;
	rc.right = rcClient.right - m_nLargeGap;
	rc.left = rc.right - nBmpWidth;
	rc.bottom = rc.top + nBmpWidth;
	m_pBmpStatus->Create(WS_CHILD, rc, this);
	m_pBmpStatusRep->Create(WS_CHILD, rc, this);

	m_ToolTip.Create(this);
	AddToolTip();

    EnableButtons(FALSE);

	m_nTimerID = SetTimer(1, 300, NULL);

	return bRet;
}

void CMovieWnd::StopTimer()
{
	if ((int)m_nTimerID > 0)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = -1;
	}
}

void CMovieWnd::RestartTimer()
{
	if ((int)m_nTimerID < 0)
		m_nTimerID = SetTimer(1, 300, NULL);
}

CSize CMovieWnd::GetBmpDim(UINT nID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);
	return CSize(b.bmWidth, b.bmHeight);
}

void CMovieWnd::AddToolTip()
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2) //Button
			m_ToolTip.AddTool(this, pInfo->m_strText, pInfo->m_rcPosition, 1);
	}
	m_ToolTip.Activate(TRUE);
}

void CMovieWnd::LoadImageList()
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2) //Button
		{	
			CBitmap bitmap;
			bitmap.LoadBitmap(pInfo->m_nBmpID);
			m_imageList.Add(&bitmap, RGB(255, 0, 255));
		}
	}
}

void CMovieWnd::DrawCtrl(CDC* pDC, CCtrlInfo* pInfo)
{
	CRect rcDraw = pInfo->m_rcPosition;
	CTRL_TYPE nType = pInfo->m_nType;
	CPoint pt;
	pt.x = pInfo->m_rcPosition.left;
	pt.y = pInfo->m_rcPosition.top;
	switch (nType)
	{
		case CTRL_SUNK_BUTTON:
		case CTRL_PUSH_BUTTON:
		case CTRL_TOGGLE_BUTTON:
		{
			if (pInfo->m_nID != IDC_PAUSE)
				m_imageList.Draw(pDC, pInfo->m_nBmpIndex * 4 + pInfo->m_nStatus, pt, ILD_TRANSPARENT);
		}
		break;

		case CTRL_STATUS_BAR:
		{
			CBrush brBg;
			brBg.CreateSolidBrush(RGB(0, 0, 0));
			pDC->FillRect(rcDraw, &brBg);
			brBg.DeleteObject();
			
			CPen penDGray, penLGray;
			penDGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
			penLGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHILIGHT));
			
			CPen* pOldPen = (CPen*)pDC->SelectObject(&penDGray);
			pDC->MoveTo(rcDraw.left, rcDraw.bottom);
			pDC->LineTo(rcDraw.left, rcDraw.top);
			pDC->LineTo(rcDraw.right, rcDraw.top);
		
			pDC->SelectObject(&penLGray);
			pDC->MoveTo(rcDraw.right, rcDraw.top);
			pDC->LineTo(rcDraw.right, rcDraw.bottom);
			pDC->LineTo(rcDraw.left - 1, rcDraw.bottom);

			CFont* pNewFont = m_pFont;
			if (pNewFont == NULL)
				pNewFont = GetParent()->GetFont();

			CFont* pOldFont = pDC->SelectObject(pNewFont);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkMode(TRANSPARENT);
			CRect rcDraw = pInfo->m_rcPosition;
			rcDraw.left += m_nLargeGap;
			pDC->DrawText(pInfo->m_strText, rcDraw, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			rcDraw.right -= m_nLargeGap;
			
			CString strCurrentTime = GetTimeStr(GetCurrentPos());
			CString strDuration = GetTimeStr(m_lTimeLength);
			pDC->DrawText(strCurrentTime + " / " + strDuration, 
						  rcDraw, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
			pDC->SelectObject(pOldFont);

			pDC->SelectObject(pOldPen);
			penDGray.DeleteObject();
			penLGray.DeleteObject();
		}
		break;
	}
}

int CMovieWnd::GetBmpCount()
{
	int nCount = 0;
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2) //Button
			nCount++;
	}
	return nCount;
}

void CMovieWnd::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
}

void CMovieWnd::AddCtrl(UINT nID, CTRL_TYPE nType, CRect rc, UINT nBmpID, 
						CString strText, UINT nStatus)
{
	CCtrlInfo* pInfo = new CCtrlInfo(nID, nType, rc, nBmpID, strText, nStatus);
	if (nType <= 2)
		pInfo->m_nBmpIndex = GetBmpCount();
	m_pCtrlArray->Add(pInfo);
}

void CMovieWnd::SetCtrlPos(int nIndex, CRect rc)
{
	if (m_pCtrlArray->GetSize() > nIndex)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(nIndex);
		pInfo->m_rcPosition = rc;
	}
}

BOOL CMovieWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	dwStyle |= WS_CLIPCHILDREN;
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}


//
//  Displays a text string in a status line near the bottom of the dialog
//
void CMovieWnd::Say(LPCTSTR szText)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nID == IDC_STATUS_BAR)
		{
			pInfo->m_strText = szText;
			Redraw(pInfo->m_rcPosition);
			break;
		}
	}
}

void CMovieWnd::PauseFile() 
{
	if (!m_pMixPlay)
		return;

	m_pMixPlay->Pause();

    CString strMsg;
	strMsg.LoadString(IDS_S_PAUSE);
    Say(strMsg);

	GetParent()->PostMessage(WM_PAUSE_VIDEO);
}

void CMovieWnd::PlayFile() 
{
	if (!m_pMixPlay)
		return;

	m_pMixPlay->Run();

	CString strMSG;
	strMSG.LoadString(IDS_RUNNING);
    Say(strMSG);
	//GetParent()->PostMessage(WM_PLAY_VIDEO);
}

void CMovieWnd::StopFile(BOOL bSetToStart) 
{
	if (!m_pMixPlay)
		return;

	m_pMixPlay->Stop();
    if (bSetToStart)
		m_pMixPlay->Seek(0);

    CString strMsg;
	strMsg.LoadString(IDS_S_STOP);
	Say(strMsg);

	GetParent()->PostMessage(WM_STOP_VIDEO);
}

void CMovieWnd::ShowState()
{
}

void CMovieWnd::GetVideoRect(CRect& rc)
{
	GetClientRect(rc);
    int nWidth =  rc.Width() - m_nLargeGap * 2;
    int nHeight = rc.Height() - m_nLargeGap * 2 - m_nSmallGap * 2 -
			      m_nBtnHeight - constStatusHeight;
	rc.left = rc.left + m_nLargeGap;
	rc.right = rc.right - m_nLargeGap;
	rc.top = rc.top + m_nLargeGap;
	rc.bottom = rc.top + nHeight;
}

LRESULT CMovieWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Field notifications from the DirectShow filter graph manager
    // and those posted by the application
    switch (message)
    {
		case WM_GRAPHNOTIFY:
		{
		}
            break;

        //
        // If the media is not running and contains a video component,
        // the following code helps to keep the video window properly
        // painted when the main window moves or returns from being
        // minimized.  This allows the current video frame to be smoothly 
        // redrawn.  Otherwise, the video portion could "lag" behind the 
        // rest of the application window as it moves.
        //
		case WM_WINDOWPOSCHANGED:
            break;

        case WM_PLAYFILE:
			{
			
				break;
			}
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

BOOL CMovieWnd::CanStep(void)
{
	return TRUE;
}

HRESULT CMovieWnd::Step(int n)
{
	if (!m_pMixPlay)
		return NULL;

	HRESULT hr = m_pMixPlay->Step(n); 

	return hr;
}

BOOL CMovieWnd::UnLoadFile() 
{
	BOOL bRet = SetTimeLine("");
	return bRet;
}

void CMovieWnd::Stop()
{
	PressButton(IDC_STOP, FALSE);
	StopFile(FALSE);
	Sleep(300);
}

BOOL CMovieWnd::ReloadFile(long lTimePos)
{
	SetTimeLine(m_strPreTimeLine);
	SetFilePosition(lTimePos);

	return TRUE;
}

BOOL CMovieWnd::SetAudioReplace(CString strParam)
{
	CString strThumbDir1 = MACRO_HTML_DIR(MACRO_MAINFRM->GetCourse())+"THUMB3";
	ClearDir(strThumbDir1);

	if (!m_pMixPlay)
		return FALSE;

	CComBSTR cbstr(strParam);
	m_pMixPlay->ChangeAudioReplaceStream(cbstr);

	long l_Byte;
	long l_Count;
	CComBSTR cbstr1(strThumbDir1 + "\\AudioThumb.wav");
	m_pMixPlay->GetAllAudioThumbnail(cbstr1, 1, &l_Byte, &l_Count);

	return TRUE;
}

BOOL CMovieWnd::SetVideoText(CString strParam)
{
	if (!m_pMixPlay)
		return FALSE;

	CComBSTR cbstr(strParam);
	HRESULT hr = m_pMixPlay->ChangeVideoTextStream(cbstr);
	
	SetFilePosition(GetCurrentPos());

	return TRUE;
}

BOOL CMovieWnd::SetVideoEffect(CString strParam)
{
	if (!m_pMixPlay)
		return FALSE;

	CComBSTR cbstr(strParam);
	m_pMixPlay->ChangeVideoEffectStream(cbstr);

	SetFilePosition(GetCurrentPos());

	return TRUE;
}

BOOL CMovieWnd::SetAudioMix(CString strParam)
{
	CString strThumbDir1 = MACRO_HTML_DIR(MACRO_MAINFRM->GetCourse())+"THUMB3";
	ClearDir(strThumbDir1);

	if (!m_pMixPlay)
		return FALSE;

	CComBSTR cbstr(strParam);
	m_pMixPlay->ChangeAudioMixerStream(cbstr);

	long l_Byte;
	long l_Count;
	CComBSTR cbstr1(strThumbDir1 + "\\AudioThumb.wav");
	m_pMixPlay->GetAllAudioThumbnail(cbstr1, 1, &l_Byte, &l_Count);

	return TRUE;
}

BOOL CMovieWnd::SetTimeLine(CString strParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();

	CString strThumbDir = MACRO_HTML_DIR(strCourse)+"THUMB2";
	//if (FileExists(strThumbDir))
		ClearDir(strThumbDir);

	CString strThumbDir1 = MACRO_HTML_DIR(strCourse)+"THUMB3";
	//if (FileExists(strThumbDir1))
		ClearDir(strThumbDir1);

	if (!m_pMixPlay)
		return FALSE;

	if (m_bLoading)
		return FALSE;

	m_bLoading = TRUE;

	m_strCurTimeLine = strParam;

	// First release any existing interfaces
	m_lTimeLength = 0;

	Stop();

	CComBSTR cbstr("");
	m_pMixPlay->ChangeMainStream(cbstr);
	m_pMixPlay->DisplayClose();
//	m_pMixPlay->Close();

	if (strParam == "")   // Check for existence
	{
		EnableButtons(FALSE);

		CString strMsg;
		strMsg.LoadString(IDS_NO_FILE);
		Say(strMsg);
		
		m_pBmpStatus->ShowWindow(SW_HIDE);
		m_pBmpStatusRep->ShowWindow(SW_HIDE);

		m_bLoading = FALSE;

		return TRUE;
	}

	CComBSTR cbstrParam(strParam);
	if (FAILED(m_pMixPlay->ChangeMainStream(cbstrParam)))
	{
		CComBSTR cbstrNull("");
		m_pMixPlay->ChangeMainStream(cbstrNull);
		return TRUE;
	}

	CRect rcVideo;
	GetVideoRect(rcVideo);
	m_pMixPlay->DisplayOpen((long)m_hWnd, rcVideo.left, rcVideo.top, rcVideo.Width(), rcVideo.Height());

	// See if the renderer supports frame stepping on this file.
    // Enable/disable frame stepping button accordingly
	RefreshBtns();

	Pause();

	m_bLoading = FALSE;

	m_strPreTimeLine = strParam;
	m_pMixPlay->GetLength(&m_lTimeLength);

	CComBSTR cbstrName(strThumbDir + "\\VideoThumb");
	long nCount = 0;
	m_pMixPlay->GetAllVideoThumbnail(cbstrName, 10000, &nCount);

	long l_Byte;
	long l_Count;
	CComBSTR cbstr1(strThumbDir1 + "\\AudioThumb.wav");
	m_pMixPlay->GetAllAudioThumbnail(cbstr1, 1, &l_Byte, &l_Count);

	return TRUE;
}

void CMovieWnd::RetailOutput(LPCTSTR tszErr, ...)
{
    TCHAR tszErrOut[MAX_PATH + 256];

    va_list valist;

    va_start(valist,tszErr);
    wvsprintf(tszErrOut, tszErr, valist);
    OutputDebugString(tszErrOut);
    va_end (valist);
}

void CMovieWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	if (!m_bClean)
		m_bClean = TRUE;
	
	StopTimer();

//	if (m_pEasyUtility != NULL) 
//	{
//		m_pEasyUtility->Release(); 
//		m_pEasyUtility = NULL;
//	}	

	if (m_pMixPlay != NULL) 
	{
		m_pMixPlay->DisplayClose();
		m_pMixPlay->Close();

		m_pMixPlay->Release(); 
		m_pMixPlay = NULL;
	}	
}

int CMovieWnd::CtrlEntered(CPoint point)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		CRect rc = pInfo->m_rcPosition;
		if (rc.PtInRect(point))
			return i;
	}
	return -1;
}

void CMovieWnd::EnterCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(nCtrl);
	CTRL_TYPE nType = pInfo->m_nType;
	if (nType == CTRL_STATUS_BAR)
		return;

	if (pInfo->m_nStatus == constBtnNormal)
	{
		pInfo->m_nStatus = constBtnOver;
		Redraw(pInfo->m_rcPosition);
	}

	m_nCtrlEntered = nCtrl;
}

void CMovieWnd::LeaveCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(nCtrl);
	CTRL_TYPE nType = pInfo->m_nType;
	if (nType == CTRL_STATUS_BAR)
		return;

	if (pInfo->m_nStatus == constBtnOver)
	{
		pInfo->m_nStatus = constBtnNormal;
		Redraw(pInfo->m_rcPosition);
	}
}

void CMovieWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	int nCtrl = CtrlEntered(point);
	if (nCtrl < 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(nCtrl);
	CTRL_TYPE nType = pInfo->m_nType;
	if ((nType == CTRL_STATUS_BAR) ||
		(nType == CTRL_SUNK_BUTTON && pInfo->m_nStatus == constBtnDown) ||
		(nType <= 2 && pInfo->m_nStatus == constBtnDisable))
	{
		if (pInfo->m_nID == IDC_PLAY && pInfo->m_nStatus == constBtnDown)
			PressButton(IDC_PAUSE);
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	m_bTracking = TRUE;
	SetCapture();
	SetFocus();

	if (nType == CTRL_TOGGLE_BUTTON && pInfo->m_nStatus == constBtnDown)
		pInfo->m_nStatus = constBtnOver;
	else
		pInfo->m_nStatus = constBtnDown;

	Redraw(pInfo->m_rcPosition);

	m_nClickCtrl = nCtrl;
	m_ptPrePos = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CMovieWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	//wangwang


    //GetParent()->SendMessage(WM_POS_CHANGED, 0);		//Buglist No.74 kagata
	m_bTracking = FALSE;

	ReleaseCapture();

	CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(m_nClickCtrl);
	CTRL_TYPE nType = pInfo->m_nType;
	int nCurCtrl = CtrlEntered(point);
	BOOL bIn = (nCurCtrl == m_nClickCtrl);
	if (bIn && nType == CTRL_PUSH_BUTTON)
		pInfo->m_nStatus = constBtnOver;
	else if (!bIn && nType == CTRL_PUSH_BUTTON)
		pInfo->m_nStatus = constBtnNormal;
	else if (bIn && nType == CTRL_SUNK_BUTTON)
		pInfo->m_nStatus = constBtnDown;
	else if (!bIn && nType == CTRL_SUNK_BUTTON)
		pInfo->m_nStatus = constBtnNormal;

	Redraw(pInfo->m_rcPosition);

	if (bIn)
	{
		SetRelatedButtons(pInfo->m_nID);
		DoAction(pInfo->m_nID);
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void CMovieWnd::EnableButton(UINT nID, BOOL bEnable, BOOL bRepaint)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2 && pInfo->m_nID == nID)
		{
			if (!bEnable)
			{
				if (pInfo->m_nStatus != constBtnDisable)
				{
					pInfo->m_nPreStatus = pInfo->m_nStatus;
					pInfo->m_nStatus = constBtnDisable;
				}
			}
			else
			{
				if (pInfo->m_nStatus == constBtnDisable)
					pInfo->m_nStatus = pInfo->m_nPreStatus;
			}
			if (bRepaint)
				Redraw(pInfo->m_rcPosition);
			break;
		}
	}
}

CRect CMovieWnd::GetAllBtnRect()
{
	CRect rc;
	rc.SetRectEmpty();
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2)
			rc.UnionRect(rc, pInfo->m_rcPosition);
	}
	return rc;
}

void CMovieWnd::EnableButtons(BOOL bEnable)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2)
		{
			if (!bEnable)
			{
				if (pInfo->m_nStatus != constBtnDisable)
				{
					pInfo->m_nPreStatus = pInfo->m_nStatus;
					pInfo->m_nStatus = constBtnDisable;
				}
			}
			else
			{
				if (pInfo->m_nStatus == constBtnDisable)
					pInfo->m_nStatus = pInfo->m_nPreStatus;
			}
		}
	}
	RepaintBtn();
}

void CMovieWnd::RepaintBtn()
{
	CRect rc = GetAllBtnRect();
	Redraw(rc);
}

void CMovieWnd::RefreshBtns()
{
    EnableButton(IDC_PLAY, TRUE, FALSE);
    EnableButton(IDC_STOP, TRUE, FALSE);
    EnableButton(IDC_PAUSE, TRUE, FALSE);
    EnableButton(IDC_PROFILE, TRUE, FALSE);
    EnableButton(IDC_CAPTURE, TRUE, FALSE);
	RepaintBtn();
}

void CMovieWnd::RefreshStatusBar()
{
	Redraw(m_rcStatusBar);
}

void CMovieWnd::RepaintCtrl(UINT nID)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nID == nID)
		{
			Redraw(pInfo->m_rcPosition);
			break;
		}
	}
}

void CMovieWnd::PressButton(UINT nID, BOOL bGo)
{
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		
		if (pInfo->m_nID == nID && pInfo->m_nStatus == constBtnDisable)
			return;

		if (pInfo->m_nType == CTRL_SUNK_BUTTON && pInfo->m_nID == nID)
		{
			pInfo->m_nStatus = constBtnDown;
			Redraw(pInfo->m_rcPosition);
			break;
		}
	}
	SetRelatedButtons(nID);
	if (bGo)
		DoAction(nID);
}

void CMovieWnd::SetRelatedButtons(UINT nID)
{
	UINT nIDUp1, nIDUp2;
	if (nID == IDC_PLAY)
	{
		nIDUp1 = IDC_STOP;
		nIDUp2 = IDC_PAUSE;
	}
	else if (nID == IDC_STOP)
	{
		nIDUp1 = IDC_PLAY;
		nIDUp2 = IDC_PAUSE;
	}
	else if (nID == IDC_PAUSE)
	{
		nIDUp1 = IDC_PLAY;
		nIDUp2 = IDC_STOP;
	}
	else
	{
		return;
	}

	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType > 2)
			continue;
		if ((pInfo->m_nID != nID) && (pInfo->m_nID == nIDUp1 || pInfo->m_nID == nIDUp2))
		{
			if (pInfo->m_nStatus == constBtnDown)
			{
				pInfo->m_nStatus = constBtnNormal;
				Redraw(pInfo->m_rcPosition);
				break;
			}
		}
	}
}



void CMovieWnd::DoAction(UINT nID)
{
	switch (nID)
	{
		case IDC_PLAY:
			{
				//CString strCourse = MACRO_MAINFRM->GetCourse();
				//if (!MACRO_MAINFRM->m_bPause)
				//GetParent()->SendMessage(WM_MODIFY_VIDEO);
				{
 				  //MACRO_MAINFRM->CollectCourse(strCourse);
				  //MACRO_MAINFRM->LoadCourse(strCourse, TRUE);
				}
				PlayFile();
			    //m_bPause=true;
				break;
			}
		

		case IDC_STOP:
			StopFile(TRUE);
			break;

		case IDC_PAUSE:
			{
			  PauseFile();
			  //m_bPause=true;
			  break;
			}
		

		case IDC_PROFILE:
			ShowInfo();
			break;

	}	
}

void CMovieWnd::CaptureImage() 
{

}

void CMovieWnd::ShowInfo() 
{
	CDlgMP4Info dlg;
	dlg.DoModal();
}

void CMovieWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_bTracking)
	{
		int nCurCtrl = CtrlEntered(point);
		int nPreCtrl = CtrlEntered(m_ptPrePos);
		BOOL bCurIn = (nCurCtrl == m_nClickCtrl);
		BOOL bPreIn = (nPreCtrl == m_nClickCtrl);
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(m_nClickCtrl);
		CTRL_TYPE nType = pInfo->m_nType;
		if (nType == CTRL_SUNK_BUTTON || nType == CTRL_PUSH_BUTTON)
		{
			if (bCurIn && !bPreIn)
				pInfo->m_nStatus = constBtnDown;
			else if (!bCurIn && bPreIn)
				pInfo->m_nStatus = constBtnNormal;
		}
		else if (nType == CTRL_TOGGLE_BUTTON)
		{
			if (bCurIn && !bPreIn)
			{
				if (pInfo->m_nStatus == constBtnNormal)
					pInfo->m_nStatus = constBtnDown;
				else if (pInfo->m_nStatus == constBtnDown)
					pInfo->m_nStatus = constBtnOver;
			}
			else if (!bCurIn && bPreIn)
			{
				if (pInfo->m_nStatus == constBtnOver)
					pInfo->m_nStatus = constBtnDown;
				else if (pInfo->m_nStatus == constBtnDown)
					pInfo->m_nStatus = constBtnNormal;
			}
		}
		if ((bCurIn && !bPreIn) || (!bCurIn && bPreIn))
			Redraw(pInfo->m_rcPosition);
	
		m_ptPrePos = point;
	}
	else
	{
		LeaveTrack();
		
		int nCtrl = CtrlEntered(point);
		if (nCtrl != m_nCtrlEntered)
			LeaveCtrl(m_nCtrlEntered);
		if (nCtrl < 0)
			return;

		EnterCtrl(nCtrl);
	}
}

void CMovieWnd::LeaveTrack() 
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

LRESULT CMovieWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		if (pInfo->m_nType <= 2 && pInfo->m_nStatus == constBtnOver) //Button
		{
			pInfo->m_nStatus = constBtnNormal;
			Redraw(pInfo->m_rcPosition);
		}
	}

	return 0;
}

void CMovieWnd::Redraw(CRect rc)
{
	m_rcRedraw = rc;
	RedrawWindow(rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	m_rcRedraw.SetRectEmpty();  
}

BOOL CMovieWnd::SetFilePosition(long lCurPos)
{
	if (!m_pMixPlay)
		return FALSE;

	if (IsRunning())
		return FALSE;

	m_pMixPlay->Seek(lCurPos);

	return TRUE;
}

long CMovieWnd::GetCurrentPos() 
{
	if (m_strCurTimeLine == "")
		return 0;

	if (!m_pMixPlay)
		return 0;

	long lmsTime;
	m_pMixPlay->GetCurrTime(&lmsTime);

	return lmsTime;
}

void CMovieWnd::OnTimer(UINT nIDEvent) 
{
	if (m_bWait)
		BeginWaitCursor();

	if (m_bNoWait)
	{
		EndWaitCursor();
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		m_bNoWait = FALSE;
	}

	if (!m_pMixPlay)
	{
		CWnd::OnTimer(nIDEvent);
		return;
	}

	long lmsTime;
	if (m_strCurTimeLine == "")
		lmsTime = 0;
	else
		m_pMixPlay->GetCurrTime(&lmsTime);

	CString strCurrentTime = GetTimeStr(lmsTime);

	int nIndex = FindSlideIndex(strCurrentTime);
	if (m_nCurIndex != nIndex && m_pstrArrayCmd->GetSize() > nIndex)
	{
		GetParent()->SendMessage(WM_SCRIPT, (WPARAM)(const char*)m_pstrArrayCmd->GetAt(nIndex), 
											(LPARAM)(const char*)m_pstrArrayScript->GetAt(nIndex));
		m_nCurIndex = nIndex;
	}

	RefreshStatusBar();

	long lPos = lmsTime;
	static long s_lPos = -1;
	if (lPos != s_lPos)
	{
		GetParent()->SendMessage(WM_POS_CHANGED, lPos);
		s_lPos = lPos;
	}

	//GetParent()->SendMessage(WM_TIME_CHANGED, lPos);

	//long lmsDuration;
	//m_pMixPlay->GetLength(&lmsDuration);
	if (lmsTime >= m_lTimeLength && IsRunning())
		StopAndBack();

	/*
	if (IsRunning())
	{
		long lmsTime;
		m_pMixPlay->GetCurrTime(&lmsTime);

		long lPreCurrentTime = m_lTimeCurrent;
		GetParent()->SendMessage(WM_CONVERT_TIME, lmsTime);
		if (m_lTimeCurrent >= 0)
		{
			CString strCurrentTime = GetTimeStr(m_lTimeCurrent);

			int nIndex = FindSlideIndex(strCurrentTime);
			if (m_nCurIndex != nIndex && m_pstrArrayCmd->GetSize() > nIndex)
			{
				GetParent()->SendMessage(WM_SCRIPT, (WPARAM)(const char*)m_pstrArrayCmd->GetAt(nIndex), 
													(LPARAM)(const char*)m_pstrArrayScript->GetAt(nIndex));
				m_nCurIndex = nIndex;
			}

			RefreshStatusBar();

			long lPos = m_lTimeCurrent;
			static long s_lPos = -1;
			if (lPos != s_lPos)
			{
				GetParent()->SendMessage(WM_POS_CHANGED, lPos);
				s_lPos = lPos;
			}

			//GetParent()->SendMessage(WM_TIME_CHANGED, lPos);

			//long lmsDuration;
			//m_pMixPlay->GetLength(&lmsDuration);
			if (m_lTimeCurrent >= m_lTimeLength && IsRunning())
				StopAndBack();
		}
		else
		{
			BOOL bIsRunning = IsRunning();
			if (bIsRunning)
				m_pMixPlay->Stop();
			GetParent()->SendMessage(WM_CHANGE_SEGMENT, lPreCurrentTime);
			if (bIsRunning && m_lTimeCurrent < 0)
				m_pMixPlay->Run();
		}
	}
*/

	CWnd::OnTimer(nIDEvent);
}

BOOL CMovieWnd::IsRunning()
{ 
	if (!m_pMixPlay)
		return FALSE;

	long lState;
	m_pMixPlay->GetState(&lState);
	
	return (lState == State_Running);
}

void CMovieWnd::ShowBmpStatus(BOOL bFlag)
{
	BOOL bVisible = m_pBmpStatus->IsWindowVisible();
	if ((bVisible && !bFlag) || (!bVisible && bFlag))
		m_pBmpStatus->ShowWindow((bFlag) ? SW_SHOW : SW_HIDE);
}

void CMovieWnd::ShowBmpStatusRep(BOOL bFlag)
{
	BOOL bVisible = m_pBmpStatusRep->IsWindowVisible();
	if ((bVisible && !bFlag) || (!bVisible && bFlag))
		m_pBmpStatusRep->ShowWindow((bFlag) ? SW_SHOW : SW_HIDE);
}

BOOL CMovieWnd::GetFileDuration(long& lDuration)
{
	if (!m_pMixPlay)
		return FALSE;

	long lmsLength;
	m_pMixPlay->GetLength(&lmsLength);
    lDuration = lmsLength;

    return TRUE;
}

void CMovieWnd::ShowErrorMsg(HRESULT hr)
{
	CString strMsg = "???";
	switch (hr)
	{
	case VFW_S_AUDIO_NOT_RENDERED:
		strMsg = "Cannot play back the audio stream: could not find a suitable renderer.";
		break;

	case VFW_S_DUPLICATE_NAME:
		strMsg = "An attempt to add a filter with a duplicate name succeeded with a modified name."; 
		break;

	case VFW_S_PARTIAL_RENDER:
		strMsg = "Some of the streams in this movie are in an unsupported format."; 
		break;

	case VFW_S_VIDEO_NOT_RENDERED:
		strMsg = "Cannot play back the video stream: could not find a suitable renderer."; 
		break;

	case E_ABORT:
		strMsg = "Operation aborted."; 
		break;

	case E_FAIL:
		strMsg = "Failure."; 
		break;

	case E_INVALIDARG:
		strMsg = "Argument is invalid."; 
		break;

	case E_OUTOFMEMORY:
		strMsg = "Insufficient memory."; 
		break;

	case E_POINTER:
		strMsg = "NULL pointer argument."; 
		break;

	case VFW_E_CANNOT_CONNECT:
		strMsg = "No combination of intermediate filters could be found to make the connection."; 
		break;

	case VFW_E_CANNOT_LOAD_SOURCE_FILTER:
		strMsg = "The source filter for this file could not be loaded."; 
		break;

	case VFW_E_CANNOT_RENDER:
		strMsg = "No combination of filters could be found to render the stream."; 
		break;

	case VFW_E_INVALID_FILE_FORMAT:
		strMsg = "The file format is invalid."; 
		break;

	case VFW_E_NOT_FOUND:
		strMsg = "An object or name was not found."; 
		break;

	case VFW_E_NOT_IN_GRAPH:
		strMsg = "Cannot perform the requested function on an object that is not in the filter graph."; 
		break;

	case VFW_E_UNKNOWN_FILE_TYPE:
		strMsg = "The media type of this file is not recognized."; 
		break;

	case VFW_E_UNSUPPORTED_STREAM:
		strMsg = "Cannot play back the file: the format is not supported.";
		break;
	}

	MessageBox(strMsg);
}

void CMovieWnd::WaitForCompletion(IUnknown *pGraph)
{
    HRESULT hr;
    LONG lEvCode = 0;
    IMediaEvent *pEvent;

    pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent);
    hr = pEvent->WaitForCompletion(30, &lEvCode);

    pEvent->Release();
}

int CMovieWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (CoCreateInstance(CLSID_EasyUtility, NULL, CLSCTX_INPROC_SERVER, 
	//					 IID_IEasyUtility, (void**)&m_pEasyUtility) != S_OK)
	//{
	//	W_SHOW_ERROR(IDS_ERROR_COM_EASY_UTIL, this);
	//	m_pEasyUtility = NULL;
	//}

	if (CoCreateInstance(CLSID_EasyMultiPlay, NULL, CLSCTX_INPROC_SERVER, 
						 IID_IEasyMultiPlay, (void**)&m_pMixPlay) != S_OK)
	{
		W_SHOW_ERROR(IDS_ERROR_COM_EASY_PLAY, this);
		m_pMixPlay = NULL;
	}
	// Modify  by angf
	if ( NULL != m_pMixPlay )
	{
		HRESULT hr = m_pMixPlay->Open();
		if (FAILED(hr))
			return FALSE;
		m_pMixPlay->SetupExportProgressHandler((long)(AfxGetMainWnd()->m_hWnd), (long)(WM_APP + 6));
	} 
	return FALSE;
}
 

void CMovieWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!::IsWindow(m_pBmpStatus->m_hWnd))
		return;

	CSize csBmp = GetBmpDim(m_nPlayID);
	csBmp.cx /= 4;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc;
	rc.bottom = rcClient.bottom - m_nLargeGap;
	rc.top = rc.bottom - constStatusHeight;
	rc.left = rcClient.left + m_nLargeGap;
	rc.right = rcClient.right - m_nLargeGap;
	SetCtrlPos(0, rc);
	m_rcStatusBar = rc;

	rc.bottom = rc.top - m_nSmallGap;
	rc.top = rc.bottom - csBmp.cy;
	rc.left = rcClient.left + m_nLargeGap;
	rc.right = rc.left + csBmp.cx;
	SetCtrlPos(1, rc);

	rc.left = rc.right + constSmallGap;
	rc.right = rc.left + csBmp.cx;
	SetCtrlPos(2, rc);

	rc.left = rc.right + constSmallGap;
	rc.right = rc.left + csBmp.cx;
	SetCtrlPos(4, rc);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_CUT_STATUS);
	BITMAP b;
	bitmap.GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;
	rc.right = rcClient.right - m_nLargeGap;
	rc.left = rc.right - nBmpWidth;
	rc.bottom = rc.top + nBmpWidth;
	m_pBmpStatus->MoveWindow(rc);
	m_pBmpStatusRep->MoveWindow(rc);

	CRect rcVideo;
	GetVideoRect(rcVideo);

}

/*
BOOL CMovieWnd::GetScript(CString strFileName) 
{
	m_strArrayCmd.RemoveAll();
	m_strArrayScript.RemoveAll();
	m_strArrayTime.RemoveAll();
	if (strFileName == "")
		return TRUE;

	CMediaUtils* pMedia = new CMediaUtils;
	if (!pMedia->Create())
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->Open(strFileName))
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->GetScriptNames(&m_strArrayCmd, &m_strArrayScript, &m_strArrayTime))
	{
		delete pMedia;
		return FALSE;
	}	

	int nSize = m_strArrayTime.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		for (int j = i + 1; j < nSize; j++)
		{
			if (m_strArrayTime[i].CompareNoCase(m_strArrayTime[j]) > 0)
			{
				CString strTmp = m_strArrayTime[i];
				m_strArrayTime[i] = m_strArrayTime[j];
				m_strArrayTime[j] = strTmp;

				strTmp = m_strArrayCmd[i];
				m_strArrayCmd[i] = m_strArrayCmd[j];
				m_strArrayCmd[j] = strTmp;

				strTmp = m_strArrayScript[i];
				m_strArrayScript[i] = m_strArrayScript[j];
				m_strArrayScript[j] = strTmp;
			}

		}
	}

	delete pMedia;
	return TRUE;
}
*/

int CMovieWnd::FindSlideIndex(CString strTime)
{
	int nLen = m_pstrArrayTime->GetSize();
	int nLower = 0;
	int nUpper = nLen - 1;
	CString strTmpTime, strTmpTime2;
	while (TRUE)
	{
		if (nLower >= nUpper)
			return nLower;
			
		int nIndex = (nLower + nUpper) / 2;
		if (nIndex == nLen - 1)
			return nLen - 1;
		strTmpTime = m_pstrArrayTime->GetAt(nIndex);
		strTmpTime2 = m_pstrArrayTime->GetAt(nIndex + 1);
		if (strTime >= strTmpTime && strTime < strTmpTime2) 	
			return nIndex;
		if (strTime < strTmpTime)
			nUpper = nIndex - 1;
		else if (strTime > strTmpTime2)
			nLower = nIndex + 1;
		else if (strTime == strTmpTime2)
			return nIndex + 1;	
	}
}

//void CMovieWnd::SetLength(long lTime, BOOL bRefresh)
//{
//	m_lTimeLength = lTime;
//	if (bRefresh)
//		RefreshStatusBar();
//}

CString CMovieWnd::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

void CMovieWnd::StopAndBack()
{
	PressButton(IDC_STOP);
}

BOOL CMovieWnd::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();

	GetVideoRect(rc);
	brBg.CreateSolidBrush(RGB(0, 0, 0));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();

	for (int i = 0; i < m_pCtrlArray->GetSize(); i++)
	{
		CCtrlInfo* pInfo = (CCtrlInfo*)m_pCtrlArray->GetAt(i);
		DrawCtrl(&dc, pInfo);
	}
	
	return TRUE;
}

BOOL CMovieWnd::SaveVideo(CString strOutput, CString strBase, int bSaveFast) 
{
	//CComBSTR bstrProfile(strProfile); 
	CComBSTR bstrBase(strBase);
	CComBSTR bstrOutput(strOutput);
	VARIANT_BOOL vbAudioFast;
	VARIANT_BOOL vbVideoFast;

	{
		//wangdebug
		//if (::FileExists(strProfile))
		{
			HRESULT hr = m_pMixPlay->ExportVideo(/*bstrProfile*/bstrBase, bstrOutput);
			if (SUCCEEDED(hr))
				return TRUE;
		}
	}

	return FALSE;

}

BOOL CMovieWnd::SaveAudio(CString strOutput, CString strProfile, int bSaveFast) 
{
	CComBSTR bstrProfile(strProfile);
	CComBSTR bstrOutput(strOutput);
	VARIANT_BOOL vbAudioFast;
	VARIANT_BOOL vbVideoFast;

	return FALSE;
}

BOOL CMovieWnd::AdjustAudio(long lFunc, long lValue) 
{
	if (lValue != 0)
		SkipFastMode();

	HRESULT hr = m_pMixPlay->AdjustAudio(lFunc, lValue);
	if (FAILED(hr))
		return FALSE;

	long lmsTime = GetCurrentPos();
	SetFilePosition(lmsTime);

	return TRUE;
}

BOOL CMovieWnd::AdjustVideo(long lFunc, long lValue) 
{
	if (lValue != 0)
		SkipFastMode();

	HRESULT hr = m_pMixPlay->AdjustVideo(lFunc, lValue);
	if (SUCCEEDED(hr))
		return TRUE;
	return FALSE;
}

void CMovieWnd::SkipFastMode()
{
	if (!m_pMixPlay)
		return;

}

BOOL CMovieWnd::TestVideoFile(CString strVideoPath)
{
	strVideoPath = "0," + strVideoPath + ",0,3000;";
	CComBSTR cbstrVideoPath(strVideoPath);
	CComBSTR cbstrNull("");
	HRESULT hr = m_pMixPlay->ChangeMainStream(cbstrVideoPath);
	m_pMixPlay->ChangeMainStream(cbstrNull);
	if (hr == NO_VOICE)
	{
		return FALSE;
	}
	return TRUE;
}
