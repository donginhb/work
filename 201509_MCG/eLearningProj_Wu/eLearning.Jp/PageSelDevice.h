#if !defined(AFX_PAGESELDEVICE_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_PAGESELDEVICE_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPage.h : header file
//

#include "NewWizPage.h"
#include "..\ComCapture\ComCapture\ComCapture_i.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSelDevice dialog
class CPageSelDevice : public CNewWizPage
{
// Construction
public:
	CPageSelDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSelDevice)
	enum { IDD = IDW_SELECT_DEVICE };
	//}}AFX_DATA
	
	CFont m_Font;
	CFont m_Font2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSelDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();
	void OnSetActive();
	BOOL OnKillActive();
	LRESULT OnWizardBack();

	// Generated message map functions
	//{{AFX_MSG(CPageSelDevice)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCam();
	afx_msg void OnRadioNoVideo();
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESELDEVICE_H__C7A07F0E_2EF0_11D4_9FA9_0030DB0011C6__INCLUDED_)
