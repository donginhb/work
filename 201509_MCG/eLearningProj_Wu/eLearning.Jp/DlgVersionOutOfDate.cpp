// DlgVersionOutOfDate.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgVersionOutOfDate.h"
//#include "DlgRegKey.h"
#include "DlgRegister.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVersionOutOfDate dialog


CDlgVersionOutOfDate::CDlgVersionOutOfDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVersionOutOfDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVersionOutOfDate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgVersionOutOfDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVersionOutOfDate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVersionOutOfDate, CDialog)
	//{{AFX_MSG_MAP(CDlgVersionOutOfDate)
	ON_BN_CLICKED(ID_REG, OnReg)
	ON_BN_CLICKED(ID_REGISTER, OnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVersionOutOfDate message handlers

void CDlgVersionOutOfDate::OnReg() 
{
}

void CDlgVersionOutOfDate::OnRegister() 
{
	CDlgRegister dlg;
	dlg.DoModal();
}

void CDlgVersionOutOfDate::OnCancel() 
{
	CDialog::OnCancel();
}
