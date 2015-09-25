// DlgNoteEdit.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgNoteEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNoteEdit dialog


CDlgNoteEdit::CDlgNoteEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNoteEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNoteEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNoteEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNoteEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNoteEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgNoteEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNoteEdit message handlers

void CDlgNoteEdit::OnOK() 
{
	GetDlgItem(IDC_EDIT_NOTE)->GetWindowText(m_strNote);
	
	CDialog::OnOK();
}

BOOL CDlgNoteEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_NOTE)->SetWindowText(m_strNote);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
