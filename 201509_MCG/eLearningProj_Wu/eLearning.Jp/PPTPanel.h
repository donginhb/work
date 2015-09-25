#if !defined(AFX_PPTPANEL_H__9CDC09DF_F36B_45F2_BC92_53B206403B9A__INCLUDED_)
#define AFX_PPTPANEL_H__9CDC09DF_F36B_45F2_BC92_53B206403B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPTPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPTPanel window
//#include "PPTEventSink.h"
//#include "msppt9.h"
class CPPT9Ctrl;
class CPPT10Ctrl;	//add 2014/7/29 kagata BugNo.130
class CPPT15Ctrl;	//add 2014/7/29 kagata BugNo.130
class CPPTWnd;		//add 2014/7/29 kagata BugNo.130
class CPPTPanel : public CWnd
{
// Construction
public:
	CPPTPanel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPTPanel)
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPPTPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPPTPanel)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				  UINT nID, CCreateContext* pContext);
//	void ReleasePPT(); 

protected:
	static CString m_strClass;

//	_Application		  m_app;
//	_Presentation         m_presentation;

//	IConnectionPoint* m_pConnectionPoint;
//    PPTEventSink* m_sink;            //Global variables
//	mso::_CommandBars m_cbs;         //= m_app.GetCommandBars();  // Construct the CommandBars
//	mso::CommandBar m_oCB;           //= cbs.GetItem(COleVariant((long)29)); // Slide Show CommandBar
//	mso::CommandBarControls m_oCBCs; //= oCB.GetControls();
//	mso::CommandBarControl m_oCBC;   //= oCBCs.GetItem(COleVariant((long)3)); // The Go control
//	mso::CommandBarControl m_oCBC2;

protected:
	//CPPT9Ctrl* m_pPPT;
	//add 2014/7/29 kagata BugNo.130
	CPPT9Ctrl* m_pPPT9;
	CPPT10Ctrl* m_pPPT10;
	CPPT15Ctrl* m_pPPT15;
	CPPTWnd* m_pPPTWnd;

public:
	BOOL LoadPPT(CString strPath, int nWidth, int nHeight);
	void PrevSlide();
	void NextSlide();
	void GotoSlide(int n);
	void FirstSlide();
	void LastSlide();
	int GetSlideCount();
	void GetSliderTitle(CStringArray& strArray);
	int GetCurrentSlideNumber();
	int GetCurrentAnimaNumber();

	//add 2014/7/29 kagata BugNo.130
	int nVer;
	int nPageflag;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTPANEL_H__9CDC09DF_F36B_45F2_BC92_53B206403B9A__INCLUDED_)
