// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Elearning.h"

#include "MainFrm.h"
#include "eLearningView.h"
#include "cmkapi.h"
#include "macro.h"
#include "FileApi.h"
#include "ProgressWnd.h"
#include "MediaUtils.h"
#include "DlgInputCourse.h"
#include "Ppt9Ctrl.h"
#include "Ppt10Ctrl.h"
#include "Ppt15Ctrl.h"		// add itoh 2013/07/11 PPT2013
#include "Registry.h"
#include "..\ComCapture\ComCapture\UserErrMsg.h"
#include "WndTimeRecord.h"
#include "eLearningAPI.h"
#include "TimeLinePanel.h"
#include "..\_Shared\HelperKit.h"
#include <direct.h>
#include <dshow.h>
#include <io.h>
#include <stdio.h>
#include <atlbase.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <Propvarutil.h>

//#include "PageVideoEdit.h"	//add 2014/7/28 kagata chiba
#include "tlhelp32.h"	//add 2014/7/31 kagata outemon
#include "adapter.h"	//add 2014/7/28 kagata chiba

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\MediaFile\MediaFile_i.c"
#include "..\MediaKit\MediaKit_i.c"
#include "..\TimeLinePlayer\VideoEdit\VideoEdit_i.c"
#include "..\VideoConvert\VideoConvert\VideoConvert_i.c"
#define ValueStartWizardTimerID 4567
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_NEW_COURSE, OnNewCourse)
	ON_MESSAGE(WM_LOAD_COURSE, OnLoadCourse)
	ON_MESSAGE(WM_GETUP, OnGetUp)
	ON_MESSAGE(WM_FILE_RECORDED, OnFileRecorded)
	ON_MESSAGE(WM_USER_PROGRESS_REPORT, OnProgress)	
	ON_MESSAGE(WM_TIME_LEFT, OnTimeLeft)
	ON_MESSAGE(WM_TIME_OVER, OnTimeOver)
	ON_COMMAND(ID_END_TRACE, OnEndTrace)
	ON_COMMAND(ID_START_TRACE, OnStartTrace)
	ON_MESSAGE(WM_WAIT_FOR_FILE, OnWaitForFile)
	ON_WM_SYSCOMMAND() 
	ON_WM_TIMER()
	//add by buxiangyi
	ON_COMMAND(ID_EXIT,OnExit)
	////////////////////////////////
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 1,OnNewAddress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT indicatorsEv[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bNoLoadingVideo = FALSE;
	m_bCannotRun = FALSE;
	m_lTimeStart = -1;
	m_lTimeEnd = -1;
	m_strWaitFile = "";
	m_strCourse = "";

	m_pMediaConvert = NULL;
    m_ChangeCourse=false;
	m_bAskJmp = FALSE;
}

CMainFrame::~CMainFrame()
{
	m_dlgProgress.DestroyWindow();

	if (m_pMediaConvert)
	{
		m_pMediaConvert->Release();
		m_pMediaConvert = NULL;
	}


}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString str;

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	////////////////////////////////////////////


	BOOL bNameValid;
	// set the visual manager and style based on persisted value

	



	//////////////////////////////////////////////

	if (!m_wndReBar.Create(this))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(30, 60);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	// Set up hot bar image lists.
	CImageList	imageList;
	CBitmap		bitmap;

/* ｭ・dｨﾒﾃCｦ箙|ﾅﾜ
//	img.Create(IDB_HOTTOOLBAR, 22, 0, RGB(255, 0, 255));
//	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
//	img.Detach();

//	img.Create(IDB_COLDTOOLBAR, 22, 0, RGB(255, 0, 255));
//	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
//	img.Detach();
*/

	// Create and set the normal toolbar image list.
	bitmap.LoadBitmap(IDB_TOOLBAR_COLD);
	imageList.Create(22, 22, ILC_COLORDDB|ILC_MASK, 10, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();

	// Create and set the hot toolbar image list.
	bitmap.LoadBitmap(IDB_TOOLBAR_HOT);
	imageList.Create(22, 22, ILC_COLORDDB|ILC_MASK, 10, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(NULL, 15);

	// set up each toolbar button
	m_wndToolBar.SetButtonInfo(0, ID_NEW_WIZARD, TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_NEW_LESSON, TBSTYLE_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_RECORD, TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(3, ID_SAVE_VIDEO, TBSTYLE_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(4, 0, TBBS_SEPARATOR, 0);
	m_wndToolBar.SetButtonInfo(5, ID_DEL_LESSON, TBSTYLE_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(6, 0, TBBS_SEPARATOR, 0);
	m_wndToolBar.SetButtonInfo(7, ID_UNDO, TBSTYLE_BUTTON, 5);
	m_wndToolBar.SetButtonInfo(8, ID_REDO, TBSTYLE_BUTTON, 6);
	m_wndToolBar.SetButtonInfo(9, 0, TBBS_SEPARATOR, 0);
	m_wndToolBar.SetButtonInfo(10, ID_STOP, TBSTYLE_BUTTON, 7);
	m_wndToolBar.SetButtonInfo(11, ID_REFRESH, TBSTYLE_BUTTON, 8);
	m_wndToolBar.SetButtonInfo(12, 0, TBBS_SEPARATOR, 0);
	m_wndToolBar.SetButtonInfo(13, ID_IE, TBSTYLE_BUTTON, 9);
	m_wndToolBar.SetButtonInfo(14, ID_COURSE_LOADER, TBSTYLE_BUTTON, 10);

	CRect rectToolBar;

	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(22,20));

	// create a combo box for the address bar
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	if (!m_wndAddress.Create(CBS_DROPDOWNLIST | WS_CHILD, CRect(0, 0, 200, nScreenY / 2), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}

	// add the toolbar, animation, and address bar to the rebar
	m_wndReBar.AddBar(&m_wndToolBar);
	str.LoadString(IDS_COURSE_NAME);
	m_wndReBar.AddBar(&m_wndAddress, str + ":", NULL, RBBS_FIXEDBMP);

	// set up min/max sizes and ideal sizes for pieces of the rebar
	REBARBANDINFO rbbi;

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 13;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	rbbi.cxMinChild = 0;

	CRect rectAddress;

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	m_wndAddress.GetComboBoxCtrl()->GetWindowRect(&rectAddress);

	rbbi.cxMinChild = 60;
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cxIdeal = 60;
	m_wndReBar.GetReBarCtrl().SetBandInfo(2, &rbbi);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	if (TRY_STATE)
	{
		if (!m_wndStatusBar.Create(this) ||
			!m_wndStatusBar.SetIndicators(indicatorsEv,
			  sizeof(indicatorsEv)/sizeof(UINT)))
		{
			TRACE0("Failed to create status bar\n");
			return -1;      // fail to create
		}
		m_wndStatusBar.SetPaneInfo(1, 0, m_wndStatusBar.GetPaneStyle(1), 96);
	}
	else
	{
		if (!m_wndStatusBar.Create(this) ||
			!m_wndStatusBar.SetIndicators(indicators,
			  sizeof(indicators)/sizeof(UINT)))
		{
			TRACE0("Failed to create status bar\n");
			return -1;      // fail to create
		}
	}

	m_dlgProgress.m_strAVIPath = ::GetSysPath() + "horse.avi";
	m_dlgProgress.Create(IDD_DIALOG_PROGRESS, this);

	CollectCourse();

	if (CoCreateInstance(CLSID_EasyVideoEdit, NULL, CLSCTX_INPROC_SERVER,
						  IID_IEasyVideoEdit, (void**)&m_pEasyVideoEdit) != S_OK)
	{
		W_SHOW_ERROR(IDS_ERROR_COM_EASY_PLAY, this);
		m_pVideoConvert = NULL;
	}
	if (NULL != m_pEasyVideoEdit)
	{
		m_pEasyVideoEdit->Open();
	}
	if (CoCreateInstance(CLSID_EasyVideoConvert, NULL, CLSCTX_INPROC_SERVER, 
						 IID_IEasyVideoConvert, (void**)&m_pVideoConvert) != S_OK)
	{
		W_SHOW_ERROR(IDS_ERROR_COM_EASY_PLAY, this);
		m_pVideoConvert = NULL;
	}
	if (NULL != m_pVideoConvert)
	{
		//m_pVideoConvert->SetupProgressHandler((long)(AfxGetMainWnd()->m_hWnd), (long)(WM_APP + 456));
	} 
	//Set_Timer(); wangwang20071227

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON1);

	CString deviceSetInfoFileDir = ::GetSysPath();
	deviceSetInfoFileDir = deviceSetInfoFileDir + "\\temp\\DeviceSetInfo.ini";
	if (FileExists(deviceSetInfoFileDir) != 1)
	{
		MakeDeviceSetInfoFile(deviceSetInfoFileDir);
	}

	m_nAuFixValueC = 0;
	m_nAuFixValueS = 0;

	return 0;
}

void CMainFrame::CollectCourse(CString strOn)
{
	CString strCourse = GetCourse();

	CStringArray strArray;
	::GetDir(MACRO_COURSE_PATH, strArray);
	::SortArray(strArray);
	
	if (strOn == "?")
		strOn = strCourse;

	int nIndex = SetComboContent(&strArray, strOn);
	m_wndAddress.SetCurSel(nIndex);
}

void CMainFrame::SetComboSel(CString strSel)
{
	int nIndex = -1;
	for (int i = 0; i < m_wndAddress.GetCount(); i++) 
	{
		char szStr[256];
		m_wndAddress.GetLBText(i, szStr);
		if (strSel == szStr)
		{
			nIndex = i;
			break;
		}
	}
	m_wndAddress.SetCurSel(nIndex);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ((CELearningApp*)AfxGetApp())->GetClassName();

	return CFrameWndEx::PreCreateWindow(cs);
}

int CMainFrame::SetComboContent(CStringArray* pArray, CString str)
{
	m_wndAddress.ResetContent();

	char szTmp[1];
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;
	item.iItem = -1;
	szTmp[0] = '\0';
	item.pszText = szTmp;
	m_wndAddress.InsertItem(&item);

	int nIndex = -1;
	int iCount = 0;		//add itoh 2013/10/08 PPT2013
	for (int i = 0; i < pArray->GetSize(); i++)
	{
		if (::FileExists(MACRO_COURSE_PATH + "\\" + pArray->GetAt(i) + "\\html\\config.js")) { // add itoh 2013/10/04 PPT2013 version check
			item.pszText = (LPTSTR)(LPCTSTR)pArray->GetAt(i);
			m_wndAddress.InsertItem(&item);
			iCount++;		//add itoh 2013/10/08 PPT2013
			if (str == item.pszText)
//				nIndex = i + 1;
				nIndex = iCount;
		}
	}

	return nIndex;
}

CString CMainFrame::GetCourse()
{
	int nIndex = m_wndAddress.GetCurSel();
	if (nIndex < 0)
		return "";

	char szStr[256];
	m_wndAddress.GetLBText(nIndex, szStr);
	return szStr;
}


CString CMainFrame::GetHtml()
{
	CString str = GetCourse();
	if (str == "")
		return "";

	CString strFileName = MACRO_COURSE_PATH + str + "\\default.htm";
	if (::FileExists(strFileName))
		return strFileName;
	return "";
}

BOOL CMainFrame::IsFirstLoading(CString str)
{
	CString strPath = MACRO_COURSE_PATH + str;

	CString strStepPath = MACRO_STEP_FILE(str);
	if (!::FileExists(strStepPath))
		return FALSE;

	CString strORGFile = MACRO_ORG_FILE(str);
	CString strWMVFile = MACRO_MP4_FILE(str);
	if (!::FileExists(strORGFile))
	{
		if (::FileExists(strWMVFile))
			return !IsASFTagged(strWMVFile);

		return FALSE;
	}
	
	return TRUE;
}

BOOL CMainFrame::MergeWMVFile(CString strCourse)
{
	CString strSeg;
	long lLength = 0;
	CString strCoursePath = MACRO_COURSE_PATH + strCourse + "\\";

	CString strPath = MACRO_ORG_FILE(strCourse);
	if (!::FileExists(strPath))
		return FALSE;
	CString strIndex1Path = strCoursePath + "video\\index1.mp4";
	if (!::FileExists(strIndex1Path))
		return TRUE;

	long lDuration = GetMediaLength(strPath);
	strSeg.AppendFormat("%d,%s,%d,%d;", lLength, strPath, 0, lDuration);
	lLength += lDuration;
	CStringArray strArrayDelFile;
	strArrayDelFile.Add(strPath);

	CString strNewPath = strCoursePath + "video\\_temp.mp4";
	CString strAppendFiles = strPath + ";";
	int i = 0;
	for (i = 1;;i++)
	{
		CString strAsfFile;
		strAsfFile.Format("index%d.mp4", i);
		CString strAsfPath = strCoursePath + "video\\" + strAsfFile;
		if (!::FileExists(strAsfPath))
		{
			break;
		}

		strAppendFiles += strAsfPath + ";";
		strArrayDelFile.Add(strAsfPath);
		lDuration = GetMediaLength(strAsfPath);
		strSeg.AppendFormat("%d,%s,%d,%d;", lLength, strAsfPath, 0, lDuration);
		lLength += lDuration;
	}

	//if (lLength != lDuration)
	//	MACRO_VIEW->MergeVideo(strSeg, strCourse);

	CComBSTR btrAppendFiles(strAppendFiles);
	CComBSTR btrNewFile(strNewPath);

	if (strAppendFiles != strPath + ";")
		HRESULT hr = m_pEasyVideoEdit->Join(btrAppendFiles, btrNewFile);
	CString strIniPath = MACRO_INI_PATH;
	int nDel = ::GetPrivateProfileInt("General", "DeleteMedia", 1, strIniPath);
	if (nDel)
	{
		for (i = 0; i < strArrayDelFile.GetSize(); i++)
			::DeleteFile(strArrayDelFile[i]);
	}
	else
	{
		rename(strPath, strCoursePath + "video\\index0.mp4");
	}

	if (::FileExists(strPath))
		return FALSE;

	rename(strNewPath, strPath);

	return TRUE;
}

BOOL CMainFrame::ProcessAllScriptTime(CString strCourse)
{
	CString strCoursePath = MACRO_COURSE_PATH + strCourse + "\\";
	if (!ProcessScriptTime(strCourse))
		return FALSE;

	return TRUE;
}

BOOL CMainFrame::ProcessScriptTime(CString strCourse)
{
	CString strScriptPath = MACRO_SPT_FILE(strCourse);
	CString strStepPath = MACRO_STEP_FILE(strCourse);

	if (!::FileExists(strScriptPath))// || !::FileExists(strStepPath))
	{
		CFile cf;
			if (!cf.Open(strScriptPath, CFile::modeCreate | CFile::modeWrite))
			{
			SHOW_ERROR(IDS_FAIL_TO_GET_SCRIPT);
			return FALSE;
		}
	}

	if (!::FileExists(strStepPath))
	{
		SHOW_ERROR(IDS_FAIL_TO_GET_SCRIPT);
		return FALSE;
	}

	CStringArray strArrayScript, strArrayStep;//, strArrayZoom, strArrayDemo;
	//CWordArray wArrayIndex;
	CStringArray strArrayIndex;

	::LoadFile(strScriptPath, strArrayScript, TRUE);
	::LoadFile(strStepPath, strArrayStep, TRUE);
	int i = 0;
	for (i = 0; i < strArrayScript.GetSize(); i++)
	{
		CString strScript = strArrayScript[i];
		if (strScript.GetLength() > 0)
		{
			if (strScript[0] == 'o')
			{
				strArrayScript.RemoveAt(i);
				i--;
			}
		}
	}

	for (i = 0; i < strArrayScript.GetSize(); i++)
	{
		CString strScript = strArrayScript[i];
		int nSpace = strScript.Find(" ");
		if (nSpace >= 0)
		{
//			wArrayIndex.Add(atoi(strScript.Left(nSpace)));
			strArrayIndex.Add(strScript.Left(nSpace));
			strScript = strScript.Mid(nSpace + 1);
			strScript.TrimRight();
			long lTime = atol(strScript);

			CString strTime = ::ConvertTimeToStr(lTime);
			strArrayScript[i] = strTime;
		}
	}
	strArrayScript.InsertAt(0, "00:00:00.0");
	strArrayIndex.InsertAt(0, "0");

	ConverTimeByIndex(strArrayStep, strArrayIndex, strArrayScript);

	::SaveFile(strStepPath, strArrayStep, TRUE);

	::DeleteFile(strScriptPath);
	return TRUE;
}

void CMainFrame::ConverTimeByIndex(CStringArray& strArray, 
								   CStringArray& strArrayIndex,
								   CStringArray& strArrayScript)
{
	CString strTime = "";
	for (int i = 1; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		int nSpace = str.Find(" ");
		if (nSpace >= 0)
		{
			CString strIndex = str.Left(nSpace);
			str = str.Mid(nSpace + 1);
			BOOL bFound = FALSE;
			int nIndex;
			int nSp = strIndex.Find(",");
			if (nSp >= 0)
            {
				for (int j = 0; j < strArrayIndex.GetSize(); j++)
				{
					CString strIndex1 = strIndex.Left(nSp);
					if (strIndex1 == strArrayIndex[j])
					{
						nIndex = j;
						bFound = TRUE;
						break;
					}
				}
				
				if (bFound)
				{
					strTime = strArrayScript[nIndex];
					str = strTime + " " + str;
					strArray[i] = str;
				}
				else
				{
					for (int j = 0; j < strArrayIndex.GetSize(); j++)
					{
						CString strIndex2 = strIndex.Mid(nSp + 1);
						if (strIndex2 == strArrayIndex[j])
						{
							nIndex = j;
							bFound = TRUE;
							break;
						}
					}
					
					if (bFound)
					{
						strTime = strArrayScript[nIndex];
						str = strTime + " " + str;
						strArray[i] = str;
					}
					else
					{
						strArray.RemoveAt(i);
						i--;
						continue;
					}
				}
			}
			else
			{
				for (int j = 0; j < strArrayIndex.GetSize(); j++)
				{
					if (strIndex == strArrayIndex[j])
					{
						nIndex = j;
						bFound = TRUE;
						break;
					}
				}
				
				if (bFound)
				{
					strTime = strArrayScript[nIndex];
				}
				else
				{
					//long lTime = ::ConvertStrToTime(strTime) + 5000;
					//strTime = ::ConvertTimeToStr(lTime);
					strArray.RemoveAt(i);
					i--;
					continue;
				}
				str = strTime + " " + str;
				strArray[i] = str;
			}
		}
	}
}

BOOL CMainFrame::GenProfile(CString strSrcASF, CString strCourse)
{
	CComBSTR bstrAsf(strSrcASF);
	CString strProfilePath = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
	CComBSTR bstrProfile(strProfilePath);

	CString strTmp, strMsg;
	// deleted by buxiangyi 一時削除
	/*HRESULT hval = m_pWMXUtility->PRXFromWMX(bstrAsf, OATRUE, 20, OAFALSE, bstrProfile);
	if (FAILED(hval))
	{
	strTmp.LoadString(IDS_FAIL_TO_GET_PROFILE);
	strMsg.Format(strTmp, strSrcASF);
	W_SHOW_ERROR_STR(strMsg, this);
	return FALSE;
	}*/

	return TRUE;
}

BOOL CMainFrame::ConvertFromASF(CString strCourse, CString strSrcASF)
{
	CComBSTR bstrAsf(strSrcASF);

	CString strProfilePath = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
	CComBSTR bstrProfile(strProfilePath);

	CString cstOutput = MACRO_MP4_FILE(strCourse);
	CComBSTR bstrOutput(cstOutput);

	CString strTitle, strMsg, strTmp;
	strTitle.LoadString(IDS_CONVERT_FROM_ASF);
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();
	
	if (!GenProfile(strSrcASF, strCourse))
	{
		HideProgress();
		return FALSE;
	}
	// deleted by buxiangyi 一時削除
	/*HRESULT hr = m_pWMXUtility->WMXConvert(bstrAsf, OAFALSE, OAFALSE, OAFALSE, bstrProfile, bstrOutput);
	if (FAILED(hr))
	{
	strTmp.LoadString(IDS_FAIL_TO_CONV_BY_PROFILE);
	strMsg.Format(strTmp, strSrcASF);
	W_SHOW_ERROR_STR(strMsg, this);
	HideProgress();
	return FALSE;
	}*/
	if (NULL != m_pVideoConvert)
	{
		HRESULT hr = m_pVideoConvert->ConvertByBase(bstrAsf, bstrAsf, bstrOutput, 0);
		if (FAILED(hr))
		{
			HideProgress();
			return FALSE;
		}
	}

	HideProgress();
	return TRUE;
}

BOOL CMainFrame::ConvertVideo(CString strCourse)
{
	CString strTitle, strMsg;
	strTitle.LoadString(IDS_CONVERT_WMV_TO_ASF);
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();

	CStringArray strArrayFileName;
	CDWordArray strArrayFileLen;
	CString strNGFile = MACRO_NG_FILE(strCourse);
	if (!MergeWMVFile(strCourse))
		return FALSE;
	if (!ConvertVideoFromWMVToNoScript(strCourse))
		return FALSE;
	if (::FileExists(strNGFile))
	{
		CStringArray strArrayDelPeriod;

		CStringArray strArrayScript;
		CString strScriptFile = MACRO_SPT_FILE(strCourse);
		::LoadFile(strScriptFile, strArrayScript, TRUE);

		CStringArray strArrayNG;
		::LoadFile(strNGFile, strArrayNG, TRUE);
		int i = 0;
		for (i = 0; i < strArrayNG.GetSize(); i++)
		{
			CString strNG = strArrayNG[i];
			int nSpc = strNG.Find(" ");
			if (nSpc > 0)
			{
				CString strScript = strNG.Left(nSpc);
				CString strScript2 = strNG.Mid(nSpc + 1);
				CString strTimeStart, strTimeEnd;
				int j = 0;
				for (j = 0; j < strArrayScript.GetSize(); j++)
				{
					CString strTmp = strArrayScript[j];
					int nSpc3 = strTmp.Find(" ");
					if (nSpc3 > 0)
					{
						CString strTime;
						strTime.Format("%ld", atol(strTmp.Mid(nSpc3 + 1)) + 500);
						CString strScript3 = strTmp.Left(nSpc3);
						if (strScript3 == strScript)
						{
							strTimeStart = strTime;
							strTimeStart.TrimLeft();
							strTimeStart.TrimRight();
							break;
						}
					}
				}

				for (j = 0; j < strArrayScript.GetSize(); j++)
				{
					CString strTmp = strArrayScript[j];
					int nSpc3 = strTmp.Find(" ");
					if (nSpc3 > 0)
					{
						CString strTime;
						strTime.Format("%ld", atol(strTmp.Mid(nSpc3 + 1)) + 500);
						CString strScript3 = strTmp.Left(nSpc3);
						if (strScript3 == strScript2)
						{
							strTimeEnd = strTmp.Mid(nSpc3 + 1);
							strTimeEnd.TrimLeft();
							strTimeEnd.TrimRight();
							break;
						}
					}
				}
				strArrayDelPeriod.Add(strTimeStart + " " + strTimeEnd);
			}
		}

		::DeleteFile(strNGFile);

		CString strEditFile = MACRO_RECORD_VIDEO(strCourse);
		CStringArray strArrayEdit;
		long lStep = 0;
		long lStep2 = 0;
		long lStep3 = 0;
		long lStart = 0;
		long lEnd = 0;
		long lLength; 
		CString strTemp;
	    CString strWMVPath = MACRO_MP4_FILE(strCourse);
		GetFileDuration(strWMVPath, lLength);
		for (i = 0; i < strArrayDelPeriod.GetSize(); i++)
		{
			CString str = strArrayDelPeriod[i];
			int nSpc = str.Find(" ");
			if (nSpc > 0)
			{
				CString strStart = str.Left(nSpc);
				CString strEnd = str.Mid(nSpc + 1);
				lStep = lStep + lStep3 - lStep2;
				lStep2 = lEnd;
				lEnd = atol(strEnd);
				lStep3 = atol(strStart);
				strTemp.Format("%ld,*,%ld,%ld", lStep, lStep2, lStep3);
				strArrayEdit.Add(strTemp);
			}
		}
		if (lStep3 < lLength)
		{
			lStep = lStep + lStep3 - lStep2;
			lStep2 = lEnd;
			lStep3 = lLength;
			strTemp.Format("%ld,*,%ld,%ld", lStep, lStep2, lStep3);
			strArrayEdit.Add(strTemp);
		}
		::SaveFile(strEditFile, strArrayEdit);

		for (i = 0; i < strArrayScript.GetSize(); i++)
		{
			CString strScript = strArrayScript[i];
			int nSpace = strScript.Find(" ");
			if (nSpace > 5)
			{
				//strArrayScript.RemoveAt(i);
				//i--;
				continue;
			}

			if (nSpace >= 0)
			{
				CString strScriptTime = strScript.Mid(nSpace + 1);
				strScriptTime.TrimRight();
				long lTime = atol(strScriptTime);
				for (int j = 0; j < strArrayDelPeriod.GetSize(); j++)
				{
					CString str = strArrayDelPeriod[j];
					int nSpc = str.Find(" ");
					if (nSpc > 0)
					{
						CString strStart = str.Left(nSpc);
						CString strEnd = str.Mid(nSpc + 1);
						long lStart = atol(strStart);
						long lEnd = atol(strEnd);
						if (lTime < lEnd && lTime > lStart)
						{
							strArrayScript.RemoveAt(i);
							i--;
							break;
						}
					}
				}
			}
		}

		for (i = 0; i < strArrayScript.GetSize(); i++)
		{
			CString strScript = strArrayScript[i];
			int nSpace = strScript.Find(" ");
			if (nSpace >= 0)
			{
				CString strScriptTIme = strScript.Mid(nSpace + 1);
				strScriptTIme.TrimRight();
				long lTime = atol(strScriptTIme);
				long lDelta = 0;
				for (int j = 0; j < strArrayDelPeriod.GetSize(); j++)
				{
					CString str = strArrayDelPeriod[j];
					int nSpc = str.Find(" ");
					if (nSpc > 0)
					{
						CString strStart = str.Left(nSpc);
						CString strEnd = str.Mid(nSpc + 1);
						long lStart = atol(strStart);
						long lEnd = atol(strEnd);
						if (lTime >= lEnd)
							lDelta += (lEnd - lStart);
					}
				}
				lTime -= lDelta;
				CString strTemp;
				strTemp.Format("%ld", lTime);
				strScript = strScript.Left(nSpace) + " " + strTemp;
				strArrayScript[i] = strScript;
			}
		}
		::SaveFile(strScriptFile, strArrayScript, TRUE);
	}

	if (!ProcessAllScriptTime(strCourse))
		return FALSE;
	
	strTitle.LoadString(IDS_CREATE_INDEX);
	SetProgress(strTitle, strMsg);
	DumpKeyAndCreateIndex(strCourse);
	HideProgress();

	return TRUE;
}

//replase 2014/7/28 kagata chiba
//BOOL CMainFrame::GenPPTFiles(CString str, CString strPPTPath, BOOL& bWithMedia, BOOL bShowProgress)
BOOL CMainFrame::GenPPTFiles(CString str, CString strPPTPath, BOOL& bWithMedia, BOOL bShowProgress, int bSize)
{
	int nVer = 0;

	if (RegKeyExists("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Office\\15.0\\PowerPoint"))
        nVer = 15;  //add itoh 20130708 PPT2013
	if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\15.0\\PowerPoint"))
        nVer = 15;  //add itoh 20130708 PPT2013

	//add 2013/11/18 kagata BugNo.84 ※if文(nVer == 0)
	if(nVer == 0)
	{
	if (RegKeyExists("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Office\\14.0\\PowerPoint"))  //leu
        nVer = 14;  //leu
	if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\14.0\\PowerPoint"))  //add itoh 20120615
        nVer = 14;  //add itoh 20120615
	}

	//add 2013/11/18 kagata BugNo.84 ※if文(nVer == 0)
	if(nVer == 0)
	{
	if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\12.0\\PowerPoint"))  //leu
        nVer = 12;  //leu
	else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\11.0\\PowerPoint"))
        nVer = 11;
	else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\10.0\\PowerPoint"))
		nVer = 10;
	else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\9.0\\PowerPoint"))
		nVer = 9;
	}

	if (nVer == 0)
	{
		W_SHOW_ERROR(IDS_PPT_NOT_INSTALL, this);
		return FALSE;
	}

	MACRO_VIEW->StopMovieWndTimer();
	((CELearningApp*)AfxGetApp())->m_pWndTime->StopTimer();

	CString strTitle, strMsg;
	CProgressWnd* pwndProgress = NULL;
	if (bShowProgress)
	{
		strTitle.LoadString(IDS_FIRST_LOAD);
		pwndProgress = new CProgressWnd(this, strTitle, FALSE, 0, 4, FALSE);
		pwndProgress->SetFocus();
	}

	CPPT9Ctrl* pPPT9;
	CPPT10Ctrl* pPPT10;
	CPPT15Ctrl* pPPT15;		// add itoh 2013/07/11 PPT2013
	//CELearningView* pView;	// add itoh 2013/07/24 PPT2013
	if (nVer == 9)
	{
		pPPT9 = new CPPT9Ctrl;
		pPPT9->Create();
	}
	else if (nVer >= 10 && nVer <= 14)	// replace itoh 2013/07/11 PPT2013
	{
		pPPT10 = new CPPT10Ctrl;
		pPPT10->Create();
	}
	else if (nVer >= 15)		// add itoh 2013/07/11 PPT2013
	{
		pPPT15 = new CPPT15Ctrl;
		pPPT15->Create();
	}

	//pView = new CELearningView;	// add itoh 2013/07/24 PPT2013

	if (bShowProgress)
	{
		strMsg.LoadString(IDS_LOAD_PPT);
		pwndProgress->SetText(strMsg);
		pwndProgress->StepIt();
		pwndProgress->SetFocus();
	}

	int nSlideNo;
	if (nVer == 9)
	{
		pPPT9->OpenPPT(strPPTPath);
		nSlideNo = pPPT9->GetSlideCount();
//		pPPT9->SetPublishtoDir();	// del itoh 2013/07/09 PPT2013
	}
	else if(nVer == 15)		// add itoh 2013/07/11 PPT2013
	{
		pPPT15->OpenPPT(strPPTPath);
		nSlideNo = pPPT15->GetSlideCount();
	}
	else
	{
		pPPT10->OpenPPT(strPPTPath);
		nSlideNo = pPPT10->GetSlideCount();
//		pPPT10->SetPublishtoDir();	// del itoh 2013/07/09 PPT2013
	}

	m_nSlideNo = nSlideNo;
	if (nSlideNo == 1)
	{
		if (nVer == 9)
			pPPT9->AppendSlide();
		else if(nVer == 15)		// add itoh 2013/07/11 PPT2013
			Sleep(500);//pPPT15->AppendSlide();		//replace 2013/11/15 kagata BugNo.84
		else
			pPPT10->AppendSlide();
	}
	CString strDir = MACRO_COURSE_PATH + str + "\\";

	if (bShowProgress)
	{
		pwndProgress->PeekAndPump();
		strMsg.LoadString(IDS_SAVE_HTM);
		pwndProgress->SetText(strMsg);
		pwndProgress->StepIt();
		pwndProgress->SetFocus();
	}

//	CString strHTMPath = strDir + "Content.htm";
	CString strPath = ::GetSysPath();
	//add 2014/7/28 kagata chiba bSize判定追加
	if(bSize == 0)
	{
		::CopyFile(strPath + "\\Template\\script.js", strDir + "\\html\\script.js", FALSE);	// add itoh 2013/07/25 PPT2013
		if (nVer == 9) {
	//		pPPT9->WebPublish(strHTMPath);
			pPPT9->WebPublish(strPath,strPPTPath,strDir);
		}
		else if(nVer == 15) {		// add itoh 2013/07/11 PPT2013
			pPPT15->WebPublish(strPath,strPPTPath,strDir);
		}
		else {
	//		pPPT10->WebPublish(strHTMPath);
			pPPT10->WebPublish(strPath,strPPTPath,strDir);
		}
	}

	if (bShowProgress)
	{
		pwndProgress->PeekAndPump();
		strMsg.LoadString(IDS_SAVE_OUTLINE);
		pwndProgress->SetText(strMsg);
		pwndProgress->StepIt();
		pwndProgress->SetFocus();
	}
	
	if (strDir.Right(1) != "\\")
		strDir += "\\";
	
	strDir += "html\\movie\\outline.lrn";

	CStringArray strArrayTitle;
	if (nVer == 9)
		pPPT9->GetSliderTitle(strArrayTitle);
	else if (nVer == 15)	// add itoh 2013.07/11 PPT2013
		pPPT15->GetSliderTitle(strArrayTitle);
	else
		pPPT10->GetSliderTitle(strArrayTitle);

	CString strUndefined;
	strUndefined.LoadString(IDS_UNDEFINED);
	for (int i = 0; i < strArrayTitle.GetSize(); i++)
	{
		if (strArrayTitle.GetAt(i) == "")
			strArrayTitle.SetAt(i, strUndefined);
	}

	strArrayTitle.InsertAt(0, "version 1.0");

	//repalse 2014/7/28 kagata chiba
	//::SaveFile(strDir, strArrayTitle, TRUE);
	::SaveFile(strDir, strArrayTitle, FALSE);

	if (bShowProgress)
	{
		pwndProgress->PeekAndPump();
		strMsg.LoadString(IDS_CLOSE_PPT);
		pwndProgress->SetText(strMsg);
		pwndProgress->StepIt();
		pwndProgress->SetFocus();
	}

	if (nVer == 9)
	{
		bWithMedia = pPPT9->WithMedia();
		pPPT9->Close();
		delete pPPT9;
	}
	else if(nVer == 15)		// add itoh 2013/07/11 PPT2013
	{
		bWithMedia = pPPT15->WithMedia();
		pPPT15->Close();
		delete pPPT15;
	}
	else
	{
		bWithMedia = pPPT10->WithMedia();
		pPPT10->Close();
		delete pPPT10;
	}

	if (bShowProgress)
	{
		pwndProgress->PeekAndPump();
		delete pwndProgress;
	}

	MACRO_VIEW->RestartMovieWndTimer();
	((CELearningApp*)AfxGetApp())->m_pWndTime->RestartTimer();

	return TRUE;
}

/*
void CMainFrame::CopyPracticeFile(int nIndex)
{
	CString strCourse = GetCourse();
	if (strCourse == "")
		return TRUE;
	
	CString strPath = ::GetSysPath();
	CString strEmptyPath = strPath + "empty.htm";
	CString strCoursePath = MACRO_COURSE_PATH + strCourse;
	CString strFile;
	strFile.Format("practice%d.htm", nIndex + 1);
	CString strHTMLPath = strCoursePath + "\\html\\" + strFile;
	::CopyFile(strEmptyPath, strHTMLPath, FALSE);
}
*/

void CMainFrame::LoadCourse(BOOL bForce)
{
	//added by angf 2015/01/09
	CString strTitle  = "";

#ifdef _KDI
	strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
	strTitle.LoadString(IDR_MAINFRAME);
#endif

	CString strCourse = GetCourse();
	SetWindowText(strTitle + "-" + strCourse);
	//added end
	CString strPath = MACRO_COURSE_PATH + strCourse;
	if (!::FileExists(strPath))
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_COURSE_RELOAD);
		strMsg.Format(strTmp, strCourse);
		SHOW_WARNING_STR(strMsg);
		CollectCourse("");
		return;
	}

	if (!::IsFileWritable(strPath))
		::SetTreeWritable(strPath);

	LoadCourse(strCourse, bForce);
}

void CMainFrame::StopRunning()
{
	CELearningView* pView = (CELearningView*)GetActiveView();
	if (pView)
		pView->Stop();
}

void CMainFrame::WriteVideoDate(CString strCourse, CString strMediaPath)
{
	CString strIniPath = MACRO_INI_FILE(strCourse);
	if (::FileExists(strMediaPath))
	{
		CString strDate = ::GetFileCreateTime(strMediaPath);
		::WritePrivateProfileString("Information", "Date", strDate, strIniPath);
	}
}

void CMainFrame::WriteVideoLength(CString strCourse)
{
	CString strIniPath = MACRO_INI_FILE(strCourse);
	CString strASFPath = MACRO_MP4_FILE(strCourse);
	long lDuration = GetMediaLength(strASFPath);

	//add by bxiangyi
	CStringArray m_oldarray, array;
	CString m_time, strPath;
	m_time.Format("%d",lDuration/1000);
	m_oldarray.Add("var vduration =" );
	array.Add("var vduration = " + m_time + ";");
	strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

	CString strDuration = ::ConvertTimeToStr(lDuration);
	strDuration = strDuration.Left(strDuration.GetLength() - 2);
	::WritePrivateProfileString("Information", "Duration", strDuration, strIniPath);
}

void CMainFrame::WriteSlideNo(CString strCourse)
{
	CString strIniPath = MACRO_INI_FILE(strCourse);
	CString strSldNo;
	strSldNo.Format("%d", m_nSlideNo);
	::WritePrivateProfileString("Information", "SlideNo", strSldNo, strIniPath);
}

//replase 2014/6/17 kagata outemon
//void CMainFrame::LoadCourse(CString strCourse, BOOL bForce, BOOL bAsk)
void CMainFrame::LoadCourse(CString strCourse, BOOL bForce, BOOL bAsk, BOOL bSkip)
{

	if (strCourse == m_strCourse && !bForce)
		return;

	m_bNoLoadingVideo = TRUE;

	CELearningView* pView = (CELearningView*)GetActiveView();
	BOOL bFirst = FALSE;

	SetComboSel(strCourse);		//add itoh 20120621
		
	BOOL bFirstLoading = IsFirstLoading(strCourse);
	if (bFirstLoading)
	{
		//add 2014/6/17 kagata outemon
		if(!bSkip)
			return;

		if (bAsk)
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_GEN_COURSE);
			strMsg.Format(strTmp, strCourse);
			if (W_ASK_YESNO_STR(strMsg, this) == IDNO)
			{

				LoadCourse("");
				return;
			}
		}
		UpdateWindow();

		bFirst = TRUE;
		
		CString strIniPath = MACRO_INI_FILE(strCourse);
		CString strWMVPath = MACRO_ORG_FILE(strCourse);
		WriteVideoDate(strCourse, strWMVPath);
		if (::FileExists(strWMVPath))
			ConvertVideo(strCourse);
		WriteVideoLength(strCourse);

		char szBuf[256];
		::GetPrivateProfileString("General", "PPTFile", "", szBuf, 255, strIniPath);
		if (szBuf[0] != '\0')
		{
			CString strPPTPath = MACRO_COURSE_PATH + strCourse + "\\" + CString (szBuf);

			BOOL bWithMedia;
			GenPPTFiles(strCourse, strPPTPath, bWithMedia, TRUE);
			UpdateWindow();

			CString strTitle;
			strTitle.LoadString(IDS_GENETATING_COURSE);
			CString strMsg;
			strMsg.LoadString(IDS_PROCESSING);
			SetProgress(strTitle, strMsg);
			ShowProgress();

			pView->SubmitLesson(strCourse, bWithMedia);

			HideProgress();
		}
		else
		{
			SaveHyperLink(strCourse);
			pView->SubmitHTMLLesson(strCourse);
		}

		WriteSlideNo(strCourse);

		CString strASFPath = MACRO_MP4_FILE(strCourse);
		AddMarkScriptToASF();
		CreateIndex(strASFPath);

		
		CString strWMVFile = MACRO_MP4_FILE(strCourse);
		CString strFinalWMVFile = MACRO_FINAL_MP4_FILE(strCourse);
		/* wangwang20080311
		//wangwang
		long lDuration = GetMediaLength(strWMVFile);
		CString strSeg;
		strSeg.Format("0,*,0,%ld;",lDuration-1300);
        WriteRecordFile(MACRO_RECORD_VIDEO(strCourse), strSeg);
		pView->SaveVideoAfterRecording( );
        ::SafeCopyFile(strFinalWMVFile,strWMVFile, FALSE);
        CStringArray strExclude;

		strExclude.Add(strWMVFile);
		strExclude.Add(MACRO_VIDEO_DIR(strCourse) + "default.prx");
		DelTree(MACRO_VIDEO_DIR(strCourse),&strExclude);
        */
		//SureDeleteFile(MACRO_RECORD_VIDEO(strCourse));
		//SureDeleteFile(MACRO_PRE_RECORD_VIDEO(strCourse));

		::SafeCopyFile(strWMVFile, strFinalWMVFile, FALSE);
		//added by angf 2014/12/01
		m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(3));
		UpdateWindow();
	//added by angf 2014/12/01
	} else {
		
		//add by buxiangyi
		if (MACRO_VIEW ->NewCourseFlag == TRUE)
		{
			m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(2));
		}
		if (MACRO_VIEW -> OpenFileFlag == TRUE)
		{
		
			if (m_bAskJmp)
			{
				m_bAskJmp = FALSE;
				m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(3));
			}
			else
			{
				m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(3));
			}
			CString strPath;
			CFileFind finder;
			strPath = MACRO_VIDEO_DIR(strCourse) + "*.*";  
			BOOL bWorking = finder.FindFile(strPath);  
			int FileCount = 0;
			while(bWorking)
			{  
			
			
				bWorking = finder.FindNextFile();  
				if(finder.IsDirectory() || finder.IsDots())
				{
					continue;
				}  
				else
				{ 
					FileCount ++;
				}  
			} 
			if (FileCount <= 0)
			{
				m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(2));


			}
			else
			{
				strPath = MACRO_HTML_DIR(strCourse) +"movie\\*.*";
				bWorking = finder.FindFile(strPath);  
				FileCount = 0;
				while(bWorking)
				{  


					bWorking = finder.FindNextFile();  
					if(finder.IsDirectory() || finder.IsDots())
					{
						continue;
					}  
					else
					{ 
						FileCount ++;
					}  
				} 
				if (FileCount <= 0)
				{
					m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(3));
				}
			}
		}
		
	}

//	SetComboSel(strCourse);		//del itoh 20120621

	m_bNoLoadingVideo = FALSE;

	m_strWaitFile = "";
	m_lTimeStart = -1;
	m_lTimeEnd = -1;

	CString strEditFile = MACRO_RECORD_VIDEO(strCourse);
	if (bFirstLoading && ::FileExists(strEditFile))
	{
#ifdef _BUNDLE

		W_SHOW_MESSAGE(IDS_NG_GENERATE, this);
		pView->SaveCourse(strCourse, FALSE);
#else

		if (W_ASK_OKCANCEL(IDS_NG_GENERATE_CONFIRM, this) == IDOK)
			pView->SaveCourse(strCourse, FALSE);
#endif
		pView->LoadCourse(strCourse, TRUE);
	}
	else
		pView->LoadCourse(strCourse, bForce);
    if (m_strCourse != strCourse)
	{
        if (m_strCourse!="")
		{
		  //m_ChangeCourse=true;
        }
	}
    m_bPause=false;
	m_strCourse = strCourse;

	m_objUndoRedo.ClearStack();
}
void CMainFrame::WriteRecordFile(CString strPath, CString strSeg)
{

	CStringArray strArraySeg;
	//GetFilenameforrecordfile(strSeg);
	ParseStringToArray(strSeg, strArraySeg);
	::SaveFile(strPath, strArraySeg, TRUE);

	
}
void CMainFrame::ParseStringToArray(CString strSeg, CStringArray& strArraySeg)
{
	int nCount = 0;
	CString strTok;
	while (::GetTokStr(strSeg, strTok, nCount, ";"))
		strArraySeg.Add(strTok);
}
void CMainFrame::SaveHyperLink(CString strCourse)
{
	CString strDir = MACRO_COURSE_PATH + strCourse + "\\";

	CStringArray strArrayTitle;
	strArrayTitle.Add("version 1.0");

	CString strPlaylistFile = strDir + "webpages\\index.pll";
	int nHTMLCount = ::GetPrivateProfileInt("General", "Count", 0, strPlaylistFile);
	m_nSlideNo = nHTMLCount;
	CString strSec, strTmp;
	char szBuffer[256];
	for (int i = 0; i < nHTMLCount; i++)
	{
		strSec.Format("Page%d", i);
		::GetPrivateProfileString(strSec, "Title", "", szBuffer, 256, strPlaylistFile);
		strArrayTitle.Add(szBuffer);
	}

	::SaveFile(strDir + "html\\movie\\outline.lrn", strArrayTitle, FALSE);
}

void CMainFrame::AddMarkScriptToASF()
{
	CString str = GetCourse();
	((CELearningView*)GetActiveView())->AddMarkScriptToASF(str);
}

BOOL CMainFrame::IsASFTagged(CString strAsf)
{
	CMediaUtils* pMedia = new CMediaUtils;
	if (!pMedia->Create())
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->Open(strAsf))
	{
		delete pMedia;
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	if (pMedia->IsMarked())
		bRet = TRUE;

	delete pMedia;

	return bRet;
}

void CMainFrame::OnNewAddress()
{
	PostMessage(WM_NEW_COURSE); //ｪｧｨ峵ﾉｶ｡･HｧKComboｱｾｦbｨｺｸﾌ､｣ｷ|ｰﾊ
}

LRESULT CMainFrame::OnLoadCourse(WPARAM wParam, LPARAM lParam)
{
	CString strCourse = m_strTmpCourse;
	LoadCourse(m_strTmpCourse, TRUE, FALSE);
	//m_strTmpCourse = "";	//del 2014/7/28 kagata chiba
	MACRO_VIEW->RegenPptSlide(TRUE,1);	//add 2014/7/28 kagata chiba
//	CPageVideoEdit* pVedit = new CPageVideoEdit();	//add 2014/7/28 kagata chiba
	CAdapter* pVedit = new CAdapter();	//add 2014/11/21 angf
	pVedit->SaveAllParamFiles(m_strTmpCourse);	//add 2014/7/28 kagata chiba

	delete(pVedit);	//add 2014/7/28 kagata chiba

	CString htmlPath = MACRO_HTML_DIR(m_strTmpCourse);
	SafeCopyFile(htmlPath + "movie\\draw.lrn", htmlPath + "draw.js");

	//add by buxiangyi
	CStringArray m_oldarray, array;
	CString  strPath;
	m_oldarray.Add("var TitleName =" );
	ConvertCodePage(m_strTmpCourse, CP_ACP ,CP_UTF8);
	array.Add("var TitleName = \"" + m_strTmpCourse + "\";");
	strPath  = MACRO_JS_PATH(m_strTmpCourse);
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);


	MACRO_VIEW ->NewArrayLayout();
	MACRO_VIEW->SaveConfig(strCourse);

	//add 2014/6/12 kagata outemon
	BeginWaitCursor();
	int eck = 1;
	do{
		eck = NasUpCourse();
	}while(eck <= 0);
	EndWaitCursor();
	
	m_strTmpCourse = "";	//add 2014/7/28 kagata chiba

	return 0L;
}


LRESULT CMainFrame::OnNewCourse(WPARAM wParam, LPARAM lParam)
{
	LoadCourse(TRUE);
	SetFocus();
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

LRESULT CMainFrame::OnGetUp(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOWMAXIMIZED);
	BringWindowToTop();
	SetForegroundWindow();
	UpdateWindow();

	//add 2014/8/5 kagata outemon
	if(wParam != 2)
	{
		//add 2014/6/13 kagata outemon
		BeginWaitCursor();
		int eck = 1;
		do{
			eck = NasUpCourse();
		}while(eck <= 0);
		EndWaitCursor();
	}

	return 0L;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pData) 
{
	ShowWindow(SW_SHOWMAXIMIZED);
	BringWindowToTop();
	SetForegroundWindow();
	SetFocus();
	UpdateWindow();
	
	if (m_bCannotRun)
		return CFrameWndEx::OnCopyData(pWnd, pData);

	if (pData->cbData == sizeof(IPCStruct))
	{
		IPCStruct ipcs;
		memcpy(&ipcs, pData->lpData, sizeof(ipcs));
		
		CString strPath = ipcs.szPath;
		CString strCoursePath = MACRO_COURSE_PATH;
		if (strPath.Left(strCoursePath.GetLength()).CompareNoCase(strCoursePath) == 0)
		{
			strPath = strPath.Mid(strCoursePath.GetLength());
			if (strPath.Right(1) == "\\")
				strPath = strPath.Left(strPath.GetLength() - 1);
			SetComboSel(strPath);
		
			m_strTmpCourse = strPath;
		}
	}

	return CFrameWndEx::OnCopyData(pWnd, pData);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CString strTitle;
	strTitle.LoadString(IDR_MAINFRAME);
	SetWindowText(strTitle);
}

void CMainFrame::OnDestroy() 
{
	CFrameWndEx::OnDestroy();
}

void CMainFrame::OnClose() 
{
	//add 2014/6/13 kagata outemon
	//終了時の保存処理
	BeginWaitCursor();
	m_pCEApp = (CELearningApp*)this;
	int	m_dEnUp = m_pCEApp->ChkNasUpEn();
	int m_dNetAccess = m_pCEApp->GetUpCompInfo();
	CString m_dAdress = m_pCEApp->GetNasDir(); 
	if(m_dEnUp == 1 && m_dAdress != "" && m_dNetAccess == 1)
	{
		//add 2014/8/8 kagata outemon
		CStringArray strInfo;
		strInfo.RemoveAll();
		::GetDir(MACRO_COURSE_PATH, strInfo);

		//add 2014/8/8 kagata outemon
		if(strInfo.GetSize() > 0)
		{
			//add 2014/8/8 kagata outemon
			CString strExitinfo;
			strExitinfo.LoadString(59505);
			int y;
			y = AfxMessageBox(strExitinfo,MB_ICONINFORMATION | MB_YESNO);
			if(y != IDYES)
				return;

			int m_dNetAccess = PathIsDirectory(m_dAdress);
			if(!m_dNetAccess)
				m_dNetAccess = m_pCEApp->NasDirRetry(m_dAdress, 59498);

			if(m_dNetAccess)
			{
				int nret;
				CString strCorPath = MACRO_COURSE_PATH;
				CStringArray strArray;

				do
				{
					strArray.RemoveAll();
					::GetDir(strCorPath, strArray);
					nret = m_pCEApp->NasLocalCheck(m_dAdress, strArray);
				}	while(nret == -1 || nret == -2);	//replase 2014/8/8 kagata outemon
			}
		}
		
		StopRunning();
	
		CFrameWndEx::OnClose();
	}
	else
	{
		StopRunning();
	
		CFrameWndEx::OnClose();
	}
	//del 2014/6/13 kagata outemon
	//StopRunning();
	
	//CFrameWndEx::OnClose();
}

BOOL CMainFrame::DumpKeyAndCreateIndex(CString strCourse)
{
	if (strCourse == "")
		return TRUE;

	CString strIndexPath = MACRO_MP4_FILE(strCourse);
	CString strFinalIndexPath = MACRO_FINAL_MP4_FILE(strCourse);
	return CreateIndex(strIndexPath) && CreateIndex(strFinalIndexPath);
}

BOOL CMainFrame::CreateIndex(CString strFile)
{
	if (!::FileExists(strFile))
		return TRUE;

	CComBSTR bstrSource(strFile);
	VARIANT_BOOL vbool = OAFALSE;
	//add by buxiangyi　一時削除
	/*HRESULT hval = m_pWMXUtility->IsWMXIndexExisted(bstrSource, &vbool);
	if (FAILED(hval)) 
		return FALSE;

	if (vbool == OAFALSE) 
	{
		hval = m_pWMXUtility->WMXMakeIndex(bstrSource);
		if (FAILED(hval)) 
			return FALSE;
	}*/
	////////////////////////////////////////////////////////////
	return TRUE;
}

LRESULT CMainFrame::OnProgress(WPARAM wParam, LPARAM lParam)
{
	int i = HIWORD(lParam); 
	int j = LOWORD(lParam); 
	int k;
	CString strPerc = "";
	if (i == 0)
		k = j;
	else
	{
		k = (int)((float)j / i * 100.0);
		if (k > 100)
			k = 100;
		strPerc = "%";
	}

	CString strMsg;
	strMsg.Format("%s (%d%s)", m_strMsg, k, strPerc);
	SetProgress(m_strTitle, strMsg);
	return 0L;
}

BOOL CMainFrame::ConvertFile(LPCTSTR szIn, LPCTSTR szOut, int nAudioFreq, int nVideoFreq, BOOL bGenProfile)
{
	//return FALSE;
	CString			cstSource(szIn);
	CString			cstOutput(szOut);
	
	UpdateWindow();

	CString strTitle;
	strTitle.LoadString(IDS_CONVERT_FROM_ASF);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();

	BSTR bstrProfile = NULL;
	if (bGenProfile)
	{
		CString strProfile = cstOutput;
		int nIndex = cstOutput.ReverseFind('.');
		if (nIndex >= 0)
		{
			strProfile = strProfile.Left(nIndex) + ".prx";
			bstrProfile = strProfile.AllocSysString();
		}
	}

	CComBSTR bstrSource(cstSource);
	CComBSTR bstrOutput(cstOutput);

	if (NULL != m_pVideoConvert)
	{
		HRESULT hr = m_pVideoConvert->ConvertByBase(bstrSource, bstrSource, bstrOutput, 0);
		if (FAILED(hr))
		{
			HideProgress();
			return FALSE;
		}
	}

	HideProgress();
	return TRUE;
}

BOOL CMainFrame::ConvertVideoFromWMVToNoScript(CString strCourse)
{
	if (strCourse == "")
		return TRUE;

	CString strOrigVideo = MACRO_ORG_FILE(strCourse);
	if (!::FileExists(strOrigVideo))
		return FALSE;
	if (!ConvertWMVWithoutScript(strCourse))
		return FALSE;

	return TRUE;
}

BOOL CMainFrame::ConvertWMVWithoutScript(CString strCourse)
{
	CString strORGPath = MACRO_ORG_FILE(strCourse);
	CString strWMVPath = MACRO_MP4_FILE(strCourse);
	CString strFinalWMVPath = MACRO_FINAL_MP4_FILE(strCourse);
	CString strScriptFile = MACRO_SPT_FILE(strCourse);

	CComBSTR bstrORGPath(strORGPath);
	CComBSTR bstrWMVPath(strWMVPath);
	CComBSTR bstrScriptFile(strScriptFile);

	//add by angf
	rename(strORGPath, strWMVPath);


	return TRUE;
}

BOOL CMainFrame::CopyTag(CString strSrcAsf, 
					     CString strDestAsf)
{

	return FALSE;
}

BOOL CMainFrame::LoadTag(CString strCourse, CStringArray& strMarkTimeArray, 
						 CStringArray& strMarkArray, CStringArray& strScriptTimeArray,
						 CStringArray& strScriptArray, CStringArray& strCmdArray)
{

	CString strStepPath = MACRO_STEP_FILE(strCourse);
	CString strOutLinePath = MACRO_OUTLINE_FILE(strCourse);
	CStringArray strStepArray;
	CStringArray strTitleArray;
	::LoadFile(strStepPath, strStepArray, TRUE);
	::LoadFile(strOutLinePath, strTitleArray, TRUE);
	strMarkTimeArray.RemoveAll();
	strMarkArray.RemoveAll();
	strScriptTimeArray.RemoveAll();
	strScriptArray.RemoveAll();
	strCmdArray.RemoveAll();
	for(int index = 1; index < strStepArray.GetSize(); index++)
	{
		CString strStep = strStepArray[index];
		int length = strStep.Find(" ");
		CString time = strStep.Left(length);
		strStep = strStep.Right(strStep.GetLength() - length - 1);
		length = strStep.Find(" ");
		CString cmd = strStep.Left(length);
		CString script = strStep.Right(strStep.GetLength() - length - 1);
		int bufLen = 0;
		char* strBuf = script.GetBuffer();
		while(1)
		{
			if (strBuf[bufLen] >= 48 && strBuf[bufLen] <= 57)
				bufLen++;
			else
				break;
		}
		CString slideNo = script.Left(bufLen);
		CString slideTitle = strTitleArray[atoi(slideNo.GetBuffer(bufLen))];
		for (; slideNo.GetLength() < 3;)
		{
			slideNo = "0" + slideNo;
		}
		CString slideMark = "s" + slideNo + " " + slideTitle;
		if ((cmd == "flipslide" || cmd == "flippage") && script.Find(".") < 0)
		{
			strMarkTimeArray.Add(time);
			strMarkArray.Add(slideMark);
		}
		if (script == "zoomin" || script == "zoomout")
		{
			cmd = script;
			char szBuf[32];
			CString strMark;
			strMark = ltoa(GetTickCount() + 10000, szBuf, 10);
			script = strMark;
			strMarkTimeArray.Add(time);
			strMarkArray.Add(::MakeCtrlMark(strMark));
		}
		if (cmd == "demo")
		{
			strMarkTimeArray.Add(time);
			strMarkArray.Add(::MakeDemoMark(script));
		}
		strScriptTimeArray.Add(time);
		strScriptArray.Add(script);
		strCmdArray.Add(cmd);
	}


	return TRUE;
}

BOOL CMainFrame::SaveTag(CString strAsfFile, CStringArray& strMarkTimeArray, 
						 CStringArray& strMarkArray, CStringArray& strScriptTimeArray,
						 CStringArray& strScriptArray, CStringArray& strCmdArray, BOOL bRemove)
{
	if (!::FileExists(strAsfFile))
		return FALSE;

	CMediaUtils* pMedia = new CMediaUtils;
	if (!pMedia->Create())
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->Open(strAsfFile))
	{
		delete pMedia;
		return FALSE;
	}

	if (bRemove)
	{
		if (!pMedia->RemoveAllMarkers(FALSE) || !pMedia->RemoveAllScripts(TRUE))
		{
			delete pMedia;
			return FALSE;
		}
	}

	CStringArray strArrayOutline, strArrayStep;
	int i = 0;
	for (i = 0; i < strMarkArray.GetSize(); i++)
		strArrayOutline.Add(strMarkTimeArray[i] + " " + strMarkArray[i] );
	for (i = 0; i < strScriptArray.GetSize(); i++)
		strArrayStep.Add(strScriptTimeArray[i] + " " + strCmdArray[i] + " " + strScriptArray[i]);

	BOOL bRet = pMedia->TagASFBothWithTime(strArrayOutline, strArrayStep);

	delete pMedia;

	return bRet;
}

void CMainFrame::SetProgress(CString strTitle, CString strMsg)
{
	CString strOldTitle;
	m_dlgProgress.GetWindowText(strOldTitle);
	if (strOldTitle != strTitle)
		m_dlgProgress.SetWindowText(strTitle);
	
	CString strOldMsg;
	m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->GetWindowText(strOldMsg);
	if (strOldMsg != strMsg)
		m_dlgProgress.GetDlgItem(IDC_STATIC_MSG)->SetWindowText(strMsg);

	int nIndex;
	if ((nIndex = strMsg.Find("(")) >= 0)
	{
		m_strMsg = strMsg.Left(nIndex);
		m_strMsg.TrimRight();
	}
	else
		m_strMsg = strMsg;

	m_strTitle = strTitle;
}

void CMainFrame::ShowProgress()
{
	m_dlgProgress.SetCapture();

	m_dlgProgress.Play();
	m_dlgProgress.ShowWindow(SW_SHOW);
	m_dlgProgress.UpdateWindow();
	m_dlgProgress.SetFocus();
}

void CMainFrame::HideProgress()
{
	m_dlgProgress.Stop();
	m_dlgProgress.ShowWindow(SW_HIDE);

	::ReleaseCapture();
}

LRESULT CMainFrame::OnTimeLeft(WPARAM wParam, LPARAM lParam)
{
	CString strTime = ::ConvertSecToMinStr(wParam);
	CString strTimeLeft;
	strTimeLeft.LoadString(IDS_TIME_LEFT_LABEL);
	m_wndStatusBar.SetPaneText(1, strTimeLeft + strTime);

	return 0L;
}

LRESULT CMainFrame::OnTimeOver(WPARAM wParam, LPARAM lParam)
{
	CString strTime = ::ConvertSecToMinStr(0);
	CString strTimeLeft;
	strTimeLeft.LoadString(IDS_TIME_LEFT_LABEL);
	m_wndStatusBar.SetPaneText(1, strTimeLeft + strTime);

	m_bCannotRun = TRUE;
	SHOW_MESSAGE(IDS_TIME_OUT);
	PostMessage(WM_CLOSE);
	return 0L;
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CFrameWndEx::OnCommand(wParam, lParam);
}

void CMainFrame::OnEndTrace() 
{
}

void CMainFrame::OnStartTrace() 
{
}

void CMainFrame::CopyToClipboard() 
{
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID == SC_RESTORE || nID == 0xF122) && IsZoomed())
		return;
	
	if (nID == SC_MINIMIZE)
		StopRunning();

	CFrameWndEx::OnSysCommand(nID, lParam);
}

LRESULT CMainFrame::OnWaitForFile(WPARAM wParam, LPARAM lParam)
{
	CString strPeriod = (const char *)wParam;
	
	sscanf(strPeriod, "%ld %ld", &m_lTimeStart, &m_lTimeEnd);
	m_strWaitFile = (const char *)lParam;

	if (::FileExists(m_strWaitFile))
		::DeleteFile(m_strWaitFile);

	return 0L;
}

LRESULT CMainFrame::OnFileRecorded(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOWMAXIMIZED);
	BringWindowToTop();
	SetForegroundWindow();
	UpdateWindow();

	if (m_strWaitFile != "" && ::FileExists(m_strWaitFile))
	{
		if (!MACRO_MAINFRM->CreateIndex(m_strWaitFile))
		{
			W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
			return FALSE;
		}
		MACRO_VIEW->ReplacePeriodByFile(m_strWaitFile, m_lTimeStart, m_lTimeEnd);
	}

	m_strWaitFile = "";
	m_lTimeStart = -1;
	m_lTimeEnd = -1;

	return 0L;
}

BOOL CMainFrame::ConvertBandWidth(CString strSrc, CString strSrcProfile, CString strDest, 
								  int nVideoBand, int nFPS, int nKeyFrame, int nVideoQuality, 
								  int nAudioBand, int nAudioFreq, int nMono, BOOL bCopyScript)
{
	if (!::FileExists(strSrc))
		return FALSE;

	CComBSTR cbstrSrcProfilePath(strSrcProfile);

	CString strTmpNewProfile = ::GetFilePath(strSrcProfile) + "_tmp.prx";
	CComBSTR cbstrDestProfilePath(strTmpNewProfile);


	
	CComBSTR cbstrSrcPath(strSrc);
	CComBSTR cbstrDestPath(strDest);
	VARIANT_BOOL vbCopyScript = (bCopyScript) ? OATRUE : OAFALSE;


	::DeleteFile(strTmpNewProfile);
	
	return TRUE;
}

BOOL CMainFrame::ConvertAudio(CString strSrc, CString strSrcProfile, CString strDest, BOOL bCopyScript)
{
	if (!::FileExists(strSrc))
		return FALSE;

	CString strTitle, strMsg, strTmp;
	strTitle.LoadString(IDS_CONVERT_TO_AUDIO);
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();

	CComBSTR cbstrSrcProfilePath(strSrcProfile);

	CString strTmpNewProfile = ::GetFilePath(strSrcProfile) + "_tmp.prx";
	CComBSTR cbstrDestProfilePath(strTmpNewProfile);
	HRESULT hr = 0;

	
	CComBSTR cbstrSrcPath(strSrc);
	CComBSTR cbstrDestPath(strDest);
	CString strExt = ::GetFileExt(strSrc);
	CString strExtDest = ::GetFileExt(strDest);
	strExt.MakeLower();
	strExtDest.MakeLower();
	if (strExt == "asf" || strExt == "mp4" || strExt == "wma")
	{
		VARIANT_BOOL vbCopyScript = (bCopyScript) ? OATRUE : OAFALSE;
		::SafeCopyFile(strSrc, strDest);

	}

	if (FAILED(hr))
	{
		HideProgress();
		return FALSE;
	}

	HideProgress();
	
	::DeleteFile(strTmpNewProfile);
	
	return TRUE;
}

long CMainFrame::GetMediaLength(CString strPath)
{
	if (!::FileExists(strPath))
		return FALSE;
#if 1
	long lDuration = 0;
	//HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	//if (SUCCEEDED(hr))
	{
		// Initialize the Media Foundation platform.
		HRESULT hr = MFStartup(MF_VERSION);
		if (SUCCEEDED(hr))
		{
			// Create the source reader.
			IMFSourceReader *pReader;
			USES_CONVERSION;
			hr = MFCreateSourceReaderFromURL(A2W(strPath), NULL, &pReader);
			if (SUCCEEDED(hr))
			{
				LONGLONG hnsDuration;
				PROPVARIANT var;
				HRESULT hr = pReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, 
					MF_PD_DURATION, &var);
				if (SUCCEEDED(hr))
				{
					hr = PropVariantToInt64(var, &hnsDuration);
					PropVariantClear(&var);
					lDuration = (long)(hnsDuration / 10000);
				}
				pReader->Release();
			}
			// Shut down Media Foundation.
			MFShutdown();
		}
		//CoUninitialize();
	}
	return lDuration;
#else
	CString strExt = ::GetFileExt(strPath);
	strExt.MakeLower();
	if (strExt == "asf" || strExt == "wmv" || strExt == "wma")
	{
		long lDuration = -1;
		::GetFileDuration(strPath, lDuration);
		return lDuration;
	}
	else if(strExt == "mp4")
	{
		byte readBuf[8] = {0};
		UINT boxSize = 0;
		UINT64 boxLargeSize = 0;
		UINT64 fileLen = 0;
		UINT64 seekLen = 0;
		UINT timeScale = 0;
		UINT duration = 0;
		CFile mp4File(strPath, CFile::modeRead | CFile::shareDenyNone);
		fileLen = mp4File.GetLength();
		if(fileLen >= 8)
		{
			mp4File.Seek(4, CFile::begin);
			mp4File.Read(readBuf, 4);
			if (memcmp(readBuf, "ftyp", sizeof(char) * 4) == 0)
			{
				mp4File.SeekToBegin();
				while(fileLen - mp4File.GetPosition() >= seekLen + 24)
				{
					mp4File.Seek(seekLen, CFile::current);
					mp4File.Read(readBuf, 4);
					boxSize = 0;
					for(int i = 0; i < 3; i++)
					{
						boxSize += readBuf[i];
						boxSize = boxSize << 8;
					}
					boxSize += readBuf[3];
					if (boxSize == 1)
					{
						mp4File.Read(readBuf, 4);
						if (memcmp(readBuf, "mdat", sizeof(char) * 4) == 0)
						{
							mp4File.Read(readBuf, 8);
							boxLargeSize = 0;
							for(int i = 0; i < 7; i++)
							{
								boxLargeSize += readBuf[i];
								boxLargeSize = boxLargeSize << 8;
							}
							boxLargeSize += readBuf[7];
							seekLen = boxLargeSize - 4;
						}
						else
						{
							break;
						}
					}
					else if (boxSize == 0)
					{
						break;
					}
					else
					{
						mp4File.Read(readBuf, 4);
						if (memcmp(readBuf, "moov", sizeof(char) * 4) == 0)
						{
							seekLen = 0;
							continue;
						}
						else if (memcmp(readBuf, "mvhd", sizeof(char) * 4) == 0)
						{
							mp4File.Seek(12, CFile::current);
							mp4File.Read(readBuf, 4);
							for (int i = 0; i < 3; i++)
							{
								timeScale += readBuf[i];
								timeScale = timeScale << 8;
							}
							timeScale += readBuf[3];
							mp4File.Read(readBuf, 4);
							for (int i = 0; i < 3; i++)
							{
								duration += readBuf[i];
								duration = duration << 8;
							}
							duration += readBuf[3];
							break;
						}
						else
						{
							seekLen = boxSize - 8;
						}
					}
				}
			}
		}
		mp4File.Close();
		if (duration > 0 && timeScale > 0)
		{
			double dDuration = duration / timeScale * 1000;
			return (long)dDuration;
		}
	}
	else
	{
		LONGLONG duration = 0;
		IGraphBuilder *pigb;
		IMediaControl *pimc;
		IMediaEventEx *pimex;
		IBasicAudio *piba;
		IMediaSeeking *pims;
		IBaseFilter *pibf;
		if (SUCCEEDED(CoCreateInstance( CLSID_FilterGraph,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder,
			(void **)&pigb)))
		{
			pigb->QueryInterface(IID_IMediaControl, (void **)&pimc);
			pigb->QueryInterface(IID_IMediaEventEx, (void **)&pimex);
			pigb->QueryInterface(IID_IBasicAudio, (void**)&piba);
			pigb->QueryInterface(IID_IMediaSeeking, (void**)&pims);

			USES_CONVERSION;
			HRESULT hr = pigb->RenderFile(A2W(strPath), NULL);
			if (SUCCEEDED(hr))
			{
				if (pims)
				{
					pims->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
					pims->GetDuration(&duration); // returns 10,000,000 for a second.
				}
			}
		}
		if (pigb)
		{
			pigb->Release();
			pigb = NULL;
		}
		if (pimc)
		{
			pimc->Release();
			pimc = NULL;
		}
		if (pimex)
		{
			pimex->Release();
			pimex = NULL;
		}
		if (piba)
		{
			piba->Release();
			piba = NULL;
		}
		if (pims)
		{
			pims->Release();
			pims = NULL;
		}
		return (long)(duration / 10000);
	}
	return 0;
#endif
}

BOOL CMainFrame::IsArrayDifferent(CStringArray& strArray1, CStringArray& strArray2)
{
	if (strArray1.GetSize() != strArray2.GetSize())
		return TRUE;

	for (int i = 0; i < strArray1.GetSize(); i++)
		if (strArray1[i] != strArray2[i])
			return TRUE;

	return FALSE;
}

BOOL CMainFrame::IsCourseVideoModified(CString strCourse, BOOL* pbOnlyTagModified)
{
	if (pbOnlyTagModified)
		*pbOnlyTagModified = FALSE;

	CStringArray strArrayData, strArrayPreData;
	CString strSrcPath = MACRO_RECORD_VIDEO(strCourse);
	CString strDestPath = MACRO_PRE_RECORD_VIDEO(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;
	
	strSrcPath = MACRO_RECORD_AUDIO_MIX(strCourse);
	strDestPath = MACRO_PRE_RECORD_AUDIO_MIX(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_AUDIO_REPLACE(strCourse);
	strDestPath = MACRO_PRE_RECORD_AUDIO_REPLACE(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_VIDEO_TEXT(strCourse);
	strDestPath = MACRO_PRE_RECORD_VIDEO_TEXT(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_VIDEO_EFFECT(strCourse);
	strDestPath = MACRO_PRE_RECORD_VIDEO_EFFECT(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
		return TRUE;

	strSrcPath = MACRO_RECORD_TAG(strCourse);
	strDestPath = MACRO_PRE_RECORD_TAG(strCourse);
	strArrayData.RemoveAll();
	strArrayPreData.RemoveAll();
	if (::FileExists(strSrcPath))
		::LoadFile(strSrcPath, strArrayData, TRUE);
	if (::FileExists(strDestPath))
		::LoadFile(strDestPath, strArrayPreData, TRUE);
	if (IsArrayDifferent(strArrayData, strArrayPreData))
	{
		if (pbOnlyTagModified)
			*pbOnlyTagModified = TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL CMainFrame::GetPRXType(CString strPath, long& lAudio)
{
	PROFILEPARAMS param;

	CComBSTR bstrPath(strPath);


	lAudio = param.ainfo.dwFormatTag;
	return TRUE;
}

BOOL CMainFrame::GetPRXParams(CString strPath, PROFILEPARAMS& param)
{
	CComBSTR bstrPath(strPath);

	return TRUE;
}

BOOL CMainFrame::GenWMAForMute(long lmsDuration, CString strProfile, CString strDestPath)
{
	CComBSTR bstrProfilePath(strProfile);
	CComBSTR bstrDestPath(strDestPath);

	if (NULL != m_pVideoConvert)
	{
		HRESULT hr = m_pVideoConvert->GenerateM4aMuteFile(lmsDuration, bstrProfilePath, bstrDestPath);
	if (FAILED(hr))
		return FALSE;
	}
	return TRUE;
}

CString CMainFrame::GetProfileFromID(long lID)
{
	if (lID == CODEC_AUDIO_MSAUDIO)
		return "Windows Media Audio V8";
	else if (lID == CODEC_AUDIO_ACELP)
		return "ACELP.net";
	else if (lID == CODEC_VIDEO_SCREENCAP)
		return "Windows Media Screen V8";
	else if (lID == CODEC_VIDEO_CAMERACAP)
		return "WMV3";
	//else if (lID == CODEC_VIDEO_MPEG43)
	//	return "Windows Media MPEG-4 Video V3";


	CString strTmp;
	strTmp.Format("%ld", lID);
	return strTmp;
}

BOOL CMainFrame::GetTwoSideImages(CString strSrcPath, long lTime, CString strPrevPath, CString strNextPath)
{
	CComBSTR bstrSrcPath(strSrcPath);
	CComBSTR bstrPrevPath(strPrevPath);
	CComBSTR bstrNextPath(strNextPath);

	return TRUE;
}

BOOL CMainFrame::GetPrevImages(CString strSrcPath, long lTime, CString strPrevPath)
{
	CComBSTR bstrSrcPath(strSrcPath);
	CComBSTR bstrPrevPath(strPrevPath);

	return TRUE;
}

BOOL CMainFrame::GetNextImages(CString strSrcPath, long lTime, CString strNextPath)
{
	CComBSTR bstrSrcPath(strSrcPath);
	CComBSTR bstrNextPath(strNextPath);

	return TRUE;
}

BOOL CMainFrame::IsSameProfile(CString strSrc, CString strDest)
{
	CComBSTR bstrSrc(strSrc);
	CComBSTR bstrDest(strDest);


	
	return TRUE;
}

BOOL CMainFrame::ConvertToSameProfile(CString strInputFile, CString strOutputPath, CString strProfilePath)
{
	CString strCourse = GetCourse();
	if (strCourse == "")
		return FALSE;

	if (!::FileExists(strProfilePath))
	{
		CString strTmp, strMsg;
		strTmp.LoadString (IDS_FAIL_TO_FIND_PROFILE);
		strMsg.Format(strTmp, strCourse);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE;
	}

	int nIndex = strInputFile.ReverseFind('.');
	if (nIndex < 0)
		return FALSE;
	CString strPostfix = strInputFile.Mid(nIndex + 1);
	strPostfix.MakeLower();

	CComBSTR bstrOutputPath(strOutputPath);
	CComBSTR bstrProfilePath(strProfilePath);
	CComBSTR bstrInputFile(strInputFile);

	CString strTitle;
	strTitle.LoadString(IDS_CONVERTING_VIDEO);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();

	HRESULT hr;
	if (strPostfix == "asf" || strPostfix == "mp4")
	{

	}
	else
		hr = m_pMediaConvert->Media2WMV(bstrInputFile, bstrProfilePath, bstrOutputPath);

	if (FAILED(hr))
	{
		W_SHOW_ERROR(IDS_FAIL_TO_CONVERT_TO_SAME_PROFILE, this);
		HideProgress();
		return FALSE;
	}

	HideProgress();
	return TRUE;
}

BOOL CMainFrame::GenScreenProfile(CString strCourse)
{
	HRESULT hval = 0;

	CString strIniPath = MACRO_INI_FILE(strCourse);

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

	CString strProfilePath =  MACRO_COURSE_PATH + strCourse + "\\video\\screen.prx";
	CComBSTR bstrProfile = strProfilePath;


	if (FAILED(hval)) 
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_GEN_PROFILE);
		strMsg.Format(strTmp, strProfilePath);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE; 
	}

	return TRUE;
}

BOOL CMainFrame::GenVideofile(CString strCourse)
{
	HRESULT hval = 0;

	CString strIniPath = MACRO_INI_FILE(strCourse);

	char szBuf[256];
	::GetPrivateProfileString("Profile", "Dimension", "240 x 180", szBuf, 256, strIniPath);
	int nWidth, nHeight;
	sscanf(szBuf, "%d x %d", &nWidth, &nHeight);
	
	::GetPrivateProfileString("Profile", "Param1", "32", szBuf, 256, strIniPath);
	int nVideoBitRate = atoi(szBuf) * 1024;

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
	pfparam.sinfo.dwBitrate = (DWORD)0;

	CString strProfilePath =  MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
	CComBSTR bstrProfile = strProfilePath;


	if (FAILED(hval)) 
	{
		CString strTmp, strMsg;
		strTmp.LoadString(IDS_FAIL_TO_GEN_PROFILE);
		strMsg.Format(strTmp, strProfilePath);
		W_SHOW_ERROR_STR(strMsg, this);
		return FALSE; 
	}

	return TRUE;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{

  	switch(nIDEvent)
	{
        case ValueStartWizardTimerID:
		{
           this->ShowWindow(SW_HIDE);
		 //  ((CELearningView*)GetActiveView())->ShowWindow(SW_HIDE);
          ((CELearningView*)GetActiveView())->doWizard();

         
        }
		break;
    }


}
void  CMainFrame::Kill_Timer()
{
  KillTimer(ValueStartWizardTimerID);
}
void CMainFrame::Set_Timer() 
{
   SetTimer(ValueStartWizardTimerID,5000,NULL);  
   int i=0;
}

int CMainFrame::NasUpCourse()
{
	//add 2014/6/12 kagata outemon
	//コース生成後にNASへコピー処理
	m_pCEApp = (CELearningApp*)this;
	CString strNasDir = m_pCEApp->GetNasDir();
	int m_dRecUp = m_pCEApp->ChkNasUpRec();
	int m_dNasUp = m_pCEApp->GetUpCompInfo();
	if(m_dRecUp == 1 && strNasDir != "" && m_dNasUp == 1)
	{
		CString strTitle  = "";
#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif
		SetWindowText(strTitle);
		int m_dNetAccess = PathIsDirectory(strNasDir);
		if(!m_dNetAccess)
			m_dNetAccess = m_pCEApp->NasDirRetry(strNasDir, 59496);

		if(m_dNetAccess)
		{
			CString m_strNasCourse = m_strCourse;
			//コース名確認
			int e = 0;
			do
			{
				e = m_pCEApp->NasNameCheck(strNasDir, m_strCourse);
				if(e == -1)
					m_strNasCourse = GetCourse();
					m_strCourse = m_strNasCourse;		//add 2014/8/8 kagata outemon
				if(e == -2){		//add 2014/8/7 kagata outemon
					LoadCourse(m_strNasCourse,0,1,0);	//replase 2014/8/8 kagata outemon
					m_strCourse = m_strNasCourse;		//add 2014/8/8 kagata outemon
				}
			} while(e == -1 || e == -2);

			//フラグがtrueの場合
			//m_pCEApp->NasCourse(strNasDir + "\\TEMP\\", MACRO_COURSE_PATH + m_strNasCourse, 0);

			//TEMPから本フォルダへ
			//第3引数が1=元ファイル削除、0=元ファイル残す
			//int d = m_pCEApp->NasCourse(strNasDir, strNasDir + "\\TEMP\\" + m_strNasCourse, 1);
			int d = m_pCEApp->NasCourse(strNasDir, MACRO_COURSE_PATH + m_strNasCourse, 0);
			
			//コピー先とローカルでのファイル名チェック
			d = m_pCEApp->Chkreload(strNasDir + "\\" + m_strNasCourse, MACRO_COURSE_PATH + m_strNasCourse);

			if(d >= 0){
				//add 2014/7/30 kagata outemon chiba
				LoadCourse("");
				BeginWaitCursor();
				Sleep(1000);
				ConImeCut();
				Sleep(1000);

				::DelTree(MACRO_COURSE_PATH + m_strNasCourse);
				AfxMessageBox(59495);
			}else{
				//コピー失敗時再試行ダイアログの表示
				CString Renas;
				Renas.LoadString(59499);
				int ret;

				//再試行ダイアログ
				ret = AfxMessageBox(Renas,MB_RETRYCANCEL);
				if(ret == IDRETRY){
					//値を返して再実行
					return d;
				}else{
					//値を返して即終了
					CollectCourse("");
					LoadCourse("");
					return d;
				}
			}

			//コースを無名でリロードする必要あり
			CollectCourse("");
			LoadCourse("");
			return d;
		}
	}
	
	return 1;
}

//add 2014/7/31 kagata outemon chiba
void CMainFrame::ConImeCut()
{
	// ハンドルの取得
	HANDLE hSnap;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// ハンドルがエラー値の場合
	if (hSnap == INVALID_HANDLE_VALUE) {
	    return;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	UINT len = 0;

	// 全てのプロセスをチェックする
	BOOL bResult = Process32First(hSnap, &pe);
	while(bResult){
	    // 次を取得
	    bResult = Process32Next(hSnap, &pe);
	    // conimeでプロセスIDが同じ場合
	    if (!strcmp(pe.szExeFile, "conime.exe")) {
	        // プロセスIDからハンドルの取得
	        HANDLE hProcess = OpenProcess(
	                       PROCESS_ALL_ACCESS,
	                       FALSE,
	                       pe.th32ProcessID);
	
	        // conimeを破棄
	        TerminateProcess(hProcess, 0);
	
	        // プロセスを閉じる
	        DWORD dwExitCode = 0;
	        GetExitCodeProcess(hProcess, &dwExitCode);
	        CloseHandle(hProcess);	
	    }
	}

	CloseHandle(hSnap);
}
//add by buxiangyi
void CMainFrame::OnExit()
{
	OnClose();
}
/////////////////////////////

void CMainFrame::MakeDeviceSetInfoFile(CString deviceSetInfoFileDir)
{
	::WritePrivateProfileString("RecordingSetting", "VideoBitRateMin", "256k", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "VideoBitRateMax", "2048k", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "VideoBitRateStep", "128k", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "FrameRateMin", "1", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "FrameRateMax", "8", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "FrameRateStep", "1", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "KeyFrameMin", "1sec", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "KeyFrameMax", "8sec", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "KeyFrameStep", "1sec", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "AudioBitRateMin", "64k", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "AudioBitRateMax", "384k", deviceSetInfoFileDir);
	::WritePrivateProfileString("RecordingSetting", "AudioBitRateStep", "16k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "VideoBitRateMin", "256k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "VideoBitRateMax", "2048k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "VideoBitRateStep", "128k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "FrameRateMin", "1", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "FrameRateMax", "20", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "FrameRateStep", "1", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "KeyFrameMin", "1sec", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "KeyFrameMax", "8sec", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "KeyFrameStep", "1", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "AudioBitRateMin", "32k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "AudioBitRateMax", "256k", deviceSetInfoFileDir);
	::WritePrivateProfileString("ScreenCapture", "AudioBitRateStep", "16k", deviceSetInfoFileDir);
}

BOOL CMainFrame::AudioFix(long nAuFixValue)
{
	CString strCoursePath = MACRO_COURSE_PATH + GetCourse();
	CComBSTR bstrIn(strCoursePath + "\\video\\default.mp4");
	CComBSTR bstrOut(strCoursePath + "\\video\\default_repair.mp4");

	CString strProfilePath = strCoursePath + "\\Course.ini";
	//CComBSTR bstrProfile(strProfilePath);

	CString strTitle, strMsg, strTmp;
	strTitle.LoadString(IDS_CONVERT_FROM_ASF);
	strMsg.LoadString(IDS_PROCESSING);
	SetProgress(strTitle, strMsg);
	ShowProgress();
	
	if (NULL != m_pVideoConvert)
	{
		if (nAuFixValue != 0)
		{
			MACRO_VIEW->UnLoadVideo();
			HRESULT hr = m_pVideoConvert->ConvertByBase(bstrIn, bstrIn, bstrOut, nAuFixValue);
			if (FAILED(hr))
			{
				MACRO_VIEW->ReLoadVideo();
				HideProgress();
				return FALSE;
			}
			CString strAuFixValue;
			strAuFixValue.Format("%d", nAuFixValue);
			::WritePrivateProfileString("repair", "FLAG", "1", strProfilePath);
			::WritePrivateProfileString("repair", "TIMESHIFT", strAuFixValue, strProfilePath);
		}
		else
		{
			::WritePrivateProfileString("repair", "FLAG", "0", strProfilePath);
			::WritePrivateProfileString("repair", "TIMESHIFT", "0", strProfilePath);
		}
	}

	HideProgress();
	LoadCourse(TRUE);
	return TRUE;
}
