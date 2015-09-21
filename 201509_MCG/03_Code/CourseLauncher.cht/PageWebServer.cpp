// PageWebServer.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "PageWebServer.h"
#include "EncodeApi.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageWebServer property page

IMPLEMENT_DYNCREATE(CPageWebServer, CPropertyPage)

CPageWebServer::CPageWebServer() : CPropertyPage(CPageWebServer::IDD)
{
	//{{AFX_DATA_INIT(CPageWebServer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreLogin = "";
	m_strPreIP = "";
	m_strPrePWD = "";
	m_strSubDir = "";

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPageWebServer::~CPageWebServer()
{
}

void CPageWebServer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageWebServer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageWebServer, CPropertyPage)
	//{{AFX_MSG_MAP(CPageWebServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageWebServer message handlers

void CPageWebServer::OnOK() 
{
	CString strText;
	CString strIniPath = ::GetSysPath() + "Launcher.ini";;

	GetDlgItem(IDC_EDIT_WEBSRV_IP)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	::WritePrivateProfileString("WebServer", "IP", strText, strIniPath);
	
	GetDlgItem(IDC_EDIT_WEBSRV_LOGIN_NAME)->GetWindowText(strText);
	::WritePrivateProfileString("WebServer", "LoginName", strText, strIniPath);

	GetDlgItem(IDC_EDIT_WEBSRV_PASSWORD)->GetWindowText(strText);
	char szPassword[256];
	KeyEncode(strText, szPassword);
	::WritePrivateProfileString("WebServer", "Password", szPassword, strIniPath);
	
	GetDlgItem(IDC_EDIT_WEBSRV_DIRECTORY)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	::WritePrivateProfileString("WebServer", "SubDir", strText, strIniPath);

	GetDlgItem(IDC_EDIT_COURSE_URL)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	::WritePrivateProfileString("WebServer", "CourseURL", strText, strIniPath);

	GetDlgItem(IDC_EDIT_WEBSRV_PORT)->GetWindowText(strText);
	::WritePrivateProfileString("WebServer", "Port", strText, strIniPath);

	CPropertyPage::OnOK();
}

BOOL CPageWebServer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("WebServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_WEBSRV_IP)->SetWindowText(szBuf);
	m_strPreIP = szBuf;

	::GetPrivateProfileString("WebServer", "LoginName", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_WEBSRV_LOGIN_NAME)->SetWindowText(szBuf);
	m_strPreLogin = szBuf;

	::GetPrivateProfileString("WebServer", "Password", "", szBuf, 256, strIniPath);
	char szPassword[256];
	KeyDecode(szBuf, szPassword);
	GetDlgItem(IDC_EDIT_WEBSRV_PASSWORD)->SetWindowText(szPassword);
	m_strPrePWD = szBuf;
	
	::GetPrivateProfileString("WebServer", "SubDir", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_WEBSRV_DIRECTORY)->SetWindowText(szBuf);
	m_strSubDir = szBuf;

	::GetPrivateProfileString("WebServer", "CourseURL", "http://", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_COURSE_URL)->SetWindowText(szBuf);
	m_strCourseURL = szBuf;	

	::GetPrivateProfileString("WebServer", "Port", "21", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_WEBSRV_PORT)->SetWindowText(szBuf);
	m_strPort = szBuf;

	GetDlgItem(IDC_EDIT_COURSENAME)->SetWindowText("/<Course Name>");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageWebServer::ConvBackSlashToSlash(CString& strInput)
{
	for (int i = 0; i < strInput.GetLength(); i++)
		if (strInput[i] == '\\')
			strInput.SetAt(i, '/');
}
