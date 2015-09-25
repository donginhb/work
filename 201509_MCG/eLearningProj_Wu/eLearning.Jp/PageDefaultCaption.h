#if !defined(AFX_PAGEDEFAULTCAPTION_H__B26234F2_2329_4F0D_88F9_342677BDABD9__INCLUDED_)
#define AFX_PAGEDEFAULTCAPTION_H__B26234F2_2329_4F0D_88F9_342677BDABD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultCaption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultCaption dialog
#include "ColorButton.h"
#include "FontCombo.h"

class CPageDefaultCaption : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDefaultCaption)

// Construction
public:
	CPageDefaultCaption();
	~CPageDefaultCaption();

// Dialog Data
	//{{AFX_DATA(CPageDefaultCaption)
	enum { IDD = IDD_DIALOG_CAPTION_DEFAULT };
	CColorButton		 m_cbFG;
	CColorButton	     m_cbBG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDefaultCaption)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDefaultCaption)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioOpaque();
	afx_msg void OnRadioTransparent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CFontCombo		m_cboFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEFAULTCAPTION_H__B26234F2_2329_4F0D_88F9_342677BDABD9__INCLUDED_)
