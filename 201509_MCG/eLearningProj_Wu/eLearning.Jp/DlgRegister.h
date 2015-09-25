#if !defined(AFX_DLGREGISTER_H__18074AFF_7D1E_4995_9EB7_0C58BEA2D6B8__INCLUDED_)
#define AFX_DLGREGISTER_H__18074AFF_7D1E_4995_9EB7_0C58BEA2D6B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRegister.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister dialog
#include "gridctrl.h"
class CDlgRegister : public CDialog
{
// Construction
public:
	CDlgRegister(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRegister)
	enum { IDD = IDD_REGISTER };
	CGridEdit m_edSerial1;
	CGridEdit m_edSerial2;
	CGridEdit m_edSerial3;
	CGridEdit m_edSerial4;
	CGridEdit m_edSerial5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFailToRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString GetEncodedString(CString strInput);
	char GetDigit(int n);
	int GetValue(char ch);
	BOOL ValidSerialPart(CString strSerial, int nStart);
	BOOL ValidSerial(CString strSerial);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREGISTER_H__18074AFF_7D1E_4995_9EB7_0C58BEA2D6B8__INCLUDED_)
