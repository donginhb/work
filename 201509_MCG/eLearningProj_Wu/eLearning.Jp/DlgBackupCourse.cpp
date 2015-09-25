// DlgBackupCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgBackupCourse.h"
#include "cmkapi.h"
#include "macro.h"
//add by buxiangyi
#include "PageBackupDir.h"
//////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupCourse dialog


CDlgBackupCourse::CDlgBackupCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBackupCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBackupCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArraySelCourse = NULL;
}


void CDlgBackupCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBackupCourse)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBackupCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgBackupCourse)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	ON_BN_CLICKED(IDC_CHECK_DEL, OnCheckDel)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BACKUP, &CDlgBackupCourse::OnClickedBackup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupCourse message handlers

BOOL CDlgBackupCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIniPath = MACRO_INI_PATH;
	int nCount = ::GetPrivateProfileInt("General", "BackupDirCount", 0, strIniPath);

	CString strBuf;
	char* pBuf = new char[2048];
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	int i = 0;
	for (i = 0; i < nCount; i++)
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

	CStringArray strArray;
	GetDir(MACRO_COURSE_PATH, strArray);
	for (i = 0; i < strArray.GetSize(); i++)
		m_clCourse.AddString(" " + strArray[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBackupCourse::OnOK() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	int nSel = pCombo->GetCurSel();
	if (nSel < 0)
		return;
	m_strPath = m_strArrayDir[nSel];

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
		
		if (!m_pstrArraySelCourse->GetSize())
		{
			W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
			return;
		}
	}
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DEL);
	m_bDel = pBtn->GetCheck();

	if (m_bDel)
	{
		if (W_ASK_YESNO(IDS_SURE_TO_DEL_AFTER_BKUP, this) != IDYES)
			return;
	}

	CDialog::OnOK();
}

void CDlgBackupCourse::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgBackupCourse::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CDlgBackupCourse::OnCheckDel() 
{
}

void CDlgBackupCourse::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

//add by buxiangyi
void CDlgBackupCourse::OnClickedBackup()
{
	// TODO: Add your control notification handler code here
	CPageBackupDir dlg;
	dlg.DoModal();
	CString strIniPath = MACRO_INI_PATH;
	int nCount = ::GetPrivateProfileInt("General", "BackupDirCount", 0, strIniPath);

	CString strBuf;
	char* pBuf = new char[2048];
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	pCombo->ResetContent();
	m_strArrayDir.RemoveAll();
	int i = 0;
	for (i = 0; i < nCount; i++)
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
}
//////////////////////