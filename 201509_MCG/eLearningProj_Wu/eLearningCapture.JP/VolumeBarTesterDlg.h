// VolumeBarTesterDlg.h : header file
//

#if !defined(AFX_VOLUMEBARTESTERDLG_H__34AE26CE_62AF_4077_95D7_58FF3D234217__INCLUDED_)
#define AFX_VOLUMEBARTESTERDLG_H__34AE26CE_62AF_4077_95D7_58FF3D234217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVolumeBarTesterDlg dialog
#define WM_USER_Volume_TIMER			(WM_APP + 1234)

typedef int (FAR PASCAL *stInitDevice)(int);
typedef int (FAR PASCAL *stUnInitDevice)();
typedef int (FAR PASCAL *GetDeviceList)(char *);
typedef int (FAR PASCAL *SetInDeviceEnabled)(int,int,int);
typedef int (FAR PASCAL *SetPlayerChannel)(int,int);
typedef int (FAR PASCAL *GetInDeviceVU)();
typedef int (FAR PASCAL *MixerVolume)(int,int,BOOL,int);
typedef int (FAR PASCAL *SetSN)(char *);
#include "MacProgressCtrl.h"


/*
 function stInitDevice(AppHandle:THandle):integer;stdcall;external DLLNAME;
  function stUnInitDevice:integer;stdcall;external DLLNAME;
  function GetDeviceList(pDeviceList:Pchar):integer;stdcall;external DLLNAME;
  function SetInDeviceEnabled(MixID:integer;LineID:integer;isEnabled:integer):integer;stdcall;external DLLNAME;
  function SetPlayerChannel(PlayerID:integer;ChannelID:integer):integer;stdcall;external DLLNAME;
  function GetInDeviceVU():integer; stdcall; external DLLNAME;
  function MixerVolume(MixerID:integer;LineID:integer;isOutLine:Bool;wVol:integer):integer;stdcall; external DLLNAME;
  function SetSN(Sn : Pchar) : integer; stdcall;external DLLNAME;
  function OpenSoundCardDialog(CardID,DlgID,DlgMode : integer):integer;stdcall;external DLLNAME;

*/

class CVolumeBarTesterDlg : public CDialog
{
// Construction
public:
	CVolumeBarTesterDlg(CWnd* pParent = NULL);	// standard constructor
    UINT m_nTimerID;
	UINT m_nTimerID2;
	HINSTANCE hDLL;
    
    stInitDevice f_stInitDevice;
    stUnInitDevice f_stUnInitDevice;
	GetDeviceList  f_GetDeviceList;
	SetInDeviceEnabled f_SetInDeviceEnabled; 
	SetPlayerChannel f_SetPlayerChannel;
	GetInDeviceVU	 f_GetInDeviceVU;
	MixerVolume      f_MixerVolume;
	SetSN			 f_SetSN;
	int              m_DeviceID;
    
	void InitVolumeDLL();
// Dialog Data
	//{{AFX_DATA(CVolumeBarTesterDlg)
	enum { IDD = IDD_VOLUMEBARTESTER_DIALOG };
	/*
	CProgressCtrl m_left;
	CProgressCtrl m_right;
	*/
	CMacProgressCtrl m_left;
	CMacProgressCtrl m_right;
	CBrush m_leftBrush,m_rightBrush;

//	CProgressBar m_left;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeBarTesterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;





	// Generated message map functions
	//{{AFX_MSG(CVolumeBarTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetVolumeValues();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg LRESULT OnWM_USER_Volume_TIMER(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEBARTESTERDLG_H__34AE26CE_62AF_4077_95D7_58FF3D234217__INCLUDED_)
