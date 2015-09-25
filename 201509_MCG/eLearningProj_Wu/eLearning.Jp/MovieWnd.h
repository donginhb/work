#if !defined(AFX_MOVIEWND_H__8F8125A0_F81E_11D4_B46F_0020AF3085DB__INCLUDED_)
#define AFX_MOVIEWND_H__8F8125A0_F81E_11D4_B46F_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MovieWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMovieWnd window
#include <dshow.h>

#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L

// Application-defined messages
#define WM_GRAPHNOTIFY		(WM_APP + 21)
#define WM_PLAYFILE			(WM_APP + 22)
#define WM_SCRIPT			(WM_APP + 23)
#define WM_POS_CHANGED		(WM_APP + 24)
#define WM_TIME_CHANGED		(WM_APP + 25)
#define WM_PAUSE_VIDEO		(WM_APP + 26)
#define WM_PLAY_VIDEO		(WM_APP + 27)
#define WM_STOP_VIDEO		(WM_APP + 28)
#define WM_CONVERT_TIME		(WM_APP + 29)
#define WM_CHANGE_SEGMENT	(WM_APP + 30)

//
// Macros
//
//#ifndef SAFE_RELEASE 
//#define SAFE_RELEASE(i) {if (i) i->Release(); i = NULL;}
//#endif

//#define JIF(x) if (FAILED(hr=(x))) \
//    {RetailOutput(TEXT("FAILED(0x%x) ") TEXT(#x) TEXT("\n"), hr); goto CLEANUP;}

#define IDC_PLAY				0
#define IDC_STOP				1
#define IDC_PAUSE				2
#define IDC_PROFILE				3
#define IDC_CAPTURE				4
#define IDC_STATUS_BAR			5
#define IDC_STATUS_BMP			6

#define constLargeGap		10
#define constSmallGap		5
#define constButtonWidth	18
#define constButtonHeight	18
#define constStatusHeight	22

#ifndef constBtnNormal
#define constBtnNormal		0
#endif

#ifndef constBtnOver
#define constBtnOver		1
#endif

#ifndef constBtnDown
#define constBtnDown		2
#endif

#ifndef constBtnDisable
#define constBtnDisable		3
#endif

#ifndef constPlay
#define constPlay		0
#endif

#ifndef constStop
#define constStop		1
#endif

#ifndef constPause
#define constPause		2
#endif

#include "..\TimeLinePlayer\TimeLinePlayer\TimeLinePlayer_i.h"
//#include "..\MediaUtility\MediaUtility.h"

#ifndef _BTN_DEF
#define _BTN_DEF
#endif

typedef enum tagCTRL_TYPE
{
	CTRL_SUNK_BUTTON = 0,
	CTRL_PUSH_BUTTON = 1,
	CTRL_TOGGLE_BUTTON = 2,
	CTRL_STATUS_BAR = 3,
	CTRL_BMP = 4
}
CTRL_TYPE;

class CCtrlInfo: public CObject
{
public:
	CCtrlInfo(UINT nID, CTRL_TYPE nType, CRect rc, UINT nBmpID, 
		      CString strText, UINT nStatus = 0)
	{
		m_nID = nID; 
		m_nType = nType; 
		m_rcPosition = rc; 
		m_nBmpID = nBmpID; 
		m_nStatus = nStatus;
		m_nPreStatus = 0;
		m_strText = strText;
	};

	~CCtrlInfo(){};
	UINT		m_nID;
	CTRL_TYPE	m_nType;
	CRect		m_rcPosition;
	UINT		m_nBmpID;
	CString		m_strText;
	UINT		m_nStatus;
	UINT		m_nPreStatus;
	UINT		m_nBmpIndex;
};

class CBmpStatic;
//class CVideoStepWnd;
class CMovieWnd : public CWnd
{
// Construction
public:
	CMovieWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovieWnd)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMovieWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMovieWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

	void AddCtrl(UINT nID, CTRL_TYPE nType, CRect rc, UINT nBmpID, 
				 CString strText, UINT nStatus = 0);
	void GetVideoRect(CRect& rc);
	void ShowErrorMsg(HRESULT hr);

protected:
	static CString m_strClass;

	BOOL			m_bAudioOnly;

	BOOL			m_bClean;

	CObArray*		m_pCtrlArray;
	CImageList		m_imageList;
	int				m_nCtrlEntered;
	BOOL			m_bTracking;
	BOOL			m_bLeaveTrack;
	int				m_nClickCtrl;
	CPoint			m_ptPrePos;
	CToolTipCtrl	m_ToolTip;
	BOOL			m_bFit;
	CRect			m_rcRedraw;
	CRect			m_rcStatusBar;
	int				m_nTimerID;
	CFont*			m_pFont;
	CBmpStatic*		m_pBmpStatus;
	CBmpStatic*		m_pBmpStatusRep;
	BOOL			m_bWait;
	BOOL			m_bNoWait;
	int				m_nBtnHeight;
	CBmpStatic*		m_pBmpSep;
//	IEasyUtility*	m_pEasyUtility;
	BOOL			m_bLoading;
	IEasyMultiPlay*	m_pMixPlay;
	int				m_nCurIndex;
	long			m_lTimeLength;
	CString			m_strPreTimeLine;
	CString			m_strCurTimeLine;
	BOOL            m_bPause;

protected:
	void ShowInfo(); 
    void Say(LPCTSTR szText);

    void ShowState(void);
    BOOL CanStep(void);

	void CleanupInterfaces(void);
	void RetailOutput(LPCTSTR tszErr, ...);
	void DrawCtrl(CDC* pDC, CCtrlInfo* pInfo);
	int GetBmpCount();
	void LoadImageList();
	int CtrlEntered(CPoint point);
	void EnterCtrl(int nCtrl);
	void LeaveCtrl(int nCtrl);
	void LeaveTrack(); 
	void DoAction(UINT nID);
	CRect GetAllBtnRect();
	void AddToolTip();
	void RepaintBtn();
	void RefreshBtns();
	void SetRelatedButtons(UINT nID);
	void PressButton(UINT nID, BOOL bGo = TRUE);
	void RepaintCtrl(UINT nID);
    HRESULT Step(int n);
	void PauseFile(); 
	void StopFile(BOOL bSetToStart); 
	void PlayFile(); 
	void Redraw(CRect rc);
	void SetCtrlPos(int nIndex, CRect rc);
	//BOOL GetScript(CString strFileName); 
	int	 FindSlideIndex(CString strTime);
	CString GetTimeStr(long nPos);
	void RefreshStatusBar();
	void SkipFastMode();

public:
    void StepFrame() {Step(1);};
	void Pause() {PressButton(IDC_PAUSE);};
	void StopAndBack();
	void Stop();
	void Play() {PressButton(IDC_PLAY);}; 

	BOOL SetTimeLine(CString strParam);
	void EnableButtons(BOOL bEnable);
	void EnableButton(UINT nID, BOOL bEnable, BOOL bRepaint = TRUE);
	BOOL GetFileDuration(long& lDuration);
	BOOL SetFilePosition(long llCurPos);
	void SetTextFont(CFont* pFont) {m_pFont = pFont;};
	void ShowBmpStatus(BOOL bFlag);
	void ShowBmpStatusRep(BOOL bFlag);
	void WaitForCompletion(IUnknown *pGraph);
	void SetWaitCursor(BOOL bWait) {m_bWait = bWait; if (!bWait) m_bNoWait = TRUE;}
	BOOL IsRunning();
	long GetCurrentPos();
	CSize GetBmpDim(UINT nID);
	BOOL UnLoadFile();
	void CaptureImage(); 
	void StopTimer();
	void RestartTimer();
	//void SetLength(long lTime, BOOL bRefresh = FALSE);
	long GetLength() {return m_lTimeLength;};
	BOOL ReloadFile(long lTimePos = 0);
	BOOL SaveVideo(CString strOutput, CString strProfile, int bSaveFast);
	BOOL SaveAudio(CString strOutput, CString strProfile, int bSaveFast); 
	BOOL SetAudioReplace(CString strParam);
	BOOL SetAudioMix(CString strParam);
	BOOL SetVideoText(CString strParam);
	BOOL SetVideoEffect(CString strParam);
	BOOL AdjustAudio(long lFunc, long lValue); 
	BOOL AdjustVideo(long lFunc, long lValue);
	BOOL TestVideoFile(CString strVideoPath);

public:
	CStringArray*	m_pstrArrayCmd;
	CStringArray*	m_pstrArrayScript;
	CStringArray*	m_pstrArrayTime;

	int		m_nLargeGap;
	int		m_nSmallGap;
	UINT	m_nPlayID;
	UINT	m_nPauseID;
	UINT    m_nStopID;
	UINT	m_nProfileID;
	UINT	m_nCaptureID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIEWND_H__8F8125A0_F81E_11D4_B46F_0020AF3085DB__INCLUDED_)
