// eLearningCapture.h : main header file for the ELEARNINGCAPTURE application
//

#if !defined(AFX_ELEARNINGCAPTURE_H__C3D88CE5_AAD8_47AA_8A21_39CBE5E6AC4E__INCLUDED_)
#define AFX_ELEARNINGCAPTURE_H__C3D88CE5_AAD8_47AA_8A21_39CBE5E6AC4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define MACRO_APP						((CELearningCaptureApp*)AfxGetApp())

#define LOG(x,y)						((CELearningCaptureApp*)AfxGetApp())->OperationLog(x,y)

#define W_SHOW_MESSAGE(x,y)				((CELearningCaptureApp*)AfxGetApp())->ShowMessage(x,y)
#define W_SHOW_WARNING(x,y)				((CELearningCaptureApp*)AfxGetApp())->ShowWarning(x,y)
#define W_SHOW_ERROR(x,y)				((CELearningCaptureApp*)AfxGetApp())->ShowError(x,y)
#define W_ASK_YESNO(x,y)				((CELearningCaptureApp*)AfxGetApp())->AskYesNo(x,y)
#define W_ASK_OKCANCEL(x,y)				((CELearningCaptureApp*)AfxGetApp())->AskOKCancel(x,y)
#define W_SHOW_MESSAGE_STR(x,y)			((CELearningCaptureApp*)AfxGetApp())->ShowMessageStr(x,y)
#define W_SHOW_WARNING_STR(x,y)			((CELearningCaptureApp*)AfxGetApp())->ShowWarningStr(x,y)
#define W_SHOW_ERROR_STR(x,y)			((CELearningCaptureApp*)AfxGetApp())->ShowErrorStr(x,y)
#define W_ASK_YESNO_STR(x,y)			((CELearningCaptureApp*)AfxGetApp())->AskYesNoStr(x,y)
#define W_ASK_OKCANCEL_STR(x,y)			((CELearningCaptureApp*)AfxGetApp())->AskOKCancelStr(x,y)
#define W_SHOW_FUNC_ERROR_STR(x,y)		((CELearningCaptureApp*)AfxGetApp())->CommonFuncErrMsg(x,y)
#define W_SHOW_EXEC_ERROR_STR(x,y)		((CELearningCaptureApp*)AfxGetApp())->CommonErrMsg(x,y)

//#define SHOW_MESSAGE(x)		((CELearningCaptureApp*)AfxGetApp())->ShowMessage(x)
#define SHOW_MESSAGE(x)		((CELearningCaptureApp*)AfxGetApp())->ShowMessage(x,NULL)
#define SHOW_WARNING(x)		((CELearningCaptureApp*)AfxGetApp())->ShowWarning(x)
#define SHOW_ERROR(x)		((CELearningCaptureApp*)AfxGetApp())->ShowError(x)
#define ASK_YESNO(x)		((CELearningCaptureApp*)AfxGetApp())->AskYesNo(x)
#define ASK_OKCANCEL(x)		((CELearningCaptureApp*)AfxGetApp())->AskOKCancel(x)

#define SHOW_MESSAGE_STR(x)		((CELearningCaptureApp*)AfxGetApp())->ShowMessageStr(x)
#define SHOW_WARNING_STR(x)		((CELearningCaptureApp*)AfxGetApp())->ShowWarningStr(x)
#define SHOW_ERROR_STR(x)		((CELearningCaptureApp*)AfxGetApp())->ShowErrorStr(x)
#define ASK_YESNO_STR(x)		((CELearningCaptureApp*)AfxGetApp())->AskYesNoStr(x)
#define ASK_OKCANCEL_STR(x)		((CELearningCaptureApp*)AfxGetApp())->AskOKCancelStr(x)

#define MACRO_PRESENT_PATH		(::GetSysPath() + "Presentation\\") 

/////////////////////////////////////////////////////////////////////////////
// CELearningCaptureApp:
// See eLearningCapture.cpp for the implementation of this class
//
#include "SingleInstanceApp.h"
class CELearningCaptureApp : public CSingleInstanceApp
{
public:
	CELearningCaptureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELearningCaptureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CELearningCaptureApp)
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
	void OperationLog(CString strOperation, CString strValue);

	int     m_wndLeft;
	int     m_wndTop;
	int     m_wndHeight;

	long m_nAuFixValueC;
	long m_nAuFixValueS;

public:
	BOOL m_bMessageBox;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEARNINGCAPTURE_H__C3D88CE5_AAD8_47AA_8A21_39CBE5E6AC4E__INCLUDED_)
