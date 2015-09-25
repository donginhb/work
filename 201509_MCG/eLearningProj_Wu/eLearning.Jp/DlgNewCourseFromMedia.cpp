// DlgNewCourseFromMedia.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgNewCourseFromMedia.h"
#include "BmpBtn.h"
#include "cmkapi.h"
#include "macro.h"
#include "DlgCourseDup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourseFromMedia dialog


CDlgNewCourseFromMedia::CDlgNewCourseFromMedia(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewCourseFromMedia::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewCourseFromMedia)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strCourseName = "";
	m_bOverwrite = FALSE;
}


void CDlgNewCourseFromMedia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewCourseFromMedia)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewCourseFromMedia, CDialog)
	//{{AFX_MSG_MAP(CDlgNewCourseFromMedia)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_HTML, OnRadioHtml)
	ON_BN_CLICKED(IDC_RADIO_PPT, OnRadioPpt)
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESENT, OnSelchangeComboPresent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourseFromMedia message handlers

BOOL CDlgNewCourseFromMedia::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBtnOpenPPT = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;

	CRect rcCourse;
	GetDlgItem(IDC_COMBO_PRESENT)->GetWindowRect(rcCourse);
	ScreenToClient(rcCourse);

	CRect rcEdit;
	GetDlgItem(IDC_EDIT_PPT)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.right = rcCourse.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenPPT->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_PPT);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_PPT)->MoveWindow(rcEdit);
	
	m_pBtnOpenMedia = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	GetDlgItem(IDC_EDIT_COURSE)->GetWindowRect(rcCourse);
	ScreenToClient(rcCourse);

	GetDlgItem(IDC_EDIT_MEDIA)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);

	rect.right = rcCourse.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenMedia->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_MEDIA);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_MEDIA)->MoveWindow(rcEdit);

	CStringArray strArray;
	::GetDir(MACRO_PRESENT_PATH, strArray);
	::SortArray(strArray);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);

	CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_PPT);
	pCheck->SetCheck(1);
	OnRadioPpt();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNewCourseFromMedia::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpenPPT->DestroyWindow();
	delete m_pBtnOpenPPT;

	m_pBtnOpenMedia->DestroyWindow();
	delete m_pBtnOpenMedia;
}

void CDlgNewCourseFromMedia::OnRadioHtml() 
{
	GetDlgItem(IDC_COMBO_PRESENT)->EnableWindow(TRUE);	
	GetDlgItem(IDC_EDIT_PPT)->EnableWindow(FALSE);	
	m_pBtnOpenPPT->EnableButton(FALSE);
}

void CDlgNewCourseFromMedia::OnRadioPpt() 
{
	GetDlgItem(IDC_COMBO_PRESENT)->EnableWindow(FALSE);	
	GetDlgItem(IDC_EDIT_PPT)->EnableWindow(TRUE);
	m_pBtnOpenPPT->EnableButton(TRUE);
}

LRESULT CDlgNewCourseFromMedia::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_OPEN_PPT:
			OnSelectPPT();
		break;

		case ID_BTN_OPEN_MEDIA:
			OnSelectMedia();
		break;
	}

	return 0L;
}

void CDlgNewCourseFromMedia::OnSelectPPT() 
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

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COURSE);
	CString strCourseName;
	pEdit->GetWindowText(strCourseName);

	int nDot = strFileName.ReverseFind('.');
	if (nDot >= 0)
	{
		strCourseName = strFileName.Left(nDot);
		pEdit->SetWindowText(strCourseName);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}

	GetDlgItem(IDC_EDIT_PPT)->SetWindowText(strPath);
}

void CDlgNewCourseFromMedia::OnSelectMedia() 
{
	CString StrTemp;
	StrTemp.LoadString(IDS_VIDEO_FILE);//IDS_MEDIA_FILE);
 	CFileDialog* pDlg = new CFileDialog(TRUE, "mp4", NULL, 
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);
	if (pDlg->DoModal() != IDOK)
	{
		delete pDlg;
		return;
	}
	
	CString strPath = pDlg->GetPathName();
	delete pDlg;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEDIA);
	pEdit->SetWindowText(strPath);
}

void CDlgNewCourseFromMedia::OnSelchangeComboPresent() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	CString strPresent;
	pCombo->GetWindowText(strPresent);	

	GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(strPresent);
}

BOOL CDlgNewCourseFromMedia::Commit() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COURSE);
	CString strCourse;
	pEdit->GetWindowText(strCourse);
	
	strCourse.TrimLeft();
	strCourse.TrimRight();

	if (strCourse == "")
	{
		W_SHOW_WARNING(IDS_COURSE_NULL, this);
		pEdit->SetFocus();
		return FALSE;
	}

	if (!::IsFileNameValid(strCourse))
	{
		W_SHOW_WARNING(IDS_INVALID_COURSE, this);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return FALSE;
	}

	CString strMediaFile;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEDIA);
	pEdit->GetWindowText(strMediaFile);
	strMediaFile.TrimLeft();
	strMediaFile.TrimRight();
	if (strMediaFile == "")
	{
		W_SHOW_WARNING(IDS_MEDIA_FILE_NULL, this);
		pEdit->SetFocus();
		return FALSE;
	}

	if (!::FileExists(strMediaFile))
	{
		W_SHOW_WARNING(IDS_MEDIA_NOT_EXIST, this);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return FALSE;
	}

	if (strMediaFile.Find(".mp4") == 0)
	{
		W_SHOW_ERROR(IDS_MEDIA_NO_SUPPORT, this);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return FALSE;
	}

	CButton* pPPTCheck = (CButton*)GetDlgItem(IDC_RADIO_PPT);
	m_bPPT = pPPTCheck->GetCheck();
	if (m_bPPT)
	{
		CString strPPT;
		GetDlgItem(IDC_EDIT_PPT)->GetWindowText(strPPT);
		strPPT.TrimLeft();
		strPPT.TrimRight();
		if (strPPT == "")
		{
			W_SHOW_WARNING(IDS_PPT_FILE_NULL, this);
			return FALSE;
		}

		if (!::FileExists(strPPT))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FILE_NOT_EXIST);
			strMsg.Format(strTmp, strPPT);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}

		m_strPresentSrc = strPPT;
	}
	else
	{
		CString strPresent;
		GetDlgItem(IDC_COMBO_PRESENT)->GetWindowText(strPresent);
		if (strPresent == "")
		{
			W_SHOW_WARNING(IDS_HTML_FILE_NULL, this);
			return FALSE;
		}
		
		m_strPresentSrc = strPresent;
	}

	if (::FileExists(MACRO_COURSE_PATH + strCourse))
	{
		CDlgCourseDup dlg;
		dlg.m_strPath = MACRO_COURSE_PATH;
		dlg.m_strStatic.LoadString(IDS_NEW_COURSE_FOUND); 
		dlg.m_strRadioOverwrite.LoadString(IDS_NEW_COURSE_OVERWRITE); 
		dlg.m_strRadioNewName.LoadString(IDS_NEW_COURSE_NEWNAME); 
		dlg.m_strTitle.LoadString(IDS_NEW_COURSE_TITLE);
		dlg.m_strCourse = strCourse;
		if (dlg.DoModal() == IDCANCEL)
			return FALSE;
		GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(dlg.m_strCourse);
		strCourse = dlg.m_strCourse;
		m_bOverwrite = dlg.m_bOverWrite;
	}

	m_strCourseName = strCourse;
	m_strMediaPath = strMediaFile;

	return TRUE;
}

void CDlgNewCourseFromMedia::OnOK() 
{
	if (Commit())	
		CDialog::OnOK();
}
