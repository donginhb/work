// DlgScreenProfileSet.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgScreenProfileSet.h"
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
// CDlgScreenProfileSet dialog


CDlgScreenProfileSet::CDlgScreenProfileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScreenProfileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScreenProfileSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIniPath = MACRO_INI_PATH;
}


void CDlgScreenProfileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScreenProfileSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScreenProfileSet, CDialog)
	//{{AFX_MSG_MAP(CDlgScreenProfileSet)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_CBN_SELCHANGE(IDC_COMBO_CODEC, OnSelchangeComboCodec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScreenProfileSet message handlers
BOOL CDlgScreenProfileSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd;
	m_pAttribSelGrid->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	m_pAttribSelGrid->SetUntitledPrefix("");

	char szBuf[512];
	// add by buxiangyi
	/*CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CODEC);
	pCombo->AddString(_T("Windows Media Audio V9.2"));*/
	
	//pCombo->AddString("ACELP.net");		//del 2013/11/18 kagata BugNo.88(‰¼)
	//pCombo->AddString("Windows Media Audio V9.1");	//replase 2014/02/25 kagata
	//add 2014/02/25 kagata

	// add by buxiangyi
	/*pCombo->SetCurSel(0);
	pCombo->SetFocus();*/

	//GetDlgItem(IDC_EDIT_VIDEO_CODEC)->SetWindowText("Windows Media Screen V9");  // add by buxiangyi

	//del 2014/02/25 kagata MSAUDIOonly
//	::GetPrivateProfileString("ScreenProfile", "Codec", "Windows Media Audio V9.1", szBuf, 512, m_strIniPath);	//replase 2014/02/25 kagata
	/*::GetPrivateProfileString("ScreenProfile", "Codec", "Windows Media Audio V9.2", szBuf, 512, m_strIniPath);
	if (strcmp(szBuf, "ACELP.net") == 0)
	{
		pCombo->SetCurSel(0);
		m_pAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
		m_pAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	}
	else
	{
		pCombo->SetCurSel(1);*/
		m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE_S, "");
		m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);
	//}
	// add by buxiangyi
	//OnSelchangeComboCodec();
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, m_strIniPath);
	if (szBuf[0] != '\0')
		m_pAttribSelGrid->SelectRow(szBuf);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgScreenProfileSet::OnOK() 
{
	CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	if (strSel == "")
	{
		W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
		return;
	}




	m_pAttribSelGrid->SaveData();
	
	::WritePrivateProfileString("ScreenProfile", "Name", strSel, m_strIniPath);

	CStringArray strArraySelected;
	m_pAttribSelGrid->GetSelectedData(strArraySelected);
	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		CString strEntry;
		strEntry.Format("Param%d", i);
		::WritePrivateProfileString("ScreenProfile", strEntry, strArraySelected[i], m_strIniPath);
	}
	// add by buxiangyi
	/*CString strCodec;
	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strCodec);
	::WritePrivateProfileString("ScreenProfile", "Codec", strCodec, m_strIniPath);*/

	CDialog::OnOK();
} 

void CDlgScreenProfileSet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;
}

LRESULT CDlgScreenProfileSet::OnRowSelected(WPARAM wParam, LPARAM lParam)
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

void CDlgScreenProfileSet::OnButtonDel() 
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
		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strSel)
			::WritePrivateProfileString("ScreenProfile", "Name", "", m_strIniPath);
	}
}

void CDlgScreenProfileSet::OnButtonRename() 
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
		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		if (szBuf == strOldName)
			::WritePrivateProfileString("ScreenProfile", "Name", strProfileName, m_strIniPath);
	}
}

void CDlgScreenProfileSet::OnSelchangeComboCodec() 
{
	CString strCodec;
	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strCodec);
	if (strCodec == "ACELP.net")
	{
		m_pAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
		m_pAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	}
	else 	
	{
		CWordArray wdArrayCol;
		CStringArray strArrayColValue;
		wdArrayCol.Add(2);
		strArrayColValue.Add("5");
		wdArrayCol.Add(3);
		strArrayColValue.Add("1");
		m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE_S, "", &wdArrayCol, &strArrayColValue);
		m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);
	}

	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
}
