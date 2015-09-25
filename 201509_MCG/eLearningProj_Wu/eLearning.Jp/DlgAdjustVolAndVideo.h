#if !defined(AFX_DLGADJUSTVOLANDVIDEO_H__8EABE52D_59C3_4C08_B0C9_EE1E0B7B1A3E__INCLUDED_)
#define AFX_DLGADJUSTVOLANDVIDEO_H__8EABE52D_59C3_4C08_B0C9_EE1E0B7B1A3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAdjustVolAndVideo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustVolAndVideo dialog

class CDlgAdjustVolAndVideo : public CDialog
{
// Construction
public:
	CDlgAdjustVolAndVideo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAdjustVolAndVideo)
	enum { IDD = IDD_DIALOG_ADJUST };
	CSliderCtrl	m_sliderVolume;
	CSliderCtrl	m_sliderLight;
	CSliderCtrl	m_sliderContrast;
	CStatic m_volume;
	CStatic m_light;
	CStatic m_contrast;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAdjustVolAndVideo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAdjustVolAndVideo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nContrastValue;
	int m_nLightValue;
	int m_nVolumeValue;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADJUSTVOLANDVIDEO_H__8EABE52D_59C3_4C08_B0C9_EE1E0B7B1A3E__INCLUDED_)
