// DlgSetProperty.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelDevice.h"
#include "cmkapi.h"
#include "CaptureDevApi.h"
#include "macro.h"
#include <dshow.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifndef OATRUE
//#define OATRUE (-1)
//#endif // OATRUE
//#ifndef OAFALSE
//#define OAFALSE (0)
//#endif // OAFALSE

#ifndef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelDevice dialog


CDlgSelDevice::CDlgSelDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelDevice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pEasyCapture = NULL;
	m_bNoAsk = TRUE;
	m_bWithCam = FALSE;
	m_bWithAudio = FALSE;
}


void CDlgSelDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelDevice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelDevice, CDialog)
	//{{AFX_MSG_MAP(CDlgSelDevice)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_CAM, OnRadioCam)
	ON_BN_CLICKED(IDC_RADIO_NO_VIDEO, OnRadioNoVideo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, OnButtonReload)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO, OnSelchangeComboAudio)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_VIDEO, OnSelchangeComboCamVideo)
	ON_CBN_SELCHANGE(IDC_COMBO_SCREEN_AUDIO, OnSelchangeComboScreenAudio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelDevice message handlers

BOOL CDlgSelDevice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    HRESULT hval = 0;
	hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&m_pEasyCapture);
    if (hval != S_OK) 
	{
		MessageBox("Error Open: EasyCapture Fail");
		return FALSE;
	}

	hval = m_pEasyCapture->Open(0);
    if((hval != S_OK)) 
	{
		MessageBox("Error Open: EasyCapture Open Fail");
		return FALSE;
	}


	ReloadDev();

	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pScAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);

	CString strType, strParam1, strParam2;
	::GetDevice(strType, strParam1, strParam2);
	int nIndex;	
//wangwang
//	if (BUNDLE_VERSION || PROMOTE_VERSION)

	if ((strType == "CAM" && m_bWithCam) || (strType == "" && m_bWithCam))
	{
		pButtonCam->SetCheck(1);
		RadioCam();

		nIndex = pCamVideoCombo->FindString(-1, strParam1);
		pCamVideoCombo->SetCurSel(nIndex);
	}

	if (strType == "-" || (strType == "CAM" && !m_bWithCam))
	{
		pButtonNoVideo->SetCheck(1);
		OnRadioNoVideo();
	}

	nIndex = pAudioCombo->FindString(-1, strParam2);
	pAudioCombo->SetCurSel(nIndex);

	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;
	::GetPrivateProfileString("Device", "ScreenAudio", "", szBuf, 255, strIniPath);
	nIndex = pScAudioCombo->FindString(-1, szBuf);
	pScAudioCombo->SetCurSel(nIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelDevice::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pEasyCapture != NULL) 
	{
		m_pEasyCapture->Close();
		RELEASE(m_pEasyCapture);
	}
}

void CDlgSelDevice::SetComboContent(UINT nID, CStringArray& strArray) 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->ResetContent();
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);
}

void CDlgSelDevice::OnOK() 
{
	CComboBox* pAudio = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pCamVideo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pScAudio = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);
	
	CButton* pRadioNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pRadioCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	int nCheckNoVideo = pRadioNoVideo->GetCheck();
	int nCheckCam = pRadioCam->GetCheck();

	CString strAudio, strCamVideo, strScAudio;
	pAudio->GetWindowText(strAudio);
	pCamVideo->GetWindowText(strCamVideo);
	pScAudio->GetWindowText(strScAudio);
	if (strAudio == "" || strScAudio == "" || (nCheckCam && strCamVideo == "")) 
		//(m_bWithCam && strCamVideo == "") || (m_bWithDV && strDVVideo == ""))
	{
		W_SHOW_ERROR(IDS_ANY_DEVICE_CANNOT_EMPTY, this);
		return;
	}

	//if (!nCheckDV && !nCheckCam)
	//	return;
	
	CString strType = "";
	if (nCheckCam)
		strType = "CAM";
	CString strIniPath = MACRO_INI_PATH;
	CString strValue;
	CAPTURESESSION eSession;

	int nAudioIndex, nVideoIndex;

	if (strType == "CAM")
	{
		::WritePrivateProfileString("Device", "Type", strType, strIniPath);
		pCamVideo->GetWindowText(strValue);
		::WritePrivateProfileString("Device", "Video", strValue, strIniPath);
		eSession = CAPTURESESSION_CAM;
		nVideoIndex = pCamVideo->GetCurSel();
	}
	else
	{
		::WritePrivateProfileString("Device", "Type", "-", strIniPath);
		::WritePrivateProfileString("Device", "Video", "", strIniPath);
	}
	
	pAudio->GetWindowText(strValue);
	::WritePrivateProfileString("Device", "Audio", strValue, strIniPath);
	nAudioIndex = pAudio->GetCurSel();

	pScAudio->GetWindowText(strValue);
	::WritePrivateProfileString("Device", "ScreenAudio", strValue, strIniPath);

	CDialog::OnOK();
}

void CDlgSelDevice::OnRadioCam() 
{
	RadioCam(); 
}

void CDlgSelDevice::OnRadioNoVideo() 
{
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	pCamVideoCombo->EnableWindow(FALSE);
}

void CDlgSelDevice::RadioCam() 
{
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	if (pButtonCam->GetCheck())
	{
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	
		if (m_bWithCam)
			pCamVideoCombo->EnableWindow(TRUE);
	}
}

HBRUSH CDlgSelDevice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (GetDlgItem(IDC_STATIC_VIDEO)->m_hWnd == pWnd->m_hWnd ||
		GetDlgItem(IDC_STATIC_AUDIO)->m_hWnd == pWnd->m_hWnd ||
		GetDlgItem(IDC_STATIC_SCREEN_AUDIO)->m_hWnd == pWnd->m_hWnd)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	return hbr;
}

void CDlgSelDevice::OnButtonReload() 
{
	ReloadDev();
}

BOOL CDlgSelDevice::ReloadDev() 
{
	CStringArray strArrayWebAudio, strArrayWebVideo;
	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	::EnumDevice(m_pEasyCapture, eType, strArrayWebAudio);
	
	eType = CAPTUREDEVICETYPE_VIDEO;
	::EnumDevice(m_pEasyCapture, eType, strArrayWebVideo);


	CString strCam, strAudio, strDemoAudio;
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pScAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);
	pCamVideoCombo->GetWindowText(strCam);
	pAudioCombo->GetWindowText(strAudio);
	pScAudioCombo->GetWindowText(strDemoAudio);

	SetComboContent(IDC_COMBO_CAM_VIDEO, strArrayWebVideo); 
	SetComboContent(IDC_COMBO_AUDIO, strArrayWebAudio); 
	SetComboContent(IDC_COMBO_SCREEN_AUDIO, strArrayWebAudio); 

	int nIndex;
	if ((nIndex = pCamVideoCombo->FindString(-1, strCam)) >= 0)
		pCamVideoCombo->SetCurSel(nIndex);
	if ((nIndex = pAudioCombo->FindString(-1, strAudio)) >= 0)
		pAudioCombo->SetCurSel(nIndex);
	if ((nIndex = pScAudioCombo->FindString(-1, strDemoAudio)) >= 0)
		pScAudioCombo->SetCurSel(nIndex);

	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);

	m_bWithCam = strArrayWebVideo.GetSize();
	m_bWithAudio = strArrayWebAudio.GetSize();

	if (m_bWithCam)
	{
		pButtonCam->EnableWindow(TRUE);
		if (pButtonCam->GetCheck())
			pCamVideoCombo->EnableWindow(TRUE);
	}
	else
	{
		pButtonCam->EnableWindow(FALSE);
		pCamVideoCombo->EnableWindow(FALSE);
	}

	if (m_bWithAudio)
	{
		pAudioCombo->EnableWindow(TRUE);
		pScAudioCombo->EnableWindow(TRUE);
	}
	else
	{
		pAudioCombo->EnableWindow(FALSE);
		pScAudioCombo->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgSelDevice::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgSelDevice::OnSelchangeComboAudio() 
{
}

void CDlgSelDevice::OnSelchangeComboCamVideo() 
{
}

void CDlgSelDevice::OnSelchangeComboScreenAudio() 
{
}
