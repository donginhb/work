// DlgDevNotInstalled.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgDevNotInstalled.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNotInstalled dialog


CDlgDevNotInstalled::CDlgDevNotInstalled(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevNotInstalled::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevNotInstalled)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Screenflag = FALSE;
}


void CDlgDevNotInstalled::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevNotInstalled)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevNotInstalled, CDialog)
	//{{AFX_MSG_MAP(CDlgDevNotInstalled)
	ON_BN_CLICKED(ID_CHANGE_DEV, OnChangeDev)
	ON_BN_CLICKED(ID_RETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNotInstalled message handlers

BOOL CDlgDevNotInstalled::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if ( m_Screenflag == TRUE)
	{
		GetDlgItem(IDC_EDIT_VIDEO_DEVICE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIDEO) ->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DEVICE)->SetWindowText(m_strDevName);
		GetDlgItem(IDC_EDIT_AUDIO_DEVICE)->SetWindowText(m_strAudioDev);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_DEVICE)->SetWindowText(m_strDevName);
		GetDlgItem(IDC_EDIT_VIDEO_DEVICE)->SetWindowText(m_strVideoDev);
		GetDlgItem(IDC_EDIT_AUDIO_DEVICE)->SetWindowText(m_strAudioDev);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDevNotInstalled::OnChangeDev() 
{
	EndDialog(ID_CHANGE_DEV);	
}

void CDlgDevNotInstalled::OnRetry() 
{
	EndDialog(ID_RETRY);	
}

void CDlgDevNotInstalled::OnCancel() 
{
	CDialog::OnCancel();
}
