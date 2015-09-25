// SheetOption.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "SheetOption.h"
#include "PageRecordOption.h"
#include "PageMisc.h"
#include "PageBackupDir.h"
#include "PageDefaultCaption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetOption

IMPLEMENT_DYNAMIC(CSheetOption, CBasicSheet)

CSheetOption::CSheetOption(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Init();
}

CSheetOption::CSheetOption(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

void CSheetOption::Init()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_pRecordOption = new CPageRecordOption;
	AddPage(m_pRecordOption);

	m_pMisc = new CPageMisc;
	AddPage(m_pMisc);
	
	//add by buxiangyi
	/*m_pBackupDir = new CPageBackupDir;
	m_pBackupDir->m_pSheet = this;
	AddPage(m_pBackupDir);*/
	////////////////////////////

	m_pDefaultCaption = new CPageDefaultCaption;
	AddPage(m_pDefaultCaption);
}

CSheetOption::~CSheetOption()
{
	delete m_pRecordOption;
	//add by buxiangyi
	//delete m_pBackupDir;
	///////////////
	delete m_pMisc;
	delete m_pDefaultCaption;
}


BEGIN_MESSAGE_MAP(CSheetOption, CBasicSheet)
	//{{AFX_MSG_MAP(CSheetOption)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetOption message handlers

BOOL CSheetOption::OnInitDialog() 
{
	BOOL bResult = CBasicSheet::OnInitDialog();
	
//	SetActivePage(m_nCurSel);
	
	return bResult;
}

BOOL CSheetOption::ContinueModal()
{
	return CPropertySheet::ContinueModal();
}
