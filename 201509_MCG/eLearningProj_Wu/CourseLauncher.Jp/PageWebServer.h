#if !defined(AFX_PAGEWEBSERVER_H__114604CF_D1D4_4649_A577_7E9699DED551__INCLUDED_)
#define AFX_PAGEWEBSERVER_H__114604CF_D1D4_4649_A577_7E9699DED551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageWebServer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageWebServer dialog

#include <afxdlgs.h>
class CPageWebServer : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageWebServer)

// Construction
public:
	CPageWebServer();
	~CPageWebServer();

// Dialog Data
	//{{AFX_DATA(CPageWebServer)
	enum { IDD = IDD_DIALOG_WEB_SRV_FTP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageWebServer)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageWebServer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString	m_strPreLogin;
	CString m_strPreIP;
	CString m_strPrePWD;
	CString m_strSubDir;
	CString m_strCourseURL;
	CString m_strPort;

protected:
	void ConvBackSlashToSlash(CString& strInput);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEWEBSERVER_H__114604CF_D1D4_4649_A577_7E9699DED551__INCLUDED_)
