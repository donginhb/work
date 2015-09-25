// SetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "PageSelPPT.h"
#include "NewWizDialog.h"
#include "BmpBtn.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSelPPT dialog


CPageSelPPT::CPageSelPPT(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CPageSelPPT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSelPPT)
	//}}AFX_DATA_INIT
}


void CPageSelPPT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSelPPT)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSelPPT, CNewWizPage)
	//{{AFX_MSG_MAP(CPageSelPPT)
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSelPPT message handlers

BOOL CPageSelPPT::OnInitDialog() 
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

	m_pBtnOpenPPT = new CBmpBtn(IDB_OPEN2, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN2);
	csBtn.cx /= 4;

	CRect rcEdit;
	GetDlgItem(IDC_EDIT_PPT)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.left = rcEdit.right + 20;
	rect.right = rect.left + csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenPPT->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_PPT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPageSelPPT::OnWizardNext()
{
	CString strFile;
	GetDlgItem(IDC_EDIT_PPT)->GetWindowText(strFile);

	strFile.TrimLeft();
	if (strFile == "")
    {
		W_SHOW_MESSAGE(IDS_SEL_PPT_FIRST, this);
		return -1;
	}

    m_strData = strFile;
	return IDW_INPUT_NAME;
}

void CPageSelPPT::OnSetActive()
{
	CString strTitle;
	strTitle.LoadString(IDS_SEL_PPT);
	m_pParent->SetTitle(strTitle);
}

BOOL CPageSelPPT::OnKillActive()
{
	return TRUE;
}

void CPageSelPPT::OnDestroyPage()
{
	m_pBtnOpenPPT->DestroyWindow();
	delete m_pBtnOpenPPT;
}

void CPageSelPPT::OnSelectPPT() 
{
	CString StrTemp;
	StrTemp.LoadString(IDS_PPT_FILE);
 	CFileDialog* pDlg = new CFileDialog(TRUE, "ppt", NULL, //pBuf
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);
	if (pDlg->DoModal() != IDOK)
	{
		delete pDlg;
		return;
	}
	
	CString strFileName = pDlg->GetFileName();
	CString strPath = pDlg->GetPathName();
	delete pDlg;

	GetDlgItem(IDC_EDIT_PPT)->SetWindowText(strPath);
}

LRESULT CPageSelPPT::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_OPEN_PPT:
			OnSelectPPT();
		break;
	}

	return 0L;
}

LRESULT CPageSelPPT::OnWizardBack()
{
	CString strFile;
	GetDlgItem(IDC_EDIT_PPT)->GetWindowText(strFile);

    m_strData = strFile;
	
	return IDW_SETUP;
}
