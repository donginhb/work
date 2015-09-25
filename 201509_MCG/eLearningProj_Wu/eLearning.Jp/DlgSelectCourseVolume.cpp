// DlgSelectCourseVolume.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelectCourseVolume.h"
#include "macro.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourseVolume dialog


CDlgSelectCourseVolume::CDlgSelectCourseVolume(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectCourseVolume::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectCourseVolume)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArraySelCourse = NULL;
}


void CDlgSelectCourseVolume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectCourseVolume)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectCourseVolume, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectCourseVolume)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, OnReleasedcaptureSliderVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectCourseVolume message handlers

BOOL CDlgSelectCourseVolume::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CDialog::OnInitDialog();
	
	CStringArray strArray;
	GetDir(MACRO_COURSE_PATH, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
		m_clCourse.AddString(" " + strArray[i]);

	m_sliderVolume.SetRange(0, 100);
	m_sliderVolume.SetPos(50);
	GetDlgItem(IDC_EDIT_PERCENT)->SetWindowText("50");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectCourseVolume::OnOK() 
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

	m_nVolumeValue = m_sliderVolume.GetPos();

	CDialog::OnOK();
}

void CDlgSelectCourseVolume::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgSelectCourseVolume::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CDlgSelectCourseVolume::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

void CDlgSelectCourseVolume::OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nPos = m_sliderVolume.GetPos();
	CString strPos;
	strPos.Format("%d", nPos);
	GetDlgItem(IDC_EDIT_PERCENT)->SetWindowText(strPos);
	
	*pResult = 0;
}
