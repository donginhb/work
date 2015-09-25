// CourseLayout.cpp : implementation file
//
#include "stdafx.h"
#include "elearning.h"
#include "CourseLayout.h"
#include "DlgTreeSelect.h"
#include "cmkapi.h"
#include "macro.h"
#include "ImageCell.h"
#include "mainfrm.h"
#include "FileApi.h"
#include "resource.h"
#include "eLearning.h"
#include "eLearningView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef constLayoutWidth
#define constLayoutWidth	270
#define constLayoutHeight	222
#endif

// CourseLayout dialog

IMPLEMENT_DYNAMIC(CourseLayout, CDialog)

CourseLayout::CourseLayout(CWnd* pParent /*=NULL*/)
	: CDialog(CourseLayout::IDD, pParent)
{
	m_bCanGetActive = FALSE;
}

CourseLayout::~CourseLayout()
{
}

void CourseLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CourseLayout, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CELL_CMD, OnCellCmd)
	ON_MESSAGE(WM_IMAGE_CELL_DBL_CLICK, OnCellDblClicked)
END_MESSAGE_MAP()


// CourseLayout message handlers
BOOL CourseLayout::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pLayoutCell = new CImageCell;
	m_pLayoutCell->m_nMenuID = IDR_MENU_CHANGE_BMP;
	m_pLayoutCell->m_bShowSize = TRUE;
	m_pLayoutCell->m_pFont = MACRO_SYS_FONT2;
	m_pLayoutCell->m_rgbTitleBg = RGB(0, 0, 128);
	m_pLayoutCell->m_rgbTitleFg = RGB(255, 255, 255);


	CString m_CourseName = MACRO_MAINFRM->GetCourse();
	if (m_CourseName == "")
	{
		return FALSE;
	}
	CString strIniPath = MACRO_INI_FILE(m_CourseName);
	char szBuf[256];
	::GetPrivateProfileString("General", "LayoutName", "", szBuf, 256, strIniPath);

	CRect rc;
	GetClientRect(rc);

	CRect rcImageCell;
	rcImageCell.left = (rc.Width() - constLayoutWidth) / 2;
	rcImageCell.top = (rc.Height() - constLayoutHeight) / 4;
	rcImageCell.right = rcImageCell.left + constLayoutWidth + 2;
	rcImageCell.bottom = rcImageCell.top + constLayoutHeight + 2;
	m_pLayoutCell->Create(WS_CHILD | WS_VISIBLE, rcImageCell, this, ID_LAYOUT_CELL);

	CString strTmp = MACRO_JS_PATH(m_CourseName);

	CString m_LayoutName, m_LayoutNameTitle;
	CStringArray strArray;
	
	CString TemstrPath;
	TemstrPath = strTmp + "config.js";
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
	
	
	layoutNo = m_LayoutName.Right(2);
	m_LayoutName = MACRO_VIEW -> GetIndexPPTLayout();
		
	
	m_LayoutName = m_LayoutName.Right(2);
	m_LayoutNameTitle = "layout" + m_LayoutName;
	m_LayoutName = "layout" + m_LayoutName + ".bmp";
	CString strLayoutPath = strTmp + m_LayoutName;

	CString srcPath,dectPath;
	srcPath = MACRO_LAYOUT_PATH + "l_Layouts\\l_" + m_LayoutName;
	dectPath =  MACRO_HTML_DIR(m_CourseName) + m_LayoutName;
	::SafeCopyFile(srcPath, dectPath, FALSE);

	if (!::FileExists(strLayoutPath))
		strLayoutPath = ::GetSysPath() + "empty_layout.bmp";
	m_pLayoutCell->SetPath(strLayoutPath);
	m_pLayoutCell->SetTitle(m_LayoutNameTitle);


	strOldName = m_CourseLayoutNumber[m_RowIndex];
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CourseLayout::SelectLayout()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return TRUE;

	//	MACRO_MAINFRM->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
	//								SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = MACRO_LAYOUT_PATH;
	dlg.m_bSelectable = FALSE;
	CString strTitle;
	strTitle.LoadString(IDS_LAYOUT_SELECT);
	dlg.m_strTitle = "レイアウトを選択";//strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(TRUE);
	dlg.ShowFileExt(FALSE);
	dlg.SetShowType(2);
	dlg.m_nWidth = 500;
	dlg.m_nHeight = 515;
	dlg.m_strShow = "選択されたレイアウト:";
	if(dlg.DoModal() == IDOK)
	{
	
		CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
		if (!::TryToMkDir(strHTML))
			return FALSE;

		
		CString strPath = dlg.m_strSelectedFile;

		CString strFile = ::GetFileName(strPath);
		int nSize = strFile.GetLength();
		int nTotal = strPath.GetLength();
		strPath = strPath.Left(nTotal - nSize);

		//CString strIndex = strFile.Mid(6, nSize - 10);
		CString strSrcPath = strPath + "l_layouts\\l_" + strFile;
		CString strLayoutPath = strHTML  +strFile;
		int nIndex = strFile.ReverseFind('.');
		strName = strFile.Left(nIndex);
		CString str;
		str = strName.Mid(6);
		str = "0" + str ;
		m_CourseLayoutNumber[m_RowIndex] = str;



		if (Layoutflag == FALSE)
		{
			BOOL NameFlag = FALSE;
			CString Path =  MACRO_COURSE_PATH + strCourse + "\\html\\" + "layout" +strOldName.Right(2) + ".bmp";
			CString name = strOldName.Right(2);
			for (int i = 0; i < m_CourseLayoutNumber.GetSize(); i++)
			{
				CString str;
				str = m_CourseLayoutNumber[i].Right(2);
				
				if (name.Find(str) >= 0)
				{
					NameFlag = TRUE;
				}
			}
			if (NameFlag == FALSE)
			{
				if (name.Find(layoutNo) < 0)
				{
					DeleteFile(Path);
				}
				
			}
			::CopyFile(strSrcPath, strLayoutPath, FALSE);
		} 
		else
		{
			BOOL NameFlag = FALSE;
			CString Path =  MACRO_COURSE_PATH + strCourse + "\\html\\" +strPreName;
			for (int i = 0; i < m_CourseLayoutNumber.GetSize(); i++)
			{
				CString str;
				str = m_CourseLayoutNumber[i].Right(2);
				CString name = strPreName.Mid(6,2);
				if (name.Find(str) >= 0)
				{
					NameFlag = TRUE;
				}
			}
			if (NameFlag == FALSE)
			{
				if (strPreName.Find(layoutNo) < 0)
				{
					DeleteFile(Path);
				}
			}
			
			::CopyFile(strSrcPath, strLayoutPath, FALSE);
		}
		strPreName = strFile;

		

		m_pLayoutCell->SetTitle(strName);
		m_pLayoutCell->SetPath(strLayoutPath);
		Layoutflag = TRUE;

	}

	return TRUE;
}

void CourseLayout::OnDestroy() 
{
	CDialog::OnDestroy();

	m_pLayoutCell->DestroyWindow();
	delete m_pLayoutCell;
}

LRESULT CourseLayout::OnCellCmd(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_LAYOUT_CELL && lParam == ID_CHANGE_BMP)
		SelectLayout();

	return 0L;
}

LRESULT CourseLayout::OnCellDblClicked(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_LAYOUT_CELL)
		SelectLayout();

	return 0L;
}

BOOL  DeleteDirectoryFile(CString DirectoryPath,CString FileName)   
{     
	CFileFind finder;  
	CString path;  
	path.Format("%s*.*",DirectoryPath);  
	BOOL bWorking = finder.FindFile(path);  
	if (bWorking == FALSE)
	{
		return FALSE;
	}
	while(bWorking)
	{  
		bWorking = finder.FindNextFile();  
		if(finder.IsDirectory() || finder.IsDots())
		{
			continue;
		}  
		else
		{ 
			CString FilePath;
			FilePath = finder.GetFilePath();
			if (FilePath.Find(FileName.Left(5)) > 0 && FilePath.Find(".bmp") > 0)
			{
				if (FilePath.Find(FileName.Mid(6)) < 0)
				{
					DeleteFile(finder.GetFilePath());  
				}
			}

		}  
	} 
	return TRUE;
} 


void CourseLayout::OnOK() 
{
	int m_over;
	pBtn = (CButton*)GetDlgItem(IDC_CHECK1);
	m_over = pBtn->GetCheck();
	CString strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	if (m_over)
	{
		if (W_ASK_YESNO(IDS_SURE_TO_APPLE_THE_CHANGE, this) != IDYES)
			return;
		CString str;
		str = strName.Mid(6);
		str = "0" + str ;
		for (int i = 0; i < m_CourseLayoutNumber.GetSize(); i++)
		{
			m_CourseLayoutNumber[i] = str;
		}
		CString  strFileName;
		strFileName = strName +".bmp";
		DeleteDirectoryFile(strPath, strFileName);

		CStringArray strArray, strOldArray;
		str = strName.Mid(6);
		str = "\"0" + str + "\";";
		str = "var layoutNo = " + str;
		strOldArray.Add("var layoutNo = ");
		strArray.Add(str);
		UpdateFileByTag(strPath, "config.js", strOldArray, strArray, TRUE);
		MACRO_VIEW -> NewArrayLayout();

	}
	else if (MACRO_VIEW -> GetPPTIndex() == 0)
	{
		CString str;
		str = strName.Mid(6);
		str = "0" + str ;
		CStringArray strArray, strOldArray;
		str = strName.Mid(6);
		str = "\"0" + str + "\";";
		str = "var layoutNo = " + str;
		strOldArray.Add("var layoutNo = ");
		strArray.Add(str);
		UpdateFileByTag(strPath, "config.js", strOldArray, strArray, TRUE);
	}

	CDialog::OnOK();
}

void CourseLayout::OnCancel() 
{
	CDialog::OnCancel();
}

