#if !defined(AFX_EFFECTMANAGEWND_H__FAA5C981_2202_4253_8F51_E4CF3D3FB0D8__INCLUDED_)
#define AFX_EFFECTMANAGEWND_H__FAA5C981_2202_4253_8F51_E4CF3D3FB0D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectManageWnd.h : header file
//

#define WM_EFFECT_PRESSED		(WM_APP + 4892)
#define WM_FADE_IN_EFFECT		(WM_APP + 4893)
#define WM_FADE_OUT_EFFECT		(WM_APP + 4894)
#define WM_DELETE_EFFECT		(WM_APP + 4895)
#define WM_TRANSIT_EFFECT		(WM_APP + 4896)
#define WM_SET_BREAKPOINT		(WM_APP + 4897)

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

/////////////////////////////////////////////////////////////////////////////
// CEffectManageWnd window

class CEffectPoint : public CObject
{
public:
	CEffectPoint(){};
	virtual ~CEffectPoint(){};
	
	long		m_lTime;
	long		m_lDuration;
	CString		m_strParam;
	int			m_nType;
	int			m_nStatus;
	CString		m_strTip;
};

class CEffectManageWnd : public CWnd
{
// Construction
public:
	CEffectManageWnd();

// Attributes
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectManageWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEffectManageWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEffectManageWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString  m_strClass;

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);

protected:
	long			m_lLength;
	long			m_lTimeStart;
	int				m_nUnit;
	int				m_nPixelUnit;
	CObArray*		m_pObBreakPoint;		
	int				m_nCtrlEntered;
	BOOL			m_bLeaveTrack;
	int				m_nClickCtrl;
	BOOL			m_bCanRMenu;
	CToolTipCtrl*	m_pToolTip;

protected:
	int TimeToPos(long lTime);
	long PosToTime(int xPos);
	BOOL IsTimeVisible(long lTime);
	void DrawMarkAtTime(CDC* pDC, int nIndex);
	void LeaveTrack(); 
	CRect GetSegRect(int nIndex); 
	void EnterCtrl(int nCtrl);
	void LeaveCtrl(int nCtrl);
	int CtrlEntered(CPoint point);
	BOOL IsAnyPeriodDeleted(long lTimeStart, long lTimeEnd);
	void AddToolTip();
	CString GetTimeStr(long nPos);
	void GetStartEndTime(long lTime, long lDuration, int nType, long& lTimeStart, long& lTimeEnd);

public:
	void DeletePeriod(long lTimeStart, long lTimeEnd);
	void InsertPeriod(long lTimeInsert, long lDuration);
	void ResetTooltip();
	void SetPixelUnit(int nPixelUnit);
	void SetTimeUnit(int nTimeUnit);
	void SetTimeStart(long lTime);
	void SetLength(long lTimeLength);
	void Initialize();
	void SetBreakPoint(CDWordArray& dwArrayBreakPoint, long lTimeStart = -1, long lTimeEnd = -1);
	BOOL TimeOverlap(long lTime, long lDuration, int nIndexNotCare);
	BOOL AddSegment(long lTime, long lDuration, int nType, CString strParam = "", CString strTip = "");
	void SetSegArray(CStringArray& strArrayVideo);
	void GetSegString(CString& str);
	void DeletePeriodByIndex(int nIndex);
	void GetSegInfo(int nIndex, long& lTime, long& lDuration, CString& strPath, int& nType);
	void SetEmptyByndex(int nIndex);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTMANAGEWND_H__FAA5C981_2202_4253_8F51_E4CF3D3FB0D8__INCLUDED_)
