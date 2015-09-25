// DlgCloseToDo.cpp : implementation file
//

#include "stdafx.h"
#include "elearningcapture.h"
#include "DlgCloseToDo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCloseToDo dialog


CDlgCloseToDo::CDlgCloseToDo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCloseToDo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCloseToDo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCloseToDo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCloseToDo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCloseToDo, CDialog)
	//{{AFX_MSG_MAP(CDlgCloseToDo)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_EXIT_GEN, OnButtonExitGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCloseToDo message handlers

void CDlgCloseToDo::OnButtonCancel() 
{
	EndDialog(IDCANCEL);
}

void CDlgCloseToDo::OnButtonExit() 
{
	EndDialog(ID_EXIT_ONLY);
}

void CDlgCloseToDo::OnButtonExitGen() 
{
	EndDialog(ID_EXIT_GEN);
}

BOOL CDlgCloseToDo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
