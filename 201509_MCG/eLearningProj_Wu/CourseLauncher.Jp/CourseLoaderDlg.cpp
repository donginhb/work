// CourseLoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "CourseLoaderDlg.h"
#include "cmkapi.h"
#include "SheetSetting.h"
#include "DlgSQL.h"
#include "EncodeApi.h"
#include "DlgTransCourse.h"
#include "DlgCourseDupFTP.h"
#include "DlgOption.h"
#include "DlgLaunchOK.h"
#include "DlgRegenIndex.h"
#include "DlgSelType.h"
#include "DlgAskCourseDup.h"
#include "..\elearning.jp\ColGrid.h"
#include "..\elearning.jp\BmpBtn.h"
#include "DlgViewSelect.h"
#include "DlgTypeSet.h"
#include "DlgInputScornInfo.h"
#include "..\_Shared\HelperKit.h"
#include "cscorm.h"
#include "..\ZipArchive\ZipArchive.h"	

#include <atlbase.h>
#include <direct.h>
#include <dshow.h>

#include "..\ComCapture\ComCapture\UserErrMsg.h"
//#include "..\MediaUtility\MediaUtility_i.c"
#include "..\MediaKit\MediaKit_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MACRO_RECORD_VIDEO(x)				(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\video_edit.lrn")
#define MACRO_RECORD_TAG(x)					(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\video_tag.lrn")
#define MACRO_RECORD_AUDIO_MIX(x)			(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\audio_mix.lrn")
#define MACRO_RECORD_AUDIO_REPLACE(x)		(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\audio_replace.lrn")
#define MACRO_RECORD_VIDEO_TEXT(x)			(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\video_text.lrn")
#define MACRO_RECORD_VIDEO_EFFECT(x)		(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\video_effect.lrn")
#define MACRO_PRE_RECORD_VIDEO(x)			(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_video_edit.lrn")
#define MACRO_PRE_RECORD_TAG(x)				(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_video_tag.lrn")
#define MACRO_PRE_RECORD_AUDIO_MIX(x)		(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_audio_mix.lrn")
#define MACRO_PRE_RECORD_AUDIO_REPLACE(x)	(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_audio_replace.lrn")
#define MACRO_PRE_RECORD_VIDEO_TEXT(x)		(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_video_text.lrn")
#define MACRO_PRE_RECORD_VIDEO_EFFECT(x)	(::GetSysPath() + "Courses\\" + CString(x) + "\\video\\pre_video_effect.lrn")


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderDlg dialog

CCourseLoaderDlg::CCourseLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCourseLoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCourseLoaderDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strCourse = "";

	CoInitialize(NULL);
}

void CCourseLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCourseLoaderDlg)
	DDX_Control(pDX, IDC_LIST_COURSE, m_clCourse);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCourseLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CCourseLoaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_PROGRESS_REPORT, OnProgress)	
	ON_COMMAND(ID_SERVER_SETTING, OnServerSetting)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_TYPE_SETTING, OnTypeSetting)
	ON_COMMAND(ID_TRANSMIT, OnTransmit)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_COMMAND(ID_VIEW_COURSE, OnViewCourse)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_FTP, OnRadioFtp)
	ON_BN_CLICKED(IDC_RADIO_FTP_LOCAL, OnRadioFtpLocal)
	ON_BN_CLICKED(IDC_RADIO_FTP_MANUAL, OnRadioFtpManual)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL, OnButtonDeselectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_MESSAGE(WM_GRD_CLICK, OnGrdClick)
	ON_BN_CLICKED(IDC_BUTTON_SET_TYPE, OnButtonSetType)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_INFO, OnButtonInputInfo)
	ON_UPDATE_COMMAND_UI(ID_TYPE_SETTING, OnUpdateTypeSetting)
	ON_BN_CLICKED(IDC_RADIO_SCORM, OnRadioScorm)
	ON_BN_CLICKED(IDC_RADIO_NO_SCORM, OnRadioNoScorm)
	ON_BN_CLICKED(IDC_RADIO_SCORM_FTP, OnRadioScormFtp)
	ON_BN_CLICKED(IDC_RADIO_SCORM_LOCAL, OnRadioScormLocal)
	ON_BN_CLICKED(IDC_RAD_ORACLE, OnRadioScormOracle)
	ON_BN_CLICKED(IDC_RAD_ORACLEOLD, OnRadioScormOracleOld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderDlg message handlers

BOOL CCourseLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

//#ifdef _BUNDLE
//	CMenu* pMenu = GetMenu();
//	if (pMenu)
//	{
//		CMenu* pSubMenu = pMenu->GetSubMenu(1);
//		if (pSubMenu)
//			pSubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
//	}
//	GetDlgItem(IDC_BUTTON_SET_TYPE)->EnableWindow(FALSE);
//#endif

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char szBuf[512];
	//::GetPrivateProfileString("General", "Scorn", "1", szBuf, 256, strIniPath);
	//((CButton*)GetDlgItem(IDC_CHECK_SCORN))->SetCheck(atoi(szBuf));

	int nCount = ::GetPrivateProfileInt("Type", "TypeCount", 0, strIniPath);	
	if (nCount == 0)
	{
		::WritePrivateProfileString("Type", "TypeCount", "3", strIniPath);
		
		CString strTmp;
		strTmp.Format("TypeInfo%d", 0);
		CString strName;
		strName.LoadString(IDS_TYPE_DESC0);
		CString strSysIniPath = ::GetSysPath() + "Elearning.ini";
	
		::WritePrivateProfileString(strTmp, "Name", strName, strIniPath);
		::WritePrivateProfileString(strTmp, "Convert", "0", strIniPath);
		
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ProfileName", szBuf, strIniPath);

		CString strParam;
		for (int i = 1; i < 8; i++)
		{
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("Profile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "Profile" + strParam, szBuf, strIniPath);
		}

		::GetPrivateProfileString("ScreenProfile", "Codec", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileCodec", szBuf, strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "ScreenProfile" + strParam, szBuf, strIniPath);
		}

		strTmp.Format("TypeInfo%d", 1);
		strName.LoadString(IDS_TYPE_DESC1);
		::WritePrivateProfileString(strTmp, "Name", strName, strIniPath);
		::WritePrivateProfileString(strTmp, "Convert", "1", strIniPath);
		::WritePrivateProfileString(strTmp, "Checked", "1", strIniPath);
		strName.LoadString(IDS_PROFILE_LOW_BAND);
		::WritePrivateProfileString(strTmp, "ProfileName", strName, strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam1", "32", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam2", "20", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam3", "3", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam4", "0", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam5", "16", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam6", "16000", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam7", "M", strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Codec", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileCodec", szBuf, strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "ScreenProfile" + strParam, szBuf, strIniPath);
		}
		
		strTmp.Format("TypeInfo%d", 2);
		strName.LoadString(IDS_TYPE_DESC2);
		::WritePrivateProfileString(strTmp, "Name", strName, strIniPath);
		::WritePrivateProfileString(strTmp, "Convert", "1", strIniPath);
		::WritePrivateProfileString(strTmp, "Checked", "1", strIniPath);
		strName.LoadString(IDS_PROFILE_NO_SOUND);
		::WritePrivateProfileString(strTmp, "ProfileName", strName, strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam1", "0", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam2", "0", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam3", "0", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam4", "0", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam5", "48", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam6", "32000", strIniPath);
		::WritePrivateProfileString(strTmp, "ProfileParam7", "S", strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Codec", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileCodec", szBuf, strIniPath);

		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, strSysIniPath);
		::WritePrivateProfileString(strTmp, "ScreenProfileName", szBuf, strIniPath);

		for (int i = 1; i < 8; i++)
		{
			strParam.Format("Param%d", i);
			::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 512, strSysIniPath);
			::WritePrivateProfileString(strTmp, "ScreenProfile" + strParam, szBuf, strIniPath);
		}
	}

	m_pGrid = new CColGrid;

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pGrid->Create(rcGrid, this, 0);
	m_pGrid->SetTotalColNo(2);
	m_pGrid->SetColAttribAt(0, cType, cGridBMP);
	m_pGrid->SetColAttribAt(0, cAlign, 1);
	m_pGrid->SetColAttribAt(0, cIndex, 0);
	m_pGrid->SetColAttribAt(0, cEditable, 0);
	m_pGrid->SetColAttribAt(1, cEditable, 0);

//#ifdef _BUNDLE
//	m_pGrid->SetColAttribAt(0, cEditable, 0);
//#endif

	CBitmap *pBmp;
	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_UNCHECK);
	m_pGrid->SetBMPAt(cBMP, 0, pBmp);

	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_CHECK);
	m_pGrid->SetBMPAt(cBMP, 1, pBmp);

	m_pGrid->SetSeqAt(cSeqBMPCombo, 0, "0\\1");
	m_pGrid->u_bDragLine = TRUE;
	m_pGrid->u_bWithFixedRow = TRUE;
	m_pGrid->u_bHScrollable = TRUE;
	m_pGrid->u_bDrawSel = FALSE;

	CString strTitle;
	strTitle.LoadString(IDS_DESCRIPTION);
	m_pGrid->SetFixedRowLabelAt(1, strTitle);
	
	strTitle.LoadString(IDS_VIDEO_FREQ);
	m_pGrid->SetFixedRowLabelAt(2, strTitle);

	strTitle.LoadString(IDS_AUDIO_FREQ);
	m_pGrid->SetFixedRowLabelAt(3, strTitle);
	
	strTitle.LoadString(IDS_FRAME_RATE);
	m_pGrid->SetFixedRowLabelAt(4, strTitle);
	
	m_pGrid->u_bDragTitle = FALSE;
	m_pGrid->u_bSort = FALSE;
	m_pGrid->u_pMSGWnd = this;
	m_pGrid->u_bHScrollable = FALSE;
	m_pGrid->SetGridFont(GetFont());
	m_pGrid->SetGridSelType(cGridNonSel, TRUE, FALSE);		//MultiSel : True , Toggle : False

	int nWidth = rcGrid.Width();
	m_pGrid->SetColWidth(0, 30); 
	m_pGrid->SetColWidth(1, nWidth - 30); 

	m_pBtnOpenFTPPath = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	CSize csBtn = ::GetBmpDim(IDB_OPEN);
	csBtn.cx /= 4;

	CRect rcEdit;
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	CRect rect;
	rect.right = rcEdit.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenFTPPath->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_FTP);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->MoveWindow(rcEdit);

	m_pBtnOpenFilePath = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);

	GetDlgItem(IDC_EDIT_PATH_DIRECT)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	rect.right = rcEdit.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenFilePath->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_FILE);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->MoveWindow(rcEdit);

	m_pBtnOpenScormPath = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	GetDlgItem(IDC_EDIT_PATH_SCORM)->GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	rect.right = rcEdit.right;
	rect.left = rect.right - csBtn.cx;
	if (rcEdit.Height() > csBtn.cy)
		rect.top = rcEdit.top + (rcEdit.Height() - csBtn.cy) / 2;
	else
		rect.top = rcEdit.top;
	rect.bottom = rect.top + csBtn.cy;
	m_pBtnOpenScormPath->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_OPEN_SCORM);

	rcEdit.right = rect.left - 6;
	GetDlgItem(IDC_EDIT_PATH_SCORM)->MoveWindow(rcEdit);

	ResetCheckItem();
	
	CollectCourse();

	if (m_strCourse != "")
	{
		int nCount = m_clCourse.GetCount();
		CString strTmp;
		for (int i = 0; i < nCount; i++)
		{
			m_clCourse.GetText(i, strTmp);
			strTmp.TrimLeft();
			if (strTmp == m_strCourse)
			{
				m_clCourse.SetCheck(i, 1);
				m_clCourse.SetTopIndex(i);
				break;

			}
		}
	}

	// FTPTREE-specific initialization follows...
	m_pFtpConnection = NULL;		

	CString str;
	if (!str.LoadString(IDS_APPNAME))
		str = _T("AppUnknown");
	m_pInetSession = new CInternetSession(str, 1, PRE_CONFIG_INTERNET_ACCESS);

	if (!m_pInetSession)
		W_SHOW_WARNING(IDS_FAIL_TO_CONNECT_INTERNET, this);

	m_dlgProgress.m_strAVIPath = ::GetSysPath() + "horse.avi";
	m_dlgProgress.Create(IDD_DIALOG_PROGRESS, this);

	LoadHistory();

	CButton* pNoScorm = (CButton*)GetDlgItem(IDC_RADIO_NO_SCORM);
	pNoScorm->SetCheck(1);
	OnRadioNoScorm();

	//ZIP Default Setchk
	CButton* pCheckZIP6 = (CButton*)GetDlgItem(IDC_RAD_ZIP6);
	pCheckZIP6->SetCheck(1);

	return FALSE;
}

void CCourseLoaderDlg::ResetCheckItem()
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	m_pGrid->DeleteAllRows();

//#ifndef _BUNDLE
	char szBuf[512];
	::GetPrivateProfileString("Type", "TypeCount", "0", szBuf, 512, strIniPath);
	int nCount = atoi(szBuf);
	m_pGrid->AppendRow(nCount);

	CString strType;
	for (int i = 0; i < nCount; i++)
	{
		strType.Format("TypeInfo%d", i);
		::GetPrivateProfileString(strType, "Name", "", szBuf, 512 , strIniPath);
		m_pGrid->SetDataAt(i, 1, szBuf);

		::GetPrivateProfileString(strType, "Checked", "0", szBuf, 512 , strIniPath);
		if (atoi(szBuf))
			SetLVCheck(i, 1);
	}
//#else
//	m_pGrid->AppendRow(1);
//	m_pGrid->SetDataAt(0, 1, "¼eÀW¼v­µ");
//	SetLVCheck(0, 1);
//#endif
	m_pGrid->RePaint();
}

void CCourseLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCourseLoaderDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCourseLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CCourseLoaderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

//	HRESULT hval = CoCreateInstance(CLSID_EasyUtility, NULL, CLSCTX_INPROC_SERVER, 
//									IID_IEasyUtility, (void**)&m_pUtility);
//	if (FAILED(hval))
//	{
//		SHOW_ERROR(IDS_ERROR_COM_EASY_UTIL);
//		return -1;
//	}
//	m_pUtility->SetupProgressHandler((long)(m_hWnd));

	//add by buxaingyi  ˆê?
	/*HRESULT hval = CoCreateInstance(CLSID_WMXUtility, NULL, CLSCTX_INPROC_SERVER, 
	IID_IWMXUtility, (void**)&m_pWMXUtility);
	if (FAILED(hval))
	{
	W_SHOW_ERROR(IDS_ERROR_COM_WMX_UTILITY, this);
	return -1;
	}
	m_pWMXUtility->SetupProgressHandler((long)(m_hWnd), (long)WM_USER_PROGRESS_REPORT);*/
	///////////////////////////////////////////////////////////////////////
	return 0;
}

void CCourseLoaderDlg::OnOK() 
{
}

void CCourseLoaderDlg::OnExit() 
{
	EndDialog(IDOK);	
}

void CCourseLoaderDlg::CollectCourse()
{
	CString strPath = ::GetSysPath() + "Courses";
	CStringArray strArray;
	::GetDir(strPath, strArray);
	::SortArray(strArray);

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		m_clCourse.AddString(" " + str);
	}
}

void CCourseLoaderDlg::SetLVCheck(WPARAM nItemIndex, int nCheck)
{
	CString str;
	str.Format("%d", nCheck);
	m_pGrid->SetDataAt(nItemIndex, 0, str);
}

BOOL CCourseLoaderDlg::GetLVCheck(int nIndex)
{
	return atoi(m_pGrid->GetDataAt(nIndex, 0));
}

BOOL CCourseLoaderDlg::AnyTypeChecked()
{
	int nCount = m_pGrid->GetDataSize();
	BOOL bFound = FALSE;
	for (int i = 0; i < nCount; i++)
	{
		BOOL b = GetLVCheck(i);
		if (b)
		{
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
		return FALSE;

	return TRUE;
}

void CCourseLoaderDlg::WriteCheckStatus() 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char szBuf[512];
	::GetPrivateProfileString("Type", "TypeCount", "0", szBuf, 512, strIniPath);
	int nCount = atoi(szBuf);

	CString strType;
	CString strCheck;
	for (int i = 0; i < nCount; i++)
	{
		strCheck = (GetLVCheck(i)) ? "1" : "0";
		strType.Format("TypeInfo%d", i);
		::WritePrivateProfileString(strType, "Checked", strCheck, strIniPath);
	}
}

BOOL CCourseLoaderDlg::GetMediaServerAlias(CString& strMediaSrv) 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	CString strMediaServer;
	char szBuf[256];
	while (true)
	{
		::GetPrivateProfileString("MediaServer", "Alias", "", szBuf, 256, strIniPath);
		strMediaServer = szBuf;
		if (strMediaServer == "")
		{
			if (W_ASK_YESNO(IDS_SET_MEDIA_SRV_ALIAS, this) == IDNO)
				return FALSE;
			if (!SetOption(1))
				return FALSE;
		}
		else
		{
			strMediaSrv = szBuf;
			if (strMediaSrv.Right(1) != "/")
				strMediaSrv += "/";
			return TRUE;
		}
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::GetOutputPath(CString& strOutputPath) 
{ 
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char* pBuf = new char[1024];
	::GetPrivateProfileString("General", "OutputPath", "", pBuf, 1024, strIniPath);
	strOutputPath = pBuf;
	delete [] pBuf;

	if (strOutputPath == "")
		strOutputPath = ::GetSysPath() + "Launched Courses";

	if (!::TryToMakeDir(strOutputPath, this))
		return FALSE;

	return TRUE;
}

//BOOL CCourseLoaderDlg::GenerateFreq(CString strSrcPath, CString strDestPath, CString strPostfix, BOOL bCopyVideo) 
BOOL CCourseLoaderDlg::GenerateFreq(CString strSrcPath, CString strDestPath, CString strPostfix, BOOL bCopyVideo, CString strDestPath1) 
{
//#ifndef _BUNDLE
	CString strDir = strDestPath1 + "\\" + strPostfix;
//#else
//	CString strDir = strDestPath;
//#endif
	if (!::TryToMakeDir(strDir, this))
		return FALSE;

	if (bCopyVideo)
	{
		CStringArray strArrayAsf;
		//::GetFiles(strSrcPath + "\\html\\videos", "wmv", strArrayAsf);
		//CString strFile;
		//for (int i = 0; i < strArrayAsf.GetSize(); i++)
		//{
		//	strFile = strArrayAsf[i];
		//	if (strFile.CompareNoCase("original.wmv") != 0)
		//		strArrayAsf[i] = strSrcPath + "\\html\\videos\\" + strArrayAsf[i];
		//}

		strArrayAsf.Add(strSrcPath + "\\html\\videos\\");
		::CopyTree(strSrcPath + "\\html", strDir + "\\html", &strArrayAsf);

		CButton* pScormMediaZip = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
		int nScormMediaZip = pScormMediaZip->GetCheck();
		
		//shien add 20080122
		CString strVideoDir = "";
		if(nScormMediaZip)
		{
			strVideoDir = strDestPath + "\\" + strPostfix + "\\html\\videos";
		}
		else
		{
			strVideoDir = strDestPath + "\\v_" + strPostfix;
		}

		//CString strVideoDir = strDestPath + "\\v_" + strPostfix;
		if (!::TryToMakeDir(strVideoDir, this))
			return FALSE;
	}
	else
		::CopyTree(strSrcPath + "\\html", strDir + "\\html", NULL);

	::CopyTree(strSrcPath + "\\webpages", strDir + "\\webpages", NULL);

	::CopyFile(strSrcPath + "\\default.htm", strDir + "\\default.htm", FALSE);
	::SetFileWritable(strDir + "\\default.htm");

	return TRUE;
}

void CCourseLoaderDlg::OnLoad() 
{
	if (LaunchCourse())
	{
		HideProgress();

		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_FTP);
		int nCheck = pCheck->GetCheck();
		if (nCheck)
		{
			CDlgLaunchOK dlg;
			if (dlg.DoModal() == IDOK)
			OnViewCourse();
		}
		else
		{
			W_SHOW_MESSAGE(IDS_COURSE_PUBLISHED, this);
		}
	}
	else
		HideProgress();
	//add by buxiangyi
	CString ConfigJsPath;
	CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);
	if (pScorm ->GetCheck())
	{
		
		for (int i = 0; i < m_strArrayCourse.GetCount(); i++)
		{
			ConfigJsPath = ::GetSysPath() + "Courses\\" + m_strArrayCourse[i] +"\\html";
			CStringArray m_array, m_oldaray;
			m_oldaray.Add("var fScorm = "); 
			m_array.Add("var fScorm = 1;");
			UpdateFileByTag(ConfigJsPath, "config.js", m_oldaray, m_array, TRUE);
		}
		
	}
	if (pScorm ->GetCheck() == BST_UNCHECKED)
	{
		for (int i = 0; i < m_clCourse.GetCount(); i++)
		{
			CString name;
			m_clCourse.GetText(i,name);
			name = name.Right(name.GetLength() -1);
			ConfigJsPath = ::GetSysPath() + "Courses\\" + name +"\\html";
			CStringArray m_array, m_oldaray;
			m_oldaray.Add("var fScorm = "); 
			m_array.Add("var fScorm = 0;");
			UpdateFileByTag(ConfigJsPath, "config.js", m_oldaray, m_array, TRUE);
		}
	}
	CButton* pButtonFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pButtonFTP_MANUAL = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);

	if (pScorm ->GetCheck() == BST_UNCHECKED)
	{
		CString ConfigJsPath;
		for (int i = 0; i < m_strArrayCourse.GetCount(); i++)
		{
			ConfigJsPath = ::GetSysPath() + "Courses\\" + m_strArrayCourse[i] +"\\html";
			CStringArray m_array, m_oldaray;
			m_oldaray.Add("var bStreaming = "); 
			m_array.Add("var bStreaming = 1;");
			UpdateFileByTag(ConfigJsPath, "config.js", m_oldaray, m_array, TRUE);
		}

	}
	if (pScorm ->GetCheck())
	{
		for (int i = 0; i < m_clCourse.GetCount(); i++)
		{
			CString name;
			m_clCourse.GetText(i,name);
			name = name.Right(name.GetLength() -1);
			ConfigJsPath = ::GetSysPath() + "Courses\\" + name +"\\html";
			CStringArray m_array, m_oldaray;
			m_oldaray.Add("var bStreaming = "); 
			m_array.Add("var bStreaming = 0;");
			UpdateFileByTag(ConfigJsPath, "config.js", m_oldaray, m_array, TRUE);
		}
	}
}	

BOOL CCourseLoaderDlg::LaunchCourse() 
{ 
	m_strArrayCourse.RemoveAll();
	int nCount = m_clCourse.GetCount();
	CString strTmp;
	for (int i = 0; i < nCount; i++)
	{
		m_clCourse.GetText(i, strTmp);
		strTmp.TrimLeft();
		if (m_clCourse.GetCheck(i))
			m_strArrayCourse.Add(strTmp);
	}

	if (m_strArrayCourse.GetSize() == 0)
	{
		W_SHOW_ERROR(IDS_SEL_COURSE_FIRST, this);
		return FALSE;
	}

	for (int i = 0; i < m_strArrayCourse.GetSize(); i++)
	{
		CString str = m_strArrayCourse[i];
		if (IsCourseVideoModified(str))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_COURSE_MODIFIED);
			strMsg.Format(strTmp, str);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}

	if (!AnyTypeChecked())
	{
		W_SHOW_ERROR(IDS_CHECK_FREQ_FIRST, this);
		return FALSE;
	}

	CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);
	CButton* pNoScorm = (CButton*)GetDlgItem(IDC_RADIO_NO_SCORM);
	CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
	CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);
	CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
	CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);
	//CButton* pCheckScorn = (CButton*)GetDlgItem(IDC_CHECK_SCORN);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	int nCheckScorm = pScorm->GetCheck();
	int nCheckNoScorm = pNoScorm->GetCheck();
	int nCheckFTP = pCheckFTP->GetCheck();
	int nCheckFTPManual = pCheckFTPManual->GetCheck();
	int nCheckFile = pCheckFile->GetCheck();
	int nScormFTP = pScormFTP->GetCheck();
	int nScormLocal = pScormLocal->GetCheck();
	//int nCheckScorn = pCheckScorn->GetCheck();
	int nProtocolHTTPNM = pProtocolHTTPNM->GetCheck();
	

	if(nScormFTP == 1 && nProtocolHTTPNM == 1)
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_NONEED);
		strMsg.Format(strTmp);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}

	int nDest = -1;
	if (nCheckNoScorm)
    {
		if (nCheckFTP)
			nDest = 0;
		else if (nCheckFTPManual)
	   		nDest = 1;
		else if (nCheckFile)
			nDest = 2;
	}
	else
	{
		if (nScormFTP)
			nDest = 3;
		else if (nScormLocal)
			nDest = 4;
	}

	if (nDest < 0)
		return FALSE;

	while (nCheckScorm)
	{
		CDlgInputScornInfo dlg;
		for (int i = 0; i < m_strArrayCourse.GetSize(); i++)
		{
			CString strCourse = m_strArrayCourse[i];
			CString strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\scorm.ini";
			if (!::FileExists(strIniPath))
				dlg.m_strArrayCourse.Add(strCourse);
		}
		if (dlg.m_strArrayCourse.GetSize() > 0)
			dlg.DoModal();
		else
			break;
	}


	CString strOutputPath;

	CButton* pAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	int nAenric = pAenrich->GetCheck();

	if (nCheckNoScorm)
	{
		if (nCheckFTP)
		{
			if (!IsWebFTPSet())
				return FALSE;

			if (!IsMediaFTPSet())
				return FALSE;

			//if(!nAenric)
			//{
				if (!GetOutputPath(strOutputPath))
					return FALSE;
			//}
		}
		else if (nCheckFTPManual)
			GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->GetWindowText(strOutputPath);
		else if (nCheckFile)	
			GetDlgItem(IDC_EDIT_PATH_DIRECT)->GetWindowText(strOutputPath);
	}
	else
	{
		if (nScormFTP)
		{
			if(nAenric)
			{
				if (!GetOutputPath(strOutputPath))
					return FALSE;

			}
			else
			{
				GetDlgItem(IDC_EDIT_PATH_SCORM)->GetWindowText(strOutputPath);
			}

			if (!IsMediaFTPSet())
				return FALSE;
		}
		else if (nScormLocal)
		{
			GetDlgItem(IDC_EDIT_PATH_SCORM)->GetWindowText(strOutputPath);
		}
	}

	CString strMsg;
	//strOutputPath.TrimLeft();
	//if (strOutputPath == "")
	//{
	//	W_SHOW_ERROR(IDS_INPUT_DEST_FIRST, this);
	//	return FALSE;
	//}

	//CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
	//nScormFTP = pScormFTP->GetCheck();

	if(strOutputPath == "" && nScormFTP == 1)
	{
		strOutputPath = ::GetSysPath() + "Temp\\";
	}
	else
	{
		if (!::FileExists(strOutputPath))
		{
			strTmp.LoadString(IDS_DEST_NOT_EXIST);
			strMsg.Format(strTmp, strOutputPath);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}
	if (strOutputPath.Right(1) != "\\")
		strOutputPath += "\\";

	m_strArrayFTPCourse.RemoveAll();

	CString strCourse;
	CStringArray strArrayNewCourse;
	for (int i = 0; i < m_strArrayCourse.GetSize(); i++)
	{
		strCourse = m_strArrayCourse[i];
		if (!PreProcessCourse(strCourse, strOutputPath, nDest))
			return FALSE;
		strArrayNewCourse.Add(strCourse);
	}

	CString strNewCourse;
	for (int i = 0; i < m_strArrayCourse.GetSize(); i++)
	{
		strCourse = m_strArrayCourse[i];
		strNewCourse = strArrayNewCourse[i];
		
		if (!ProcessCourse(strCourse, strNewCourse, strOutputPath, nDest, nScormFTP || nScormLocal))
			return FALSE;

#ifdef _WRITE_LOG
		if (nDest == 1)
			WritePublishRecord(strOutputPath + strNewCourse + "\\index.htm");
#endif
	}
	
	return TRUE;
}

BOOL CCourseLoaderDlg::WritePublishRecord(CString strPath)
{
	CString strLogFile = ::GetSysPath() + "publish.log";
	FILE *stream;
	if ((stream = fopen(strLogFile, "a" )) == NULL)
		return FALSE;

	if (::FileExists(strLogFile))
	{
		fseek(stream, 0, SEEK_END);
	}
	
	fputs(strPath + "\r\n", stream); 
	fclose(stream);

	return TRUE;
}

BOOL CCourseLoaderDlg::PreProcessCourse(CString& strCourse, CString strOutputPath, int nDest)
{
	if (nDest == 0)	
	{
		CString strTmp;
		CString strFTP;
		if (!ConnectToWebServer(strFTP))
		{
			W_SHOW_ERROR(IDS_CONECT_FAIL, this);
			return FALSE;
		}

		while (1)
		{
			CFtpFileFind ftpFind(m_pFtpConnection);
			BOOL bExist = ftpFind.FindFile(strCourse);
			ftpFind.Close();
			if (bExist)
			{
				BOOL bOverwrite = FALSE;
				if (!GetFTPCourseName(strCourse, bOverwrite))
					return FALSE;
				if (bOverwrite)
					break;
			}
			else
				break;
		}
	}
	else if (nDest == 1 || nDest == 2 || nDest == 3 || nDest == 4)
	{
		CString strPath = strOutputPath + strCourse;
		if (::FileExists(strPath))
		{
			BOOL bOverwrite = FALSE;
			if (!GetPathCourseName(strOutputPath, strCourse, bOverwrite))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::ProcessCourse(CString strCourse, CString strNewCourse, CString strOutputPath, 
									 int nDest, int nCheckScorn)
{

	CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);
	nCheckScorn = pScorm->GetCheck();

	//shien ADD Aenrich LMS
	CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	int nScormAenric = pScormAenrich->GetCheck();

	CString strDestPath = strOutputPath + strNewCourse;

	if (::FileExists(strDestPath))
		::DelTree(strDestPath);
	if (!::TryToMakeDir(strDestPath, this))
		return FALSE;

	CString strDestPath1;
	if(nScormAenric)
	{
			strDestPath1 = strOutputPath + strNewCourse + "\\" + strNewCourse;
			if (!::TryToMakeDir(strDestPath1, this))
				return FALSE;
	}
	else
	{
			strDestPath1 = strDestPath;
	}

	CString strSrcPath = ::GetSysPath() + "Courses\\" + strCourse;

	CString strDir;
	CString strPostfix = "";
	CString strAlias;
	int nCount = m_pGrid->GetDataSize();
	CStringArray strArrayPostfix;
	
	CString strTitle, strTmp;
	strTmp.LoadString(IDS_GEN_MEDIA_FILE);
	strTitle.Format(strTmp, strCourse);
	CString strMsg = "";
	SetProgress(strTitle, strMsg);
	m_dlgProgress.EnableCancel(FALSE);
	ShowProgress();

	CStringArray strArrayFiles, strArrayAllFiles;
	::GetFiles(strSrcPath + "\\html\\videos", "mp4", strArrayFiles);
	for (int i = 0; i < strArrayFiles.GetSize(); i++)
		strArrayAllFiles.Add("html\\videos\\" + strArrayFiles[i]);
	strArrayFiles.RemoveAll();
	::GetFiles(strSrcPath + "\\html", "mp4", strArrayFiles);
	for (int i = 0; i < strArrayFiles.GetSize(); i++)
		strArrayAllFiles.Add("html\\" + strArrayFiles[i]);


	CStringArray strArrayTableEntry;
	CString strEntry;
	for (int i = 0; i < nCount; i++)
	{
		BOOL b = GetLVCheck(i);
		if (b)
		{
			CStringArray strArrayParam;
			RetriveInfo(i, strArrayParam);
			int nConvert = atoi(strArrayParam[1]);
			strPostfix.Format("combination%d", i);

			//shien test
			//nConvert=0;
			
			if (!nConvert)
			{
				if (nDest == 0 || nDest == 1 || nDest == 3 || nDest == 4)
				{
					if (!GenerateFreq(strSrcPath, strDestPath, strPostfix, TRUE, strDestPath1))
						return FALSE;

					for (int j = 0; j < strArrayAllFiles.GetSize(); j++)
					{
						CString strTmp, strFile;
						strTmp.LoadString(IDS_COPY_FILE);
						strFile = strArrayAllFiles[j];
						strMsg.Format(strTmp, strPostfix + "\\" + strFile);
						SetProgress(strTitle, strMsg);
					
						if (!PeekAndPump())
							return FALSE;
						
						CString strFileOnly = ::GetFileName(strFile);

						//shien add 20080122
						CButton* pScormMediaZip = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
						int nScormMediaZip = pScormMediaZip->GetCheck();
						
						CString strDir = "";

						if(nScormMediaZip)
						{
							strDir = strDestPath + "\\" + strPostfix + "\\html\\videos\\" + strFileOnly;
						}
						else
						{
							strDir = strDestPath + "\\v_" + strPostfix + "\\" + strFileOnly;
						}


						//CString strDir = strDestPath + "\\v_" + strPostfix + "\\" + strFileOnly;
						::CopyFile(strSrcPath + "\\" + strFile, strDir, FALSE);
						::SetFileWritable(strDir);
					}
				}
				else if (nDest == 2)
				{
					if (!GenerateFreq(strSrcPath, strDestPath, strPostfix, FALSE, strDestPath1))
						return FALSE;
				}
			}
			else 
			{
				if (nDest == 0 || nDest == 1 || nDest == 3 || nDest == 4)
				{
					if (!ConverCourseByBandwidth(strPostfix, strSrcPath, strDestPath, 
												 strArrayParam, strArrayAllFiles, TRUE, strDestPath1))
					{
						return FALSE;
					}
				}
				else if (nDest == 2)
				{
					if (!ConverCourseByBandwidth(strPostfix, strSrcPath, strDestPath, 
					strArrayParam, strArrayAllFiles, FALSE, strDestPath1))
					{
					return FALSE;
					}
				}
			}

			if (nDest == 3 || nDest == 4)
			{
				CString strPath = strDestPath + "\\" + strPostfix;

				// add by shien
				if(nScormAenric)
				{
					//if (nDest == 3)
						strPath = strDestPath1 + "\\" + strPostfix;

					//if (nDest == 4)
					//	strPath = strDestPath1 + "\\" + strPostfix;
				}
				::DeleteFile(strPath + "\\default.htm");
//				rename(strPath + "\\html\\_frame.htm", strPath + "\\html\\index.htm");

				CStringArray strArrayTag, strArrayReplace;
				strArrayTag.Add("var g_bUseScorm=0;");
				strArrayReplace.Add("var g_bUseScorm=1;");
				::UpdateFileByTag(strPath + "\\html", "_exoption.js", strArrayTag, strArrayReplace);
			}

			strArrayPostfix.Add(strPostfix);
			strEntry = GetTableHTML(m_pGrid->GetDataAt(i, 1), GetLinkText(), strPostfix + "/default.htm");
			strArrayTableEntry.Add(strEntry);
		}	
	}
	
	if (nDest != 3 && nDest != 4)
	{
			if(nScormAenric)
			{
				if (!WriteIndexHTMIndex(strCourse, strNewCourse, strDestPath1, strArrayTableEntry))
					return FALSE;
			}
			else
			{
				if (!WriteIndexHTMIndex(strCourse, strNewCourse, strDestPath, strArrayTableEntry))
					return FALSE;
			}
	}
	
	CScorm scorm;
	if (nDest == 0 || nDest == 1 || nDest == 3 || nDest == 4)
	{
		if(nScormAenric)
		{
			WriteMediaPos(strCourse, strNewCourse, strDestPath1, strArrayPostfix); 
		}
		else
		{
			WriteMediaPos(strCourse, strNewCourse, strDestPath, strArrayPostfix); 
		}

		if (nDest == 3 || nDest == 4)
		{
			CString strPath = strDestPath + "\\" + strPostfix;

			// add by shien
			if(nScormAenric)
				strPath = strDestPath1 + "\\" + strPostfix;

			::DeleteFile(strPath + "\\default.htm");
			rename(strPath + "\\html\\_frame.htm", strPath + "\\html\\index.htm");

			//::CopyTree(strPath + "\\html", strDestPath);

			// add by shien
			CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);
			CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);

			if(nScormAenric)
			{
				::CopyTree(strPath + "\\html", strDestPath1);
				::CopyTree(strPath + "\\webpages", strDestPath1 + "\\webpages\\");
			}
			else
			{
				::CopyTree(strPath + "\\html", strDestPath);
				if (pProtocolHTTPNM->GetCheck() && pScorm->GetCheck())
				{
					::CopyTree(strDestPath + "\\" + "/v_" + strPostfix, strDestPath);
				}

				::CopyTree(strPath + "\\webpages", strDestPath1 + "\\webpages\\");
			}

			::DelTree(strPath);
			if (pProtocolHTTPNM->GetCheck() && pScorm->GetCheck())
			{
				::DelTree(strDestPath + "\\" + "/v_" + strPostfix);
			}

		}

		if (nCheckScorn)
		{
			
			CString strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\scorm.ini";
			//::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\scorm.ini", FALSE);

			//add by shien 
			if(nScormAenric)
			{
				::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\" + strNewCourse + "\\scorm.ini", FALSE);
			}
			else
			{
				::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\scorm.ini", FALSE);
			}

			strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\course.ini";
			//::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\course.ini", FALSE);

			if(nScormAenric)
			{
				::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\" + strNewCourse + "\\course.ini", FALSE);
			}
			else
			{
				::CopyFile(strIniPath, strOutputPath + strNewCourse + "\\course.ini", FALSE);
			}

			//shien add oracle 5
			int nOracle = 0;

			CButton* pScormOracle = (CButton*)GetDlgItem(IDC_RAD_ORACLE);
			CButton* pScormOracleOld = (CButton*)GetDlgItem(IDC_RAD_ORACLEOLD);
			CButton* pScormEnhance = (CButton*)GetDlgItem(IDC_CHECK_ENHANCE);
			int nScormOracle = pScormOracle->GetCheck();
			int nScormOracleOld = pScormOracleOld->GetCheck();
			int nScormEnhance = pScormEnhance->GetCheck();

			if (nScormOracle == 1)
			{
				nOracle = 1;
			}
			else if	(nScormOracleOld == 1)
			{
				nOracle = 2;
			}

			if (nOracle == 1 || nScormEnhance == 1)
			{

				if(nScormAenric)
					strDestPath = strDestPath1;

				CString strIndexHtmFile = strDestPath + "\\index.htm";
				CString strNsoHtmFile = strDestPath + "\\_nso.htm";

				if (::FileExists(strIndexHtmFile))
				{
					::SetFileWritable(strIndexHtmFile);
					CStringArray strArrayTag, strArrayReplace;
  
					strArrayTag.Add("<SCRIPT	LANGUAGE=JAVASCRIPT	SRC=\"APIWrapper.js\"></SCRIPT>");
					strArrayReplace.Add("");
					strArrayTag.Add("<SCRIPT	LANGUAGE=JAVASCRIPT	SRC=\"SCOFunctions.js\"></SCRIPT>");
					strArrayReplace.Add("");
					strArrayTag.Add(" onUnload=\"if (g_bUseScorm) return unloadPage();\"");
					strArrayReplace.Add("");
					strArrayTag.Add("\tif (g_bUseScorm)");
					strArrayReplace.Add("\t//if (g_bUseScorm)");
					strArrayTag.Add("\t   loadPage();");
					strArrayReplace.Add("\t//   loadPage();");

					::UpdateFileByTag(strDestPath, "index.htm", strArrayTag, strArrayReplace);
				}

				if (::FileExists(strNsoHtmFile))
				{
					::SetFileWritable(strIndexHtmFile);
					CStringArray strArrayTag, strArrayReplace;
  
					strArrayTag.Add("src=_freehand.js></script>");
					strArrayReplace.Add("src=_freehand.js></script>\n<SCRIPT\tLANGUAGE=JAVASCRIPT\tSRC=\"APIWrapper.js\"></SCRIPT>\n<SCRIPT\tLANGUAGE=JAVASCRIPT\tSRC=\"SCOFunctions.js\"></SCRIPT>");

					strArrayTag.Add("//if (g_bUseScorm)");
					strArrayReplace.Add("if (g_bUseScorm)");
					strArrayTag.Add("//\tloadPage();");
					strArrayReplace.Add("\tloadPage();");
					strArrayTag.Add("//\tdoContinue('completed');");
					strArrayReplace.Add("\tdoContinue('completed');");

					::UpdateFileByTag(strDestPath, "_nso.htm", strArrayTag, strArrayReplace);
				}
			}

			////scorm.MakeintoScorm(strDestPath + "\\", strCourse, pScormOracle->GetCheck());

			//scorm.MakeintoScorm(strDestPath + "\\", strCourse, nOracle);
			
			if(nScormAenric)
			{
				scorm.MakeintoScorm(strDestPath1 + "\\", strCourse, nOracle, nScormAenric);
			}
			else
			{
				scorm.MakeintoScorm(strDestPath + "\\", strCourse, nOracle, nScormAenric);
			}

			CString strZipPath = strOutputPath + strNewCourse + ".zip";
			CString strFolder = strOutputPath + strNewCourse;
			strFolder = strOutputPath + strNewCourse;
			AddZipFolder(strZipPath, strFolder);

			if(nScormAenric && nDest == 3)
			{
				CString strNewZipPath = strOutputPath + strNewCourse + "\\" + strNewCourse + ".zip";
				::CopyFile(strZipPath, strNewZipPath, FALSE);
				if (::FileExists(strNewZipPath))
					::DeleteFile(strZipPath);

			}

			CStringArray strArrayDir;
			::GetDir(strFolder, strArrayDir);
			for (int i = 0; i < strArrayDir.GetSize(); i++)
			{
				if (strArrayDir[i].Left(2) != "v_")
					::DelTree(strFolder + "\\" + strArrayDir[i]);
			}
			
			CStringArray strArrayFile;
			::GetFiles(strFolder, "*", strArrayFile);
			for (int i = 0; i < strArrayFile.GetSize(); i++)
			{
				if (strArrayFile[i].CompareNoCase(strNewCourse + ".zip") != 0)
					::DeleteFile(strFolder + "\\" + strArrayFile[i]);
			}

			if(!nScormAenric)
			{
			CString strNewZipPath = strOutputPath + strNewCourse + "\\" + strNewCourse + ".zip";
			::CopyFile(strZipPath, strNewZipPath, FALSE);
			if (::FileExists(strNewZipPath))
				::DeleteFile(strZipPath);
			}
		}
		else
		{

			if(nScormAenric)
			{
				CString strZipPath = strOutputPath + strNewCourse + ".zip";
				CString strFolder = strOutputPath + strNewCourse;
				AddZipFolder(strZipPath, strFolder);
				CStringArray strArrayDir;
				::GetDir(strFolder, strArrayDir);
				for (int i = 0; i < strArrayDir.GetSize(); i++)
				{
					if (strArrayDir[i].Left(2) != "v_")
					::DelTree(strFolder + "\\" + strArrayDir[i]);
				}
			
				CStringArray strArrayFile;
				::GetFiles(strFolder, "*", strArrayFile);
				for (int i = 0; i < strArrayFile.GetSize(); i++)
				{
					if (strArrayFile[i].CompareNoCase(strNewCourse + ".zip") != 0)
						::DeleteFile(strFolder + "\\" + strArrayFile[i]);
				}
				
				if(nDest == 0)
				{
				CString strNewZipPath = strOutputPath + strNewCourse + "\\" + strNewCourse + ".zip";
				::CopyFile(strZipPath, strNewZipPath, FALSE);
				if (::FileExists(strNewZipPath))
					::DeleteFile(strZipPath);
				}
	
			}
	
		}
		if (nDest == 0 || nDest == 3)
		{
			if(nScormAenric)
			{
				if (!TransferByFTP(strCourse, strNewCourse, strDestPath,  strArrayPostfix, TRUE, 0))
					return FALSE;
			}
			else
			{
				if (!TransferByFTP(strCourse, strNewCourse, strDestPath,  strArrayPostfix, TRUE, nDest == 3))
					return FALSE;
			}

			m_strArrayFTPCourse.Add(strNewCourse);

			int nFileCount =  ::GetFileCount(strDestPath, NULL);
			if (nFileCount == 0)
				::DelTree(strDestPath);
		}
	}
	else if (nDest == 2 && nScormAenric) //add by shien 
	{
				CString strZipPath = strOutputPath + strNewCourse + ".zip";
				CString strFolder = strOutputPath + strNewCourse;
				AddZipFolder(strZipPath, strFolder);
				CStringArray strArrayDir;
				::DelTree(strFolder);
	}


	return TRUE;
}

void CCourseLoaderDlg::RetriveInfo(int nIndex, CStringArray& strArrayParam)
{
	CString strTmp;
	strTmp.Format("TypeInfo%d", nIndex);

	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char szBuf[512];
	::GetPrivateProfileString(strTmp, "Name", "", szBuf, 512, strIniPath);
	strArrayParam.Add(szBuf);

	::GetPrivateProfileString(strTmp, "Convert", "", szBuf, 512, strIniPath);
//#ifdef _BUNDLE
//	strArrayParam.Add("0");
//#else
	strArrayParam.Add(szBuf);
//#endif

	::GetPrivateProfileString(strTmp, "ProfileName", "", szBuf, 512, strIniPath);
	strArrayParam.Add(szBuf);

	for (int i = 1; i < 8; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString(strTmp, "Profile" + strParam, "", szBuf, 512, strIniPath);
		strArrayParam.Add(szBuf);
	}

	::GetPrivateProfileString(strTmp, "ScreenProfileCodec", "", szBuf, 512, strIniPath);
	strArrayParam.Add(szBuf);

	::GetPrivateProfileString(strTmp, "ScreenProfileName", "", szBuf, 512, strIniPath);
	strArrayParam.Add(szBuf);

	for (int i = 1; i < 8; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString(strTmp, "ScreenProfile" + strParam, "", szBuf, 512, strIniPath);
		strArrayParam.Add(szBuf);
	}
}

CString CCourseLoaderDlg::GetLinkText()
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	
	CString strLink;
	strLink.LoadString(IDS_DEFAULT_LINK);
	char szBuf[256];
	::GetPrivateProfileString("General", "LinkCaption", "", szBuf, 256, strIniPath);
	if (szBuf[0] != '\0')
		strLink = szBuf;

	return strLink;
}

BOOL CCourseLoaderDlg::PeekAndPump()
{
	m_dlgProgress.PeekAndPump();
	if (m_dlgProgress.IsCanceled())
	{
			return FALSE;
	}
	return TRUE;
}

BOOL CCourseLoaderDlg::IsWebFTPSet()
{
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("WebServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	CString strFtpSite = szBuf;
	if (strFtpSite == "0.0.0.0")
	{
		W_SHOW_ERROR(IDS_WEB_FTP_NOT_SET, this);
		SetOption(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::IsMediaFTPSet()
{
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("MediaServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	CString strFtpSite = szBuf;
	if (strFtpSite == "0.0.0.0")
	{
		W_SHOW_ERROR(IDS_MEDIA_FTP_NOT_SET, this);
		SetOption(1);
		return FALSE;
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::GetFTPCourseName(CString& strNewCourse, BOOL& bOverwrite) 
{
	CDlgCourseDupFTP dlg;
	dlg.m_strCourse = strNewCourse;
	if (dlg.DoModal() == IDCANCEL)
		return FALSE;
	strNewCourse = dlg.m_strCourse;
	bOverwrite = dlg.m_bOverWrite;

	return TRUE;
}

BOOL CCourseLoaderDlg::GetPathCourseName(CString strOutputPath, CString& strNewCourse, BOOL& bOverwrite) 
{
	CDlgAskCourseDup dlg;
	dlg.m_strCourse = strNewCourse;
	dlg.m_strPath = strOutputPath;
	if (dlg.DoModal() == IDCANCEL)
		return FALSE;
	strNewCourse = dlg.m_strCourse;
	bOverwrite = dlg.m_bOverWrite;

	return TRUE;
}

BOOL CCourseLoaderDlg::TransferByFTP(CString strCourse,
									 CString& strNewCourse, 
									 CString strDestPath, 
									 CStringArray& strArrayPostfix, 
									 BOOL bCheckExist, 
									 BOOL bCopyMediaOnly)
{
	CString strFTP;

	m_dlgProgress.EnableCancel(TRUE);

	if (!bCopyMediaOnly)
	{
		char szBuf[256];
		CString strIniPath = ::GetSysPath() + "Launcher.ini";
		::GetPrivateProfileString("MediaServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
		CString strFTP = szBuf;

		CString strTmp, strMsg, strTitle;
		strTmp.LoadString(IDS_TRANSMIT_FILE);
		strTitle.Format(strTmp, strNewCourse);
		strTmp.LoadString(IDS_CONNECT_TO_FTP);
		strMsg.Format(strTmp, strFTP);
		SetProgress(strTitle, strMsg);
		
		if (!ConnectToWebServer(strFTP))
			return FALSE;

		//shien add aEnrich & noScorm
		CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
		int nScormAenric = pScormAenrich->GetCheck();

		int nDest = -1;
		if (nScormAenric)
		{

			CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);
			CButton* pNoScorm = (CButton*)GetDlgItem(IDC_RADIO_NO_SCORM);
			CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
			CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);
			CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
			CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
			CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);
			//CButton* pCheckScorn = (CButton*)GetDlgItem(IDC_CHECK_SCORN);
		
			int nCheckScorm = pScorm->GetCheck();
			int nCheckNoScorm = pNoScorm->GetCheck();
			int nCheckFTP = pCheckFTP->GetCheck();
			int nCheckFTPManual = pCheckFTPManual->GetCheck();
			int nCheckFile = pCheckFile->GetCheck();
			int nScormFTP = pScormFTP->GetCheck();
			int nScormLocal = pScormLocal->GetCheck();

			if (nCheckNoScorm)
			{
				if (nCheckFTP)
					nDest = 0;
				else if (nCheckFTPManual)
					nDest = 1;
				else if (nCheckFile)
					nDest = 2;
			}
			else
			{
				if (nScormFTP)
					nDest = 3;
				else if (nScormLocal)
					nDest = 4;
			}
		}

		if(!nScormAenric && nDest != 0)
			{
				if (!FTPCreateDir(strNewCourse, strFTP))
				return FALSE;
			}

		if (m_pFtpConnection != NULL)
		{
			//CString strIndexFile = strDestPath + "\\index.htm";
			//CString strTmpPath = ::GetSysPath() + "Launched Courses\\tmp_index.htm";
			//::DeleteFile(strTmpPath);
			//BOOL bExist = m_pFtpConnection->GetFile("index.htm", strTmpPath, FALSE);
			//int nType = 1;
			//if (bExist)
			//{
			//	CDlgRegenIndex dlg;
			//	int nRet = dlg.DoModal();
			//	if (nRet == ID_RESERVE)
			//		nType = 0;
			//	else if (nRet == ID_OVERWRITE)
			//		nType = 1;
			//	else if (nRet == ID_SELECT)
			//		nType = 2;
			//}

			//if (nType == 2)
			//{
			//	CStringArray strArrayTableEntry;
			//	CDlgSelType dlg;
			//	dlg.m_pstrArrayType = &strArrayTableEntry;
			//	if (dlg.DoModal() == IDCANCEL)
			//		return FALSE;

			//	if (!WriteIndexHTMIndex(strCourse, strNewCourse, strDestPath, strArrayTableEntry))
			//		return FALSE;
			//}

			//if (nType != 0)
			//{
			//	if (::FileExists(strIndexFile))
			//	{
			//		if (!PutFile(strIndexFile, "index.htm"))
			//			return FALSE;
			//		::DeleteFile(strIndexFile);
			//	}
			//}

			CStringArray strArrayFiles;
			::GetFiles(strDestPath, "*", strArrayFiles);
			for (int i = 0; i < strArrayFiles.GetSize(); i++)
			{
				CString strFile = strArrayFiles[i];
				CString strSrcPath = strDestPath + "\\" + strFile;

				CString strTmp, strMsg;
				strTmp.LoadString(IDS_COPY_FILE_TO_FTP);
				strMsg.Format(strTmp, strFile);
				SetProgress(strMsg);
				if (!PeekAndPump())
					return FALSE;

				if (!PutFile(strSrcPath, strFile))
					return FALSE;
				::DeleteFile(strSrcPath);
			}
		}

		for (int i = 0; i < strArrayPostfix.GetSize(); i++)
		{
			strTmp = strArrayPostfix[i];
			if (::FileExists(strDestPath + "\\" + strTmp))
			{
				if (!CopyDirToFTPServer(strDestPath, strTmp, strFTP))
					return FALSE;
			}
		}
	}

	if (!ConnectToMediaServer(strNewCourse, strFTP))
		return FALSE;

	if (!FTPCreateDir(strNewCourse, strFTP))
		return FALSE;

	for (int i = 0; i < strArrayPostfix.GetSize(); i++)
	{
		CString strTmp = "v_" + strArrayPostfix[i];
		if (!CopyDirToFTPServer(strDestPath, strTmp, strFTP))
			return FALSE;

		int nFileCount =  ::GetFileCount(strDestPath + "\\" + strTmp, NULL);
		if (nFileCount == 0)
			::DelTree(strDestPath + "\\" + strTmp);

		//shien add
		CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
		int nAenric = pScormAenrich->GetCheck();
		if(nAenric)
			::DelTree(strDestPath);

	}

	return TRUE;
}

BOOL CCourseLoaderDlg::WriteMediaPos(CString strCourse, CString strNewCourse, 
									 CString strDestPath, CStringArray& strArrayPostfix) 
{

	CString strMediaServer;
	if (!GetMediaServerAlias(strMediaServer))
		return FALSE;

	CString strSrcPath = ::GetSysPath() + "Courses\\" + strCourse;

	CString strAlias = "";

	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	for (int i = 0; i < strArrayPostfix.GetSize(); i++)
	{
		CString strPostfix = strArrayPostfix[i];


	//shien add 20080122
	CButton* pScormMediaZip = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
	int nScormMediaZip = pScormMediaZip->GetCheck();
	
	if(nScormMediaZip)
	{
		if (pProtocolMMS->GetCheck())
			strAlias = "mms://" + strMediaServer + strNewCourse + "/videos";
		else if (pProtocolRTSP->GetCheck())
			strAlias = "rtsp://" + strMediaServer + strNewCourse + "/videos";
		else if (pProtocolHTTP->GetCheck())
			strAlias = "http://" + strMediaServer + strNewCourse + "/videos";
		else if(pProtocolHTTPNM->GetCheck())
			strAlias = "";
	}
	else
	{
		if (pProtocolMMS->GetCheck())
			strAlias = "mms://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if (pProtocolRTSP->GetCheck())
			strAlias = "rtsp://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if (pProtocolHTTP->GetCheck())
			strAlias = "http://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if(pProtocolHTTPNM->GetCheck())
			strAlias = "";
	}



/*
		if (pProtocolMMS->GetCheck())
			strAlias = "mms://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if (pProtocolRTSP->GetCheck())
			strAlias = "rtsp://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if (pProtocolHTTP->GetCheck())
			strAlias = "http://" + strMediaServer + strNewCourse + "/v_" + strPostfix;
		else if(pProtocolHTTPNM->GetCheck())
			strAlias = "";
*/

//#ifdef _BUNDLE
//		CString strDir = strDestPath;
//#else
		CString strDir = strDestPath + "\\" + strPostfix;
//#endif
		UpdateMediaFile(strSrcPath, strDir, strAlias);
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::WriteIndexHTMIndex(CString strCourse, CString strNewCourse, 
										  CString strDestPath, CStringArray& strArrayTableEntry)
{
	CString strTmpPath = ::GetSysPath() + "Template\\_indextmp.htm";
	if (!::FileExists(strTmpPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_FIND_FILE);
		strMsg.Format(strTmp, strTmpPath);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}

	if (!::CopyFile(strTmpPath, strDestPath + "\\index.htm", FALSE))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_COPY);
		strMsg.Format(strTmp, strTmpPath, strDestPath + "\\index.htm");
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}
	::SetFileWritable(strDestPath + "\\index.htm");

	CString strCourseName, strLecturer, strCompany;
	CString strDate, strURL;
	CStringArray strArrayTag, strArrayReplace;
	CString strIniPath = ::GetSysPath() + "Courses\\" + strCourse + "\\Course.ini";
	char szBuf[512];
	::GetPrivateProfileString("Information", "Date", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%DATE%");
	strArrayReplace.Add(szBuf);
	strDate = szBuf;

	::GetPrivateProfileString("Information", "Duration", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%TIME%");
	strArrayReplace.Add(szBuf);

	::GetPrivateProfileString("Information", "SlideNo", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%SLD%");
	strArrayReplace.Add(szBuf);

	::GetPrivateProfileString("Information", "Advisor", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%LECTURER%");
	strArrayReplace.Add(szBuf);
	strLecturer = szBuf;

	::GetPrivateProfileString("Information", "Message", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%MESSAGE%");
	strArrayReplace.Add(szBuf);
	strCompany = szBuf;

	::GetPrivateProfileString("Information", "CourseName", "", szBuf, 512, strIniPath);
	strArrayTag.Add("%TITLE%");
	strArrayReplace.Add(szBuf);
	strCourseName = szBuf;

	CString strContentPath = ::GetSysPath() + "Courses\\" + strCourse + "\\html\\content.lrn";
	CString strContent = "";
	if (::FileExists(strContentPath))
	{
		CFile cf;
		cf.Open(strContentPath, CFile::modeRead);
		char* pBuf = new char[4096];
		int nRead = cf.Read(pBuf, 4096);
		if (nRead < 4096 && nRead >= 0)
			pBuf[nRead] = '\0';
		cf.Close();
		strContent = pBuf;
		delete [] pBuf;
	}
	strArrayTag.Add("%CONTENT%");
	strArrayReplace.Add(strContent);

    CString strTable; 
	strTable = "<table border=\"1\" width=\"71%\" height=\"99\">\n";
	for (int i = 0; i < strArrayTableEntry.GetSize(); i++)
		strTable += strArrayTableEntry[i];
	strTable += "</table>\n";
	
	strArrayTag.Add("%LINK_TABLE%");
	strArrayReplace.Add(strTable);

	::UpdateFileByTag(strDestPath, "index.htm", strArrayTag, strArrayReplace);

	GetCourseURL(strNewCourse, strURL);

	//if (!WriteDB(strCourseName, strLecturer, strCompany, strDate, strURL, strContent))
	//{
	//	W_SHOW_ERROR(IDS_FAIL_TO_WRITE_DB, this);
	//	return FALSE;
	//}

	return TRUE;
}


BOOL CCourseLoaderDlg::GetCourseURL(CString strNewCourse, CString& strURL)
{
	CString strParamIniPath = ::GetSysPath() + "Launcher.ini";
	char szBuf[512];
	::GetPrivateProfileString("WebServer", "CourseURL", "", szBuf, 512, strParamIniPath);
	if (szBuf[0] == '\0')
		return FALSE;

	strURL = szBuf;
	if (strURL.Right(1) != "/")
		strURL += "/";
	strURL += strNewCourse + "/index.htm";

	return TRUE;
}

CString CCourseLoaderDlg::GetTableHTML(CString strLabel, CString strLink, CString strURL)
{
	CString strTable;
    strTable = "<tr><td width=\"80%\" height=\"28\"><b><font face=\"Arial\" size=\"3\" color=\"#000000\">" + 
			   strLabel + "</font></b></td>\n";
    strTable += "<td width=\"20%\" align=\"center\" height=\"28\"><a href=\"#\" onclick=\"ShowWindow('" + strURL + "')\">" + strLink + 
			   "</a></td></tr>\n";

	return strTable;
}

/*BOOL CCourseLoaderDlg::ConverCourseByBandwidth(CString strPostfix, 
											   CString strSrcPath, 
											   CString strDestPath,
											   CStringArray& strArrayParam,
											   CStringArray& strArrayFiles,
											   BOOL bVideoPath)*/

BOOL CCourseLoaderDlg::ConverCourseByBandwidth(CString strPostfix, 
											   CString strSrcPath, 
											   CString strDestPath,
											   CStringArray& strArrayParam,
											   CStringArray& strArrayFiles,
											   BOOL bVideoPath, 
											   CString strDestPath1)
{
	//if (!GenerateFreq(strSrcPath, strDestPath, strPostfix, bVideoPath))
	if (!GenerateFreq(strSrcPath, strDestPath, strPostfix, bVideoPath, strDestPath1))
		return FALSE;

	for (int j = 0; j < strArrayFiles.GetSize(); j++)
	{
		CString strTmp, strFile, strMsg;
		strTmp.LoadString(IDS_CONVERT_FILE);
		strFile = strArrayFiles[j];
		strMsg.Format(strTmp, strPostfix + "\\" + strFile);
		SetProgress(strMsg);
		if (!PeekAndPump())
			return FALSE;
		
		int nIndex = strFile.ReverseFind('\\');
		CString strFileName = strFile.Mid(nIndex + 1);

		BOOL bMainASF = (strFileName.CompareNoCase("default.mp4") == 0);
		BOOL bSubASF = (strFile.Left(12) != "html\\videos\\");
		BOOL bDemo = (strFile.Left(16) == "html\\videos\\demo");

		CString strFileOnly = ::GetFileName(strFile);
		CString strDir;
		
		/*if (bVideoPath)
			strDir = strDestPath + "\\v_" + strPostfix + "\\" + strFileOnly;
		else
			strDir = strDestPath + "\\" + strPostfix + "\\" + strFile;*/

		//add by shien******************************************
		CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
		int nAenric = pScormAenrich->GetCheck();

		int nDest1 = -1;

			CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);
			CButton* pNoScorm = (CButton*)GetDlgItem(IDC_RADIO_NO_SCORM);
			CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
			CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);
			CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
			CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
			CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);
		
			int nCheckScorm1 = pScorm->GetCheck();
			int nCheckNoScorm1 = pNoScorm->GetCheck();
			int nCheckFTP1 = pCheckFTP->GetCheck();
			int nCheckFTPManual1 = pCheckFTPManual->GetCheck();
			int nCheckFile1 = pCheckFile->GetCheck();
			int nScormFTP1 = pScormFTP->GetCheck();
			int nScormLocal1 = pScormLocal->GetCheck();

			if (nCheckNoScorm1)
			{
				if (nCheckFTP1)
					nDest1 = 0;
				else if (nCheckFTPManual1)
					nDest1 = 1;
				else if (nCheckFile1)
					nDest1 = 2;
			}
			else
			{
				if (nScormFTP1)
					nDest1 = 3;
				else if (nScormLocal1)
					nDest1 = 4;
			}

		if (bVideoPath)
		{
			if(nAenric && nDest1 == 2)
				strDir = strDestPath1 + "\\v_" + strPostfix + "\\" + strFileOnly;
			else
				strDir = strDestPath + "\\v_" + strPostfix + "\\" + strFileOnly;
		}
		else
		{
			if(nAenric && nDest1 == 2)
				strDir = strDestPath1 + "\\" + strPostfix + "\\" + strFile;
			else
				strDir = strDestPath + "\\" + strPostfix + "\\" + strFile;
		}
		//********************************************************

		if (bDemo)
		{
			CString strIniPath = ::GetSysPath() + "Launcher.ini";
			char szBuf[256];				
			::GetPrivateProfileString("General", "ConvDemo", "0", szBuf, 256, strIniPath);
			if (!atoi(szBuf))
			{
				::CopyFile(strSrcPath + "\\" + strFile, strDir, FALSE);
				continue;
			}
		}

		CString strSrc = strSrcPath + "\\" + strFile;
		if (!bDemo)
		{
			if (!bMainASF && atoi(strArrayParam[3]) == 0)
			{
				int nAudioBPS = atoi(strArrayParam[7]);
				int nVideoBPS, nMono, nSampleRate;
				if (nAudioBPS < 56)
				{
					nVideoBPS = 32;
					nAudioBPS = 16;
					nSampleRate = 16000;
					nMono = 1;
				}
				else if (nAudioBPS < 96)
				{
					nVideoBPS = 72;
					nAudioBPS = 16;
					nSampleRate = 16000;
					nMono = 1;
				}
				else if (nAudioBPS < 128)
				{
					nVideoBPS = 88;
					nAudioBPS = 32;
					nSampleRate = 32000;
					nMono = 2;
				}
				else
				{
					nVideoBPS = 152;
					nAudioBPS = 32;
					nSampleRate = 32000;
					nMono = 2;
				}
				//add by buxiangyi
				/*if (!ConvertBandWidth(strSrc, strDir, nVideoBPS * 1024, 20, 5, 0, 
				nAudioBPS * 1000, nSampleRate, nMono, ""))
				{
				W_SHOW_ERROR(IDS_ERROR_CONVERT, this);
				return FALSE;
				}*/
			}
			else
			{
				//add by buxiangyi
				/*if (!ConvertBandWidth(strSrc, strDir, atoi(strArrayParam[3]) * 1024, atoi(strArrayParam[4]),
				atoi(strArrayParam[5]), atoi(strArrayParam[6]),atoi(strArrayParam[7]) * 1000, 
				atoi(strArrayParam[8]), (strArrayParam[9] == "S") ? 2 : 1, ""))
				{
				W_SHOW_ERROR(IDS_ERROR_CONVERT, this);
				return FALSE;
				}*/
			}
		}

		else
		{
			//add by buxiangyi
			/*if (!ConvertBandWidth(strSrc, strDir, atoi(strArrayParam[12]) * 1024, atoi(strArrayParam[13]),
			atoi(strArrayParam[14]), atoi(strArrayParam[15]),atoi(strArrayParam[16]) * 1000, 
			atoi(strArrayParam[17]), (strArrayParam[18] == "S") ? 2 : 1, strArrayParam[10]))
			{
			W_SHOW_ERROR(IDS_ERROR_CONVERT, this);
			return FALSE;
			}*/
		}

		//if (!ConvertCourse(strSrcPath + "\\" + strFile, strDir, nConvVideo, nConvAudio, nConvFPS))
		//	return FALSE;
	}

	return TRUE;
}

int CCourseLoaderDlg::GetNumberPart(CString str)
{
	int i = 0;
	for (i = 0; i < str.GetLength(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			break;
	}

	return atoi(str.Left(i));
}

BOOL CCourseLoaderDlg::IsOurMedia(CString strFile)
{
	strFile.MakeLower();
	if (strFile.Right(4) != ".mp4")
		return FALSE;

	if (strFile == "default.mp4")
		return TRUE;

	if (strFile.Left(4) == "demo")
		return TRUE;

	return FALSE;
}

void CCourseLoaderDlg::UpdateMediaFile(CString strSrcPath, CString strPath, CString strMediaServer)
{
	CStringArray strArrayTag, strArrayReplace;
	strArrayTag.Add("var g_strServer=\"videos/\";");


	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);
	CButton* pScorm = (CButton*)GetDlgItem(IDC_RADIO_SCORM);

	if (pProtocolHTTPNM->GetCheck() && pScorm->GetCheck())
	{
			CButton* pScormMediaZip = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
			int nScormMediaZip = pScormMediaZip->GetCheck();

			if(nScormMediaZip)
			{
				strArrayReplace.Add("var g_strServer=\"videos/\";");
			}
			else
			{
				strArrayReplace.Add("var g_strServer=\"\";");

			}
	}
	else
	{
		strArrayReplace.Add("var g_strServer=\"" + strMediaServer + "/\";");
	}


	//strArrayReplace.Add("var g_strServer=\"" + strMediaServer + "/\";");
	strArrayTag.Add("var g_strDefaultASF=\"videos/default.mp4\";");
	strArrayReplace.Add("var g_strDefaultASF=\"default.mp4\";");

	//wang fix webpage 20090315
	strArrayTag.Add("../webpages/");
	strArrayReplace.Add("../webpages/");

	::UpdateFileByTag(strPath + "\\html", "_frame.htm", strArrayTag, strArrayReplace);

	CStringArray strArrayASFFile;
	::GetFiles(strSrcPath + "\\html", "mp4", strArrayASFFile);
	strArrayTag.RemoveAll();
	strArrayReplace.RemoveAll();
	for (int i = 0; i < strArrayASFFile.GetSize(); i++)
	{
		CString strFile = strArrayASFFile[i];
		if (IsOurMedia(strFile))
			continue;
		strFile.MakeLower();
		strArrayTag.Add(strFile);
		strArrayReplace.Add(strMediaServer + "/" + strFile);
	}

	CStringArray strArrayHTMFile;
	::GetFiles(strPath + "\\html", "htm*", strArrayHTMFile);
	for (int i = 0; i < strArrayHTMFile.GetSize(); i++)
	{
		CString strFile = strArrayHTMFile[i];
		::UpdateFileByTag(strPath + "\\html", strFile, strArrayTag, strArrayReplace);
	}
}

BOOL CCourseLoaderDlg::CreateIndex(CString strFile)
{
	if (!::FileExists(strFile))
		return TRUE;

	CComBSTR bstrSource(strFile);
	VARIANT_BOOL vbool = OAFALSE;
	//add by buxiangyi   ˆê?
	/*HRESULT hval = m_pWMXUtility->IsWMXIndexExisted(bstrSource, &vbool);
	if (FAILED(hval)) 
		return FALSE;

	if (vbool == OAFALSE) 
	{
		hval = m_pWMXUtility->WMXMakeIndex(bstrSource);
		if (FAILED(hval)) 
			return FALSE;
	}*/

	return TRUE;
}
//add by buxiangyi
//BOOL CCourseLoaderDlg::ConvertBandWidth(CString strSrc, CString strDest, 
//										int nVideoBitRate, int nFPS, int nSec, int nQuality, 
//										int nAudioBitRate, int nSampleRate, int nMono, CString strCodec)
//{
//	if (!::FileExists(strSrc))
//		return FALSE;
//
//	CComBSTR cbstrSrcPath(strSrc);
//
//	HRESULT hr;
//	
//	AUDIOPARAM ainfo;
//	VIDEOPARAM vinfo;
//	hr = m_pWMXUtility->GetWMXProfileInfo(cbstrSrcPath, &ainfo, &vinfo, NULL);
//	if (FAILED(hr))
//		return FALSE;
//
//	DWORD dwWidth = vinfo.dwWidth;
//	DWORD dwHeight = vinfo.dwHeight;
//	DWORD dwFormatTag = ainfo.dwFormatTag;
//	DWORD dwFourCC = vinfo.dwFourCC;
//
//	CString strTempPath =  ::GetSysPath() + "temp";
//	if (!::FileExists(strTempPath))
//		_mkdir(strTempPath);
//	CString strTmpNewProfile = strTempPath + "\\_tmp.prx";
//	CComBSTR cbstrDestProfilePath(strTmpNewProfile);
//	
//	PROFILEPARAMS pfparam;
//	pfparam.ainfo.dwBitrate = (DWORD)nAudioBitRate;
//	if (strCodec == "")
//		pfparam.ainfo.dwFormatTag = dwFormatTag;
//	else if (strCodec == "ACELP.net")
//		pfparam.ainfo.dwFormatTag = CODEC_AUDIO_ACELP;
//	else
//		pfparam.ainfo.dwFormatTag = CODEC_AUDIO_MSAUDIO;
//	pfparam.ainfo.dwSampleRate = (DWORD)nSampleRate;
//	pfparam.ainfo.dwChannels = (DWORD)nMono;		
//	pfparam.vinfo.dwFourCC = dwFourCC;
//	pfparam.vinfo.dwBitrate = (DWORD)nVideoBitRate;
//	pfparam.vinfo.dwHeight = dwHeight;
//	pfparam.vinfo.dwWidth = dwWidth;
//	pfparam.vinfo.dwFPS = (DWORD)nFPS;
//	pfparam.vinfo.dwQuality = (DWORD)nQuality;
//	pfparam.vinfo.dwSecPerKey = (DWORD)nSec;
//	pfparam.sinfo.dwBitrate = (DWORD)0;
//
//	hr = m_pWMXUtility->PRXGen(&pfparam, cbstrDestProfilePath);
//	if (FAILED(hr)) 
//	return FALSE; 
//
//	CComBSTR cbstrDestPath(strDest);
//	CString strExt = ::GetFileExt(strSrc);
//	strExt.MakeLower();
//	//AfxMessageBox("1");
//	hr = m_pWMXUtility->WMXConvert(cbstrSrcPath, OAFALSE, OATRUE, OATRUE, cbstrDestProfilePath, cbstrDestPath);
//	if (FAILED(hr))
//	return FALSE;
//   //	AfxMessageBox("2");
//	/* wangwang
//	if (!CreateIndex(strDest))
//
//
//		return FALSE;
//    */
//	::DeleteFile(strTmpNewProfile);
//	
//	return TRUE;
//}

/*
BOOL CCourseLoaderDlg::ConvertCourse(CString strSrcPath, CString strDestPath, 
									 int nVideo, int nAudio, int nFPS)
{
	CONVERTPARAM ConvParam;
	memset(&ConvParam, 0, sizeof(ConvParam));
	ConvParam.dwAudioKBPS = nAudio;
	ConvParam.vbVideo = (nVideo == 0) ? OAFALSE : OATRUE;
	ConvParam.dwVideoFPS = nFPS;
	ConvParam.dwVideoKBPS = nVideo;
	ConvParam.vbScript = OATRUE;
	ConvParam.dwScriptKBPS = 2;
	ConvParam.vbHeader = OATRUE;
	ConvParam.vbHeaderMarker = OATRUE;
	ConvParam.vbHeaderScript = OATRUE;	

	HRESULT hval = E_FAIL; // For Error Checking!!!
	BSTR bstrSource = strSrcPath.AllocSysString();
	BSTR bstrOutput = strDestPath.AllocSysString();
	if (bstrSource != NULL && bstrOutput != NULL) 
	{
		hval = m_pUtility->ConvertAsf(bstrSource, bstrOutput, &ConvParam);
		if(SUCCEEDED(hval))	
			hval = m_pUtility->MakeIndex(bstrOutput);
	}
	
	if(bstrSource != NULL) 
		SysFreeString(bstrSource);
	
	if(bstrOutput != NULL) 
		SysFreeString(bstrOutput);
	
	if (!SUCCEEDED(hval)) 
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_CONVERT_ASF);
		strMsg.Format(strTmp, strSrcPath, strDestPath);
		W_SHOW_ERROR_STR(strMsg, this);

		return FALSE;
	}

	return TRUE;
}
*/

void CCourseLoaderDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	//if (m_pUtility)
	//{
	//	m_pUtility->Release();
	//	m_pUtility = NULL;
	//}
	//add by buxiangyi  ˆê?
	/*if (m_pWMXUtility)
	{
	m_pWMXUtility->Release();
	m_pWMXUtility = NULL;
	}*/
	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}

	if (m_pInetSession != NULL)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}

	m_dlgProgress.DestroyWindow();

	m_pGrid->DestroyWindow();
	delete m_pGrid;

	m_pBtnOpenFTPPath->DestroyWindow();
	delete m_pBtnOpenFTPPath;

	m_pBtnOpenFilePath->DestroyWindow();
	delete m_pBtnOpenFilePath;

	m_pBtnOpenScormPath->DestroyWindow();
	delete m_pBtnOpenScormPath;

	CoUninitialize();
}

BOOL CCourseLoaderDlg::SetOption(int nSel) 
{
	CString strTitle;
	strTitle.LoadString(IDS_SRV_SETTING_CAPTION);

	CSheetSetting shSetting(strTitle);
	shSetting.m_nSelPage = nSel;
	int nRet = shSetting.DoModal();
	if (nRet == IDOK)
		return TRUE;

	return FALSE;
}

BOOL CCourseLoaderDlg::CopyDirToFTPServer(CString strSrcPath, CString strSubDir, CString strFtpSite)
{
	if (m_pFtpConnection == NULL)
		return FALSE;

	CString strParentName;
	m_pFtpConnection->GetCurrentDirectory(strParentName); 

	if (!FTPCreateDir(strSubDir, strFtpSite))
		return FALSE;

	if (strSrcPath.Right(1) != "\\")
		strSrcPath += "\\";

	CString strPath = strSrcPath + strSubDir;
	CStringArray strArrayFile, strArrayDir;
	::GetFiles(strPath, "*", strArrayFile);
	::GetDir(strPath, strArrayDir);

	for (int i = 0; i < strArrayFile.GetSize(); i++)
	{
		CString strFile = strArrayFile[i];
		CString strCopyFrom = strPath + "\\" + strFile;
		
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_COPY_FILE_TO_FTP);
		strMsg.Format(strTmp, strFile);
		SetProgress(strMsg);
		if (!PeekAndPump())
			return FALSE;

		if (!PutFile(strCopyFrom, strFile))
			return FALSE;

		::DeleteFile(strCopyFrom);
	}

	for (int i = 0; i < strArrayDir.GetSize(); i++)
	{
		if (strArrayDir[i].CompareNoCase("video") != 0)
		{
			if (!CopyDirToFTPServer(strPath, strArrayDir[i], strFtpSite))
				return FALSE;
		}

		if (!m_pFtpConnection->SetCurrentDirectory(strParentName))
			return FALSE;
	}

	if (!m_pFtpConnection->SetCurrentDirectory(strParentName))
		return FALSE;

	return TRUE;
}

BOOL CCourseLoaderDlg::PutFile(CString strCopyFrom, CString strFile)
{
	if (!m_pFtpConnection->PutFile(strCopyFrom, strFile))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_PUT_FILE);
		strMsg.Format(strTmp, strCopyFrom);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::ConnectToMediaServer(CString strNewCourse, CString& strFtpSite)
{
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("MediaServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	strFtpSite = szBuf;

	::GetPrivateProfileString("MediaServer", "LoginName", "", szBuf, 256, strIniPath);
	CString strUserName = szBuf;

	::GetPrivateProfileString("MediaServer", "Password", "", szBuf, 256, strIniPath);
	char szPassword[256];
	KeyDecode(szBuf, szPassword);
	CString strPWD = szPassword;
	
	::GetPrivateProfileString("MediaServer", "SubDir", "", szBuf, 256, strIniPath);
	CString strRemotePath = szBuf;

	::GetPrivateProfileString("MediaServer", "Port", "21", szBuf, 256, strIniPath);
	CString strPort = szBuf;

	CString strTmp, strMsg, strTitle;
	strTmp.LoadString(IDS_TRANSMIT_FILE);
	strTitle.Format(strTmp, strNewCourse);
	strTmp.LoadString(IDS_CONNECT_TO_FTP);
	strMsg.Format(strTmp, strFtpSite);
	SetProgress(strTitle, strMsg);

	int bPassive = ::GetPrivateProfileInt("General", "PassiveFTP", 0, strIniPath);
	if (!ConnectToFTP(strFtpSite, strUserName, strPWD, strRemotePath, strPort, bPassive))
		return FALSE;

	return TRUE;
}

BOOL CCourseLoaderDlg::FTPCreateDir(CString strNewCourse, CString strFtpSite)
{
	if (m_pFtpConnection != NULL)
	{
		CFtpFileFind ftpFind(m_pFtpConnection);
		BOOL bContinue = ftpFind.FindFile(strNewCourse);
		ftpFind.Close();

		BOOL bEnter = FALSE;
		if (!bContinue)
		{
			if (!m_pFtpConnection->CreateDirectory(strNewCourse))
			{
				if (!m_pFtpConnection->SetCurrentDirectory(strNewCourse))
				{
					CString strTmp, strMsg;
					strTmp.LoadString(IDS_FAIL_TO_CREATE_FTP_DIR);

					strMsg.Format(strTmp, strNewCourse, strFtpSite);

	#ifdef _CHT
					strMsg.Format(strTmp, strFtpSite, strNewCourse);
	#endif

	#ifdef _CHS
					strMsg.Format(strTmp, strFtpSite, strNewCourse);
	#endif

					W_SHOW_ERROR_STR(strMsg, this);
					return FALSE;
				}
				else
					bEnter = TRUE;
			}
		}
	
		if (!bEnter)
		{
			if (!m_pFtpConnection->SetCurrentDirectory(strNewCourse))
			{
				CString strTmp, strMsg;
				strTmp.LoadString(IDS_FAIL_TO_CHANGE_DIR);

				strMsg.Format(strTmp, strNewCourse, strFtpSite);

	#ifdef _CHT
				strMsg.Format(strTmp, strFtpSite, strNewCourse);
	#endif

	#ifdef _CHS
				strMsg.Format(strTmp, strFtpSite, strNewCourse);
	#endif

				W_SHOW_ERROR_STR(strMsg, this);
				return FALSE;
			}
		
			return TRUE;
		}
		else
			return TRUE;
	}

	return FALSE;
}

BOOL CCourseLoaderDlg::ConnectToWebServer(CString& strFtpSite)
{
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("WebServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	strFtpSite = szBuf;

	::GetPrivateProfileString("WebServer", "LoginName", "", szBuf, 256, strIniPath);
	CString strUserName = szBuf;

	::GetPrivateProfileString("WebServer", "Password", "", szBuf, 256, strIniPath);
	char szPassword[256];
	KeyDecode(szBuf, szPassword);
	CString strPWD = szPassword;
	
	::GetPrivateProfileString("WebServer", "SubDir", "", szBuf, 256, strIniPath);
	CString strRemotePath = szBuf;

	::GetPrivateProfileString("WebServer", "Port", "21", szBuf, 256, strIniPath);
	CString strPort = szBuf;

	//CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_PASSIVE);
	//int bPassive = pCheck->GetCheck();
	int bPassive = ::GetPrivateProfileInt("General", "PassiveFTP", 0, strIniPath);
	if (!ConnectToFTP(strFtpSite, strUserName, strPWD, strRemotePath, strPort, bPassive))
		return FALSE;

	return TRUE;
}

BOOL CCourseLoaderDlg::CheckURL(CString strFtpSite)
{
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL(strFtpSite, dwServiceType, strServerName, strObject, nPort))
	{
		// try adding the "ftp://" protocol
		CString strFtpURL = _T("ftp://");
		strFtpURL += strFtpSite;

		if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, nPort))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_INVALID_URL);
			strMsg.Format(strTmp, strFtpSite);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCourseLoaderDlg::ConnectToFTP(CString strFtpSite, CString strUserName, 
									CString strPWD, CString strRemotePath, CString strPort, BOOL bPassive) 
{
	if (m_pFtpConnection != NULL)
		m_pFtpConnection->Close();
	delete m_pFtpConnection;
	m_pFtpConnection = NULL;

	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL(strFtpSite, dwServiceType, strServerName, strObject, nPort))
	{
		// try adding the "ftp://" protocol
		CString strFtpURL = _T("ftp://");
		strFtpURL += strFtpSite;

		if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, nPort))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_INVALID_URL);
			strMsg.Format(strTmp, strFtpSite);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}

	CWaitCursor cursor;	// this will automatically display a wait cursor

	// Now open an annonymous FTP connection to the server
	if ((dwServiceType == INTERNET_SERVICE_FTP) && !strServerName.IsEmpty())
	{
		try
		{
			m_pFtpConnection = m_pInetSession->GetFtpConnection(strServerName, strUserName, 
																strPWD, atoi(strPort), bPassive);
		}
		catch (CInternetException* pEx)
		{
			// catch errors from WinINet
			TCHAR szErr[1024];
			if (pEx->GetErrorMessage(szErr, 1024))
				W_SHOW_ERROR_STR(szErr, this);
			else
				W_SHOW_ERROR(IDS_EXCEPTION, this);
			pEx->Delete();

			m_pFtpConnection = NULL;

			return FALSE;
		}
	}
	else
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_INVALID_URL);
		strMsg.Format(strTmp, strFtpSite);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}

	if (m_pFtpConnection != NULL && strRemotePath != "")
	{
		if (!m_pFtpConnection->SetCurrentDirectory(strRemotePath))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FAIL_TO_CHANGE_DIR);
			
			strMsg.Format(strTmp, strRemotePath, strFtpSite);

#ifdef _CHT
			strMsg.Format(strTmp, strFtpSite, strRemotePath);
#endif

#ifdef _CHS
			strMsg.Format(strTmp, strFtpSite, strRemotePath);
#endif

			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}

	return TRUE;
}

void CCourseLoaderDlg::SetProgress(CString strTitle, CString strMsg)
{
	CString strOldTitle;
	m_dlgProgress.GetWindowText(strOldTitle);
	if (strOldTitle != strTitle)
		m_dlgProgress.SetWindowText(strTitle);
	
	CString strOldMsg;
	m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->GetWindowText(strOldMsg);
	if (strOldMsg != strMsg)
		m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->SetWindowText(strMsg);

	int nIndex;
	if ((nIndex = strMsg.Find("(")) >= 0)
	{
		m_strMsg = strMsg.Left(nIndex);
		m_strMsg.TrimRight();
	}
	else
		m_strMsg = strMsg;

	m_strTitle = strTitle;
}

void CCourseLoaderDlg::SetProgress(CString strMsg)
{
	CString strOldMsg;
	m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->GetWindowText(strOldMsg);
	if (strOldMsg != strMsg)
		m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->SetWindowText(strMsg);

	int nIndex;
	if ((nIndex = strMsg.Find("(")) >= 0)
	{
		m_strMsg = strMsg.Left(nIndex);
		m_strMsg.TrimRight();
	}
	else
		m_strMsg = strMsg;
}

void CCourseLoaderDlg::ShowProgress()
{
	m_dlgProgress.Show();
	m_dlgProgress.SetFocus();
}

void CCourseLoaderDlg::HideProgress()
{
	m_dlgProgress.Hide();
}

LRESULT CCourseLoaderDlg::OnProgress(WPARAM wParam, LPARAM lParam)
{
	int i = HIWORD(lParam); 
	int j = LOWORD(lParam); 
	int k;
	CString strPerc = "";
	if (i == 0)
		k = j;
	else
	{
		k = (int)((float)j / i * 100.0);
		if (k > 100)
			k = 100;
		strPerc = "%";
	}

	CString strMsg;
	strMsg.Format("%s (%d%s)", m_strMsg, k, strPerc);
	SetProgress(m_strTitle, strMsg);
	m_dlgProgress.PeekAndPump();

	return 0L;
}

BOOL CCourseLoaderDlg::WriteDB(CString strCourseName, CString strLecturer, CString strCompany,
							   CString strDate, CString strURL, CString strContent)
{
/*
	CDBObject* pDBObj = new CDBObject;
	BOOL b = pDBObj->Open("DSN=MCG;UID=sa;PWD=");
	if (!b)
	{
//		W_SHOW_ERROR(IDS_FAIL_TO_OPEN_DB, this);
		delete pDBObj;
		return FALSE;
	}

	if (strDate.GetLength() == 10)
		strDate = strDate.Left(4) + strDate.Mid(5, 2) + strDate.Mid(8, 2);
	CString strSQL = "Insert Into COURSE_BAS(COURSE_NAME,LECTURER,COMPANY,COURSE_DATE,COURSE_URL,ABSTRACT) Values (";
	strSQL += "'" + strCourseName + "','" + strLecturer + "','" + strCompany + "','" + strDate + "','" + strURL + 
		      "','" + strContent + "')";

	if (!pDBObj->ExecuteSQLStmt(strSQL))
	{
		delete pDBObj;
		return FALSE;
	}
	
	//CStringArray TableArray;
	//pDBObj->EnumTable(TableArray);
	//for (int i = 0; i < TableArray.GetSize(); i++)
	//	AfxMessageBox(TableArray[i]);

	delete pDBObj;
*/
	return TRUE;
}

void CCourseLoaderDlg::OnServerSetting() 
{
	SetOption(0);
}

void CCourseLoaderDlg::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CCourseLoaderDlg::OnTypeSetting() 
{
	CDlgTypeSet	dlg;
	dlg.DoModal();
	ResetCheckItem();
}

void CCourseLoaderDlg::OnTransmit() 
{
	CDlgTransCourse	dlg;
	if (dlg.DoModal() != IDOK)
		return;

	CString strNewCourse = dlg.m_strCourse;
	CString strDestPath = ::GetSysPath() + "Launched Courses\\" + strNewCourse;
	CStringArray strArrayPostfix;
	::GetDir(strDestPath, strArrayPostfix);
	for (int i = strArrayPostfix.GetSize() - 1; i >= 0; i--)
	{
		CString strPath = strArrayPostfix[i];
		if (strPath.Left(2) == "v_")
			strArrayPostfix.RemoveAt(i);
	}

	CString strTitle = "";
	CString strMsg = "";
	SetProgress(strTitle, strMsg);
	ShowProgress();

	if (!TransferByFTP("", strNewCourse, strDestPath, strArrayPostfix))
	{
		HideProgress();
		return;
	}

	int nFileCount =  ::GetFileCount(strDestPath, NULL);
	if (nFileCount == 0)
		::DelTree(strDestPath);

	HideProgress();
	W_SHOW_MESSAGE(IDS_COURSE_TRANSMITTED, this);
}

void CCourseLoaderDlg::OnOption() 
{
	CDlgOption dlg;
	dlg.DoModal();
}

void CCourseLoaderDlg::ConvBackSlashToSlash(CString& strInput)
{
	for (int i = 0; i < strInput.GetLength(); i++)
		if (strInput[i] == '\\')
			strInput.SetAt(i, '/');
}

void CCourseLoaderDlg::OnViewCourse() 
{
	if (m_strArrayFTPCourse.GetSize() > 0)
	{
		CString strFTPCourse;
		if (m_strArrayFTPCourse.GetSize() == 1)
		{
			strFTPCourse = m_strArrayFTPCourse[0];

			CString strURL;
			if (!GetCourseURL(strFTPCourse, strURL))
			{
				W_SHOW_WARNING(IDS_SET_COURSE_URL_FIRST, this);
				SetOption(0);
				return;
			}

			::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
		}
		else
		{
			CDlgViewSelect dlgView;
			dlgView.m_pstrArrayCourse = &m_strArrayFTPCourse;
			dlgView.DoModal();
		}
	}
	else
		W_SHOW_WARNING(IDS_NOT_TRANS_ANY_COURSE, this);
}

void CCourseLoaderDlg::OnRadioFtp() 
{
	m_pBtnOpenFTPPath->EnableButton(FALSE);	
	m_pBtnOpenFilePath->EnableButton(FALSE);	
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->EnableWindow(FALSE);
	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	pProtocolMMS ->EnableWindow(TRUE);
	pProtocolRTSP ->EnableWindow(TRUE); 
	pProtocolHTTP ->EnableWindow(TRUE); 
	pProtocolHTTPNM ->EnableWindow(FALSE); 

}

void CCourseLoaderDlg::OnRadioFtpLocal() 
{
	m_pBtnOpenFTPPath->EnableButton(FALSE);	
	m_pBtnOpenFilePath->EnableButton(TRUE);	
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->EnableWindow(TRUE);
	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	pProtocolMMS ->EnableWindow(FALSE);
	pProtocolRTSP ->EnableWindow(FALSE); 
	pProtocolHTTP ->EnableWindow(FALSE); 
	pProtocolHTTPNM ->EnableWindow(FALSE); 

}

void CCourseLoaderDlg::OnRadioFtpManual() 
{
	m_pBtnOpenFTPPath->EnableButton(TRUE);	
	m_pBtnOpenFilePath->EnableButton(FALSE);	
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->EnableWindow(FALSE);
	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	pProtocolMMS ->EnableWindow(TRUE);
	pProtocolRTSP ->EnableWindow(TRUE); 
	pProtocolHTTP ->EnableWindow(TRUE); 
	pProtocolHTTPNM ->EnableWindow(FALSE); 

}

void CCourseLoaderDlg::OnButtonDeselectAll() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 0);
}

void CCourseLoaderDlg::OnButtonSelectAll() 
{
	for (int iItem = 0; iItem < m_clCourse.GetCount(); iItem++ )
		m_clCourse.SetCheck(iItem, 1);
}

void CCourseLoaderDlg::LoadHistory() 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char* pBuf = new char[2048];

	::GetPrivateProfileString("History", "FTPPath", "", pBuf, 2048, strIniPath);
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->SetWindowText(pBuf);
	
	::GetPrivateProfileString("History", "FilePath", "", pBuf, 2048, strIniPath);
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->SetWindowText(pBuf);

	::GetPrivateProfileString("History", "DestSel", "0", pBuf, 2048, strIniPath);
	CString strCheck = pBuf;
	CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
	CButton* pCheckFile = (CButton*)GetDlgItem(IDC_EDIT_PATH_LOAD_LATER);
	if (strCheck == "0")
	{
		pCheckFTP->SetCheck(1);
		OnRadioFtp(); 
	}
	else if (strCheck == "1")
	{
		pCheckFTPManual->SetCheck(1);
		OnRadioFtpManual();
	}
	else if (strCheck == "2")
	{
		pCheckFile->SetCheck(1);
		OnRadioFtpLocal();
	}

	delete [] pBuf;
}

void CCourseLoaderDlg::WriteHistory() 
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	CString strTmp;
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->GetWindowText(strTmp);
	::WritePrivateProfileString("History", "FTPPath", strTmp, strIniPath);
	
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->GetWindowText(strTmp);
	::WritePrivateProfileString("History", "FilePath", strTmp, strIniPath);

	CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
	CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);
	int nCheckFTP = pCheckFTP->GetCheck();
	int nCheckFTPManual = pCheckFTPManual->GetCheck();
	int nCheckFile = pCheckFile->GetCheck();
	CString strCheck;	
	if (nCheckFTP)
		strCheck = "0";
	else if (nCheckFTPManual)
		strCheck = "1";
	else if (nCheckFile)
		strCheck = "2";
	::WritePrivateProfileString("History", "DestSel", strCheck, strIniPath);
}

void CCourseLoaderDlg::OnCancel() 
{
	if (m_dlgProgress.IsWindowVisible())
		return;

	WriteHistory();
	
	CDialog::OnCancel();
}

void CCourseLoaderDlg::OnClose() 
{
	if (m_dlgProgress.IsWindowVisible())
		return;

	WriteHistory();

	CDialog::OnClose();
}

LRESULT CCourseLoaderDlg::OnGrdClick(WPARAM wParam, LPARAM lParam)
{
	int nCol = LOWORD(lParam);
	int nRow = HIWORD(lParam);
	if (nCol == 0)
	{
		int nValue = atoi(m_pGrid->GetDataAt(nRow, nCol));
		if (!nValue)
		{
			SetLVCheck(nRow, 1);
			m_pGrid->RePaintCell(nRow, 0);
			for (int i = 0; i < m_pGrid->GetDataSize(); i++)
			{
				if (i != nRow)
				{
					if (GetLVCheck(i) != 0)
					{
						SetLVCheck(i, 0);
						m_pGrid->RePaintCell(i, 0);
					}
				}
			}
		}

		WriteCheckStatus(); 
	}
	return 0L;
}

LRESULT CCourseLoaderDlg::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	CString strPath;
	switch (nID)
	{
		case ID_BTN_OPEN_FTP:
			GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->GetWindowText(strPath);
			SelFolder(strPath);
			GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->SetWindowText(strPath);
		break;

		case ID_BTN_OPEN_FILE:
			GetDlgItem(IDC_EDIT_PATH_DIRECT)->GetWindowText(strPath);
			SelFolder(strPath);
			GetDlgItem(IDC_EDIT_PATH_DIRECT)->SetWindowText(strPath);
		break;

		case ID_BTN_OPEN_SCORM:
			GetDlgItem(IDC_EDIT_PATH_SCORM)->GetWindowText(strPath);
			SelFolder(strPath);
			GetDlgItem(IDC_EDIT_PATH_SCORM)->SetWindowText(strPath);
		break;
	}

	return 0L;
}

void CCourseLoaderDlg::SelFolder(CString& strFolder) 
{
	CString strLabel;
	strLabel.LoadString(ID_BTN_OPEN_FILE);
	CString strFolderPath;
	if (::GetFolder(&strFolderPath, strLabel, m_hWnd, NULL, strFolder))
		strFolder = strFolderPath;
}

void CCourseLoaderDlg::OnButtonSetType() 
{
	OnTypeSetting();
}

BOOL CCourseLoaderDlg::IsCourseVideoModified(CString strCourse, BOOL* pbOnlyTagModified)
{
	if (pbOnlyTagModified)
		*pbOnlyTagModified = FALSE;

	CStringArray strArrayData, strArrayPreData;
	CString strSrcPath = MACRO_RECORD_VIDEO(strCourse);
	CString strDestPath = MACRO_PRE_RECORD_VIDEO(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;
	
	strSrcPath = MACRO_RECORD_AUDIO_MIX(strCourse);
	strDestPath = MACRO_PRE_RECORD_AUDIO_MIX(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_AUDIO_REPLACE(strCourse);
	strDestPath = MACRO_PRE_RECORD_AUDIO_REPLACE(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_VIDEO_TEXT(strCourse);
	strDestPath = MACRO_PRE_RECORD_VIDEO_TEXT(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_VIDEO_EFFECT(strCourse);
	strDestPath = MACRO_PRE_RECORD_VIDEO_EFFECT(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_TAG(strCourse);
	strDestPath = MACRO_PRE_RECORD_TAG(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
	{
		if (pbOnlyTagModified)
			*pbOnlyTagModified = TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL CCourseLoaderDlg::IsArrayDifferent(CStringArray& strArray1, CStringArray& strArray2)
{
	if (strArray1.GetSize() != strArray2.GetSize())
		return TRUE;

	for (int i = 0; i < strArray1.GetSize(); i++)
		if (strArray1[i] != strArray2[i])
			return TRUE;

	return FALSE;
}

void CCourseLoaderDlg::OnButtonInputInfo() 
{
	CDlgInputScornInfo dlg;

	int nCount = m_clCourse.GetCount();
	CString strTmp;
	for (int i = 0; i < nCount; i++)
	{
		m_clCourse.GetText(i, strTmp);
		strTmp.TrimLeft();
		
		dlg.m_strArrayCourse.Add(strTmp);
	}

	dlg.DoModal();
}

void CCourseLoaderDlg::AddZipFolder(CString strZipPath, CString strFolder) 
{
	CZipArchive* pZip ;
	pZip = new CZipArchive;
	//pZip->SetAdvanced(1500000);
	
	pZip->SetAdvanced(65536,65536,32768);

	BOOL bAfterException = false;
	try
	{
		//pZip->Open(strZipPath, CZipArchive::create, 1457500);
		//pZip->Open(strZipPath, CZipArchive::zipCreate, 1457500);
		pZip->Open(strZipPath, CZipArchive::zipCreate);
		//pZip->SetRootPath("d:\\1234\\Visual Basic .NET");
		//CString strOutputPath1;
		//GetOutputPath(strOutputPath1);
		pZip->SetRootPath(strFolder);
	}
	catch (CException* e)
	{
		e->Delete();
		bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		bAfterException = true;
	}
	if (bAfterException)
	{
		AfxMessageBox(_T("Create new failed"), MB_ICONSTOP);
		ArchiveClose(pZip, true);
		delete pZip;
		m_progress.Hide();
		return;
	}

	if (strFolder.Right(1) == "\\")
		strFolder = strFolder.Left(strFolder.GetLength() - 1);
	
	CProgressInfo p;
	p.m_pProgress = &m_progress;
	CAddInfoList list;
	AddFolder(strFolder, list, p);
	m_progress.Init(p.m_iTotal);

	bAfterException = false;
	try
	{
		CButton* pCheckZIP1 = (CButton*)GetDlgItem(IDC_RAD_ZIP1);
		CButton* pCheckZIP6 = (CButton*)GetDlgItem(IDC_RAD_ZIP6);
		CButton* pCheckZIP9 = (CButton*)GetDlgItem(IDC_RAD_ZIP9);
	
		int nCheckZIP;
	
		if(pCheckZIP6->GetCheck())
		{
			nCheckZIP = 6;
		}
		else if(pCheckZIP1->GetCheck())
		{
			nCheckZIP = 1;
		}
		else if(pCheckZIP9->GetCheck())
		{
			nCheckZIP = 9;
		}

		POSITION pp = list.GetHeadPosition();
		while (pp)
		{
			CAddInfo* padi = &list.GetNext(pp);
			//pZip->SetRootPath("d:\\1234\\Visual Basic .NET");
			//pZip->AddNewFile(padi->m_szName, strFolder.GetLength(), -1, true);
			pZip->AddNewFile(padi->m_szName, nCheckZIP, false);
			p.m_iTotalSoFar += padi->m_iSize;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		bAfterException = true;		
	}
	if (bAfterException)
	{
		AfxMessageBox(_T("Add folder failed.  Closing the archive."), MB_ICONSTOP);
		ArchiveClose(pZip, true);
		delete pZip;
		m_progress.Hide();
		return;
	}

	ArchiveClose(pZip, false);
	delete pZip;
	m_progress.Hide();
}

void CCourseLoaderDlg::ArchiveClose(CZipArchive* pZip, bool bExcep) 
{
	bool berr = false;
	try
	{
		pZip->Close(bExcep);	
	}
	catch (CException* e)
	{
		e->Delete();
		berr = true;
	}
	catch(...) // thrown in the STL version
	{
		berr = true;		
	}
	if (berr)
		AfxMessageBox(_T("Close failed"), MB_ICONSTOP);
}

void CCourseLoaderDlg::AddFolder(CString szFolder, CAddInfoList& list, CProgressInfo& p)
{
	szFolder.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(szFolder + _T("\\*"));
	CStringArray folders;
	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			CString strPath = ff.GetFilePath();
			CString strFile = ::GetFileName(strPath);
			if (strFile.Left(2) != "v_") 
				folders.Add(strPath);
		}
		else
		{
			CAddInfo ai;
			ai.m_szName = ff.GetFilePath();
			CFileStatus fs;
			if (CFile::GetStatus(ai.m_szName, fs))
			{
				ai.m_bDir = false;
				ai.m_iSize = fs.m_size;
				list.AddTail(ai);
				p.m_iTotal += fs.m_size;
			}
		}

	}
	for (int i = 0; i < folders.GetSize(); i++)
	{
		list.AddTail(CAddInfo(folders[i], 0, true));
												// add the folder before adding its files
												// it is not needed to add the root folder
		AddFolder(folders[i], list, p);
	}
}


void CCourseLoaderDlg::OnUpdateTypeSetting(CCmdUI* pCmdUI) 
{
}

void CCourseLoaderDlg::OnRadioScorm() 
{
	
	CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
	CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);
	CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
	CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);

	CButton* pScormDefault = (CButton*)GetDlgItem(IDC_RAD_DEFAULT);
	pScormDefault->EnableWindow(TRUE);
	CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	pScormAenrich->EnableWindow(TRUE);

	CButton* pScormEnhance = (CButton*)GetDlgItem(IDC_CHECK_ENHANCE);
	pScormEnhance->EnableWindow(TRUE);

	CButton* pScormMediaZIP = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
	pScormMediaZIP->EnableWindow(TRUE);		// add itoh 2012/12/13
/*	del itoh 2013/01/31
	int nCheckFTPManual = pCheckFTPManual->GetCheck();
	int nScormFTP = pScormFTP->GetCheck();

	if (nCheckFTPManual == 1 && nScormFTP == 0)
	{
		pScormMediaZIP->EnableWindow(TRUE);
	}
	else
	{
		pScormMediaZIP->EnableWindow(FALSE);
	}
*/
	CButton* pScormOracle = (CButton*)GetDlgItem(IDC_RAD_ORACLE);
	pScormOracle->EnableWindow(TRUE);
	CButton* pScormOracleOld = (CButton*)GetDlgItem(IDC_RAD_ORACLEOLD);
	pScormOracleOld->EnableWindow(TRUE);

	if (!pScormDefault->GetCheck() && !pScormAenrich->GetCheck() && !pScormOracle->GetCheck() && !pScormOracleOld->GetCheck())
		pScormDefault->SetCheck(1);

	pScormFTP->EnableWindow(TRUE);
	pScormLocal->EnableWindow(TRUE);
	if (!pScormFTP->GetCheck() && !pScormLocal->GetCheck())
		pScormLocal->SetCheck(1);

	pCheckFTP->EnableWindow(FALSE);
	pCheckFTPManual->EnableWindow(FALSE);
	pCheckFile->EnableWindow(FALSE);
	m_pBtnOpenFTPPath->EnableButton(FALSE);	
	m_pBtnOpenFilePath->EnableButton(FALSE);	
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(TRUE);
	m_pBtnOpenScormPath->EnableButton(TRUE);


	if(pScormAenrich->GetCheck() && !pScormLocal->GetCheck())
	{
		GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(TRUE);
	}

	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	pProtocolMMS ->EnableWindow(TRUE);
	pProtocolRTSP ->EnableWindow(TRUE); 
	pProtocolHTTP ->EnableWindow(TRUE); 
	pProtocolHTTPNM ->EnableWindow(TRUE); 
	
	if (!pProtocolMMS->GetCheck() && !pProtocolRTSP->GetCheck() && !pProtocolHTTP->GetCheck() && !pProtocolHTTPNM->GetCheck())
		pProtocolMMS->SetCheck(1);

}

void CCourseLoaderDlg::OnRadioNoScorm() 
{
	CButton* pScormMediaZIP = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
	pScormMediaZIP->SetCheck(0);
	pScormMediaZIP->EnableWindow(FALSE);

	CButton* pScormFTP = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);
	CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);
	CButton* pCheckFTP = (CButton*)GetDlgItem(IDC_RADIO_FTP);
	CButton* pCheckFTPManual = (CButton*)GetDlgItem(IDC_RADIO_FTP_MANUAL);
	CButton* pCheckFile = (CButton*)GetDlgItem(IDC_RADIO_FTP_LOCAL);

	CButton* pScormDefault = (CButton*)GetDlgItem(IDC_RAD_DEFAULT);
	pScormDefault->EnableWindow(TRUE);
	CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	pScormAenrich->EnableWindow(TRUE);

	CButton* pScormEnhance = (CButton*)GetDlgItem(IDC_CHECK_ENHANCE);
	pScormEnhance->SetCheck(0);
	pScormEnhance->EnableWindow(FALSE);

	CButton* pScormOracle = (CButton*)GetDlgItem(IDC_RAD_ORACLE);
	pScormOracle->EnableWindow(FALSE);
	CButton* pScormOracleOld = (CButton*)GetDlgItem(IDC_RAD_ORACLEOLD);
	pScormOracleOld->EnableWindow(FALSE);

	//if (!pScormOracle->GetCheck() && !pScormAenrich->GetCheck() && !pScormDefault->GetCheck())
	if (!pScormDefault->GetCheck() && !pScormAenrich->GetCheck())
		pScormDefault->SetCheck(1);

	if (!pCheckFTP->GetCheck() && !pCheckFTPManual->GetCheck() && !pCheckFile->GetCheck())
		pCheckFTP->SetCheck(1);

	pScormFTP->EnableWindow(FALSE);
	pScormLocal->EnableWindow(FALSE);
	pCheckFTP->EnableWindow(TRUE);
	pCheckFTPManual->EnableWindow(TRUE);
	pCheckFile->EnableWindow(TRUE);
	m_pBtnOpenFTPPath->EnableButton(pCheckFTPManual->GetCheck());	
	m_pBtnOpenFilePath->EnableButton(pCheckFile->GetCheck());	
	GetDlgItem(IDC_EDIT_PATH_LOAD_LATER)->EnableWindow(pCheckFTPManual->GetCheck());
	GetDlgItem(IDC_EDIT_PATH_DIRECT)->EnableWindow(pCheckFile->GetCheck());
	GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(FALSE);
	m_pBtnOpenScormPath->EnableButton(FALSE);
	CButton* pProtocolMMS = (CButton*)GetDlgItem(IDC_RAD_MMS);
	CButton* pProtocolRTSP = (CButton*)GetDlgItem(IDC_RAD_RTSP);
	CButton* pProtocolHTTP = (CButton*)GetDlgItem(IDC_RAD_HTTP);
	CButton* pProtocolHTTPNM = (CButton*)GetDlgItem(IDC_RAD_HTTPNM);

	pProtocolMMS ->EnableWindow(TRUE);
	pProtocolRTSP ->EnableWindow(TRUE); 
	pProtocolHTTP ->EnableWindow(TRUE); 
	pProtocolHTTPNM ->EnableWindow(FALSE); 
	
	if (!pProtocolMMS->GetCheck() && !pProtocolRTSP->GetCheck() && !pProtocolHTTP->GetCheck())
	{
		pProtocolHTTPNM->SetCheck(0);
		pProtocolMMS->SetCheck(1);
	}

	if(pCheckFile->GetCheck())
	{
		pProtocolMMS ->EnableWindow(FALSE);
		pProtocolRTSP ->EnableWindow(FALSE); 
		pProtocolHTTP ->EnableWindow(FALSE); 
		pProtocolHTTPNM ->EnableWindow(FALSE); 
	}

}

void CCourseLoaderDlg::OnRadioScormFtp() 
{

	CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	pScormAenrich->EnableWindow(TRUE);

	CButton* pScormFtp = (CButton*)GetDlgItem(IDC_RADIO_SCORM_FTP);

	if(pScormFtp->GetCheck())
	{

		CButton* pScormMediaZIP = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
		pScormMediaZIP->SetCheck(0);
		pScormMediaZIP->EnableWindow(FALSE);


		if(pScormAenrich->GetCheck())
		{
			GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(TRUE);
	}
}

void CCourseLoaderDlg::OnRadioScormLocal() 
{

	CButton* pScormMediaZIP = (CButton*)GetDlgItem(IDC_MEDIA_IN_ZIP);
	pScormMediaZIP->EnableWindow(TRUE);


	CButton* pScormAenrich = (CButton*)GetDlgItem(IDC_RAD_AENRICH);
	pScormAenrich->EnableWindow(TRUE);

	CButton* pScormLocal = (CButton*)GetDlgItem(IDC_RADIO_SCORM_LOCAL);

	GetDlgItem(IDC_EDIT_PATH_SCORM)->EnableWindow(TRUE);
}

void CCourseLoaderDlg::OnRadioScormOracle() 
{
	CButton* pScormEnhance = (CButton*)GetDlgItem(IDC_CHECK_ENHANCE);
	pScormEnhance->SetCheck(0);
}

void CCourseLoaderDlg::OnRadioScormOracleOld() 
{
	CButton* pScormEnhance = (CButton*)GetDlgItem(IDC_CHECK_ENHANCE);
	pScormEnhance->SetCheck(0);
}
