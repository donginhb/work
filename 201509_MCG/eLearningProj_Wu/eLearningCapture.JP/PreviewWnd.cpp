// PreviewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "eLearningCapture.h"
#include "PreviewWnd.h"
#include "..\elearning.jp\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd

CString CPreviewWnd::m_strClass = "";

CPreviewWnd::CPreviewWnd()
{
	m_pMSGWnd = NULL;
//	m_pMenu = new CMenu;
//	m_pMenu->LoadMenu(IDR_PREVIEW_MENU);
	m_bPreviewAudioMute = TRUE;
}

CPreviewWnd::~CPreviewWnd()
{
//	delete m_pMenu;
}


BEGIN_MESSAGE_MAP(CPreviewWnd, CWnd)
	//{{AFX_MSG_MAP(CPreviewWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_PREVIEW_VOICE, OnPreviewVoice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd message handlers

BOOL CPreviewWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	CString strTitle;
	strTitle.LoadString(IDS_PREVIEW);
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, strTitle, dwStyle, rect, pParentWnd, 0, NULL);

	return bRet;
}

BOOL CPreviewWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), 
						(pParentWnd) ? pParentWnd->m_hWnd: NULL, NULL))
						//(HMENU)m_pMenu->m_hMenu))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CPreviewWnd::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	return TRUE;
}

void CPreviewWnd::OnClose() 
{
	if (m_pMSGWnd)
		m_pMSGWnd->SendMessage(WM_CLOSE_PREVIEW);
}


int CPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CPreviewWnd::OnPreviewVoice() 
{
	if (m_pMSGWnd)
	{
		CMenu* pMenu = m_pMenu->GetSubMenu(0);
		if (m_bPreviewAudioMute)
			pMenu->CheckMenuItem(0, MF_BYPOSITION | MF_CHECKED); 
		else
			pMenu->CheckMenuItem(0, MF_BYPOSITION | MF_UNCHECKED); 

		m_pMSGWnd->SendMessage(WM_SET_AUDIO_PREVIEW);
	}
}

