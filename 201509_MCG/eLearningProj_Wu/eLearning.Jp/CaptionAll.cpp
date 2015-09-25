// CaptionAll.cpp : implementation file
//

#include "stdafx.h"
#include "CaptionAll.h"
#include "afxdialogex.h"


// CCaptionAll dialog

IMPLEMENT_DYNAMIC(CCaptionAll, CDialogEx)

CCaptionAll::CCaptionAll(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCaptionAll::IDD, pParent)
{

}

CCaptionAll::~CCaptionAll()
{
}

void CCaptionAll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCaptionAll, CDialogEx)
END_MESSAGE_MAP()


// CCaptionAll message handlers


BOOL CCaptionAll::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CStatic* pStaticWarning = (CStatic*)this->GetDlgItem(IDC_STATIC_WARNING);
	CFont* pOldFont = pStaticWarning->GetFont();
	CFont* pNewFont = new CFont();
	LOGFONT* pLf = new LOGFONT();
	pOldFont->GetLogFont(pLf);
	pLf->lfHeight -= 2;
	pNewFont->CreateFontIndirect(pLf);
	pStaticWarning->SetFont(pNewFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
