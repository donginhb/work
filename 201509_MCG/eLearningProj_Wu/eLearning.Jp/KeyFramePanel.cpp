// KeyFramePanel.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "KeyFramePanel.h"
#include "MemDC.h"
#include "cmkapi.h"
#include "macro.h"
#include "ToolTipWnd.h"
#include "eLearningView.h"
#include "MainFrm.h"
//#include "PPTWnd.h"	//add 2014/7/29 kagata BugNo.130

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyFramePanel

CString CKeyFramePanel::m_strClass = "";

CKeyFramePanel::CKeyFramePanel()
{
	m_pImageArray = new CObArray;
	AddImageByID(IDB_RED_KEY, POS_CENTER);
	AddImageByID(IDB_GREEN_KEY, POS_CENTER);
	AddImageByID(IDB_BLUE_KEY, POS_CENTER);
	AddImageByID(IDB_SQ_IN, POS_BOTTOM);
	AddImageByID(IDB_SQ_OUT, POS_BOTTOM);
	AddImageByID(IDB_DEMO, POS_BOTTOM);
	AddImageByID(IDB_PRACTICE, POS_BOTTOM);
	AddImageByID(IDB_DRAWMARK, POS_BOTTOM);

	m_rcChannel.SetRectEmpty();
	m_pKeyFrameArray = new CObArray;
	m_pTooltipWnd = new CToolTipWnd;

	m_bCanRMenu = TRUE;
	
	//CBitmap bitmap;
	//bitmap.LoadBitmap(IDB_RED_KEY);
	//BITMAP b;
	//bitmap.GetBitmap(&b);
	//m_nBmpWidth = b.bmWidth / 4;
    //m_nBmpHeight = b.bmHeight;

	m_nCtrlEntered = -1;
	m_bTracking = FALSE;
	m_bLeaveTrack = FALSE;
	m_pToolTip = NULL;
	m_bDeletable = FALSE;
	m_bEditable = TRUE;
	m_lTimeStart = 0;
	m_bFirstMove = FALSE;
	m_lLength = 0;
}

CKeyFramePanel::~CKeyFramePanel()
{
	RemoveAllKeyFrame(); 
	delete m_pKeyFrameArray;

	for (int i = m_pImageArray->GetSize() - 1; i >= 0; i--)
	{
		CImageObj* pObj = (CImageObj*)m_pImageArray->GetAt(i);
		delete pObj;
		m_pImageArray->RemoveAt(i);
	}
	delete m_pImageArray;

	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CKeyFramePanel, CWnd)
	//{{AFX_MSG_MAP(CKeyFramePanel)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKeyFramePanel message handlers

CImageObj* CKeyFramePanel::GetImageObjByType(int nType)
{
	UINT nID = MapTypeToID(nType);
	for (int i = 0; i < m_pImageArray->GetSize(); i++)
	{
		CImageObj* pObj = (CImageObj*)m_pImageArray->GetAt(i);
		if (pObj->m_nID == nID)
			return pObj;
	}
	return NULL;
}

UINT CKeyFramePanel::MapTypeToID(int nType)
{
	if (nType == KEY_RED)
		return IDB_RED_KEY;
	else if (nType == KEY_GREEN)
		return IDB_GREEN_KEY;
	else if (nType == KEY_BLUE)
		return IDB_BLUE_KEY;
	else if (nType == KEY_SQ_IN)
		return IDB_SQ_IN;
	else if (nType == KEY_SQ_OUT)
		return IDB_SQ_OUT;
	else if (nType == KEY_DEMO)
		return IDB_DEMO;
	else if (nType == KEY_PRACTICE)
		return IDB_PRACTICE;
	else if (nType == KEY_DRAW)
		return IDB_DRAWMARK;

	return 0;
}

void CKeyFramePanel::AddImageByID(UINT nID, IMAGE_POS nPos)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);

	CImageObj* pObj = new CImageObj;
	pObj->m_nID = nID;
	pObj->m_nWidth = b.bmWidth / 4;
	pObj->m_nHeight = b.bmHeight;
	pObj->m_nPos = nPos;

	pObj->m_lstImage.Create(pObj->m_nWidth, pObj->m_nHeight, 
					        ILC_COLORDDB | ILC_MASK, 4, 1);
	pObj->m_lstImage.Add(&bitmap, RGB(255, 0, 255));

	m_pImageArray->Add(pObj);
}

void CKeyFramePanel::RemoveAllKeyFrame()  
{
    int nArraySize = m_pKeyFrameArray->GetSize();
    for (int i = 0; i < nArraySize; i++)
	{
		CObject* pObj = m_pKeyFrameArray->GetAt(0);
		m_pKeyFrameArray->RemoveAt(0);
	    delete pObj;                      
	}
	m_nCtrlEntered = -1;

	ResetTooltip();
}

BOOL CKeyFramePanel::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)  
{
	if (m_strClass == "")
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 0, NULL);
	BOOL bRet = CreateEx(m_strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL);

	return bRet;
}

BOOL CKeyFramePanel::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

int CKeyFramePanel::TimeToPos(long lTime)
{
	return (lTime - m_lTimeStart) * m_nPixelUnit / m_nUnit;
}

long CKeyFramePanel::PosToTime(int xPos)
{
	long lRet = m_lTimeStart + m_nUnit * xPos / m_nPixelUnit;
	return lRet;
}

BOOL CKeyFramePanel::OnEraseBkgnd(CDC* pSDC) 
{
   	CMemDC_MCG memDC(pSDC);
	CDC* pDC = &memDC;

	CBrush backBrush(RGB(144, 144, 144));//RGB(132, 130, 132));//::GetSysColor(COLOR_3DFACE));

	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	
	CRect rcClient;
	GetClientRect(rcClient);
	
	CPen penBlack(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));
	CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penLtGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
	CPen* pOldPen = pDC->SelectObject(&penWhite);
	
	//if (m_lTimeStart == 0)
	//{
	//	pDC->MoveTo(1, rcClient.bottom);
	//	pDC->LineTo(1, 0);
	//}
	pDC->MoveTo(1, 0);
	//pDC->LineTo(rcClient.right, 0);
	
	pDC->SelectObject(&penWhite);
	pDC->MoveTo(1, rcClient.bottom - 3);
	//pDC->LineTo(rcClient.right, rcClient.bottom - 3);

	pDC->SelectObject(&penDkGray);
	pDC->MoveTo(1, rcClient.bottom - 1);
	//pDC->LineTo(rcClient.right, rcClient.bottom - 1);
	
	if (!m_rcChannel.IsRectEmpty())
	{
		DrawChannel(pDC, m_rcChannel);
		DrawKeyFrame(pDC);
	}

	if (PosToTime(rcClient.right) > m_lLength)
	{
		int nPos = TimeToPos(m_lLength);
		pDC->SelectObject(&penBlack);
		pDC->MoveTo(nPos + 1, 0);
		//pDC->LineTo(nPos + 1, rcClient.bottom - 1);

		pDC->SelectObject(&penDkGray);
		pDC->MoveTo(nPos, 1);
		//pDC->LineTo(nPos, rcClient.bottom - 2);

		CBrush bkBrush(RGB(132, 130, 132));
		CRect rc = rcClient;
		rc.left = nPos + 2;
		pDC->SelectObject(&bkBrush);
	    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
	}

	pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

	return TRUE;
}

void CKeyFramePanel::DrawKeyFrame(CDC *pDC)
{
	CRect rcClient, drawRc;
	GetClientRect(rcClient);
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CRect rcPos = GetPosRect(pObj->m_lTime, pObj->m_nType);
		if (rcPos.IsRectEmpty())
			continue;

		CPoint pt;
		pt.x = rcPos.left;
		pt.y = rcPos.top;
		if (pt.x >= rcClient.left - 10 && pt.x <= rcClient.right + 10)
		{
			int nType = pObj->m_nType;
			if (nType != KEY_RED)
				continue;
			CImageObj* pImageObj = GetImageObjByType(nType);
			pImageObj->m_lstImage.Draw(pDC, pObj->m_nStatus, pt, ILD_TRANSPARENT);
#if 1
			if (pObj->m_strCmd == "flipslide" && pObj->m_strTag.Find(".") < 0)
			{
				CString strTag = pObj->m_strTag;
				int bufLen = 0;
				char* strBuf = strTag.GetBuffer();
				while(1)
				{
					if (strBuf[bufLen] >= 48 && strBuf[bufLen] <= 57)
						bufLen++;
					else
						break;
				}
				CString slideNo = strTag.Left(bufLen);
				CString slidePath = m_slidePath + "Slide" + slideNo + ".png";
				CImage slideImage;
				slideImage.Load(slidePath);
				if (!slideImage.IsNull())
				{
					int imageHeight = slideImage.GetHeight();
					int imageWidth = slideImage.GetWidth();
					drawRc = rcClient;
					drawRc.left = rcPos.left;
					drawRc.right = drawRc.bottom * imageWidth / imageHeight + drawRc.left;
					slideImage.Draw(pDC->m_hDC, drawRc, Gdiplus::InterpolationModeDefault);
				}
			}
#endif
		}
	}
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CRect rcPos = GetPosRect(pObj->m_lTime, pObj->m_nType);
		if (rcPos.IsRectEmpty())
			continue;

		CPoint pt;
		pt.x = rcPos.left;
		pt.y = rcPos.top;
		if (pt.x >= rcClient.left - 10 && pt.x <= rcClient.right + 10)
		{
			int nType = pObj->m_nType;
			if (nType == KEY_RED)
				continue;
			CImageObj* pImageObj = GetImageObjByType(nType);
			pImageObj->m_lstImage.Draw(pDC, pObj->m_nStatus, pt, ILD_TRANSPARENT);
		}
	}
}

void CKeyFramePanel::AddKeyFrame(CString strTitle, CString strTag, 
								 long nPos, KEY_TYPE nType, CString strCmd)
{
	CKeyFrameObj* pObj = new CKeyFrameObj;
	pObj->m_strTitle = strTitle;
	pObj->m_strTag = strTag;
	pObj->m_lTime = nPos;
	pObj->m_nType = nType;
	pObj->m_rcPos.SetRectEmpty();
	pObj->m_strCmd = strCmd;

	BOOL isAdded = FALSE;
	for (int i = m_pKeyFrameArray->GetSize(); i > 0; i--)
	{
		if (((CKeyFrameObj*)m_pKeyFrameArray->GetAt(i - 1))->m_lTime < pObj->m_lTime)
		{
			m_pKeyFrameArray->InsertAt(i, pObj, 1);
			isAdded = TRUE;
			break;
		}
	}
	if (!isAdded)
	{
		m_pKeyFrameArray->Add(pObj);
	}

}

void CKeyFramePanel::DrawChannel(CDC *pDC, const CRect &rect)
{
	if (rect.IsRectEmpty())
		return;

	CPen penBlack(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));
	CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penGray(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));

	CPen* pOldPen = pDC->SelectObject(&penWhite);
	pDC->MoveTo(rect.left, rect.top - 1);
	//pDC->LineTo(rect.right, rect.top - 1);

	pDC->SelectObject(&penGray);
	pDC->MoveTo(rect.left, rect.top + 2);
	//pDC->LineTo(rect.right, rect.top + 2);
	
	pDC->SelectObject(&penDkGray);
	pDC->MoveTo(rect.left, rect.top + 3);
	//pDC->LineTo(rect.right, rect.top + 3);

	//CBrush br(RGB(128, 128, 128));
	//pDC->FillRect(rect, &br);

	pDC->SelectObject(pOldPen);
}

void CKeyFramePanel::LoadImageList()
{
}

void CKeyFramePanel::AddToolTip()
{
	delete m_pToolTip;
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	CString strTitle;
	CString strPreMark;
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CRect rcCur = pObj->m_rcPos;
		if (rcCur.IsRectEmpty())
			continue;
		
		CString strScript = pObj->m_strTag;
		CString strOp = strScript.Right(1);
		if (strOp == "=" || strOp == "+" || strOp == "-")
			strScript = strScript.Left(strScript.GetLength() - 1);

		CString strCmd = pObj->m_strCmd;
		CString strMark = pObj->m_strTitle;
		CString strTime = GetTimeStr(pObj->m_lTime);
		CString strTip;
		if (::IsCtrlMark(strMark) && (strCmd == "zoomin" || strCmd == "zoomout"))
		{
			CString strZoom;
			if (strCmd == "zoomin")
				strZoom.LoadString(IDS_ZOOM_IN);
			else
				strZoom.LoadString(IDS_ZOOM_OUT);
			strTip = strZoom + " <" + strTime + ">";
		}
		else if (::IsCtrlMark(strMark) && strCmd == "demo")
		{
			CString strDemo;
			strDemo.LoadString(IDS_DEMO);
			strTip = strDemo + strScript + " <" + strTime + ">";
		}
		else if (strCmd == "drawline" || strCmd == "drawrect" || strCmd == "drawmark" || strCmd == "freehand")
		{
			CString strDraw;
			if (strCmd == "drawrect")
				strDraw.LoadString(IDS_DRAWRECT);
			else if (strCmd == "drawline")
				strDraw.LoadString(IDS_DRAWLINE);
			else if (strCmd == "drawmark")
				strDraw.LoadString(IDS_DRAWMARK);
			else if (strCmd == "freehand")
				strDraw.LoadString(IDS_FREEHAND);
			strTip = strDraw + " <" + strTime + ">";
		}
		//else if (::IsCtrlMark(strMark) && strCmd == "practice")
		//{
		//	CString strDemo;
		//	strDemo.LoadString(IDS_PRACTICE);
		//	strTip = strDemo + strScript + " <" + pObj->m_strTime + ">";
		//}
		else
		{
			CString strTime = GetTimeStr(pObj->m_lTime);
			if (strMark != "")
				strPreMark = strMark;
			if (strMark.GetLength() > 5)
				strTip = "(" + strScript + ") " + strMark.Mid(5) + " <" + strTime + ">"; 
			else if (strPreMark.GetLength() > 5)
				strTip = "(" + strScript + ") " + strPreMark.Mid(5) + " <" + strTime + ">"; 
			else
				strTip = "(" + strScript + ") ??? <" + strTime + ">"; 
		}

		if (i != m_pKeyFrameArray->GetSize() - 1)
		{
			CKeyFrameObj* pObj2 = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i + 1);
			CRect rcNext = pObj2->m_rcPos;
			if (rcCur.right > rcNext.left && rcCur.top == rcNext.top)
				rcCur.right = rcNext.left;
		}
		m_pToolTip->AddTool(this, strTip, rcCur, 1);
	}
	m_pToolTip->Activate(TRUE);
}

void CKeyFramePanel::GetKeyFrameData(CStringArray& strArrayTimeMarker, 
									 CStringArray& strArrayMarker, 
									 CStringArray& strArrayTimeScript,
									 CStringArray& strArrayScript,
									 CStringArray& strArrayCmd)
{
	strArrayTimeMarker.RemoveAll(); 
	strArrayMarker.RemoveAll(); 
	strArrayTimeScript.RemoveAll(); 
	strArrayScript.RemoveAll(); 
	strArrayCmd.RemoveAll(); 
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CString strMarker = pObj->m_strTitle;
		CString strTime = GetTimeStr(pObj->m_lTime);
		if (strMarker != "")
		{
			BOOL bInsert = FALSE;
			for (int j = 0; j < strArrayTimeMarker.GetSize(); j++)
			{
				CString strTime2 = strArrayTimeMarker[j];
				if (strTime2 > strTime)
				{
					strArrayTimeMarker.InsertAt(j, strTime);
					strArrayMarker.InsertAt(j, strMarker);
					bInsert = TRUE;
					break;
				}
			}
			if (!bInsert)
			{
				strArrayTimeMarker.Add(strTime);
				strArrayMarker.Add(strMarker);
			}
		}

		BOOL bInsert = FALSE;
		for (int j = 0; j < strArrayTimeScript.GetSize(); j++)
		{
			CString strTime2 = strArrayTimeScript[j];
			if (strTime2 > strTime)
			{
				strArrayTimeScript.InsertAt(j, strTime);
				strArrayScript.InsertAt(j, pObj->m_strTag);
				strArrayCmd.InsertAt(j, pObj->m_strCmd);
				bInsert = TRUE;
				break;
			}
		}
		if (!bInsert)
		{
			strArrayTimeScript.Add(strTime);
			strArrayScript.Add(pObj->m_strTag);
			strArrayCmd.Add(pObj->m_strCmd);
		}
	}
}

void CKeyFramePanel::GetKeyFrameData(CStringArray& strArrayTime, 
									 CStringArray& strArrayMarker, 
									 CStringArray& strArrayScript,
									 CStringArray& strArrayCmd)
{
	strArrayTime.RemoveAll(); 
	strArrayMarker.RemoveAll(); 
	strArrayScript.RemoveAll(); 
	strArrayCmd.RemoveAll(); 
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CString strMarker = pObj->m_strTitle;
		CString strTime = GetTimeStr(pObj->m_lTime);

		BOOL bInsert = FALSE;
		for (int j = 0; j < strArrayTime.GetSize(); j++)
		{
			CString strTime2 = strArrayTime[j];
			if (strTime2 > strTime)
			{
				strArrayTime.InsertAt(j, strTime);
				strArrayMarker.InsertAt(j, strMarker);
				strArrayScript.InsertAt(j, pObj->m_strTag);
				strArrayCmd.InsertAt(j, pObj->m_strCmd);
				bInsert = TRUE;
				break;
			}
		}
		if (!bInsert)
		{
			strArrayTime.Add(strTime);
			strArrayMarker.Add(strMarker);
			strArrayScript.Add(pObj->m_strTag);
			strArrayCmd.Add(pObj->m_strCmd);
		}
	}
}

int CKeyFramePanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	m_pTooltipWnd->Create(this);
	m_pTooltipWnd->SetDrawFont(MACRO_SYS_FONT2);

	return 0;
}

void CKeyFramePanel::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	ResetTooltip();

	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcChannel;
	rcChannel.left = rcClient.left;
	rcChannel.top = rcClient.top + 10;
	rcChannel.right = rcClient.right;
	rcChannel.bottom = rcChannel.top + 3;
	SetRectChannel(rcChannel);
}

void CKeyFramePanel::ResetTooltip()
{
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
		SetKeyPos(i);
	AddToolTip();
}

void CKeyFramePanel::SetKeyPos(int nIndex)
{
	CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nIndex);
	CRect rcPos = GetPosRect(pObj->m_lTime, pObj->m_nType);
	pObj->m_rcPos = rcPos;
}

CRect CKeyFramePanel::GetPosRect(long lTime, int nType)
{
	CRect rcEmpty;
	rcEmpty.SetRectEmpty();

	if (m_rcChannel.IsRectEmpty())
		return rcEmpty;
	
	if (lTime < m_lTimeStart)
		return rcEmpty;
	
	CImageObj* pObj = GetImageObjByType(nType);
	int nBmpWidth = pObj->m_nWidth;
	int nBmpHeight = pObj->m_nHeight;

	CRect rcClient;
	GetClientRect(rcClient);
	int nLeft = (lTime - m_lTimeStart) / m_nUnit * m_nPixelUnit - nBmpWidth / 2;
	if (nLeft > rcClient.right)
		return rcEmpty;

	CRect rcPos;
	if (pObj->m_nPos == POS_CENTER)
		rcPos.top = (m_rcChannel.top + m_rcChannel.bottom) / 2 - nBmpHeight / 2;
	else if (pObj->m_nPos == POS_BOTTOM)
		rcPos.top = m_rcChannel.bottom + 1;

	rcPos.left = nLeft;
	rcPos.right = rcPos.left + nBmpWidth;
	rcPos.bottom = rcPos.top + nBmpHeight;

	return rcPos;
}

CKeyFrameObj* CKeyFramePanel::GetFrameObj(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_pKeyFrameArray->GetSize())
		return NULL;

	CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nIndex);
	return pInfo;
}

void CKeyFramePanel::RemoveFrameObj(int nIndex)
{
	int nSize = m_pKeyFrameArray->GetSize();
	if (nIndex >= nSize)
		return;
	int count = 0;
	MACRO_VIEW -> LayoutExistFlag = FALSE;
	if (nIndex >= 0)
	{
		
		for (int i=0; i <= nIndex; i++ )
		{
			CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
			CString strMarker = pObj->m_strTitle;
			if (i == nIndex)
			{
				if (strMarker != "" && !::IsCtrlMark(strMarker))
				{
					MACRO_VIEW -> LayoutExistFlag = TRUE;
				}
			}
			if (strMarker != "" && !::IsCtrlMark(strMarker))
			{
				count++;
			}
		}
		MACRO_VIEW -> CountNumber = count;
		CObject* pObj = m_pKeyFrameArray->GetAt(nIndex);
		m_pKeyFrameArray->RemoveAt(nIndex);
		
		delete pObj;                      

		if (m_nCtrlEntered == nIndex)
			m_nCtrlEntered = -1;
	}
	else
	{
		if (W_ASK_YESNO(IDS_DELETE_ALL_CP, this) == IDNO)
			return;

		for (int i = 0; i < nSize; i++)
		{
			CObject* pObj = m_pKeyFrameArray->GetAt(0);
			m_pKeyFrameArray->RemoveAt(0);
			delete pObj;                      
		}
		m_nCtrlEntered = -1;
	}
}

int CKeyFramePanel::CtrlEntered(CPoint point)
{
	for (int i = m_pKeyFrameArray->GetSize() - 1; i >= 0; i--)
	{
		CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		CRect rc = pInfo->m_rcPos;
#if 1
		if (pInfo->m_nType == KEY_RED)
		{
			rc.top -= (rc.bottom - rc.top) * 2;
			rc.right += (rc.right - rc.left) * 4;
			rc.bottom += (rc.bottom - rc.top) * 2;
		}
#endif
		if (rc.PtInRect(point))
			return i;
	}
	return -1;
}

void CKeyFramePanel::EnterCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nCtrl);
	if (pInfo->m_nStatus == constBtnNormal)
	{
		pInfo->m_nStatus = constBtnOver;
		RedrawWindow(pInfo->m_rcPos);
	}

	m_nCtrlEntered = nCtrl;
}

void CKeyFramePanel::LeaveCtrl(int nCtrl)
{
	if (nCtrl < 0)
		return;

	if (nCtrl >= m_pKeyFrameArray->GetSize())
		return;

	CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nCtrl);

	if (pInfo->m_nStatus == constBtnOver)
	{
		pInfo->m_nStatus = constBtnNormal;
		RedrawWindow(pInfo->m_rcPos);
	}
}

void CKeyFramePanel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	int nCtrl = CtrlEntered(point);
	if (nCtrl < 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nCtrl);
	if (pInfo->m_nStatus == constBtnDisable)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	GetParent()->SendMessage(WM_KEY_FRAME_PRESSING);

	m_rectTracker = pInfo->m_rcPos;
	m_ptOffset.x = point.x - m_rectTracker.left;
	m_ptOffset.y = point.y - m_rectTracker.top;

	SetCapture();
	SetFocus();

	pInfo->m_nStatus = constBtnDown;

	RedrawWindow(pInfo->m_rcPos);

	m_bTracking = TRUE;
	//OnInvertTracker(m_rectTracker);
	int n = m_rectTracker.left + m_rectTracker.Width() / 2;
	ShowToolTip(m_lTimeStart + m_nUnit * n / m_nPixelUnit);

	m_nClickCtrl = nCtrl;
	m_ptPrePos = point;
	m_bFirstMove = FALSE;

	CWnd::OnLButtonDown(nFlags, point);
}

void CKeyFramePanel::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	m_bTracking = FALSE;
	m_pTooltipWnd->ShowWindow(SW_HIDE);

	ReleaseCapture();

	CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(m_nClickCtrl);

	BOOL bMoved = FALSE;
	if (m_bFirstMove)
	{
		OnInvertTracker(m_rectTracker);

		int n = m_rectTracker.left + m_rectTracker.Width() / 2;
		long lTime = m_lTimeStart + m_nUnit * n / m_nPixelUnit;
		m_bFirstMove = FALSE;
		bMoved = TRUE;

		if (!CheckTimeDup(lTime))
		{
			GetParent()->SendMessage(WM_KEY_FRAME_TOBE_MOVED);

			pInfo->m_lTime = lTime;
#if 1
			m_pKeyFrameArray->RemoveAt(m_nClickCtrl);
			BOOL isAdded = FALSE;
			for (int i = m_pKeyFrameArray->GetSize(); i > 0; i--)
			{
				if (((CKeyFrameObj*)m_pKeyFrameArray->GetAt(i - 1))->m_lTime < pInfo->m_lTime)
				{
					m_pKeyFrameArray->InsertAt(i, pInfo, 1);
					isAdded = TRUE;
					break;
				}
			}
			if (!isAdded)
			{
				m_pKeyFrameArray->Add(pInfo);
			}
#endif
			ResetTooltip();
			
			GetParent()->SendMessage(WM_KEY_FRAME_MOVED, m_nClickCtrl, (LPARAM)pInfo->m_lTime);
		}
		else
		{
			W_SHOW_ERROR(IDS_CONTROL_POINT_EXISTS, this);
		}
	}

	int nCurCtrl = CtrlEntered(point);
	BOOL bIn = (nCurCtrl == m_nClickCtrl);
	if (bIn)
		pInfo->m_nStatus = constBtnOver;
	else if (!bIn)
		pInfo->m_nStatus = constBtnNormal;

	RedrawWindow();

	if (bIn && !bMoved)
		GetParent()->PostMessage(WM_KEY_FRAME_PRESSED, m_nClickCtrl, (LPARAM)pInfo->m_lTime);

	CWnd::OnLButtonUp(nFlags, point);
}

void CKeyFramePanel::OnInvertTracker(const CRect& rect)
{
	CRect rc = rect;
	ClientToScreen(rc);
	::DrawResizeBar(rc);
}

void CKeyFramePanel::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_bTracking)
	{
		m_ptPrePos = point;
		
		CRect rcClient;
		GetClientRect(rcClient);

		int nPos = TimeToPos(m_lLength);
		int nMaxPos = min(rcClient.right, nPos);

		int nWidth = m_rectTracker.Width();
		if (point.x - m_ptOffset.x <= -nWidth / 2)
			point.x = m_ptOffset.x - nWidth / 2;
		else if (point.x - m_ptOffset.x + nWidth / 2 >= nMaxPos)
			point.x = nMaxPos - nWidth / 2 + m_ptOffset.x;

		if (m_rectTracker.left + m_ptOffset.x != point.x)
		{
			if (m_bFirstMove)
				OnInvertTracker(m_rectTracker);
			
			m_rectTracker.left = point.x - m_ptOffset.x;
			m_rectTracker.right = point.x - m_ptOffset.x + nWidth;
			
			OnInvertTracker(m_rectTracker);

			int n = m_rectTracker.left + m_rectTracker.Width() / 2;
			ShowToolTip(m_lTimeStart + m_nUnit * n / m_nPixelUnit);

			m_bFirstMove = TRUE;
		}
	}
	else
	{
		if (m_pKeyFrameArray->GetSize() == 0)
			return;

		LeaveTrack();
		
		int nCtrl = CtrlEntered(point);
		if (nCtrl != m_nCtrlEntered)
			LeaveCtrl(m_nCtrlEntered);
		if (nCtrl < 0)
		{
	   		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			return;
		}
		::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
		EnterCtrl(nCtrl);
	}
}

void CKeyFramePanel::ShowToolTip(long nPos)
{
	CPoint pt;
	GetCursorPos(&pt);
	pt.x += 6;
	pt.y += 20;

	CString strTime = GetTimeStr(nPos);
	m_pTooltipWnd->ShowText(pt, strTime);
}

CString CKeyFramePanel::GetTimeStr(long nPos)
{
	CString strTime = ::ConvertTimeToStr(nPos);
	return strTime;
}

void CKeyFramePanel::LeaveTrack() 
{
	if (!m_bLeaveTrack)
	{
		m_bLeaveTrack = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
	}
}

BOOL CKeyFramePanel::CheckTimeDup(long lTime)
{
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		if (lTime == pInfo->m_lTime && i != m_nClickCtrl)
			return TRUE;
	}

	return FALSE;
}

LRESULT CKeyFramePanel::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pInfo = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		if (pInfo->m_nStatus == constBtnOver) //Button
		{
			pInfo->m_nStatus = constBtnNormal;
			RedrawWindow(pInfo->m_rcPos);
		}
	}
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return 0;
}

void CKeyFramePanel::SetRectChannel(CRect rcChannel)
{
	m_rcChannel = rcChannel;
	RedrawWindow();
}

LRESULT CKeyFramePanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
    {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			{
				MSG msg;
				msg.hwnd = m_hWnd;
				msg.message = message;
				msg.wParam = wParam;
				msg.lParam = lParam;

				if (m_pToolTip)
					m_pToolTip->RelayEvent(&msg);
			}
    }

	return CWnd::WindowProc(message, wParam, lParam);
}

void CKeyFramePanel::SetKeyFrame(CStringArray& strArrayMark,
								 CStringArray& strArrayScript,
								 CStringArray& strArrayCmd,
								 CStringArray& strArrayTime)
{
	RemoveAllKeyFrame();
	
	for (int i = 0; i < strArrayTime.GetSize(); i++)
	{
		CString strMark = strArrayMark[i];
		CString strScript = strArrayScript[i];
		CString strCmd = strArrayCmd[i];
		CString strTime = strArrayTime[i];
		long lTime = ::ConvertStrToTime(strTime);
		KEY_TYPE nType;
		if (::IsCtrlMark(strMark) && (strCmd == "zoomin" || strCmd == "zoomout"))
			nType = (strCmd == "zoomin") ? KEY_SQ_IN : KEY_SQ_OUT;
		else if (::IsCtrlMark(strMark) && strCmd == "demo")
			nType = KEY_DEMO;
		else if (strCmd == "drawrect" || strCmd == "drawline" || strCmd == "drawmark" || strCmd == "freehand")
			nType = KEY_DRAW;
		else if (strCmd == "flippage")
			nType = KEY_BLUE;
		else if (strCmd == "flipslide")
			nType = (strScript.Find(".") < 0) ? KEY_RED : KEY_GREEN;
		AddKeyFrame(strMark, strScript, lTime, nType, strCmd);
	}
	ResetTooltip();
	RedrawWindow();
}

void CKeyFramePanel::OnRButtonUp(UINT nFlags, CPoint point) 
{
#ifdef _BUNDLE
	#ifndef _AENRICH
		CWnd::OnRButtonUp(nFlags, point);
		return;
	#endif
#endif

	if (!m_bDeletable && !m_bEditable)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	int nCtrl = CtrlEntered(point);
	if (nCtrl < 0)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	m_nCtrlEntered = -1;

	GetParent()->SendMessage(WM_RBUTTON_CLICKED);
	if (!m_bCanRMenu)
	{
		CWnd::OnRButtonUp(nFlags, point);
		return;
	}

	CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(nCtrl);
	int nType = pObj->m_nType;
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_CP));
	CMenu* pPopup = menu.GetSubMenu(0);
	
	if (nType != KEY_DEMO)
	{
		pPopup->DeleteMenu(ID_DEMO_CAPTURE, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_IMPORT_DEMO_FILE, MF_BYCOMMAND);
	}
	else
	{
		pPopup->InsertMenu(ID_DEMO_CAPTURE, MF_BYCOMMAND | MF_SEPARATOR);
		pPopup->InsertMenu(ID_VIEW_OBJ, MF_BYCOMMAND | MF_SEPARATOR);
	}

	if (nType != KEY_PRACTICE)
		pPopup->DeleteMenu(ID_DESIGN, MF_BYCOMMAND);
	if (nType != KEY_PRACTICE && nType != KEY_DEMO)
		pPopup->DeleteMenu(ID_VIEW_OBJ, MF_BYCOMMAND);

	//if (!m_bDeletable)
	//{
	//	if (nType == KEY_RED || nType == KEY_GREEN)
	//		pPopup->DeleteMenu(ID_DELETE_CP, MF_BYCOMMAND);
	//}
	
	if (!m_bEditable)
		pPopup->DeleteMenu(ID_DELETE_CP, MF_BYCOMMAND);

	ASSERT(pPopup != NULL);
	CPoint pts(point);
	ClientToScreen(&pts);
	UINT nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RIGHTBUTTON | TPM_RETURNCMD, pts.x, pts.y,this);

	if (nCmd > 0)
		GetParent()->PostMessage(WM_CP_ACTION, nCtrl, nCmd);

	CWnd::OnRButtonUp(nFlags, point);
}

void CKeyFramePanel::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pTooltipWnd->DestroyWindow();
	delete m_pTooltipWnd;
}

void CKeyFramePanel::SetPixelUnit(int lTime)
{
	m_nPixelUnit = lTime;
}

void CKeyFramePanel::SetTimeStart(long lTime)
{
	m_lTimeStart = lTime;
}

void CKeyFramePanel::SetTimeUnit(int nTimeUnit)
{
	m_nUnit = nTimeUnit;
}

void CKeyFramePanel::DeletePeriod(long lTimeStart, long lTimeEnd)
{
	BOOL bFound = FALSE;
	int i = 0;
	for (i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		long lTime = pObj->m_lTime;
		if (lTime > lTimeStart && lTime < lTimeEnd)
		{
			bFound = TRUE;
			break;
		}
	}

	BOOL bDel = FALSE;
	if (bFound)
	{
		if (W_ASK_YESNO(IDS_SLIDER_DELETED, this) == IDYES)
			bDel = TRUE;
	}

	long lOffsetTime = 1000;
	for (i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		long lTime = pObj->m_lTime;
		if (lTime > lTimeStart && lTime < lTimeEnd)
		{
			if (bDel)
			{
				m_pKeyFrameArray->RemoveAt(i);
				delete pObj;
				i--;
			}
			else
			{
				pObj->m_lTime = lTimeStart + lOffsetTime;
				lOffsetTime += 1000;
			}
		}
		else if (lTime >= lTimeEnd)
		{
			long lTimeDelta = lTimeEnd - lTimeStart;
			pObj->m_lTime -= lTimeDelta;
		}
	}

	m_lLength -= (lTimeEnd - lTimeStart);
}

void CKeyFramePanel::InsertPeriod(long lTimeStart, long lDuration)
{
	BOOL bFound = FALSE;
	for (int i = 0; i < m_pKeyFrameArray->GetSize(); i++)
	{
		CKeyFrameObj* pObj = (CKeyFrameObj*)m_pKeyFrameArray->GetAt(i);
		long lTime = pObj->m_lTime;
		if (lTime > lTimeStart)
			pObj->m_lTime = lTime + lDuration;
	}

	m_lLength += lDuration;
}

void CKeyFramePanel::TagChanged()
{		
	ResetTooltip();
	RedrawWindow();

	GetParent()->SendMessage(WM_REGEN_TAG);
}

void CKeyFramePanel::Initialize()
{
}

void CKeyFramePanel::GetFrameInfo(int nIndex, CString& strTitle, long& lTime, 
								  CString& strCmd, CString& strStep)
{
	CKeyFrameObj* pObj = GetFrameObj(nIndex);
	strTitle = pObj->m_strTitle;
	lTime = pObj->m_lTime;
	strCmd = pObj->m_strCmd;
	strStep = pObj->m_strTag;

	return;
}
