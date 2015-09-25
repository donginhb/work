#if !defined(AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_1)
#define AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelDevice dialog
#include "..\ComCapture\ComCapture\ComCapture_i.h"
//add by buxiangyi
class CDataBrowseWnd1;
///////////////////////////////
class CDevice : public CDialog
{
	// Construction
public:
	CDevice(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgSelDevice)
	enum { IDD = IDD_DIALOG_DEVICE_SET };
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelDevice)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelDevice)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	//add by buxiangyi
	CDataBrowseWnd1*		m_pAttribSelGrid;
	CDataBrowseWnd1*		m_pAttribSelGrid1;
	BOOL	m_bNoAsk;

	//////////////////////////////////////
public:
	CString				m_strIniPath;
	CRect               rect;
	CRect               rect1;
	CPoint              mousepoint;
	//add by buxiangyi
	IEasyCapture*		m_pEasyCapture;
	BOOL				 m_bWithCam;
	BOOL				m_bWithAudio;
	BOOL				m_Initialization;
	BOOL                 m_Cameraflag;
	BOOL                 m_ScreenCapture;
	BOOL                 m_NewCourse;
	//////////////////////////////////////////////////////
protected:
	void				OnSelectFile(); 
	BOOL				ReloadDev(); 
	void				RadioCam();
	void				SetComboContent(UINT nID, CStringArray& strArray); 

public:
	afx_msg void OnBnClickedButtonRename();
	afx_msg void OnClickedButtonRename1();
	afx_msg void OnClickedButtonDel();
	afx_msg void OnClickedButtonDel1();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRadioNoVideo();
	afx_msg void OnClickedButtonReload();
	afx_msg void OnSelchangeComboAudio();
	afx_msg void OnSelchangeComboCamVideo();
	afx_msg void OnSelchangeComboScreenAudio();
	afx_msg void OnSelchangeComboAudioFrame();
	afx_msg void OnSelchangeComboCamVideoFrame();
	afx_msg void OnSelchangeComboScreenAudioFrame();
	afx_msg void OnRadioCam();
	afx_msg void OnRadioIP();
public:
	void SetNoAsk(BOOL b) { m_bNoAsk = b; }
	
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_1)
