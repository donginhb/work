// DlgLaunchOK.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgLaunchOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLaunchOK dialog


CDlgLaunchOK::CDlgLaunchOK(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLaunchOK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLaunchOK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLaunchOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLaunchOK)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLaunchOK, CDialog)
	//{{AFX_MSG_MAP(CDlgLaunchOK)
	ON_BN_CLICKED(ID_VIEW_RESULT, OnViewResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLaunchOK message handlers

void CDlgLaunchOK::OnViewResult() 
{
	EndDialog(IDOK);	
}
