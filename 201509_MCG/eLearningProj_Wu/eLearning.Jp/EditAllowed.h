#if !defined(AFX_EditAllowed_H__3DAC4B94_0ECD_4C86_8477_9AD69BF48E8B__INCLUDED_)
#define AFX_EditAllowed_H__3DAC4B94_0ECD_4C86_8477_9AD69BF48E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAllowed.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditAllowed window

class CEditAllowed : public CEdit
{
// Construction
public:
	CEditAllowed();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditAllowed)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditAllowed();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditAllowed)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	CString m_strAllowed;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EditAllowed_H__3DAC4B94_0ECD_4C86_8477_9AD69BF48E8B__INCLUDED_)
