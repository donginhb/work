#if !defined(AFX_DLGEDITMARK_H__6524D060_F6D7_11D4_B46F_0020AF3085DB__INCLUDED_)
#define AFX_DLGEDITMARK_H__6524D060_F6D7_11D4_B46F_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditMark.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEditMark dialog
class CColGrid;
class CEditAllowed;
class CDlgEditMark : public CDialog
{
// Construction
public:
	CDlgEditMark(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditMark)
	enum { IDD = IDD_DIALOG_EDIT_MARK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditMark)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditMark)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDelete();
	virtual void OnCancel();
	afx_msg LRESULT OnGrdUpdate(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CColGrid*			m_pSliderGrid;
	CColGrid*			m_pZoomGrid;
	CColGrid*			m_pDemoGrid;
	CColGrid*			m_pPracticeGrid;
	CColGrid*			m_pMarkGrid;
	CStringArray*		m_pstrArrayTime; 
	CStringArray*		m_pstrArrayHyper; 
	CStringArray*		m_pstrArrayStep;
	CStringArray*		m_pstrArrayCmd;
	CEditAllowed*		m_pEditSDelay;
	CEditAllowed*		m_pEditADelay;
	CUIntArray			m_deleteArrayDraw;

public:
	CString			m_strTimeMax;
//	BOOL			m_bDeleteAtPoint;
	int				m_nPrePage;
	int				m_nPreCell;

public:
	void SetArrayPtr(CStringArray* pstrArrayTime, 
				     CStringArray* pstrArrayHyper, 
				     CStringArray* pstrArrayStep,
					 CStringArray* pstrArrayCmd);

protected:
	CString MakeupZero(double d);
	CString MakeupZero(CString strD);
	void SkipLeftZero(CString& strTmp);
	CString SkipLeftZero(CString strTmp);
	void ChangeSel();
	void GetSlideRange(int nIndex, int& nLowBound, int& nUpBound); 
	int GetDataSize(CColGrid* pGrid);
	BOOL CheckTimeRange(CColGrid* pGrid);
	BOOL IsValidTime1(CString strTime);
	BOOL CheckTimeRange1(CColGrid* pGrid);
	BOOL SelfCheckTimeDup(CColGrid* pGrid); 
	int GetTabIndex(CColGrid* pGrid);
	int GetTimeIndex(CColGrid* pGrid);
	BOOL CheckTimeDupStr(CColGrid* pGrid, CString str1, int i); 
	BOOL CheckColNull(CColGrid* pGrid, int nCheckCol);
	BOOL CheckMarkGrid();
	CString MlsecToHour(CString strMlsec);
	CString HourToMlsec(CString strHour);
	CString GetTypeText(CString strCmd);
	CString GetStampIndex(CString strBmpId);
	CString GetBmpId(CString strStampIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITMARK_H__6524D060_F6D7_11D4_B46F_0020AF3085DB__INCLUDED_)
