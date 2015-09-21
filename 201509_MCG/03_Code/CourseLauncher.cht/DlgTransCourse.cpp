// DlgTransCourse.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgTransCourse.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTransCourse dialog


CDlgTransCourse::CDlgTransCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTransCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTransCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTransCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTransCourse)
	DDX_Control(pDX, IDC_COMBO_COURSE, m_cbCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTransCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgTransCourse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTransCourse message handlers

BOOL CDlgTransCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strPath = ::GetSysPath() + "Launched Courses";
	CStringArray strArray;
	::GetDir(strPath, strArray);
	::SortArray(strArray);

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		m_cbCourse.AddString(str);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTransCourse::OnOK() 
{
	m_cbCourse.GetWindowText(m_strCourse);
	
	CDialog::OnOK();
}
