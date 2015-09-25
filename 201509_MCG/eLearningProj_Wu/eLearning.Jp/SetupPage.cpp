// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "SetupPage.h"
#include "NewWizDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupPage dialog


CSetupPage::CSetupPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CSetupPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupPage)
	//}}AFX_DATA_INIT
}


void CSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupPage)
	DDX_Control(pDX, ST_CAPTION, m_CaptionCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupPage, CNewWizPage)
	//{{AFX_MSG_MAP(CSetupPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupPage message handlers

BOOL CSetupPage::OnInitDialog() 
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

	m_CaptionCtrl.SetFont(&m_Font, TRUE);
    GetDlgItem(IDC_STATIC_STEP)->SetFont(&m_Font2, TRUE);

	((CButton*)GetDlgItem(IDC_RADIO_PPT))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupPage::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_PRESENT);
	m_pParent->SetTitle(strTitle);
}

BOOL CSetupPage::OnKillActive()
{
	return TRUE;
}

// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the next one, return the 
// Dialog resource ID of the page to display
LRESULT CSetupPage::OnWizardNext()
{
	int nPPT = ((CButton*)GetDlgItem(IDC_RADIO_PPT))->GetCheck();
	
	if (nPPT)
	{
	    m_strData = "PPT";	
		return IDW_SELECT_PPT;
	}

    m_strData = "HTM";	
	return IDW_SELECT_HTML;
}