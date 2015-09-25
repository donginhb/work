// PPTWnd.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PPTWnd.h"
#include "PPTPanel.h"
#include "BmpBtn.h"
#include "NumberEdit.h"
#include "cmkapi.h"
#include "Macro.h"
#include "BmpStatic.h"
#include "SliderWnd.h"
#include "ToolTipWnd.h"
#include "DlgGotoSlide.h"
#include "Registry.h"	//add 2014/7/29 kagata BugNo.130

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define constSmallGap 10
#define constEditWidth 40
/////////////////////////////////////////////////////////////////////////////
// CPPTWnd

CString CPPTWnd::m_strClass = "";
HINSTANCE CPPTWnd::m_hmodHook = NULL;
HHOOK CPPTWnd::m_hHook = NULL;

CPPTWnd::CPPTWnd()
{
	m_pBtnFirst = new CBmpBtn(IDB_FIRST_SLIDE, BTN_PUSH_BUTTON);
	m_pBtnPrev = new CBmpBtn(IDB_PREV_SLIDE, BTN_PUSH_BUTTON);
	m_pBtnNext = new CBmpBtn(IDB_NEXT_SLIDE, BTN_PUSH_BUTTON);
	m_pBtnLast = new CBmpBtn(IDB_LAST_SLIDE, BTN_PUSH_BUTTON);
	m_pBtnGoto = new CBmpBtn(IDB_GOTO, BTN_PUSH_BUTTON);
	m_pBtnLocate = new CBmpBtn(IDB_LOCATE, BTN_PUSH_BUTTON);
	m_pBtnLocateNext = new CBmpBtn(IDB_LOCATE_NEXT, BTN_PUSH_BUTTON);
	m_pBtnLocatePrev = new CBmpBtn(IDB_LOCATE_PREV, BTN_PUSH_BUTTON);
	
	m_pBmpLabel1 = new CBmpStatic(IDB_SEP);
	m_pBmpLabel1->SetBorder(FALSE);
	
	m_pBmpLabel2= new CBmpStatic(IDB_SEP);
	m_pBmpLabel2->SetBorder(FALSE);

	//m_pBmpLabel3= new CBmpStatic(IDB_SEP);
	//m_pBmpLabel3->SetBorder(FALSE);

	m_pPageNoEdit = new CNumberEdit;
	m_pPPTPanel = new CPPTPanel;
	m_pSliderWnd = new CSliderWnd;
	m_lTime = 0;
	m_pToolTip = new CToolTipWnd;

	//m_bChanged = TRUE;
}

CPPTWnd::~CPPTWnd()
{
	delete m_pBtnFirst;
	delete m_pBtnPrev;
	delete m_pBtnNext;
	delete m_pBtnLast;
	delete m_pBtnGoto;
	delete m_pPageNoEdit;
	delete m_pPPTPanel;
	delete m_pBmpLabel1;
	delete m_pBmpLabel2;
	//delete m_pBmpLabel3;
	delete m_pBtnLocate;
	delete m_pBtnLocateNext;
	delete m_pBtnLocatePrev;
	delete m_pSliderWnd;
	delete m_pToolTip;

	if (m_hmodHook)
	{
		FreeLibrary(m_hmodHook);
		m_hmodHook = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPPTWnd, CWnd)
	//{{AFX_MSG_MAP(CPPTWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BUTTON_UP, OnButtonUp)
	ON_MESSAGE(WM_ENTER_PRESSED, OnEnterPressed)
	ON_MESSAGE(WM_NEXT_SLIDE, OnNextSlide)
	ON_MESSAGE(WM_PREVIOUS_SLIDE, OnPreviousSlide)
	ON_MESSAGE(WM_FIRST_SLIDE, OnFirstSlide)
	ON_MESSAGE(WM_LAST_SLIDE, OnLastSlide)
	ON_MESSAGE(WM_END_SLIDE, OnEndSlide)
	ON_MESSAGE(WM_LBTN_DOWN, OnLBtnDown)
	ON_MESSAGE(WM_SLIDER_HSCROLL, OnSliderHScroll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPPTWnd message handlers
BOOL CPPTWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	//if (m_strClass == "")
	//	m_strClass = AfxRegisterWndClass(CS_DBLCLKS, 0, NULL);
	BOOL bRet = CreateEx(NULL, NULL, dwStyle, rect, pParentWnd, 0, NULL);

	//add 2014/7/30 kagata BugNo.130
	nVer = ChkPPTver();
	//add 2014/8/5 kagata BugNo.130
	nPageflag = 0;

	return bRet;
}

BOOL CPPTWnd::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							 UINT nID, CCreateContext* pContext)
{
	CRect rc = rect;
	if (!CWnd::CreateEx(WS_EX_DLGMODALFRAME, lpszClassName, lpszWindowName, dwStyle, 
						rect.left, rect.top, rc.Width(), rc.Height(), pParentWnd->m_hWnd, (HMENU)nID))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CPPTWnd::OnEraseBkgnd(CDC* pDC) 
{
	CBrush backBrush(GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
	
	return CWnd::OnEraseBkgnd(pDC);
}

BOOL CPPTWnd::LoadPPT(CString strPath, int nPPTWidth, int nPPTHeight) 
{
	CWnd* pMainFrm = AfxGetMainWnd();
	pMainFrm->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrm->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
						   SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	pMainFrm->UpdateWindow();

	if (!m_pPPTPanel->LoadPPT(strPath, nPPTWidth, nPPTHeight))
		return FALSE;

	m_pPPTPanel->GetSliderTitle(m_strArrayTitle);
	m_pSliderWnd->SetRange(0, m_strArrayTitle.GetSize() - 1);

	CRect rcPanel;
	m_pPPTPanel->GetWindowRect(rcPanel);
	int nWidth = rcPanel.Width() + 30;
	int nHeight = rcPanel.Height() + 90;
	SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOMOVE);

	CSize cs = GetBmpDim(IDB_FIRST_SLIDE);
	int nBtnWidth = cs.cx / 4;
	ScreenToClient(rcPanel);

	CSize csSep = ::GetBmpDim(IDB_SEP);

	CRect rect;

	rect.left = rcPanel.left - 3;
	rect.right = rcPanel.right + 3;
	rect.top = rcPanel.bottom + 6;
	rect.bottom = rect.top + 30;
	m_pSliderWnd->MoveWindow(rect);
	
	rect.left = rcPanel.left;
	rect.right = rect.left + nBtnWidth;
	rect.top = rcPanel.bottom + 32;
	rect.bottom = rect.top + cs.cy;
	m_pBtnLocatePrev->MoveWindow(rect);

	rect.left = rect.right + 1;
	rect.right = rect.left + nBtnWidth;
	m_pBtnLocateNext->MoveWindow(rect);

	rect.left = rect.right + 3;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel1->MoveWindow(rect);

	rect.left = rect.right + 3;
	rect.right = rect.left + nBtnWidth;
	m_pBtnFirst->MoveWindow(rect);

	rect.left = rect.right + 1;
	rect.right = rect.left + nBtnWidth;
	m_pBtnPrev->MoveWindow(rect);

	rect.left = rect.right + 1;
	rect.right = rect.left + nBtnWidth;
	m_pBtnNext->MoveWindow(rect);

	rect.left = rect.right + 1;
	rect.right = rect.left + nBtnWidth;
	m_pBtnLast->MoveWindow(rect);

	rect.left = rect.right + 1;
	rect.right = rect.left + nBtnWidth;
	m_pBtnGoto->MoveWindow(rect);

	rect.left = rect.right + constSmallGap;
	rect.right = rect.left + constEditWidth;
	m_pPageNoEdit->MoveWindow(rect);

	rect.left = rect.right + 3;
	rect.right = rect.left + csSep.cx;
	m_pBmpLabel2->MoveWindow(rect);

	rect.left = rect.right + 3;
	rect.right = rect.left + nBtnWidth;
	m_pBtnLocate->MoveWindow(rect);

	pMainFrm->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, 
						   SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

	//if (hwTray)
	//{
	//	::SetWindowPos(hwTray, HWND_NOTOPMOST, 0, 0, 0, 0, 
	//			   SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	//}

	return TRUE;
}


void CPPTWnd::OnDestroy() 
{
	UnSetHook();

	m_pBtnLocate->DestroyWindow();
	m_pBtnLocatePrev->DestroyWindow();
	m_pBtnLocateNext->DestroyWindow();
	m_pBtnFirst->DestroyWindow();
	m_pBtnPrev->DestroyWindow();
	m_pBtnNext->DestroyWindow();
	m_pBtnLast->DestroyWindow();
	m_pBtnGoto->DestroyWindow();
	m_pPageNoEdit->DestroyWindow();
	m_pPPTPanel->DestroyWindow();
	m_pBmpLabel1->DestroyWindow();
	m_pBmpLabel2->DestroyWindow();
	//m_pBmpLabel3->DestroyWindow();
	m_pSliderWnd->DestroyWindow();

	CWnd::OnDestroy();

	//add 2013/11/05 kagata
	CWnd* pMainFrm = AfxGetMainWnd();
	pMainFrm->UpdateWindow();
}

int CPPTWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcPos;
	GetClientRect(rcPos);
	rcPos.left = 10;
	rcPos.top = 10; 

	if (!m_pPPTPanel->Create(WS_VISIBLE | WS_CHILDWINDOW | WS_OVERLAPPED | 
							 WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
						     rcPos, this))
	{
		return -1;
	}

	m_pPPTPanel->SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
				              SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	CRect rect;
	rect.SetRectEmpty();
	m_pBtnFirst->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_FIRST_SLIDE);
	m_pBtnPrev->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_PREV_SLIDE);
	m_pBtnNext->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_NEXT_SLIDE);
	m_pBtnLast->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LAST_SLIDE);
	m_pBtnGoto->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_GOTO);
	m_pBmpLabel1->Create(WS_CHILD | WS_VISIBLE, rect, this);
	m_pBmpLabel2->Create(WS_CHILD | WS_VISIBLE, rect, this);
	//m_pBmpLabel3->Create(WS_CHILD | WS_VISIBLE, rect, this);
	m_pBtnLocate->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LOCATE);
	m_pBtnLocatePrev->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LOCATE_PREV);
	m_pBtnLocateNext->Create(WS_CHILD | WS_VISIBLE, rect, this, ID_BTN_LOCATE_NEXT);

	m_pSliderWnd->Create(WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_HORZ, rect, this, ID_SLIDER);
	m_pSliderWnd->SetRange(0, 0);
	m_pSliderWnd->SetChannelColor(RGB(128, 128, 196));

	m_pPageNoEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rect, this, ID_PAGE_NO_EDIT);
	m_pPageNoEdit->SetFont(MACRO_SYS_FONT);
	m_pPageNoEdit->SetWindowText("1");

	m_pToolTip->Create(this);
	m_pToolTip->SetDrawFont(MACRO_SYS_FONT2);

    if (!m_hmodHook)
    {
		if (!(m_hmodHook = LoadLibrary("HookProc.dll")))
	    {
            W_SHOW_ERROR(IDS_CANNOT_LOAD_DLL, this);
			return FALSE;
        }
		SetHook();
    }

	return 0;
}

LRESULT CPPTWnd::OnButtonUp(WPARAM wParam, LPARAM lParam)
{
	long nID = wParam;
	switch (nID)
	{
		case ID_BTN_FIRST_SLIDE:
		{
			OnFirst();
			break;
		}

		case ID_BTN_PREV_SLIDE:
		{
			OnPrev();
			break;
		}

		case ID_BTN_NEXT_SLIDE:
		{
			OnNext();
			break;
		}
		
		case ID_BTN_LAST_SLIDE:
		{
			OnLast();
			break;
		}

		case ID_BTN_LOCATE:
		{
			OnLocate();
			break;
		}

		case ID_BTN_LOCATE_PREV:
		{
			OnLocatePrev();
			break;
		}

		case ID_BTN_LOCATE_NEXT:
		{
			OnLocateNext();
			break;
		}

		case ID_BTN_GOTO:
		{
			OnGotoTitle();
			break;
		}
	}

	return 0L;
}

LRESULT CPPTWnd::OnEnterPressed(WPARAM wParam, LPARAM lParam)
{
	if (GetFocus() != m_pPageNoEdit)
		return 0L;

	CString strSlideNo;
	m_pPageNoEdit->GetWindowText(strSlideNo);

	int i = atoi(strSlideNo);
	int nSliderCount = m_pPPTPanel->GetSlideCount();
	if (i <= 0 || i > nSliderCount)
	{
		W_SHOW_WARNING(IDS_SLIDE_OUT_OF_RANGE, this);
		return 0L;
	}
	
	OnGoto(i);

	SetFocus();

	return 0L;
}

BOOL CPPTWnd::SetPageNo()
{
	int n = m_pPPTPanel->GetCurrentSlideNumber();
	int nCount = m_pPPTPanel->GetSlideCount();

	//add 2013/10/11 kagata
	int g = 0;
	if ( n == 0 )
	{
		while( n == 0 && g != 100 )
		{
			g++;
			Sleep(100);
			n = m_pPPTPanel->GetCurrentSlideNumber();
		}
	}
	/////////////

	//add 2014/7/29 kagata BugNo.130
	if(n != prev){
		nPageflag = 0;
	}else{
		nPageflag = nPageflag + 1;
	}
	/////////////
	
	if (nCount < n)
	{
		W_SHOW_WARNING(IDS_LAST_SLIDE, this);
		m_pPPTPanel->PrevSlide();
		return FALSE;
	}

	CString strSlideNo;
	m_pPageNoEdit->GetWindowText(strSlideNo);
	if (strSlideNo != n)
	{
		strSlideNo.Format("%d", n);
		m_pPageNoEdit->SetWindowText(strSlideNo);
		m_pSliderWnd->SetPos(n - 1);
	}

	return TRUE;
}

void CPPTWnd::OnGoto(int i)
{
	BeginWaitCursor();
	m_pPPTPanel->GotoSlide(i);
	m_strOp = "=";
	GetParent()->SendMessage(WM_CHANGE_SLIDE);
	SetPageNo();
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnGotoTitle()
{
	CDlgGotoSlide dlg;
	dlg.m_pstrArrayTitle = &m_strArrayTitle;
	dlg.m_nIndex = m_pPPTPanel->GetCurrentSlideNumber(); 
	if (dlg.DoModal() == ID_GO)
	{
		OnGoto(dlg.m_nIndex);
	}
}

void CPPTWnd::OnLast()
{
	BeginWaitCursor();
	m_pPPTPanel->LastSlide();
	m_strOp = "=";
	int nLast = m_pPPTPanel->GetSlideCount();
	GetParent()->SendMessage(WM_CHANGE_SLIDE);
	SetPageNo();
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnNext()
{
	GetSlideWnd();	//add 2014/10/22 kagata

	BeginWaitCursor();
	m_pPPTPanel->NextSlide();
	if (SetPageNo())
	{
		m_strOp = "+";
		GetParent()->SendMessage(WM_CHANGE_SLIDE);
	}
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnPrev()
{
	BeginWaitCursor();
	m_pPPTPanel->PrevSlide();
	m_strOp = "-";
	GetParent()->SendMessage(WM_CHANGE_SLIDE);
	SetPageNo();
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnLocate()
{
	BeginWaitCursor();
	GetParent()->SendMessage(WM_GET_TIME_STAMP);
	GetParent()->SendMessage(WM_ADD_PPT_RECORD);
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnLocatePrev()
{
	BeginWaitCursor();
	GetParent()->SendMessage(WM_GET_TIME_STAMP);
	m_pPPTPanel->PrevSlide();
	m_strOp = "-";
	GetParent()->SendMessage(WM_CHANGE_SLIDE);
	SetPageNo();
	GetParent()->SendMessage(WM_ADD_PPT_RECORD);
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnLocateNext()
{
	BeginWaitCursor();

	//add2014/7/29 kagata BugNo.130
	prev = m_pPPTPanel->GetCurrentSlideNumber();

	GetParent()->SendMessage(WM_GET_TIME_STAMP);

	GetSlideWnd();	//add 2014/10/22 kagata

	m_pPPTPanel->NextSlide();
	if (SetPageNo())
	{
		m_strOp = "+";
		GetParent()->SendMessage(WM_CHANGE_SLIDE);
		GetParent()->SendMessage(WM_ADD_PPT_RECORD);	//replace 2014/10/22 kagata//del 2014/7/29 kagata BugNo.130

		//del 2014/10/22 kagata
		//add 2014/7/29 kagata BugNo.130
		//nVer = ChkPPTver();
		/*CString str;
		str.Format(_T("%d"), nVer);
		AfxMessageBox(str);*/
		//if(nVer < 14){
		//	GetParent()->SendMessage(WM_ADD_PPT_RECORD);
		//}else{
		//	if(nPageflag == 1){
		//		OnLocateNext();
		//	}else{
		//		GetParent()->SendMessage(WM_ADD_PPT_RECORD);
		//	}
		//}
		//add 2014/7/29 kagata BugNo.130
		//del 2014/10/22 kagata
	}
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CPPTWnd::OnFirst()
{
	BeginWaitCursor();
	m_pPPTPanel->FirstSlide();
	m_strOp = "=";
	GetParent()->SendMessage(WM_CHANGE_SLIDE);
	SetPageNo();
	EndWaitCursor();

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

BOOL CPPTWnd::SetHook()
{   
	if (!m_hmodHook)
		return FALSE;

	if (m_hHook)
		UnSetHook();
 
    if (!m_hHook)
    {
		//replase 2014/10/22 kagata
		//if (!(m_hHook = SetWindowsHookEx(WH_KEYBOARD,
	    //    (HOOKPROC)GetProcAddress(m_hmodHook, "HookProcedure"), m_hmodHook, 0)))  //leu;20060609
	    if (!(m_hHook = SetWindowsHookEx(WH_GETMESSAGE,
	        (HOOKPROC)GetProcAddress(m_hmodHook, "HookProcedure"), m_hmodHook, 0)))  //leu;20060609
		{
            return FALSE;
        }
    }

	return TRUE;
};

void CPPTWnd::UnSetHook()
{
	if (m_hHook)
	{
		::UnhookWindowsHookEx(m_hHook);
		m_hHook = NULL;
	}
};

LRESULT CPPTWnd::OnLBtnDown(WPARAM wParam, LPARAM lParam)
{
	//del 2014/10/22 kagata
	/*if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnNext();
	}*/

	return 0L;
}

LRESULT CPPTWnd::OnFirstSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnFirst();
	}
	return 0L;
}

LRESULT CPPTWnd::OnLastSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnLast();
	}
	return 0L;
}

LRESULT CPPTWnd::OnEndSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
	}
	return 0L;
}

LRESULT CPPTWnd::OnNextSlide(WPARAM wParam, LPARAM lParam)
{
	if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnNext();
	}
	return 0L;
}

LRESULT CPPTWnd::OnPreviousSlide(WPARAM wParam, LPARAM lParam)
{
	//del 2014/10/22 kagata
	/*if (m_lTime != wParam)
	{
		m_lTime = wParam;
		OnPrev();
	}*/
	return 0L;
}

void CPPTWnd::GetSliderTitle(CStringArray& strArray)
{
	m_pPPTPanel->GetSliderTitle(strArray);
}

void CPPTWnd::ShowToolTip(long nPos)
{
	CPoint pt;
	GetCursorPos(&pt);
	pt.x += 6;
	pt.y += 20;

	CString strTitle;
	strTitle.Format("(%d) %s", nPos + 1, m_strArrayTitle.GetAt(nPos));
	m_pToolTip->ShowText(pt, strTitle);
}

LRESULT CPPTWnd::OnSliderHScroll(WPARAM wParam, LPARAM lParam) 
{
	CSliderWnd* pSlider = (CSliderWnd*)lParam;
	int nID = pSlider->GetDlgCtrlID();
	if (nID == ID_SLIDER)
	{
		int nSPos = pSlider->GetPos();
		switch (wParam)
		{
			case SB_THUMBTRACK:
				ShowToolTip(nSPos);
				UpdateWindow();
			break;

			case SB_ENDSCROLL:
				m_pToolTip->ShowWindow(SW_HIDE);
				UpdateWindow();
			break;

			case SB_THUMBPOSITION:
			{
				OnGoto(nSPos + 1);
				UpdateWindow();
				break;
			}
		}
	}

	return 0L;
}

int CPPTWnd::GetCurrentSlideNumber()
{
	return m_pPPTPanel->GetCurrentSlideNumber();
};

int CPPTWnd::GetCurrentAnimaNumber()
{
	return m_pPPTPanel->GetCurrentAnimaNumber();
};

//add 2014/7/29 kagata BugNo.130
int CPPTWnd::ChkPPTver()
{
	int nVer = 0;

	if (RegKeyExists("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Office\\15.0\\PowerPoint"))
        nVer = 15;
	if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\15.0\\PowerPoint"))
        nVer = 15;

	if(nVer == 0)
	{
		if (RegKeyExists("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Office\\14.0\\PowerPoint"))
			nVer = 14;
		if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\14.0\\PowerPoint"))
		    nVer = 14;
	}

	if(nVer == 0)
	{
		if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\12.0\\PowerPoint"))
	        nVer = 12;
		else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\11.0\\PowerPoint"))
	        nVer = 11;
		else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\10.0\\PowerPoint"))
			nVer = 10;
		else if (RegKeyExists("HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\9.0\\PowerPoint"))
			nVer = 9;
	}

	return nVer;
}

//add 2014/10/22 kagata
BOOL CPPTWnd::GetSlideWnd()
{
	HWND hWndArray[10];
	for (int i = 0; i < 10; i++)
		hWndArray[i] = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)hWndArray);

	return TRUE;
}

//add 2014/10/22 kagata
BOOL CALLBACK CPPTWnd::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClassName[256];
	TCHAR szCaptionName[256];
	GetClassName(hWnd, szClassName, 256);
	::GetWindowText(hWnd, szCaptionName, 256);
	CString strTitle;
	strTitle.LoadString(IDS_PPT_SYNC_WINDOW);
	if (lstrcmp(szCaptionName, strTitle/*"ƒXƒ‰ƒCƒh‚ðÄ¶"*/) != 0)
	{
		return TRUE;
	}else{
		HWND hCWndArray[10];
		for (int i = 0; i < 10; i++)
			hCWndArray[i] = NULL;
		EnumChildWindows(hWnd, EnumChildProc, (LPARAM)hCWndArray);

	return FALSE;
	}
}

//add 2014/10/22 kagata
BOOL CALLBACK CPPTWnd::EnumChildProc(HWND htwnd, LPARAM lParam)
{
	TCHAR szClassNameChild[256];
	TCHAR szCaptionNameChild[256];
	GetClassName(htwnd, szClassNameChild, 256);
	::GetWindowText(htwnd, szCaptionNameChild, 256);
	//AfxMessageBox(szClassNameChild);
	if (lstrcmp(szClassNameChild, "screenClass") != 0)
	{
		return TRUE;
	}else{
		//::SendMessage(htwnd, WM_SETFOCUS, 0, 0);
		::SendMessage(htwnd, WM_ACTIVATE, 00000002, 00000000);
		return FALSE;
	}
}
