// DlgMP4Rec.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgMP4Rec.h"
#include "BmpBtn.h"
#include "cmkapi.h"
#include "DataBrowseWnd.h"
#include "macro.h"
#include "Device.h"
#include "CaptureDevApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Rec dialog


CDlgMP4Rec::CDlgMP4Rec(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMP4Rec::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMP4Rec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIniPath = MACRO_INI_PATH;
}


void CDlgMP4Rec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMP4Rec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMP4Rec, CDialog)
	//{{AFX_MSG_MAP(CDlgMP4Rec)
	ON_WM_DESTROY()
	//add by buxiangyi
	//ON_MESSAGE(WM_ROW_SELECTED, OnRowSelected)
	////////////////////////////////////////////////
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEVICE_SET, &CDlgMP4Rec::OnClickedDeviceSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMP4Rec message handlers

BOOL CDlgMP4Rec::OnInitDialog() 
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

	CWordArray wdArrayCol;
	CStringArray strArrayColValue;
	wdArrayCol.Add(3);
	strArrayColValue.Add("3");
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
		pRadioSize6->SetCheck(1);*/
	///////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMP4Rec::OnOK() 
{
	CString strPath;
	GetDlgItem(IDC_EDIT_FILE_NAME)->GetWindowText(strPath);
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
	//add by buxiangyi
	/*CString strSel;
	GetDlgItem(IDC_EDIT_DEFAULT_PROFILE)->GetWindowText(strSel);
	if (strSel == "")
	{
		W_SHOW_ERROR(IDS_PROFILE_INPUT_FIRST, this);
		return;
	}

	::WritePrivateProfileString("Profile", "Name", strSel, m_strOutIniPath);

	CStringArray strArraySelected;
	m_pAttribSelGrid->GetSelectedData(strArraySelected);
	for (int i = 1; i < strArraySelected.GetSize(); i++)
	{
		CString strEntry;
		strEntry.Format("Param%d", i);
		::WritePrivateProfileString("Profile", strEntry, strArraySelected[i], m_strOutIniPath);
	}*/
	///////////////////////////////////////////////////////////////////
	//add bby buxiangyi
	/*CString strDim;
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
	::WritePrivateProfileString("Profile", "Dimension", strDim, m_strOutIniPath);*/
	//////////////////////////////////////////////////////////////
	m_strVideoPath = strPath;
	
	CString strType, strParam1, strParam2;
	::GetDevice(strType, strParam1, strParam2);
	int n = ::CheckAndInputDevValid(strType, strParam1, strParam2);
	if (n != DEV_MATCH)
	{
		W_SHOW_ERROR(IDS_CRAME_EMPTY,this);
		return;
	}
	CDialog::OnOK();
}

void CDlgMP4Rec::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpen->DestroyWindow();
	delete m_pBtnOpen;

	//addbybuxiangyi
	/*m_pAttribSelGrid->DestroyWindow();
	delete m_pAttribSelGrid;*/
}
//add by buxiangyi
//LRESULT CDlgMP4Rec::OnRowSelected(WPARAM wParam, LPARAM lParam)
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
///////////////////////////////////////
LRESULT CDlgMP4Rec::OnButtonUp(WPARAM wParam, LPARAM lParam)
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

void CDlgMP4Rec::OnSelectFile() 
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

void CDlgMP4Rec::OnClickedDeviceSet()
{
	// TODO: Add your control notification handler code here
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = MACRO_INI_PATH;//strTempPath + "\\temp_profile.ini";
	CDevice dlg;
	dlg.m_strIniPath = strIniPath;
	dlg.m_ScreenCapture= FALSE;
	dlg.DoModal();
}
