// DlgSelType.cpp : implementation file
//

#include "stdafx.h"
#include "courseloader.h"
#include "DlgSelType.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelType dialog


CDlgSelType::CDlgSelType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArrayType = NULL;
}


void CDlgSelType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelType)
	DDX_Control(pDX, IDC_LIST_TYPE, m_clType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelType, CDialog)
	//{{AFX_MSG_MAP(CDlgSelType)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnButtonSelectall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelType message handlers

BOOL CDlgSelType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	char szBuf[512];
	::GetPrivateProfileString("General", "TypeCount", "0", szBuf, 512, strIniPath);
	int nCount = atoi(szBuf);
	CString strType, strDesc, strVideo, strAudio, strFPS, strPostfix, strEntry;
	for (int i = 0; i < nCount; i++)
	{
		strType.Format("Type%d", i);
		::GetPrivateProfileString(strType, "Description", "", szBuf, 512 , strIniPath);
		strDesc = szBuf;

		::GetPrivateProfileString(strType, "VideoFreq", "", szBuf, 512 , strIniPath);
		strVideo = szBuf;

		::GetPrivateProfileString(strType, "AudioFreq", "", szBuf, 512 , strIniPath);
		strAudio = szBuf;

		::GetPrivateProfileString(strType, "FPS", "", szBuf, 512 , strIniPath);
		strFPS = szBuf;

		int nVideo = GetNumberPart(strVideo);
		int nAudio = GetNumberPart(strAudio);
		int nFPS = atoi(strFPS);

		if (strVideo == "-" && strAudio == "-" && strFPS == "-")
			strPostfix = "x_x_x";
		else if (nVideo == 0 && strFPS == "-")
			strPostfix.Format("0_%d_x", nAudio);
		else
			strPostfix.Format("%d_%d_%d", nVideo, nAudio, nFPS);

		strEntry = GetTableHTML(strType, GetLinkText(), strPostfix + "/default.htm");
		
		m_strArrayHTML.Add(strEntry);

		m_clType.AddString(strDesc);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgSelType::GetNumberPart(CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			break;
	}

	return atoi(str.Left(i));
}

void CDlgSelType::OnOK() 
{
	CString strType;
	if (m_pstrArrayType)
	{
		m_pstrArrayType->RemoveAll();
		for (int iItem = 0; iItem < m_clType.GetCount(); iItem++)
		{
			if (m_clType.GetCheck(iItem))
				m_pstrArrayType->Add(m_strArrayHTML.GetAt(iItem));
		}

		if (m_pstrArrayType->GetSize() == 0)
		{
			return;
		}
	}

	CDialog::OnOK();
}

CString CDlgSelType::GetLinkText()
{
	CString strIniPath = ::GetSysPath() + "Launcher.ini";
	
	CString strLink;
	strLink.LoadString(IDS_DEFAULT_LINK);
	char szBuf[256];
	::GetPrivateProfileString("General", "LinkCaption", "", szBuf, 256, strIniPath);
	if (szBuf[0] != '\0')
		strLink = szBuf;

	return strLink;
}

CString CDlgSelType::GetTableHTML(CString strLabel, CString strLink, CString strURL)
{
	CString strTable;
    strTable = "<tr><td width=\"80%\" height=\"28\"><b><font face=\"Arial\" size=\"3\" color=\"#000000\">" + 
			   strLabel + "</font></b></td>\r\n";
    strTable += "<td width=\"20%\" align=\"center\" height=\"28\"><a href=\"" + strURL + "\">" + strLink + 
			   "</a></td></tr>\r\n";

	return strTable;
}

void CDlgSelType::OnButtonSelectall() 
{
	for (int iItem = 0; iItem < m_clType.GetCount(); iItem++)
		m_clType.SetCheck(iItem, 1);
}
