// hintlabe.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "noteedit.h"
#include "uigrid.h"
#include "grdapi.h"
#include "gridctrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoteEdit

CNoteEdit::CNoteEdit()
{
	u_pEditable = TRUE;
	u_pEditWnd = NULL;
	u_pGridWnd = NULL;
	//u_bNoFocus = FALSE;
}

CNoteEdit::~CNoteEdit()
{
    delete u_pEdit;
}   

BEGIN_MESSAGE_MAP(CNoteEdit, CWnd)
	//{{AFX_MSG_MAP(CNoteEdit)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_EDIT_KEY_DOWN, OnEditKeyDown)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_EDIT_LOSE_FOCUS_MODIFIED, OnLoseFocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteEdit message handlers

BOOL CNoteEdit::Create(LPCSTR lpszClassName,
					   DWORD dwStyle,
					   const RECT& rect)
{
	return CreateEx(0, lpszClassName, "", dwStyle,
					rect.left, rect.top, rect.right - rect.left, 
					rect.bottom - rect.top,	NULL, (HMENU)NULL);
}


int CNoteEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	u_pEdit = new CGridEdit;
	CRect rc;
	rc.SetRect(0, 0, 0, 0);
	u_pEdit->u_bMutiLine = TRUE;
   	u_pEdit->Create(WS_CHILD | WS_TABSTOP | ES_LEFT | WS_VSCROLL |
					ES_WANTRETURN | ES_AUTOVSCROLL | ES_MULTILINE | WS_BORDER,
    				rc, this, 34533, TRUE);
    
	return 0;
}

void CNoteEdit::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	u_pEdit->SetWindowPos(&wndTop, 0, 0, cx - 1, cy - 1, SWP_SHOWWINDOW);
}

LRESULT CNoteEdit::OnEditKeyDown(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
	{
		//case VK_RETURN:
		case VK_ESCAPE:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		{
			ShowWindow(SW_HIDE);

			CString StrText;
			u_pEdit->GetWindowText(StrText);
			if (u_pEditWnd != NULL)
			{
				u_pEditWnd->SetWindowText(StrText);
				//mAppView->SendMessage(WM_EDIT_KEY_DOWN, wParam, (long)u_pEditWnd);
			}
			else if (u_pGridWnd != NULL)
			{
				int nRow = u_pGridWnd->u_PreRow;
				int nCol = u_pGridWnd->u_PreCol;
				CString StrOrig = u_pGridWnd->GetDataAt(nRow, nCol);
				if (StrOrig != StrText)
				{
					u_pGridWnd->SetDataAt(nRow, nCol, StrText);
					u_pGridWnd->OnGrdUpdate(nRow, nCol, TRUE, StrText);
				}

				u_pGridWnd->SendMessage(WM_EDIT_KEY_DOWN, wParam, 0);
			}
		}
	}

	return 0L;
}


void CNoteEdit::OnClose() 
{
	//GetWindowRect(mMainFrm->u_NoteRect);
	ShowWindow(SW_HIDE);
	return;
}

LRESULT CNoteEdit::OnLoseFocus(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);

	//if (mAppView == NULL)
	//	return 0L;


	CString StrText;
	u_pEdit->GetWindowText(StrText);

	if (u_pEditWnd != NULL && ::IsWindow(u_pEditWnd->m_hWnd))
	{
		u_pEditWnd->SetWindowText(StrText);
		((CEdit*)u_pEditWnd)->SetModify(u_pEdit->GetModify());
		//mAppView->SendMessage(WM_EDIT_LOSE_FOCUS_MODIFIED, 0, (LPARAM)u_pEditWnd);
	}
	else if (u_pGridWnd != NULL && ::IsWindow(u_pGridWnd->m_hWnd))
	{
		int nRow = u_pGridWnd->u_PreRow;
		int nCol = u_pGridWnd->u_PreCol;
		CString StrOrig = u_pGridWnd->GetDataAt(nRow, nCol);
		if (StrOrig != StrText)
		{
			u_pGridWnd->SetDataAt(nRow, nCol, StrText);
			u_pGridWnd->OnGrdUpdate(nRow, nCol, TRUE, StrText);
		}
	}


	return 0L;
}

void CNoteEdit::SetGridWnd(CUIGrid* pGrid)
{
	u_pGridWnd = pGrid;
	u_pEditWnd = NULL;
}

void CNoteEdit::SetEditWnd(CWnd* pWnd)
{
	u_pGridWnd = NULL;
	u_pEditWnd = pWnd;
}

void CNoteEdit::SetFontEx(CFont* pFont)
{
	u_pEdit->SetFontEx(pFont);
}