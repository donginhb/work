// DlgSelVideoSrc.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgSelVideoSrc.h"
#include "BmpBtn.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelVideoSrc dialog


CDlgSelVideoSrc::CDlgSelVideoSrc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelVideoSrc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelVideoSrc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelVideoSrc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelVideoSrc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelVideoSrc, CDialog)
	//{{AFX_MSG_MAP(CDlgSelVideoSrc)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_EXISTING_FILE, OnRadioExistingFile)
	ON_BN_CLICKED(IDC_RADIO_RECORD, OnRadioRecord)
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_EN_KILLFOCUS(IDC_EDIT_FILE, OnKillfocusEditFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelVideoSrc message handlers

BOOL CDlgSelVideoSrc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBtnOpen = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;

	CRect rcCourse;
	GetDlgItem(IDCANCEL)->GetWindowRect(rcCourse);
	ScreenToClient(rcCourse);

	CRect rcEdit;
	GetDlgItem(IDC_EDIT_FILE)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.right = rcCourse.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpen->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_ASF);
	
	CButton* pRadioCapture = (CButton*)GetDlgItem(IDC_RADIO_RECORD);
	pRadioCapture->SetCheck(1);

	EnableFileSel(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelVideoSrc::EnableFileSel(BOOL b) 
{
	GetDlgItem(IDC_EDIT_FILE)->EnableWindow(b);
	m_pBtnOpen->EnableButton(b);
}

void CDlgSelVideoSrc::OnOK() 
{
	CButton* pRadioCapture = (CButton*)GetDlgItem(IDC_RADIO_RECORD);
	m_bCaptureNow = pRadioCapture->GetCheck();
	if (m_bCaptureNow)
	{
		HWND hRecorder = ::FindWindowEx(NULL, NULL, NULL, "ASF Recoder / Photon");
		if ( hRecorder)
		{
			W_SHOW_WARNING(IDS_RECORDER_IS_OPENED, this);
			return;
		}
	}
	else 
	{
		CString strFile;
		GetDlgItem(IDC_EDIT_FILE)->GetWindowText(strFile);
		if (strFile == "")
		{
			W_SHOW_WARNING(IDS_FILE_NOT_INPUT, this);
			return;
		}

		if (!::FileExists(strFile))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FILE_NOT_EXIST);
			strMsg.Format(strTmp, strFile);
			W_SHOW_ERROR_STR(strMsg, this);
			return;
		}
	}

	CDialog::OnOK();
}

void CDlgSelVideoSrc::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pBtnOpen)
	{
		m_pBtnOpen->DestroyWindow();
		delete m_pBtnOpen;
	}
}

void CDlgSelVideoSrc::SelASFFile() 
{
	CString StrTemp;
	StrTemp.LoadString(IDS_VIDEO_FILE);//IDS_MEDIA_FILE);
 	CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, 
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);

	if (pDlg->DoModal() != IDOK)
	{
		delete pDlg;
		return;
	}

	m_strPath = pDlg->GetPathName();
	CString strFileName = pDlg->GetFileName();
	GetDlgItem(IDC_EDIT_FILE)->SetWindowText(m_strPath);
	delete pDlg;
}


void CDlgSelVideoSrc::OnRadioExistingFile() 
{
	EnableFileSel(TRUE);	
}

void CDlgSelVideoSrc::OnRadioRecord() 
{
	EnableFileSel(FALSE);	
}

LRESULT CDlgSelVideoSrc::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_OPEN_ASF:
			SelASFFile();
		break;
	}

	return 0L;
}

void CDlgSelVideoSrc::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgSelVideoSrc::OnKillfocusEditFile() 
{
}

