#if !defined(AFX_PAGEBACKUPDIR_H__B8CA3063_8DFF_4627_9AA4_4AF8FCF6CA2C__INCLUDED_)
#define AFX_PAGEBACKUPDIR_H__B8CA3063_8DFF_4627_9AA4_4AF8FCF6CA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageBackupDir.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageBackupDir dialog
class CColGrid;
class CPageBackupDir : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageBackupDir)

// Construction
public:
	CPageBackupDir();
	~CPageBackupDir();

// Dialog Data
	//{{AFX_DATA(CPageBackupDir)
	enum { IDD = IDD_DIALOG_BACKUP_SETTING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageBackupDir)
	public:
	virtual void OnOK();
	//add by buxiangyi
	//virtual BOOL OnApply();
	//////////////////////////////////////
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageBackupDir)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonDelete();
	afx_msg LRESULT OnGrdNote(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CColGrid* m_pGrid;

public:
	//add by buxiangyi
	//CPropertySheet*	m_pSheet;

protected:
	BOOL CheckColNull(int nCheckCol); 
	BOOL CheckFileExist();
	BOOL CheckNameDup(); 
public:
	//add by buxiangyi
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBACKUPDIR_H__B8CA3063_8DFF_4627_9AA4_4AF8FCF6CA2C__INCLUDED_)
