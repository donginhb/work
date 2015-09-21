// DlgAskType.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgAskType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAskType dialog


CDlgAskType::CDlgAskType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAskType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAskType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strType = "";
}


void CDlgAskType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAskType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAskType, CDialog)
	//{{AFX_MSG_MAP(CDlgAskType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAskType message handlers

BOOL CDlgAskType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TYPE);
	pEdit->SetWindowText(m_strType);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAskType::OnOK() 
{
	CString str;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TYPE);
	pEdit->GetWindowText(str);
	if (str == "")
	{
		W_SHOW_ERROR(IDS_EMPTY_TYPE, this);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}
	
	for (int i = 0; i < m_strArrayTypes.GetSize(); i++)
	{
		CString strType = m_strArrayTypes[i];
		if (str == strType && str != m_strType)
		{
			W_SHOW_ERROR(IDS_DUP_TYPE, this);
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
			return;
		}
	}

	m_strType = str;

	CDialog::OnOK();
}
