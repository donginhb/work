// eLearningView.cpp : implementation of the CELearningView class
//

#include "stdafx.h"
#include "eLearning.h"

#include "eLearningDoc.h"
#include "eLearningView.h"
#include "adapter.h"
#include "Macro.h"
#include "Mainfrm.h"
#include "cmkapi.h"
#include "DlgNewCourse.h"
#include "DlgInputCourse.h"
#include "FileApi.h"
#include "MediaUtils.h"
#include "ProgressWnd.h"
#include "DlgSelDevice.h"
#include "CaptureDevApi.h"
#include "jpgapi.h"
#include "eLearningAPI.h"
#include "SheetOption.h"
#include "dlgProgress.h"
#include "DlgSelectCourse.h"
#include "SheetDefaultLayout.h"
#include "DlgBackupCourse.h"
#include "DlgRestoreCourse.h"
#include "DlgCourseDup.h"
#include "DlgBackupMaintain.h"
#include "DlgBackupOneCourse.h"
#include "DlgCheckOut.h"
#include "DlgRegister.h"
#include "DlgUseQuota.h"
#include "DlgImportCourse.h"
#include "DlgSelHTMPresent.h"
#include "DlgExportCourse.h"
#include "DlgDefaultProfileSet.h"
#include "DlgScreenProfileSet.h"
#include "DlgMP4Rec.h"
#include "DlgScreenCapture.h"
#include "DlgNewCourseFromMedia.h"
#include "TimeLinePanel.h"   //for using WM_SET_PPT_POINT
#include "DlgSelectCourseVolume.h"
#include "MasterDlg.h"
#include "Device.h"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include "SetupPage.h"
#include "PageSelPPt.h"
#include "PageSelHTML.h"
#include "PageInputName.h"
#include "PageSelDevice.h"
#include "PageSelFreq.h"
#include "PageSelLayout.h"
#include "PageSelTV.h"
#include "DlgOpenCourse.h"
#include "DlgCourseFolder.h"
#include "PageDefaultLayout.h"
#include "CourseSheetDefaultLayout.h"
#include "CourseLayout.h"
#include "registry.h"		// add itoh 2013/07/23 PPT2013
#include "encodeapi.h"		// add itoh 2013/07/23 PPT2013
//#include "TimeLineCtrl.h"		// add itoh 2013/02/01
//#include "KeyFramePanel.h"		// add itoh 2013/02/01
#include "DlgAuFixWarning.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//leu, add
extern CString GetMACAddressString();
//leu, add

/////////////////////////////////////////////////////////////////////////////
// CELearningView

IMPLEMENT_DYNCREATE(CELearningView, CView)

BEGIN_MESSAGE_MAP(CELearningView, CView)
	//{{AFX_MSG_MAP(CELearningView)
	ON_WM_SIZE()
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_RECORD, OnRecord)
	ON_COMMAND(ID_NEW_LESSON, OnNewLesson)
	ON_COMMAND(ID_IE, OnIe)
	ON_COMMAND(ID_DEFAULT_IE, OnDefaultBrowser)
	ON_COMMAND(ID_DEL_LESSON, OnDelLesson)
	ON_COMMAND(ID_DEV_SETTING, OnDevSetting)
	ON_COMMAND(ID_DEFAULT_LAYOUT, OnDefaultLayout)
	ON_COMMAND(ID_RECORD_ASF_FILE, OnRecordAsfFile)
	ON_COMMAND(ID_RENAME_COURSE, OnRenameCourse)
	ON_COMMAND(ID_SAVE_AS, OnSaveAs)
	ON_UPDATE_COMMAND_UI(ID_DEL_LESSON, OnUpdateDelLesson)
	ON_UPDATE_COMMAND_UI(ID_RENAME_COURSE, OnUpdateRenameCourse)
	ON_UPDATE_COMMAND_UI(ID_SAVE_AS, OnUpdateSaveAs)
	ON_UPDATE_COMMAND_UI(ID_RECORD, OnUpdateRecord)
	ON_UPDATE_COMMAND_UI(ID_UPLOAD_COURSE, OnUpdateUploadCourse)
	ON_COMMAND(ID_NEW_FROM_MEDIA, OnNewFromMedia)
	ON_COMMAND(ID_SCREEN_CAPTURE, OnScreenCapture)
	ON_COMMAND(ID_BKUP_COURSE, OnBkupCourse)
	ON_COMMAND(ID_RESTORE_COURSE, OnRestoreCourse)
	ON_UPDATE_COMMAND_UI(ID_BACKUP_COURSE, OnUpdateBackupCourse)
	ON_COMMAND(ID_BACKUP_COURSE, OnBackupCourse)
	ON_COMMAND(ID_DEL_COURSE, OnDelCourse)
	ON_COMMAND(ID_BACKUP_MAINTAIN, OnBackupMaintain)
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_COMMAND(ID_ONLINE_HELP, OnOnlineHelp)
	ON_UPDATE_COMMAND_UI(ID_REG_CODE, OnUpdateRegCode)
	ON_UPDATE_COMMAND_UI(ID_REGISTER, OnUpdateRegister)
	ON_UPDATE_COMMAND_UI(ID_NEW_FROM_MEDIA, OnUpdateNewFromMedia)
	ON_COMMAND(ID_COURSE_LOADER, OnCourseLoader)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_HELP, OnUpdateOnlineHelp)
	ON_UPDATE_COMMAND_UI(ID_NEW_LESSON, OnUpdateNewLesson)
	ON_UPDATE_COMMAND_UI(ID_RECORD_ASF_FILE, OnUpdateRecordAsfFile)
	ON_UPDATE_COMMAND_UI(ID_SCREEN_CAPTURE, OnUpdateScreenCapture)
	ON_UPDATE_COMMAND_UI(ID_DEV_SETTING, OnUpdateDevSetting)
	ON_UPDATE_COMMAND_UI(ID_DEFAULT_LAYOUT, OnUpdateDefaultLayout)
	ON_UPDATE_COMMAND_UI(ID_SETTING, OnUpdateSetting)
	ON_COMMAND(ID_CHECK_OUT, OnCheckOut)
	ON_UPDATE_COMMAND_UI(ID_CHECK_OUT, OnUpdateCheckOut)
	ON_COMMAND(ID_CONTACT_US, OnContactUs)
	ON_COMMAND(ID_SERIAL_REUSE, OnSerialReuse)
	ON_UPDATE_COMMAND_UI(ID_SERIAL_REUSE, OnUpdateSerialReuse)
	ON_COMMAND(ID_BUY_QUOTA, OnBuyQuota)
	ON_COMMAND(ID_USE_QUOTA, OnUseQuota)
	ON_UPDATE_COMMAND_UI(ID_COURSE_LOADER, OnUpdateCourseLoader)
	ON_COMMAND(ID_IMPORT_COURSE, OnImportCourse)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_COURSE, OnUpdateImportCourse)
	ON_COMMAND(ID_HTML_PRESENT, OnHtmlPresent)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_PROFILE_SETTING, OnProfileSetting)
	ON_COMMAND(ID_SCREEN_PROFILE_SETTING, OnScreenProfileSetting)
	ON_COMMAND(ID_UNDO, OnUndo)
	ON_COMMAND(ID_REDO, OnRedo)
	ON_UPDATE_COMMAND_UI(ID_UNDO, OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_REDO, OnUpdateRedo)
	ON_COMMAND(ID_SAVE_VIDEO, OnSaveVideo)
	ON_UPDATE_COMMAND_UI(ID_SAVE_VIDEO, OnUpdateSaveVideo)
	ON_COMMAND(ID_BATCH_VOLUME, OnBatchVolume)
	ON_UPDATE_COMMAND_UI(ID_SCREEN_PROFILE_SETTING, OnUpdateScreenProfileSetting)
	ON_COMMAND(ID_NEW_WIZARD, OnNewWizard)
	ON_UPDATE_COMMAND_UI(ID_DEL_COURSE, OnUpdateDelCourse)
	ON_UPDATE_COMMAND_UI(ID_BKUP_COURSE, OnUpdateBkupCourse)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_COURSE, OnUpdateRestoreCourse)
	ON_UPDATE_COMMAND_UI(ID_BACKUP_MAINTAIN, OnUpdateBackupMaintain)
	ON_COMMAND(ID_OPEN, OnOpen)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_PPT, &CELearningView::OnEditPpt)
	ON_COMMAND(ID_CHANGE_PPT, &CELearningView::OnChangePpt)
	ON_COMMAND(IDB_REGEN_PPT, &CELearningView::OnRegenPpt)
	ON_COMMAND(ID_BTN_MARK, &CELearningView::OnPBMarkUp)
	ON_COMMAND(ID_BTN_DELETE, &CELearningView::OnPBDeleteUp)
	ON_COMMAND(ID_BTN_MUTE, &CELearningView::OnPBMuteUp)
	ON_COMMAND(ID_BTN_REPLACE, &CELearningView::OnPBReplaceUp)
	ON_COMMAND(ID_BTN_EDITMARK, &CELearningView::OnPBEditMarkUp)
	ON_COMMAND(ID_BTN_ADD_EVENT, &CELearningView::OnPBAddEventUp)
	ON_COMMAND(ID_ADD_ZOOM_IN, &CELearningView::OnPBZoomInUp)
	ON_COMMAND(ID_ADD_ZOOM_OUT, &CELearningView::OnPBZoomOutUp)
	ON_COMMAND(ID_ADD_HTML, &CELearningView::OnPBAddHtmlUp)
	ON_COMMAND(ID_ADD_SCREEN_CAPTURE, &CELearningView::OnPBAddSCUp)
	ON_COMMAND(ID_BTN_SAVE_VIDEO, &CELearningView::OnPBSaveVideo)
	ON_COMMAND(ID_BTN_ADD_WAVE, &CELearningView::OnPBAddWave)
	ON_COMMAND(ID_ADD_AUDIO_REPLACE, &CELearningView::OnPBAddAR)
	ON_COMMAND(ID_ADD_AUDIO_MIX, &CELearningView::OnPBAddAM)
	ON_COMMAND(ID_BTN_ADD_TEXT, &CELearningView::OnPBAddText)
	ON_COMMAND(ID_BTN_ADD_EFFECT, &CELearningView::OnPBAddEffectUp)
	ON_COMMAND(ID_BTN_SET_POINTS, &CELearningView::OnPBSetPoints)
	ON_COMMAND(ID_BTN_ADJUST, &CELearningView::OnPBAdjustUp)
	ON_COMMAND(ID_SAVE_FILE, &CELearningView::OnSaveFile)
	ON_COMMAND(ID_COURSE_HELP, &CELearningView::OnHelp)
	ON_COMMAND(ID_COURSE_DEFAULT_LAYOUT, &CELearningView::OnCourseDefaultLayout)
	ON_UPDATE_COMMAND_UI(ID_COURSE_DEFAULT_LAYOUT, &CELearningView::OnUpdateCourseDefaultLayout)
	ON_COMMAND(ID_COURSE_PAGE_DEFAULT_LAYOUT, &CELearningView::OnCoursePageDefaultLayout)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PPT, OnUpdateEditPPT)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_PPT, OnUpdateChangePPT)
	ON_UPDATE_COMMAND_UI(IDB_REGEN_PPT, OnUpdateRegenPPT)
	ON_COMMAND(ID_BTN_AUFIX, OnAudioFix)
	ON_UPDATE_COMMAND_UI(ID_BTN_AUFIX, OnUpdateAudioFix)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELearningView construction/destruction

CELearningView::CELearningView()
{
	m_pAdapter = NULL;
}

CELearningView::~CELearningView()
{
	delete m_pAdapter;
}

BOOL CELearningView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CELearningView drawing

void CELearningView::OnDraw(CDC* pDC)
{
	CELearningDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CELearningView diagnostics

#ifdef _DEBUG
void CELearningView::AssertValid() const
{
	CView::AssertValid();
}

void CELearningView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CELearningDoc* CELearningView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CELearningDoc)));
	return (CELearningDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CELearningView message handlers


void CELearningView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	CAdapter *pAdapter = new CAdapter(this);
	CRect rcView;
	this->GetWindowRect(rcView);
	rcView = new CRect(0,0,0,0);
    if (!pAdapter->Create(WS_VISIBLE | WS_CHILD, rcView, this))
	{
		delete pAdapter;
		pAdapter = NULL;
		return;
	}
	m_pAdapter = pAdapter;
}

BOOL CELearningView::SaveCourse(CString strCourse, BOOL bAsk)
{
	if (m_pAdapter)
		return m_pAdapter->SaveCourse(strCourse, bAsk);
	return TRUE;
}

void CELearningView::LoadCourse(CString str, BOOL bForce)
{
	if (str != "")
	{
		CString strIniPath = MACRO_INI_FILE(str);
		m_nCourseType = ::GetPrivateProfileInt("General", "CourseType", 0, strIniPath);
	}
	LoadPPTFile(str);
	if (m_pAdapter)
		m_pAdapter->LoadCourse(str, bForce);
}

BOOL CELearningView::GetSlideFileName(CString strIndex, CString strCmd, CString& strPath)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	CString strCoursePath = MACRO_COURSE_PATH + strCourse + "\\";
	if (strCmd == "flippage")
		strPath = strCoursePath + "webpages\\" + strIndex + "\\index.htm";
	else if (strCmd == "flipslide")
	{

		int iLen = strIndex.GetLength();
		int iFind = strIndex.Find('@');
		int iSlide, iAnima;
		if (iFind > 0)
		{
			iSlide = atoi(strIndex.Left(iFind));
			iAnima = atoi(strIndex.Right(iLen - iFind - 1));
		}
		else
		{
			iSlide = atoi(strIndex);
			iAnima = 0;
		}
		changeHTML(iSlide, iAnima);
		strPath = MACRO_HTML_DIR(strCourse) + "viewer.html";

	}

	return (::FileExists(strPath));
}

//Preparation of the character string which replaces.
//Argument: (Slide number, Animation number)
void CELearningView::changeHTML(int sNo, int aNo)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == " ")
	{
		return;
	}
	CString strHtmlPath = MACRO_HTML_DIR(strCourse);
	CString strSrcPath = strHtmlPath + "temp_viewer.html";
	CString strnewSrcPath = strHtmlPath + "viewer.html";

	FILE *streamin;
	if ((streamin = fopen((LPCTSTR)strSrcPath, "r")) == NULL)
		return;

	char* pline = new char[4096];
	if (fgets(pline, 4096, streamin) == NULL)
	{
		delete[] pline;
		fclose(streamin);
		return;
	}

	CStringArray strArrayTag;
	CStringArray strArrayReplace;
	while (fgets(pline, 4096, streamin) != NULL)
	{
		if(strncmp(pline, "				msg = \"jumpToAnimation:1:0:0\"", 33) == 0)
		{
			CString first;
			first.Format(_T("%d"), sNo);
			CString second;
			second.Format(_T("%d"), aNo);

			CString NewWord = _T("				msg = \"jumpToAnimation:") + first + _T(":") + second + _T(":0\"\n");
			strArrayTag.Add(pline);
			strArrayReplace.Add(NewWord);
		}
	}
	
	delete[] pline;
	fclose(streamin);

	//Argument: (temp_viewer.html File path,
	//			 viewer.html File path,
	//			 The original character string,
	//			 The replaced character string)
	replaceHTML(strSrcPath, strnewSrcPath, strArrayTag, strArrayReplace, false);

	return;
}

//Insertion of the character string which replaced.
//Argument: (temp_viewer.html File path,
//			 viewer.html File path,
//			 The original character string,
//			 The replaced character string)
void CELearningView::replaceHTML(CString destpath, CString newpath, CStringArray& strsTag, CStringArray& strsReplace, bool bReplaceToLast)
{
	FILE *stream_in;
	if ((stream_in = fopen((LPCTSTR)destpath, "r" )) == NULL)
		return;

	FILE *stream_out;
	if ((stream_out = fopen((LPCTSTR)newpath, "w+" )) == NULL)
	{
		fclose(stream_in);
		return;
	}

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		fclose(stream_out);
		return;
	}

	BOOL bUpdate = FALSE;
	CString strBuf = pline;
	while (TRUE)
	{
		for (int i = 0; i < strsTag.GetSize(); i++)
		{
			CString strFind = strsTag[i];
			CString strReplace = strsReplace[i];
			int nIndex = strBuf.Find(strFind);
			if (nIndex >= 0)
			{
				CString strRemain = strBuf.Mid(nIndex + strFind.GetLength());
				strBuf = strBuf.Left(nIndex) + strReplace;
				if (!bReplaceToLast) 
					strBuf += strRemain;
				else
					strBuf += "\n";

				bUpdate = TRUE;
			}
		}

		if (fputs(strBuf, stream_out) == EOF)
		{
			//bRet = FALSE;
			break;
		}
		
		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		strBuf = pline;
	}

	fclose(stream_in);
	fclose(stream_out);
	delete [] pline;

	return;
}

void CELearningView::UnLoadVideo()
{
	if (m_pAdapter)
		m_pAdapter->UnLoadVideo();
}

void CELearningView::ReLoadVideo()
{
	if (m_pAdapter)
		m_pAdapter->ReLoadVideo();
}

void CELearningView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_pAdapter != NULL)
		m_pAdapter->Resize();
}

BOOL CELearningView::IsLocked(CString strSrcPath)
{
	if (strSrcPath.Right(1) == "\\")
		strSrcPath = strSrcPath.Left(strSrcPath.GetLength() - 1);
	CString strDestPath = strSrcPath + "Tmp";
	if (::FileExists(strDestPath))
		::DelTree(strDestPath);
	if (rename(strSrcPath, strDestPath) != 0)
		return TRUE;
	if (rename(strDestPath, strSrcPath) != 0)
		return TRUE;
	return FALSE;
}

void CELearningView::SubmitHTMLLesson(CString strCourse) 
{
	strCourse.TrimLeft();
	if (strCourse == "")
		return;

	CString strTmplate = MACRO_TEMPLATE_PATH;
	CString strCoursePath = MACRO_COURSE_PATH + strCourse + "\\";
	CString strHTML = strCoursePath + "html\\";
	if (!::SafeCopyFile(strTmplate + "default.htm", strCoursePath + "default.htm", FALSE))
		return;

	if (LayoutFilesNotExists(strCourse))
		CopyLayoutFiles(strCourse);

	CopyFunctionalFiles(strCourse);

	CStringArray strArrayTag, strArrayReplace;
	strArrayTag.Add("<title>%TITLE%</title>");
	strArrayReplace.Add("<title>" + strCourse + "</title>");
	::UpdateFileByTag(strHTML, "_frame.htm", strArrayTag, strArrayReplace, FALSE);

	if (!MACRO_MAINFRM->DumpKeyAndCreateIndex(strCourse))
	{
		W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
		return;
	}

	CString strMediaPath = strCoursePath + "html\\media.lrn";
	::DeleteFile(strMediaPath); //Cannot be deleted in MCGcapture, why?

	SetFocus();
}

void CELearningView::SubmitLesson(CString strCourse, BOOL bWithMedia) 
{
	strCourse.TrimLeft();
	if (strCourse == "")
		return;

	CString strWarning1, strWarning2;

	CString strHtmDir = "";
	BOOL bPPTHTMFound = FALSE;
	CStringArray strsDir;
	CStringArray strsHtm;

	BOOL bOldHTML = FALSE;
	CString strCoursePath = MACRO_COURSE_PATH + strCourse + "\\";
	CString strHTML = strCoursePath + "html\\";
//	CString strPPT = strCoursePath + "ppt\\";	// del itoh 2013/07/04 PPT2013
//	if (::FileExists(strPPT))
//	{
//		if (!::DelTree(strPPT))
//		{
//			CString strTmp, strMsg;
//			strTmp.LoadString(IDS_FAIL_TO_DEL_TREE);
//			strMsg.Format(strTmp, strPPT);
//			W_SHOW_ERROR_STR(strMsg, this);
//		}
//	}

	if (::FileExists(strHTML))
	{
/* del itoh 2013/07/25 PPT2013
		if (IsLocked(strHTML))
		{
			CString strMsg, strTmp;
			strTmp.LoadString(IDS_PATH_LOCKED);
			strMsg.Format(strTmp, strHTML);
			W_SHOW_ERROR_STR(strMsg, this);
			return;
		}
*/
		bOldHTML = TRUE;
	}

	if (::GetDir(strCoursePath, strsDir))
	{
		for (int i = 0; i < strsDir.GetSize(); i++)
		{
			CString strDir = strsDir[i];
//			if (strDir.CompareNoCase("html") == 0)	// del itoh 2013/07/04 PPT2013
			if (strDir.CompareNoCase("html") != 0)
				continue;

			CString strPath = strCoursePath + strDir;
//			if (::FileExists(strPath + "\\pres.xml"))	// del itoh 2013/07/04 PPT2013
			if (::FileExists(strPath + "\\setting.js"))
			{
//				rename(strPath + "\\frame.htm", strPath + "\\oldframe.htm");

//				if (rename(strPath, strPPT) != 0)
//				{
//					strWarning1.LoadString(IDS_RENAME_DIR);
//					strWarning2.Format(strWarning1, strDir);
//					W_SHOW_ERROR_STR(strWarning2, this);
//					return;
//				}

//				if (!::CopyDir(strPPT, strHTML))
//					return;			

//				if (!ModifyPPTFiles(strPPT))
//				if (!ModifyPPTFiles(strHTML))
//					return;
				
				bPPTHTMFound = TRUE;
			}
		}
		
		if (!bPPTHTMFound)
		{
			W_SHOW_ERROR(IDS_NO_PPT_FILE, this);
			return;
		}
	}
	else
	{
		W_SHOW_ERROR(IDS_NO_PPT_FILE, this);
		return;
	}
/* del itoh 2013/07/04 PPT2013
	if (::GetFiles(strCoursePath, "htm", strsHtm))
	{
		if (strsHtm.GetSize() == 0)
		{
			W_SHOW_ERROR(IDS_NO_PPT_FILE, this);
			return;
		}
		else if (strsHtm.GetSize() > 1)
		{
			W_SHOW_ERROR(IDS_PPT_ERR, this);
			return;
		}
	}
	else
	{
		W_SHOW_ERROR(IDS_NO_PPT_FILE, this);
		return;
	}

	CString strHtmFile = strsHtm[0];

	CString strOldDefaultHtm = strCoursePath + strHtmFile;
*/
	CString strTmplate = MACRO_TEMPLATE_PATH;
	CString strIniFile = MACRO_INI_FILE(strCourse);

	::WritePrivateProfileString("General", "PPTVersion", "10", strIniFile);
/* del itoh 2013/07/04 PPT2013
	int nPPTVer = GetPPTVersion(strOldDefaultHtm);
	if (nPPTVer >= 10)
	{
		if (!::SafeCopyFile(strTmplate + "_ppt10.js", strHTML + "_pptver.js", FALSE))
			return;
		::WritePrivateProfileString("General", "PPTVersion", "10", strIniFile);
	}
	else
	{
		if (!::SafeCopyFile(strTmplate + "_ppt9.js", strHTML + "_pptver.js", FALSE))
			return;
		::WritePrivateProfileString("General", "PPTVersion", "9", strIniFile);
	}

	::DeleteFile(strOldDefaultHtm);
*/
	if (!::SafeCopyFile(strTmplate + "html\\default.htm", strCoursePath + "default.htm", FALSE))
		return;

// del itoh 2013/7/04 PPT2013
	CStringArray strsTag, strsReplace;			// replace kagata 2013/9/10 PPT2013
//	strsTag.RemoveAll();
//	strsReplace.RemoveAll();
//	strsTag.Add("var g_isKiosk = 0;");
//	strsReplace.Add("var g_isKiosk = 1;");
//	strsTag.Add("g_allowAdvOnClick = true");
//	strsReplace.Add("g_allowAdvOnClick = false");
//	strsTag.Add("function IsFramesMode()");
//	strsReplace.Add("function IsFramesMode(){return true;} //");
//	strsTag.Add("function UpdNtsPane()");
//	strsReplace.Add("function UpdNtsPane(){} //");
//	strsTag.Add("if( PPTSld.event ) PPTSld.event.cancelBubble=true");
//	strsReplace.Add(";//if( PPTSld.event ) PPTSld.event.cancelBubble=true");
	//shien Add 20050418 PowerPoint Slide §£Ød∂°∂Z
//	strsTag.Add("padding=6");
//	strsReplace.Add("padding=0");

	BOOL bRet;			// add kagata 2013/9/10 PPT2013
//	BOOL bRet = UpdateFileByTag(strHTML, "script.js", strsTag, strsReplace);
//	if (!bRet)
//	{
//		strWarning1.LoadString(IDS_UPDATE_FILE);
//		strWarning2.Format(strWarning1, "script.js");
//		W_SHOW_ERROR_STR(strWarning2, this);
//		return;
//	}

//	del kagata 2013/9/09 PPT2013
/*	CStringArray strArrayHTMLFiles;
	::GetFiles(strHTML, "htm", strArrayHTMLFiles);
	for (int i = 0; i < strArrayHTMLFiles.GetSize(); i++)
	{
		CString strFileName = strArrayHTMLFiles[i]; 
		if (IsOurFile(strFileName))
		{
			continue;
		}

		strsTag.RemoveAll();
		strsReplace.RemoveAll();
		strsTag.Add("<body lang");
		strsReplace.Add("<script src=_outlook.js></script><script src=_option.js></script><SCRIPT LANGUAGE=\"JavaScript\">	document.write('<body ONDRAGSTART=""window.event.returnValue=false"" ONCONTEXTMENU=""window.event.returnValue=false"" onSelectStart=""event.returnValue=false"" '); if (g_bUseBGImage) document.write('background=\"_backgnd.jpg\" '); else document.write('bgcolor=\"' + g_slcl + '\" '); </script>");
 		bRet = UpdateFileByTag(strCoursePath + "html", strFileName, strsTag, strsReplace, TRUE);
		if (!bRet)
		{
			strWarning1.LoadString(IDS_UPDATE_FILE);
			strWarning2.Format(strWarning1, "htm files");
			W_SHOW_ERROR_STR(strWarning2, this);
			return;
		}
	}
*/

// add kagata 2013/9/09 PPT2013

	if (LayoutFilesNotExists(strCourse))
		CopyLayoutFiles(strCourse);
	CopyViewerFiles(strCourse);

/* del itoh 2013/07/04 PPT2013
	CStringArray strArrayTag, strArrayReplace;
	if (::FileExists(strHTML + "oldframe.htm"))
	{
		CString strOfficeXP = "<!--[if gte IE 5]>\n<object id=MSOANIM classid=\"CLSID:A4639D2F-774E-11D3-A490-00C04F6843FB\"\n";
		strOfficeXP += " codebase=\"http://download.microsoft.com/download/vizact2000/Install/10/WIN98Me/EN-US/msorun.cab#version=10,0,0,1\"></object>\n";
		strOfficeXP += "<![endif]-->\n";
		
		if (::FileContainString(strHTML + "oldframe.htm", "object id=MSOANIM"))
		{
			strArrayTag.Add("<![endif]></head>");
			strArrayReplace.Add(strOfficeXP + CString("<![endif]>\n") + "</head>\n");
		}

	}
*/
	CStringArray strArrayTag, strArrayReplace;	// add itoh 2013/07/04 PPT2013
	strArrayTag.Add("<title>%TITLE%</title>");
	strArrayReplace.Add("<title>" + strCourse + "</title>");
	::UpdateFileByTag(strHTML, "_frame.htm", strArrayTag, strArrayReplace, FALSE);

	if (!MACRO_MAINFRM->DumpKeyAndCreateIndex(strCourse))
	{
		W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
		return;
	}

	if (bWithMedia)
		ProcessAVIToASF(strCourse);
/* del itoh 2013/0704 PPT2013
	CString strIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("Option", "ConvertPNG", "0", szBuf, 256, strIniPath);
	if (atoi(szBuf))
		ProcessPNGToJPG(strCourse);
*/
	CString strMediaPath = strCoursePath + "html\\media.lrn";
	::DeleteFile(strMediaPath); //Cannot be deleted in MCGcapture, why?

	SetFocus();
}

BOOL CELearningView::ModifyPPTFiles(CString strDir)
{
	CStringArray strArrayFiles;
	::GetFiles(strDir, "htm;html", strArrayFiles);

	CStringArray strArrayTag, strArrayReplace;
//	strArrayTag.Add("gId=");
//	strArrayReplace.Add(" ");
//	strArrayTag.Add("if( !IsNts() )");
//	strArrayReplace.Add("  ");
//	strArrayTag.Add("if( !IsSldOrNts() )");
//	strArrayReplace.Add("  ");
//	strArrayTag.Add("if( MakeNotesVis() )");
//	strArrayReplace.Add("  ");
//	strArrayTag.Add("<body lang");
//	strArrayReplace.Add("<body style='margin:0px;background-color:white'");
//	strArrayTag.Add("{visibility:hidden;}</style>");	// add itoh 2013/07/04 PPT2013
//	strArrayReplace.Add("{visibility:visible;}</style>");	// add itoh 2013/07/04 PPT2013
//	for (int i = 0; i < strArrayFiles.GetSize(); i++)
//	{
//		CString strFile = strArrayFiles[i];
//		::UpdateFileByTag(strDir, strFile, strArrayTag, strArrayReplace, TRUE);
//	}

//	strArrayTag.RemoveAll();
//	strArrayReplace.RemoveAll();
//	strArrayTag.Add("var g_showAnimation =");
//	strArrayReplace.Add("var g_showAnimation = 0;var g_hasTrans = false, g_autoTrans = false, g_transSecs = 0;");
//	::UpdateFileByTag(strDir, "script.js", strArrayTag, strArrayReplace, TRUE);

	strArrayTag.RemoveAll();
	strArrayReplace.RemoveAll();
//	strArrayTag.Add("var g_isKiosk = 0;");
//	strArrayReplace.Add("var g_isKiosk = 1;");
//	strArrayTag.Add("g_allowAdvOnClick = true");
//	strArrayReplace.Add("g_allowAdvOnClick = false");
//	strArrayTag.Add("function IsFramesMode()");
//	strArrayReplace.Add("function IsFramesMode(){return true;} //");
//	strArrayTag.Add("function UpdNtsPane()");
//	strArrayReplace.Add("function UpdNtsPane(){} //");
//	::UpdateFileByTag(strDir, "script.js", strArrayTag, strArrayReplace, FALSE);

	return TRUE;
}

BOOL CELearningView::LoadPPTFile(CString strCourse)
{
	if (strCourse == "")
		return FALSE;
//	CString strPath = MACRO_COURSE_PATH + strCourse + "\\ppt\\script.js";	// replace itoh 2013/07/10 PPT2013
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\script.js";	// replace itoh 2013/07/10 PPT2013
	
	FILE *stream_in;
	if ((stream_in = fopen(strPath, "r" )) == NULL)
		return FALSE;

	char* pline = new char[4096];
	if (fgets(pline, 4096, stream_in) == NULL)
	{
		delete [] pline;
		fclose(stream_in);
		return FALSE;
	}

	BOOL bFound = FALSE;
	CString strRet = "";
	CString strBuf = pline;// + CString("\r\n");
	while (TRUE)
	{
		if (!bFound)
		{
			if (strBuf.Find("var gMainDoc=new Array(") >= 0 || strBuf.Find("var gDocTable = new Array(") >= 0)
				bFound = TRUE;
		}
		
		if (bFound)
		{
			strRet += strBuf;
			if (strBuf.Find(";") >= 0)
				break;
		}

		if (fgets(pline, 4096, stream_in) == NULL)
			break;

		strBuf = pline;// + CString("\r\n");
	}

	fclose(stream_in);
	delete [] pline;

	m_strArraySlide.RemoveAll();
	while (TRUE)
	{
		int nIndex = strRet.Find("hrefList(\"");
		if (nIndex < 0)
			break;
		strRet = strRet.Mid(nIndex + 10);
		int nIndex2 = strRet.Find("\"");
		if (nIndex2 < 0)
			break;
		CString strFile = strRet.Left(nIndex2);
		m_strArraySlide.Add(strFile);
		strRet = strRet.Mid(nIndex2);
	}

	return TRUE;
}

BOOL CELearningView::GetSlideFile(int nIndex, CString& strFile)
{
	int nSize = m_strArraySlide.GetSize();
	if (nIndex < 0 || nIndex >= nSize)
		return FALSE;

	strFile = m_strArraySlide[nIndex];
	return TRUE;
}

BOOL CELearningView::IsOurFile(CString strFileName)
{
	if (strFileName.CompareNoCase("_nso.htm") == 0 ||
		strFileName.CompareNoCase("_logo.htm") == 0 ||
		strFileName.CompareNoCase("_screencam.htm") == 0 ||
		strFileName.CompareNoCase("_frame.htm") == 0 ||
		strFileName.CompareNoCase("_demo.htm") == 0 ||
		strFileName.CompareNoCase("_demo_70.htm") == 0 ||
		strFileName.CompareNoCase("_indextmp.htm") == 0 ||
		strFileName.CompareNoCase("_video_otl.htm") == 0 ||
		strFileName.CompareNoCase("_toolbar.htm") == 0 ||
		strFileName.CompareNoCase("_toc.htm") == 0 ||
		strFileName.CompareNoCase("_practice.htm") == 0 ||
		strFileName.CompareNoCase("_largetv.htm") == 0) 
	{
		return TRUE;
	}

	return FALSE;
}

int CELearningView::GetPPTVersion(CString strPath)
{
	CStringArray strArrayText;
	::LoadFile(strPath, strArrayText);
	CString strTag = "<meta name=Generator content=\"Microsoft PowerPoint";
	CString strLine;
	for (int i = 0; i < strArrayText.GetSize(); i++)
	{
		strLine = strArrayText[i];
		int nIndex;
		if ((nIndex = strLine.Find(strTag)) >= 0)
		{
			CString strLeft = strLine.Mid(nIndex + strTag.GetLength());
			strLeft.TrimLeft();
			nIndex = strLeft.Find("\"");
			CString strVer = strLeft.Left(nIndex);
			return atoi(strVer);
		}
	}

	return -1;
}

void CELearningView::WriteOutlookName(CString strCourse)
{
	CString strIniPath = MACRO_INI_FILE(strCourse);
	char szBuf[256];
	::GetPrivateProfileString("General", "OutlookName", "", szBuf, 255, strIniPath);
	if (szBuf[0] == '\0')
	{
		CString strSysIniPath = MACRO_INI_PATH;
		::GetPrivateProfileString("General", "DefaultOutlook", "", szBuf, 255, strSysIniPath);
		::WritePrivateProfileString("General", "OutlookName", szBuf, strIniPath);
	}
}

BOOL CELearningView::LayoutFilesNotExists(CString strCourse)
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";

	CStringArray strArrayCopy;
	GetLayoutFiles(strArrayCopy);
	for (int i = 0; i < 3; i++)
	{
		CString strFile = strArrayCopy[i];
		if (!::FileExists(strHTML + strFile.Mid(strFile.Find('\\', 0) + 1)))
			return TRUE;
	}

	return FALSE;
}

BOOL CELearningView::CopyViewerFiles(CString strCourse, BOOL bFailIfExist)
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CString strTmp = MACRO_TEMPLATE_PATH + "html\\";

	CStringArray strArrayCopy;
	strArrayCopy.Add("temp_viewer.html");
	strArrayCopy.Add("viewer.html");
	for (int i = 0; i < strArrayCopy.GetSize(); i++)
	{
		CString strFile = strArrayCopy[i];
		if (!::SafeCopyFile(strTmp + strFile, 
						    strHTML + strFile,
							bFailIfExist))
		return FALSE;
	}

	return TRUE;
}

BOOL CELearningView::CopyFunctionalFiles(CString strCourse, BOOL bFailIfExist, BOOL bCopySemiFunc)
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CString strTmp = MACRO_TEMPLATE_PATH;

	CStringArray strArrayCopy;
	GetFunctionalFiles(strArrayCopy, bCopySemiFunc);
	for (int i = 0; i < strArrayCopy.GetSize(); i++)
	{
		CString strFile = strArrayCopy[i];
		if (!::SafeCopyFile(strTmp + strFile, 
						    strHTML + strFile,
							bFailIfExist))
		return FALSE;
	}
/*
	// add itoh 2013/07/08 PPT2013 Slide Count
	FILE *stream_in;
	if ((stream_in = fopen(strHTML + "convert.log", "r" )) == NULL)
		return FALSE;

	char* pline = new char[65535];
	char *slideNo; 
	int iSlideNo;
	for(i = 0;;i++) {
		if (fgets(pline, 4096, stream_in) == NULL)
		{
			delete [] pline;
			fclose(stream_in);
			return FALSE;
		}
		if(strncmp(pline,"Processing slide ",17) == 0) {
			slideNo = strstr(pline,"of ");
			iSlideNo = atoi(&slideNo[3]);
			break;
		}
	}
	fclose(stream_in);

	if ((stream_in = fopen(strHTML + "script.js", "r" )) == NULL)
		return FALSE;

	for(i = 0;;i++) {
		if (fgets(pline, 65535, stream_in) == NULL)
		{
			delete [] pline;
			fclose(stream_in);
			return FALSE;
		}
		if(strncmp(pline,"var gMainDoc=new Array(",23) == 0) {
			break;
		}
	}
	fclose(stream_in);

	CStringArray strArrayTag, strArrayReplace;
	CString strSlideNum;
	CString strSlide = "var gMainDoc=new Array(";
	for( i=1;i<=iSlideNo;i++) {
		strSlideNum.Format(_T("%d"),i);
		if(i != 1) {
			strSlide = strSlide + ",";
		}
		strSlide = strSlide + "new hrefList(\"slide" + strSlideNum + ".html\",1,-1,1)";
	}
	strSlide = strSlide + ");\n";
	strArrayTag.Add(pline);
	strArrayReplace.Add(strSlide);
	::UpdateFileByTag(strHTML, "script.js", strArrayTag, strArrayReplace, FALSE);

	delete [] pline;

	// add end itoh
*/
	return TRUE;
}

BOOL CELearningView::CopyLayoutFiles(CString strCourse)
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CString strTmp = MACRO_TEMPLATE_PATH;

	CStringArray strArrayCopy;
	GetLayoutFiles(strArrayCopy);
	for (int i = 0; i < strArrayCopy.GetSize(); i++)
	{
		CString strFile = strArrayCopy[i];
		if (!::SafeCopyFile(strTmp + strFile, 
						    strHTML + strFile.Mid(strFile.Find('\\', 0) + 1),
							TRUE))
		return FALSE;
	}

	BOOL bHTMLLOGOJPG = ::FileExists(strHTML + "logo.jpg");
	BOOL bHTMLLOGOGIF = ::FileExists(strHTML + "logo.gif");
	CStringArray strsTag;
	CStringArray strsReplace;
	if (bHTMLLOGOJPG || bHTMLLOGOGIF)
	{
		strsTag.Add("<img border=0 src=");
		CString strCourseIni = MACRO_INI_FILE(strCourse);
		char szBuf[128];
		::GetPrivateProfileString("General", "Logo", "logo.jpg", szBuf, 128, strCourseIni);
		CString strLogo = szBuf;
		if (bHTMLLOGOJPG && !bHTMLLOGOGIF)
			strsReplace.Add("<img border=0 src=\"logo.jpg\">");
		else if (!bHTMLLOGOJPG && bHTMLLOGOGIF)
			strsReplace.Add("<img border=0 src=\"logo.gif\">");
		else if (bHTMLLOGOJPG && bHTMLLOGOGIF)
		{
			strsReplace.Add("<img border=0 src=\"" + strLogo + "\">");

			if (strLogo.CompareNoCase("logo.jpg") == 0)
				::DeleteFile(strHTML + "logo.gif");
			else
				::DeleteFile(strHTML + "logo.jpg");
		}
		::UpdateFileByTag(strHTML, "_logo.htm", strsTag, strsReplace, TRUE);
	}
	else
	{
		strsTag.RemoveAll();
		strsReplace.RemoveAll();
		strsTag.Add("<img border=0 src=");
		CString strSystemIni = MACRO_INI_PATH;
		char szBuf[128];
		::GetPrivateProfileString("General", "Logo", "logo.jpg", szBuf, 128, strSystemIni);
		CString strLogo = szBuf;
		BOOL bJPGExist = ::FileExists(strTmp + "logo\\" + "logo.jpg");
		BOOL bGIFExist = ::FileExists(strTmp + "logo\\" + "logo.gif");
		if (bJPGExist && !bGIFExist)
		{
			if (!::SafeCopyFile(strTmp + "logo\\" + "logo.jpg", 
							    strHTML + "logo.jpg"))
				return FALSE;
			strsReplace.Add("<img border=0 src=\"logo.jpg\">");
		}
		else if (!bJPGExist && bGIFExist)
		{
			if (!::SafeCopyFile(strTmp + "logo\\" + "logo.gif", 
							    strHTML + "logo.gif"))
				return FALSE;
			strsReplace.Add("<img border=0 src=\"logo.gif\">");
		}
		else if (bJPGExist && bGIFExist)
		{
			if (!::SafeCopyFile(strTmp + "logo\\" + strLogo, 
							    strHTML + strLogo))
				return FALSE;
			strsReplace.Add("<img border=0 src=\"" + strLogo + "\">");

			if (strLogo.CompareNoCase("logo.jpg") == 0)
				::DeleteFile(strTmp + "logo\\" + "logo.gif");
			else
				::DeleteFile(strTmp + "logo\\" + "logo.jpg");
		}
		else
		{
			W_SHOW_ERROR(IDS_NO_DEFAULT_LOGO, this);
			return FALSE;
		}
		::UpdateFileByTag(strHTML, "_logo.htm", strsTag, strsReplace, TRUE);
	}

	return TRUE;
}

void CELearningView::GetFunctionalFiles(CStringArray& strArrayCopy, BOOL bCopySemiFunc)
{

}

void CELearningView::GetLayoutFiles(CStringArray& strArrayCopy)
{

	strArrayCopy.Add("html\\_frame.htm");
	strArrayCopy.Add("html\\default.htm");
	strArrayCopy.Add("html\\frame.htm");
	strArrayCopy.Add("js\\config.js");
	strArrayCopy.Add("js\\contents.js");
	strArrayCopy.Add("js\\draw.js");
	strArrayCopy.Add("js\\drawfunc.js");
	strArrayCopy.Add("js\\indexout.js");
	strArrayCopy.Add("js\\layout.js");
	strArrayCopy.Add("js\\layout001.js");
	strArrayCopy.Add("js\\layout002.js");
	strArrayCopy.Add("js\\layout003.js");
	strArrayCopy.Add("js\\layout004.js");
	strArrayCopy.Add("js\\layout005.js");
	strArrayCopy.Add("js\\layout006.js");
	strArrayCopy.Add("js\\layout007.js");
	strArrayCopy.Add("js\\layout008.js");
	strArrayCopy.Add("js\\layout009.js");
	strArrayCopy.Add("js\\layout010.js");
	strArrayCopy.Add("js\\layout011.js");
	strArrayCopy.Add("js\\layout012.js");
	strArrayCopy.Add("js\\layout013.js");
	strArrayCopy.Add("js\\layout014.js");
	strArrayCopy.Add("js\\layout015.js");
	strArrayCopy.Add("js\\layout016.js");
	strArrayCopy.Add("js\\layout017.js");
	strArrayCopy.Add("js\\layout018.js");
	strArrayCopy.Add("js\\layout019.js");
	strArrayCopy.Add("js\\layout020.js");
	strArrayCopy.Add("js\\layout021.js");
	strArrayCopy.Add("js\\layout022.js");
	strArrayCopy.Add("js\\layout023.js");
	strArrayCopy.Add("js\\layout999.js");
	strArrayCopy.Add("js\\mark.js");
	strArrayCopy.Add("js\\msgtext.js");
	strArrayCopy.Add("js\\player.js");
	strArrayCopy.Add("js\\timeline.js");
	strArrayCopy.Add("res\\bmp00007.png");
	strArrayCopy.Add("res\\c1.png");
	strArrayCopy.Add("res\\c2.png");
	strArrayCopy.Add("res\\c3.png");
	strArrayCopy.Add("res\\c4.png");
	strArrayCopy.Add("res\\c5.png");
	strArrayCopy.Add("res\\d1.png");
	strArrayCopy.Add("res\\d2.png");
	strArrayCopy.Add("res\\d3.png");
	strArrayCopy.Add("res\\d4.png");
	strArrayCopy.Add("res\\e1.png");
	strArrayCopy.Add("res\\e2.png");
	strArrayCopy.Add("res\\e3.png");
	strArrayCopy.Add("res\\e4.png");
	strArrayCopy.Add("res\\e5.png");
	strArrayCopy.Add("res\\e6.png");
	strArrayCopy.Add("res\\f1.png");
	strArrayCopy.Add("res\\f2.png");
	strArrayCopy.Add("res\\f3.png");
	strArrayCopy.Add("res\\f4.png");
	strArrayCopy.Add("res\\f5.png");
	strArrayCopy.Add("res\\f6.png");
	strArrayCopy.Add("res\\f7.png");
	strArrayCopy.Add("res\\f8.png");
	strArrayCopy.Add("res\\g2.png");
	strArrayCopy.Add("res\\g3.png");
	strArrayCopy.Add("res\\g4.png");
	strArrayCopy.Add("res\\g5.png");
	strArrayCopy.Add("res\\g6.png");
	strArrayCopy.Add("res\\h1.png");
	strArrayCopy.Add("res\\h2.png");
	strArrayCopy.Add("res\\h3.png");
	strArrayCopy.Add("res\\i1.png");
	strArrayCopy.Add("res\\i2.png");
	strArrayCopy.Add("res\\i3.png");
	strArrayCopy.Add("res\\p1.png");
	strArrayCopy.Add("res\\p2.png");
	strArrayCopy.Add("res\\p3.png");
	strArrayCopy.Add("res\\p4.png");
	strArrayCopy.Add("res\\p5.png");
	strArrayCopy.Add("res\\p6.png");
	strArrayCopy.Add("res\\p7.png");
	strArrayCopy.Add("res\\p8.png");
	strArrayCopy.Add("res\\ptr_h4.png");
	strArrayCopy.Add("res\\_advisor.jpg");
	strArrayCopy.Add("res\\_backgnd.jpg");

}

BOOL CELearningView::CopyVideo_jsFile(CString strCourse)
{
	CString templateDir = MACRO_TEMPLATE_PATH + "video-js\\";
	CString CourseHtmlDir = MACRO_COURSE_PATH + strCourse + "\\html\\video-js\\";
	CStringArray strArrayCopy;
	GetVideo_jsFile(strArrayCopy);
	for (int i = 0; i < strArrayCopy.GetSize(); i++)
	{
		CString strFile = strArrayCopy[i];
		if (!::SafeCopyFile(templateDir + strFile, 
			CourseHtmlDir + strFile,
			TRUE))
			return FALSE;
	}
	return TRUE;
}

void CELearningView::GetVideo_jsFile(CStringArray& strArrayCopy)
{
	strArrayCopy.Add("demo.captions.vtt");
	strArrayCopy.Add("demo.html");
	strArrayCopy.Add("video.dev.js");
	strArrayCopy.Add("video.js");
	strArrayCopy.Add("video-js.css");
	strArrayCopy.Add("video-js.less");
	strArrayCopy.Add("video-js.min.css");
	strArrayCopy.Add("video-js.swf");
	strArrayCopy.Add("font\\vjs.eot");
	strArrayCopy.Add("font\\vjs.svg");
	strArrayCopy.Add("font\\vjs.ttf");
	strArrayCopy.Add("font\\vjs.woff");
	strArrayCopy.Add("lang\\de.js");
	strArrayCopy.Add("lang\\es.js");
	strArrayCopy.Add("lang\\fr.js");
	strArrayCopy.Add("lang\\it.js");
	strArrayCopy.Add("lang\\ja.js");
	strArrayCopy.Add("lang\\ko.js");
	strArrayCopy.Add("lang\\nl.js");
	strArrayCopy.Add("lang\\pt-BR.js");
	strArrayCopy.Add("lang\\ru.js");
	strArrayCopy.Add("lang\\uk.js");
	strArrayCopy.Add("lang\\zh.js");
}

void CELearningView::OnStop() 
{
	Stop();
}

void CELearningView::Stop() 
{
	if (m_pAdapter)
		m_pAdapter->Stop();
}

void CELearningView::OnSetting() 
{
	Stop();

	CString strTitle;
	strTitle.LoadString(IDS_OPTION_CAPTION);
	CSheetOption shOption(strTitle);
	shOption.m_pFont = MACRO_SYS_FONT2;
	shOption.DoModal();
}

void CELearningView::OnRefresh() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	MACRO_MAINFRM->CollectCourse(strCourse);
	MACRO_MAINFRM->LoadCourse(strCourse, TRUE);
}

BOOL CELearningView::ReplacePptFile() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	if (m_nCourseType == 0)
	{
		CString StrTemp;
		StrTemp.LoadString(IDS_PPT_FILE);
 		CFileDialog* pDlg = new CFileDialog(TRUE, "ppt", NULL, 
											OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
											StrTemp, NULL);
		CString strTitle;
		strTitle.LoadString(IDS_SEL_PPT_FOR_REPLACE);
		pDlg->m_ofn.lpstrTitle = (LPCTSTR)strTitle;

		if (pDlg->DoModal() != IDOK)
		{
			delete pDlg;
			return FALSE;
		}

		CString strFilePath = pDlg->GetPathName();
		CString strFileName = pDlg->GetFileName();
		CString strPPTPath = MACRO_COURSE_PATH + strCourse + "\\" + strFileName;
		delete pDlg;

		if (!::CopyFile(strFilePath, strPPTPath, FALSE))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_COPY_PPT_FILE_ERR);
			strMsg.Format(strTmp, strFilePath, strPPTPath);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}

		CString strIniPath = MACRO_INI_FILE(strCourse);
		::WritePrivateProfileString("General", "PPTFile", strFileName, strIniPath);

		if (IsCourseGenerated(strCourse))
		{
			if (W_ASK_YESNO(IDS_IF_REGEN_COURSE, this) == IDYES)
				RegenPptSlide(TRUE);
			else
				W_SHOW_MESSAGE(IDS_PPT_REPLACED, this);
		}
		else
		{
			W_SHOW_MESSAGE(IDS_PPT_REPLACED, this);
		}
	}
	else if (m_nCourseType == 1)
	{
		CDlgSelHTMPresent dlg;
		if (dlg.DoModal() == IDCANCEL)
			return FALSE;

		CString strPresent = dlg.m_strPresent;
			
		BeginWaitCursor();
		CopyPresent(strCourse, strPresent);
		EndWaitCursor();

		RegenPptSlide(TRUE);
		W_SHOW_MESSAGE(IDS_PPT_REPLACED, this);
	}

	return TRUE;
}

BOOL CELearningView::CopyPresent(CString strCourse, CString strPresent) 
{
	CString strSrcPath = MACRO_PRESENT_PATH + strPresent;
	CString strPlaylistFile = strSrcPath + "\\index.pll";
	CString strDestPath = MACRO_COURSE_PATH + strCourse;
	CString strHTMLPath = strDestPath + "\\webpages";

	if (::FileExists(strHTMLPath))
	{
		if (!::DelTree(strHTMLPath))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FAIL_TO_DEL_TREE);
			strMsg.Format(strTmp, strHTMLPath);
			W_SHOW_ERROR_STR(strMsg, this);
			return FALSE;
		}
	}

	if (!::TryToMkDir(strHTMLPath))
		return FALSE;

	if (!::FileExists(strPlaylistFile))
	{
		W_SHOW_ERROR(IDS_PRESENT_EMPTY, this);
		return FALSE;
	}

	CString strDestPlaylistFile = strHTMLPath + "\\index.pll";
	::SafeCopyFile(strPlaylistFile, strDestPlaylistFile, FALSE);

	int nCount = ::GetPrivateProfileInt("General", "Count", 0, strPlaylistFile);
	CString strSec, strTmp;
	char* pszBuffer = new char[1024];
	for (int i = 0; i < nCount; i++)
	{
		strSec.Format("Page%d", i);

		strTmp.Format("%d", i + 1);
		CString strDestDir = strHTMLPath + "\\" + strTmp;
		CString strSrcDir = strSrcPath + "\\" + strTmp;
		::CopyTree(strSrcDir, strDestDir, NULL);
	}
	delete [] pszBuffer;

	return TRUE;
}

void CELearningView::GetPresentTitles(CString strCourse, CStringArray& strArrayOutline) 
{
	CString strCoursePath = MACRO_COURSE_PATH + strCourse;
	CString strPlaylistFile = strCoursePath + "\\webpages\\index.pll";

	int nCount = ::GetPrivateProfileInt("General", "Count", 0, strPlaylistFile);
	CString strSec, strTmp;
	char* pszBuffer = new char[1024];
	for (int i = 0; i < nCount; i++)
	{
		strSec.Format("Page%d", i);
		strTmp.Format("%d", i + 1);
		::GetPrivateProfileString(strSec, "Title", "", pszBuffer, 1024, strPlaylistFile);
		strArrayOutline.Add(pszBuffer);
	}
	delete [] pszBuffer;
}

void CELearningView::OnRecord() 
{
	RecordCourse();
}
	
void CELearningView::RecordCourse(int bCourseWizard) 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
	{
		W_SHOW_WARNING(IDS_NO_COURSE, this);
		return;
	}

	Stop();

	CString strCoursePath = MACRO_COURSE_PATH + strCourse; 
	CString strIniPath = MACRO_INI_FILE(strCourse);

	char szBuf[512];
	if (m_nCourseType == 0)
	{
		::GetPrivateProfileString("General", "PPTFile", "", szBuf, 512, strIniPath);
		BOOL bSel = FALSE;
		if (szBuf[0] != '\0')
		{
			if (!::FileExists(strCoursePath + "\\" + CString(szBuf)))
				bSel = TRUE;
			if (bSel)
			{
				if (W_ASK_YESNO(IDS_PPT_NOT_EXIST_AND_SEL, this) == IDYES)
				{
					if (!ReplacePptFile())	
						return;
				}
				else
					return;
			}
		}
	}

	::GetPrivateProfileString("General", "AudioOnly", "0", szBuf, 256, strIniPath);
	if (!atoi(szBuf))
	{
		CString strType, strParam1, strParam2;
		if (bCourseWizard)
			::GetDevice(strType, strParam1, strParam2, strCourse);
		else
			::GetDevice(strType, strParam1, strParam2);
		int n = ::CheckAndInputDev(strType, strParam1, strParam2);
		if (n != DEV_MATCH && n != AUDIO_ONLY)
		{
			return;
		}
	}

	if (::FileExists(MACRO_MP4_FILE(strCourse)) || ::FileExists(MACRO_ORG_FILE(strCourse)))
	{
		UnLoadVideo();

		CString strTmp, strMsg;
		strTmp.LoadString(IDS_DEL_BEFORE_REC);
		strMsg.Format(strTmp, strCourse);
		if (ASK_OKCANCEL_STR(strMsg) == IDCANCEL)
		{
			ReLoadVideo();
			return;
		}
	
		CStringArray strExclude;
		strExclude.Add(strCoursePath + "\\Course.ini");  //Some Information still in course.ini
		
		CString strHTML = strCoursePath + "\\html\\";


		::GetPrivateProfileString("General", "PPTFile", "", szBuf, 512, strIniPath);
		if (szBuf[0] != '\0')
			strExclude.Add(strCoursePath + "\\" + szBuf);
		DelTree(strHTML, &strExclude);

		CString strVideoPath = strCoursePath + "\\video\\";
		DelTree(strVideoPath, NULL);
		::TryToMkDir(strVideoPath);

		CString strPPTPath = strCoursePath + "\\ppt\\";
		DelTree(strPPTPath, NULL);
#if 1
		CString strPath = MACRO_COURSE_PATH + strCourse;
		if (!::TryToMkDir(strPath) || 
			!::TryToMkDir(strPath + "\\html") || 
			!::TryToMkDir(strPath + "\\video") || 
			!::TryToMkDir(strPath + "\\html\\movie") || 
			!::TryToMkDir(strPath + "\\html\\video-js") || 
			!::TryToMkDir(strPath + "\\html\\video-js\\font") || 
			!::TryToMkDir(strPath + "\\html\\video-js\\lang")
			)
		{
			return;
		}

		if (!CopyLayoutFiles(strCourse))
			return;

		if (!CopyVideo_jsFile(strCourse))
			return;
#endif
		DeleteFile(strCoursePath + "\\default.htm");
		m_pAdapter->LoadCourse(strCourse, TRUE);
	}
	
	CString strSysPath = ::GetSysPath();
	char szDisk[32];
	_splitpath(strSysPath, szDisk, NULL, NULL, NULL);
	CString strDisk = szDisk + CString("\\");
	LONGLONG llSpace = ::GetVolFreeSpace(szDisk);
	if (llSpace < (LONGLONG)1024 * 1024 * 100)
		W_SHOW_WARNING(IDS_DISK_FREE_LOW, this);

	CString strExePath;
	if (EV_VERSION)
		strExePath = strSysPath + "bin\\MCGCapture_eval.exe ";
	else if (PROMOTE_VERSION)
		strExePath = strSysPath + "bin\\MCGCapture_promote.exe ";
	else if (BUNDLE_VERSION)
		strExePath = ::GetSysPath() + "bin\\MCGCapture_bundle.exe ";
	else
		strExePath = strSysPath + "bin\\MCGCapture.exe ";

	CString strParam = "\"" + strCoursePath + "\\\"";

	m_pAdapter->UpdateWindow();

	CString strAuFixValueC;
	strAuFixValueC.Format("AuFixValueC=%d;", MACRO_MAINFRM->m_nAuFixValueC);
	CString strAuFixValueS;
	strAuFixValueS.Format("AuFixValueS=%d;", MACRO_MAINFRM->m_nAuFixValueS);

	if (::WinExec(strExePath + strParam + strAuFixValueC + strAuFixValueS, SW_SHOWNORMAL) < 31)
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CANNOT_CAPTURE_EXE);
		strMsg.Format(strTmp, strExePath); 
		W_SHOW_ERROR_STR(strMsg, this);
	}
	else
	{
		MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
	}
}

BOOL CELearningView::SaveProfileParamsToLocal(BOOL bCheckDevice) 
{
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	while (1)
	{
		::GetPrivateProfileString("Profile", "Param1", "", szBuf, 256, strSysIniPath);
		int nVideoFreq = atoi(szBuf);
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
		if (szBuf[0] == '\0' && nVideoFreq != 0 && bCheckDevice)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return FALSE;
		}
		else
			break;
	}	

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_info.ini";

	::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("Profile", "Name", szBuf, strIniPath);
	int i = 0;
	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("Profile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("Profile", strParam, szBuf, strIniPath);
	}

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("ScreenProfile", "Name", szBuf, strIniPath);

	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("ScreenProfile", strParam, szBuf, strIniPath);
	}

	return TRUE;
}

void CELearningView::NewCourse(CString strCourse) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse;
	if (!::TryToMkDir(strPath) || 
		!::TryToMkDir(strPath + "\\html") || 
		!::TryToMkDir(strPath + "\\video") || 
		!::TryToMkDir(strPath + "\\html\\movie") || 
		!::TryToMkDir(strPath + "\\html\\video-js") || 
		!::TryToMkDir(strPath + "\\html\\video-js\\font") || 
		!::TryToMkDir(strPath + "\\html\\video-js\\lang")
		)
	{
		return;
	}

	CString strIniPath = MACRO_INI_FILE(strCourse);

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strTmpIniPath = strTempPath + "\\temp_info.ini";
	::CopyFile(strTmpIniPath, strIniPath, FALSE);
	::DeleteFile(strTmpIniPath);

	if (!CopyLayoutFiles(strCourse))
		return;

	if (!CopyVideo_jsFile(strCourse))
		return;

	char szValue[256];
	::GetPrivateProfileString("General", "TVSize", "1", szValue, 256, MACRO_INI_PATH);
	::WritePrivateProfileString("General", "TVSize", szValue, strIniPath);

	::GetPrivateProfileString("General", "LayoutName", "", szValue, 256, MACRO_INI_PATH);
	::WritePrivateProfileString("General", "LayoutName", szValue, strIniPath);
	
	::WritePrivateProfileString("Information", "CourseName", strCourse, strIniPath);

	::SetCourseName(strCourse, strCourse);

	WriteOutlookName(strCourse);
}

void CELearningView::NewHTMLLesson(CString strCourse, CString strPresent) 
{
	CString strSrcPath = MACRO_PRESENT_PATH + strPresent;
	CString strPlaylistFile = strSrcPath + "\\index.pll";
	if (!::FileExists(strPlaylistFile))
	{
		W_SHOW_ERROR(IDS_PRESENT_EMPTY, this);
		return;
	}

	NewCourse(strCourse);

	BeginWaitCursor();	
	CopyPresent(strCourse, strPresent);
	EndWaitCursor();
}

void CELearningView::NewLesson(CString strCourse, CString strPPTPath) 
{
	NewCourse(strCourse);
	CString strPath = MACRO_COURSE_PATH + strCourse;

	CString strPPT = strPPTPath;
	CString strFile = ::GetFileName(strPPT);
	CString strNewPath = strPath + "\\" + strFile;
	::SafeCopyFile(strPPT, strNewPath);

	CString strIniPath = MACRO_INI_FILE(strCourse);
	::WritePrivateProfileString("General", "PPTFile", strFile, strIniPath);
}

void CELearningView::CheckDefaultProfile() 
{
	while (TRUE)
	{
		char szBuf[512];
		CString strSysIniPath = MACRO_INI_PATH;
		::GetPrivateProfileString("Profile", "Name", "", szBuf, 512, strSysIniPath);
		CString strDefaultProfile = szBuf;

		::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 512, strSysIniPath);
		CString strScreenProfile = szBuf;

		if (strDefaultProfile != "" && strScreenProfile != "")
			break;
			
		W_SHOW_MESSAGE(IDS_SET_PROFILE_FIRST, this);

		if (strDefaultProfile == "")
		{
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return;
		}

		if (strScreenProfile == "")
		{
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return;
		}
	}
}

void CELearningView::OnNewLesson() 
{
	//add by buxiangyi
	CString CourseName;
	CourseName = MACRO_MAINFRM-> GetCourse();
	if (CourseName != "")
	{
		m_pAdapter->SaveVideo();
	}
	OpenFileFlag = FALSE;
	NewCourseFlag = TRUE;


	if (!((CELearningApp*)AfxGetApp())->CheckVersion2())
		return;

	CheckDefaultProfile();
	if (!SaveProfileParamsToLocal())
		return;

	CDlgNewCourse dlg;
	if (dlg.DoModal() == IDOK)
	{
		UpdateWindow();
		
		CString strPath = MACRO_COURSE_PATH + dlg.m_strCourseName;

		if (dlg.m_bOverwrite && ::FileExists(strPath))
		{
			int nRet = DelCourse(dlg.m_strCourseName, TRUE); 
			if (nRet < 0)
				return;
		}
		
		int nCourseType;
		if (dlg.m_bPPT)
		{
			NewLesson(dlg.m_strCourseName, dlg.m_strSrc);
			nCourseType = 0;
		}
		else
		{
			NewHTMLLesson(dlg.m_strCourseName, dlg.m_strSrc);
			nCourseType = 1;
		}

		//add by buxiangyi
		CString strTmp;
		strTmp = MACRO_TEMPLATE_PATH;
		CString TemstrPath, m_LayoutName;
		TemstrPath = strTmp + "js\\config.js";
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
		m_LayoutName = m_LayoutName.Right(2);
		m_LayoutName = "layout" + m_LayoutName + ".bmp";
		CString srcPath,dectPath;
		srcPath = MACRO_LAYOUT_PATH + "l_Layouts\\l_" + m_LayoutName;
		dectPath =  MACRO_HTML_DIR(dlg.m_strCourseName) + m_LayoutName;
		::SafeCopyFile(srcPath, dectPath, FALSE);

		CString strIniPath = MACRO_INI_FILE(dlg.m_strCourseName);
		strTmp.Format("%d", nCourseType);
		::WritePrivateProfileString("General", "CourseType", strTmp, strIniPath);

		MACRO_MAINFRM->CollectCourse(dlg.m_strCourseName);
		MACRO_MAINFRM->LoadCourse();


		if (dlg.m_bRec)
			OnRecord();
	}
}


void CELearningView::OnIe() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString str = MACRO_MAINFRM->GetHtml();

// add ito 2013/07/23 IE PATH GET PPT2013
	CString strValue = "";
	CString strVersion = "";
	if (::RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE\\", strValue)) 
	{
		if (::RegGetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\svcVersion", strVersion)) 
		{
			strVersion = strVersion.Left(strVersion.Find("."));
			int version = atoi(strVersion);
			if (version < 10)
			{
				W_SHOW_WARNING(IDS_IE_VERSION_LOW, this);
				return;
			}
		}

		if (strCourse != "")
		{
			if (MACRO_MAINFRM->IsCourseVideoModified(strCourse))
				W_SHOW_WARNING(IDS_VIDEO_MODIFIED_PREVIEW, this);

			SetActivePage(0);

			strValue = "\"" + strValue + "\" \"file://" + str + "\"";
//		::ShellExecute(this->m_hWnd, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
			::WinExec(strValue, SW_SHOWMAXIMIZED);	// add itoh 2013/07/23
		}
		else
		{
			strValue = "\"" + strValue + "\" \"file://" + MACRO_EMPTY_HTM + "\"";
//			::ShellExecute(this->m_hWnd, "open", strValue, NULL, NULL, SW_SHOWNORMAL);
			::WinExec(strValue, SW_SHOWNORMAL);
		}

		Stop();
	}
// add ito 2013/07/23 PPT2013
}

void CELearningView::OnDefaultBrowser() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString str = MACRO_MAINFRM->GetHtml();

	if (strCourse != "")
	{
		if (MACRO_MAINFRM->IsCourseVideoModified(strCourse))
			W_SHOW_WARNING(IDS_VIDEO_MODIFIED_PREVIEW, this);

		SetActivePage(0);
		::ShellExecute(this->m_hWnd, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
	}
	else
	{
		::ShellExecute(this->m_hWnd, "open", MACRO_EMPTY_HTM, NULL, NULL, SW_SHOWNORMAL);
	}

	Stop();
}

BOOL CELearningView::AddMarkScriptToASF(CString strCourse, BOOL bForce) 
{
	if (strCourse == "")
		return TRUE;

	CString strAsf = MACRO_MP4_FILE(strCourse);
	if (!::FileExists(strAsf))
		return TRUE;

	long lDuration = MACRO_MAINFRM->GetMediaLength(strAsf);
	//add by buxiangyi
	CStringArray m_oldarray, array;
	CString m_time, strPath;
	m_time.Format("%d",lDuration/1000);
	m_oldarray.Add("var vduration =" );
	array.Add("var vduration = " + m_time + ";");
	strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	UpdateFileByTag(strPath, "config.js", m_oldarray, array, TRUE);

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
	
	if (pMedia->IsMarked() && !bForce)
	{
		delete pMedia;
		return TRUE;
	}

	if (bForce)
	{
		pMedia->RemoveAllMarkers();
		pMedia->RemoveAllScripts();
	}

	CString strOutlinePath = MACRO_OUTLINE_FILE(strCourse);
	CString strOutlineTimePath = MACRO_OUTLINETIME_FILE(strCourse);
	CString strStepPath = MACRO_STEP_FILE(strCourse);
	CString strTmp, strMsg;
	BOOL bOutline = ::FileExists(strOutlinePath);
	BOOL bOutlineTime = ::FileExists(strOutlineTimePath);
	if (!bOutline && !bOutlineTime)
	{
		delete pMedia;
		return TRUE;
	}

	if (!::FileExists(strStepPath))
	{
		delete pMedia;
		return TRUE;
	}

	CStringArray strArrayOutline, strArrayStep, strArrayZoom, strArrayDemo;
	if (bOutlineTime)
		::LoadFile(strOutlineTimePath, strArrayOutline, TRUE);
	else
		::LoadFile(strOutlinePath, strArrayOutline, FALSE);

	::LoadFile(strStepPath, strArrayStep, TRUE);
	if (strArrayOutline.GetSize() < 2 || strArrayStep.GetSize() < 2)
	{
		W_SHOW_WARNING(IDS_NO_CONTROL_POINT, this);
		delete pMedia;
		return FALSE;
	}

	strArrayOutline.RemoveAt(0);
	strArrayStep.RemoveAt(0);
	char szBuf[256];
	if (!bOutlineTime)
	{
		ReplaceUndefinedTitle(strArrayOutline);
		AddMarkHead(strArrayOutline);
		if (!pMedia->MergeOutline(strArrayOutline, strArrayStep))
		{
			W_SHOW_ERROR(IDS_ADD_SCRIPT_ERROR, this);
			delete pMedia;
			return FALSE;
		}
	}
	int i = 0;
	for (i = 0; i < strArrayStep.GetSize(); i++)
	{
		CString str = strArrayStep[i];

		int nIdx = str.Find(' ');
		if (nIdx < 0)
			continue;

		CString strTime = str.Left(nIdx);

		CString strCmd = str.Mid(nIdx + 1);
		if (strCmd == "zoomin" || strCmd == "zoomout")
		{
			strArrayZoom.Add(str);
			strArrayStep.RemoveAt(i);
			i--;
		}
		else if (strCmd.Left(4) == "demo")
		{
			strArrayDemo.Add(str);
			strArrayStep.RemoveAt(i);
			i--;
		}
	}

	long lBaseTick = GetTickCount() + 10000;
	for (i = 0; i < strArrayZoom.GetSize(); i++)
	{
		CString strZoom = strArrayZoom.GetAt(i);
		int nIndex = strZoom.Find(" ");
		CString strTime = strZoom.Left(nIndex);

		CString strZoomValue = strZoom.Mid(nIndex + 1);
		strZoomValue.TrimLeft();
		strZoomValue.TrimRight();

		CString strTag = ltoa(lBaseTick + i, szBuf, 10);
		CString strStep = strTime + " " + strZoomValue + " " + strTag;
		CString strMark = strTime + " " + ::MakeCtrlMark(strTag);
		InsertIntoArray(strStep, strMark, strArrayStep, strArrayOutline);
	}

	for (i = 0; i < strArrayDemo.GetSize(); i++)
	{
		CString strDemo = strArrayDemo.GetAt(i);
		int nIndex = strDemo.Find(" ");
		CString strTime = strDemo.Left(nIndex);
		
		CString strValue = strDemo.Mid(nIndex + 1);
		nIndex = strValue.Find(" ");
		strValue = strValue.Mid(nIndex);
		strValue.TrimLeft();
		strValue.TrimRight();

		CString strStep = strDemo;
		CString strMark = strTime + " " + ::MakeDemoMark(strValue);
		InsertIntoArray(strStep, strMark, strArrayStep, strArrayOutline);
	}

	CheckTimeDuration(strArrayOutline, lDuration);     //±æ¶b≥Ã´·™∫demo∑|show§£•X®”
	CheckTimeDuration(strArrayStep, lDuration);

	if (!pMedia->TagASFBothWithTime(strArrayOutline, strArrayStep))
	{
		W_SHOW_ERROR(IDS_ADD_SCRIPT_ERROR, this);
		delete pMedia;
		return FALSE;
	}

	::DeleteFile(strOutlinePath);
	::DeleteFile(strOutlineTimePath);
	::DeleteFile(strStepPath);

	delete pMedia;
	return TRUE;
}

void CELearningView::CheckTimeDuration(CStringArray& strArray, long lDuration)
{
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strMark = strArray.GetAt(i);
		int nIndex = strMark.Find(" ");
		CString strTime = strMark.Left(nIndex);
		CString strRemain = strMark.Mid(nIndex);

		long lTime = ::ConvertStrToTime(strTime);
		if (lTime >= lDuration)
		{
			lTime = lDuration - 100;
			strTime = ::ConvertTimeToStr(lTime);
			strArray.SetAt(i, strTime + strRemain);
		}
	}
}

void CELearningView::ReplaceUndefinedTitle(CStringArray& strArrayOutline)
{
	CString strUndefined;
	strUndefined.LoadString(IDS_UNDEFINED);
	CString strSlideNo;
	strSlideNo.LoadString(IDS_SLIDE_NO);
	for (int i = 0; i < strArrayOutline.GetSize(); i++)
	{
		CString strOl = strArrayOutline.GetAt(i);
		if (strOl == strUndefined)
			strOl.Format(strSlideNo, i + 1); 
		strArrayOutline.SetAt(i, strOl);
	}
}

void CELearningView::AddMarkHead(CStringArray& strArrayOutline)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CString strIniPath = MACRO_INI_FILE(strCourse);
	int nCourseType = ::GetPrivateProfileInt("General", "CourseType", 0, strIniPath);

	CString strHead = (nCourseType == 0) ? "s" : "w";
	for (int i = 0; i < strArrayOutline.GetSize(); i++)
	{
		CString strOutline;
		strOutline.Format("%s%03d %s", strHead, i + 1, strArrayOutline.GetAt(i));
		strArrayOutline.SetAt(i, strOutline);
	}
}

void CELearningView::InsertIntoArray(CString strStep,
									 CString strMark,
									 CStringArray& strArrayStep, 
									 CStringArray& strArrayMark)
{
	InsertByTime(strStep, strArrayStep);
	InsertByTime(strMark, strArrayMark);
}

void CELearningView::InsertByTime(CString str,
								  CStringArray& strArray) 
{
	int nIndex = str.Find(" ");
	if (nIndex < 0)
		return;
	CString strTime = str.Left(nIndex);

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strTmp = strArray[i];
		int nIdx = strTmp.Find(" ");
		if (nIdx < 0)
			continue;
		CString strT = strTmp.Left(nIdx);
		if (strTime <= strT)
		{
			strArray.InsertAt(i, str);
			return;
		}
	}

	strArray.Add(str);
}

void CELearningView::OnDelLesson() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return;
	}

	DelCourse(strCourse, FALSE);
}

int CELearningView::DelCourse(CString strCourse, BOOL bSlient) 
{
	Stop();

	CString strCurCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == strCurCourse)
		UnLoadVideo();

	if (!bSlient)
	{
		CString strConfirm;
		strConfirm.LoadString(IDS_DEL_CONFIRM);
		if (ASK_OKCANCEL_STR(strConfirm) == IDCANCEL)
		{
			if (strCourse == strCurCourse)
				ReLoadVideo();  // ¡Ÿ≠ÅEÅE˝≤M±º™∫
			return 0;
		}
	}

	int bRet;
	CString strMsg, strTmp;
	CString strCoursePath = MACRO_COURSE_PATH + strCourse;
	if (::DelTree(strCoursePath))
	{
		strTmp.LoadString(IDS_COURSE_DELETED);
		strMsg.Format(strTmp, strCourse);
		MACRO_MAINFRM->CollectCourse("");
		if (!bSlient)
			W_SHOW_MESSAGE_STR(strMsg, this);
		bRet = 1;
	}
	else
	{
		strTmp.LoadString(IDS_COURSE_CANNOT_DELETED);
		strMsg.Format(strTmp, strCourse);
		MACRO_MAINFRM->CollectCourse(strCourse);
		if (!bSlient)
			W_SHOW_WARNING_STR(strMsg, this);
		bRet = -1;
	}

	if (strCourse == strCurCourse)
		m_pAdapter->LoadCourse("", TRUE);

	return bRet;
}

//replase 2014/7/28 kagata chiba
//BOOL CELearningView::RegenPptSlide(BOOL bWarning)
BOOL CELearningView::RegenPptSlide(BOOL bWarning, int newCourseex)
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return FALSE;
	}

	//add 2013/11/20 BugNo.88 kagata
	CString strIniPath = MACRO_INI_FILE(strCourse);
	char szBuf[256];
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

	if (!IsCourseGenerated(strCourse))
	{
		W_SHOW_WARNING(IDS_COURSE_NOT_GEN, this);
		return FALSE;
	}

	Stop();
	//UnLoadVideo();  //•HßK¶≥¿…Æ◊±˛§£±º

	BOOL bReplace = FALSE;
	//if (bWarning)
	if (bWarning && newCourseex == 0)	//replase 2014/7/28 kagata chiba
	{
		int nRet = W_ASK_YESNOCANCEL(IDS_UPDATE_PPT_CONFIRM, this);
		if (nRet == IDCANCEL)
		{
			//ReLoadVideo();  // ¡Ÿ≠ÅEÅE˝≤M±º™∫
			return FALSE;
		}
		else if (nRet == IDYES)
			bReplace = TRUE;
	}

	if (m_nCourseType == 0)
		RegenCoursePPT(strCourse, bReplace, TRUE, newCourseex);	//add 2014/7/28 kagata chiba
		//RegenCoursePPT(strCourse, bReplace, TRUE);
	else if (m_nCourseType == 1 && bReplace)
	{
		CStringArray strArrayOutline;
		GetPresentTitles(strCourse, strArrayOutline); 
		ReGenTag(strCourse, strArrayOutline);
	}

	m_pAdapter->RefreshSlide();

	//ReLoadVideo();

	return TRUE;
}

BOOL CELearningView::IsCourseGenerated(CString strCourse)
{
	if (m_nCourseType == 0)
	{
//		CString strFramePath = MACRO_COURSE_PATH + strCourse + "\\ppt";
		CString strFramePath = MACRO_COURSE_PATH + strCourse + "\\html";	// add itoh 2013/07/18 PPT2013
		return ::FileExists(strFramePath);
	}

	return TRUE;
}

//replase 2014/7/28 kagata chiba
//BOOL CELearningView::RegenCoursePPT(CString strCourse, BOOL bReplace, BOOL bShowProgress)
BOOL CELearningView::RegenCoursePPT(CString strCourse, BOOL bReplace, BOOL bShowProgress, int newCourseex)
{
	//CKeyFramePanel*		m_pKeyFrame;		// add itoh 2013/02/01
	//m_pKeyFrame = new CKeyFramePanel;		// add itoh 2013/02/01

	//add 2014/7/28 kagata chiba
	if(newCourseex == 1)
		bReplace = TRUE;

	CString strDir = MACRO_COURSE_PATH + strCourse;

	char szBuf[256];
	CString strPPTPath;
	CString strIniPath = MACRO_INI_FILE(strCourse);
	::GetPrivateProfileString("General", "PPTFile", "", szBuf, 255, strIniPath);
	if (szBuf[0] == '\0')
		return FALSE;

	strPPTPath = strDir + "\\" + szBuf;

	::DeleteFile(strDir + "\\html\\movie\\outline.lrn");
	::DeleteFile(strDir + "\\default.htm");

	BOOL bWithMedia;
	if(newCourseex == 0){	//add 2014/7/28 kagata chiba
		MACRO_MAINFRM->GenPPTFiles(strCourse, strPPTPath, bWithMedia, bShowProgress);
	}else{	//add 2014/7/28 kagata chiba
		int bSize = 1;	//add 2014/7/28 kagata chiba
		MACRO_MAINFRM->GenPPTFiles(strCourse, strPPTPath, bWithMedia, FALSE, bSize);	//add 2014/7/28 kagata chiba
	}	//add 2014/7/28 kagata chiba
	MACRO_MAINFRM->UpdateWindow();

	CString strOutline = MACRO_OUTLINE_FILE(strCourse);
	if (bReplace)
	{
		if (::FileExists(strOutline))
		{
			CStringArray strArrayOutline;
			::LoadFile(strOutline, strArrayOutline, TRUE); 
			if (strArrayOutline.GetSize() > 0)
				strArrayOutline.RemoveAt(0);
			
			ReplaceUndefinedTitle(strArrayOutline);
			m_pAdapter->ChangeMarker(strArrayOutline);

//			m_pKeyFrame->ResetTooltip();			// add itoh 2013/02/01 BUG No.17
//			m_pPropertySheet->Refresh();			//	add itoh 2012/12/11 BUG No.17
		}
	}
	::DeleteFile(strOutline);

	CString strTitle;
	strTitle.LoadString(IDS_GENETATING_COURSE);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	SubmitLesson(strCourse, bWithMedia);

	LoadPPTFile(strCourse);

	MACRO_MAINFRM->HideProgress();

	LoadCourse(strCourse,true);		// add itoh 2013/02/01 BUG No.17

	return TRUE;
}

BOOL CELearningView::ReGenTag(CString strCourse, CStringArray& strArrayNewOutline)
{
	if (strCourse == "")
		return TRUE;

	CString strAsf = MACRO_MP4_FILE(strCourse);
	if (!::FileExists(strAsf))
		return TRUE;

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

	CStringArray strMarkArray;
	CStringArray strMarkTimeArray;
	CStringArray strScriptArray;
	CStringArray strScriptTimeArray;
	CStringArray strCmdArray;
	if (!pMedia->GetMarkerNames(&strMarkArray, &strMarkTimeArray))
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->GetScriptNames(&strCmdArray, &strScriptArray, &strScriptTimeArray))
	{
		delete pMedia;
		return FALSE;
	}	
	
	CStringArray strArrayOutline, strArrayStep;
	int i = 0;
	for (i = 0; i < strMarkArray.GetSize(); i++)
	{
		CString strMark = strMarkArray[i];
		if (strMark.GetLength() > 5)
		{
			CString strLeft5 = strMark.Left(5);
			if (strLeft5[0] == 's' &&
				strLeft5[1] >= '0' && strLeft5[1] <= '9' &&
				strLeft5[2] >= '0' && strLeft5[2] <= '9' &&
				strLeft5[3] >= '0' && strLeft5[3] <= '9' &&
				strLeft5[4] == ' ')
			{
				int nSlide = atoi(strLeft5.Mid(1, 3));
				int nSize = strArrayNewOutline.GetSize();
				if (nSlide <= nSize && nSlide >= 1)
					strMark.Format("%s%s", strLeft5, strArrayNewOutline[nSlide - 1]);
				else
				{
					CString strSlideNo;
					strSlideNo.LoadString(IDS_SLIDE_NO);
					strMark.Format(strSlideNo, nSlide);
					strMark = strLeft5 + strMark;
				}
			}
			strArrayOutline.Add(strMarkTimeArray[i] + " " + strMark);
		}
	}

	for (i = 0; i < strScriptArray.GetSize(); i++)
		strArrayStep.Add(strScriptTimeArray[i] + " " + strCmdArray[i] + " " + strScriptArray[i]);

	pMedia->RemoveAllMarkers();
	pMedia->RemoveAllScripts();

	if (!pMedia->TagASFBothWithTime(strArrayOutline, strArrayStep))
	{
		W_SHOW_ERROR(IDS_ADD_SCRIPT_ERROR, this);
		delete pMedia;
		return FALSE;
	}

	delete pMedia;
	return TRUE;
}

void CELearningView::EnableTaskBar(BOOL bShow)
{
	static const TCHAR szTrayClass[] = _T("Shell_TrayWnd");
	HWND hwTray = ::FindWindowEx(NULL, NULL, szTrayClass, NULL);
	if (!hwTray)
		return;

	RECT rc;
	::GetWindowRect(hwTray, &rc);
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	if (rc.right >= nScreenX && rc.bottom >= nScreenY)
	{
		int nShow = (bShow) ? SW_SHOW : SW_HIDE;
		::ShowWindow(hwTray, nShow);
	}
}

BOOL CELearningView::ProcessPNGToJPG(CString strCourse) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strHTMLArray;
	::GetFiles(strPath, "htm*;xml", strHTMLArray);

	CStringArray strPNGArray, strJPGArray;
	::GetFiles(strPath, "png", strPNGArray);

	CString strFileName;
	int i = 0;
	for (i = 0; i < strPNGArray.GetSize(); i++)
	{
		strFileName = strPNGArray[i];
		CString strFileNameJPG = strFileName.Left(strFileName.GetLength() - 4) + ".jpg";
		strJPGArray.Add(strFileNameJPG);
		if (!::ConvertImageToJPG(strPath + strFileName, strPath + strFileNameJPG))
			return FALSE;
	}

	for (i = 0; i < strHTMLArray.GetSize(); i++)
	{
		strFileName = strHTMLArray[i];
		::UpdateFileByTag(strPath, strFileName, strPNGArray, strJPGArray);
	}

	for (i = 0; i < strPNGArray.GetSize(); i++)
	{
		strFileName = strPath + strPNGArray[i];
		::DeleteFile(strFileName);
	}

	return TRUE;
}

void CELearningView::ProcessAVIToASF(CString strCourse) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strArray;
	::GetFiles(strPath, "htm*", strArray);

	CString strTitle;
	strTitle.LoadString(IDS_CONVERT_AVI);
	CProgressWnd wndProgress(this, strTitle, FALSE, 0, strArray.GetSize(), FALSE);

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strFileName = strArray[i];

		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CONVERTING_FILE);
		strMsg.Format(strTmp, strFileName);
		wndProgress.SetText(strMsg);
		wndProgress.StepIt();
		wndProgress.SetFocus();
		wndProgress.PeekAndPump();

		CStringArray strArrayAVI, strArrayOldLine, strArrayNewLine, strArrayTypes;
		strArrayTypes.Add("avi");
		strArrayTypes.Add("mpg");
		strArrayTypes.Add("mpeg");
		strArrayTypes.Add("mov");
		::ParseForVideo(strPath + strFileName, strArrayAVI, strArrayOldLine, 
					    strArrayNewLine, strArrayTypes);

		strArrayTypes.RemoveAll();
		strArrayTypes.Add("wav");
		::ParseForAudio(strPath + strFileName, strArrayAVI, strArrayOldLine, 
					    strArrayNewLine, strArrayTypes);
		int j = 0;
		for (j = 0; j < strArrayAVI.GetSize(); j++)
		{
			CString strAVI = strArrayAVI[j];
	        CString strParam;
			int nDot = strAVI.Find(".");
			CString strName = strAVI.Left(nDot);
			CString strExt = strAVI.Mid(nDot + 1);
			CString strAVIFile = strPath + strAVI;
			CString strASFFile = strPath + strName + ".mp4";

			if (MACRO_MAINFRM->ConvertFile(strAVIFile, strASFFile, 22, 348))
			{
				::DeleteFile(strAVIFile);
			}
		}

		for (j = 0; j < strArrayOldLine.GetSize(); j++)
		{
			CString strOldLine = strArrayOldLine[j];
			CString strNewLine = strArrayNewLine[j];

			CStringArray strsTag, strsReplace;
			strsTag.Add(strOldLine);
			strsReplace.Add(strNewLine);
			::UpdateFileByTag(strPath, strFileName, strsTag, strsReplace);
		}
	}
}

void CELearningView::OnDevSetting() 
{
	Stop();

	CheckDefaultProfile();
	if (!SaveProfileParamsToLocal())
		return;

	CDevice dlg;
	dlg.m_strIniPath = MACRO_INI_PATH;
	dlg.m_Initialization = TRUE;
	dlg.m_Cameraflag = TRUE;
	dlg.DoModal();
}

void CELearningView::OnDefaultLayout() 
{
	Stop();

	CString strTitle;
	strTitle.LoadString(IDS_DEFAULT_SETTING_CAPTION);

	CSheetDefaultLayout shLayout(strTitle);
	shLayout.m_pFont = MACRO_SYS_FONT2;
	shLayout.m_nCurSel = 0;
	shLayout.DoModal();
}

void CELearningView::OnRecordAsfFile() 
{
	Stop();

	CString strType, strParam1, strParam2;
	::GetDevice(strType, strParam1, strParam2);
	int n = ::CheckAndInputDev(strType, strParam1, strParam2);
	if (n != DEV_MATCH)
		return;
	//add by buxiangyi
	CheckDefaultProfile();
	if (!SaveRecordParamsToLocal())
		return;
	/////////////////////////////////////////
	CDlgMP4Rec dlg;
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strTmpIniPath = strTempPath + "\\temp_profile.ini";
	//add by buxaingyi
	//::DeleteFile(strTmpIniPath);
	/////////////////////////
	if (!::FileExists(strTmpIniPath))
	{
		SaveProfileParamsToLocal("Profile", strTmpIniPath);
	} 
	if (dlg.DoModal() == IDOK)
	{
		CString strAuFixValueC;
		strAuFixValueC.Format("AuFixValueC=%d;", MACRO_MAINFRM->m_nAuFixValueC);
		CString strPath = dlg.m_strVideoPath;
		CString strExePath = ::GetSysPath() + "bin\\RecordMP4.exe ";
		CString strParam = "/p \"" + strPath + "\" /profilepath_xxx \"" + strTmpIniPath + "\"";
		if (::WinExec(strExePath + strParam + strAuFixValueC, SW_SHOWNORMAL) < 31)
		{
			CString strMsg, strTmp;
			strTmp.LoadString(IDS_CANNOT_CAPTURE_EXE);
			strMsg.Format(strTmp, strExePath); 
			W_SHOW_ERROR_STR(strMsg, this);
		}
		else
		{
			MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
		}
	}
}

//add 2014/6/16 kagata outemon
void CELearningView::OnRenameCourse()
{
	CString strOldName;
	strOldName = MACRO_MAINFRM->GetCourse();
	RenameCourse(strOldName,0);
}
int CELearningView::RenameCourse(CString strOldName, int flag) 
{
	//del 2014/6/16 kagata outemon
	//CString strOldName;
	//strOldName = MACRO_MAINFRM->GetCourse();

	if (strOldName == "")
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return 0;
	}

	if(flag == 0)												//add 2014/6/16 kagata outemon
		UnLoadVideo();

	CString strNewName;
	CDlgInputCourse dlg;
	dlg.m_strCourseName = strOldName;
	dlg.m_strCaption = "ñºëOÇÃïœçX";
	CString strDestCourse;

	//add 2014/7/31 kagata outemon
	//MACRO_MAINFRM->LoadCourse("");

	if (dlg.DoModal() == IDOK)
	{
		MACRO_MAINFRM->LoadCourse("");
		strNewName = dlg.m_strCourseName;
		CString strOldPath = MACRO_COURSE_PATH + strOldName;
		CString strNewPath = MACRO_COURSE_PATH + strNewName;
		if (rename(strOldPath, strNewPath) == 0)
		{
			MACRO_MAINFRM->CollectCourse(strNewName);
			CString strTitle  = "";
			//MACRO_MAINFRM->LoadCourse(strNewName);
			if(flag == 0)										//add 2014/6/16 kagata outemon
			{													//add 2014/6/16 kagata outemon
				MACRO_MAINFRM->LoadCourse(strNewName);
				W_SHOW_MESSAGE(IDS_RENAME_COURSE_OK, this);
				//add by buxiangyi

#ifdef _KDI
				strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
				strTitle.LoadString(IDR_MAINFRAME);
#endif

				MACRO_MAINFRM->SetWindowText(strTitle + "-" + strNewName);
				return 1;										//add 2014/6/16 kagata outemon
			}													//add 2014/6/16 kagata outemon
			else												//add 2014/6/16 kagata outemon
			{													//add 2014/6/16 kagata outemon
				MACRO_MAINFRM->LoadCourse(strNewName,false,true,false);		//add 2014/6/16 kagata outemon
				//add by buxiangyi

#ifdef _KDI
				strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
				strTitle.LoadString(IDR_MAINFRAME);
#endif

				MACRO_MAINFRM->SetWindowText(strTitle + "-" + strNewName);
				return 1;										//add 2014/6/16 kagata outemon
			}													//add 2014/6/16 kagata outemon
		}
		else
		{
			if(flag == 0)										//add 2014/6/16 kagata outemon
			{													//add 2014/6/16 kagata outemon
				W_SHOW_ERROR(IDS_RENAME_COURSE_ERROR, this);
				MACRO_MAINFRM->LoadCourse(strOldName);			//add 2014/7/31 kagata outemon
				ReLoadVideo();
				return 2;										//add 2014/6/16 kagata outemon
			}													//add 2014/6/16 kagata outemon
			else												//add 2014/6/16 kagata outemon
			{													//add 2014/6/16 kagata outemon
				return 2;										//add 2014/6/16 kagata outemon
			}													//add 2014/6/16 kagata outemon
		}
	}
	else
	{
		if(flag == 0)											//add 2014/6/16 kagata outemon
			ReLoadVideo();
		return 3;												//add 2014/6/16 kagata outemon
	}
}

BOOL CELearningView::HasCourseSelected()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	return (strCourse != "");
}

void CELearningView::OnSaveAs() 
{
	CString strSrcCourse = MACRO_MAINFRM->GetCourse();
	if (strSrcCourse == "")
	{
		W_SHOW_WARNING(IDS_NO_SEL_COURSE, this);
		return;
	}

	Stop();
	//UnLoadVideo();
	
	CDlgInputCourse dlg;
	dlg.m_strCourseName = strSrcCourse;
	dlg.m_strCaption = "ñºëOÇïtÇØÇƒï€ë∂";
	CString strDestCourse;
	if (dlg.DoModal() == IDOK)
	{
		strDestCourse = dlg.m_strCourseName;
		CString strPath = MACRO_COURSE_PATH + strDestCourse;
		if (!TryToMkDir(strPath))
		{
			//ReLoadVideo();	
			return;
		}
	}
	else
	{
		//ReLoadVideo();	
		return;
	}

	BeginWaitCursor();

	CString strTitle;
	strTitle.LoadString(IDS_SAVING_AS);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	CString strSrcPath = MACRO_COURSE_PATH + strSrcCourse;
	CString strDestPath = MACRO_COURSE_PATH + strDestCourse;
	::CopyTree(strSrcPath, strDestPath);
	
	MACRO_MAINFRM->HideProgress();
	EndWaitCursor();

	W_SHOW_MESSAGE(IDS_COURSE_SAVE_AS_OK, this);

	MACRO_MAINFRM->CollectCourse(strDestCourse);
	CString strNewTitle  = "";
#ifdef _KDI
	strNewTitle.LoadString(IDR_MAINFRAME_KDI);
#else
	strNewTitle.LoadString(IDR_MAINFRAME);
#endif
	MACRO_MAINFRM->SetWindowText(strNewTitle + "-" + strDestCourse);
	MACRO_MAINFRM->LoadCourse(strDestCourse);
}

void CELearningView::OnDelCourse() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();

	CDlgSelectCourse dlg;
	CStringArray strArraySelCourse;
	dlg.m_strWndText = "çÌèú";//.LoadString(IDS_DELETE_COURSE);
	dlg.m_pstrArraySelCourse = &strArraySelCourse;
	dlg.m_strTitle = "çÌèúÇ∑ÇÈÉRÉìÉeÉìÉcÇëIëÇµÇƒÇ≠ÇæÇ≥Ç¢";//.LoadString(IDS_SELECT_TO_DELETE);
	if (dlg.DoModal() != IDOK)
		return;
	
	if (strArraySelCourse.GetSize() == 0)
	{
		W_SHOW_WARNING(IDS_COURSE_NUMBER_NULL, this);
		return;
	}
		

	Stop();

	BOOL bUnLoad = FALSE;
	for (int i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		CString str = strArraySelCourse[i];
		if (str == strCourse)
		{
			UnLoadVideo();
			bUnLoad = TRUE;
		}
	}

	CString strMsg, strTmp;
	strTmp.LoadString(IDS_SURE_TO_DELETE);
	strMsg.Format(strTmp, strArraySelCourse.GetSize());
	if (W_ASK_YESNO_STR(strMsg, this) == IDYES)
	{
		BeginWaitCursor();
		CString strTitle;
		strTitle.LoadString(IDS_DELETE_COURSE);
		CString strMsg;
		strMsg.LoadString(IDS_PROCESSING);
		MACRO_MAINFRM->SetProgress(strTitle, strMsg);
		MACRO_MAINFRM->ShowProgress();

		BOOL bFail = FALSE;
		for (int i = 0; i < strArraySelCourse.GetSize(); i++)
		{
			CString str = strArraySelCourse[i];
			CString strPath = MACRO_COURSE_PATH + str;
			if (!::DelTree(strPath))
			{
				CString strTmp, strMsg;
				strTmp.LoadString(IDS_FAIL_TO_DEL_COURSE);
				strMsg.Format(strTmp, str);
				W_SHOW_ERROR_STR(strMsg, this);
				bFail = TRUE;
			}
		}
		
		EndWaitCursor();
		MACRO_MAINFRM->HideProgress();
		MACRO_MAINFRM->CollectCourse("");
		MACRO_MAINFRM->LoadCourse("", TRUE);
		
		if (!bFail)
			W_SHOW_MESSAGE(IDS_DELETE_COMPLETE, this);
#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif
		if (bUnLoad)
			MACRO_MAINFRM->SetWindowText(strTitle);
	}
	else
	{
		if (bUnLoad)
			ReLoadVideo();
	}
}

BOOL CELearningView::OnGetfolder(CString& strFolderPath) 
{
	CString strLabel;
	strLabel.LoadString(IDS_SELECT_FOLDER);
	if (::GetFolder(&strFolderPath, strLabel, m_hWnd, NULL, NULL))
	{
		if (!strFolderPath.IsEmpty())
			return TRUE;
	}
	return FALSE;
}


void CELearningView::OnUpdateDelLesson(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnUpdateRenameCourse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnUpdateSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnUpdateRecord(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected() && !BACKEND_VERSION);
}

void CELearningView::OnUpdateUploadCourse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnNewFromMedia() 
{
	if (!((CELearningApp*)AfxGetApp())->CheckVersion2())
		return;

	//MACRO_MAINFRM->LoadCourse("", TRUE);
	m_pAdapter->LoadCourse("", TRUE);	//kagata ppt
	CDlgNewCourseFromMedia dlg;
	if (dlg.DoModal() == IDOK)
	{
		MACRO_MAINFRM->UpdateWindow();

		CheckDefaultProfile();
		if (!SaveProfileParamsToLocal(FALSE))
			return;

		CString strCourse = dlg.m_strCourseName;
		CString strCoursePath = MACRO_COURSE_PATH + strCourse;
		CString strMediaPath = dlg.m_strMediaPath;
		CString strPresentSrc = dlg.m_strPresentSrc;
		BOOL bPPT = dlg.m_bPPT;
		BOOL bOverwrite = dlg.m_bOverwrite;

		if (TestVideoFile(strMediaPath) == FALSE)
		{
			CString strMsg;
			strMsg.LoadString(IDS_VIDEO_NOVOICE);
			AfxMessageBox(strMsg);
			return;
		}

		if (bOverwrite && ::FileExists(strCoursePath))
		{
			int nRet = DelCourse(strCourse, TRUE); 
			if (nRet < 0)
				return;
		}
		
		int nCourseType;
		if (bPPT)
		{
			NewLesson(strCourse, strPresentSrc);
			nCourseType = 0;
			BOOL bWithMedia;
			MACRO_MAINFRM->GenPPTFiles(strCourse, strPresentSrc, bWithMedia, TRUE);
			MACRO_MAINFRM->UpdateWindow();
			SubmitLesson(strCourse, bWithMedia);
		}
		else
		{
			NewHTMLLesson(strCourse, strPresentSrc);
			nCourseType = 1;
			SubmitHTMLLesson(strCourse);
		}

		CString strIniPath = MACRO_INI_FILE(strCourse);
		CString strTmp;
		strTmp.Format("%d", nCourseType);
		::WritePrivateProfileString("General", "CourseType", strTmp, strIniPath);

		MACRO_MAINFRM->CollectCourse(strCourse);

		m_pAdapter->LoadCourse(strCourse, TRUE);

		CString strTitle;
		CString strMsg;
		strMsg.LoadString(IDS_PROCESSING);

		strTitle.LoadString(IDS_CONVERTFILES);
		MACRO_MAINFRM->SetProgress(strTitle, strMsg);

		CString strExt = ::GetFileExt(strMediaPath);
		CString strFile = ::GetFileName(strMediaPath);
		CString strDestASF = strCoursePath + "\\video\\default.mp4";
		if (strExt.CompareNoCase("mp4") == 0)// || strExt.CompareNoCase("asf") == 0)
		{
			::SafeCopyFile(strMediaPath, strDestASF, FALSE);
			/*if (!MACRO_MAINFRM->ConvertFromASF(strCourse, strMediaPath))
			{
				strTmp.LoadString(IDS_CONVERT_ERROR);
				strMsg.Format(strTmp, strMediaPath, strDestASF);
				W_SHOW_ERROR_STR(strMsg, this);
				MACRO_MAINFRM->HideProgress();
				return;
			}*/
		}
		else
		{
			if (!MACRO_MAINFRM->ConvertFile(strMediaPath, strDestASF, 20, 128, TRUE))
			{
				strTmp.LoadString(IDS_CONVERT_ERROR);
				strMsg.Format(strTmp, strMediaPath, strDestASF);
				W_SHOW_ERROR_STR(strMsg, this);
				MACRO_MAINFRM->HideProgress();
				return;
			}
		}

		MACRO_MAINFRM->WriteVideoDate(strCourse, strMediaPath);
		MACRO_MAINFRM->WriteVideoLength(strCourse);
		MACRO_MAINFRM->WriteSlideNo(strCourse);

		strTitle.LoadString(IDS_DUMPING_KEY);
		MACRO_MAINFRM->SetProgress(strTitle, strMsg);
		if (!MACRO_MAINFRM->DumpKeyAndCreateIndex(strCourse))
		{
			W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
			MACRO_MAINFRM->HideProgress();
			return;
		}

		CString strFinalWMVFile = MACRO_FINAL_MP4_FILE(strCourse);
		//::SafeCopyFile(strMediaPath, strDestASF, FALSE);
		::SafeCopyFile(strDestASF, strFinalWMVFile, FALSE);

		MACRO_MAINFRM->HideProgress();
		m_pAdapter->LoadCourse(strCourse, TRUE);

		W_SHOW_MESSAGE(IDS_SET_SYNC_POINTS, this);

		m_pAdapter->setviewer();	//kagata ppt

		m_pAdapter->PostMessage(WM_SET_PPT_POINT);
#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif
		MACRO_MAINFRM->SetWindowText(strTitle + "-" + strCourse);

		CStringArray strPPtLayout;
		GetPPTLayout(strPPtLayout);

		int index = m_pAdapter ->GetArrayLayoutNameSize();
		if (strPPtLayout.GetSize() == 0)
		{
			NewArrayLayout();
		}
		else
		{
			CString strDefaultLayout = GetDefaultLayout();
			if (strPPtLayout.GetSize() < index)
				for (int i = strPPtLayout.GetSize(); i < index; i++)
					strPPtLayout.Add(strDefaultLayout);
			m_pAdapter ->m_strArrayLayoutName.RemoveAll();
			for (int i = 0; i < index; i++)
			{
				m_pAdapter ->m_strArrayLayoutName.Add(strPPtLayout[i]);
			}
		}
	}

	//add 2013/11/05 kagata BugNo.5
	//OnRefresh();
}

CString CELearningView::GetPPTFilePath()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return "";

	if (m_nCourseType == 0)
	{
		CString strIniPath = MACRO_INI_FILE(strCourse);
		char szBuf[256];
		::GetPrivateProfileString("General", "PPTFile", "", szBuf, 255, strIniPath);
		if (szBuf[0] == '\0')
			return "";
		
		CString strPath = MACRO_COURSE_PATH + strCourse + CString("\\") + szBuf;
		if (::FileExists(strPath))
			return strPath;
	}

	return "";
}

BOOL CELearningView::OpenPptFile() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return FALSE;

	if (m_nCourseType == 0)
	{
		CString strIniPath = MACRO_INI_FILE(strCourse);
		char szBuf[256];
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

		::ShellExecute(this->m_hWnd, "open", strPath, NULL, NULL, SW_SHOWNORMAL);
	}
	else if (m_nCourseType == 1)
		HtmlPresent(TRUE);

	return TRUE;
}

void CELearningView::OnUpdateBackupCourse(CCmdUI* pCmdUI) 
{
#ifdef _BUNDLE
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(HasCourseSelected());
#endif
}
//add by buxiangyi
BOOL CELearningView::SaveProfileParamsToLocal(CString Name, CString strInipath)
{
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	while (1)
	{
		::GetPrivateProfileString("Profile", "Param1", "", szBuf, 256, strSysIniPath);
		int nVideoFreq = atoi(szBuf);
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
		if (szBuf[0] == '\0' && nVideoFreq != 0)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return FALSE;
		}
		else
			break;
	}	

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);

	::GetPrivateProfileString(Name, "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString(Name, "Name", szBuf, strInipath);
	int i = 0;
	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString(Name, strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString(Name, strParam, szBuf, strInipath);
	}

}


void CELearningView::OnScreenCapture() 
{
	Stop();

	CString strType, strParam1, strParam2;
	::GetDevice(strType, strParam1, strParam2);
	int n = ::CheckAndInputDevSCreen(strType, strParam1, strParam2);
	if (n != DEV_MATCH)
		return;

	//add by buxaingyi
	CheckDefaultProfile();
	if (!SaveScreenParamsToLocal())
		return;
	//////////////////////////////////////////////////////////
	CDlgScreenCapture dlg;
	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strTmpIniPath = strTempPath + "\\temp_s_profile.ini";

	//add by buxiangyi
	//::DeleteFile(strTmpIniPath);
	//////////////////////////////////////
	if (!::FileExists(strTmpIniPath))
	{
		SaveProfileParamsToLocal("ScreenProfile", strTmpIniPath);
	}

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.m_strVideoPath;
		CString strExePath;
	
		/*
		if (EV_VERSION)
			strExePath = ::GetSysPath() + "bin\\MCGCapture_eval.exe /screen ";
		else if (PROMOTE_VERSION)
			strExePath = ::GetSysPath() + "bin\\MCGCapture_promote.exe /screen ";
		else
			strExePath = ::GetSysPath() + "bin\\MCGCapture.exe /screen ";
		*/


		if (EV_VERSION)
			strExePath = ::GetSysPath() + "bin\\MCGCapture_eval.exe /screen ";
		else if (PROMOTE_VERSION)
			strExePath = ::GetSysPath() + "bin\\MCGCapture_promote.exe /screen ";
		else if (BUNDLE_VERSION)
			strExePath = ::GetSysPath() + "bin\\MCGCapture_bundle.exe /screen ";
		else
			strExePath = ::GetSysPath() + "bin\\MCGCapture.exe /screen ";



		CString strParam = "\"" + strPath + "\" /profilepath_xxx \"" + strTmpIniPath + "\"";
		if (::WinExec(strExePath + strParam, SW_SHOWNORMAL) < 31)
		{
			CString strMsg, strTmp;
			strTmp.LoadString(IDS_CANNOT_CAPTURE_EXE);
			strMsg.Format(strTmp, strExePath); 
			W_SHOW_ERROR_STR(strMsg, this);
		}
		else
		{
			MACRO_MAINFRM->ShowWindow(SW_MINIMIZE);
		}
	}
}

void CELearningView::OnBkupCourse() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();

	CDlgBackupCourse dlg;
	CStringArray strArraySelCourse;
	dlg.m_pstrArraySelCourse = &strArraySelCourse;
	if (dlg.DoModal() != IDOK)
		return;
	UpdateWindow();

	BOOL bDel = dlg.m_bDel;
	CString strPath = dlg.m_strPath;

	if (strArraySelCourse.GetSize() == 0)
		return;

	Stop();

	BOOL bUnLoad = FALSE;
	if (bDel)
	{
		for (int i = 0; i < strArraySelCourse.GetSize(); i++)
		{
			CString str = strArraySelCourse[i];
			if (str == strCourse)
			{
				UnLoadVideo();
				bUnLoad = TRUE;
			}
		}
	}

	BOOL bShowEnding = FALSE;

	for (int i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		int nRet = BackupOneCourse(strPath, strArraySelCourse[i], bDel);
		if (nRet < 0)
		{
			//add by buxiangyi
			/*CString strMsg, strTmp;
			strTmp.LoadString(IDS_FAIL_TO_BACK_COURSE);
			strMsg.Format(strTmp, "%s", strArraySelCourse[i]);*/
			/////////////////////////////////////////////////////////
			W_SHOW_ERROR(IDS_FAIL_TO_COURSE, this);
		}
		else if (nRet > 0)
			bShowEnding = TRUE;
	}
	
	MACRO_MAINFRM->CollectCourse();

	if (bUnLoad && bDel)
	{
		CString strTitle  = "";
#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif
		MACRO_MAINFRM->SetWindowText(strTitle);

		MACRO_MAINFRM->LoadCourse("");
	}

	if (bUnLoad && !bDel)
		ReLoadVideo();

	if (bShowEnding)
		W_SHOW_MESSAGE(IDS_BACKUP_COMPLETE, this);
}

int CELearningView::BackupOneCourse(CString strPath, CString strCourse, BOOL bDel)
{
	CString strSrc = MACRO_COURSE_PATH + strCourse;

	if (strPath.Right(1) != "\\")
		strPath += "\\";

	CString strBkup = strPath + strCourse;
	TryToMkDir(strPath);
	int nRet = 1;
	BOOL bDelDest = FALSE;
	if (::FileExists(strBkup))
	{
		CDlgCourseDup dlg;
		dlg.m_strPath = strPath;
		dlg.m_strStatic.LoadString(IDS_BACKUP_COURSE_FOUND); 
		dlg.m_strRadioOverwrite.LoadString(IDS_BACKUP_COURSE_OVERWRITE); 
		dlg.m_strRadioNewName.LoadString(IDS_BACKUP_COURSE_NEWNAME); 
		dlg.m_strTitle.LoadString(IDS_BACKUP_COURSE_TITLE);
		dlg.m_strCourse = strCourse;
		if (dlg.DoModal() == IDOK)
		{
			UpdateWindow();

			if (dlg.m_bOverWrite)
				bDelDest = TRUE;
			strBkup = strPath + dlg.m_strCourse;
		}
		else
			return 0;
	}

	UpdateWindow();
	
	BeginWaitCursor();
	CString strTitle;
	strTitle.LoadString(IDS_BACKINGUP_COURSE);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	if (bDelDest)
		::DelTree(strBkup);

	if (!::CopyTree(strSrc, strBkup, NULL))
		nRet = -1;

	if (bDel && nRet >= 0)
		::DelTree(strSrc);

	MACRO_MAINFRM->HideProgress();
	EndWaitCursor();

	return nRet;
}

int CELearningView::RestoreOneCourse(CString strSrcPath, CString strCourse, BOOL bDel, BOOL bover, 
									 CString& strNewCourse)
{
	CString strBkup = MACRO_COURSE_PATH + strCourse;

	if (strSrcPath.Right(1) != "\\")
		strSrcPath += "\\";
	CString strSrc = strSrcPath + strCourse;

	int nRet = 1;
	BOOL bDelDest = FALSE;
	strNewCourse = strCourse;

	//add by buxiangyi
	if (bover != 1)
	{

		if (::FileExists(strBkup))
		{
			CDlgCourseDup dlg;
			dlg.m_strPath = MACRO_COURSE_PATH;
			dlg.m_strStatic.LoadString(IDS_RESTORE_COURSE_FOUND); 
			dlg.m_strRadioOverwrite.LoadString(IDS_RESTORE_COURSE_OVERWRITE); 
			dlg.m_strRadioNewName.LoadString(IDS_RESTORE_COURSE_NEWNAME); 
			dlg.m_strTitle.LoadString(IDS_RESTORE_COURSE_TITLE);
			dlg.m_strCourse = strCourse;
			if (dlg.DoModal() == IDOK)
			{
				UpdateWindow();

				if (dlg.m_bOverWrite)
				{
					bDelDest = TRUE;
				}

				strBkup = MACRO_COURSE_PATH + dlg.m_strCourse;
				strNewCourse = dlg.m_strCourse;
				if (bDelDest == TRUE)
				{
					::DelTree(strBkup);
				}
			}
			else
				return 0;
		}
	}
	else 
	{
			::DelTree(strBkup);
	}
	
	UpdateWindow();

	BeginWaitCursor();
	CString strTitle;
	strTitle.LoadString(IDS_RESTORING_COURSE);

	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	////////////////////////////
	if (!::CopyTree(strSrc, strBkup, NULL))
		nRet = -1;

	if (bDel && nRet >= 0)
		::DelTree(strSrc);

	MACRO_MAINFRM->HideProgress();
	EndWaitCursor();

	return nRet;
}

int CELearningView::ImportOneCourse(CString strSrcPath, CString strCourse, CString& strNewCourse)
{
	CString strBkup = MACRO_COURSE_PATH + strCourse;

	if (strSrcPath.Right(1) != "\\")
		strSrcPath += "\\";
	CString strSrc = strSrcPath + strCourse + "\\";

	int nRet = 1;
	BOOL bDelDest = FALSE;
	strNewCourse = strCourse;
	if (::FileExists(strBkup))
	{
		CDlgCourseDup dlg;
		dlg.m_strPath = MACRO_COURSE_PATH;
		dlg.m_strStatic.LoadString(IDS_RESTORE_COURSE_FOUND); 
		dlg.m_strRadioOverwrite.LoadString(IDS_IMPORT_COURSE_OVERWRITE); 
		dlg.m_strRadioNewName.LoadString(IDS_IMPORT_COURSE_NEWNAME); 
		dlg.m_strTitle.LoadString(IDS_IMPORT_COURSE_TITLE);

		dlg.m_strCourse = strCourse;
		if (dlg.DoModal() == IDOK)
		{
			UpdateWindow();

			if (dlg.m_bOverWrite)
				bDelDest = TRUE;
			strBkup = MACRO_COURSE_PATH + dlg.m_strCourse;

			strNewCourse = dlg.m_strCourse;
		}
		else
			return 0;
	}

	UpdateWindow();
	
	BeginWaitCursor();
	CString strTitle;
	strTitle.LoadString(IDS_IMPORTING_COURSE);
	CString strMsg;
	strMsg.LoadString(IDS_PROCESSING);
	MACRO_MAINFRM->SetProgress(strTitle, strMsg);
	MACRO_MAINFRM->ShowProgress();

	if (bDelDest)
		::DelTree(strBkup);

	CString strIniPath = strSrc + "Course.ini";
	char szBuf[512];
	::GetPrivateProfileString("General", "PPTFile", "", szBuf, 256, strIniPath);
	if (szBuf[0] == '\0')
		return 0;
	CString strPPTPath = strSrc + szBuf;
	NewLesson(strNewCourse, strPPTPath); 

	CString strVideoPath = MACRO_COURSE_PATH + strNewCourse + "\\video\\";
	CString strResultVideoPath = MACRO_COURSE_PATH + strNewCourse + "\\html\\movie\\";
	CString strDefaultWMV = strSrc + "default.mp4";
	CString strDefaultWMV2 = strSrc + "html\\default.mp4";
	if (::FileExists(strDefaultWMV))
	{
		::SafeCopyFile(strDefaultWMV, strVideoPath + "movie.mp4");
		::CopyFile(strSrc + "default.prx", strVideoPath + "default.prx", FALSE);
	}
	else if (::FileExists(strDefaultWMV2))
	{
		::SafeCopyFile(strDefaultWMV2, strVideoPath + "movie.mp4");
		::CopyFile(strSrc + "html\\default.prx", strVideoPath + "default.prx", FALSE);
	}

	CStringArray strWMVFiles;
	::GetFiles(strSrc + "html", "mp4", strWMVFiles);
	for (int i = 0; i < strWMVFiles.GetSize(); i++)
	{
		CString strFile = strWMVFiles[i];
		if (strFile.Left(4).CompareNoCase("demo") == 0)
			::SafeCopyFile(strSrc + "html\\" + strFile, strResultVideoPath + strFile);
	}

	//CStringArray strArrayEx, strArrayWMV;
	//::GetFiles(strSrc, "wmv;prx;kfr;lrn", strArrayWMV);

	//for (int i = 0; i < strArrayWMV.GetSize(); i++)
	//{
	//	CString strPath = strSrc + "\\" + strArrayWMV[i];
	//	strArrayEx.Add(strPath);	
	//}

	//for (i = 0; i < strArrayEx.GetSize(); i++)
	//{
	//	CString strDestPath = strBkup + "\\html\\" + strArrayWMV[i];
	//	if (!::SafeCopyFile(strArrayEx[i], strDestPath, FALSE))
	//	{
	//		nRet = -1;
	//		break;
	//	}
	//}
	
	if (nRet >= 0)
	{
		if (!RegenCoursePPT(strNewCourse, FALSE, FALSE))
			nRet = -1;
	}

	if (nRet >= 0)
	{
		if (!CheckTags(strNewCourse))
			nRet = -1;
		else
			::SafeCopyFile(strVideoPath + "default.mp4", strResultVideoPath + "movie.mp4");
	}

	CString strNewIniPath = MACRO_INI_FILE(strNewCourse);
	::GetPrivateProfileString("Information", "CourseName", "", szBuf, 512, strIniPath);
	::WritePrivateProfileString("Information", "CourseName", szBuf, strNewIniPath);
	::SetCourseName(strNewCourse, szBuf); 

	::GetPrivateProfileString("Information", "Message", "", szBuf, 512, strIniPath);
	::WritePrivateProfileString("Information", "Message", szBuf, strNewIniPath);
	::SetMessage(strNewCourse, szBuf);
	CString strSrcContentPath = strSrc + "\\html\\content.lrn";
	CString strDestContentPath = MACRO_COURSE_PATH + strNewCourse + "\\html\\content.lrn";
	::CopyFile(strSrcContentPath, strDestContentPath, FALSE);

	::GetPrivateProfileString("Information", "Advisor", "", szBuf, 512, strIniPath);
	::WritePrivateProfileString("Information", "Advisor", szBuf, strNewIniPath);
	::SetAdvisor(strNewCourse, szBuf);

	EndWaitCursor();
	MACRO_MAINFRM->HideProgress();

	return nRet;
}

BOOL CELearningView::CheckTags(CString strCourse)
{
	if (strCourse == "")
		return FALSE;

	CString strAsf = MACRO_MP4_FILE(strCourse);
	if (!::FileExists(strAsf))
		return FALSE;

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
	CStringArray strMarkArray;
	CStringArray strMarkTimeArray;
	CStringArray strScriptArray;
	CStringArray strScriptTimeArray;
	CStringArray strCmdArray;
	if (!pMedia->GetMarkerNames(&strMarkArray, &strMarkTimeArray))
	{
		delete pMedia;
		return FALSE;
	}

	if (!pMedia->GetScriptNames(&strCmdArray, &strScriptArray, &strScriptTimeArray))
	{
		delete pMedia;
		return FALSE;
	}	
	
	CStringArray strArrayOutline, strArrayStep;
	BOOL bModified = FALSE;
	int i = 0;
	for (i = 0; i < strMarkArray.GetSize(); i++)
	{
		CString strMark = strMarkArray[i];
		if (strMark.GetLength() > 5 && strMark[3] == ' ')
		{
			if (strMark[0] != 's')
			{
				strMark = "s" + strMark;
				bModified = TRUE;
			}
		}
		else if (strMark.Left(3) == "!@#")
		{
			strMark = "!@#$" + strMark.Mid(3);
		}
		strArrayOutline.Add(strMarkTimeArray[i] + " " + strMark);
	}

	for (i = 0; i < strScriptArray.GetSize(); i++)
		strArrayStep.Add(strScriptTimeArray[i] + " " + strCmdArray[i] + " " + strScriptArray[i]);
	
	if (bModified)
	{
		pMedia->RemoveAllMarkers();
		pMedia->RemoveAllScripts();

		if (!pMedia->TagASFBothWithTime(strArrayOutline, strArrayStep))
		{
			W_SHOW_ERROR(IDS_ADD_SCRIPT_ERROR, this);
			delete pMedia;
			return FALSE;
		}
	}

	delete pMedia;

	return TRUE;
}

void CELearningView::OnRestoreCourse() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();

	CDlgRestoreCourse dlg;
	CStringArray strArraySelCourse;
	dlg.m_pstrArraySelCourse = &strArraySelCourse;
	if (dlg.DoModal() != IDOK)
		return;
	UpdateWindow();

	BOOL bDel = dlg.m_bDel;
	CString strPath = dlg.m_strPath; 
	//add by buxiangyi
	BOOL bover = dlg.m_over;

	if (strArraySelCourse.GetSize() == 0)
		return;

	Stop();

	BOOL bUnLoad = FALSE;
	int i = 0;
	for (i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		CString str = strArraySelCourse[i];
		if (str == strCourse)
		{
			UnLoadVideo();
			bUnLoad = TRUE;
		}
	}

	BOOL bShowEnding = FALSE;
	CString strNewCourse;
	CString strFirstCourse = "";
	for (i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		//add  by buxiangyi
		int nRet = RestoreOneCourse(strPath, strArraySelCourse[i], bDel, bover, strNewCourse);

		if (nRet < 0)
		{
			CString strMsg, strTmp;
			strTmp.LoadString(IDS_FAIL_TO_RESTORE_COURSE);
			strMsg.Format(strTmp, strArraySelCourse[i]);
			W_SHOW_ERROR_STR(strMsg, this);
		}
		else if (nRet > 0)
		{
			bShowEnding = TRUE;
			if (i == 0)
				strFirstCourse = strNewCourse;
		}
	}
	
	MACRO_MAINFRM->CollectCourse();

	if (strArraySelCourse.GetSize() == 1 && strFirstCourse != "")
	{
		MACRO_MAINFRM->LoadCourse(strFirstCourse, TRUE);
		CString strTitle  = "";

#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif

		MACRO_MAINFRM->SetWindowText(strTitle + "-" + strFirstCourse);
	}
	else if (bUnLoad)
		ReLoadVideo();

	if (bShowEnding)
		W_SHOW_MESSAGE(IDS_RESTORE_COMPLETE, this);
}


void CELearningView::OnBackupCourse() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CDlgBackupOneCourse dlg;
	dlg.m_strCourse = strCourse;
	dlg.DoModal();
}

void CELearningView::OnBackupMaintain() 
{
	CDlgBackupMaintain dlg;
	dlg.DoModal();
}

void CELearningView::OnRegister() 
{
	Stop();
	
	CDlgRegister dlg;
	dlg.DoModal();
}

void CELearningView::OnOnlineHelp() 
{
	CString str = ::GetSysPath() + "mcg.chm";
	if (str != "")
		::ShellExecute(this->m_hWnd, "open", str, NULL, NULL, SW_SHOWNORMAL);
}

void CELearningView::OnUpdateRegCode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BUNDLE_VERSION && !BACKEND_VERSION);
}

void CELearningView::OnUpdateRegister(CCmdUI* pCmdUI) 
{
	BOOL bRightVersion = !EV_VERSION && !BACKEND_VERSION;
	BOOL bNotRegistered = !((CELearningApp*)AfxGetApp())->m_bFormal;
	pCmdUI->Enable(bRightVersion && bNotRegistered);
}

void CELearningView::OnUpdateNewFromMedia(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BACKEND_VERSION && !PROMOTE_VERSION && !BUNDLE_VERSION);
}

void CELearningView::OnCourseLoader() 
{
#ifdef _BUNDLE
	CString strExePath = ::GetSysPath() + "bin\\CourseLauncher_express.exe ";
#else
	CString strExePath = ::GetSysPath() + "bin\\CourseLauncher.exe ";
#endif

	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString strParam = "";
	if (strCourse != "")
		strParam = "/c" + strCourse;
	if (::WinExec(strExePath + strParam, SW_SHOWNORMAL) < 31)
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CANNOT_LOAD_EXE);
		strMsg.Format(strTmp, strExePath); 
		W_SHOW_ERROR_STR(strMsg, this);
	}
}


void CELearningView::OnUpdateOnlineHelp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);	
}

void CELearningView::OnUpdateNewLesson(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!BACKEND_VERSION);
}

void CELearningView::OnUpdateRecordAsfFile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!BACKEND_VERSION && !BUNDLE_VERSION);
}

void CELearningView::OnUpdateScreenCapture(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(!BACKEND_VERSION && !BUNDLE_VERSION);
	#ifndef _AENRICH
		pCmdUI->Enable(!BACKEND_VERSION && !BUNDLE_VERSION);
	#else
		pCmdUI->Enable(!BACKEND_VERSION);
	#endif
}

void CELearningView::OnUpdateDevSetting(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!BACKEND_VERSION);
}

void CELearningView::OnUpdateDefaultLayout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!BACKEND_VERSION && !PROMOTE_VERSION);
}

void CELearningView::OnUpdateSetting(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!BACKEND_VERSION && !BUNDLE_VERSION);
}

void CELearningView::SetActivePage(int nPage)
{

}

void CELearningView::StopMovieWndTimer()
{
	if (m_pAdapter)
		m_pAdapter->StopMovieWndTimer();
}

void CELearningView::RestartMovieWndTimer()
{
	if (m_pAdapter)
		m_pAdapter->RestartMovieWndTimer();
}

void CELearningView::OnCheckOut() 
{
	Stop();
		
	CDlgCheckOut dlg;
	dlg.DoModal();
}

void CELearningView::OnUpdateCheckOut(CCmdUI* pCmdUI) 
{
	BOOL bRightVersion = !EV_VERSION && !BACKEND_VERSION;
	BOOL btRegistered = ((CELearningApp*)AfxGetApp())->m_bFormal;
	pCmdUI->Enable(bRightVersion && btRegistered);
}

void CELearningView::OnContactUs() 
{
	CString strURL;
	strURL.LoadString(IDS_COMPANY_URL);
	::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}

void CELearningView::OnSerialReuse() 
{
    CString MACstr;  //leu
	CString strURL;
	strURL.LoadString(IDS_VERIFY_URL);

    /********************* leu begin **************************/
    /*
     * SerialReuse URL parameters
     * 1.MAC address, new for MCG 5.0,
    */
    /*
     * get MAC address
     * 1.need include Netapi32.lib
     * 2.get MAc address, xx(1)-xx(2)-xx(3)-xx(4)-xx(5)-xx(6), convert into string"xx(1)xx(2)xx(3)xx(4)xx(5)xx(6)"
     * 3.encode MAC address
    */
    MACstr = GetMACAddressString();
    ((CELearningApp*)AfxGetApp())->EncodeID(MACstr);

    strURL += MACstr;
    /********************* leu end ****************************/

	::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}

void CELearningView::OnUpdateSerialReuse(CCmdUI* pCmdUI) 
{
	BOOL bRightVersion = !EV_VERSION && !BACKEND_VERSION;
	pCmdUI->Enable(bRightVersion);
}

void CELearningView::OnBuyQuota() 
{
	CString strURL;
	strURL.LoadString(IDS_BUY_QUOTA_URL);
	::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}

void CELearningView::OnUseQuota() 
{
#ifdef _RENT	
	CDlgUseQuota dlg;
	dlg.DoModal();
#endif
}

void CELearningView::OnUpdateCourseLoader(CCmdUI* pCmdUI) 
{
	BOOL bRightVersion = !EV_VERSION && !PROMOTE_VERSION;
	pCmdUI->Enable(bRightVersion);
}
 
void CELearningView::OnImportCourse() 
{
	CDlgImportCourse dlg;
	CStringArray strArraySelCourse;
	dlg.m_pstrArrayImportCourse = &strArraySelCourse;
	if (dlg.DoModal() != IDOK)
		return;
	UpdateWindow();

	if (strArraySelCourse.GetSize() == 0)
		return;

	if (dlg.m_strPath == "")
		return;

	Stop();

	CString strCourse = MACRO_MAINFRM->GetCourse();

	BOOL bUnLoad = FALSE;
	int i = 0;
	for (i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		CString str = strArraySelCourse[i];
		if (str == strCourse)
		{
			UnLoadVideo();
			bUnLoad = TRUE;
		}
	}

	BOOL bShowEnding = FALSE;
	CString strNewCourse;
	CString strFirstCourse = "";
	for (i = 0; i < strArraySelCourse.GetSize(); i++)
	{
		int nRet = ImportOneCourse(dlg.m_strPath + "Courses\\", strArraySelCourse[i], strNewCourse);
		if (nRet < 0)
		{
			CString strMsg, strTmp;
			strTmp.LoadString(IDS_FAIL_TO_IMPORT_COURSE);
			strMsg.Format(strTmp, strArraySelCourse[i]);
			W_SHOW_ERROR_STR(strMsg, this);
		}
		else if (nRet > 0)
		{
			bShowEnding = TRUE;
			if (i == 0)
				strFirstCourse = strNewCourse;
		}
	}
	
	MACRO_MAINFRM->CollectCourse();

	if (strArraySelCourse.GetSize() == 1 && strFirstCourse != "")
		MACRO_MAINFRM->LoadCourse(strFirstCourse, TRUE);
	else if (bUnLoad)
		ReLoadVideo();

	if (bShowEnding)
		W_SHOW_MESSAGE(IDS_IMPORT_COMPLETE, this);
}

void CELearningView::OnUpdateImportCourse(CCmdUI* pCmdUI) 
{
	BOOL bRightVersion = !EV_VERSION && !BUNDLE_VERSION;
	pCmdUI->Enable(bRightVersion);
}

void CELearningView::OnHtmlPresent() 
{
	HtmlPresent(FALSE); 
}

void CELearningView::HtmlPresent(BOOL bOpenPresent) 
{
	CString strExePath = ::GetSysPath() + "bin\\HTMLPresentEditor.exe ";

	CString strCourse = MACRO_MAINFRM->GetCourse();
	CString strParam = "";
	if (bOpenPresent && strCourse != "")
	{
		CString strFilePath = MACRO_COURSE_PATH + strCourse + "\\webpages\\index.pll";
		if (::FileExists(strFilePath))
			strParam = "/c" + strFilePath;
	}
	if (::WinExec(strExePath + strParam, SW_SHOWNORMAL) < 31)
	{
		CString strMsg, strTmp;
		strTmp.LoadString(IDS_CANNOT_LOAD_EXE);
		strMsg.Format(strTmp, strExePath); 
		W_SHOW_ERROR_STR(strMsg, this);
	}
}

void CELearningView::ReplacePeriodByFile(CString strVideoPath, long lTimeStart, long lTimeEnd)
{
	m_pAdapter->ReplacePeriodByFile(strVideoPath, lTimeStart, lTimeEnd);
}

BOOL CELearningView::ExportCourseNoWMV(CString strFolderPath, CString strCourse)
{
	CString strSrcPath = MACRO_COURSE_PATH + strCourse;
	CString strSrcHTMLPath = strSrcPath + "\\html";
	CString strSrcWebPath = strSrcPath + "\\webpages";

	if (!TryToMkDir(strFolderPath))
		return FALSE;

	CString strDestHTMLPath = strFolderPath + "html";
	if (!TryToMkDir(strDestHTMLPath))
		return FALSE;

	CString strDestWebPath = strFolderPath + "webpages";
	if (!TryToMkDir(strDestWebPath))
		return FALSE;

	CStringArray strArrayEx;
	strArrayEx.Add(strSrcPath + "\\html\\movie\\");

	CStringArray strArrayWMV;
	::GetFiles(strSrcHTMLPath, "mp4", strArrayWMV);
	for (int i = 0; i < strArrayWMV.GetSize(); i++)
		strArrayEx.Add(strSrcHTMLPath + "\\" + strArrayWMV[i]);

	if (!::CopyTree(strSrcHTMLPath, strDestHTMLPath, &strArrayEx))
		return FALSE;
	if (::FileExists(strSrcWebPath))
	{
		if (!::CopyTree(strSrcWebPath, strDestWebPath, NULL))
			return FALSE;
	}

	if (!::SafeCopyFile(strSrcPath + "\\default.htm", 
					    strFolderPath + "default.htm"))
		return FALSE;

	return TRUE;
}

void CELearningView::OnExport() 
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	if (MACRO_MAINFRM->IsCourseVideoModified(strCourse))
	{
		W_SHOW_ERROR(IDS_VIDEO_NOT_SAVED, this);
		return;
	}

	CDlgExportCourse dlg;
	dlg.m_lScreenAudioCodecID = -1;
	CString strDemo1 = MACRO_COURSE_PATH + strCourse + "\\video\\screen.prx";
	if (::FileExists(strDemo1))
		MACRO_MAINFRM->GetPRXType(strDemo1, dlg.m_lScreenAudioCodecID);

	if (dlg.DoModal() == IDOK)
	{
		UpdateWindow();

		int nType = dlg.m_nType;
		CString strFolder = dlg.m_strFolder;
		if (strFolder.Right(1) != "\\")
			strFolder += "\\";

		CString strSrcHTMLPath = MACRO_COURSE_PATH + strCourse + "\\html";
		CString strSrcVideoPath = MACRO_COURSE_PATH + strCourse + "\\html\\movie";
		CString strDestVideoPath = strFolder + "html\\movie";

		ExportCourseNoWMV(strFolder, strCourse);
		::TryToMkDir(strDestVideoPath);

		CString strSrc = MACRO_FINAL_MP4_FILE(strCourse);
		CString strDest = strFolder + "html\\movie\\movie.mp4";
		CString strSrcProfile = MACRO_COURSE_PATH + strCourse + "\\video\\default.prx";
		CString strSrcScreenProfile = MACRO_COURSE_PATH + strCourse + "\\video\\screen.prx";

		CString strTitle;
		strTitle.LoadString(IDS_EXPORTING_COURSE);
		CString strMsg;
		strMsg.LoadString(IDS_PROCESSING);
		MACRO_MAINFRM->SetProgress(strTitle, strMsg);
		MACRO_MAINFRM->ShowProgress();

		if (dlg.m_nType == 0)
		{
			::CopyTree(strSrcVideoPath, strDestVideoPath);
			CopyAssociatedFiles(strCourse, strFolder); 
		}
		else
		{
			if (!::FileExists(strSrcProfile))
			{
				CString strTmp, strMsg;
				strTmp.LoadString (IDS_FAIL_TO_FIND_PROFILE);
				strMsg.Format(strTmp, strCourse);
				W_SHOW_ERROR_STR(strMsg, this);
				return;
			}

			if (dlg.m_nType == 1)
			{
				CStringArray strArrayEx;
				strArrayEx.Add(strSrc);
				::CopyTree(strSrcVideoPath, strDestVideoPath, &strArrayEx);
				MACRO_MAINFRM->ConvertAudio(strSrc, strSrcProfile, strDest, TRUE);
				CopyAssociatedFiles(strCourse, strFolder); 
			}
			else if (dlg.m_nType == 2)
			{
				strTitle.LoadString(IDS_CONVERTING_DEFAULT);
				MACRO_MAINFRM->SetProgress(strTitle, strMsg);
				MACRO_MAINFRM->ConvertBandWidth(strSrc, strSrcProfile, strDest, dlg.m_nVideoBand, dlg.m_nFPS, 
												dlg.m_nKeyFrame, dlg.m_nVideoQuality, dlg.m_nAudioBand, 
												dlg.m_nAudioFreq, (dlg.m_strMono == "M") ? 1 : 2, TRUE);

				if (dlg.m_lScreenAudioCodecID >= 0)
				{
					if (!::FileExists(strSrcScreenProfile))
					{
						W_SHOW_ERROR(IDS_FAIL_TO_FIND_SCREEN_PROFILE, this);
						return;
					}

					strTitle.LoadString(IDS_CONVERTING_DEMO);
					MACRO_MAINFRM->SetProgress(strTitle, strMsg);
					CStringArray strArrayDemoFiles;
					GetDemoFiles(strCourse, strArrayDemoFiles);
					for (int i = 0; i < strArrayDemoFiles.GetSize(); i++)
					{
						strSrc = strArrayDemoFiles[i];
						strDest = strFolder + "html\\movie\\" + ::GetFileName(strSrc);
						MACRO_MAINFRM->ConvertBandWidth(strSrc, strSrcScreenProfile, strDest, dlg.m_nDemoVideoBand, 
														dlg.m_nDemoFPS, dlg.m_nDemoKeyFrame, dlg.m_nDemoVideoQuality, 
														dlg.m_nDemoAudioBand, dlg.m_nDemoAudioFreq, 
														(dlg.m_strDemoMono == "M") ? 1 : 2);
					}
				}

				CStringArray strArrayAssociated;
				GetAssociatedFiles(strCourse, strArrayAssociated);
				for (int i = 0; i < strArrayAssociated.GetSize(); i++)
				{
					strTitle.LoadString(IDS_CONVERTING_ASSOCIATED);
					MACRO_MAINFRM->SetProgress(strTitle, strMsg);
					strSrc = strArrayAssociated[i];
					strDest = strFolder + "html\\" + ::GetFileName(strSrc);
					MACRO_MAINFRM->ConvertBandWidth(strSrc, strSrcProfile, strDest, dlg.m_nVideoBand, dlg.m_nFPS, 
													dlg.m_nKeyFrame, dlg.m_nVideoQuality, dlg.m_nAudioBand, 
													dlg.m_nAudioFreq, (dlg.m_strMono == "M") ? 1 : 2);
				}
			}

			if (!MACRO_MAINFRM->CreateIndex(strDest))
			{
				W_SHOW_ERROR(IDS_DUMP_KEY_ERROR, this);
				MACRO_MAINFRM->HideProgress();
				return;
			}
		}

		MACRO_MAINFRM->HideProgress();
	}
}

void CELearningView::CopyAssociatedFiles(CString strCourse, CString strFolder) 
{
	CStringArray strArrayAssociated;
	GetAssociatedFiles(strCourse, strArrayAssociated);
	for (int i = 0; i < strArrayAssociated.GetSize(); i++)
	{
		CString strSrc = strArrayAssociated[i];
		CString strDest = strFolder + "html\\" + ::GetFileName(strSrc);
		::SafeCopyFile(strSrc, strDest, FALSE);
	}
}

void CELearningView::OnUpdateExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::GetDemoFiles(CString strCourse, CStringArray& strArrayDemoFiles)
{
	strArrayDemoFiles.RemoveAll();

	CString strDemoPath = MACRO_COURSE_PATH + strCourse + "\\html\\movie\\";
	CStringArray strArrayPath;
	::GetFiles(strDemoPath, "mp4", strArrayPath);
	for (int i = 0; i < strArrayPath.GetSize(); i++)
	{
		CString strFile = strArrayPath[i];
		if (strFile.Left(4).CompareNoCase("demo") == 0)
			strArrayDemoFiles.Add(strDemoPath + strFile);
	}
}

void CELearningView::GetAssociatedFiles(CString strCourse, CStringArray& strArrayAssociatedFiles)
{
	strArrayAssociatedFiles.RemoveAll();

	CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
	CStringArray strArrayPath;
	::GetFiles(strPath, "mp4", strArrayPath);
	for (int i = 0; i < strArrayPath.GetSize(); i++)
	{
		CString strFile = strArrayPath[i];
		strArrayAssociatedFiles.Add(strPath + strFile);
	}
}

void CELearningView::OnProfileSetting() 
{
	CDlgDefaultProfileSet dlg;
	dlg.DoModal();
}

void CELearningView::OnScreenProfileSetting() 
{
	CDlgScreenProfileSet dlg;
	dlg.DoModal();
}

void CELearningView::OnUndo() 
{
	MACRO_MAINFRM->m_objUndoRedo.Undo();
}

void CELearningView::OnRedo() 
{
	MACRO_MAINFRM->m_objUndoRedo.Redo();
}

void CELearningView::OnUpdateUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!MACRO_MAINFRM->m_objUndoRedo.IsUndoStackEmpty());
}

void CELearningView::OnUpdateRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!MACRO_MAINFRM->m_objUndoRedo.IsRedoStackEmpty());
}

void CELearningView::OnSaveVideo() 
{
	m_pAdapter->SaveVideo();
}
void CELearningView::SaveVideoAfterRecording() 
{
	m_pAdapter->SaveVideo();
}

void CELearningView::OnUpdateSaveVideo(CCmdUI* pCmdUI) 
{
//#ifdef _BUNDLE
//	pCmdUI->Enable(FALSE);
	//#else

//#endif
}

void CELearningView::OnBatchVolume() 
{
	CDlgSelectCourseVolume dlg;
	CStringArray strArrayCourses;
	dlg.m_pstrArraySelCourse = &strArrayCourses;
	if (dlg.DoModal() == IDOK)
	{
		for (int i = 0; i < strArrayCourses.GetSize(); i++)
		{
			CString strCourse = MACRO_COURSE_PATH + strArrayCourses[i] + "video";


		}
	}
}

void CELearningView::OnUpdateScreenProfileSetting(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(!BUNDLE_VERSION);
	#ifndef _AENRICH
		pCmdUI->Enable(!BUNDLE_VERSION);
	#endif
}

BOOL CELearningView::SaveProfileParamsToLocalWizard() 
{
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_info.ini";

	::GetPrivateProfileString("ScreenProfile", "Codec", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("ScreenProfile", "Codec", szBuf, strIniPath);

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("ScreenProfile", "Name", szBuf, strIniPath);

	for (int i = 1; i < 8; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("ScreenProfile", strParam, szBuf, strIniPath);
	}

	return TRUE;
}

void CELearningView::OnNewWizard() 
{
	CMasterDlg Dlg(this);
	((CMainFrame *)AfxGetMainWnd())->Kill_Timer();
	CSetupPage SetupPage;
	CPageSelPPT PageSelPPt;
	CPageSelHTML PageSelHTML;
	CPageInputName PageInputName;
	CPageSelDevice PageSelDevice;
	CPageSelFreq PageSelFreq;
	CPageSelLayout PageSelLayout;
	CPageSelTV PageSelTV;

	SetupPage.m_pstrArray = &(Dlg.m_strArray);
	PageSelPPt.m_pstrArray = &(Dlg.m_strArray);
	PageSelHTML.m_pstrArray = &(Dlg.m_strArray);
	PageInputName.m_pstrArray = &(Dlg.m_strArray);
	PageSelDevice.m_pstrArray = &(Dlg.m_strArray);
    PageSelFreq.m_pstrArray = &(Dlg.m_strArray);
    PageSelLayout.m_pstrArray = &(Dlg.m_strArray);
    PageSelTV.m_pstrArray = &(Dlg.m_strArray);

	SetupPage.m_pstrArray2 = &(Dlg.m_strArray2);
	PageSelPPt.m_pstrArray2 = &(Dlg.m_strArray2);
	PageSelHTML.m_pstrArray2 = &(Dlg.m_strArray2);
	PageInputName.m_pstrArray2 = &(Dlg.m_strArray2);
	PageSelDevice.m_pstrArray2 = &(Dlg.m_strArray2);
    PageSelFreq.m_pstrArray2 = &(Dlg.m_strArray2);
    PageSelLayout.m_pstrArray2 = &(Dlg.m_strArray2);
    PageSelTV.m_pstrArray2 = &(Dlg.m_strArray2);

	SetupPage.m_pstrArray3 = &(Dlg.m_strArray3);
	PageSelPPt.m_pstrArray3 = &(Dlg.m_strArray3);
	PageSelHTML.m_pstrArray3 = &(Dlg.m_strArray3);
	PageInputName.m_pstrArray3 = &(Dlg.m_strArray3);
	PageSelDevice.m_pstrArray3 = &(Dlg.m_strArray3);
    PageSelFreq.m_pstrArray3 = &(Dlg.m_strArray3);
    PageSelLayout.m_pstrArray3 = &(Dlg.m_strArray3);
    PageSelTV.m_pstrArray3 = &(Dlg.m_strArray3);

	Dlg.AddPage(&SetupPage, CSetupPage::IDD);
	Dlg.AddPage(&PageSelPPt, CPageSelPPT::IDD);
	Dlg.AddPage(&PageSelHTML, CPageSelHTML::IDD);
	Dlg.AddPage(&PageInputName, CPageInputName::IDD);
	Dlg.AddPage(&PageSelDevice, CPageSelDevice::IDD);
	Dlg.AddPage(&PageSelFreq, CPageSelFreq::IDD);
	Dlg.AddPage(&PageSelLayout, CPageSelLayout::IDD);
	Dlg.AddPage(&PageSelTV, CPageSelTV::IDD);

	if (Dlg.DoModal() == ID_WIZFINISH)
	{
		if (!((CELearningApp*)AfxGetApp())->CheckVersion2())
			return;

		SaveProfileParamsToLocalWizard();

		CString strCourseName = Dlg.m_strArray.GetAt(3);
		CString strPath = MACRO_COURSE_PATH + strCourseName;
		if (::FileExists(MACRO_COURSE_PATH + strCourseName))
		{
			CDlgCourseDup dlg;
			dlg.m_strPath = MACRO_COURSE_PATH;
			dlg.m_strStatic.LoadString(IDS_NEW_COURSE_FOUND); 
			dlg.m_strRadioOverwrite.LoadString(IDS_NEW_COURSE_OVERWRITE); 
			dlg.m_strRadioNewName.LoadString(IDS_NEW_COURSE_NEWNAME); 
			dlg.m_strTitle.LoadString(IDS_NEW_COURSE_TITLE);
			dlg.m_strCourse = strCourseName;
			if (dlg.DoModal() == IDCANCEL)
				return;
			strCourseName = dlg.m_strCourse;
			BOOL bOverwrite = dlg.m_bOverWrite;
			if (bOverwrite && ::FileExists(strPath))
			{
				int nRet = DelCourse(strCourseName, TRUE); 
				if (nRet < 0)
					return;
			}
		}

		int nCourseType;
		char szBuf[256];
		CString strLayout = Dlg.m_strArray.GetAt(6);
		if (strLayout == "")
		{
			::GetPrivateProfileString("General", "LayoutName", "", szBuf, 256, MACRO_INI_PATH);
			strLayout = MACRO_LAYOUT_PATH + CString(szBuf) + ".bmp";
		}
		if (Dlg.m_strArray.GetAt(0) == "PPT")
		{
			NewLessonWizard(strCourseName, Dlg.m_strArray.GetAt(1), strLayout);
			nCourseType = 0;
		}
		else
		{
			NewHTMLLessonWizard(strCourseName, Dlg.m_strArray.GetAt(2), strLayout);
			nCourseType = 1;
		}

		CString strIniPath = MACRO_INI_FILE(strCourseName);
		CString strTmp;
		strTmp.Format("%d", nCourseType);
		::WritePrivateProfileString("General", "CourseType", strTmp, strIniPath);

		CString strLayoutPath = Dlg.m_strArray.GetAt(6);
		if (strLayoutPath != "")
			ChangeLayout(strCourseName, strLayoutPath);

		CString strTVPath = Dlg.m_strArray2.GetAt(7);
		if (strTVPath != "")
			ChangeTV(strCourseName, ::GetFileName(strTVPath));

		CString strOutlook = Dlg.m_strArray.GetAt(7);
		if (strOutlook != "")
			ChangeOutlook(strCourseName, strOutlook);

		WriteDevice(strCourseName, Dlg.m_strArray.GetAt(4), Dlg.m_strArray2.GetAt(4), Dlg.m_strArray3.GetAt(4));

		MACRO_MAINFRM->CollectCourse(strCourseName);
		MACRO_MAINFRM->LoadCourse();

		RecordCourse(1);
	}
}

void CELearningView::ChangeTV(CString strCourse, CString strFileName) 
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	
	int nDot = strFileName.Find(".");
	CString strIndex = strFileName.Mid(2, nDot - 2);
	CString strPath = MACRO_TV_PATH + "TV" + strIndex;
	
	CStringArray strArray;
	::GetFiles(strPath, "*", strArray);
	CString strSrcPath, strDestPath;
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		strSrcPath = strPath + "\\" + strArray[i];
		strDestPath = strHTML + strArray[i];
		if (!::SafeCopyFile(strSrcPath, strDestPath))
			return;			
	}

	strSrcPath = MACRO_TV_PATH + "TV" + strIndex + ".gif";
	strDestPath = strHTML + "_vidpanel.gif";
	if (!::SafeCopyFile(strSrcPath, strDestPath))
		return;			
}

void CELearningView::ChangeLayout(CString strCourse, CString strPath) 
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	if (!::TryToMkDir(strHTML))
		return;

	CString strLayoutPath = strHTML + "_layout.bmp";
	
	CString strFile = ::GetFileName(strPath);
	int nSize = strFile.GetLength();
	int nTotal = strPath.GetLength();
	strPath = strPath.Left(nTotal - nSize);

	//CString strIndex = strFile.Mid(6, nSize - 10);
	CString strSrcPath = strPath + "l_layouts\\l_" + strFile;
	::CopyFile(strSrcPath, strLayoutPath, FALSE);

	int nIndex = strFile.ReverseFind('.');
	CString strName;
	if (nIndex >= 0)
	{
		strName = strFile.Left(nIndex);
		strFile = strName + ".htm";
		::CopyFile(strPath + strFile, strHTML + "_frame.htm", FALSE);
	}
}

void CELearningView::ChangeOutlook(CString strCourse, CString strOutlook) 
{
	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	if (!::TryToMkDir(strHTML))
		return;

	CString strOutlookPath = strHTML + "_outlook.js";
	
	CString strSrcPath = MACRO_OUTLOOK_PATH + strOutlook + ".js";
	::CopyFile(strSrcPath, strOutlookPath, FALSE);
}

void CELearningView::NewCourseWizard(CString strCourse, CString strLayoutName) 
{
	CString strPath = MACRO_COURSE_PATH + strCourse;
	if (!::TryToMkDir(strPath) || 
		!::TryToMkDir(strPath + "\\html") || 
		!::TryToMkDir(strPath + "\\video") || 
		!::TryToMkDir(strPath + "\\html\\movie") || 
		!::TryToMkDir(strPath + "\\html\\video-js") || 
		!::TryToMkDir(strPath + "\\html\\video-js\\font") || 
		!::TryToMkDir(strPath + "\\html\\video-js\\lang")
		)
	{
		return;
	}

	CString strIniPath = MACRO_INI_FILE(strCourse);

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strTmpIniPath = strTempPath + "\\temp_info.ini";
	::CopyFile(strTmpIniPath, strIniPath, FALSE);
	::DeleteFile(strTmpIniPath);

	if (!CopyLayoutFiles(strCourse))
		return;

	if (!CopyVideo_jsFile(strCourse))
		return;

	CString strFile = ::GetFileName(strLayoutName);
	int nIndex = strFile.ReverseFind('.');
	CString strName;
	if (nIndex >= 0)
		strName = strFile.Left(nIndex);
	::WritePrivateProfileString("General", "LayoutName", strName, strIniPath);
	
	::WritePrivateProfileString("Information", "CourseName", strCourse, strIniPath);

	::SetCourseName(strCourse, strCourse);
}

void CELearningView::NewHTMLLessonWizard(CString strCourse, CString strPresent, CString strLayoutName) 
{
	CString strSrcPath = MACRO_PRESENT_PATH + strPresent;
	CString strPlaylistFile = strSrcPath + "\\index.pll";
	if (!::FileExists(strPlaylistFile))
	{
		W_SHOW_ERROR(IDS_PRESENT_EMPTY, this);
		return;
	}

	NewCourseWizard(strCourse, strLayoutName);

	BeginWaitCursor();	
	CopyPresent(strCourse, strPresent);
	EndWaitCursor();
}

void CELearningView::NewLessonWizard(CString strCourse, CString strPPTPath, CString strLayoutName) 
{
	NewCourseWizard(strCourse, strLayoutName);
	
	CString strPath = MACRO_COURSE_PATH + strCourse;

	CString strPPT = strPPTPath;
	CString strFile = ::GetFileName(strPPT);
	CString strNewPath = strPath + "\\" + strFile;
	::SafeCopyFile(strPPT, strNewPath);

	CString strIniPath = MACRO_INI_FILE(strCourse);
	::WritePrivateProfileString("General", "PPTFile", strFile, strIniPath);
}

void CELearningView::WriteDevice(CString strCourse, CString strAudio, CString strVideoType, CString strVideo) 
{
	CString strType = "-";
	int nType = atoi(strVideoType);
	if (nType == 1)
		strType = "DV";
	else if (nType == 2)
		strType = "CAM";
	CString strIniPath = MACRO_INI_FILE(strCourse);

	::WritePrivateProfileString("Device", "Type", strType, strIniPath);
	::WritePrivateProfileString("Device", "Video", strVideo, strIniPath);
	::WritePrivateProfileString("Device", "Audio", strAudio, strIniPath);

	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("Device", "ScreenAudio", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("Device", "ScreenAudio", szBuf, strIniPath);
}

void CELearningView::OnUpdateDelCourse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BUNDLE_VERSION && !BACKEND_VERSION);
}

void CELearningView::OnUpdateBkupCourse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BUNDLE_VERSION && !BACKEND_VERSION);
}

void CELearningView::OnUpdateRestoreCourse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BUNDLE_VERSION && !BACKEND_VERSION);
}

void CELearningView::OnUpdateBackupMaintain(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!EV_VERSION && !BUNDLE_VERSION && !BACKEND_VERSION);
}

void CELearningView::OnOpen() 
{
	//add by buxiangyi
	CString CourseName;
	CourseName = MACRO_MAINFRM-> GetCourse();
	if (CourseName != "")
	{
		//m_pAdapter->SaveVideo();
	}
	OpenFileFlag = TRUE;
	NewCourseFlag = FALSE;
	MACRO_MAINFRM->CollectCourse();
	CDlgOpenCourse dlg;
	//wangwang20090826
	if (dlg.DoModal() == IDOK)
	{
		if (CourseName != "")
		{
			MACRO_MAINFRM->m_bAskJmp = TRUE;
			if (W_ASK_OKCANCEL(IDS_ASK_JUMP_TO_EDIT, this) != IDOK)
			{
				MACRO_MAINFRM->m_bAskJmp = FALSE;
				return;
			}
			MACRO_MAINFRM->LoadCourse("");
		}
		CString strPath = MACRO_COURSE_PATH + dlg.m_strCourse;
		if (!::IsFileWritable(strPath))
	 	  ::SetTreeWritable(strPath);
		CString strTitle  = "";
#ifdef _KDI
		strTitle.LoadString(IDR_MAINFRAME_KDI);
#else
		strTitle.LoadString(IDR_MAINFRAME);
#endif
		MACRO_MAINFRM->SetWindowText(strTitle + "-" + dlg.m_strCourse);
		CString strTmp;
		strTmp.LoadString(IDS_PROCESSING);
		MACRO_MAINFRM->SetProgress(strTmp, strTmp);
		MACRO_MAINFRM->ShowProgress();
		MACRO_MAINFRM->LoadCourse(dlg.m_strCourse, TRUE);
		MACRO_MAINFRM->HideProgress();
		//MACRO_MAINFRM->SendMessage(WM_NEW_COURSE);	//OLD COMMENT
	}
	//MACRO_MAINFRM->LoadCourse(TRUE);

	//add by buxiangyi
	CStringArray strPPtLayout;
	GetPPTLayout(strPPtLayout);

	int index = m_pAdapter ->GetArrayLayoutNameSize();
	if (strPPtLayout.GetSize() == 0)
	{
		NewArrayLayout();
	}
	else
	{
		CString strDefaultLayout = GetDefaultLayout();
		if (strPPtLayout.GetSize() < index)
			for (int i = strPPtLayout.GetSize(); i < index; i++)
				strPPtLayout.Add(strDefaultLayout);
		m_pAdapter ->m_strArrayLayoutName.RemoveAll();
		for (int i = 0; i < index; i++)
		{
			m_pAdapter ->m_strArrayLayoutName.Add(strPPtLayout[i]);
		}
	}
	
	
	//MACRO_MAINFRM->SendMessage(OnNewCourse, 0);
}
void CELearningView::doWizard()
{
    OnNewWizard();
  //  PostMessage(ID_NEW_WIZARD);
}

//add 2013/11/26 kagata BugNo.26
void CELearningView::fontFix(CString strCoursePath,CString strFileName)
{
	/*CString strSrcPath = strCoursePath + "html\\" + strFileName;
	BOOL ret;
	CString strLine;
	CString strMessage;
	CString strReMessage;
	CStdioFile cFile;
	int stFont;
	int enFont;
	int bottomPos;
	int err = -1;
	CString zeropx;
	CStringArray strsTag, strsReplace;

	ret = cFile.Open(_T(strSrcPath),CFile::modeRead | CFile::shareDenyNone);

	strsTag.RemoveAll();
	strsReplace.RemoveAll();

	if(ret == FALSE)
	{
		AfxMessageBox("HTMLÉtÉ@ÉCÉãÇÃèëÇ´ä∑Ç¶Ç…é∏îsÇµÇ‹ÇµÇΩÅB\n\nèàóùèIóπå„Ç…ÉRÅ[ÉXÇçƒçÏê¨ÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB");
		return;
	}

	while(ret)
	{
		ret = cFile.ReadString(strLine);
		strMessage = strLine;
		strReMessage = strLine;
		stFont = 0;

		while(0 <= stFont)
		{
			int cline = strReMessage.Find("font:",stFont);
			if(cline > 0 && cline != stFont)
			{
				bottomPos = strReMessage.Find("bottom:",cline);
				enFont = strReMessage.Find("\">",cline);
				if(enFont < 0 || bottomPos < 0)
				{
					break;
				}

				if(bottomPos < enFont)
				{
					int nPos = bottomPos + 7;
					int dPos = nPos + 4;
					int aPos = enFont + 4 - dPos;

					//AfxMessageBox(strReMessage);

					strReMessage.Insert(nPos,("0px\;"));
					strReMessage.Delete(dPos, aPos);

					//AfxMessageBox(strReMessage);

					strsTag.Add(strMessage);
					strsReplace.Add(strReMessage);
				}

				strMessage = strReMessage;

				stFont = cline + 1;
			}
			else
			{
				break;
			}
		}
	}
	cFile.Close();

	UpdateFileByTag(strCoursePath + "html", strFileName, strsTag, strsReplace, TRUE);*/
}


void CELearningView::OnEditPpt()
{
	// TODO: Add your command handler code here
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return ;

	//add 2013/11/26 kagata BugNo.90
	CWnd* pMainFrm = AfxGetMainWnd();
	OpenPptFile();

	return ;
}


void CELearningView::OnChangePpt()
{
	// TODO: Add your command handler code here
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return ;

	//add 2013/11/26 kagata BugNo.90
	CWnd* pMainFrm = AfxGetMainWnd();

	ReplacePptFile();
	//add 2013/11/26 kagata BugNo.90
	pMainFrm->UpdateWindow();

	return ;
}


void CELearningView::OnRegenPpt()
{
	// TODO: Add your command handler code here
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return ;

	//add 2013/11/26 kagata BugNo.90
	CWnd* pMainFrm = AfxGetMainWnd();

	RegenPptSlide(TRUE);
		//add 2013/11/26 kagata BugNo.90
	pMainFrm->UpdateWindow();

	return ;
}


void CELearningView::OnSaveFile()
{
	// TODO: Add your command handler code here
	CDlgCourseFolder dlg;
	dlg.DoModal();
}


void CELearningView::OnHelp()
{
	// TODO: Add your command handler code here
	CString m_PDFPath;
	HINSTANCE m_hInstance;
	m_PDFPath = MACRO_PDF_PATH;
	if (!::FileExists(m_PDFPath))
	{
		CString strTmp;
		strTmp.LoadString(IDC_PDF_FILE_NULL);
		W_SHOW_ERROR_STR(strTmp, this);
		return ;
	}
	m_hInstance = ShellExecute(NULL,"open",m_PDFPath,NULL,NULL,SW_SHOWNORMAL);
	if ((int)m_hInstance < 32)
	{
		CString strTmp;
		strTmp.LoadString(IDC_DEVICE_NULL);
		W_SHOW_ERROR_STR(strTmp, this);
		return ;
		
	}
}
void CELearningView::OnPBMarkUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_MARK, 0);

	return ;
}

void CELearningView::OnPBDeleteUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_DELETE, 0);

	return ;
}

void CELearningView::OnPBMuteUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_MUTE, 0);

	return ;
}

void CELearningView::OnPBReplaceUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_REPLACE, 0);

	return ;
}

void CELearningView::OnPBEditMarkUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_EDITMARK, 0);

	return ;
}

void CELearningView::OnPBAddEventUp()
{
	/*if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EVENT, 0);*/

	return ;
}

void CELearningView::OnPBZoomInUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EVENT, ID_ADD_ZOOM_IN);

	return ;
}

void CELearningView::OnPBZoomOutUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EVENT, ID_ADD_ZOOM_OUT);

	return ;
}

void CELearningView::OnPBAddHtmlUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EVENT, ID_ADD_HTML);

	return ;
}

void CELearningView::OnPBAddSCUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EVENT, ID_ADD_SCREEN_CAPTURE);

	return ;
}

void CELearningView::OnPBSaveVideo()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_SAVE_VIDEO, 0);

	return ;
}

void CELearningView::OnPBAddWave()
{
	/*if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_WAVE, 0);*/

	return ;
}

void CELearningView::OnPBAddAR()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_WAVE, ID_ADD_AUDIO_REPLACE);

	return ;
}

void CELearningView::OnPBAddAM()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_WAVE, ID_ADD_AUDIO_MIX);

	return ;
}

void CELearningView::OnPBAddText()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_TEXT, 0);

	return ;
}

void CELearningView::OnPBAddEffectUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADD_EFFECT, 0);

	return ;
}

void CELearningView::OnPBSetPoints()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_SET_POINTS, 0);

	return ;
}

void CELearningView::OnPBAdjustUp()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_ADJUST, 0);

	return ;
}


void CELearningView::OnCourseDefaultLayout()
{
	// TODO: Add your command handler code here
	Stop();

	CString strTitle;
	strTitle.LoadString(IDC_PROPERTY);

	CCourseSheetDefaultLayout shCourseLayout(strTitle);
	shCourseLayout.m_pFont = MACRO_SYS_FONT2;
	shCourseLayout.m_CourseSheetDefaultFlag = FALSE;
	shCourseLayout.m_nCurSel = 0;
	shCourseLayout.DoModal();

}


void CELearningView::OnUpdateCourseDefaultLayout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(HasCourseSelected() && !BACKEND_VERSION);
}


void CELearningView::OnCoursePageDefaultLayout()
{
	// TODO: Add your command handler code here
	CourseLayout dlg;
	CString coursename;
	coursename = MACRO_MAINFRM->GetCourse();
	if (m_pAdapter ->m_RowIndex == -1)
	{
		m_pAdapter ->m_RowIndex = 0;
		dlg.m_RowIndex = m_pAdapter ->m_RowIndex;
	} 
	else
	{
		dlg.m_RowIndex = m_pAdapter ->m_RowIndex;
	}

	for (int i = 0; i < m_pAdapter ->m_strArrayLayoutName.GetSize(); i++)
	{
		dlg.m_CourseLayoutNumber.Add(m_pAdapter ->m_strArrayLayoutName[i]);
	}

	dlg.Layoutflag = FALSE;
	if (coursename != "")
	{
		dlg.DoModal();
		m_pAdapter ->m_strArrayLayoutName.RemoveAll();
		for (int i = 0; i < dlg.m_CourseLayoutNumber.GetSize(); i++)
		{
			m_pAdapter ->m_strArrayLayoutName.Add(dlg.m_CourseLayoutNumber[i]);
		}
		
	}
	
}

//add by buxiangyi
BOOL CELearningView::SaveScreenParamsToLocal(BOOL bCheckDevice) 
{
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	while (1)
	{
		::GetPrivateProfileString("Profile", "Param1", "", szBuf, 256, strSysIniPath);
		int nVideoFreq = atoi(szBuf);
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
		if (szBuf[0] == '\0' && nVideoFreq != 0 && bCheckDevice)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return FALSE;
		}
		else
			break;
	}	

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_s_profile.ini";

	::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("Profile", "Name", szBuf, strIniPath);
	int i = 0;
	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("Profile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("Profile", strParam, szBuf, strIniPath);
	}

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("ScreenProfile", "Name", szBuf, strIniPath);

	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("ScreenProfile", strParam, szBuf, strIniPath);
	}

	return TRUE;
}

BOOL CELearningView::SaveRecordParamsToLocal(BOOL bCheckDevice) 
{
	CString strSysIniPath = MACRO_INI_PATH;
	char szBuf[256];
	while (1)
	{
		::GetPrivateProfileString("Profile", "Param1", "", szBuf, 256, strSysIniPath);
		int nVideoFreq = atoi(szBuf);
		::GetPrivateProfileString("Device", "Video", "", szBuf, 256, strSysIniPath);
		if (szBuf[0] == '\0' && nVideoFreq != 0 && bCheckDevice)
		{
			W_SHOW_ERROR(IDS_DEV_NO_VIDEO, this);
			CDevice dlg;
			if (dlg.DoModal() == IDCANCEL)
				return FALSE;
		}
		else
			break;
	}	

	CString strTempPath =  ::GetSysPath() + "temp";
	if (!::FileExists(strTempPath))
		::TryToMkDir(strTempPath);
	CString strIniPath = strTempPath + "\\temp_profile.ini";

	::GetPrivateProfileString("Profile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("Profile", "Name", szBuf, strIniPath);
	int i = 0;
	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("Profile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("Profile", strParam, szBuf, strIniPath);
	}

	::GetPrivateProfileString("ScreenProfile", "Name", "", szBuf, 256, strSysIniPath);
	::WritePrivateProfileString("ScreenProfile", "Name", szBuf, strIniPath);

	for (i = 1; i < 7; i++)
	{
		CString strParam;
		strParam.Format("Param%d", i);
		::GetPrivateProfileString("ScreenProfile", strParam, "", szBuf, 256, strSysIniPath);
		::WritePrivateProfileString("ScreenProfile", strParam, szBuf, strIniPath);
	}

	return TRUE;
}

void CELearningView::NewArrayLayout()
{
	//add by buxiangyi
	CString strTmp, coursename;
	coursename =  MACRO_MAINFRM->GetCourse();
	strTmp = MACRO_JS_PATH(coursename);
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

	m_pAdapter ->m_strArrayLayoutName.RemoveAll();
	int index = m_pAdapter ->GetArrayLayoutNameSize();
	for (int i = 0; i < index; i++)
	{
		m_pAdapter ->m_strArrayLayoutName.Add(m_LayoutName);
	}
}
void CELearningView::GetPPTLayout(CStringArray &m_strArrayPPtName)
{
	m_strArrayPPtName.RemoveAll();
	CString strTmp,CourseName;
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
		if (strItem.Find("new hScriptList") >= 0)
		{
			int EndIndex = strItem.Find(")");
			CString ScriptTable;
			ScriptTable = strItem.Mid(EndIndex - 1, 1);
			if (ScriptTable == "1")
			{
				m_LayoutName = strItem.Mid(EndIndex - 7, 3);
				m_strArrayPPtName.Add(m_LayoutName);
			}
		}
	}
}


CString CELearningView::GetIndexPPTLayout()
{
	return m_pAdapter ->m_strArrayLayoutName[m_pAdapter -> m_RowIndex];
}

void CELearningView::SaveConfig(CString strCourse)
{
	m_pAdapter->SaveConfig(strCourse);
}

int CELearningView::GetPPTIndex()
{
	return m_pAdapter -> m_RowIndex;
}

void CELearningView::OnUpdateEditPPT(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnUpdateChangePPT(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

void CELearningView::OnUpdateRegenPPT(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}

CStringArray* CELearningView::GetDrawTime()
{
	return m_pAdapter->GetDrawTime();
}

CStringArray* CELearningView::GetDrawColor()
{
	return m_pAdapter->GetDrawColor();
}

CStringArray* CELearningView::GetDrawWidth()
{
	return m_pAdapter->GetDrawWidth();
}

CStringArray* CELearningView::GetDrawCmd()
{
	return m_pAdapter->GetDrawCmd();
}

CStringArray* CELearningView::GetDrawLeft()
{
	return m_pAdapter->GetDrawLeft();
}

CStringArray* CELearningView::GetDrawTop()
{
	return m_pAdapter->GetDrawTop();
}

CStringArray* CELearningView::GetDrawRight()
{
	return m_pAdapter->GetDrawRight();
}

CStringArray* CELearningView::GetDrawBottom()
{
	return m_pAdapter->GetDrawBottom();
}

void CELearningView::DeleteDrawPoint(int nIndex)
{
	m_pAdapter->DeleteDrawPoint(nIndex);
}

CString CELearningView::GetDefaultLayout()
{
	CString strTmp, coursename;
	coursename =  MACRO_MAINFRM->GetCourse();
	strTmp = MACRO_JS_PATH(coursename);
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

void CELearningView::EditDrawMark()
{
	if (m_pAdapter != NULL)
		m_pAdapter->SendMessage(WM_PALETTE_BTN_UP, ID_BTN_EDITMARK, 3);

	return ;
}

void CELearningView::MergeVideo(CString strSeg, CString strCourse)
{
	m_pAdapter->MergeVideo(strSeg, strCourse);
}

BOOL CELearningView::TestVideoFile(CString strVideoPath)
{
	return m_pAdapter->TestVideoFile(strVideoPath);
}

void CELearningView::OnAudioFix() 
{
	CDlgAuFixWarning dlg;
	dlg.DoModal();
}

void CELearningView::OnUpdateAudioFix(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(HasCourseSelected());
}