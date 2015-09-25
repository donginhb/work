// DlgExportCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgExportCourse.h"
#include "cmkapi.h"
#include "BmpBtn.h"
#include "DataBrowseWnd.h"
#include "Macro.h"
#include <direct.h>
#include "..\_Shared\HelperKit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportCourse dialog


CDlgExportCourse::CDlgExportCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgExportCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportCourse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgExportCourse)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_RADIO_AUDIO_ONLY, OnRadioAudioOnly)
	ON_BN_CLICKED(IDC_RADIO_FREQ, OnRadioFreq)
	ON_BN_CLICKED(IDC_RADIO_ORIGINAL, OnRadioOriginal)
	ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, OnRadioDefault)
	ON_BN_CLICKED(IDC_RADIO_SCREEN, OnRadioScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportCourse message handlers

BOOL CDlgExportCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBtnOpen = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;

	CRect rcRight;
	GetDlgItem(IDC_STATIC_EXPORT)->GetWindowRect(rcRight);
	ScreenToClient(rcRight);

	CRect rcEdit;
	GetDlgItem(IDC_EDIT_DIR)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	
	CRect rect;
	rect.right = rcRight.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpen->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_DIR);

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd;
	m_pAttribSelGrid->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);

	((CButton*)GetDlgItem(IDC_RADIO_ORIGINAL))->SetCheck(1);
	OnRadioOriginal();

	CString strTmpPath = ::GetSysPath() + "temp\\";
	if (!::FileExists(strTmpPath))
		_mkdir(strTmpPath);
	
	m_strPathDefault = strTmpPath + "msaudio.lrn";
	::SafeCopyFile(MACRO_MSAUDIO_FILE, m_strPathDefault, FALSE);

	if (m_lScreenAudioCodecID == CODEC_AUDIO_ACELP)
	{
		m_strPathScreen = strTmpPath + "acelp.lrn";
		::SafeCopyFile(MACRO_ACELP_FILE, m_strPathScreen, FALSE);
	}
	else
		m_strPathScreen = strTmpPath + "msaudio.lrn";

	OnRadioDefault();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExportCourse::OnOK() 
{
	CString strFolder;
	GetDlgItem(IDC_EDIT_DIR)->GetWindowText(strFolder);
	strFolder.TrimRight();
	if (strFolder == "")
	{
		W_SHOW_ERROR(IDS_FOLDER_EMPTY, this);
		return;
	}

	if (!::FileExists(strFolder))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FOLDER_NOT_EXIST);
		strMsg.Format(strTmp, strFolder);
		if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
		{
			if (!::TryToMkDir(strFolder))
				return;
		}
		else
			return;
	}
	
	if (((CButton*)GetDlgItem(IDC_RADIO_FREQ))->GetCheck())
	{
		m_pAttribSelGrid->SaveData();

		CString strSel;
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
		if (strSel == "")
		{
			W_SHOW_ERROR(IDS_PROFILE_NOT_INPUT, this);
			return;
		}

		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strSel);
		if (m_lScreenAudioCodecID >= 0 && strSel == "")
		{
			W_SHOW_ERROR(IDS_SCREEN_NOT_INPUT, this);
			return;
		}
	}

	m_strFolder = strFolder;
	if (((CButton*)GetDlgItem(IDC_RADIO_ORIGINAL))->GetCheck())
		m_nType = 0;
	else if (((CButton*)GetDlgItem(IDC_RADIO_AUDIO_ONLY))->GetCheck())
		m_nType = 1;
	else if (((CButton*)GetDlgItem(IDC_RADIO_FREQ))->GetCheck())
	{
		m_nType = 2;

		CString strProfileName, strDemoProfileName;
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strProfileName);
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strDemoProfileName);
		
		CStringArray strArrayData;
		::LoadFile(m_strPathDefault, strArrayData);
		GetValue(strArrayData, strProfileName, m_nVideoBand, m_nFPS, m_nKeyFrame, 
			     m_nVideoQuality, m_nAudioBand, m_nAudioFreq, m_strMono);

		if (m_lScreenAudioCodecID >= 0)
		{
			::LoadFile(m_strPathScreen, strArrayData);
			GetValue(strArrayData, strProfileName, m_nDemoVideoBand, m_nDemoFPS, m_nDemoKeyFrame, 
					 m_nDemoVideoQuality, m_nDemoAudioBand, m_nDemoAudioFreq, m_strDemoMono);
		}
	}

	CDialog::OnOK();
}

BOOL CDlgExportCourse::GetValue(CStringArray& strArrayData, CString strProfileName, int& nVideoBand, 
								int& nFPS, int& nKeyFrame, int& nVideoQuality, int& nAudioBand, 
								int& nAudioFreq, CString& strMono)
{
	int nCount;
	CString strData, strTok;
	for (int i = 0; i < strArrayData.GetSize(); i++)
	{
		strData = strArrayData[i];
		nCount = 0;
		::GetTokStr(strData, strTok, nCount, "\t");
		if (strTok == strProfileName)
		{
			strData = strData.Mid(strTok.GetLength() + 1);
			nCount = 0;

			::GetTokStr(strData, strTok, nCount, ",");
			nVideoBand = atoi(strTok);

			::GetTokStr(strData, strTok, nCount, ",");
			nFPS = atoi(strTok);

			::GetTokStr(strData, strTok, nCount, ",");
			nKeyFrame = atoi(strTok);
			
			::GetTokStr(strData, strTok, nCount, ",");
			nVideoQuality = atoi(strTok);

			::GetTokStr(strData, strTok, nCount, ",");
			nAudioBand = atoi(strTok);
			
			::GetTokStr(strData, strTok, nCount, ",");
			nAudioFreq = atoi(strTok);

			::GetTokStr(strData, strTok, nCount, ",");
			strMono = strTok;

			return TRUE;
		}
	}

	return FALSE;
}

void CDlgExportCourse::SelDir()
{
	CString strLabel;
	strLabel.LoadString(IDS_SELECT_FOLDER);
	CString strFolderPath;
	CString strStartFolder;
	GetDlgItem(IDC_EDIT_DIR)->GetWindowText(strStartFolder);
	if (::GetFolder(&strFolderPath, strLabel, m_hWnd, NULL, strStartFolder))
		GetDlgItem(IDC_EDIT_DIR)->SetWindowText(strFolderPath);
}

void CDlgExportCourse::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpen->DestroyWindow();
	delete m_pBtnOpen;

	m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;
}

LRESULT CDlgExportCourse::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	if (nID == ID_BTN_OPEN_DIR)
		SelDir();

	return 0L;
}

void CDlgExportCourse::OnRadioAudioOnly() 
{
	GetDlgItem(IDC_RADIO_DEFAULT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_SCREEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->EnableWindow(FALSE);
	m_pAttribSelGrid->EnableWindow(FALSE);
}

void CDlgExportCourse::OnRadioFreq() 
{
	GetDlgItem(IDC_RADIO_DEFAULT)->EnableWindow(TRUE);
	if (m_lScreenAudioCodecID >= 0)
		GetDlgItem(IDC_RADIO_SCREEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->EnableWindow(TRUE);
	m_pAttribSelGrid->EnableWindow(TRUE);

	CButton* pRadioDef = (CButton*)GetDlgItem(IDC_RADIO_DEFAULT);
	CButton* pRadioScr = (CButton*)GetDlgItem(IDC_RADIO_SCREEN);
	if (!pRadioDef->GetCheck() && !pRadioScr->GetCheck())
		pRadioDef->SetCheck(1);
}

void CDlgExportCourse::OnRadioOriginal() 
{
	GetDlgItem(IDC_RADIO_DEFAULT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_SCREEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->EnableWindow(FALSE);
	m_pAttribSelGrid->EnableWindow(FALSE);
}

LRESULT CDlgExportCourse::OnRowSelected(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR pStr = (LPCTSTR)lParam;

	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_DEFAULT);
	if (pRadio->GetCheck())
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(pStr);
	else
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->SetWindowText(pStr);

	return 0L;
}

void CDlgExportCourse::OnRadioDefault() 
{
	CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	
	CWordArray wdArrayCol;
	CStringArray strArrayColValue;
	wdArrayCol.Add(3);
	strArrayColValue.Add("3");

	m_pAttribSelGrid->SetProfileData(m_strPathDefault, strSel, &wdArrayCol, &strArrayColValue);
	m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	CString str;
	str.LoadString(IDS_GENERAL_PROFILE);
	m_pAttribSelGrid->SetUntitledPrefix(str);
}

void CDlgExportCourse::OnRadioScreen() 
{
	CString strSel;
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strSel);
	
	CWordArray wdArrayCol;
	CStringArray strArrayColValue;
	wdArrayCol.Add(2);
	strArrayColValue.Add("2");
	wdArrayCol.Add(3);
	strArrayColValue.Add("10");
	m_pAttribSelGrid->SetProfileData(m_strPathScreen, strSel, &wdArrayCol, &strArrayColValue);
	
	if (m_lScreenAudioCodecID == CODEC_AUDIO_ACELP)
		m_pAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	else
		m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "2", "15 sec", "0", "16 K", "16000 Hz", strMono); 
	
	CString str;
	str.LoadString(IDS_SCREEN_PROFILE);
	m_pAttribSelGrid->SetUntitledPrefix(str);
}
