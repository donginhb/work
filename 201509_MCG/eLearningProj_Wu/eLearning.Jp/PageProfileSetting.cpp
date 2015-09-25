// PageProfileSetting.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageProfileSetting.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageProfileSetting property page

IMPLEMENT_DYNCREATE(CPageProfileSetting, CPropertyPage)

CPageProfileSetting::CPageProfileSetting() : CPropertyPage(CPageProfileSetting::IDD)
{
	//{{AFX_DATA_INIT(CPageProfileSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
}

CPageProfileSetting::~CPageProfileSetting()
{
}

void CPageProfileSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageProfileSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageProfileSetting, CPropertyPage)
	//{{AFX_MSG_MAP(CPageProfileSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_QUALITY, OnSelchangeComboAudioQuality)
	ON_CBN_SELCHANGE(IDC_COMBO_DIM, OnSelchangeComboDim)
	ON_CBN_SELCHANGE(IDC_COMBO_FPS, OnSelchangeComboFps)
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEO_QUALITY, OnSelchangeComboVideoQuality)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageProfileSetting message handlers

BOOL CPageProfileSetting::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIM);
	pCombo->AddString("320 x 240"); 
	pCombo->AddString("240 x 180"); 
	pCombo->AddString("160 x 120"); 
	::GetPrivateProfileString("Profile", "Dimension", "240 x 180", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FPS);
	pCombo->AddString("25"); 
	pCombo->AddString("20"); 
	pCombo->AddString("15"); 
	::GetPrivateProfileString("Profile", "FPS", "25", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_QUALITY);
	pCombo->AddString("Good"); 
	pCombo->AddString("Normal"); 
	pCombo->AddString("Rough"); 
	::GetPrivateProfileString("Profile", "VideoQuality", "Normal", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_QUALITY);
	pCombo->AddString("CD Music"); 
	pCombo->AddString("Sound Effect"); 
	pCombo->AddString("FM Radio"); 
	pCombo->AddString("AM Radio"); 
	pCombo->AddString("Voice"); 
	::GetPrivateProfileString("Profile", "AudioQuality", "FM Radio", szBuf, 256, strIniPath);
	pCombo->SetCurSel(pCombo->FindStringExact(-1, szBuf));

	::GetPrivateProfileString("Option", "Priority", "0", szBuf, 256, strIniPath);
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


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageProfileSetting::OnOK() 
{
	CString strText;
	CString strIniPath = MACRO_INI_PATH;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIM);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("Profile", "Dimension", strText, strIniPath);
	
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FPS);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("Profile", "FPS", strText, strIniPath);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_QUALITY);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("Profile", "VideoQuality", strText, strIniPath);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_QUALITY);
	pCombo->GetWindowText(strText);
	::WritePrivateProfileString("Profile", "AudioQuality", strText, strIniPath);

	CButton* pBtnHigh = (CButton*)GetDlgItem(IDC_RADIO_HIGH);
	CButton* pBtnLow = (CButton*)GetDlgItem(IDC_RADIO_LOW);

	int nHigh = pBtnHigh->GetCheck();
	CString strHigh;
	strHigh.Format("%d", nHigh);

	::WritePrivateProfileString("Option", "Priority", strHigh, strIniPath);

	CPropertyPage::OnOK();
}

BOOL CPageProfileSetting::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageProfileSetting::OnSelchangeComboAudioQuality() 
{
}

void CPageProfileSetting::OnSelchangeComboDim() 
{
}

void CPageProfileSetting::OnSelchangeComboFps() 
{
}

void CPageProfileSetting::OnSelchangeComboVideoQuality() 
{
}
