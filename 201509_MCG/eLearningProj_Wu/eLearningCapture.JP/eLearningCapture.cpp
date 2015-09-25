// eLearningCapture.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "eLearningCapture.h"
#include "CtrlPanelWnd.h"
#include "cmkapi.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELearningCaptureApp

BEGIN_MESSAGE_MAP(CELearningCaptureApp, CWinApp)
	//{{AFX_MSG_MAP(CELearningCaptureApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELearningCaptureApp construction

CELearningCaptureApp::CELearningCaptureApp()
{
	m_bMessageBox = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CELearningCaptureApp object

CELearningCaptureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CELearningCaptureApp initialization

BOOL CELearningCaptureApp::InitInstance()
{
	//AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	//CELearningCaptureDlg dlg;
	//int nResponse = dlg.DoModal();

	BOOL bSingleInstance = CheckSingleInstance(IDR_MAINFRAME);	
	if (!bSingleInstance)
	{
		return FALSE;
	}

	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "elearning.ini";
	::GetPrivateProfileString("Option", "Priority", "0", szBuf, 255, strIniPath);
	if (atoi(szBuf))
	{
		HANDLE hProcess = ::GetCurrentProcess();
		SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS); 
	}

	CCtrlPanelWnd* pMainWnd = new CCtrlPanelWnd;
	m_pMainWnd = pMainWnd;

	CString strPath;
	CString strCmd = m_lpCmdLine;
	int nIndex = strCmd.Find("AuFixValueC=");
	if (nIndex > 0)
	{
		CString strTmp;
		int nTmpIndex;
		strTmp = strCmd.Right(strCmd.GetLength() - nIndex);
		nTmpIndex = strTmp.Find(";");
		strTmp = strTmp.Left(nTmpIndex);
		strCmd.Delete(nIndex, nTmpIndex + 1);
		strTmp = strTmp.Right(strTmp.GetLength() - 12);
		m_nAuFixValueC = atoi(strTmp);
	}
	else
	{
		m_nAuFixValueC = 0;
	}
	nIndex = strCmd.Find("AuFixValueS=");
	if (nIndex > 0)
	{
		CString strTmp;
		int nTmpIndex;
		strTmp = strCmd.Right(strCmd.GetLength() - nIndex);
		nTmpIndex = strTmp.Find(";");
		strTmp = strTmp.Left(nTmpIndex);
		strCmd.Delete(nIndex, nTmpIndex + 1);
		strTmp = strTmp.Right(strTmp.GetLength() - 12);
		m_nAuFixValueS = atoi(strTmp);
	}
	else
	{
		m_nAuFixValueS = 0;
	}
	if (strCmd.Left(8) == "/screen ")
	{
		pMainWnd->m_bCaptureScreenMode = TRUE;

		strPath = strCmd.Mid(8);
		strPath.TrimLeft();
		strPath.TrimRight();
		int nIndex = strPath.Find("/profilepath_xxx");
		if (nIndex > 0)
		{
			CString strProfilePath = strPath.Mid(nIndex + 16);
			strProfilePath.TrimLeft();
			strProfilePath.TrimRight();
			strProfilePath = strProfilePath.Mid(1, strProfilePath.GetLength() - 2);

			strPath = strPath.Left(nIndex);
			strPath.TrimLeft();
			strPath.TrimRight();
			strPath = strPath.Mid(1, strPath.GetLength() - 2);

			pMainWnd->m_strPath = strPath;
			pMainWnd->m_bWithScreenProfileIni = TRUE;
			pMainWnd->m_strScreenProfilePath = strProfilePath;
		}
		else
		{
			strPath = strPath.Mid(1, strPath.GetLength() - 2);
			pMainWnd->m_strPath = strPath;
		}
	}
	else
	{
		strPath = strCmd;
		strPath.TrimLeft();
		strPath.TrimRight();
		strPath = strPath.Mid(1, strPath.GetLength() - 2);

		if (strPath.Right(1) != "\\" && strPath.Right(4).CompareNoCase(".mp4") != 0)
			strPath = strPath + "\\";
		pMainWnd->m_strPath = strPath;
	}

	if (strPath.Right(1) == "\\")
	{
		 //Lijinlong for temp
		//pMainWnd->m_strMediaPath = "C:\\eLearningProj_Wu\\eLearningTitle.JP\\";
		//pMainWnd->m_strRootPath = "C:\\eLearningProj_Wu\\eLearningTitle.JP\\";
		pMainWnd->m_strMediaPath = strPath + "video\\";
		pMainWnd->m_strRootPath = strPath;
		pMainWnd->m_strHTMLPath = strPath + "html\\movie\\";
		pMainWnd->m_bRecPath = TRUE;
	}
	else if (strPath.Right(4).CompareNoCase(".mp4") == 0)
	{
		CString strFilePath = ::GetFilePath(strPath);
		if (strFilePath.Right(12).CompareNoCase("html\\movie\\") == 0)
		{
			pMainWnd->m_strHTMLPath = strFilePath;
			pMainWnd->m_strRootPath = strFilePath.Left(strFilePath.GetLength() - 12);
			pMainWnd->m_strMediaPath = pMainWnd->m_strRootPath + "video\\";
			 //Lijinlong for temp
			//pMainWnd->m_strMediaPath = "C:\\eLearningProj_Wu\\eLearningTitle.JP\\";
			//pMainWnd->m_strRootPath = "C:\\eLearningProj_Wu\\eLearningTitle.JP\\";
		}
	}

	if (pMainWnd->m_strPath == "")
	{
		_mkdir("c:\\temp");
		pMainWnd->m_strPath = "C:\\temp\\";
	}

	CRect rc;
	rc.SetRectEmpty();
	if (!pMainWnd->Create(WS_VISIBLE | WS_BORDER | WS_POPUP, rc, NULL))
	{
		return FALSE;
	}
//	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRM));
//	m_pMainWnd->SetIcon(hIcon, TRUE);

	return TRUE;
}

void CELearningCaptureApp::ShowMessage(UINT nMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);

	m_bMessageBox = FALSE;
}

void CELearningCaptureApp::ShowWarning(UINT nMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);

	m_bMessageBox = FALSE;
}

void CELearningCaptureApp::ShowError(UINT nMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);

	m_bMessageBox = FALSE;
}

int CELearningCaptureApp::AskYesNo(UINT nMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

	m_bMessageBox = FALSE;

	return nRet;
}

int CELearningCaptureApp::AskOKCancel(UINT nMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

	m_bMessageBox = FALSE;

	return nRet;
}

void CELearningCaptureApp::ShowMessageStr(CString strMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);

	m_bMessageBox = FALSE;
}

void CELearningCaptureApp::ShowWarningStr(CString strMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);

	m_bMessageBox = FALSE;
}

void CELearningCaptureApp::ShowErrorStr(CString strMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);

	m_bMessageBox = FALSE;
}

int CELearningCaptureApp::AskYesNoStr(CString strMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

	m_bMessageBox = FALSE;

	return nRet;
}

int CELearningCaptureApp::AskOKCancelStr(CString strMsg, CWnd* pWnd)
{
	m_bMessageBox = TRUE;

	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

	m_bMessageBox = FALSE;

	return nRet;
}

void CELearningCaptureApp::OperationLog(CString strOperation, CString strValue)
{
}
