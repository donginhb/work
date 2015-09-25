// PPTPanel.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PPTPanel.h"
#include "PPT9Ctrl.h"
//add 2014/7/29 kagata BugNo.130
#include "PPT10Ctrl.h"
#include "PPT15Ctrl.h"
#include "PPTWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPTPanel

CString CPPTPanel::m_strClass = "";

CPPTPanel::CPPTPanel()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10 = new CPPT10Ctrl;			//new
	}else{									//new
		m_pPPT9 = new CPPT9Ctrl;
	}										//new
	//add
}

CPPTPanel::~CPPTPanel()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		delete m_pPPT10;					//new
	}else{									//new
		delete m_pPPT9;
	}//new
	//add
}


BEGIN_MESSAGE_MAP(CPPTPanel, CWnd)
	//{{AFX_MSG_MAP(CPPTPanel)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPPTPanel message handlers
BOOL CPPTPanel::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	//if (m_strClass == "")
	//	m_strClass = AfxRegisterWndClass(CS_DBLCLKS, 0, NULL);
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);
	return bRet;
}

BOOL CPPTPanel::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CPPTPanel::LoadPPT(CString strPath, int nWidth, int nHeight) 
{
	//HWND hWndPptEx = ::FindWindow ("screenClass", NULL);
	//if (hWndPptEx)
	//{
	//	SHOW_WARNING(IDS_PPT_IS_RUNNING);
	//	return FALSE;
	//}

	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->OpenPresToWin(strPath, m_hWnd, nWidth, nHeight);					//new
	}else{									//new
		m_pPPT9->OpenPresToWin(strPath, m_hWnd, nWidth, nHeight);
	}										//new
	//add
	

	return TRUE;
}

void CPPTPanel::OnDestroy() 
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->Close();					//new
	}else{									//new
		m_pPPT9->Close();
	}										//new
	//add
	

	CWnd::OnDestroy();
}

int CPPTPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		if (!m_pPPT10->Create())
		{
			CString str;
			str.LoadString(IDS_PPT_OLE_NOT_FOUND);
			AfxMessageBox(str, MB_ICONSTOP | MB_OK);
			return -1;
		}					//new
	}else{									//new
		if (!m_pPPT9->Create())
		{
			CString str;
			str.LoadString(IDS_PPT_OLE_NOT_FOUND);
			AfxMessageBox(str, MB_ICONSTOP | MB_OK);
			return -1;
		}
	}										//new
	//add

	SetWindowText("pptpanel-photon");
	
	return 0;
}

void CPPTPanel::PrevSlide()
{
	//add 2014/7/29 kagata BugNo.130
	if (nVer == 14){	//new
		m_pPPT10->PrevSlide();					//new
	}else{									//new
		m_pPPT9->PrevSlide();
	}										//new
	//add
}

void CPPTPanel::NextSlide()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->NextSlide();					//new
	}else{									//new
		m_pPPT9->NextSlide();
	}										//new
	//add
}

void CPPTPanel::GotoSlide(int n)
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->GotoSlide(n);					//new
	}else{									//new
		m_pPPT9->GotoSlide(n);
	}										//new
	//add
}

void CPPTPanel::FirstSlide()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->FirstSlide();					//new
	}else{									//new
		m_pPPT9->FirstSlide();
	}										//new
	//add
}

void CPPTPanel::LastSlide()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->LastSlide();					//new
	}else{									//new
		m_pPPT9->LastSlide();
	}										//new
	//add
}

int CPPTPanel::GetSlideCount()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		return m_pPPT10->GetSlideCount();					//new
	}else{									//new
		return m_pPPT9->GetSlideCount();
	}										//new
	//add
}

void CPPTPanel::GetSliderTitle(CStringArray& strArray)
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		m_pPPT10->GetSliderTitle(strArray);					//new
	}else{									//new
		m_pPPT9->GetSliderTitle(strArray);
	}										//new
	//add
}

int CPPTPanel::GetCurrentSlideNumber()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		return m_pPPT10->GetCurrentSlideNumber();					//new
	}else{									//new
		return m_pPPT9->GetCurrentSlideNumber();
	}										//new
	//add
}

int CPPTPanel::GetCurrentAnimaNumber()
{
	//add 2014/7/29 kagata BugNo.130
	nVer = m_pPPTWnd->ChkPPTver();			//new
	if (nVer == 14){	//new
		return m_pPPT10->GetCurrentAnimaNumber();					//new
	}else{									//new
		return m_pPPT9->GetCurrentAnimaNumber();
	}										//new
	//add
}
