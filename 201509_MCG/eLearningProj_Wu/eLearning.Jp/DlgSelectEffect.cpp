// DlgSelectEffect.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelectEffect.h"
#include "ImageSetContainer.h"
#include "cmkapi.h"
#include "macro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectEffect dialog


CDlgSelectEffect::CDlgSelectEffect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectEffect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectEffect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelectEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectEffect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectEffect, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectEffect)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_PICTURE_SET_SELECTED, OnPictureSetSelected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectEffect message handlers

BOOL CDlgSelectEffect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	int nDlgWidth = nScreenX * 3 / 4;
	int nDlgHeight = nScreenY * 3 / 4;
	CRect rcPos;
	rcPos.left = (nScreenX - nDlgWidth) / 2;
	rcPos.top = (nScreenY - nDlgHeight) / 2;
	rcPos.right = rcPos.left + nDlgWidth;
	rcPos.bottom = rcPos.top + nDlgHeight;
	MoveWindow(rcPos);	

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc;
	GetDlgItem(IDCANCEL)->GetWindowRect(rc);
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	rc.right = rcClient.right - 10;
	rc.left = rc.right - nWidth;
	rc.bottom = rcClient.bottom - 10;
	rc.top = rc.bottom - nHeight;
	GetDlgItem(IDCANCEL)->MoveWindow(rc);

	rc.right = rc.left - 10;
	rc.left = rc.right - nWidth;
	GetDlgItem(IDOK)->MoveWindow(rc);

	CRect rcStatic;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(rcStatic);
	ScreenToClient(rcStatic);
	CRect rcContainer;
	rcContainer.bottom = rc.top - 15;
	rcContainer.top = rcStatic.bottom + 10;
	rcContainer.left = 10;
	rcContainer.right = rcClient.right - 10;

	m_pImageSet = new CImageSetContainer;
	m_pImageSet->m_strSelectedFile = m_strEffectPath;
	m_pImageSet->Create(WS_BORDER | WS_CHILD | WS_VISIBLE, rcContainer, this);
	m_pImageSet->SetTextFont(MACRO_SYS_FONT2);
	m_pImageSet->SetPath(MACRO_EFFECT_PATH);
	m_pImageSet->m_bShowFileName = TRUE;
	m_pImageSet->m_bShowFileExt = FALSE;
	m_pImageSet->m_bSelectable = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectEffect::OnOK() 
{
	CString strPath = "";
	m_pImageSet->GetSelectedFile(strPath);
	if (strPath == "")
	{
		W_SHOW_ERROR(IDS_SELECT_EFFECT_FIRST, this);
		return;
	}
	m_strEffectPath = strPath.Left(strPath.GetLength() - 3) + "vfx";
		
	CDialog::OnOK();
}

void CDlgSelectEffect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pImageSet->DestroyWindow();
	delete m_pImageSet;	
}

LRESULT CDlgSelectEffect::OnPictureSetSelected(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0L;
}

