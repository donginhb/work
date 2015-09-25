// SheetSetting.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "SheetSetting.h"
#include "PageMediaServer.h"
#include "PageWebServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetSetting

IMPLEMENT_DYNAMIC(CSheetSetting, CPropertySheet)

CSheetSetting::CSheetSetting(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Init();
}

CSheetSetting::CSheetSetting(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

CSheetSetting::~CSheetSetting()
{
	delete m_pWebSrv;
	delete m_pMediaSrv;
}

void CSheetSetting::Init()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_pWebSrv = new CPageWebServer;
	AddPage(m_pWebSrv);

	m_pMediaSrv = new CPageMediaServer;
	AddPage(m_pMediaSrv);

	m_nSelPage = 0;
}

BEGIN_MESSAGE_MAP(CSheetSetting, CPropertySheet)
	//{{AFX_MSG_MAP(CSheetSetting)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetSetting message handlers

BOOL CSheetSetting::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetActivePage(m_nSelPage);
	
	return bResult;
}
