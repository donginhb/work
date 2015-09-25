// eLearning.cpp : Defines the class behaviors for the application.
//
//  version:
//
//  (1) pp: Pre Professional
//  (2) pr: Professional
//  (3) ev: Evaluation
//  (4) bd: Bundle 
//  (5) pm: Promotional
//  (6) po: Post Promotional
//
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eLearning.h"

#include "MainFrm.h"
#include "eLearningDoc.h"
#include "eLearningView.h"
#include "cmkapi.h"
#include "wndLogo.h"
#include "l_bitmap.h"
#include "dlgAbout.h"
#include "encodeapi.h"
#include "registry.h"
#include "DlgVersionOutOfDate.h"
#include "WndTimeRecord.h"
#include "PPT9Ctrl.h"
#include "macro.h"
#include "CaptureDevApi.h"
#include "DlgSelDevice.h"
#include "Device.h"
#include "FileApi.h"
#include "..\ComCapture\ComCapture\ComCapture_i.h"

//add 2013/11/07 kagata BugNo.22
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winuser.h>

//add 2014/05/15 kagata outemon
#include <shlwapi.h>

//#ifdef _BUNDLE
//#include "LockServer.h"
//CLockServer g_LockServer;
//#endif

#include <string.h>
#include <direct.h>
#define constTrialDays 30
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELearningApp

BEGIN_MESSAGE_MAP(CELearningApp, CWinApp)
	//{{AFX_MSG_MAP(CELearningApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELearningApp construction

CELearningApp::CELearningApp()
{
	m_pWndTime = NULL;
	m_bFormal = FALSE;
	m_bReRun = FALSE;

	if (m_pszAppName != NULL)
		free((void*)m_pszAppName);

	CString strTitleName = "012345678901234567890123456789";
	m_pszAppName = _tcsdup(strTitleName);   
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CELearningApp object

CELearningApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CELearningApp initialization

BOOL CELearningApp::InitInstance()
{
	CString strTitleName;

#ifdef _KDI 
	strTitleName.LoadString(IDR_MAINFRAME_KDI);
#else
	strTitleName.LoadString(IDR_MAINFRAME);
#endif

	strcpy((char*)m_pszAppName, strTitleName);   

	BOOL bSingleInstance = CheckSingleInstance(IDR_MAINFRAME);	
	if (!bSingleInstance)
		return FALSE;

	CString str;
	int	nDepth = ::GetBitsPerPixel();
	if (nDepth < 16)
	{
		str.LoadString(IDS_RESOLUTION_NOT_ENOUGH);
		AfxMessageBox(str, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	CString strTmpPath = ::GetSysPath() + "InsTemp";
	if (::FileExists(strTmpPath))
		::DelTree(strTmpPath);

	CoInitialize(NULL);

	CPPT9Ctrl* pPPT = new CPPT9Ctrl;
	if (!pPPT->Create())
	{
		str.LoadString(IDS_PPT_NOT_INSTALL);
		AfxMessageBox(str, MB_ICONSTOP | MB_OK);
	}
	pPPT->Close();
	delete pPPT;

	m_nType = 0;

#ifdef _EVALUATION
	m_nType = 1;
#endif;

#ifdef _BUNDLE
	m_nType = 2;
#endif;

#ifdef _BACKEND
	m_nType = 3;
#endif;

#ifdef _PROMOTE
	m_nType = 4;
#endif

	m_bEv = FALSE;
	m_strRegKey = "";
	if (m_nType == 0) //WANGWANG20080629
	{
		//m_strRegKey = "HKEY_CLASSES_ROOT\\grbfile\\ID";	//leu
        //m_strRegKey = "HKEY_CLASSES_ROOT\\mgrbile5\\ID";	//leu, for mcg5.x
		//m_strRegKey = "HKEY_CURRENT_USER\\mgrbile5\\ID";
		//* WANGWANG20090629
		//#ifndef _VISTA

          m_strRegKey = "HKEY_CLASSES_ROOT\\mgrbile5\\ID";

		//#endif;
		//
		//m_strFormerRegPath="HKEY_CLASSES_ROOT\\mgrbile5\\ID";
    }
	else if (m_nType == 1)
	{
		//m_strRegKey = "HKEY_CLASSES_ROOT\\grbfile\\IDE";  //leu

        //m_strRegKey = "HKEY_CURRENT_USER\\mgrbile5\\IDE";  //leu, for mcg5.x


        //* WANGWANG20090629
		//#ifndef _VISTA
          m_strRegKey = "HKEY_CLASSES_ROOT\\mgrbile5\\ID";

		//#endif;
	//
		//m_strFormerRegPath="HKEY_CLASSES_ROOT\\mgrbile5\\IDE";

	}
	else if (m_nType == 2)
		//m_strRegKey = "HKEY_CLASSES_ROOT\\grbfile\\IDB";  //leu
         m_strRegKey = "HKEY_CLASSES_ROOT\\mgrbile5\\ID"; //leu, for mcg5.x
	else if (m_nType == 4)
		//m_strRegKey = "HKEY_CLASSES_ROOT\\grbfile\\IDP";  //leu
        m_strRegKey = "HKEY_CURRENT_USER\\mgrbile5\\IDP";  //leu, for mcg5.x

	if (m_nType != 3)
	{
		m_pWndTime = new CWndTimeRecord(m_strRegKey);
		//m_pWndTime->SetstrFormerRegPath(m_strFormerRegPath);
		if (!CheckVersion())
			return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
//if launched as OLE server
	//if (RunEmbedded() || RunAutomated())
		// Parse the command line to see 
	//{
		// Register all OLE server (factories) as running.  This enables the
		//  OLE libraries to create objects from other applications.
	//	COleTemplateServer::RegisterAll();
	//}
	//else
	//{
		// When a server application is launched stand-alone, it is a good idea
		//  to update the system registry in case it has been damaged.
	//	COleObjectFactory::UpdateRegistryAll();
	//}

	CString strLogoPath = ::GetSysPath() + "logo\\logo.gif";
	CWndLogo* pLogo = NULL;
	if (::FileExists(strLogoPath))
	{
		pLogo = new CWndLogo(strLogoPath);
		//pLogo->Create();
		//::Sleep(2000);
	}

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	m_pSysFont = new CFont;
	if (!m_pSysFont->CreateFont(-11,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_ROMAN,
#ifdef _JAPANESE
							"MS UI Gothic"))
#else
							"Arial"))
#endif
	{
		m_pSysFont = NULL;
		return -1;
	}

	m_pSysFont2 = new CFont;
	if (!m_pSysFont2->CreateFont(-12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_ROMAN,
#ifdef _JAPANESE
							"MS UI Gothic"))
#else
							"Arial"))
#endif
	{
		m_pSysFont2 = NULL;
		return -1;
	}

	m_pSysFont3 = new CFont;
	if (!m_pSysFont3->CreateFont(-12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_ROMAN,
#ifdef _JAPANESE
							"MS UI Gothic"))
#else
							"Times New Roman"))
#endif
	{
		m_pSysFont3 = NULL;
		return -1;
	}

	m_pSysFont4 = new CFont;
	if (!m_pSysFont4->CreateFont(-12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_DONTCARE,
#ifdef _JAPANESE
							"MS UI Gothic"))
#else
							"System"))
#endif
	{
		m_pSysFont4 = NULL;
		return -1;
	}

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CELearningDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CELearningView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

//#ifndef _LOG
//	if (pMenu)
//	{
//		int nCount = pMenu->GetMenuItemCount();
//		if (nCount == 7)
//			pMenu->DeleteMenu(nCount - 1, MF_BYPOSITION);
//	}
//#endif

#ifndef _RENT
	CMenu* pMenu = (CMenu*)m_pMainWnd->GetMenu();
	if (pMenu)
	{
		int nCount = pMenu->GetMenuItemCount();
		CMenu* pSubMenu = pMenu->GetSubMenu(nCount - 1);
		if (pSubMenu)
		{
			pSubMenu->DeleteMenu(2, MF_BYPOSITION);
			pSubMenu->DeleteMenu(ID_BUY_QUOTA, MF_BYCOMMAND);
			pSubMenu->DeleteMenu(ID_USE_QUOTA, MF_BYCOMMAND);
		}
	}
#endif

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
//	m_pMainWnd->ShowWindow(SW_HIDE);//wanwwang
	m_pMainWnd->UpdateWindow();

	if (pLogo)
	{
		//pLogo->ShowWindow(SW_HIDE);	//add 2013/11/07 kagata BugNo.22

		//pLogo->DestroyWindow();
		delete pLogo;
	}

	if (m_pWndTime)
	{
		if (!m_pWndTime->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), m_pMainWnd, 0))
			return FALSE;
		m_pWndTime->Activate();
	}

	CString strTitle, strTmp = "";
	strTmp = GetVerStr();

#ifdef _KDI
	strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
	strTitle.LoadString(IDR_MAINFRAME);
#endif

	m_pMainWnd->SetWindowText(strTitle + " " + strTmp);
	
	InitSetting();

	BOOL bAvail;
	if (DeviceAvailable(bAvail, TRUE))
	{
		if (bAvail)
		{
			CString strDevType = "";
			CString strParam1, strParam2;
			::GetDevice(strDevType, strParam1, strParam2);
			if (strDevType == "")
			{
				SHOW_MESSAGE(IDS_SET_DEVICE);

				CDevice dlg;
				dlg.m_ScreenCapture = FALSE;
				dlg.m_NewCourse = FALSE;
				dlg.DoModal();
			}
		}
	}

	//add 2013/11/07 kagata BugNo.22
	const char *argv[] = {"abcdefg", "mcgpro", "testchar"};
	const unsigned char *p = (const unsigned char *)(argv[0]);
	for(int v = 0; v < 4; v++)
	{
		keybd_event(VK_INSERT, 0, 0, 0);
		keybd_event(VK_INSERT, 0, KEYEVENTF_KEYUP, 0);
		Sleep(250);
	}
	//ここまで

	//add 2014/8/5 kagata outemon
	BeginWaitCursor();

	//add 2014/05/15 kagata outemon
	//MCGPro起動時接続確認
	CString strNasPath = GetNasDir();
	if (strNasPath == "")
	{
		m_pNasPath = "";
		m_pNetAccess = false;
	}
	else
	{
		m_pNasPath = strNasPath;
		m_pNetAccess = PathIsDirectory(m_pNasPath);
		if(!m_pNetAccess)
		{
			m_pNetAccess = NasDirRetry(m_pNasPath, 59491);
		}
	}

	//接続状態のフラグを設定ファイルに書き込み
	UpCompInfo(m_pNetAccess);

	//各種フラグ回収
	m_pRecUp = ChkNasUpRec();
	//m_pStUp = ChkNasUpSt();
	m_pEnUp = ChkNasUpEn();

	//起動時コース保存
	CString strCorPath = MACRO_COURSE_PATH;
	CStringArray strArray;
	::GetDir(strCorPath, strArray);

	//if(strArray.GetSize() != 0 && m_pNetAccess && m_pStUp)
	if(strArray.GetSize() != 0 && m_pNetAccess && m_pRecUp)
	{
		//BOOL stCopy;
		int ret;
		int nret;
		CString NasSt;
		NasSt.LoadString(59492);
		ret = AfxMessageBox(NasSt,MB_YESNO);
		if(ret == IDYES)
		{
			do
			{
				strArray.RemoveAll();
				::GetDir(strCorPath, strArray);
				nret = NasLocalCheck(m_pNasPath, strArray);
			}	while(nret == -1 || nret == -2);	//replase 2014/8/8 kagata outemon

			if(nret == 1)		//add 2014/8/5 kagata outemon
			{
				CString t;
				t.LoadString(59500);
				AfxMessageBox(t);
			}
		}
	}
	//ここまで/////////
	
	//add 2014/8/5 kagata outemon
	EndWaitCursor();

	return TRUE;
}

void CELearningApp::InitSetting()
{
	CString strIniPath = MACRO_INI_PATH;
	int nCount = ::GetPrivateProfileInt("General", "BackupDirCount", 0, strIniPath);
	if (nCount == 0)
	{
		::WritePrivateProfileString("General", "BackupDirCount", "1", strIniPath);
		CString strBuf, strTmp;
		strBuf.Format("Backup%d", 0);
		
		CString strDefault;
		strDefault.LoadString(IDS_DEFAULT);
		::WritePrivateProfileString(strBuf, "Name", strDefault, strIniPath);

		strTmp = MACRO_BACKUP_PATH;
		::WritePrivateProfileString(strBuf, "Dir", strTmp, strIniPath);
	}
	//add by buxiangyi
	char szBuf[512];
	::GetPrivateProfileString("CourseFolder", "Name", "", szBuf, 512, strIniPath);
	CString CourseFolderName = szBuf;
	if (CourseFolderName == "")
	{
		CString strDefault, strTmp;;
		strDefault.LoadString(IDS_DEFAULT);
		::WritePrivateProfileString("CourseFolder", "Name", strDefault, strIniPath);

		strTmp = MACRO_COURSE_PATH;
		::WritePrivateProfileString("CourseFolder", "Dir", strTmp, strIniPath);
	}

	/////////////////////////////////////////////////////
	CString strTmp;
//#ifndef _BUNDLE
	strTmp.LoadString(IDS_DEFAULT_PROFILE_NAME1);
	::GetPrivateProfileString("Profile", "Name", "@@@", szBuf, 512, strIniPath);
	if (strcmp(szBuf, "@@@") == 0)
	{
		::WritePrivateProfileString("Profile", "Name", strTmp, strIniPath);
		::WritePrivateProfileString("Profile", "Param1", "256", strIniPath);
		::WritePrivateProfileString("Profile", "Param2", "", strIniPath);
		::WritePrivateProfileString("Profile", "Param3", "1", strIniPath);
		::WritePrivateProfileString("Profile", "Param4", "64", strIniPath);
		::WritePrivateProfileString("Profile", "Param5", "", strIniPath);
		::WritePrivateProfileString("Profile", "Param6", "", strIniPath);
		//::WritePrivateProfileString("Profile", "Dimension", "240 x 180", strIniPath);
	}
//#else
//	strTmp = "<110K>";
//	char szBuf[512];
//	::GetPrivateProfileString("Profile", "Name", "@@@", szBuf, 512, strIniPath);
//	if (strcmp(szBuf, "@@@") == 0)
//	{
//		::WritePrivateProfileString("Profile", "Name", strTmp, strIniPath);
//		::WritePrivateProfileString("Profile", "Param1", "88", strIniPath);
//		::WritePrivateProfileString("Profile", "Param2", "20", strIniPath);
//		::WritePrivateProfileString("Profile", "Param3", "1", strIniPath);
//		::WritePrivateProfileString("Profile", "Param4", "0", strIniPath);
//		::WritePrivateProfileString("Profile", "Param5", "22", strIniPath);
//		::WritePrivateProfileString("Profile", "Param6", "22050", strIniPath);
//		::WritePrivateProfileString("Profile", "Param7", "S", strIniPath);
//		::WritePrivateProfileString("Profile", "Dimension", "200 x 150", strIniPath);
//	}
//#endif

	strTmp.LoadString(IDS_DEFAULT_SCREEN_NAME1);
	::GetPrivateProfileString("ScreenProfile", "Name", "@@@", szBuf, 512, strIniPath);
	if (strcmp(szBuf, "@@@") == 0)
	{
		::WritePrivateProfileString("ScreenProfile", "Name", strTmp, strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param1", "256", strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param2", "1", strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param3", "1", strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param4", "32", strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param5", "", strIniPath);
		::WritePrivateProfileString("ScreenProfile", "Param6", "", strIniPath);
		//::WritePrivateProfileString("ScreenProfile", "Codec", "Windows Media Audio V8", strIniPath);
	}

	::GetPrivateProfileString("General", "DefaultOutlook", "@@@", szBuf, 512, strIniPath);
	if (strcmp(szBuf, "@@@") == 0)
	{
		strTmp.LoadString(IDS_DEFAULT_OUTLOOK);
		::WritePrivateProfileString("General", "DefaultOutlook", strTmp, strIniPath);
	}

	CStringArray m_oldarray, array;
	CString m_time, strPath;
	m_oldarray.Add("var vsizex =" );
	m_oldarray.Add("var vsizey =" );

	array.Add("var vsizex = 320;");
	array.Add("var vsizey = 240;");
	strPath = strPath = MACRO_TEMPLATE_PATH +  "js\\";
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);
}

CString CELearningApp::GetVerStr()
{
	CString strTmp = "";
	if (m_nType == 1)
		strTmp.LoadString(IDS_EVAL_VERSION);
	else if (m_nType == 2)
		strTmp.LoadString(IDS_BUNDLE_VERSION);
	else if (m_nType == 0 && m_bEv)
		strTmp.LoadString(IDS_PP_VERSION);
	else if (m_nType == 0 && !m_bEv)
		strTmp.LoadString(IDS_PRO_VERSION);
	else if (m_nType == 3)
		strTmp.LoadString(IDS_BACKEND_VERSION);
	else if (m_nType == 4)
		strTmp.LoadString(IDS_PROMOTE_VERSION);

	return strTmp;
}

BOOL CELearningApp::CheckVersion()
{
	CString str;
	if (m_pWndTime->KeyExist())
	{
		BOOL bCheckKey = CheckKey();
	
		CString strType;
		long nTimeLeft;
		m_pWndTime->GetKeyInfo(strType, nTimeLeft);
//		if ((strType == "ev" && m_nType != 1) ||
//			(strType != "ev" && m_nType == 1) ||
//			(strType == "bd" && m_nType != 2) ||
//			(strType != "bd" && m_nType == 2))
//		{
//			str.LoadString(IDS_INCOMPLETE_INSTALL);
//			AfxMessageBox(str, MB_ICONSTOP | MB_OK);
//			return FALSE;
//		}


		if ((strType == "pr" || strType == "po" || strType == "bf") && bCheckKey)
			m_bFormal = TRUE;
		else if ((strType == "pr" || strType == "po" || strType == "bf") && !bCheckKey)  //format edition and key check error
		{
			//if (m_nType == 0)
			//{
				//Auto Recovery
			//	BOOL bRet = TRUE;
				//CString strKey;
				//if (!GetMachineID(strKey))
				//	bRet = TRUE;

				//char szBuf[256];
				//KeyEncode(strKey, szBuf);
				//strKey = szBuf;

				//if (!bRet)
				//	if (!WriteKey(strKey))
				//		bRet = TRUE;

				//if (!bRet)
				//	if (!CheckKey())
				//		bRet = TRUE;

				//if (bRet)
				//{
				//	str.LoadString(IDS_NO_KEY);
				//	AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				//	return FALSE;
				//}
			//}
			//else
			//{
				str.LoadString(IDS_NO_KEY);
				AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				return FALSE;
			//}
		}
		else if ((strType == "pp" || strType == "ev" || strType == "bd" || strType == "pm") && 
				 !bCheckKey && nTimeLeft == 0) //eval edition and out-of-date
		{
			if (m_nType == 0 || m_nType == 2 || m_nType == 4)
			{
				CDlgVersionOutOfDate dlg;
				if (dlg.DoModal() != IDOK)
					return FALSE;
			}
			else if (m_nType == 1)
			{
				str.LoadString(IDS_OUT_OF_DATE);
				AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				return FALSE;
			}
		}
		else if ((strType == "pp" || strType == "pm" || strType == "bd") && bCheckKey) //eval edition and pass key check
		{
			if (m_nType == 0 || m_nType == 4 || m_nType == 2)
			{
				m_pWndTime->PromoteToFormal();
				m_bFormal = TRUE;
			}
		}
		else if ((strType == "pp" && !bCheckKey && nTimeLeft > 0) ||
				 (strType == "pm" && !bCheckKey && nTimeLeft > 0) ||
				 (strType == "ev" && nTimeLeft > 0) ||
				 (strType == "bd" && nTimeLeft > 0)) 
		{                                           //eval edition and in-date
			m_bEv = TRUE;
			CString strMsg, strTmp, strVer;

			strTmp.LoadString(IDS_EVAL_TIME_LEFT);
			CString strTime = ::ConvertSecToMinStr(nTimeLeft);
			strMsg.Format(strTmp, strTime);

			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
		}

		CString strSysPath = ::GetSysPath();
		char szDisk[32];
		_splitpath(strSysPath, szDisk, NULL, NULL, NULL);
		CString strDisk = szDisk + CString("\\");
		LONGLONG llSpace = ::GetVolFreeSpace(szDisk);
		CString strMsg;
		strMsg.LoadString(IDS_DISK_FREE_LOW);
		if (llSpace < (LONGLONG)1024 * 1024 * 100)
			AfxMessageBox(strMsg, MB_ICONSTOP | MB_OK);
	}
	else
	{

		str.LoadString(IDS_KEY_CHECK_OUT);

		AfxMessageBox(str, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CELearningApp::DeviceAvailable(BOOL& bAvailable, BOOL bSilent)
{
	IEasyCapture* pEasyCapture;

	HRESULT hval = 0;
	hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&pEasyCapture);
	if (hval != S_OK) 
	{  
		if (!bSilent)
			AfxMessageBox("Error Open: EasyCapture Fail");
		return FALSE;
	}

	hval = pEasyCapture->Open(0);
	if((hval != S_OK)) 
	{
		if (!bSilent)
			AfxMessageBox("Error Open: EasyCapture Open Fail");
		return FALSE;
	}

	CStringArray strArrayWebAudio, strArrayWebVideo, strArrayDV;
	CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
	::EnumDevice(pEasyCapture, eType, strArrayWebAudio);

	eType = CAPTUREDEVICETYPE_VIDEO;
	::EnumDevice(pEasyCapture, eType, strArrayWebVideo);

	pEasyCapture->Close();
	pEasyCapture->Release();

	int nVideoSize = strArrayWebVideo.GetSize() + strArrayDV.GetSize();
	int nAudioSize = strArrayWebAudio.GetSize();

	bAvailable = (nVideoSize > 0 && nAudioSize > 0);

	return TRUE;
}

void CELearningApp::CommonFuncErrMsg(CString strFunc, CWnd* pWnd)
{
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_COMMON_FUNC_ERR);
	strMsg.Format(strTmp, strFunc);
	W_SHOW_ERROR_STR(strMsg, pWnd);
}

BOOL CELearningApp::CheckVersion2()     //for protection
{
	if (m_nType == 3)
		return TRUE;

	CString str1 = "";
	CString str2 = "";
	CString str;

	CWndTimeRecord WndTime(m_strRegKey);
	if (WndTime.KeyExist())
	{
		BOOL bCheckKey = CheckKey();
	
		CString strType;
		long nTimeLeft;
		WndTime.GetKeyInfo(strType, nTimeLeft);
//		if ((strType == "ev" && m_nType != 1) ||
//			(strType != "ev" && m_nType == 1) ||
//			(strType == "bd" && m_nType != 2) ||
//			(strType != "bd" && m_nType == 2))
//		{
			//str.LoadString(IDS_INCOMPLETE_INSTALL);
			//AfxMessageBox(str, MB_ICONSTOP | MB_OK);
//			return FALSE;
//		}

		if ((strType == "pr" || strType == "po" || strType == "bf") && bCheckKey)
			m_bFormal = TRUE;
		else if ((strType == "pr" || strType == "po" || strType == "bf") && !bCheckKey)  //format edition and key check error
		{
			//if (m_nType == 0)
			//{
				//Auto Recovery
			//	BOOL bRet = TRUE;
				//CString strKey;
				//if (!GetMachineID(strKey))
				//	bRet = TRUE;

				//char szBuf[256];
				//KeyEncode(strKey, szBuf);
				//strKey = szBuf;

				//if (!bRet)
				//	if (!WriteKey(strKey))
				//		bRet = TRUE;

				//if (!bRet)
				//	if (!CheckKey())
				//		bRet = TRUE;

				//if (bRet)
				//{
				//	str.LoadString(IDS_NO_KEY);
				//	AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				//	return FALSE;
				//}
			//}
			//else
			//{
				//str.LoadString(IDS_NO_KEY);
				//AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				return FALSE;
			//}
		}
		else if ((strType == "pp" || strType == "ev" || strType == "bd" || strType == "pm") && 
				 !bCheckKey && nTimeLeft == 0) //eval edition and out-of-date
		{
			if (m_nType == 0 || m_nType == 2 || m_nType == 4)
			{
				//CDlgVersionOutOfDate dlg;
				//if (dlg.DoModal() != IDOK)
					return FALSE;
			}
			else if (m_nType == 1)
			{
				//str.LoadString(IDS_OUT_OF_DATE);
				//AfxMessageBox(str, MB_ICONSTOP | MB_OK);
				return FALSE;
			}
		}
		else if ((strType == "pp" || strType == "pm" || strType == "bd") && bCheckKey) //eval edition and pass key check
		{
			//if (m_nType == 0)
			//{
			//	m_pWndTime->PromoteToFormal();
			//	m_bFormal = TRUE;
			//}
		}
		else if ((strType == "pp" && !bCheckKey && nTimeLeft > 0) ||
				 (strType == "pm" && !bCheckKey && nTimeLeft > 0) ||
				 (strType == "ev" && nTimeLeft > 0) ||
				 (strType == "bd" && nTimeLeft > 0)) 
		{                                           //eval edition and in-date
			//CString strMsg, strTmp;
			//strTmp.LoadString(IDS_TIME_LEFT);
			//CString strTime = ::ConvertSecToMinStr(nTimeLeft);
			//strMsg.Format(strTmp, strTime);
			//AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
			//m_bEv = TRUE;
		}
	}
	else
	{
		//str.LoadString(IDS_INCOMPLETE_INSTALL);
		//AfxMessageBox(str, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CELearningApp::CommonErrMsg(CString strFunc, CWnd* pWnd)
{
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_COMMON_ERR);
	strMsg.Format(strTmp, strFunc);
	W_SHOW_ERROR_STR(strMsg, pWnd);
}


void CELearningApp::ShowMessage(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);

}

void CELearningApp::ShowWarning(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);
}

void CELearningApp::ShowError(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);
}

int CELearningApp::AskYesNo(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet =	pWnd->MessageBox(strMsg, AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

	return nRet;
}

int CELearningApp::AskYesNoCancel(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet =	pWnd->MessageBox(strMsg, AfxGetAppName(), MB_ICONQUESTION | MB_YESNOCANCEL);

	return nRet;
}

int CELearningApp::AskOKCancel(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;

	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

	return nRet;

}

void CELearningApp::ShowMessageStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);
}

void CELearningApp::ShowWarningStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);
}

void CELearningApp::ShowErrorStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);
}

int CELearningApp::AskYesNoStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

	return nRet;
}

int CELearningApp::AskYesNoCancelStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_YESNOCANCEL);

	return nRet;
}

int CELearningApp::AskOKCancelStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	int	nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

	return nRet;
}

// App command to run the dialog
void CELearningApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.m_strFileName = ::GetSysPath() + "about.bmp";
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CELearningApp message handlers

BOOL CELearningApp::CheckKey() 
{
	CString strKey;
	if (!GetKey(strKey))
		return FALSE;

	return CheckKey(strKey);
}

BOOL CELearningApp::WriteKey(CString strKey) 
{
	CFile cf;
	//CString strPath = ::GetSysPath() + "bin\\CriticalRegion.bin";  //leu
    CString strPath = ::GetSysPath() + "bin\\MCriticalion.slc";  //leu, for mcg5.x

	//shien add 20050323 get windows install path
	CString strPathWin = "";
	RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SystemRoot", strPathWin);

	#ifdef _PRO
		//strPathWin = strPathWin + "\\system32\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system32\\MCriticalion.slc";  //leu, for mcg5.x
	#else
		//strPathWin = strPathWin + "\\system\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system\\MCriticalion.slc";  //leu, for mcg5.x
	#endif;

	//if (!cf.Open(strPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	//	return FALSE;

	if (!cf.Open(strPathWin, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		return FALSE;
	}
	/*else
	{
		if (!cf.Open(strPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			return FALSE;
		}
	}
	*/
	strPath = strPathWin;

	int nBound = 1024 * 28;
	char* psz = new char[nBound];
	psz[57] = strKey.GetLength() / 2;
	int nIndex;
	int i = 0;
	for (i = 0; i < strKey.GetLength(); i += 2)
	{
		if (i + 2 <= strKey.GetLength())
		{
			CString str = strKey.Mid(i, 2);
			char sz = (char)(GetHexValue(str[0]) * 16 + GetHexValue(str[1]));
			nIndex = i / 2 + 1;
			psz[57 + nIndex] = sz;
		}
	}

	for (i = 0; i < 57; i++)
		psz[i] = rand() * 255 / RAND_MAX;

	for (i = nIndex + 58; i < nBound; i++)
		psz[i] = rand() * 255 / RAND_MAX;

	cf.Write(psz, nBound);
	cf.Close();

	delete[]psz;

	return TRUE;
}

int CELearningApp::GetHexValue(char sz)
{
	if (sz >= '0' && sz <= '9')
		return sz - '0';
	else if (sz >= 'a' && sz <= 'f')
		return sz - 'a' + 10;
	else if (sz >= 'A' && sz <= 'F')
		return sz - 'A' + 10;
	return 0;
}

BOOL CELearningApp::GetKey(CString& strKey) 
{
	CFile cf;
	//CString strPath = ::GetSysPath() + "bin\\CriticalRegion.bin";  //leu
    CString strPath = ::GetSysPath() + "bin\\MCriticalion.slc";  //leu, for mcg5.x

	//shien add 20050323 get windows install path
	CString strPathWin = "";
	RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SystemRoot", strPathWin);

	#ifdef _PRO
		//strPathWin = strPathWin + "\\system32\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system32\\MCriticalion.slc";  //leu, for mcg5.x
	#else
		//strPathWin = strPathWin + "\\system\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system\\MCriticalion.slc";  //leu, for mcg5.x
	#endif;

	//if (!::FileExists(strPath))
	//{
	//	strPath = ::GetSysPath() + "CriticalRegion.bin";
	//	if (!::FileExists(strPath))
	//		return FALSE;
	//}

	if (!::FileExists(strPath))
	{
		//strPath = ::GetSysPath() + "CriticalRegion.bin";
		//if (!::FileExists(strPath))
		//{
			strPath = strPathWin;
			if (!::FileExists(strPath))
			{
				return FALSE;
			}
		//}
	}
	
	if (!cf.Open(strPath, CFile::modeRead))
		return FALSE;

	char szBuf[256];
	char szKey[256];
	szKey[0] = '\0';
	UINT nBytesRead = cf.Read(szBuf, 255);
	if (nBytesRead > 0)
	{
		unsigned char chCount = szBuf[57];
		if (chCount > 0 && nBytesRead >= UINT(chCount + 1))
		{
			for (UINT i = 1; i <= chCount; i++)
			{
				char ch = szBuf[i + 57];
				char szch[3];
				sprintf(szch, "%02x", ch);
				szch[2] = '\0';
				strcat(szKey, szch);
			}
		}
	}
	cf.Close();

	strKey = szKey;

	return TRUE;
}

void CELearningApp::RemoveKey() 
{
	//CString strPath = ::GetSysPath() + "bin\\CriticalRegion.bin";  //leu
    CString strPath = ::GetSysPath() + "bin\\MCriticalion.slc";  //leu, for mcg5.x
	::DeleteFile(strPath);

	//shien add 20050323 get windows install path
	CString strPathWin = "";
	RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SystemRoot", strPathWin);

	#ifdef _PRO
		//strPathWin = strPathWin + "\\system32\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system32\\MCriticalion.slc";  //leu, for mcg5.x
	#else
		//strPathWin = strPathWin + "\\system\\CriticalRegion.bin";  //leu
        strPathWin = strPathWin + "\\system\\MCriticalion.slc";  //leu, for mcg5.x
	#endif;

	::DeleteFile(strPathWin);

	if (m_strRegKey != "")
		::RegSetKey(m_strRegKey, "xxxxxxxxxxxxxxxx");
}

BOOL CELearningApp::CheckKey(CString strKey) 
{
	CString strSN = "";

	char szKey[128];
	::KeyDecode(strKey, szKey);
	strSN = szKey;

	CString strID = "";
	if (!GetMachineID(strID))
		return FALSE;

	if (strSN != strID)
		return FALSE;

	return TRUE;
}

void CELearningApp::EncodeID(CString& strID) 
{
	int i = 0;
	for (i = 0; i < strID.GetLength() - 1; i += 2)
	{
		char ch = strID[i];
		strID.SetAt(i, strID[i + 1]);
		strID.SetAt(i + 1, ch);
	}

	for (i = 0; i < strID.GetLength(); i++)
	{
		char ch = strID[i];
		char n = ch - '0' + (i % 4) + 'a';
		strID.SetAt(i, n);
	}
}

BOOL CELearningApp::GetMachineID(CString &strID)
{
	DWORD dw = 0;
	char szID[128];
	CString strPath = ::GetSysPath();
	char szDrive[128];
	_splitpath(strPath, szDrive, NULL, NULL, NULL);
	GetVolumeInformation(szDrive + CString("\\"), NULL, 0, &dw, NULL, NULL, NULL, 0);
	sprintf(szID, "%u", dw);
	strID = szID;

	return TRUE;
}

int CELearningApp::ExitInstance() 
{
	CoUninitialize();

	if (m_pSysFont != NULL)
	{
		m_pSysFont->DeleteObject();
		delete m_pSysFont;
	}
	
	if (m_pSysFont2 != NULL)
	{
		m_pSysFont2->DeleteObject();
		delete m_pSysFont2;
	}

	if (m_pSysFont3 != NULL)
	{
		m_pSysFont3->DeleteObject();
		delete m_pSysFont3;
	}

	if (m_pSysFont4 != NULL)
	{
		m_pSysFont4->DeleteObject();
		delete m_pSysFont4;
	}

	if (m_pWndTime)
	{
		if (::IsWindow(m_pWndTime->m_hWnd))
			m_pWndTime->DestroyWindow();
		delete m_pWndTime;
	}

	//if (m_bReRun)
	//{
	//	char* pszBuf = new char[2048];
	//	::GetModuleFileName(AfxGetInstanceHandle(), pszBuf, 2048);
	//	::WinExec(pszBuf, SW_SHOWMAXIMIZED);
	//	delete [] pszBuf;
	//}

	return CWinApp::ExitInstance();
}

void CELearningApp::StartLog()
{
}

void CELearningApp::EndLog()
{
}

BOOL CELearningApp::AddHour(int nHour) 
{
	long nTimeLeft = m_pWndTime->GetTimeLeft();
	long nTotalTime = nTimeLeft + nHour * 3600;
	m_pWndTime->SetTimeLeft(nTotalTime);
	m_pWndTime->WriteTimeLeft();
	m_pWndTime->TimerAction();

	return TRUE;
}

void CELearningApp::OperationLog(CString strOperation, CString strValue)
{
}

#define constZoomMark "!@#$"

BOOL WINAPI IsZoom(CString str)
{
	return (atoi(str) > 10000);
}

BOOL WINAPI IsCtrlMark(CString str) 
{ 
	return str.Left(4) == constZoomMark;
}

CString WINAPI MakeCtrlMark(CString str)
{ 
	return constZoomMark + str;
}

CString WINAPI MakeDemoMark(CString str)
{ 
	return constZoomMark + CString("#") + str;
}

//コースデータのローカルチェック
int CELearningApp::NasLocalCheck(CString NasPath, CStringArray& CourseArray)
{
	int nRet = 1;
	for (int y = 0; y < CourseArray.GetSize(); y++)
	{
		CString nasTempDir;
		CString nasTempcourse;
		nasTempDir = NasPath + "\\TEMP";
		nasTempcourse = NasPath + "\\TEMP\\" + CourseArray.GetAt(y);

		//Course.iniの確認
		//ファイルがないコースは削除する
		nRet = CourseiniChk(MACRO_COURSE_PATH + CourseArray.GetAt(y));

		if(nRet == -1)
			return nRet;

		//同名コースのあるなし判断
		//第1引数=保存先のフルパス、第2引数=コース名
		nRet = NasNameCheck(NasPath, CourseArray.GetAt(y));

		//replase 2014/8/8 kagata outemon
		if(nRet == -1 || nRet == -2)
			return nRet;

		//add 2014/8/8 kagata outemon
		BeginWaitCursor();

		//ローカルからTEMPフォルダへ
		//第3引数が1=元ファイル削除、0=元ファイル残す
		//NasCourse(nasTempDir, MACRO_COURSE_PATH + CourseArray.GetAt(y), 0);

		//TEMPから本フォルダへ
		//第3引数が1=元ファイル削除、0=元ファイル残す
		//int d = NasCourse(NasPath, nasTempDir + "\\" + CourseArray.GetAt(y), 1);
		int d = NasCourse(NasPath, MACRO_COURSE_PATH + CourseArray.GetAt(y), 0);
		
		d = Chkreload(NasPath + "\\" + CourseArray.GetAt(y), MACRO_COURSE_PATH + CourseArray.GetAt(y));
		
		if(d >= 0){
			//add 2014/7/31 kagata outemon
			HWND cie = FindWindow("IEFrame", NULL);
			if(cie)		//add 2014/8/1 kagata outemon
			{
				CString iemsg;
				iemsg.LoadString(59503);
				AfxMessageBox(iemsg, MB_OK);
			}

			//add 2014/7/30 kagata outemon chiba
			MACRO_MAINFRM->LoadCourse("");
			BeginWaitCursor();
			Sleep(1000);
			MACRO_MAINFRM->ConImeCut();
			Sleep(1000);

			//コピー成功時ローカルフォルダの削除
			::DelTree(MACRO_COURSE_PATH + CourseArray.GetAt(y));
		}else{
			//コピー失敗時再試行ダイアログの表示
			CString Renas;
			Renas.LoadString(59499);
			int ret;

			//再試行ダイアログ
			ret = AfxMessageBox(Renas,MB_RETRYCANCEL);
			if(ret == IDRETRY){
				//値を返して再実行
				return d;
			}else{
				//add 2014/8/5 kagata outemon
				nRet = 2;
				//値を返して即終了
				MACRO_MAINFRM->CollectCourse("");
				MACRO_MAINFRM->LoadCourse("");
				return nRet;
			}
		}
	}
	
	MACRO_MAINFRM->CollectCourse("");
	MACRO_MAINFRM->LoadCourse("");
	return nRet;
}

//NASへのコピー
//第1引数=コースコピー先フォルダパス　第2引数=コピー元のコースパス
//第3引数が1=元ファイル削除、0=元ファイル残す
int CELearningApp::NasCourse(CString strPath, CString strCourse, BOOL bDel)
{
	CString strSrc = strCourse;

	if (strPath.Right(1) != "\\")
		strPath += "\\";

	CString strCor = strCourse.Right(strCourse.GetLength()-strCourse.ReverseFind(_T('\\'))-1);

	int nRet = 1;
	CString strBkup = strPath + strCor;
	BOOL m = TryToMkDir(strPath);
	if(!m){
		nRet = -1;
		return nRet;
	}

	BOOL bDelDest = FALSE;
	if (::FileExists(strBkup))
	{
		//保存先にコースがある場合
		::DelTree(strBkup);
	}
	
	BeginWaitCursor();
	CString strTitle;
	strTitle.LoadString(59497);
	CString strMsg;
	strMsg.LoadString(59494);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	if (bDelDest)
		::DelTree(strBkup);

	if (!::CopyTree(strSrc, strBkup, NULL)){
		nRet = -1;
		return nRet;
	}


	if (bDel && nRet >= 0)
		::DelTree(strSrc);

	MACRO_MAINFRM->HideProgress();
	EndWaitCursor();

	return nRet;
}

//add 2014/6/13 kagata outemon
//各種アップロードフラグの回収
int CELearningApp::ChkNasUpRec()
{
	CString strAutoIniPath = MACRO_AUTOINI_FILE;
	char szBuf[255];
	::GetPrivateProfileString("UPSET", "RecUpload", "", szBuf, 255, strAutoIniPath);
	CString y(szBuf[0]);
	if(szBuf[0] == '\0' || y == "0")
	{
		//収録後の自動保存フラグ
		return 0;
	}
	else
	{
		return 1;
	}
}
int CELearningApp::ChkNasUpSt()
{
	CString strAutoIniPath = MACRO_AUTOINI_FILE;
	char szBuf[255];
	::GetPrivateProfileString("UPSET", "StartUpload", "", szBuf, 255, strAutoIniPath);
	CString y(szBuf[0]);
	if(szBuf[0] == '\0' || y == "0")
	{
		//MCGPro起動時のコース保存フラグ
		return 0;
	}
	else
	{
		return 1;
	}
}
int CELearningApp::ChkNasUpEn()
{
	CString strAutoIniPath = MACRO_AUTOINI_FILE;
	char szBuf[255];
	::GetPrivateProfileString("UPSET", "EndUpload", "", szBuf, 255, strAutoIniPath);
	CString y(szBuf[0]);
	if(szBuf[0] == '\0' || y == "0")
	{
		//MCGPro終了時のコース保存フラグ
		return 0;
	}
	else
	{
		return 1;
	}
}

//NASへのフォルダパス回収
CString CELearningApp::GetNasDir()
{
	CString strAutoIniPath = MACRO_AUTOINI_FILE;
	char szBuf[256];
	::GetPrivateProfileString("UPDATE", "FilePath", "", szBuf, 255, strAutoIniPath);
	CString nasDir = szBuf;

	return nasDir;
}

//接続再試行確認
bool CELearningApp::NasDirRetry(CString m_pNasPath, UINT nID)
{
	CString Renas;
	Renas.LoadString(nID);
	int ret;
	int m_NetAccess;

	do	//接続再試行ループ
	{
		ret = AfxMessageBox(Renas,MB_RETRYCANCEL);
		m_NetAccess = PathIsDirectory(m_pNasPath);
	}	while(ret == IDRETRY && !m_NetAccess);

	if(m_NetAccess != 0)
	{
		AfxMessageBox("保存先との接続が確認できました。");
		return true;
	}
	return false;
}

//接続確認フラグの挿入
void CELearningApp::UpCompInfo(int m_NetAccess)
{
	CString strElIniPath = MACRO_INI_PATH;
	if(m_NetAccess)
	{
		::WritePrivateProfileString("UPSTATUS", "UpComp", "1", strElIniPath);
	}
	else
	{
		::WritePrivateProfileString("UPSTATUS", "UpComp", "0", strElIniPath);
	}
}

//接続確認フラグの回収
int CELearningApp::GetUpCompInfo()
{
	CString strElIniPath = MACRO_INI_PATH;
	char szBuf[24];
	::GetPrivateProfileString("UPSTATUS", "UpComp", "", szBuf, 24, strElIniPath);
	CString y(szBuf[0]);
	if(szBuf[0] == '\0' || y == "0")
	{
		//MCGPro終了時のコース保存フラグ
		return 0;
	}
	else
	{
		return 1;
	}
}

//コース名照合
//第1引数：保存先のフルパス
//第2引数：コース名
int CELearningApp::NasNameCheck(CString NasPath, CString CourseName)
{
	int nRet = 1;
	if(PathIsDirectory(NasPath + "\\" + CourseName))
	{
		int result;
		CString NasSaveAs;
		NasSaveAs.LoadString(59493);
		NasSaveAs.Format(59493, CourseName);
		result = AfxMessageBox(NasSaveAs,MB_YESNO);
		if(result == IDNO)
		{
			//別名保存を行う
			nRet = -1;
			//リネームして再度コースデータ読み込み
			CELearningView* m_pView = NULL;
			int u = m_pView->RenameCourse(CourseName, 1);

			if(u == 1){
				AfxMessageBox("コンテンツ名の変更に成功しました。");
			}else if(u == 2){
				AfxMessageBox("コンテンツ名の変更に失敗しました。");
			}else if(u == 3){		//add 2014/8/7 kagata outemon
				nRet = -2;
			}

			return nRet;
		}
		else if(result == IDYES)
		{
			//上書き保存を行う
			//処理なし
		}
	}
	return nRet;
}

//Course.iniがない場合は削除する処理
int CELearningApp::CourseiniChk(CString CourseName)
{
	int inRet = 1;
	CString intPath = CourseName + "\\Course.ini";
	if (!::FileExists(intPath)){
		//iniファイルがなかった時の処理
		inRet = -1;

		//コースフォルダを削除
		::DelTree(CourseName);

		return inRet;
	}else{
		//iniファイルがあった時の処理
		return inRet;
	}
}

//ローカルとサーバの相互ファイルチェック
int CELearningApp::Chkreload(CString NasPath, CString CourseName)
{
	CStringArray ChkSaverArray;
	ChkSaverArray.RemoveAll();
	CStringArray ChkLocalArray;
	ChkLocalArray.RemoveAll();
	int r = DirAndFileChk(NasPath, CourseName, ChkSaverArray, ChkLocalArray);
	return r;
}
int CELearningApp::DirAndFileChk(CString NasPath, CString CourseName, CStringArray& ChkSaverArray, CStringArray& ChkLocalArray)
{
	int r = 1;

	::GetFiles(NasPath, "*", ChkSaverArray);
	::GetFiles(CourseName, "*", ChkLocalArray);

	r = Chkfilecommon(ChkSaverArray, ChkLocalArray);
	if(r <= 0)
		return -1;

	::GetDir(NasPath, ChkSaverArray);
	::GetDir(CourseName, ChkLocalArray);

	if(ChkSaverArray.GetSize() != ChkLocalArray.GetSize())
		return -1;

	for (int q = 0; q < ChkLocalArray.GetSize(); q++)
	{
		if(ChkSaverArray.GetAt(q) != ChkLocalArray.GetAt(q))
			return -1;

		r = Chkdircommon(NasPath + "\\"+ ChkSaverArray.GetAt(q), CourseName + "\\"+ ChkLocalArray.GetAt(q));
		if(r <= 0)
			return -1;
	}
	return 1;
}
int CELearningApp::Chkdircommon(CString ChkSavdir, CString ChkLocdir)
{
	int r = 1;

	CStringArray ChkSavdArray;
	ChkSavdArray.RemoveAll();
	CStringArray ChkLocdArray;
	ChkLocdArray.RemoveAll();

	::GetFiles(ChkSavdir, "*", ChkSavdArray);
	::GetFiles(ChkLocdir, "*", ChkLocdArray);

	r = Chkfilecommon(ChkSavdArray, ChkLocdArray);
	if(r <= 0)
		return -1;

	::GetDir(ChkSavdir, ChkSavdArray);
	::GetDir(ChkLocdir, ChkLocdArray);

	if(ChkSavdArray.GetSize() != ChkLocdArray.GetSize())
		return -1;

	for (int q = 0; q < ChkLocdArray.GetSize(); q++)
	{
		if(ChkSavdArray.GetAt(q) != ChkLocdArray.GetAt(q))
			return -1;
		r = Chkdircommon2(ChkSavdir + "\\"+ ChkSavdArray.GetAt(q), ChkLocdir + "\\"+ ChkLocdArray.GetAt(q));
		if(r <= 0)
			return -1;
	}
	return 1;
}
int CELearningApp::Chkfilecommon(CStringArray& ChkSavfArray, CStringArray& ChkLocfArray)
{
	if(ChkSavfArray.GetSize() != ChkLocfArray.GetSize())
		return -1;

	for (int y = 0; y < ChkLocfArray.GetSize(); y++)
	{
		if(ChkSavfArray.GetAt(y) != ChkLocfArray.GetAt(y))
			return -1;
	}

	return 1;
}
int CELearningApp::Chkdircommon2(CString ChkSav2dir, CString ChkLoc2dir)
{
	int r = 1;

	CStringArray ChkSavd2Array;
	ChkSavd2Array.RemoveAll();
	CStringArray ChkLocd2Array;
	ChkLocd2Array.RemoveAll();

	::GetFiles(ChkSav2dir, "*", ChkSavd2Array);
	::GetFiles(ChkLoc2dir, "*", ChkLocd2Array);

	r = Chkfilecommon(ChkSavd2Array, ChkLocd2Array);
	if(r <= 0)
		return -1;

	return 1;
}
