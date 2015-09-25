// ToolTipWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "ToolTipWnd.h"
#include "MemDC.h"

#ifdef _DEBUG 
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd
CString CToolTipWnd::m_strClass = "";

CToolTipWnd::CToolTipWnd()
{
}

CToolTipWnd::~CToolTipWnd()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CToolTipWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd message handlers

BOOL CToolTipWnd::Create(CWnd* pParentWnd)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, WS_POPUP | WS_BORDER, CRect(0, 0, 0, 0), 
						 pParentWnd, 0, NULL);
	if (bRet)
		SetOwner(pParentWnd);
	return bRet;
}

BOOL CToolTipWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(NULL, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), 
						pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CToolTipWnd::OnEraseBkgnd(CDC* pSDC) 
{
	CMemDC_MCG dc(pSDC);
	CDC* pDC = &dc;

	CBrush backBrush(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);

	CFont* pOldFont = pDC->SelectObject(m_pFont);
	pDC->SetTextColor(::GetSysColor(COLOR_INFOTEXT));
	pDC->SetBkMode(TRANSPARENT);
	
	CRect rcClient;
	GetClientRect(rcClient);

	pDC->DrawText(m_strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	return CWnd::OnEraseBkgnd(pDC);
}

void CToolTipWnd::ShowText(CPoint pt, CString strText) 
{
	m_strText = strText;
	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(m_pFont);
	CSize cs = pDC->GetOutputTextExtent(strText, strText.GetLength());
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nWidth = cs.cx + 3 * 2;
	int nHeight = cs.cy + 2;
	if (pt.x + nWidth > nScreenX - 1)
		pt.x = nScreenX - 1 - nWidth;
	SetWindowPos(NULL, pt.x, pt.y, nWidth, nHeight, 
				 SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);   
	RedrawWindow();
}