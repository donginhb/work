// SheetDefaultLayout.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "SheetDefaultLayout.h"
#include "PageDefaultOption.h"
#include "PageDefaultImage.h"
#include "PageDefaultLayout.h"
#include "PageDefaultOutlook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout

IMPLEMENT_DYNAMIC(CSheetDefaultLayout, CBasicSheet)

CSheetDefaultLayout::CSheetDefaultLayout(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Init();
}

CSheetDefaultLayout::CSheetDefaultLayout(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

void CSheetDefaultLayout::Init()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_pPageLayout = new CPageDefaultLayout;
	m_pPageLayout->Layoutflag = FALSE;
	AddPage(m_pPageLayout);

	m_pPageImage = new CPageDefaultImage;
	AddPage(m_pPageImage);

	m_pPageOption = new CPageDefaultOption;
	AddPage(m_pPageOption);

	m_pPageOutlook = new CPageDefaultOutlook;
	AddPage(m_pPageOutlook);

	m_nCurSel = 0; 
}

CSheetDefaultLayout::~CSheetDefaultLayout()
{
	delete m_pPageLayout;
	delete m_pPageImage;
	delete m_pPageOption;
	delete m_pPageOutlook;
}


BEGIN_MESSAGE_MAP(CSheetDefaultLayout, CBasicSheet)
	//{{AFX_MSG_MAP(CSheetDefaultLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout message handlers

BOOL CSheetDefaultLayout::ContinueModal()
{
	return CPropertySheet::ContinueModal();
}

