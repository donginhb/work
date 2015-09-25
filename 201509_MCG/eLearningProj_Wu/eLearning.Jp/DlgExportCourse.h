#if !defined(AFX_DLGEXPORTCOURSE_H__B79C3DA2_BC33_4EEF_8065_944800C0B13B__INCLUDED_)
#define AFX_DLGEXPORTCOURSE_H__B79C3DA2_BC33_4EEF_8065_944800C0B13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportCourse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportCourse dialog
class CBmpBtn;
class CDataBrowseWnd;
class CDlgExportCourse : public CDialog
{
// Construction
public:
	CDlgExportCourse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExportCourse)
	enum { IDD = IDD_DIALOG_EXPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportCourse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportCourse)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioAudioOnly();
	afx_msg void OnRadioFreq();
	afx_msg void OnRadioOriginal();
	afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioDefault();
	afx_msg void OnRadioScreen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SelDir();
	void CheckData();
	BOOL GetValue(CStringArray& strArrayData, CString strProfileName, int& nVideoBand, 
		 		  int& nFPS, int& nKeyFrame, int& nVideoQuality, int& nAudioBand, 
				  int& nAudioFreq, CString& strMono);

protected:
	CBmpBtn* m_pBtnOpen;
	CDataBrowseWnd* m_pAttribSelGrid;
	CString	m_strPathDefault;
	CString	m_strPathScreen;

public:
	CString m_strFolder;
	int		m_nType;

	long m_lScreenAudioCodecID;

	int m_nVideoBand;
	int m_nFPS;
	int m_nKeyFrame;
	int m_nVideoQuality;
	int m_nAudioBand;
	int m_nAudioFreq;
	CString m_strMono;

	int m_nDemoVideoBand;
	int m_nDemoFPS;
	int m_nDemoKeyFrame;
	int m_nDemoVideoQuality;
	int m_nDemoAudioBand;
	int m_nDemoAudioFreq;
	CString m_strDemoMono;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTCOURSE_H__B79C3DA2_BC33_4EEF_8065_944800C0B13B__INCLUDED_)
