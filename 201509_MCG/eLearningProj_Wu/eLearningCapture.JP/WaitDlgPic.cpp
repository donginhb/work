// WaitDlgPic.cpp: CWaitDlgPic クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "WaitDlgPic.h"
#include "Resource.h"
#include "StdAfx.h"
//#include "jpgapi.h"
#include "cmkapi.h"
#include "bmpapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CWaitDlgPic::CWaitDlgPic()
{
	m_Waitbitmap = NULL;
	m_WbitWidth = 0;
	m_WbitHeight = 0;

	// 実行ファイルのフルパス取得
	char szPath[_MAX_PATH];
	GetModuleFileName( NULL, szPath, MAX_PATH);

	// フルパスを分解
	char szDir[_MAX_DIR];
	char szDrive[_MAX_DRIVE];
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(szPath,szDrive,szDir,szFName,szExt);

	CString P1 = szDrive;
	CString P2 = szDir;
	CString P4 = P1 + P2 + "waitPic.jpg";

	m_Waitbitmap = ::LoadFile(P4);

	BITMAP b;
	m_Waitbitmap->GetBitmap(&b);
	m_WbitWidth = b.bmWidth;
	m_WbitHeight = b.bmHeight;
}

CWaitDlgPic::~CWaitDlgPic()
{
	if (m_Waitbitmap)
	{
		m_Waitbitmap->DeleteObject();
		delete m_Waitbitmap;
	}
}

BEGIN_MESSAGE_MAP(CWaitDlgPic, CWnd)
	//{{AFX_MSG_MAP(CWaitDlgPic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWaitDlgPic::Create()
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
                        m_WbitWidth, m_WbitHeight,                           // Size - updated soon
                        pParentWnd->GetSafeHwnd(),            // handle to parent
                        0,                                 // No menu
                        NULL);    
    if (!bSuccess) return FALSE;
	
	CenterWindow();

	ShowWindow(SW_SHOW);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return TRUE;
}

void CWaitDlgPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcDC;
	GetClientRect(rcDC);
	::DDBPaint(dc.m_hDC, rcDC, (HBITMAP)m_Waitbitmap->m_hObject, rcDC, NULL);
}
