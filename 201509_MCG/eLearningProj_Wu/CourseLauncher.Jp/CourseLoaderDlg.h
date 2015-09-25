// CourseLoaderDlg.h : header file
//

#if !defined(AFX_COURSELOADERDLG_H__4D9D2710_6090_4CA5_A1E6_D0930DACFD3C__INCLUDED_)
#define AFX_COURSELOADERDLG_H__4D9D2710_6090_4CA5_A1E6_D0930DACFD3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCourseLoaderDlg dialog
//#include "..\MediaUtility\MediaUtility.h"
#include "..\MediaKit\MediaKit.h"
#include "afxinet.h"
#include "DlgProgress.h"
#include "TextProgressCtrl.h"

//#include "..\eLearning.cht\TextProgressCtrl.h"

struct CProgressInfo
{
	CProgressInfo()
        {
                m_iTotalSoFar = 0;
                m_iTotal = 0;
        }
	DWORD m_iTotal;
	DWORD m_iTotalSoFar;
	CTextProgressCtrl* m_pProgress;
};

struct CAddInfo
{
	CAddInfo(){}
	CAddInfo(const CString& szName, DWORD iSize, bool bDir = false)
		:m_szName(szName), m_iSize(iSize), m_bDir(bDir){}
	CString m_szName;
	bool m_bDir;
	DWORD m_iSize;
};
#include <afxtempl.h>
typedef CList<CAddInfo, CAddInfo> CAddInfoList;

class CColGrid;
class CBmpBtn;
class CZipArchive;
class CCourseLoaderDlg : public CDialog
{
// Construction
public:
	CCourseLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCourseLoaderDlg)
	enum { IDD = IDD_COURSELOADER_DIALOG };
	CCheckListBox m_clCourse;
	CTextProgressCtrl m_progress;

//	CComboBox	m_cbCourse;
//	CListCtrl	m_cListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCourseLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCourseLoaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg void OnLoad();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg void OnServerSetting();
	afx_msg void OnAbout();
	afx_msg void OnTypeSetting();
	afx_msg void OnTransmit();
	afx_msg void OnOption();
	afx_msg void OnViewCourse();
	afx_msg void OnClose();
	afx_msg void OnRadioFtp();
	afx_msg void OnRadioFtpLocal();
	afx_msg void OnRadioFtpManual();
	afx_msg void OnButtonDeselectAll();
	afx_msg void OnButtonSelectAll();
	virtual void OnCancel();
	afx_msg LRESULT OnButtonUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrdClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonSetType();
	afx_msg void OnButtonInputInfo();
	afx_msg void OnUpdateTypeSetting(CCmdUI* pCmdUI);
	afx_msg void OnRadioScorm();
	afx_msg void OnRadioNoScorm();
	afx_msg void OnRadioScormFtp();
	afx_msg void OnRadioScormLocal();
	afx_msg void OnRadioScormOracle();
	afx_msg void OnRadioScormOracleOld();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
//	CToolBar			m_wndToolBar;
//	IEasyUtility*		m_pUtility;
	//add by buxiangyi
	//IWMXUtility*		m_pWMXUtility;			
	CInternetSession*	m_pInetSession;
	CFtpConnection*		m_pFtpConnection;
	CDlgProgress		m_dlgProgress;
	CString				m_strMsg;
	CString				m_strTitle;
	CString				m_strFTPCourse;
	CColGrid*			m_pGrid;
	CBmpBtn*			m_pBtnOpenFTPPath;
	CBmpBtn*			m_pBtnOpenFilePath;
	CBmpBtn*			m_pBtnOpenScormPath;
	CStringArray		m_strArrayCourse;
	CStringArray		m_strArrayFTPCourse;

public:
	CString			m_strCourse;

protected:	
	void CollectCourse();
	void SetLVCheck(WPARAM nItemIndex, BOOL bCheck);
	BOOL GetLVCheck(int nIndex);
	int GetNumberPart(CString str);
	void ConvertFiles(CString strDestPath, CString strPostfix);
	BOOL ConvertCourse(CString strSrcPath, CString strDestPath, 
					   int nVideo, int nAudio, int nFPS);
	void UpdateMediaFile(CString strSrcPath, CString strPath, CString strMediaServer);
	BOOL SetOption(int nSel);
	BOOL AnyTypeChecked();
	BOOL GetMediaServerAlias(CString& strMediaSrv); 
	BOOL ConnectToFTP(CString strFtpSite, CString strUserName, 
					  CString strPWD, CString strRemotePath, CString strPort, BOOL bPassive);
	BOOL CheckURL(CString strFtpSite);
	//BOOL GetCourseName(CString strOutputPath, CString& strCourse);
	BOOL IsOurMedia(CString strFile);
	BOOL GetOutputPath(CString& strOutputPath); 

	//BOOL GenerateFreq(CString strSrcPath, CString strDestPath, CString strPostfix, BOOL bCopyVideo); 
	//Add by shien
	BOOL GenerateFreq(CString strSrcPath, CString strDestPath, CString strPostfix, BOOL bCopyVideo, CString strDestPath1); 
	
	BOOL ConnectToMediaServer(CString strNewCourse, CString& strFtpSite);
	BOOL ConnectToWebServer(CString& strFtpSite);
	BOOL FTPCreateDir(CString strNewCourse, CString strFtpSite);
	BOOL CopyDirToFTPServer(CString strSrcPath, CString strSubDir, CString strFtpSite);
	BOOL LaunchCourse(); 
	void SetProgress(CString strTitle, CString strMsg);
	void SetProgress(CString strMsg);
	void ShowProgress();
	void HideProgress();
	BOOL WriteMediaPos(CString strCourse, CString strNewCourse, 
					   CString strDestPath, CStringArray& strArrayPostfix); 
	//BOOL ConverCourseByBandwidth(CString strPostfix, CString strSrcPath, CString strDestPath, 
	//							 CStringArray& strArrayParam, CStringArray& strArrayFiles, BOOL bWithVideo);
	//add by shien
	BOOL ConverCourseByBandwidth(CString strPostfix, CString strSrcPath, CString strDestPath, 
								 CStringArray& strArrayParam, CStringArray& strArrayFiles, BOOL bWithVideo, CString strSrcPath1);
	BOOL WriteIndexHTMIndex(CString strCourse, CString strNewCourse, 
							CString strDestPath, CStringArray& strArrayTableEntry);
	CString GetTableHTML(CString strLabel, CString strLink, CString strURL);
	BOOL WriteDB(CString strCourseName, CString strLecturer, CString strCompany,
			     CString strDate, CString strURL, CString strContent);

	BOOL TransferByFTP(CString strCourse, CString& strNewCourse, CString strDestPath, 
					   CStringArray& strArrayPostfix, BOOL bCheckExist = TRUE, BOOL bCopyMediaOnly = FALSE);

	BOOL IsMediaFTPSet();
	BOOL IsWebFTPSet();
	BOOL PutFile(CString strCopyFrom, CString strFile);
	BOOL PeekAndPump();
	void ResetCheckItem();
	BOOL GetFTPCourseName(CString& strNewCourse, BOOL& bOverwrite);
	CString GetLinkText();
	void ConvBackSlashToSlash(CString& strInput);
	BOOL GetCourseURL(CString strNewCourse, CString& strURL);
	void WriteCheckStatus(); 
	void SelFolder(CString& strFolder); 
	BOOL PreProcessCourse(CString& strCourse, CString strOutputPath, int nDest);
	BOOL ProcessCourse(CString strCourse, CString strNewCourse, CString strOutputPath, int nDest, int nCheckScorn);
	BOOL GetPathCourseName(CString strOutputPath, CString& strNewCourse, BOOL& bOverwrite); 
	void LoadHistory(); 
	void WriteHistory(); 
	BOOL CreateIndex(CString strFile);
	//add by buxiangyi
	/*BOOL ConvertBandWidth(CString strSrc, CString strDest, int nVideoBitRate, int nFPS, int nSec, 
						  int nQuality, int nAudioBitRate, int nSampleRate, int nMono, CString strCodec);*/
	void RetriveInfo(int nIndex, CStringArray& strArrayParam);
	BOOL IsCourseVideoModified(CString strCourse, BOOL* pbOnlyTagModified = NULL);
	BOOL IsArrayDifferent(CStringArray& strArray1, CStringArray& strArray2);
	BOOL WritePublishRecord(CString strPath);
	void ArchiveClose(CZipArchive* pZip, bool bExcep); 
	void AddZipFiles();
	void AddZipFolder(CString strZipPath, CString strFolder); 
	void AddFolder(CString szFolder, CAddInfoList& list, CProgressInfo& p);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COURSELOADERDLG_H__4D9D2710_6090_4CA5_A1E6_D0930DACFD3C__INCLUDED_)
