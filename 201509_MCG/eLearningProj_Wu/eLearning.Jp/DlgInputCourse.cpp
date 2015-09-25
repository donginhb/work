// DlgNewCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgInputCourse.h"
#include "cmkapi.h"
#include "Macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCourse dialog


CDlgInputCourse::CDlgInputCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strCourseName = "";
	m_strPreCourse = "";
}


void CDlgInputCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputCourse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgInputCourse)
	ON_EN_KILLFOCUS(IDC_EDIT_COURSE, OnKillfocusEditCourse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCourse message handlers

void CDlgInputCourse::OnOK() 
{
	CString strCourse;
	GetDlgItem(IDC_EDIT_COURSE)->GetWindowText(strCourse);
	
	strCourse.TrimLeft();
	strCourse.TrimRight();

	if (strCourse == "")
	{
		W_SHOW_WARNING(IDS_COURSE_NULL, this);
		return;
	}

	if (strlen(strCourse) > 128)
	{
		W_SHOW_ERROR(IDS_COURSE_TOO_LONG, this);
		return;
	}
	
	if (!::IsFileNameValid(strCourse))
	{
		W_SHOW_WARNING(IDS_INVALID_COURSE, this);
		return;
	}

	if (::FileExists(MACRO_COURSE_PATH + strCourse))
	{
		W_SHOW_WARNING(IDS_COURSE_DUP, this);
		return;
	}

	m_strCourseName = strCourse;

	CDialog::OnOK();
}

BOOL CDlgInputCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->SetWindowText(m_strCaption);
	
	GetDlgItem(IDC_EDIT_COURSE)->SetFocus();
	if (m_strCourseName != "")
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COURSE);
		pEdit->SetWindowText(m_strCourseName);
		m_strPreCourse = m_strCourseName;
		pEdit->SetSel(0, -1);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgInputCourse::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgInputCourse::OnKillfocusEditCourse() 
{
}
