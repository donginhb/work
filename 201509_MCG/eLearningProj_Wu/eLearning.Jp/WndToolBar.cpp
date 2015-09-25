// WndToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "WndToolBar.h"
#include "MemDC.h"
#include "BmpBtn.h"
#include "Cmkapi.h"
#include "Mainfrm.h"
#include "Macro.h"
#include "eLearningView.h"
#include "HtmlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndToolBar

CString CWndToolBar::m_strClass = "";

CWndToolBar::CWndToolBar()
{

//	m_pSlideWnd = new CImageCtrl;
	
	m_pSlideWnd = new CHtmlWnd;

	BITMAP bBMP;

	CBitmap bitmapLeftup;
	bitmapLeftup.LoadBitmap(IDB_LEFT_UP);
	bitmapLeftup.GetBitmap(&bBMP);
	int nBmpWidth = bBMP.bmWidth;
	int nBmpHeight = bBMP.bmHeight;
	m_imageList_lefttop.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_lefttop.Add(&bitmapLeftup, RGB(255, 0, 255));

	CBitmap bitmapLeft;
	bitmapLeft.LoadBitmap(IDB_LEFT);
	bitmapLeft.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_left.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_left.Add(&bitmapLeft, RGB(255, 0, 255));

	CBitmap bitmapLeftBottom;
	bitmapLeftBottom.LoadBitmap(IDB_LEFT_BOTTOM);
	bitmapLeftBottom.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_leftbottom.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_leftbottom.Add(&bitmapLeftBottom, RGB(255, 0, 255));

	CBitmap bitmapBottom;
	bitmapBottom.LoadBitmap(IDB_BOTTOM);
	bitmapBottom.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_bottom.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_bottom.Add(&bitmapBottom, RGB(255, 0, 255));

	CBitmap bitmapRightBottom;
	bitmapRightBottom.LoadBitmap(IDB_RIGHT_BOTTOM);
	bitmapRightBottom.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_rightbottom.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_rightbottom.Add(&bitmapRightBottom, RGB(255, 0, 255));

	CBitmap bitmapRight;
	bitmapRight.LoadBitmap(IDB_RIGHT);
	bitmapRight.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_right.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_right.Add(&bitmapRight, RGB(255, 0, 255));

	CBitmap bitmapRightTop;
	bitmapRightTop.LoadBitmap(IDB_RIGHT_UP);
	bitmapRightTop.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_righttop.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_righttop.Add(&bitmapRightTop, RGB(255, 0, 255));

	CBitmap bitmapTop;
	bitmapTop.LoadBitmap(IDB_UP);
	bitmapTop.GetBitmap(&bBMP);
	nBmpWidth = bBMP.bmWidth;
	nBmpHeight = bBMP.bmHeight;
	m_imageList_top.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_imageList_top.Add(&bitmapTop, RGB(255, 0, 255));

	m_cslefttop = ::GetBmpDim(IDB_LEFT_UP);
	m_csleft = ::GetBmpDim(IDB_LEFT);
	m_csleftbottom = ::GetBmpDim(IDB_LEFT_BOTTOM);
	m_csbottom = ::GetBmpDim(IDB_BOTTOM);
	m_csrightbottom = ::GetBmpDim(IDB_RIGHT_BOTTOM);
	m_csright = ::GetBmpDim(IDB_RIGHT);
	m_csrighttop = ::GetBmpDim(IDB_RIGHT_UP);
	m_cstop = ::GetBmpDim(IDB_UP);

	m_strPreSlide = "";
	m_strPreCmd = "";
}

CWndToolBar::~CWndToolBar()
{

	delete m_pSlideWnd;
}


BEGIN_MESSAGE_MAP(CWndToolBar, CWnd)
	//{{AFX_MSG_MAP(CWndToolBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndToolBar message handlers

BOOL CWndToolBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CWndToolBar::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(NULL, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWndToolBar::OnEraseBkgnd(CDC* pSDC) 
{
   	CMemDC_MCG memDC(pSDC);
	CDC* pDC = &memDC;

	CBrush backBrush(GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);

	CRect rc;
	GetClientRect(rc);
	rc.bottom--;
	rc.right--;

	m_imageList_lefttop.Draw(pDC, 0, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
	int nStart = rc.left + m_cslefttop.cx;
	for (int i = 0;; i++)
	{
		m_imageList_top.Draw(pDC, 0, CPoint(nStart, rc.top), ILD_TRANSPARENT);
		nStart += m_cstop.cx;
		if (nStart + m_csrighttop.cx > rc.right)
			break;
	}
	m_imageList_righttop.Draw(pDC, 0, CPoint(rc.right - m_csrighttop.cx, rc.top), 
							  ILD_TRANSPARENT);

	nStart = rc.top + m_cslefttop.cy;
	for (int i = 0;; i++)
	{
		m_imageList_left.Draw(pDC, 0, CPoint(rc.left, nStart), ILD_TRANSPARENT);
		nStart += m_csleft.cy;
		if (nStart + m_csleftbottom.cy > rc.bottom)
			break;
	}
	m_imageList_leftbottom.Draw(pDC, 0, CPoint(rc.left, rc.bottom - m_csleftbottom.cy), 
							    ILD_TRANSPARENT);

	nStart = rc.left + m_csleftbottom.cx;
	for (int i = 0;; i++)
	{
		m_imageList_bottom.Draw(pDC, 0, CPoint(nStart, rc.bottom), ILD_TRANSPARENT);
		nStart += m_csbottom.cx;
		if (nStart + m_csrightbottom.cx > rc.right)
			break;
	}
	m_imageList_rightbottom.Draw(pDC, 0, 
								 CPoint(rc.right - m_csrightbottom.cx + 1, 
									    rc.bottom - m_csrightbottom.cy + 1), 
								 ILD_TRANSPARENT);

	nStart = rc.top + m_csrighttop.cy;
	for (int i = 0;; i++)
	{
		m_imageList_right.Draw(pDC, 0, CPoint(rc.right, nStart), ILD_TRANSPARENT);
		nStart += m_csright.cy;
		if (nStart + m_csrightbottom.cy > rc.bottom)
			break;
	}
	//m_imageList_rightbottom.Draw(pDC, 0, CPoint(rc.right, rc.bottom - m_csrightbottom.cy), 
	//						    ILD_TRANSPARENT);

	return TRUE;
}


int CWndToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_pSlideWnd->Create(this);
//	m_pSlideWnd->EnableWindow(FALSE);
	return 0;
}

void CWndToolBar::OnDestroy() 
{
	CWnd::OnDestroy();

	m_pSlideWnd->DestroyWindow();
}

void CWndToolBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);


	int nGap = 4;

	CRect rcSlide;
	rcSlide.left = 8;
	rcSlide.bottom = cy - 4;
	rcSlide.top = 8;
	rcSlide.right = cx - nGap;
	m_pSlideWnd->MoveWindow(rcSlide);

}

LRESULT CWndToolBar::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	long nID = wParam;
	//add 2013/11/26 kagata BugNo.90
	CWnd* pMainFrm = AfxGetMainWnd();

	switch (nID)
	{
		case ID_BTN_EDIT_PPT:
			MACRO_VIEW->OpenPptFile();
		break;

		case ID_BTN_CHANGE_PPT:
			MACRO_VIEW->ReplacePptFile();
			//add 2013/11/26 kagata BugNo.90
			pMainFrm->UpdateWindow();
		break;

		case ID_BTN_REGEN_PPT:
			MACRO_VIEW->RegenPptSlide(TRUE);
			//add 2013/11/26 kagata BugNo.90
			pMainFrm->UpdateWindow();
		break;
	}

	return 0L;
}

/*
CString CWndToolBar::FindJpegFilePrefix(LPCTSTR strCourse)
{
	CString strJpegPath = MACRO_JPEG_PATH(strCourse) + "*.jpg";
	WIN32_FIND_DATA FindInf;
	HANDLE hSearch = ::FindFirstFile(strJpegPath, &FindInf);
	if (hSearch == INVALID_HANDLE_VALUE)
		return "";  //Something Wrong??
	CString strFileName = FindInf.cFileName;
	int nIndex = strFileName.Find(".");
	if (nIndex <= 0)
	{
		::FindClose(hSearch);
		return "";
	}
	strFileName = strFileName.Mid(0, nIndex);
	for (int i = nIndex - 1; i >= 0; i--)
		if (strFileName[i] < '0' || strFileName[i] > '9')
			break;
	CString strPrefix = strFileName.Mid(0, i + 1);

	::FindClose(hSearch);
	return strPrefix;
}
*/

void CWndToolBar::RefreshSlide()
{
	m_pSlideWnd->Refresh();
}

void CWndToolBar::ShowSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce)
{
	if (strCourse == "")
	{
		m_pSlideWnd->Navigate2(MACRO_EMPTY_HTM);
		m_strPreSlide = "";
		m_strPreCmd = "";
		return;
	}

	if (strSlide == m_strPreSlide && strCmd == m_strPreCmd && !bForce)
		return;

	//CString strPrefix = FindJpegFilePrefix(strCourse);
	CString strSld = strSlide;
	if (strSld.Find(".") < 0)
	{
		CString strPath;
		if (MACRO_VIEW->GetSlideFileName(strSld, strCmd, strPath))
		{
			if (strCmd == "flipslide")
				m_pSlideWnd->EnableWindow(FALSE);
			else
				m_pSlideWnd->EnableWindow(TRUE);

			m_pSlideWnd->Navigate2(strPath);
			m_strPreSlide = strSlide;
			m_strPreCmd = strCmd;
		}
		else
			m_pSlideWnd->Navigate2(MACRO_EMPTY_HTM);
			
		
		//CString strPath = MACRO_JPEG_PATH(strCourse) + strPrefix + strSld + ".jpg";
		//if (!::FileExists(strPath))
		//	m_pSlideWnd->LoadFile("");
		//else
		//	m_pSlideWnd->LoadFile(strPath, bForce);
	}
}

void CWndToolBar::ChangeSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce)
{
	if (strCourse == "")
	{
		m_pSlideWnd->Navigate2(MACRO_EMPTY_HTM);
		m_strPreSlide = "";
		m_strPreCmd = "";
		return;
	}

	if (strSlide == m_strPreSlide && strCmd == m_strPreCmd && !bForce)
		return;

	CString strSld = strSlide;
	if (strSld.Find(".") < 0)
	{
		CString ew;
		int iLen = strSld.GetLength();
		int iFind = strSld.Find('@');
		if (iFind > 0)
		{
			ew = strSld.Left(iFind) + "," + strSld.Right(iLen - iFind - 1);
		}
		else
		{
			ew = strSld + ",0";
		}

		CString strPath = "javascript:callanime(" + ew + ");";
		if (strCmd == "flipslide")
				m_pSlideWnd->EnableWindow(FALSE);
			else
				m_pSlideWnd->EnableWindow(TRUE);

		m_pSlideWnd->Navigate2(strPath);
		m_strPreSlide = strSlide;
		m_strPreCmd = strCmd;
	}
}

void CWndToolBar::EnableButtons(BOOL b)
{
	//if (PROMOTE_VERSION)
	//	b = FALSE;

}

void CWndToolBar::setviewer(CString strcourse)
{
	CString strPath = MACRO_HTML_DIR(strcourse) + "viewer.html";
	m_pSlideWnd->Navigate2(strPath);
	return;
}
