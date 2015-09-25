// ToolPaleteVideoEdit.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "ToolPaleteVideoEdit.h"
#include "MemDC.h"
#include "BmpBtn.h"
#include "cmkapi.h"
#include "BitmapMenu.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolPaleteVideoEdit

CToolPaleteVideoEdit::CToolPaleteVideoEdit()
{
	m_csBtn = ::GetBmpDim(IDB_MARK);
	m_csBtn.cx /= 4;
}

CToolPaleteVideoEdit::~CToolPaleteVideoEdit()
{
}


BEGIN_MESSAGE_MAP(CToolPaleteVideoEdit, CWnd)
	//{{AFX_MSG_MAP(CToolPaleteVideoEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolPaleteVideoEdit message handlers

BOOL CToolPaleteVideoEdit::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();

	COLORREF crDkGray = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF crWhite = ::GetSysColor(COLOR_3DHILIGHT);
	COLORREF crBlack = ::GetSysColor(COLOR_3DDKSHADOW);
	CPen penWhite(PS_SOLID, 1, crWhite);
	CPen penDkGray(PS_SOLID, 1, crDkGray);
	CPen penBlack(PS_SOLID, 1, crBlack);

	CRect rcClient;
	GetClientRect(rcClient);

	CPen* pOldPen = dc.SelectObject(&penDkGray);
	dc.MoveTo(0, rcClient.bottom - 2);
	dc.LineTo(0, 0);
	dc.LineTo(rcClient.right - 1, 0);
	
	dc.SelectObject(&penWhite);
	dc.MoveTo(rcClient.right - 1, 0);
	dc.LineTo(rcClient.right - 1, rcClient.bottom - 1);
	dc.LineTo(-1, rcClient.bottom - 1);

	dc.SelectObject(pOldPen);

	return TRUE;
}

int CToolPaleteVideoEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	rect.left = 2;
	rect.right = rect.left + m_csBtn.cx;
	rect.top = 2;
	rect.bottom = rect.top + m_csBtn.cy;
	
	m_pBtnMark = new CBmpBtn(IDB_MARK, BTN_PUSH_BUTTON);
	m_pBtnMark->SetBorderType(2);
	m_pBtnMark->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_MARK);

#ifdef _BUNDLE
	m_pBtnMark->EnableButton(FALSE);
#endif

	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;

	m_pBtnCut = new CBmpBtn(IDB_CUT, BTN_PUSH_BUTTON);
	m_pBtnCut->SetBorderType(2);
	m_pBtnCut->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_DELETE);
#ifdef _BUNDLE
	m_pBtnCut->EnableButton(FALSE);
#endif

	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnInsert = new CBmpBtn(IDB_INSERT, BTN_PUSH_BUTTON);
	m_pBtnInsert->SetBorderType(2);
	m_pBtnInsert->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_REPLACE);
#ifdef _BUNDLE
	m_pBtnInsert->EnableButton(FALSE);
#endif

	rect.left = 2;
	rect.right = rect.left + m_csBtn.cx;
	rect.top = 3 + m_csBtn.cy;
	rect.bottom = rect.top + m_csBtn.cy;
	m_pBtnAddEvent = new CBmpBtn(IDB_ADD_EVENT, BTN_PUSH_BUTTON);
	m_pBtnAddEvent->SetBorderType(2);
	m_pBtnAddEvent->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ADD_EVENT);
#ifdef _BUNDLE
	//Shien Modify 20051014 for aenrich
	//m_pBtnAddEvent->EnableButton(FALSE);
	#ifndef _AENRICH
		m_pBtnAddEvent->EnableButton(FALSE);
	#else
		m_pBtnAddEvent->EnableButton(TRUE);
	#endif

#endif

	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnAddWave = new CBmpBtn(IDB_ADD_WAVE, BTN_PUSH_BUTTON);
	m_pBtnAddWave->SetBorderType(2);
	m_pBtnAddWave->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ADD_WAVE);
#ifdef _BUNDLE
	m_pBtnAddWave->EnableButton(FALSE);
#endif

	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnAddText = new CBmpBtn(IDB_ADD_TEXT, BTN_PUSH_BUTTON);
	m_pBtnAddText->SetBorderType(2);
	m_pBtnAddText->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ADD_TEXT);
#ifdef _BUNDLE
	m_pBtnAddText->EnableButton(FALSE);
#endif

	rect.left = 2;
	rect.right = rect.left + m_csBtn.cx;
	rect.top = 3 + m_csBtn.cy * 2;
	rect.bottom = rect.top + m_csBtn.cy;
	m_pBtnAddEffect = new CBmpBtn(IDB_ADD_EFFECT, BTN_PUSH_BUTTON);
	m_pBtnAddEffect->SetBorderType(2);
	m_pBtnAddEffect->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ADD_EFFECT);
#ifdef _BUNDLE
	m_pBtnAddEffect->EnableButton(FALSE);
#endif

	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnMute = new CBmpBtn(IDB_MUTE, BTN_PUSH_BUTTON);
	m_pBtnMute->SetBorderType(2);
	m_pBtnMute->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_MUTE);
#ifdef _BUNDLE
	m_pBtnMute->EnableButton(FALSE);
#endif


	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnEditMark = new CBmpBtn(IDB_EDIT, BTN_PUSH_BUTTON);
	m_pBtnEditMark->SetBorderType(2);
	m_pBtnEditMark->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_EDITMARK);
#ifdef _BUNDLE
	//Shien Modify 20051014 for aenrich
	//m_pBtnEditMark->EnableButton(FALSE);
	#ifndef _AENRICH
		m_pBtnEditMark->EnableButton(FALSE);
	#else
		m_pBtnEditMark->EnableButton(TRUE);
	#endif

#endif

	rect.left = 2;
	rect.right = rect.left + m_csBtn.cx;
	rect.top = 3 + m_csBtn.cy * 3;
	rect.bottom = rect.top + m_csBtn.cy;
	m_pBtnSetPoints = new CBmpBtn(IDB_SET_POINTS, BTN_PUSH_BUTTON);
	m_pBtnSetPoints->SetBorderType(2);
	m_pBtnSetPoints->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_SET_POINTS);
#ifdef _BUNDLE
	m_pBtnSetPoints->EnableButton(FALSE);
#endif
	
	rect.left = rect.right + 1;
	rect.right = rect.left + m_csBtn.cx;
	m_pBtnAdjust = new CBmpBtn(IDB_ADJUST, BTN_PUSH_BUTTON);
	m_pBtnAdjust->SetBorderType(2);
	m_pBtnAdjust->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ADJUST);
#ifdef _BUNDLE
	m_pBtnAdjust->EnableButton(FALSE);
#endif

	//rect.left = rect.right + 1;
	//rect.right = rect.left + m_csBtn.cx;
	//m_pBtnEditSave = new CBmpBtn(IDB_GO, BTN_PUSH_BUTTON);
	//m_pBtnEditSave->SetBorderType(2);
	//m_pBtnEditSave->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_SAVE_VIDEO);

	return 0;
}

void CToolPaleteVideoEdit::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pBtnMark->DestroyWindow();
	delete m_pBtnMark;

	m_pBtnCut->DestroyWindow();
	delete m_pBtnCut;

	m_pBtnInsert->DestroyWindow();
	delete m_pBtnInsert;

	m_pBtnAddEvent->DestroyWindow();
	delete m_pBtnAddEvent;

	m_pBtnEditMark->DestroyWindow();
	delete m_pBtnEditMark;

	//m_pBtnEditSave->DestroyWindow();
	//delete m_pBtnEditSave;

	m_pBtnAddWave->DestroyWindow();
	delete m_pBtnAddWave;

	m_pBtnAddText->DestroyWindow();
	delete m_pBtnAddText;

	m_pBtnAddEffect->DestroyWindow();
	delete m_pBtnAddEffect;

	m_pBtnMute->DestroyWindow();
	delete m_pBtnMute;

	m_pBtnAdjust->DestroyWindow();
	delete m_pBtnAdjust;

	m_pBtnSetPoints->DestroyWindow();
	delete m_pBtnSetPoints;
}

LRESULT CToolPaleteVideoEdit::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_BTN_ADD_EVENT)
	{
		CMenu menuTmp;
		menuTmp.LoadMenu(IDR_MENU_ADD_EVENT);
		CMenu* pSubMenu = menuTmp.GetSubMenu(0);
		
		if (BUNDLE_VERSION)
		{
			//pSubMenu->DeleteMenu(0, MF_BYPOSITION);
			//pSubMenu->DeleteMenu(1, MF_BYPOSITION);

			//pSubMenu->DeleteMenu(4, MF_BYPOSITION);
			#ifndef _AENRICH
				pSubMenu->DeleteMenu(4, MF_BYPOSITION);
			#endif
		}

		CBitmapMenu menuEditEvent;
		menuEditEvent.Attach(pSubMenu->GetSafeHmenu());

		menuEditEvent.SetMenuFont(MACRO_SYS_FONT);
		//if (!_BUNDLE)
		//{
			menuEditEvent.AddImage(IDB_ZOOM_IN);
			menuEditEvent.AddImage(IDB_ZOOM_OUT);
		//}

		//menuEditEvent.AddImage(IDB_WEB);
		menuEditEvent.AddImage(IDB_ADD_DEMO);
		menuEditEvent.CreateImage();

		CRect rcPos;
		m_pBtnAddEvent->GetWindowRect(rcPos);
		UINT nCmd = menuEditEvent.TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | 
												 TPM_RIGHTBUTTON | TPM_RETURNCMD, 
												 rcPos.right, rcPos.bottom, this);
		if (nCmd == 0)
			return 0L;

		lParam = nCmd;
	}
	else if (wParam == ID_BTN_ADD_WAVE)
	{
		//if (_BUNDLE)
		//{
		//	W_SHOW_WARNING(IDS_NOT_AVAILABLE_FOR_TRIAL, this);
		//	return 0L;
		//}
		
		CMenu menuTmp;
		menuTmp.LoadMenu(IDR_MENU_ADD_WAVE);
		CMenu* pSubMenu = menuTmp.GetSubMenu(0);
		
		CBitmapMenu menuEditEvent;
		menuEditEvent.Attach(pSubMenu->GetSafeHmenu());

		menuEditEvent.SetMenuFont(MACRO_SYS_FONT);
		menuEditEvent.AddImage(IDB_REPLACE_WAVE);
		menuEditEvent.AddImage(IDB_MIX_WAVE);
		menuEditEvent.CreateImage();

		CRect rcPos;
		m_pBtnAddWave->GetWindowRect(rcPos);
		UINT nCmd = menuEditEvent.TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | 
												 TPM_RIGHTBUTTON | TPM_RETURNCMD, 
												 rcPos.right, rcPos.bottom, this);
		if (nCmd == 0)
			return 0L;

		lParam = nCmd;
	}
	else if (wParam == ID_BTN_MUTE || wParam == ID_BTN_DELETE || wParam == ID_BTN_REPLACE || 
		     wParam == ID_BTN_ADD_TEXT || wParam == ID_BTN_ADD_EFFECT || wParam == ID_BTN_SET_POINTS)
	{
		//if (_BUNDLE)
		//{
		//	W_SHOW_WARNING(IDS_NOT_AVAILABLE_FOR_TRIAL, this);
		//	return 0L;
		//}
	}
	

	GetParent()->SendMessage(WM_PALETE_BTN_DOWN, wParam, lParam);
	return  0L;
}