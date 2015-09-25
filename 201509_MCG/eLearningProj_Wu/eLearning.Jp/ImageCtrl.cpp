// ImageCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "ImageCtrl.h"
#include "MemDC.h"
#include "BmpApi.h"
#include "JPGApi.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageCtrl
CString CImageCtrl::m_strClass = "";
CImageCtrl::CImageCtrl()
{
	m_pBMP = NULL;
}

CImageCtrl::~CImageCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageCtrl, CWnd)
	//{{AFX_MSG_MAP(CImageCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageCtrl message handlers

int CImageCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//m_LBitmapWnd.SetWndHandle(GetSafeHwnd());	
	//m_LBitmapWnd.SetPatternBackColor(RGB(255, 255, 255));

	return 0;
}

BOOL CImageCtrl::LoadFile(LPCTSTR strFileName, BOOL bForce)
{
	if (m_strFileName == strFileName && !bForce)
		return TRUE;

	if (m_pBMP)
	{
		m_pBMP->DeleteObject();
		delete m_pBMP;
		m_pBMP = NULL;
	}

	if (strFileName[0] == '\0' || _access(strFileName, 0) != 0)   // Check for existence
	{
		m_strFileName = "";
		RedrawWindow(NULL, NULL);
		return FALSE;
	}

	m_strFileName = strFileName;
	m_pBMP = ::LoadFile(strFileName);
	RedrawWindow(NULL, NULL);

	//m_LBitmapWnd.SetFileName((L_CHAR *) strFileName);

	//if(m_LBitmapWnd.Load(24)!=SUCCESS)
	//{
	//	m_LBitmapWnd.DisplayErrorFromList(NULL);
	//	return FALSE;
	//}
	//m_LBitmapWnd.SetZoomMode(ZOOM_FIT);
	
	return TRUE;
}

BOOL CImageCtrl::Create(DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, dwExStyle, rect, pParentWnd, 0, NULL);

	return bRet;
}

BOOL CImageCtrl::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

void CImageCtrl::DrawPictureFrame(CDC* pDC, CRect rc)
{
	rc.right--;
	rc.bottom--;

	CPen penLight, penDark;
	penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	CRect rcSq = rc;

	CPen* pOldPen;
//	if (m_nStatus == constBtnNormal)
//		pOldPen = pDC->SelectObject(&penLight);
//	else
		pOldPen = pDC->SelectObject(&penDark);
	pDC->MoveTo(rcSq.left, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.top);
	pDC->LineTo(rcSq.right, rcSq.top);
//	if (m_nStatus == constBtnNormal)
//		pDC->SelectObject(&penDark);
//	else
		pDC->SelectObject(&penLight);
	pDC->LineTo(rcSq.right, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.bottom);

	pDC->SelectObject(pOldPen);
	penLight.DeleteObject();
	penDark.DeleteObject();
}

BOOL CImageCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG memDC(pDC);

	CRect rcClient;
	GetClientRect(rcClient);
	DrawPictureFrame(&memDC, rcClient);

	rcClient.InflateRect(-1, -1, -1, -1);

	CBrush br(RGB(255, 251, 240));
	memDC.FillRect(rcClient, &br);

	if (m_pBMP)
	{
		BITMAP b;
		m_pBMP->GetBitmap(&b);
		int nBmpWidth = b.bmWidth;
		int nBmpHeight = b.bmHeight;

		CRect rcDC, rcDDB;
		rcDDB.left = 0;
		rcDDB.top = 0;
		rcDDB.right = nBmpWidth - 1;
		rcDDB.bottom = nBmpHeight - 1;
		
		int nClientWidth = rcClient.Width();
		int nClientHeight = rcClient.Height();
		double dBmpRatio = (double)nBmpWidth / (double)nBmpHeight;
		double dDCRatio = (double)nClientWidth / (double)nClientHeight;
		if (dBmpRatio > dDCRatio)
		{
			rcDC.left = rcClient.left;
			rcDC.right = rcDC.left + nClientWidth ;
			int nH = (int)(nClientWidth / dBmpRatio);
			rcDC.top = rcClient.top + (nClientHeight - nH) / 2;
			rcDC.bottom = rcDC.top + nH;
		}
		else
		{
			rcDC.top = rcClient.top;
			rcDC.bottom = rcDC.top + nClientHeight;
			int nW = (int)(nClientHeight * dBmpRatio);
			rcDC.left = rcClient.left + (nClientWidth - nW) / 2;
			rcDC.right = rcDC.left + nW;
		}

		DDBPaint(memDC.m_hDC, rcDC, (HBITMAP)(m_pBMP->m_hObject), rcDDB, NULL);
	}

	return CWnd::OnEraseBkgnd(pDC);
}

void CImageCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pBMP)
	{
		m_pBMP->DeleteObject();
		delete m_pBMP;
		m_pBMP = NULL;
	}
}
