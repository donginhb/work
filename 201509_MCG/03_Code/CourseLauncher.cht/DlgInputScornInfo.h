#if !defined(AFX_DLGINPUTSCORNINFO_H__F1D72EB3_3535_481B_B1E6_1A276FF6CC61__INCLUDED_)
#define AFX_DLGINPUTSCORNINFO_H__F1D72EB3_3535_481B_B1E6_1A276FF6CC61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputScornInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputScornInfo dialog

class CDlgInputScornInfo : public CDialog
{
// Construction
public:
	CDlgInputScornInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputScornInfo)
	enum { IDD = IDD_DIALOG_INPUT_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputScornInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputScornInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListCourse();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnCancel2();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStringArray m_strArrayCourse;

protected:
	CString m_strPreSel;

protected:
	void WriteData(CString strCourse);
	/************ leu:add begin ********************/
	void EncodeScormStr(CString InStr, char *OutText);
    void DecodeScormStr(CString InStr, char *OutText);
	/************ leu:add end **********************/
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTSCORNINFO_H__F1D72EB3_3535_481B_B1E6_1A276FF6CC61__INCLUDED_)
