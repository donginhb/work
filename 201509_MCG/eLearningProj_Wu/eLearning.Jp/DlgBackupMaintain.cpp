// DlgBackupMaintain.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgBackupMaintain.h"
#include "DlgInputCourse.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupMaintain dialog


CDlgBackupMaintain::CDlgBackupMaintain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBackupMaintain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBackupMaintain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBackupMaintain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBackupMaintain)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBackupMaintain, CDialog)
	//{{AFX_MSG_MAP(CDlgBackupMaintain)
	ON_COMMAND(ID_DEL_COURSE, OnDelCourse)
	ON_BN_CLICKED(ID_RENAME_COURSE, OnRenameCourse)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	ON_CBN_SELCHANGE(IDC_COMBO_DIR, OnSelchangeComboDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupMaintain message handlers

BOOL CDlgBackupMaintain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
	{
		pCombo->SetCurSel(0);
		LoadBackup();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBackupMaintain::LoadBackup() 
{
	m_clCourse.ResetContent();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	int nSel = pCombo->GetCurSel();
	if (nSel < 0)
		return;

	CStringArray strArray;
	GetDir(m_strArrayDir[nSel], strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
		m_clCourse.AddString(" " + strArray[i]);
}

void CDlgBackupMaintain::OnDelCourse() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DIR);
	int nSel = pCombo->GetCurSel();
	if (nSel < 0)
		return;

	CString strPath = m_strArrayDir[nSel];
	if (strPath.Right(1) != "\\")
		strPath += "\\";

	CStringArray strArraySel;
	if (GetSelCourse(strArraySel))
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_SURE_TO_DELETE);
		strMsg.Format(strTmp, strArraySel.GetSize());
		if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
		{
			for (int i = 0; i < strArraySel.GetSize(); i++)
			{
				CString strCourse = strArraySel[i];
				::DelTree(strPath + strCourse);
			}

			LoadBackup();
		}
	}
}

BOOL CDlgBackupMaintain::GetSelCourse(CStringArray& strArraySel) 
{
	strArraySel.RemoveAll();
	CString strCourse;
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
	{
		if (m_clCourse.GetCheck(iItem))
		{
			m_clCourse.GetText(iItem, strCourse);
			strCourse.TrimLeft();
			strArraySel.Add(strCourse);
		}
	}

	if (!strArraySel.GetSize())
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return FALSE;
	}

	return TRUE;
}

void CDlgBackupMaintain::OnRenameCourse() 
{
	int n = m_clCourse.GetCurSel();	
	if (n < 0)
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return;
	}

	CString strOldName;
	m_clCourse.GetText(n, strOldName);
	strOldName.TrimLeft();

	CString strNewName;
	CDlgInputCourse dlg;
	dlg.m_strCourseName = strOldName;
	CString strDestCourse;
	if (dlg.DoModal() == IDOK)
	{
		strNewName = dlg.m_strCourseName;
		CString strOldPath = MACRO_BACKUP_PATH + strOldName;
		CString strNewPath = MACRO_BACKUP_PATH + strNewName;
		if (rename(strOldPath, strNewPath) == 0)
		{
			W_SHOW_MESSAGE(IDS_RENAME_COURSE_OK, this);
			LoadBackup();
		}
		else

			W_SHOW_ERROR(IDS_RENAME_COURSE_ERROR, this);
	}
}

void CDlgBackupMaintain::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgBackupMaintain::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CDlgBackupMaintain::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

void CDlgBackupMaintain::OnSelchangeComboDir() 
{
	LoadBackup();	
}
