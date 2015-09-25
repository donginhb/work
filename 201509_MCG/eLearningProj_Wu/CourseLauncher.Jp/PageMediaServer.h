#if !defined(AFX_PAGEMEDIASERVER_H__C6360500_2309_4DC0_A8A5_71A724490E49__INCLUDED_)
#define AFX_PAGEMEDIASERVER_H__C6360500_2309_4DC0_A8A5_71A724490E49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMediaServer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageMediaServer dialog

#include <afxdlgs.h>
class CPageMediaServer : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMediaServer)

// Construction
public:
	CPageMediaServer();
	~CPageMediaServer();

// Dialog Data
	//{{AFX_DATA(CPageMediaServer)
	enum { IDD = IDD_DIALOG_MEDIA_SRV_FTP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMediaServer)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMediaServer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString	m_strPreLogin;
	CString m_strPreIP;
	CString m_strPrePWD;
	CString m_strSubDir;
	CString m_strAlias;
	CString m_strPort;

protected:
	void ConvBackSlashToSlash(CString& strInput);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMEDIASERVER_H__C6360500_2309_4DC0_A8A5_71A724490E49__INCLUDED_)
