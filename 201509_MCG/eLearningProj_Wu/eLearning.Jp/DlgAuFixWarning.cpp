// DlgAuFixWarning.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAuFixWarning.h"
#include "afxdialogex.h"
#include "DlgAuFixSet.h"


// CDlgAuFixWarning dialog

IMPLEMENT_DYNAMIC(CDlgAuFixWarning, CDialogEx)

CDlgAuFixWarning::CDlgAuFixWarning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAuFixWarning::IDD, pParent)
{

}

CDlgAuFixWarning::~CDlgAuFixWarning()
{
}

void CDlgAuFixWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAuFixWarning, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAuFixWarning::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAuFixWarning message handlers


BOOL CDlgAuFixWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	this->SetWindowText("音ズレ補正の注意事項");

	CStatic* pStaticWarning = (CStatic*)this->GetDlgItem(IDC_STATIC_WARNING);
	CFont* pOldFont = pStaticWarning->GetFont();
	CFont* pNewFont = new CFont();
	LOGFONT* pLf = new LOGFONT();
	pOldFont->GetLogFont(pLf);
	pLf->lfHeight -= 2;
	pNewFont->CreateFontIndirect(pLf);
	pStaticWarning->SetFont(pNewFont);

	CString strWarning;
	strWarning += "        ★☆★☆★注意事項★☆★☆★\r\n";
	strWarning += "\r\n"; 
	strWarning += "  １．コンテンツ作成時に生成された動画にのみ\r\n" ;
	strWarning += "  補正が適用されます。挿入した動画には\r\n";
	strWarning += "  適用されませんのでご注意ください。\r\n";
	strWarning += "\r\n";
	strWarning += "  ２．補正の効果はご使用になられている\r\n";
	strWarning += "  パソコンやカメラなど環境により差があります。\r\n";
	strWarning += "  必ず設定した補正値通りに適用されるとは\r\n";
	strWarning += "  限りません。\r\n";
	strWarning += "  \r\n";
	strWarning += "  ３．補正処理には時間がかかります。\r\n";
	strWarning += "  録画時間以上かかる可能性があるので\r\n";
	strWarning += "  ご注意ください。\r\n";
	strWarning += "  \r\n";
	strWarning += "  ４．もし編集済の場合は３．よりもさらに時間が\r\n";
	strWarning += "  かかります。ご注意ください。\r\n";
	pStaticWarning->SetWindowText(strWarning);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAuFixWarning::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
	CDlgAuFixSet dlg;
	dlg.DoModal();
	CDialogEx::OnOK();
}
