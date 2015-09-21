// CourseLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "CourseLoaderDlg.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderApp

BEGIN_MESSAGE_MAP(CCourseLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CCourseLoaderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderApp construction

CCourseLoaderApp::CCourseLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCourseLoaderApp object

CCourseLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderApp initialization

BOOL CCourseLoaderApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCourseLoaderDlg dlg;

	CString strCmd = m_lpCmdLine;
	if (strCmd.Left(2) == "/c")
		dlg.m_strCourse = strCmd.Mid(2);

	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CCourseLoaderApp::ShowMessage(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);
}

void CCourseLoaderApp::ShowWarning(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);
}

void CCourseLoaderApp::ShowError(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);
}

int CCourseLoaderApp::AskYesNo(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet =	pWnd->MessageBox(strMsg, AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

#ifdef _LOG
	LOG("顯示<確認訊息盒>", strMsg);
	if (nRet == IDYES)
		LOG("點取<訊息盒>按鈕", "Yes");
	else
		LOG("點取<訊息盒>按鈕", "No");
#endif

	return nRet;
}

int CCourseLoaderApp::AskOKCancel(UINT nMsg, CWnd* pWnd)
{
	CString strMsg;
	strMsg.LoadString(nMsg);
	if (!pWnd)
		pWnd = m_pMainWnd;

	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

#ifdef _LOG
	LOG("顯示<確認訊息盒>", strMsg);
	if (nRet == IDOK)
		LOG("點取<訊息盒>按鈕", "OK");
	else
		LOG("點取<訊息盒>按鈕", "Cancel");
#endif

	return nRet;

}

void CCourseLoaderApp::ShowMessageStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONINFORMATION | MB_OK);
}

void CCourseLoaderApp::ShowWarningStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONWARNING | MB_OK);
}

void CCourseLoaderApp::ShowErrorStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONSTOP | MB_OK);
}

int CCourseLoaderApp::AskYesNoStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	int nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_YESNO);

#ifdef _LOG
	LOG("顯示<確認訊息盒>", strMsg);
	if (nRet == IDYES)
		LOG("點取<訊息盒>按鈕", "Yes");
	else
		LOG("點取<訊息盒>按鈕", "No");
#endif

	return nRet;
}

int CCourseLoaderApp::AskOKCancelStr(CString strMsg, CWnd* pWnd)
{
	if (!pWnd)
		pWnd = m_pMainWnd;
	int	nRet = pWnd->MessageBox(strMsg,  AfxGetAppName(), MB_ICONQUESTION | MB_OKCANCEL);

#ifdef _LOG
	LOG("顯示<確認訊息盒>", strMsg);
	if (nRet == IDOK)
		LOG("點取<訊息盒>按鈕", "OK");
	else
		LOG("點取<訊息盒>按鈕", "Cancel");
#endif

	return nRet;
}

void CCourseLoaderApp::StartLog()
{
#ifdef _LOG
	m_strLogFile = "op.log";
	CString strPath = ::GetSysPath();
	CString strFileName = strPath + m_strLogFile;
	::DeleteFile(strFileName);
#endif
}

void CCourseLoaderApp::EndLog()
{
#ifdef _LOG
	CString strPath = ::GetSysPath();
	CString strFileName = strPath + m_strLogFile;
	CStringArray strFileArray;
	::LoadFile(strFileName, strFileArray);

	for (int i = 0; i < strFileArray.GetSize(); i++)
	{
		CString str = strFileArray[i];
		int nIndex = str.Find("\t");
		if (nIndex >= 0)
		{
			str = str.Mid(nIndex + 1);
			strFileArray[i] = str;
		}	
	}

	CString strLast = "";
	int nStart = -1;
	for (i = 0; i < strFileArray.GetSize(); i++)
	{
		CString str = strFileArray[i];
		if (str == strLast)
		{
			if (nStart < 0)
				nStart = i - 1;
		}
		else
		{
			if (nStart >= 0)
			{
				if (strLast.Find("點取<對話盒>按鈕") < 0)
				{
					strFileArray.RemoveAt(nStart, i - nStart - 1);
					CString strTmp;
					int n;
					if ((n = strLast.Find("\t")) >= 0)
					{
						CString strHead = strLast.Left(n);
						CString strTail = strLast.Mid(n + 1);
						strTmp.Format("%s\t[重覆%d次]%s", strHead, i - nStart, strTail);
						strFileArray[nStart] = strTmp;
						i = nStart + 1;
						nStart = -1;
					}
				}
				else
					nStart = -1;
			}
		}

		strLast = str;
	}

	for (i = 0; i < strFileArray.GetSize(); i++)
	{
		CString str = strFileArray[i];
		CString strNew;
		strNew.Format("%d\t%s", i + 1, str);
		strFileArray[i] = strNew;
	}

	::SaveFile(strFileName, strFileArray);

	m_pMainWnd->SetFocus();
	m_strLogFile = "";
#endif
}

void CCourseLoaderApp::OperationLog(CString strOperation, CString strValue)
{
#ifdef _LOG
	CString strPath = ::GetSysPath();
	
	CString strFileName = strPath + "\\op.log";
	CStringArray strFileArray;
	::LoadFile(strFileName, strFileArray);

	for (int i = 0; i < strFileArray.GetSize(); i++)
	{
		CString str = strFileArray[i];
		int nIndex = str.Find("\t");
		if (nIndex >= 0)
		{
			str = str.Mid(nIndex + 1);
			strFileArray[i] = str;
		}	
	}

	CString strOp;
	if (strValue == "EMPTY")
		strOp.Format("Course Launcher\t%s\t", strOperation);
	else
		strOp.Format("Course Launcher\t%s\t<%s>", strOperation, strValue);
	strFileArray.Add(strOp);

	for (i = 0; i < strFileArray.GetSize(); i++)
	{
		CString str = strFileArray[i];
		CString strNew;
		strNew.Format("%d\t%s", i + 1, str);
		strFileArray[i] = strNew;
	}

	::SaveFile(strFileName, strFileArray);
#endif
}