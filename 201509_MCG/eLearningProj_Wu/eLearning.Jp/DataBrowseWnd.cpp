// DataBrowseWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "resource.h"
#include "DataBrowseWnd.h"
#include "MemDC.h"
#include "macro.h"
#include "ColGrid.h"
#include "cmkapi.h"
#include "grdapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataBrowseWnd

CDataBrowseWnd::CDataBrowseWnd()
{
	m_pRstArray = new CObArray;
	m_strPath = "";
	m_pMSGWnd = NULL;
}

CDataBrowseWnd::~CDataBrowseWnd()
{
	::ReleaseObArray(m_pRstArray);
	delete m_pRstArray;
}


BEGIN_MESSAGE_MAP(CDataBrowseWnd, CWnd)
	//{{AFX_MSG_MAP(CDataBrowseWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_GRD_ROW_SELECTED, OnGrdRowSelected)
	ON_MESSAGE(WM_GRD_COMBO_SEL_CHANGE, OnGrdComboSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataBrowseWnd message handlers

BOOL CDataBrowseWnd::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC_MCG dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	return TRUE;
}

int CDataBrowseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pMSGWnd == NULL)
		m_pMSGWnd = GetParent();

	CRect rect;
	rect.SetRectEmpty();
	m_pGridBrowse = new CColGrid;
	m_pGridBrowse->SetGridFont(m_pFont);
	m_pGridBrowse->Create(rect, this, 0);
	m_pGridBrowse->SetTotalColNo(8);
	for (int j = 1; j < 8; j++)
		m_pGridBrowse->SetColWidth(j, 0);
	m_pGridBrowse->u_pMSGWnd = this;
	m_pGridBrowse->SetGridSelType(cGridRowSel, FALSE, FALSE);
	m_pGridBrowse->u_bHScrollable = FALSE;
	CString strLabel;
	strLabel.LoadString(IDS_PROFILE_NAME);
	m_pGridBrowse->SetFixedRowLabel(strLabel);

	m_pGridDetail = new CColGrid;
	m_pGridDetail->SetGridFont(m_pFont);
	m_pGridDetail->Create(rect, this, 0);
	m_pGridDetail->u_bVScrollable = FALSE;
	m_pGridDetail->u_bDragCell = FALSE; 
	m_pGridDetail->u_bDragLine = FALSE; 
	m_pGridDetail->SetTotalColNo(2);
	m_pGridDetail->SetGridSelType(cGridNonSel, FALSE, FALSE);
	m_pGridDetail->u_bHScrollable = FALSE;
	m_pGridDetail->u_pMSGWnd = this;
	m_pGridDetail->SetSeqAt(cSeqRGB, 0, "255\\255\\192");

	strLabel.LoadString(IDS_LABEL_ATTRIB_VALUE);
	m_pGridDetail->SetFixedRowLabel(strLabel);

	m_pGridDetail->SetColAttribAt(0, cEditable, 0);
	m_pGridDetail->SetColAttribAt(0, cBGColor, 0);
	m_pGridDetail->SetColAttribAt(1, cType, cGridCombo);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 0, "0 K\\16 K\\20 K\\22 K\\24 K\\28 K\\32 K\\36 K\\40 K\\44 K\\48 K\\52 K\\56 K\\64 K\\72 K\\80 K\\88 K\\96 K\\104 K\\112 K\\120 K\\128 K\\136 K\\144 K\\152 K\\160 K\\168 K\\176 K\\184 K\\192 K\\224 K\\256 K");
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 1, "1\\2\\3\\4\\5\\6\\7\\8\\9\\10\\15\\20\\25\\30");
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 2, "1 sec\\2 sec\\3 sec\\4 sec\\5 sec\\6 sec\\7 sec\\8 sec\\9 sec\\10 sec\\15 sec\\20 sec\\25 sec\\30 sec");
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 3, "0\\25\\50\\75\\100");
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 4, "5 K\\6 K\\8 K\\10 K\\12 K\\16 K\\20 K\\22 K\\32 K\\40 K\\48 K\\64 K\\80 K\\96 K\\128 K\\160 K\\192 K");
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 5, "");
	
	CString strMono, strStereo;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 6, "");

	m_pGridDetail->AppendRow(7);

	strLabel.LoadString(IDS_VIDEO_BANDWIDTH);
	m_pGridDetail->SetDataAt(0, 0, strLabel);
	m_pGridDetail->SetAttribAt(0, 1, cIndex, 0);

	strLabel.LoadString(IDS_VIDEO_FPS);
	m_pGridDetail->SetDataAt(1, 0, strLabel);
	m_pGridDetail->SetAttribAt(1, 1, cIndex, 1);

	strLabel.LoadString(IDS_VIDEO_KEY_PER_SEC);
	m_pGridDetail->SetDataAt(2, 0, strLabel);
	m_pGridDetail->SetAttribAt(2, 1, cIndex, 2);

	strLabel.LoadString(IDS_VIDEO_QUALITY);
	m_pGridDetail->SetDataAt(3, 0, strLabel);
	m_pGridDetail->SetAttribAt(3, 1, cIndex, 3);

	strLabel.LoadString(IDS_AUDIO_BANDWIDTH);
	m_pGridDetail->SetDataAt(4, 0, strLabel);
	m_pGridDetail->SetAttribAt(4, 1, cIndex, 4);

	strLabel.LoadString(IDS_AUDIO_SAMPLE_RATE);
	m_pGridDetail->SetDataAt(5, 0, strLabel);
	m_pGridDetail->SetAttribAt(5, 1, cIndex, 5);

	m_pGridDetail->SetDataAt(6, 0, strMono + "/" + strStereo);
	m_pGridDetail->SetAttribAt(6, 1, cIndex, 6);

	return 0;
}

BOOL CDataBrowseWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CDataBrowseWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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


void CDataBrowseWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_pGridBrowse->DestroyWindow();
	delete m_pGridBrowse;

	m_pGridDetail->DestroyWindow();
	delete m_pGridDetail;
}

void CDataBrowseWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rcClient;
	GetClientRect(rcClient);
	
	CRect rcGrid = rcClient;

	int nGridWidth;
#ifdef _EN
	nGridWidth = 190;
#else
	nGridWidth = 160;
#endif

#ifdef _JAPANESE
	nGridWidth = 190;
#endif

	rcGrid.right -= nGridWidth;
	int nRowHeight = m_pGridBrowse->GetRowHeight();
	rcGrid.bottom = rcGrid.top + nRowHeight * 8 + 4;
	m_pGridBrowse->MoveWindow(rcGrid);
	m_pGridBrowse->SetColWidth(0, rcGrid.Width());
	
	rcGrid.left = rcGrid.right + 3;
	rcGrid.right = rcClient.right;
	m_pGridDetail->MoveWindow(rcGrid);
	int nWidth = rcGrid.right - rcGrid.left;

	int nColWidth;

#ifdef _EN
	nColWidth = 105;
#else
	nColWidth = 75;
#endif
	
#ifdef _JAPANESE
	nColWidth = 105;
#endif
	
	m_pGridDetail->SetColWidth(0, nColWidth);
	m_pGridDetail->SetColWidth(1, nWidth - nColWidth - 3);
}

void CDataBrowseWnd::EnableWindow(BOOL bEnable)
{
	if (!bEnable)
	{
		m_pGridBrowse->u_rgbFG = RGB(128, 128, 128);
		m_pGridDetail->u_rgbFG = RGB(128, 128, 128);
		m_pGridBrowse->SetColor(cSelRowColor, RGB(128, 128, 128));
		m_pGridDetail->SetColor(cSelRowColor, RGB(128, 128, 128));
		m_pGridBrowse->EnableWindow(FALSE);
		m_pGridDetail->EnableWindow(FALSE);
	}
	else
	{
		m_pGridBrowse->u_rgbFG = ::GetSysColor(COLOR_WINDOWTEXT);
		m_pGridDetail->u_rgbFG = ::GetSysColor(COLOR_WINDOWTEXT);
		m_pGridBrowse->SetColor(cSelRowColor, ::GetSysColor(COLOR_WINDOWTEXT));
		m_pGridDetail->SetColor(cSelRowColor, ::GetSysColor(COLOR_WINDOWTEXT));
		m_pGridBrowse->EnableWindow(TRUE);
		m_pGridDetail->EnableWindow(TRUE);
	}

	m_pGridBrowse->RePaint();
	m_pGridDetail->RePaint();
}

void CDataBrowseWnd::SetRestrictedData(CString strPath)
{
	if (!::FileExists(strPath))
		return;

	::ReleaseObArray(m_pRstArray);

	CStringArray strArrayData;
	::LoadFile(strPath, strArrayData, TRUE);

	CString strSeq = "";
	CString strData, strTok;
	int nCount;
	for (int i = 0; i < strArrayData.GetSize(); i++)
	{
		nCount = 0;
		strData = strArrayData[i];
		::GetTokStr(strData, strTok, nCount, ":");

		CRestrictedData* p = new CRestrictedData;
		p->m_nKbps = atoi(strTok);

		strSeq += strTok + " K\\";

		strData = strData.Mid(strTok.GetLength() + 1);
		nCount = 0;
		while (::GetTokStr(strData, strTok, nCount, ";"))
		{
			int nIndex = strTok.Find(",");
			if (nIndex >= 0)
			{
				p->m_wdArrayFreq.Add((WORD)atoi(strTok.Left(nIndex)));
				p->m_btArrayMono.Add(strTok.Mid(nIndex + 1)[0]);
			}
		}
		m_pRstArray->Add(p);
	}

	if (strSeq.Right(1) == "\\")
		strSeq = strSeq.Left(strSeq.GetLength() - 1);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 4, strSeq);
}

BOOL CDataBrowseWnd::SetProfileData(CString strPath, CString strSel, 
									CWordArray* pColArray, CStringArray* pStrArray)
{
	if (!::FileExists(strPath))
		return FALSE;

	if (m_strPath != "")
	{
		if (!SaveData(m_strPath))
			return FALSE;
	}

	m_strPath = strPath;
	
	m_pGridBrowse->DeleteAllRows();
	
	CStringArray strArrayData;

//#ifndef _BUNDLE
	::LoadFile(strPath, strArrayData, TRUE);
//#else
//	strArrayData.Add("<¶È§tÁn­µ,FM­µ½E28K)>	0,20,1,0,22,22050,S");
//	strArrayData.Add("<110K>\t88,15,1,0,22,22050,S");
//#endif

	m_pGridBrowse->AppendRow(strArrayData.GetSize());
	
	m_pGridBrowse->u_SelectedRow = -1;
	CString strData, strTok;
	int nCount;
	int nVScrollStart = 0;
	int jjj=strArrayData.GetSize();
	for (int i = 0; i < strArrayData.GetSize(); i++)
	{
		nCount = 0;
		strData = strArrayData[i];
		::GetTokStr(strData, strTok, nCount, "\t");
		m_pGridBrowse->SetDataAt(i, 0, strTok);
		CString strName = strTok;

		strData = strData.Mid(strTok.GetLength() + 1);
		nCount = 0;
		int j = 1;
		for (j = 1; j < 8; j++)
		{
			::GetTokStr(strData, strTok, nCount, ",");
			m_pGridBrowse->SetDataAt(i, j, strTok);
		}

		if (pColArray && pStrArray)
		{
			for (j = 0; j < pColArray->GetSize(); j++)
			{
				if (strName.GetLength() > 2)
				{
					if (strName[0] == '<' && strName.Right(1) == ">")
						m_pGridBrowse->SetDataAt(i, pColArray->GetAt(j), pStrArray->GetAt(j));
				}
			}
		}

		if (strName == strSel)
		{
			m_pGridBrowse->u_SelectedRow = i;
			nVScrollStart = i;
			SelectRow(m_pGridBrowse->u_SelectedRow);
		}
	}

	m_pGridBrowse->SetVScrollPos(nVScrollStart - 3, TRUE);

	if (m_pGridBrowse->u_SelectedRow == -1)
		ClearDetail();

	m_pGridBrowse->RePaint();
	m_pGridDetail->RePaint();

	return TRUE;
}

void CDataBrowseWnd::ClearDetail()
{
	for (int i = 0; i < 7; i++)
		m_pGridDetail->SetDataAt(i, 1, "");
}

LRESULT CDataBrowseWnd::OnGrdRowSelected(WPARAM wParam, LPARAM lParam)
{
	int nRow = HIWORD(lParam);
	SelectRow(nRow);

	return 0L;
}

BOOL CDataBrowseWnd::SelectRow(CString strSel)
{
	for (int i = 0; i < m_pGridBrowse->GetDataSize(); i++)
	{
		CString strData = m_pGridBrowse->GetDataAt(i, 0);
		if (strSel == strData)
		{
			m_pGridBrowse->u_SelectedRow = i;
			m_pGridBrowse->SetVScrollPos(i - 3, TRUE);
			SelectRow(i);
			return TRUE;
		}
	}

	m_pGridBrowse->u_SelectedRow = -1;
	m_pGridBrowse->SetVScrollPos(0, TRUE);
	m_pGridBrowse->RePaint();
	ClearDetail();
	m_pGridDetail->RePaint();

	return FALSE;
}

void CDataBrowseWnd::SelectRow(int nRow)
{
	CString strName = m_pGridBrowse->GetDataAt(nRow, 0);
	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, nRow, (LPARAM)(LPCTSTR)strName);

	CString strMono, strStereo;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);
	for (int i = 1; i < 8; i++)
	{
		CString str = m_pGridBrowse->GetDataAt(nRow, i);

		if (i == 1 && str != "")
			str += " K";
		else if (i == 3 && str != "")
			str += " sec";
		else if (i == 5 && str != "")
			str += " K";
		else if (i == 6 && str != "")
			str += " Hz";
		else if (i == 7)
		{
			if (str == "M")
				str = strMono;
			else if (str == "S")
				str = strStereo;
		}

		m_pGridDetail->SetDataAt(i - 1, 1, str);
	}

	SetComboFreq();
	SetComboMono();

	CheckVideoBand();
	m_pGridBrowse->RePaint();
	m_pGridDetail->RePaint();
}

void CDataBrowseWnd::CheckVideoBand()
{
	CString strData = m_pGridDetail->GetDataCtrlAt(0, 1);
	if (strData == "0 K")
	{
		m_pGridDetail->SetAttribAt(1, 1, cEditable, 0);
		m_pGridDetail->SetAttribAt(2, 1, cEditable, 0);
		m_pGridDetail->SetAttribAt(3, 1, cEditable, 0);

		m_pGridDetail->SetDataAt(1, 1, "");
		m_pGridDetail->SetDataAt(2, 1, "");
		m_pGridDetail->SetDataAt(3, 1, "");
	}
	else
	{
		m_pGridDetail->SetAttribAt(1, 1, cEditable, 1);
		m_pGridDetail->SetAttribAt(2, 1, cEditable, 1);
		m_pGridDetail->SetAttribAt(3, 1, cEditable, 1);

		if (m_pGridDetail->GetDataAt(1, 1) == "")
			m_pGridDetail->SetDataAt(1, 1, m_strDefFPS);
		
		if (m_pGridDetail->GetDataAt(2, 1) == "")
			m_pGridDetail->SetDataAt(2, 1, m_strDefKerFrame);

		if (m_pGridDetail->GetDataAt(3, 1) == "")
			m_pGridDetail->SetDataAt(3, 1, m_strDefVideoQuality);
	}

//#ifdef _BUNDLE
//	m_pGridDetail->SetColAttribAt(1, cEditable, 0, TRUE);
//#endif
}

LRESULT CDataBrowseWnd::OnGrdComboSelChange(WPARAM wParam, LPARAM lParam)
{
	int nRow = m_pGridDetail->u_PreRow;
	int nCol = m_pGridDetail->u_PreCol;

	int nCount = 0;
	CString strTok, str;

	if (nRow == 0 && nCol == 1)
		CheckVideoBand();
	else if (nRow == 4 && nCol == 1)
	{
		SetComboFreq();

		str = m_pGridDetail->GetSeqAt(cSeqTextCombo, 5);
		::GetTokStr(str, strTok, nCount, "\\");
		m_pGridDetail->SetDataAt(5, 1, strTok);
		
		SetComboMono();

		nCount = 0;
		str = m_pGridDetail->GetSeqAt(cSeqTextCombo, 6);
		::GetTokStr(str, strTok, nCount, "\\");		
		m_pGridDetail->SetDataAt(6, 1, strTok);
	}
	else if (nRow == 5 && nCol == 1)
	{
		str = m_pGridDetail->GetSeqAt(cSeqTextCombo, 6);
		::GetTokStr(str, strTok, nCount, "\\");		
		m_pGridDetail->SetDataAt(6, 1, strTok);

		SetComboMono();
	}

	int nSel = m_pGridBrowse->u_SelectedRow;
	CString strName = m_pGridBrowse->GetDataAt(nSel, 0);
	CString strUntitled;
	strUntitled.LoadString(IDS_UNTITLE);
	strUntitled += m_strPrefix;
	int nLen = strUntitled.GetLength();
	if (strName.Left(nLen) != strUntitled)
	{
		int nIndex = GetUntitledIndex();
		//CString strNewName;
		//strNewName.Format("%s-%d", strUntitled, nIndex + 1);
		strName.Format("%s-%d", strUntitled, nIndex + 1);m_pGridBrowse->AppendRow(1);
		int nSize = m_pGridBrowse->GetDataSize();
		m_pGridBrowse->SetDataAt(nSize - 1, 0, strName);
		m_pGridBrowse->u_SelectedRow = nSize - 1;
		m_pGridBrowse->u_EndSelectedRow = nSize - 1;
		m_pGridBrowse->SetVScrollPos(nSize - 5, TRUE);
		m_pGridBrowse->RePaint();
	}

//		CString str;
		str = m_pGridDetail->GetDataCtrlAt(0, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(0, 1, m_strDefVideoBand);
		if (str != "0 K")
		{
			str = m_pGridDetail->GetDataCtrlAt(1, 1);
			if (str == "")
				m_pGridDetail->SetDataAt(1, 1, m_strDefFPS);

			str = m_pGridDetail->GetDataCtrlAt(2, 1);
			if (str == "")
				m_pGridDetail->SetDataAt(2, 1, m_strDefKerFrame);

			str = m_pGridDetail->GetDataCtrlAt(3, 1);
			if (str == "")
				m_pGridDetail->SetDataAt(3, 1, m_strDefVideoQuality);
		}

		str = m_pGridDetail->GetDataCtrlAt(4, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(4, 1, m_strDefAudioBand);

		str = m_pGridDetail->GetDataCtrlAt(5, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(5, 1, m_strDefAudioFreq);

		str = m_pGridDetail->GetDataCtrlAt(6, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(6, 1, m_strDefMonoStereo);

		SetComboFreq();
		SetComboMono();
	//}

	CString strMono, strStereo;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);

	nSel = m_pGridBrowse->u_SelectedRow;
	for (int i = 1; i < 8; i++)
	{
		CString str = m_pGridDetail->GetDataCtrlAt(i - 1, 1);
		int nSize = str.GetLength();
		
		if (i == 1)
			str = str.Left(nSize - 2);
		else if (i == 3)
			str = str.Left(nSize - 4);
		else if (i == 5)
			str = str.Left(nSize - 2);
		else if (i == 6)
			str = str.Left(nSize - 3);
		else if (i == 7)
		{
			if (str == strMono)
				str = "M";
			else if (str == strStereo)
				str = "S";
		}

		m_pGridBrowse->SetDataAt(nSel, i, str);
	}

	m_pGridDetail->RePaint();

	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, m_pGridBrowse->GetDataSize() - 1, (LPARAM)(LPCTSTR)strName);
	m_pMSGWnd->SendMessage(WM_ROW_APPENDED);

	return 0L;
}

void CDataBrowseWnd::SetComboFreq()
{
	CString strData = m_pGridDetail->GetDataCtrlAt(4, 1);
	int nK = atoi(strData.Left(strData.GetLength() - 2));
	CString strFreq;
	GetAvailFreqStr(nK, strFreq);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 5, strFreq);
}

void CDataBrowseWnd::SetComboMono()
{
	CString strK = m_pGridDetail->GetDataCtrlAt(4, 1);
	int nK = atoi(strK.Left(strK.GetLength() - 2));
	CString strFreq = m_pGridDetail->GetDataCtrlAt(5, 1);
	int nFreq = atoi(strFreq.Left(strFreq.GetLength() - 3));

	CString strMonoStereo;
	GetAvailMonoStr(nK, nFreq, strMonoStereo);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 6, strMonoStereo);
}

int CDataBrowseWnd::GetUntitledIndex()
{
	int nCount = m_pGridBrowse->GetDataSize();
	CString str;
	CString strUntitled;
	strUntitled.LoadString(IDS_UNTITLE);
	strUntitled += m_strPrefix;
	int nLen = strUntitled.GetLength();
	int nIndex = 0;
	for (int i = nCount - 1; i >= 0; i--)
	{
		str = m_pGridBrowse->GetDataAt(i, 0);
		if (str.Left(nLen) == strUntitled)
		{
			int n = atoi(str.Mid(nLen + 1));
			if (n > nIndex)
				nIndex = n;
		}
		else
			break;
	}

	return nIndex;
}

void CDataBrowseWnd::GetAvailFreqStr(int nK, CString& strFreq)
{
	for (int i = 0; i < m_pRstArray->GetSize(); i++)
	{
		CRestrictedData* p = (CRestrictedData*)m_pRstArray->GetAt(i);
		if (p->m_nKbps == nK)
		{
			CWordArray wdNoDup;
			int j = 0;
			for (j = 0; j < p->m_wdArrayFreq.GetSize(); j++)
			{
				WORD wdFreq = p->m_wdArrayFreq[j];
				BOOL bFound = FALSE;
				for (int k = 0; k < wdNoDup.GetSize(); k++)
				{
					if (wdNoDup[k] == wdFreq)
					{
						bFound = TRUE;
						break;
					}
				}
				if (!bFound)
					wdNoDup.Add(wdFreq);
			}

			for (j = 0; j < wdNoDup.GetSize(); j++)
			{
				WORD wdFreq = wdNoDup[j];
				char szBuf[32];
				strFreq += itoa(wdFreq, szBuf, 10) + CString(" Hz\\");
			}

			if (strFreq.Right(1) == "\\")
				strFreq = strFreq.Left(strFreq.GetLength() - 1);

			return;
		}
	}
}

void CDataBrowseWnd::GetAvailMonoStr(int nK, int nFreq, CString& strMonoStereo)
{
	CString strMono, strStereo;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);
	for (int i = 0; i < m_pRstArray->GetSize(); i++)
	{
		CRestrictedData* p = (CRestrictedData*)m_pRstArray->GetAt(i);
		if (p->m_nKbps == nK)
		{
			CWordArray wdNoDup;
			for (int j = 0; j < p->m_wdArrayFreq.GetSize(); j++)
			{
				WORD wdFreq = p->m_wdArrayFreq[j];
				if (wdFreq == nFreq)
				{
					CString strTmp = (p->m_btArrayMono[j] == 'M') ? strMono : strStereo;
					if (strMonoStereo == "")
						strMonoStereo = strTmp;
					else if (strMonoStereo.Find("\\") < 0 && strMonoStereo != strTmp)
						strMonoStereo += "\\" + strTmp;
				}
			}

			if (strMonoStereo.Find("\\") < 0 && strMonoStereo != "")
				m_pGridDetail->SetDataAt(6, 1, strMonoStereo);

			return;
		}
	}
}

void CDataBrowseWnd::SetDefaultValue(CString strVB, CString strFPS, CString strKeyFrame, CString strVideoQuality,
									 CString strAB, CString strAudioFreq, CString strMono)
{
	m_strDefVideoBand = strVB;
	m_strDefFPS = strFPS;
	m_strDefKerFrame = strKeyFrame;
	m_strDefVideoQuality = strVideoQuality;
	m_strDefAudioBand = strAB;
	m_strDefAudioFreq = strAudioFreq;
	m_strDefMonoStereo = strMono;
}

BOOL CDataBrowseWnd::SaveData(CString strPath)
{
	if (strPath == "\\\\\\")
		strPath = m_strPath;

	CStringArray strArrayData;
	int nSize = m_pGridBrowse->GetDataSize();
	for (int i = 0; i < nSize; i++)
	{
		CString str = m_pGridBrowse->GetDataAt(i, 0) + "\t";
		for (int j = 1; j < 8; j++)
			str += m_pGridBrowse->GetDataAt(i, j) + ",";
		str = str.Left(str.GetLength() - 1);
		
		strArrayData.Add(str);
	}
	::SaveFile(strPath, strArrayData, TRUE);

	return TRUE;
}

int CDataBrowseWnd::GetSelectedRow()
{
	return m_pGridBrowse->u_SelectedRow;
}

void CDataBrowseWnd::DeleteSelectedRow()
{
	if (m_pGridBrowse->u_SelectedRow < 0)
		return;

	m_pGridBrowse->DeleteRows(m_pGridBrowse->u_SelectedRow, m_pGridBrowse->u_SelectedRow);
	m_pGridBrowse->u_SelectedRow = -1;
	m_pGridBrowse->RePaint();

	ClearDetail();
	m_pGridDetail->RePaint();

	SaveData();

	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, -1);
}

void CDataBrowseWnd::GetProfileName(CStringArray& strArrayProfile)
{
	strArrayProfile.RemoveAll();
	int nSize = m_pGridBrowse->GetDataSize();
	for (int i = 0; i < nSize; i++)
		strArrayProfile.Add(m_pGridBrowse->GetDataAt(i, 0));
}

void CDataBrowseWnd::ReplaceSelected(CString strProfileName)
{
	int nSelRow = m_pGridBrowse->u_SelectedRow;
	m_pGridBrowse->SetDataAt(nSelRow, 0, strProfileName);
	m_pGridBrowse->RePaintRow(nSelRow, nSelRow);

	SaveData();

	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, nSelRow, (LPARAM)(LPCTSTR)strProfileName);
}

void CDataBrowseWnd::GetSelectedData(CStringArray& strArraySelected)
{
	strArraySelected.RemoveAll();
	int nSelRow = m_pGridBrowse->u_SelectedRow;
	for (int i = 0; i < 8; i++)
		strArraySelected.Add(m_pGridBrowse->GetDataAt(nSelRow, i));
}

