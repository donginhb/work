// DlgRegenIndex.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgRegenIndex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRegenIndex dialog


CDlgRegenIndex::CDlgRegenIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRegenIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRegenIndex)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRegenIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRegenIndex)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRegenIndex, CDialog)
	//{{AFX_MSG_MAP(CDlgRegenIndex)
	ON_BN_CLICKED(IDC_BUTTON_OVERWRITE, OnButtonOverwrite)
	ON_BN_CLICKED(IDC_BUTTON_RESERVE, OnButtonReserve)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRegenIndex message handlers

void CDlgRegenIndex::OnButtonOverwrite() 
{
	EndDialog(ID_OVERWRITE);	
}

void CDlgRegenIndex::OnButtonReserve() 
{
	EndDialog(ID_RESERVE);	
}

void CDlgRegenIndex::OnButtonSelect() 
{
	EndDialog(ID_SELECT);	
}
