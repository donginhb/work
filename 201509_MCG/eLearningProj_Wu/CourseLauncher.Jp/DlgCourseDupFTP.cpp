// DlgCourseDup.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "DlgCourseDupFTP.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCourseDupFTP dialog


CDlgCourseDupFTP::CDlgCourseDupFTP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCourseDupFTP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCourseDupFTP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreCourse = "";
}


void CDlgCourseDupFTP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCourseDupFTP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCourseDupFTP, CDialog)
	//{{AFX_MSG_MAP(CDlgCourseDupFTP)
	ON_BN_CLICKED(IDC_RADIO_NEW_NAME, OnRadioNewName)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, OnRadioOverwrite)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCourseDupFTP message handlers

BOOL CDlgCourseDupFTP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pRadio->SetCheck(1);

	CString strDefaultCourse = m_strCourse;
	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strDefaultCourse);
	m_strPreCourse = strDefaultCourse;
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(m_strCourse);

#ifdef _LOG
	CString strText;
	GetWindowText(strText);

	LOG("顯示<對話盒>", strText);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCourseDupFTP::OnOK() 
{
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	m_bOverWrite = pRadio->GetCheck();

	CString strCourse;
	if (!m_bOverWrite)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
		pEdit->GetWindowText(strCourse);
		m_strCourse = strCourse;
	}

#ifdef _LOG
	CString strTmp;
	GetDlgItem(IDOK)->GetWindowText(strTmp);
	LOG("點取<對話盒>按鈕", strTmp);
#endif

	CDialog::OnOK();
}

void CDlgCourseDupFTP::OnRadioNewName() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	pEdit->EnableWindow(TRUE);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);

#ifdef _LOG
	LOG("點取選項", m_strRadioNewName);
#endif
}

void CDlgCourseDupFTP::OnRadioOverwrite() 
{
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);

#ifdef _LOG
	LOG("點取選項", m_strRadioOverwrite);
#endif
}


void CDlgCourseDupFTP::OnCancel() 
{
#ifdef _LOG
	CString strTmp;
	GetDlgItem(IDCANCEL)->GetWindowText(strTmp);
	LOG("點取<對話盒>按鈕", strTmp);
#endif
	
	CDialog::OnCancel();
}

void CDlgCourseDupFTP::OnKillfocusEditName() 
{
#ifdef _LOG	
	CString strCourse;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strCourse);
	if (m_strPreCourse != strCourse)
	{
		LOG("輸入<Course Name>", strCourse);
		m_strPreCourse = strCourse;
	}
#endif	
}
