#if !defined(AFX_PAGEDEFAULTIMAGE_H__E8EDE10B_90D4_45FC_A6F4_2EFBD9A604E4__INCLUDED_)
#define AFX_PAGEDEFAULTIMAGE_H__E8EDE10B_90D4_45FC_A6F4_2EFBD9A604E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDefaultImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultImage dialog
class CImageCell;
class CPageDefaultImage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDefaultImage)

// Construction
public:
	CPageDefaultImage();
	~CPageDefaultImage();

// Dialog Data
	//{{AFX_DATA(CPageDefaultImage)
	enum { IDD = IDD_DIALOG_DEFAULT_IMAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDefaultImage)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDefaultImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeListImageItem();
	afx_msg LRESULT OnCellCmd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellDblClicked(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioLarge();
	afx_msg void OnRadioMedium();
	afx_msg void OnRadioSmall();
	afx_msg void OnRadioExlarge();
	afx_msg void OnRadioHuge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CImageCell* m_pImageCell;
	int m_nSel;

public:	
	BOOL	m_bCanGetActive;
	BOOL	m_PageDefaultImageFlag;
protected:
	void SetImage();
	BOOL StrEqual(CString strInput, UINT nID);
	BOOL SelectImage();
	void SetTVImageLayout(BOOL bFlag);
	//add by buxiangyi
	CString GetLogoFile(CString strCourse); 
	void SetCourseImage();
	BOOL SelectCourseImage();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEFAULTIMAGE_H__E8EDE10B_90D4_45FC_A6F4_2EFBD9A604E4__INCLUDED_)
