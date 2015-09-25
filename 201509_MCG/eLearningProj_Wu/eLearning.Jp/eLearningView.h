// eLearningView.h : interface of the CELearningView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEARNINGVIEW_H__7337AAAC_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
#define AFX_ELEARNINGVIEW_H__7337AAAC_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdapter;
class CELearningDoc;
class CELearningView : public CView
{
protected: // create from serialization only
	CELearningView();
	DECLARE_DYNCREATE(CELearningView)

// Attributes
public:
	CELearningDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELearningView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CELearningView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CELearningView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStop();
	afx_msg void OnSetting();
	afx_msg void OnRefresh();
	afx_msg void OnRecord();
	afx_msg void OnNewLesson();
	afx_msg void OnIe();
	afx_msg void OnDefaultBrowser();
	afx_msg void OnDelLesson();
	afx_msg void OnDevSetting();
	afx_msg void OnDefaultLayout();
	afx_msg void OnRecordAsfFile();
	afx_msg void OnRenameCourse();
	afx_msg void OnSaveAs();
	afx_msg void OnUpdateDelLesson(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenameCourse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUploadCourse(CCmdUI* pCmdUI);
	afx_msg void OnNewFromMedia();
	afx_msg void OnScreenCapture();
	afx_msg void OnBkupCourse();
	afx_msg void OnRestoreCourse();
	afx_msg void OnUpdateBackupCourse(CCmdUI* pCmdUI);
	afx_msg void OnBackupCourse();
	afx_msg void OnDelCourse();
	afx_msg void OnBackupMaintain();
	afx_msg void OnRegister();
	afx_msg void OnOnlineHelp();
	afx_msg void OnUpdateRegCode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRegister(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNewFromMedia(CCmdUI* pCmdUI);
	afx_msg void OnCourseLoader();
	afx_msg void OnUpdateOnlineHelp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNewLesson(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordAsfFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateScreenCapture(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDevSetting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDefaultLayout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetting(CCmdUI* pCmdUI);
	afx_msg void OnCheckOut();
	afx_msg void OnUpdateCheckOut(CCmdUI* pCmdUI);
	afx_msg void OnContactUs();
	afx_msg void OnSerialReuse();
	afx_msg void OnUpdateSerialReuse(CCmdUI* pCmdUI);
	afx_msg void OnBuyQuota();
	afx_msg void OnUseQuota();
	afx_msg void OnUpdateCourseLoader(CCmdUI* pCmdUI);
	afx_msg void OnImportCourse();
	afx_msg void OnUpdateImportCourse(CCmdUI* pCmdUI);
	afx_msg void OnHtmlPresent();
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnProfileSetting();
	afx_msg void OnScreenProfileSetting();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
	afx_msg void OnSaveVideo();
	afx_msg void OnUpdateSaveVideo(CCmdUI* pCmdUI);
	afx_msg void OnBatchVolume();
	afx_msg void OnUpdateScreenProfileSetting(CCmdUI* pCmdUI);
	afx_msg void OnNewWizard();
	afx_msg void OnUpdateDelCourse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBkupCourse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestoreCourse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBackupMaintain(CCmdUI* pCmdUI);
	afx_msg void OnOpen();
	afx_msg void OnUpdateEditPPT(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChangePPT(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRegenPPT(CCmdUI* pCmdUI);
	afx_msg void OnAudioFix();
	afx_msg void OnUpdateAudioFix(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//2006-03-22
public:
	int					m_nCourseType;
	int					CountNumber;
	BOOL                LayoutExistFlag;
	BOOL                OpenFileFlag;
	BOOL                NewCourseFlag;

protected:
	CStringArray		m_strArraySlide;
	CAdapter*			m_pAdapter;

public:

	BOOL GetSlideFileName(CString strIndex, CString strCmd, CString& strPath);
	void changeHTML(int sNo, int aNo);
	void replaceHTML(CString destpath, CString newpath, CStringArray& strsTag, CStringArray& strsReplace, bool bReplaceToLast);
	int	 GetCourseType() {return m_nCourseType;};
	BOOL GetSlideFile(int nIndex, CString& strFile);
	BOOL AddMarkScriptToASF(CString strCourse, BOOL bForce = FALSE); 
	void Navigate2(CString str);
	void LoadCourse(CString str, BOOL bForce = FALSE);
	void SaveVideoAfterRecording() ;
	void SubmitLesson(CString str, BOOL bWithMedia);
	void Stop();
	void UnLoadVideo();
	void ReLoadVideo();

	BOOL ReplacePptFile(); 
	BOOL OpenPptFile(); 
	//add by buxiangyi
	void NewArrayLayout();
	void GetPPTLayout(CStringArray &m_strArrayPPtName);
	CString GetIndexPPTLayout();

	//replase 2014/7/28 kagata chiba
	//BOOL RegenPptSlide(BOOL bWarning); 
	BOOL RegenPptSlide(BOOL bWarning, int newCourseex = 0); 
	void StopMovieWndTimer();
	void doWizard();
	void RestartMovieWndTimer();
	void NewLesson(CString strCourse, CString strPPTPath); 
	void SetActivePage(int nPage);
	int BackupOneCourse(CString strPath, CString strCourse, BOOL bDel);
	void SubmitHTMLLesson(CString strCourse); 
	void ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd);
	BOOL SaveCourse(CString strCourse, BOOL bAsk);
	CString GetPPTFilePath();
	BOOL SaveProfileParamsToLocal(BOOL bCheckDevice = TRUE); 
	BOOL SaveProfileParamsToLocalWizard(); 
	//add by buxiangyi
	BOOL  SaveProfileParamsToLocal(CString Name, CString strInipath); 
	////////////////////////////////
	//BOOL CheckVideoAndSave(BOOL bCancel = FALSE, BOOL bReload = TRUE);

	void fontFix(CString strCoursePath,CString strFileName);	//add 2013/11/26 kagata BugNo.26
	int RenameCourse(CString strOldName, int flag);	//add 2014/6/16 kagata outemon
	BOOL LoadPPTFile(CString strCourse);	//replase 2014/7/28 kagata chiba
	void SaveConfig(CString strCourse);
	int GetPPTIndex();
	CStringArray* GetDrawTime();
	CStringArray* GetDrawColor();
	CStringArray* GetDrawWidth();
	CStringArray* GetDrawCmd();
	CStringArray* GetDrawLeft();
	CStringArray* GetDrawTop();
	CStringArray* GetDrawRight();
	CStringArray* GetDrawBottom();
	void DeleteDrawPoint(int nIndex);
	CString GetDefaultLayout();
	void EditDrawMark();
	void MergeVideo(CString strSeg, CString strCourse);
	BOOL TestVideoFile(CString strVideoPath);

protected:
	void OnNewLessonWizard();
	void HtmlPresent(BOOL bOpenPresent); 
	void GetPresentTitles(CString strCourse, CStringArray& strArrayOutline); 
	BOOL CopyPresent(CString strCourse, CString strPresent); 
	//BOOL LoadPPTFile(CString strCourse);	//del 2014/7/28 kagata chiba
	int ImportOneCourse(CString strSrcPath, CString strCourse, CString& strNewCourse);
	//BOOL MergeOptionFile(CString strSrcFile, CString strDestFile);
	BOOL IsOurFile(CString strFileName);
	BOOL IsCourseGenerated(CString strCourse);
	//replase 2014/7/28 kagata chiba
	//BOOL RegenCoursePPT(CString strCourse, BOOL bReplace, BOOL bShowProgress);
	BOOL RegenCoursePPT(CString strCourse, BOOL bReplace, BOOL bShowProgress, int newCourseex = 0);
	BOOL LayoutFilesNotExists(CString strCourse);
	void ReplaceUndefinedTitle(CStringArray& strArrayOutline);
	void GetLayoutFiles(CStringArray& strArrayCopy);
	void GetFunctionalFiles(CStringArray& strArrayCopy, BOOL bCopySemiFunc);
	BOOL CopyViewerFiles(CString strCourse, BOOL bFailIfExist = TRUE);
	BOOL CopyFunctionalFiles(CString strCourse, BOOL bFailIfExist = TRUE, BOOL bCopySemiFunc = TRUE);
	BOOL CopyLayoutFiles(CString strCourse);
	void EnableTaskBar(BOOL bShow);
	void ProcessAVIToASF(CString strCourse); 
	BOOL OnGetfolder(CString& strFolderPath); 
 	BOOL HasCourseSelected();
	BOOL IsLocked(CString strSrcPath);
	//BOOL SetControlPoint(CString strPPTPath, CString strASFPath, CString strCourse, BOOL bWriteFile = TRUE);
	int SetupMedia(CString& strCourse);   //0:User abort; 1: success; -1: fail
	BOOL ReGenTag(CString strCourse, CStringArray& strArrayNewOutline);
	void InsertIntoArray(CString strStep,
						 CString strMark,
						 CStringArray& strArrayStep, 
						 CStringArray& strArrayMark);
	void InsertByTime(CString str, CStringArray& strArray); 
	//add by buxiangyi
	int RestoreOneCourse(CString strSrcPath, CString strCourse, BOOL bDel, BOOL bover, CString& strNewCourse);

	int DelCourse(CString strCourse, BOOL bSlient); 
	void WriteOutlookName(CString strCourse);
	void EncodeID(CString& strID); 
	void DecodeID(LPTSTR pstrID); 
	int GetPPTVersion(CString strPath);
	BOOL ProcessPNGToJPG(CString strCourse); 
	void CheckTimeDuration(CStringArray& strArray, long lDuration);
	void NewCourse(CString strCourse); 
	void NewHTMLLesson(CString strCourse, CString strPresent); 
	BOOL ModifyPPTFiles(CString strDir);
	void AddMarkHead(CStringArray& strArrayOutline);
	BOOL ExportCourseNoWMV(CString strFolderPath, CString strCourse);
	void GetDemoFiles(CString strCourse, CStringArray& strArrayDemoFiles);
	void GetAssociatedFiles(CString strCourse, CStringArray& strArrayAssociatedFiles);
	void CopyAssociatedFiles(CString strCourse, CString strFolder); 
	void CheckDefaultProfile(); 
	BOOL CheckTags(CString strCourse);
	void NewCourseWizard(CString strCourse, CString strLayoutName);
	void NewHTMLLessonWizard(CString strCourse, CString strPresent, CString strLayoutName);
	void NewLessonWizard(CString strCourse, CString strPPTPath, CString strLayoutName);
	void ChangeLayout(CString strCourse, CString strPath);
	void ChangeTV(CString strCourse, CString strPath);
	void WriteDevice(CString strCourse, CString strAudio, CString strVideoType, CString strVideo); 
	void RecordCourse(int bCourseWizard = 0);
	//add by buxaingyi
	void ChangeOutlook(CString strCourse, CString strOutlook);
	BOOL SaveRecordParamsToLocal(BOOL bCheckDevice = TRUE); 
	BOOL SaveScreenParamsToLocal(BOOL bCheckDevice = TRUE);
	BOOL CELearningView::CopyVideo_jsFile(CString strCourse);
	void CELearningView::GetVideo_jsFile(CStringArray& strArrayCopy);
public:
	afx_msg void OnEditPpt();
	afx_msg void OnChangePpt();
	afx_msg void OnRegenPpt();
	afx_msg void OnPBMarkUp();
	afx_msg void OnPBDeleteUp();
	afx_msg void OnPBMuteUp();
	afx_msg void OnPBReplaceUp();
	afx_msg void OnPBEditMarkUp();
	afx_msg void OnPBAddEventUp();
	afx_msg void OnPBZoomInUp();
	afx_msg void OnPBZoomOutUp();
	afx_msg void OnPBAddHtmlUp();
	afx_msg void OnPBAddSCUp();
	afx_msg void OnPBSaveVideo();
	afx_msg void OnPBAddWave();
	afx_msg void OnPBAddAR();
	afx_msg void OnPBAddAM();
	afx_msg void OnPBAddText();
	afx_msg void OnPBAddEffectUp();
	afx_msg void OnPBSetPoints();
	afx_msg void OnPBAdjustUp();
	afx_msg void OnSaveFile();
	afx_msg void OnHelp();
	afx_msg void OnCourseDefaultLayout();
	afx_msg void OnUpdateCourseDefaultLayout(CCmdUI *pCmdUI);
	afx_msg void OnCoursePageDefaultLayout();
};

#ifndef _DEBUG  // debug version in eLearningView.cpp
inline CELearningDoc* CELearningView::GetDocument()
   { return (CELearningDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEARNINGVIEW_H__7337AAAC_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
