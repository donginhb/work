// DlgCourseDup.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgCourseDup.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCourseDup dialog


CDlgCourseDup::CDlgCourseDup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCourseDup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCourseDup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreCourse = "";
}


void CDlgCourseDup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCourseDup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCourseDup, CDialog)
	//{{AFX_MSG_MAP(CDlgCourseDup)
	ON_BN_CLICKED(IDC_RADIO_NEW_NAME, OnRadioNewName)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, OnRadioOverwrite)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCourseDup message handlers

BOOL CDlgCourseDup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_EXIST)->SetWindowText(m_strStatic);
	GetDlgItem(IDC_RADIO_OVERWRITE)->SetWindowText(m_strRadioOverwrite);
	GetDlgItem(IDC_RADIO_NEW_NAME)->SetWindowText(m_strRadioNewName);

	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pRadio->SetCheck(1);

	CString strPath = m_strPath + m_strCourse;
	int nIndex = GetCurrentCourseIndex();
	CString strDefaultCourse = m_strCourse;
	if (::FileExists(strPath))
		strDefaultCourse.Format("%s(%d)", m_strCourse, nIndex + 1);
	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strDefaultCourse);
	m_strPreCourse = strDefaultCourse;
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(m_strCourse);

	SetWindowText(m_strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCourseDup::OnOK() 
{
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	m_bOverWrite = pRadio->GetCheck();

	CString strCourse;
	if (!m_bOverWrite)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
		pEdit->GetWindowText(strCourse);
		CString strPath = m_strPath + strCourse;
		if (::FileExists(strPath))
		{
			W_SHOW_ERROR(IDS_COURSE_DUP, this);
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
			return;	
		}
		m_strCourse = strCourse;
	}

	CDialog::OnOK();
}

void CDlgCourseDup::OnRadioNewName() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	pEdit->EnableWindow(TRUE);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);
}

void CDlgCourseDup::OnRadioOverwrite() 
{
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
}

int CDlgCourseDup::GetCurrentCourseIndex()
{
	if (m_strCourse == "")
		return 0;

	CString strPath = m_strPath;
	CStringArray strArray;
	::GetDir(strPath, strArray);

	int nIndex = 0;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int nLenDir = str.GetLength();
		int nLenCourse = m_strCourse.GetLength();
		if (nLenDir >= nLenCourse + 3)
		{
			if (str.Left(nLenCourse).CompareNoCase(m_strCourse) == 0)
			{
				if (str[nLenCourse] == '(' && str[nLenDir - 1] == ')')
				{
					int n = atoi(str.Mid(nLenCourse + 1, nLenDir - nLenCourse - 2));
					if (n > nIndex)
						nIndex = n;
				}
			}
		}
	}

	return nIndex;
}

void CDlgCourseDup::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgCourseDup::OnKillfocusEditName() 
{
}
