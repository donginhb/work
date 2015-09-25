#if !defined(AFX_DATABROWSEWND_H__C9DE2DC8_DB98_426E_BAB7_847544316833__INCLUDED_1)
#define AFX_DATABROWSEWND_H__C9DE2DC8_DB98_426E_BAB7_847544316833__INCLUDED_1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataBrowseWnd.h : header file
//

#define WM_ROW_SELECTED (WM_APP + 5745)
#define WM_ROW_APPENDED (WM_APP + 5746)

/////////////////////////////////////////////////////////////////////////////
// CDataBrowseWnd window
class CColGrid;

class CDataBrowseWnd1 : public CWnd
{
	// Construction
public:
	CDataBrowseWnd1();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataBrowseWnd)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd); 
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CDataBrowseWnd1();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataBrowseWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGrdRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrdComboSelChange(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:	
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
		UINT nID, CCreateContext* pContext);
	void CheckVideoBand();
	int GetUntitledIndex();
	void GetAvailFreqStr(int nK, CString& strFreq);
	void GetAvailMonoStr(int nK, int nFreq, CString& strMono);
	void SetComboFreq();
	void SetComboMono();
	void SelectRow(int nRow);
	void ClearDetail();
	CString GetCString(CString strpath ,int index, int flag);
	CString ReturnCString(CString str, CString str1,CString str2, CString str3, CString path);

protected:
	CObArray*	m_pRstArray;
	CString		m_strPath;
	CString		m_strDefVideoBand;
	CString		m_strDefFPS;
	CString		m_strDefKerFrame;
	CString		m_strDefVideoQuality;
	CString		m_strDefAudioBand;
	CString		m_strDefAudioFreq;
	CString		m_strDefMonoStereo;
	CString		m_strPrefix;
	CWnd*		m_pMSGWnd;

public:
	void EnableWindow(BOOL bEnable);
	BOOL SetProfileData(CString strPath, CString strSel, CWordArray* pColArray = NULL, CStringArray* pStrArray = NULL);
	void SetRestrictedData(CString strPath);
	void SetDefaultValue(CString strVB, CString strFPS, CString strKeyFrame, CString strVideoQuality,
		CString strAB, CString strAudioFreq, CString strMono);
	void SetUntitledPrefix(CString strPrefix){m_strPrefix = strPrefix;};
	BOOL SaveData(CString strPath = "\\\\\\");
	BOOL SelectRow(CString strSel);
	int GetSelectedRow();
	void DeleteSelectedRow();
	void GetProfileName(CStringArray& strArrayProfile);
	void ReplaceSelected(CString strProfileName);
	void GetSelectedData(CStringArray& strArraySelected);
	void SetMSGWindow(CWnd* pWnd) {m_pMSGWnd = pWnd;};
	long GetAuFixValue();

public:
	CFont*	m_pFont;
	BOOL    m_InfoTypeflag;
	BOOL          m_Cameraflag;
	CColGrid*	m_pGridBrowse;
	CColGrid*	m_pGridDetail;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABROWSEWND_H__C9DE2DC8_DB98_426E_BAB7_847544316833__INCLUDED_)
