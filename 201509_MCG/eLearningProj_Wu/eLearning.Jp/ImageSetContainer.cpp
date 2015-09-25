// ImageContainer.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "ImageSetContainer.h"
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

CString CImageSetContainer::m_strClass = "";

/////////////////////////////////////////////////////////////////////////////
// CImageSetContainer

CImageSetContainer::CImageSetContainer()
{
	m_bScroll = FALSE;
	m_pDrawArray = new CObArray;
	m_nTotalRow = 0;
	m_nVisibleY = 0;
	m_nVisibleX = 0;
	m_nFocus = -1;
	m_nBMPWidth = 128;
	m_nBMPHeight = 96;
	m_nInitBMPHeight = 80;
	m_pFont = NULL;
	m_bSelectable = FALSE;
	m_bShowFileName = TRUE;
}

CImageSetContainer::~CImageSetContainer()
{
	::ReleaseObArray(m_pDrawArray);
	delete m_pDrawArray;
}


BEGIN_MESSAGE_MAP(CImageSetContainer, CWnd)
	//{{AFX_MSG_MAP(CImageSetContainer)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageSetContainer message handlers

BOOL CImageSetContainer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CImageSetContainer::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

BOOL CImageSetContainer::OnEraseBkgnd(CDC* pDC) 
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

void CImageSetContainer::SetDrawArray(CStringArray* pFileArray, 
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
	strTitle.LoadString(IDS_LOADING_IMAGE_SET);
//	CProgressWnd wndProgress(this, strTitle, FALSE, 0, pFileArray->GetSize(), TRUE);	//del 2013/11/21 kagata BugNo.90
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_LOADING_IMAGE_SET);
//	wndProgress.SetFocus();	//del 2013/11/21 kagata BugNo.90

	m_wdArrayCurrentIndex.RemoveAll();
	for (int i = 0; i < pFileArray->GetSize(); i++)
	{
		CString strFile = pFileArray->GetAt(i);
		CString sPath = StrPath + strFile;
		CBitmap* pBMP = ::LoadFile(sPath);

		BITMAP b;
		pBMP->GetBitmap(&b);
		int nHeight = b.bmHeight;
		int nWidth = nHeight * 4 / 3;
		int nCount = b.bmWidth / nWidth;
		CImageList* pImgList = new CImageList;
		pImgList->Create(nWidth, nHeight, ILC_COLORDDB, 1, 1);
		pImgList->Add(pBMP, RGB(255, 0, 255));
		m_pDrawArray->Add(pImgList);
		
		pBMP->DeleteObject();
		delete pBMP;
	
		m_wdArrayCurrentIndex.Add(0);

		strMsg.Format("%s [%d]", strTmp, i + 1);
//		wndProgress.SetText(strMsg);	//del 2013/11/21 kagata BugNo.90
//		wndProgress.StepIt();			//del 2013/11/21 kagata BugNo.90
//		wndProgress.PeekAndPump();		//del 2013/11/21 kagata BugNo.90
//		if (wndProgress.Cancelled())	//del 2013/11/21 kagata BugNo.90
//			break;						//del 2013/11/21 kagata BugNo.90
	}

	GetDrawCount();
	SetScrollPos(0, TRUE);

	SetFocus();

	EndWaitCursor();
}

void CImageSetContainer::GetDrawCount()
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

void CImageSetContainer::DrawPicture(CDC* pDC)
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

			CImageList* pImageList = (CImageList*)m_pDrawArray->GetAt(nIndex);
			int n = m_wdArrayCurrentIndex.GetAt(nIndex);
			if (pImageList != NULL)
			{
				DrawInnerFrame(pDC, rcDraw);
				pImageList->Draw(pDC, n, CPoint(rcDraw.left, rcDraw.top), ILD_NORMAL);
			}

			nIndex++;
		}
	}
	
    pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();
}

void CImageSetContainer::ReCalcIndex()
{
	for (int i = 0; i < m_pDrawArray->GetSize(); i++)
	{
		CImageList* pImageList = (CImageList*)m_pDrawArray->GetAt(i);
		int nIndex = m_wdArrayCurrentIndex.GetAt(i);
		m_wdArrayCurrentIndex[i] = (nIndex + 1) % pImageList->GetImageCount();
	}
}

void CImageSetContainer::DrawSelRect(CDC* pDC, CRect rc)
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

int CImageSetContainer::IndexFromPt(CPoint pt)
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

CRect CImageSetContainer::GetRectFromIndex(int nIndex)
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

void CImageSetContainer::DrawInnerFrame(CDC* pDC, CRect rc)
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

void CImageSetContainer::DrawPictureFrame(CDC* pDC, CRect rc)
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

void CImageSetContainer::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CImageSetContainer::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nIndex = IndexFromPt(point);
	if (nIndex >= 0)
		GetParent()->PostMessage(WM_PICTURE_SET_SELECTED);

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CImageSetContainer::SetPath(CString strPath)
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
	CString strFileName;
	if (m_strSelectedFile != "")
	{
		m_strSelectedFile = m_strSelectedFile.Left(m_strSelectedFile.GetLength() - 4);
		for (int i = 0; i < m_strArrayFile.GetSize(); i++)
		{
			strFileName = ::GetFileName(m_strArrayFile[i]);
			strFileName = strFileName.Left(strFileName.GetLength() - 4);
			if (m_strSelectedFile.CompareNoCase(strFileName) == 0)
			{
				m_nFocus = i;
				break;
			}
		}
	}

	RedrawWindow();
}

void CImageSetContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	GetDrawCount();	
}

BOOL CImageSetContainer::GetSelectedFile(CString& strPath)
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

void CImageSetContainer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

BOOL CImageSetContainer::GetFirstFileDim(CSize& cs)
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

void CImageSetContainer::CalcFrameSize()
{
	CSize cs;
	int nBMPHeight = m_nInitBMPHeight;
	if (GetFirstFileDim(cs))
	{
		cs.cx = cs.cy * 4 / 3;

		m_csImage.cx = cs.cx;
		m_csImage.cy = cs.cy;
		
		m_nBMPWidth = cs.cx + cFrameGap * 2;
		nBMPHeight = cs.cy + cFrameGap * 2;
		m_nBMPHeight = nBMPHeight + ((m_bShowFileName) ? cFileTextHeight : 0);
	}
}

void CImageSetContainer::DrawFileName(CDC* pDC, CRect rcDC, int nIndex)
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

	pDC->DrawText(strFile, rcDraw, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CImageSetContainer::SetFrameSize(int nWidth, int nHeight)
{
	m_nBMPWidth = nWidth;
	m_nBMPHeight = nHeight;
	m_nInitBMPHeight = nHeight;
}

int CImageSetContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_nID = SetTimer(1, 500, NULL);
	
	return 0;
}

void CImageSetContainer::OnDestroy() 
{
	CWnd::OnDestroy();

	KillTimer(m_nID);
}

void CImageSetContainer::OnTimer(UINT nIDEvent) 
{
	RedrawWindow();
	ReCalcIndex();

	CWnd::OnTimer(nIDEvent);
}
