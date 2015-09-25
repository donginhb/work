#if !defined(AFX_SHEETOPTION_H__6C11FEC7_2B4B_42D9_8C40_BB1C1795BCF4__INCLUDED_)
#define AFX_SHEETOPTION_H__6C11FEC7_2B4B_42D9_8C40_BB1C1795BCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetOption
class CPageRecordOption;
class CPageMisc;

//add by buxiangyi
//class CPageBackupDir;
//////////////////////////
class CPageDefaultCaption;

#include "basicsheet.h"
class CSheetOption : public CBasicSheet
{
	DECLARE_DYNAMIC(CSheetOption)

// Construction
public:
	CSheetOption(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetOption(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetOption)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheetOption();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetOption)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void Init();
	BOOL ContinueModal();

protected:
	CPageRecordOption*		m_pRecordOption;
	CPageMisc*				m_pMisc;
	//add by buxiangyi
	//CPageBackupDir*			m_pBackupDir;
	////////////////////
	CPageDefaultCaption*	m_pDefaultCaption;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETOPTION_H__6C11FEC7_2B4B_42D9_8C40_BB1C1795BCF4__INCLUDED_)
