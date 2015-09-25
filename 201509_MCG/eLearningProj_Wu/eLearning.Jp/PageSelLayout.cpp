// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelLayout.h"
#include "NewWizDialog.h"
#include "cmkapi.h"
#include "ImageCell.h"
#include "macro.h"
#include "DlgTreeSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelLayout dialog

#define constSmallLayoutWidth	180
#define constSmallLayoutHeight	154


CPageSelLayout::CPageSelLayout(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelLayout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelLayout)
	//}}AFX_DATA_INIT
	m_strData = "";
}


void CPageSelLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelLayout)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelLayout, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelLayout)
	ON_MESSAGE(WM_IMAGE_CELL_DBL_CLICK, OnCellDblClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelLayout message handlers

BOOL CPageSelLayout::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_Font.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, //_T("Arial"));
#ifdef _JAPANESE
	_T("MS UI Gothic"));
#else
	_T("Arial"));
#endif

	m_Font2.CreateFont(-16, 0, 0, 0, 
		FW_NORMAL, TRUE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, //_T("Arial"));
#ifdef _JAPANESE
	_T("MS UI Gothic"));
#else
	_T("Arial"));
#endif

    GetDlgItem(IDC_STATIC_STEP)->SetFont(&m_Font2, TRUE);

	m_pLayoutCell = new CImageCell;
	m_pLayoutCell->m_nMenuID = IDR_MENU_CHANGE_BMP;
	m_pLayoutCell->m_bShowSize = FALSE;
	m_pLayoutCell->m_pFont = MACRO_SYS_FONT2;
	m_pLayoutCell->m_bWithFrame = TRUE;
	m_pLayoutCell->SetTitle("");
	m_pLayoutCell->m_rgbTitleBg = RGB(0, 0, 128);
	m_pLayoutCell->m_rgbTitleFg = RGB(255, 255, 255);
	CRect rect;
	GetDlgItem(IDC_STATIC_PICTURE)->GetWindowRect(rect);
    ScreenToClient(rect);

	CString strTmp = MACRO_TEMPLATE_PATH;
	CString strLayoutPath = strTmp + "_slayout.bmp";
    int nW = rect.Width();
	int nH = rect.Height();
	rect.left = rect.left + (nW - constSmallLayoutWidth) / 2;
	rect.top = rect.top + (nH - constSmallLayoutHeight) / 2;
    rect.right = rect.left + constSmallLayoutWidth;
    rect.bottom = rect.top + constSmallLayoutHeight;
	m_pLayoutCell->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_LAYOUT_CELL);
	m_pLayoutCell->SetPath(strLayoutPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPageSelLayout::OnWizardNext()
{
	m_strData = m_strPath;

	return 0;
}

void CPageSelLayout::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_LAYOUT);
	m_pParent->SetTitle(strTitle);
}

BOOL CPageSelLayout::OnKillActive()
{
	return TRUE;
}

void CPageSelLayout::OnDestroyPage()
{	
	m_pLayoutCell->DestroyWindow();
	delete m_pLayoutCell;
}

void CPageSelLayout::OnSelectLayout() 
{
}


LRESULT CPageSelLayout::OnWizardBack()
{
	m_strData = "";

	return 0;
}

LRESULT CPageSelLayout::OnCellDblClicked(WPARAM wParam, LPARAM lParam)
{
	SelectLayout();

	return 0L;
}

BOOL CPageSelLayout::SelectLayout()
{
	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = MACRO_LAYOUT_PATH;
	dlg.m_bSelectable = FALSE;
	CString strTitle;
	strTitle.LoadString(IDS_LAYOUT_SELECT);
	dlg.m_strTitle = strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(TRUE);
	dlg.ShowFileExt(FALSE);
	dlg.SetShowType(2);
	dlg.m_nWidth = 500;
	dlg.m_nHeight = 515;
	if(dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.m_strSelectedFile;
		m_pLayoutCell->SetPath(strPath);
		m_strPath = strPath;
	}

	return TRUE;
}
