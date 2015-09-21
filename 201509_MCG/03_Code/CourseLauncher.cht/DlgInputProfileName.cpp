// DlgInputProfileName.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgInputProfileName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputProfileName dialog


CDlgInputProfileName::CDlgInputProfileName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputProfileName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputProfileName)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArrayProfile = NULL;
}


void CDlgInputProfileName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputProfileName)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputProfileName, CDialog)
	//{{AFX_MSG_MAP(CDlgInputProfileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputProfileName message handlers

BOOL CDlgInputProfileName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PROFILE);
	pEdit->SetWindowText(m_strProfileName);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputProfileName::OnOK() 
{
	CString strProfileName;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PROFILE);
	pEdit->GetWindowText(strProfileName);

	strProfileName.TrimRight();
	if (strProfileName == "")
	{
		W_SHOW_ERROR(IDS_PROFILE_NAME_NOT_NULL, this);
		pEdit->SetFocus();
		pEdit->SetSel(0, -1);
		return;
	}

	if (strProfileName.GetLength() > 2)
	{
		if (strProfileName[0] == '<' && strProfileName.Right(1) == ">")
		{
			W_SHOW_WARNING(IDS_SYSTEM_DEFAULT_NOT_ALLOWED, this);
			return;
		}
	}

	if (m_pstrArrayProfile)
	{
		for (int i = 0; i < m_pstrArrayProfile->GetSize(); i++)
		{
			CString str = m_pstrArrayProfile->GetAt(i);
			if (str == strProfileName)
			{
				W_SHOW_ERROR(IDS_PROFILE_NAME_EXIST, this);
				pEdit->SetFocus();
				pEdit->SetSel(0, -1);
				return;
			}
		}
	}

	m_strProfileName = strProfileName;

	CDialog::OnOK();
}
