// mysheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBasicSheet
#ifndef _BASIC_PROPERTY
#define _BASIC_PROPERTY
class CBasicSheet : public CPropertySheet
{
protected:
//	void BuildPropPageArray();
	RECT m_rctPage;
	//CFont m_fntPage;

	DECLARE_DYNAMIC(CBasicSheet)

// Construction
public:
	CBasicSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBasicSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CFont* m_pFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasicSheet();

	// Generated message map functions
protected:
	virtual void BuildPropPageArray ();
	//{{AFX_MSG(CBasicSheet)
	//}}AFX_MSG
	//afx_msg LONG OnResizePage (UINT, LONG);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif