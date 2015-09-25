// DlgInputOutlookName.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgInputOutlookName.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutlookName dialog


CDlgInputOutlookName::CDlgInputOutlookName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputOutlookName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputOutlookName)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreOutlook = "";
}


void CDlgInputOutlookName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputOutlookName)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputOutlookName, CDialog)
	//{{AFX_MSG_MAP(CDlgInputOutlookName)
	ON_CBN_KILLFOCUS(IDC_COMBO_OUTLOOK, OnKillfocusComboOutlook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutlookName message handlers

BOOL CDlgInputOutlookName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray strArray;
	::GetFiles(MACRO_OUTLOOK_PATH, "js", strArray);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OUTLOOK);
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int nDot = str.ReverseFind('.');
		str = str.Left(nDot);
		pCombo->AddString(str);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputOutlookName::OnOK() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OUTLOOK);
	CString strOutlook;
	pCombo->GetWindowText(strOutlook);
	if (strOutlook == "")
	{
		W_SHOW_WARNING(IDS_INPUT_OUTLOOK_NAME, this);
		return;
	}

	CString strPath = MACRO_OUTLOOK_PATH + strOutlook + ".js";
	if (::FileExists(strPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_OUTLOOK_NAME_EXIST);
		strMsg.Format(strTmp, strOutlook);
		if (W_ASK_YESNO_STR(strMsg, this) == IDNO)
			return;
	}
	
	m_strOutlook = strOutlook;

	CDialog::OnOK();
}

void CDlgInputOutlookName::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgInputOutlookName::OnKillfocusComboOutlook() 
{
}
