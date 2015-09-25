// DlgInputScornInfo.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgInputScornInfo.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputScornInfo dialog


CDlgInputScornInfo::CDlgInputScornInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputScornInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputScornInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreSel = "";
}


void CDlgInputScornInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputScornInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputScornInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgInputScornInfo)
	ON_LBN_SELCHANGE(IDC_LIST_COURSE, OnSelchangeListCourse)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOnOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputScornInfo message handlers

BOOL CDlgInputScornInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (int i = 0; i < m_strArrayCourse.GetSize(); i++)
		((CListBox*)GetDlgItem(IDC_LIST_COURSE))->AddString(m_strArrayCourse[i]);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputScornInfo::OnSelchangeListCourse() 
{
	if (m_strPreSel != "")
		WriteData(m_strPreSel);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_COURSE);
	if (pListBox->GetCurSel() < 0)
		return;

	CString strCourse;
	pListBox->GetText(pListBox->GetCurSel(), strCourse);
	
	CString strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\scorm.ini";

	char szBuf[1024];
	::GetPrivateProfileString("Title", "Name", strCourse, szBuf, 1024, strIniPath);
	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(szBuf);
	GetDlgItem(IDC_EDIT_KEYWORD)->SetWindowText(szBuf);

	::GetPrivateProfileString("Title", "Subtitle", strCourse, szBuf, 1024, strIniPath);
	GetDlgItem(IDC_EDIT_SUB_TITLE)->SetWindowText(szBuf);

	CString strPath = ::GetSysPath() + "Launcher.ini";
	::GetPrivateProfileString("General", "Organization", "", szBuf, 1024, strPath);
	CString strOrg = szBuf;

	::GetPrivateProfileString("General", "OrganizationsIdentity", strOrg, szBuf, 1024, strIniPath);
	GetDlgItem(IDC_EDIT_ORG)->SetWindowText(szBuf);

	CString strContentPath = ::GetSysPath() + "Courses\\" + strCourse + "\\html\\content.lrn";
	CString strContent = "";
	if (::FileExists(strContentPath))
	{
		CFile cf;
		cf.Open(strContentPath, CFile::modeRead);
		char* pBuf = new char[4096];
		int nRead = cf.Read(pBuf, 4096);
		if (nRead < 4096 && nRead >= 0)
			pBuf[nRead] = '\0';
		cf.Close();
		strContent = pBuf;
		delete [] pBuf;
	}

	for (int i = 0; i < strContent.GetLength() - 1; i++)
	{
		if (strContent[i] == '\r' && strContent[i + 1] == '\n')
		{
			strContent.SetAt(i, ' ');
			strContent.SetAt(i + 1, ' ');
		}
	}

	::GetPrivateProfileString("General", "Manifestidentity", strContent, szBuf, 1024, strIniPath);
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(szBuf);

	m_strPreSel = strCourse;
}

void CDlgInputScornInfo::WriteData(CString strCourse) 
{
	CString strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\scorm.ini";
	CString strTmp;

	GetDlgItem(IDC_EDIT_TITLE)->GetWindowText(strTmp);
	::WritePrivateProfileString("Title", "Name", strTmp, strIniPath);

	GetDlgItem(IDC_EDIT_SUB_TITLE)->GetWindowText(strTmp);
	::WritePrivateProfileString("Title", "Subtitle", strTmp, strIniPath);

	GetDlgItem(IDC_EDIT_ORG)->GetWindowText(strTmp);
	::WritePrivateProfileString("General", "OrganizationsIdentity", strTmp, strIniPath);
	::WritePrivateProfileString("General", "OrganizationsDefault", strTmp, strIniPath);

	GetDlgItem(IDC_EDIT_DESC)->GetWindowText(strTmp);
	::WritePrivateProfileString("General", "Manifestidentity", strTmp, strIniPath);

	GetDlgItem(IDC_EDIT_KEYWORD)->GetWindowText(strTmp);
	::WritePrivateProfileString("General", "Keyword", strTmp, strIniPath);
}

void CDlgInputScornInfo::OnClose() 
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_COURSE);
	if (pListBox->GetCurSel() >= 0)
	{
		CString strCourse;
		pListBox->GetText(pListBox->GetCurSel(), strCourse);

		WriteData(strCourse);
	}
	
	CDialog::OnClose();
}

void CDlgInputScornInfo::OnCancel() 
{
	/*
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_COURSE);
	if (pListBox->GetCurSel() >= 0)
	{
		CString strCourse;
		pListBox->GetText(pListBox->GetCurSel(), strCourse);


		CString strEDITORG;
		GetDlgItem(IDC_EDIT_ORG)->GetWindowText(strEDITORG);
		if (strEDITORG == "")
		{
			W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
			GetDlgItem(IDC_EDIT_ORG)->SetFocus();
			return;
		}


		WriteData(strCourse);
	}
	*/
	CDialog::OnCancel();
}

void CDlgInputScornInfo::OnOK() 
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_COURSE);
	if (pListBox->GetCurSel() >= 0)
	{
		CString strCourse;
		pListBox->GetText(pListBox->GetCurSel(), strCourse);


		CString strTITLE, strSUBTITLE, strEDITORG, strKEYWORD;

		GetDlgItem(IDC_EDIT_TITLE)->GetWindowText(strTITLE);
		GetDlgItem(IDC_EDIT_SUB_TITLE)->GetWindowText(strSUBTITLE);
		GetDlgItem(IDC_EDIT_ORG)->GetWindowText(strEDITORG);
		GetDlgItem(IDC_EDIT_KEYWORD)->GetWindowText(strKEYWORD);

		if (strTITLE == "")
		{
			W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
			GetDlgItem(IDC_EDIT_TITLE)->SetFocus();
			return;
		}

		if (strSUBTITLE == "")
		{
			W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
			GetDlgItem(IDC_EDIT_SUB_TITLE)->SetFocus();
			return;
		}

		if (strEDITORG == "")
		{
			W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
			GetDlgItem(IDC_EDIT_ORG)->SetFocus();
			return;
		}

		if (strKEYWORD == "")
		{
			W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
			GetDlgItem(IDC_EDIT_KEYWORD)->SetFocus();
			return;
		}

		WriteData(strCourse);
	}
	
	CDialog::OnOK();
}
