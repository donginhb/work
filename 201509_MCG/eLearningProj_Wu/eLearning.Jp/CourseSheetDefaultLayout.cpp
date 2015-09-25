// SheetDefaultLayout.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "CourseSheetDefaultLayout.h"
#include "PageDefaultOption.h"
#include "PageDefaultImage.h"
#include "PageDefaultOutlook.h"
#include "RelatedInformation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout

IMPLEMENT_DYNAMIC(CCourseSheetDefaultLayout, CBasicSheet)

	CCourseSheetDefaultLayout::CCourseSheetDefaultLayout(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Init();
}

CCourseSheetDefaultLayout::CCourseSheetDefaultLayout(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBasicSheet(pszCaption, pParentWnd, iSelectPage)
{
	Init();
}

void CCourseSheetDefaultLayout::Init()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_pPageImage = new CPageDefaultImage;
	AddPage(m_pPageImage);
	m_pPageImage ->m_PageDefaultImageFlag = m_CourseSheetDefaultFlag;

	m_pPageOption = new CPageDefaultOption;
	AddPage(m_pPageOption);
	m_pPageOption -> m_PageDefaultOptionFlag = m_CourseSheetDefaultFlag; 

	m_pPageOutlook = new CPageDefaultOutlook;
	AddPage(m_pPageOutlook);
	m_pPageOutlook ->m_PageDefaultOutLookFlag = m_CourseSheetDefaultFlag;

	m_pPageRelate = new CRelatedInformation;
	AddPage(m_pPageRelate);
	m_nCurSel = 0; 
}

CCourseSheetDefaultLayout::~CCourseSheetDefaultLayout()
{
	delete m_pPageImage;
	delete m_pPageOption;
	delete m_pPageOutlook;
	delete m_pPageRelate;
}


BEGIN_MESSAGE_MAP(CCourseSheetDefaultLayout, CBasicSheet)
	//{{AFX_MSG_MAP(CSheetDefaultLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout message handlers

BOOL CCourseSheetDefaultLayout::ContinueModal()
{
	return CPropertySheet::ContinueModal();
}

