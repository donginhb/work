//{{AFX_INCLUDES()
//#include "screen2video1.h"
//}}AFX_INCLUDES
#if !defined(AFX_CTRLPANELWND_H__7D3224A4_8380_4D4C_AA8D_702B5E81F365__INCLUDED_)
#define AFX_CTRLPANELWND_H__7D3224A4_8380_4D4C_AA8D_702B5E81F365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlPanelWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCtrlPanelWnd window
#define WM_USER_CAPTURE_STATUS			(WM_APP + 5)
#define MSGPARAM_CAPTURE_TRYSTART		1
#define MSGPARAM_CAPTURE_TRYSTOP		2
#define MSGPARAM_CAPTURE_TRYPAUSE		3
#define MSGPARAM_CAPTURE_TRYRESUME		4

#define WM_USER_CAPTURE_ERROR			(WM_APP + 10)
#define MSGERROR_DEVICE_LOST			1			// DEVICE Be Removed!!
#define MSGERROR_GRAPH_ABORT			2			// Graph Abort!! (for Many Condition)

#define WM_ACCEPT_ERR					(WM_APP + 169)
#define WM_GETMSG						(WM_APP + 238)
#define WM_NEXT_SLIDE			(WM_APP + 634)
#define WM_PREVIOUS_SLIDE		(WM_APP + 635)
#define WM_FIRST_SLIDE			(WM_APP + 636)
#define WM_LAST_SLIDE			(WM_APP + 637)
#define WM_END_SLIDE			(WM_APP + 638)
#define WM_LBTN_DOWN			(WM_APP + 639)
#define WM_MOUSEMOVE_EX			(WM_APP + 640)
#define WM_LBUTTONUP_EX			(WM_APP + 641)
#define WM_GETUP				(WM_APP + 902)
#define WM_KEY_PRESSED			(WM_APP + 911)
#define WM_USER_CountDownOver_TIMER			(WM_APP + 912)

#define constBtnWidth		18
#define constSmallGap		8//2
#define constMediumGap		6
#define constLargeGap		9
#define constCaptionWidth	12
#define constEditWidth		36//28->40
#define constTimeWidth		64//50->80
#define constPanelWidth		320
#define constPreviewWidth	300
#define constPreviewHeight	200
#define constMargin 10

#include "..\ComCapture\ComCapture\ComCapture_i.h"
//#include "..\MediaProfile\MediaProfile.h"
#include "..\MediaKit\MediaKit.h"
#include "VolumeBar.h"
#include "VolumeBarTesterDlg.h"
#include  "DLG_COUNT_DOWN.h"
#include	"WaitDlgPic.h"		//add 2013/11/08 kagata BugNo.35

typedef struct
{
	char	szPath[MAX_PATH];
} IPCStruct;

typedef enum 
{
	EASY_IDLE = 0,
	EASY_PREVIEW = 1,
	EASY_CAPTURE = 2,
} EASYSTATUS;

typedef enum 
{
	RECORDING = 0,
	PAUSED = 1,
	STOPPED = 2,
} CAPTURE_STATE;

class CPageMedia : public CObject
{
public:
	int m_nPageNo;
	CObArray* m_pRects;
};

//interface _PPTUtil;
class CBmpBtn;
class CNumberEdit;
class C3DStatic;
class CBmpStatic;
class CPreviewWnd;
//class CPPT9Ctrl;
class CPPT15Ctrl;		// add itoh 2013/07/11 PPT2013
class CWaitDlgPic;		//add 2013/11/08 kagata BugNo.35
class CHtmlWnd;
class CFreedraw;
class CCtrlPanelWnd : public CWnd //CDialog
{
// Construction
public:
	CCtrlPanelWnd();

// Attributes
public:
   CVolumeBar	m_VolumeHorizontal;
   CVolumeBarTesterDlg* m_dlgVolumeBAr;
   CDLG_COUNT_DOWN* dlgclock;
 //  CToolTipCtrl     m_ToolTip;

 //               m_pMainWnd;
  // CSlideBarCtrl m_VolumeHorizontal;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlPanelWnd)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
 	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCtrlPanelWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCtrlPanelWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterPressed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClosePreview(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnUserStatusHandler(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserErrorHandler(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAcceptError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNextSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPreviousSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFirstSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLastSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndSlide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBtnDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetAudioPreview(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetAudioPreview(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseMoveEx(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBtnUpEx(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyPressed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_USER_CountDownOver_TIMER(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  
public:
	CString m_strPath;

	CString m_strMediaPath;
	CString m_strHTMLPath;
	CString	m_strRootPath;
	CString	m_strScreenProfilePath;
	BOOL	m_bCaptureScreenMode;
	BOOL	m_bRecPath;
	BOOL	m_bWithScreenProfileIni;
    BOOL    m_hasVideo;
	//CScreen2Video	m_Screen2Video1;
	int		ChkPPTver();

protected:
	CString GetTimeStamp();
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

	CSize	GetBmpDim(UINT nID);
	void	OnRec();
	
	void	OnStop();
	void	OnPause();
	void	OnView();
	void	OnPreview();
	void	OnClose();
	void	OnFirst();
	void	OnLast();
	void	OnNext();
	void	OnPrev();
	BOOL	OpenPPTFile(CString strPath);

	void    PPTOpen(); 
	void	PPTClose(BOOL bQuit); 
	void	PPTGoto(long nNo); 
	void	PPTNext(); 
	void	PPTPrev(BOOL bAddRecord = TRUE); 
	void	PPTLast(); 
	long	PPTTotalSlideNo();

	void	PPTSaveMediaPos(); 
	BOOL	TestConnected(); 

public:
	int				GetCurRecIndex();
	int				GetCurrentDemoIndex();
	void			ReloadNG();
	void			AddScript();
	void			WriteNGFile();
	long	PPTCurSlideNo();
	long	PPTCurAnimaNo();
	BOOL			StartCapture(CString strPath); 
	void			ClearStep();
	void			ReloadStep();
	BOOL			AttachVideo();
#ifndef _OLD_VER_
	void	SlideArea();
	CPoint	PointRecover(CPoint);
	BOOL DrawTemp(POINT, BOOL, BOOL);
	void moveLine(void);
	void moveRect(void);
	void moveFreeHand(void);
	void TestTime();
	BOOL	PointCheck(CPoint);
	void JSWrite(CStringArray*);
	BOOL	m_fFreehand;
	BOOL	m_fLine;
	BOOL	m_fRect;
	BOOL	m_fMark;
	BOOL	m_fClear;
	BOOL	m_dFreehand;
	BOOL	m_dLine;
	BOOL	m_dRect;
	BOOL	m_dMark;
	CRect	m_slidesize;
	CRect	m_Slidearea;
	CStringArray	m_strArrayDrawNew;
	double			m_dStartTime;
#endif
	static CString	m_strClass;
	static HHOOK  m_hHook;
	BOOL   m_bCouldRecord;
	static HINSTANCE m_hmodHook;
	CBmpBtn*		m_pBtnZoomIn;
	CBmpBtn*		m_pBtnZoomOut;
	CBmpBtn*		m_pBtnMaxToMin;
	CBmpBtn*		m_pBtnMinToMax;
	CBmpBtn*		m_pBtnClose;
	CBmpBtn*		m_pBtnCapture;
	CBmpBtn*		m_pBtnRecDemo;
	CBmpBtn*		m_pBtnStopDemo;
	CBmpBtn*		m_pBtnView;
	CBmpBtn*		m_pBtnNG;
	CBmpBtn*		m_pBtnDemo;
	CBmpBtn*		m_pBtnRec;
	CRect			m_rectBak;
	CBmpBtn*		m_pBtnPause;
	CBmpBtn*		m_pBtnStop;
	CBmpBtn*		m_pBtnFirst;
	CBmpBtn*		m_pBtnPrev;
	CBmpBtn*		m_pBtnNext;
	CBmpBtn*		m_pBtnLast;
	CBmpBtn*		m_pBtnGoto;
	CBmpBtn*		m_pBtnPreview;
	CBmpBtn*		m_pBtnHelp;
	CBmpBtn*		m_pBtnRect;
	CBmpBtn*		m_pBtnLine;
	CBmpBtn*		m_pBtnPointer;
	CBmpBtn*		m_pBtnFreeHand;
	CBmpBtn*		m_pBtnEraser;
	CBmpBtn*		m_pBtnPenSet;
	CBmpStatic*		m_pBmpLabel1;
	CBmpStatic*		m_pBmpLabel2;
	CBmpStatic*		m_pBmpLabel3;
	CBmpStatic*		m_pBmpLabel4;
	CBmpStatic*		m_pBmpLabel5;
	CBmpStatic*		m_pBmpLabel6;
	CBmpStatic*		m_pBmpStaticBlue;
	CBmpStatic*		m_pBmpStaticRed;
	CBmpStatic*		m_pBmpStaticOrange;
	CBmpStatic*		m_pBmpStaticYellow;
	CNumberEdit*	m_pPageNoEdit;
	C3DStatic*		m_pTimeStatic;
//	CPPT9Ctrl*		m_pPPT;
	CPPT15Ctrl*		m_pPPT;		// add itoh 2013/07/11 PPT2013
	int				m_nCloseBtnWidth;
	CFont*			m_pSysFont;
	CPreviewWnd*	m_pPreviewWnd;
	CAPTURE_STATE	m_nState;
	DWORD			m_nTickCount;
	DWORD			m_nLastTick;
	DWORD			m_nBaseMS;
	UINT			m_nTimerID;
	UINT			m_nTimerID2;
	UINT			m_nTimerID3;
    UINT			m_nTimerID4;
	int				m_nW;
	int				m_nH;

	EASYSTATUS		m_eStatus;
	IEasyCapture	*m_pEasyCapture;
	CStringArray	m_cstVideoArray, m_cstAudioArray;
	CStringArray	m_cstProfileNameArray, m_cstProfileDscrpArray;
	BOOL			m_bNoAcceptError;
	CStringArray	m_strArrayStep;
	CStringArray	m_strArraySpt;
	CStringArray	m_strArrayNG;
	CStringArray	m_strArrayTitle;
	long			m_nPreSlideNo;
	long			m_nPreSubSlideNo;
	BOOL			m_bTrackMove;
	CRect			m_rcDown;
    CPoint			m_ptDown;
	HCURSOR			m_hMoveCursor;
	CString			m_strPPTPath;
	CString			m_strType;
	CString			m_strParam1;
	CString			m_strParam2;
	int				m_nVideoIndex;
	int				m_nAudioIndex;
	int				m_nScreenAudioIndex;
	unsigned long	m_lTime;
	CObArray*		m_pPageMedias;
	BOOL			m_bPreviewAudioMute;
	int				m_nIndex;
	int				m_nDrawIndex;
	CString			m_strOp;
	BOOL			m_bOpen;
	int				m_nMinBtnEnd;
	int				m_nShortBtnEnd;
	int				m_nLongBtnEnd;
	int				m_nTop;
	int				m_nBottom;
	CString			m_strDemoFile;
	int				m_nDemoIndex;
	BOOL			m_bDisConnected;
	CStringArray	m_strArrayDemoStep;
	CStringArray	m_strArrayDraw;
	BOOL			m_bWarnLow;
	HWND			m_hWndPPT;
	HWND			m_hWndPane;
	CRect			m_rect;
	BOOL			m_bDragingRect;
	BOOL			m_bDragingLine;
	BOOL			m_bDragingPointer;
	CRect			m_rectOld;
	int				m_nWidth;
	int				m_color;
	CBitmap*		m_pPointerBMP;
	CSize			m_szPointer;
	CImageList*		m_pimageList;
	UINT			m_nID;
	BOOL			m_bAudioOnly;
	int				m_nPreDemoIndex;
	BOOL			m_bCaptureMode;
	CHtmlWnd*		m_pWndHTML;
	int				m_nHTMLCount;
	CStringArray	m_strArrayHTMLPath;
	BOOL			m_bUsePPT;
	BOOL			m_bUseHTML;
	BOOL			m_bFreeHand;
	int				m_nHTMLIndex;
	IWMXUtility*	m_pWMXUtility;			
	CFreedraw*		m_pFreedraw;
	long			m_lCurrTime;
	long			m_lStartTime;
	long			m_lCurrID;
	CString			m_strCurFile;
	int				m_nMaxIndex;
	BOOL			m_bNG;
	HANDLE			hGlobalWriteEvent; 
	HANDLE			m_hPlayHandle;
	HANDLE			m_hRecHandle;
	void    DoRecMost();

	BOOL			c_Flag;		//add 2013/9/20 kagata PPT2013
	CWaitDlgPic*		m_wBitmap;	//add 2013/11/08 kagata BugNo.35
	
protected:
	void			HTMLGoto(long nNo); 
	void			HTMLNext(); 
	void			HTMLPrev(); 
	void			GotoHTMLPage(int nIndex);
	void			LoadHTMLData();
	void			ResetButtons();
	BOOL			AudioOnly(); 
	CString			MakeupZero(double d);
	void			MoveToStepArray();
	void			OnDrawMark(BOOL bLine);
	void			RemoveAllPageMedias();
	void			UnSetHook();
	BOOL			SetHook();
	void			UnSetKBHook();
	BOOL			SetKBHook();
	void 			EnablePPTBtns(BOOL bFlag);
	BOOL			SetDevice();
	
	void			DetachVideo();
	BOOL			CreateInterface();
	void			ReleaseInterface();
	BOOL			StartPreview();
	void			StopPreview();

	void			StopCapture();
	BOOL			PreviewPrepare();
	BOOL			CapturePrepare(BSTR bstrFile);
	CString			ConvertMSToStr(DWORD dwtime, BOOL bPoint);
	BOOL			GetWidthHeight(long& nWidth, long& nHeight);
	BOOL			CreatePPTCtrl();
	void			ClosePPTCtrl(); 
	void			BringToTop(); 
	void			SetSlideNo(); 
	void			RedrawCaption();
	BOOL			WriteToFile(CString strFileName, CStringArray& strArray);
	void			InitStepArray();
	void			InitNGArray();
	void			InitZoomArray();
	void			AddPPTRecord();
	void			WriteStepFile();
	void			WriteSptFile();
	void ReloadSpt();
	
	int				GetTaskbarPos();
	void			EnableTaskBar(BOOL bShow, BOOL bForce = FALSE);
	void			GetDevice(CString& strType, CString& strParam1, CString& strParam2);
	BOOL			EnumDevice(CAPTUREDEVICETYPE eType, CStringArray& strArray);
	BOOL			GetDeviceIndex(); 
	int				GetIndex(CStringArray& strArray, CString str); 
	BOOL			UpdateDeviceTable();
	BOOL			OpenDeviceByType(CAPTURESESSION eSession);
	BOOL			OpenDevice(CString strType);
	BOOL			CloseDevice();
	BOOL			Start(); 
	BOOL			Stop(); 
	BOOL			Pause(); 
	BOOL			Resume(); 
	void			CommonErrMsg(CString strFunc);
	void			CommonFuncErrMsg(CString strFunc);
	BOOL			InitForVideoCapture();
	BOOL			InitForScreenCapture();
	BOOL			CleanUpForCapture();
	BOOL			LoadMediaFile();
	BOOL			IsPointInMedia(CPoint pt);
	void			OnZoomIn();
	void			OnZoomOut();
	void			OnGoto(); 

	void			ChangeToRecMode(BOOL bFlag);
	void			ChangeToMaxMode(BOOL bFlag);
	void			AppBarQuerySetPos(UINT uEdge, LPRECT lprc, PAPPBARDATA pabd);
	void			ResetTaskbarPos();
	void			GetTaskBarRect(CRect& rc); 
	void			OnDemo(); 
	void			OnNG(); 
	void			OnCapture();
	void			ClosePreviewWnd();
	void			OnRecDemo();
	void			OnStopDemo();
	BOOL			CapturePrepareScreen(BSTR bstrFile);
	BOOL			StartCaptureScreen(CString strPath); 
	BOOL			OpenCapture(BOOL bScreen);
	BOOL			OpenScreenDevice();
	CString			GetDemoFile();

	void			OnAddDemo();
	void			AddDemoTag(); 
	CString			GetNextRecFile();

	BOOL			IsCaptureScreenMode();
	void			TryToReconnect(); 
	void			ShowHideTaskBar(BOOL bHide = FALSE);


	void			ClearNG();

	void			DrawRect(LPRECT lpRect, COLORREF color, int nWidth, BOOL bEx);
	void			DrawLine(LPRECT lpRect, COLORREF color, int nWidth, BOOL bEx);

	BOOL			IsRectMode();
	BOOL			IsLineMode();
	BOOL			IsPointerMode();
	void			GetPenProperty();
	void			ReloadBitmap();
	BOOL			GetLangBarRect(CRect& rc); 
	BOOL			OpenHTMLFile(CString strWebPath);
	BOOL			IsFreeHandMode();
	CString			GetNext2RecFile();
	BOOL			FindDevice();
	BOOL			FindDevice(CAPTUREDEVICETYPE eType);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLPANELWND_H__7D3224A4_8380_4D4C_AA8D_702B5E81F365__INCLUDED_)
