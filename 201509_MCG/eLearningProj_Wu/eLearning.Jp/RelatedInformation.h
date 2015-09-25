#if !defined(AFX_CRELATEDINFORMATION_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_)
#define AFX_CRELATEDINFORMATION_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// RelatedInformation dialog
class CUIGrid;
#include "resource.h"
class CRelatedInformation : public CPropertyPage
{
	DECLARE_DYNAMIC(CRelatedInformation)

public:
	CRelatedInformation();
	virtual ~CRelatedInformation();
	virtual void OnOK();
// Dialog Data
	enum { IDD = IDD_DIALOG_DEFAULT_RELATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//afx_msg LRESULT OnGrdUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	BOOL LoadCourseLayout(LPCTSTR szCourse);
protected:
	CString			m_strCourse;
	CStatic			*m_Whole;
	CStatic			*m_StaticName;
	CStatic			*m_StaticMessage;
	CEdit			*m_strNameCEdit;
	CEdit			*m_strMessage;
	CPoint			HorizontallineBegin;
	CPoint			HorizontallineEnd;
	CPoint			VerticallineBegin;
	CPoint			VerticallineEnd;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRELATEDINFORMATION_H__1F75474B_CAF4_4D73_8C48_D2307205F497__INCLUDED_)