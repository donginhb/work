// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageInputName.h"
#include "NewWizDialog.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageInputName dialog


CPageInputName::CPageInputName(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageInputName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageInputName)
	//}}AFX_DATA_INIT
}


void CPageInputName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageInputName)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageInputName, CNewWizPage)
	//{{AFX_MSG_MAP(CPageInputName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageInputName message handlers

BOOL CPageInputName::OnInitDialog() 
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

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageInputName::OnWizardBack()
{
	if (m_pstrArray->GetAt(0) == "PPT")
		return IDW_SELECT_PPT;
	return IDW_SELECT_HTML;
}

LRESULT CPageInputName::OnWizardNext()
{
	CString strFile;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strFile);

	strFile.TrimLeft();
	if (strFile == "")
    {
		W_SHOW_MESSAGE(IDS_INPUT_NAME_FIRST, this);
		return -1;
	}

	m_strData = strFile;

	return 0;
}

void CPageInputName::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_INPUT_NAME);
	m_pParent->SetTitle(strTitle);

    CString strName;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
	if (strName == "") 
	{
		if (m_pstrArray->GetAt(0) == "PPT")
		{
			CString strFileName = ::GetFileName(m_pstrArray->GetAt(1));
			int nDot = strFileName.ReverseFind('.');
			if (nDot >= 0)
				strName = strFileName.Left(nDot);
		}
		else
			strName = m_pstrArray->GetAt(2);

		GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strName);
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}

BOOL CPageInputName::OnKillActive()
{
	return TRUE;
}


