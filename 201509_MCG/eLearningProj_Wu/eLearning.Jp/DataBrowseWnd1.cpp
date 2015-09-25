// DataBrowseWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "resource.h"
#include "DataBrowseWnd1.h"
#include "MemDC.h"
#include "macro.h"
#include "ColGrid.h"
#include "cmkapi.h"
#include "grdapi.h"
#include "DataBrowseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataBrowseWnd

CDataBrowseWnd1::CDataBrowseWnd1()
{
	m_pRstArray = new CObArray;
	m_strPath = "";
	m_pMSGWnd = NULL;

	m_Cameraflag = FALSE;
}

CDataBrowseWnd1::~CDataBrowseWnd1()
{
	::ReleaseObArray(m_pRstArray);
	delete m_pRstArray;
}


BEGIN_MESSAGE_MAP(CDataBrowseWnd1, CWnd)
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

BOOL CDataBrowseWnd1::OnEraseBkgnd(CDC* pDC) 
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

int CDataBrowseWnd1::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	m_pGridBrowse->SetTotalColNo(5);
	for (int j = 1; j < 5; j++)
		m_pGridBrowse->SetColWidth(j, 0);
	m_pGridBrowse->u_pMSGWnd = this;
	m_pGridBrowse->SetGridSelType(cGridRowSel, FALSE, FALSE);
	m_pGridBrowse->u_bHScrollable = FALSE;
	CString strLabel;


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

	CString StrPath;
	StrPath = MACRO_DEVICEINFO_INI;//MACRO_COURSE_PATH;
	CString m_str;
	//m_str = StrPath.Left(strlen(StrPath) - 9);
	//StrPath = m_str + "\\temp\\DeviceSetInfo.ini";


	m_pGridDetail->SetColAttribAt(0, cEditable, 0);
	m_pGridDetail->SetColAttribAt(0, cBGColor, 0);
	m_pGridDetail->SetColAttribAt(1, cType, cGridCombo);
	m_str = GetCString(StrPath, 0, m_InfoTypeflag);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 0, m_str);
	m_str = GetCString(StrPath, 1, m_InfoTypeflag);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 1, m_str);
#ifdef _OLD_
	m_str = GetCString(StrPath, 2, m_InfoTypeflag);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 2, m_str);
	m_str = GetCString(StrPath, 3, m_InfoTypeflag);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 3, m_str);
#else
	m_str = GetCString(StrPath, 3, m_InfoTypeflag);
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 2, m_str);
	m_str = "";
	for (int i = -1000; i <= 1000; i += 100)
	{
		m_str.AppendFormat("%d ms\\", i);
	}
	m_pGridDetail->SetSeqAt(cSeqTextCombo, 3, m_str);
#endif

	m_pGridDetail->AppendRow(4);

	strLabel.LoadString(IDS_VIDEO_BIT_RATE);
	m_pGridDetail->SetDataAt(0, 0, strLabel);
	m_pGridDetail->SetAttribAt(0, 1, cIndex, 0);

	strLabel.LoadString(IDS_FRAME_RATE);
	m_pGridDetail->SetDataAt(1, 0, strLabel);
	m_pGridDetail->SetAttribAt(1, 1, cIndex, 1);
#ifdef _OLD_
	strLabel.LoadString(IDS_KEY_FRAME);
	m_pGridDetail->SetDataAt(2, 0, strLabel);
	m_pGridDetail->SetAttribAt(2, 1, cIndex, 2);

	strLabel.LoadString(IDS_AUDIO_BIT_RATE);
	m_pGridDetail->SetDataAt(3, 0, strLabel);
	m_pGridDetail->SetAttribAt(3, 1, cIndex, 3);
#else
	strLabel.LoadString(IDS_AUDIO_BIT_RATE);
	m_pGridDetail->SetDataAt(2, 0, strLabel);
	m_pGridDetail->SetAttribAt(2, 1, cIndex, 2);

	strLabel = "‰¹ƒYƒŒ•â³";
	m_pGridDetail->SetDataAt(3, 0, strLabel);
	m_pGridDetail->SetAttribAt(3, 1, cIndex, 3);
#endif

	return 0;
}

CString CDataBrowseWnd1::GetCString(CString strpath, int index, int flag)
{
	CString returncstring  = "";
	CString str;
	CString str1;
	CString str2;
	CString str3;
	
	if (flag == TRUE)
	{
		str3 = "RecordingSetting";
	} 
	else
	{
		str3 = "ScreenCapture";
	}

	switch (index)
	{
	case 0:
		str = "VideoBitRateMin";
		str1 = "VideoBitRateMax";
		str2 = "VideoBitRateStep";
		returncstring = ReturnCString(str, str1, str2, str3, strpath);
		break;
	case 1:
		if (flag == FALSE)
		{
			str = "FrameRateMin";
			str1 = "FrameRateMax";
			str2 = "FrameRateStep";
			returncstring = ReturnCString(str, str1, str2, str3, strpath);
		}
		else
		{
			if (m_Cameraflag == FALSE)
			{
				returncstring = "";
			} 
		}
		
		break;
	case 2:
		str = "KeyFrameMin";
		str1 = "KeyFrameMax";
		str2 = "KeyFrameStep";
		returncstring = ReturnCString(str, str1, str2, str3, strpath);
		break;
	case 3:
		str = "AudioBitRateMin";
		str1 = "AudioBitRateMax";
		str2 = "AudioBitRateStep";
		returncstring = ReturnCString(str, str1, str2, str3, strpath);
		break;
	}
	return returncstring;
}
CString CDataBrowseWnd1::ReturnCString(CString str, CString str1,CString str2, CString str3, CString path)
{
	char szBuf[256];
	char szBuf1[256];
	char szBuf2[256];
	int count;
	int count1;
	int count2;
	CString number;
	CString number1;
	CString number2;
	CString ShowStr = "";
	::GetPrivateProfileString(str3, str, "", szBuf, 256, path);
	::GetPrivateProfileString(str3, str1, "", szBuf1, 256, path);
	::GetPrivateProfileString(str3, str2, "", szBuf2, 256, path);
	number = szBuf;
	number1 = szBuf1;
	number2 = szBuf2;
	if (number.Find("k") > 0)
	{
		number.Remove('k');
		count = atoi(number);
		count1 = atoi(number1);
		count2 = atoi(number2);
		for(int i = count; i <= count1; i += count2)
		{
			number.Format("%d",i);
			ShowStr += number +" k\\";
		}
	} 
	else if (number.Find("sec") > 0)
	{
		number.Remove('sec');
		count = atoi(number);
		count1 = atoi(number1);
		count2 = atoi(number2);
		for(int i = count; i <= count1; i += count2)
		{
			number.Format("%d",i);
			ShowStr += number +" sec\\";
		}
	} 
	else
	{
		count = atoi(number);
		count1 = atoi(number1);
		count2 = atoi(number2);
		for(int i = count; i <= count1; i += count2)
		{
			number.Format("%d",i);
			ShowStr += number +"\\";
		}
	}
	return ShowStr;
}

BOOL CDataBrowseWnd1::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CDataBrowseWnd1::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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


void CDataBrowseWnd1::OnDestroy() 
{
	CWnd::OnDestroy();

	m_pGridBrowse->DestroyWindow();
	delete m_pGridBrowse;

	m_pGridDetail->DestroyWindow();
	delete m_pGridDetail;
}

void CDataBrowseWnd1::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcGrid = rcClient;

	int nGridWidth;

	int nRowHeight = m_pGridBrowse->GetRowHeight();
	rcGrid.bottom = rcGrid.top + nRowHeight * 4 + 4;
	m_pGridDetail->MoveWindow(rcGrid);
	m_pGridDetail->SetColWidth(0, rcGrid.Width()/2);
	m_pGridDetail->SetColWidth(1, rcGrid.Width()/2);

	rcGrid.top = rcGrid.bottom + 10;
	rcGrid.bottom = rcGrid.top + nRowHeight * 4 + 4;
	m_pGridBrowse->MoveWindow(rcGrid);
	m_pGridBrowse->SetColWidth(0, rcGrid.Width());
}

void CDataBrowseWnd1::EnableWindow(BOOL bEnable)
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

void CDataBrowseWnd1::SetRestrictedData(CString strPath)
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

BOOL CDataBrowseWnd1::SetProfileData(CString strPath, CString strSel, 
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
#ifdef _OLD_
		for (j = 1; j < 5; j++)
		{
			::GetTokStr(strData, strTok, nCount, ",");
			m_pGridBrowse->SetDataAt(i, j, strTok);
		}
#else
		for (j = 1; j < 3; j++)
		{
			::GetTokStr(strData, strTok, nCount, ",");
			m_pGridBrowse->SetDataAt(i, j, strTok);
		}
		::GetTokStr(strData, strTok, nCount, ",");
		::GetTokStr(strData, strTok, nCount, ",");
		m_pGridBrowse->SetDataAt(i, 3, strTok);
		::GetTokStr(strData, strTok, nCount, ",");
		m_pGridBrowse->SetDataAt(i, 4, strTok);
#endif
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

void CDataBrowseWnd1::ClearDetail()
{
	for (int i = 0; i < 4; i++)
		m_pGridDetail->SetDataAt(i, 1, "");
}

LRESULT CDataBrowseWnd1::OnGrdRowSelected(WPARAM wParam, LPARAM lParam)
{
	int nRow = HIWORD(lParam);
	SelectRow(nRow);

	return 0L;
}

BOOL CDataBrowseWnd1::SelectRow(CString strSel)
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

void CDataBrowseWnd1::SelectRow(int nRow)
{
	CString strName = m_pGridBrowse->GetDataAt(nRow, 0);
	strName.AppendFormat("%020d", this->GetDlgCtrlID());
	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, nRow, (LPARAM)(LPCTSTR)strName);

	for (int i = 1; i < 5; i++)
	{
		CString str = m_pGridBrowse->GetDataAt(nRow, i);
#ifdef _OLD_
		if (i == 1 && str != "")
			str += " K";
		else if (i == 3 && str != "")
			str += " sec";
		else if (i == 4 && str != "")
			str += " K";
#else
		if (i == 1 && str != "")
			str += " K";
		else if (i == 3 && str != "")
			str += " K";
		else if (i == 4 && str != "")
			str += " ms";
#endif
		m_pGridDetail->SetDataAt(i - 1, 1, str);
	}

	SetComboFreq();
	SetComboMono();

	CheckVideoBand();
	m_pGridBrowse->RePaint();
	m_pGridDetail->RePaint();
}

void CDataBrowseWnd1::CheckVideoBand()
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

LRESULT CDataBrowseWnd1::OnGrdComboSelChange(WPARAM wParam, LPARAM lParam)
{
	int nRow = m_pGridDetail->u_PreRow;
	int nCol = m_pGridDetail->u_PreCol;

	int nCount = 0;
	CString strTok, str;

	if (nRow == 0 && nCol == 1)
		CheckVideoBand();
	//add bu buxiangyi
	/*else if (nRow == 4 && nCol == 1)
	{
		SetComboFreq();

		str = m_pGridDetail->GetSeqAt(cSeqTextCombo, 4);
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
*/
	////////////////////////////////////////////////
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
#ifdef _OLD_
		if (str == "")
			m_pGridDetail->SetDataAt(2, 1, m_strDefKerFrame);

		str = m_pGridDetail->GetDataCtrlAt(3, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(3, 1, m_strDefVideoQuality);
#else
		if (str == "")
			m_pGridDetail->SetDataAt(2, 1, m_strDefVideoQuality);

		str = m_pGridDetail->GetDataCtrlAt(3, 1);
		if (str == "")
			m_pGridDetail->SetDataAt(3, 1, "0 ms");
#endif
	}
	//add by buxiangyi
	/*str = m_pGridDetail->GetDataCtrlAt(4, 1);
	if (str == "")
	m_pGridDetail->SetDataAt(4, 1, m_strDefAudioBand);*/
	//////////////////////////////////////////////////

	str = m_pGridDetail->GetDataCtrlAt(4, 1);
	if (str == "")
		m_pGridDetail->SetDataAt(4, 1, m_strDefAudioFreq);

	//add by buxiangyi
	/*str = m_pGridDetail->GetDataCtrlAt(6, 1);
	if (str == "")
	m_pGridDetail->SetDataAt(6, 1, m_strDefMonoStereo);*/
	
	//SetComboFreq();
	//////////////////////////////////////////////////////
	SetComboMono();
	//}

	CString strMono, strStereo;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);

	nSel = m_pGridBrowse->u_SelectedRow;
	for (int i = 1; i < 5; i++)
	{
		CString str = m_pGridDetail->GetDataCtrlAt(i - 1, 1);
		int nSize = str.GetLength();

		if (i == 1)
			str = str.Left(nSize - 2);
#ifdef _OLD_
		else if (i == 3)
			str = str.Left(nSize - 4);
		else if (i == 4)
			str = str.Left(nSize - 2);
#else
		else if (i == 3)
			str = str.Left(nSize - 2);
		else if (i == 4)
			str = str.Left(nSize - 3);
#endif
		//add by xiangyi
		/*else if (i == 4)
			str = str.Left(nSize - 2);*/
		//////////////////////////////////////

		m_pGridBrowse->SetDataAt(nSel, i, str);
	}

	m_pGridDetail->RePaint();

	strName.AppendFormat("%020d", this->GetDlgCtrlID());
	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, m_pGridBrowse->GetDataSize() - 1, (LPARAM)(LPCTSTR)strName);
	m_pMSGWnd->SendMessage(WM_ROW_APPENDED);

	return 0L;
}

void CDataBrowseWnd1::SetComboFreq()
{

}

void CDataBrowseWnd1::SetComboMono()
{

}

int CDataBrowseWnd1::GetUntitledIndex()
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

void CDataBrowseWnd1::GetAvailFreqStr(int nK, CString& strFreq)
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

void CDataBrowseWnd1::GetAvailMonoStr(int nK, int nFreq, CString& strMonoStereo)
{

}

void CDataBrowseWnd1::SetDefaultValue(CString strVB, CString strFPS, CString strKeyFrame, CString strVideoQuality,
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

BOOL CDataBrowseWnd1::SaveData(CString strPath)
{
	if (strPath == "\\\\\\")
		strPath = m_strPath;

	CStringArray strArrayData;
	int nSize = m_pGridBrowse->GetDataSize();
	for (int i = 0; i < nSize; i++)
	{
		CString str = m_pGridBrowse->GetDataAt(i, 0) + "\t";
#ifdef _OLD_
		for (int j = 1; j < 5; j++)
			str += m_pGridBrowse->GetDataAt(i, j) + ",";
		str = str.Left(str.GetLength() - 1);
#else
		str += m_pGridBrowse->GetDataAt(i, 1) + ",";
		str += m_pGridBrowse->GetDataAt(i, 2) + ",";
		str += "1,";
		str += m_pGridBrowse->GetDataAt(i, 3) + ",";
		str += m_pGridBrowse->GetDataAt(i, 4);
#endif
		strArrayData.Add(str);
	}
	::SaveFile(strPath, strArrayData, TRUE);

	return TRUE;
}

int CDataBrowseWnd1::GetSelectedRow()
{
	return m_pGridBrowse->u_SelectedRow;
}

void CDataBrowseWnd1::DeleteSelectedRow()
{
	if (m_pGridBrowse->u_SelectedRow < 0)
		return;

	m_pGridBrowse->DeleteRows(m_pGridBrowse->u_SelectedRow, m_pGridBrowse->u_SelectedRow);
	m_pGridBrowse->u_SelectedRow = -1;
	m_pGridBrowse->RePaint();

	ClearDetail();
	m_pGridDetail->RePaint();

	SaveData();

	CString strName;
	strName.AppendFormat("%020d", this->GetDlgCtrlID());
	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, -1, (LPARAM)(LPCTSTR)strName);
}

void CDataBrowseWnd1::GetProfileName(CStringArray& strArrayProfile)
{
	strArrayProfile.RemoveAll();
	int nSize = m_pGridBrowse->GetDataSize();
	for (int i = 0; i < nSize; i++)
		strArrayProfile.Add(m_pGridBrowse->GetDataAt(i, 0));
}

void CDataBrowseWnd1::ReplaceSelected(CString strProfileName)
{
	int nSelRow = m_pGridBrowse->u_SelectedRow;
	m_pGridBrowse->SetDataAt(nSelRow, 0, strProfileName);
	m_pGridBrowse->RePaintRow(nSelRow, nSelRow);

	SaveData();

	strProfileName.AppendFormat("%020d", this->GetDlgCtrlID());
	m_pMSGWnd->SendMessage(WM_ROW_SELECTED, nSelRow, (LPARAM)(LPCTSTR)strProfileName);
}

void CDataBrowseWnd1::GetSelectedData(CStringArray& strArraySelected)
{
	strArraySelected.RemoveAll();
	int nSelRow = m_pGridBrowse->u_SelectedRow;
#ifdef _OLD_
	for (int i = 0; i < 5; i++)
		strArraySelected.Add(m_pGridBrowse->GetDataAt(nSelRow, i));
#else
	for (int i = 0; i < 5; i++)
		strArraySelected.Add(m_pGridBrowse->GetDataAt(nSelRow, i));
	strArraySelected.RemoveAt(4);
	strArraySelected.InsertAt(3,"1");
#endif
}

long CDataBrowseWnd1::GetAuFixValue()
{
	long nAuFixValue = 0;
	int nSelRow = m_pGridBrowse->u_SelectedRow;
	CString strAuFixValue = m_pGridBrowse->GetDataAt(nSelRow, 4);
	if (::IsValidNumber(strAuFixValue))
		nAuFixValue = atoi(strAuFixValue);
	return nAuFixValue;
}
