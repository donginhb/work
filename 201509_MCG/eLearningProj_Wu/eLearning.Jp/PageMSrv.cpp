// PageMSrv.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageMSrv.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMSrv property page

IMPLEMENT_DYNCREATE(CPageMSrv, CPropertyPage)

CPageMSrv::CPageMSrv() : CPropertyPage(CPageMSrv::IDD)
{
	//{{AFX_DATA_INIT(CPageMSrv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
	m_strPreLogin = "";
	m_strPreIP = "";
	m_strPrePWD = "";
}

CPageMSrv::~CPageMSrv()
{
}

void CPageMSrv::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMSrv)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMSrv, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMSrv)
	ON_EN_KILLFOCUS(IDC_EDIT_IP, OnKillfocusEditIp)
	ON_EN_KILLFOCUS(IDC_EDIT_LOGIN_NAME, OnKillfocusEditLoginName)
	ON_EN_KILLFOCUS(IDC_EDIT_PASSWORD, OnKillfocusEditPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMSrv message handlers

BOOL CPageMSrv::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;

	::GetPrivateProfileString("MediaServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(szBuf);
	m_strPreIP = szBuf;

	::GetPrivateProfileString("MediaServer", "LoginName", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_LOGIN_NAME)->SetWindowText(szBuf);
	m_strPreLogin = szBuf;

	::GetPrivateProfileString("MediaServer", "Password", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(szBuf);
	m_strPrePWD = szBuf;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMSrv::OnOK() 
{
	CString strText;
	CString strIniPath = MACRO_INI_PATH;

	GetDlgItem(IDC_EDIT_IP)->GetWindowText(strText);
	::WritePrivateProfileString("MediaServer", "IP", strText, strIniPath);
	
	GetDlgItem(IDC_EDIT_LOGIN_NAME)->GetWindowText(strText);
	::WritePrivateProfileString("MediaServer", "LoginName", strText, strIniPath);

	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(strText);
	::WritePrivateProfileString("MediaServer", "Password", strText, strIniPath);

	CPropertyPage::OnOK();
}

BOOL CPageMSrv::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageMSrv::OnKillfocusEditIp() 
{
}

void CPageMSrv::OnKillfocusEditLoginName() 
{
}

void CPageMSrv::OnKillfocusEditPassword() 
{
}
