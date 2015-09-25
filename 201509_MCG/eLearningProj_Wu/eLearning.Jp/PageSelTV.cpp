// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelTV.h"
#include "NewWizDialog.h"
#include "cmkapi.h"
#include "ImageCell.h"
#include "macro.h"
#include "DlgTreeSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelTV dialog

#define constSmallLayoutWidth	150
#define constSmallLayoutHeight	130


CPageSelTV::CPageSelTV(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelTV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelTV)
	//}}AFX_DATA_INIT
}


void CPageSelTV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelTV)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelTV, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelTV)
	ON_MESSAGE(WM_IMAGE_CELL_DBL_CLICK, OnCellDblClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelTV message handlers

BOOL CPageSelTV::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_Font.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, //_T("Arial"));
#ifdef _JAPANESE
	_T("MS UI Gothic"));
#else
	_T("Arial"));
#endif

	m_Font2.CreateFont(-16, 0, 0, 0, 
		FW_NORMAL, TRUE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, //_T("Arial"));
#ifdef _JAPANESE
	_T("MS UI Gothic"));
#else
	_T("Arial"));
#endif

    GetDlgItem(IDC_STATIC_STEP)->SetFont(&m_Font2, TRUE);

	m_pLayoutCell = new CImageCell;
	m_pLayoutCell->m_nMenuID = IDR_MENU_CHANGE_BMP;
	m_pLayoutCell->m_bShowSize = FALSE;
	m_pLayoutCell->m_pFont = MACRO_SYS_FONT2;
	m_pLayoutCell->m_bWithFrame = TRUE;
	m_pLayoutCell->SetTitle("");
	m_pLayoutCell->m_rgbTitleBg = RGB(0, 0, 128);
	m_pLayoutCell->m_rgbTitleFg = RGB(255, 255, 255);
	CRect rect;
	GetDlgItem(IDC_STATIC_PICTURE)->GetWindowRect(rect);
    ScreenToClient(rect);

	CString strTmp = MACRO_TEMPLATE_PATH;
	CString strLayoutPath = strTmp + "_vidpanel.gif";
    int nW = rect.Width();
	int nH = rect.Height();
	rect.left = rect.left + (nW - constSmallLayoutWidth) / 2;
	rect.top = rect.top + (nH - constSmallLayoutHeight) / 2;
    rect.right = rect.left + constSmallLayoutWidth;
    rect.bottom = rect.top + constSmallLayoutHeight;
	m_pLayoutCell->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_LAYOUT_CELL);
	m_pLayoutCell->SetPath(strLayoutPath);

	CString strIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("General", "DefaultOutlook", "", szBuf, 255, strIniPath);

	CollectLayout(szBuf);

	char szValue[256];
	::GetPrivateProfileString("General", "TVSize", "1", szValue, 255, strIniPath);
	int nSel = atoi(szValue);

	CButton* pRadioSmall = (CButton*)GetDlgItem(IDC_RADIO_SMALL);
	CButton* pRadioMedium = (CButton*)GetDlgItem(IDC_RADIO_MEDIUM);
	CButton* pRadioLarge = (CButton*)GetDlgItem(IDC_RADIO_LARGE);
	CButton* pRadioExLarge = (CButton*)GetDlgItem(IDC_RADIO_EXLARGE);
	CButton* pRadioHuge = (CButton*)GetDlgItem(IDC_RADIO_HUGE);
	if (nSel == 0)
	{
		pRadioSmall->SetCheck(1);
		pRadioMedium->SetCheck(0);
		pRadioLarge->SetCheck(0);
		pRadioExLarge->SetCheck(0);
		pRadioHuge->SetCheck(0);
	}
	else if (nSel == 1)
	{
		pRadioSmall->SetCheck(0);
		pRadioMedium->SetCheck(1);
		pRadioLarge->SetCheck(0);
		pRadioExLarge->SetCheck(0);
		pRadioHuge->SetCheck(0);
	}
	else if (nSel == 2)
	{
		pRadioSmall->SetCheck(0);
		pRadioMedium->SetCheck(0);
		pRadioLarge->SetCheck(1);
		pRadioExLarge->SetCheck(0);
		pRadioHuge->SetCheck(0);
	}
	else if (nSel == 3)
	{
		pRadioSmall->SetCheck(0);
		pRadioMedium->SetCheck(0);
		pRadioLarge->SetCheck(0);
		pRadioExLarge->SetCheck(1);
		pRadioHuge->SetCheck(0);
	}
	else if (nSel == 4)
	{
		pRadioSmall->SetCheck(0);
		pRadioMedium->SetCheck(0);
		pRadioLarge->SetCheck(0);
		pRadioExLarge->SetCheck(0);
		pRadioHuge->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPageSelTV::OnWizardNext()
{
	return 0;
}

void CPageSelTV::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_PPT);
	m_pParent->SetTitle(strTitle);
}

BOOL CPageSelTV::OnKillActive()
{
	return TRUE;
}

void CPageSelTV::OnDestroyPage()
{	
	m_pLayoutCell->DestroyWindow();
	delete m_pLayoutCell;
}

void CPageSelTV::OnSelectLayout() 
{
}


LRESULT CPageSelTV::OnWizardBack()
{
	return 0;
}

LRESULT CPageSelTV::OnCellDblClicked(WPARAM wParam, LPARAM lParam)
{
	SelectLayout();

	return 0L;
}

BOOL CPageSelTV::SelectLayout()
{
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);

	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = MACRO_TV_PATH;
	dlg.m_bSelectable = FALSE;
	CString strTitle;
	strTitle.LoadString(IDS_TV_SELECT);
	dlg.m_strTitle = strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(TRUE);
	dlg.SetShowType(0);
	dlg.SetFrameSize(120, 120);
	dlg.m_nWidth = nScreenX * 2 / 3;
	dlg.m_nHeight = nScreenY * 2 / 3;
	if(dlg.DoModal() == IDOK)
	{
		m_pLayoutCell->SetPath(dlg.m_strSelectedFile);
		m_strPath = dlg.m_strSelectedFile;
	}


	return TRUE;
}

void CPageSelTV::CollectLayout(CString strSel) 
{
	CStringArray strArray;
	::GetFiles(MACRO_OUTLOOK_PATH, "js", strArray);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OUTLOOK);
	pCombo->ResetContent();
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int nDot = str.ReverseFind('.');
		str = str.Left(nDot);
		pCombo->AddString(str);	
	}

	if (strSel == "")
	{
		pCombo->SetCurSel(-1);
	}
	else
	{
		int nSel = pCombo->FindString(-1, strSel);
		if (nSel >= 0)
		{
			pCombo->SetCurSel(nSel);
		}
	}
}

BOOL CPageSelTV::OnWizardFinish()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OUTLOOK);
	
	CString str;
	pCombo->GetWindowText(str);

	m_strData = str;

	m_strData2 = m_strPath;

	CWnd* pRadio = NULL;
	CButton* pRadioSmall = (CButton*)GetDlgItem(IDC_RADIO_SMALL);
	CButton* pRadioMedium = (CButton*)GetDlgItem(IDC_RADIO_MEDIUM);
	CButton* pRadioLarge = (CButton*)GetDlgItem(IDC_RADIO_LARGE);
	CButton* pRadioExLarge = (CButton*)GetDlgItem(IDC_RADIO_EXLARGE);
	CButton* pRadioHuge = (CButton*)GetDlgItem(IDC_RADIO_HUGE);
	if (pRadioSmall->GetCheck())
	{
		pRadio = pRadioSmall;
		str = "0";
	}
	else if (pRadioMedium->GetCheck())
	{
		pRadio = pRadioMedium;
		str = "1";
	}
	else if (pRadioLarge->GetCheck())
	{
		pRadio = pRadioLarge;
		str = "2";
	}
	else if (pRadioExLarge->GetCheck())
	{
		pRadio = pRadioExLarge;
		str = "3";
	}
	else if (pRadioHuge->GetCheck())
	{
		pRadio = pRadioHuge;
		str = "4";
	}
	
	if (pRadio)
		pRadio->GetWindowText(m_strData3);

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_info.ini";

	::WritePrivateProfileString("General", "TVSize", str, strIniPath);

	::WritePrivateProfileString("General", "OutlookName", m_strData, strIniPath);

	return TRUE;
}
