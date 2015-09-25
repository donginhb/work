// DlgEditMark.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgEditMark.h"
#include "ColGrid.h"
#include "cmkapi.h"
#include "EditAllowed.h"
#include "macro.h"
#include "eLearningView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditMark dialog


CDlgEditMark::CDlgEditMark(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditMark::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditMark)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArrayTime = NULL;
	m_pstrArrayHyper = NULL;
	m_pstrArrayStep = NULL;
	m_strTimeMax = "99:99:99.9";
	m_pEditSDelay = new CEditAllowed;
	m_pEditADelay = new CEditAllowed;
	m_pEditSDelay->m_strAllowed = "0123456789.-";
	m_pEditADelay->m_strAllowed = "0123456789.-";
	//m_bDeleteAtPoint = FALSE;
	m_nPrePage = -1;
	m_nPreCell = -1;
}


void CDlgEditMark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditMark)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditMark, CDialog)
	//{{AFX_MSG_MAP(CDlgEditMark)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_MESSAGE(WM_GRD_UPDATE, OnGrdUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditMark message handlers

BOOL CDlgEditMark::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	
	CString strTitle;
	strTitle.LoadString(IDS_SLIDER_POINT);
    TabCtrlItem.pszText = "スライド";//(LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(0, &TabCtrlItem);
	
	strTitle.LoadString(IDS_ZOOM_POINT);
	TabCtrlItem.pszText = "ズーム";//(LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(1, &TabCtrlItem);

	strTitle.LoadString(IDS_DEMO_POINT);
	TabCtrlItem.pszText = "デモ";//(LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(2, &TabCtrlItem);

	strTitle.LoadString(IDS_MARK_POINT);
	TabCtrlItem.pszText = "手書き";//(LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(3, &TabCtrlItem);

	//strTitle.LoadString(IDS_PRACTICE_POINT);
	//TabCtrlItem.pszText = (LPTSTR)(LPCTSTR)strTitle;
	//pTab->InsertItem(3, &TabCtrlItem);

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	pTab->ScreenToClient(rcGrid);
	int nWidth = rcGrid.Width() - ::GetSystemMetrics(SM_CXVSCROLL);

	m_pZoomGrid = new CColGrid;
	m_pZoomGrid->Create(rcGrid, pTab, 0);
	m_pZoomGrid->ShowWindow(SW_HIDE);

	m_pZoomGrid->SetTotalColNo(3);
	m_pZoomGrid->SetHeadWidth((int)(nWidth * 0.1));
	m_pZoomGrid->SetColWidth(0, 0); 
	m_pZoomGrid->SetColWidth(1, (int)(nWidth * 0.2));
	m_pZoomGrid->SetColWidth(2, (int)(nWidth * 0.2));
	strTitle.LoadString(IDS_GRID_ZOOM_TITLE);
	m_pZoomGrid->SetFixedRowLabel(strTitle);
	m_pZoomGrid->u_bLastRowStar = TRUE;
	m_pZoomGrid->u_bDragTitle = FALSE;
	m_pZoomGrid->u_bSort = FALSE;
	m_pZoomGrid->u_pMSGWnd = this;
	m_pZoomGrid->SetGridFont(GetFont());
	m_pZoomGrid->SetGridSelType(cGridHeadSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pZoomGrid->SetColAttribAt(0, cEditable, 0); 
	m_pZoomGrid->SetColAttribAt(1, cEditable, 1); 
	m_pZoomGrid->SetColAttribAt(2, cEditable, 1); 
	m_pZoomGrid->SetColAttribAt(2, cType, cGridCombo);
	m_pZoomGrid->SetColAttribAt(2, cIndex, 0);
	CString strCombo, strTmp;
	strTmp.LoadString(IDS_ZOOM_IN);
	strCombo = strTmp;
	strTmp.LoadString(IDS_ZOOM_OUT);
	strCombo += "\\" + strTmp;
	m_pZoomGrid->SetSeqAt(cSeqTextCombo, 0, strCombo); 

	if (PROMOTE_VERSION)
		m_pZoomGrid->u_bLastRowStar = FALSE;
	else
		m_pZoomGrid->AppendRow(1);	

	m_pDemoGrid = new CColGrid;
	m_pDemoGrid->Create(rcGrid, pTab, 0);
	m_pDemoGrid->ShowWindow(SW_HIDE);

	m_pDemoGrid->SetTotalColNo(2);
	m_pDemoGrid->SetHeadWidth((int)(nWidth * 0.1));
	m_pDemoGrid->SetColWidth(0, (int)(nWidth * 0.2));
	m_pDemoGrid->SetColWidth(1, (int)(nWidth * 0.2));
	strTitle.LoadString(IDS_GRID_DEMO_TITLE);
	m_pDemoGrid->SetFixedRowLabel(strTitle);
	m_pDemoGrid->u_bLastRowStar = TRUE;
	m_pDemoGrid->u_bDragTitle = FALSE;
	m_pDemoGrid->u_bSort = FALSE;
	m_pDemoGrid->u_pMSGWnd = this;
	m_pDemoGrid->SetGridFont(GetFont());
	m_pDemoGrid->SetGridSelType(cGridHeadSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pDemoGrid->SetColAttribAt(0, cEditable, 1); 
	m_pDemoGrid->SetColAttribAt(1, cEditable, 1); 
	m_pDemoGrid->SetColAttribAt(1, cType, cGridCombo);
	m_pDemoGrid->SetColAttribAt(1, cIndex, 0);
	CString strDemo;
	strDemo.LoadString(IDS_DEMO);
	strCombo = "";
	int i = 0;
	for (i = 0; i < 20; i++)
	{
		strTmp.Format("%s%d", strDemo, i + 1);
		strCombo += strTmp + "\\";
	}
	m_pDemoGrid->SetSeqAt(cSeqTextCombo, 0, strCombo); 

	if (PROMOTE_VERSION)
		m_pDemoGrid->u_bLastRowStar = FALSE;
	else
		m_pDemoGrid->AppendRow(1);	

	m_pMarkGrid = new CColGrid;
	m_pMarkGrid->Create(rcGrid, pTab, 0);
	m_pMarkGrid->ShowWindow(SW_HIDE);

	m_pMarkGrid->SetTotalColNo(10);
	m_pMarkGrid->SetHeadWidth((int)(nWidth * 0.1));
	m_pMarkGrid->SetColWidth(0, (int)(nWidth * 0.2));
	m_pMarkGrid->SetColWidth(1, (int)(nWidth * 0.2));
	m_pMarkGrid->SetColWidth(2, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(3, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(4, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(5, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(6, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(7, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(8, (int)(nWidth * 0.15));
	m_pMarkGrid->SetColWidth(9, 0);
	strTitle.LoadString(IDS_GRID_MARK_TITLE);
	m_pMarkGrid->SetFixedRowLabel(strTitle);
	m_pMarkGrid->u_bLastRowStar = FALSE;
	m_pMarkGrid->u_bDragTitle = FALSE;
	m_pMarkGrid->u_bSort = FALSE;
	m_pMarkGrid->u_pMSGWnd = this;
	m_pMarkGrid->SetColAttribAt(1, cEditable, 0); 
	m_pMarkGrid->SetColAttribAt(9, cEditable, 0); 
	m_pMarkGrid->SetColAttribAt(8, cType, cGridBMPCombo);
	CBitmap *pBmp1;
	CString strBmpComboSeq;
	for (int i = IDB_PTR_FIRST; i <= IDB_PTR_LAST; i++)
	{
		strBmpComboSeq.AppendFormat("%d", i - IDB_PTR_FIRST);
		strBmpComboSeq += "\\";
		pBmp1 = new CBitmap();
		pBmp1->LoadBitmap(i);
		m_pMarkGrid->SetBMPAt(cBMP, i - IDB_PTR_FIRST, pBmp1);
	}
	strBmpComboSeq = strBmpComboSeq.Left(strBmpComboSeq.GetLength() - 1);
	m_pMarkGrid->SetSeqAt(cSeqBMPCombo, 0, strBmpComboSeq);
	m_pMarkGrid->SetGridFont(GetFont());
	m_pMarkGrid->SetGridSelType(cGridHeadSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pMarkGrid->SetColAttribAt(7, cType, cGridCombo);
	m_pMarkGrid->SetColAttribAt(7, cIndex, 0);
	strCombo = "";
	for (i = 1; i <= 15; i++)
	{
		strTmp.Format("%d", i);
		strCombo += strTmp + "\\";
	}
	m_pMarkGrid->SetSeqAt(cSeqTextCombo, 0, strCombo); 

	m_pSliderGrid = new CColGrid;
	m_pSliderGrid->Create(rcGrid, pTab, 0);
	m_pSliderGrid->SetTotalColNo(6);
	m_pSliderGrid->SetHeadWidth((int)(nWidth * 0.1));
	m_pSliderGrid->SetColWidth(0, (int)(nWidth * 0.2)); 
	m_pSliderGrid->SetColWidth(1, (int)(nWidth * 0.45));
	m_pSliderGrid->SetColWidth(2, (int)(nWidth * 0.1));
	m_pSliderGrid->SetColWidth(3, (int)(nWidth * 0.15));
	m_pSliderGrid->SetColWidth(4, 0);
	m_pSliderGrid->SetColWidth(5, 0);
	strTitle.LoadString(IDS_GRID_SLIDER_TITLE);
	m_pSliderGrid->SetFixedRowLabel("時間\\インデックス名\\カテゴリ\\ページ\\");//strTitle);
	m_pSliderGrid->u_bLastRowStar = FALSE;
	m_pSliderGrid->u_bDragTitle = FALSE;
	m_pSliderGrid->u_bSort = FALSE;
	m_pSliderGrid->u_pMSGWnd = this;
	m_pSliderGrid->SetGridFont(GetFont());
	m_pSliderGrid->SetGridSelType(cGridHeadSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pSliderGrid->SetColAttribAt(0, cEditable, 1); 
	m_pSliderGrid->SetColAttribAt(1, cEditable, 1); 
	m_pSliderGrid->SetColAttribAt(2, cEditable, 0); 
	m_pSliderGrid->SetColAttribAt(3, cEditable, 0); 
	m_pSliderGrid->SetColAttribAt(4, cEditable, 0); 
	m_pSliderGrid->SetColAttribAt(5, cEditable, 0); 
	m_pSliderGrid->SetColAttribAt(2, cAlign, 1); 
	m_pSliderGrid->SetColAttribAt(2, cType, cGridBMP); 
	m_pSliderGrid->SetColAttribAt(2, cIndex, 0); 
	m_pSliderGrid->SetSeqAt(cSeqBMPCombo, 0, "0\\1");
	
	CBitmap *pBmp;
	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_PPT);
	m_pSliderGrid->SetBMPAt(cBMP, 0, pBmp);

	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_HTM);
	m_pSliderGrid->SetBMPAt(cBMP, 1, pBmp);

	if (m_pstrArrayTime && m_pstrArrayHyper && m_pstrArrayStep)
	{
		int nSize = m_pstrArrayTime->GetSize();
		int nIndexSlider = 0;
		int nIndexZoom = 0;
		int nIndexDemo = 0;
		int nIndexDraw = 0;
		//int nIndexPractice = 0;
		for (int i = 0; i < nSize; i++)
		{
			CString strTime = m_pstrArrayTime->GetAt(i);
			CString strHyper = m_pstrArrayHyper->GetAt(i);
			CString strStep = m_pstrArrayStep->GetAt(i);
			CString strCmd = m_pstrArrayCmd->GetAt(i);
			if (!::IsCtrlMark(strHyper))
			{
				if (strCmd == "flipslide" || strCmd == "flippage")
				{
					m_pSliderGrid->AppendRow(1);
					m_pSliderGrid->SetDataAt(nIndexSlider, 0, strTime);
					if (strHyper.GetLength() >= 5)
						m_pSliderGrid->SetDataAt(nIndexSlider, 1, strHyper.Mid(5));
					else
						m_pSliderGrid->SetDataAt(nIndexSlider, 1, "");
					if (strHyper == "")
						m_pSliderGrid->SetAttribAt(nIndexSlider, 1, cEditable, 0);

					int iFind = strStep.Find('@');
					if (iFind > 0)
					{
						CString strOp = strStep.Right(strStep.GetLength() - iFind);
						strStep = strStep.Left(iFind);
						m_pSliderGrid->SetDataAt(nIndexSlider, 4, strOp);
					}
					m_pSliderGrid->SetDataAt(nIndexSlider, 3, strStep);
					m_pSliderGrid->SetDataAt(nIndexSlider, 5, strCmd);
					CString strType;
					if (strCmd == "flipslide")
						strType = "0";
					else 
						strType = "1";
					m_pSliderGrid->SetDataAt(nIndexSlider, 2, strType);


					nIndexSlider++;
				}

			}
			else
			{
				if (strCmd == "zoomin" || strCmd == "zoomout")
				{
					m_pZoomGrid->AppendRow(1);
					m_pZoomGrid->SetDataAt(nIndexZoom, 0, strStep);
					m_pZoomGrid->SetDataAt(nIndexZoom, 1, strTime);
					CString strZoom;
					if (strCmd == "zoomin")
						strZoom.LoadString(IDS_ZOOM_IN);
					else
						strZoom.LoadString(IDS_ZOOM_OUT);
					m_pZoomGrid->SetDataAt(nIndexZoom++, 2, strZoom);
				}
				else if (strCmd == "demo")
				{
					m_pDemoGrid->AppendRow(1);
					m_pDemoGrid->SetDataAt(nIndexDemo, 0, strTime);
					CString strDemo;
					strDemo.LoadString(IDS_DEMO);
					m_pDemoGrid->SetDataAt(nIndexDemo++, 1, strDemo + strStep);
				}
				//else if (strCmd == "practice")
				//{
				//	m_pPracticeGrid->AppendRow(1);
				//	m_pPracticeGrid->SetDataAt(nIndexPractice, 0, strTime);
				//	CString strPractice;
				//	strPractice.LoadString(IDS_PRACTICE);
				//	m_pPracticeGrid->SetDataAt(nIndexPractice++, 1, strPractice + strStep);
				//}
			}
		}
	}

	CStringArray* strArrayDrawTime = MACRO_VIEW->GetDrawTime();
	CStringArray* strArrayDrawColor = MACRO_VIEW->GetDrawColor();
	CStringArray* strArrayDrawWidth = MACRO_VIEW->GetDrawWidth();
	CStringArray* strArrayDrawCmd = MACRO_VIEW->GetDrawCmd();
	CStringArray* strArrayDrawLeft = MACRO_VIEW->GetDrawLeft();
	CStringArray* strArrayDrawTop = MACRO_VIEW->GetDrawTop();
	CStringArray* strArrayDrawRight = MACRO_VIEW->GetDrawRight();
	CStringArray* strArrayDrawBottom = MACRO_VIEW->GetDrawBottom();
	for (int i = 0; i < strArrayDrawTime->GetSize(); i++)
	{
		m_pMarkGrid->AppendRow(1);
		m_pMarkGrid->SetDataAt(i, 0, MlsecToHour(strArrayDrawTime->GetAt(i)));
		m_pMarkGrid->SetDataAt(i, 1, GetTypeText(strArrayDrawCmd->GetAt(i)));
		if (strArrayDrawCmd->GetAt(i) != "freehand" && strArrayDrawCmd->GetAt(i) != "eraser")
		{
			m_pMarkGrid->SetDataAt(i, 2, SkipLeftZero(strArrayDrawLeft->GetAt(i)));
			m_pMarkGrid->SetDataAt(i, 3, SkipLeftZero(strArrayDrawTop->GetAt(i)));
			m_pMarkGrid->SetDataAt(i, 4, SkipLeftZero(strArrayDrawRight->GetAt(i)));
			m_pMarkGrid->SetDataAt(i, 5, SkipLeftZero(strArrayDrawBottom->GetAt(i)));
		}
		else
		{
			m_pMarkGrid->SetAttribAt(i, 2, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 2, cEditable, 0);
			m_pMarkGrid->SetAttribAt(i, 3, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 3, cEditable, 0);
			m_pMarkGrid->SetAttribAt(i, 4, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 4, cEditable, 0);
			m_pMarkGrid->SetAttribAt(i, 5, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 5, cEditable, 0);
		}
		m_pMarkGrid->SetDataAt(i, 6, strArrayDrawColor->GetAt(i));
		if (strArrayDrawCmd->GetAt(i) != "mark")
		{
			m_pMarkGrid->SetDataAt(i, 7, strArrayDrawWidth->GetAt(i));
			m_pMarkGrid->SetAttribAt(i, 8, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 8, cEditable, 0);
		}
		else
		{
			m_pMarkGrid->SetDataAt(i, 8, GetStampIndex(strArrayDrawWidth->GetAt(i)));
			m_pMarkGrid->SetAttribAt(i, 7, cType, cGridText);
			m_pMarkGrid->SetAttribAt(i, 7, cEditable, 0);
		}
	}

	m_pEditSDelay->SubclassDlgItem(IDC_EDIT_DELAY, this);
	m_pEditADelay->SubclassDlgItem(IDC_EDIT_A_DELAY, this);

//	if (m_bDeleteAtPoint)
		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);

	BOOL bRet = TRUE;
	if (m_nPrePage >= 0)
	{
		CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
		if (m_nPrePage < 4)
		{
			pTab->SetCurSel(m_nPrePage);
			ChangeSel();
			if (m_nPrePage == 0)
				m_pSliderGrid->ClickRowCol(m_nPreCell, 0);
			else if (m_nPrePage == 1)
				m_pZoomGrid->ClickRowCol(m_nPreCell, 1);
			else if (m_nPrePage == 2)
				m_pDemoGrid->ClickRowCol(m_nPreCell, 0);
			else if (m_nPrePage == 3)
				m_pMarkGrid->ClickRowCol(m_nPreCell, 0);

			//else if (m_nPrePage == 3)
			//	m_pPracticeGrid->ClickRowCol(m_nPreCell, 0);

			bRet = FALSE;
		}
	}

	return bRet;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CDlgEditMark::MakeupZero(double d)
{
	CString strRet;
	strRet.Format("%3.2f", d);
	int nZero = 6 - strRet.GetLength();
	for (int i = 0; i < nZero; i++)
		strRet = "0" + strRet;
	return strRet;
}

CString CDlgEditMark::MakeupZero(CString strD)
{
	float d = atof(strD);
	CString strRet;
	strRet.Format("%3.2f", d);
	int nZero = 6 - strRet.GetLength();
	for (int i = 0; i < nZero; i++)
		strRet = "0" + strRet;
	return strRet;
}

void CDlgEditMark::SkipLeftZero(CString& strTmp)
{
	double d = atof(strTmp);
	strTmp.Format("%.2f", d);
}

CString CDlgEditMark::SkipLeftZero(CString strTmp)
{
	double d = atof(strTmp);
	strTmp.Format("%.2f", d);
	return strTmp;
}

void CDlgEditMark::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pSliderGrid->DestroyWindow();
	delete m_pSliderGrid;	

	m_pZoomGrid->DestroyWindow();
	delete m_pZoomGrid;	

	m_pDemoGrid->DestroyWindow();
	delete m_pDemoGrid;	

	m_pMarkGrid->DestroyWindow();
	delete m_pMarkGrid;	

	//m_pPracticeGrid->DestroyWindow();
	//delete m_pPracticeGrid;	

	delete m_pEditSDelay;
	delete m_pEditADelay;
}

void CDlgEditMark::SetArrayPtr(CStringArray* pstrArrayTime, 
							   CStringArray* pstrArrayHyper, 
							   CStringArray* pstrArrayStep,
							   CStringArray* pstrArrayCmd)
{
	m_pstrArrayTime = pstrArrayTime; 
	m_pstrArrayHyper = pstrArrayHyper; 
	m_pstrArrayStep = pstrArrayStep;
	m_pstrArrayCmd = pstrArrayCmd;
}

int CDlgEditMark::GetDataSize(CColGrid* pGrid)
{
	if (pGrid == m_pSliderGrid || pGrid == m_pMarkGrid)
		return pGrid->GetDataSize();
	return pGrid->GetDataSize() - 1;
}

int CDlgEditMark::GetTabIndex(CColGrid* pGrid)
{
	if (pGrid == m_pSliderGrid)
		return 0;
	else if (pGrid == m_pZoomGrid)
		return 1;
	else if (pGrid == m_pDemoGrid)
		return 2;
	else if (pGrid == m_pMarkGrid)
		return 3;
	//else if (pGrid == m_pPracticeGrid)
	//	return 3;
	
	return -1;
}

int CDlgEditMark::GetTimeIndex(CColGrid* pGrid)
{
	if (pGrid == m_pSliderGrid)
		return 0;
	else if (pGrid == m_pZoomGrid)
		return 1;
	else if (pGrid == m_pDemoGrid)
		return 0;
	else if (pGrid == m_pMarkGrid)
		return 0;
	//else if (pGrid == m_pPracticeGrid)
	//	return 0;

	return -1;
}

BOOL CDlgEditMark::CheckTimeRange(CColGrid* pGrid) 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(pGrid);
	int nTimeCol = GetTimeIndex(pGrid);
	int nTabIndex = GetTabIndex(pGrid);
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = pGrid->GetDataAt(i, nTimeCol);
		str1.TrimLeft();
		str1.TrimRight();
		BOOL bValid1 = ::IsValidTime(str1);
		if (!bValid1)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TIME_ERROR_LINE_I);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(i, nTimeCol);
			return FALSE;
		}
		else if (str1 > m_strTimeMax)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TIME_ERROR_OVER);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(i, nTimeCol);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDlgEditMark::IsValidTime1(CString strTime)
{
	strTime.TrimLeft();
	strTime.TrimRight();
	if (strTime.GetLength() != 11)
		return FALSE;
	if (strTime[2] != ':' || strTime[5] != ':' || strTime[8] != '.')
		return FALSE;
	if (!IsDigit(strTime.Mid(0, 2)) || !IsDigit(strTime.Mid(3, 2)) || 
		!IsDigit(strTime.Mid(6, 2)) || !IsDigit(strTime.Mid(9, 2)))
		return FALSE;
	if (!(atoi(strTime.Mid(3, 2)) < 60) || !(atoi(strTime.Mid(6, 2)) < 60))
		return FALSE;
	return TRUE;
}

BOOL CDlgEditMark::CheckTimeRange1(CColGrid* pGrid) 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(pGrid);
	int nTimeCol = GetTimeIndex(pGrid);
	int nTabIndex = GetTabIndex(pGrid);
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = pGrid->GetDataAt(i, nTimeCol);
		str1.TrimLeft();
		str1.TrimRight();
		BOOL bValid1 = IsValidTime1(str1);
		if (!bValid1)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TIME_ERROR_LINE_I);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(i, nTimeCol);
			return FALSE;
		}
		else if (str1 > m_strTimeMax)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TIME_ERROR_OVER);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(i, nTimeCol);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDlgEditMark::SelfCheckTimeDup(CColGrid* pGrid) 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(pGrid);
	int nTimeCol = GetTimeIndex(pGrid);
	int nTabIndex = GetTabIndex(pGrid);
	for (int i = 0; i < nSize; i++)
	{
		for (int j = i + 1;  j < nSize; j++)
		{
			CString str1 = pGrid->GetDataAt(i, nTimeCol);
			CString str2 = pGrid->GetDataAt(j, nTimeCol);
			if (str1 == str2)
			{
				pTab->SetCurSel(nTabIndex);
				ChangeSel();

				strTmp.LoadString(IDS_TIME_ERROR_DUP);
				strMsg.Format(strTmp, i + 1, j + 1);
				W_SHOW_ERROR_STR(strMsg, this);
				pGrid->ClickRowCol(i, nTimeCol);
	
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CDlgEditMark::CheckTimeDupStr(CColGrid* pGrid, CString str1, int i) 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(pGrid);
	int nTimeCol = GetTimeIndex(pGrid);
	int nTabIndex = GetTabIndex(pGrid);
	for (int j = 0;  j < nSize; j++)
	{
		CString str2 = pGrid->GetDataAt(j, nTimeCol);
		if (str1 == str2)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TIME_ERROR_DUP_P);
			strMsg.Format(strTmp, j + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(j, nTimeCol);

			return FALSE;
		}
	}

	return TRUE;
}


BOOL CDlgEditMark::CheckMarkGrid()
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(m_pMarkGrid);
	int nTabIndex = GetTabIndex(m_pMarkGrid);
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		CString str;
		int nEditable;
		int j = 0;
		for (j = 2; j <= 5; j++)
		{
			str = m_pMarkGrid->GetDataAt(i, j);
			nEditable = m_pMarkGrid->GetAttribAt(i, j, cEditable);
			str.TrimLeft();
			str.TrimRight();
			double d = -1.0;
			CString strU = str.MakeUpper();
			CString strL = str.MakeLower();
			if (::IsValidNumber(str) && strU == strL)
				d = atof(str);
			if ((d < 0 || d > 100) && nEditable)
			{
				pTab->SetCurSel(nTabIndex);
				ChangeSel();

				strTmp.LoadString(IDS_RANGE_ERROR);
				strMsg.Format(strTmp, i + 1);
				W_SHOW_ERROR_STR(strMsg, this);
				m_pMarkGrid->ClickRowCol(i, j);
				return FALSE;
			}
		}

		str = m_pMarkGrid->GetDataAt(i, 6);
		nEditable = m_pMarkGrid->GetAttribAt(i, 6, cEditable);
		str.TrimLeft();
		str.TrimRight();
		BOOL bLenError = (str.GetLength() != 6);
		BOOL bError = FALSE;
		for (j = 0; j <	str.GetLength(); j++)
		{
			char ch = str[j];
			if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f')))
				bError = TRUE;
		}

		if ((bError || bLenError) && nEditable)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_COLOR_ERROR);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			m_pMarkGrid->ClickRowCol(i, 6);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgEditMark::CheckColNull(CColGrid* pGrid, int nCheckCol) 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSize = GetDataSize(pGrid);
	int nTabIndex = GetTabIndex(pGrid);
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = pGrid->GetDataAt(i, nCheckCol);
		int nEditable = pGrid->GetAttribAt(i, nCheckCol, cEditable);
		str1.TrimLeft();
		str1.TrimRight();
		if (str1 == "" && nEditable)
		{
			pTab->SetCurSel(nTabIndex);
			ChangeSel();

			strTmp.LoadString(IDS_TYPE_ERROR);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			pGrid->ClickRowCol(i, nCheckCol);
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgEditMark::OnOK() 
{
	CString strTmp, strMsg;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	if (!CheckTimeRange(m_pSliderGrid))
	 	return;
	if (!CheckTimeRange(m_pZoomGrid))
		return;

	if (!CheckColNull(m_pSliderGrid, 1))
		return;
	if (!CheckColNull(m_pZoomGrid, 2))
		return;
	if (!CheckColNull(m_pDemoGrid, 1))
		return;
	if (!CheckColNull(m_pMarkGrid, 2))
		return;
	if (!CheckColNull(m_pMarkGrid, 3))
		return;
	if (!CheckColNull(m_pMarkGrid, 4))
		return;
	if (!CheckColNull(m_pMarkGrid, 5))
		return;
	if (!CheckColNull(m_pMarkGrid, 6))
		return;
	if (!CheckColNull(m_pMarkGrid, 7))
		return;
	
	if (!CheckMarkGrid())
		return;

	//if (!CheckColNull(m_pPracticeGrid, 1))
	//	return;
	int i = 0;
	for (i = 0; i < m_pZoomGrid->GetDataSize() - 1; i++)
	{
		CString str1 = m_pZoomGrid->GetDataAt(i, 0);
		str1.TrimLeft();
		str1.TrimRight();
		if (str1 == "")
		{
			char szBuf[32];
			CString strTag = ltoa(GetTickCount() + 10000, szBuf, 10);
			m_pZoomGrid->SetDataAt(i, 0, strTag);
		}
	}

	if (!CheckTimeRange(m_pDemoGrid))
		return;
	if (!CheckTimeRange1(m_pMarkGrid))
		return;
	//if (!CheckTimeRange(m_pPracticeGrid))
	//	return;

	if (!SelfCheckTimeDup(m_pSliderGrid))
		return;
	if (!SelfCheckTimeDup(m_pZoomGrid))
		return;
	if (!SelfCheckTimeDup(m_pDemoGrid))
		return;
	if (!SelfCheckTimeDup(m_pMarkGrid))
		return;
	//if (!SelfCheckTimeDup(m_pPracticeGrid))
	//	return;

	for (i = 0; i < m_pSliderGrid->GetDataSize(); i++)
	{
		CString str1 = m_pSliderGrid->GetDataAt(i, 0);
		if (!CheckTimeDupStr(m_pZoomGrid, str1, i))
			return;
		if (!CheckTimeDupStr(m_pDemoGrid, str1, i))
			return;
		if (!CheckTimeDupStr(m_pMarkGrid, str1, i))
			return;
		//if (!CheckTimeDupStr(m_pPracticeGrid, str1, i))
		//	return;
	}

	for (i = 0; i < m_pZoomGrid->GetDataSize() - 1; i++)
	{
		CString str1 = m_pZoomGrid->GetDataAt(i, 1);
		if (!CheckTimeDupStr(m_pDemoGrid, str1, i))
			return;
		if (!CheckTimeDupStr(m_pMarkGrid, str1, i))
			return;
		//if (!CheckTimeDupStr(m_pPracticeGrid, str1, i))
		//	return;
	}

	for (i = 0; i < m_pDemoGrid->GetDataSize() - 1; i++)
	{
		CString str1 = m_pDemoGrid->GetDataAt(i, 0);
		if (!CheckTimeDupStr(m_pMarkGrid, str1, i))
			return;
	//	if (!CheckTimeDupStr(m_pPracticeGrid, str1, i))
	//		return;
	}

	if (m_pstrArrayTime && m_pstrArrayHyper && m_pstrArrayStep && m_pstrArrayCmd)
	{
		m_pstrArrayTime->RemoveAll();
		m_pstrArrayHyper->RemoveAll();
		m_pstrArrayStep->RemoveAll();
		m_pstrArrayCmd->RemoveAll();
		int i = 0;
		for (i = 0; i < m_pSliderGrid->GetDataSize(); i++)
		{
			CString str = m_pSliderGrid->GetDataAt(i, 0);
			m_pstrArrayTime->Add(str);

			CString strStep = m_pSliderGrid->GetDataAt(i, 3);
			CString strOp = m_pSliderGrid->GetDataAt(i, 4);
			CString strCmd = m_pSliderGrid->GetDataAt(i, 5);
			m_pstrArrayStep->Add(strStep + strOp);		

			str = m_pSliderGrid->GetDataAt(i, 1);
			if (str != "")
			{
				CString strType = m_pSliderGrid->GetDataAt(i, 2);
				if (strType == "0")
					strType = "s";
				else
					strType = "w";
				CString strPrefix;
				strPrefix.Format("%s%03d ", strType, atoi(strStep));
				m_pstrArrayHyper->Add(strPrefix + str);	
			}
			else
				m_pstrArrayHyper->Add("");
			
			if (str.GetLength() > 30)
			{
				W_SHOW_ERROR(IDS_INDEX_LONGGER_THAN_30, this);
				return;
			}

			m_pstrArrayCmd->Add(strCmd);		
		}

		for (i = 0; i < m_pZoomGrid->GetDataSize() - 1; i++)
		{
			CString str = m_pZoomGrid->GetDataAt(i, 1);
			m_pstrArrayTime->Add(str);

			str = m_pZoomGrid->GetDataAt(i, 0);
			m_pstrArrayHyper->Add(::MakeCtrlMark(str));	
			m_pstrArrayStep->Add(str);		

			CString strCmd = m_pZoomGrid->GetDataAt(i, 2);
			CString strZoomIn;
			strZoomIn.LoadString(IDS_ZOOM_IN);
			if (strCmd == strZoomIn)
				m_pstrArrayCmd->Add("zoomin");		
			else
				m_pstrArrayCmd->Add("zoomout");		
		}

		for (i = 0; i < m_pDemoGrid->GetDataSize() - 1; i++)
		{
			CString str = m_pDemoGrid->GetDataAt(i, 0);
			m_pstrArrayTime->Add(str);

			str = m_pDemoGrid->GetDataAt(i, 1);
			str = str.Mid(5);
			m_pstrArrayHyper->Add(::MakeDemoMark(str));	
			m_pstrArrayStep->Add(str);		
			m_pstrArrayCmd->Add("demo");		
		}

		while(m_deleteArrayDraw.GetSize())
		{
			MACRO_VIEW->DeleteDrawPoint(m_deleteArrayDraw.GetAt(0));
			m_deleteArrayDraw.RemoveAt(0);
		}

		CStringArray* strArrayDrawTime = MACRO_VIEW->GetDrawTime();
		CStringArray* strArrayDrawColor = MACRO_VIEW->GetDrawColor();
		CStringArray* strArrayDrawWidth = MACRO_VIEW->GetDrawWidth();
		CStringArray* strArrayDrawCmd = MACRO_VIEW->GetDrawCmd();
		CStringArray* strArrayDrawLeft = MACRO_VIEW->GetDrawLeft();
		CStringArray* strArrayDrawTop = MACRO_VIEW->GetDrawTop();
		CStringArray* strArrayDrawRight = MACRO_VIEW->GetDrawRight();
		CStringArray* strArrayDrawBottom = MACRO_VIEW->GetDrawBottom();
		for (int i = 0; i < m_pMarkGrid->GetDataSize(); i++)
		{
			CString strDrawTime = m_pMarkGrid->GetDataAt(i, 0);
			CString strDrawCmd = m_pMarkGrid->GetDataAt(i, 1);
			CString strDrawLeft = m_pMarkGrid->GetDataAt(i, 2);
			CString strDrawTop = m_pMarkGrid->GetDataAt(i, 3);
			CString strDrawRight = m_pMarkGrid->GetDataAt(i, 4);
			CString strDrawBottom = m_pMarkGrid->GetDataAt(i, 5);
			CString strDrawColor = m_pMarkGrid->GetDataAt(i, 6);
			CString strDrawWidth;
			if (strArrayDrawCmd->GetAt(i) != "mark")
				strDrawWidth = m_pMarkGrid->GetDataAt(i, 7);
			else
				strDrawWidth = GetBmpId(m_pMarkGrid->GetDataAt(i, 8));
			strArrayDrawTime->RemoveAt(i);
			strArrayDrawTime->InsertAt(i, HourToMlsec(strDrawTime));
			strArrayDrawColor->RemoveAt(i);
			strArrayDrawColor->InsertAt(i, strDrawColor);
			strArrayDrawWidth->RemoveAt(i);
			strArrayDrawWidth->InsertAt(i, strDrawWidth);
			if (strArrayDrawCmd->GetAt(i) != "freehand" && strArrayDrawCmd->GetAt(i) != "eraser")
			{
				strArrayDrawLeft->RemoveAt(i);
				strArrayDrawLeft->InsertAt(i, MakeupZero(strDrawLeft));
				strArrayDrawTop->RemoveAt(i);
				strArrayDrawTop->InsertAt(i, MakeupZero(strDrawTop));
				strArrayDrawRight->RemoveAt(i);
				strArrayDrawRight->InsertAt(i, MakeupZero(strDrawRight));
				strArrayDrawBottom->RemoveAt(i);
				strArrayDrawBottom->InsertAt(i, MakeupZero(strDrawBottom));
			}
		}

	}

	CDialog::OnOK();
}

void CDlgEditMark::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ChangeSel();
	
	*pResult = 0;
}

void CDlgEditMark::ChangeSel() 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
	{
		m_pSliderGrid->ShowWindow(SW_SHOW);
		m_pZoomGrid->ShowWindow(SW_HIDE);
		m_pDemoGrid->ShowWindow(SW_HIDE);
		m_pMarkGrid->ShowWindow(SW_HIDE);
		//m_pPracticeGrid->ShowWindow(SW_HIDE);

//		if (m_bDeleteAtPoint)
//			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
//		else
//			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
	}
	else if (nSel == 1)
	{
		m_pSliderGrid->ShowWindow(SW_HIDE);
		m_pZoomGrid->ShowWindow(SW_SHOW);
		m_pDemoGrid->ShowWindow(SW_HIDE);
		m_pMarkGrid->ShowWindow(SW_HIDE);
		//m_pPracticeGrid->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
	}
	else if (nSel == 2)
	{
		m_pSliderGrid->ShowWindow(SW_HIDE);
		m_pZoomGrid->ShowWindow(SW_HIDE);
		m_pDemoGrid->ShowWindow(SW_SHOW);
		m_pMarkGrid->ShowWindow(SW_HIDE);
		//m_pPracticeGrid->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
	}
	else if (nSel == 3)
	{
		m_pSliderGrid->ShowWindow(SW_HIDE);
		m_pZoomGrid->ShowWindow(SW_HIDE);
		m_pDemoGrid->ShowWindow(SW_HIDE);
		m_pMarkGrid->ShowWindow(SW_SHOW);
		//m_pPracticeGrid->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
	}
}

void CDlgEditMark::GetSlideRange(int nIndex, int& nLowBound, int& nUpBound) 
{
	nUpBound = nIndex;
	nLowBound = nIndex;
	CString strStep = m_pSliderGrid->GetDataAt(nIndex, 3);
	int nNo;
	int nDot = strStep.Find(".");
	if (nDot < 0)
		nNo = atoi(strStep);
	else
		nNo = atoi(strStep.Left(nDot));

	int nFrameCount = m_pSliderGrid->GetDataSize();
	CString strLowStep = strStep;
	CString strUpStep = strStep;
	int i = 0;
	for (i = nIndex + 1; i < nFrameCount; i++)
	{
		CString strStep2 = m_pSliderGrid->GetDataAt(i, 3);
		int nNo2;
		int nDot2 = strStep2.Find(".");
		if (nDot2 < 0)
			nNo2 = atoi(strStep2);
		else
			nNo2 = atoi(strStep2.Left(nDot2));

		if (nNo != nNo2)
			break;

		nUpBound = i;
		strUpStep = strStep2;
	}

	for (i = nIndex - 1; i >= 0; i--)
	{
		CString strStep2 = m_pSliderGrid->GetDataAt(i, 3);
		int nNo2;
		int nDot2 = strStep2.Find(".");
		if (nDot2 < 0)
			nNo2 = atoi(strStep2);
		else
			nNo2 = atoi(strStep2.Left(nDot2));

		if (nNo != nNo2)
			break;

		nLowBound = i;
		strLowStep = strStep2;
	}
}

void CDlgEditMark::OnButtonDelete() 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
	{
		if (m_pSliderGrid->u_SelectedRow < 0)
		{
			W_SHOW_MESSAGE(IDS_SELECT_FIRST, this);
			return;
		}

		int nIndex = m_pSliderGrid->u_SelectedRow;
		int nIndexEnd = m_pSliderGrid->u_EndSelectedRow;
		int nLowBound, nUpBound, nLowBoundEnd, nUpBoundEnd;
		GetSlideRange(nIndex, nLowBound, nUpBound); 
		GetSlideRange(nIndexEnd, nLowBoundEnd, nUpBoundEnd); 
		int nL = min(nLowBound, nLowBoundEnd);
		int nU = max(nUpBound, nUpBoundEnd);

		if (nL != nIndex || nU != nIndexEnd)
		{
			m_pSliderGrid->u_SelectedRow = nL;
			m_pSliderGrid->u_EndSelectedRow = nU;
			m_pSliderGrid->RePaint();
			if (W_ASK_YESNO(IDS_DEL_SAME_SLIDE, this) == IDNO)
				return;			
		}

		m_pSliderGrid->DelSelected();
	}
	else if (nSel == 1)
	{
		if (m_pZoomGrid->u_SelectedRow < 0)
		{
			W_SHOW_MESSAGE(IDS_SELECT_FIRST, this);
		}

		m_pZoomGrid->DelSelected();
	}
	else if (nSel == 2)
	{
		if (m_pDemoGrid->u_SelectedRow < 0)
		{
			W_SHOW_MESSAGE(IDS_SELECT_FIRST, this);
		}

		m_pDemoGrid->DelSelected();
	}
	else if (nSel == 3)
	{
		if (m_pMarkGrid->u_SelectedRow < 0)
		{
			W_SHOW_MESSAGE(IDS_SELECT_FIRST, this);
		}

		m_deleteArrayDraw.Add(m_pMarkGrid->u_SelectedRow);
		m_pMarkGrid->DelSelected();
	}
	//else if (nSel == 3)
	//{
	//	if (m_pPracticeGrid->u_SelectedRow < 0)
	//	{
	//		W_SHOW_MESSAGE(IDS_SELECT_FIRST, this);
	//	}

	//	m_pPracticeGrid->DelSelected();
	//}
}

void CDlgEditMark::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgEditMark::OnGrdUpdate(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

CString CDlgEditMark::MlsecToHour(CString strMlsec)
{
	long nMlsec = atol(strMlsec);
	int nHour = nMlsec / 3600 / 1000;
	int nMinute = nMlsec / 60 / 1000 - 60 * nHour;
	int nSecond = nMlsec / 1000 - 3600 * nHour - 60 * nMinute;
	nMlsec = (nMlsec - 3600 * 1000 * nHour - 60 * 1000 * nMinute - 1000 * nSecond) / 10;
	CString strHour;
	strHour.Format("%02d:%02d:%02d.%02d", nHour, nMinute, nSecond, nMlsec);
	return strHour;
}

CString CDlgEditMark::HourToMlsec(CString strHour)
{
	CString strTmp = strHour.Left(2);
	int nHour = atoi(strTmp);
	strHour = strHour.Right(strHour.GetLength() - 3);
	strTmp = strHour.Left(2);
	int nMinute = atoi(strTmp);
	strHour = strHour.Right(strHour.GetLength() - 3);
	strTmp = strHour.Left(2);
	int nSecond = atoi(strTmp);
	strHour = strHour.Right(strHour.GetLength() - 3);
	strTmp = strHour.Right(2);
	long nMlsec = atoi(strTmp) * 10;
	nMlsec += 3600 * 1000 * nHour + 60 * 1000 * nMinute + 1000 * nSecond;
	strTmp.Format("%ld", nMlsec);
	return strTmp;
}

CString CDlgEditMark::GetTypeText(CString strCmd)
{
	CString strType;
	if (strCmd == "rect")
		strType.LoadString(IDS_RECT);
	else if (strCmd == "line")
		strType.LoadString(IDS_LINE);
	else if (strCmd == "mark")
		strType.LoadString(IDS_MARK);
	else if (strCmd == "freehand")
		strType.LoadString(IDS_FREEDRAW);
	else if (strCmd == "eraser")
		strType.LoadStringA(IDS_ERASER);
	return strType;
}

CString CDlgEditMark::GetStampIndex(CString strBmpId)
{
	CString strStampIndex;
	int nStampIndex = 0;
	int nBmpId = atoi(strBmpId) + 2000;
	if (nBmpId >= IDB_PTR_FIRST && nBmpId <= IDB_PTR_LAST)
		nStampIndex = nBmpId - IDB_PTR_FIRST;
	strStampIndex.Format("%d", nStampIndex);
	return strStampIndex;
}

CString CDlgEditMark::GetBmpId(CString strStampIndex)
{
	CString strBmpId;
	int nBmpId = IDB_PTR_FIRST;
	int nStampIndex = atoi(strStampIndex);
	if (nStampIndex >= 0 && nStampIndex <= IDB_PTR_LAST - IDB_PTR_FIRST)
		nBmpId = nStampIndex + IDB_PTR_FIRST;
	strBmpId.Format("%d", nBmpId - 2000);
	return strBmpId;
}
