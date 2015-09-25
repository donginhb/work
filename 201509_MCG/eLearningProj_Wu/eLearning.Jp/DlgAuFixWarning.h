#pragma once
#include "resource.h"

// CDlgAuFixWarning dialog

class CDlgAuFixWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAuFixWarning)

public:
	CDlgAuFixWarning(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAuFixWarning();

// Dialog Data
	enum { IDD = IDD_DIALOG_AUFIXW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
