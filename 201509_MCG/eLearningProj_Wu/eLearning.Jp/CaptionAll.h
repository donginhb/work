#pragma once
#include "resource.h"

// CCaptionAll dialog

class CCaptionAll : public CDialogEx
{
	DECLARE_DYNAMIC(CCaptionAll)

public:
	CCaptionAll(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCaptionAll();

// Dialog Data
	enum { IDD = IDD_DIALOG_CAPTIONALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
