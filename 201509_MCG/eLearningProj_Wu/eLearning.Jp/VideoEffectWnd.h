#if !defined(AFX_VIDEOEFFECTWND_H__7FA6AB4A_9C84_48DB_9BD1_8423D80A593E__INCLUDED_)
#define AFX_VIDEOEFFECTWND_H__7FA6AB4A_9C84_48DB_9BD1_8423D80A593E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoEffectWnd.h : header file
//

#define WM_PERIOD_TOBE_MOVED	(WM_APP + 2914)
#define WM_PERIOD_MOVED			(WM_APP + 2915)
#define WM_PERIOD_DELETE		(WM_APP + 2916)
#define WM_PERIOD_VIEW			(WM_APP + 2917)
#define WM_CLICK_VIDEOEFFECT	(WM_APP + 2918)
#define WM_PERIOD_MOVING		(WM_APP + 2919)
#define WM_PERIOD_EDIT			(WM_APP + 2920)

/////////////////////////////////////////////////////////////////////////////
// CVideoEffectWnd window

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
class CVideoEffectSeg : public CObject
{
public:
	CVideoEffectSeg(){};
	virtual ~CVideoEffectSeg(){};
	
	long		m_lTimeStart;
	long		m_lTimeEnd;
	CString		m_strParam;
	int			m_nType;
	int			m_nStatus;
	CString		m_strTip;
	COLORREF	m_crNormal;
	COLORREF	m_crHot;
};

class CVideoEffectWnd : public CWnd
{
// Construction
public:
	CVideoEffectWnd();

// Attributes
public:

// Operations
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoEffectWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVideoEffectWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVideoEffectWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString  m_strClass;

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	void OnInvertTracker(const CRect& rect);

protected:
	long			m_lLength;
	long			m_lTimeStart;
	int				m_nUnit;
	int				m_nPixelUnit;
	CObArray*		m_pSegArray;
	BOOL			m_bLeaveTrack;
	BOOL			m_bTracking;
	int				m_nCtrlEntered;
	CRect			m_rectTracker;
	CPoint			m_ptOffset;
	BOOL			m_bFirstMove;
	int				m_nClickCtrl;
	CPoint			m_ptPrePos;
	int				m_nDragType;
	BOOL			m_bCanRMenu;
	CToolTipCtrl*	m_pToolTip;
	CToolTipWnd*	m_pTooltipWnd;

public:
	BOOL			m_bCanREdit;
	BOOL			m_bCanRDelete;
	BOOL			m_bCanRView;
	COLORREF		m_crNormal;
	COLORREF		m_crHot;
	BOOL			m_bShowParamAsTip;

protected:
	int TimeToPos(long lTime);
	long PosToTime(int xPos);
	BOOL IsTimeVisible(long lTime);
	void LeaveTrack(); 
	CRect GetSegRect(int nIndex); 
	void EnterCtrl(int nCtrl);
	void LeaveCtrl(int nCtrl);
	int CtrlEntered(CPoint point, int* pnType = NULL);
	CString GetTimeStr(long nPos);
	void AddToolTip();
	void ShowToolTip(long lTimeStart, long lTimeEnd);

public:
	void SetPixelUnit(int nPixelUnit);
	void SetTimeUnit(int nTimeUnit);
	void SetTimeStart(long lTime);
	void SetLength(long lTimeLength) { m_lLength = lTimeLength;};
	void Initialize();
	BOOL AddSegment(long lTimeStart, long lTimeEnd, CString strParam, int nType, 
					CString strTip = "", COLORREF crNormal = RGB(181, 230, 29)/*RGB(0, 0, 192)*/, COLORREF crHot = RGB(255, 255, 0));
	void GetSegString(CString& str);
	void SetSegArray(CStringArray& strArrayVideo, int nType);
	BOOL TimeOverlap(long lTimeStart, long lTimeEnd, int nIndexNotCare);
	void DeletePeriodByIndex(int nIndex);
	void GetSegInfo(int nIndex, long& lTimeStart, long& lTimeEnd, CString& strPath, int& nType); 
	void DeletePeriod(long lTimeStart, long lTimeEnd);
	void InsertPeriod(long lTimeStart, long lDuration);
	BOOL IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd);
	void ResetTooltip();
	void SetSegTip(int nIndex, CString strTip);
	int GetSegCount() {return m_pSegArray->GetSize();};
	void SetSegColor(int nIndex, COLORREF crNormal, COLORREF crHot); 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOEFFECTWND_H__7FA6AB4A_9C84_48DB_9BD1_8423D80A593E__INCLUDED_)
