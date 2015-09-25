// DlgCourseFolder.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgCourseFolder.h"
#include "resource.h"
#include "macro.h"
#include "BmpBtn.h"
#include "cmkapi.h"

// CDlgCourseFolder dialog
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgCourseFolder::CDlgCourseFolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCourseFolder::IDD, pParent)
{
	m_strIniPath = MACRO_INI_PATH;
}

CDlgCourseFolder::~CDlgCourseFolder()
{
}

void CDlgCourseFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCourseFolder, CDialog)
	//{{AFX_MSG_MAP(CDlgScreenCapture)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgCourseFolder message handlers
BOOL CDlgCourseFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBtnOpen = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;
	CRect rcEdit;
	GetDlgItem(IDC_EDIT_FILE_NAME)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.right = rcEdit.right + 20;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpen->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_SEL_FILE);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_FILE_NAME)->MoveWindow(rcEdit);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCourseFolder::OnOK() 
{
	CString strPath;
	GetDlgItem(IDC_EDIT_FILE_NAME)->GetWindowText(strPath);
	
	if (strPath == "")
	{
		W_SHOW_ERROR(IDS_INPUT_COURSE_FOLDER, this);
		return;
	}
	if (!::FileExists(strPath))
	{
		W_SHOW_ERROR(IDS_INPUT_COURSE_FOLDER, this);
		return ;
	}

	if (strPath.Right(1) != "\\")
	{
		strPath += "\\";
	}


	::WritePrivateProfileString("CourseFolder", "Name", "Ý’è", m_strIniPath);
	::WritePrivateProfileString("CourseFolder", "Dir", strPath, m_strIniPath);
	CDialog::OnOK();
}

void CDlgCourseFolder::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpen->DestroyWindow();
	delete m_pBtnOpen;
}

LRESULT CDlgCourseFolder::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_SEL_FILE:
			OnSelectFile();
		break;
	}

	return 0L;
}

void CDlgCourseFolder::OnSelectFile() 
{
	

	CString strLabel;
	strLabel.LoadString(IDS_SELECT_FOLDER);
	CString strFolderPath;
	if (::GetFolder(&strFolderPath, strLabel, m_hWnd, NULL, NULL))

	GetDlgItem(IDC_EDIT_FILE_NAME)->SetWindowText(strFolderPath);
}
