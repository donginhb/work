// PageDefaultImage.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageDefaultImage.h"
#include "macro.h"
#include "ImageCell.h"
#include "cmkapi.h"
#include "DlgTreeSelect.h"
#include "jpgapi.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultImage property page

IMPLEMENT_DYNCREATE(CPageDefaultImage, CPropertyPage)

CPageDefaultImage::CPageDefaultImage() : CPropertyPage(CPageDefaultImage::IDD)
{
	//{{AFX_DATA_INIT(CPageDefaultImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
	m_nSel = -1;
	m_PageDefaultImageFlag = TRUE;
}

CPageDefaultImage::~CPageDefaultImage()
{
}

void CPageDefaultImage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDefaultImage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDefaultImage, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDefaultImage)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_IMAGE_ITEM, OnSelchangeListImageItem)
	ON_MESSAGE(WM_CELL_CMD, OnCellCmd)
	ON_MESSAGE(WM_IMAGE_CELL_DBL_CLICK, OnCellDblClicked)
	ON_BN_CLICKED(IDC_RADIO_LARGE, OnRadioLarge)
	ON_BN_CLICKED(IDC_RADIO_MEDIUM, OnRadioMedium)
	ON_BN_CLICKED(IDC_RADIO_SMALL, OnRadioSmall)
	ON_BN_CLICKED(IDC_RADIO_EXLARGE, OnRadioExlarge)
	ON_BN_CLICKED(IDC_RADIO_HUGE, OnRadioHuge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultImage message handlers

BOOL CPageDefaultImage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CWordArray idArray;
	idArray.Add(IDS_LOGO);
	idArray.Add(IDS_BG_IMAGE);
	//add by buxiangyi
	//idArray.Add(IDS_TV_IMAGE);
	//////////////////////////
	idArray.Add(IDS_ADVISOR_PHOTO);
	
	CListBox* pBox = (CListBox*)GetDlgItem(IDC_LIST_IMAGE_ITEM);
	for (int i = 0; i < idArray.GetSize() - 1; i++)
	{
		CString str;
		str.LoadString(idArray[i]);
		pBox->AddString(str);
	}
	pBox->AddString("動画プレイヤーの画像");

	m_pImageCell = new CImageCell;
	m_pImageCell->m_bWithFrame = TRUE;
	m_pImageCell->m_nMenuID = IDR_MENU_CHANGE_BMP;
	m_pImageCell->m_pFont = MACRO_SYS_FONT2;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc;
	pBox->GetWindowRect(rc);
	ScreenToClient(rc);
	CRect rect = rc;
	rect.left = rc.right + 10;
	rect.right = rcClient.right - rc.left;
	m_pImageCell->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_IMAGE_CELL);

	pBox->SetCurSel(0);
	if (m_PageDefaultImageFlag == TRUE)
	{
		SetImage();
	} 
	else
	{
		SetCourseImage();
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDefaultImage::SetTVImageLayout(BOOL bFlag)
{
	CRect rect, rc;
	if (bFlag)
	{
		GetDlgItem(IDC_STATIC_IMAGE)->GetWindowRect(rect);
		ScreenToClient(rect);
	}
	else
	{
		CRect rcClient;
		GetClientRect(rcClient);
		
		GetDlgItem(IDC_LIST_IMAGE_ITEM)->GetWindowRect(rc);
		ScreenToClient(rc);
		rect = rc;
		rect.left = rc.right + 10;
		rect.right = rcClient.right - rc.left;
	}

	m_pImageCell->MoveWindow(rect);

	int nCmd = (bFlag) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_STATIC_TV_SIZE)->ShowWindow(nCmd);
	GetDlgItem(IDC_RADIO_SMALL)->ShowWindow(nCmd);
	GetDlgItem(IDC_RADIO_MEDIUM)->ShowWindow(nCmd);
	GetDlgItem(IDC_RADIO_LARGE)->ShowWindow(nCmd);
	GetDlgItem(IDC_RADIO_EXLARGE)->ShowWindow(nCmd);
	GetDlgItem(IDC_RADIO_HUGE)->ShowWindow(nCmd);

	if (bFlag)
	{
		if (m_nSel < 0)
		{
			CString strIniPath = MACRO_INI_PATH;
			char szValue[256];
			::GetPrivateProfileString("General", "TVSize", "1", szValue, 255, strIniPath);
			m_nSel = atoi(szValue);
		}

		CButton* pRadioSmall = (CButton*)GetDlgItem(IDC_RADIO_SMALL);
		CButton* pRadioMedium = (CButton*)GetDlgItem(IDC_RADIO_MEDIUM);
		CButton* pRadioLarge = (CButton*)GetDlgItem(IDC_RADIO_LARGE);
		CButton* pRadioExLarge = (CButton*)GetDlgItem(IDC_RADIO_EXLARGE);
		CButton* pRadioHuge = (CButton*)GetDlgItem(IDC_RADIO_HUGE);
		if (m_nSel == 0)
		{
			pRadioSmall->SetCheck(1);
			pRadioMedium->SetCheck(0);
			pRadioLarge->SetCheck(0);
			pRadioExLarge->SetCheck(0);
			pRadioHuge->SetCheck(0);
		}
		else if (m_nSel == 1)
		{
			pRadioSmall->SetCheck(0);
			pRadioMedium->SetCheck(1);
			pRadioLarge->SetCheck(0);
			pRadioExLarge->SetCheck(0);
			pRadioHuge->SetCheck(0);
		}
		else if (m_nSel == 2)
		{
			pRadioSmall->SetCheck(0);
			pRadioMedium->SetCheck(0);
			pRadioLarge->SetCheck(1);
			pRadioExLarge->SetCheck(0);
			pRadioHuge->SetCheck(0);
		}
		else if (m_nSel == 3)
		{
			pRadioSmall->SetCheck(0);
			pRadioMedium->SetCheck(0);
			pRadioLarge->SetCheck(0);
			pRadioExLarge->SetCheck(1);
			pRadioHuge->SetCheck(0);
		}
		else if (m_nSel == 4)
		{
			pRadioSmall->SetCheck(0);
			pRadioMedium->SetCheck(0);
			pRadioLarge->SetCheck(0);
			pRadioExLarge->SetCheck(0);
			pRadioHuge->SetCheck(1);
		}
	}
}

void CPageDefaultImage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	m_pImageCell->DestroyWindow();
	delete m_pImageCell;
}

void CPageDefaultImage::OnSelchangeListImageItem() 
{
	if (m_PageDefaultImageFlag == TRUE)
	{
		SetImage();
	} 
	else
	{
		SetCourseImage();
	}
}

void CPageDefaultImage::SetImage()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_IMAGE_ITEM);
	int nSel = pList->GetCurSel();
	if (nSel != LB_ERR)
	{
		CString strText;
		pList->GetText(nSel, strText);
		
		CString strTemplate = MACRO_TEMPLATE_PATH;
		CString strPath;
		if (StrEqual(strText, IDS_LOGO))
		{
			strPath = strTemplate + "logo\\" + "logo.jpg";
			if (!::FileExists(strPath))
			{
				strPath = strTemplate + "logo\\" + "logo.gif";
				if (!::FileExists(strPath))
					strPath = "";
			}

			SetTVImageLayout(FALSE);
		}
		else if (StrEqual(strText, IDS_BG_IMAGE))
		{
			strPath = strTemplate + "res\\" + "_backgnd.jpg";
			SetTVImageLayout(FALSE);
		}
		//add by buxaingyi
		/*else if (StrEqual(strText, IDS_TV_IMAGE))
		{
		strPath = strTemplate + "_vidpanel.gif";	
		SetTVImageLayout(TRUE);
		}*/
		else if (strText == "動画プレイヤーの画像")//StrEqual(strText, IDS_ADVISOR_PHOTO))
		{
			strPath = strTemplate + "res\\" + "_advisor.jpg";	
			SetTVImageLayout(FALSE);
		}

		m_pImageCell->SetPath(strPath);
	}
}

BOOL CPageDefaultImage::StrEqual(CString strInput, UINT nID)
{
	CString str;
	str.LoadString(nID);

	strInput.TrimLeft();

	return (strInput == str);
}

LRESULT CPageDefaultImage::OnCellCmd(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_IMAGE_CELL && lParam == ID_CHANGE_BMP)
	{
		if (m_PageDefaultImageFlag == TRUE)
		{
			SelectImage();
		}else
		{
			SelectCourseImage();
		}
	}
	
	return 0L;
}

LRESULT CPageDefaultImage::OnCellDblClicked(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_IMAGE_CELL)
	{
		if (m_PageDefaultImageFlag == TRUE)
		{
			SelectImage();
		}else
		{
			SelectCourseImage();
		}
	}
		

	return 0L;
}

BOOL CPageDefaultImage::SelectImage()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_IMAGE_ITEM);
	int nSel = pList->GetCurSel();
	if (nSel == LB_ERR)
		return TRUE;

	CString strText;
	pList->GetText(nSel, strText);

	CString strTemplate = MACRO_TEMPLATE_PATH;

//	MACRO_MAINFRM->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
//								SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);

	CDlgTreeSelect dlg;
	if (StrEqual(strText, IDS_LOGO))
	{
		dlg.m_strDefaultPath = MACRO_LOGO_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_LOGO_SELECT);
		dlg.m_strTitle = strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetShowType(3);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;
		//addby buxiangyi
		dlg.m_pPageImageLogo = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strFile = dlg.m_strSelectedFile;
			CString strExt = ::GetFileExt(strFile);
			CString strSystemIni = MACRO_INI_PATH;
			if (strExt.CompareNoCase("gif") == 0)
			{
				CString strLogoPath = strTemplate + "logo\\" + "logo.gif";
				::CopyFile(strFile, strLogoPath, FALSE);
				::WritePrivateProfileString("General", "Logo", "logo.gif", strSystemIni);
				m_pImageCell->SetPath(strLogoPath);
				if (::FileExists(strTemplate + "logo\\" + "logo.jpg"))
					::DeleteFile(strTemplate + "logo\\" + "logo.jpg");
			}
			else
			{
				CString strLogoPath = strTemplate + "logo\\" + "logo.jpg";
				::WritePrivateProfileString("General", "Logo", "logo.jpg", strSystemIni);
				ConvertToJPG(dlg.m_strSelectedFile, strLogoPath);
				m_pImageCell->SetPath(strLogoPath);
				if (::FileExists(strTemplate + "logo\\" + "logo.gif"))
					::DeleteFile(strTemplate + "logo\\" + "logo.gif");
			}
		}
	}
	else if (StrEqual(strText, IDS_BG_IMAGE))
	{
		dlg.m_strDefaultPath = MACRO_BG_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_BG_SELECT);
		dlg.m_strTitle = strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetShowType(0);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;
		//add by buxiangyi
		dlg.m_pPageImageBackGround = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strBgPath = strTemplate + "res\\" + "_backgnd.jpg";
			ConvertToJPG(dlg.m_strSelectedFile, strBgPath);
			m_pImageCell->SetPath(strBgPath);
		}
	}
	else if (strText == "動画プレイヤーの画像")//StrEqual(strText, IDS_ADVISOR_PHOTO))
	{
		dlg.m_strDefaultPath = MACRO_PHOTO_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_PHOTO_SELECT);
		dlg.m_strTitle = "動画プレイヤーの画像を選択";//strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetShowType(0);
		dlg.SetFrameSize(112, 112);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;
		//add by buxiangyi
		dlg.m_pPageImagePhoto = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strPhotoPath = strTemplate + "res\\" + "_advisor.jpg";
			ConvertToJPG(dlg.m_strSelectedFile, strPhotoPath);
			m_pImageCell->SetPath(strPhotoPath);
		}
	}
	//add by buxaingyi
	//else if (StrEqual(strText, IDS_TV_IMAGE))
	//{
	//	dlg.m_strDefaultPath = MACRO_TV_PATH;
	//	dlg.m_bSelectable = FALSE;
	//	CString strTitle;
	//	strTitle.LoadString(IDS_TV_SELECT);
	//	dlg.m_strTitle = strTitle;
	//	dlg.m_bResize = TRUE;
	//	dlg.ShowFileName(TRUE);
	//	dlg.SetShowType(0);
	//	dlg.SetFrameSize(120, 120);
	//	dlg.m_nWidth = nScreenX * 2 / 3;
	//	dlg.m_nHeight = nScreenY * 2 / 3;
	//	if(dlg.DoModal() == IDOK)
	//	{
	//		CString strFileName = ::GetFileName(dlg.m_strSelectedFile);
	//		
	//		int nDot = strFileName.Find(".");
	//		CString strIndex = strFileName.Mid(2, nDot - 2);
	//		CString strPath = MACRO_TV_PATH + "TV" + strIndex;
	//		
	//		if (!::CopyDir(strPath, strTemplate))
	//			return FALSE;			

	//		CString strSrcPath = MACRO_TV_PATH + "TV" + strIndex + ".gif";
	//		CString strDestPath = strTemplate + "_vidpanel.gif";
	//		if (!::SafeCopyFile(strSrcPath, strDestPath))
	//			return FALSE;			

	//		CString strTVPath = strTemplate + "_vidpanel.gif";
	//		m_pImageCell->SetPath(strTVPath);
	//	}
	//}

	return TRUE;
}

BOOL CPageDefaultImage::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageDefaultImage::OnRadioLarge() 
{
	m_nSel = 2;
}

void CPageDefaultImage::OnRadioMedium() 
{
	m_nSel = 1;
}

void CPageDefaultImage::OnRadioSmall() 
{
	m_nSel = 0;
}

void CPageDefaultImage::OnOK() 
{
	if (m_nSel >= 0)
	{
		CString strIniPath = MACRO_INI_PATH;
		CString str;
		str.Format("%d", m_nSel);
		::WritePrivateProfileString("General", "TVSize", str, strIniPath);
	}

	CPropertyPage::OnOK();
}

void CPageDefaultImage::OnRadioExlarge() 
{
	m_nSel = 3;
}

void CPageDefaultImage::OnRadioHuge() 
{
	m_nSel = 4;
}
//add by buxiangyi
CString CPageDefaultImage::GetLogoFile(CString strCourse) 
{
	CString strIniPath = MACRO_INI_FILE(strCourse);

	char szBuf[256];
	::GetPrivateProfileString("General", "Logo", "logo.jpg", szBuf, 256, strIniPath);
	CString strLogo = szBuf;

	CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
	BOOL bJPGExist = ::FileExists(strHTML + "logo.jpg");
	BOOL bGIFExist = ::FileExists(strHTML + "logo.gif");

	if (bJPGExist && !bGIFExist)
		return "logo.jpg";
	else if (!bJPGExist && bGIFExist)
		return "logo.gif";
	else if (bJPGExist && bGIFExist)
	{
		if (strLogo.CompareNoCase("logo.jpg") == 0)
			::DeleteFile(strHTML + "logo.gif");
		else
			::DeleteFile(strHTML + "logo.jpg");

		return strLogo;
	}

	return "";
}

void CPageDefaultImage::SetCourseImage()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return;

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_IMAGE_ITEM);
	int nSel = pList->GetCurSel();
	if (nSel != LB_ERR)
	{
		CString strText;
		pList->GetText(nSel, strText);

		CString strPath = MACRO_COURSE_PATH + strCourse + "\\html\\";
		if (StrEqual(strText, IDS_LOGO))
		{
			strPath += GetLogoFile(strCourse); 
			SetTVImageLayout(FALSE);
		}
		else if (StrEqual(strText, IDS_BG_IMAGE))
		{
			strPath += "_backgnd.jpg";
			SetTVImageLayout(FALSE);
		}
		else if (strText == "動画プレイヤーの画像")//StrEqual(strText, IDS_ADVISOR_PHOTO))
		{
			strPath += "_advisor.jpg";	
			SetTVImageLayout(FALSE);
		}

		m_pImageCell->SetPath(strPath);
	}
	else
		SetTVImageLayout(FALSE);
}

BOOL CPageDefaultImage::SelectCourseImage()
{
	CString strCourse = MACRO_MAINFRM->GetCourse();
	if (strCourse == "")
		return TRUE;

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_IMAGE_ITEM);
	int nSel = pList->GetCurSel();
	if (nSel == LB_ERR)
		return TRUE;

	CString strText;
	pList->GetText(nSel, strText);

	CDlgTreeSelect dlg;
	//MACRO_MAINFRM->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
	//							SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	if (StrEqual(strText, IDS_LOGO))
	{
		dlg.m_strDefaultPath = MACRO_LOGO_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_LOGO_SELECT);
		dlg.m_strTitle = strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetShowType(3);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;

		//add by buxiangyi
		dlg.m_pPageImageLogo = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strFile = dlg.m_strSelectedFile;
			CString strExt = ::GetFileExt(strFile);
			CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";

			CString strIniPath = MACRO_INI_FILE(strCourse);
			if (strExt.CompareNoCase("gif") == 0)
			{
				CString strLogoPath = strHTML + "logo.gif";
				::CopyFile(strFile, strLogoPath, FALSE);
				m_pImageCell->SetPath(strLogoPath);

				::WritePrivateProfileString("General", "Logo", "logo.gif", strIniPath);
				if (::FileExists(strHTML + "logo.jpg"))
					::DeleteFile(strHTML + "logo.jpg");
			}
			else
			{
				CString strLogoPath = strHTML + "logo.jpg";
				ConvertToJPG(strFile, strLogoPath);
				m_pImageCell->SetPath(strLogoPath);

				::WritePrivateProfileString("General", "Logo", "logo.jpg", strIniPath);
				if (::FileExists(strHTML + "logo.gif"))
					::DeleteFile(strHTML + "logo.gif");
			}
		}
	}
	else if (StrEqual(strText, IDS_BG_IMAGE))
	{
		dlg.m_strDefaultPath = MACRO_BG_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_BG_SELECT);
		dlg.m_strTitle = strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetShowType(0);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;

		//add by buxiangyi
		dlg.m_pPageImageBackGround = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
			CString strBgPath = strHTML + "_backgnd.jpg";
			ConvertToJPG(dlg.m_strSelectedFile, strBgPath);
			m_pImageCell->SetPath(strBgPath);
		}
	}
	else if (strText == "動画プレイヤーの画像")//StrEqual(strText, IDS_ADVISOR_PHOTO))
	{
		dlg.m_strDefaultPath = MACRO_PHOTO_PATH;
		dlg.m_bSelectable = TRUE;
		CString strTitle;
		strTitle.LoadString(IDS_PHOTO_SELECT);
		dlg.m_strTitle = "動画プレイヤーの画像を選択";//strTitle;
		dlg.m_bResize = TRUE;
		dlg.ShowFileName(TRUE);
		dlg.SetFrameSize(112, 112);
		dlg.SetShowType(0);
		dlg.m_nWidth = nScreenX * 4 / 5;
		dlg.m_nHeight = nScreenY * 4 / 5;

		//add by buxiangyi
		dlg.m_pPageImagePhoto = TRUE;
		if(dlg.DoModal() == IDOK)
		{
			CString strHTML = MACRO_COURSE_PATH + strCourse + "\\html\\";
			CString strPhotoPath = strHTML + "_advisor.jpg";
			ConvertToJPG(dlg.m_strSelectedFile, strPhotoPath);
			m_pImageCell->SetPath(strPhotoPath);
		}
	}

	return TRUE;
}