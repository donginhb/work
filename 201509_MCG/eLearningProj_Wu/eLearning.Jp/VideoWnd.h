#if !defined(AFX_VIDEOWND_H__19DD25B9_5C50_4CB7_9A03_D74FBC52D6DF__INCLUDED_)
#define AFX_VIDEOWND_H__19DD25B9_5C50_4CB7_9A03_D74FBC52D6DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoWnd.h : header file
//

#define WM_GET_POINTER_TIME (WM_APP + 2394)
#define WM_PERIOD_CLICKED   (WM_APP + 2395)
#define WM_PERIOD_UNINSERT  (WM_APP + 2396)
#define WM_PERIOD_UNDELETE  (WM_APP + 2397)

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

class CTimeSeg : public CObject
{
public:
	CTimeSeg(){};
	virtual ~CTimeSeg(){};

public:	
	CString m_strFileName;
	long	m_lLogicalTimeStart;
	long	m_lLogicalTimeEnd;
	long	m_lPhysicalTimeStart;
	long	m_lPhysicalTimeEnd;
	int		m_nStatus;
};

/////////////////////////////////////////////////////////////////////////////
// CVideoWnd window
class CPointerWnd;
class CToolTipWnd;
class CVideoWnd : public CWnd
{
// Construction
public:
	CVideoWnd();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVideoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVideoWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPointerDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPointerUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPointerMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static CString  m_strClass;
	CSize			m_csPointer;
	CPointerWnd*	m_pPointer;
	CToolTipWnd*	m_pToolTip;
	long			m_lTimePointer;
	long			m_lTimeStart;
	int				m_nUnit;
	int				m_nPixelUnit;
	long			m_lLength;
	CPoint			m_ptOffset;
	long			m_lTimeSelStart;
	long			m_lTimeSelEnd;
	CObArray*		m_pTimeSeq;
	CObArray*		m_pFileSeq;
	CObArray*		m_pDelSeq;
	CDWordArray		m_dwArrayDel;
	BOOL			m_bLeaveTrack;
	int				m_nClickedIndex;
	int				m_nClickedDel;
	int				m_nCtrlEntered;
	int				m_nDelEntered;

public:
	void SetLength(long lTimeLength);
	void Initialize();
	int	TimeToPos(long lTime);
	long PosToTime(int xPos);
	void HidePointer();
	void SetPointerTime(long lTime);
	void SetPixelUnit(int nPixelUnit);
	void SetTimeUnit(int nTimeUnit);
	void SetTimeStart(long lTime);
	void DeletePeriod(long lTimeStart, long lTimeEnd);
	void ReplacePeriod(long lTimeSelStart, long lTimeSelEnd, CString strFileName, long lDuration);
	void GetSelRange(long& lTimeStart, long& lTimeEnd) {lTimeStart = m_lTimeSelStart; lTimeEnd = m_lTimeSelEnd;};
	BOOL GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName);
	BOOL GetTimeByVideoPos(long& lTime, long lVideoPos, CString strFileName);
	BOOL GetVideoPosNextToTime(long lTime, long& lVideoPos, CString& strFileName);
	long GetPointerTime() {return m_lTimePointer;};
	void GetSegString(CString& str, CString strDefault, BOOL bConvDefault, CString strSubPath);
	void SetSegArray(CStringArray& strArrayVideo, CString strSubPath);
	void SetNewSegArray();
	BOOL IsDel(int nIndex);
	void EnterDel(int nCtrl);
	BOOL UnDeletePeriod(long lTime, long& lLenAdded);
	void SetNoSelectAndRepaint();
	void GetBreakPoint(CDWordArray& dwArrayBreakPoint);
	BOOL GetBoundaryFiles(long lTime, CString& strPathPrev, long& lTimePrev, CString& strPathNext, long& lTimeNext);

protected:
	CRect GetFileSeqRect(int nIndex);
	void MergeNode();
	void ArrangeNodeDelete(long lTimeSelStart, long lTimeSelEnd);
	void ArrangeNodeInsert(long lTimeInsert, long lDuration, CString strFilePath);
	void SetNoSelect();
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
	BOOL IsPointerVisible();
	void SetPointerPos();
	void MovePointer(int nX, int nY);
	CString GetTimeStr(long nPos);
	void ShowToolTip(long nPos);
	void DrawSelRect(CRect& rect, CDC* pdc);
	HBITMAP CreateDitherBitmap();
	BOOL IsTimeVisible(long lTime);
	void LeaveTrack(); 
	CRect GetBoundaryRect(int nIndex);
	CRect GetDelRect(int nIndex);
	CRect GetTimeRect(long lTime);
	void ReCalcFileSeq();
	int CtrlEntered(CPoint point);
	void LeaveCtrl(int nCtrl);
	void EnterCtrl(int nCtrl);
	int DelEntered(CPoint point);
	void LeaveDel(int nCtrl);
	void DrawThumbnails(CDC *pDC);

public:
	long			m_lTimeCurrent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOWND_H__19DD25B9_5C50_4CB7_9A03_D74FBC52D6DF__INCLUDED_)
