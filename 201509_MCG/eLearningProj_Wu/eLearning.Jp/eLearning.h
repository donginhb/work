// eLearning.h : main header file for the ELearning application
//

#if !defined(AFX_ELearning_H__7337AAA4_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
#define AFX_ELearning_H__7337AAA4_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define W_SHOW_MESSAGE(x,y)				((CELearningApp*)AfxGetApp())->ShowMessage(x,y)
#define W_SHOW_WARNING(x,y)				((CELearningApp*)AfxGetApp())->ShowWarning(x,y)
#define W_SHOW_ERROR(x,y)				((CELearningApp*)AfxGetApp())->ShowError(x,y)
#define W_ASK_YESNOCANCEL(x,y)			((CELearningApp*)AfxGetApp())->AskYesNoCancel(x,y)
#define W_ASK_YESNO(x,y)				((CELearningApp*)AfxGetApp())->AskYesNo(x,y)
#define W_ASK_OKCANCEL(x,y)				((CELearningApp*)AfxGetApp())->AskOKCancel(x,y)
#define W_SHOW_MESSAGE_STR(x,y)			((CELearningApp*)AfxGetApp())->ShowMessageStr(x,y)
#define W_SHOW_WARNING_STR(x,y)			((CELearningApp*)AfxGetApp())->ShowWarningStr(x,y)
#define W_SHOW_ERROR_STR(x,y)			((CELearningApp*)AfxGetApp())->ShowErrorStr(x,y)
#define W_ASK_YESNO_STR(x,y)			((CELearningApp*)AfxGetApp())->AskYesNoStr(x,y)
#define W_ASK_YESNOCANCEL_STR(x,y)		((CELearningApp*)AfxGetApp())->AskYesNoCancelStr(x,y)
#define W_ASK_OKCANCEL_STR(x,y)			((CELearningApp*)AfxGetApp())->AskOKCancelStr(x,y)
#define W_SHOW_FUNC_ERROR_STR(x,y)		((CELearningApp*)AfxGetApp())->CommonFuncErrMsg(x,y)
#define W_SHOW_EXEC_ERROR_STR(x,y)		((CELearningApp*)AfxGetApp())->CommonErrMsg(x,y)

#define SHOW_MESSAGE(x)				((CELearningApp*)AfxGetApp())->ShowMessage(x,NULL)
#define SHOW_WARNING(x)				((CELearningApp*)AfxGetApp())->ShowWarning(x,NULL)
#define SHOW_ERROR(x)				((CELearningApp*)AfxGetApp())->ShowError(x,NULL)
#define ASK_YESNO(x)				((CELearningApp*)AfxGetApp())->AskYesNo(x,NULL)
#define ASK_YESNOCANCEL(x)			((CELearningApp*)AfxGetApp())->AskYesNoCancel(x,NULL)
#define ASK_OKCANCEL(x)				((CELearningApp*)AfxGetApp())->AskOKCancel(x,NULL)
#define SHOW_MESSAGE_STR(x)			((CELearningApp*)AfxGetApp())->ShowMessageStr(x,NULL)
#define SHOW_WARNING_STR(x)			((CELearningApp*)AfxGetApp())->ShowWarningStr(x,NULL)
#define SHOW_ERROR_STR(x)			((CELearningApp*)AfxGetApp())->ShowErrorStr(x,NULL)
#define ASK_YESNO_STR(x)			((CELearningApp*)AfxGetApp())->AskYesNoStr(x,NULL)
#define ASK_YESNOCANCEL_STR(x)		((CELearningApp*)AfxGetApp())->AskYesNoCancelStr(x,NULL)
#define ASK_OKCANCEL_STR(x)			((CELearningApp*)AfxGetApp())->AskOKCancelStr(x,NULL)
#define SHOW_FUNC_ERROR_STR(x)		((CELearningApp*)AfxGetApp())->CommonFuncErrMsg(x,NULL)
#define SHOW_EXEC_ERROR_STR(x)		((CELearningApp*)AfxGetApp())->CommonErrMsg(x,NULL)

#define PRO_VERSION					(((CELearningApp*)AfxGetApp())->m_nType == 0)
#define EV_VERSION					(((CELearningApp*)AfxGetApp())->m_nType == 1)
#define BUNDLE_VERSION				(((CELearningApp*)AfxGetApp())->m_nType == 2)
#define BACKEND_VERSION				(((CELearningApp*)AfxGetApp())->m_nType == 3)
#define PROMOTE_VERSION				(((CELearningApp*)AfxGetApp())->m_nType == 4)
#define TRY_STATE					(((CELearningApp*)AfxGetApp())->m_bEv)
#define LOG(x,y)					((CELearningApp*)AfxGetApp())->OperationLog(x,y)

/////////////////////////////////////////////////////////////////////////////
// CELearningApp:
// See eLearning.cpp for the implementation of this class
//
#include "SingleInstanceApp.h"
class CWndTimeRecord;
class CELearningApp : public CSingleInstanceApp
{
public:
	CELearningApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELearningApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CELearningApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ShowMessage(UINT nMsg, CWnd* pWnd = NULL);
	void ShowWarning(UINT nMsg, CWnd* pWnd = NULL);
	void ShowError(UINT nMsg, CWnd* pWnd = NULL);
	int AskYesNo(UINT nMsg, CWnd* pWnd = NULL);
	int AskYesNoCancel(UINT nMsg, CWnd* pWnd = NULL);
	int AskOKCancel(UINT nMsg, CWnd* pWnd = NULL);
	void ShowMessageStr(CString nMsg, CWnd* pWnd = NULL);
	void ShowWarningStr(CString nMsg, CWnd* pWnd = NULL);
	void ShowErrorStr(CString nMsg, CWnd* pWnd = NULL);
	int AskYesNoStr(CString nMsg, CWnd* pWnd = NULL);
	int AskYesNoCancelStr(CString nMsg, CWnd* pWnd = NULL);
	int AskOKCancelStr(CString nMsg, CWnd* pWnd = NULL);
	void CommonFuncErrMsg(CString strFunc, CWnd* pWnd = NULL);
	void CommonErrMsg(CString strFunc, CWnd* pWnd = NULL);
	BOOL CheckKey(CString strKey); 
	BOOL GetKey(CString& strKey); 
	BOOL WriteKey(CString strKey); 
	BOOL GetMachineID(CString &strID);
	void EncodeID(CString& strID); 
	BOOL CheckVersion2();
	//add 2014/6/13 kagata outemon
	int ChkNasUpRec();
	int ChkNasUpSt();
	int ChkNasUpEn();
	CString GetNasDir();
	bool NasDirRetry(CString m_pNasPath, UINT nID);
	void UpCompInfo(int m_NetAccess);
	int GetUpCompInfo();
	int CourseiniChk(CString CourseName);

public:
	CFont* m_pSysFont;
	CFont* m_pSysFont2;
	CFont* m_pSysFont3;
	CFont* m_pSysFont4;
	BOOL   m_bEv;
	int    m_nType;	
	CWndTimeRecord* m_pWndTime;
	BOOL   m_bFormal;
	BOOL   m_bReRun;
	//add 20140515 kagata outemon
	CString m_pNasPath;
	BOOL	m_pRecUp;
	BOOL	m_pStUp;
	BOOL	m_pEnUp;
	BOOL	m_pNetAccess;

protected:
	BOOL CheckKey(); 
	BOOL DeviceAvailable(BOOL& bAvailable, BOOL bSilent);
	int GetHexValue(char sz);
	BOOL CheckVersion();
	CString GetVerStr();
	void InitSetting();

protected:
	CString m_strRegKey;
	CString m_strFormerRegPath;
	//m_strFormerRegPath

public:
	CString m_strLogFile;
	void OperationLog(CString strOperation, CString strValue);
	void StartLog();
	void EndLog();
	void RemoveKey();
	BOOL AddHour(int nHour); 
	//add 2014/05/15 kagata outemon
	int NasCourse(CString strPath, CString strCourse, BOOL bDel);
	int NasLocalCheck(CString NasPath, CStringArray& CourseArray);
	int NasNameCheck(CString NasPath, CString CourseName);
	int Chkreload(CString NasPath, CString CourseName);
	int DirAndFileChk(CString NasPath, CString CourseName, CStringArray& ChkSaverArray, CStringArray& ChkLocalArray);
	int Chkdircommon(CString ChkSavdir, CString ChkLocdir);
	int Chkfilecommon(CStringArray& ChkSavfArray, CStringArray& ChkLocfArray);
	int Chkdircommon2(CString ChkSav2dir, CString ChkLoc2dir);
};

BOOL	WINAPI IsZoom(CString str);
BOOL	WINAPI IsCtrlMark(CString str); 
CString WINAPI MakeCtrlMark(CString str); 
CString WINAPI MakeDemoMark(CString str);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELearning_H__7337AAA4_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
