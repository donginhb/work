// DlgVideoExist.cpp : implementation file
//

#include "stdafx.h"
#include "elearningcapture.h"
#include "DlgVideoExist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoExist dialog


CDlgVideoExist::CDlgVideoExist(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVideoExist::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVideoExist)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgVideoExist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVideoExist)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVideoExist, CDialog)
	//{{AFX_MSG_MAP(CDlgVideoExist)
	ON_BN_CLICKED(IDC_BUTTON_APPEND, OnButtonAppend)
	ON_BN_CLICKED(IDC_BUTTON_OVERWRITE, OnButtonOverwrite)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoExist message handlers

void CDlgVideoExist::OnButtonAppend() 
{
	EndDialog(ID_APPEND);
}

void CDlgVideoExist::OnButtonOverwrite() 
{
	EndDialog(ID_OVERWRITE);	
}

void CDlgVideoExist::OnButtonCancel() 
{
	EndDialog(IDCANCEL);	
}

BOOL CDlgVideoExist::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
