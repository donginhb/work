#if !defined(AFX_BMPCOMBO_H__27A80724_8226_4C85_9101_AC7ADA271FA7__INCLUDED_)
#define AFX_BMPCOMBO_H__27A80724_8226_4C85_9101_AC7ADA271FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMPCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBMPCombo window

class CBMPCombo : public CComboBox
{
// Construction
public:
	CBMPCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMPCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBMPCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBMPCombo)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	//int GetIndexFromID(CString strID);

public:
	CObArray*		m_pBMPArray;
	CStringArray*	m_pBMPIDArray;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPCOMBO_H__27A80724_8226_4C85_9101_AC7ADA271FA7__INCLUDED_)
