// DlgScreenCapture.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgScreenCapture.h"
#include "BmpBtn.h"
#include "cmkapi.h"
#include "DataBrowseWnd.h"
#include "macro.h"
#include "DlgSelDevice.h"
#include "Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgScreenCapture dialog


CDlgScreenCapture::CDlgScreenCapture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScreenCapture::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScreenCapture)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIniPath = MACRO_INI_PATH;
}


void CDlgScreenCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScreenCapture)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScreenCapture, CDialog)
	//{{AFX_MSG_MAP(CDlgScreenCapture)
	ON_WM_DESTROY()
	//add by buxaingyi
	/*ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	ON_CBN_SELCHANGE(IDC_COMBO_CODEC, OnSelchangeComboCodec)*/
	//////////////////////////////////////////////
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEVICE_SET, &CDlgScreenCapture::OnClickedDeviceSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScreenCapture message handlers

BOOL CDlgScreenCapture::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBtnOpen = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;
	//add by buxiangyi
	/*CRect rcStatic;
	GetDlgItem(IDC_STATIC_ATTRIB)->GetWindowRect(rcStatic);
	ScreenToClient(rcStatic);*/
	/////////////////////////////////
	CRect rcEdit;
	GetDlgItem(IDC_EDIT_FILE_NAME)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.right = rcEdit.right + 20;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpen->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_SEL_FILE);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_FILE_NAME)->MoveWindow(rcEdit);
	//add by buxiangyi
	/*CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pAttribSelGrid = new CDataBrowseWnd;
	m_pAttribSelGrid->m_pFont = MACRO_SYS_FONT2;
	m_pAttribSelGrid->Create(WS_VISIBLE | WS_CHILD, rcGrid, this);

	CString strMono;
	strMono.LoadString(IDS_MONO);
	m_pAttribSelGrid->SetDefaultValue("32 K", "15", "3 sec", "0", "16 K", "16000 Hz", strMono); 
	m_pAttribSelGrid->SetUntitledPrefix("");

	char szBuf[512];*/
	/////////////////////////////////////////////////////

	// add by buxiangyi
	//CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CODEC);
	//pCombo->AddString("Windows Media Audio V9.2");	//replase 2014/02/25 kagata

	//pCombo->AddString("ACELP.net");		//del 2013/12/2 kagata
	//pCombo->AddString("Windows Media Audio V9.1");	//replase 2014/02/25 kagata
	//add 2014/02/25 kagata

	// add by buxiangyi
	/*pCombo->SetCurSel(0);
	pCombo->SetFocus();

	GetDlgItem(IDC_EDIT_VIDEO_CODEC)->SetWindowText("Windows Media Screen V9");*/

	//del 2014/02/25 kagata MSAUDIOonly
	/*::GetPrivateProfileString("ScreenProfile", "Codec", "ACELP.net", szBuf, 512, m_strIniPath);
	if (strcmp(szBuf, "ACELP.net") == 0)
	{
		pCombo->SetCurSel(0);
		m_pAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
		m_pAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
	}
	else
	{*/
		//pCombo->SetCurSel(1);
	//add by buxiangyi
	/*m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE_S, "");
	m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);*/
	///////////////////////////////////////////////////////////
	//}

	//add by buxiangyi
	/*::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, m_strIniPath);
	if (szBuf[0] != '\0')
		m_pAttribSelGrid->SelectRow(szBuf);*/
	///////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgScreenCapture::OnOK() 
{
	CString strPath;
	GetDlgItem(IDC_EDIT_FILE_NAME)->GetWindowText(strPath);

	//add kagata 2013/10/08

	CString strDpath;
	strDpath = strPath.Mid( 1,2 );
	if (strDpath != ":\\")
	{
		W_SHOW_ERROR(IDS_INPUT_VIDEO_FILE, this);
		return;
	}

	//add kagata 2013/10/08
	
	if (strPath == "")
	{
		W_SHOW_ERROR(IDS_INPUT_VIDEO_FILE, this);
		return;
	}
// add itoh 2012/12/13
	else {
		int nComma = strPath.ReverseFind('.');

		if( nComma == -1)
		{
			strPath = strPath + ".mp4";
		}
		else {
			if(strPath.Right(strPath.GetLength() - nComma) != ".mp4") {
				strPath = strPath + ".mp4";
			}
		}
	}
// add end itoh 2012/12/13

	// add by buxiangyi
	/*CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	if (strSel == "")
	{
		W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
		return;
	}

	::WritePrivateProfileString("ScreenProfile", "Name", strSel, m_strOutIniPath);*/

	//add by buxiangyi
	/*CStringArray strArraySelected;
	m_pAttribSelGrid->GetSelectedData(strArraySelected);
	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		CString strEntry;
		strEntry.Format("Param%d", i);
		::WritePrivateProfileString("ScreenProfile", strEntry, strArraySelected[i], m_strOutIniPath);
	}*/
	///////////////////////////////////////////////////
	/*CString strCodec;
	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strCodec);
	::WritePrivateProfileString("ScreenProfile", "Codec", strCodec, m_strOutIniPath);*/
	
	m_strVideoPath = strPath;

	CDialog::OnOK();
}

void CDlgScreenCapture::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpen->DestroyWindow();
	delete m_pBtnOpen;

	//add by buxiangyi
	/*m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;*/
	////////////////////////////////
}

//add by buxiangyi

//LRESULT CDlgScreenCapture::OnRowSelected(WPARAM wParam, LPARAM lParam)
//{
//	if ((int)wParam < 0)
//	{
//		GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
//		return 0L;
//	}
//
//	LPCTSTR pStr = (LPCTSTR)lParam;
//	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText(pStr);
//
//	return 0L;
//}
//////////////////////////////////////////////////
LRESULT CDlgScreenCapture::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_SEL_FILE:
			OnSelectFile();
		break;
	}

	return 0L;
}

void CDlgScreenCapture::OnSelectFile() 
{
	CString StrTemp;
	StrTemp.LoadString(IDS_ASF_FILE);
 	CFileDialog* pDlg = new CFileDialog(FALSE, "mp4", NULL, 
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);

	if (pDlg->DoModal() != IDOK)
	{
		delete pDlg;
		return;
	}
	
	CString strPath = pDlg->GetPathName();
	delete pDlg;

	GetDlgItem(IDC_EDIT_FILE_NAME)->SetWindowText(strPath);
}

//add by buxiangyi

//void CDlgScreenCapture::OnSelchangeComboCodec() 
//{
//	CString strCodec;
//	GetDlgItem(IDC_COMBO_CODEC)->GetWindowText(strCodec);
//	if (strCodec == "ACELP.net")
//	{
//		m_pAttribSelGrid->SetProfileData(MACRO_ACELP_FILE, "");
//		m_pAttribSelGrid->SetRestrictedData(MACRO_ACELP_RST_FILE);
//	}
//	else 	
//	{
//		m_pAttribSelGrid->SetProfileData(MACRO_MSAUDIO_FILE_S, "");
//		m_pAttribSelGrid->SetRestrictedData(MACRO_MSAUDIO_RST_FILE);
//	}
//
//	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->SetWindowText("");
//}
//////////////////////////////////////////////////////////////

void CDlgScreenCapture::OnClickedDeviceSet()
{
	// TODO: Add your control notification handler code here
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = MACRO_INI_PATH;//strTempPath + "\\temp_s_profile.ini";
	CDevice dlg;
	dlg.m_strIniPath = strIniPath;
	dlg.m_NewCourse = FALSE;
	dlg.DoModal();
}
