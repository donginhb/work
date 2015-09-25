// DLG_COUNT_DOWN.cpp : implementation file
//

#include "stdafx.h"
#include "elearningcapture.h"

#include "CtrlPanelWnd.h"

#include "DLG_COUNT_DOWN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define DEMO_DAYS		0
#define DEMO_HOURS		0
#define DEMO_MINUTES	0
#define DEMO_SECONDS	5
/////////////////////////////////////////////////////////////////////////////
// CDLG_COUNT_DOWN dialog

void TakeAction(UINT CtrlID)
{
	//this->SendMessage(WM_CLOSE);
}
CDLG_COUNT_DOWN::CDLG_COUNT_DOWN(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_COUNT_DOWN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_COUNT_DOWN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLG_COUNT_DOWN::DoDataExchange(CDataExchange* pDX)
{

	DDX_Control(pDX, IDC_STATIC_CNTDWN, m_MyCtrl);
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_COUNT_DOWN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_COUNT_DOWN, CDialog)
	//{{AFX_MSG_MAP(CDLG_COUNT_DOWN)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_CountDown_TIMER, OnWM_USER_CountDown_TIMER)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_COUNT_DOWN message handlers

BOOL CDLG_COUNT_DOWN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here



	m_MyCtrl.SetOn(FALSE);
	m_MyCtrl.SetBackgroundOffColor(RGB(0,0,120));
	m_MyCtrl.SetModalFrame();
	m_MyCtrl.SetPointFont(36,"Arial");
	actfuncptr = TakeAction;

	

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
int CDLG_COUNT_DOWN::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
    nH=rect.bottom-rect.top;



	int     wndLeft;
	int     wndTop;
	int     wndHeight;
  
	wndLeft=((CELearningCaptureApp*)AfxGetApp())->m_wndLeft-nW-1;
    wndTop=((CELearningCaptureApp*)AfxGetApp())->m_wndTop;
    wndHeight=((CELearningCaptureApp*)AfxGetApp())->m_wndHeight;

	SetWindowPos(&wndTopMost,(nScreenX-nW)/2, (nScreenY-nH)/2 , nW, nH,SWP_HIDEWINDOW);  
	BringWindowToTop();
	SetForegroundWindow();
	RedrawWindow();
    return 1;
//	AfxMessageBox("Here we start");
}
LRESULT CDLG_COUNT_DOWN::OnWM_USER_CountDown_TIMER(WPARAM wParam, LPARAM lParam)
{
	//Sleep(200);
   
//	AfxMessageBox("Timer started");
	//m_MyCtrl.SetPointFont(24);
	m_MyCtrl.Set(DEMO_DAYS,
		         DEMO_HOURS,
				 DEMO_MINUTES,
				 DEMO_SECONDS,
				 true,
				 CGCCountdownCtrl::CD_SECONDS ,
				 TakeAction);
	m_MyCtrl.Start();
    m_nTimerID = SetTimer(99, 10, NULL);
    
	return 0L;
}
void CDLG_COUNT_DOWN::OnTimer(UINT nIDEvent) 
{
	static  int timecount=0;
  	if (nIDEvent == 99)
	
	{
      // if (m_MyCtrl.m_bZeroPointReached)
	   timecount++;
	   if (timecount == 200)
       { 
		   //KillTimer(m_nTimerID);
		   //	AfxMessageBox("Here we start");
		   ((CELearningCaptureApp*)AfxGetApp())->m_pMainWnd->SendMessage(WM_USER_CountDownOver_TIMER);
		   //	AfxMessageBox("Here we End");



		   //m_MyCtrl.m_bZeroPointReached=false;
		   //timecount=0;
		   //AfxMessageBox("Here we end");
		   //SendMessage(WM_CLOSE);
	   }      
	   if (m_MyCtrl.m_bZeroPointReached)
	   {

		   KillTimer(m_nTimerID);
		   m_MyCtrl.m_bZeroPointReached=false;
		   timecount=0;
		   //AfxMessageBox("Here we end");
		   SendMessage(WM_CLOSE);
	   }
    }
}
