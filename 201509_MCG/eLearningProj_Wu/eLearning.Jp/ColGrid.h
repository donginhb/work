// ColGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColGrid window

#ifndef _COLGRID
#define _COLGRID

#include "uigrid.h"
class CColGrid : public CUIGrid
{
// Construction
public:
	CColGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColGrid)
	afx_msg void OnEditChange(); 
	afx_msg LRESULT OnComboEditChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CStringArray*	u_pColAttrib;
	CStringArray*	u_pColInit;

public:
   	BOOL			u_bLastRowStar;
	BOOL			u_bDeletable;
	BOOL			u_bInsertable;

protected:
	virtual void PostSetColNo(int nCol);
	virtual CString AddAttribNew(int nRow, int nCol, CString StrContent);
	virtual CString AddAttribSet(int nRow, int nCol, CString StrContent);
	virtual void PostLButtonDown(int ClickedRow, int ClickedCol);
	virtual CString SetHeadString(long i);
	virtual void KeyDown(UINT nChar);
	virtual CBrush* GetDefBrush(int nCol);
	void OnDelKeyDown();
	void InsertAtSelectedRow();
	void	CheckToAddLastRow(int ClickedRow, int ClickedCol);
//	virtual CBrush* GetBrush(int nRow, int nCol);

public:
	void	SetColAttribAt(int nCol, int nType, char nValue, BOOL bReflect = FALSE);
	//void	SetColAttribAt(int nCol, char nType, char nIndex, 
//					     char nEditable, char nAlign,
//					     char nBGColor, char nFGColor);
//	CString GetColAttribAt(int nCol);
	char	GetColAttribAt(int nCol, int nType);
	void	SetColInitAt(int nCol, CString StrInit);

	void	DelSelected();
};

#endif
/////////////////////////////////////////////////////////////////////////////
