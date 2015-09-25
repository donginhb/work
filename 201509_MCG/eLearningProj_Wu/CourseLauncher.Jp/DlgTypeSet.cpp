// DlgTypeSet.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgTypeSet.h"
#include "cmkapi.h"
#include <afxcmn.h>
#include "..\elearning.jp\databrowsewnd.h"
#include "DlgAskType.h"
#include "DlgInputProfileName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTypeSet dialog

#define MACRO_MSAUDIO_FILE					(::GetSysPath() + "Profiles\\Profile\\msaudio.lrn")
#define MACRO_MSAUDIO_RST_FILE				(::GetSysPath() + "Profiles\\Profile\\msaudio.rst")
#define MACRO_ACELP_FILE					(::GetSysPath() + "Profiles\\Profile\\acelp.lrn")
#define MACRO_ACELP_RST_FILE				(::GetSysPath() + "Profiles\\Profile\\acelp.rst")
#define MACRO_INI_PATH						(::GetSysPath() + "Elearning.ini")

CDlgTypeSet::CDlgTypeSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTypeSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTypeSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bLoading = FALSE;
}


void CDlgTypeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTypeSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTypeSet, CDialog)
	//{{AFX_MSG_MAP(CDlgTypeSet)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_NEW_TYPE, OnButtonNewType)
	ON_BN_CLICKED(IDC_BUTTON_RENAME_TYPE, OnButtonRenameType)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_ORIGINAL, OnRadioOriginal)
	ON_BN_CLICKED(IDC_RADIO_CONVERT, OnRadioConvert)
	ON_CBN_SELCHANGE(IDC_COMBO_CODEC, OnSelchangeComboCodec)
	ON_BN_CLICKED(IDC_BUTTON_DEL_TYPE, OnButtonDelType)
	ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_MESSAGE(WM_ROW_APPENDED, OnRowAppended)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTypeSet message handlers

void CDlgTypeSet::OnSelchangeComboType() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	int nSel = pCombo->GetCurSel();
	LoadData(nSel);	
}

BOOL CDlgTypeSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	
	CString strTitle;
	strTitle.LoadString(IDS_VIDEO_CAPTURE);
    TabCtrlItem.pszText = (LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(0, &TabCtrlItem);
	
	strTitle.LoadString(IDS_SCREEN_CAPTURE);
	TabCtrlItem.pszText = (LPTSTR)(LPCTSTR)strTitle;
	pTab->InsertItem(1, &TabCtrlItem);

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	pTab->ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd;
	m_pAttribSelGrid->m_pFont = GetFont();
	m_pAttribSelGrid->SetMSGWindow(this);
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, pTab);

	CWordArray wdArrayCol;
	CStringArray strArrayColValue;
	wdArrayCol.Add(2);
	strArrayColValue.Add("20");
	wdArrayCol.Add(3);
	strArrayColValue.Add("3");
	m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE, "", &wdArrayCol, &strArrayColValue);
	m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	m_pAttribSelGrid->SetUntitledPrefix("");
	
	m_pScreenAttribSelGrid = new CDataBrowseWnd;
	m_pScreenAttribSelGrid->m_pFont = GetFont();
	m_pScreenAttribSelGrid->SetMSGWindow(this);
	m_pScreenAttribSelGrid->Create(WS_CHILD, rcGrid, pTab);

	m_pScreenAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	m_pScreenAttribSelGrid->SetUntitledPrefix("");

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CODEC);
	pCombo->AddString("ACELP.net");
	pCombo->AddString("Windows Media Audio V8");
	pCombo->SetFocus();

	CollectType();
	((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(0);
	OnSelchangeComboType();
	ChangeSel();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTypeSet::OnButtonNewType() 
{
	CDlgAskType dlg;
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	int nCount = ::GetPrivateProfileInt("Type", "TypeCount", 0, strIniPath);	
	char szBuf[512];
	CString strTmp;
	for (int i = 0; i < nCount; i++)
	{
		strTmp.Format("TypeInfo%d", i);
		::GetPrivateProfileString(strTmp, "Name", "", szBuf, 512, strIniPath);
		dlg.m_strArrayTypes.Add(szBuf);
	}

	if (dlg.DoModal() == IDOK)
	{
		strTmp.Format("%d", nCount + 1);
		::WritePrivateProfileString("Type", "TypeCount", strTmp, strIniPath);

		strTmp.Format("TypeInfo%d", nCount);
		::WritePrivateProfileString(strTmp, "Name", dlg.m_strType, strIniPath);

		CollectType();
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
		pCombo->SetCurSel(pCombo->GetCount() - 1);

		::WritePrivateProfileString(strTmp, "Convert", "1", strIniPath);

		CString strSysIniPath = MACRO_INI_PATH;
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			CString strParam;
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("Profile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "Profile" + strParam, szBuf, strIniPath);
		}

		::GetPrivateProfileString("ScreenProfile", "Codec", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileCodec", szBuf, strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			CString strParam;
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "ScreenProfile" + strParam, szBuf, strIniPath);
		}

		LoadData(nCount);
		ChangeSel();
	}
}

void CDlgTypeSet::LoadData(int nIndex)
{
	m_bLoading = TRUE;

	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	CString strTmp;
	strTmp.Format("TypeInfo%d", nIndex);
	char szBuf[512];
	::GetPrivateProfileString(strTmp, "Convert", "1", szBuf, 512, strIniPath);
	int nConvert = atoi(szBuf);
	CButton* pButtonOriginal = (CButton*)GetDlgItem(IDC_RADIO_ORIGINAL);
	CButton* pButtonConvert = (CButton*)GetDlgItem(IDC_RADIO_CONVERT);
	pButtonOriginal->SetCheck(1 - nConvert);
	pButtonConvert->SetCheck(nConvert);

	if (nConvert)
		RadioConvert();
	else
		RadioOriginal();

	::GetPrivateProfileString(strTmp, "ScreenProfileCodec", "ACELP.net", szBuf, 512, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CODEC);
	if (strcmp(szBuf, "ACELP.net") == 0)
	{
		pCombo->SetCurSel(0);
		m_pScreenAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
		m_pScreenAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	}
	else
	{
		pCombo->SetCurSel(1);

		CWordArray wdArrayCol;
		CStringArray strArrayColValue;
		wdArrayCol.Add(2);
		strArrayColValue.Add("2");
		wdArrayCol.Add(3);
		strArrayColValue.Add("20");
		m_pScreenAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE, "", &wdArrayCol, &strArrayColValue);
		m_pScreenAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);
	}

	::GetPrivateProfileString(strTmp, "ProfileName", "", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(szBuf);
	BOOL bSel;
	if (szBuf[0] != '\0')
	{
		bSel = m_pAttribSelGrid->SelectRow(szBuf);
		if (!bSel)
			W_SHOW_MESSAGE(IDS_DEFAULT_PROFILE_NOT_FOUND, this);
	}

	::GetPrivateProfileString(strTmp, "ScreenProfileName", "", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->SetWindowText(szBuf);
	if (szBuf[0] != '\0')
	{
		bSel = m_pScreenAttribSelGrid->SelectRow(szBuf);
		if (!bSel)
			W_SHOW_MESSAGE(IDS_SCREEN_PROFILE_NOT_FOUND, this);
	}

	m_bLoading = FALSE;
}

void CDlgTypeSet::CollectType()
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	int nCount = ::GetPrivateProfileInt("Type", "TypeCount", 0, strIniPath);	
	char szBuf[512];
	CString strTmp;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	pCombo->ResetContent();
	for (int i = 0; i < nCount; i++)
	{
		strTmp.Format("TypeInfo%d", i);
		::GetPrivateProfileString(strTmp, "Name", "", szBuf, 512, strIniPath);
		pCombo->AddString(szBuf);
	}
}

void CDlgTypeSet::OnButtonRenameType() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	int nSel = pCombo->GetCurSel();
	if (nSel < 0)
		return;

	CDlgAskType dlg;
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	int nCount = ::GetPrivateProfileInt("Type", "TypeCount", 0, strIniPath);	
	char szBuf[512];
	CString strTmp;
	for (int i = 0; i < nCount; i++)
	{
		strTmp.Format("TypeInfo%d", i);
		::GetPrivateProfileString(strTmp, "Name", "", szBuf, 512, strIniPath);
		dlg.m_strArrayTypes.Add(szBuf);
	}

	pCombo->GetWindowText(dlg.m_strType);
	if (dlg.DoModal() == IDOK)
	{
		strTmp.Format("TypeInfo%d", nSel);
		::WritePrivateProfileString(strTmp, "Name", dlg.m_strType, strIniPath);

		CollectType();
		pCombo->SetCurSel(nSel);
	}
}

void CDlgTypeSet::OnButtonRename() 
{
	CDataBrowseWnd* pDB;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
		pDB = m_pAttribSelGrid;
	else if (nSel == 1)
		pDB = m_pScreenAttribSelGrid;

	if (pDB->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_RENAME_FIRST, this);
		return;
	}

	CString strOldName;
	CString strSel;
	if (nSel == 0)
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strOldName);
	else
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strOldName);
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
	pDB->GetProfileName(strArrayProfile);
	dlg.m_pstrArrayProfile = &strArrayProfile;
	if (dlg.DoModal() == IDOK)
	{
		CString strProfileName = dlg.m_strProfileName;
		pDB->ReplaceSelected(strProfileName);

		//char szBuf[256];
		//::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		//if (szBuf == strOldName)
		//	::WritePrivateProfileString("ScreenProfile", "Name", strProfileName, m_strIniPath);
	}
}

void CDlgTypeSet::OnButtonDelete() 
{
	CDataBrowseWnd* pDB;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
		pDB = m_pAttribSelGrid;
	else if (nSel == 1)
		pDB = m_pScreenAttribSelGrid;

	if (pDB->GetSelectedRow() < 0)
	{
		W_SHOW_WARNING(IDS_SEL_DEL_FIRST, this);
		return;
	}

	CString strSel;
	if (nSel == 0)
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	else
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strSel);

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
		pDB->DeleteSelectedRow();

		//char szBuf[256];
		//::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, m_strIniPath);
		//if (szBuf == strSel)
		//	::WritePrivateProfileString("ScreenProfile", "Name", "", m_strIniPath);
	}
}

void CDlgTypeSet::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	ChangeSel();
}

void CDlgTypeSet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;

	m_pScreenAttribSelGrid->DestroyWindow();
	delete m_pScreenAttribSelGrid;
}

void CDlgTypeSet::ChangeSel() 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
	{
		GetDlgItem(IDC_COMBO_CODEC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CODEC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->ShowWindow(SW_HIDE);
		m_pAttribSelGrid->ShowWindow(SW_SHOW);
		m_pScreenAttribSelGrid->ShowWindow(SW_HIDE);
	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_COMBO_CODEC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CODEC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->ShowWindow(SW_SHOW);
		m_pAttribSelGrid->ShowWindow(SW_HIDE);
		m_pScreenAttribSelGrid->ShowWindow(SW_SHOW);
	}
}

void CDlgTypeSet::OnRadioOriginal() 
{
	RadioOriginal();
	SaveData();
}

void CDlgTypeSet::RadioOriginal() 
{
	m_pAttribSelGrid->EnableWindow(FALSE);
	m_pScreenAttribSelGrid->EnableWindow(FALSE);
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	pTab->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RENAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_SCREEN_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CODEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_CODEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->EnableWindow(FALSE);
}

void CDlgTypeSet::OnRadioConvert() 
{
	RadioConvert();
	SaveData();
}

void CDlgTypeSet::RadioConvert() 
{
	m_pAttribSelGrid->EnableWindow(TRUE);
	m_pScreenAttribSelGrid->EnableWindow(TRUE);
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	pTab->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RENAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_SCREEN_TYPE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_CODEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_CODEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->EnableWindow(TRUE);
}

void CDlgTypeSet::OnSelchangeComboCodec() 
{
	CString strCodec;
	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strCodec);
	if (strCodec == "ACELP.net")
	{
		m_pScreenAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
		m_pScreenAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	}
	else 	
	{
		CWordArray wdArrayCol;
		CStringArray strArrayColValue;
		wdArrayCol.Add(2);
		strArrayColValue.Add("2");
		wdArrayCol.Add(3);
		strArrayColValue.Add("20");
		m_pScreenAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE, "", &wdArrayCol, &strArrayColValue);
		m_pScreenAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);
	}

	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->SetWindowText("");
}

void CDlgTypeSet::SaveData() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	int nSel = pCombo->GetCurSel();
	if (nSel >= 0)
		SaveData(nSel);
}

void CDlgTypeSet::SaveData(int nIndex) 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	char szBuf[512];
	CString strTmp, strText;
	strTmp.Format("TypeInfo%d", nIndex);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_CONVERT);
	int nConvert = pButton->GetCheck();
	itoa(nConvert, szBuf, 10);
	::WritePrivateProfileString(strTmp, "Convert", szBuf, strIniPath);

	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strText);
	::WritePrivateProfileString(strTmp, "ProfileName", strText, strIniPath);

	CString strParam;
	CStringArray strArraySelected;
	m_pAttribSelGrid->GetSelectedData(strArraySelected);
	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		strParam.Format("Param%d", i);
		::WritePrivateProfileString(strTmp, "Profile" + strParam, strArraySelected[i], strIniPath);
	}

	GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->GetWindowText(strText);
	::WritePrivateProfileString(strTmp, "ScreenProfileName", strText, strIniPath);

	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strText);
	::WritePrivateProfileString(strTmp, "ScreenProfileCodec", strText, strIniPath);

	strArraySelected.RemoveAll();
	m_pScreenAttribSelGrid->GetSelectedData(strArraySelected);
	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		strParam.Format("Param%d", i);
		::WritePrivateProfileString(strTmp, "ScreenProfile" + strParam, strArraySelected[i], strIniPath);
	}
}

void CDlgTypeSet::DelData(int nIndex) 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	int nCount = ::GetPrivateProfileInt("Type", "TypeCount", 0, strIniPath);	
	if (nCount == 0)
		return;
	if (nIndex < 0 || nIndex >= nCount)
		return;

	CString strTmp;
	strTmp.Format("%d", nCount - 1);
	::WritePrivateProfileString("Type", "TypeCount", strTmp, strIniPath);

	char szBuf[512];
	CString strTmp1;
	for (int i = nIndex + 1; i < nCount; i++)
	{
		strTmp.Format("TypeInfo%d", i);
		strTmp1.Format("TypeInfo%d", i - 1);

		::GetPrivateProfileString(strTmp, "Name", "", szBuf, 512, strIniPath);
		::WritePrivateProfileString(strTmp1, "Name", szBuf, strIniPath);

		::GetPrivateProfileString(strTmp, "Convert", "", szBuf, 512, strIniPath);
		::WritePrivateProfileString(strTmp1, "Convert", szBuf, strIniPath);

		::GetPrivateProfileString(strTmp, "ProfileName", "", szBuf, 512, strIniPath);
		::WritePrivateProfileString(strTmp1, "ProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			CString strParam;
			strParam.Format("Param%d", i);
			::GetPrivateProfileString(strTmp, "Profile" + strParam, "", szBuf, 512, strIniPath);
			::WritePrivateProfileString(strTmp1, "Profile" + strParam, szBuf, strIniPath);
		}

		::GetPrivateProfileString(strTmp, "ScreenProfileCodec", "", szBuf, 512, strIniPath);
		::WritePrivateProfileString(strTmp1, "ScreenProfileCodec", szBuf, strIniPath);

		::GetPrivateProfileString(strTmp, "ScreenProfileName", "", szBuf, 512, strIniPath);
		::WritePrivateProfileString(strTmp1, "ScreenProfileName", szBuf, strIniPath);

		for (i = 1; i < 8; i++)
		{
			CString strParam;
			strParam.Format("Param%d", i);
			::GetPrivateProfileString(strTmp, "ScreenProfile" + strParam, "", szBuf, 512, strIniPath);
			::WritePrivateProfileString(strTmp1, "ScreenProfile" + strParam, szBuf, strIniPath);
		}
	}
}

void CDlgTypeSet::OnButtonDelType() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	int nSel = pCombo->GetCurSel();
	if (nSel >= 0)
	{
		if (W_ASK_YESNO(IDS_SURE_DEL, this) == IDNO)
			return;
		DelData(nSel);
		CollectType();
		if (pCombo->GetCount() > 0)
		{
			pCombo->SetCurSel(0);
			LoadData(0);
		}
	}
}

LRESULT CDlgTypeSet::OnRowAppended(WPARAM wParam, LPARAM lParam)
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if (nSel == 0)
		m_pAttribSelGrid->SaveData();
	else
		m_pScreenAttribSelGrid->SaveData();

	return 0L;
}

LRESULT CDlgTypeSet::OnRowSelected(WPARAM wParam, LPARAM lParam)
{
	if (m_bLoading)
		return 0L;
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB);
	int nSel = pTab->GetCurSel();
	if ((int)wParam < 0)
	{
		if (nSel == 0)
			GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
		else
			GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->SetWindowText("");

		return 0L;
	}

	LPCTSTR pStr = (LPCTSTR)lParam;
	if (nSel == 0)
		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(pStr);
	else
		GetDlgItem(IDC_EDIT_SCREEN_PROFILE)->SetWindowText(pStr);

	SaveData();

	return 0L;
}

