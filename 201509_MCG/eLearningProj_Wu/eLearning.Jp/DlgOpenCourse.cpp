// DlgOpenCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgOpenCourse.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenCourse dialog


CDlgOpenCourse::CDlgOpenCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpenCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpenCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgOpenCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpenCourse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOpenCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgOpenCourse)
	ON_LBN_DBLCLK(IDC_LIST_COURSE, OnDblclkListCourse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenCourse message handlers

BOOL CDlgOpenCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStringArray strArray;
	::GetDir(MACRO_COURSE_PATH, strArray);
	::SortArray(strArray);

	CListBox* pList = (CListBox*) GetDlgItem(IDC_LIST_COURSE);
// add itoh 2013/10/04 PPT2013 version check
//	for (int i = 0; i < strArray.GetSize(); i++)
//		pList->AddString(strArray[i]);
	for (int i = 0; i < strArray.GetSize(); i++) {
		if (::FileExists(MACRO_COURSE_PATH + "\\" + strArray[i] + "\\html\\config.js"))
			pList->AddString(strArray[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOpenCourse::OnOK() 
{
	CListBox* pList = (CListBox*) GetDlgItem(IDC_LIST_COURSE);
	int nSel = pList->GetCurSel();
	if (nSel >= 0)
		pList->GetText(nSel, m_strCourse);
	else
	{
		CString strMsg;
		strMsg.LoadString(IDS_NO_COURSE_SELECTED);
		SHOW_WARNING_STR(strMsg);
		return;
	}

	CDialog::OnOK();
}

void CDlgOpenCourse::OnDblclkListCourse() 
{
	OnOK();
}
