#if !defined(AFX_DLGGOTOSLIDE_H__5B5D6EC8_A692_468A_B2E6_BDEC415AB71D__INCLUDED_)
#define AFX_DLGGOTOSLIDE_H__5B5D6EC8_A692_468A_B2E6_BDEC415AB71D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGotoSlide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGotoSlide dialog
class CColGrid;
class CDlgGotoSlide : public CDialog
{
// Construction
public:
	CDlgGotoSlide(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGotoSlide)
	enum { IDD = IDD_GOTO_SLIDE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGotoSlide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGotoSlide)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnHyperLink(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CColGrid* m_pGrid;

protected:
	void LoadHyperLink(); 

public:
	CStringArray* m_pstrArrayTitle;
	int	m_nIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGOTOSLIDE_H__5B5D6EC8_A692_468A_B2E6_BDEC415AB71D__INCLUDED_)
