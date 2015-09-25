// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelDevice.h"
#include "NewWizDialog.h"
#include "cmkapi.h"
#include "CaptureDevApi.h"
#include "macro.h"
#include <dshow.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelDevice dialog


CPageSelDevice::CPageSelDevice(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelDevice)
	//}}AFX_DATA_INIT
	m_pEasyCapture = NULL;
	m_bNoAsk = TRUE;
	m_bWithCam = FALSE;
	m_bWithAudio = FALSE;
}


void CPageSelDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelDevice)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelDevice, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelDevice)
	ON_BN_CLICKED(IDC_RADIO_CAM, OnRadioCam)
	ON_BN_CLICKED(IDC_RADIO_NO_VIDEO, OnRadioNoVideo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelDevice message handlers

BOOL CPageSelDevice::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_Font2.CreateFont(-16, 0, 0, 0, 
		FW_NORMAL, TRUE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, //_T("Arial"));
#ifdef _JAPANESE
	_T("MS UI Gothic"));
#else
	_T("Arial"));
#endif

    GetDlgItem(IDC_STATIC_STEP)->SetFont(&m_Font2, TRUE);

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

	CString strType, strParam1, strParam2;
	::GetDevice(strType, strParam1, strParam2);
	int nIndex;	

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

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageSelDevice::OnWizardBack()
{
	m_strData  = "";
	m_strData2 = "";
	m_strData3 = "";

	return IDW_INPUT_NAME;
}

LRESULT CPageSelDevice::OnWizardNext()
{
	CComboBox* pAudio = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pCamVideo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	
	CButton* pRadioNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pRadioCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	int nCheckNoVideo = pRadioNoVideo->GetCheck();
	int nCheckCam = pRadioCam->GetCheck();

	CString strAudio, strCamVideo, strScAudio;
	pAudio->GetWindowText(strAudio);
	pCamVideo->GetWindowText(strCamVideo);
	if (strAudio == "" || (nCheckCam && strCamVideo == "")) 
		//(m_bWithCam && strCamVideo == "") || (m_bWithDV && strDVVideo == ""))
	{
		W_SHOW_ERROR(IDS_ANY_DEVICE_CANNOT_EMPTY, this);
		return -1;
	}

	m_strData = strAudio;
	if (nCheckNoVideo)
		m_strData2 = "0";

	else 
	{
		m_strData2 = "2";
		m_strData3 = strCamVideo;
	}

	return 0;
}

void CPageSelDevice::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_DEVICE);
	m_pParent->SetTitle(strTitle);
}

BOOL CPageSelDevice::OnKillActive()
{
	return TRUE;
}

BOOL CPageSelDevice::ReloadDev() 
{
	CStringArray strArrayWebAudio, strArrayWebVideo;
	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	::EnumDevice(m_pEasyCapture, eType, strArrayWebAudio);
	
	eType = CAPTUREDEVICETYPE_VIDEO;
	::EnumDevice(m_pEasyCapture, eType, strArrayWebVideo);

	CString strCam, strAudio, strDemoAudio;
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	pCamVideoCombo->GetWindowText(strCam);
	pAudioCombo->GetWindowText(strAudio);

	SetComboContent(IDC_COMBO_CAM_VIDEO, strArrayWebVideo); 
	SetComboContent(IDC_COMBO_AUDIO, strArrayWebAudio); 
	//SetComboContent(IDC_COMBO_DV_VIDEO, strArrayDV); 

	int nIndex;
	if ((nIndex = pCamVideoCombo->FindString(-1, strCam)) >= 0)
		pCamVideoCombo->SetCurSel(nIndex);
	if ((nIndex = pAudioCombo->FindString(-1, strAudio)) >= 0)
		pAudioCombo->SetCurSel(nIndex);

	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);

	//m_bWithDV = strArrayDV.GetSize();
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
	}
	else
	{
		pAudioCombo->EnableWindow(FALSE);
	}

	return TRUE;
}

void CPageSelDevice::OnRadioCam() 
{
	RadioCam(); 
}

void CPageSelDevice::OnRadioNoVideo() 
{
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	pCamVideoCombo->EnableWindow(FALSE);
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	pButtonCam->SetCheck(0);
}

void CPageSelDevice::RadioCam() 
{
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	pButtonNoVideo->SetCheck(0);
	if (pButtonCam->GetCheck())
	{
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	
		if (m_bWithCam)
			pCamVideoCombo->EnableWindow(TRUE);
	}
}
/*
void CPageSelDevice::OnRadioDv() 
{
	RadioDv();
}

void CPageSelDevice::RadioDv() 
{
	CButton* pButtonDV = (CButton*)GetDlgItem(IDC_RADIO_DV);
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	pButtonCam->SetCheck(0);
	pButtonNoVideo->SetCheck(0);
	if (pButtonDV->GetCheck())
	{
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
		CComboBox* pDVVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DV_VIDEO);
	
		pCamVideoCombo->EnableWindow(FALSE);
		if (m_bWithDV)
			pDVVideoCombo->EnableWindow(TRUE);
	}
}
*/
void CPageSelDevice::SetComboContent(UINT nID, CStringArray& strArray) 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->ResetContent();
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);
}
