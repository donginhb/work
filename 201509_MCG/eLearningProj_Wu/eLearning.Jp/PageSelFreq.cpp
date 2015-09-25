// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelFreq.h"
#include "NewWizDialog.h"
#include "DlgDefaultProfileSet.h"
#include "cmkapi.h"
#include "eLearningView.h"
#include "macro.h"
#include "Mainfrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelFreq dialog


CPageSelFreq::CPageSelFreq(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelFreq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelFreq)
	//}}AFX_DATA_INIT
}


void CPageSelFreq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelFreq)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelFreq, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelFreq)
	ON_BN_CLICKED(IDC_RADIO_OTHER, OnRadioOther)
	ON_BN_CLICKED(IDC_RADIO_FREQ1, OnRadioFreq1)
	ON_BN_CLICKED(IDC_RADIO_FREQ2, OnRadioFreq2)
	ON_BN_CLICKED(IDC_RADIO_FREQ3, OnRadioFreq3)
	ON_BN_CLICKED(IDC_RADIO_FREQ4, OnRadioFreq4)
	ON_BN_CLICKED(IDC_RADIO_FREQ5, OnRadioFreq5)
	ON_BN_CLICKED(IDC_RADIO_FREQ6, OnRadioFreq6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelFreq message handlers

BOOL CPageSelFreq::OnInitDialog() 
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

    ((CButton*)GetDlgItem(IDC_RADIO_FREQ4))->SetCheck(1);
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);

	MACRO_VIEW->SaveProfileParamsToLocal(FALSE);

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageSelFreq::OnWizardBack()
{
	m_strData = "";
	m_strData2 = "";
	return 0;
}

LRESULT CPageSelFreq::OnWizardNext()
{
    CString str;
	GetDlgItem(IDC_EDIT_OTHER)->GetWindowText(str);
	
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_OTHER);
	if (pRadio->GetCheck() && str == "")
	{
		W_SHOW_ERROR(IDS_SEL_FREQ_FIRST, this);
		return -1;
	}

	CButton* pRadio1 = (CButton*)GetDlgItem(IDC_RADIO_FREQ1);
	CButton* pRadio2 = (CButton*)GetDlgItem(IDC_RADIO_FREQ2);
	CButton* pRadio3 = (CButton*)GetDlgItem(IDC_RADIO_FREQ3);
	CButton* pRadio4 = (CButton*)GetDlgItem(IDC_RADIO_FREQ4);
	CButton* pRadio5 = (CButton*)GetDlgItem(IDC_RADIO_FREQ5);
	CButton* pRadio6 = (CButton*)GetDlgItem(IDC_RADIO_FREQ6);
	CString strDimension, strName;
	CString strParam[7]; 
    if (pRadio1->GetCheck())
	{
		m_strData = "0";
		strDimension = "640 x 480";
		strName = "<DVD Quality>";
		strParam[0] = "1890";
		strParam[1] = "20";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "160";
		strParam[5] = "44100";
		strParam[6] = "S";
	}
	else if (pRadio2->GetCheck())
	{
		m_strData = "1";
		strDimension = "480 x 360";
		strName = "<SVCD Quality>";
		strParam[0] = "890";
		strParam[1] = "20";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "96";
		strParam[5] = "44100";
		strParam[6] = "S";
	}
	else if (pRadio3->GetCheck())
	{
		m_strData = "2";
		strDimension = "320 x 240";
		strName = "<VCD Quality>";
		strParam[0] = "340";
		strParam[1] = "20";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "64";
		strParam[5] = "44100";
		strParam[6] = "S";
	}
	else if (pRadio4->GetCheck())
	{
		m_strData = "3";
		strDimension = "240 x 180";
		strName = "<ADSL Quality>";
		strParam[0] = "100";
		strParam[1] = "15";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "22";
		strParam[5] = "22050";
		strParam[6] = "S";
	}
	else if (pRadio5->GetCheck())
	{
		m_strData = "4";
		strDimension = "200 x 150";
		strName = "<IDSN Quality>";
		strParam[0] = "40";
		strParam[1] = "10";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "16";
		strParam[5] = "16000";
		strParam[6] = "M";
	}
	else if (pRadio6->GetCheck())
	{
		m_strData = "5";
		strDimension = "200 x 150";
		strName = "<Audio Only>";
		strParam[0] = "0";
		strParam[1] = "20";
		strParam[2] = "1";
		strParam[3] = "0";
		strParam[4] = "22";
		strParam[5] = "22050";
		strParam[6] = "S";
	}
	else if (pRadio->GetCheck())
	{
		m_strData = "6";
		m_strData2 = str;
	}

	if (m_strData != "6")
	{
		CString strTempPath =  ::GetSysPath() + "temp";
		if (!::FileExists(strTempPath))
			::TryToMkDir(strTempPath);
		CString strIniPath = strTempPath + "\\temp_info.ini";

		::WritePrivateProfileString("Profile", "Dimension", strDimension, strIniPath);
		
		::WritePrivateProfileString("Profile", "Name", strName, strIniPath);

		for (int i = 0; i < 7; i++)
		{
			CString strPara;
			strPara.Format("Param%d", i + 1);
			::WritePrivateProfileString("Profile", strPara, strParam[i], strIniPath);
		}
	}

	return 0;
}

void CPageSelFreq::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_FREQ);
	m_pParent->SetTitle(strTitle);

	if (m_pstrArray2->GetAt(4) == "0")
	{
		GetDlgItem(IDC_RADIO_FREQ1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREQ2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREQ3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREQ4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREQ5)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREQ6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_OTHER)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ2))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ3))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ4))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ5))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_FREQ6))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_OTHER))->SetCheck(0);
	}
	else
	{
		GetDlgItem(IDC_RADIO_FREQ1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREQ2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREQ3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREQ4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREQ5)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREQ6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_OTHER)->EnableWindow(TRUE);
	}
}

BOOL CPageSelFreq::OnKillActive()
{
	return TRUE;
}

void CPageSelFreq::OnRadioOther()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(TRUE);

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strTmpIniPath = strTempPath + "\\temp_info.ini";
	CDlgDefaultProfileSet dlg;
	dlg.m_strIniPath = strTmpIniPath;
	dlg.m_bNoVideo = (m_pstrArray2->GetAt(4) == "0");
	if (dlg.DoModal() == IDOK)
	{
		char szBuf[512];
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 512, strTmpIniPath);
		GetDlgItem(IDC_EDIT_OTHER)->SetWindowText(szBuf);
	}
}

void CPageSelFreq::OnRadioFreq1()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}

void CPageSelFreq::OnRadioFreq2()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}

void CPageSelFreq::OnRadioFreq3()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}

void CPageSelFreq::OnRadioFreq4()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}

void CPageSelFreq::OnRadioFreq5()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}

void CPageSelFreq::OnRadioFreq6()
{
    GetDlgItem(IDC_EDIT_OTHER)->EnableWindow(FALSE);
}
