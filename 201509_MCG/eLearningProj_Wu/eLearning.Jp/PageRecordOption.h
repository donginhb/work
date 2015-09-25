#if !defined(AFX_PAGERECORDOPTION_H__BF368D6F_64DD_4F51_BFD3_E0B57DEB2628__INCLUDED_)
#define AFX_PAGERECORDOPTION_H__BF368D6F_64DD_4F51_BFD3_E0B57DEB2628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageRecordOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageRecordOption dialog

class CPageRecordOption : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageRecordOption)

// Construction
public:
	CPageRecordOption();
	~CPageRecordOption();

// Dialog Data
	//{{AFX_DATA(CPageRecordOption)
	enum { IDD = IDD_DIALOG_PAGE_RECORD_OPTION };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageRecordOption)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageRecordOption)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGERECORDOPTION_H__BF368D6F_64DD_4F51_BFD3_E0B57DEB2628__INCLUDED_)
