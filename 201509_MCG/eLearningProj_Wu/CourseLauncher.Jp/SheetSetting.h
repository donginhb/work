#if !defined(AFX_SHEETSETTING_H__2E75862A_DAE3_4032_A9B0_3605F6530265__INCLUDED_)
#define AFX_SHEETSETTING_H__2E75862A_DAE3_4032_A9B0_3605F6530265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetSetting

#include <afxdlgs.h>

class CPageWebServer;
class CPageMediaServer;
class CSheetSetting : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheetSetting)

// Construction
public:
	CSheetSetting(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetSetting(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetSetting)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheetSetting();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetSetting)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPageWebServer* m_pWebSrv;
	CPageMediaServer* m_pMediaSrv;

public:
	int m_nSelPage;

protected:
	void Init();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETSETTING_H__2E75862A_DAE3_4032_A9B0_3605F6530265__INCLUDED_)
