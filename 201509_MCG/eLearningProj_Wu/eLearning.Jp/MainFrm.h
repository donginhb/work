// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C37D834B_EF28_11D4_BE14_0020AF3085DB__INCLUDED_)
#define AFX_MAINFRM_H__C37D834B_EF28_11D4_BE14_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_NEW_COURSE			(WM_APP + 100)
#define WM_LOAD_COURSE			(WM_APP + 459)
#define WM_GETUP				(WM_APP + 902)
#define WM_FILE_RECORDED		(WM_APP + 903)


typedef struct
{
	char	szPath[MAX_PATH];
} IPCStruct;

#include "DlgProgress.h"
#include "..\MediaFile\MediaFile.h"
#include "..\MediaKit\MediaKit.h"
#include "UndoRedoObj.h"
#include "Resource.h"

#include "eLearning.h"	//add 2014/7/28 kagata outemon
#include "adapter.h"	//add 2014/7/28 kagata chiba
#include "..\TimeLinePlayer\VideoEdit\VideoEdit_i.h"
#include "..\VideoConvert\VideoConvert\VideoConvert_i.h"

class CPPTCtrl;
class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CReBar			m_wndReBar;
	CDlgProgress	m_dlgProgress;
	CComboBoxEx		m_wndAddress;

	IMediaConvert*	m_pMediaConvert;
	CString			m_strMsg;
	CString			m_strTitle;
	int				m_nSlideNo;
	long			m_lTimeStart;
	long			m_lTimeEnd;
	CString			m_strWaitFile;
	CString			m_strTmpCourse;
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_StatusBar;
	CMFCShellTreeCtrl m_wndTree;
	CMFCCaptionBar    m_wndCaptionBar;

//	CString			m_strCourse;

	//2014/6/13 kagata outemon
	CELearningApp*	m_pCEApp;
	IEasyVideoEdit	*m_pEasyVideoEdit;
	IEasyVideoConvert*	m_pVideoConvert;

public:
	BOOL			m_bNoLoadingVideo;
//
	BOOL			m_bCannotRun;
	BOOL			m_bPause;
	BOOL	        m_ChangeCourse;
	CString			m_strCourse;
	CUndoRedoObj	m_objUndoRedo;
	BOOL			m_bAskJmp;
	long m_nAuFixValueC;
	long m_nAuFixValueS;

public:
	CString	GetPreCourse() {return m_strCourse;};
	int SetComboContent(CStringArray* pArray, CString str = "");
	CString GetHtml();
	void  Kill_Timer();
	void Set_Timer() ;
	void LoadCourse(BOOL bForce = TRUE);
	//replase 2014/6/17 kagata outemon
	void LoadCourse(CString strCourse, BOOL bForce = FALSE, BOOL bAsk = TRUE, BOOL bSkip = TRUE);
	void CollectCourse(CString strOn = "?");
	CString GetCourse();
	BOOL DumpKeyAndCreateIndex(CString strCourse);
	BOOL CopyAsfAudioOnly(CString strSrcCourse, CString strDestCourse);
	BOOL ExportAudioFile(CString strSrcCourse, CString strDestCourse);
	BOOL ConvertFile(LPCTSTR szIn, LPCTSTR szOut, int nAudioFreq, int nVideoFreq, BOOL bGenProfile = FALSE);
	BOOL ConvertASFFile(LPCTSTR szIn, LPCTSTR szOut, 
					    int nAudioFreq, int nFPS, int nVideoFreq);
	void StopRunning();
	void SetProgress(CString strTitle, CString strMsg);
	void ShowProgress();
	void HideProgress();
	BOOL ConvertVideo(CString str);
	//replase 2014/7/28 kagata chiba
	//BOOL GenPPTFiles(CString str, CString strPPTPath, BOOL& bWithMedia, BOOL bShowProgress);
	BOOL GenPPTFiles(CString str, CString strPPTPath, BOOL& bWithMedia, BOOL bShowProgress, int bSize = 0);
	BOOL ConvertFromASF(CString strCourse, CString strSrcASF);
	BOOL GenProfile(CString strSrcASF, CString strCourse);
	BOOL CreateIndex(CString strFile);
	BOOL SaveTag(CString strAsfFile, CStringArray& strMarkTimeArray, 
				 CStringArray& strMarkArray, CStringArray& strScriptTimeArray,
				 CStringArray& strScriptArray, CStringArray& strCmdArray, BOOL bRemove = FALSE);
	BOOL LoadTag(CString strAsfFile, CStringArray& strMarkTimeArray, 
				 CStringArray& strMarkArray, CStringArray& strScriptTimeArray,
				 CStringArray& strScriptArray, CStringArray& strCmdArray);
	long GetMediaLength(CString strPath);
	BOOL ConvertAudio(CString strSrc, CString strSrcProfile, CString strDest, BOOL bCopyScript = FALSE);
	BOOL ConvertBandWidth(CString strSrc, CString strSrcProfile, CString strDest, 
						  int nVideoBand, int nFPS, int nKeyFrame, int nVideoQuality, 
						  int nAudioBand, int nAudioFreq, int nMono, BOOL bCopyScript = FALSE);
	BOOL IsCourseVideoModified(CString strCourse, BOOL* pbOnlyTagModified = NULL);
	BOOL IsArrayDifferent(CStringArray& strArray1, CStringArray& strArray2);
	BOOL GetPRXType(CString strPath, long& lAudio);
	BOOL GenWMAForMute(long lmsDuration, CString strProfile, CString strDestPath);
	BOOL GetPRXParams(CString strPath, PROFILEPARAMS& param);
	CString GetProfileFromID(long lID);
	BOOL GetTwoSideImages(CString strSrcPath, long lTime, CString strPrevPath, CString strNextPath);
	BOOL GetPrevImages(CString strSrcPath, long lTime, CString strPrevPath);
	BOOL GetNextImages(CString strSrcPath, long lTime, CString strNextPath);
	BOOL IsSameProfile(CString strSrc, CString strDest);
	BOOL ConvertToSameProfile(CString strInputFile, CString strOutputPath, CString strProfilePath);
	BOOL GenScreenProfile(CString strCourse);
	BOOL GenVideofile(CString strCourse);
	void WriteVideoDate(CString strCourse, CString strMediaPath);
	void WriteVideoLength(CString strCourse);
	void WriteSlideNo(CString strCourse);
	BOOL CopyTag(CString strSrcAsf, CString strDestAsf);
	void WriteRecordFile(CString strPath, CString strSeg);
    void ParseStringToArray(CString strSeg, CStringArray& strArraySeg);
	//add 2014/6/13 kagata outemon
	int NasUpCourse();
	void ConImeCut();
	void MakeDeviceSetInfoFile(CString temp);
	BOOL AudioFix(long nAuFixValue);

protected:
	void SaveHyperLink(CString strCourse);
	void AddMarkScriptToASF();
	void SetComboSel(CString strSel);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	BOOL IsFirstLoading(CString str);
	BOOL IsASFTagged(CString strAsf);
	BOOL CopyASFVariousBandwidth(CString strSrcCourse, CString strDestCourse);
	BOOL ConvertVideoFromWMVToNoScript(CString strCourse);
	BOOL MergeWMVFile(CString strCourse);
	BOOL ConvertWMVWithoutScript(CString strCourse);
	BOOL ProcessScriptTime(CString strCourse);
	BOOL ProcessAllScriptTime(CString strCourse);
	void ConverTimeByIndex(CStringArray& strArray, 
						   CStringArray& wArrayIndex,
						   CStringArray& strArrayScript);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnNewCourse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadCourse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileRecorded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimeLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimeOver(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndTrace();
	afx_msg void OnStartTrace();
	afx_msg LRESULT OnWaitForFile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//add by buxiangyi
	afx_msg void OnExit();
	//////////////////////////////////
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CopyToClipboard();
	void OnNewAddress();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C37D834B_EF28_11D4_BE14_0020AF3085DB__INCLUDED_)
