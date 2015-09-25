// DlgMP4Info.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgMP4Info.h"
#include "macro.h"
#include "mainfrm.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Info dialog


CDlgMP4Info::CDlgMP4Info(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMP4Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMP4Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMP4Info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMP4Info)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMP4Info, CDialog)
	//{{AFX_MSG_MAP(CDlgMP4Info)
	ON_BN_CLICKED(IDC_RADIO_SCREEN, OnRadioScreen)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, OnRadioDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Info message handlers

BOOL CDlgMP4Info::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CListBox* pListVideo = (CListBox*)GetDlgItem(IDC_LIST_VIDEO);
	CListBox* pListAudio = (CListBox*)GetDlgItem(IDC_LIST_AUDIO);
	pListVideo->SetTabStops(48);
	pListAudio->SetTabStops(48);
	CString strTmp;

	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString strProfile = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
	CString strScreenProfile = MACRO_COURSE_PATH + strCourse + "\\video\\screen.prx";
	GetProfileData(strProfile);

	if (!::FileExists(strProfile))
		GetDlgItem(IDC_RADIO_DEFAULT)->EnableWindow(FALSE);
	else
		((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(1);

	if (!::FileExists(strScreenProfile))
		GetDlgItem(IDC_RADIO_SCREEN)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMP4Info::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgMP4Info::GetProfileData(CString strProfile)
{
	if (!::FileExists(strProfile))
		return;

	PROFILEPARAMS param;
	MACRO_MAINFRM->GetPRXParams(strProfile, param);

 	CStringArray strArrayVideo, strArrayAudio;

	CString strTmp;
	CString strProp;
	strProp.Format("CODEC:\t%s", MACRO_MAINFRM->GetProfileFromID(param.vinfo.dwFourCC));
	strArrayVideo.Add(strProp);

	strTmp.LoadString(IDS_SIZE);
	strProp.Format("%s:\t%d x %d", strTmp, param.vinfo.dwWidth, param.vinfo.dwHeight);
	strArrayVideo.Add(strProp);

	strTmp.LoadString(IDS_VIDEO_BANDWIDTH);
	strProp.Format("%s:\t%d", strTmp, param.vinfo.dwBitrate);
	strArrayVideo.Add(strProp);

	strTmp.LoadString(IDS_VIDEO_FPS);
	strProp.Format("%s:\t%d", strTmp, param.vinfo.dwFPS);
	strArrayVideo.Add(strProp);

	strTmp.LoadString(IDS_VIDEO_KEY_PER_SEC);
	strProp.Format("%s:\t%d", strTmp, param.vinfo.dwSecPerKey);
	strArrayVideo.Add(strProp);

	strTmp.LoadString(IDS_VIDEO_QUALITY);
	strProp.Format("%s:\t%d", strTmp, param.vinfo.dwQuality);
	strArrayVideo.Add(strProp);

	strProp.Format("CODEC:\t%s", MACRO_MAINFRM->GetProfileFromID(param.ainfo.dwFormatTag));
	strArrayAudio.Add(strProp);

	strTmp.LoadString(IDS_AUDIO_BANDWIDTH);
	strProp.Format("%s:\t%d", strTmp, param.ainfo.dwBitrate);
	strArrayAudio.Add(strProp);

	strTmp.LoadString(IDS_AUDIO_SAMPLE_RATE);
	strProp.Format("%s:\t%d", strTmp, param.ainfo.dwSampleRate);
	strArrayAudio.Add(strProp);

	CString strMono, strStereo, strChannel;
	strMono.LoadString(IDS_MONO);
	strStereo.LoadString(IDS_STEREO);
	if (param.ainfo.dwChannels == 1)
		strChannel = strMono;
	else
		strChannel = strStereo;

	strProp.Format("%s/%s:\t%s", strMono, strStereo, strChannel);
	strArrayAudio.Add(strProp);

	CListBox* pListVideo = (CListBox*)GetDlgItem(IDC_LIST_VIDEO);
	CListBox* pListAudio = (CListBox*)GetDlgItem(IDC_LIST_AUDIO);
	pListVideo->ResetContent();
	pListAudio->ResetContent();
	int i = 0;
	for (i = 0; i < strArrayVideo.GetSize(); i++)
	{
		strTmp = strArrayVideo[i];
		pListVideo->AddString(strTmp);
	}
	
	for (i = 0; i < strArrayAudio.GetSize(); i++)
	{
		strTmp = strArrayAudio[i];
		pListAudio->AddString(strTmp);
	}
}

void CDlgMP4Info::OnRadioScreen() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString strScreenProfile = MACRO_COURSE_PATH + strCourse + "\\video\\screen.prx";
	GetProfileData(strScreenProfile);
}

void CDlgMP4Info::OnRadioDefault() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString strProfile = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
	GetProfileData(strProfile);
}
