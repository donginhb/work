// NumberEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "eLearning.h"
#include "NumberEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumberEdit

CNumberEdit::CNumberEdit()
{
}

CNumberEdit::~CNumberEdit()
{
}


BEGIN_MESSAGE_MAP(CNumberEdit, CEdit)
	//{{AFX_MSG_MAP(CNumberEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumberEdit message handlers

BOOL CNumberEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	dwStyle |= ES_RIGHT;
	BOOL bRet = CreateEx("edit", "", dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CNumberEdit::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), 
						(pParentWnd) ? pParentWnd->m_hWnd: NULL, 
						(HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

void CNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
		GetParent()->SendMessage(WM_ENTER_PRESSED);
		return;
	}

	if ((nChar > '9' || nChar <'0') && (nChar != VK_BACK)) 
		return;
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

