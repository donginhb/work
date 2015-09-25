#if !defined(AFX_COURSESHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_)
#define AFX_COURSESHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetDefaultLayout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetDefaultLayout
class CPageDefaultOption;
class CPageDefaultImage;
class CPageDefaultOutlook;
class CRelatedInformation;
#include "basicsheet.h"
class CCourseSheetDefaultLayout : public CBasicSheet
{
	DECLARE_DYNAMIC(CCourseSheetDefaultLayout)

	// Construction
public:
	CCourseSheetDefaultLayout(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCourseSheetDefaultLayout(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

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
	virtual ~CCourseSheetDefaultLayout();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetDefaultLayout)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPageDefaultOption *m_pPageOption;
	CPageDefaultImage *m_pPageImage;
	CPageDefaultOutlook *m_pPageOutlook;
	CRelatedInformation *m_pPageRelate;

protected:
	void Init();
	BOOL ContinueModal();

public:
	int m_nCurSel;
	BOOL m_CourseSheetDefaultFlag;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COURSESHEETDEFAULTLAYOUT_H__0F41E64A_8008_4E1A_9724_29AE320A1973__INCLUDED_)
