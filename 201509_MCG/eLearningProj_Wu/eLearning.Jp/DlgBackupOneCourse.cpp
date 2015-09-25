// DlgBackupOneCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgBackupOneCourse.h"
#include "macro.h"
#include "cmkapi.h"
#include "elearningview.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupOneCourse dialog


CDlgBackupOneCourse::CDlgBackupOneCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBackupOneCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBackupOneCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBackupOneCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBackupOneCourse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBackupOneCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgBackupOneCourse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupOneCourse message handlers

BOOL CDlgBackupOneCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(m_strCourse);

	CString strIniPath = MACRO_INI_PATH;
	int nCount = ::GetPrivateProfileInt("General", "BackupDirCount", 0, strIniPath);

	CString strBuf;
	char* pBuf = new char[2048];
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	for (int i = 0; i < nCount; i++)
	{
		strBuf.Format("Backup%d", i);
	
		::GetPrivateProfileString(strBuf, "Name", "", pBuf, 2048, strIniPath);
		pCombo->AddString(pBuf);

		::GetPrivateProfileString(strBuf, "Dir", "", pBuf, 2048, strIniPath);
		m_strArrayDir.Add(pBuf);
	}
	delete [] pBuf;
	if (nCount > 0)
		pCombo->SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBackupOneCourse::OnOK() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	int nSel = pCombo->GetCurSel();
	if (nSel < 0)
		return;
	m_strPath = m_strArrayDir[nSel];

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DEL);
	m_bDel = pBtn->GetCheck();

	if (m_bDel)
	{
		if (W_ASK_YESNO(IDS_SURE_TO_DEL_AFTER_BKUP, this) != IDYES)
			return;
	}
	
	int n = MACRO_VIEW->BackupOneCourse(m_strPath, m_strCourse, m_bDel);
	if (n <= 0)
		return;

	CDialog::OnOK();
}
