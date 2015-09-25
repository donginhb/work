// PageDefaultOutlook.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageDefaultOutlook.h"
#include "cmkapi.h"
#include "macro.h"
#include "FileApi.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOutlook property page

IMPLEMENT_DYNCREATE(CPageDefaultOutlook, CPropertyPage)

CPageDefaultOutlook::CPageDefaultOutlook() : CPropertyPage(CPageDefaultOutlook::IDD)
{
	//{{AFX_DATA_INIT(CPageDefaultOutlook)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCanGetActive = FALSE;
	m_PageDefaultOutLookFlag = TRUE;
}

CPageDefaultOutlook::~CPageDefaultOutlook()
{
}

void CPageDefaultOutlook::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDefaultOutlook)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDefaultOutlook, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDefaultOutlook)
	ON_LBN_SELCHANGE(IDC_LIST_OUTLOOK, OnSelchangeListOutlook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultOutlook message handlers

BOOL CPageDefaultOutlook::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CString strIniPath;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_OUTLOOK);
	if (m_PageDefaultOutLookFlag == TRUE)
	{
		strIniPath= MACRO_TEMPLATE_PATH + "js\\config.js";
		
		CStringArray strArrayOutlook;
		CString m_stroutlook;
		::LoadFile(strIniPath, strArrayOutlook, TRUE);
		for (int i = 0; i < strArrayOutlook.GetSize(); i++)
		{
			CString strOutlook = strArrayOutlook[i];
			strOutlook.TrimLeft();
			if (strOutlook == "")
				continue;

			if (strOutlook.Find("bBpattern") >= 0)
			{
				CString strCheck = strOutlook.Right(2);
				int nCheck = atoi(strCheck.Left(1));
				if (nCheck == 1)
				{
					m_stroutlook = "標準";
				} 
				else if (nCheck == 2)
				{
					m_stroutlook = "白";
				} 
				else if (nCheck == 3)
				{
					m_stroutlook = "青";
				}
				else if (nCheck == 4)
				{
					m_stroutlook = "黒";
				}
			}
		}
		CStringArray strArray;
		::GetFiles(MACRO_OUTLOOK_PATH, "js", strArray);
		CString strDefault = m_stroutlook;
		for (int i = 0; i < strArray.GetSize(); i++)
		{
			CString str = strArray[i];

			int nDot = str.ReverseFind('.');
			str = str.Left(nDot);
			pList->AddString(str);	

			if (strDefault == "")
				strDefault = str;

			if (strDefault.CompareNoCase(str) == 0)
				pList->SetCurSel(i);
		}
	} 
	else
	{	
		strIniPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()) + "config.js";
		CStringArray strArrayOutlook;
		CString m_stroutlook;
		::LoadFile(strIniPath, strArrayOutlook, TRUE);
		for (int i = 0; i < strArrayOutlook.GetSize(); i++)
		{
			CString strOutlook = strArrayOutlook[i];
			strOutlook.TrimLeft();
			if (strOutlook == "")
				continue;

			if (strOutlook.Find("bBpattern") >= 0)
			{
				CString strCheck = strOutlook.Right(2);
				int nCheck = atoi(strCheck.Left(1));
				if (nCheck == 1)
				{
					m_stroutlook = "標準";
				} 
				else if (nCheck == 2)
				{
					m_stroutlook = "白";
				} 
				else if (nCheck == 3)
				{
					m_stroutlook = "青";
				}
				else if (nCheck == 4)
				{
					m_stroutlook = "黒";
				}
			}
		}
		CStringArray strArray;
		::GetFiles(MACRO_OUTLOOK_PATH, "js", strArray);
		CString strDefault = m_stroutlook;
		for (int i = 0; i < strArray.GetSize(); i++)
		{
			CString str = strArray[i];

			int nDot = str.ReverseFind('.');
			str = str.Left(nDot);
			pList->AddString(str);	

			if (strDefault == "")
				strDefault = str;

			if (strDefault.CompareNoCase(str) == 0)
				pList->SetCurSel(i);
		}
	}
	pList->CenterWindow(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDefaultOutlook::OnOK() 
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_OUTLOOK);
	int nIndex;
	if ((nIndex = pList->GetCurSel()) != LB_ERR)
	{
		CString strIniPath ;
		CString strSrcPath ;
		CString strDestPath;
		CStringArray strArray, strOldArray;
		if (m_PageDefaultOutLookFlag == TRUE)
		{
			strIniPath = MACRO_INI_PATH;
			CString strValue; 
			pList->GetText(nIndex, strValue);
			::WritePrivateProfileString("General", "DefaultOutlook", strValue, strIniPath);

			strIniPath = MACRO_TEMPLATE_PATH + "js\\config.js";
			::LoadFile(strIniPath, strOldArray);
			::LoadFile(strIniPath, strArray);
			pList->GetText(nIndex, strValue);
			int m_value;
			if (strValue == "標準")
			{
				m_value = 1;
			} 
			else if (strValue == "白")
			{
				m_value = 2;
			} 
			else if (strValue == "青")
			{
				m_value = 3;
			}
			else if (strValue == "黒")
			{
				m_value = 4;
			}
			CString strLine;
			for (int i = 0; i < strArray.GetSize(); i++)
			{
				CString strOutlook = strArray[i];
				strOutlook.TrimLeft();
				if (strOutlook == "")
					continue;

				if (strOutlook.Find("bBpattern") >= 0)
				{
					strLine.Format("var %s = %d;", "bBpattern", m_value);
					strArray[i] = strLine;
				}
			}
			::UpdateFileByTag( MACRO_TEMPLATE_PATH + "js\\", "config.js", strOldArray, strArray, TRUE);
			strSrcPath = MACRO_OUTLOOK_PATH + strValue + ".js";
			strDestPath = MACRO_TEMPLATE_PATH + "_outlook.js";
		} 
		else
		{
			strIniPath = MACRO_INI_FILE(MACRO_MAINFRM->GetCourse());
			CString strValue; 
			pList->GetText(nIndex, strValue);
			::WritePrivateProfileString("General", "OutlookName", strValue, strIniPath);

			strIniPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()) + "config.js";
			::LoadFile(strIniPath, strOldArray);
			::LoadFile(strIniPath, strArray); 
			pList->GetText(nIndex, strValue);
			int m_value;
			if (strValue == "標準")
			{
				m_value = 1;
			} 
			else if (strValue == "白")
			{
				m_value = 2;
			} 
			else if (strValue == "青")
			{
				m_value = 3;
			}
			else if (strValue == "黒")
			{
				m_value = 4;
			}
			CString strLine;
			for (int i = 0; i < strArray.GetSize(); i++)
			{
				CString strOutlook = strArray[i];
				strOutlook.TrimLeft();
				if (strOutlook == "")
					continue;

				if (strOutlook.Find("bBpattern") >= 0)
				{
					strLine.Format("var %s = %d;", "bBpattern", m_value);
					strArray[i] = strLine;
				}
			}
			::UpdateFileByTag(MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()), "config.js", strOldArray, strArray, TRUE);
			strSrcPath = MACRO_OUTLOOK_PATH + strValue + ".js";
			strDestPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse()) + "_outlook.js";
		}
		
		::CopyFile(strSrcPath, strDestPath, FALSE);
	}

	CPropertyPage::OnOK();
}

BOOL CPageDefaultOutlook::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPageDefaultOutlook::OnSelchangeListOutlook() 
{
}
