// DlgSetProperty.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "Device.h"
#include "cmkapi.h"
#include "CaptureDevApi.h"
#include "macro.h"
#include <dshow.h>
//add by buxiangyi
#include "DataBrowseWnd1.h"
#include "DataBrowseWnd.h"
#include "DlgInputProfileName.h"
#include "ColGrid.h"
#include "MainFrm.h"
////////////////////////////
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
// CDevice dialog


CDevice::CDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDevice)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIniPath = MACRO_INI_PATH;
	m_pEasyCapture = NULL;
	m_bWithCam = FALSE;
	m_bWithAudio = FALSE;
	m_bNoAsk = TRUE;
	m_Initialization = FALSE;
	m_ScreenCapture = TRUE;
	m_NewCourse = TRUE;
}


void CDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDevice)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDevice, CDialog)
	//{{AFX_MSG_MAP(CDevice)
	ON_WM_DESTROY()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &CDevice::OnBnClickedButtonRename)
	ON_BN_CLICKED(IDC_BUTTON_RENAME1, &CDevice::OnClickedButtonRename1)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDevice::OnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEL1, &CDevice::OnClickedButtonDel1)
	ON_BN_CLICKED(IDC_RADIO_NO_VIDEO, OnRadioNoVideo)
	ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CDevice::OnClickedButtonReload)
	ON_BN_CLICKED(IDC_RADIO_CAM, OnRadioCam)
	ON_BN_CLICKED(	IDC_RADIO_IP, OnRadioIP)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO, OnSelchangeComboAudio)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_VIDEO, OnSelchangeComboCamVideo)
	ON_CBN_SELCHANGE(IDC_COMBO_SCREEN_AUDIO, OnSelchangeComboScreenAudio)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_FRAME, OnSelchangeComboAudioFrame)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_VIDEO_FRAME, OnSelchangeComboCamVideoFrame)
	ON_CBN_SELCHANGE(IDC_COMBO_SCREEN_AUDIO_FRAME, OnSelchangeComboScreenAudioFrame)
	ON_BN_CLICKED(IDCANCEL, &CDevice::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevice message handlers

BOOL CDevice::OnInitDialog() 
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
	CButton* pButtonIP = (CButton*)GetDlgItem(IDC_RADIO_IP);
	CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pScAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);

	CComboBox* pAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_FRAME);
	CComboBox* pCamVideoFormat = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
	CComboBox* pScAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO_FRAME);
	CComboBox* pScMouse = (CComboBox*)GetDlgItem(IDC_COMBO_MOUSE_RECORD);

	CButton* pIpVoice = (CButton*)GetDlgItem(IDC_CHECK_IPVOICE);
	pIpVoice->EnableWindow(FALSE);

	CString strType, strParam1, strParam2;
	CStringArray strCamFrame, strAudioFrame, strScAudioFrame;
	int DeviceIndex, DeviceIndexFrame;
	char szBuf[256];
	::GetDevice(strType, strParam1, strParam2);
	int nIndex, pCamVideoComboindex;
	if ((strType == "CAM" && m_bWithCam) || (strType == "" && m_bWithCam))
	{
		pButtonCam->SetCheck(1);
		RadioCam();

		nIndex = pCamVideoCombo->FindString(-1, strParam1);
		pCamVideoCombo->SetCurSel(nIndex);
		pCamVideoComboindex = nIndex;
		DeviceIndex = ::FindDeviceValid(CAPTUREDEVICETYPE_VIDEO, strParam1);
		if (DeviceIndex >= 0)
		{
			::GetPrivateProfileString("Profile", "Param6", "", szBuf, 512, m_strIniPath);
			::FindDeviceFrameValid(CAPTUREDEVICETYPE_VIDEO,DeviceIndex, szBuf, strCamFrame);
			for (int i = 0; i < strCamFrame.GetSize(); i++)
			{
				CString strOneCamFrame = strCamFrame.GetAt(i);
				strOneCamFrame = strOneCamFrame.Left(strOneCamFrame.Find(" "));
				int nWidth = atoi(strOneCamFrame.Left(strOneCamFrame.Find("x")));
				int nHeight = atoi(strOneCamFrame.Right(strOneCamFrame.Find("x")));
				if (nWidth <= 300 || nHeight <= 300 || nWidth >= 1920 || nHeight >= 1920 
					|| nWidth % 16 != 0 || nHeight % 16 != 0)
				{
					strCamFrame.RemoveAt(i);
					i--;
				}
			}
			pCamVideoFormat ->ResetContent();
			SetComboContent(IDC_COMBO_CAM_VIDEO_FRAME, strCamFrame);
			DeviceIndexFrame = pCamVideoFormat->FindString(-1, szBuf);
			pCamVideoFormat ->SetCurSel(DeviceIndexFrame);
		}
		else
		{
			pCamVideoFormat ->SetCurSel(-1);
		}
	}

	if (strType == "-" || (strType == "CAM" && !m_bWithCam))
	{
		pButtonNoVideo->SetCheck(1);

		CButton* pButtonIP = (CButton*)GetDlgItem(IDC_RADIO_IP);
		CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
		CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
		if (pButtonNoVideo->GetCheck())
		{
			pButtonCam->SetCheck(0);
			pButtonIP ->SetCheck(0);
			CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
			pCamVideoCombo->EnableWindow(FALSE);
			CComboBox* pCamVideoComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
			pCamVideoComboFrame->EnableWindow(FALSE);

			CEdit* pCEditURL = (CEdit*)GetDlgItem(IDC_EDIT_URL); 
			pCEditURL ->EnableWindow(FALSE);
			CEdit* pCEditIP = (CEdit*)GetDlgItem(IDC_EDIT_IP);
			pCEditIP ->EnableWindow(FALSE);
			CEdit* pCEditPassword = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
			pCEditPassword ->EnableWindow(FALSE);
		}
		//OnRadioNoVideo();
	}

	nIndex = pAudioCombo->FindString(-1, strParam2);
	pAudioCombo->SetCurSel(nIndex);
	DeviceIndex = ::FindDeviceValid(CAPTUREDEVICETYPE_AUDIO, strParam2);
	if (DeviceIndex >= 0)
	{
		::GetPrivateProfileString("Profile", "Param5", "", szBuf, 512, m_strIniPath);
		::FindDeviceFrameValid(CAPTUREDEVICETYPE_AUDIO,DeviceIndex, szBuf, strAudioFrame);
		for (int i = 0; i < strAudioFrame.GetSize(); i++)
		{
			if (strAudioFrame.GetAt(i).Find("16 Bits") < 0 
				|| (strAudioFrame.GetAt(i).Find("1 Channels") < 0 && strAudioFrame.GetAt(i).Find("2 Channels") < 0) 
				|| (strAudioFrame.GetAt(i).Find("44100 Hz") < 0 && strAudioFrame.GetAt(i).Find("48000 Hz") < 0))
			{
				strAudioFrame.RemoveAt(i);
				i--;
			}
		}
		pAudioFormat ->ResetContent();
		SetComboContent(IDC_COMBO_AUDIO_FRAME, strAudioFrame); 
		DeviceIndexFrame = pAudioFormat->FindString(-1, szBuf);
		pAudioFormat ->SetCurSel(DeviceIndexFrame);

	}
	else
	{
		pAudioFormat ->SetCurSel(-1);
	}



	CString strIniPath = MACRO_INI_PATH;
	::GetPrivateProfileString("Device", "ScreenAudio", "", szBuf, 255, strIniPath);
	nIndex = pScAudioCombo->FindString(-1, szBuf);
	pScAudioCombo->SetCurSel(nIndex);
	DeviceIndex = ::FindDeviceValid(CAPTUREDEVICETYPE_AUDIO, szBuf);
	if (DeviceIndex >= 0)
	{
		::GetPrivateProfileString("ScreenProfile", "Param5", "", szBuf, 512, m_strIniPath);
		::FindDeviceFrameValid(CAPTUREDEVICETYPE_AUDIO,DeviceIndex, szBuf,strScAudioFrame);
		for (int i = 0; i < strScAudioFrame.GetSize(); i++)
		{
			if (strScAudioFrame.GetAt(i).Find("16 Bits") < 0 
				|| (strScAudioFrame.GetAt(i).Find("1 Channels") < 0 && strScAudioFrame.GetAt(i).Find("2 Channels") < 0) 
				|| (strScAudioFrame.GetAt(i).Find("44100 Hz") < 0 && strScAudioFrame.GetAt(i).Find("48000 Hz") < 0))
			{
				strScAudioFrame.RemoveAt(i);
				i--;
			}
		}
		pScAudioFormat ->ResetContent();
		SetComboContent(IDC_COMBO_SCREEN_AUDIO_FRAME, strScAudioFrame); 
		DeviceIndexFrame = pScAudioFormat->FindString(-1, szBuf);
		pScAudioFormat ->SetCurSel(DeviceIndexFrame);
	}
	else
	{
		pScAudioFormat ->SetCurSel(-1);
	}

	CString m_Mouserecord;
	m_Mouserecord.LoadString(IDS_MOUSE_CURSOR_ENABLE);
	pScMouse ->AddString(m_Mouserecord);
	m_Mouserecord.LoadString(IDS_MOUSE_CURSOR_DISABLE);
	pScMouse ->AddString(m_Mouserecord);

	::GetPrivateProfileString("ScreenProfile", "Param6", "", szBuf, 255, strIniPath);
	CString MouseMark;
	MouseMark = szBuf;
	
	if (MouseMark == "False")
	{
		MouseMark = "‚µ‚È‚¢";
	} 
	else
	{
		MouseMark = "‚·‚é";
	}
	nIndex = pScMouse->FindString(-1, MouseMark);
	pScMouse->SetCurSel(nIndex);


	

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID10)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd1;
	m_pAttribSelGrid->m_InfoTypeflag = TRUE;
	m_pAttribSelGrid->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);
#ifdef _OLD_
	m_pAttribSelGrid->SetDefaultValue("1024 K", "1", "3 sec", "64 k", "", "", ""); 
#else
	m_pAttribSelGrid->SetDefaultValue("1000 K", "10", "1 sec", "128 k", "", "", ""); 
#endif
	m_pAttribSelGrid->SetUntitledPrefix("");


	m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE, "");
	m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	::GetPrivateProfileString("Profile", "Name", "", szBuf, 512, m_strIniPath);
	if (szBuf[0] != '\0')
	{
		m_pAttribSelGrid->SelectRow(szBuf);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(szBuf);
	}
	GetDlgItem(IDC_STATIC_GRID11)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid1 = new CDataBrowseWnd1;
	m_pAttribSelGrid1->m_InfoTypeflag = FALSE;
	m_pAttribSelGrid1->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid1->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);

	m_pAttribSelGrid1->SetDefaultValue("1024 K", "1", "3 sec", "32 k", "", "", ""); 
	m_pAttribSelGrid1->SetUntitledPrefix("");

	m_pAttribSelGrid1->SetProfileData(MACRO_MSAUDIO_FILE_S, "");
	m_pAttribSelGrid1->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, m_strIniPath);
	if (szBuf[0] != '\0')
	{
		m_pAttribSelGrid1->SelectRow(szBuf);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->SetWindowText(szBuf);
	}
	GetDlgItem(IDC_STATIC_GRID10)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_GRID11)->GetWindowRect(rect1);
	ScreenToClient(rect);
	ScreenToClient(rect1);
	rect.top -= 20;
	rect.right += 20;
	rect.left -= 20;
	rect.bottom += 20;
	rect1.top -= 20;
	rect1.right += 20;
	rect1.left -= 20;
	rect1.bottom += 20;

	CString m_Camera;
	m_Camera = "";
	if ( pCamVideoComboindex >= 0)
	{
		HRESULT hval = 0;
		hval = m_pEasyCapture->Open(0);
		if((hval != S_OK)) 
		{
			MessageBox("Error Open: EasyCapture Open Fail");
			return FALSE;
		}
		::EnumDeviceCamera(m_pEasyCapture, pCamVideoComboindex, m_Camera);
	} 
	m_pAttribSelGrid->m_pGridDetail->SetSeqAt(cSeqTextCombo, 1, m_Camera);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDevice::OnDestroy() 
{
	CDialog::OnDestroy();

	m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;

	m_pAttribSelGrid1->DestroyWindow();
	delete m_pAttribSelGrid1;

	if (m_pEasyCapture != NULL) 
	{
		m_pEasyCapture->Close();
		RELEASE(m_pEasyCapture);
	}
}

void CDevice::OnOK() 
{
	CComboBox* pAudio = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	CComboBox* pCamVideo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	CComboBox* pScAudio = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);

	CComboBox* pAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_FRAME);
	CComboBox* pCamVideoFormat = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
	CComboBox* pScAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO_FRAME);
	CComboBox* pScMouse = (CComboBox*)GetDlgItem(IDC_COMBO_MOUSE_RECORD);

	CButton* pRadioNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pRadioCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	int nCheckNoVideo = pRadioNoVideo->GetCheck();
	int nCheckCam = pRadioCam->GetCheck();

	CString strAudio, strCamVideo, strScAudio;
	pAudio->GetWindowText(strAudio);
	pCamVideo->GetWindowText(strCamVideo);
	pScAudio->GetWindowText(strScAudio);

	CString strAudioFormat, strCamVideoFormat, strScAudioFormat, strMouse;
	pAudioFormat->GetWindowText(strAudioFormat);
	pCamVideoFormat->GetWindowText(strCamVideoFormat);
	pScAudioFormat->GetWindowText(strScAudioFormat);
	pScMouse->GetWindowText(strMouse);

	if (strAudioFormat == "" || 
		strScAudioFormat == "" || 
		strMouse == "" || 
		(nCheckCam && strCamVideoFormat == "")) 
	{
		W_SHOW_ERROR(IDS_ANY_DEVICE_CANNOT_EMPTY, this);
		return;
	}

	CString strSel;
	if (m_strIniPath != "")
	{
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
		if (strSel == "")
		{
			W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
			return ;
		}

		m_pAttribSelGrid->SaveData();
		::WritePrivateProfileString("Profile", "Name", strSel, m_strIniPath);
		CStringArray strArraySelected;
		m_pAttribSelGrid->GetSelectedData(strArraySelected);
		CString strEntry;
		int i = 1;
		for (i = 1; i < strArraySelected.GetSize(); i++)
		{
			strEntry.Format("Param%d", i);
			::WritePrivateProfileString("Profile", strEntry, strArraySelected[i], m_strIniPath);
		}
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("Profile", strEntry, strAudioFormat, m_strIniPath);
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("Profile", strEntry, strCamVideoFormat, m_strIniPath);
		if (nCheckNoVideo)
		{
			::WritePrivateProfileString("Profile", "Param1", "0", m_strIniPath);
			::WritePrivateProfileString("Profile", "Param2", "5", m_strIniPath);
			::WritePrivateProfileString("Profile", "Param6", "320x240 YUY2, 16 bit", m_strIniPath);
		}
	
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->GetWindowText(strSel);
		if (strSel == "")
		{
			W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
			return ;
		}
		m_pAttribSelGrid1->SaveData();
		::WritePrivateProfileString("ScreenProfile", "Name", strSel, m_strIniPath);

		CStringArray strArraySelected1;
		m_pAttribSelGrid1->GetSelectedData(strArraySelected1);
		for (i = 1; i < strArraySelected1.GetSize(); i++)
		{
			strEntry.Format("Param%d", i);
			::WritePrivateProfileString("ScreenProfile", strEntry, strArraySelected1[i], m_strIniPath);
		}
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("ScreenProfile", strEntry, strScAudioFormat, m_strIniPath);
		CString tempMouseEnable = "";
		tempMouseEnable.LoadString(IDS_MOUSE_CURSOR_ENABLE);
		CString tempMouseDisable = "";
		tempMouseDisable.LoadString(IDS_MOUSE_CURSOR_DISABLE);
		strEntry.Format("Param%d", i++);
		if ( strMouse == tempMouseEnable )
		{
			::WritePrivateProfileString("ScreenProfile", strEntry, "true", m_strIniPath);
		} else if ( strMouse == tempMouseDisable )
		{
			::WritePrivateProfileString("ScreenProfile", strEntry, "false", m_strIniPath);
		} else {
			::WritePrivateProfileString("ScreenProfile", strEntry, "", m_strIniPath);
		}
	}

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

	if (m_strIniPath == strIniPath)
	{
		strIniPath = MACRO_INI_FILE(MACRO_MAINFRM->GetCourse());
	}
	if (::FileExists(strIniPath))
	{
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
		if (strSel == "")
		{
			W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
			return ;
		}

		m_pAttribSelGrid->SaveData();
		::WritePrivateProfileString("Profile", "Name", strSel, strIniPath);
		CStringArray strArraySelected;
		m_pAttribSelGrid->GetSelectedData(strArraySelected);
		CString strEntry;
		int i = 1;
		for (i = 1; i < strArraySelected.GetSize(); i++)
		{
			strEntry.Format("Param%d", i);
			::WritePrivateProfileString("Profile", strEntry, strArraySelected[i], strIniPath);
		}
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("Profile", strEntry, strAudioFormat, strIniPath);
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("Profile", strEntry, strCamVideoFormat, strIniPath);
		if (nCheckNoVideo)
		{
			::WritePrivateProfileString("Profile", "Param1", "0", strIniPath);
			::WritePrivateProfileString("Profile", "Param2", "5", strIniPath);
			::WritePrivateProfileString("Profile", "Param6", "320x240 YUY2, 16 bit", strIniPath);
		}
	
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->GetWindowText(strSel);
		if (strSel == "")
		{
			W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
			return ;
		}
		m_pAttribSelGrid1->SaveData();
		::WritePrivateProfileString("ScreenProfile", "Name", strSel, strIniPath);

		CStringArray strArraySelected1;
		m_pAttribSelGrid1->GetSelectedData(strArraySelected1);
		for (i = 1; i < strArraySelected1.GetSize(); i++)
		{
			strEntry.Format("Param%d", i);
			::WritePrivateProfileString("ScreenProfile", strEntry, strArraySelected1[i], strIniPath);
		}
		strEntry.Format("Param%d", i++);
		::WritePrivateProfileString("ScreenProfile", strEntry, strScAudioFormat, strIniPath);
		CString tempMouseEnable = "";
		tempMouseEnable.LoadString(IDS_MOUSE_CURSOR_ENABLE);
		CString tempMouseDisable = "";
		tempMouseDisable.LoadString(IDS_MOUSE_CURSOR_DISABLE);
		strEntry.Format("Param%d", i++);
		if ( strMouse == tempMouseEnable )
		{
			::WritePrivateProfileString("ScreenProfile", strEntry, "true", strIniPath);
		} else if ( strMouse == tempMouseDisable )
		{
			::WritePrivateProfileString("ScreenProfile", strEntry, "false", strIniPath);
		} else {
			::WritePrivateProfileString("ScreenProfile", strEntry, "", strIniPath);
		}
	}

	MACRO_MAINFRM->m_nAuFixValueC = m_pAttribSelGrid->GetAuFixValue();
	MACRO_MAINFRM->m_nAuFixValueS = m_pAttribSelGrid1->GetAuFixValue();

	CDialog::OnOK();
}

void CDevice::OnBnClickedButtonRename()
{
	// TODO: Add your control notification handler code here
	if (m_pAttribSelGrid->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_RENAME_FIRST, this);
		return;
	}

	CString strOldName;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strOldName);
	if (strOldName.GetLength() > 2)
	{
		if (strOldName[0] == '<' && strOldName.Right(1) == ">")
		{
			W_SHOW_WARNING(IDS_SYSTEM_DEFAULT_NOT_REN, this);
			return;
		}
	}

	CDlgInputProfileName dlg;
	dlg.m_strProfileName = strOldName;

	CStringArray strArrayProfile;
	m_pAttribSelGrid->GetProfileName(strArrayProfile);
	dlg.m_pstrArrayProfile = &strArrayProfile;
	if (dlg.DoModal() == IDOK)
	{
		CString strProfileName = dlg.m_strProfileName;
		m_pAttribSelGrid->ReplaceSelected(strProfileName);

		char szBuf[256];
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strOldName)
		{
			::WritePrivateProfileString("Profile", "Name", strProfileName, m_strIniPath);
		}
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE) ->SetWindowText(strProfileName);
	}
}


void CDevice::OnClickedButtonRename1()
{
	// TODO: Add your control notification handler code here
	if (m_pAttribSelGrid1->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_RENAME_FIRST, this);
		return;
	}

	CString strOldName;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->GetWindowText(strOldName);
	if (strOldName.GetLength() > 2)
	{
		if (strOldName[0] == '<' && strOldName.Right(1) == ">")
		{
			W_SHOW_WARNING(IDS_SYSTEM_DEFAULT_NOT_REN, this);
			return;
		}
	}

	CDlgInputProfileName dlg;
	dlg.m_strProfileName = strOldName;

	CStringArray strArrayProfile;
	m_pAttribSelGrid1->GetProfileName(strArrayProfile);
	dlg.m_pstrArrayProfile = &strArrayProfile;
	if (dlg.DoModal() == IDOK)
	{
		CString strProfileName = dlg.m_strProfileName;
		m_pAttribSelGrid1->ReplaceSelected(strProfileName);

		char szBuf[256];
		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strOldName)
		{
			::WritePrivateProfileString("ScreenProfile", "Name", strProfileName, m_strIniPath);
		}
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1) -> SetWindowText(strProfileName);
	}
}

void CDevice::OnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	if (m_pAttribSelGrid->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_DEL_FIRST, this);
		return;
	}

	CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	if (strSel.GetLength() > 2)
	{
		if (strSel[0] == '<' && strSel.Right(1) == ">")
		{
			W_SHOW_WARNING(IDS_SYSTEM_DEFAULT_NOT_DEL, this);
			return;
		}
	}

	CString strTmp, strMsg;
	strTmp.LoadString(IDS_SURE_DEL_PROFILE);
	strMsg.Format(strTmp, strSel);
	if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
	{
		m_pAttribSelGrid->DeleteSelectedRow();

		char szBuf[256];
		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strSel)
			::WritePrivateProfileString("ScreenProfile", "Name", "", m_strIniPath);
	}
}


void CDevice::OnClickedButtonDel1()
{
	// TODO: Add your control notification handler code here
	if (m_pAttribSelGrid1->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_DEL_FIRST, this);
		return;
	}

	CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->GetWindowText(strSel);
	if (strSel.GetLength() > 2)
	{
		if (strSel[0] == '<' && strSel.Right(1) == ">")
		{
			W_SHOW_WARNING(IDS_SYSTEM_DEFAULT_NOT_DEL, this);
			return;
		}
	}

	CString strTmp, strMsg;
	strTmp.LoadString(IDS_SURE_DEL_PROFILE);
	strMsg.Format(strTmp, strSel);
	if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
	{
		m_pAttribSelGrid1->DeleteSelectedRow();

		char szBuf[256];
		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strSel)
			::WritePrivateProfileString("ScreenProfile", "Name", "", m_strIniPath);
	}
}
LRESULT CDevice::OnRowSelected(WPARAM wParam, LPARAM lParam)
{
	CString pStr = (LPCTSTR)lParam;
	int cid = atoi(pStr.Right(20));
	if (cid == m_pAttribSelGrid->GetDlgCtrlID())//rect.PtInRect(mousepoint) != 0)
	{
		if ( (int)wParam < 0)
		{
			GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
			return 0L;
		}
		pStr = pStr.Left(pStr.GetLength() - 20);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(pStr);
	}
	else if (cid == m_pAttribSelGrid1->GetDlgCtrlID())//rect1.PtInRect(mousepoint) != 0)
	{
		if ( (int)wParam < 0)
		{
			GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->SetWindowText("");
			return 0L;
		}
		pStr = pStr.Left(pStr.GetLength() - 20);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->SetWindowText(pStr);
	}

	return 0L;
}


void CDevice::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousepoint.x = point.x;
	mousepoint.y = point.y;
	CDialog::OnMouseMove(nFlags, point);
}



BOOL CDevice::ReloadDev() 
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

	m_bWithCam = strArrayWebVideo.GetSize();
	m_bWithAudio = strArrayWebAudio.GetSize();
	//add buxiangyi
	CStringArray strArrayWebAudioFrame, strArrayWebVideoFrame;
	if (m_bWithAudio > 0 && m_Initialization)
	{
		CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
		::EnumDeviceFrame(m_pEasyCapture, eType, 0,strArrayWebAudioFrame);
	}
	
	if (m_bWithCam > 0 && m_Initialization)
	{
		eType = CAPTUREDEVICETYPE_VIDEO;
		::EnumDeviceFrame(m_pEasyCapture, eType, 0, strArrayWebVideoFrame);
	}
	
	for (int i = 0; i < strArrayWebAudioFrame.GetSize(); i++)
	{
		if (strArrayWebAudioFrame.GetAt(i).Find("16 Bits") < 0 
			|| (strArrayWebAudioFrame.GetAt(i).Find("1 Channels") < 0 && strArrayWebAudioFrame.GetAt(i).Find("2 Channels") < 0) 
			|| (strArrayWebAudioFrame.GetAt(i).Find("44100 Hz") < 0 && strArrayWebAudioFrame.GetAt(i).Find("48000 Hz") < 0))
		{
			strArrayWebAudioFrame.RemoveAt(i);
			i--;
		}
	}

	for (int i = 0; i < strArrayWebVideoFrame.GetSize(); i++)
	{
		CString strOneCamFrame = strArrayWebVideoFrame.GetAt(i);
		strOneCamFrame = strOneCamFrame.Left(strOneCamFrame.Find(" "));
		int nWidth = atoi(strOneCamFrame.Left(strOneCamFrame.Find("x")));
		int nHeight = atoi(strOneCamFrame.Right(strOneCamFrame.Find("x")));
		if (nWidth <= 300 || nHeight <= 300 || nWidth >= 1920 || nHeight >= 1920 
			|| nWidth % 16 != 0 || nHeight % 16 != 0)
		{
			strArrayWebVideoFrame.RemoveAt(i);
			i--;
		}
	}

	SetComboContent(IDC_COMBO_CAM_VIDEO_FRAME, strArrayWebVideoFrame); 
	SetComboContent(IDC_COMBO_AUDIO_FRAME, strArrayWebAudioFrame); 
	SetComboContent(IDC_COMBO_SCREEN_AUDIO_FRAME, strArrayWebAudioFrame); 

	////////////////////////////////////////////////////////////////////////////////

	int nIndex;
	if ((nIndex = pCamVideoCombo->FindString(-1, strCam)) >= 0)
		pCamVideoCombo->SetCurSel(nIndex);
	if ((nIndex = pAudioCombo->FindString(-1, strAudio)) >= 0)
		pAudioCombo->SetCurSel(nIndex);
	if ((nIndex = pScAudioCombo->FindString(-1, strDemoAudio)) >= 0)
		pScAudioCombo->SetCurSel(nIndex);

	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);

	CComboBox* pAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_FRAME);
	CComboBox* pCamVideoFormat = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
	CComboBox* pScAudioFormat = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO_FRAME);
	

	if (m_bWithCam)
	{
		pButtonCam->EnableWindow(TRUE);
		if (pButtonCam->GetCheck())
		{
			pCamVideoCombo->EnableWindow(TRUE);
			pCamVideoFormat ->EnableWindow(TRUE);
		}
	}
	else
	{
		pButtonCam->EnableWindow(FALSE);
		pCamVideoCombo->EnableWindow(FALSE);
		pCamVideoFormat ->EnableWindow(FALSE);
	}

	if (m_bWithAudio)
	{
		pAudioCombo->EnableWindow(TRUE);
		pScAudioCombo->EnableWindow(TRUE);
		pAudioFormat->EnableWindow(TRUE);
		pScAudioFormat->EnableWindow(TRUE);
	}
	else
	{
		pAudioCombo->EnableWindow(FALSE);
		pScAudioCombo->EnableWindow(FALSE);
		pAudioFormat->EnableWindow(FALSE);
		pScAudioFormat->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDevice::OnRadioNoVideo() 
{
	CButton* pButtonIP = (CButton*)GetDlgItem(IDC_RADIO_IP);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	if (pButtonNoVideo->GetCheck())
	{
		pButtonCam->SetCheck(0);
		pButtonIP ->SetCheck(0);
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
		pCamVideoCombo->EnableWindow(FALSE);
		CComboBox* pCamVideoComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
		pCamVideoComboFrame->EnableWindow(FALSE);

		CEdit* pCEditURL = (CEdit*)GetDlgItem(IDC_EDIT_URL); 
		pCEditURL ->EnableWindow(FALSE);
		CEdit* pCEditIP = (CEdit*)GetDlgItem(IDC_EDIT_IP);
		pCEditIP ->EnableWindow(FALSE);
		CEdit* pCEditPassword = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
		pCEditPassword ->EnableWindow(FALSE);
		CButton* pIpVoice = (CButton*)GetDlgItem(IDC_CHECK_IPVOICE);
		pIpVoice->EnableWindow(FALSE);
	}
}


void CDevice::OnClickedButtonReload()
{
	// TODO: Add your control notification handler code here
	HRESULT hval = 0;
	hval = m_pEasyCapture->Open(0);
	if((hval != S_OK)) 
	{
		MessageBox("Error Open: EasyCapture Open Fail");
		return ;
	}

	ReloadDev();
}
void CDevice::SetComboContent(UINT nID, CStringArray& strArray) 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->ResetContent();
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);
}

void CDevice::OnRadioCam() 
{
	RadioCam(); 
}

void CDevice::RadioCam() 
{
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);

	CButton* pButtonIP = (CButton*)GetDlgItem(IDC_RADIO_IP);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	if (pButtonCam->GetCheck())
	{
		pButtonIP->SetCheck(0);
		pButtonNoVideo->SetCheck(0);
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
		CComboBox* pCamVideoComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);

		CEdit* pCEditURL = (CEdit*)GetDlgItem(IDC_EDIT_URL); 
		pCEditURL ->EnableWindow(FALSE);
		CEdit* pCEditIP = (CEdit*)GetDlgItem(IDC_EDIT_IP);
		pCEditIP ->EnableWindow(FALSE);
		CEdit* pCEditPassword = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
		pCEditPassword ->EnableWindow(FALSE);
		CButton* pIpVoice = (CButton*)GetDlgItem(IDC_CHECK_IPVOICE);
		pIpVoice->EnableWindow(FALSE);
		if (m_bWithCam)
		{
			pCamVideoCombo->EnableWindow(TRUE);
			pCamVideoComboFrame->EnableWindow(TRUE);
		}
			
	}
}

void CDevice::OnSelchangeComboAudio() 
{
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO);
	int nIndex =pAudioCombo -> GetCurSel();
	CStringArray strArrayWebAudioFrame;
	
	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	char buf[256]={0};
	::FindDeviceFrameValid(eType, nIndex,buf,strArrayWebAudioFrame);
	for (int i = 0; i < strArrayWebAudioFrame.GetSize(); i++)
	{
		if (strArrayWebAudioFrame.GetAt(i).Find("16 Bits") < 0 
			|| (strArrayWebAudioFrame.GetAt(i).Find("1 Channels") < 0 && strArrayWebAudioFrame.GetAt(i).Find("2 Channels") < 0) 
			|| (strArrayWebAudioFrame.GetAt(i).Find("44100 Hz") < 0 && strArrayWebAudioFrame.GetAt(i).Find("48000 Hz") < 0))
		{
			strArrayWebAudioFrame.RemoveAt(i);
			i--;
		}
	}
	SetComboContent(IDC_COMBO_AUDIO_FRAME, strArrayWebAudioFrame); 
}

void CDevice::OnSelchangeComboCamVideo() 
{
	
	CComboBox* pVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
	int nIndex =pVideoCombo -> GetCurSel();
	CStringArray strArrayWebVideoFrame;

	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_VIDEO;
	char buf[256]={0};
	::FindDeviceFrameValid(eType, nIndex,buf,strArrayWebVideoFrame);
	for (int i = 0; i < strArrayWebVideoFrame.GetSize(); i++)
	{
		CString strOneCamFrame = strArrayWebVideoFrame.GetAt(i);
		strOneCamFrame = strOneCamFrame.Left(strOneCamFrame.Find(" "));
		int nWidth = atoi(strOneCamFrame.Left(strOneCamFrame.Find("x")));
		int nHeight = atoi(strOneCamFrame.Right(strOneCamFrame.Find("x")));
		if (nWidth <= 300 || nHeight <= 300 || nWidth >= 1920 || nHeight >= 1920 
			|| nWidth % 16 != 0 || nHeight % 16 != 0)
		{
			strArrayWebVideoFrame.RemoveAt(i);
			i--;
		}
	}
	SetComboContent(IDC_COMBO_CAM_VIDEO_FRAME, strArrayWebVideoFrame); 

	CString m_Camera;
	m_Camera = "";
	if (nIndex >=0)
	{
		HRESULT hval = 0;
		hval = m_pEasyCapture->Open(0);
		if((hval != S_OK)) 
		{
			MessageBox("Error Open: EasyCapture Open Fail");
			return ;
		}
		::EnumDeviceCamera(m_pEasyCapture, nIndex, m_Camera);
	} 
	m_pAttribSelGrid->m_pGridDetail->SetSeqAt(cSeqTextCombo, 1, m_Camera);
}

void CDevice::OnSelchangeComboScreenAudio() 
{
	CComboBox* pAudioCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO);
	int nIndex =pAudioCombo -> GetCurSel();
	CStringArray strArrayWebAudioFrame;

	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	char buf[256]={0};
	::FindDeviceFrameValid(eType, nIndex,buf,strArrayWebAudioFrame);
	for (int i = 0; i < strArrayWebAudioFrame.GetSize(); i++)
	{
		if (strArrayWebAudioFrame.GetAt(i).Find("16 Bits") < 0 
			|| (strArrayWebAudioFrame.GetAt(i).Find("1 Channels") < 0 && strArrayWebAudioFrame.GetAt(i).Find("2 Channels") < 0) 
			|| (strArrayWebAudioFrame.GetAt(i).Find("44100 Hz") < 0 && strArrayWebAudioFrame.GetAt(i).Find("48000 Hz") < 0))
		{
			strArrayWebAudioFrame.RemoveAt(i);
			i--;
		}
	}
	SetComboContent(IDC_COMBO_SCREEN_AUDIO_FRAME, strArrayWebAudioFrame); 
}


void CDevice:: OnSelchangeComboAudioFrame()
{
	CComboBox* pAudioComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_FRAME);
	int nIndex =pAudioComboFrame -> GetCurSel();
}
void CDevice:: OnSelchangeComboCamVideoFrame()
{
	CComboBox* pVideoComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
	int nIndex =pVideoComboFrame -> GetCurSel();
}
void CDevice:: OnSelchangeComboScreenAudioFrame()
{
	CComboBox* pAudioComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_SCREEN_AUDIO_FRAME);
	int nIndex =pAudioComboFrame -> GetCurSel();
}
void CDevice::OnRadioIP()
{
	CButton* pButtonIP = (CButton*)GetDlgItem(IDC_RADIO_IP);
	CButton* pButtonNoVideo = (CButton*)GetDlgItem(IDC_RADIO_NO_VIDEO);
	CButton* pButtonCam = (CButton*)GetDlgItem(IDC_RADIO_CAM);
	if (pButtonIP->GetCheck())
	{
		pButtonCam ->SetCheck(0);
		pButtonNoVideo ->SetCheck(0);
		CEdit* pCEditURL = (CEdit*)GetDlgItem(IDC_EDIT_URL); 
		pCEditURL ->EnableWindow(TRUE);
		CEdit* pCEditIP = (CEdit*)GetDlgItem(IDC_EDIT_IP);
		pCEditIP ->EnableWindow(TRUE);
		CEdit* pCEditPassword = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
		pCEditPassword ->EnableWindow(TRUE);
		CButton* pIpVoice = (CButton*)GetDlgItem(IDC_CHECK_IPVOICE);
		pIpVoice->EnableWindow(TRUE);
		CComboBox* pCamVideoCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO);
		pCamVideoCombo->EnableWindow(FALSE);
		CComboBox* pCamVideoComboFrame = (CComboBox*)GetDlgItem(IDC_COMBO_CAM_VIDEO_FRAME);
		pCamVideoComboFrame->EnableWindow(FALSE);
	}

}

void CDevice::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE1)->SetWindowText("");

	CDialog::OnCancel();
}