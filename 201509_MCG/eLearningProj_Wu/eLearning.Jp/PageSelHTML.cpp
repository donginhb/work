// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelHTML.h"
#include "NewWizDialog.h"
#include "BmpBtn.h"
#include "cmkapi.h"
#include "Macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelHTML dialog


CPageSelHTML::CPageSelHTML(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelHTML::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelHTML)
	//}}AFX_DATA_INIT
}


void CPageSelHTML::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelHTML)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelHTML, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelHTML)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelHTML message handlers

BOOL CPageSelHTML::OnInitDialog() 
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

	CStringArray strArray;
	::GetDir(MACRO_PRESENT_PATH, strArray);
	::SortArray(strArray);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPageSelHTML::OnWizardNext()
{
	CString strFile;
	GetDlgItem(IDC_COMBO_PRESENT)->GetWindowText(strFile);

	strFile.TrimLeft();
	if (strFile == "")
    {
		W_SHOW_MESSAGE(IDS_SEL_HTML_FIRST, this);
		return -1;
	}

    m_strData = strFile;
    
	return 0;
}

void CPageSelHTML::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_HTML);
	m_pParent->SetTitle(strTitle);
}

BOOL CPageSelHTML::OnKillActive()
{
	return TRUE;
}

// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the previous one, return the 
// Dialog resource ID of the page to display
LRESULT CPageSelHTML::OnWizardBack()
{
	CString strFile;
	GetDlgItem(IDC_COMBO_PRESENT)->GetWindowText(strFile);

    m_strData = strFile;
	
	return IDW_SETUP;
}

