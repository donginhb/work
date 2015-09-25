// PageDefaultImage.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageDefaultLayout.h"
#include "DlgTreeSelect.h"
#include "cmkapi.h"
#include "macro.h"
#include "ImageCell.h"
#include "mainfrm.h"
//add by buxiangyi
#include "FileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef constLayoutWidth
#define constLayoutWidth	270
#define constLayoutHeight	222
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultLayout property page

IMPLEMENT_DYNCREATE(CPageDefaultLayout, CPropertyPage)

CPageDefaultLayout::CPageDefaultLayout() : CPropertyPage(CPageDefaultLayout::IDD)
{
	//{{AFX_DATA_INIT(CPageDefaultLayout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
}

CPageDefaultLayout::~CPageDefaultLayout()
{
}

void CPageDefaultLayout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDefaultLayout)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDefaultLayout, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDefaultLayout)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CELL_CMD, OnCellCmd)
	ON_MESSAGE(WM_IMAGE_CELL_DBL_CLICK, OnCellDblClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultLayout message handlers

BOOL CPageDefaultLayout::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pLayoutCell = new CImageCell;
	m_pLayoutCell->m_nMenuID = IDR_MENU_CHANGE_BMP;
	m_pLayoutCell->m_bShowSize = TRUE;
	m_pLayoutCell->m_pFont = MACRO_SYS_FONT2;
	m_pLayoutCell->m_rgbTitleBg = RGB(0, 0, 128);
	m_pLayoutCell->m_rgbTitleFg = RGB(255, 255, 255);

	CString strIniPath = MACRO_INI_PATH;
	char szBuf[256];
	::GetPrivateProfileString("General", "LayoutName", "", szBuf, 256, strIniPath);
	

	CRect rc;
	GetClientRect(rc);

	CRect rcImageCell;
	rcImageCell.left = (rc.Width() - constLayoutWidth) / 2;
	rcImageCell.top = (rc.Height() - constLayoutHeight) / 2;
	rcImageCell.right = rcImageCell.left + constLayoutWidth + 2;
	rcImageCell.bottom = rcImageCell.top + constLayoutHeight + 2;
	m_pLayoutCell->Create(WS_CHILD | WS_VISIBLE, rcImageCell, this, ID_LAYOUT_CELL);

	CString strTmp = MACRO_TEMPLATE_PATH;
	CString TemstrPath, m_LayoutName, m_LayoutNameTitle;
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
	m_LayoutNameTitle = "layout" + m_LayoutName;
	m_LayoutName = "layout" + m_LayoutName + ".bmp";
	CString strLayoutPath = strTmp + m_LayoutName;
	if (!::FileExists(strLayoutPath))
		strLayoutPath = ::GetSysPath() + "empty_layout.bmp";
	m_pLayoutCell->SetPath(strLayoutPath);
	m_pLayoutCell->SetTitle(m_LayoutNameTitle);
	//add by buxiangyi
	strOldName = m_LayoutName;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageDefaultLayout::SelectLayout()
{
	CDlgTreeSelect dlg;
	dlg.m_strDefaultPath = MACRO_LAYOUT_PATH;
	dlg.m_bSelectable = FALSE;
	CString strTitle;
	strTitle.LoadString(IDS_LAYOUT_SELECT);
	dlg.m_strTitle = strTitle;
	dlg.m_bResize = TRUE;
	dlg.ShowFileName(TRUE);
	dlg.ShowFileExt(FALSE);
	dlg.SetShowType(2);
	dlg.m_nWidth = 500;
	dlg.m_nHeight = 515;
	dlg.m_pPageLayout = TRUE;
	dlg.m_strShow = "選択された画像ファイル:";
	CString strName;
//	MACRO_MAINFRM->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
//								SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	if(dlg.DoModal() == IDOK)
	{
		CString strTmp = MACRO_TEMPLATE_PATH;
		
		CString strPath = dlg.m_strSelectedFile;

		::CopyFile(strPath, strTmp + "_slayout.bmp", FALSE);
	
		CString strFile = ::GetFileName(strPath);
		int nSize = strFile.GetLength();
		int nTotal = strPath.GetLength();
		strPath = strPath.Left(nTotal - nSize);
		CString strLayoutPath = strTmp + strFile;

		CString strIndex = strFile.Mid(6, nSize - 10);
		//CString strSrcPath = strPath + "l_layouts\\l_layout" + strIndex + ".bmp";
		CString strSrcPath = strPath + "l_layouts\\l_" + strFile;

		int nIndex = strFile.ReverseFind('.');
		strName = strFile.Left(nIndex);

		if (Layoutflag == FALSE)
		{
			CString Path =  MACRO_TEMPLATE_PATH + strOldName;
			
			DeleteFile(Path);
			::CopyFile(strSrcPath, strLayoutPath, FALSE);
		} 
		else
		{
			CString Path =  MACRO_TEMPLATE_PATH  +strPreName;
		
			DeleteFile(Path);
			::CopyFile(strSrcPath, strLayoutPath, FALSE);
		}
		strPreName = strFile;


		CString strIniPath = MACRO_INI_PATH;
		::WritePrivateProfileString("General", "LayoutName", strName, strIniPath);
		m_pLayoutCell->SetTitle(strName);

		m_pLayoutCell->SetPath(strLayoutPath);

		CStringArray strArray, strOldArray;
		strPath = MACRO_TEMPLATE_PATH + "js\\";
		CString str;
		str = strName.Mid(6);
		str = "\"0" + str + "\";";
		str = "var layoutNo = " + str;
		strOldArray.Add("var layoutNo = ");
		strArray.Add(str);
		UpdateFileByTag(strPath, "config.js", strOldArray, strArray, TRUE);
	}
	
	Layoutflag = TRUE;
	return TRUE;
}

void CPageDefaultLayout::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	m_pLayoutCell->DestroyWindow();
	delete m_pLayoutCell;
}

LRESULT CPageDefaultLayout::OnCellCmd(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_LAYOUT_CELL && lParam == ID_CHANGE_BMP)
		SelectLayout();

	return 0L;
}

LRESULT CPageDefaultLayout::OnCellDblClicked(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_LAYOUT_CELL)
		SelectLayout();

	return 0L;
}

BOOL CPageDefaultLayout::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageDefaultLayout::OnOK() 
{
	CPropertyPage::OnOK();
}

void CPageDefaultLayout::OnCancel() 
{
	CPropertyPage::OnCancel();
}
