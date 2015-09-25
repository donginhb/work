#if !defined(AFX_DLGPENPROPERTY_H__73C453E5_140B_468D_A458_1A33B3041B40__INCLUDED_)
#define AFX_DLGPENPROPERTY_H__73C453E5_140B_468D_A458_1A33B3041B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPenProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPenProperty dialog

class CBmpStatic;
class CBmpBtn;
#include "..\elearning.jp\ColorButton.h"
#include "BMPCombo.h"
class CDlgPenProperty : public CDialog
{
// Construction
public:
	CDlgPenProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPenProperty)
	enum { IDD = IDD_DIALOG_PEN_PROPERTY };
	CColorButton	     m_cbPen;
	CBMPCombo			 m_cbPointer;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPenProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPenProperty)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CObArray		m_BMPArray;
	CStringArray	m_strArrayID;

protected:
	CBitmap* ConvertBMP(CBitmap* pBMP); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPENPROPERTY_H__73C453E5_140B_468D_A458_1A33B3041B40__INCLUDED_)
