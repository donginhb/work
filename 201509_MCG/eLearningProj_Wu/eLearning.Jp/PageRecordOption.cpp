// PageRecordOption.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageRecordOption.h"
#include "Macro.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageRecordOption property page

IMPLEMENT_DYNCREATE(CPageRecordOption, CPropertyPage)

CPageRecordOption::CPageRecordOption() : CPropertyPage(CPageRecordOption::IDD)
{
	//{{AFX_DATA_INIT(CPageRecordOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageRecordOption::~CPageRecordOption()
{
}

void CPageRecordOption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageRecordOption)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageRecordOption, CPropertyPage)
	//{{AFX_MSG_MAP(CPageRecordOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageRecordOption message handlers

BOOL CPageRecordOption::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString strIniPath = MACRO_INI_PATH;

	char szBuf[256];
	//add by buxiangyi
	/*CButton* pCheckPNG = (CButton*)GetDlgItem(IDC_CHECK_PNG);
	::GetPrivateProfileString("Option", "ConvertPNG", "0", szBuf, 256, strIniPath);
	pCheckPNG->SetCheck(atoi(szBuf));*/
	////////////////////////////////////////////////////////////////////
	::GetPrivateProfileString("Option", "StopCapture", "Q", szBuf, 256, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HOT);
	pCombo->SetCurSel(pCombo->FindString(-1, szBuf));
	//add by buxiangyi
	/*::GetPrivateProfileString("Option", "Priority", "0", szBuf, 256, strIniPath);
	
	CButton* pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_HIGH);
	CButton* pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_LOW);
	if (atoi(szBuf) != 0)
	{
		pBtnHigh->SetCheck(1);
		pBtnLow->SetCheck(0);
	}
	else
	{
		pBtnHigh->SetCheck(0);
		pBtnLow->SetCheck(1);
	}
	::GetPrivateProfileString("Option", "ScreenPriority", "0", szBuf, 255, strIniPath);
	pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_SHIGH);
	pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_SLOW);
	if (atoi(szBuf) != 0)
	{
		pBtnHigh->SetCheck(1);
		pBtnLow->SetCheck(0);
	}
	else
	{
		pBtnHigh->SetCheck(0);
		pBtnLow->SetCheck(1);
	}*/
	////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageRecordOption::OnOK() 
{
	CString strIniPath = MACRO_INI_PATH;

	//add by buxiangyi
	/*CButton* pCheckPNG = (CButton*)GetDlgItem(IDC_CHECK_PNG);
	int nPNG = pCheckPNG->GetCheck();
	CString strPNG;
	strPNG.Format("%d", nPNG);
	::WritePrivateProfileString("Option", "ConvertPNG", strPNG, strIniPath);*/
	/////////////////////////////////////////////////////////////////

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HOT);
	CString strHot;
	pCombo->GetWindowText(strHot);
	::WritePrivateProfileString("Option", "StopCapture", strHot, strIniPath);

	//add by buxiangyi

	/*CButton* pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_SHIGH);
	CButton* pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_SLOW);
	int nHigh = pBtnHigh->GetCheck();
	CString strHigh;
	strHigh.Format("%d", nHigh);
	::WritePrivateProfileString("Option", "ScreenPriority", strHigh, strIniPath);

	pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_HIGH);
	pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_LOW);
	nHigh = pBtnHigh->GetCheck();
	strHigh.Format("%d", nHigh);
	::WritePrivateProfileString("Option", "Priority", strHigh, strIniPath);*/

	CPropertyPage::OnOK();
}
