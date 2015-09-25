#if !defined(AFX_SHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_)
#define AFX_SHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetDefaultLayout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout
class CPageDefaultOption;
class CPageDefaultLayout;
class CPageDefaultImage;
class CPageDefaultOutlook;
#include "basicsheet.h"
class CSheetDefaultLayout : public CBasicSheet
{
	DECLARE_DYNAMIC(CSheetDefaultLayout)

// Construction
public:
	CSheetDefaultLayout(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetDefaultLayout(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetDefaultLayout)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheetDefaultLayout();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetDefaultLayout)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPageDefaultOption *m_pPageOption;
	CPageDefaultLayout *m_pPageLayout;
	CPageDefaultImage *m_pPageImage;
	CPageDefaultOutlook *m_pPageOutlook;

protected:
	void Init();
	BOOL ContinueModal();

public:
	int m_nCurSel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_)
