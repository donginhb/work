#if !defined(AFX_KEYFRAMEPANEL_H__9FF8185C_AB49_45C4_8D98_15B3403D2F82__INCLUDED_)
#define AFX_KEYFRAMEPANEL_H__9FF8185C_AB49_45C4_8D98_15B3403D2F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyFramePanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyFramePanel window

#define WM_KEY_FRAME_PRESSED		(WM_APP + 544)
#define	WM_RBUTTON_CLICKED			(WM_APP + 558)
#define WM_KEY_FRAME_MOVED			(WM_APP + 559)
#define WM_KEY_FRAME_PRESSING		(WM_APP + 560)
#define WM_REGEN_TAG				(WM_APP + 561)
#define WM_CP_ACTION				(WM_APP + 562)
#define WM_KEY_FRAME_TOBE_MOVED		(WM_APP + 563)

typedef enum tagKEY_TYPE
{
	KEY_RED = 0,
	KEY_GREEN = 1,
	KEY_BLUE = 2,
	KEY_SQ_IN = 3,
	KEY_SQ_OUT = 4,
	KEY_DEMO = 5,
	KEY_PRACTICE = 6,
	KEY_DRAW = 7
}
KEY_TYPE;

typedef enum tagIMAGE_POS
{
	POS_TOP = 0,
	POS_CENTER = 1,
	POS_BOTTOM = 2,
}
IMAGE_POS;

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

class CToolTipWnd;
class CImageObj : public CObject
{
public:
	CImageObj(){};
	~CImageObj(){};

	UINT		m_nID;
	int			m_nWidth;
	int			m_nHeight;
	IMAGE_POS	m_nPos;
	CImageList	m_lstImage;
};

class CKeyFrameObj : public CObject
{
public:		
	CKeyFrameObj(){m_nStatus = constBtnNormal;};
	~CKeyFrameObj(){};

	CString		m_strTitle;
	CString		m_strTag;
	long		m_lTime;
	KEY_TYPE	m_nType;
	int			m_nStatus;
	CString		m_strCmd;
	CRect		m_rcPos;
};

class CKeyFramePanel : public CWnd
{
// Construction
public:
	CKeyFramePanel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyFramePanel)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyFramePanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyFramePanel)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString  m_strClass;
	CObArray*		m_pImageArray;
	CObArray*		m_pKeyFrameArray;
	int				m_nCtrlEntered;
	BOOL			m_bTracking;
	BOOL			m_bLeaveTrack;
	CToolTipCtrl*	m_pToolTip;
	CToolTipWnd*	m_pTooltipWnd;
	CRect			m_rcChannel;
	CRect			m_rectTracker;
	CPoint			m_ptOffset;
	BOOL			m_bFirstMove;
	int				m_nClickCtrl;
	CPoint			m_ptPrePos;
	long			m_lLength;
	long			m_lTimeStart;
	int				m_nUnit;
	int				m_nPixelUnit;

public:	
	BOOL			m_bDeletable;
	BOOL			m_bEditable;
	BOOL			m_bCanRMenu;
	CString			m_slidePath;

protected:
	BOOL CheckTimeDup(long lTime);
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

	void DrawChannel(CDC *pDC, const CRect &rect);
	void LoadImageList();
	void DrawKeyFrame(CDC *pDC);
	void SetKeyPos(int nIndex);
	CRect GetPosRect(long nTime, int nType);
	int CtrlEntered(CPoint point);
	void EnterCtrl(int nCtrl);
	void LeaveCtrl(int nCtrl);
	void LeaveTrack(); 
	void AddKeyFrame(CString strTitle, CString strTag, long nPos, 
					 KEY_TYPE nType, CString strCmd);
	void AddImageByID(UINT nID, IMAGE_POS nPos);
	UINT MapTypeToID(int nType);
	CImageObj* GetImageObjByType(int nType);
	void OnInvertTracker(const CRect& rect);
	void ShowToolTip(long nPos);
	CString GetTimeStr(long nPos);
	int TimeToPos(long lTime);
	long PosToTime(int xPos);

public:
	void SetPixelUnit(int nPixelUnit);
	void SetTimeUnit(int nTimeUnit);
	void SetTimeStart(long lTime);

	void ResetTooltip();
	void AddToolTip();
	void RemoveFrameObj(int nIndex);
	void SetLength(long lTimeLength) { m_lLength = lTimeLength;};
	void Initialize();
	void RemoveAllKeyFrame(); 
	void SetRectChannel(CRect rcChannel);
	void SetKeyFrame(CStringArray& strArrayMark, CStringArray& strArrayScript, 
					 CStringArray& strArrayCmd, CStringArray& strArrayTime);
	void GetKeyFrameData(CStringArray& strArrayTimeMarker, 
						 CStringArray& strArrayMarker, 
						 CStringArray& strArrayTimeScript,
						 CStringArray& strArrayScript,
						 CStringArray& strArrayCmd);
	void GetKeyFrameData(CStringArray& strArrayTime, 
						 CStringArray& strArrayMarker, 
						 CStringArray& strArrayScript,
						 CStringArray& strArrayCmd);
	CKeyFrameObj* GetFrameObj(int nIndex);
	int GetFrameCount() { return m_pKeyFrameArray->GetSize();}
	void DeletePeriod(long lTimeStart, long lTimeEnd);
	void InsertPeriod(long lTimeStart, long lDuration);
	void TagChanged();
	void GetFrameInfo(int nIndex, CString& strTitle, long& lTime, CString& strCmd, CString& strStep);
	void SetSlidePath(CString slidePath) { m_slidePath = slidePath;};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYFRAMEPANEL_H__9FF8185C_AB49_45C4_8D98_15B3403D2F82__INCLUDED_)
