// DlgDefaultProfileSet.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgDefaultProfileSet.h"
#include "DataBrowseWnd.h"
#include "Macro.h"
#include "cmkapi.h"
#include "DlgInputProfileName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultProfileSet dialog


CDlgDefaultProfileSet::CDlgDefaultProfileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefaultProfileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDefaultProfileSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIniPath = MACRO_INI_PATH;
}


void CDlgDefaultProfileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefaultProfileSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDefaultProfileSet, CDialog)
	//{{AFX_MSG_MAP(CDlgDefaultProfileSet)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultProfileSet message handlers

BOOL CDlgDefaultProfileSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd;
	m_pAttribSelGrid->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);
	
	CWordArray wdArrayCol;
	CStringArray strArrayColValue;
	wdArrayCol.Add(2);
//	strArrayColValue.Add("20");
//	strArrayColValue.Add("30");		// replace itoh 2013/01/23 20FPS->30FPS
	strArrayColValue.Add("15");		// replace itoh 2013/02/15 30FPS->15FPS
	wdArrayCol.Add(3);
	strArrayColValue.Add("1");
	m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE, "", &wdArrayCol, &strArrayColValue);
	m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	m_pAttribSelGrid->SetUntitledPrefix("");

	char szBuf[256];
	::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, m_strIniPath);
	if (szBuf[0] != '\0')
		m_pAttribSelGrid->SelectRow(szBuf);

	::GetPrivateProfileString("Profile", "Dimension", "200 x 150", szBuf, 256, m_strIniPath);
	CString strDim = szBuf;
	CButton* pRadioSize1 = (CButton*)GetDlgItem(IDC_RADIO_SIZE1);
	CButton* pRadioSize2 = (CButton*)GetDlgItem(IDC_RADIO_SIZE2);
	CButton* pRadioSize3 = (CButton*)GetDlgItem(IDC_RADIO_SIZE3);
	CButton* pRadioSize4 = (CButton*)GetDlgItem(IDC_RADIO_SIZE4);
	CButton* pRadioSize5 = (CButton*)GetDlgItem(IDC_RADIO_SIZE5);
	CButton* pRadioSize6 = (CButton*)GetDlgItem(IDC_RADIO_SIZE6);
	if (strDim == "160 x 120")
		pRadioSize1->SetCheck(1);
	else if (strDim == "200 x 150")
		pRadioSize2->SetCheck(1);
	else if (strDim == "240 x 180")
		pRadioSize3->SetCheck(1);
	else if (strDim == "320 x 240")
		pRadioSize4->SetCheck(1);
	else if (strDim == "480 x 360")
		pRadioSize5->SetCheck(1);
	else if (strDim == "640 x 480")
		pRadioSize6->SetCheck(1);

//#ifdef _BUNDLE
//	pRadioSize2->SetCheck(1);
//	pRadioSize1->EnableWindow(FALSE);
//	pRadioSize3->EnableWindow(FALSE);
//	pRadioSize4->EnableWindow(FALSE);
//	pRadioSize5->EnableWindow(FALSE);
//	pRadioSize6->EnableWindow(FALSE);
//#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDefaultProfileSet::OnOK() 
{
	CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	if (strSel == "")
	{
		W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
		return;
	}

	CStringArray strArraySelected;
	m_pAttribSelGrid->GetSelectedData(strArraySelected);

	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	if (m_strIniPath == strSysIniPath)
	{
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, m_strIniPath);
		if (szBuf[0] == '\0' && atoi(strArraySelected[1]) != 0)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			return;
		}
	}
	else
	{
		if (m_bNoVideo && atoi(strArraySelected[1]) != 0)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			return;
		}
	}

	m_pAttribSelGrid->SaveData();
	
	::WritePrivateProfileString("Profile", "Name", strSel, m_strIniPath);

	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		CString strEntry;
		strEntry.Format("Param%d", i);
		::WritePrivateProfileString("Profile", strEntry, strArraySelected[i], m_strIniPath);
	}

	CString strDim;
	CButton* pRadioSize1 = (CButton*)GetDlgItem(IDC_RADIO_SIZE1);
	CButton* pRadioSize2 = (CButton*)GetDlgItem(IDC_RADIO_SIZE2);
	CButton* pRadioSize3 = (CButton*)GetDlgItem(IDC_RADIO_SIZE3);
	CButton* pRadioSize4 = (CButton*)GetDlgItem(IDC_RADIO_SIZE4);
	CButton* pRadioSize5 = (CButton*)GetDlgItem(IDC_RADIO_SIZE5);
	CButton* pRadioSize6 = (CButton*)GetDlgItem(IDC_RADIO_SIZE6);
	if (pRadioSize1->GetCheck())
		pRadioSize1->GetWindowText(strDim);
	else if (pRadioSize2->GetCheck())
		pRadioSize2->GetWindowText(strDim);
	else if (pRadioSize3->GetCheck())
		pRadioSize3->GetWindowText(strDim);
	else if (pRadioSize4->GetCheck())
		pRadioSize4->GetWindowText(strDim);
	else if (pRadioSize5->GetCheck())
		pRadioSize5->GetWindowText(strDim);
	else if (pRadioSize6->GetCheck())
		pRadioSize6->GetWindowText(strDim);
	::WritePrivateProfileString("Profile", "Dimension", strDim, m_strIniPath);

	CDialog::OnOK();
}

void CDlgDefaultProfileSet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;
}

LRESULT CDlgDefaultProfileSet::OnRowSelected(WPARAM wParam, LPARAM lParam)
{
	if ((int)wParam < 0)
	{
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
		return 0L;
	}

	LPCTSTR pStr = (LPCTSTR)lParam;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(pStr);

	return 0L;
}

void CDlgDefaultProfileSet::OnButtonDel() 
{
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
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strSel)
			::WritePrivateProfileString("Profile", "Name", "", m_strIniPath);
	}
}

void CDlgDefaultProfileSet::OnButtonRename() 
{
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
			::WritePrivateProfileString("Profile", "Name", strProfileName, m_strIniPath);
	}
}
