// RelatedInformation.cpp : implementation file
//

#include "stdafx.h"
#include "RelatedInformation.h"
#include "afxdialogex.h"
#include "resource.h"
#include "UIGrid.h"
#include "Macro.h"
#include "elearning.h"
#include "Macro.h"
#include "Mainfrm.h"
#include "Cmkapi.h"
#include "eLearningAPI.h"
#include "FileApi.h"
#define    min         3
#define    headwith    100
#define    Spacing     1
// RelatedInformation dialog

IMPLEMENT_DYNAMIC(CRelatedInformation, CPropertyPage)

CRelatedInformation::CRelatedInformation()
	: CPropertyPage(CRelatedInformation::IDD)
{

}

CRelatedInformation::~CRelatedInformation()
{
}

void CRelatedInformation::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRelatedInformation, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//ON_MESSAGE(WM_GRD_UPDATE, OnGrdUpdate)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// RelatedInformation message handlers
void CRelatedInformation::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	CRect Wholerect;
	Wholerect.left = rect.left + 20;
	Wholerect.right = rect.right - 25;
	Wholerect.top = rect.top + 20;
	Wholerect.bottom = rect.bottom * 0.4;
	m_Whole -> MoveWindow(Wholerect);

	CRect StaticNamerect;
	StaticNamerect.left = Wholerect.left + min;
	StaticNamerect.right = Wholerect.left + headwith ;
	StaticNamerect.top = Wholerect.top + min;
	StaticNamerect.bottom = Wholerect.top +Wholerect.bottom * 0.25;
	m_StaticName -> MoveWindow(StaticNamerect);

	CRect StaticMessagerect;
	StaticMessagerect.left = Wholerect.left + min;
	StaticMessagerect.right = Wholerect.left + headwith;
	StaticMessagerect.top = StaticNamerect.bottom + Spacing;
	StaticMessagerect.bottom = Wholerect.bottom -min ;
	m_StaticMessage -> MoveWindow(StaticMessagerect);

	CRect strNameCEditrect;
	strNameCEditrect.left = StaticNamerect.left + headwith +Spacing;
	strNameCEditrect.right = Wholerect.right -min ;
	strNameCEditrect.top = StaticNamerect.top ;
	strNameCEditrect.bottom = StaticNamerect.bottom ;
	m_strNameCEdit -> MoveWindow(strNameCEditrect);


	CRect strMessageCEditrect;
	strMessageCEditrect.left = StaticNamerect.left + headwith + Spacing;
	strMessageCEditrect.right = Wholerect.right -min;
	strMessageCEditrect.top = StaticMessagerect.top ;
	strMessageCEditrect.bottom = StaticMessagerect.bottom ;
	m_strMessage -> MoveWindow(strMessageCEditrect);

	HorizontallineBegin.x = StaticNamerect.left;
	HorizontallineBegin.y = strNameCEditrect.bottom;
	HorizontallineEnd.x = strNameCEditrect.left;
	HorizontallineEnd.y = strNameCEditrect.bottom;

	VerticallineBegin.x = StaticNamerect.left + headwith;
	VerticallineBegin.y = strNameCEditrect.top -min;
	VerticallineEnd.x = StaticNamerect.left + headwith;
	VerticallineEnd.y = StaticMessagerect.bottom +min;
	
	// TODO: Add your message handler code here
}


void CRelatedInformation::OnDestroy()
{
	CPropertyPage::OnDestroy();

	m_Whole->DestroyWindow();
	delete m_Whole;

	m_StaticName->DestroyWindow();
	delete m_StaticName;

	m_StaticMessage->DestroyWindow();
	delete m_StaticMessage;

	m_strNameCEdit->DestroyWindow();
	delete m_strNameCEdit;

	m_strMessage->DestroyWindow();
	delete m_strMessage;

	// TODO: Add your message handler code here
}


int CRelatedInformation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Whole = new CStatic;
	m_Whole->Create("", WS_CHILD | WS_VISIBLE |SS_SUNKEN |SS_ETCHEDFRAME , CRect(0, 0, 0, 0), this, 0);

	m_StaticName = new CStatic;
	CString strText;
	strText.LoadString(IDS_COURSE_CONTENT_NAME);
	m_StaticName->Create(strText, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	m_StaticMessage = new CStatic;
	strText.LoadString(IDS_COURSE_CONTENT_MESSAGE);
	m_StaticMessage->Create(strText, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	m_strNameCEdit = new CEdit;
	m_strNameCEdit->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	m_strMessage = new CEdit;
	m_strMessage->Create(WS_CHILD | WS_VISIBLE |ES_MULTILINE |ES_WANTRETURN | ES_AUTOVSCROLL, CRect(0, 0, 0, 0), this, 0);

	LoadCourseLayout(MACRO_MAINFRM->GetCourse());
	return 0;
}
BOOL CRelatedInformation::LoadCourseLayout(LPCTSTR szCourse) 
{
	if (m_strCourse == szCourse)
		return TRUE;

	CString strCourse = szCourse;

	CString strHTML = MACRO_JS_PATH(szCourse);

	CString strJSPath = strHTML + "config.js"; 
	CStringArray strArray;
	::LoadFile(strJSPath, strArray);
	CString strTmp;
	int nIndex;
	if (strArray.GetSize() >= 1)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
		{
			CString str = strArray[i];
			if ((nIndex = str.Find("var TitleName = \"")) >= 0)
			{
				strTmp = str.Mid(17, str.GetLength() - 19);
				ConvertCodePage(strTmp,  CP_UTF8,CP_ACP);
				if (strTmp == "")
					strTmp = szCourse;
				m_strNameCEdit ->SetWindowText(strTmp);
			}
			if ((nIndex = str.Find("var iMessage = ")) >= 0)
			{
				strTmp = str.Mid(15, str.GetLength() - 16);
				int index = atoi(strTmp);
				if (index == 1)
				{
					CStringArray m_CourseMessage, Message;
					CString path;
					CString ContentText;
					path = strHTML + "msgtext.js";
					::LoadFile(path, m_CourseMessage);
					for (int i = 0; i<m_CourseMessage.GetSize();i++)
					{
						if(index = m_CourseMessage[i].Find("document.writeln(\'") >= 0)
						{
							str = m_CourseMessage[i];
							strTmp = str.Mid(22, str.GetLength() - 26);
							ContentText +=strTmp;
							ContentText += "\r\n";
						}
					}
					ContentText = ContentText.Left(ContentText.GetLength() - 2);
					ConvertCodePage(ContentText,  CP_UTF8,CP_ACP);
					m_strMessage ->SetWindowText(ContentText);
				}

			}
		}
	}


	return TRUE;
}
BOOL CRelatedInformation::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *oldpen = dc.SelectObject(&pen);
	dc.MoveTo(HorizontallineBegin);
	dc.LineTo(HorizontallineEnd);
	dc.MoveTo(VerticallineBegin);
	dc.LineTo(VerticallineEnd);
	dc.SelectObject(oldpen);

	return TRUE;
}

void CRelatedInformation::OnOK() 
{
	CString m_CourseName, strPath;
	CStringArray m_CourseMessgae;
	CStringArray CourseName,OldCourseName;
	CStringArray CourseMessage,OldCourseMessage;
	strPath = MACRO_JS_PATH(MACRO_MAINFRM->GetCourse());
	m_strNameCEdit -> GetWindowText(m_CourseName);
	if (m_CourseName != "")
	{
		OldCourseName.Add("var TitleName =" );
		ConvertCodePage(m_CourseName, CP_ACP ,CP_UTF8);
		CourseName.Add("var TitleName = \"" + m_CourseName +"\";");
		UpdateFileByTag(strPath, "config.js", OldCourseName, CourseName, TRUE);
	}
	int m_lineCount = m_strMessage -> GetLineCount();
	CString text;
	m_strMessage->GetWindowTextA(text);
	if (text != "")//m_lineCount >=1)
	{
		OldCourseMessage.Add("var iMessage =" );
		CourseMessage.Add("var iMessage = 1;");
		UpdateFileByTag(strPath, "config.js", OldCourseMessage, CourseMessage, TRUE);

		for (int i = 0; i<m_lineCount +2; i++)
		{ 
			if (i == 0)
			{
				m_CourseMessgae.Add("function messagetext(){\r\n");
			} 
			else if (i == m_lineCount +1)
			{
				m_CourseMessgae.Add("{");
			} 
			else
			{
				char m_LineMessage[256];
				CString linemessage;
				int len = m_strMessage->GetLine(i - 1, m_LineMessage, 256);
				m_LineMessage[len] = 0;
				int count = strlen(m_LineMessage);

				for (int i = 0; i<count ;i++)
				{
					linemessage += m_LineMessage[i];
				}
				linemessage = "    document.writeln(\'" + linemessage + "\');" + "\r\n";
				ConvertCodePage(linemessage, CP_ACP ,CP_UTF8);
				m_CourseMessgae.Add(linemessage);
			}
			
		}
		CString strDestPath;
		strDestPath = strPath +"msgtext.js";
		FILE *stream_out;
		if ((stream_out = fopen((LPCTSTR)strDestPath, "w+" )) == NULL)
		{
			return ;
		}
		for (int i = 0; i<m_lineCount +2; i++)
		{
			fputs(m_CourseMessgae[i], stream_out);
		}
		fclose(stream_out);
	}
	else
	{
		OldCourseMessage.Add("var iMessage =" );
		CourseMessage.Add("var iMessage = 0;");
		UpdateFileByTag(strPath, "config.js", OldCourseMessage, CourseMessage, TRUE);
	}

	CPropertyPage::OnOK();
}