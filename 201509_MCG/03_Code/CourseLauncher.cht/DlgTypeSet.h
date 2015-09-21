#if !defined(AFX_DLGTYPESET_H__20A3E531_D08A_423A_9660_85346044D2B8__INCLUDED_)
#define AFX_DLGTYPESET_H__20A3E531_D08A_423A_9660_85346044D2B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTypeSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTypeSet dialog
class CDataBrowseWnd;
class CDlgTypeSet : public CDialog
{
// Construction
public:
	CDlgTypeSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTypeSet)
	enum { IDD = IDD_DIALOG_TYPE_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTypeSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTypeSet)
	afx_msg void OnSelchangeComboType();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNewType();
	afx_msg void OnButtonRenameType();
	afx_msg void OnButtonRename();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnRadioOriginal();
	afx_msg void OnRadioConvert();
	afx_msg void OnSelchangeComboCodec();
	afx_msg void OnButtonDelType();
	afx_msg LRESULT OnRowSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRowAppended(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDataBrowseWnd*	m_pAttribSelGrid;
	CDataBrowseWnd*	m_pScreenAttribSelGrid;
	BOOL			m_bLoading;

protected:
	void ChangeSel();
	void CollectType();
	void LoadData(int nIndex);
	void DelData(int nIndex);
	void SaveData(int nIndex); 
	void SaveData(); 
	void RadioConvert(); 
	void RadioOriginal(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTYPESET_H__20A3E531_D08A_423A_9660_85346044D2B8__INCLUDED_)
