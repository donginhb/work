// CtrlPanelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "eLearningCapture.h"
#include "CtrlPanelWnd.h"
#include "..\elearning.Cht\MemDC.h"
#include "..\elearning.Cht\BmpBtn.h"
#include "..\elearning.Cht\NumberEdit.h"
#include "3DStatic.h"
#include "..\elearning.Cht\BmpStatic.h"
#include "..\elearning.Cht\BmpApi.h"
#include "..\elearning.Cht\HTMLWnd.h"
#include "PreviewWnd.h"
#include "cmkapi.h"
#include "DlgCloseToDo.h"
//#include "..\eLearning.Cht\Ppt9Ctrl.h"
#include "..\eLearning.Cht\Ppt15Ctrl.h"		// add itoh 2013/07/11 PPT2013
#include "DlgGotoSlide.h"
#include "DlgVideoExist.h"
#include "DlgPenProperty.h"
#include "..\_Shared\HelperKit.h"
#include "..\MediaKit\MediaKit_i.c"
#include "Freedraw.h"
#include "HelperFree.h"
#include "..\eLearning.Cht\Registry.h"
#include "windows.h"		// add itoh 2013/02/13 Disable annotation on Windows8

/*
#include "videocompressors.h"
#include "audiocompressors.h"
#include "wmvprofiles.h"
#include "audiodevices.h"
#include "audioinputpins.h"
*/
#include <io.h>
#include <direct.h>
#include <stdio.h>
///cch
#include <dshow.h>

#define	HIMETRIC_INCH	2540
#define WM_LOAD_COURSE  (WM_APP + 459)

//extern "C" {
//	#include "..\PPTControl\PPT_i.c"
//}

extern "C" {
#include "..\ComCapture\ComCapture_i.c"
}

//extern "C" {
//#include "..\MediaProfile\MediaProfile_i.c"
//}
//////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifndef OATRUE
//#define OATRUE (-1)
//#endif // OATRUE
//#ifndef OAFALSE
//#define OAFALSE (0)
//#endif // OAFALSE

/////////////////////////////////////////////////////////////////////////////
// CCtrlPanelWnd


HINSTANCE CCtrlPanelWnd::m_hmodHook = NULL;
//HINSTANCE CCtrlPanelWnd::m_hKBmodHook = NULL;
HHOOK CCtrlPanelWnd::m_hHook = NULL;
//HHOOK CCtrlPanelWnd::m_hKBHook = NULL;
CString CCtrlPanelWnd::m_strClass = "";

DWORD WINAPI PlayMessageThread(LPVOID lpParameter)
//void CCtrlPanelWnd::OnRec()
{
	
	CCtrlPanelWnd	*pPlay = (CCtrlPanelWnd *)lpParameter;
   
	if ( !pPlay ) 
		return 0;
	//AfxMessageBox("Here we start record 3");
	if (pPlay->dlgclock !=NULL)
	{
 	  pPlay->dlgclock->ShowWindow(SW_SHOW);
	  #ifndef _VISTA
		  pPlay->m_dlgVolumeBAr->SendMessage(WM_USER_Volume_TIMER);
	  #endif;
	
      pPlay->dlgclock->SendMessage(WM_USER_CountDown_TIMER);
	}
	//AfxMessageBox("Here we start record 3");
   
	//AfxMessageBox("Here we start record 4");
	/*
	#ifndef _VISTA
		if (pPlay->m_dlgVolumeBAr)
		{
		  
			  pPlay->m_dlgVolumeBAr->SendMessage(WM_USER_Volume_TIMER);
		
		 
		  //AfxMessageBox("Here we start record 5");
		}
   		return 0;
	#endif;
	*/


}

//wangwangwang
DWORD WINAPI DoRecMostThread(LPVOID lpParameter)
{
  CCtrlPanelWnd	*pPlay = (CCtrlPanelWnd *)lpParameter;
  if ( !pPlay )  
		return 0;
  pPlay->DoRecMost();  


}
void CCtrlPanelWnd::OnRec()
{


	DWORD dwThreadID;
   


    m_hPlayHandle = CreateThread(NULL, 0, PlayMessageThread, this, 0, &dwThreadID);
    CloseHandle( m_hPlayHandle );
	m_bCouldRecord=false;
	if (m_bCaptureScreenMode) 
	  m_bCouldRecord=true;
	m_nTimerID3 = SetTimer(666, 100, NULL);
}
void CCtrlPanelWnd::DoRecMost()
{
   	CString strPageNo;


//	m_bCouldRecord=false;
    KillTimer(m_nTimerID3);
	m_pPageNoEdit->GetWindowText(strPageNo);
	strPageNo.TrimLeft();
	strPageNo.TrimRight();

//	AfxMessageBox("Here we start record");
//	dlg->SendMessage(WM_USER_Volume_TIMER);


//	AfxMessageBox("Here we start volume");

//	MSG gg;
	/*
    while (!m_bCouldRecord)
    {
		Sleep(100);
		PeekMessage(&gg,NULL,0,0,PM_NOREMOVE);
    }
	*/
	if (strPageNo == "")
	{
		if (W_ASK_YESNO(IDS_PPT_NOT_OPEN, this) == IDNO)
		{
			m_pBtnRec->SetButtonDown(FALSE);
			return;
		}
	}

	int nIndex = GetCurRecIndex();
	CString strAsf;
	BOOL bDefault = TRUE;
	if (nIndex < 0)
	{
		strAsf = m_strMediaPath + "original.wmv";
		m_strCurFile = "original.wmv";
	}
	else if (nIndex == 0 && m_nState != PAUSED)
	{
		int nDemoIndex = GetCurrentDemoIndex();
		if (nDemoIndex == 0)
		{
#ifdef _TRIAL
	strAsf = m_strMediaPath + "original.wmv";
#else
			if (!m_bNG)
			{
				CDlgVideoExist dlg;
				
				MACRO_APP->m_bMessageBox = TRUE;
				UINT nRet = dlg.DoModal();
				OnView();							//add 2013/9/19 kagata PPT2013
				MACRO_APP->m_bMessageBox = FALSE;

				if (nRet == ID_APPEND)
				{
					strAsf = m_strMediaPath + "index1.wmv";
					m_strCurFile = "index1.wmv";
					bDefault = FALSE;
				}
				else if (nRet == ID_OVERWRITE)
				{
					strAsf = m_strMediaPath + "original.wmv";
					m_strCurFile = "original.wmv";
				}
				else
				{
					m_pBtnRec->SetButtonDown(FALSE);
					return;
				}
			}
			else
			{
				strAsf = m_strMediaPath + "index1.wmv";
				m_strCurFile = "index1.wmv";
				bDefault = FALSE;
			}
#endif
		}
		else
		{
			strAsf = m_strMediaPath + "index1.wmv";
			m_strCurFile = "index1.wmv";
			bDefault = FALSE;
		}
	}
	else
	{
		CString strFile;
		strFile.Format("index%d.wmv", nIndex + 1); 
		strAsf = m_strMediaPath + strFile;
		m_strCurFile = strFile;
		bDefault = FALSE;
	}

//2005-11-01
	if (m_bNG)
	{
		ReloadNG();
		if (m_strArrayNG.GetSize() > 0)
		{
			AddScript();
			CString strTemp = m_strArrayNG.GetAt(m_strArrayNG.GetSize() - 1);
			int nIndex = strTemp.Find(" ", 0);
			if (nIndex >= 0)
			{
				char szBuf[32];
				strTemp = strTemp.Left(nIndex) + CString(" ") + itoa(m_nIndex, szBuf, 10);

				m_strArrayNG.SetAt(m_strArrayNG.GetSize() - 1, strTemp);
				WriteNGFile();
				m_nMaxIndex = m_nIndex;
			}
		}
	}

	m_bNG = FALSE;

	m_pBtnRec->SetButtonDown(TRUE);
	m_pBtnStop->SetButtonDown(FALSE);
	m_pBtnPause->EnableButton(TRUE);
	m_pBtnStop->EnableButton(TRUE);
	m_pBtnNG->EnableButton(TRUE);
	m_pTimeStatic->SetText("00:00:00");
	UpdateWindow();
    CString uu;
	if (m_nState == STOPPED || m_nState == PAUSED)
	{
		m_nTickCount = GetTickCount();
		//uu.Format("%d", m_nTickCount);
		//AfxMessageBox(uu);
		StartCapture(strAsf); 
        //AfxMessageBox(uu);

		m_nPreSlideNo = 0;
		m_nPreSubSlideNo = 0;
		
		//InitStepArray();
		if (bDefault)
		{
			ClearStep();
		}
		else
		{
			ReloadStep();
		}

		m_nState = RECORDING;
		if (strPageNo != "")
		{
			CString strStep;
			if (m_bUsePPT)
			{
				long n = PPTCurSlideNo();
				m_nPreSlideNo = n;
				strStep.Format("0 flipslide %ld=", n);
				m_strArrayStep.Add(strStep);
				m_nMaxIndex = -1;
			}
			else if (m_bUseHTML)
			{
				strStep.Format("0 flippage %s=", strPageNo);
				m_strArrayStep.Add(strStep);
				m_nMaxIndex = -1;
			}
		}
	}
	//else if (m_nState == PAUSED)
	//{
	//	m_nTickCount = GetTickCount();
	//	Resume();
	//}
	
	m_pBmpStaticBlue->ShowWindow(SW_HIDE);
	m_pBmpStaticRed->ShowWindow(SW_SHOW);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_HIDE);

	AttachVideo(m_pPreviewWnd);
	m_nState = RECORDING;
	m_bDisConnected = FALSE;
	//SendMessage(WM_USER_Volume_TIMER);
}
/*
void CCtrlPanelWnd::OnRec()
//DWORD WINAPI PlayMessageThread(LPVOID lpParameter)
{
	DWORD dwThreadID;
   

	dlgclock->ShowWindow(SW_SHOW);
    dlgclock->SendMessage(WM_USER_CountDown_TIMER);
    dlg->SendMessage(WM_USER_Volume_TIMER);
    m_hPlayHandle = CreateThread(NULL, 0, PlayMessageThread, this, 0, &dwThreadID);
    CloseHandle( m_hPlayHandle );

	//Sleep(100);
	return ;
}
*/
CCtrlPanelWnd::CCtrlPanelWnd()
{
	m_nPreDemoIndex = -1;
   
	CSize cs = GetBmpDim(IDB_CLOSE);
	m_nCloseBtnWidth = cs.cx / 4;

	m_pBtnClose = new CBmpBtn(IDB_CLOSE, BTN_PUSH_BUTTON);

	m_pBtnCapture = new CBmpBtn(IDB_CAPTURE, BTN_PUSH_BUTTON);
	m_pBtnRecDemo = new CBmpBtn(IDB_REC_DEMO, BTN_SUNK_BUTTON);
	m_pBtnStopDemo = new CBmpBtn(IDB_STOP_DEMO, BTN_SUNK_BUTTON);

	m_pBtnDemo = new CBmpBtn(IDB_DEMO, BTN_PUSH_BUTTON);
	//m_pBtnOpen = new CBmpBtn(IDB_OPEN, BTN_PUSH_BUTTON);
	m_pBtnView = new CBmpBtn(IDB_VIEW, BTN_PUSH_BUTTON);
    m_dlgVolumeBAr=NULL;

	m_pBtnRec = new CBmpBtn(IDB_REC, BTN_SUNK_BUTTON);
	m_pBtnPause = new CBmpBtn(IDB_PAUSE, BTN_SUNK_BUTTON);
	m_pBtnStop = new CBmpBtn(IDB_STOP, BTN_SUNK_BUTTON);
//2005-11-01
//	m_pBtnNG = new CBmpBtn(IDB_NG, BTN_PUSH_BUTTON);
	m_pBtnNG = new CBmpBtn(IDB_NG, BTN_SUNK_BUTTON);

	m_pBtnFirst = new CBmpBtn(IDB_FIRST, BTN_PUSH_BUTTON);
	m_pBtnPrev = new CBmpBtn(IDB_PREV, BTN_PUSH_BUTTON);
	m_pBtnNext = new CBmpBtn(IDB_NEXT, BTN_PUSH_BUTTON);
	m_pBtnLast = new CBmpBtn(IDB_LAST, BTN_PUSH_BUTTON);
	m_pBtnGoto = new CBmpBtn(IDB_GOTO, BTN_PUSH_BUTTON);

/*	del itoh 2013/07/24 PPT2013
	m_pBtnRect = new CBmpBtn(IDB_RECT, BTN_TOGGLE_BUTTON);
	m_pBtnLine = new CBmpBtn(IDB_LINE, BTN_TOGGLE_BUTTON);
	m_pBtnPointer = new CBmpBtn(IDB_POINTER, BTN_TOGGLE_BUTTON);	
	m_pBtnFreeHand = new CBmpBtn(IDB_FREE_HAND, BTN_TOGGLE_BUTTON);	
	m_pBtnEraser = new CBmpBtn(IDB_ERASER, BTN_PUSH_BUTTON);
	m_pBtnPenSet = new CBmpBtn(IDB_PEN_SET, BTN_PUSH_BUTTON);
*/
	m_pBtnZoomIn = new CBmpBtn(IDB_ZOOM_IN, BTN_PUSH_BUTTON);
	m_pBtnZoomOut = new CBmpBtn(IDB_ZOOM_OUT, BTN_PUSH_BUTTON);

	m_pBtnPreview = new CBmpBtn(IDB_PREVIEW, BTN_PUSH_BUTTON);
	//m_pBtnHelp = new CBmpBtn(IDB_HELP, BTN_PUSH_BUTTON);

	m_pBmpLabel1 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel2 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel3 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel4 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel5 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel6 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel1->SetBorder(FALSE);
	m_pBmpLabel2->SetBorder(FALSE);
//	m_pBmpLabel3->SetBorder(FALSE);
	m_pBmpLabel4->SetBorder(FALSE);
	m_pBmpLabel5->SetBorder(FALSE);
	m_pBmpLabel6->SetBorder(FALSE);

	m_pBmpStaticBlue = new CBmpStatic(IDB_GREEN_LIGHT);
	m_pBmpStaticRed = new CBmpStatic(IDB_RED_LIGHT);
	m_pBmpStaticOrange = new CBmpStatic(IDB_DARK_LIGHT);
	m_pBmpStaticYellow = new CBmpStatic(IDB_ORANGE_LIGHT);


	m_pPageNoEdit = new CNumberEdit;

	m_pTimeStatic = new C3DStatic;

	m_pSysFont = new CFont;
	m_pPreviewWnd = NULL;

	m_strPath = "";
	m_strMediaPath = "";
	m_strHTMLPath = "";
	m_nState = STOPPED;
	m_nTickCount = 0;
	m_nBaseMS = 0;

	m_bNoAcceptError = FALSE;
	m_nPreSlideNo = 0;
	m_nPreSubSlideNo = 0;
	m_bTrackMove = FALSE;

	m_eStatus = EASY_IDLE;
	m_pEasyCapture = NULL;
	m_hMoveCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);

	m_lTime = 0;
	m_pPageMedias = new CObArray;
	m_nIndex = 0;



	m_pPPT = NULL;
	m_bOpen = FALSE;
	m_strOp = "=";
	m_strDemoFile = "";
	m_nDemoIndex = 0;
	m_bCaptureScreenMode = FALSE;
	m_nAudioIndex = -1;
	m_nScreenAudioIndex =		-1;
	m_nVideoIndex = -1;
	m_bDisConnected = FALSE;
	m_bWarnLow = FALSE;
	m_bDragingRect = FALSE;
	m_bDragingLine = FALSE;
	m_bDragingPointer = FALSE;
	m_bFreeHand = FALSE;

	GetPenProperty();

	m_pPointerBMP = NULL;
	m_pimageList = NULL;

	ReloadBitmap();

	m_bCaptureMode = TRUE;
	m_pWndHTML = NULL;
	m_bUsePPT = FALSE;
	m_bUseHTML = FALSE;
	m_nHTMLIndex = 1;
	m_bRecPath = FALSE;
	m_bWithScreenProfileIni = FALSE;
	m_lStartTime = 0;
	m_lCurrTime = -1;
	m_lCurrID = 0;
	m_pFreedraw = NULL;
	m_nMaxIndex = 0;
    m_dlgVolumeBAr=NULL; //wwangwang
  	dlgclock=NULL;
//2005-11-01
	m_bNG = FALSE;
	 m_hasVideo=false;
}

CCtrlPanelWnd::~CCtrlPanelWnd()
{
	delete m_pimageList;

	if (m_pPointerBMP)
	{
		m_pPointerBMP->DeleteObject();
		delete m_pPointerBMP;
	}

	RemoveAllPageMedias();

	delete m_pPageMedias;
	
	delete m_pBtnClose;

	delete m_pBtnCapture;
	delete m_pBtnRecDemo;
	delete m_pBtnStopDemo;

	delete m_pBtnDemo;
	delete m_pBtnView;

	delete m_pBtnRec;
	delete m_pBtnPause;
	delete m_pBtnStop;
	delete m_pBtnNG;

	delete m_pBtnFirst;
	delete m_pBtnPrev;
	delete m_pBtnNext;
	delete m_pBtnLast;
	delete m_pBtnGoto;

/*	del itoh 2013/07/24 PPT2013
	delete m_pBtnRect;
	delete m_pBtnLine;
	delete m_pBtnPointer;
	delete m_pBtnFreeHand;
	delete m_pBtnEraser;
	delete m_pBtnPenSet;
*/
	delete m_pBtnZoomIn;
	delete m_pBtnZoomOut;

	delete m_pBtnPreview;

	delete m_pBmpLabel1;
	delete m_pBmpLabel2;
//	delete m_pBmpLabel3;
	delete m_pBmpLabel4;
	delete m_pBmpLabel5;
	delete m_pBmpLabel6;

	delete m_pBmpStaticBlue;
	delete m_pBmpStaticRed;
	delete m_pBmpStaticOrange;
	delete m_pBmpStaticYellow;

	delete m_pPageNoEdit;
	delete m_pTimeStatic;

	if (m_pSysFont != NULL)
	{
		m_pSysFont->DeleteObject();
		delete m_pSysFont;
	}

	delete m_pPreviewWnd;
	delete m_pWndHTML;
		
	if (m_dlgVolumeBAr!=NULL) //wwangwang
      delete m_dlgVolumeBAr;

	if (dlgclock!=NULL)
      delete dlgclock;
	
	if (m_hmodHook)
		FreeLibrary(m_hmodHook);
} 

void CCtrlPanelWnd::DoDataExchange(CDataExchange* pDX)
{
//	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeBarTesterDlg)
//	DDX_Control(pDX, IDC_VOLUME_VERTICAL, m_VolumeVertical);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCtrlPanelWnd, CWnd)
	//{{AFX_MSG_MAP(CCtrlPanelWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_MESSAGE(WM_BUTTON_DOWN, OnButtonDown)
	ON_MESSAGE(WM_ENTER_PRESSED, OnEnterPressed)
	ON_MESSAGE(WM_CLOSE_PREVIEW, OnClosePreview)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_USER_CAPTURE_STATUS, OnUserStatusHandler)
	ON_MESSAGE(WM_USER_CAPTURE_ERROR, OnUserErrorHandler)
	ON_MESSAGE(WM_ACCEPT_ERR, OnAcceptError)
	ON_MESSAGE(WM_NEXT_SLIDE, OnNextSlide)
	ON_MESSAGE(WM_PREVIOUS_SLIDE, OnPreviousSlide)
	ON_MESSAGE(WM_FIRST_SLIDE, OnFirstSlide)
	ON_MESSAGE(WM_LAST_SLIDE, OnLastSlide)
	ON_MESSAGE(WM_END_SLIDE, OnEndSlide)
	ON_MESSAGE(WM_LBTN_DOWN, OnLBtnDown)
	ON_MESSAGE(WM_MOUSEMOVE_EX, OnMouseMoveEx)
	ON_MESSAGE(WM_LBUTTONUP_EX, OnLBtnUpEx)
	ON_MESSAGE(WM_SET_AUDIO_PREVIEW, OnSetAudioPreview)
	ON_MESSAGE(WM_GET_AUDIO_PREVIEW, OnGetAudioPreview)
	ON_MESSAGE(WM_KEY_PRESSED, OnKeyPressed)
	ON_MESSAGE(WM_USER_CountDownOver_TIMER, OnWM_USER_CountDownOver_TIMER)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCtrlPanelWnd::ReloadBitmap()
{
	CString strIniPath = ::GetSysPath() + "Elearning.ini";

	char szBuf[256];
	::GetPrivateProfileString("PenProperty", "Pointer", "0", szBuf, 256, strIniPath);

	if (m_pPointerBMP)
	{
		m_pPointerBMP->DeleteObject();
		delete m_pPointerBMP;
	}

	if (m_pimageList)
		delete m_pimageList;

	m_pPointerBMP = new CBitmap;
	m_nID = (UINT)atoi(szBuf);
	if (m_nID == 0)
		m_nID = IDB_PTR_FIRST;
	m_pPointerBMP->LoadBitmap(m_nID);

	BITMAP b;
	m_pPointerBMP->GetBitmap(&b);
	m_szPointer.cx = b.bmWidth;
	m_szPointer.cy = b.bmHeight;

	m_pimageList = new CImageList;
	m_pimageList->Create(m_szPointer.cx, m_szPointer.cy, ILC_COLORDDB | ILC_MASK, 1, 1);
	m_pimageList->Add(m_pPointerBMP, RGB(255, 0, 255));
}

void CCtrlPanelWnd::RemoveAllPageMedias()
{
	for (int i = 0; i < m_pPageMedias->GetSize(); i++)
	{
		CPageMedia* p = (CPageMedia*)m_pPageMedias->GetAt(i);
		CObArray* pRect = p->m_pRects;
		for (int j = 0; j < pRect->GetSize(); j++)
		{
			CRect* prc = (CRect*)pRect->GetAt(j);
			delete prc;
		}
		pRect->RemoveAll();
		delete pRect;

		delete p;
	}
	m_pPageMedias->RemoveAll();
}

BOOL CCtrlPanelWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	CString strTitle;
	strTitle.LoadString(IDS_TITLE);
	if (m_strClass == "")
	{
		HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRM);
		m_strClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
										 AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
										 0, hIcon);
	}
	CString strClass = ((CELearningCaptureApp*)AfxGetApp())->GetClassName();
	BOOL bRet = CreateEx(strClass, strTitle, dwStyle, rect, pParentWnd, 0, NULL);

	return bRet;
}

BOOL CCtrlPanelWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), 
						(pParentWnd) ? pParentWnd->m_hWnd: NULL, 
						(HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCtrlPanelWnd message handlers

void CCtrlPanelWnd::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
	// this->SetFocus();
}

int CCtrlPanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bAudioOnly = AudioOnly();

	if (!m_pSysFont->CreateFont(-10,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_ROMAN,
							"Arial"))
	{
		m_pSysFont = NULL;
		return -1;
	}

	CRect rect;
	rect.SetRect(2, 2, -1, -1);
	m_pBtnClose->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_CLOSE);

	int nX = m_nCloseBtnWidth + 3;
	CSize cs = GetBmpDim(IDB_FIRST);
	int nWidth = cs.cx / 4;
	CSize csSep = GetBmpDim(IDB_SEP);

	m_nTop = constSmallGap;
	m_nBottom = constSmallGap + cs.cy;

	rect.SetRect(nX + constLargeGap, constSmallGap, 
				 nX + constLargeGap + nWidth, constSmallGap + cs.cy);
	m_pBtnView->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_VIEW);

	CRect rect2 = rect;

#ifndef _BUNDLE
	m_pBtnRecDemo->Create(WS_CHILD, rect2, this, ID_BTN_REC_DEMO);

//	rect.left = rect.right;														//del 2013/9/19 kagata PPT2013
//	rect.right = rect.left + nWidth;
//	m_pBtnPreview->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PREVIEW);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel1->Create(WS_CHILD | WS_VISIBLE, rect, this);
#endif

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBtnRec->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_REC);

//	rect.left = rect.right + constSmallGap;
//	rect.right = rect.left + nWidth;
//	m_pBtnNG->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_NG);		//del 2013/9/19 kagata PPT2013

#ifndef _BUNDLE
	rect2.left = rect2.right;
	rect2.right = rect2.left + nWidth;
	m_pBtnStopDemo->Create(WS_CHILD, rect2, this, ID_BTN_STOP_DEMO);
	m_pBtnStopDemo->SetButtonDown(TRUE);

	rect2.left = rect2.right + constSmallGap + 2;
	rect2.right = rect2.left + csSep.cx;
	m_pBmpLabel4->Create(WS_CHILD, rect2, this);
#endif

	rect2.left = rect2.right + constSmallGap + 2;
	rect2.right = rect2.left + nWidth;
	m_pBtnCapture->Create(WS_CHILD, rect2, this, ID_BTN_CAPTURE);

//	rect.left = rect.right;													//del 2013/9/19 kagata PPT2013
//	rect.right = rect.left + nWidth;
//	m_pBtnPause->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PAUSE);
//	m_pBtnPause->EnableButton(FALSE);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnStop->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_STOP);
	m_pBtnStop->SetButtonDown(TRUE);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel2->Create(WS_CHILD | WS_VISIBLE, rect, this);

#ifndef _BUNDLE
	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBtnFirst->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_FIRST);
#endif
	
	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnPrev->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PREV);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnNext->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_NEXT);

#ifndef _BUNDLE
	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnLast->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LAST);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnGoto->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_GOTO);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + constEditWidth;
	m_pPageNoEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rect, this, ID_PAGE_NO_EDIT);
	m_pPageNoEdit->SetFont(m_pSysFont);
/*
	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel3->Create(WS_CHILD | WS_VISIBLE, rect, this);
*/
/* del itoh 2013/07/24 PPT2013
	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBtnRect->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_RECT);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnLine->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LINE);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnFreeHand->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_FREEHAND);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnPointer->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_POINTER);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnEraser->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ERASER);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnPenSet->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PENSET);
*/	
	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel6->Create(WS_CHILD | WS_VISIBLE, rect, this);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBtnZoomIn->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ZOOM_IN);

	rect.left = rect.right;
	rect.right = rect.left + nWidth;
	m_pBtnZoomOut->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_ZOOM_OUT);
	m_pBtnZoomOut->EnableButton(FALSE);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel5->Create(WS_CHILD | WS_VISIBLE, rect, this);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBtnDemo->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_DEMO);

	m_nShortBtnEnd = rect2.right;
	m_nLongBtnEnd = rect.right;

	// add itoh 2013/02/13 start
	// Get win ver
    OSVERSIONINFO OSver; 
    OSver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    GetVersionEx(&OSver); 

/*	Disable Annotation on windows8 or PPT2013
    if((OSver.dwMajorVersion == 6 && OSver.dwMinorVersion >= 2)) 
    { 
		m_pBtnRect->EnableButton(FALSE);
		m_pBtnLine->EnableButton(FALSE);
		m_pBtnFreeHand->EnableButton(FALSE);
		m_pBtnPointer->EnableButton(FALSE);
		m_pBtnEraser->EnableButton(FALSE);
		m_pBtnPenSet->EnableButton(FALSE);
	}	// del itoh 2013/07/23 PPT2013
*/
	// del end itoh 2013/02/13 Windows8
#else
	CRect rcTmp;
	rcTmp.SetRectEmpty();
	m_pPageNoEdit->Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, rcTmp, this, ID_PAGE_NO_EDIT);
#endif
    
   

	rect.left = rect.right + constSmallGap + 2;
	rect.right = rect.left + constTimeWidth;
	m_pTimeStatic->SetText("00:00:00", FALSE);
	// _T("STATIC"), "Hi",

	m_pTimeStatic->CreateEx(NULL,"HI",WS_CHILD | WS_VISIBLE, rect, this,ID_TIME_DISPLAY,NULL);
	//CreateEx
//	m_pTimeStatic->Create(WS_CHILD | WS_VISIBLE, rect, this,ID_TIME_DISPLAY);
	m_pTimeStatic->SetDrawFont(m_pSysFont);

/* wangwang Volume Display Begins here
	rect.left = rect.right+constSmallGap;
	rect.left = rect.right+2;
    rect.right = rect.left + 50;
	m_VolumeHorizontal.Create(NULL, WS_CHILD| WS_VISIBLE, rect, this);

   m_VolumeHorizontal.Create(NULL,WS_CHILD | WS_VISIBLE, rect, this);
*/
//wangwang Volume Display ends here




	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBmpStaticBlue->Create(WS_CHILD | WS_VISIBLE, rect, this);
	m_pBmpStaticRed->Create(WS_CHILD, rect, this);
	m_pBmpStaticOrange->Create(WS_CHILD, rect, this);
	m_pBmpStaticYellow->Create(WS_CHILD, rect, this);

	EnablePPTBtns(FALSE);

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	int nW = rect.right + constLargeGap + constMediumGap;
	int nH = cs.cy + constSmallGap * 2 + 6;
	
    ((CELearningCaptureApp*)AfxGetApp())->m_wndLeft=nScreenX - nW;
	((CELearningCaptureApp*)AfxGetApp())->m_wndTop=nScreenY - nH;
	((CELearningCaptureApp*)AfxGetApp())->m_wndHeight=nH; 




	if(FAILED(CoInitialize(NULL))) 
	{
		W_SHOW_ERROR(IDS_COM_INIT_ERR, this);
		return -1;
	}

	m_nTimerID = SetTimer(1, 500, NULL);
	m_nTimerID2 = SetTimer(99, 50, NULL);



	
    if (!m_hmodHook)
    {
		if (!(m_hmodHook = LoadLibrary("HookProc.dll")))
	    {
            W_SHOW_ERROR(IDS_CANNOT_LOAD_DLL, this);
			return -1;
        }
		SetHook();
    }

 	if (!m_bCaptureScreenMode)
	{
//		m_pPPT = new CPPT9Ctrl;
		m_pPPT = new CPPT15Ctrl;
		if (!m_pPPT->Create())
		{
			W_SHOW_ERROR(IDS_PPT_OLE_NOT_FOUND, this);
			return -1;		
		}

		if (!InitForVideoCapture())
			return -1;
		EnableTaskBar(FALSE);

		CString strIniPath = m_strPath + "Course.ini";
		char szBuf[256];
		::GetPrivateProfileString("General", "PPTFile", "", szBuf, 255, strIniPath);
		if (szBuf[0] != '\0')
		{
			m_bUsePPT = TRUE;
			CString strPPTPath = m_strPath + szBuf;
			if (!OpenPPTFile(strPPTPath))
			{
				W_SHOW_ERROR(IDS_PPT_FILE_NOT_EXIST, this);
				return -1;
			}
		}
		else
		{
			CString strWebPage = m_strPath + "webpages";
			if (::FileExists(strWebPage))
			{
				m_bUseHTML = TRUE;
				LoadHTMLData();
				OpenHTMLFile(strWebPage);
			}
		}
	}
	else
	{
		ChangeToRecMode(FALSE);
		m_pBtnCapture->EnableButton(FALSE);
	}
	
#ifdef _BUNDLE
	m_pBtnZoomIn->EnableButton(FALSE);
	m_pBtnZoomOut->EnableButton(FALSE);
	m_pBtnDemo->EnableButton(FALSE);
#endif


	if (!m_bCaptureScreenMode && m_strPath.Right(1) == "\\")
	{
		m_pFreedraw = new CFreedraw();
		CString strPath = m_strPath + "free.tmp";
		if(!m_pFreedraw->Open((LPTSTR)(LPCTSTR)strPath)) 
		{
			delete m_pFreedraw;
			return -1;
		}
	}
//	AfxMessageBox("Volume create0");
   if(!m_bCaptureScreenMode)
   {
	   	#ifndef _VISTA
		    m_dlgVolumeBAr->SendMessage(WM_USER_Volume_TIMER);
	
			m_dlgVolumeBAr = new CVolumeBarTesterDlg();
		   //Check if new succeeded and we got a valid pointer to a dialog object
		   if(m_dlgVolumeBAr != NULL)
		   {
			  BOOL ret = m_dlgVolumeBAr->Create(IDD_VOLUMEBARTESTER_DIALOG,this);
			   //AfxMessageBox("Volume create");
			  if(!ret)   //Create failed.
				 AfxMessageBox("Error creating Dialog");
			  m_dlgVolumeBAr->ShowWindow(SW_SHOW);
			  //dlg->ShowWindow(SW_HIDE);
		   }
		   else
			  AfxMessageBox("Error Creating Dialog Object");
	   #endif;
      // AfxMessageBox("Volume create");

	   dlgclock = new CDLG_COUNT_DOWN();
	   //Check if new succeeded and we got a valid pointer to a dialog object
	  // CDLG_COUNT_DOWN* dlgclock;
		 if(dlgclock != NULL)
	   {
		  BOOL ret = dlgclock->Create(IDD_COUNT_DOWN,this);
		  if(!ret)   //Create failed.
			 AfxMessageBox("Error creating Dialog");
		  //dlgclock->ShowWindow(SW_SHOW);
		  dlgclock->ShowWindow(SW_HIDE);
	   }
	   else
		  AfxMessageBox("Error Creating Dialog Object");
   
        //AfxMessageBox("Volume create1");
   		SetWindowPos(&wndTop, nScreenX - nW, nScreenY - nH, nW, nH, SWP_SHOWWINDOW);  
		BringWindowToTop();
		SetForegroundWindow();
		SetFocus();
		RedrawWindow();
   
		if (m_hasVideo)
		{
		  //SendMessage(ID_BTN_VIEW);  
		  //StartPreview();
		  //  AfxMessageBox("Has Camera");
			OnView();
		//	Sleep(1000);
		//	m_pPreviewWnd->SendMessage(WM_SIZE);
		}
		return 0;
		//AfxMessageBox("Volume create2");
		
     }
     //m_nTimerID4 = SetTimer(667, 2000, NULL);
    //wangwang20080304
     
//wanwang
	/*
m_ToolTip.Create (this);
m_ToolTip.Activate (TRUE);
 
CWnd*    pWnd = GetWindow (GW_CHILD);
while (pWnd)
{
    int nID = pWnd->GetDlgCtrlID ();
    if (nID != -1)
    {
        m_ToolTip.AddTool (pWnd, pWnd->GetDlgCtrlID ());
    }
    pWnd = pWnd->GetWindow (GW_HWNDNEXT);
}
*/


   
    //m_Screen2Video1.SetLicenseKey("10850 single developer license");
	//CAudioDevices audiodevice = m_Screen2Video1.GetAudioDevices();
	
	//int iAudioDeviceCount = audiodevice.GetCount();
	//for(int i=0; i < iAudioDeviceCount; i++)
	//	m_CboAudioDevice.AddString(audiodevice.FindDeviceName(i));
	//m_CboAudioDevice.SetCurSel(0);
	return 0;
}



void CCtrlPanelWnd::LoadHTMLData()
{
	CString strPlaylistFile = m_strPath + "webpages\\index.pll";
	m_nHTMLCount = ::GetPrivateProfileInt("General", "Count", 0, strPlaylistFile);
	CString strSec, strTmp;
	char szBuffer[256];
	for (int i = 0; i < m_nHTMLCount; i++)
	{
		strSec.Format("Page%d", i);
		::GetPrivateProfileString(strSec, "Title", "", szBuffer, 256, strPlaylistFile);
		m_strArrayTitle.Add(szBuffer);

		::GetPrivateProfileString(strSec, "Path", "", szBuffer, 256, strPlaylistFile);
		m_strArrayHTMLPath.Add(szBuffer);
	}
}

BOOL CCtrlPanelWnd::InitForVideoCapture()
{
	if (!CreateInterface())
		return FALSE;
	
	if (!OpenCapture(FALSE))
		return FALSE;

	if (!SetDevice())
		return FALSE;

	return TRUE;
}

BOOL CCtrlPanelWnd::InitForScreenCapture()
{
	if (!CreateInterface())
		return FALSE;
	
	if (!OpenCapture(TRUE))
		return FALSE;

	if (!OpenScreenDevice())
		return FALSE;

	return TRUE;
}

BOOL CCtrlPanelWnd::CleanUpForCapture()
{
	CloseDevice();
	ReleaseInterface();
	return TRUE;
}

void CCtrlPanelWnd::EnablePPTBtns(BOOL bFlag)
{
	m_pBtnPrev->EnableButton(bFlag);
	m_pBtnFirst->EnableButton(bFlag);
	m_pBtnNext->EnableButton(bFlag);
	m_pBtnLast->EnableButton(bFlag);
	m_pBtnGoto->EnableButton(bFlag);
	m_pPageNoEdit->EnableWindow(bFlag);
}

void CCtrlPanelWnd::BringToTop() 
{
	BringWindowToTop();
	SetForegroundWindow();
}

/*
BOOL CCtrlPanelWnd::CreatePPTCtrl() 
{
	HRESULT hval = ::CoCreateInstance(CLSID_PPTUtil, NULL, CLSCTX_ALL, IID__PPTUtil, (void**)&m_pPowerControl);
	if (FAILED(hval))
	{
		SHOW_ERROR(IDS_NO_PPT_CTRL);
		return FALSE;
	}
	return TRUE;
}

void CCtrlPanelWnd::ClosePPTCtrl() 
{
	if (m_pPowerControl != NULL) 
		m_pPowerControl->Release();

	m_pPowerControl = NULL;
}
*/

BOOL CCtrlPanelWnd::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC dc(pDC);
	
	CRect rc;
	GetClientRect(rc);

	CBrush brBg;
	brBg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rc, &brBg);
	brBg.DeleteObject();
	
	rc.right = rc.left + m_nCloseBtnWidth + 3;

	CBrush brCaption;
	HWND hWndFocus = ::GetFocus();
	if (hWndFocus == m_hWnd)
		brCaption.CreateSolidBrush(::GetSysColor(COLOR_ACTIVECAPTION));
	else
		brCaption.CreateSolidBrush(::GetSysColor(COLOR_INACTIVECAPTION));
	dc.FillRect(rc, &brCaption);
	brCaption.DeleteObject();

	return TRUE;
}

LRESULT CCtrlPanelWnd::OnKeyPressed(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime == wParam)
		return 0L;

	m_lTime = wParam;

	if (!m_bCaptureMode)
	{
		if (m_nState == RECORDING)
		{
			m_pBtnStopDemo->SetButtonDown(TRUE);
			OnStopDemo();
		}
		ShowWindow(SW_SHOWNORMAL);
	}

	return 0L;	
}

LRESULT CCtrlPanelWnd::OnSetAudioPreview(WPARAM wParam, LPARAM lParam)
{
	//if (m_pEasyCapture == NULL) 
	//	return 0L;

	//if (m_bPreviewAudioMute)
	//	m_pEasyCapture->SetupPreviewAudioMute(OAFALSE);
	//else
	//	m_pEasyCapture->SetupPreviewAudioMute(OATRUE);
	//m_bPreviewAudioMute = !m_bPreviewAudioMute;
	//m_pPreviewWnd->m_bPreviewAudioMute = m_bPreviewAudioMute;

	return 0L;
}

LRESULT CCtrlPanelWnd::OnGetAudioPreview(WPARAM wParam, LPARAM lParam)
{
	m_pPreviewWnd->m_bPreviewAudioMute = m_bPreviewAudioMute;
	return 0L;
}

void CCtrlPanelWnd::GetPenProperty()
{	
	CString strIniPath = ::GetSysPath() + "Elearning.ini";

	char szBuf[256];
	::GetPrivateProfileString("PenProperty", "Width", "2px", szBuf, 256, strIniPath);
	sscanf(szBuf, "%dpx", &m_nWidth);

	CString strR, strG, strB;
	::GetPrivateProfileString("PenProperty", "ColorR", "0", szBuf, 256, strIniPath);
	strR = szBuf;
	::GetPrivateProfileString("PenProperty", "ColorG", "0", szBuf, 256, strIniPath);
	strG = szBuf;
	::GetPrivateProfileString("PenProperty", "ColorB", "0", szBuf, 256, strIniPath);
	strB = szBuf;
	m_color = RGB(atoi(strR), atoi(strG), atoi(strB));
}

LRESULT CCtrlPanelWnd::OnLBtnUpEx(WPARAM wParam, LPARAM lParam)
{
	if (m_bDragingRect)
	{
		DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);
		DrawRect(m_rect, m_color, m_nWidth, FALSE);
		OnDrawMark(1);
	}
	else if (m_bDragingLine)
	{
		DrawLine(m_rect, RGB(255, 255, 255), 2, TRUE);
		DrawLine(m_rect, m_color, m_nWidth, FALSE);
		OnDrawMark(0);
	}
	else if (m_bDragingPointer)
	{
		DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);

		CWnd wnd;
		if (m_bUsePPT)
			wnd.Attach(m_hWndPane);
		else if (m_bUseHTML && m_pWndHTML)
		{
			HWND hWnd = m_pWndHTML->GetIEWnd();
			wnd.Attach(hWnd);
		}
		CDC* pDC = wnd.GetDC();
		m_pimageList->Draw(pDC, 0, CPoint(m_rect.left, m_rect.top), ILD_TRANSPARENT);
		wnd.ReleaseDC(pDC);
		wnd.Detach();
		OnDrawMark(2);
	}
	else if (m_bFreeHand)
	{
		m_bFreeHand = FALSE;
		if (m_pFreedraw)
			m_pFreedraw->StopDraw();
	}

	m_rect.SetRectEmpty(); 
	m_bDragingRect = FALSE;
	m_bDragingLine = FALSE;
	m_bDragingPointer = FALSE;

	return 0L;
}

LRESULT CCtrlPanelWnd::OnMouseMoveEx(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	point.x  = LOWORD(lParam);   
	point.y  = HIWORD(lParam);  
	if (m_bDragingRect)
	{
		m_rect.right = point.x;
		m_rect.bottom = point.y;

		if (m_rect != m_rectOld)
		{
			DrawRect(m_rectOld, RGB(255, 255, 255), 1, TRUE);
			DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);
			m_rectOld = m_rect;
		}
	}
	else if (m_bDragingLine)
	{
		m_rect.right = point.x;
		m_rect.bottom = point.y;

		if (m_rect != m_rectOld)
		{
			DrawLine(m_rectOld, RGB(255, 255, 255), 2, TRUE);
			DrawLine(m_rect, RGB(255, 255, 255), 2, TRUE);
			m_rectOld = m_rect;
		}
	}
	else if (m_bDragingPointer)
	{
		int nx = m_szPointer.cx / 2;
		int ny = m_szPointer.cy / 2;
		m_rect.SetRect(point.x - nx, point.y - ny, point.x + nx, point.y + ny);

		if (m_rect != m_rectOld)
		{
			DrawRect(m_rectOld, RGB(255, 255, 255), 1, TRUE);
			DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);
			m_rectOld = m_rect;
		}
	}
	else if (m_bFreeHand)
	{
		m_rect.right = point.x;
		m_rect.bottom = point.y;
		DrawLine(m_rect, m_color, m_nWidth, FALSE);
		m_rect.left = point.x;
		m_rect.top = point.y;
		m_rectOld = m_rect;

		if (m_pFreedraw && (point.x >= 0) && (point.y >= 0)) 
		{
			if (m_pFreedraw->IsDrawed()) 
				m_pFreedraw->AddPoint(m_lCurrTime - m_lStartTime, (WORD)point.x, (WORD)point.y);
		}
	}
	
	return 0L;
}

void CCtrlPanelWnd::DrawRect(LPRECT lpRect, COLORREF color, int nWidth, BOOL bEx)
{   
	HDC hDC = ::GetDC(NULL);
	::SetROP2(hDC, (bEx) ? R2_XORPEN : R2_COPYPEN);

   	HPEN Pen1;                                      
	Pen1 = ::CreatePen(PS_SOLID, nWidth, color);
  	
	HPEN OldPen;
	OldPen = (HPEN)::SelectObject(hDC, Pen1);

	::MoveToEx(hDC, lpRect->left, lpRect->top, NULL);
	::LineTo(hDC, lpRect->left, lpRect->bottom);
	::LineTo(hDC, lpRect->right, lpRect->bottom); 
	::LineTo(hDC, lpRect->right, lpRect->top); 
	::LineTo(hDC, lpRect->left, lpRect->top);
    
	::SelectObject(hDC, OldPen);

    ::DeleteObject(Pen1);

	::ReleaseDC(NULL, hDC);
} 

void CCtrlPanelWnd::DrawLine(LPRECT lpRect, COLORREF color, int nWidth, BOOL bEx)
{   
	HDC hDC = ::GetDC(NULL);
	::SetROP2(hDC, (bEx) ? R2_XORPEN : R2_COPYPEN);

   	HPEN Pen1;                
	Pen1 = ::CreatePen(PS_SOLID, nWidth, color);
	HPEN OldPen;
	OldPen = (HPEN)::SelectObject(hDC, Pen1);

	::MoveToEx(hDC, lpRect->left, lpRect->top, NULL);
	::LineTo(hDC, lpRect->right, lpRect->bottom);
    
	::SelectObject(hDC, OldPen);

    ::DeleteObject(Pen1);
	::ReleaseDC(NULL, hDC);
} 

LRESULT CCtrlPanelWnd::OnLBtnDown(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime == wParam)
		return 0L;

	m_lTime = wParam;
		
//W_ASK_YESNO(ID_BTN_NEXT, this);	// ito test PPT2013
	int xPos = LOWORD(lParam);  
	int yPos = HIWORD(lParam); 
	CPoint point(xPos, yPos);
	if (IsPointInMedia(point))
	{
		if (m_hWndPane)
			::PostMessage(m_hWndPane, WM_LBUTTONDOWN, 987654321, lParam);
	}
	else
	{
/* del itoh 2013/07/24 PPT2013
		if (IsRectMode())
		{
			m_bDragingRect = TRUE;

			m_rect.SetRect(point.x, point.y, point.x, point.y);
			m_rectOld = m_rect;

			DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);
		}
		else if (IsLineMode())
		{
			m_bDragingLine = TRUE;

			m_rect.SetRect(point.x, point.y, point.x, point.y);
			m_rectOld = m_rect;

			DrawLine(m_rect, RGB(255, 255, 255), 2, TRUE);
		}
		else if (IsPointerMode())
		{
			m_bDragingPointer = TRUE;
			int nx = m_szPointer.cx / 2;
			int ny = m_szPointer.cy / 2;
			m_rect.SetRect(point.x - nx, point.y - ny, point.x + nx, point.y + ny);
			m_rectOld = m_rect;

			DrawRect(m_rect, RGB(255, 255, 255), 1, TRUE);
		}
		else if (IsFreeHandMode())
		{
			m_bFreeHand = TRUE;
			m_rect.SetRect(point.x, point.y, point.x, point.y);
			m_rectOld = m_rect;
			DrawLine(m_rect, m_color, m_nWidth, FALSE);

			if (m_pFreedraw && (point.x >= 0) && (point.y >= 0)) 
			{
				if (!m_pFreedraw->StartDraw(m_lCurrID, m_color, m_nWidth))
					return 0L;

				if (!m_pFreedraw->AddPoint(0, (WORD)point.x, (WORD)point.y))
					return 0L;

				OnDrawMark(3);

				m_lCurrID++;
				m_lStartTime = m_lCurrTime;
			}
		}
		else
*/
		OnNext();
	}

	return 0L;
}

LRESULT CCtrlPanelWnd::OnFirstSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnFirst();
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnLastSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnLast();
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnEndSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		if (m_nState == RECORDING)
		{
			if (W_ASK_YESNO(IDS_STOP_REC_AND_CLOSE_PPT_NOW, this) == IDYES)
			{
				OnStop();
				PPTClose(FALSE);
			}
		}	
		else
		{
			if (W_ASK_YESNO(IDS_CLOSE_PPT_NOW, this) == IDYES)
				PPTClose(FALSE);
		}
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnNextSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnNext();
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnPreviousSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnPrev();
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnEnterPressed(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	BringToTop();	

	CString strPageNo;
	m_pPageNoEdit->GetWindowText(strPageNo);
	
	strPageNo.TrimLeft();
	if (strPageNo == "")
		return 0L;

	int nPageNo = atoi(strPageNo);
	if (nPageNo <= PPTTotalSlideNo())
		PPTGoto(atoi(strPageNo));
	else
	{
		long nCurSlideNo = PPTCurSlideNo();
		char szBuffer[32];
		ltoa(nCurSlideNo, szBuffer, 10);
		m_pPageNoEdit->SetWindowText(szBuffer);
	}

	return 0L;
}

LRESULT CCtrlPanelWnd::OnClosePreview(WPARAM wParam, LPARAM lParam)
{
	ClosePreviewWnd();
	return 0L;
}

void CCtrlPanelWnd::ClosePreviewWnd()
{
	if (!m_pPreviewWnd)
		return;

	DetachVideo();

	m_pPreviewWnd->DestroyWindow();
	delete m_pPreviewWnd;
	m_pPreviewWnd = NULL;
}

BOOL CCtrlPanelWnd::OpenHTMLFile(CString strWebPath)
{
	if (!::FileExists(strWebPath))
		return FALSE;
	
	if (!m_pWndHTML)
	{
		m_pWndHTML = new CHtmlWnd;
		CRect rect;
		rect.left = rect.top = 0;
		rect.right = ::GetSystemMetrics(SM_CXSCREEN) - 1;
		rect.bottom = ::GetSystemMetrics(SM_CYSCREEN) - 1;
		m_pWndHTML->Create(m_strClass, "", WS_POPUP | WS_VISIBLE, NULL, rect, NULL, 0, NULL);
	}

	GotoHTMLPage(1);
	m_nHTMLIndex = 1;
	SetSlideNo();
	EnablePPTBtns(TRUE);
	m_bOpen = TRUE;

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);  
	BringWindowToTop();
	SetForegroundWindow();
	SetFocus();

	m_pWndHTML->SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);  

	return TRUE;
}

BOOL CCtrlPanelWnd::OpenPPTFile(CString strPPTPath)
{
	if (!::FileExists(strPPTPath))
		return FALSE;

	m_strPPTPath = strPPTPath;

	PPTOpen();
	PPTSaveMediaPos();
	LoadMediaFile();
	SetSlideNo();
	m_pPPT->GetSliderTitle(m_strArrayTitle);
	
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);  
	BringWindowToTop();
	SetForegroundWindow();
	SetFocus();

	return TRUE;
}

void CCtrlPanelWnd::OnRecDemo()
{
	CString strDemoFile;
	if (m_bRecPath)
		strDemoFile = m_strHTMLPath + m_strDemoFile;
	else
		strDemoFile = m_strPath;
	if (::FileExists(strDemoFile))
	{
		if (W_ASK_YESNO(IDS_RECAPTURE, this) == IDNO)
		{
			m_pBtnRecDemo->SetButtonDown(FALSE);
			return;
		}
	}

	m_pBtnStopDemo->SetButtonDown(FALSE);
	UpdateWindow();

	m_pBmpStaticBlue->ShowWindow(SW_HIDE);
	m_pBmpStaticRed->ShowWindow(SW_SHOW);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_HIDE);

	ShowWindow(SW_MINIMIZE);

	m_nTickCount = GetTickCount();
	StartCaptureScreen(strDemoFile); 
	m_nState = RECORDING;
}



void CCtrlPanelWnd::OnStopDemo()
{
	m_pBtnRecDemo->SetButtonDown(FALSE);

	m_pBmpStaticBlue->ShowWindow(SW_SHOW);
	m_pBmpStaticRed->ShowWindow(SW_HIDE);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_HIDE);

	UpdateWindow();
	StopCapture();
	
	AddDemoTag();

	m_nState = STOPPED;
	m_nBaseMS = 0;
	m_nTickCount = 0;
}

void CCtrlPanelWnd::ResetButtons()
{
	m_pBtnPause->SetButtonDown(FALSE);
	m_pBtnPause->EnableButton(FALSE);
	m_pBtnRec->SetButtonDown(FALSE);
	m_pBtnNG->SetButtonDown(FALSE);
	m_pBtnStop->SetButtonDown(TRUE);
	m_pBtnZoomOut->EnableButton(FALSE);
	m_pBmpStaticBlue->ShowWindow(SW_SHOW);
	m_pBmpStaticRed->ShowWindow(SW_HIDE);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_HIDE);

#ifdef _BUNDLE
	m_pBtnZoomIn->EnableButton(FALSE);
#else
	m_pBtnZoomIn->EnableButton(TRUE);
#endif
}

void CCtrlPanelWnd::OnStop()
{
	if (m_nState == STOPPED)
		return;

	OnAddDemo();

	MoveToStepArray();

	ResetButtons();

	UpdateWindow();

	StopCapture();
	StopPreview();

	WriteStepFile();
	WriteNGFile();

	m_nState = STOPPED;
	m_nBaseMS = 0;
	m_nTickCount = 0;
	//m_nIndex = 0;
}

BOOL CCtrlPanelWnd::GetWidthHeight(long& nWidth, long& nHeight)
{
	nWidth = 160; 
	nHeight = 120;

	//if (!m_pEasyCapture)
	//	return FALSE;

	//HRESULT hval = 0;
	//hval = m_pEasyCapture->DisplayGetReferSize(&nWidth, &nHeight);
	//if (FAILED(hval)) 
	//{
	//	CommonFuncErrMsg("IEasyCapture::DisplayGetReferSize");
	//	return FALSE; 
	//}
	
	return TRUE;
}

void CCtrlPanelWnd::OnPreview()
{
	CString strAsf;
	if (m_strPath == "")
		strAsf = "C:\\TEST.wmv";
	else
		strAsf = m_strMediaPath + "original.wmv";
	
	if (m_nState != STOPPED)
	{
		CString strMsg;
		strMsg.LoadString(IDS_ACTING);
		if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
		{
			OnStop();
		}
		else
			return;
	}

	if (::FileExists(strAsf))
		::ShellExecute(this->m_hWnd, "open", strAsf, NULL, NULL, SW_SHOWNORMAL);
}

CString CCtrlPanelWnd::GetTimeStamp()
{
	DWORD dwTick = GetTickCount();
	CString strTime = ConvertMSToStr(m_nBaseMS + dwTick - m_nTickCount, TRUE);	
	return strTime;
}

void CCtrlPanelWnd::OnFirst()
{
	if (MACRO_APP->m_bMessageBox)
		return;

	if (m_bUsePPT)
	{
		MoveToStepArray();
		PPTGoto(1);
	}
	else
	{
		MoveToStepArray();
		HTMLGoto(1);
	}
}

void CCtrlPanelWnd::OnLast()
{
	if (MACRO_APP->m_bMessageBox)
		return;

	if (m_bUsePPT)
	{
		MoveToStepArray();
		PPTLast();
	}
	else
	{
		MoveToStepArray();
		HTMLGoto(m_strArrayHTMLPath.GetSize());
	}
}

void CCtrlPanelWnd::OnNext()
{
	if (MACRO_APP->m_bMessageBox)
		return;

	if (m_bUsePPT)
	{
		MoveToStepArray();
		PPTNext();
	}
	else if (m_bUseHTML)
	{
		MoveToStepArray();
		HTMLNext();
	}
}

void CCtrlPanelWnd::OnPrev()
{
//	AfxMessageBox("previous");
	if (MACRO_APP->m_bMessageBox)
		return;

	if (m_bUsePPT)
	{
		MoveToStepArray();
		PPTPrev();
	}
	else if (m_bUseHTML)
	{
		MoveToStepArray();
		HTMLPrev();
	}
}

void CCtrlPanelWnd::OnGoto() 
{
	if (MACRO_APP->m_bMessageBox)
		return;

	CDlgGotoSlide dlg;
	dlg.m_pstrArrayTitle = &m_strArrayTitle;
	dlg.m_nIndex = m_pPPT->GetCurrentSlideNumber(); 
	
	MACRO_APP->m_bMessageBox = TRUE;
	if (dlg.DoModal() == ID_GO)
	{
		if (m_bUsePPT)
		{
			MoveToStepArray();
			PPTGoto(dlg.m_nIndex);
		}
		else if (m_bUseHTML)
		{
			MoveToStepArray();
			HTMLGoto(dlg.m_nIndex);
		}
	}
	MACRO_APP->m_bMessageBox = FALSE;
}

void CCtrlPanelWnd::OnView()
{
	if (!m_pPreviewWnd)
	{
		StartPreview();

		m_pPreviewWnd = new CPreviewWnd;

		long nWidth, nHeight;
		GetWidthHeight(nWidth, nHeight);
		while (nWidth > 300)
		{
			nWidth /= 2;
			nHeight /= 2;
		}

		CRect rect;
		rect.SetRect(0, 0, 100, 100);
		m_pPreviewWnd->Create(WS_BORDER | WS_OVERLAPPED | 
							WS_CAPTION | WS_SYSMENU | WS_DLGFRAME, 
							rect, this);
		CRect rcOut, rcIn;
		m_pPreviewWnd->GetWindowRect(rcOut);
		m_pPreviewWnd->GetClientRect(rcIn);
		int nGapW = rcOut.Width() - rcIn.Width() + constMargin * 2;
		int nGapH = rcOut.Height() - rcIn.Height() + constMargin * 2;
		nWidth += nGapW;
		nHeight += nGapH;

		CRect rcWindow;
		GetWindowRect(rcWindow);
		rect.SetRect(rcWindow.right - nWidth, rcWindow.top - nHeight, 
					 rcWindow.right , rcWindow.top);

		m_pPreviewWnd->SetWindowPos(&wndBottom      , rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		m_pPreviewWnd->SetMSGWnd(this);
		AttachVideo(m_pPreviewWnd);

		//SetFocus();
	}
}

BOOL CCtrlPanelWnd::IsCaptureScreenMode()
{
	return m_pBtnRecDemo->IsWindowVisible();
}
void CCtrlPanelWnd::OnPause()
{
	m_pBtnStop->SetButtonDown(FALSE);
	m_pBtnStop->EnableButton(FALSE);
	m_pBtnRec->SetButtonDown(FALSE);
	
	m_pBmpStaticBlue->ShowWindow(SW_HIDE);
	m_pBmpStaticRed->ShowWindow(SW_HIDE);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_SHOW);

	UpdateWindow();

	//Pause();
	StopCapture();
	StopPreview();

	m_nState = PAUSED;
	m_nBaseMS += (GetTickCount() - m_nTickCount);
}
void CCtrlPanelWnd::OnClose()
{
	HWND hWnd = FindWindowEx(NULL, NULL, "ELearning:97654:23801:Photon", NULL);
	if (m_bCaptureScreenMode)
	{
		if (m_nState == RECORDING)
		{
			if (W_ASK_YESNO(IDS_STOP_REC_SCREEN_NOW, this) == IDNO)
				return;
			OnStopDemo();
		}
		m_pEasyCapture->SetupUserMsgHandler(0);

		Stop();
		CleanUpForCapture();

		DestroyWindow();

		if (hWnd) 
			::PostMessage(hWnd, WM_GETUP, 1, 0);

		return;
	}

	int nID = ID_EXIT_ONLY;
	if (::FileExists(m_strMediaPath + "original.wmv"))
	{
		CDlgCloseToDo dlg;

		MACRO_APP->m_bMessageBox = TRUE;
		nID = dlg.DoModal();
		MACRO_APP->m_bMessageBox = FALSE;
		
		if (nID == IDCANCEL)
		{											//add 2013/9/19 kagata PPT2013
			if(!c_Flag)								//add 2013/9/19 kagata PPT2013
			m_pBtnStop->SetButtonDown(FALSE);		//add 2013/9/19 kagata PPT2013
			c_Flag = FALSE;							//add 2013/9/19 kagata PPT2013
			return;
		}											//add 2013/9/19 kagata PPT2013
	}

	m_pEasyCapture->SetupUserMsgHandler(0);
	DetachVideo();

	if (!IsCaptureScreenMode())
		OnStop();
	else
		OnStopDemo();

	if (m_pFreedraw) 
	{
		m_pFreedraw->Close();		
		CString strPath = m_strPath + "free.tmp";
		if (::FileExists(strPath))
		{
			CPtrArray	ptrArrayDrawUnits;
			if (Helper_ParseFile2DrawUnits((LPTSTR)(LPCTSTR)strPath, ptrArrayDrawUnits)) 
			{
				if (m_strRootPath.Right(1) == "\\")
				{
					//CString strJSPath = m_strRootPath + "html\\_freehand.js";
					//Helper_DrawUnitsDumpJScript(ptrArrayDrawUnits, 250, (LPTSTR)(LPCTSTR)strJSPath);

					Helper_DrawUnitsRefine(ptrArrayDrawUnits);
					CString strJSPath = m_strRootPath + "html\\_freehand.js";
				
					char szBuf[256];
					CString strIniPath = ::GetSysPath() + "Elearning.ini";
					::GetPrivateProfileString("Option", "FreeHandUnitTime", "100", szBuf, 256, strIniPath);

					Helper_DrawUnitsDumpJScript(ptrArrayDrawUnits, atoi(szBuf), (LPTSTR)(LPCTSTR)strJSPath);
				}
			}
			Helper_DrawUnitsFree(ptrArrayDrawUnits);
			DeleteFile(strPath);
		}
		delete m_pFreedraw;
	}

	if (nID == ID_EXIT_GEN)
	{
		if (m_bOpen)
			PPTClose(FALSE);

		if (hWnd != NULL) 
		{
			IPCStruct	ipcs;
			strcpy(ipcs.szPath, m_strPath);

			COPYDATASTRUCT	cds;
			cds.dwData = 0;
			cds.cbData = sizeof(ipcs);
			cds.lpData = &ipcs;

			::SendMessage(hWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM) &cds);
		}

		delete m_pPPT;

		Stop();
		CleanUpForCapture();

		DestroyWindow();

		::PostMessage(hWnd, WM_LOAD_COURSE, 1, 0);
	}
	else if (nID == ID_EXIT_ONLY)
	{
		if (m_bOpen)
			PPTClose(TRUE);

		delete m_pPPT;

		Stop();
		CleanUpForCapture();

		DestroyWindow();

		if (hWnd) 
			::PostMessage(hWnd, WM_GETUP, 1, 0);
	}
	
//	::PostMessage(hWnd, WM_CLOSE, 0, 0);
}

LRESULT CCtrlPanelWnd::OnButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(m_hWnd))
	{
		SetFocus();
		BringToTop();	
	}

	return 0L;
}

LRESULT CCtrlPanelWnd::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	UINT n;

	BeginWaitCursor();

	switch (wParam)
	{
		case ID_BTN_CLOSE:
			c_Flag = TRUE;
			OnClose();
			break;

		case ID_BTN_VIEW:
			OnView();
			break;

		case ID_BTN_REC:
			OnRec();
			break;

//		case ID_BTN_NG:		//del 2013/9/19 kagata PPT2013
//			OnNG();
//			break;

		case ID_BTN_REC_DEMO:
			OnRecDemo();
			break;

//		case ID_BTN_PAUSE:			//del 2013/9/19 kagata PPT2013
//			OnPause();
//			if (m_pPreviewWnd)
//				StartPreview();
//			break;

		case ID_BTN_STOP_DEMO:
		{
			if (m_nState == RECORDING)
				OnStopDemo();
			break;
		}

		case ID_BTN_STOP:
		{
			/* wangwang
			if (m_nState == RECORDING)
			{
				if (W_ASK_YESNO(IDS_STOP_REC_NOW, this) == IDYES)
					OnStop();
				else
					m_pBtnStop->SetButtonDown(FALSE);

				if (m_pPreviewWnd)
					StartPreview();

			}
			else
				ResetButtons();
            */
			OnClose();
			break;
		}

//		case ID_BTN_PREVIEW:	//del 2013/9/19 kagata PPT2013
//			OnPreview();
//			break;

		case ID_BTN_NEXT:
			OnNext();
			break;

		case ID_BTN_PREV:
			OnPrev();
			break;

		case ID_BTN_FIRST:
			OnFirst();
			break;

		case ID_BTN_LAST:
			OnLast();
			break;

		case ID_BTN_GOTO:
			OnGoto();
			break;

		case ID_BTN_ZOOM_IN:
			OnZoomIn();
			break;

		case ID_BTN_ZOOM_OUT:
			OnZoomOut();
			break;

		case ID_BTN_DEMO:
			OnDemo();
			break;

		case ID_BTN_CAPTURE:
			OnCapture();
			break;
/* del itoh 2013/07/24 PPT2013
		case ID_BTN_RECT:
			n = m_pBtnRect->GetButtonStatus();
			if (n == constBtnDown)
			{
				m_pBtnLine->SetButtonDown(FALSE);
				m_pBtnPointer->SetButtonDown(FALSE);
				m_pBtnFreeHand->SetButtonDown(FALSE);
				if (m_bUseHTML)
					m_pWndHTML->StartHook(1);
			}
			else
			{
				if (m_bUseHTML)
					m_pWndHTML->StartHook(0);
			}

			break;

		case ID_BTN_LINE:
			n = m_pBtnLine->GetButtonStatus();
			if (n == constBtnDown)
			{
				m_pBtnRect->SetButtonDown(FALSE);
				m_pBtnPointer->SetButtonDown(FALSE);
				m_pBtnFreeHand->SetButtonDown(FALSE);
				if (m_bUseHTML)
					m_pWndHTML->StartHook(1);
			}
			else
			{
				if (m_bUseHTML)
					m_pWndHTML->StartHook(0);
			}
			
			break;

		case ID_BTN_POINTER:
			n = m_pBtnPointer->GetButtonStatus();
			if (n == constBtnDown)
			{
				m_pBtnRect->SetButtonDown(FALSE);
				m_pBtnLine->SetButtonDown(FALSE);
				m_pBtnFreeHand->SetButtonDown(FALSE);
				if (m_bUseHTML)
					m_pWndHTML->StartHook(1);
			}
			else
			{
				if (m_bUseHTML)
					m_pWndHTML->StartHook(0);
			}

			break;

		case ID_BTN_FREEHAND:
			n = m_pBtnFreeHand->GetButtonStatus();
			if (n == constBtnDown)
			{
				m_pBtnRect->SetButtonDown(FALSE);
				m_pBtnLine->SetButtonDown(FALSE);
				m_pBtnPointer->SetButtonDown(FALSE);
				if (m_bUseHTML)
					m_pWndHTML->StartHook(1);
			}
			else
			{
				if (m_bUseHTML)
					m_pWndHTML->StartHook(0);
			}

			break;

		case ID_BTN_ERASER:
			if (m_bUsePPT)
				::RedrawWindow(m_hWndPane, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			else if (m_bUseHTML)
			{
				HWND hWnd = m_pWndHTML->GetIEWnd();
				if (hWnd)
					::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			}

			m_strArrayDraw.RemoveAll();
			break;

		case ID_BTN_PENSET:
		{
			CDlgPenProperty dlg;

			MACRO_APP->m_bMessageBox = TRUE;
			if (dlg.DoModal() == IDOK)
			{
				ReloadBitmap();
				GetPenProperty();
			}
			MACRO_APP->m_bMessageBox = FALSE;

			break;
		}
*/
	}

	EndWaitCursor();
	
	return 0L;
}

//BOOL CCtrlPanelWnd::IsRectMode()
//{
//	UINT n = m_pBtnRect->GetButtonStatus();		// del itoh 2013/07/24 PPT2013
//	return (n == constBtnDown);
//}

//BOOL CCtrlPanelWnd::IsLineMode()
//{
//	UINT n = m_pBtnLine->GetButtonStatus();		// del itoh 2013/07/24 PPT2013
//	return (n == constBtnDown);
//}

//BOOL CCtrlPanelWnd::IsPointerMode()
//{
//	UINT n = m_pBtnPointer->GetButtonStatus();		// del itoh 2013/07/24 PPT2013
//	return (n == constBtnDown);
//}

//BOOL CCtrlPanelWnd::IsFreeHandMode()
//{
//	UINT n = m_pBtnFreeHand->GetButtonStatus();
//	return (n == constBtnDown);
//}

void CCtrlPanelWnd::OnNG() 
{

	if (m_nState == RECORDING)
	{
	//	if (W_ASK_YESNO(IDS_STOP_AND_NG , this) == IDNO)
	//		return;
	}
	else
		return;

	//m_nTimerID3 = SetTimer(991, 100, NULL);
	AddScript();

//2005-11-01
//	OnStop();

	m_pBtnPause->SetButtonDown(FALSE);
	m_pBtnPause->EnableButton(FALSE);
	m_pBtnRec->SetButtonDown(FALSE);
	m_pBtnNG->SetButtonDown(TRUE);
	m_pBtnStop->SetButtonDown(FALSE);
	#ifndef _BUNDLE
		m_pBtnZoomOut->EnableButton(FALSE);
	#endif

	/*NG
	m_pBmpStaticBlue->ShowWindow(SW_HIDE);
	m_pBmpStaticRed->ShowWindow(SW_SHOW);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_HIDE);
	*/
	//NG時燈號改為黃色
	m_pBmpStaticBlue->ShowWindow(SW_HIDE);
	m_pBmpStaticRed->ShowWindow(SW_HIDE);
	m_pBmpStaticOrange->ShowWindow(SW_HIDE);
	m_pBmpStaticYellow->ShowWindow(SW_SHOW);



    
	UpdateWindow();


	if (!::FileExists(m_strMediaPath + "original.wmv"))
	{
		W_SHOW_WARNING(IDS_MUST_REC_FIRST, this);
		return;
	}

	if (m_nMaxIndex < 0)
	{
		::DeleteFile(m_strMediaPath + m_strCurFile);
		m_nMaxIndex = 0;
//2005-11-01		
		OnStop();
	}
	else
	{
		ReloadNG();

//2005-11-01
		int nMin = m_nMaxIndex;
		BOOL bMerge = FALSE;
		if (m_strArrayNG.GetSize() > 0)
		{
			CString strTmp = m_strArrayNG.GetAt(m_strArrayNG.GetSize() - 1);

			int nIndex = strTmp.Find(" ", 0);
			if (nIndex >= 0)
			{
				int nM = atoi(strTmp.Mid(nIndex + 1));
				if (nM == m_nMaxIndex)
				{


					nMin = atoi(strTmp.Left(nIndex));
					bMerge = TRUE;
				}
			}
		}

//		strTemp.Format("%d %d %s", m_nMaxIndex, m_nIndex, m_strCurFile);
		CString strTemp;
		strTemp.Format("%d %d", nMin, m_nIndex);

		if (bMerge && m_strArrayNG.GetSize() > 0)
		{
			m_strArrayNG.SetAt(m_strArrayNG.GetSize() - 1, strTemp);
		}
		else
			m_strArrayNG.Add(strTemp);

		WriteNGFile();
	}

	m_bNG = TRUE;

}

void CCtrlPanelWnd::OnDemo() 
{
	if (m_nState == RECORDING)
	{
		if (W_ASK_YESNO(IDS_STOP_TO_DEMO, this) == IDNO)
			return;
	}

	OnStop();

	if (!::FileExists(m_strMediaPath + "original.wmv"))
	{
		W_SHOW_WARNING(IDS_MUST_REC_FIRST, this);
		return;
	}

	ChangeToRecMode(FALSE);
}

void CCtrlPanelWnd::OnCapture() 
{
	if (m_nState == RECORDING)
	{
		//if (W_ASK_YESNO(IDS_STOP_REC_SCREEN_NOW, this) == IDYES)
			OnStopDemo();
		//else
		//{
		//	m_pBtnStopDemo->SetButtonDown(FALSE);
		//	return;
		//}
	}

	ChangeToRecMode(TRUE);

	if (W_ASK_YESNO(IDS_START_RECORD, this) == IDYES)
		OnRec();
}

void CCtrlPanelWnd::AddDemoTag() 
{
	if (!IsCaptureScreenMode())
		return;

	if (m_bCaptureScreenMode)
		return;

	CString strPath = m_strHTMLPath + m_strDemoFile;
	if (m_strDemoFile != "" && ::FileExists(strPath) && m_strArrayDemoStep.GetSize() != 0)
	{
		CStringArray strArrayStep;
		CString strStepFile = m_strHTMLPath + "step.lrn";
		if (::FileExists(strStepFile))
		{
			::LoadFile(strStepFile, strArrayStep, TRUE);
			int nSize = strArrayStep.GetSize();

			m_nIndex++;
			for (int i = 0; i < m_strArrayDemoStep.GetSize(); i++)
				strArrayStep.Add(m_strArrayDemoStep[i]);
			::SaveFile(strStepFile, strArrayStep, TRUE);
			m_strArrayDemoStep.RemoveAll();
		}
	}
}

void CCtrlPanelWnd::PostNcDestroy() 
{
	delete this;
	
	CWnd::PostNcDestroy();
}

void CCtrlPanelWnd::OnDestroy() 
{
	UnSetHook();

	if (m_strHTMLPath.Right(1) == "\\")
	{
		CString strPath = m_strHTMLPath + "media.lrn";
		::DeleteFile(strPath);
	}

	CoUninitialize();

	EnableTaskBar(TRUE);

	KillTimer(m_nTimerID);
	KillTimer(m_nTimerID2);
	KillTimer(m_nTimerID4);

	m_pBtnClose->DestroyWindow();	
	
	m_pBtnCapture->DestroyWindow();	
	m_pBtnRecDemo->DestroyWindow();	
	m_pBtnStopDemo->DestroyWindow();

	m_pBtnDemo->DestroyWindow();	
	m_pBtnView->DestroyWindow();

	m_pBtnRec->DestroyWindow();
	m_pBtnPause->DestroyWindow();
	m_pBtnStop->DestroyWindow();
	m_pBtnNG->DestroyWindow();

	m_pBtnFirst->DestroyWindow();
	m_pBtnPrev->DestroyWindow();
	m_pBtnNext->DestroyWindow();
	m_pBtnLast->DestroyWindow();
	m_pBtnGoto->DestroyWindow();

/* del itoh 2013/07/24 PPT2013
	m_pBtnRect->DestroyWindow();
	m_pBtnLine->DestroyWindow();
	m_pBtnPointer->DestroyWindow();
	m_pBtnEraser->DestroyWindow();
	m_pBtnPenSet->DestroyWindow();
*/
	m_pBtnZoomIn->DestroyWindow();
	m_pBtnZoomOut->DestroyWindow();

	m_pBtnPreview->DestroyWindow();

	m_pBmpLabel1->DestroyWindow();
	m_pBmpLabel2->DestroyWindow();
//	m_pBmpLabel3->DestroyWindow();
	m_pBmpLabel4->DestroyWindow();
	m_pBmpLabel5->DestroyWindow();
	m_pBmpLabel6->DestroyWindow();

	m_pBmpStaticBlue->DestroyWindow();
	m_pBmpStaticRed->DestroyWindow();
	m_pBmpStaticOrange->DestroyWindow();
	m_pBmpStaticYellow->DestroyWindow();

	m_pTimeStatic->DestroyWindow();

	m_pPageNoEdit->DestroyWindow();

	if (m_pPreviewWnd)
		m_pPreviewWnd->DestroyWindow();

	if (m_pWndHTML)
		m_pWndHTML->DestroyWindow();
   
	CWnd::OnDestroy();
}

CSize CCtrlPanelWnd::GetBmpDim(UINT nID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nID);
	BITMAP b;
	bitmap.GetBitmap(&b);
	return CSize(b.bmWidth, b.bmHeight);
}

///////////////////////////////////////////////////////////////////
// UI Related!!

void CCtrlPanelWnd::DetachVideo()
{
	if (m_pEasyCapture == NULL) 
		return;

	m_pEasyCapture->DisplayClose();
}

BOOL CCtrlPanelWnd::AttachVideo(CWnd* pWnd)
{
	if (!pWnd)
		return FALSE;

	if (!::IsWindow(pWnd->m_hWnd))
		return FALSE;

	CRect rc;
	pWnd->GetClientRect(rc);
	rc.left += constMargin;
	rc.top += constMargin;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DisplayOpen((long)pWnd->GetSafeHwnd(), 
										  rc.left, rc.top, 
										  rc.Width() - constMargin, 
										  rc.Height() - constMargin);
	if (FAILED(hval)) 
	{
		W_SHOW_ERROR(IDS_ATTACH_WND_ERR, this);
		return FALSE;
	}

	if (m_pPreviewWnd)
		m_pPreviewWnd->RedrawWindow();

	return TRUE;
}

BOOL CCtrlPanelWnd::AudioOnly() 
{
	CString strIniPath = m_strPath + "Course.ini";

	char szBuf[256];
	::GetPrivateProfileString("Profile", "Param1", "32", szBuf, 256, strIniPath);
	int nVideoBitRate = atoi(szBuf);
	if (nVideoBitRate == 0)
		return TRUE;

	return FALSE;
}

BOOL CCtrlPanelWnd::SetDevice() 
{
	GetDevice(m_strType, m_strParam1, m_strParam2);
	if (!GetDeviceIndex())
		return FALSE;
	if (m_bAudioOnly)
		OpenDevice("AUDIO");
	else
		OpenDevice(m_strType);

	HRESULT hval;
	hval = m_pEasyCapture->SetupUserMsgHandler((long)GetSafeHwnd());
	if (FAILED(hval)) 
		W_SHOW_WARNING(IDS_NO_ERR_HANDLER, this);
	
	return TRUE;
}

int CCtrlPanelWnd::GetIndex(CStringArray& strArray, CString str) 
{
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		if (strArray[i] == str)
			return i;
	}
	return -1;
}

BOOL CCtrlPanelWnd::GetDeviceIndex() 
{
	if (m_pEasyCapture == NULL)
		return FALSE;

	//CString strType, strParam1, strParam2;
	//GetDevice(strType, strParam1, strParam2);
	CAPTUREDEVICETYPE eType;
	CStringArray strArrayWebAudio, strArrayWebVideo, strArrayDV;

	if (m_bAudioOnly)
	{
		eType = CAPTUREDEVICETYPE_AUDIO;
		if (!EnumDevice(eType, strArrayWebAudio))
			return FALSE;
		m_nAudioIndex = GetIndex(strArrayWebAudio, m_strParam2);
		m_nVideoIndex = -1;
		return TRUE;
	}

	if (m_strType == "DV")
	{
		eType = CAPTUREDEVICETYPE_DV;
		if (!EnumDevice(eType, strArrayDV))
			return FALSE;
		m_nVideoIndex = GetIndex(strArrayDV, m_strParam1);

		eType = CAPTUREDEVICETYPE_AUDIO;
		if (!EnumDevice(eType, strArrayWebAudio))
			return FALSE;
		m_nAudioIndex = GetIndex(strArrayWebAudio, m_strParam2);

		if (m_nAudioIndex >= 0 && m_nVideoIndex >= 0)
			return TRUE;
	}
	else if (m_strType == "CAM")
	{
		eType = CAPTUREDEVICETYPE_AUDIO;
		if (!EnumDevice(eType, strArrayWebAudio))
			return FALSE;
		m_nAudioIndex = GetIndex(strArrayWebAudio, m_strParam2);
		
		eType = CAPTUREDEVICETYPE_VIDEO;
		if (!EnumDevice(eType, strArrayWebVideo))
			return FALSE;
		m_nVideoIndex = GetIndex(strArrayWebVideo, m_strParam1);

		if (m_nAudioIndex >= 0 && m_nVideoIndex >= 0)
			return TRUE;
	}
  
	return FALSE;
}

void CCtrlPanelWnd::CommonFuncErrMsg(CString strFunc)
{
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_COMMON_FUNC_ERR);
	strMsg.Format(strTmp, strFunc);
	W_SHOW_ERROR_STR(strMsg, this);
}

void CCtrlPanelWnd::CommonErrMsg(CString strFunc)
{
	CString strTmp, strMsg;
	strTmp.LoadString(IDS_COMMON_ERR);
	strMsg.Format(strTmp, strFunc);
	W_SHOW_ERROR_STR(strMsg, this);
}

BOOL CCtrlPanelWnd::CreateInterface()
{
	if (m_pEasyCapture != NULL) 
		ReleaseInterface();

	HRESULT hval = ::CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, 
									  (void**)&m_pEasyCapture);
	if (FAILED(hval))
	{
		W_SHOW_ERROR(IDS_COM_CAPTURE_NOT_REGISTERED, this);
		return FALSE;
	}

	hval = CoCreateInstance(CLSID_WMXUtility, NULL, CLSCTX_INPROC_SERVER, 
							IID_IWMXUtility, (void**)&m_pWMXUtility);
	if (FAILED(hval))
	{
		W_SHOW_ERROR(IDS_ERROR_COM_WMX_UTILITY, this);
		return FALSE;
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::OpenCapture(BOOL bScreen)
{
	HRESULT hval = m_pEasyCapture->Open(0);
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::Open");	
		return FALSE;
	}
	return TRUE;
}

void CCtrlPanelWnd::ReleaseInterface()
{
	if (m_pEasyCapture != NULL) 
	{
		m_pEasyCapture->Close();
		m_pEasyCapture->Release();
		m_pEasyCapture = NULL;
	}

	if (m_pWMXUtility)
	{
		m_pWMXUtility->Release();
		m_pWMXUtility = NULL;
	}

	m_eStatus = EASY_IDLE;
}



/////////////////////////////////////////////////////////////////////
// NOT UI

BOOL CCtrlPanelWnd::PreviewPrepare()
{
	HRESULT hval = 0;
	hval = m_pEasyCapture->PreviewPrepare();
	if (FAILED(hval)) 
	{
		//CommonFuncErrMsg("IEasyCapture::PreviewPrepare");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::StartPreview()
{
	if (m_bAudioOnly)
		return TRUE;

	if (m_pEasyCapture == NULL) 
		return FALSE;

	if (m_eStatus == EASY_PREVIEW)
		return TRUE;
	
	if (m_nState == STOPPED)
	{
		Stop();
		if (!PreviewPrepare())
			return FALSE;
		Start();
		m_eStatus = EASY_PREVIEW;
	}

	if (m_pPreviewWnd)
		AttachVideo(m_pPreviewWnd);

	return TRUE;
}

void CCtrlPanelWnd::StopPreview()
{
	if (m_pEasyCapture == NULL) 
		return;
	
	if (m_eStatus == EASY_PREVIEW) 
	{
		Stop();
		m_eStatus = EASY_IDLE;
	}
}

BOOL CCtrlPanelWnd::StartCaptureScreen(CString strPath) 
{
// If Graph Still Running, Cannot Chanage!!
// So DoStop Should be Done Before This Function!!
	Stop();

	BSTR bstr = strPath.AllocSysString();
	BOOL fOK = FALSE;
	fOK = CapturePrepareScreen(bstr); // Profile Index From UI
	SysFreeString(bstr);

	if (!fOK) 
		return FALSE;
    //wang20080505
	Start();

	m_eStatus = EASY_CAPTURE;

	return TRUE;
}

BOOL CCtrlPanelWnd::StartCapture(CString strPath) 
{
// If Graph Still Running, Cannot Chanage!!
// So DoStop Should be Done Before This Function!!
	Stop();

	BSTR bstr = strPath.AllocSysString();
	BOOL fOK = FALSE;
	//AfxMessageBox("clock begin");
	fOK = CapturePrepare(bstr); // Profile Index From UI
	SysFreeString(bstr);
   	//AfxMessageBox("clock end");
	if (!fOK) 
		return FALSE;

	Start();
  // 	AfxMessageBox("clock 423");
	m_eStatus = EASY_CAPTURE;

	return TRUE;
}

void CCtrlPanelWnd::StopCapture()
{
	if (m_pEasyCapture == NULL) 
		return;
	
	if (m_eStatus == EASY_CAPTURE) 
	{
		Stop();
		m_eStatus = EASY_IDLE;
	}
}

BOOL CCtrlPanelWnd::CapturePrepare(BSTR bstrFile)
{
	HRESULT hval = 0;
    //AfxMessageBox((LCPSTR)bstrFile);
	CString strIniPath = m_strRootPath + "Course.ini";

	char szBuf[256];
	::GetPrivateProfileString("Profile", "Dimension", "240 x 180", szBuf, 256, strIniPath);
	int nWidth, nHeight;
	sscanf(szBuf, "%d x %d", &nWidth, &nHeight);
	
	::GetPrivateProfileString("Profile", "Param1", "32", szBuf, 256, strIniPath);
	int nVideoBitRate = atoi(szBuf) * 1024;
	if (nVideoBitRate == 0)
		m_bAudioOnly = TRUE;

	::GetPrivateProfileString("Profile", "Param2", "15", szBuf, 256, strIniPath);
	int nFPS = atoi(szBuf);

	::GetPrivateProfileString("Profile", "Param3", "3", szBuf, 256, strIniPath);
	int nSec = atoi(szBuf);
    
	::GetPrivateProfileString("Profile", "Param4", "0", szBuf, 256, strIniPath);
	int nQuality = atoi(szBuf);

	::GetPrivateProfileString("Profile", "Param5", "32", szBuf, 256, strIniPath);
	int nAudioBitRate = atoi(szBuf) * 1000;

	::GetPrivateProfileString("Profile", "Param6", "32000", szBuf, 256, strIniPath);
	int nSampleRate = atoi(szBuf);

	::GetPrivateProfileString("Profile", "Param7", "S", szBuf, 256, strIniPath);
	int nMono = (szBuf[0] == 'S') ? 2 : 1;

	PROFILEPARAMS pfparam;
	pfparam.ainfo.dwBitrate = (DWORD)nAudioBitRate;
	pfparam.ainfo.dwFormatTag = CODEC_AUDIO_MSAUDIO;
	pfparam.ainfo.dwSampleRate = (DWORD)nSampleRate;
	pfparam.ainfo.dwChannels = (DWORD)nMono;		
	pfparam.vinfo.dwFourCC = CODEC_VIDEO_CAMERACAP;
	pfparam.vinfo.dwBitrate = (DWORD)nVideoBitRate;
	pfparam.vinfo.dwHeight = (DWORD)nHeight;
	pfparam.vinfo.dwWidth = (DWORD)nWidth;
	pfparam.vinfo.dwFPS = (DWORD)nFPS;
	pfparam.vinfo.dwQuality = (DWORD)nQuality;
	pfparam.vinfo.dwSecPerKey = (DWORD)nSec;
	pfparam.sinfo.dwBitrate = (DWORD)4096;

	CString strProfilePath = m_strMediaPath + "default.prx";
	BSTR bstrProfile = strProfilePath.AllocSysString();

	hval = m_pWMXUtility->PRXGen(&pfparam, bstrProfile);
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IWMXUtility::PRXGen");
		SysFreeString(bstrProfile);
		return FALSE; 
	}

	hval = m_pEasyCapture->CapturePrepare(bstrFile, bstrProfile, OATRUE);

	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::CapturePrepare");
		SysFreeString(bstrProfile);
		return FALSE; 
	}

	SysFreeString(bstrProfile);

	return TRUE;
}

BOOL CCtrlPanelWnd::CapturePrepareScreen(BSTR bstrFile)
{
	HRESULT hval = 0;

	CString strIniPath;
	CString strProfilePath;
	if (m_bWithScreenProfileIni)
	{
		strIniPath = m_strScreenProfilePath;
		
		strProfilePath = ::GetFilePath(strIniPath) + "screen.prx";
	}
	else
	{
		strIniPath = m_strRootPath + "Course.ini";
		strProfilePath = m_strMediaPath + "screen.prx";
	}
	//AfxMessageBox(strIniPath );
	//AfxMessageBox(strProfilePath );

	char szBuf[256];
	::GetPrivateProfileString("ScreenProfile", "Codec", "ACELP.net", szBuf, 256, strIniPath);
	CString strCodec = szBuf;
	
	::GetPrivateProfileString("ScreenProfile", "Param1", "32", szBuf, 256, strIniPath);
	int nVideoBitRate = atoi(szBuf) * 1024;

	::GetPrivateProfileString("ScreenProfile", "Param2", "15", szBuf, 256, strIniPath);
	int nFPS = atoi(szBuf);

	::GetPrivateProfileString("ScreenProfile", "Param3", "3", szBuf, 256, strIniPath);
	int nSec = atoi(szBuf);
    
	::GetPrivateProfileString("ScreenProfile", "Param4", "0", szBuf, 256, strIniPath);
	int nQuality = atoi(szBuf);

	::GetPrivateProfileString("ScreenProfile", "Param5", "32", szBuf, 256, strIniPath);
	int nAudioBitRate = atoi(szBuf) * 1000;

	::GetPrivateProfileString("ScreenProfile", "Param6", "32000", szBuf, 256, strIniPath);
	int nSampleRate = atoi(szBuf);

	::GetPrivateProfileString("ScreenProfile", "Param7", "S", szBuf, 256, strIniPath);
	int nMono = (szBuf[0] == 'S') ? 2 : 1;

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);

	PROFILEPARAMS pfparam;
	pfparam.ainfo.dwBitrate = (DWORD)nAudioBitRate;
	if (strCodec == "ACELP.net")
		pfparam.ainfo.dwFormatTag = CODEC_AUDIO_ACELP;
	else
		pfparam.ainfo.dwFormatTag = CODEC_AUDIO_MSAUDIO;
	pfparam.ainfo.dwSampleRate = (DWORD)nSampleRate;
	pfparam.ainfo.dwChannels = (DWORD)nMono;		
	pfparam.vinfo.dwFourCC = CODEC_VIDEO_SCREENCAP;
	pfparam.vinfo.dwBitrate = (DWORD)nVideoBitRate;
	pfparam.vinfo.dwHeight = (DWORD)nScreenY;
	pfparam.vinfo.dwWidth = (DWORD)nScreenX;
	pfparam.vinfo.dwFPS = (DWORD)nFPS;
	pfparam.vinfo.dwQuality = (DWORD)nQuality;
	pfparam.vinfo.dwSecPerKey = (DWORD)nSec;
	pfparam.sinfo.dwBitrate = (DWORD)0;

	BSTR bstrProfile = strProfilePath.AllocSysString();

	hval = m_pWMXUtility->PRXGen(&pfparam, bstrProfile);
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IWMXUtility::PRXGen");
		SysFreeString(bstrProfile);
		return FALSE; 
	}
	//new capture
	/*
	m_Screen2Video1.SetWMVCustomProfile((CString)bstrProfile);

	m_Screen2Video1.SetCaptureLeft(0);
	m_Screen2Video1.SetCaptureTop(0);
	m_Screen2Video1.SetCaptureWidth(nScreenX);
	m_Screen2Video1.SetCaptureHeight(nScreenY);
    */

	//CString strFPS;
	//m_CboFPS.GetLBText(m_CboFPS.GetCurSel(), strFPS);
	/*wang20080506
	m_Screen2Video1.SetFps(nFPS);
	m_Screen2Video1.SetFileName((CString)bstrFile);	
	
	strIniPath = ::GetSysPath() + "Elearning.ini";
	::GetPrivateProfileString("Device", "ScreenAudio", "", szBuf, 255, strIniPath);	
	m_Screen2Video1.SetAudioDevice(0);
	m_Screen2Video1.SetAudioInputPin(0);
    */

	//AfxMessageBox((CString)bstrFile );
	//wang20080505
	
	hval = m_pEasyCapture->CapturePrepare(bstrFile, bstrProfile, OAFALSE);
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::CapturePrepare");
		SysFreeString(bstrProfile);
		return FALSE; 
	}

	SysFreeString(bstrProfile);

	return TRUE;
}

CString CCtrlPanelWnd::ConvertMSToStr(DWORD nTotalMS, BOOL bPoint)
{
    // Convert the LONGLONG duration into human-readable format
    int nMS = (nTotalMS % 1000) / 100;
    int nSeconds = nTotalMS / 1000;
    int nMinutes = nSeconds / 60;
	nSeconds %= 60;
	int nHour = nMinutes / 60;
	nMinutes %= 60;
 
    // Update the display
    TCHAR szDuration[24];
	if (bPoint)
		wsprintf(szDuration, _T("%02d:%02d:%02d.%01d\0"), nHour, nMinutes, nSeconds, nMS);
	else
		wsprintf(szDuration, _T("%02d:%02d:%02d\0"), nHour, nMinutes, nSeconds);
    return szDuration;
}

void CCtrlPanelWnd::OnTimer(UINT nIDEvent) 
{
	static int iCount = 0;
	static BOOL bGo=true;
    //SetFocus();
	if (nIDEvent == 99)
	{
		m_lCurrTime += 50;
		CWnd::OnTimer(nIDEvent);
		//AfxMessageBox("Here we come 99");
		return;
	}
   	if (nIDEvent == 991)
	{
		KillTimer(m_nTimerID3);
		//ShowWindow(SW_SHOW);
		//SendMessage(WM_USER_CountDown_TIMER);
	}
	if (nIDEvent == 666)
    {
      //if (m_bCouldRecord)
      {
	    //KillTimer(m_nTimerID3);
        DoRecMost();
		// DWORD dwThreadID;
		// m_hRecHandle = CreateThread(NULL, 0, DoRecMostThread, this, 0, &dwThreadID);
        // CloseHandle( m_hRecHandle );
	   
      }
	 
    }

	if (nIDEvent == 667)
    {
       
	   //	SendMessage(WM_BUTTON_DOWN,0,0);
	   //wangwang20080304
	   /*
       if (bGo)
       {
		   SendMessage(WM_BUTTON_UP,ID_BTN_NEXT,0);
		   SendMessage(WM_BUTTON_UP,ID_BTN_PREV,0);
		   bGo=false;
       } 
	   */
	 // OnPrev();
	 // m_pPreviewWnd->SendMessage(WM_PAINT);
    }
	if (m_nState == RECORDING)
	{
		DWORD dwTick = GetTickCount();

		//shien add 20050323 get windows install path
		CFile cf;

#ifndef _TT

		CString strPath = ::GetSysPath() + "bin\\MCriticalion.slc";

		CString strPathWin = "";
		RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SystemRoot", strPathWin);

	#ifdef _PRO
		#ifndef _TDTBUNDLE
			strPathWin = strPathWin + "\\system32\\MCriticalion.slc";
		#else
			strPathWin = strPathWin + "\\system\\MCriticalion.slc";
		#endif;
	#else
		strPathWin = strPathWin + "\\system\\MCriticalion.slc";
	#endif;

		//strPathWin = strPathWin + "\\system32\\MCriticalion.slc";

		if (!::FileExists(strPath))
		{
			//strPath = ::GetSysPath() + "MCriticalion.slc";
			//if (!::FileExists(strPath))
			//{
				strPath = strPathWin;
				if (!::FileExists(strPath))
				{
					//return FALSE;

					long mTime = m_nBaseMS + dwTick - m_nTickCount;
					//if (mTime > 301000) 
					if (mTime > 180600)  //改為試用3分鐘
					{
						OnStop();
						W_SHOW_WARNING(IDS_TIME_UP, this);
						CWnd::OnTimer(nIDEvent);
						return;
					}

				}
			//}
		}
#endif
/*
//#ifdef _TRIAL
		long mTime = m_nBaseMS + dwTick - m_nTickCount;
		if (mTime > 301000)
		{
			OnStop();
			W_SHOW_WARNING(IDS_TIME_UP, this);
			CWnd::OnTimer(nIDEvent);
			return;
		}
//#endif
*/	
		CString strTime = ConvertMSToStr(m_nBaseMS + dwTick - m_nTickCount, FALSE);	
		m_pTimeStatic->SetText(strTime);
		if (m_pBmpStaticOrange->IsWindowVisible())
		{
			m_pBmpStaticOrange->ShowWindow(SW_HIDE);
			m_pBmpStaticRed->ShowWindow(SW_SHOW);
		}
		else if (m_pBmpStaticRed->IsWindowVisible())
		{
			m_pBmpStaticOrange->ShowWindow(SW_SHOW);
			m_pBmpStaticRed->ShowWindow(SW_HIDE);
		}
		

		iCount++;
		if (iCount % 4 == 0)
		{
			if (!m_bDisConnected && !IsCaptureScreenMode())
			{
				if (!TestConnected())
				{
					m_bDisConnected = TRUE;
					TryToReconnect(); 
				}
			}
		}


		if (!m_bWarnLow && iCount % 120 == 0)
		{
			char szDisk[32];
			_splitpath(m_strPath, szDisk, NULL, NULL, NULL);
			CString strDisk = szDisk + CString("\\");
			LONGLONG llSpace = ::GetVolFreeSpace(szDisk);
			if (llSpace < (LONGLONG)1024 * 1024 * 100)
			{
				W_SHOW_WARNING(IDS_DISK_FREE_LOW, this);
				m_bWarnLow = TRUE;
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CCtrlPanelWnd::TryToReconnect() 
{
	StopCapture();
	CleanUpForCapture();
	W_SHOW_WARNING(IDS_ERR_OCCUR, this);		
	
	while (!InitForVideoCapture())
	{
		CleanUpForCapture();
		if (W_ASK_YESNO(IDS_ERR_CREATE, this) == IDNO)		
		{
			Stop();
			CleanUpForCapture();

			DestroyWindow();
			return;
		}
	}
	
	OnStop();
	m_pTimeStatic->SetText("00:00:00", TRUE);

	OnRec();
}

BOOL CCtrlPanelWnd::TestConnected() 
{
	VARIANT_BOOL vbool;
	HRESULT hval = m_pEasyCapture->IsDigitalCamActive(&vbool);
	if (FAILED(hval)) 
		return FALSE;

	if (vbool == OAFALSE)
		return FALSE;

	return TRUE;
}

void CCtrlPanelWnd::PPTSaveMediaPos() 
{
	if (m_strHTMLPath == "")
		return;

	m_pPPT->SaveMedia(m_strHTMLPath);
}

void CCtrlPanelWnd::PPTOpen() 
{
	m_pPPT->OpenPresentation(m_strPPTPath);

	m_hWndPPT = m_pPPT->GetPPTWnd();
	char szClass[256];
	HWND h = ::GetWindow(m_hWndPPT, GW_CHILD);
	h = ::GetWindow(h, GW_HWNDFIRST);
	::GetClassName(h, szClass, 256);
	BOOL bFound = FALSE;
	while (TRUE)
	{
//		if (strcmp(szClass, "paneClassDC") == 0)	// del itoh 2013/07/31 PPT2013
		if (strcmp(szClass, "screenClass") == 0)
		{
			bFound = TRUE;
			break;
		}

		h = ::GetWindow(h, GW_HWNDNEXT);
		if (!h)
			break;
		::GetClassName(h, szClass, 256);
	}	
	if (bFound)
		m_hWndPane = h;

	EnablePPTBtns(TRUE);
	m_bOpen = TRUE;
}

void CCtrlPanelWnd::PPTClose(BOOL bQuit) 
{	
	m_pPPT->ClosePresentation(bQuit);
	EnablePPTBtns(FALSE);
	m_bOpen = FALSE;
}

void CCtrlPanelWnd::GotoHTMLPage(int nIndex)
{
	if (nIndex < 1 || nIndex > m_strArrayHTMLPath.GetSize())
		return;

	CString strWebPath = m_strPath + "webpages\\";
	CString strPath = strWebPath + m_strArrayHTMLPath.GetAt(nIndex - 1) + "\\index.htm";
	m_pWndHTML->Navigate2(strPath);
}

void CCtrlPanelWnd::HTMLGoto(long nNo) 
{
	if (m_nHTMLIndex == nNo)
		return;

	if (m_nState == RECORDING)
	{
		m_strOp = "=";
		AddScript();
	}

	GotoHTMLPage(nNo);	
	m_nHTMLIndex = nNo;
	SetSlideNo();

	if (m_nState == RECORDING)
		AddPPTRecord();
}

void CCtrlPanelWnd::PPTGoto(long nNo) 
{
	if (m_nState == RECORDING)
	{
		m_strOp = "=";
		AddScript();
	}

	m_pPPT->GotoSlide(nNo);
	SetSlideNo();

	if (m_nState == RECORDING)
	{
		if (m_nPreSlideNo != nNo)
			AddPPTRecord();
	}
}

long CCtrlPanelWnd::PPTCurSlideNo() 
{
	long nNo = m_pPPT->GetCurrentSlideNumber();

	return nNo;
}

long CCtrlPanelWnd::PPTTotalSlideNo() 
{
	long nNo = m_pPPT->GetSlideCount();
	return nNo;
}

void CCtrlPanelWnd::PPTLast() 
{
	long nTotal = PPTTotalSlideNo();
	PPTGoto(nTotal);

//	SetSlideNo();  由PPTGoto負責
}

void CCtrlPanelWnd::PPTNext() 
{
	if (m_nState == RECORDING)
	{
		m_strOp = "+";
		AddScript();
	}

	m_pPPT->NextSlide();

	long nCurNo = PPTCurSlideNo();
	long nTotal = PPTTotalSlideNo();
	BOOL bOutOfRange = FALSE;
	if (nCurNo == 0 || nCurNo > nTotal)
		bOutOfRange = TRUE;	

	SetSlideNo();
	
	if (m_nState == RECORDING)
	{
		if (!bOutOfRange)
			AddPPTRecord();
	}
}

void CCtrlPanelWnd::HTMLNext() 
{
	if (m_nHTMLIndex < m_strArrayHTMLPath.GetSize())
		m_nHTMLIndex++;
	else 
		return;

	if (m_nState == RECORDING)
	{
		m_strOp = "+";
		AddScript();
	}

	GotoHTMLPage(m_nHTMLIndex);

	SetSlideNo();
	
	if (m_nState == RECORDING)
		AddPPTRecord();
}

void CCtrlPanelWnd::HTMLPrev() 
{
	if (m_nHTMLIndex > 1)
		m_nHTMLIndex--;
	else
		return;

	if (m_nState == RECORDING)
	{
		m_strOp = "-";
		AddScript();
	}

	GotoHTMLPage(m_nHTMLIndex);

	SetSlideNo();
	
	if (m_nState == RECORDING)
		AddPPTRecord();
}

void CCtrlPanelWnd::PPTPrev(BOOL bAddRecord) 
{
	if (m_nState == RECORDING)
	{
		m_strOp = "-";
		AddScript();
	}

	m_pPPT->PrevSlide();

	SetSlideNo();
	
	if (m_nState == RECORDING)
	{
		if (bAddRecord)
			AddPPTRecord();
	}
}

void CCtrlPanelWnd::SetSlideNo() 
{
	if (m_bUsePPT)
	{
		long nCurNo = PPTCurSlideNo();
		long nTotal = PPTTotalSlideNo();
		if (nCurNo == 0 || nCurNo > nTotal)
		{
			m_pPageNoEdit->SetWindowText("");
			if (m_nState == RECORDING)
			{
				if (W_ASK_YESNO(IDS_STOP_REC_NOW, this) == IDYES)
					OnStop();
			}	
			else
				W_SHOW_WARNING(IDS_END_OF_SLIDE, this);
			PPTPrev(FALSE);
		}
		else
		{
			char szBuffer[32];
			itoa(nCurNo, szBuffer, 10);
			m_pPageNoEdit->SetWindowText(szBuffer);
		}
	}
	else if (m_bUseHTML)
	{
		CString strTmp;
		strTmp.Format("%d", m_nHTMLIndex);
		m_pPageNoEdit->SetWindowText(strTmp);
	}
}


LRESULT CCtrlPanelWnd::OnUserStatusHandler(WPARAM wParam, LPARAM lParam)
{ 
	switch(wParam) 
	{
		case MSGPARAM_CAPTURE_TRYSTART:
			m_nTickCount = GetTickCount();
			break;
	
		case MSGPARAM_CAPTURE_TRYSTOP:
			break;
	
		case MSGPARAM_CAPTURE_TRYRESUME:
			m_nTickCount = GetTickCount();
			break;
	
		case MSGPARAM_CAPTURE_TRYPAUSE:
			break;
	}
	return 0L;
}

LRESULT CCtrlPanelWnd::OnAcceptError(WPARAM wParam, LPARAM lParam)
{
	m_bNoAcceptError = FALSE;
	return 0L;
}

LRESULT CCtrlPanelWnd::OnUserErrorHandler(WPARAM wParam, LPARAM lParam)
{
	if (m_pEasyCapture == NULL) 
		return 0L;

	if (!::IsWindow(m_hWnd))
		return 0L;

	if (m_bNoAcceptError)
		return 0L;

	switch(wParam) 
	{
		case MSGERROR_GRAPH_ABORT:
		case MSGERROR_DEVICE_LOST:
		{	
			Stop();
			CleanUpForCapture();
			W_SHOW_WARNING(IDS_ERR_OCCUR, this);		
			
			while (!InitForVideoCapture())
			{
				CleanUpForCapture();
				if (W_ASK_YESNO(IDS_ERR_CREATE, this) == IDNO)		
				{
					Stop();
					CleanUpForCapture();

					DestroyWindow();
					return 0L;
				}
			}
			
			OnStop();
			m_pTimeStatic->SetText("00:00:00", TRUE);

			m_bNoAcceptError = TRUE;
			PostMessage(WM_ACCEPT_ERR);

			break;
		}
	}

	return 0L;
}

void CCtrlPanelWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_NEXT || nChar == VK_RETURN || nChar == ' ')
		OnNext();
	else if (nChar == VK_PRIOR)
		OnPrev();
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCtrlPanelWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	BringToTop();	
	RedrawCaption();	
}

void CCtrlPanelWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	RedrawCaption();	
}

void CCtrlPanelWnd::RedrawCaption()
{
	CRect rc;
	GetClientRect(rc);
	rc.right = rc.left + m_nCloseBtnWidth + 3;
	RedrawWindow(rc);	
}

BOOL CCtrlPanelWnd::WriteToFile(CString strFileName, CStringArray& strArray)
{
	CFile cf;
	if (!cf.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;
	
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strBuf = strArray[i];
		if (i != strArray.GetSize() - 1)
			strBuf += "\r\n";
		cf.Write(strBuf, strBuf.GetLength());
	}

	cf.Close();

	return TRUE;
}

void CCtrlPanelWnd::InitStepArray()
{
	m_strArrayStep.RemoveAll();
	m_strArrayStep.Add("version: 1.00");
}

void CCtrlPanelWnd::InitNGArray()
{
	m_strArrayNG.RemoveAll();
	m_strArrayNG.Add("version: 1.00");
}

void CCtrlPanelWnd::AddScript()
{
	m_nIndex++;

	CString strScript;
	strScript.Format("%d", m_nIndex);
	BSTR bstrStep = strScript.AllocSysString();
	m_pEasyCapture->SendIDScript(bstrStep);
	SysFreeString(bstrStep);
}

void CCtrlPanelWnd::AddPPTRecord()
{
	if (m_nState != RECORDING)
		return;

	CString strStep, strTmp;
	if (m_bUsePPT)
	{
		long nCurNo = PPTCurSlideNo();
		if (nCurNo < 1)
			return;

		if (nCurNo == m_nPreSlideNo)
			m_nPreSubSlideNo++;
		else
			m_nPreSubSlideNo = 0;
		m_nPreSlideNo = nCurNo;

		if (m_nPreSubSlideNo == 0)
			strStep.Format("%d%s", m_nPreSlideNo, m_strOp);
		else
			strStep.Format("%d.%s", m_nPreSlideNo, m_strOp);

		strTmp.Format("%d flipslide %s", m_nIndex, strStep);
		m_strArrayStep.Add(strTmp);
		m_nMaxIndex = m_nIndex;
	}
	else if (m_bUseHTML)
	{
		strStep.Format("%d%s", m_nHTMLIndex, m_strOp);		
		strTmp.Format("%d flippage %s", m_nIndex, strStep);
		m_strArrayStep.Add(strTmp);
		m_nMaxIndex = m_nIndex;
	}
}

void CCtrlPanelWnd::OnAddDemo()
{
	int nDemoIndex = GetCurrentDemoIndex();
	if (m_nPreDemoIndex != nDemoIndex)
	{
		AddScript();
		CString strDemoStep;
		//CString strNext = GetNext2RecFile();
		CString strNext = GetNextRecFile();
		strDemoStep.Format("%d,%s demo %d", m_nIndex, strNext, nDemoIndex + 1);
		m_strArrayDemoStep.Add(strDemoStep);
		m_strDemoFile.Format("demo%d.wmv", nDemoIndex + 1);
		m_nPreDemoIndex = nDemoIndex;
	}
}

void CCtrlPanelWnd::OnDrawMark(int nType)
{
	if (m_nState != RECORDING)
		return;

	AddScript();

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	double dLeft = (double)m_rect.left / nScreenX * 100.0;
	double dRight = (double)m_rect.right / nScreenX * 100.0;
	double dTop = (double)m_rect.top / nScreenY * 100.0;
	double dBottom = (double)m_rect.bottom / nScreenY * 100.0;
	CString strLeft, strRight, strTop, strBottom;
	strLeft = MakeupZero(dLeft);
	strRight = MakeupZero(dRight);
	strTop = MakeupZero(dTop);
	strBottom = MakeupZero(dBottom);

	CString strTmp;
	CString strDraw;
	if (nType == 0)
		strDraw = "drawline";
	else if (nType == 1)
		strDraw = "drawrect";
	else if (nType == 2)
		strDraw = "drawmark";
	else if (nType == 3)
		strDraw = "freehand";
	if (nType == 0 || nType == 1)
	{
		strTmp.Format("%d %s %s,%s,%s,%s,%02x%02x%02x,%d", 
					  m_nIndex, strDraw, strLeft, strTop, strRight, strBottom, 
					  GetRValue(m_color), GetGValue(m_color), GetBValue(m_color), m_nWidth);
	}
	else if (nType == 2)
	{
		strTmp.Format("%d %s %s,%s,%06d,000000,000000,0", 
					  m_nIndex, strDraw, strLeft, strTop, m_nID);
	}
	else if (nType == 3)
		strTmp.Format("%d %s %ld", m_nIndex, strDraw, m_lCurrID);
	m_strArrayDraw.Add(strTmp);
}

CString CCtrlPanelWnd::MakeupZero(double d)
{
	CString strRet;
	strRet.Format("%3.2f", d);
	int nZero = 6 - strRet.GetLength();
	for (int i = 0; i < nZero; i++)
		strRet = "0" + strRet;
	return strRet;
}

void CCtrlPanelWnd::MoveToStepArray()
{
	for (int i = 0; i < m_strArrayDraw.GetSize(); i++)
	{
		CString str = m_strArrayDraw[i];
		m_strArrayStep.Add(str);
	}
	m_strArrayDraw.RemoveAll();
}

void CCtrlPanelWnd::OnZoomIn()
{
	if (m_nState != RECORDING)
		return;

	AddScript();
	
	CString strTmp;
	strTmp.Format("%d zoomin", m_nIndex);
	m_strArrayStep.Add(strTmp);
	
	m_pBtnZoomIn->EnableButton(FALSE);
	m_pBtnZoomOut->EnableButton(TRUE);
}

void CCtrlPanelWnd::OnZoomOut()
{
	if (m_nState != RECORDING)
		return;

	AddScript();
	
	CString strTmp;
	strTmp.Format("%d zoomout", m_nIndex);
	m_strArrayStep.Add(strTmp);
	
	m_pBtnZoomIn->EnableButton(TRUE);
	m_pBtnZoomOut->EnableButton(FALSE);
}

void CCtrlPanelWnd::ShowHideTaskBar(BOOL bHide /*=FALSE*/)
{
	CRect rectWorkArea = CRect(0,0,0,0);
	CRect rectTaskBar = CRect(0,0,0,0);
	
	CWnd* pWnd = CWnd::FindWindow("Shell_TrayWnd", "");

	CRect rc;
	GetWindowRect(rc);
	int nHeight = rc.Height() + 2;

	if (bHide)
	{
		// Code to Hide the System Task Bar
		SystemParametersInfo(SPI_GETWORKAREA,
							 0,
							 (LPVOID)&rectWorkArea,
							 0);
		
		if (pWnd)
		{
			pWnd->GetWindowRect(rectTaskBar);
			rectWorkArea.bottom += rectTaskBar.Height();
			rectWorkArea.bottom -= nHeight;
			SystemParametersInfo(SPI_SETWORKAREA,
								 0,
								 (LPVOID)&rectWorkArea,
								 0);

			pWnd->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		// Code to Show the System Task Bar
		SystemParametersInfo(SPI_GETWORKAREA,
							 0,
							 (LPVOID)&rectWorkArea,
							 0);
		if (pWnd)
		{
			pWnd->GetWindowRect(rectTaskBar);
			rectWorkArea.bottom -= rectTaskBar.Height();
			rectWorkArea.bottom += nHeight;
			SystemParametersInfo(SPI_SETWORKAREA,
								 0,
								 (LPVOID)&rectWorkArea,
								 0);

			pWnd->ShowWindow(SW_SHOW);
		}
	}
}

void CCtrlPanelWnd::EnableTaskBar(BOOL bShow)
{
	int nBar = GetTaskbarPos();
	if ((UINT)nBar == ABE_BOTTOM)
		ShowHideTaskBar(!bShow);
}

/*
void CCtrlPanelWnd::LeaveTrack() 
{
	if (!m_bLeaveTrack)
	{
		m_bLeaveTrack = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);

	}
}

LRESULT CCtrlPanelWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	EnableTaskBar(TRUE);
	return 0;
}
*/

void CCtrlPanelWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	
	//CRect rcWindow;
	//GetWindowRect(rcWindow);
	//CPoint pt = point;
	//ClientToScreen(&pt);
	
	//CWnd* pWnd = WindowFromPoint(pt);
	//if (rcWindow.PtInRect(pt) && pWnd && pWnd->m_hWnd == m_hWnd)
	//{
		SetFocus();

	//	if (GetCapture() != this)
			SetCapture();
		
		m_ptDown = point;
		ClientToScreen(&m_ptDown);
		GetWindowRect(&m_rcDown);
		m_bTrackMove = TRUE;
		SetCursor(m_hMoveCursor);
	//}
	return;
}

void CCtrlPanelWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (m_bTrackMove)
	{
//		if (m_pBtnRect->GetButtonStatus() != constBtnDown &&
//			m_pBtnLine->GetButtonStatus() != constBtnDown &&
//			m_pBtnPointer->GetButtonStatus() != constBtnDown &&
//			m_pBtnFreeHand->GetButtonStatus() != constBtnDown)
//		{	
//			ReleaseCapture();
//		}

		ReleaseCapture();
		m_bTrackMove = FALSE;
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));	
	}

	return;
}

void CCtrlPanelWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (nFlags & MK_LBUTTON) 
	{
		if (m_bTrackMove)
		//	OnLButtonDown(nFlags, point);
		//else
		{
			SetCursor(m_hMoveCursor);
			CRect rcWndInScreen;
			ClientToScreen(&point);
			CSize DistanceMoved = point - m_ptDown;
			rcWndInScreen = m_rcDown + DistanceMoved;
			SetWindowPos(&wndTopMost, rcWndInScreen.left, rcWndInScreen.top, 0, 0, SWP_NOSIZE);
		}
		return;
	}

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return;
}

void CCtrlPanelWnd::GetTaskBarRect(CRect& rc) 
{
	// Get a pointer to the Windows task manager.
	CWnd *pwnd = FindWindow("Shell_TrayWnd", NULL);

	if (pwnd != NULL)
	{
		APPBARDATA abd;

		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = pwnd->m_hWnd;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

		rc = abd.rc;
		
		return;
	}

	rc.SetRectEmpty();
	return;
}

BOOL CCtrlPanelWnd::GetLangBarRect(CRect& rc) 
{
	// Get a pointer to the Windows task manager.
	CWnd *pwnd = FindWindow("CiceroUIWndFrame", NULL);

	if (pwnd != NULL)
	{
		pwnd->GetWindowRect(rc);
		return TRUE;
	}

	return FALSE;
}

void CCtrlPanelWnd::ResetTaskbarPos()  // Message handler.
{
	// Get a pointer to the Windows task manager.
	CWnd *pwnd = FindWindow("Shell_TrayWnd", NULL);

	if (pwnd != NULL)
	{
		APPBARDATA abd;

		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = pwnd->m_hWnd;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

		RECT rc;
		rc.top = 0; 
		rc.left = 0; 
		rc.right = GetSystemMetrics(SM_CXSCREEN); 
		rc.bottom = GetSystemMetrics(SM_CYSCREEN); 
		if (abd.uEdge == ABE_BOTTOM)
		{
	        int iHeight = abd.rc.bottom - abd.rc.top; 
			rc.bottom = rc.top + iHeight;
			AppBarQuerySetPos(ABE_TOP, &rc, &abd);
		}
	}
}

int CCtrlPanelWnd::GetTaskbarPos()  // Message handler.
{
	// Get a pointer to the Windows task manager.
	CWnd *pwnd = FindWindow("Shell_TrayWnd", NULL);

	if (pwnd != NULL)
	{
		APPBARDATA abd;

		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = pwnd->m_hWnd;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

		return abd.uEdge;

		//UINT uEdge = GetEdge(abd.rc);

		 //switch(uEdge)
		 //{
		//	 case ABE_LEFT:
		//	 case ABE_RIGHT:
		//	 case ABE_TOP:
		//	 case ABE_BOTTOM:
		//		return (int)uEdge;

		//	 default:
		//		return -1;
		//}
	}
	return -1;
}

void CCtrlPanelWnd::AppBarQuerySetPos(UINT uEdge, LPRECT lprc, PAPPBARDATA pabd) 
{ 
    int iHeight = 0; 
    int iWidth = 0; 
 
    pabd->rc = *lprc; 
    pabd->uEdge = uEdge; 
 
    // Copy the screen coordinates of the appbar's bounding 
    // rectangle into the APPBARDATA structure. 
    if ((uEdge == ABE_LEFT) || (uEdge == ABE_RIGHT)) 
	{ 
        iWidth = pabd->rc.right - pabd->rc.left; 
        pabd->rc.top = 0; 
        pabd->rc.bottom = GetSystemMetrics(SM_CYSCREEN); 
    } 
	else 
	{ 
        iHeight = pabd->rc.bottom - pabd->rc.top; 
        pabd->rc.left = 0; 
        pabd->rc.right = GetSystemMetrics(SM_CXSCREEN); 
    } 
 
    // Query the system for an approved size and position. 
    SHAppBarMessage(ABM_QUERYPOS, pabd); 
 
    // Adjust the rectangle, depending on the edge to which the 
    // appbar is anchored. 
    switch (uEdge) 
	{ 
        case ABE_LEFT: 
            pabd->rc.right = pabd->rc.left + iWidth; 
            break; 
 
        case ABE_RIGHT: 
            pabd->rc.left = pabd->rc.right - iWidth; 
            break; 
 
        case ABE_TOP: 
            pabd->rc.bottom = pabd->rc.top + iHeight; 
            break; 
 
        case ABE_BOTTOM: 
            pabd->rc.top = pabd->rc.bottom - iHeight; 
            break; 
    } 
 
    // Pass the final bounding rectangle to the system. 
    SHAppBarMessage(ABM_SETPOS, pabd); 
 
    // Move and size the appbar so that it conforms to the 
    // bounding rectangle passed to the system. 
	::SetWindowPos(pabd->hWnd, NULL, pabd->rc.left, pabd->rc.top, 
		         pabd->rc.right-pabd->rc.left,pabd->rc.bottom-pabd->rc.top,     
				 SWP_NOZORDER | SWP_NOACTIVATE);
}

void CCtrlPanelWnd::GetDevice(CString& strType, CString& strParam1, CString& strParam2)
{
	CString strIniPath;

	char szValue[256];
	strIniPath = m_strPath + "Course.ini";
	::GetPrivateProfileString("Device", "Type", "", szValue, 255, strIniPath);
	if (szValue[0] == '\0')
		strIniPath = ::GetSysPath() + "Elearning.ini";

	::GetPrivateProfileString("Device", "Type", "", szValue, 255, strIniPath);
	strType = szValue;
	if (strType == "DV")
	{
		::GetPrivateProfileString("Device", "Video", "", szValue, 255, strIniPath);
		strParam1 = szValue;
	}
	else if (strType == "CAM")
	{
		::GetPrivateProfileString("Device", "Video", "", szValue, 255, strIniPath);
		strParam1 = szValue;
	}
	::GetPrivateProfileString("Device", "Audio", "", szValue, 255, strIniPath);
	strParam2 = szValue;
//	AfxMessageBox(strParam1);
	if ( strParam1.GetLength() > 0)
	   m_hasVideo=true;
}


BOOL CCtrlPanelWnd::EnumDevice(CAPTUREDEVICETYPE eType, CStringArray& strArray)
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	long lCount = 0;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DeviceTableUpdate();
	if (FAILED(hval)) 
	{
		W_SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE, this);
		return FALSE; 
	}

	hval = m_pEasyCapture->DeviceTableGetCount(eType, &lCount);
	if (FAILED(hval)) 
	{
		//CommonFuncErrMsg("IEasyCapture::DeviceTableGetCount");
		W_SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE, this);
		return FALSE; 
	}

	CString	cst;
	BSTR bstr = NULL;
	long ii = 0;
	for (ii = 0; ii < lCount; ii++) 
	{		
		hval = m_pEasyCapture->DeviceTableGetName(eType, ii, &bstr);
		if (FAILED(hval)) 
		{
			//CommonFuncErrMsg("IEasyCapture::DeviceTableGetName");
			W_SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE, this);
			return FALSE; 
		}
		cst = bstr;
		SysFreeString(bstr);
		strArray.Add(cst);
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::UpdateDeviceTable()
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DeviceTableUpdate();
	if (FAILED(hval)) 
	{
		W_SHOW_ERROR(IDS_FAIL_TO_FIND_DEVICE, this);
		//CommonFuncErrMsg("IEasyCapture::DeviceTableUpdate");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::OpenDeviceByType(CAPTURESESSION eSession)
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DeviceOpen(eSession, m_nVideoIndex, m_nAudioIndex); // 0, 0 Can Be Change By UI
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::DeviceOpen");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::OpenScreenDevice()
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	if (!UpdateDeviceTable()) 
		return FALSE;

	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Elearning.ini";
	::GetPrivateProfileString("Device", "ScreenAudio", "", szBuf, 255, strIniPath);
	if (szBuf[0] == '\0')
		m_nScreenAudioIndex = m_nAudioIndex;
	else
	{
		CStringArray strArrayAudio;
		CAPTUREDEVICETYPE eType = CAPTUREDEVICETYPE_AUDIO;
		if (!EnumDevice(eType, strArrayAudio))
			return FALSE;
		m_nScreenAudioIndex = GetIndex(strArrayAudio, szBuf);
	}

	if (m_nScreenAudioIndex < 0)
		m_nScreenAudioIndex = 0;


	HRESULT hval = 0;
	hval = m_pEasyCapture->DeviceOpen(CAPTURESESSION_PHOTONSCREEN, 0, m_nScreenAudioIndex); 
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::DeviceOpen");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::CloseDevice()
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	m_pEasyCapture->DeviceClose();
	return TRUE;
}

BOOL CCtrlPanelWnd::OpenDevice(CString strType) 
{
	if (!UpdateDeviceTable()) 
		return FALSE;

	if (strType == "DV")
	{
		if (!OpenDeviceByType(CAPTURESESSION_DV))
		{
			CommonErrMsg("open the camera device");
			return FALSE; 
		}
	}
	else if (strType == "CAM")
	{
		if (!OpenDeviceByType(CAPTURESESSION_CAM))
		{
			CommonErrMsg("open the DV device");
			return FALSE; 
		}
	}
	else if (strType == "AUDIO")
	{
		if (!OpenDeviceByType(CAPTURESESSION_AUDIO))
		{
			CommonErrMsg("open the Audio device");
			return FALSE; 
		}
	}
	else
		return FALSE;

	return TRUE;
}


BOOL CCtrlPanelWnd::Start() 
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DoStart();
	if (FAILED(hval))
	{
		CommonFuncErrMsg("IEasyCapture::DoStart");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::Stop() 
{

	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	Sleep(2000);
	hval = m_pEasyCapture->DoStop();
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::DoStop");
		return FALSE; 
	}

	return TRUE;
}


BOOL CCtrlPanelWnd::Pause() 
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DoPause();
	if (FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::DoPause");
		return FALSE; 
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::Resume() 
{
	if (m_pEasyCapture == NULL) 
		return FALSE;

	HRESULT hval = 0;
	hval = m_pEasyCapture->DoResume();
	if(FAILED(hval)) 
	{
		CommonFuncErrMsg("IEasyCapture::DoResume");
		return FALSE; 
	}
	return TRUE;
}

BOOL CCtrlPanelWnd::SetHook()
{   
	if (!m_hmodHook)
		return FALSE;

	if (m_hHook)
		UnSetHook();
 
    if (!m_hHook)
    {
		if (!(m_hHook = SetWindowsHookEx(WH_GETMESSAGE,
	        (HOOKPROC)GetProcAddress(m_hmodHook, "HookProcedure"), m_hmodHook, 0)))
	    {
            return FALSE;
        }
    }

	return TRUE;
};

void CCtrlPanelWnd::UnSetHook()
{
	if (m_hHook)
	{
		::UnhookWindowsHookEx(m_hHook);
		m_hHook = NULL;
	}
};

BOOL CCtrlPanelWnd::SetKBHook()
{   
//	if (!m_hKBmodHook)
//		return FALSE;

//	if (m_hKBHook)
//		UnSetKBHook();
 
//    if (!m_hKBHook)
//    {
//		if (!(m_hKBHook = SetWindowsHookEx(WH_GETMESSAGE,
//	        (HOOKPROC)GetProcAddress(m_hKBmodHook, "HookProcedure"), m_hKBmodHook, 0)))
//	    {
//          return FALSE;
//        }
//    }

	return TRUE;
};


void CCtrlPanelWnd::UnSetKBHook()
{
//	if (m_hKBHook)
//	{
//		::UnhookWindowsHookEx(m_hKBHook);
//		m_hKBHook = NULL;
//	}
};

BOOL CCtrlPanelWnd::LoadMediaFile()
{
	RemoveAllPageMedias();

	CString strPath = m_strHTMLPath + "media.lrn";
	if (!::FileExists(strPath))
		return TRUE;

	CStringArray strArrayMedia;
	if (!::LoadFile(strPath, strArrayMedia, TRUE))
		return FALSE;

	if (strArrayMedia.GetSize() <= 1)
		return FALSE;
	
	CString strRes = strArrayMedia.GetAt(0);
	int nCount = 0;
	strRes.TrimLeft();
	CString strToken;
	::GetTokStr(strRes, strToken, nCount, " ");
	int nHres = atoi(strToken);
	::GetTokStr(strRes, strToken, nCount, " ");
	int nVres = atoi(strToken);
	strArrayMedia.RemoveAt(0);

	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	for (int i = 0; i < strArrayMedia.GetSize(); i++)
	{
		CString strPage = strArrayMedia.GetAt(i);
		strPage.TrimLeft();
		nCount = 0;
		if (!::GetTokStr(strPage, strToken, nCount, " "))
			break;
		int nPage = atoi(strToken);
		CPageMedia* pPageMedia = new CPageMedia;
		pPageMedia->m_nPageNo = nPage;
		pPageMedia->m_pRects = new CObArray;
		m_pPageMedias->Add(pPageMedia);
		while (TRUE)
		{
			if (!::GetTokStr(strPage, strToken, nCount, " "))
				break;
			if (strToken == "")
				break;
			int nX = atoi(strToken);

			if (!::GetTokStr(strPage, strToken, nCount, " "))
				break;
			if (strToken == "")
				break;
			int nY = atoi(strToken);

			if (!::GetTokStr(strPage, strToken, nCount, " "))
				break;
			if (strToken == "")
				break;
			int nW = atoi(strToken);

			if (!::GetTokStr(strPage, strToken, nCount, " "))
				break;
			if (strToken == "")
				break;
			int nH = atoi(strToken);
			CRect* prc = new CRect;
			prc->left = (int)((double)(nX * nScreenWidth) / (double)nHres);
			prc->top = (int)((double)(nY * nScreenHeight) / (double)nVres);
			prc->right = (int)((double)((nX + nW) * nScreenWidth) / (double)nHres);
			prc->bottom = (int)((double)((nY + nH)* nScreenHeight) / (double)nVres);
			pPageMedia->m_pRects->Add((CObject*)prc);
		}
	}

	return TRUE;
}

BOOL CCtrlPanelWnd::IsPointInMedia(CPoint pt)
{
	long nCurSlide = PPTCurSlideNo();
	for (int i = 0; i < m_pPageMedias->GetSize(); i++)
	{
		CPageMedia* pPageMedia = (CPageMedia*)m_pPageMedias->GetAt(i);
		if (pPageMedia->m_nPageNo == nCurSlide)
		{
			for (int j = 0; j < pPageMedia->m_pRects->GetSize(); j++)
			{
				CRect* pRect = (CRect*)pPageMedia->m_pRects->GetAt(j);
				if (pRect->PtInRect(pt))
					return TRUE;
			}
			return FALSE;
		}
	}
	return FALSE;
}

void CCtrlPanelWnd::ChangeToRecMode(BOOL bFlag)
{
	m_bCaptureMode = bFlag;

	HANDLE hProcess = ::GetCurrentProcess();
	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "elearning.ini";
	if (bFlag)
		::GetPrivateProfileString("Option", "Priority", "0", szBuf, 255, strIniPath);
	else
		::GetPrivateProfileString("Option", "ScreenPriority", "0", szBuf, 255, strIniPath);
	
	////////////////////////////////////////////////////////////////////////////////////////////
	::SetPriorityClass(hProcess, (atoi(szBuf)) ? HIGH_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS); 
	//Debug Mode 時, ScreenCapture, 直接關上(未錄製), 會當機///////////////////////////////////

	int nShowCmd = (bFlag) ? SW_SHOW : SW_HIDE;
	int nShowCmd2 = (!bFlag) ? SW_SHOW : SW_HIDE;

	m_pBtnDemo->ShowWindow(nShowCmd);
	m_pBtnView->ShowWindow(nShowCmd);

	m_pBtnRec->ShowWindow(nShowCmd);
	m_pBtnPause->ShowWindow(nShowCmd);
	m_pBtnStop->ShowWindow(nShowCmd);
	m_pBtnNG->ShowWindow(nShowCmd);

	m_pBtnFirst->ShowWindow(nShowCmd);
	m_pBtnPrev->ShowWindow(nShowCmd);
	m_pBtnNext->ShowWindow(nShowCmd);
	m_pBtnLast->ShowWindow(nShowCmd);
	m_pBtnGoto->ShowWindow(nShowCmd);
/* del itoh 2013/07/24 PPT2013
	m_pBtnRect->ShowWindow(nShowCmd);
	m_pBtnLine->ShowWindow(nShowCmd);
	m_pBtnPointer->ShowWindow(nShowCmd);
	m_pBtnEraser->ShowWindow(nShowCmd);
	m_pBtnPenSet->ShowWindow(nShowCmd);
*/
	m_pBtnZoomIn->ShowWindow(nShowCmd);
	m_pBtnZoomOut->ShowWindow(nShowCmd);
	m_pBtnPreview->ShowWindow(nShowCmd);
	m_pBmpLabel1->ShowWindow(nShowCmd);
	m_pBmpLabel2->ShowWindow(nShowCmd);
//	m_pBmpLabel3->ShowWindow(nShowCmd);
	m_pBmpLabel5->ShowWindow(nShowCmd);
	m_pBmpLabel6->ShowWindow(nShowCmd);
	m_pPageNoEdit->ShowWindow(nShowCmd);

	m_pBtnCapture->ShowWindow(nShowCmd2);
	m_pBtnRecDemo->ShowWindow(nShowCmd2);
	m_pBtnStopDemo->ShowWindow(nShowCmd2);
	m_pBmpLabel4->ShowWindow(nShowCmd2);

	int nRight = (bFlag) ? m_nLongBtnEnd : m_nShortBtnEnd;
	CRect rect;
	rect.top = m_nTop;
	rect.bottom = m_nBottom;
	rect.left = nRight + constSmallGap + 2;
	rect.right = rect.left + constTimeWidth;
	m_pTimeStatic->MoveWindow(rect);

	CSize cs = GetBmpDim(IDB_FIRST);
	int nWidth = cs.cx / 4;
	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + nWidth;
	m_pBmpStaticBlue->MoveWindow(rect);
	m_pBmpStaticRed->MoveWindow(rect);
	m_pBmpStaticOrange->MoveWindow(rect);
	m_pBmpStaticYellow->MoveWindow(rect);

	EnableTaskBar(!bFlag);

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	int nW = rect.right + constLargeGap + constMediumGap;
	int nH = cs.cy + constSmallGap * 2 + 6;
	int nPosX = nScreenX - nW;
	int nPosY = nScreenY - nH;
	int nTaskPos = GetTaskbarPos();
	if (nTaskPos == ABE_BOTTOM && !bFlag)
	{
		CRect rc;
		GetTaskBarRect(rc);
		nPosY = rc.top - nH;

		if (GetLangBarRect(rc))
			nPosX = rc.left - nW - 5;
	}
	
	SetWindowPos(&wndTopMost, nPosX, nPosY, nW, nH, SWP_SHOWWINDOW);  
	BringWindowToTop();
	SetForegroundWindow();
	SetFocus();

	m_pTimeStatic->SetText("00:00:00");

	RedrawWindow();

	if (bFlag)
	{
		if (m_bUsePPT)
			m_pPPT->ShowPPT(TRUE);
		else if (m_bUseHTML)
			m_pWndHTML->ShowWindow(SW_SHOW);

		if (!InitForVideoCapture())
			return;
	}
	else
	{
		if (m_bUsePPT)
			m_pPPT->ShowPPT(FALSE);
		else if (m_bUseHTML)
			m_pWndHTML->ShowWindow(SW_HIDE);

		ClosePreviewWnd();
		InitForScreenCapture();
	}
}

int CCtrlPanelWnd::GetCurRecIndex()
{
	CStringArray strArray;
	::GetFiles(m_strMediaPath, "wmv", strArray);
	int nIndex = -1;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		str = str.Left(str.GetLength() - 4);
		if (str.CompareNoCase("original") == 0)
		{
			if (nIndex < 0)
				nIndex = 0;
		}
		else
		{
			if (str.Left(5).CompareNoCase("index") == 0)
			{
				int n = atoi(str.Right(str.GetLength() - 5));
				if (n > nIndex)
					nIndex = n;
			}
		}
	}

	return nIndex;
}

CString CCtrlPanelWnd::GetNext2RecFile()
{
	CString strFile;
	int nIndex = GetCurRecIndex();
	if (nIndex < 0)
		strFile = "index1.wmv";
	else
		strFile.Format("index%d.wmv", nIndex + 2);

	return strFile;
}

CString CCtrlPanelWnd::GetNextRecFile()
{
	CString strFile;
	int nIndex = GetCurRecIndex();
	if (nIndex < 0)
		strFile = "original.wmv";
	else
		strFile.Format("index%d.wmv", nIndex + 1);

	return strFile;
}

/*
CString CCtrlPanelWnd::GetStepFile()
{
	CString strFile = "";
	int nIndex = GetCurRecIndex();
	if (nIndex == 0)
		strFile = "step.lrn";
	else if (nIndex > 0)
		strFile.Format("step%d.lrn", nIndex);

	return strFile;
}
*/

int CCtrlPanelWnd::GetCurrentDemoIndex()
{
	CStringArray strArray;
	::GetFiles(m_strHTMLPath, "wmv", strArray);
	int nIndex = 0;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		str = str.Left(str.GetLength() - 4);
		if (str.Left(4).CompareNoCase("demo") == 0)
		{
			int n = atoi(str.Right(str.GetLength() - 4));
			if (n > nIndex)
				nIndex = n;
		}
	}

	return nIndex;
}

void CCtrlPanelWnd::ClearStep()
{
	CString strStep = m_strHTMLPath + "step.lrn";
	if (::FileExists(strStep))
		::DeleteFile(strStep);

	InitStepArray();
}

void CCtrlPanelWnd::ReloadStep()
{
	CString strStep = m_strHTMLPath + "step.lrn";
	if (::FileExists(strStep))
	{
		::LoadFile(strStep, m_strArrayStep, TRUE);
		::DeleteFile(strStep);
	}
}

void CCtrlPanelWnd::ClearNG()
{
	CString strStep = m_strHTMLPath + "NGstep.lrn";
	if (::FileExists(strStep))
		::DeleteFile(strStep);

	InitNGArray();
}

void CCtrlPanelWnd::ReloadNG()
{
	CString strStep = m_strHTMLPath + "NGstep.lrn";
	if (::FileExists(strStep))
	{
		::LoadFile(strStep, m_strArrayNG, TRUE);
		::DeleteFile(strStep);
	}
}

void CCtrlPanelWnd::WriteStepFile()
{
	int nSize = m_strArrayStep.GetSize();
	if (nSize != 0)
	{
		BOOL bFind = TRUE;
		int nSlide = -1;
		CString strOp = "";
		for (int i = 0; i < m_strArrayStep.GetSize(); i++)
		{
			CString str = m_strArrayStep[i];
			CString strFlip;
			if (m_bUsePPT)
				strFlip = "flipslide";
			else if (m_bUseHTML)
				strFlip = "flippage";
			int n;
			if ((n = str.Find(strFlip)) >= 0)
			{
				int nPreSlide = nSlide;

				CString strSlide = str.Mid(n + strFlip.GetLength() + 1);
				strSlide.TrimLeft();
				strOp = strSlide.Right(1);
				nSlide = atoi(strSlide.Left(strSlide.GetLength() - 1));

				if (nPreSlide == nSlide && strOp == "=")
				{
					m_strArrayStep.RemoveAt(i);
					i--;
				}
			}
		}

		CString strFileName = m_strHTMLPath + "step.lrn";
		WriteToFile(strFileName, m_strArrayStep);

		m_strArrayStep.RemoveAll();
	}
}

void CCtrlPanelWnd::WriteNGFile()
{
	int nSize = m_strArrayNG.GetSize();
	if (nSize != 0)
	{
		CString strFileName = m_strHTMLPath + "NGstep.lrn";
		WriteToFile(strFileName, m_strArrayNG);

		m_strArrayNG.RemoveAll();
	}
}
LRESULT CCtrlPanelWnd::OnWM_USER_CountDownOver_TIMER(WPARAM wParam, LPARAM lParam)
{
	m_bCouldRecord=true;
//	AfxMessageBox("end clock");
	SetEvent(hGlobalWriteEvent);
	return 0L;
}
/*
BOOL CCtrlPanelWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CWnd::PreTranslateMessage(pMsg);
	if (m_hWnd)
    {
	//	AfxMessageBox("PreTranslate");
        m_ToolTip.RelayEvent (pMsg);
        return CWnd::PreTranslateMessage(pMsg);
    }
    return (FALSE);

}
*/
