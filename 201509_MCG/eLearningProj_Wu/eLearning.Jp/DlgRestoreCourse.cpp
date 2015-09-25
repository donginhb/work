// DlgRestoreCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgRestoreCourse.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRestoreCourse dialog


CDlgRestoreCourse::CDlgRestoreCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRestoreCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRestoreCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArraySelCourse = NULL;
}


void CDlgRestoreCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRestoreCourse)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRestoreCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgRestoreCourse)
	ON_MESSAGE(WM_CHECK_CHANGE, OnCheckChange)
	ON_BN_CLICKED(IDC_CHECK_DEL, OnCheckDel)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	ON_CBN_SELCHANGE(IDC_COMBO_DIR, OnSelchangeComboDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRestoreCourse message handlers

BOOL CDlgRestoreCourse::OnInitDialog() 
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

void CDlgRestoreCourse::LoadBackup() 
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

void CDlgRestoreCourse::OnOK() 
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
		if (W_ASK_YESNO(IDS_SURE_TO_DEL_AFTER_RESTORE, this) != IDYES)
			return;
	}
	//add by buxiangyi
	pBtn = (CButton*)GetDlgItem(IDC_CHECK_OVER);
	m_over = pBtn->GetCheck();

	if (m_over)
	{
		if (W_ASK_YESNO(IDS_SURE_TO_OVER_AFTER_RESTORE, this) != IDYES)
			return;
	}
	
	CDialog::OnOK();
}

void CDlgRestoreCourse::OnCancel() 
{
	CDialog::OnCancel();
}

LRESULT CDlgRestoreCourse::OnCheckChange(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CDlgRestoreCourse::OnCheckDel() 
{
}


void CDlgRestoreCourse::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

void CDlgRestoreCourse::OnSelchangeComboDir() 
{
	LoadBackup();	
}
