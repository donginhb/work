// ImageContainer.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "ImageContainer.h"
#include "MemDC.h"
#include "bmpapi.h"
#include "cmkapi.h"
#include "grdapi.h"
#include "jpgapi.h"
#include "ProgressWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define cGapWidth			4
#define cMargin				(cGapWidth - 1) * 2
#define cGap				(cGapWidth - 1) * 2
#define cSingleGap			3
#define cFrameGap			10
#define cMaxFrameWidth		400	
#define cMaxFrameHeight		400	
#define cFileTextHeight		22

CString CImageContainer::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CImageContainer

CImageContainer::CImageContainer()
{
	m_bScroll = FALSE;
	m_pDrawArray = new CObArray;
	m_nTotalRow = 0;
	m_nVisibleY = 0;
	m_nVisibleX = 0;
	m_nFocus = -1;
	m_nBMPWidth = 80;
	m_nBMPHeight = 80;
	m_nInitBMPHeight = 80;
	m_pFont = NULL;
	m_nShowType = 0;
	m_bSelectable = FALSE;
}

CImageContainer::~CImageContainer()
{
	::ReleaseObArray(m_pDrawArray);
	delete m_pDrawArray;
}


BEGIN_MESSAGE_MAP(CImageContainer, CWnd)
	//{{AFX_MSG_MAP(CImageContainer)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageContainer message handlers

BOOL CImageContainer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CImageContainer::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CImageContainer::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	DrawPicture(&dc);

	return TRUE;
}

void CImageContainer::SetDrawArray(CStringArray* pFileArray, 
								   CString StrPath)
{
	UpdateWindow();

	::ReleaseObArray(m_pDrawArray);
	if (pFileArray == NULL)
	{
		m_bScroll = FALSE;
		SetScrollPos(SB_VERT, 0);
		ShowScrollBar(SB_VERT, FALSE);
		return;
	}

	CalcFrameSize();

	BeginWaitCursor();

	m_bScroll = FALSE;
	SetScrollPos(SB_VERT, 0);
	ShowScrollBar(SB_VERT, FALSE);

	CString strTitle;
	strTitle.LoadString(IDS_LOADING_IMAGE);
	CProgressWnd wndProgress(this, strTitle, FALSE, 0, pFileArray->GetSize(), TRUE);
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_LOADING_IMAGE);
	wndProgress.SetFocus();

	for (int i = 0; i < pFileArray->GetSize(); i++)
	{
		CString strFile = pFileArray->GetAt(i);
		CString sPath = StrPath + strFile;
		CBitmap* pBMP = ::LoadFile(sPath);
		m_pDrawArray->Add(pBMP);
		
		strMsg.Format("%s [%d]", strTmp, i + 1);
		wndProgress.SetText(strMsg);
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled())
			break;
	}

	GetDrawCount();
	SetScrollPos(0, TRUE);

	SetFocus();

	EndWaitCursor();
}

void CImageContainer::GetDrawCount()
{
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.left += cMargin;
	rcClient.top += cMargin;
	rcClient.right -= cMargin;
	rcClient.bottom -= cMargin;

	int nPicCount = m_pDrawArray->GetSize();

	int nWidth = rcClient.Width() + cGap + m_nBMPWidth / 3;
	int nHeight = rcClient.Height() + cGap;
	int nWPic = cGap + m_nBMPWidth;
	int nHPic = cGap + m_nBMPHeight;
	int nX = nWidth / nWPic;
	int nY = nHeight / nHPic;
	if (nX == 0 || nY == 0)
	{
		m_nVisibleY = nY;
		m_nVisibleX = nX;
		m_nTotalRow = 0;
		m_bScroll = FALSE;
		SetScrollPos(SB_VERT, 0);
		ShowScrollBar(SB_VERT, FALSE);
		return;
	}

	int nTotalRow = (nPicCount % nX == 0) ? nPicCount / nX : nPicCount / nX + 1;
	if (nX * nY < nPicCount)
	{
		int nSBWidth = ::GetSystemMetrics(SM_CXVSCROLL);
		nWidth -= nSBWidth;
		nX = nWidth / nWPic;
		if (nX == 0 || nY == 0)
		{
			m_nVisibleY = nY;
			m_nVisibleX = nX;
			m_nTotalRow = 0;
			m_bScroll = FALSE;
			SetScrollPos(SB_VERT, 0);
			ShowScrollBar(SB_VERT, FALSE);
			return;
		}

		nTotalRow = (nPicCount % nX == 0) ? nPicCount / nX : nPicCount / nX + 1;
		m_bScroll = TRUE;
		ShowScrollBar(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, nTotalRow - nY, TRUE);
		SetScrollPos(SB_VERT, 0);
	}
	else
	{
		m_bScroll = FALSE;
		SetScrollPos(SB_VERT, 0);
		ShowScrollBar(SB_VERT, FALSE);
	}

	m_nTotalRow = nTotalRow;
	m_nVisibleY = nY;
	m_nVisibleX = nX;
}

void CImageContainer::DrawPicture(CDC* pDC)
{
	int nPicCount = m_pDrawArray->GetSize();
	if (nPicCount == 0)
		return;

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.DeflateRect(cMargin, cMargin);

    CRgn rgn;
    rgn.CreateRectRgn(0, 0, rcClient.right, rcClient.bottom);
    pDC->SelectClipRgn(&rgn);
	
	int nWPic = cGap + m_nBMPWidth;
	int nHPic = cGap + m_nBMPHeight;

	int nIndex = GetScrollPos(SB_VERT) * m_nVisibleX;
	for (int j = 0; j < m_nVisibleY + 1; j++)
	{
		for (int i = 0; i < m_nVisibleX; i++)
		{
			if (nIndex == nPicCount)
				return;

			int nXPos = rcClient.left + i * nWPic;
			int nYPos = rcClient.top + j * nHPic;
			
			CRect rcDC;
			rcDC.left = nXPos;
			rcDC.top = nYPos;
			rcDC.right = nXPos + m_nBMPWidth;
			rcDC.bottom = nYPos + m_nBMPHeight;
			
			if (nIndex == m_nFocus)
				DrawSelRect(pDC, rcDC);
			DrawPictureFrame(pDC, rcDC);
			if (m_bShowFileName)
				DrawFileName(pDC, rcDC, nIndex);

			CRect rcDraw = rcDC;
			rcDraw.bottom -= ((m_bShowFileName) ? cFileTextHeight : 0);

			rcDraw.InflateRect(-cFrameGap, -cFrameGap);
			int nWidth = rcDraw.Width();
			int nHeight = rcDraw.Height();

			CBitmap* pBMP = (CBitmap*)m_pDrawArray->GetAt(nIndex);
/*			if (pBMP == NULL)
			{
				LBitmapBase MyBitmap;
				HDC hDC = ::GetDC(0);
				CString strFile = m_strArrayFile.GetAt(nIndex);
				CString sPath = m_strPath + strFile;
				int nRet;
				if ((nRet = MyBitmap.Load((LPTSTR)(LPCTSTR)sPath, 0, ORDER_BGR, NULL)) >= 1)
				{
					HBITMAP hBMP = MyBitmap.ConvertToDDB(hDC);
					if (hBMP)
					{
						pBMP = new CBitmap;
						pBMP->Attach(hBMP);
					}
					m_pDrawArray->SetAt(nIndex, pBMP);
				}
				::ReleaseDC(0, hDC);
			}
*/			
			if (pBMP != NULL)
			{
				BITMAP bm;
				pBMP->GetBitmap(&bm);
				int nBMPHeight = bm.bmHeight;
				int nBMPWidth = bm.bmWidth;
				if (nBMPHeight < nHeight && nBMPWidth < nWidth)
				{
					rcDraw.left += (nWidth - nBMPWidth) / 2;
					rcDraw.right = rcDraw.left + nBMPWidth;
					rcDraw.top += (nHeight - nBMPHeight) / 2;
					rcDraw.bottom = rcDraw.top + nBMPHeight;
				}
				else 
				{
					double dH = (double) nBMPHeight / nHeight;
					double dW = (double) nBMPWidth / nWidth;
					if (dH > dW)
					{
						int nNewWidth = nHeight * nBMPWidth / nBMPHeight;
						rcDraw.left += (nWidth - nNewWidth) / 2;
						rcDraw.right = rcDraw.left + nNewWidth;
					}
					else 
					{
						int nNewHeight = nWidth * nBMPHeight / nBMPWidth;
						rcDraw.top += (nHeight - nNewHeight) / 2;
						rcDraw.bottom = rcDraw.top + nNewHeight;
					}
				}

				DrawInnerFrame(pDC, rcDraw);

				CRect rcBMP;
				rcBMP.left = 0;
				rcBMP.top = 0;
				rcBMP.right = rcBMP.left + nBMPWidth;
				rcBMP.bottom = rcBMP.top + nBMPHeight;
				::DDBPaint(pDC->m_hDC, rcDraw, (HBITMAP)pBMP->m_hObject, rcBMP, NULL);
			}

			nIndex++;
		}
	}
	
    pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();
}

void CImageContainer::DrawSelRect(CDC* pDC, CRect rc)
{
	CBrush br;
	br.CreateSolidBrush(RGB(0, 0, 128));
	CRect rcSq = rc;
	rcSq.bottom -= ((m_bShowFileName) ? cFileTextHeight : 0);

	pDC->FillRect(rcSq, &br);

	if (m_bShowFileName)
	{
		rcSq.top = rcSq.bottom + cSingleGap;
		rcSq.bottom = rc.bottom;
		pDC->FillRect(rcSq, &br);
	}
	
	br.DeleteObject();
}

int CImageContainer::IndexFromPt(CPoint pt)
{
	CRect rc;
	GetClientRect(rc);
	pt.x -= rc.left;
	pt.y -= rc.top;

	int nNumX = (pt.x - cMargin) / (m_nBMPWidth + cGap);
	if (nNumX >= m_nVisibleX)
		return -1;
	int nX = nNumX * (m_nBMPWidth + cGap) + cMargin;
	if (pt.x - nX > m_nBMPWidth)
		return -1;
	if (pt.x - nX < 0)
		return -1;

	int nNumY = (pt.y - cMargin) / (m_nBMPHeight + cGap);
	int nY = nNumY * (m_nBMPHeight + cGap) + cMargin;
	if (pt.y - nY > m_nBMPHeight)
		return -1;
	if (pt.y - nY < 0)
		return -1;

	int nTop = 0;
	if (m_bScroll)
		nTop = GetScrollPos(SB_VERT);
	int n = (nTop + nNumY) * m_nVisibleX + nNumX;
	if (n >= m_pDrawArray->GetSize())
		return -1;
	return n;
}

CRect CImageContainer::GetRectFromIndex(int nIndex)
{
	CRect rcRet(0, 0, 0, 0);
	if (nIndex < 0)
		return rcRet;

	int nScrollTop = 0;
	if (m_bScroll)
		nScrollTop = GetScrollPos(SB_VERT);
	
	int nStartIndex = m_nVisibleX * nScrollTop;
	if (nIndex < nStartIndex)
		return rcRet;
	int nOffset = nIndex - nStartIndex;
	int nXOffset = nOffset % m_nVisibleX;
	int nYOffset = nOffset / m_nVisibleX;

	CRect rcClient;
	GetClientRect(rcClient);
	int nTop = rcClient.top + (m_nBMPHeight + cGap) * nYOffset + cMargin;
	int nLeft = rcClient.left + (m_nBMPWidth + cGap) * nXOffset + cMargin;

	int nBottomMost = rcClient.bottom - cMargin;
	if (nBottomMost < nTop)
		return rcRet;

	rcRet.SetRect(nLeft, nTop, nLeft + m_nBMPWidth, nTop + m_nBMPHeight);
	return rcRet;
}

void CImageContainer::DrawInnerFrame(CDC* pDC, CRect rc)
{
	CPen penLight, penDark;
	penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	rc.left--;
	rc.top--;

	CPen* pOldPen = pDC->SelectObject(&penDark);
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);
	pDC->SelectObject(&penLight);
	pDC->LineTo(rc.right, rc.bottom);
	pDC->LineTo(rc.left, rc.bottom);

	pDC->SelectObject(pOldPen);
	penLight.DeleteObject();
	penDark.DeleteObject();
}

void CImageContainer::DrawPictureFrame(CDC* pDC, CRect rc)
{
	CPen penLight, penDark;
	penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	penDark.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	CRect rcSq = rc;
	rcSq.bottom -= ((m_bShowFileName) ? cFileTextHeight : 0);

	CPen* pOldPen = pDC->SelectObject(&penLight);
	pDC->MoveTo(rcSq.left, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.top);
	pDC->LineTo(rcSq.right, rcSq.top);
	pDC->SelectObject(&penDark);
	pDC->LineTo(rcSq.right, rcSq.bottom);
	pDC->LineTo(rcSq.left, rcSq.bottom);

	if (m_bShowFileName)
	{
		rcSq.top = rcSq.bottom + cSingleGap;
		rcSq.bottom = rc.bottom;
		pDC->SelectObject(&penDark);
		pDC->MoveTo(rcSq.left, rcSq.bottom);
		pDC->LineTo(rcSq.left, rcSq.top);
		pDC->LineTo(rcSq.right, rcSq.top);
		pDC->SelectObject(&penLight);
		pDC->LineTo(rcSq.right, rcSq.bottom);
		pDC->LineTo(rcSq.left, rcSq.bottom);	
	}

	pDC->SelectObject(pOldPen);
	penLight.DeleteObject();
	penDark.DeleteObject();
}

void CImageContainer::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nIndex = IndexFromPt(point);
	if (nIndex != m_nFocus)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.DeflateRect(cMargin, cMargin);

		int nPreFocus = m_nFocus;
		m_nFocus = nIndex;
		CRect rc = GetRectFromIndex(nIndex);
		if (!rc.IsRectEmpty())
		{
			rc.bottom = min(rc.bottom, rcClient.bottom + 1);
			rc.right = min(rc.right, rcClient.right);
			RedrawWindow(&rc);
		}

		CRect rcOldFocus = GetRectFromIndex(nPreFocus);
		if (!rcOldFocus.IsRectEmpty())
		{
			rcOldFocus.DeflateRect(-cSingleGap, -cSingleGap, -cSingleGap, -cSingleGap);
			rcOldFocus.bottom = min(rcOldFocus.bottom, rcClient.bottom + 1);
			rcOldFocus.right = min(rcOldFocus.right, rcClient.right + 1);
			RedrawWindow(&rcOldFocus);
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CImageContainer::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nIndex = IndexFromPt(point);
	if (nIndex >= 0)
		GetParent()->PostMessage(WM_PICTURE_SELECTED);

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CImageContainer::SetPath(CString strPath)
{
	if (strPath.Right(1) != "\\")
		strPath = strPath + "\\";
	
	m_strArrayFile.RemoveAll();
	
	BeginWaitCursor();
	::GetFiles(strPath, "bmp;gif;jpeg;jpg;png", m_strArrayFile);
	EndWaitCursor();

	::SortArray(m_strArrayFile);

	m_strPath = strPath;

	SetDrawArray(&m_strArrayFile, strPath);
	m_nFocus = -1;
	RedrawWindow();
}

void CImageContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_nShowType == constShowOneEachRow)
		CalcFrameSize();

	GetDrawCount();	
}

BOOL CImageContainer::GetSelectedFile(CString& strPath)
{
	if (m_nFocus < 0)
		return FALSE;

	if (m_nFocus < 0 && m_nFocus >= m_strArrayFile.GetSize())
		return FALSE;

	if (m_strPath.Right(1) != "\\")
		m_strPath += "\\";
	strPath = m_strPath + m_strArrayFile.GetAt(m_nFocus);
	return TRUE;
}

void CImageContainer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    int zOrig, z; 
    int zMax, zMin;
    int nOneUnit;
    int nInterval = 3;
	
    zOrig = z = GetScrollPos(SB_VERT);
    GetScrollRange(SB_VERT, &zMin, &zMax);
    nOneUnit = 1;
    ASSERT(zMin == 0);
    
    if (zMax <= 0)
    {
       ASSERT(z == 0);
       return;
    }         
    switch (nSBCode)
    {
	case SB_TOP:
    		z = 0;
    		break;
    		
    case SB_BOTTOM:
    		z = zMax;
    		break;
 
    case SB_LINEUP:
           	z -= nOneUnit; 
    		break;
 
    case SB_LINEDOWN:
           	z += nOneUnit; 
    		break;       
    
    case SB_PAGEUP:
           	z -= nInterval; 
    		break;
	
	case SB_PAGEDOWN:
           	z += nInterval; 
    		break;
	
	case SB_THUMBTRACK: 
	case SB_THUMBPOSITION: 
    		z = (int)nPos; 
    		break;  
			
    default:
    		return;
    }
    
    if (z < 0)
    	z = 0;
    else if (z > zMax)
    	z = zMax;

   	if (z != zOrig)
	{
		SetScrollPos(SB_VERT, z, TRUE);
		RedrawWindow();
	}
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CImageContainer::GetFirstFileDim(CSize& cs)
{
	if (m_strArrayFile.GetSize() > 0)
	{
		CString strFullPath = m_strPath + m_strArrayFile.GetAt(0);
		CBitmap* pBMP = ::LoadFile(strFullPath);
		if (pBMP)
		{
			BITMAP bm;
			pBMP->GetBitmap(&bm);
			int nBMPHeight = bm.bmHeight;
			int nBMPWidth = bm.bmWidth;
			cs.cx = nBMPWidth; 
			cs.cy = nBMPHeight;
			pBMP->DeleteObject();
			delete pBMP;

			return TRUE;
		}
	}

	return FALSE;
}

void CImageContainer::CalcFrameSize()
{
	CSize cs;
	int nBMPHeight = m_nInitBMPHeight;
	if (m_nShowType == constShowFit)
	{
		if (GetFirstFileDim(cs))
		{
			m_nBMPWidth = cs.cx + cFrameGap * 2;
			nBMPHeight = cs.cy + cFrameGap * 2;
		}
	}
	else if (m_nShowType == constShowByRate)
	{
		if (GetFirstFileDim(cs))
		{
			double dRate = (double)cs.cx / cs.cy;
			if (cs.cx > cMaxFrameWidth)
			{
				cs.cx = cMaxFrameWidth;
				cs.cy = (int)(cs.cx / dRate);
				if (cs.cy > cMaxFrameHeight)
				{
					cs.cy = cMaxFrameHeight;
					cs.cx = (int)(cs.cy * dRate);
				}
			}
			else if (cs.cy > cMaxFrameHeight)
			{
				cs.cy = cMaxFrameHeight;
				cs.cx = (int)(cs.cy * dRate);
				if (cs.cx > cMaxFrameWidth)
				{
					cs.cx = cMaxFrameWidth;
					cs.cy = (int)(cs.cx / dRate);
				}
			}
			
			m_nBMPWidth = cs.cx + cFrameGap * 2;;
			nBMPHeight = cs.cy + cFrameGap * 2;
		}
	}
	else if (m_nShowType == constShowOneEachRow)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		int nWidth = rcClient.Width();
		nWidth = nWidth - ::GetSystemMetrics(SM_CXVSCROLL) - cMargin * 2 - cFrameGap * 2;
		if (GetFirstFileDim(cs))
		{
			double dRate = (double)cs.cx / cs.cy;
			if (cs.cx > nWidth)
			{
				cs.cx = nWidth;
				cs.cy = (int)(cs.cx / dRate);
			}
			
			m_nBMPWidth = cs.cx + cFrameGap * 2;;
			nBMPHeight = cs.cy + cFrameGap * 2;
		}
	}

	m_nBMPHeight = nBMPHeight + ((m_bShowFileName) ? cFileTextHeight : 0);
}

void CImageContainer::DrawFileName(CDC* pDC, CRect rcDC, int nIndex)
{
	if (m_pFont == NULL)
		m_pFont = GetFont();

	CFont* pOldFont = pDC->SelectObject(m_pFont);

	if (nIndex == m_nFocus)
		pDC->SetTextColor(RGB(255, 255, 255));
	else
		pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	
	CRect rcDraw = rcDC;
	rcDraw.top = rcDC.bottom - cFileTextHeight + 1;
	rcDraw.left += 2;
	rcDraw.right -= 2;
	CString strFile = m_strArrayFile[nIndex];
	if (!m_bShowFileExt)
	{
		int nIndex = strFile.ReverseFind('.');
		strFile = strFile.Left(nIndex);
	}

	//strFile.MakeLower();
	pDC->DrawText(strFile, rcDraw, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CImageContainer::SetFrameSize(int nWidth, int nHeight)
{
	m_nBMPWidth = nWidth;
	m_nBMPHeight = nHeight;
	m_nInitBMPHeight = nHeight;
}

/*
CBitmap* CImageContainer::LoadDDB(CString sPath)
{
	LBitmapBase MyBitmap;

	int nRet;
	if ((nRet = MyBitmap.Load((LPTSTR)(LPCTSTR)sPath, 0, ORDER_BGR, NULL)) < 1)
		return NULL;
	else
	{
		HDC hDC = ::GetDC(0);

		HBITMAP hBMP = MyBitmap.ConvertToDDB(hDC);
		CBitmap* pBMP = NULL;
		if (hBMP)
		{
			pBMP = new CBitmap;
			pBMP->Attach(hBMP);
		}
	
		::ReleaseDC(0, hDC);
		return pBMP;
	}
}
*/