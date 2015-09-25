// DlgSelectCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelectCourse.h"
#include "macro.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourse dialog


CDlgSelectCourse::CDlgSelectCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArraySelCourse = NULL;
	m_strWndText = "";
}


void CDlgSelectCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectCourse)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectCourse)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourse message handlers

BOOL CDlgSelectCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray strArray;
	GetDir(MACRO_COURSE_PATH, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
		m_clCourse.AddString(" " + strArray[i]);

	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(m_strTitle);
	SetWindowText(m_strWndText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectCourse::OnOK() 
{
	CString strCourse;
	if (m_pstrArraySelCourse)
	{
		m_pstrArraySelCourse->RemoveAll();
		for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		{
			if (m_clCourse.GetCheck(iItem))
			{
				m_clCourse.GetText(iItem, strCourse);
				strCourse.TrimLeft();
				m_pstrArraySelCourse->Add(strCourse);
			}
		}
	}

	CDialog::OnOK();
}

void CDlgSelectCourse::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgSelectCourse::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CDlgSelectCourse::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}
