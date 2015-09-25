// DlgNewCourse.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgNewCourse.h"
#include "cmkapi.h"
#include "Macro.h"
#include "BmpBtn.h"
#include "DlgCourseDup.h"
#include "DlgDefaultProfileSet.h"
#include "DlgScreenProfileSet.h"
#include "DlgSelDevice.h"
#include "Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourse dialog


CDlgNewCourse::CDlgNewCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewCourse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewCourse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bRec = FALSE;
	m_bNoRecord = FALSE;
	m_strCourseName = "";
	m_bOverwrite = FALSE;
}


void CDlgNewCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewCourse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewCourse, CDialog)
	//{{AFX_MSG_MAP(CDlgNewCourse)
	ON_BN_CLICKED(ID_REC, OnRec)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_EN_KILLFOCUS(IDC_EDIT_COURSE, OnKillfocusEditCourse)
	ON_BN_CLICKED(IDC_RADIO_HTML, OnRadioHtml)
	ON_BN_CLICKED(IDC_RADIO_PPT, OnRadioPpt)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESENT, OnSelchangeComboPresent)
	ON_BN_CLICKED(ID_DEFAULT_PROFILE, OnDefaultProfile)
	ON_BN_CLICKED(ID_SCREEN_PROFILE, OnScreenProfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewCourse message handlers

void CDlgNewCourse::OnOK() 
{
	if (Commit())
		CDialog::OnOK();
}

BOOL CDlgNewCourse::Commit() 
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

		m_strSrc = strPPT;
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
		
		m_strSrc = strPresent;
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

	return TRUE;
}

BOOL CDlgNewCourse::OnInitDialog() 
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

	if (m_bNoRecord)
		GetDlgItem(ID_REC)->ShowWindow(SW_HIDE);
	
	BOOL bRet = TRUE;
	if (m_strCourseName != "")
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COURSE);
		pEdit->SetWindowText(m_strCourseName);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		bRet = FALSE;
	}

	CStringArray strArray;
	::GetDir(MACRO_PRESENT_PATH, strArray);
	::SortArray(strArray);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	for (int i = 0; i < strArray.GetSize(); i++)
		pCombo->AddString(strArray[i]);

	CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_PPT);
	pCheck->SetCheck(1);
	OnRadioPpt();
 
#ifdef _BUNDLE
	GetDlgItem(ID_SCREEN_PROFILE)->EnableWindow(FALSE);
#endif

	return bRet;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNewCourse::OnRec() 
{
	if (Commit())
	{
		CDialog::OnOK();
		m_bRec = TRUE;
	}
}

void CDlgNewCourse::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pBtnOpenPPT->DestroyWindow();
	delete m_pBtnOpenPPT;
}

void CDlgNewCourse::OnSelectPPT() 
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

LRESULT CDlgNewCourse::OnButtonUp(WPARAM wParam, LPARAM lParam)
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

void CDlgNewCourse::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgNewCourse::OnKillfocusEditCourse() 
{
}

void CDlgNewCourse::OnRadioHtml() 
{
	GetDlgItem(IDC_COMBO_PRESENT)->EnableWindow(TRUE);	
	GetDlgItem(IDC_EDIT_PPT)->EnableWindow(FALSE);	
	m_pBtnOpenPPT->EnableButton(FALSE);
}

void CDlgNewCourse::OnRadioPpt() 
{
	GetDlgItem(IDC_COMBO_PRESENT)->EnableWindow(FALSE);	
	GetDlgItem(IDC_EDIT_PPT)->EnableWindow(TRUE);
	m_pBtnOpenPPT->EnableButton(TRUE);
}

void CDlgNewCourse::OnSelchangeComboPresent() 
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESENT);
	CString strPresent;
	pCombo->GetWindowText(strPresent);	

	GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(strPresent);
}

void CDlgNewCourse::OnDefaultProfile() 
{
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_info.ini";
	CDlgDefaultProfileSet dlg;
	dlg.m_strIniPath = strIniPath;
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
	dlg.m_bNoVideo = atoi(szBuf);
	dlg.DoModal();
}

void CDlgNewCourse::OnScreenProfile() 
{
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_info.ini";
	CDevice dlg;
	dlg.m_strIniPath = strIniPath;

	
	dlg.DoModal();
}
