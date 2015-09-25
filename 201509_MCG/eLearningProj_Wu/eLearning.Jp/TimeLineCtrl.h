#if !defined(AFX_TIMELINECTRL_H__790E437F_92F1_47BF_B227_E18581587850__INCLUDED_)
#define AFX_TIMELINECTRL_H__790E437F_92F1_47BF_B227_E18581587850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeLineCtrl.h : header file
//

#define WM_SHIFT_LEFT			(WM_APP + 2773)
#define WM_SHIFT_RIGHT			(WM_APP + 2774)
#define WM_TIME_POS_CHANGED		(WM_APP + 2775)
#define WM_CHANGE_SLIDER		(WM_APP + 2776)
#define WM_SET_LENGTH			(WM_APP + 2777)

/////////////////////////////////////////////////////////////////////////////
// CTimeLineCtrl window
class CPointerWnd;
class CToolTipWnd;
class CColorWnd;
class CVideoWnd;
class CAudioWnd;
class CDrawWnd;
class CKeyFramePanel;
class CVideoEffectWnd;
class CEffectManageWnd;
class CTimeLineCtrl : public CWnd
{
// Construction
public:
	CTimeLineCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeLineCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTimeLineCtrl();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd); 

	// Generated message map functions
protected:
	//{{AFX_MSG(CTimeLineCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg	LRESULT OnPointerMove(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPointerUp(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPointerDown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnPointerMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnColorWndMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEffectPressed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyFramePressed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyFrameTobeMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyFrameMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyFramePressing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetPointerTime(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegenTag(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickVideoEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodEdit(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPeriodUnInsert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodUnDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeInEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeOutEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransitEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeInOutEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBreakPoint(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void MovePointer(int nX, int nY);
	void ShowToolTip(long nPos);
	CString GetTimeStr(long nPos);
	void SetTmpLinePos(CPoint point);
	void ShowTmpLocateLine(BOOL bShow);
	void SetLinePos(); 
	void AddstrArrayNoDup(CString str, CStringArray& strArray);
	void SetPointerTime(long lTime);
	BOOL IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd);

public:
	void SetBreakPoint(long lTimeStart = -1, long lTimeEnd = -1);
	BOOL DeletePeriod();
	BOOL DeletePeriod(long lTimeSelStart, long lTimeSelEnd);
	void SetMark();
	long PosToTime(int xPos);
	int TimeToPos(long lTime);
	BOOL IsPointerVisible();
	int GetPixelUnit() {return m_nPixelUnit;};
	int GetTimeUnit() {return m_nUnit;};
	long GetTimeStart() { return m_lTimeStart;};
	void SetPixelUnit(int nPixelUnit);
	void SetTimeUnit(int nTimeUnit);
	void SetTimeStart(long lTime);
	void SetPointerPos();
	void SetLength(long lLength);
	void ShowTimeAt(long lTime);
	void ShiftWindow(int nShiftAmount, BOOL bAdjust = TRUE);
	void SetKeyFrame(CStringArray& strMarkArray, CStringArray& strMarkTimeArray, 
					 CStringArray& strScriptArray, CStringArray& strCmdArray, 
					 CStringArray& strScriptTimeArray);
	void RemoveAllKeyFrame();
	void OnEventPosChanged();
	void SetPointerTimeAndShow(long lTime);
	long GetPointerTime() { return m_lTimePointer; };
	void GetTagFromUI(CStringArray& strArrayTimeMarker, 
		 			  CStringArray& strArrayMarker, 
					  CStringArray& strArrayTimeScript,
					  CStringArray& strArrayScript,
					  CStringArray& strArrayCmd);
	void GetTagFromUI(CStringArray& strArrayTime, 
					  CStringArray& strArrayMarker, 
					  CStringArray& strArrayScript,
					  CStringArray& strArrayCmd);
	BOOL GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName);
	BOOL GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName);
	BOOL GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName);
	void Initialize();
	BOOL ReplacePeriod(long lTimeStart, long lTimeEnd, CString strVideoPath, long lDuration);
	void GetReplacePeriod(long& lTimeStart, long& lTimeEnd);
	void GetVideoSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath);
	void GetAudioReplaceSegString(CString& str);
	void GetAudioMixSegString(CString& str);
	void GetVideoTextSegString(CString& str);
	void GetVideoEffectSegString(CString& str);
	void SetKeyFrame(CStringArray& strArrayTime, CStringArray& strArrayMark,  
					 CStringArray& strArrayScript, CStringArray& strArrayCmd);
	void GetFrameInfo(int nIndex, CString& strTitle, long& lTime, CString& strCmd, CString& strStep);
	void RemoveFrameObj(int nIndex);
	void GetZoomIn(int& nRetTimeUnit, int& nRetPixelUnit);
	void GetZoomOut(int& nRetTimeUnit, int& nRetPixelUnit);
	BOOL IsMinScale();
	CRect GetCtrlRect(int nIndex);
//	BOOL IsVideoModified();
	void SetVideoSegArray(CStringArray& strArrayVideo, CString strSubPath);
	void SetNewSegArray();
	BOOL AddAudioReplace(long lTime, long lTimeEnd, CString strDestPath, COLORREF crNormal, COLORREF crHot);
	BOOL AddAudioMix(long lTime, long lTimeEnd, CString strDestPath);
	BOOL AddVideoText(long lTime, long lTimeEnd, CString strDestPath, CString strTip);
	BOOL AddVideoEffect(long lTime, long lDuration, int nType, CString strDestPath);
	void SetAudioReplaceSegArray(CStringArray& strArrayVideo);
	void SetAudioMixSegArray(CStringArray& strArrayVideo);
	void SetVideoTextSegArray(CStringArray& strArrayVideo);
	void SetVideoEffectSegArray(CStringArray& strArrayVideo);
	void DeleteAudioReplaceByIndex(int nIndex);
	void DeleteAudioMixByIndex(int nIndex);
	void DeleteVideoTextByIndex(int nIndex);
	void DeleteVideoEffectByIndex(int nIndex);
	void GetAudioReplaceSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetAudioMixSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetVideoTextSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetVideoEffectSegInfo(int nIndex, long& lTime, long& lDuration, CString& strPath, int& nType);
	void SetVideoTextSegTip(int nIndex, CString strTip); 
	int GetVideoTextSegCount();
	void ResetVideoTextTooltip();
	void ResetVideoEffectTooltip();
	void ResetAudioReplaceTooltip();
	void ResetAudioMixTooltip();
	BOOL IsVideoTextOverlap(long lTime, long lTimeEnd);
	BOOL IsVideoEffectOverlap(long lTime, long lDuration);
	BOOL IsAudioReplaceOverlap(long lTime, long lTimeEnd);
	BOOL IsAudioMixOverlap(long lTime, long lTimeEnd);
	void UnDeletePeriod(long lTime);
	void GetSelRange(long& lTimeSelStart, long& lTimeSelEnd);
	void SetAudioReplaceSegColor(int nIndex, COLORREF crNormal, COLORREF crHot);
	void SetNoSelectAndRepaint();
	BOOL GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, CString& strPathNext, long& lTimeNext);
	void SetSlidePath(CString slidePath);

protected:
	long				m_lLength;
	CPointerWnd*		m_pPointer;
	CSize				m_csPointer;
	CPoint				m_ptOffset;
	long				m_lTimePointer;
	CToolTipWnd*		m_pToolTip;
	CColorWnd*			m_pLocateLine;
	CColorWnd*			m_pTmpLocateLine;
	int					m_nLeftShift;
	int					m_nRightShift;
	UINT				m_nID;
	UINT				m_nID2;
	CEffectManageWnd*	m_pVideoEffectWnd;
	CVideoEffectWnd*	m_pAudioReplaceWnd;
	CVideoEffectWnd*	m_pAudioMixWnd;
	CVideoEffectWnd*	m_pVideoTextWnd;
	long				m_lTimeStart;
	int					m_nUnit;
	int					m_nPixelUnit;
	CVideoWnd*			m_pVideoWnd;
	CAudioWnd*			m_pAudioWnd;
	CDrawWnd*			m_pDrawWnd;
	CKeyFramePanel*		m_pKeyFrame;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMELINECTRL_H__790E437F_92F1_47BF_B227_E18581587850__INCLUDED_)
