// DlgSelHTMPresent.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelHTMPresent.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelHTMPresent dialog


CDlgSelHTMPresent::CDlgSelHTMPresent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelHTMPresent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelHTMPresent)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelHTMPresent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelHTMPresent)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelHTMPresent, CDialog)
	//{{AFX_MSG_MAP(CDlgSelHTMPresent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelHTMPresent message handlers

BOOL CDlgSelHTMPresent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray strArray;
	::GetDir(MACRO_PRESENT_PATH, strArray);
	::SortArray(strArray);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelHTMPresent::OnOK() 
{
	CString strPresent;
	GetDlgItem(IDC_COMBO_PRESENT)->GetWindowText(strPresent);
	if (strPresent == "")
		return;

	m_strPresent = strPresent;

	CDialog::OnOK();
}
