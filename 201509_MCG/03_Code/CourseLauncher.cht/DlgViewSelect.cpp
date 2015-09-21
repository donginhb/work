// DlgViewSelect.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgViewSelect.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewSelect dialog


CDlgViewSelect::CDlgViewSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArrayCourse = NULL;
}


void CDlgViewSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgViewSelect)
	ON_BN_CLICKED(IDVIEW, OnView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewSelect message handlers

BOOL CDlgViewSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_COURSE);
	if (m_pstrArrayCourse)
	{
		for (int i = 0; i < m_pstrArrayCourse->GetSize(); i++)
		{
			CString strCourse = m_pstrArrayCourse->GetAt(i);
			pCombo->AddString(strCourse);
		}

		if (m_pstrArrayCourse->GetSize() > 0)
			pCombo->SetCurSel(0);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewSelect::OnView() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_COURSE);
	CString strCourse;	
	pCombo->GetWindowText(strCourse);

	CString strURL;
	GetCourseURL(strCourse, strURL);

	::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CDlgViewSelect::GetCourseURL(CString strNewCourse, CString& strURL)
{
	CString strParamIniPath = ::GetSysPath() + "Launcher.ini";
	char szBuf[512];
	::GetPrivateProfileString("WebServer", "CourseURL", "", szBuf, 512, strParamIniPath);
	if (szBuf[0] == '\0')
		return FALSE;

	strURL = szBuf;
	if (strURL.Right(1) != "/")
		strURL += "/";
	strURL += strNewCourse + "/index.htm";

	return TRUE;
}
