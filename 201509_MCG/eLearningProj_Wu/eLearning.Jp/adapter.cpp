/*
added by angf 2014/11/19
*/
// adapter.cpp : implementation file
//
#include "stdafx.h"
#include "elearning.h"
#include "cmkapi.h"
#include "FileApi.h"
#include "MovieWnd.h"
#include "Mainfrm.h"
#include "Macro.h"
#include "KeyFramePanel.h"
#include "PPTSyncWnd.h"
#include "MediaUtils.h"
#include "ColGrid.h"
#include "WndToolbar.h"
#include "TimeLinePanel.h"
#include "KeyFramePanel.h"
#include "VideoWnd.h"
#include "TimeLineCtrl.h"
#include "eLearningAPI.h"
#include "VideoEffectWnd.h"
#include "DlgInputCaption.h"
#include "office_ctrl.h"
#include "EffectManageWnd.h"
#include "DlgSelectEffect.h"
#include "eLearningView.h"
#include "DlgAdjustVolAndVideo.h"
#include "grdapi.h"
#include <direct.h>
#include "adapter.h"
#include "resource.h"


#define cHorzCursor				1
#define cVertCursor				2
#define cSplitBox				2
#define cxSplitter				6
#define cySplitter				6
#define cMaxControlPanel		180
#define cMinControlPanel		60
#define cGap					5
#define constPanelHeight		168
#define constMinHtperHeight		30
#define constPanelGap			5

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAdapter, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SCRIPT, OnScriptAccepted)
	ON_MESSAGE(WM_GRD_CTRL_ON, OnHyperLink)
	ON_MESSAGE(WM_TIME_POS_CHANGED, OnTimePosChanged)
	ON_MESSAGE(WM_POS_CHANGED, OnPosChanged)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_KEY_FRAME_MOVED, OnKeyCtrlMoved)
	ON_MESSAGE(WM_KEY_FRAME_TOBE_MOVED, OnKeyCtrlTobeMoved)
	ON_MESSAGE(WM_PERIOD_MOVED, OnPeriodMoved)
	ON_MESSAGE(WM_PERIOD_TOBE_MOVED, OnPeriodTobeMoved)
	ON_MESSAGE(WM_PERIOD_MOVING, OnPeriodMoving)
	ON_WM_PAINT()
	ON_MESSAGE(WM_KEY_FRAME_PRESSING, OnKeyCtrlPressing)
	ON_MESSAGE(WM_REGEN_TAG, OnRegenTag)
	ON_MESSAGE(WM_MODIFY_VIDEO, OnModifyVideo)
	ON_MESSAGE(WM_WAIT_FOR_FILE, OnWaitForFile)
	ON_MESSAGE(WM_GRD_UPDATE, OnGrdUpdate)
	ON_MESSAGE(WM_PERIOD_DELETE, OnPeriodDelete)
	ON_MESSAGE(WM_PERIOD_VIEW, OnPeriodView)
	ON_MESSAGE(WM_PERIOD_MOVING, OnPeriodMoving)
	ON_MESSAGE(WM_PERIOD_EDIT, OnPeriodEdit)
	ON_MESSAGE(WM_PAUSE_VIDEOPLAY, OnPauseVideoPlay)
	ON_MESSAGE(WM_PALETTE_BTN_UP, OnPaletteButtonUp)
	ON_MESSAGE(WM_FADE_IN_EFFECT, OnFadeInEffect)
	ON_MESSAGE(WM_FADE_OUT_EFFECT, OnFadeOutEffect)
	ON_MESSAGE(WM_TRANSIT_EFFECT, OnTransitEffect)
	ON_MESSAGE(WM_DELETE_EFFECT, OnDeleteEffect)
	ON_MESSAGE(WM_SET_PPT_POINT, OnSetPPTPoints)
	ON_MESSAGE(WM_SYNC_WND_CLOSED, OnSyncWndClosed)
	ON_MESSAGE(WM_ADD_PPT_POINT, OnAddPPTRecord)
	ON_MESSAGE(WM_UNDO_ACTION, OnUndoAction)
	ON_MESSAGE(WM_REDO_ACTION, OnRedoAction)
	ON_MESSAGE(WM_RETRIEVE_VIDEO_DATA, OnRetrieveVideoData)
	ON_CBN_SELCHANGE(ID_SIZE_LIST,OnCComboBo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Adapter
CAdapter::CAdapter(CWnd* pParentWnd)
{
	m_pMovieWnd = NULL;
	m_pGrid = NULL;
	m_strCourse = "";
	m_bTracking = FALSE;
	m_bAdjusted = FALSE;
	m_pPPTSyncWnd = FALSE;
	m_bVideoAdjusted = FALSE;
	//add by buxiangyi
	m_RowIndex = -1;
	m_strClassName = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL);
}

CAdapter::~CAdapter()
{
}

BOOL CAdapter::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	BOOL bRet = CWnd::Create(m_strClassName, NULL, dwStyle, rect, pParentWnd, 0);//CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CAdapter::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, m_strClassName, lpszWindowName, dwStyle, 
	rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
	return FALSE;
	}
	return TRUE;
}

int CAdapter::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	Initialize();
	return 0;
}

void CAdapter::StopMovieWndTimer()
{
	if (m_pMovieWnd)
		m_pMovieWnd->StopTimer();
}

void CAdapter::RestartMovieWndTimer()
{
	if (m_pMovieWnd)
		m_pMovieWnd->RestartTimer();
}

void CAdapter::Initialize()
{

	m_pTimePanel = new CTimeLinePanel;
	CRect rcTimePanel;
	//this->GetWindowRect(rcTimePanel);
	//m_pTimePanel->GetWindowRect(rcTimePanel);
	//ScreenToClient(rcTimePanel);
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	rcTimePanel.top = cGap;
	rcTimePanel.left = cGap;
	rcTimePanel.right = rcTimePanel.left + nScreenX - cGap * 3;
	rcTimePanel.bottom = rcTimePanel.top + nScreenY * 5 / 20;//46;
	m_pTimePanel->Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rcTimePanel, this);
	

	m_pMovieWnd = new CMovieWnd;
	m_pMovieWnd->Create(WS_BORDER | WS_VISIBLE | WS_CHILD, CRect (0, 0, 0, 0), this);
	m_pMovieWnd->SetTextFont(MACRO_SYS_FONT2);

	CRect rcTool;
	rcTool.SetRectEmpty();
	m_pPPTToolbar = new CWndToolBar;
	m_pPPTToolbar->Create(WS_CHILD | WS_VISIBLE, rcTool, this, 0);

	m_pGrid = new CColGrid;
	CRect rcGrid;
	rcGrid.SetRectEmpty();
	m_pGrid->Create(rcGrid, this, 0);
	m_pGrid->SetTotalColNo(2);

	m_pGrid->SetColAttribAt(0, cType, cGridHyperBMP);
	m_pGrid->SetColAttribAt(1, cType, cGridHyperLink);
	m_pGrid->SetColAttribAt(0, cEditable, 1);
	m_pGrid->SetColAttribAt(1, cEditable, 1);
	m_pGrid->SetColAttribAt(0, cIndex, 0);
	m_pGrid->u_bDragLine = FALSE;
	m_pGrid->SetSeqAt(cSeqRGB, 0, "255\\255\\255");
	m_pGrid->SetSeqAt(cSeqRGB, 1, "255\\255\\0");
	m_pGrid->u_bPaintBk = FALSE;
	m_pGrid->u_bMultilineEdit = TRUE;

	CBitmap *pBmp;
	pBmp = new CBitmap;
	pBmp->LoadBitmap(IDB_PEN);
	m_pGrid->SetBMPAt(cBMP, 0, pBmp);

	pBmp = new CBitmap;
	pBmp->LoadBitmap(IDB_PEN_HOT);
	m_pGrid->SetBMPAt(cBMP, 1, pBmp);
	m_pGrid->SetSeqAt(cSeqBMPCombo, 0, "0\\1");
	m_pGrid->u_bLastRowStar = FALSE;
	m_pGrid->u_pMSGWnd = this;
	m_pGrid->SetGridSelType(cGridNonSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	//add by buxianyi
	m_pGrid->SetColor(cHLineColor, RGB(66, 167, 195));
	m_pGrid->SetColor(cVLineColor, m_pGrid->GetColor(cBGColor));
	///////////////////////////////////////////////////////////
	m_pGrid->SetColor(cSelColor, m_pGrid->GetColor(cBGColor));
	m_pGrid->SetColor(cFGColor, RGB(0, 0, 255));
	m_pGrid->SetRowHeight(18);
	m_pGrid->u_bHScrollable = FALSE;
	m_pGrid->SetGridFont(MACRO_SYS_FONT2);
	
	m_font = new CFont;
	m_font->CreatePointFont(100, "黑体", NULL);
	m_wndEdit = new CStatic;
	CString strText;
	strText.LoadString(IDS_MOVWND_SIZE);
	m_wndEdit->Create(strText, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);
	m_wndEdit->SetFont(m_font, TRUE);
	
	m_cbExamble = new CComboBox;
	m_cbExamble ->Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL  | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_SIZE_LIST);
	m_cbExamble->SetFont(m_font, TRUE);
	strText.LoadString(IDS_320_240);
	m_cbExamble->InsertString(0, strText);
	strText.LoadString(IDS_480_360);
	m_cbExamble->InsertString(1, strText);
	strText.LoadString(IDS_640_480);
	m_cbExamble->InsertString(2, strText);
	strText.LoadString(IDS_800_600);
	m_cbExamble->InsertString(3, strText);
	strText.LoadString(IDS_320_180);
	m_cbExamble->InsertString(4, strText);
	strText.LoadString(IDS_480_270);
	m_cbExamble->InsertString(5, strText);
	strText.LoadString(IDS_640_360);
	m_cbExamble->InsertString(6, strText);
	strText.LoadString(IDS_800_450);
	m_cbExamble->InsertString(7, strText);
	m_cbExamble ->SetCurSel(0);

}

void CAdapter::OnDestroy() 
{
	m_pMovieWnd->DestroyWindow();
	delete m_pMovieWnd;

	m_pPPTToolbar->DestroyWindow();
	delete m_pPPTToolbar;

	m_pTimePanel->DestroyWindow();
	delete m_pTimePanel;

	m_pGrid->DestroyWindow();
	delete m_pGrid;
	
	m_wndEdit->DestroyWindow();
	delete m_wndEdit;

	m_cbExamble->DestroyWindow();
	delete m_cbExamble;

	m_font->DeleteObject();
	delete m_font;

	if (m_pPPTSyncWnd)
	{
		m_pPPTSyncWnd->DestroyWindow();
		delete m_pPPTSyncWnd;
	}

	CWnd::OnDestroy();
}

void CAdapter::Refresh() 
{
	CString str;
	str = MACRO_MAINFRM->GetCourse();
	LoadCourse(str, TRUE);
}

void CAdapter::Stop() 
{
	m_pMovieWnd->Stop();
}

void CAdapter::Pause() 
{
	m_pMovieWnd->Pause();
}

BOOL CAdapter::ReLoadVideo() 
{
	if (!m_pMovieWnd)
		return TRUE;

	BeginWaitCursor();
	m_pMovieWnd->ReloadFile(m_lUnloadPos);
	EndWaitCursor();

	return TRUE;
}

BOOL CAdapter::UnLoadVideo() 
{
	if (!m_pMovieWnd)
		return TRUE;

	BeginWaitCursor();
	m_lUnloadPos = m_pMovieWnd->GetCurrentPos();
	if (!m_pMovieWnd->UnLoadFile())
	{	
		m_pMovieWnd->SetWaitCursor(FALSE);
		return FALSE;
	}
	EndWaitCursor();

	return TRUE;
}

BOOL CAdapter::LoadCourse(LPCTSTR szCourse, BOOL bForce) 
{
	if (m_strCourse == szCourse && !bForce)
		return TRUE;

	if (!m_pMovieWnd)
		return TRUE;

	m_bVideoAdjusted = FALSE;

	LoadDrawArray(szCourse);

	CString strCourse = szCourse;
	CString strAsfFile = "";
	if (strCourse != "")
		strAsfFile = MACRO_MP4_FILE(strCourse);

	if (strCourse == "" || !::FileExists(strAsfFile))
	{
		LoadHyperLink("");
		m_pMovieWnd->SetFilePosition(0);
		m_pMovieWnd->SetTimeLine("");

		m_pTimePanel->RemoveAllKeyFrame();

		CStringArray straEmpty;
		m_pTimePanel->SetAudioReplaceSegArray(straEmpty);
		m_pTimePanel->SetAudioMixSegArray(straEmpty);

		m_pTimePanel->SetLength(0);
		m_pTimePanel->Initialize();
		m_pTimePanel->RedrawPanel();

		m_pPPTToolbar->EnableButtons(FALSE);
		//SetSlideAndHyperPos("", 0, bForce);
		RefreshSlide();
		SetSlideAndHyperPos2("", 0, bForce); 
		m_strCourse = strCourse;

		return TRUE;
	}

	if (!LoadHyperLink(szCourse))
		return FALSE;

	BeginWaitCursor();
	m_pMovieWnd->SetWaitCursor(TRUE);

	CString strSeg;
	long lDuration;
	m_pTimePanel->Initialize();
	if (LoadVideoFile(strCourse))
	{
		CString strSubPath = MACRO_COURSE_PATH + strCourse + "\\video\\";
		m_pTimePanel->GetVideoSegString(strSeg, MACRO_MP4_FILE(strCourse), TRUE, strSubPath);
		m_pMovieWnd->SetTimeLine(strSeg);
		m_pMovieWnd->SetFilePosition(0);
		lDuration = m_pMovieWnd->GetLength();
		m_pTimePanel->SetLength(lDuration);
	}
	else
	{
		lDuration = MACRO_MAINFRM->GetMediaLength(strAsfFile);
		//add by buxiangyi
		CStringArray m_oldarray, array;
		CString m_time, strPath;
		m_time.Format("%d",lDuration/1000);
		m_oldarray.Add("var vduration =" );
		array.Add("var vduration = " + m_time + ";");
		strPath  = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
		UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

		strSeg.Format("0,%s,0,%ld;", strAsfFile, lDuration);
		m_pMovieWnd->SetTimeLine(strSeg);
		m_pTimePanel->SetLength(lDuration);
		m_pTimePanel->SetNewSegArray();
	}

	m_pMovieWnd->SetAudioReplace("");
	if (LoadAudioReplaceFile(strCourse))
	{
		GetAudioandVideolrnAndSetArray(strCourse,strSeg,1);
	}
    //wangwangreplace
	m_pMovieWnd->SetAudioMix("");
	if (LoadAudioMixFile(strCourse))
	{
		GetAudioandVideolrnAndSetArray(strCourse,strSeg,0);
	}

    m_pMovieWnd->SetVideoText("");

	if (LoadVideoTextFile(strCourse))
	{
	  GetAudioandVideolrnAndSetArray(strCourse,strSeg,2);
	}
    m_pMovieWnd->SetVideoEffect("");
	
	if (LoadVideoEffectFile(strCourse))
	{
	  GetAudioandVideolrnAndSetArray(strCourse,strSeg,3);
	} 

	m_pTimePanel->RedrawPanel();
 
	m_pTimePanel->SetSlidePath(MACRO_SLIDE_PATH(strCourse));
	m_pTimePanel->SetKeyFrame(m_strArrayMarkName, m_strArrayMarkTime, 
							  m_strArrayScriptName, m_strArrayScriptCmd, m_strArrayScriptTime);

	long lPos = m_pMovieWnd->GetCurrentPos();
	//SetSlideAndHyperPos(strCourse, lPos, bForce);
	RefreshSlide();
	SetSlideAndHyperPos2(strCourse, lPos, bForce); 

#ifndef _BUNDLE
	m_pPPTToolbar->EnableButtons(TRUE);
#endif

	m_strCourse = szCourse;

	m_pMovieWnd->SetWaitCursor(FALSE);

	char szBuf[256];
	CString strIniPath = MACRO_INI_FILE(strCourse);
	::GetPrivateProfileString("General", "Contrast", "0", szBuf, 256, strIniPath);
	m_pMovieWnd->AdjustVideo(1, atoi(szBuf));
	::GetPrivateProfileString("General", "Light", "0", szBuf, 256, strIniPath);
	m_pMovieWnd->AdjustVideo(0, atoi(szBuf));
	::GetPrivateProfileString("General", "Volume", "0", szBuf, 256, strIniPath);
	m_pMovieWnd->AdjustAudio(0, atoi(szBuf));

	EndWaitCursor();

	return TRUE;
}
/*
   ToDoType
   0:Audio mix
   1:Audio Replace
   2:Video Text
   3:Video Effect
*/

BOOL CAdapter::GetAudioandVideolrnAndSetArray(CString strCourse,CString strSeg,int ToDoType)
{
  	    //int IFindComma;
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
		if (ToDoType==0)
		  	m_pTimePanel->GetAudioMixSegString(strSeg);
		else if (ToDoType==1)
			m_pTimePanel->GetAudioReplaceSegString(strSeg);
		else if (ToDoType==2)
			m_pTimePanel->GetVideoTextSegString(strSeg);
		else if (ToDoType==3)
			m_pTimePanel->GetVideoEffectSegString(strSeg);
	
		IFindSemiColon=strSeg.Find(';',IStart);

		//wangwang20071019
		while (IFindSemiColon >=0)
		{

			StrSentence=strSeg.Mid(IStart,IFindSemiColon-IStart+1);
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
			StrSentence=StrSentence.Left(IFind)+","+MACRO_VIDEO_DIR(strCourse)+StrSentence.Right(ILen-IFind-1);
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+1;
			IFindSemiColon=strSeg.Find(';',IStart);
	
		}
		//m_pMovieWnd->SetVideoText(StrFinal);
		if (ToDoType==0)
		  m_pMovieWnd->SetAudioMix(StrFinal);
	    else if (ToDoType==1)
		  m_pMovieWnd->SetAudioReplace(StrFinal);
	    else if (ToDoType==2)
		  m_pMovieWnd->SetVideoText(StrFinal);
	    else if (ToDoType==3)
		  m_pMovieWnd->SetVideoEffect(StrFinal);
		return true;

}
BOOL CAdapter::SaveCourse(CString strCourse, BOOL bAsk)
{
	if (strCourse == "")
		return TRUE;
	
	Stop();

	SaveConfig(strCourse);

	SaveDrawArray(strCourse);

	BOOL bOnlyTagModified;
	if (!MACRO_MAINFRM->IsCourseVideoModified(strCourse, &bOnlyTagModified) && m_bVideoAdjusted == FALSE)
	{
		if (W_ASK_YESNO(IDS_NO_MODIFY_MADE, this) == IDNO)
			return TRUE;
	}
	else if(bOnlyTagModified)
	{
		if (bAsk)
		{
			if (W_ASK_YESNO(IDS_ONLY_TAG_MODIFY, this) == IDNO)
				return TRUE;
		}
	}
	else
	{
		if (bAsk)
		{
			if (W_ASK_YESNO(IDS_SURE_TO_MODIFY_VIDEO, this) == IDNO)
				return TRUE;
		}
	}

	UpdateWindow();

	CString strTitle;
	strTitle.LoadString(ID_BTN_SAVE_VIDEO);
	CString strMsg;


	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	if (!bOnlyTagModified)
	{
		if (!SaveVideo(strCourse))
		{
			MACRO_MAINFRM->HideProgress();
			return FALSE;
		}
	}

	MACRO_MAINFRM->HideProgress();

	SaveAllParamFiles(strCourse);

	return TRUE;
}

void CAdapter::SaveAllParamFiles(CString strCourse)
{
	CString strSrcPath = MACRO_RECORD_VIDEO(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_VIDEO(strCourse), FALSE);
	
	strSrcPath = MACRO_RECORD_TAG(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_TAG(strCourse), FALSE);
	
	strSrcPath = MACRO_RECORD_AUDIO_MIX(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_AUDIO_MIX(strCourse), FALSE);

	strSrcPath = MACRO_RECORD_AUDIO_REPLACE(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_AUDIO_REPLACE(strCourse), FALSE);

	strSrcPath = MACRO_RECORD_VIDEO_TEXT(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_VIDEO_TEXT(strCourse), FALSE);

	strSrcPath = MACRO_RECORD_VIDEO_EFFECT(strCourse);
	if (::FileExists(strSrcPath))
		::SafeCopyFile(strSrcPath, MACRO_PRE_RECORD_VIDEO_EFFECT(strCourse), FALSE);
}

LRESULT CAdapter::OnPosChanged(WPARAM wParam, LPARAM lParam)
{
	m_pTimePanel->SetPointerTimeAndShow(wParam);
    
	return 0L;
}

LRESULT CAdapter::OnTimePosChanged(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	long lPos = wParam;

	if (m_pMovieWnd->IsRunning())
		m_pMovieWnd->Pause();

	SetVideoFile(lPos); 

	SetSlideAndHyperPos(strCourse, lPos, FALSE);

	return 0L;
}

void CAdapter::SetVideoFile(long lPos) 
{
	CString strFileName;
	long lVideoPos;
	GetVideoPosAtTime(lPos, lVideoPos, strFileName);
	m_pMovieWnd->SetFilePosition(lPos);
}

void CAdapter::SetSlideAndHyperPos(CString strCourse, long lPos, BOOL bForce) 
{
	if (strCourse == "")
	{
		ShowSlide("", "", "", bForce);
		return;
	}

	int nIndex;
	CString strScriptName, strCmd;
	GetSlideNoAndIndexByTime(lPos, strCmd, strScriptName, nIndex);
	//ShowSlide(strCourse, strCmd, strScriptName, bForce);

	OnSlideChange(nIndex);
}

void CAdapter::SetSlideAndHyperPos2(CString strCourse, long lPos, BOOL bForce) 
{
	if (strCourse == "")
	{
		ShowSlide("", "", "", bForce);
		return;
	}

	int nIndex;
	CString strScriptName, strCmd;
	GetSlideNoAndIndexByTime(lPos, strCmd, strScriptName, nIndex);
	ShowSlide(strCourse, strCmd, strScriptName, bForce);

	OnSlideChange(nIndex);
}

void CAdapter::GetSlideNoAndIndexByTime(long lPos, CString& strCmd, CString& strScript, int& nIndex)
{
	int nOrigIndex;
	if (GetScriptByTime(lPos, nOrigIndex))
	{
		int n = nOrigIndex;

		CString strScriptName = m_strArrayScriptName[n];
		strCmd = m_strArrayScriptCmd[n];
		if (strCmd != "flipslide" && strCmd != "flippage")     
		{
			while (TRUE)
			{
				if (n <= 0)
					break;
				n--;
				CString strCmd2 = m_strArrayScriptCmd[n];
				if (strCmd2 == "flipslide" || strCmd2 == "flippage")
				{
					strScriptName = m_strArrayScriptName[n];
					strCmd = strCmd2;
					break;
				}
			}
		}

		GetSlideNo(strScriptName);
		strScript = strScriptName;
		
		nIndex = SkipNonSlide(nOrigIndex);
		OnSlideChange(nIndex);
	}
}

int CAdapter::SkipNonSlide(int nOrigIndex)
{
	int nIndex = nOrigIndex;
	for (int j = 0; j <= nOrigIndex; j++)
	{
		CString strScriptName = m_strArrayScriptName[j];
		CString strScriptCmd = m_strArrayScriptCmd[j];
		if ((strScriptCmd != "flipslide" && strScriptCmd != "flippage") || strScriptName.Find(".") >= 0)      
			nIndex--;
	}
	return nIndex;
}

void CAdapter::GetSlideNo(CString& strScriptName)
{
	CString strOp = strScriptName.Right(1);
	if (strOp == "=" || strOp == "+" || strOp == "-")
		strScriptName = strScriptName.Left(strScriptName.GetLength() - 1);
	int nDot;
	if ((nDot = strScriptName.Find(".")) >= 0)
		strScriptName = strScriptName.Left(nDot);
}

BOOL CAdapter::GetScriptByTime(long lPos, int& nIndex)
{
	BOOL bFound = FALSE;
	for (int i = 0; i < m_strArrayScriptTime.GetSize(); i++)
	{
		long lTime1 = ::ConvertStrToTime(m_strArrayScriptTime[i]);
		long lTime2;
		if (i == m_strArrayScriptTime.GetSize() - 1)
		{
			lTime2 = m_pMovieWnd->GetLength();
			if (lTime1 <= lPos && lTime2 >= lPos)
			{
				nIndex = i;
				bFound = TRUE;
				break;
			}
		}
		else
		{
			lTime2 = ::ConvertStrToTime(m_strArrayScriptTime[i + 1]);
			if (lTime1 <= lPos && lTime2 > lPos)
			{
				nIndex = i;
				bFound = TRUE;
				break;
			}
		}
	}

	return bFound;
}

LRESULT CAdapter::OnScriptAccepted(WPARAM wParam, LPARAM lParam)
{
	LPTSTR pszCmd = (LPTSTR)wParam;
	LPTSTR pszParam = (LPTSTR)lParam;
	long lTime = m_pMovieWnd->GetCurrentPos();
	if (strcmp(pszCmd, "flipslide") == 0 ||
		strcmp(pszCmd, "flippage") == 0)
	{
		CString strScript = pszParam;
		GetSlideNo(strScript);
		//ShowSlide(m_strCourse, pszCmd, strScript);
		ChangeSlide(m_strCourse, pszCmd, strScript);	//kagata ppt
		
		CString strStep = pszParam;
		if (strStep.Find(".") >= 0)
			return 0L;

		int nIndex = -1;
		for (int i = 0; i < m_strArrayScriptName.GetSize(); i++)
		{
			if (m_strArrayScriptName[i] == pszParam)
			{
				CString strScriptTime = m_strArrayScriptTime[i];
				long lScriptTime = ::ConvertStrToTime(strScriptTime);
				long lDelta = lTime - lScriptTime;

				if (lDelta <= 2000 && lDelta >= -2000)
				{
					nIndex = i;
					break;
				}
			}
		}
		if (nIndex < 0)
			return 0L;

		nIndex = SkipNonSlide(nIndex);
		OnSlideChange(nIndex);
	}

	return 0L;
}

LRESULT CAdapter::OnHyperLink(WPARAM wParam, LPARAM lParam)
{
	int nRow = HIWORD(lParam);
	int nCol = LOWORD(lParam);
	if (nCol == 0)
	{
		m_pGrid->SetAttribAt(nRow, 1, cType, cGridText);
		m_pGrid->ClickRowCol(nRow, 1);
		m_pGrid->SetAttribAt(nRow, 1, cType, cGridHyperLink);
	}
	else if (nCol == 1)
	{
		int nType = m_pGrid->GetAttribAt(nRow, nCol, cType);
		if (nType == cGridText)
			return 0L;

		CString strAsf = MACRO_MP4_FILE(m_strCourse);

		int nPreRow = HIWORD(lParam);
		//add by buxiangyi
		m_RowIndex =  HIWORD(lParam);
		/////////////////////////////////////////////////////////////
		int nIndex = 0;
		int nSlide = -1;
		CString strTime;
		CString strCmd;
		CString strScript;
		for (int i = 0; i < m_strArrayMarkName.GetSize(); i++)
		{
			CString strMark = m_strArrayMarkName[i];
			if (::IsCtrlMark(strMark))
				continue;
			if (nPreRow == nIndex)
			{
				nSlide = atoi(strMark.Mid(1, 3));
				strTime = m_strArrayMarkTime[i];
				strCmd = m_strArrayScriptCmd[i];
				strScript = m_strArrayScriptName[i];
				break;
			}
			nIndex++;
		}

		if (nSlide > 0)
		{
			OnSlideChange(nPreRow);

			CString strSlide;
			strSlide.Format("%d", nSlide);
			//ShowSlide(m_strCourse, strCmd, strScript);
			ChangeSlide(m_strCourse, strCmd, strScript);	//kagata ppt
			long lPos = ::ConvertStrToTime(strTime);

			BOOL bRunning = FALSE;
			if (m_pMovieWnd->IsRunning())
			{
				m_pMovieWnd->Pause();
				bRunning = TRUE;
			}
			m_pMovieWnd->SetFilePosition(lPos);
			if (bRunning)
				m_pMovieWnd->Play();
		}
	}

	return 0L;
}

void CAdapter::ShowSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce)
{
	m_pPPTToolbar->ShowSlide(strCourse, strCmd, strSlide, bForce);
	m_pPPTToolbar->UpdateWindow();
	return;
}

//kagata ppt
void CAdapter::ChangeSlide(CString strCourse, CString strCmd, CString strSlide, BOOL bForce)
{
	m_pPPTToolbar->ChangeSlide(strCourse, strCmd, strSlide, bForce);
	return;
}

void CAdapter::RefreshSlide()
{
	m_pPPTToolbar->RefreshSlide();
	return;
}

void CAdapter::OnSize(UINT nType, int cx, int cy) 
{

	CWnd::OnSize(nType, cx, cy);

	if (!m_pTimePanel)
		return;

	if (!::IsWindow(m_pTimePanel->m_hWnd))
		return;

	cx = max(cx, 400);
	cy = max(cy, 300);

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.right = rcClient.left + cx;
	rcClient.bottom = rcClient.top + cy;

	CRect rcTimePanel;
	m_pTimePanel->GetWindowRect(rcTimePanel);
	
	ScreenToClient(rcTimePanel);

	int nHGap = rcTimePanel.left;
	int nVGap = rcTimePanel.top;

	CRect rcMovie;	
	m_pMovieWnd->m_pstrArrayCmd = &m_strArrayScriptCmd;
	m_pMovieWnd->m_pstrArrayScript = &m_strArrayScriptName;
	m_pMovieWnd->m_pstrArrayTime = &m_strArrayScriptTime;
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	rcMovie.left = cGap;
	rcMovie.top = rcTimePanel.bottom + constPanelGap +30;
	rcMovie.right = rcMovie.left + nScreenX * 7 / 21;//24;
	rcMovie.bottom = rcTimePanel.bottom + cGap + nScreenY * 7 / 21;//24;	
	m_pMovieWnd->m_nLargeGap = 5;
	m_pMovieWnd->m_nSmallGap = 4;
	m_pMovieWnd->MoveWindow(rcMovie);


	CRect editrect;
	LOGFONT logfont;
	CClientDC  dc(m_wndEdit);
	GetWindowRect(&editrect); 
	CString strText;
	strText.LoadString(IDS_MOVWND_SIZE);
	CSize width = dc.GetOutputTextExtent(strText);
	m_font->GetLogFont(&logfont);
	editrect.left = cGap;
	editrect.right = editrect.left + width.cx;
	editrect.top = rcTimePanel.bottom + constPanelGap + 7;
	editrect.bottom = editrect.top + 20;
	m_wndEdit->MoveWindow(editrect);

	CRect comrect;
	GetWindowRect(&comrect); 
	strText.LoadString(IDS_800_450);
	width = dc.GetOutputTextExtent(strText);
	comrect.left = editrect.right + 5;
	comrect.right = comrect.left + width.cx + constPanelGap * 4;
	comrect.top = rcTimePanel.bottom + constPanelGap + 2;
	comrect.bottom = comrect.top + 100;
	m_cbExamble->MoveWindow(comrect);
	
	CRect rcHyper;
	rcHyper.left = rcMovie.left;
	rcHyper.right = rcMovie.right;
	rcHyper.top = rcMovie.bottom + constPanelGap ;
	rcHyper.bottom = rcClient.bottom;// - nVGap - constPanelHeight - constPanelGap;
	if (rcHyper.Height() < constMinHtperHeight)
		rcHyper.bottom = rcHyper.top + constMinHtperHeight;
	m_pGrid->SetColWidth(0, 24); 
	m_pGrid->SetColWidth(1, rcMovie.Width()); 
	m_pGrid->MoveWindow(rcHyper);
	
	CRect rcSlide;
	rcSlide.left = rcMovie.right + constPanelGap  ;
	rcSlide.bottom = rcHyper.bottom;
	rcSlide.top = rcTimePanel.bottom + constPanelGap  ;
	int nGap = 64;
	int nWidth = rcClient.right - nHGap - rcSlide.left - nGap;
	rcSlide.right = rcSlide.left + nWidth;

	CRect rcToolbar = rcSlide;
	rcToolbar.right = rcClient.right - nHGap;
	rcToolbar.bottom--;
	m_pPPTToolbar->MoveWindow(rcToolbar);
	

}

BOOL CAdapter::LoadHyperLink(CString strCourse)
{
	if (strCourse == "")
	{
		m_pGrid->DeleteAllRows();
		m_pGrid->RePaint();
	
		m_strArrayMarkName.RemoveAll();
		m_strArrayMarkTime.RemoveAll();
		m_strArrayScriptName.RemoveAll();
		m_strArrayScriptTime.RemoveAll();
		m_strArrayScriptCmd.RemoveAll();

		return TRUE;
	}

	if (!LoadTagFile(strCourse))
	{
		CString strAsf = MACRO_MP4_FILE(strCourse);
		if (!MACRO_MAINFRM->LoadTag(strCourse, m_strArrayMarkTime, m_strArrayMarkName,
									m_strArrayScriptTime, m_strArrayScriptName, m_strArrayScriptCmd))
		{
			return FALSE;
		}
		WriteTagFile(strCourse);
	}

	BOOL bRet =  SetHyperLinkToGrid();

	return bRet;
}

BOOL CAdapter::LoadDrawArray(CString strCourse)
{
	if (strCourse == "")
	{
		m_strArrayDrawIndex.RemoveAll();
		m_strArrayDrawTime.RemoveAll();
		m_strArrayDrawColor.RemoveAll();
		m_strArrayDrawWidth.RemoveAll();
		m_strArrayDrawCmd.RemoveAll();
		m_strArrayDrawLeft.RemoveAll();
		m_strArrayDrawTop.RemoveAll();
		m_strArrayDrawRight.RemoveAll();
		m_strArrayDrawBottom.RemoveAll();
		return TRUE;
	}

	CString strPath = MACRO_DRAW_FILE(strCourse);
	if (::FileExists(strPath))
	{
		CStringArray strArrayTag;
		::LoadFile(strPath, strArrayTag, TRUE);

		m_strArrayDrawIndex.RemoveAll();
		m_strArrayDrawTime.RemoveAll();
		m_strArrayDrawColor.RemoveAll();
		m_strArrayDrawWidth.RemoveAll();
		m_strArrayDrawCmd.RemoveAll();
		m_strArrayDrawLeft.RemoveAll();
		m_strArrayDrawTop.RemoveAll();
		m_strArrayDrawRight.RemoveAll();
		m_strArrayDrawBottom.RemoveAll();
		CString strLine, strTok;
		int nCount;
		for (int i = 0; i < strArrayTag.GetSize(); i++)
		{
			strLine = strArrayTag[i];
			if (strLine.Find("draw[") < 0)
			{
				continue;
			}

			strTok = strLine.Right(strLine.GetLength() - strLine.Find("draw[") - 5);
			strTok = strTok.Left(strTok.Find("]"));
			m_strArrayDrawIndex.Add(strTok);
			nCount = strLine.Find("\"");
			::GetTokStr(strLine, strTok, nCount, ",");
			::GetTokStr(strLine, strTok, nCount, ",");
			strTok.TrimLeft();
			strTok = strTok.Left(strTok.GetLength() - 3) + strTok.Right(2) + "0";
			m_strArrayDrawTime.Add(strTok);
			::GetTokStr(strLine, strTok, nCount, ",");
			m_strArrayDrawColor.Add(strTok);
			::GetTokStr(strLine, strTok, nCount, ",");
			m_strArrayDrawWidth.Add(strTok);
			::GetTokStr(strLine, strTok, nCount, ",");
			m_strArrayDrawCmd.Add(strTok);
			if (strTok != "freehand" && strTok != "eraser")
			{
				::GetTokStr(strLine, strTok, nCount, ",");
				m_strArrayDrawLeft.Add(strTok);
				::GetTokStr(strLine, strTok, nCount, ",");
				m_strArrayDrawTop.Add(strTok);
				::GetTokStr(strLine, strTok, nCount, ",");
				m_strArrayDrawRight.Add(strTok);
				::GetTokStr(strLine, strTok, nCount, ",");
				strTok =  strTok.Left(strTok.GetLength() - 2);
				m_strArrayDrawBottom.Add(strTok);
			}
			else
			{
				m_strArrayDrawLeft.Add("000.00");
				m_strArrayDrawTop.Add("000.00");
				m_strArrayDrawRight.Add("000.00");
				m_strArrayDrawBottom.Add("000.00");
			}
		}

		return TRUE;
	}

	return FALSE;
}


//LRESULT CAdapter::OnReloadTag(WPARAM wParam, LPARAM lParam)
//{
//	ReloadTag();
//	return 0L;
//}

//void CAdapter::ReloadTag()
//{
//	CString strCourse;
//	strCourse = MACRO_MAINFRM->GetCourse();
//	if (strCourse == "")
//		return;

//	CString strAsf = MACRO_MP4_FILE(strCourse);
//	ReloadMarkerAndScript(strAsf);
//}

BOOL CAdapter::SetHyperLinkToGrid()
{
	if (m_strArrayMarkName.GetSize() == 0)
	{
		m_pGrid->DeleteAllRows();
		m_pGrid->RePaint();

		return TRUE;
	}
	m_pGrid->DeleteAllRows();
	int nIndex = 0;

	for (int i = 0; i < m_strArrayMarkName.GetSize(); i++)
	{
		CString strMark = m_strArrayMarkName[i];
		if (!::IsCtrlMark(strMark))
		{
			m_pGrid->AppendRow(1);
			m_pGrid->SetAttribAt(nIndex, 1, cBGColor, 0);
			m_pGrid->SetDataAt(nIndex, 0, "0");
			if (strMark.GetLength() >= 5)
				m_pGrid->SetDataAt(nIndex++, 1, strMark.Mid(5));
			else
				m_pGrid->SetDataAt(nIndex++, 1, "???");
		}
	}

	//m_pGrid->SetAttribAt(0, 1, cBGColor, 1);
	
	CString strScriptName, strCmd;
	GetSlideNoAndIndexByTime(m_pMovieWnd->GetCurrentPos(), strCmd, strScriptName, nIndex);

	m_pGrid->RePaint();

	return TRUE;
}

void CAdapter::OnSlideChange(int nSlide)
{
	int nSize = m_pGrid->GetDataSize();
	for (int i = 0; i < nSize; i++)
	{
		if (i == nSlide)
			m_pGrid->SetAttribAt(i, 1, cBGColor, 1);
		else
			m_pGrid->SetAttribAt(i, 1, cBGColor, 0);
	}

	if (!m_pGrid->IsRowVisible(nSlide))
		m_pGrid->SetVScrollPos(nSlide);

	m_RowIndex = nSlide;

	m_pGrid->RePaint();
}

void CAdapter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//if (m_bTracking)
	//	return;

	//StartTracking(HitTest(point));
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CAdapter::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*
	BOOL bRepaint = FALSE;
	if (m_bTracking)
		bRepaint = TRUE;

	StopTracking(TRUE);
	
	if (bRepaint)
		RedrawWindow();
*/

	CWnd::OnLButtonUp(nFlags, point);
}

void CAdapter::OnMouseMove(UINT nFlags, CPoint pt) 
{
/*
	if (GetCapture() != this)
		StopTracking(FALSE);

	if (m_bTracking)
	{
		// move tracker to current cursor position

		pt.y -= 1; // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_rectTracker.top != pt.y)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
			OnInvertTracker(m_rectTracker);
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor

		int ht = HitTest(pt);
		if (ht)
			::SetSplitCursor(cVertCursor);
		else
		{
			HCURSOR hcr = (HCURSOR)::GetClassLong(m_hWnd, GCL_HCURSOR);
			SetCursor(hcr);
		}
	}
*/
	
	CWnd::OnMouseMove(nFlags, pt);
}

void CAdapter::OnInvertTracker(const CRect& rect)
{
	CRect rc = rect;
	ClientToScreen(rc);
	::DrawResizeBar(rc);
}

int CAdapter::HitTest(CPoint pt)
{
	ASSERT_VALID(this);

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc;
	rc.top = rcGrid.bottom + cGap;
	rc.bottom = rc.top + cySplitter;
	rc.left = 0;
	rc.right = rcClient.right;
	if (rc.PtInRect(pt))
		return 1;

	return 0;
}

void CAdapter::GetHitRect(CRect& rectHit)
{
	ASSERT_VALID(this);

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	CRect rcClient;
	GetClientRect(rcClient);

	rectHit.top = rcGrid.bottom + cGap;
	rectHit.bottom = rectHit.top + cySplitter;
	rectHit.left = 0;
	rectHit.right = rcClient.right;
}

void CAdapter::StartTracking(int ht)
{
	if (!ht)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);
	GetHitRect(m_rectTracker);

	// steal focus and capture
	SetCapture();
	SetFocus();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	OnInvertTracker(m_rectTracker);
	::SetSplitCursor(cVertCursor);
}

void CAdapter::StopTracking(BOOL bAccept)
{
	if (!m_bTracking)
		return;

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	m_bTracking = FALSE;

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-1 , -1);
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	CRect rcPos, rcPosGrid, rcGridPPT, rcClient;
	GetClientRect(rcClient);

	m_pTimePanel->GetWindowRect(rcPos);
	m_pGrid->GetWindowRect(rcPosGrid);
	m_pPPTToolbar->GetWindowRect(rcGridPPT);

	ScreenToClient(rcPos);
	rcPos.top = m_rectTracker.top + cySplitter + cGap;
	m_pTimePanel->MoveWindow(rcPos);

	ScreenToClient(rcPosGrid);
	rcPosGrid.bottom = m_rectTracker.top - cGap;
	m_pGrid->MoveWindow(rcPosGrid);

	ScreenToClient(rcGridPPT);
	rcGridPPT.bottom = rcPosGrid.bottom;
	m_pPPTToolbar->MoveWindow(rcGridPPT);

	m_bAdjusted = TRUE;
}

BOOL CAdapter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && pWnd == this && !m_bTracking)
		return TRUE;    // we will handle it in the mouse move

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CAdapter::GetInsideRect(CRect& rect) 
{
	ASSERT_VALID(this);

	GetClientRect(rect);
	ASSERT(rect.left == 0 && rect.top == 0);
	
	rect.top = rect.bottom - cMaxControlPanel + cySplitter;
	rect.bottom = rect.bottom - cMinControlPanel - cySplitter;
}


void CAdapter::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CWnd::OnPaint();

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);

	ScreenToClient(rcGrid);

	int nTop = rcGrid.bottom + cGap;

	CRect rcClient;
	GetClientRect(rcClient);

	CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHILIGHT));
	CPen penGray(PS_SOLID, 3, ::GetSysColor(COLOR_BTNFACE));
	CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen penBlack(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));

	static CBrush* br = new CBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(rcClient, br);

	/*CPen* pOldPen = (CPen*)dc.SelectObject(&penWhite);
	dc.MoveTo(rcClient.left, nTop);
	dc.LineTo(rcClient.right, nTop);

	dc.SelectObject(&penGray);
	dc.MoveTo(rcClient.left, nTop + 2);
	dc.LineTo(rcClient.right, nTop + 2);

	dc.SelectObject(&penDkGray);
	dc.MoveTo(rcClient.left, nTop + 4);
	dc.LineTo(rcClient.right, nTop + 4);

	dc.SelectObject(&penBlack);
	dc.MoveTo(rcClient.left, nTop + 5);
	dc.LineTo(rcClient.right, nTop + 5);

	dc.SelectObject(pOldPen);*/

}

LRESULT CAdapter::OnKeyCtrlMoved(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	OnRegenTag(wParam, lParam);

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);
	CStringArray* pstrArray = (CStringArray*)pobArrayBeforeOp->GetAt(1);
	pstrArray->RemoveAll();
	for (int i = 0; i < m_strArrayTmpTag.GetSize(); i++)
		pstrArray->Add(m_strArrayTmpTag[i]);

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);

	return 0L;
}

LRESULT CAdapter::OnKeyCtrlTobeMoved(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	m_strArrayTmpTag.RemoveAll();
	WriteTagToArray(strCourse, m_strArrayTmpTag);

	return 0L;
}

LRESULT CAdapter::OnRegenTag(WPARAM wParam, LPARAM lParam)
{
	m_pTimePanel->GetTagFromUI(m_strArrayMarkTime, m_strArrayMarkName, 
							   m_strArrayScriptTime, m_strArrayScriptName, m_strArrayScriptCmd);

	//add by buxiangyi
	if (m_strArrayMarkName.GetSize() == 0)
	{
		m_strArrayLayoutName.RemoveAll();
	} 
	else
	{
		if (MACRO_VIEW -> LayoutExistFlag == TRUE)
		{
			m_strArrayLayoutName.RemoveAt(MACRO_VIEW -> CountNumber -1);
		} 
	}

	CString strCourse = MACRO_MAINFRM->GetCourse();
	WriteTagFile(strCourse);

	SetHyperLinkToGrid();

	return 0L;
}

LRESULT CAdapter::OnKeyCtrlPressing(WPARAM wParam, LPARAM lParam)
{
	if (m_pMovieWnd->IsRunning())
		m_pMovieWnd->Pause();

	return 0L;
}

BOOL CAdapter::GetVideoPosAtTime(long lTime, long& lVideoPos, CString& strFileName)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	BOOL bRet = m_pTimePanel->GetVideoPosAtTime(lTime, lVideoPos, strFileName);

	if (strFileName == "*")
		strFileName = MACRO_MP4_FILE(strCourse);

	return bRet;
}

LRESULT CAdapter::OnModifyVideo(WPARAM wParam, LPARAM lParam)
{
	ModifyVideo();
	return 0L;
}

void CAdapter::ModifyVideo()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CString strSeg;
	CString strSubPath = MACRO_COURSE_PATH + strCourse + "\\video\\";
	m_pTimePanel->GetVideoSegString(strSeg, MACRO_MP4_FILE(strCourse), TRUE, strSubPath);
	m_pMovieWnd->SetTimeLine(strSeg);

	m_pTimePanel->GetVideoSegString(strSeg, MACRO_MP4_FILE(strCourse), FALSE, "");
	WriteRecordFile(MACRO_RECORD_VIDEO(strCourse), strSeg);

	SetAudioReplace(strCourse);
	SetAudioMix(strCourse);
	SetVideoText(strCourse);
	SetVideoEffect(strCourse);

	return;
}

LRESULT CAdapter::OnWaitForFile(WPARAM wParam, LPARAM lParam)
{
	MACRO_MAINFRM->SendMessage(WM_WAIT_FOR_FILE, wParam, lParam);
	return 0L;
}

void CAdapter::ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd)
{
	m_pTimePanel->ReplacePeriodByFile(strVideoPath, lTimeStart, lTimeEnd);
}

LRESULT CAdapter::OnGrdUpdate(WPARAM wParam, LPARAM lParam)
{
	int nRow = HIWORD(lParam);
	int nCol = LOWORD(lParam);

	CString strOrigData, strNewData;
	UpdateGrid(nRow, nCol, strOrigData, strNewData);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "hyperlink_upd", strOrigData, strNewData, nRow, nCol);

	return 0L;
}

void CAdapter::UpdateGrid(int nRow, int nCol, CString& strOrigData, CString& strNewData)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	if (nCol == 1)
	{
		CString strData = m_pGrid->GetDataAt(nRow, nCol);
		strNewData = strData;
		int nIndex = 0;
		for (int i = 0; i < m_strArrayMarkName.GetSize(); i++)
		{
			CString strMark = m_strArrayMarkName[i];
			if (::IsCtrlMark(strMark))
				continue;
			if (nRow == nIndex)
			{
				strMark = strMark.Left(5);
				strOrigData = m_strArrayMarkName[i].Mid(5);
				m_strArrayMarkName[i] = strMark + strData;
				m_pTimePanel->SetKeyFrame(m_strArrayMarkName, m_strArrayMarkTime, 
										  m_strArrayScriptName, m_strArrayScriptCmd, m_strArrayScriptTime);
				WriteTagFile(strCourse);
				break;
			}
			nIndex++;
		}
	}

	return;
}

BOOL CAdapter::AddControlPoint(CString strTime, CString strMark, CString strScript, CString strCmd)
{
	int i = 0;
	for (i = 0; i < m_strArrayMarkTime.GetSize(); i++)
	{
		if (m_strArrayMarkTime[i] == strTime)
			return FALSE;
	}

	for (i = 0; i < m_strArrayScriptTime.GetSize(); i++)
	{
		if (m_strArrayScriptTime[i] == strTime)
			return FALSE;
	}
	//add by buxiangyi
	CString DefaultLayout = GetDefaultLatout();

	BOOL bFound = FALSE;
	if (strMark != "")
	{
		for (i = 0; i < m_strArrayMarkTime.GetSize(); i++)
		{
			CString strT = m_strArrayMarkTime[i];
			if (strT >= strTime)
			{
				m_strArrayMarkTime.InsertAt(i, strTime);
				m_strArrayMarkName.InsertAt(i, strMark);
				//add by buxiangyi
				m_strArrayLayoutName.InsertAt(i,DefaultLayout);
				m_RowIndex = i;
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
		{
			m_strArrayMarkTime.Add(strTime);
			m_strArrayMarkName.Add(strMark);
			m_strArrayLayoutName.Add(DefaultLayout);
		}
	}

	bFound = FALSE;
	for (i = 0; i < m_strArrayScriptTime.GetSize(); i++)
	{
		CString strT = m_strArrayScriptTime[i];
		if (strT >= strTime)
		{
			m_strArrayScriptTime.InsertAt(i, strTime);
			m_strArrayScriptName.InsertAt(i, strScript);
			m_strArrayScriptCmd.InsertAt(i, strCmd);
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		m_strArrayScriptTime.Add(strTime);
		m_strArrayScriptName.Add(strScript);
		m_strArrayScriptCmd.Add(strCmd);
	}

	m_pTimePanel->SetKeyFrame(m_strArrayMarkName, m_strArrayMarkTime, 
							  m_strArrayScriptName, m_strArrayScriptCmd, m_strArrayScriptTime);

	return TRUE;
}

BOOL CAdapter::AddEvent(long lTime, long lID)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CString strTime = ::ConvertTimeToStr(lTime);
	
	char szBuf[32];
	CString strMark;	

	if (lID == ID_ADD_ZOOM_IN)
	{
		strMark = ltoa(GetTickCount() + 10000, szBuf, 10);
		if (!AddControlPoint(strTime, ::MakeCtrlMark(strMark), strMark, "zoomin"))
		{
			W_SHOW_ERROR(IDS_CONTROL_POINT_EXISTS, this);
			return FALSE;
		}
	}
	else if (lID == ID_ADD_ZOOM_OUT)
	{
		strMark = ltoa(GetTickCount() + 10000, szBuf, 10);
		if (!AddControlPoint(strTime, ::MakeCtrlMark(strMark), strMark, "zoomout"))
		{
			W_SHOW_ERROR(IDS_CONTROL_POINT_EXISTS, this);
			return FALSE;
		}
	}
	else if (lID == ID_ADD_HTML)
	{
		CString strTmp;
		strTmp.LoadString(IDS_HTM_FILE);
		CFileDialog* pDlg = new CFileDialog(TRUE, "htm;html", NULL, 
											OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
											strTmp, NULL);
		if (pDlg->DoModal() == IDCANCEL)
		{
			delete pDlg;
			return FALSE;
		}

		CString strPath = pDlg->GetPathName();
		delete pDlg;

		CString strExt = ::GetFileExt(strPath);
		strExt.MakeLower();

		int nType = -1;
		if (strExt == "htm" || strExt == "html")
			nType = 0;
		else if (strExt == "doc" || strExt == "rtf")
			nType = 1;
		else if (strExt == "xls")
			nType = 2;
		else if (strExt == "bmp" || strExt == "gif" || strExt == "jpg" || strExt == "png" || 
				 strExt == "tif" || strExt == "wmf" || strExt == "ras" || strExt == "eps" || 
				 strExt == "pcx" || strExt == "tga" || strExt == "pcd"||strExt == "swf")
			nType = 3;

		if (nType < 0)
			return FALSE;
		
		int nCurIndex = GetCurrentWebPageIndex();
		CString strDestPath, strPathIndex;
		strPathIndex.Format("%d", nCurIndex + 1);
		strDestPath = MACRO_COURSE_PATH + strCourse + "\\webpages\\" + strPathIndex;
		CString strDestHTM = strDestPath + "\\index.htm";

		BeginWaitCursor();

		::TryToMkDir(strDestPath);

		CString strTitle = "";
		BOOL bRet;
		if (nType == 0)
		{
			bRet = ::CopyHTMLFile(strPath, strDestHTM, TRUE);
			::GetHTMLTitle(strPath, strTitle);
		}
		else if (nType == 1)
			bRet = ::Word2HTML(strPath, strDestHTM);
		else if (nType == 2)
			bRet = ::Excel2HTML(strPath, strDestHTM);
		else if (nType == 3)
		{
			CString strImageFile = ::GetFileName(strPath);
			bRet = ::SafeCopyFile(strPath, strDestPath + "\\" + strImageFile);

			CStringArray strHTMLData;
			strHTMLData.Add("<html><head></head><body>");
//			strHTMLData.Add("<img border=\"0\" src=\"" + strImageFile + "\">");	// del itoh 2013/01/08 BUGNO33
			if (strExt == "swf")
			{
			  MakeFlashObject(strHTMLData,strImageFile);
			}
			else
				strHTMLData.Add("<p><img border=\"0\" src=\"" + strImageFile + "\"></p>");
			strHTMLData.Add("</body></html>");
			::SaveFile(strDestHTM, strHTMLData, TRUE);
		}

		if (!bRet)
		{
			W_SHOW_ERROR(IDS_FAIL_TO_CONVERT_HTM, this);
			EndWaitCursor();
			return FALSE;
		}

		if (strTitle == "")
		{
			CString strFileName = ::GetFileName(strPath);
			int nIndex = strFileName.ReverseFind('.');
			if (nIndex >= 0)
				strTitle = strFileName.Left(nIndex);
		}
		
		EndWaitCursor();		
		
		CString strMark;
		strMark.Format("w%03d %s", nCurIndex + 1, strTitle);
		if (!AddControlPoint(strTime, strMark, strPathIndex + "=", "flippage"))
		{
			W_SHOW_ERROR(IDS_CONTROL_POINT_EXISTS, this);
			return FALSE;
		}

		SetHyperLinkToGrid();
	}
	else if (lID == ID_ADD_SCREEN_CAPTURE)
	{
		int nIndex = GetCurrentDemoIndex();
		strMark.Format("%d", nIndex + 1);
		if (!AddControlPoint(strTime, ::MakeDemoMark(strMark), strMark, "demo"))
		{
			W_SHOW_ERROR(IDS_CONTROL_POINT_EXISTS, this);
			return FALSE;
		}
	}

	WriteTagFile(strCourse);

	return TRUE;
}
void CAdapter::MakeFlashObject(CStringArray& strFlash,CString strFilename)
{

  CString strTmp;
  strFlash.Add("<object classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" codebase=\"http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,19,0\" width=\"720\" height=\"600\" >");
  strTmp="<param name=\"movie\" value=\""+strFilename+"\" />";
  strFlash.Add(strTmp);
  strFlash.Add("<param name=\"quality\" value=\"high\" />");
  strTmp= "<embed src=\""+strFilename+"\"" +" quality=\"high\" pluginspage=\"http://www.macromedia.com/go/getflashplayer\" type=\"application/x-shockwave-flash\" ></embed>";
  strFlash.Add(strTmp);
  strFlash.Add("</object>");


 


}
int CAdapter::GetCurrentWebPageIndex()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return -1;

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\webpages\\";
	if (!::FileExists(strPath))
		_mkdir(strPath);

	CStringArray strArray;
	::GetDir(strPath, strArray);
	int nIndex = 0;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int n = atoi(str);
		if (n > nIndex)
			nIndex = n;
	}

	return nIndex;
}

int CAdapter::GetCurrentDemoIndex()
{
	int nIndex = 0;
	for (int i = 0; i < m_strArrayScriptTime.GetSize(); i++)
	{
		CString strName = m_strArrayScriptName[i];
		CString	strCmd = m_strArrayScriptCmd[i];
		if (strCmd == "demo")
		{
			int n = atoi(strName);
			if (n > nIndex)
				nIndex = n;
		}
	}

	return nIndex;
}

BOOL CAdapter::SaveVideo()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;
	
	return SaveCourse(strCourse);
}

BOOL CAdapter::SaveTag(CString strCourse)
{
	CString strDefaultFile = MACRO_FINAL_MP4_FILE(strCourse);
	return SaveTagToFile(strDefaultFile);
}

BOOL CAdapter::SaveTagToFile(CString strDefaultFile)
{
	if (!MACRO_MAINFRM->SaveTag(strDefaultFile, m_strArrayMarkTime, m_strArrayMarkName,
								m_strArrayScriptTime, m_strArrayScriptName, m_strArrayScriptCmd, TRUE))
	{
		W_SHOW_ERROR(IDS_FAIL_TO_TAG, this);
		return FALSE;
	}

	return TRUE;
}

BOOL CAdapter::SaveVideoToFile(CString strCourse, CString strDestPath)
{
	m_bVideoAdjusted == FALSE;
	if (strCourse == "")
		return TRUE;

	CString strProfilePath = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";

	CString strVideoPath = MACRO_COURSE_PATH + strCourse + "\\video\\_temp.mp4";
	if (::FileExists(strVideoPath))
		if (!::SureDeleteFile(strVideoPath))
		{
			AfxMessageBox("#2 正常に動画が保存できていません。\n\nブラウザでプレビューが起動している場合はブラウザを閉じてください、動画ファイルが使用中の場合は動画ファイルの使用を終了してください。MCGProを再起動後に再度メディアファイルを保存してください。");
			return FALSE;
		}
	
	CString strIniPath = MACRO_INI_PATH;
	int n = ::GetPrivateProfileInt("Option", "ConvertMethod", 0, strIniPath);
	n=1; //wangwang
	CString strBase = MACRO_MP4_FILE(strCourse);
	BOOL bRet = m_pMovieWnd->SaveVideo(strVideoPath, strBase, 1 - n); 
	if (!bRet)
	{
		W_SHOW_ERROR(IDS_FAIL_TO_SAVE_VIDEO, this);
		AfxMessageBox("#3 正常に動画が保存できていません。\n\nブラウザでプレビューが起動している場合はブラウザを閉じてください、動画ファイルが使用中の場合は動画ファイルの使用を終了してください。MCGProを再起動後に再度メディアファイルを保存してください。");
		return FALSE;
	}

	if (::FileExists(strDestPath))
		if (!::SureDeleteFile(strDestPath))
		{
			AfxMessageBox("#4 正常に動画が保存できていません。\n\nブラウザでプレビューが起動している場合はブラウザを閉じてください、動画ファイルが使用中の場合は動画ファイルの使用を終了してください。MCGProを再起動後に再度メディアファイルを保存してください。");
			return FALSE;
		}
	
	if (!MACRO_MAINFRM->CreateIndex(strVideoPath))
	{
		W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
		AfxMessageBox("#5 正常に動画が保存できていません。\n\nブラウザでプレビューが起動している場合はブラウザを閉じてください、動画ファイルが使用中の場合は動画ファイルの使用を終了してください。MCGProを再起動後に再度メディアファイルを保存してください。");
		return FALSE;
	}

	if (rename(strVideoPath, strDestPath) == 0)
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox("#6 正常に動画が保存できていません。\n\nブラウザでプレビューが起動している場合はブラウザを閉じてください、動画ファイルが使用中の場合は動画ファイルの使用を終了してください。MCGProを再起動後に再度メディアファイルを保存してください。");
		return FALSE;
	}
}

BOOL CAdapter::SaveVideo(CString strCourse)
{
	if (strCourse == "")
		return TRUE;
    //wangwang20071217
	LoadCourse(strCourse, true) ;
	CString strDestPath = MACRO_FINAL_MP4_FILE(strCourse);
	if (::FileExists(strDestPath))
		if (!::SureDeleteFile(strDestPath))
			return FALSE;
	return SaveVideoToFile(strCourse, strDestPath);
}

/*
BOOL CAdapter::IsVideoModified()
{
	return m_pTimePanel->IsVideoModified();
}
*/

void CAdapter::ParseStringToArray(CString strSeg, CStringArray& strArraySeg)
{
	int nCount = 0;
	CString strTok;
	while (::GetTokStr(strSeg, strTok, nCount, ";"))
		strArraySeg.Add(strTok);
}
BOOL CAdapter::GetFilenameforrecordfile(CString& strSeg)
{

  	    //int IFindComma;
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
	
	
		IFindSemiColon=strSeg.Find(';',IStart);
        int hhh=0;
		while (IFindSemiColon >=0)
		{

			StrSentence=strSeg.Mid(IStart-1,IFindSemiColon-IStart+1);
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
            int IFind2=StrSentence.Find(',',IFind+1);
            CString sFileName=StrSentence.Mid(IFind+1,IFind2-IFind-1);
			sFileName=GetFileName(sFileName);
			StrSentence=StrSentence.Left(IFind)+","+sFileName+StrSentence.Right(ILen-IFind2);
			if (hhh >0)
               StrSentence+=";";
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+2;
			IFindSemiColon=strSeg.Find(';',IStart);
			hhh++;
		}
		//m_pMovieWnd->SetVideoText(StrFinal);
	    strSeg=StrFinal;
		return true;

}

void CAdapter::WriteRecordFile(CString strPath, CString strSeg)
{

	CStringArray strArraySeg;
	GetFilenameforrecordfile(strSeg);
	ParseStringToArray(strSeg, strArraySeg);
	::SaveFile(strPath, strArraySeg, TRUE);

	
}

BOOL CAdapter::LoadVideoFile(CString strCourse)
{
	CStringArray strArrayVideo;
	CString strPath = MACRO_RECORD_VIDEO(strCourse);
	if (::FileExists(strPath))
	{
		::LoadFile(strPath, strArrayVideo, TRUE);
		m_pTimePanel->SetVideoSegArray(strArrayVideo);
		return TRUE;
	}
	return FALSE;
}

BOOL CAdapter::LoadAudioReplaceFile(CString strCourse)
{
	CStringArray strArrayVideo;
	CString strPath = MACRO_RECORD_AUDIO_REPLACE(strCourse);
	if (::FileExists(strPath))
	{
		::LoadFile(strPath, strArrayVideo, TRUE);
		m_pTimePanel->SetAudioReplaceSegArray(strArrayVideo);
		return TRUE;
	}
	else
		m_pTimePanel->SetAudioReplaceSegArray(strArrayVideo);
	return FALSE;
}

BOOL CAdapter::LoadAudioMixFile(CString strCourse)
{
	CStringArray strArrayVideo;
	CString strSysIniPath = MACRO_INI_PATH;
//		::GetPrivateProfileString("General", "DefaultOutlook", "", szBuf, 255, strSysIniPath);
	::WritePrivateProfileString("General", "CourseName", strCourse, strSysIniPath);
	CString strPath = MACRO_RECORD_AUDIO_MIX(strCourse);
	if (::FileExists(strPath))
	{
		::LoadFile(strPath, strArrayVideo, TRUE);
		m_pTimePanel->SetAudioMixSegArray(strArrayVideo);
		return TRUE;
	}
	else
		m_pTimePanel->SetAudioMixSegArray(strArrayVideo);
	
	return FALSE;
}

void CAdapter::LoadVideoTextFromArray(CStringArray& strArrayVideo,CString strCourse)
{
	m_pTimePanel->SetVideoTextSegArray(strArrayVideo);

	int nCount = m_pTimePanel->GetVideoTextSegCount();
	for (int i = 0; i < nCount; i++)
	{
		long lTimeStart, lTimeEnd;
		CString strPath;
		int nType;
		m_pTimePanel->GetVideoTextSegInfo(i, lTimeStart, lTimeEnd, strPath, nType);

		char szBuf[512];
		//wangwang20071217
		strPath=MACRO_VIDEO_DIR(strCourse)+strPath;
		::GetPrivateProfileString("DATA", "HINT", "", szBuf, 512, strPath);
		m_pTimePanel->SetVideoTextSegTip(i, szBuf);
	}

	m_pTimePanel->ResetVideoTextTooltip();
}

BOOL CAdapter::LoadVideoTextFile(CString strCourse)
{
	CStringArray strArrayVideo;
	CString strPath = MACRO_RECORD_VIDEO_TEXT(strCourse);
	if (::FileExists(strPath))
	{
		::LoadFile(strPath, strArrayVideo, TRUE);
		LoadVideoTextFromArray(strArrayVideo,strCourse);
		return TRUE;
	}
	else
		m_pTimePanel->SetVideoTextSegArray(strArrayVideo);
	return FALSE;
}

BOOL CAdapter::LoadVideoEffectFile(CString strCourse)
{
	CStringArray strArrayVideo;
	CString strPath = MACRO_RECORD_VIDEO_EFFECT(strCourse);
	if (::FileExists(strPath))
	{
		::LoadFile(strPath, strArrayVideo, TRUE);
		m_pTimePanel->SetVideoEffectSegArray(strArrayVideo);
		return TRUE;
	}
	else
		m_pTimePanel->SetVideoEffectSegArray(strArrayVideo);
	return FALSE;
	//wang20080426



}

void CAdapter::WriteTagToArray(CString strCourse, CStringArray& strTagArray)
{
	if (strCourse == "")
		return;

	CString strTime, strScript, strCmd;
	int i = 0;
	for (i = 0; i < m_strArrayScriptTime.GetSize(); i++)
	{
		strTime = m_strArrayScriptTime[i];
		strScript = m_strArrayScriptName[i];
		strCmd = m_strArrayScriptCmd[i];
		strTagArray.Add(strTime + " " + strCmd + " " + strScript);
	}

	strTagArray.Add("[Marker]");
	for (i = 0; i < m_strArrayMarkTime.GetSize(); i++)
	{
		strTime = m_strArrayMarkTime[i];
		strCmd = m_strArrayMarkName[i];
		strTagArray.Add(strTime + " " + strCmd);
	}
}

void CAdapter::WriteTagFile(CString strCourse)
{
	CStringArray strArrayTag;
	WriteTagToArray(strCourse, strArrayTag);

	CString strPath = MACRO_RECORD_TAG(strCourse);
	::SaveFile(strPath, strArrayTag, TRUE);
	strPath = MACRO_STEP_FILE(strCourse);
	::SureDeleteFile(strPath);
	strPath = MACRO_OUTLINE_FILE(strCourse);
	::SureDeleteFile(strPath);
}

BOOL CAdapter::LoadTagFile(CString strCourse)
{
	CString strPath = MACRO_RECORD_TAG(strCourse);
	if (::FileExists(strPath))
	{
		CStringArray strArrayTag;
		::LoadFile(strPath, strArrayTag, TRUE);
		LoadTagFromStringArray(strArrayTag);
		return TRUE;
	}

	return FALSE;
}

void CAdapter::LoadTagFromStringArray(CStringArray& strArrayTag)
{
	m_strArrayMarkName.RemoveAll();
	m_strArrayMarkTime.RemoveAll();
	m_strArrayScriptName.RemoveAll();
	m_strArrayScriptTime.RemoveAll();
	m_strArrayScriptCmd.RemoveAll();

	CString strLine, strTok;
	int nCount;
	int nMarkerIndex = strArrayTag.GetSize();
	int i = 0;
	for (i = 0; i < strArrayTag.GetSize(); i++)
	{
		strLine = strArrayTag[i];
		if (strLine == "[Marker]")
		{
			nMarkerIndex = i + 1;
			break;
		}
		
		nCount = 0;
		::GetTokStr(strLine, strTok, nCount, " ");
		m_strArrayScriptTime.Add(strTok);

		::GetTokStr(strLine, strTok, nCount, " ");
		m_strArrayScriptCmd.Add(strTok);

		::GetTokStr(strLine, strTok, nCount, " ");
		m_strArrayScriptName.Add(strTok);
	}

	for (i = nMarkerIndex; i < strArrayTag.GetSize(); i++)
	{
		strLine = strArrayTag[i];

		nCount = 0;
		::GetTokStr(strLine, strTok, nCount, " ");
		m_strArrayMarkTime.Add(strTok);

		strTok = strLine.Mid(nCount);
		m_strArrayMarkName.Add(strTok);
	}
}

BOOL CAdapter::AddWave(long lTime, long lID)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CString StrTemp;
	//StrTemp.LoadString(IDS_WAV_FILE);
	StrTemp = "音声ファイル (*.m4a;)|*.m4a;|メディアファイル (*.mp4;)|*.mp4;||";
 	CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, 
									    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
									    StrTemp, NULL);

	if (pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;
		return FALSE;
	}

	CString strPath = pDlg->GetPathName();
	delete pDlg;

	//20150327
	if (strPath.Find("mp4") <= 0 && strPath.Find("m4a") <= 0)
	{
		W_SHOW_ERROR(IDS_AUDIO_FILE_NOT_SUPPORT, this);
	}
	
	UpdateWindow();

	long lDuration = MACRO_MAINFRM->GetMediaLength(strPath);

	//add by buxiangyi
	CStringArray m_oldarray, array;
	CString m_time, strjsPath;
	m_time.Format("%d",lDuration/1000);
	m_oldarray.Add("var vduration =" );
	array.Add("var vduration = " + m_time + ";");
	CString strConfigPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	UpdateFileByTag(strConfigPath, "config.js", m_oldarray, array, TRUE);

	long lTimeEnd = lTime + lDuration;
	BOOL bOverlap = (lID == ID_ADD_AUDIO_REPLACE && m_pTimePanel->IsAudioReplaceOverlap(lTime, lTimeEnd)) ||
					(lID == ID_ADD_AUDIO_MIX && m_pTimePanel->IsAudioMixOverlap(lTime, lTimeEnd));
	if (bOverlap)
	{
		W_SHOW_ERROR(IDS_OVERLAP_WITH_EXISTING, this);
		return FALSE;
	}

	CString strVideoPath = MACRO_COURSE_PATH + strCourse + "\\video";
	int nIndex = ::GetIndex(strVideoPath, "wma", "wma_tmp");

	CString strFile;
	strFile.Format("wma_tmp%d.wma", nIndex + 1);
	CString strDestPath = strVideoPath + CString("\\") + strFile;
	CString strProfilePath;

	if (!MACRO_MAINFRM->ConvertAudio(strPath, strProfilePath, strDestPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_CONVERT_WMA);
		strMsg.Format(strTmp, strPath);
		W_SHOW_ERROR_STR(strMsg, this);
	}

	//lDuration = MACRO_MAINFRM->GetMediaLength(strDestPath);
	//add by buxiangyi
	m_oldarray.RemoveAll();
	array.RemoveAll();
	m_time.Format("%d",lDuration/1000);
	m_oldarray.Add("var vduration =" );
	array.Add("var vduration = " + m_time + ";");
	strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

	lTimeEnd = lTime + lDuration;
	CString strSeg;
	if (lID == ID_ADD_AUDIO_REPLACE)
	{
		m_pTimePanel->AddAudioReplace(lTime, lTimeEnd, strDestPath);
		SetAudioReplace(strCourse);
	}
	else if (lID == ID_ADD_AUDIO_MIX)
	{
		m_pTimePanel->AddAudioMix(lTime, lTimeEnd, strDestPath);
		SetAudioMix(strCourse);
	}

	return TRUE;
}

BOOL CAdapter::AddText(long lTime)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CDlgInputCaption dlg;
	UINT nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
		return FALSE;

	CString strCaption = dlg.m_strCaption;

	CString strIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("Option", "DefaultCaptionTime", "10", szBuf, 256, strIniPath);
	long lTimeEnd = lTime + max(1000, atol(szBuf) * 1000);
	BOOL bOverlap = m_pTimePanel->IsVideoTextOverlap(lTime, lTimeEnd);
	if (bOverlap)
	{
		W_SHOW_ERROR(IDS_EFFECT_OVERLAP_WITH_EXISTING, this);
		return FALSE;
	}

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\video";
	int nIndex = ::GetIndex(strPath, "vfx", "caption");
	CString strFile;
	strFile.Format("caption%d.vfx", nIndex + 1);
	CString strDestPath = strPath + "\\" + strFile;
	CString strSrcPath = MACRO_CAPTION_FILE;
	::SafeCopyFile(strSrcPath, strDestPath, FALSE);

	BeginWaitCursor();
	SetCaptionData(strDestPath, strCaption, dlg.m_strFontName, dlg.m_strFontSize, dlg.m_strItalic,
				   dlg.m_strBold, dlg.m_strUnderline, dlg.m_strPosition, dlg.m_strHIndent,
				   dlg.m_strVIndent, dlg.m_strBkMode, dlg.m_strTextColor, dlg.m_strBkColor, nRet == ID_APPLY);

	m_pTimePanel->AddVideoText(lTime, lTimeEnd, strDestPath, strCaption);
	SetVideoText(strCourse);
	EndWaitCursor();

	return TRUE;
}

LRESULT CAdapter::OnPeriodTobeMoved(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	m_strArrayTmpTag.RemoveAll();
	CString strTmp;
	if (lParam == ID_AUDIO_REPLACE)
		m_pTimePanel->GetAudioReplaceSegString(strTmp);
	else if (lParam == ID_AUDIO_MIX)
		m_pTimePanel->GetAudioMixSegString(strTmp);
	else if (lParam == ID_VIDEO_TEXT)
		m_pTimePanel->GetVideoTextSegString(strTmp);
	else if (lParam == ID_VIDEO_EFFECT)
		m_pTimePanel->GetVideoEffectSegString(strTmp);
	ParseStringToArray(strTmp, m_strArrayTmpTag);

	return 0L;
}

LRESULT CAdapter::OnPeriodMoved(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	CString strSeg;
	int nIndex;
	if (lParam == ID_AUDIO_REPLACE)
	{
		SetAudioReplace(strCourse);
		nIndex = 2;
	}
	else if (lParam == ID_AUDIO_MIX)
	{
		SetAudioMix(strCourse);
		nIndex = 3;
	}
	else if (lParam == ID_VIDEO_TEXT)
	{
		SetVideoText(strCourse);
		nIndex = 4;
	}
	else if (lParam == ID_VIDEO_EFFECT)
	{
		SetVideoEffect(strCourse);
		nIndex = 5;
	}
	else
	{
		return 0L;
	}

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);
	CStringArray* pstrArray = (CStringArray*)pobArrayBeforeOp->GetAt(nIndex);
	pstrArray->RemoveAll();
	for (int i = 0; i < m_strArrayTmpTag.GetSize(); i++)
		pstrArray->Add(m_strArrayTmpTag[i]);

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);
    //wangwang20080222
	return 0L;
}

LRESULT CAdapter::OnPeriodDelete(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);

	if (lParam == ID_AUDIO_REPLACE)
	{
		m_pTimePanel->DeleteAudioReplaceByIndex(wParam);
		SetAudioReplace(strCourse);
	}
	else if (lParam == ID_AUDIO_MIX)
	{
		m_pTimePanel->DeleteAudioMixByIndex(wParam);
		SetAudioMix(strCourse);
	}
	else if (lParam == ID_VIDEO_TEXT)
	{
		m_pTimePanel->DeleteVideoTextByIndex(wParam);
		SetVideoText(strCourse);
	}

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);

	return 0L;
}

LRESULT CAdapter::OnPeriodView(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();


	long lTimeStart, lTimeEnd;
	CString strPath;
	int nType;
	if (lParam == ID_AUDIO_REPLACE)
		m_pTimePanel->GetAudioReplaceSegInfo(wParam, lTimeStart, lTimeEnd, strPath, nType);
	else if (lParam == ID_AUDIO_MIX)
		m_pTimePanel->GetAudioMixSegInfo(wParam, lTimeStart, lTimeEnd, strPath, nType);
    strPath=MACRO_VIDEO_DIR(strCourse)+strPath;
	::ShellExecute(this->m_hWnd, "open", strPath, NULL, NULL, SW_SHOWNORMAL);

	return 0L;
}

LRESULT CAdapter::OnPeriodEdit(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	long lTimeStart, lTimeEnd;
	CString strPath;
	int nType;
	if (lParam == ID_VIDEO_TEXT)
	{
		m_pTimePanel->GetVideoTextSegInfo(wParam, lTimeStart, lTimeEnd, strPath, nType);
		char szBuf[512];
		int iFind=strPath.Find("\\");
		if (iFind  < 0)
		  strPath=MACRO_VIDEO_DIR(strCourse)+strPath;
		//wangwang20071217
		::GetPrivateProfileString("DATA", "HINT", "", szBuf, 512, strPath);
		CString str = szBuf;
		if (str != "")
		{
			if (str[0] == '\"')
				str = str.Mid(1);
			if (str[str.GetLength() - 1] == '\"')
				str = str.Left(str.GetLength() - 1);

			CDlgInputCaption dlg;
			dlg.m_strCaption = str;
			dlg.m_strIniPath = strPath;
			dlg.m_strSection = "DATA";
			UINT nRet = dlg.DoModal();
			if (nRet == IDOK || nRet == ID_APPLY)
			{
				BeginWaitCursor();
				CObArray* pObArrayBefore = new CObArray;
				CStringArray* pStraBrfore = new CStringArray();
				pStraBrfore->Add(strPath);
				pStraBrfore->Add(dlg.m_strCaptionBak);
				pStraBrfore->Add(dlg.m_strFontNameBak);
				pStraBrfore->Add(dlg.m_strFontSizeBak);
				pStraBrfore->Add(dlg.m_strItalicBak);
				pStraBrfore->Add(dlg.m_strBoldBak);
				pStraBrfore->Add(dlg.m_strUnderlineBak);
				pStraBrfore->Add(dlg.m_strPositionBak);
				pStraBrfore->Add(dlg.m_strHIndentBak);
				pStraBrfore->Add(dlg.m_strVIndentBak);
				pStraBrfore->Add(dlg.m_strBkModeBak);
				pStraBrfore->Add(dlg.m_strTextColorBak);
				pStraBrfore->Add(dlg.m_strBkColorBak);
				pObArrayBefore->Add(pStraBrfore);
				CObArray* pObArrayAfter = new CObArray;
				CStringArray* pStraAfter = new CStringArray();
				pStraAfter->Add(strPath);
				pStraAfter->Add(dlg.m_strCaption);
				pStraAfter->Add(dlg.m_strFontName);
				pStraAfter->Add(dlg.m_strFontSize);
				pStraAfter->Add(dlg.m_strItalic);
				pStraAfter->Add(dlg.m_strBold);
				pStraAfter->Add(dlg.m_strUnderline);
				pStraAfter->Add(dlg.m_strPosition);
				pStraAfter->Add(dlg.m_strHIndent);
				pStraAfter->Add(dlg.m_strVIndent);
				pStraAfter->Add(dlg.m_strBkMode);
				pStraAfter->Add(dlg.m_strTextColor);
				pStraAfter->Add(dlg.m_strBkColor);
				pObArrayAfter->Add(pStraAfter);
				SetCaptionData(strPath, dlg.m_strCaption, dlg.m_strFontName, dlg.m_strFontSize, dlg.m_strItalic,
							   dlg.m_strBold, dlg.m_strUnderline, dlg.m_strPosition, dlg.m_strHIndent,
							   dlg.m_strVIndent, dlg.m_strBkMode, dlg.m_strTextColor, dlg.m_strBkColor, nRet == ID_APPLY);

				CString strSeg; 
				m_pTimePanel->GetVideoTextSegString(strSeg);
				m_pMovieWnd->SetVideoText(strSeg);
				if (nRet != ID_APPLY)
					MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "caption_op", pObArrayBefore, pObArrayAfter);
				EndWaitCursor();
			}
		}
	}

	return 0L;
}

void CAdapter::SetCaptionData(CString strPath, CString strCaption, CString strFontName, CString strFontSize,
									CString strItalic, CString strBold, CString strUnderline, CString strPosition,
									CString strHIndent, CString strVIndent, CString strBkMode, CString strTextColor,
									CString strBkColor, BOOL bApplyAll)
{
	if (bApplyAll)
	{
		CString strFilePath = ::GetFilePath(strPath);
		CStringArray strArrayFiles;
		::GetFiles(strFilePath, "vfx", strArrayFiles);
		for (int i = 0; i < strArrayFiles.GetSize(); i++)
		{
			CString strFile = strArrayFiles[i];
			if (strFile.Left(7).CompareNoCase("caption") == 0)
			{
				CString strFullPath = strFilePath + strFile;
				SetOneCaptionData(strFullPath, "", strFontName, strFontSize, strItalic, 
								  strBold, strUnderline, strPosition, strHIndent, strVIndent, 
								  strBkMode, strTextColor, strBkColor);
			}
		}
	}
	else
	{
		SetOneCaptionData(strPath, strCaption, strFontName, strFontSize, strItalic, strBold, strUnderline, 
				      strPosition, strHIndent, strVIndent, strBkMode, strTextColor, strBkColor);
	}

}

void CAdapter::SetOneCaptionData(CString strPath, CString strCaption, CString strFontName, CString strFontSize,
									   CString strItalic, CString strBold, CString strUnderline, CString strPosition,
									   CString strHIndent, CString strVIndent, CString strBkMode, CString strTextColor,
									   CString strBkColor)
{
	if (strCaption != "")
		::WritePrivateProfileString("DATA", "HINT", "\"" + strCaption + "\"", strPath);
	::WritePrivateProfileString("DATA", "FontName", "\"" + strFontName + "\"", strPath);
	::WritePrivateProfileString("DATA", "FontSize", strFontSize, strPath);
	::WritePrivateProfileString("DATA", "FontItalic", strItalic, strPath);
	::WritePrivateProfileString("DATA", "FontBold", strBold, strPath);
	::WritePrivateProfileString("DATA", "FontUnderline", strUnderline, strPath);
	::WritePrivateProfileString("DATA", "Position", strPosition, strPath);
	::WritePrivateProfileString("DATA", "H_INDENT", strHIndent, strPath);
	::WritePrivateProfileString("DATA", "V_INDENT", strVIndent, strPath);
	::WritePrivateProfileString("DATA", "BkMode", strBkMode, strPath);
	::WritePrivateProfileString("DATA", "TextColor", strTextColor, strPath);
	::WritePrivateProfileString("DATA", "BkColor", strBkColor, strPath);

	int nVer = ::GetPrivateProfileInt("INFO", "DATAVERSION", 0, strPath);
	CString strTmp;
	strTmp.Format("%d", nVer + 1);
	::WritePrivateProfileString("INFO", "DATAVERSION", strTmp, strPath);
}


void CAdapter::SetAudioReplace(CString strCourse)
{
	CString strSeg;
	m_pTimePanel->GetAudioReplaceSegString(strSeg);
//	m_pMovieWnd->SetAudioReplace(strSeg);
	WriteRecordFile(MACRO_RECORD_AUDIO_REPLACE(strCourse), strSeg);
	//LoadCourse( strCourse, true) ;
	m_pMovieWnd->SetAudioReplace("");
	if (LoadAudioReplaceFile(strCourse))
	{
	
		//int IFindComma;
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
		
		m_pTimePanel->GetAudioReplaceSegString(strSeg);
        IFindSemiColon=strSeg.Find(';',IStart);
		while (IFindSemiColon >=0)
		{

			StrSentence=strSeg.Mid(IStart,IFindSemiColon-IStart+1);
			//StrSentence=strSeg.
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
			StrSentence=StrSentence.Left(IFind)+","+MACRO_VIDEO_DIR(strCourse)+StrSentence.Right(ILen-IFind-1);
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+1;
			IFindSemiColon=strSeg.Find(';',IStart);
		}
	
		m_pMovieWnd->SetAudioReplace(StrFinal);
	}
}

void CAdapter::SetAudioMix(CString strCourse)
{
	CString strSeg;
	//CString strSysIniPath = MACRO_INI_PATH;
	//::WritePrivateProfileString("General", "CourseName", strCourse, strSysIniPath);
	//wangwangmix
	m_pTimePanel->GetAudioMixSegString(strSeg);

	//m_pMovieWnd->SetAudioMix(strSeg);
	
	WriteRecordFile(MACRO_RECORD_AUDIO_MIX(strCourse), strSeg);
	m_pMovieWnd->SetAudioMix("");
	if (LoadAudioMixFile(strCourse))
	
//	CString strSysIniPath = MACRO_INI_PATH;
//	::WritePrivateProfileString("General", "CourseName", szCourse, strSysIniPath);
//wangwang
	

	{
		//int IFindComma;
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
		
		m_pTimePanel->GetAudioMixSegString(strSeg);
        IFindSemiColon=strSeg.Find(';',IStart);
		while (IFindSemiColon >=0)
		{

			StrSentence=strSeg.Mid(IStart,IFindSemiColon-IStart+1);
			//StrSentence=strSeg.
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
			StrSentence=StrSentence.Left(IFind)+","+MACRO_VIDEO_DIR(strCourse)+StrSentence.Right(ILen-IFind-1);
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+1;
			IFindSemiColon=strSeg.Find(';',IStart);
		}
	
		m_pMovieWnd->SetAudioMix(StrFinal);
	}
	//LoadCourse( strCourse, true) ;
	//LoadAudioReplaceFile(strCourse);
	//LoadAudioMixFile(strCourse);
	//wangwangmix

}

void CAdapter::SetVideoText(CString strCourse)
{
	CString strSeg;
	m_pTimePanel->GetVideoTextSegString(strSeg);
	//m_pMovieWnd->SetVideoText(strSeg);
	WriteRecordFile(MACRO_RECORD_VIDEO_TEXT(strCourse), strSeg);
	m_pMovieWnd->SetVideoText("");
	//wangwang20071225
	if (LoadVideoTextFile(strCourse))
	{
	  
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
		
		m_pTimePanel->GetVideoTextSegString(strSeg);
        IFindSemiColon=strSeg.Find(';',IStart);
		while (IFindSemiColon >=0)
		{

			StrSentence=strSeg.Mid(IStart,IFindSemiColon-IStart+1);
			//StrSentence=strSeg.
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
			StrSentence=StrSentence.Left(IFind)+","+MACRO_VIDEO_DIR(strCourse)+StrSentence.Right(ILen-IFind-1);
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+1;
			IFindSemiColon=strSeg.Find(';',IStart);
		}
		m_pMovieWnd->SetVideoText(StrFinal);//wangwang20080524 GetAudioandVideolrnAndSetArray(strCourse,strSeg,2);
	}
//	LoadVideoTextFile(strCourse);
}


void CAdapter::SetVideoEffect(CString strCourse)
{

	CString strSeg;
	m_pTimePanel->GetVideoEffectSegString(strSeg);
	//m_pMovieWnd->SetVideoEffect(strSeg);
	WriteRecordFile(MACRO_RECORD_VIDEO_EFFECT(strCourse), strSeg);
	m_pMovieWnd->SetVideoEffect("");

    if (LoadVideoEffectFile(strCourse))
	{
		int IFindSemiColon,IStart=0;
		CString StrSentence;
		CString StrFinal="";
		

		m_pTimePanel->GetVideoEffectSegString(strSeg);
        IFindSemiColon=strSeg.Find(';',IStart);
		while (IFindSemiColon >=0)
		{
			StrSentence=strSeg.Mid(IStart,IFindSemiColon-IStart+1);
			//StrSentence=strSeg.
			int ILen=StrSentence.GetLength();
			int IFind=StrSentence.Find(',');
			StrSentence=StrSentence.Left(IFind)+","+MACRO_VIDEO_DIR(strCourse)+StrSentence.Right(ILen-IFind-1);
			StrFinal+=StrSentence;
			IStart=IFindSemiColon+1;
			IFindSemiColon=strSeg.Find(';',IStart);
		}
	
		m_pMovieWnd->SetVideoEffect(StrFinal);
	  //wangwang20080824 GetAudioandVideolrnAndSetArray(strCourse,strSeg,3);
	}
}

LRESULT CAdapter::OnPeriodMoving(WPARAM wParam, LPARAM lParam)
{
	Pause(); 
	return 0L;
}

LRESULT CAdapter::OnRetrieveVideoData(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;
	CObArray* pObArray = (CObArray*)wParam;
	RetrieveVideoData(strCourse, pObArray);

	if (lParam)
	{
		CObArray* pObArray2 = (CObArray*)lParam;
		CompareAndProcessVideoData(pObArray, pObArray2);
	}

	return 0L;
}

LRESULT CAdapter::OnPauseVideoPlay(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	return 0L;
}

LRESULT CAdapter::OnPaletteButtonUp(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);

	BOOL bResult = FALSE;
	CString strTmp;
	switch (wParam)
	{
		case ID_BTN_MARK:
			m_pTimePanel->SetMark();
		break;

		case ID_BTN_DELETE:
			strTmp.LoadString(IDS_PROCESSING);
			MACRO_MAINFRM->SetProgress(strTmp, strTmp);
			MACRO_MAINFRM->ShowProgress();
			if (m_pTimePanel->DeletePeriod())
			{
				ModifyVideo();
				bResult = TRUE;
			}
			MACRO_MAINFRM->HideProgress();
		break;

		case ID_BTN_MUTE:
		{
			long lTimeSelStart, lTimeSelEnd;
			m_pTimePanel->GetSelRange(lTimeSelStart, lTimeSelEnd);
			if (lTimeSelStart < 0)
			{
				W_SHOW_WARNING(IDS_NOT_MARK_COMPLETE_FOR_MUTE, this);
				::ReleaseObArray(pobArrayBeforeOp);
				delete pobArrayBeforeOp;
				return 0L;
			}
			if (MutePeriod(lTimeSelStart, lTimeSelEnd))
				bResult = TRUE;
		}
		break;
			
		case ID_BTN_REPLACE:
			strTmp.LoadString(IDS_PROCESSING);
			MACRO_MAINFRM->SetProgress(strTmp, strTmp);
			MACRO_MAINFRM->ShowProgress();
			bResult = m_pTimePanel->ReplacePeriod();
			MACRO_MAINFRM->HideProgress();
		break;

		case ID_BTN_EDITMARK:
			if (lParam > 0)
				bResult = m_pTimePanel->EditMark(lParam, -1);
			else
				bResult = m_pTimePanel->EditMark(-1, -1);
			LoadHyperLink(strCourse);
		break;

		case ID_BTN_ADD_EVENT:
		{
			long lTime = m_pTimePanel->GetPointerTime();
			bResult = AddEvent(lTime, lParam);
		}
		break;

		case ID_BTN_SAVE_VIDEO:
			bResult = SaveVideo();
		break;

		case ID_BTN_ADD_WAVE:
		{
			long lTime = m_pTimePanel->GetPointerTime();
			bResult = AddWave(lTime, lParam);
			break;
		}

		case ID_BTN_ADD_TEXT:
		{
			long lTime = m_pTimePanel->GetPointerTime();
			bResult = AddText(lTime);
			break;
		}

		case ID_BTN_ADD_EFFECT:
		{
			long lTime = m_pTimePanel->GetPointerTime();
				//add by shien avoid addeffect error		part1
			CString strType;
			//CString strIniPath = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";//MACRO_INI_FILE(strCourse);
			CString strIniPath = MACRO_INI_FILE(strCourse);
			char szValue[256];
			::GetPrivateProfileString("Profile", "Dimension", "", szValue, 255, strIniPath);
			//if (::FileContainString(strIniPath, "<bitmapinfoheader biwidth=""160""") || ::FileContainString(strIniPath, "<bitmapinfoheader biwidth=""200"""))
			//::GetPrivateProfileString("", "<bitmapinfoheader biwidth", "", szValue, 255, strIniPath);
			strType = szValue;

			if (strType == "160 x 120" || strType == "200 x 150")
			{
					W_SHOW_WARNING(IDS_EFFECT_SIZE, this);
					::ReleaseObArray(pobArrayBeforeOp);
					delete pobArrayBeforeOp;
					return 0L;
			}
			else
			{				//add by shien avoid addeffect error	part2

				if (lTime == 0 || lTime == m_pMovieWnd->GetLength())
				{
					W_SHOW_WARNING(IDS_EFFECT_CANNOT_AT_ENDS, this);
					::ReleaseObArray(pobArrayBeforeOp);
					delete pobArrayBeforeOp;
					return 0L;
				}

			}				//add by shien avoid addeffect error	part3


			bResult = TransitEffect(-1, lTime);
			break;

		}

		case ID_BTN_SET_POINTS:
		{
			bResult = SetPPTPoints();
			break;
		}

		case ID_BTN_ADJUST:
		{
			CDlgAdjustVolAndVideo dlg;
			CObArray* pObaBefore = new CObArray();
			CObArray* pObaAfter = new CObArray();
			CStringArray *pStraBefore = new CStringArray();
			CStringArray *pStraAfter = new CStringArray();
			CString strObject;

			char szBuf[256];
			CString strIniPath = MACRO_INI_FILE(strCourse);
			::GetPrivateProfileString("General", "Contrast", "0", szBuf, 256, strIniPath);
			dlg.m_nContrastValue = atoi(szBuf) / 100 + 100;
			strObject.Format("%d", dlg.m_nContrastValue);
			pStraBefore->Add(strObject);
			::GetPrivateProfileString("General", "Light", "0", szBuf, 256, strIniPath);
			dlg.m_nLightValue = atoi(szBuf) / 100 + 100;
			strObject.Format("%d", dlg.m_nLightValue);
			pStraBefore->Add(strObject);
			::GetPrivateProfileString("General", "Volume", "0", szBuf, 256, strIniPath);
			dlg.m_nVolumeValue = atoi(szBuf) / 100 + 100;
			strObject.Format("%d", dlg.m_nVolumeValue);
			pStraBefore->Add(strObject);
			pObaBefore->Add(pStraBefore);
			if (dlg.DoModal() == IDOK)
			{
				int nContrastValue = (dlg.m_nContrastValue - 100) * 100;
				int nLightValue = (dlg.m_nLightValue - 100) * 100;
				int nVolumeValue = (dlg.m_nVolumeValue - 100) * 100;
				m_pMovieWnd->AdjustVideo(1, nContrastValue);
				m_pMovieWnd->AdjustVideo(0, nLightValue);
				m_pMovieWnd->AdjustAudio(0, nVolumeValue);
				strObject.Format("%d", dlg.m_nContrastValue);
				pStraAfter->Add(strObject);
				strObject.Format("%d", dlg.m_nLightValue);
				pStraAfter->Add(strObject);
				strObject.Format("%d", dlg.m_nVolumeValue);
				pStraAfter->Add(strObject);
				pObaAfter->Add(pStraAfter);
				MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "adjust_op", pObaBefore, pObaAfter);

				CString strTmp;
				strTmp.Format("%d", nContrastValue);
				::WritePrivateProfileString("General", "Contrast", strTmp, strIniPath);

				strTmp.Format("%d", nLightValue);
				::WritePrivateProfileString("General", "Light", strTmp, strIniPath);
				
				strTmp.Format("%d", nVolumeValue);
				::WritePrivateProfileString("General", "Volume", strTmp, strIniPath);
				m_bVideoAdjusted = TRUE;
			}
		}
	}

	if (bResult)
	{
		CObArray* pobArrayAfterOp = new CObArray;
		RetrieveVideoData(strCourse, pobArrayAfterOp);
		CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
		MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);
	}
	else
	{
		::ReleaseObArray(pobArrayBeforeOp);
		delete pobArrayBeforeOp;
	}

	return 0L;
}

void CAdapter::ChangeMarker(CStringArray& strArrayNewMarker)
{
	for (int i = 0; i <	m_strArrayMarkName.GetSize(); i++)
	{
		CString strMark = m_strArrayMarkName[i];
		if (::IsCtrlMark(strMark))
			continue;
		CString strIndex = strMark.Left(4);
		if (strIndex[0] == 's')		// del itoh 2013/07/30 PPT2013 BUG?
		if (strIndex[0] == 's' && i < strArrayNewMarker.GetSize())
		{
			int nIndex = atoi(strIndex.Mid(1)) - 1;
			m_strArrayMarkName[i] = strIndex + " " + strArrayNewMarker[nIndex];
		}
	}

	CString strCourse = MACRO_MAINFRM->GetCourse();
	WriteTagFile(strCourse);

	SetHyperLinkToGrid();
}

BOOL CAdapter::MutePeriod(long lTimeSelStart, long lTimeSelEnd)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	long lDuration = lTimeSelEnd - lTimeSelStart;
	long lTime = lTimeSelStart;
	long lTimeEnd = lTimeSelEnd;
	BOOL bOverlap = m_pTimePanel->IsAudioReplaceOverlap(lTime, lTimeEnd);
	if (bOverlap)
	{
		W_SHOW_ERROR(IDS_OVERLAP_WITH_EXISTING, this);
		return FALSE;
	}

	CString strVideoPath = MACRO_COURSE_PATH + strCourse + "\\video";
	int nIndex = ::GetIndex(strVideoPath, "wma", "mute_tmp");

	CString strFile;
	strFile.Format("mute_tmp%d.wma", nIndex + 1);
	CString strDestPath = strVideoPath + CString("\\") + strFile;
	CString strProfilePath;

	CString strTitle;
	strTitle.LoadString(ID_BTN_MUTE_FILE);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	if (!MACRO_MAINFRM->GenWMAForMute(lDuration, strVideoPath + "\\default.mp4", strDestPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_GEN_MUTE);
		strMsg.Format(strTmp, strDestPath);
		W_SHOW_ERROR_STR(strMsg, this);
		MACRO_MAINFRM->HideProgress();
		return FALSE;
	}

	m_pTimePanel->AddAudioReplace(lTime, lTimeEnd, strDestPath);
	SetAudioReplace(strCourse);

	MACRO_MAINFRM->HideProgress();

	return TRUE;
}

LRESULT CAdapter::OnFadeInEffect(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;;

	Pause(); 

	CString strIniPath = MACRO_INI_PATH;
	char szBuf[128];
	::GetPrivateProfileString("Option", "DefaultEffectTime", "3", szBuf, 128, strIniPath);

	long lDuration = max(1000, atol(szBuf) * 1000);

	CString strPath = MACRO_EFFECT_PATH + "FADE_IN.vfx";

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);

	AddEffect(strCourse, wParam, lDuration, 1, strPath, FALSE);
	SetVideoEffect(strCourse);

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);

	return 0L;
}

LRESULT CAdapter::OnFadeOutEffect(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;;

	Pause(); 

	CString strIniPath = MACRO_INI_PATH;
	char szBuf[128];
	::GetPrivateProfileString("Option", "DefaultEffectTime", "3", szBuf, 128, strIniPath);

	long lDuration = max(1000, atol(szBuf) * 1000);

	CString strPath = MACRO_EFFECT_PATH + "FADE_OUT.vfx";

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);

	AddEffect(strCourse, wParam, lDuration, 2, strPath, FALSE);
	SetVideoEffect(strCourse);

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);

	return 0L;
}

LRESULT CAdapter::OnTransitEffect(WPARAM wParam, LPARAM lParam)
{
	TransitEffect(lParam, wParam);
	return 0L;
}
	
BOOL CAdapter::TransitEffect(int nIndex, long lEffectTime)
{							
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	Pause(); 

	char szBuf[256];
	CString strIniPath = MACRO_INI_PATH;
	::GetPrivateProfileString("Option", "DefaultEffectTime", "3", szBuf, 256, strIniPath);
	long lDuration = max(1000, atol(szBuf) * 1000);

	szBuf[0] = '\0';
	if (nIndex >= 0)
	{
		long lTime, lDuration;
		CString strPath;
		int nType;
		m_pTimePanel->GetVideoEffectSegInfo(nIndex, lTime, lDuration, strPath, nType);
		::GetPrivateProfileString("INFO", "FileName", "", szBuf, 256, strPath);
	}
	else
	{
		BOOL bOverlap = m_pTimePanel->IsVideoEffectOverlap(lEffectTime, lDuration);
		if (bOverlap)
		{
			W_SHOW_ERROR(IDS_EFFECT_OVERLAP_WITH_EXISTING, this);
			return FALSE;
		}
	}

	CDlgSelectEffect dlg;
	dlg.m_strEffectPath = szBuf;
	if (dlg.DoModal() == IDOK)
	{
		UpdateWindow();

		CString strPath = dlg.m_strEffectPath;

		if (::FileExists(strPath))
		{
			CObArray* pobArrayBeforeOp = new CObArray;
			RetrieveVideoData(strCourse, pobArrayBeforeOp);

			AddEffect(strCourse, lEffectTime, lDuration, 4, strPath, TRUE);
			// wang20080524 SetVideoEffect(strCourse);
            SetVideoEffect(strCourse);
			CObArray* pobArrayAfterOp = new CObArray;
			RetrieveVideoData(strCourse, pobArrayAfterOp);
			CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
			MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);
		}
	}

	return TRUE;
}

BOOL CAdapter::AddEffect(CString strCourse, long lTime, long lDuration, int nType, 
							   CString strSrcPath, BOOL bGenBmp)
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\video";
	int nIndex = ::GetIndex(strPath, "vfx", "effect");
	CString strFile;
	strFile.Format("effect%d.vfx", nIndex + 1);
	CString strDestPath = strPath + "\\" + strFile;
	::SafeCopyFile(strSrcPath, strDestPath, FALSE);

	CString strFileName = ::GetFileName(strSrcPath);
	::WritePrivateProfileString("INFO", "FileName", strFileName, strDestPath);

	if (bGenBmp)
	{
//		strFile.Format("bimtap%d_1.bmp", nIndex + 1);
//		::WritePrivateProfileString("DATA", "PREFETCH1", strFile, strDestPath);
//		CString strBmpPath1 = strPath + "\\" + strFile;
//		strFile.Format("bimtap%d_2.bmp", nIndex + 1);
//		::WritePrivateProfileString("DATA", "PREFETCH2", strFile, strDestPath);
//		CString strBmpPath2 = strPath + "\\" + strFile;
//		if (!GenImageFiles(strCourse, lTime, strBmpPath1, strBmpPath2))
//		{
//			W_SHOW_ERROR(IDS_FAIL_TO_GEN_SIDE_IMAGE, this);
//			return 0L;
//		}
	}

	m_pTimePanel->AddVideoEffect(lTime, lDuration, nType, strDestPath);
	SetVideoEffect(strCourse);

	return 0L;
}

BOOL CAdapter::GenImageFiles(CString strCourse, long lTime, CString strBmpPath1, CString strBmpPath2)
{
	CString strPathPrev, strPathNext;
	long lTimePrev, lTimeNext;
	if (!m_pTimePanel->GetBoundaryFiles(lTime, strPathPrev, lTimePrev, strPathNext, lTimeNext))
		return FALSE;

	if (strPathPrev == "*")
		strPathPrev = MACRO_MP4_FILE(strCourse);  
	if (strPathNext == "*")
		strPathNext = MACRO_MP4_FILE(strCourse);
	if (strPathPrev.CompareNoCase(strPathNext) == 0 && lTimePrev == lTimeNext)
	{
		if (!MACRO_MAINFRM->GetTwoSideImages(strPathPrev, lTimePrev, strBmpPath1, strBmpPath2))
			return FALSE;
	}
	else
	{
		if (!MACRO_MAINFRM->GetPrevImages(strPathPrev, lTimePrev, strBmpPath1))
			return FALSE;
		if (!MACRO_MAINFRM->GetNextImages(strPathNext, lTimeNext, strBmpPath2))
			return FALSE;
	}

	return TRUE;
}

LRESULT CAdapter::OnDeleteEffect(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return 0L;

	Pause();

	CObArray* pobArrayBeforeOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayBeforeOp);

	m_pTimePanel->DeleteVideoEffectByIndex(lParam);
	SetVideoEffect(strCourse);

	CObArray* pobArrayAfterOp = new CObArray;
	RetrieveVideoData(strCourse, pobArrayAfterOp);
	CompareAndProcessVideoData(pobArrayBeforeOp, pobArrayAfterOp);
	MACRO_MAINFRM->m_objUndoRedo.PushToUndoStack(this, "video_op", pobArrayBeforeOp, pobArrayAfterOp);

	return 0L;
}

LRESULT CAdapter::OnSyncWndClosed(WPARAM wParam, LPARAM lParam)
{
	delete m_pPPTSyncWnd;
	m_pPPTSyncWnd = NULL;
	
	return 0L;
}

LRESULT CAdapter::OnSetPPTPoints(WPARAM wParam, LPARAM lParam)
{
	SetPPTPoints();
	return 0L;
}

BOOL CAdapter::SetPPTPoints()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	//add 2013/11/21 BugNo.88 kagata
	char szBuf[256];
	CString strIniPath = MACRO_INI_FILE(strCourse);
	::GetPrivateProfileString("General", "PPTFile", "", szBuf, 255, strIniPath);
	if (szBuf[0] == '\0')
	{
		W_SHOW_WARNING(IDS_NO_PPT_SET, this);
		return FALSE;
	}

	CString strPath = MACRO_COURSE_PATH + strCourse + CString("\\") + szBuf;
	if (!::FileExists(strPath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FILE_NOT_EXIST);
		strMsg.Format(strTmp, strPath);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}
	//No.88

	//CString strIniPath = MACRO_INI_FILE(strCourse);		//del 2013/11/21 BugNo.88 kagata
	int nCourseType = ::GetPrivateProfileInt("General", "CourseType", 0, strIniPath);

	if (nCourseType == 0)
	{
		if (!m_pPPTSyncWnd)
		{
			m_pPPTSyncWnd = new CPPTSyncWnd;
			m_pPPTSyncWnd->m_pMSGWnd = this;
			
			//2006-03-22
			MACRO_VIEW->m_nCourseType = 0;
			
			m_pPPTSyncWnd->m_strPPTPath = MACRO_VIEW->GetPPTFilePath();
			CString strTitle;// = "スライドの再設定";
			strTitle.LoadString(IDS_PPT_SYNC_WINDOW);
			CRect rcPPT;
			m_pPPTToolbar->GetWindowRect(rcPPT);
			if (!m_pPPTSyncWnd->CreateEx(0, m_strClassName, strTitle, WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU, 
										 rcPPT, this, 0, NULL))
			{
				return FALSE;
			}

			if (m_pPPTSyncWnd->m_strArrayTitle.GetSize() > 0 && m_strArrayMarkTime.GetSize() == 0)
			{
				CString strTitle;
				strTitle.Format("s%03d %s", 1, m_pPPTSyncWnd->m_strArrayTitle[0]);
				CString strStep = "1@0=";
				AddPPTRecord(0, strStep, "flipslide", strTitle);
				return TRUE;
			}
		}
	}

	return FALSE;
}

LRESULT CAdapter::OnAddPPTRecord(WPARAM wParam, LPARAM lParam)
{
	CString strStep = (LPCTSTR)wParam;
	CString strTitle = (LPCTSTR)lParam;
	long lTime = m_pMovieWnd->GetCurrentPos();
	AddPPTRecord(lTime, strStep, "flipslide", strTitle);	
	return 0L;
}

void CAdapter::AddPPTRecord(long lTime, CString strStep, CString strCmd, CString strTitle)	
{	
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CString strTime = ::ConvertTimeToStr(lTime);
	if (!AddControlPoint(strTime, strTitle, strStep, strCmd))
	{
		W_SHOW_WARNING(IDS_CONTROL_POINT_EXISTS, this);
		return;
	}

	WriteTagFile(strCourse);
	SetHyperLinkToGrid();
}

LRESULT CAdapter::OnUndoAction(WPARAM wParam, LPARAM lParam)
{
	CStackObj* pObj = (CStackObj*)wParam;
	if (pObj->m_strOp == "video_op")
	{
		CObArray* pobArrayBefore = pObj->m_pobArrayBeforeOp;
		ReloadAttrib(pobArrayBefore);
	}
	else if (pObj->m_strOp == "hyperlink_upd")
	{
		int nRow = pObj->m_nOp1;
		int nCol = pObj->m_nOp2;
		CString strData = pObj->m_strBeforeOp;
		m_pGrid->SetDataAt(nRow, nCol, strData);
		m_pGrid->RePaintRow(nRow, nRow);
		CString str1, str2;
		UpdateGrid(nRow, nCol, str1, str2);
	}
	else if (pObj->m_strOp == "adjust_op")
	{
		CString strIniPath = MACRO_INI_FILE(MACRO_MAINFRM->GetCourse());
		CObArray* pobArrayBefore = pObj->m_pobArrayBeforeOp;
		CStringArray* pStraBefore = (CStringArray*)pobArrayBefore->GetAt(0);
		int nContrastValue = (atoi(pStraBefore->GetAt(0)) - 100) * 100;
		int nLightValue = (atoi(pStraBefore->GetAt(1)) - 100) * 100;
		int nVolumeValue = (atoi(pStraBefore->GetAt(2)) - 100) * 100;
		m_pMovieWnd->AdjustVideo(1, nContrastValue);
		m_pMovieWnd->AdjustVideo(0, nLightValue);
		m_pMovieWnd->AdjustAudio(0, nVolumeValue);
		CString strTmp;
		strTmp.Format("%d", nContrastValue);
		::WritePrivateProfileString("General", "Contrast", strTmp, strIniPath);
		strTmp.Format("%d", nLightValue);
		::WritePrivateProfileString("General", "Light", strTmp, strIniPath);
		strTmp.Format("%d", nVolumeValue);
		::WritePrivateProfileString("General", "Volume", strTmp, strIniPath);
	}
	else if (pObj->m_strOp == "caption_op")
	{
		CObArray* pobArrayBefore = pObj->m_pobArrayBeforeOp;
		CStringArray* pStraBefore = (CStringArray*)pobArrayBefore->GetAt(0);
		SetCaptionData(pStraBefore->GetAt(0), pStraBefore->GetAt(1), pStraBefore->GetAt(2), pStraBefore->GetAt(3), pStraBefore->GetAt(4), 
						pStraBefore->GetAt(5), pStraBefore->GetAt(6), pStraBefore->GetAt(7), pStraBefore->GetAt(8), 
						pStraBefore->GetAt(9), pStraBefore->GetAt(10), pStraBefore->GetAt(11), pStraBefore->GetAt(12), 0);
		CString strSeg; 
		m_pTimePanel->GetVideoTextSegString(strSeg);
		m_pMovieWnd->SetVideoText(strSeg);
	}

	return 0L;
}

LRESULT CAdapter::OnRedoAction(WPARAM wParam, LPARAM lParam)
{
	CStackObj* pObj = (CStackObj*)wParam;
	if (pObj->m_strOp == "video_op")
	{
		CObArray* pobArrayAfter = pObj->m_pobArrayAfterOp;
		ReloadAttrib(pobArrayAfter);
	}
	else if (pObj->m_strOp == "hyperlink_upd")
	{
		int nRow = pObj->m_nOp1;
		int nCol = pObj->m_nOp2;
		CString strData = pObj->m_strAfterOp;
		m_pGrid->SetDataAt(nRow, nCol, strData);
		m_pGrid->RePaintRow(nRow, nRow);
		CString str1, str2;
		UpdateGrid(nRow, nCol, str1, str2);
	}
	else if (pObj->m_strOp == "adjust_op")
	{
		CString strIniPath = MACRO_INI_FILE(MACRO_MAINFRM->GetCourse());
		CObArray* pobArrayAfter = pObj->m_pobArrayAfterOp;
		CStringArray* pStraAfter = (CStringArray*)pobArrayAfter->GetAt(0);
		int nContrastValue = (atoi(pStraAfter->GetAt(0)) - 100) * 100;
		int nLightValue = (atoi(pStraAfter->GetAt(1)) - 100) * 100;
		int nVolumeValue = (atoi(pStraAfter->GetAt(2)) - 100) * 100;
		m_pMovieWnd->AdjustVideo(1, nContrastValue);
		m_pMovieWnd->AdjustVideo(0, nLightValue);
		m_pMovieWnd->AdjustAudio(0, nVolumeValue);
		CString strTmp;
		strTmp.Format("%d", nContrastValue);
		::WritePrivateProfileString("General", "Contrast", strTmp, strIniPath);
		strTmp.Format("%d", nLightValue);
		::WritePrivateProfileString("General", "Light", strTmp, strIniPath);
		strTmp.Format("%d", nVolumeValue);
		::WritePrivateProfileString("General", "Volume", strTmp, strIniPath);
	}
	else if (pObj->m_strOp == "caption_op")
	{
		CObArray* pobArrayAfter = pObj->m_pobArrayAfterOp;
		CStringArray* pStraAfter = (CStringArray*)pobArrayAfter->GetAt(0);
		SetCaptionData(pStraAfter->GetAt(0), pStraAfter->GetAt(1), pStraAfter->GetAt(2), pStraAfter->GetAt(3), pStraAfter->GetAt(4), 
						pStraAfter->GetAt(5), pStraAfter->GetAt(6), pStraAfter->GetAt(7), pStraAfter->GetAt(8), 
						pStraAfter->GetAt(9), pStraAfter->GetAt(10), pStraAfter->GetAt(11), pStraAfter->GetAt(12), 0);
		CString strSeg; 
		m_pTimePanel->GetVideoTextSegString(strSeg);
		m_pMovieWnd->SetVideoText(strSeg);
	}

	return 0L;
}

void CAdapter::ReloadAttrib(CObArray* pObArray)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CString strSeg;
	CStringArray* pstrArray = (CStringArray*)pObArray->GetAt(0);
	if (pstrArray)
	{
		m_pTimePanel->SetVideoSegArray(*pstrArray);

		CString strSeg;
		CString strWMVFile = MACRO_MP4_FILE(strCourse);
		CString strSubPath = MACRO_COURSE_PATH + strCourse + "\\video\\";
		m_pTimePanel->GetVideoSegString(strSeg, strWMVFile, TRUE, strSubPath);
		m_pMovieWnd->SetTimeLine(strSeg);
		m_pMovieWnd->SetFilePosition(0);
		long lDuration = m_pMovieWnd->GetLength();
		m_pTimePanel->SetLength(lDuration);

		m_pTimePanel->GetVideoSegString(strSeg, strWMVFile, FALSE, "");
		WriteRecordFile(MACRO_RECORD_VIDEO(strCourse), strSeg);
	}

	pstrArray = (CStringArray*)pObArray->GetAt(1);
	if (pstrArray)
	{
		LoadTagFromStringArray(*pstrArray);
		m_pTimePanel->SetKeyFrame(m_strArrayMarkName, m_strArrayMarkTime, 
								  m_strArrayScriptName, m_strArrayScriptCmd, m_strArrayScriptTime);
		CString strPath = MACRO_RECORD_TAG(strCourse);
		::SaveFile(strPath, *pstrArray, TRUE);
		LoadHyperLink(strCourse);
	}

	pstrArray = (CStringArray*)pObArray->GetAt(2);
	if (pstrArray)
	{
		m_pTimePanel->SetAudioReplaceSegArray(*pstrArray);
		SetAudioReplace(strCourse);
	}

	pstrArray = (CStringArray*)pObArray->GetAt(3);
	if (pstrArray)
	{
		m_pTimePanel->SetAudioMixSegArray(*pstrArray);
		SetAudioMix(strCourse);
	}
	
	pstrArray = (CStringArray*)pObArray->GetAt(4);
	if (pstrArray)
	{
		LoadVideoTextFromArray(*pstrArray,strCourse);
		SetVideoText(strCourse);
	}

	pstrArray = (CStringArray*)pObArray->GetAt(5);
	if (pstrArray)
	{
		m_pTimePanel->SetVideoEffectSegArray(*pstrArray);
		SetVideoEffect(strCourse);
	}

	m_pTimePanel->SetBreakPoint();
	m_pTimePanel->RedrawPanel();
}

void CAdapter::RetrieveVideoData(CString strCourse, CObArray* pobArray)	
{
	CString strTmp;
	CStringArray* pstrArray = new CStringArray;
	CString strSubPath = MACRO_COURSE_PATH + strCourse + "\\video\\";
	m_pTimePanel->GetVideoSegString(strTmp, MACRO_MP4_FILE(strCourse), FALSE, strSubPath);
	ParseStringToArray(strTmp, *pstrArray);
	pobArray->Add(pstrArray);

	pstrArray = new CStringArray;
	WriteTagToArray(strCourse, *pstrArray);
	pobArray->Add(pstrArray);

	pstrArray = new CStringArray;
	m_pTimePanel->GetAudioReplaceSegString(strTmp);
	ParseStringToArray(strTmp, *pstrArray);
	pobArray->Add(pstrArray);

	pstrArray = new CStringArray;
	m_pTimePanel->GetAudioMixSegString(strTmp);
	ParseStringToArray(strTmp, *pstrArray);
	pobArray->Add(pstrArray);

	pstrArray = new CStringArray;
	m_pTimePanel->GetVideoTextSegString(strTmp);
	ParseStringToArray(strTmp, *pstrArray);
	pobArray->Add(pstrArray);

	pstrArray = new CStringArray;
	m_pTimePanel->GetVideoEffectSegString(strTmp);
	ParseStringToArray(strTmp, *pstrArray);
	pobArray->Add(pstrArray);
}

void CAdapter::CompareAndProcessVideoData(CObArray* pobArray1, CObArray* pobArray2)	
{
	if (pobArray1->GetSize() != pobArray2->GetSize())
		return;

	int nSize = pobArray1->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CStringArray* pstrArray1 = (CStringArray*)pobArray1->GetAt(i);
		CStringArray* pstrArray2 = (CStringArray*)pobArray2->GetAt(i);
		if (pstrArray1 == NULL || pstrArray2 == NULL)
			continue;
		if (pstrArray1->GetSize() != pstrArray2->GetSize())
			continue;
		
		BOOL bSame = TRUE;
		for (int j = 0; j < pstrArray1->GetSize(); j++)
		{
			if (pstrArray1->GetAt(j) != pstrArray2->GetAt(j))
			{
				bSame = FALSE;
				break;
			}

		}
		if (bSame)
		{
			delete pstrArray1;
			delete pstrArray2;
			pobArray1->SetAt(i, NULL);
			pobArray2->SetAt(i, NULL);
		}
	}
}

void CAdapter::Resize()
{
	// Find parent
	CWnd* pParent;
	CRect rectParent;

	pParent = GetParent();
	if (pParent == NULL)
	{
		AfxMessageBox("Cannot resize property sheet. Sheet has no parent", MB_ICONEXCLAMATION | MB_OK | MB_APPLMODAL);
		return;
	}

	// Get parents client area
	pParent->GetClientRect(&rectParent);

	// Resize the sheet
	// First find relative change
	CSize sizeRelChange;
	CRect rectWindow;

	GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	sizeRelChange.cx = rectWindow.Width() - rectParent.Width();
	sizeRelChange.cy = rectWindow.Height() - rectParent.Height();

	rectWindow.right -= sizeRelChange.cx;
	rectWindow.bottom -= sizeRelChange.cy;
	// Then resize the sheet
	MoveWindow(&rectWindow);

	/*
	// Resize the CTabCtrl
	CTabCtrl* pTab = GetTabControl();
	ASSERT(pTab);
	pTab->GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	rectWindow.right -= sizeRelChange.cx;
	rectWindow.bottom -= sizeRelChange.cy;
	pTab->MoveWindow(&rectWindow);
	
	// Resize the active page
	CPropertyPage* pPage = GetActivePage();
	ASSERT(pPage);
	// Store page size in m_rectPage
	pPage->GetWindowRect(&m_rectPage);
	ScreenToClient(&m_rectPage);
	m_rectPage.right -= sizeRelChange.cx;
	m_rectPage.bottom -= sizeRelChange.cy;
	pPage->MoveWindow(&m_rectPage);
	*/
}
//add by buxiangyi
void CAdapter::OnCComboBo()
{
	CString m_size;
	m_cbExamble ->GetWindowText(m_size);
	CString m_vsizex ,m_vsizey;
	int xindex, yindex; 
	xindex = m_size.Find("x");
	m_vsizex = m_size.Left(xindex);
	yindex = m_size.Find(" ");
	m_vsizey = m_size.Mid(xindex +1,yindex - xindex -1);

	CString m_name, strPath;
	CStringArray strArray, strOldArray;
	m_name = MACRO_MAINFRM->GetCourse();
	if (m_name == "")
	{
		strPath = MACRO_TEMPLATE_PATH +"js\\";
		strOldArray.Add("var vsizex = ");
		strOldArray.Add("var vsizey = ");
		CString str;
		str = "var vsizex = " + m_vsizex + ";";
		strArray.Add(str);
		str = "var vsizey = " + m_vsizey + ";";
		strArray.Add(str);
		UpdateFileByTag(strPath, "config.js", strOldArray, strArray, TRUE);
	}
	else
	{

		strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
		strOldArray.Add("var vsizex = ");
		strOldArray.Add("var vsizey = ");
		CString str;
		str = "var vsizex = " + m_vsizex + ";";
		strArray.Add(str);
		str = "var vsizey = " + m_vsizey + ";";
		strArray.Add(str);
		UpdateFileByTag(strPath, "config.js", strOldArray, strArray, TRUE);
	}

}

BOOL CAdapter::SaveConfig(CString strCourse)
{
	if (strCourse == "")
		return TRUE;

	CString strHtmlPath = MACRO_HTML_DIR(strCourse);
	CString strConfigFile = "config.js";
	CString strConfigPath = strHtmlPath + strConfigFile;
	if (!::FileExists(strConfigPath))
		return FALSE;

	this->m_strArrayMarkName;
	this->m_strArrayMarkTime;
	this->m_strArrayScriptCmd;
	this->m_strArrayScriptName;
	this->m_strArrayScriptTime;
	this->m_strArrayLayoutName;
	int nSize = m_strArrayScriptCmd.GetSize();
	CStringArray strsTag;
	strsTag.Add("var gScriptTable = new Array(\n");
	CString strTime;
	int iMarkIndex = -1;
	BOOL bMove;
	double dTime;
	CString strMark;
	CString strCmd;
	CString strName;
	BOOL bShow;
	CString strLastMark;
	int iLen;
	int iFind;
	int iIndex;
	int iLayoutIndex = -1;
	CString strLayout;
	int iAnimaIndex;
	for(int iCount = 0; iCount < nSize;)
	{
		strTime = m_strArrayScriptTime[iCount];
		if (iMarkIndex + 1 < m_strArrayMarkTime.GetSize())
		{
			if (m_strArrayMarkTime[iMarkIndex + 1].Compare(strTime) == 0)
			{
				iMarkIndex++;
				bMove = TRUE;
			}
		}
		dTime = (double)::ConvertStrToTime(strTime) / 1000;
		strMark = m_strArrayMarkName[iMarkIndex];
		strCmd = m_strArrayScriptCmd[iCount];
		strName = m_strArrayScriptName[iCount];
		bShow = TRUE;
		if (::IsCtrlMark(strMark) && bMove == TRUE)
		{
			bMove = FALSE;
			bShow = FALSE;
			if (strCmd.Compare("demo") == 0)
			{
				strMark = strCmd + strName;
			}
			else if (strCmd.Compare("zoomin") == 0 || strCmd.Compare("zoomout") == 0)
			{
				strMark = strLastMark;
				iLen = strMark.GetLength();
				iFind = strMark.Find(' ');
				strMark = strMark.Right(iLen - iFind - 1);
			}
		}
		else
		{
			if (bMove == FALSE)
			{
				strMark = strLastMark;
			}
			else
			{
				strLastMark = strMark;
			}
			iLen = strMark.GetLength();
			iFind = strMark.Find(' ');
			iIndex = atoi(strMark.Left(iFind).Right(iFind - 1));
			strMark = strMark.Right(iLen - iFind - 1);
		}
		if (strCmd.Compare("flipslide") == 0)
		{
			if (strName.Find('.') > 0)
			{
				bShow = FALSE;
			}
			iLen = strName.GetLength();
			iFind = strName.Find('@');
			CString strAnima = strName.Right(iLen - iFind - 1);
			char* strBuf = strAnima.GetBuffer();
			iLen = 0;
			while(1)
			{
				if (strBuf[iLen] >= 48 && strBuf[iLen] <= 57)
					iLen++;
				else
					break;
			}
			iAnimaIndex = atoi(strAnima.Left(iLen));
			if (bShow)
			{
				iLayoutIndex++;
				if (iLayoutIndex < m_strArrayLayoutName.GetSize())
					strLayout = m_strArrayLayoutName[iLayoutIndex];
				else
					strLayout = MACRO_VIEW->GetDefaultLayout();
			}
		}
		iCount++;
		CString str;
		if (strCmd.Compare("zoomin") == 0)
			str.Format("new hScriptList(%d, %.1f, %d, %d, \"%s\", \"%s\", \"999\", %d)", iCount, dTime, iIndex, iAnimaIndex, strCmd, strMark, bShow);
		else
			str.Format("new hScriptList(%d, %.1f, %d, %d, \"%s\", \"%s\", \"%s\", %d)", iCount, dTime, iIndex, iAnimaIndex, strCmd, strMark, strLayout, bShow);
		if (iCount != nSize)
		{
			str += ",";
		}
		str += "\n";
		::ConvertCodePage(str, CP_ACP, CP_UTF8);
		strsTag.Add(str);
	}
	strsTag.Add(");\n");

	return UpdateFileByStartEnd(strHtmlPath, strConfigFile, "//@@@StartPos", "//@@@EndPos\\", strsTag);
}
int CAdapter::GetArrayLayoutNameSize()
{
	if (m_strArrayMarkName.GetSize() == 0)
	{
		return 0;
	}
	int nIndex = 0;

	for (int i = 0; i < m_strArrayMarkName.GetSize(); i++)
	{
		CString strMark = m_strArrayMarkName[i];
		if (!::IsCtrlMark(strMark))
		{
			nIndex++;
		}
	}


	return nIndex;
}
//add by buxiangyi
CString CAdapter::GetDefaultLatout()
{
	CString strTmp, CourseName;
	CourseName =  MACRO_MAINFRM->GetCourse();
	strTmp = MACRO_JS_PATH(CourseName);
	CString TemstrPath, m_LayoutName;
	TemstrPath = strTmp + "config.js";
	CStringArray strArray;
	::LoadFile(TemstrPath, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strItem = strArray[i];
		strItem.TrimLeft();
		if (strItem.Find("var layoutNo") >= 0)
		{
			int BeginIndex = strItem.Find("\"");
			int EndIndex = strItem.Find(";");
			m_LayoutName = strItem.Mid(BeginIndex +1, EndIndex - BeginIndex - 2);
		}
	}
	return m_LayoutName;
}

CStringArray* CAdapter::GetDrawTime()
{
	return &m_strArrayDrawTime;
}

CStringArray* CAdapter::GetDrawColor()
{
	return &m_strArrayDrawColor;
}

CStringArray* CAdapter::GetDrawWidth()
{
	return &m_strArrayDrawWidth;
}

CStringArray* CAdapter::GetDrawCmd()
{
	return &m_strArrayDrawCmd;
}

CStringArray* CAdapter::GetDrawLeft()
{
	return &m_strArrayDrawLeft;
}

CStringArray* CAdapter::GetDrawTop()
{
	return &m_strArrayDrawTop;
}

CStringArray* CAdapter::GetDrawRight()
{
	return &m_strArrayDrawRight;
}

CStringArray* CAdapter::GetDrawBottom()
{
	return &m_strArrayDrawBottom;
}

void CAdapter::DeleteDrawPoint(int nIndex)
{
	m_strArrayDrawIndex.RemoveAt(nIndex);
	m_strArrayDrawTime.RemoveAt(nIndex);
	m_strArrayDrawColor.RemoveAt(nIndex);
	m_strArrayDrawWidth.RemoveAt(nIndex);
	m_strArrayDrawCmd.RemoveAt(nIndex);
	m_strArrayDrawLeft.RemoveAt(nIndex);
	m_strArrayDrawTop.RemoveAt(nIndex);
	m_strArrayDrawRight.RemoveAt(nIndex);
	m_strArrayDrawBottom.RemoveAt(nIndex);
}

void CAdapter::SaveDrawArray(CString strCourse)
{
	CString strPath = MACRO_DRAW_FILE(strCourse);
	if (::FileExists(strPath))
	{
		CStringArray strArrayTag;
		::LoadFile(strPath, strArrayTag, FALSE);

		CString strLine, strTok;
		int nIndex = 0;
		int nCount;
		for (int i = 0; i < strArrayTag.GetSize(); i++)
		{
			strLine = strArrayTag[i];
			if (strLine.Find("draw[") < 0)
			{
				continue;
			}
			strTok = strLine.Right(strLine.GetLength() - strLine.Find("draw[") - 5);
			strTok = strTok.Left(strTok.Find("]"));
			if (m_strArrayDrawIndex.GetSize() == 0 || m_strArrayDrawIndex.GetAt(nIndex) != strTok)
			{
				strArrayTag.RemoveAt(i);
				i--;
				continue;
			}
			if (m_strArrayDrawCmd.GetAt(nIndex) == "rect" || m_strArrayDrawCmd.GetAt(nIndex) == "line" || m_strArrayDrawCmd.GetAt(nIndex) == "mark")
			{
				CString strNewLine;
				strNewLine.Format("draw[%d] = \"%d,%9.2f,%s,%s,%s,%s,%s,%s,%s\";", nIndex, nIndex, atof(m_strArrayDrawTime.GetAt(nIndex)) / 1000, m_strArrayDrawColor.GetAt(nIndex), m_strArrayDrawWidth.GetAt(nIndex), m_strArrayDrawCmd.GetAt(nIndex), m_strArrayDrawLeft.GetAt(nIndex), m_strArrayDrawTop.GetAt(nIndex), m_strArrayDrawRight.GetAt(nIndex), m_strArrayDrawBottom.GetAt(nIndex));
				strArrayTag.RemoveAt(i);
				strArrayTag.InsertAt(i, strNewLine);
			}
			else if (m_strArrayDrawCmd.GetAt(nIndex) == "freehand" || m_strArrayDrawCmd.GetAt(nIndex) == "eraser")
			{
				CString strNewLine;
				strNewLine.Format("draw[%d] = \"%d,%9.2f,%s,%s,%s", nIndex, nIndex, atof(m_strArrayDrawTime.GetAt(nIndex)) / 1000, m_strArrayDrawColor.GetAt(nIndex), m_strArrayDrawWidth.GetAt(nIndex), strLine.Right(strLine.GetLength() - strLine.Find( m_strArrayDrawCmd.GetAt(nIndex))));
				strArrayTag.RemoveAt(i);
				strArrayTag.InsertAt(i, strNewLine);
			}
			nIndex++;
		}
		::SaveFile(strPath, strArrayTag, FALSE);
		CString strJSPath = MACRO_JS_PATH(strCourse) + "draw.js";
		::SafeCopyFile(strPath, strJSPath);
	}
}

void CAdapter::MergeVideo(CString strSeg, CString strCourse)
{
	m_pMovieWnd->SetTimeLine(strSeg); 
	m_pMovieWnd->SaveVideo(MACRO_MP4_FILE(strCourse), MACRO_ORG_FILE(strCourse), 0);
}

BOOL CAdapter::TestVideoFile(CString strVideoPath)
{
	return m_pMovieWnd->TestVideoFile(strVideoPath);
}

void CAdapter::setviewer()
{
	m_pPPTToolbar->setviewer(m_strCourse);
	return;
}
