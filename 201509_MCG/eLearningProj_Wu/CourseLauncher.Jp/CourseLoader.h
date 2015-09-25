// CourseLoader.h : main header file for the COURSELOADER application
//

#if !defined(AFX_COURSELOADER_H__B63474B4_6FF1_48C2_BE7D_9FCBD7913317__INCLUDED_)
#define AFX_COURSELOADER_H__B63474B4_6FF1_48C2_BE7D_9FCBD7913317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderApp:
// See CourseLoader.cpp for the implementation of this class
//

#define W_SHOW_MESSAGE(x,y)				((CCourseLoaderApp*)AfxGetApp())->ShowMessage(x,y)
#define W_SHOW_WARNING(x,y)				((CCourseLoaderApp*)AfxGetApp())->ShowWarning(x,y)
#define W_SHOW_ERROR(x,y)				((CCourseLoaderApp*)AfxGetApp())->ShowError(x,y)
#define W_ASK_YESNO(x,y)				((CCourseLoaderApp*)AfxGetApp())->AskYesNo(x,y)
#define W_ASK_OKCANCEL(x,y)				((CCourseLoaderApp*)AfxGetApp())->AskOKCancel(x,y)
#define W_SHOW_MESSAGE_STR(x,y)			((CCourseLoaderApp*)AfxGetApp())->ShowMessageStr(x,y)
#define W_SHOW_WARNING_STR(x,y)			((CCourseLoaderApp*)AfxGetApp())->ShowWarningStr(x,y)
#define W_SHOW_ERROR_STR(x,y)			((CCourseLoaderApp*)AfxGetApp())->ShowErrorStr(x,y)
#define W_ASK_YESNO_STR(x,y)			((CCourseLoaderApp*)AfxGetApp())->AskYesNoStr(x,y)
#define W_ASK_OKCANCEL_STR(x,y)			((CCourseLoaderApp*)AfxGetApp())->AskOKCancelStr(x,y)
#define W_SHOW_FUNC_ERROR_STR(x,y)		((CCourseLoaderApp*)AfxGetApp())->CommonFuncErrMsg(x,y)
#define W_SHOW_EXEC_ERROR_STR(x,y)		((CCourseLoaderApp*)AfxGetApp())->CommonErrMsg(x,y)

#define SHOW_MESSAGE(x)				((CCourseLoaderApp*)AfxGetApp())->ShowMessage(x,NULL)
#define SHOW_WARNING(x)				((CCourseLoaderApp*)AfxGetApp())->ShowWarning(x,NULL)
#define SHOW_ERROR(x)				((CCourseLoaderApp*)AfxGetApp())->ShowError(x,NULL)
#define ASK_YESNO(x)				((CCourseLoaderApp*)AfxGetApp())->AskYesNo(x,NULL)
#define ASK_OKCANCEL(x)				((CCourseLoaderApp*)AfxGetApp())->AskOKCancel(x,NULL)
#define SHOW_MESSAGE_STR(x)			((CCourseLoaderApp*)AfxGetApp())->ShowMessageStr(x,NULL)
#define SHOW_WARNING_STR(x)			((CCourseLoaderApp*)AfxGetApp())->ShowWarningStr(x,NULL)
#define SHOW_ERROR_STR(x)			((CCourseLoaderApp*)AfxGetApp())->ShowErrorStr(x,NULL)
#define ASK_YESNO_STR(x)			((CCourseLoaderApp*)AfxGetApp())->AskYesNoStr(x,NULL)
#define ASK_OKCANCEL_STR(x)			((CCourseLoaderApp*)AfxGetApp())->AskOKCancelStr(x,NULL)
#define SHOW_FUNC_ERROR_STR(x)		((CCourseLoaderApp*)AfxGetApp())->CommonFuncErrMsg(x,NULL)
#define SHOW_EXEC_ERROR_STR(x)		((CCourseLoaderApp*)AfxGetApp())->CommonErrMsg(x,NULL)
#define LOG(x,y)					((CCourseLoaderApp*)AfxGetApp())->OperationLog(x,y)

class CCourseLoaderApp : public CWinApp
{
public:
	CCourseLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCourseLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCourseLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ShowMessage(UINT nMsg, CWnd* pWnd = NULL);
	void ShowWarning(UINT nMsg, CWnd* pWnd = NULL);
	void ShowError(UINT nMsg, CWnd* pWnd = NULL);
	int AskYesNo(UINT nMsg, CWnd* pWnd = NULL);
	int AskOKCancel(UINT nMsg, CWnd* pWnd = NULL);
	void ShowMessageStr(CString nMsg, CWnd* pWnd = NULL);
	void ShowWarningStr(CString nMsg, CWnd* pWnd = NULL);
	void ShowErrorStr(CString nMsg, CWnd* pWnd = NULL);
	int AskYesNoStr(CString nMsg, CWnd* pWnd = NULL);
	int AskOKCancelStr(CString nMsg, CWnd* pWnd = NULL);

public:
	CString m_strLogFile;
	void OperationLog(CString strOperation, CString strValue);
	void StartLog();
	void EndLog();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COURSELOADER_H__B63474B4_6FF1_48C2_BE7D_9FCBD7913317__INCLUDED_)
