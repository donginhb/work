// VolumeBarTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elearningCapture.h"
#include "VolumeBarTesterDlg.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <time.h>
#define Volumeconst 5
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeBarTesterDlg dialog

CVolumeBarTesterDlg::CVolumeBarTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVolumeBarTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeBarTesterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitVolumeDLL();
    srand( (unsigned)time( NULL ) );
}

void CVolumeBarTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeBarTesterDlg)
	DDX_Control(pDX, IDC_PROGRESS_Left, m_left);
	DDX_Control(pDX, IDC_PROGRESS_Right, m_right);

//	DDX_Control(pDX, IDC_VOLUME_HORIZONTAL, m_VolumeHorizontal);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVolumeBarTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CVolumeBarTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_Volume_TIMER, OnWM_USER_Volume_TIMER)
//	ON_BN_CLICKED(ID_GET_VOLUME_VALUES, OnGetVolumeValues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeBarTesterDlg message handlers

BOOL CVolumeBarTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_leftBrush.CreateSolidBrush(RGB(10,118,43));
	m_rightBrush.CreateSolidBrush(RGB(36,2,255));
    hDLL=NULL;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CVolumeBarTesterDlg::OnDestroy() 
{
  KillTimer(m_nTimerID);
  KillTimer(m_nTimerID2);
  f_stUnInitDevice();
  FreeLibrary(hDLL);
  CDialog::OnDestroy();
}
void CVolumeBarTesterDlg::InitVolumeDLL()
{
  char szLibName[256];
  //if (hDLL==NULL)
  {
	 sprintf(szLibName,"multOut.dll");
	 //AfxMessageBox("Here we start dll 1");
	 hDLL=LoadLibrary(szLibName);
	// AfxMessageBox("Here we start dll 2");
	 f_SetSN=(SetSN)GetProcAddress(hDLL,"SetSN");
     f_stInitDevice=(stInitDevice)GetProcAddress(hDLL,"stInitDevice");
	 f_stUnInitDevice=(stUnInitDevice)GetProcAddress(hDLL,"stUnInitDevice");
	 f_GetDeviceList=(GetDeviceList)GetProcAddress(hDLL,"GetDeviceList");
	 f_SetInDeviceEnabled=(SetInDeviceEnabled)GetProcAddress(hDLL,"SetInDeviceEnabled");
	 f_SetPlayerChannel=(SetPlayerChannel)GetProcAddress(hDLL,"SetPlayerChannel");
	 f_GetInDeviceVU=(GetInDeviceVU)GetProcAddress(hDLL,"GetInDeviceVU");
	 f_MixerVolume=(MixerVolume)GetProcAddress(hDLL,"MixerVolume");
	 f_SetSN("MKF20-4NOOK-N589H-HYTBE-UZYKJ");

  }


}
void CVolumeBarTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
 
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVolumeBarTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVolumeBarTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVolumeBarTesterDlg::OnGetVolumeValues() 
{
	// TODO: Add your control notification handler code here
	CString msg;

//	msg.Format("Horizontal = %d Vertical = %d", m_VolumeHorizontal.GetVolume(), m_VolumeVertical.GetVolume() );
	AfxMessageBox(msg);

   srand( (unsigned)time( NULL ) );
  
   /*
   for( int i = 0;   i < 20;i++ )
   {
      Sleep(1000);
      jj= rand() % 100;
	  m_VolumeHorizontal.SetVolume(jj);
   }
   */
}
void CVolumeBarTesterDlg::OnTimer(UINT nIDEvent) 
{
	int jj;
  	if (nIDEvent == 999)
	{
	//	AfxMessageBox("Here we come");
    //    char hh[256];
	
	//	int kk=f_GetInDeviceVU();
		//AfxMessageBox("Here we come 1");
		f_SetPlayerChannel(m_DeviceID,0);
		//AfxMessageBox("Here we come 2");
		jj=f_GetInDeviceVU()/Volumeconst;
		//AfxMessageBox("Here we come 3");
		m_left.SetPos(jj);
		//AfxMessageBox("Here we come 4");
    }
	//	sprintf(hh,"%d",kk);
      //  AfxMessageBox(hh);
		/*
          m_VolumeVertical.SetVolume(jj );
        */
	//	m_VolumeVertical.SetVolume(jj );
	if (nIDEvent == 1000)
	{
		f_SetPlayerChannel(m_DeviceID,1);
		jj=f_GetInDeviceVU()/Volumeconst;
		m_right.SetPos(jj);

	
	}
	CWnd::OnTimer(nIDEvent);
	return;

}
int CVolumeBarTesterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//	AfxMessageBox("Here we start 1");
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
//	int nW = rect.right + constLargeGap + constMediumGap;
//	int nH = cs.cy + constSmallGap * 2 + 6;

	CRect rect;
	int nH,nW;
    this->GetWindowRect(&rect);
	nW=rect.right-rect.left;
    nH=rect.bottom-rect.top-10;



	int     wndLeft;
	int     wndTop;
	int     wndHeight;
  
	wndLeft=((CELearningCaptureApp*)AfxGetApp())->m_wndLeft-nW-1;
    wndTop=((CELearningCaptureApp*)AfxGetApp())->m_wndTop;
    wndHeight=((CELearningCaptureApp*)AfxGetApp())->m_wndHeight;

	SetWindowPos(&wndTopMost,wndLeft, wndTop , nW, wndHeight,SWP_SHOWWINDOW);  
	BringWindowToTop();
	SetForegroundWindow();
	RedrawWindow();
    return 1;
//	AfxMessageBox("Here we start");
}
LRESULT CVolumeBarTesterDlg::OnWM_USER_Volume_TIMER(WPARAM wParam, LPARAM lParam)
{
	Sleep(1000);
    
	
    //AfxMessageBox("Here we start volume 1");
    CString strIniPath;
	CString sUsedAudio;
    f_stInitDevice(0); 
	char* szValue = new char[1024];
	

	strIniPath = ::GetSysPath() + "Elearning.ini";
	::GetPrivateProfileString("Device", "Audio", "", szValue, 255, strIniPath);
  	sUsedAudio=szValue;
	sUsedAudio.MakeUpper();
   // AfxMessageBox(sUsedAudio);
	CString sDevices,sTemp;
//
	int iDeviceCount; 
   	//AfxMessageBox("Here we start volume 1");
    iDeviceCount=f_GetDeviceList(szValue);
	//AfxMessageBox("Here we start volume 2");
	sDevices=szValue;
	sDevices.MakeUpper();
    //AfxMessageBox(sDevices);

	int i_InitPos=0;
	int i_find;
	//AfxMessageBox(sUsedAudio);
    m_DeviceID=0;

	for (int i=0; i < iDeviceCount;i++)
    {
		
      i_find=sDevices.Find('\n',i_InitPos);
	  if (i_find==-1)
		 break;

	  sTemp=sDevices.Mid(i_InitPos,i_find-i_InitPos);
	  //AfxMessageBox(sTemp);
	 
	  if (sTemp==sUsedAudio)
      {
         m_DeviceID=i;
		 //AfxMessageBox("Successful!!");
		 break;
      }
	  i_InitPos=i_find+1;

      
    }
	delete [] szValue;
	f_SetInDeviceEnabled(m_DeviceID,0,1);

    m_nTimerID = SetTimer(999, 100, NULL);
 

//	f_SetInDeviceEnabled(0,0,1);
	m_left.SetRange(0,10);
	m_right.SetRange(0,10);
    m_left.SetColor(RGB(10,255,10));

    m_right.SetColor(RGB(10,10,255));
    //	AfxMessageBox("Here we start volume 2");
    m_nTimerID = SetTimer(999, 200, NULL);
	Sleep(100);
//		AfxMessageBox("Here we start volume 3");
    m_nTimerID2 = SetTimer(1000, 200, NULL);
//		AfxMessageBox("Here we start volume 4");

	return 0L;
}
HBRUSH CVolumeBarTesterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	/*
	switch (nCtlColor)
	{
     case CTLCOLOR_STATIC:
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(225,255,251));
    
     default:
	 {
	   switch (pWnd->GetDlgCtrlID())
	   {
		   //m_leftBrush,m_rightBrush;
	     case IDC_PROGRESS_Left:	
			 pDC->SetTextColor(RGB(10,118,43));
             hbr=(HBRUSH) m_leftBrush;
         case IDC_PROGRESS_Right:	
			 pDC->SetTextColor(RGB(36,2,255));
             hbr=(HBRUSH) m_rightBrush;
         break;

       }
     }
      break;
    }
	*/
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
