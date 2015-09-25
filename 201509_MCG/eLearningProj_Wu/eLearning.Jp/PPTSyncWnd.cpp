// PPTSyncWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PPTSyncWnd.h"
#include "PPTWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPTSyncWnd

CPPTSyncWnd::CPPTSyncWnd()
{
	m_bSamePage = FALSE;
	m_nPreSlideNo = 1;
}

CPPTSyncWnd::~CPPTSyncWnd()
{
}


BEGIN_MESSAGE_MAP(CPPTSyncWnd, CWnd)
	//{{AFX_MSG_MAP(CPPTSyncWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_CHANGE_SLIDE, OnChangeSlide)
	ON_MESSAGE(WM_ADD_PPT_RECORD, OnAddPPTRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPPTSyncWnd message handlers

int CPPTSyncWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	BeginWaitCursor();
	m_pPPTWnd = new CPPTWnd;
	m_pPPTWnd->m_strOp = "=";

	CRect rcClient;
	GetClientRect(rcClient);
	m_pPPTWnd->Create(WS_VISIBLE | WS_CHILDWINDOW | WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
						rcClient, this);
	m_pPPTWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
				            SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
	int nPPTX = 296;
	int nPPTY = 222;
	if (nScreenX > 800)
	{
		nPPTX = 400;
		nPPTY = 300;
	}

	if (m_strPPTPath != "")
	{
		if (!m_pPPTWnd->LoadPPT(m_strPPTPath, nPPTX, nPPTY))
		{
			CString strTmp, strMsg;
			strTmp.LoadString(IDS_FAIL_TO_LOAD_FILE);
			strMsg.Format(strTmp, m_strPPTPath);
			W_SHOW_ERROR_STR(strMsg, this);
			return -1;
		}

		CRect rcPPT;
		m_pPPTWnd->GetWindowRect(rcPPT);

		CRect rcThis;
		GetWindowRect(rcThis);

		int nWidth = rcPPT.Width() + (rcPPT.left - rcThis.left) * 2;
		int nHeight = rcPPT.Height() + (rcPPT.top - rcThis.top) + (rcPPT.left - rcThis.left);
		CRect rcPos;
		rcPos.left = rcThis.left + (rcThis.Width() - nWidth) / 2;
		rcPos.top = rcThis.top + (rcThis.Height() - nHeight) / 2;
		rcPos.right = rcPos.left + nWidth;
		rcPos.bottom = rcPos.top + nHeight;
		MoveWindow(rcPos);

		CStringArray* pstrArrayTitle = m_pPPTWnd->GetTitleArray();
		CString strUndefined = "";
		strUndefined.LoadString(IDS_UNDEFINED);
		for (int i = 0; i < pstrArrayTitle->GetSize(); i++)
		{
			CString strTmp = pstrArrayTitle->GetAt(i);
			if (strTmp == "")
				strTmp = strUndefined;
			m_strArrayTitle.Add(strTmp);
		}
	}

    //leu:load Icon
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE);			// Set big icon

	m_pPPTWnd->OnGoto(1);

    return 0;
}

void CPPTSyncWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pPPTWnd)
	{
		m_pPPTWnd->DestroyWindow();
		delete m_pPPTWnd;
	}
}

void CPPTSyncWnd::OnClose() 
{
	m_pMSGWnd->PostMessage(WM_SYNC_WND_CLOSED);
	
	CWnd::OnClose();
}

LRESULT CPPTSyncWnd::OnChangeSlide(WPARAM wParam, LPARAM lParam)
{
	m_bSamePage = FALSE;
	long nCurNo = m_pPPTWnd->GetCurrentSlideNumber(); 

	//add 2013/11 kagata
	int g = 0;
	if ( nCurNo == 0 )
	{
		while ( nCurNo == 0 && g != 100)
		{
			g++;
			Sleep(100);
			nCurNo = m_pPPTWnd->GetCurrentSlideNumber();
		}
	}
	//////////////////////

	if (nCurNo == m_nPreSlideNo)
		m_bSamePage = TRUE;
	m_nPreSlideNo = nCurNo;
	m_nPreAnimaNo = m_pPPTWnd->GetCurrentAnimaNumber();

	return 0L;
}

LRESULT CPPTSyncWnd::OnAddPPTRecord(WPARAM wParam, LPARAM lParam)
{
	CString strStep;
	CString strTitle = "";
#if 1
	if (!m_bSamePage || m_nPreAnimaNo == 0)
	{
		strStep.Format("%d@%d%s", m_nPreSlideNo, m_nPreAnimaNo, m_pPPTWnd->m_strOp);
		strTitle.Format("s%03d %s", m_nPreSlideNo, m_strArrayTitle[m_nPreSlideNo - 1]);
	}
	else
		strStep.Format("%d@%d.%s", m_nPreSlideNo, m_nPreAnimaNo, m_pPPTWnd->m_strOp);
#else
	strStep.Format("%d@%d%s", m_nPreSlideNo, m_nPreAnimaNo, m_pPPTWnd->m_strOp);
	strTitle.Format("s%03d %s", m_nPreSlideNo, m_strArrayTitle[m_nPreSlideNo - 1]);
#endif

	m_pMSGWnd->SendMessage(WM_ADD_PPT_POINT, (WPARAM)(LPCTSTR)strStep, (LPARAM)(LPCTSTR)strTitle);

	return 0L;
}
