// PageMisc.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageMisc.h"
#include "Macro.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMisc property page

IMPLEMENT_DYNCREATE(CPageMisc, CPropertyPage)

CPageMisc::CPageMisc() : CPropertyPage(CPageMisc::IDD)
{
	//{{AFX_DATA_INIT(CPageMisc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
}

CPageMisc::~CPageMisc()
{
}

void CPageMisc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMisc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMisc, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMisc)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_HIGH, OnRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_LOW, OnRadioLow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMisc message handlers

BOOL CPageMisc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;

	::GetPrivateProfileString("Option", "DefaultCaptionTime", "10", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_CAPTION_LENGTH)->SetWindowText(szBuf);

	::GetPrivateProfileString("Option", "DefaultEffectTime", "3", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_EFFECT_LENGTH)->SetWindowText(szBuf);

	//add by buxiangyi
	/*int n = ::GetPrivateProfileInt("Option", "ConvertMethod", 0, strIniPath);
	if (n == 0)
		((CButton*)GetDlgItem(IDC_RADIO_GENERAL))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RADIO_BEST))->SetCheck(1);
	

	::GetPrivateProfileString("Option", "FreeHandUnitTime", "100", szBuf, 256, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FREEHAND_UNIT);
	int nSel = pCombo->FindString(-1, szBuf);
	pCombo->SetCurSel(nSel);*/
	/////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMisc::OnOK() 
{
	CString strIniPath = MACRO_INI_PATH;
	//add by buxaingyi

	CString strcaptiontime, streffecttime;
	GetDlgItem(IDC_EDIT_CAPTION_LENGTH)->GetWindowText(strcaptiontime);
	GetDlgItem(IDC_EDIT_EFFECT_LENGTH)->GetWindowText(streffecttime);

	
	if (strcaptiontime == "")
	{
		strcaptiontime = "10";
	} 
	if (streffecttime == "")
	{
		streffecttime = "3";
	} 
	
	::WritePrivateProfileString("Option", "DefaultCaptionTime", strcaptiontime, strIniPath);
	::WritePrivateProfileString("Option", "DefaultEffectTime", streffecttime, strIniPath);

	//add by buxiangyi
	/*int n = ((CButton*)GetDlgItem(IDC_RADIO_GENERAL))->GetCheck();
	if (n == 0)
		::WritePrivateProfileString("Option", "ConvertMethod", "1", strIniPath);
	else
		::WritePrivateProfileString("Option", "ConvertMethod", "0", strIniPath);
	
	CString strUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FREEHAND_UNIT);
	pCombo->GetWindowText(strUnit);
	::WritePrivateProfileString("Option", "FreeHandUnitTime", strUnit, strIniPath);*/
	///////////////////////////////////////////////////////////////
	CPropertyPage::OnOK();
}

void CPageMisc::OnDestroy() 
{
	CPropertyPage::OnDestroy();
}

BOOL CPageMisc::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageMisc::OnCancel() 
{
	CPropertyPage::OnCancel();
}

void CPageMisc::OnRadioHigh() 
{
}

void CPageMisc::OnRadioLow() 
{
}
