// BasicCheckListBox.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "BasicCheckListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicCheckListBox

CBasicCheckListBox::CBasicCheckListBox()
{
}

CBasicCheckListBox::~CBasicCheckListBox()
{
}


BEGIN_MESSAGE_MAP(CBasicCheckListBox, CCheckListBox)
	//{{AFX_MSG_MAP(CBasicCheckListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicCheckListBox message handlers

void CBasicCheckListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i = 0;
	CWordArray wArrayCheck;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck.Add(GetCheck(i) ? 1 : 0);
	
	CCheckListBox::OnLButtonDown(nFlags, point);

	CWordArray wArrayCheck2;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck2.Add(GetCheck(i) ? 1 : 0);

	for (i = 0; i < wArrayCheck.GetSize(); i++)
	{
		if (wArrayCheck[i] != wArrayCheck2[i])
		{
			GetParent()->SendMessage(WM_CHECK_CHANGE, i);
			return;
		}
	}
}

void CBasicCheckListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWordArray wArrayCheck;
	int i = 0;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck.Add(GetCheck(i) ? 1 : 0);
	
	CCheckListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	CWordArray wArrayCheck2;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck2.Add(GetCheck(i) ? 1 : 0);

	for (i = 0; i < wArrayCheck.GetSize(); i++)
	{
		if (wArrayCheck[i] != wArrayCheck2[i])
		{
			GetParent()->SendMessage(WM_CHECK_CHANGE, i);
			return;
		}
	}
}

void CBasicCheckListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int i = 0;
	CWordArray wArrayCheck;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck.Add(GetCheck(i) ? 1 : 0);
	
	CCheckListBox::OnLButtonDblClk(nFlags, point);

	CWordArray wArrayCheck2;
	for (i = 0; i < GetCount(); i++)
		wArrayCheck2.Add(GetCheck(i) ? 1 : 0);

	for (i = 0; i < wArrayCheck.GetSize(); i++)
	{
		if (wArrayCheck[i] != wArrayCheck2[i])
		{
			GetParent()->SendMessage(WM_CHECK_CHANGE, i);
			return;
		}
	}
}
