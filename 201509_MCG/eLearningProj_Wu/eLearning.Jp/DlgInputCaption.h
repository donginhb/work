#if !defined(AFX_DLGINPUTCAPTION_H__1EDC1C47_A485_4C80_99BA_B85797BC0D5E__INCLUDED_)
#define AFX_DLGINPUTCAPTION_H__1EDC1C47_A485_4C80_99BA_B85797BC0D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputCaption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCaption dialog

#include "ColorButton.h"
#include "FontCombo.h"

class CDlgInputCaption : public CDialog
{
// Construction
public:
	CDlgInputCaption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputCaption)
	enum { IDD = IDD_DIALOG_INPUT_CAPTION };
	CColorButton		 m_cbFG;
	CColorButton	     m_cbBG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputCaption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputCaption)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioTransparent();
	afx_msg void OnRadioOpaque();
	afx_msg void OnApply();
	afx_msg void OnPaint();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnCheckBold();
	afx_msg void OnCheckItalic();
	afx_msg void OnCheckUnderline();
	afx_msg void OnButtonColorFg();
	afx_msg void OnButtonColorBg();
	afx_msg void OnSelchangeComboFont();
	afx_msg void OnSelchangeComboFontSize();
	afx_msg LRESULT OnColorSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontChanged(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCaption;
	CString m_strIniPath;
	CString m_strSection;

	CString	m_strFontName;
	CString	m_strFontSize;
	CString m_strItalic;
	CString m_strBold;
	CString m_strUnderline;
	CString m_strPosition;
	CString m_strHIndent;
	CString m_strVIndent;
	CString m_strBkMode;
	CString m_strTextColor;
	CString m_strBkColor;

	CString m_strCaptionBak;
	CString m_strIniPathBak;
	CString m_strSectionBak;
	
	CString	m_strFontNameBak;
	CString	m_strFontSizeBak;
	CString m_strItalicBak;
	CString m_strBoldBak;
	CString m_strUnderlineBak;
	CString m_strPositionBak;
	CString m_strHIndentBak;
	CString m_strVIndentBak;
	CString m_strBkModeBak;
	CString m_strTextColorBak;
	CString m_strBkColorBak;

protected:
	BOOL Apply(); 
	BOOL ApplyBak(); 
	void RedrawText(); 

protected:
	CFontCombo		m_cboFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTCAPTION_H__1EDC1C47_A485_4C80_99BA_B85797BC0D5E__INCLUDED_)
