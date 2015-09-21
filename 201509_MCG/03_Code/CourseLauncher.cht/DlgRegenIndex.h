#if !defined(AFX_DLGREGENINDEX_H__C2242C9C_0A35_4E43_AC48_145DFF738243__INCLUDED_)
#define AFX_DLGREGENINDEX_H__C2242C9C_0A35_4E43_AC48_145DFF738243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRegenIndex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRegenIndex dialog

#define ID_OVERWRITE	12000
#define ID_RESERVE	    12001
#define ID_SELECT	    12002

class CDlgRegenIndex : public CDialog
{
// Construction
public:
	CDlgRegenIndex(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRegenIndex)
	enum { IDD = IDD_DIALOG_REGEN_INDEX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRegenIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRegenIndex)
	afx_msg void OnButtonOverwrite();
	afx_msg void OnButtonReserve();
	afx_msg void OnButtonSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREGENINDEX_H__C2242C9C_0A35_4E43_AC48_145DFF738243__INCLUDED_)
