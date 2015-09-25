#include "stdafx.h"
#include "eLearning.h"
#include "dlgabout.h"
#include "ImageCtrl.h"
#include "bmpapi.h"
#include <io.h>

#define constMargin 15

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_strFileName = "";
	m_pImageCtrl = NULL;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_strFileName == "")
		return TRUE;

	if (_access(m_strFileName, 0) != 0)
		return TRUE;

	CBitmap* pBitmap = ::LoadFile(m_strFileName);
	BITMAP b;
	pBitmap->GetBitmap(&b);
	int nBmpWidth = b.bmWidth;
	int nBmpHeight = b.bmHeight;
	pBitmap->DeleteObject();
	delete pBitmap;

	CRect rect;
	rect.left = constMargin;
	rect.top = constMargin;
	rect.bottom = rect.top + nBmpHeight;
	rect.right = rect.left + nBmpWidth;

	m_pImageCtrl = new CImageCtrl;
	m_pImageCtrl->Create(WS_CHILD | WS_VISIBLE, NULL, rect, this);
	m_pImageCtrl->LoadFile(m_strFileName);

	CRect rcOK;
	GetDlgItem(IDCANCEL)->GetWindowRect(rcOK);
	int nWidth = rcOK.Width();
	int nHeight = rcOK.Height();
	rcOK.right = rect.right;
	rcOK.left = rcOK.right - nWidth;
	rcOK.top = rect.bottom + constMargin;
	rcOK.bottom = rcOK.top + nHeight;
	GetDlgItem(IDCANCEL)->MoveWindow(rcOK);

	int nClientWidth = nBmpWidth + constMargin * 2;
	int nClientHeight = rcOK.bottom + constMargin;

	CRect rcClient, rcWindow;
	GetClientRect(rcClient);
	GetWindowRect(rcWindow);
	int nOffsetX = rcWindow.Width() - rcClient.Width();
	int nOffsetY = rcWindow.Height() - rcClient.Height();

	rect.left = rect.top = 0;
	rect.right = nClientWidth + nOffsetX;
	rect.bottom = nClientHeight + nOffsetY;
	MoveWindow(rect);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pImageCtrl)
	{
		m_pImageCtrl->DestroyWindow();
		delete m_pImageCtrl;
	}
}

void CAboutDlg::OnCancel() 
{
	CDialog::OnCancel();
}
