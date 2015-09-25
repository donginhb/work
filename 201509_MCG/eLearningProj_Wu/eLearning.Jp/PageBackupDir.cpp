// PageBackupDir.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageBackupDir.h"
#include "ColGrid.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageBackupDir property page

IMPLEMENT_DYNCREATE(CPageBackupDir, CPropertyPage)

CPageBackupDir::CPageBackupDir() : CPropertyPage(CPageBackupDir::IDD)
{
	//{{AFX_DATA_INIT(CPageBackupDir)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageBackupDir::~CPageBackupDir()
{
}

void CPageBackupDir::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBackupDir)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageBackupDir, CPropertyPage)
	//{{AFX_MSG_MAP(CPageBackupDir)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_MESSAGE(WM_GRD_BTN_DOWN, OnGrdNote)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPageBackupDir::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageBackupDir::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBackupDir message handlers

BOOL CPageBackupDir::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CRect rcClient;
	GetClientRect(rcClient);
	int nWidth = rcClient.Width();

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pGrid = new CColGrid;
	m_pGrid->Create(rcGrid, this, 0);
	m_pGrid->SetTotalColNo(2);
	m_pGrid->SetHeadWidth((int)(nWidth * 0.1));
	m_pGrid->SetColWidth(0, (int)(nWidth * 0.2)); 
	m_pGrid->SetColWidth(1, (int)(nWidth * 0.6));
	CString strTitle;
	strTitle.LoadString(IDS_GRID_BACKUP_TITLE);
	m_pGrid->SetFixedRowLabel(strTitle);
	m_pGrid->u_bLastRowStar = TRUE;
	m_pGrid->u_bDragTitle = FALSE;
	m_pGrid->u_bSort = FALSE;
	m_pGrid->u_pMSGWnd = this;
	m_pGrid->SetGridFont(GetFont());
	m_pGrid->SetGridSelType(cGridHeadSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pGrid->SetColAttribAt(0, cType, cGridComboEdit);
	m_pGrid->SetColAttribAt(1, cType, cGridEditBtn);
	
	CString strDefault;
	strDefault.LoadString(IDS_DEFAULT);
	CString strBackup;
	strBackup.LoadString(IDS_BACKUP);
	CString strCombo = strDefault + "\\";
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		CString strIndex;
		strIndex.Format("%s%d\\", strBackup, i + 1);
		strCombo += strIndex;
	}
	m_pGrid->SetSeqAt(cSeqTextCombo, 0, strCombo);

	CString strIniPath = MACRO_INI_PATH;
	int nCount = ::GetPrivateProfileInt("General", "BackupDirCount", 0, strIniPath);

	CString strBuf;
	char* pBuf = new char[2048];
	for (i = 0; i < nCount; i++)
	{
		m_pGrid->AppendRow(1);

		strBuf.Format("Backup%d", i);
	
		::GetPrivateProfileString(strBuf, "Name", "", pBuf, 2048, strIniPath);
		m_pGrid->SetDataAt(i, 0, pBuf);
		
		::GetPrivateProfileString(strBuf, "Dir", "", pBuf, 2048, strIniPath);
		m_pGrid->SetDataAt(i, 1, pBuf);
	}
	delete [] pBuf;

	m_pGrid->AppendRow(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageBackupDir::OnDestroy() 
{
	CPropertyPage::OnDestroy();

	m_pGrid->DestroyWindow();
	delete m_pGrid;
}

void CPageBackupDir::OnButtonDelete() 
{
	if (m_pGrid->u_SelectedRow < 0)
	{
		W_SHOW_MESSAGE(IDS_SELECT_FILE_FIRST, this);
		return;
	}
	else
	{
		if (W_ASK_YESNO(IDS_BACKUP_DELETE_CONFIRM, this) == IDYES)
			m_pGrid->DelSelected();
	}	
}

void CPageBackupDir::OnOK() 
{
	CPropertyPage::OnOK();
}

BOOL CPageBackupDir::CheckColNull(int nCheckCol) 
{
	CString strTmp, strMsg;
	int nSize = m_pGrid->GetDataSize() - 1;
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = m_pGrid->GetDataAt(i, nCheckCol);
		str1.TrimLeft();
		if (str1 == "")
		{
			//add by buxiangyi
			//m_pSheet->SetActivePage(this);
			///////////////////////////////////////////
			strTmp.LoadString(IDS_TYPE_ROW_ERROR);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			m_pGrid->ClickRowCol(i, nCheckCol);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPageBackupDir::CheckFileExist() 
{
	CString strTmp, strMsg;
	int nSize = m_pGrid->GetDataSize() - 1;
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = m_pGrid->GetDataAt(i, 1);
		str1.TrimLeft();
		if (!::FileExists(str1))
		{
			//add by buxaingyi
			//m_pSheet->SetActivePage(this);
			//////////////////////////////
			strTmp.LoadString(IDS_DIR_ERROR);
			strMsg.Format(strTmp, i + 1);
			W_SHOW_ERROR_STR(strMsg, this);
			m_pGrid->ClickRowCol(i, 1);
			return FALSE;
		}
	}
	return TRUE;
}

//add by buxiangyi
//BOOL CPageBackupDir::OnApply() 
//{
//	if (!CheckColNull(0))
//		return FALSE; 
//
//	if (!CheckColNull(1))
//		return FALSE; 
//
//	if (!CheckFileExist())
//		return FALSE;
//
//	if (!CheckNameDup())
//		return FALSE;
//
//	CString strIniPath = MACRO_INI_PATH;
//	int nCount = m_pGrid->GetDataSize();
//	CString strCount;
//	strCount.Format("%d", nCount - 1);
//	::WritePrivateProfileString("General", "BackupDirCount", strCount, strIniPath);
//	CString strBuf, strTmp;
//	char* pBuf = new char[2048];
//	for (int i = 0; i < nCount - 1; i++)
//	{
//		strBuf.Format("Backup%d", i);
//	
//		strTmp = m_pGrid->GetDataAt(i, 0);
//		::WritePrivateProfileString(strBuf, "Name", strTmp, strIniPath);
//		
//		strTmp = m_pGrid->GetDataAt(i, 1);
//		::WritePrivateProfileString(strBuf, "Dir", strTmp, strIniPath);
//	}
//	delete [] pBuf;
//	
//	return CPropertyPage::OnApply();
//}
/////////////////////////////////////////////////////////
BOOL CPageBackupDir::CheckNameDup() 
{
	CString strTmp, strMsg;
	int nSize = m_pGrid->GetDataSize() - 1;
	for (int i = 0; i < nSize; i++)
	{
		CString str1 = m_pGrid->GetDataAt(i, 0);
		for (int j = i + 1;  j < nSize; j++)
		{
			CString str2 = m_pGrid->GetDataAt(j, 0);
			if (str1 == str2)
			{
				//add by buxiangyi
				//m_pSheet->SetActivePage(this);
				/////////////////////////////////////
				strTmp.LoadString(IDS_NAME_DUP);
				strMsg.Format(strTmp, j + 1);
				W_SHOW_ERROR_STR(strMsg, this);
				m_pGrid->ClickRowCol(j, 0);

				return FALSE;
			}
		}
	}

	return TRUE;
}

LRESULT CPageBackupDir::OnGrdNote(WPARAM wParam, LPARAM lParam)
{
	int nRow = HIWORD(lParam);
	int nCol = LOWORD(lParam);

	CString strLabel;
	strLabel.LoadString(IDS_SELECT_FOLDER);
	CString strFolderPath;
	CString strStartFolder = m_pGrid->GetDataAt(nRow, nCol);
	if (::GetFolder(&strFolderPath, strLabel, m_hWnd, NULL, strStartFolder))
	{
		m_pGrid->SetDataAt(nRow, nCol, strFolderPath);
		m_pGrid->RePaintRow(nRow, nRow);
	}

	return 0L;
}


void  CPageBackupDir::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!CheckColNull(0))
		return ; 

	if (!CheckColNull(1))
		return ; 

	if (!CheckFileExist())
		return ;

	if (!CheckNameDup())
		return ;

	CString strIniPath = MACRO_INI_PATH;
	int nCount = m_pGrid->GetDataSize();
	CString strCount;
	strCount.Format("%d", nCount - 1);
	::WritePrivateProfileString("General", "BackupDirCount", strCount, strIniPath);
	CString strBuf, strTmp;
	char* pBuf = new char[2048];
	for (int i = 0; i < nCount - 1; i++)
	{
		strBuf.Format("Backup%d", i);

		strTmp = m_pGrid->GetDataAt(i, 0);
		::WritePrivateProfileString(strBuf, "Name", strTmp, strIniPath);

		strTmp = m_pGrid->GetDataAt(i, 1);
		::WritePrivateProfileString(strBuf, "Dir", strTmp, strIniPath);
	}
	delete [] pBuf;
	CDialog ::OnOK();
}


void CPageBackupDir::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog ::OnCancel();
}
