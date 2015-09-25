// WndLogo.cpp : implementation file
//

#include "stdafx.h"
//#include "eLearning.h"
#include "WndLogo.h"
#include "cmkapi.h"
#include "jpgapi.h"
#include "bmpapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndLogo

CWndLogo::CWndLogo(CString StrLogoPath)
{
	m_pBitmap = NULL;
	m_nBmpWidth = 0;
	m_nBmpHeight = 0;
	if (::FileExists(StrLogoPath))
	{ 
		//m_pBitmap = new CBitmap;
		//HBITMAP hBMP = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		//							 StrLogoPath, IMAGE_BITMAP, 0, 0, 
		//							 LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		//m_pBitmap->Attach(hBMP);

		m_pBitmap = ::LoadFile(StrLogoPath);

		BITMAP b;
		m_pBitmap->GetBitmap(&b);
		m_nBmpWidth = b.bmWidth;
		m_nBmpHeight = b.bmHeight;
	}
}

CWndLogo::~CWndLogo()
{
	if (m_pBitmap)
	{
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
	}
}


BEGIN_MESSAGE_MAP(CWndLogo, CWnd)
	//{{AFX_MSG_MAP(CWndLogo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndLogo message handlers

BOOL CWndLogo::Create() 
{
    // Register window class
    CString csClassName = AfxRegisterWndClass(CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
                                              ::LoadCursor(NULL, IDC_APPSTARTING),
                                              CBrush(::GetSysColor(COLOR_BTNFACE)));

    // If no parent supplied then try and get a pointer to it anyway
    CWnd* pParentWnd = AfxGetMainWnd();

	// Create popup window
    BOOL bSuccess = CreateEx(WS_EX_TOPMOST, // Extended style
                        csClassName,                        // Classname
                        "",									// Title
                        WS_POPUP,				            // style
                        0, 0 ,                               // position - updated soon.
                        m_nBmpWidth, m_nBmpHeight,                           // Size - updated soon
                        pParentWnd->GetSafeHwnd(),            // handle to parent
                        0,                                 // No menu
                        NULL);    
    if (!bSuccess) return FALSE;
	
	CenterWindow();

	ShowWindow(SW_SHOW);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return TRUE;
}

void CWndLogo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcDC;
	GetClientRect(rcDC);
	::DDBPaint(dc.m_hDC, rcDC, (HBITMAP)m_pBitmap->m_hObject, rcDC, NULL);
}
