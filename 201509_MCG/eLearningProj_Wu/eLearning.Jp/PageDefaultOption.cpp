// PageDefaultOption.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageDefaultOption.h"
#include "macro.h"
#include "cmkapi.h"
#include "Mainfrm.h"
#include "FileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOption property page

IMPLEMENT_DYNCREATE(CPageDefaultOption, CPropertyPage)

CPageDefaultOption::CPageDefaultOption() : CPropertyPage(CPageDefaultOption::IDD)
{
	//{{AFX_DATA_INIT(CPageDefaultOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
	m_PageDefaultOptionFlag = TRUE;
}

CPageDefaultOption::~CPageDefaultOption()
{
}

void CPageDefaultOption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDefaultOption)
	DDX_Control(pDX, IDC_LIST_OPTION, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDefaultOption, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDefaultOption)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOption message handlers

BOOL CPageDefaultOption::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CDWordArray idArray;
	//add by buxiangyi
	//idArray.Add(IDS_OPTION_FRAME_BORDER);
	//idArray.Add(IDS_OPTION_STATUS_BAR);
	///////////////////////////////
	idArray.Add(IDS_OPTION_SHOW_TIME);
//	idArray.Add(IDS_OPTION_SET_TIME_ACCU);
	idArray.Add(IDS_OPTION_SHOW_HEADNO);
	//add by buxiangyi
	//idArray.Add(IDS_OPTION_HYPER_UNDERLINE);
	idArray.Add(IDS_OPTION_BG_IMAGE);
	//add by buxiangyi
	//idArray.Add(IDS_OPTION_TV_IMAGE);
	////////////////////////////////////
	idArray.Add(IDS_OPTION_SHOW_ADVISOR);
	//add by buxiangyi
	//idArray.Add(IDS_OPTION_ADVISOR_TRANS);
	idArray.Add(59501);	//add 2014/7/24 kagata BugNo.133
	//add by buxiangyi
	idArray.Add(IDS_OPTION_POINT);
	idArray.Add(IDS_PPT_INDEX);
	////////////////////////////////
#ifdef _PRO
//	idArray.Add(IDS_USE_DRAW_FUNCTION);		// del itoh 2013/07/24 PPT2013
//	idArray.Add(IDS_OPTION_USE_ACTIVEX70);
#endif

	//shien add 20050429
#ifdef _TDT
//	idArray.Add(IDS_USE_DRAW_FUNCTION);		// del itoh 2013/07/24 PPT2013
#endif


	for (int i = 0; i < idArray.GetSize(); i++)
	{
		CString str;
		str.LoadString(idArray[i]);
		m_clCourse.AddString(" " + str);
	}
	

	CString strOptionPath;
	if (m_PageDefaultOptionFlag == TRUE)
	{
		strOptionPath = MACRO_TEMPLATE_PATH + "js\\config.js";
	} 
	else
	{
		strOptionPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()) + "config.js";
	}
	SetOption(strOptionPath);

	m_clCourse.CenterWindow(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDefaultOption::ClearCheckItem()
{
	for (int i = 0; i < m_clCourse.GetCount(); i++)
		m_clCourse.SetCheck(i, 0);
}

void CPageDefaultOption::SetOption(CString strOptionPath)
{

	if (!::FileExists(strOptionPath))
	{
		ClearCheckItem();
		return;
	}
	CStringArray strArrayOption;
	::LoadFile(strOptionPath, strArrayOption, TRUE);
	for (int i = 0; i < strArrayOption.GetSize(); i++)
	{
		CString strOption = strArrayOption[i];
		strOption.TrimLeft();
		if (strOption == "")
			continue;

		CString strCheck = strOption.Right(2);
		int nCheck = atoi(strCheck.Left(1));
		if (strOption.Find("iSlideTime") >= 0)
			SetCheckItem(IDS_OPTION_SHOW_TIME, nCheck);
		else if (strOption.Find("iNumber") >= 0)
			SetCheckItem(IDS_OPTION_SHOW_HEADNO, nCheck);
		else if (strOption.Find("bBackimage") >= 0)
			SetCheckItem(IDS_OPTION_BG_IMAGE, nCheck);
		else if (strOption.Find("vAdviser") >= 0)
			SetCheckItem(IDS_OPTION_SHOW_ADVISOR, nCheck);
		else if (strOption.Find("vFastbutton") >= 0)		//add 2014/7/24 kagata BugNo.133
			SetCheckItem(59501, nCheck);//add 2014/7/24 kagata BugNo.133
		//add by buxiangyi
		else if (strOption.Find("drawflag") >= 0)
			SetCheckItem(IDS_OPTION_POINT, nCheck);
		else if (strOption.Find("iThumb") >= 0)
			SetCheckItem(IDS_PPT_INDEX, nCheck);
	}
}

void CPageDefaultOption::SetCheckItem(UINT nID, int nCheck)
{
	CString str;
	str.LoadString(nID);

	for (int i = 0; i < m_clCourse.GetCount(); i++)
	{
		CString strItem;
		m_clCourse.GetText(i, strItem);
		strItem.TrimLeft();

		if (strItem == str)
		{
			m_clCourse.SetCheck(i, nCheck);
			return;
		}
	}
}

void CPageDefaultOption::WriteOptionFile(CString strPath)
{
	CStringArray strArray, strOldArray;
	::LoadFile(strPath, strOldArray);
	::LoadFile(strPath, strArray);
	for (int i = 0; i < m_clCourse.GetCount(); i++)
	{
		CString strItem;
		m_clCourse.GetText(i, strItem);
		strItem.TrimLeft();
		CString strVar = "";
		int nCheck = m_clCourse.GetCheck(i);
		if (StrEqual(strItem, IDS_OPTION_SHOW_TIME))
			strVar = "iSlideTime";
		else if (StrEqual(strItem, IDS_OPTION_SHOW_HEADNO))
			strVar = "iNumber";
		else if (StrEqual(strItem, IDS_OPTION_BG_IMAGE))
			strVar = "bBackimage";
		else if (StrEqual(strItem, IDS_OPTION_SHOW_ADVISOR))
			strVar = "vAdviser";
		else if (StrEqual(strItem, 59501))	//add 2014/7/24 kagata BugNo.133
			strVar = "vFastbutton";		//add 2014/7/24 kagata BugNo.133
		else if (StrEqual(strItem, IDS_OPTION_POINT))
			strVar = "drawflag";
		else if (StrEqual(strItem, IDS_PPT_INDEX))
			strVar = "iThumb";

		if (strVar != "")
		{
			for (int i = 0; i < strArray.GetCount(); i++)
			{
				CString str = strArray[i];
				if (str.Find(strVar)>=0)
				{
					CString strLine;
					strLine.Format("var %s = %d;", strVar, nCheck);
					strArray[i] = strLine;
				}
			}
		}
	}
	UpdateFileByTag(strPath.Left(strPath.Find("config.js")), "config.js", strOldArray, strArray, TRUE);
}

BOOL CPageDefaultOption::StrEqual(CString strInput, UINT nID)
{
	CString str;
	str.LoadString(nID);

	strInput.TrimLeft();

	return (strInput == str);
}

void CPageDefaultOption::OnOK() 
{
	//add by buxiangyi
	CString strPath;
	if (m_PageDefaultOptionFlag == TRUE)
	{
		strPath = MACRO_TEMPLATE_PATH + "js\\config.js";
	} 
	else
	{
		strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()) + "config.js";
	}
	WriteOptionFile(strPath);
	/////////////////////////////////////////////////////////////////////////////
	

	CPropertyPage::OnOK();
}

BOOL CPageDefaultOption::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

LRESULT CPageDefaultOption::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}