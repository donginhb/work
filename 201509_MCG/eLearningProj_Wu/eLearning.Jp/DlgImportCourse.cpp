// DlgImportCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgImportCourse.h"
#include "registry.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImportCourse dialog


CDlgImportCourse::CDlgImportCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImportCourse)
	//}}AFX_DATA_INIT
	m_strPath = "";
}


void CDlgImportCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImportCourse)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImportCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgImportCourse)
	ON_CBN_SELCHANGE(IDC_COMBO_VERSION, OnSelchangeComboVersion)
	ON_BN_CLICKED(ID_SELECTALL, OnSelectall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImportCourse message handlers

BOOL CDlgImportCourse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VERSION);

	CString strURL = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Photon\\MCG\\";
	CString strValue;
	if (::RegGetKey(strURL + "2.0\\Path", strValue))
	{
		m_strArrayPath.Add(strValue);
		pCombo->AddString("2.0");
		pCombo->SetCurSel(0);
		LoadCourse(strValue);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImportCourse::OnSelchangeComboVersion() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VERSION);
	int nSel = pCombo->GetCurSel();	
	if (nSel < 0)
	{
		m_clCourse.ResetContent();
		return;
	}

	CString strPath = m_strArrayPath[nSel];
	LoadCourse(strPath);
}

void CDlgImportCourse::LoadCourse(CString strPath) 
{
	m_clCourse.ResetContent();

	strPath += "Courses";
	CStringArray strArray;
	GetDir(strPath, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
		m_clCourse.AddString(" " + strArray[i]);
}

void CDlgImportCourse::OnSelectall() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

void CDlgImportCourse::OnOK() 
{
	CString strCourse;
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
	{
		if (m_clCourse.GetCheck(iItem))
		{
			m_clCourse.GetText(iItem, strCourse);
			strCourse.TrimLeft();
			m_pstrArrayImportCourse->Add(strCourse);
		}
	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VERSION);
	int nSel = pCombo->GetCurSel();	
	if (nSel >= 0)
		m_strPath = m_strArrayPath[nSel];

	CDialog::OnOK();
}
