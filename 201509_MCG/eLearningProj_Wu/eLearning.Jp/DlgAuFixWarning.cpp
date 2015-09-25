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
	this->SetWindowText("���Y���␳�̒��ӎ���");

	CStatic* pStaticWarning = (CStatic*)this->GetDlgItem(IDC_STATIC_WARNING);
	CFont* pOldFont = pStaticWarning->GetFont();
	CFont* pNewFont = new CFont();
	LOGFONT* pLf = new LOGFONT();
	pOldFont->GetLogFont(pLf);
	pLf->lfHeight -= 2;
	pNewFont->CreateFontIndirect(pLf);
	pStaticWarning->SetFont(pNewFont);

	CString strWarning;
	strWarning += "        �������������ӎ�������������\r\n";
	strWarning += "\r\n"; 
	strWarning += "  �P�D�R���e���c�쐬���ɐ������ꂽ����ɂ̂�\r\n" ;
	strWarning += "  �␳���K�p����܂��B�}����������ɂ�\r\n";
	strWarning += "  �K�p����܂���̂ł����ӂ��������B\r\n";
	strWarning += "\r\n";
	strWarning += "  �Q�D�␳�̌��ʂ͂��g�p�ɂȂ��Ă���\r\n";
	strWarning += "  �p�\�R����J�����ȂǊ��ɂ�荷������܂��B\r\n";
	strWarning += "  �K���ݒ肵���␳�l�ʂ�ɓK�p�����Ƃ�\r\n";
	strWarning += "  ����܂���B\r\n";
	strWarning += "  \r\n";
	strWarning += "  �R�D�␳�����ɂ͎��Ԃ�������܂��B\r\n";
	strWarning += "  �^�掞�Ԉȏォ����\��������̂�\r\n";
	strWarning += "  �����ӂ��������B\r\n";
	strWarning += "  \r\n";
	strWarning += "  �S�D�����ҏW�ς̏ꍇ�͂R�D��������Ɏ��Ԃ�\r\n";
	strWarning += "  ������܂��B�����ӂ��������B\r\n";
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
