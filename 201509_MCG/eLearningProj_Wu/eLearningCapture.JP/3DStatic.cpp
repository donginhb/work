// 3DStatic.cpp : implementation file
//

#include "stdafx.h"
#include "eLearningCapture.h"
#include "3DStatic.h"
#include "..\elearning.jp\MemDC.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DStatic

CString C3DStatic::m_strClass = "";

C3DStatic::C3DStatic()
{
}

C3DStatic::~C3DStatic()
{
}


BEGIN_MESSAGE_MAP(C3DStatic, CWnd)
	//{{AFX_MSG_MAP(C3DStatic)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// C3DStatic message handlers
BOOL C3DStatic::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, "", dwStyle, rect, pParentWnd, 0, NULL);

	return bRet;
}

BOOL C3DStatic::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(0, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), 
						(pParentWnd) ? pParentWnd->m_hWnd: NULL, 
						(HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL C3DStatic::OnEraseBkgnd(CDC* pOrigDC) 
{
	CMemDC_MCG dc(pOrigDC);
	CDC* pDC = &dc;
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	CPen penDGray, penLGray;
	penDGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	penLGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHILIGHT));
	
	CRect rcDraw;
	GetClientRect(rcDraw);
	rcDraw.right--;
	rcDraw.bottom--;

	CPen* pOldPen = (CPen*)pDC->SelectObject(&penDGray);
	pDC->MoveTo(rcDraw.left, rcDraw.bottom);
	pDC->LineTo(rcDraw.left, rcDraw.top);
	pDC->LineTo(rcDraw.right, rcDraw.top);

	pDC->SelectObject(&penLGray);
	pDC->MoveTo(rcDraw.right, rcDraw.top);
	pDC->LineTo(rcDraw.right, rcDraw.bottom);
	pDC->LineTo(rcDraw.left - 1, rcDraw.bottom);

	CFont* pOldFont = pDC->SelectObject(m_pFont);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	rcDraw.left += 4;
	pDC->DrawText(m_strText, rcDraw, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	rcDraw.right -= 4;
	pDC->SelectObject(pOldFont);

	pDC->SelectObject(pOldPen);
	penDGray.DeleteObject();
	penLGray.DeleteObject();

	return TRUE;
}
