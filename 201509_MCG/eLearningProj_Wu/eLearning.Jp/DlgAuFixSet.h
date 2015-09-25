#pragma once
#include "resource.h"

// CDlgAuFixSet dialog

class CDlgAuFixSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAuFixSet)

public:
	CDlgAuFixSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAuFixSet();

// Dialog Data
	enum { IDD = IDD_DIALOG_AUFIXSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	CSliderCtrl	m_sliderAuFix;
	CStatic m_auFixValue;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
};
