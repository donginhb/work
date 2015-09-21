// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCancelled = FALSE;
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_ANIMATE, m_avi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_avi.Open(m_strAVIPath); // open the avi resource.
	m_avi.Play(0, -1, -1);

	CenterWindow();

#ifdef _LOG
	CString strText;
	GetWindowText(strText);

	LOG("顯示<對話盒>", strText);
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::Stop()
{
	m_avi.Stop();
}

void CDlgProgress::Play()
{
	m_avi.Play(0, -1, -1);
}

void CDlgProgress::Show()
{
	Play();
	ShowWindow(SW_SHOW);
	UpdateWindow();
    m_bCancelled = FALSE;
}

void CDlgProgress::EnableCancel(BOOL b)
{
	GetDlgItem(IDCANCEL)->EnableWindow(b);
}

void CDlgProgress::Hide()
{
	Stop();
	ShowWindow(SW_HIDE);
	UpdateWindow();
}

void CDlgProgress::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_avi.Close();
}

void CDlgProgress::PeekAndPump()
{
    if (::GetFocus() != m_hWnd)
        SetFocus();

    MSG msg;
    while (!m_bCancelled && ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
    {
        if ((msg.message == WM_CHAR) && (msg.wParam == VK_ESCAPE))
            OnCancel();

        // Cancel button disabled if modal, so we fake it.
        if (msg.message == WM_LBUTTONUP)
        {
            CRect rect;
            GetDlgItem(IDCANCEL)->GetWindowRect(rect);
			BOOL bEnable = GetDlgItem(IDCANCEL)->IsWindowEnabled();
            if (bEnable && rect.PtInRect(msg.pt))
                OnCancel();
        }
  
        if (!AfxGetApp()->PumpMessage()) 
        {
            ::PostQuitMessage(0);
            return;
        } 
    }
}

void CDlgProgress::OnCancel() 
{
    m_bCancelled = TRUE;

	Hide();

    CWnd *pWnd = AfxGetMainWnd();
    if (pWnd && ::IsWindow(pWnd->m_hWnd))
        pWnd->SetForegroundWindow();
}
