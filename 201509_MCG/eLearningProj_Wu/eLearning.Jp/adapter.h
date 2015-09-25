/*
added by angf 2014/11/19
*/

// adapter.h : header file

#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RESIZEPAGE (WM_USER + 111)

class CWndToolBar;
class CMovieWnd;
class CColGrid;
class CTimeLinePanel;
class CPPTSyncWnd;
class CAdapter : public CWnd
{

// Construction
public:
	CAdapter(CWnd* pParentWnd = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	virtual ~CAdapter();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void Initialize();
	BOOL SaveVideo();
	BOOL LoadCourse(LPCTSTR szFilename, BOOL bForce = FALSE); 
	void Stop(); 
	void Pause(); 
	void Refresh(); 
	BOOL UnLoadVideo(); 
	void StopMovieWndTimer();
	void RestartMovieWndTimer();
	void ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd);
	BOOL AddControlPoint(CString strTime, CString strMark, CString strScript, CString strCmd);
	BOOL ReLoadVideo(); 
	BOOL IsModified(CString strCourse);
	BOOL SaveCourse(CString strCourse, BOOL bAsk = TRUE);
	void SaveAllParamFiles(CString strCourse);
	BOOL SaveVideoToFile(CString strCourse, CString strDestPath);
	BOOL SaveTagToFile(CString strDefaultFile);
	void ChangeMarker(CStringArray& strArrayNewMarker);
	void RefreshSlide();
	void Resize();
	int  GetArrayLayoutNameSize();
	BOOL SaveConfig(CString strCourse);
	CStringArray* GetDrawTime();
	CStringArray* GetDrawColor();
	CStringArray* GetDrawWidth();
	CStringArray* GetDrawCmd();
	CStringArray* GetDrawLeft();
	CStringArray* GetDrawTop();
	CStringArray* GetDrawRight();
	CStringArray* GetDrawBottom();
	void DeleteDrawPoint(int nIndex);
	void SaveDrawArray(CString strCourse);
	void MergeVideo(CString strSeg, CString strCourse);
	BOOL TestVideoFile(CString strVideoPath);
	void setviewer();

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
					DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
					UINT nID, CCreateContext* pContext);
protected:
	CMovieWnd*			m_pMovieWnd;
	CColGrid*			m_pGrid;
	CStatic*			m_wndEdit;
	CComboBox*          m_cbExamble;
	CFont*              m_font;
	CString				m_strCourse;

	CStringArray		m_strArrayMarkTime;
	CStringArray		m_strArrayScriptName;
	CStringArray		m_strArrayScriptTime;
	CStringArray		m_strArrayScriptCmd;
	//CStringArray		m_strArrayLayoutName;

	int					m_nUnit;
	CWndToolBar*		m_pPPTToolbar;
	CTimeLinePanel*		m_pTimePanel;

	int					m_ncxGap;
	int					m_ncyGap;
	int					m_ncyBetween;
	BOOL				m_bTracking;
	CRect				m_rectLimit;
	CRect				m_rectTracker;
	BOOL				m_bAdjusted;
	long				m_lUnloadPos;
	CPPTSyncWnd*		m_pPPTSyncWnd;
	CString				m_strClassName;
	CStringArray		m_strArrayTmpTag;
	BOOL				m_bVideoAdjusted;

protected:
	void ShowSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce = FALSE);
	void ChangeSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce = FALSE);
	CString FindJpegFilePrefix(LPCTSTR strCourse);
	BOOL SetHyperLinkToGrid();
	BOOL LoadHyperLink(CString strCourse);
	BOOL GetScriptByTime(long lPos, int& nIndex);
	void OnSlideChange(int nSlideNo);
	void GetSlideNo(CString& strScriptName);
	void GetSlideNoAndIndexByTime(long lPos, CString& strCmd, CString& strScript, int& nIndex);
	int SkipNonSlide(int nOrigIndex);
	void SetSlideAndHyperPos(CString strCourse, long lPos, BOOL bForce); 
	void SetSlideAndHyperPos2(CString strCourse, long lPos, BOOL bForce);

	void DrawResizeBar(CRect& rect);
	void StopTracking(BOOL bAccept);
	void StartTracking(int ht);
	void GetHitRect(CRect& rectHit);
	int HitTest(CPoint pt);
	void OnInvertTracker(const CRect& rect);
	void GetInsideRect(CRect& rect); 
	void MakeFlashObject(CStringArray& strFlash,CString strFilename);
	BOOL GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName);
	void SetVideoFile(long lPos); 
	int GetCurrentDemoIndex();
	int GetCurrentWebPageIndex();
	BOOL SaveVideo(CString strCourse);
	BOOL SaveTag(CString strCourse);
	void WriteRecordFile(CString strPath, CString strSeg);
	BOOL LoadVideoFile(CString strCourse);
	void WriteTagFile(CString strCourse);
	BOOL LoadTagFile(CString strCourse);
	BOOL LoadAudioReplaceFile(CString strCourse);
	BOOL LoadAudioMixFile(CString strCourse);
	BOOL LoadVideoTextFile(CString strCourse);
	BOOL LoadVideoEffectFile(CString strCourse);
	BOOL GetFilenameforrecordfile(CString& strSeg);

	BOOL GetAudioandVideolrnAndSetArray(CString strCourse,CString strSeg,int ToDoType);

	void SetAudioReplace(CString strCourse);
	void SetAudioMix(CString strCourse);
	void SetVideoText(CString strCourse);
	void SetVideoEffect(CString strCourse);
	void SetCaptionData(CString strPath, CString strCaption, CString strFontName, CString strFontSize,
		CString strItalic, CString strBold, CString strUnderline, CString strPosition,
		CString strHIndent, CString strVIndent, CString strBkMode, CString strTextColor,
		CString strBkColor, BOOL bApplyAll);
	void SetOneCaptionData(CString strPath, CString strCaption, CString strFontName, CString strFontSize,
		CString strItalic, CString strBold, CString strUnderline, CString strPosition,
		CString strHIndent, CString strVIndent, CString strBkMode, CString strTextColor,
		CString strBkColor);
	BOOL AddEffect(CString strCourse, long lTime, long lDuration, int nType, CString strSrcPath, BOOL bGenBmp);
	BOOL GenImageFiles(CString strCourse, long lTime, CString strBmpPath1, CString strBmpPath2);
	BOOL TransitEffect(int nIndex, long lEffectTime);
	void AddPPTRecord(long lTime, CString strStep, CString strCmd, CString strTitle);	
	void ModifyVideo();
	BOOL MutePeriod(long lTimeSelStart, long lTimeSelEnd);
	BOOL SetPPTPoints();
	BOOL AddEvent(long lTime, long lID);
	BOOL AddWave(long lTime, long lID);
	BOOL AddText(long lTime);
	void WriteTagToArray(CString strCourse, CStringArray& strArrayTag);
	void RetrieveVideoData(CString strCourse, CObArray* pobArray);	
	void ReloadAttrib(CObArray* pObArray);
	void ParseStringToArray(CString strSeg, CStringArray& strArraySeg);
	void LoadTagFromStringArray(CStringArray& strArrayTag);
	void LoadVideoTextFromArray(CStringArray& strArrayVideo,CString strCourse);
	void CompareAndProcessVideoData(CObArray* pobArray1, CObArray* pobArray2);
	void UpdateGrid(int nRow, int nCol, CString& strOrigData, CString& strNewData);
	//add buxiangyi
	CString GetDefaultLatout();
	BOOL LoadDrawArray(CString strCourse);

// Implementation
protected:
	afx_msg void OnDestroy();
	afx_msg LRESULT OnScriptAccepted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHyperLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimePosChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPosChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnKeyCtrlMoved(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg	LRESULT OnKeyCtrlPressing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegenTag(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModifyVideo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWaitForFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrdUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPaletteButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeInEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeOutEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeInOutEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransitEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetPPTPoints(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSyncWndClosed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddPPTRecord(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUndoAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedoAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRetrieveVideoData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyCtrlTobeMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPauseVideoPlay(WPARAM wParam, LPARAM lParam);
	//add by buxaingyi
	afx_msg void OnCComboBo( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//add by buxiangyi
public:
	int                  m_RowIndex; 
	CStringArray         m_strArrayLayoutName;
	CStringArray		m_strArrayMarkName;
	CStringArray		m_strArrayDrawIndex;
	CStringArray		m_strArrayDrawTime;
	CStringArray		m_strArrayDrawColor;
	CStringArray		m_strArrayDrawWidth;
	CStringArray		m_strArrayDrawCmd;
	CStringArray		m_strArrayDrawLeft;
	CStringArray		m_strArrayDrawTop;
	CStringArray		m_strArrayDrawRight;
	CStringArray		m_strArrayDrawBottom;
};



#endif //_ADAPTER_H_