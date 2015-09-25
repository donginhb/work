// DlgAuFixSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAuFixSet.h"
#include "afxdialogex.h"
#include "Macro.h"
#include "MainFrm.h"
#include "Cmkapi.h"


// CDlgAuFixSet dialog

IMPLEMENT_DYNAMIC(CDlgAuFixSet, CDialogEx)

CDlgAuFixSet::CDlgAuFixSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAuFixSet::IDD, pParent)
{

}

CDlgAuFixSet::~CDlgAuFixSet()
{
}

void CDlgAuFixSet::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SLIDER_AUFIX, m_sliderAuFix);
	DDX_Control(pDX, IDC_STATIC_AUFIX, m_auFixValue);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAuFixSet, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgAuFixSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAuFixSet message handlers


BOOL CDlgAuFixSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strCoursePath = MACRO_COURSE_PATH + MACRO_MAINFRM->GetCourse();
	CString strProfilePath = strCoursePath + "\\Course.ini";
	char szBuf[256];
	::GetPrivateProfileString("repair", "TIMESHIFT", "0", szBuf, 256, strProfilePath);
	int nAuFixValue = atoi(szBuf);
	m_sliderAuFix.SetRange(0, 20);
	m_sliderAuFix.SetPos(nAuFixValue/100 + 10);

	SetTimer(1, 200, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAuFixSet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		int nAuFixValue = m_sliderAuFix.GetPos();
		CString strAuFixValue;
		strAuFixValue.Format("%dms", nAuFixValue * 100 - 1000);
		m_auFixValue.SetWindowText(strAuFixValue);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgAuFixSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
	int nAuFixValue = m_sliderAuFix.GetPos() * 100 - 1000;
	MACRO_MAINFRM->AudioFix(nAuFixValue);

	CDialogEx::OnOK();
}
