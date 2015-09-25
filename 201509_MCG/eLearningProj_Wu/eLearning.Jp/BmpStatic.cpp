// BmpStatic.cpp : implementation file
//

#include "stdafx.h"
#include "BmpStatic.h"
#include "BmpApi.h"
#include "MemDC.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	HIMETRIC_INCH	2540

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic
CString CBmpStatic::m_strClass = "";

CBmpStatic::CBmpStatic()
{
	m_pimageList = NULL;
	m_bBorder = TRUE;
}

CBmpStatic::CBmpStatic(UINT nBmpID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nBmpID);

	BITMAP b;
	bitmap.GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;
	
	m_pimageList = new CImageList;
	m_pimageList->Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_pimageList->Add(&bitmap, RGB(255, 0, 255));
	
	m_nHeight = nBmpHeight;
	m_nWidth = nBmpWidth;

	m_bBorder = TRUE;
}

CBmpStatic::~CBmpStatic()
{
	delete m_pimageList;
}


BEGIN_MESSAGE_MAP(CBmpStatic, CWnd)
	//{{AFX_MSG_MAP(CBmpStatic)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBmpStatic message handlers

void CBmpStatic::SetImage(CString strPath)
{
	if (_access(strPath, 0) != 0)
		return;

	CBitmap* pBMP = ::LoadFile(strPath);
	BITMAP b;
	pBMP->GetBitmap(&b);
	m_nHeight = b.bmHeight;
	m_nWidth = b.bmWidth;

	if (m_pimageList)
		delete m_pimageList;

	m_pimageList = new CImageList;
	m_pimageList->Create(m_nWidth, m_nHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_pimageList->Add(pBMP, RGB(255, 0, 255));
	
	pBMP->DeleteObject();
	delete pBMP;
}

BOOL CBmpStatic::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, "", dwStyle, rect, pParentWnd, 0, NULL);

	return bRet;
}

BOOL CBmpStatic::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

BOOL CBmpStatic::OnEraseBkgnd(CDC* pOrigDC) 
{
	CMemDC_MCG dc(pOrigDC);
	CDC* pDC = &dc;
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	CRect rcDraw;
	GetClientRect(rcDraw);
	rcDraw.right--;
	rcDraw.bottom--;

	CPen* pOldPen;
	CPen penDGray, penLGray;
	if (m_bBorder)
	{
		penDGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
		penLGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHILIGHT));

		pOldPen = (CPen*)pDC->SelectObject(&penDGray);
		pDC->MoveTo(rcDraw.left, rcDraw.bottom);
		pDC->LineTo(rcDraw.left, rcDraw.top);
		pDC->LineTo(rcDraw.right, rcDraw.top);

		pDC->SelectObject(&penLGray);
		pDC->MoveTo(rcDraw.right, rcDraw.top);
		pDC->LineTo(rcDraw.right, rcDraw.bottom);
		pDC->LineTo(rcDraw.left - 1, rcDraw.bottom);
	}

	int nX = (rc.Width() - m_nWidth) / 2;
	int nY = (rc.Height() - m_nHeight) / 2;
	m_pimageList->Draw(pDC, 0, CPoint(nX, nY), ILD_TRANSPARENT);

	if (m_bBorder)
	{
		pDC->SelectObject(pOldPen);
		penDGray.DeleteObject();
		penLGray.DeleteObject();
	}

	return TRUE;
}

