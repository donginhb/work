#if !defined(AFX_TIMELINEPANEL_H__776562AF_58AC_4EDB_A72C_B76CF2C81E07__INCLUDED_)
#define AFX_TIMELINEPANEL_H__776562AF_58AC_4EDB_A72C_B76CF2C81E07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeLinePanel.h : header file
//

#define WM_MODIFY_VIDEO			(WM_APP + 1490)
#define WM_WAIT_FOR_FILE		(WM_APP + 1491)
#define WM_PALETTE_BTN_UP		(WM_APP + 1497)
#define WM_MUTE_PERIOD			(WM_APP + 1498)
#define WM_SET_PPT_POINT		(WM_APP + 1499)
#define WM_RETRIEVE_VIDEO_DATA	(WM_APP + 1500)
#define WM_PAUSE_VIDEOPLAY		(WM_APP + 1501)
#include "..\ComCapture\ComCapture\ComCapture_i.h"
/////////////////////////////////////////////////////////////////////////////
// CTimeLinePanel window
class CTimeLineCtrl;
class CSliderWnd;
class CToolTipWnd;
class CBmpBtn;
class CToolPaleteVideoEdit;
class CTimeLinePanel : public CWnd
{
// Construction
public:
	CTimeLinePanel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeLinePanel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTimeLinePanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTimeLinePanel)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSliderHScroll(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShiftLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShiftRight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimePosChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeSlider(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyPressing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPaleteBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetLength(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegenTag(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodEdit(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnPeriodUnInsert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPeriodUnDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeInEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFadeOutEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransitEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyTobeMoved(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		 		  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void ShowToolTip(long lTime);
	CString GetTimeStr(long nPos);
	void SetSliderRange();
	BOOL GetVideo(CString strCourse, CString& strVideoPath);
	int GetCurrentInsTmpIndex();
	void RecordASFFile(CString strPath);
	BOOL IsArrayDifferent(CStringArray& strArrayInputTime1, 
						  CStringArray& strArrayInputValue1,
						  CStringArray& strArrayInputTime2, 
				 		  CStringArray& strArrayInputValue2);
	void CutAndConv(CStringArray& strArrayInput, 
			        CStringArray& strArrayOutputTime,
					CStringArray& strArrayOutputValue);
	BOOL IsTagChanged(CStringArray& strArrayOutline, 
			          CStringArray& strArrayStep,
					  CStringArray& strArrayOldOutline, 
				 	  CStringArray& strArrayOldStep);
	BOOL OnEditCP(int nIndex); 
	void OnDeleteCP(int nIndex); 
	BOOL OnCaptureCP(int nIndex); 
	BOOL OnViewCP(int nIndex); 
	void OnDeleteAllCP(); 
	BOOL OnImportDemoFile(int nIndex); 

public:
	BOOL ReplacePeriod();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd); 
	void SetLength(long lDuration);
	long GetLength() { return m_lLength; };
	void SetKeyFrame(CStringArray& strMarkArray, CStringArray& strMarkTimeArray, 
					 CStringArray& strScriptArray, CStringArray& strCmdArray, 
					 CStringArray& strScriptTimeArray);
	void RemoveAllKeyFrame();
	void SetPointerTimeAndShow(long lTime);
	void GetTagFromUI(CStringArray& strArrayTimeMarker, 
					  CStringArray& strArrayMarker, 
					  CStringArray& strArrayTimeScript,
					  CStringArray& strArrayScript,
					  CStringArray& strArrayCmd);
	BOOL GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName);
	BOOL GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName);
	BOOL GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName);
	void Initialize();
	void GetVideoSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath);
	void GetAudioReplaceSegString(CString& str);
	void GetAudioMixSegString(CString& str);
	void GetVideoTextSegString(CString& str);
	void GetVideoEffectSegString(CString& str);
	void ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd);
	BOOL EditMark(int nPrePage, int nPreCell);
	void SetVideoSegArray(CStringArray& strArrayVideo);
	void SetAudioReplaceSegArray(CStringArray& strArrayVideo);
	void SetAudioMixSegArray(CStringArray& strArrayVideo);
	void SetVideoTextSegArray(CStringArray& strArrayVideo);
	void SetVideoEffectSegArray(CStringArray& strArrayVideo);
	void SetNewSegArray();
	BOOL AddAudioMix(long lTime, long lTimeEnd, CString strDestPath);
	BOOL AddAudioReplace(long lTime, long lTimeEnd, CString strDestPath);
	BOOL AddVideoText(long lTime, long lTimeEnd, CString strDestPath, CString strTip);
	BOOL AddVideoEffect(long lTime, long lDuration, int nType, CString strDestPath);
	void DeleteAudioReplaceByIndex(int nIndex);
	void DeleteAudioMixByIndex(int nIndex);
	void DeleteVideoTextByIndex(int nIndex);
	void DeleteVideoEffectByIndex(int nIndex);
	void GetAudioReplaceSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetAudioMixSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetVideoTextSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	void GetVideoEffectSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType);
	BOOL IsVideoTextOverlap(long lTime, long lTimeEnd);
	BOOL IsVideoEffectOverlap(long lTime, long lDuration);
	BOOL IsAudioReplaceOverlap(long lTime, long lTimeEnd);
	BOOL IsAudioMixOverlap(long lTime, long lTimeEnd);
	void SetVideoTextSegTip(int nIndex, CString strTip); 
	int GetVideoTextSegCount();
	void ResetVideoTextTooltip();
	void ResetVideoEffectTooltip();
	void ResetAudioReplaceTooltip();
	void ResetAudioMixTooltip();
	BOOL GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, CString& strPathNext, long& lTimeNext);
	void SetMark();
	BOOL DeletePeriod();
	void GetSelRange(long& lTimeSelStart, long& lTimeSelEnd);
	long GetPointerTime();
	void SetBreakPoint();
	void RedrawPanel();
	void SetSlidePath(CString slidePath);

protected:
	CTimeLineCtrl*			m_pTimeCtrl;
	CSliderWnd*				m_pSliderWnd;
	long					m_lLength;
	CToolTipWnd*			m_pToolTip;
	CBmpBtn*				m_pBtnPrev;
	CBmpBtn*				m_pBtnNext;
	CBmpBtn*				m_pBtnPrevPage;
	CBmpBtn*				m_pBtnNextPage;
	CBmpBtn*				m_pBtnZoomin;
	CBmpBtn*				m_pBtnZoomout;
	CSize					m_csBtn;
	CSize					m_csSmallBtn;
	CToolPaleteVideoEdit*	m_pToolPalete;
	IEasyCapture* m_pEasyCapture;
	PROCESS_INFORMATION m_pi;
//	IEasyUtility*			m_pEasyUtility;
//	IEasyConvert*			m_pConvert;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMELINEPANEL_H__776562AF_58AC_4EDB_A72C_B76CF2C81E07__INCLUDED_)
