// DlgOption.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgOption.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOption dialog


CDlgOption::CDlgOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOption)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOption, CDialog)
	//{{AFX_MSG_MAP(CDlgOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOption message handlers

BOOL CDlgOption::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	
	char szBuf[256];
	::GetPrivateProfileString("General", "Organization", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_ORG)->SetWindowText(szBuf);
	
	CString strLink;
	strLink.LoadString(IDS_DEFAULT_LINK);
	::GetPrivateProfileString("General", "LinkCaption", "", szBuf, 256, strIniPath);
	if (szBuf[0] != '\0')
		strLink = szBuf;
	GetDlgItem(IDC_EDIT_LINK)->SetWindowText(strLink);

	::GetPrivateProfileString("General", "Scorm", "1", szBuf, 256, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_DEFAULT_SCORN))->SetCheck(atoi(szBuf));

	::GetPrivateProfileString("General", "ConvDemo", "0", szBuf, 256, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_CONV_DEMO))->SetCheck(atoi(szBuf));

	int bPassive = ::GetPrivateProfileInt("General", "PassiveFTP", 0, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_PASSIVE))->SetCheck(bPassive);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOption::OnOK() 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	
	CString strOrg;
	GetDlgItem(IDC_EDIT_ORG)->GetWindowText(strOrg);
	::WritePrivateProfileString("General", "Organization", strOrg, strIniPath);

	CString strLink;
	GetDlgItem(IDC_EDIT_LINK)->GetWindowText(strLink);
	::WritePrivateProfileString("General", "LinkCaption", strLink, strIniPath);

	int nScorn = ((CButton*)GetDlgItem(IDC_CHECK_DEFAULT_SCORN))->GetCheck();
	CString strScorn;
	strScorn.Format("%d", nScorn);
	::WritePrivateProfileString("General", "Scorm", strScorn, strIniPath);

	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_CONV_DEMO))->GetCheck();
	CString strConv;
	strConv.Format("%d", nCheck);
	::WritePrivateProfileString("General", "ConvDemo", strConv, strIniPath);

	int nPassive = ((CButton*)GetDlgItem(IDC_CHECK_PASSIVE))->GetCheck();
	CString strTmp;
	strTmp.Format("%d", nPassive);
	::WritePrivateProfileString("General", "PassiveFTP", strTmp, strIniPath);

	CDialog::OnOK();
}
