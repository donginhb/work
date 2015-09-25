// hintlabe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoteEdit window
class CGridEdit;
class CUIGrid;
class CNoteEdit : public CWnd
{
// Construction
public:
	CNoteEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CNoteEdit();

protected:
	// Generated message map functions      
	//{{AFX_MSG(CNoteEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnEditKeyDown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg LRESULT OnLoseFocus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()   

public:
	BOOL Create(LPCSTR lpszClassName, DWORD dwStyle,
				const RECT& rect);  
	void SetGridWnd(CUIGrid* pGrid);
	void SetEditWnd(CWnd* pGrid);
	void SetFontEx(CFont* pFont);
	//void SetNoFocus() {u_bNoFocus = TRUE;};

public:
	CGridEdit* 	u_pEdit;	
	BOOL		u_pEditable;
	
protected:
	CWnd*		u_pEditWnd;
	CUIGrid*	u_pGridWnd;
	//BOOL		u_bNoFocus;
};

/////////////////////////////////////////////////////////////////////////////
