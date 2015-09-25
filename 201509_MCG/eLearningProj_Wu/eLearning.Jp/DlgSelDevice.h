#if !defined(AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_)
#define AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelDevice dialog
#include "..\ComCapture\ComCapture\ComCapture_i.h"
class CDlgSelDevice : public CDialog
{
// Construction
public:
	CDlgSelDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelDevice)
	enum { IDD = IDD_DIALOG_DEVICE };
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
	afx_msg void OnRadioCam();
	afx_msg void OnRadioNoVideo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonReload();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboAudio();
	afx_msg void OnSelchangeComboCamVideo();
	afx_msg void OnSelchangeComboScreenAudio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	IEasyCapture* m_pEasyCapture;
	BOOL	m_bNoAsk;
	BOOL m_bWithCam;
	BOOL m_bWithAudio;

protected:
	void SetComboContent(UINT nID, CStringArray& strArray); 
	BOOL ReloadDev(); 
	void RadioCam(); 

public:
	void SetNoAsk(BOOL b) { m_bNoAsk = b; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPROPERTY_H__ED62C9F1_675E_4624_A9DA_6B7EC8C307DB__INCLUDED_)
