// PageScreenProfile.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageScreenProfile.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageScreenProfile property page

IMPLEMENT_DYNCREATE(CPageScreenProfile, CPropertyPage)

CPageScreenProfile::CPageScreenProfile() : CPropertyPage(CPageScreenProfile::IDD)
{
	//{{AFX_DATA_INIT(CPageScreenProfile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
}

CPageScreenProfile::~CPageScreenProfile()
{
}

void CPageScreenProfile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageScreenProfile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageScreenProfile, CPropertyPage)
	//{{AFX_MSG_MAP(CPageScreenProfile)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_QUALITY_S, OnSelchangeComboAudioQualityS)
	ON_CBN_SELCHANGE(IDC_COMBO_FPS_S, OnSelchangeComboFpsS)
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEO_QUALITY_S, OnSelchangeComboVideoQualityS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageScreenProfile message handlers

BOOL CPageScreenProfile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FPS_S);
	pCombo->AddString("1"); 
	pCombo->AddString("2"); 
	pCombo->AddString("3"); 
	pCombo->AddString("4"); 
	pCombo->AddString("5"); 
	pCombo->AddString("6"); 
	pCombo->AddString("7"); 
	pCombo->AddString("8"); 
	pCombo->AddString("9"); 
	pCombo->AddString("10"); 
	::GetPrivateProfileString("ProfileScreen", "FPS", "2", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_QUALITY_S);
	pCombo->AddString("320"); 
	pCombo->AddString("288"); 
	pCombo->AddString("256"); 
	pCombo->AddString("224"); 
	pCombo->AddString("192"); 
	pCombo->AddString("160"); 
	pCombo->AddString("128"); 
	pCombo->AddString("102"); 
	pCombo->AddString("96"); 
	pCombo->AddString("80"); 
	pCombo->AddString("64"); 
	::GetPrivateProfileString("ProfileScreen", "VideoQuality", "128", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_QUALITY_S);
	pCombo->AddString("A"); 
	pCombo->AddString("B"); 
	pCombo->AddString("C"); 
	pCombo->AddString("D"); 
	pCombo->AddString("E"); 
	::GetPrivateProfileString("ProfileScreen", "AudioQuality", "A", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	::GetPrivateProfileString("ProfileScreen", "CodecID", "0", szBuf, 256, strIniPath);
	CButton* pRadioV = (CButton*)GetDlgItem(IDC_RADIO_VOICE);
	CButton* pRadioS = (CButton*)GetDlgItem(IDC_RADIO_SCREEN);
	if (atoi(szBuf))
		pRadioV->SetCheck(1);
	else
		pRadioS->SetCheck(1);

	::GetPrivateProfileString("Option", "ScreenPriority", "0", szBuf, 255, strIniPath);
	CButton* pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_SHIGH);
	CButton* pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_SLOW);
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageScreenProfile::OnOK() 
{
	CString strText;
	CString strIniPath = MACRO_INI_PATH;

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FPS_S);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("ProfileScreen", "FPS", strText, strIniPath);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_QUALITY_S);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("ProfileScreen", "VideoQuality", strText, strIniPath);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_QUALITY_S);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("ProfileScreen", "AudioQuality", strText, strIniPath);

	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_VOICE);
	strText.Format("%d", pRadio->GetCheck());
	::WritePrivateProfileString("ProfileScreen", "CodecID", strText, strIniPath);

	CButton* pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_SHIGH);
	CButton* pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_SLOW);

	int nHigh = pBtnHigh->GetCheck();
	CString strHigh;
	strHigh.Format("%d", nHigh);
	::WritePrivateProfileString("Option", "ScreenPriority", strHigh, strIniPath);

	CPropertyPage::OnOK();
}

BOOL CPageScreenProfile::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageScreenProfile::OnSelchangeComboAudioQualityS() 
{
}

void CPageScreenProfile::OnSelchangeComboFpsS() 
{
}

void CPageScreenProfile::OnSelchangeComboVideoQualityS() 
{
}
