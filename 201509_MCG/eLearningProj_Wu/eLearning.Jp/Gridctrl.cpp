#include "stdafx.h"
#include "resource.h"
#include "gridctrl.h"
#include "bmpapi.h"
#include "grdapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif 

BOOL CGridEdit::u_bOverWrite = FALSE;

CGridEdit::CGridEdit()
{  
	u_bMutiLine = FALSE;
	u_bEditable = TRUE;
	u_TextLimit = 0;
	u_LimitReached = 39;
	u_AllowedStr = "";
	u_bLeading = FALSE;
	u_bRet = FALSE;
}

CGridEdit::~CGridEdit()
{
}

IMPLEMENT_DYNAMIC(CGridEdit, CEdit)

BEGIN_MESSAGE_MAP(CGridEdit, CEdit)
	//{{AFX_MSG_MAP(CGridEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEdit message handlers

BOOL CGridEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL b3D)			
{
	//if ((dwStyle | ES_MULTILINE) == dwStyle)
	//	u_bMutiLine = TRUE;
	
	BOOL bRet;
	if (b3D) 
		bRet = CWnd::CreateEx(WS_EX_CLIENTEDGE, "EDIT", "", dwStyle,
						      rect.left, rect.top, 
						      rect.right - rect.left + 1, 
						      rect.bottom - rect.top + 1, 
						      pParentWnd->GetSafeHwnd(), (HMENU)nID);
	else
		bRet = CWnd::Create("EDIT", "", dwStyle, rect, pParentWnd, nID);

	//CRect rc;
	//rc.SetRect(0, 0, 20, 30);
	//SetRect(rc);

	return bRet;
}

void CGridEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!u_bEditable)
		return;
	
	if (nChar == VK_TAB)
	{
		if (!u_bMutiLine)
			return;
		else
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
	}

	if (nChar == VK_RETURN)
	{
		if (u_bMutiLine)
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if (nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if (nChar == VK_ESCAPE)
		return;
		
	if (u_AllowedStr != "")
	{
		if (u_AllowedStr.Find(nChar) < 0)
		{
			MessageBeep(0xFFFFFFFF);
			return;
		}
	}

    int nStartChar, nEndChar;
	GetSel(nStartChar, nEndChar);

	CString StrTxt;
	GetWindowText(StrTxt);
	int nLen = StrTxt.GetLength();
	if (u_TextLimit != 0 && u_LimitReached == 0)
	{
		BOOL bMarked = (nStartChar != nEndChar);
		BOOL bMarked1 = (nEndChar - nStartChar == 1);
		BOOL bBeep1 = (!bMarked && 
					   (nLen == u_TextLimit ||
					   (nLen == u_TextLimit - 1 && IsDBCSLeadByte(nChar))));
		BOOL bBeep2 = (bMarked1 && 
					   nLen == u_TextLimit &&
					   IsDBCSLeadByte(nChar));
		if (bBeep1 || bBeep2)
		{
			MessageBeep(0xFFFFFFFF);
			return;
		}
	}

	if (u_bOverWrite)
	{
		if (nEndChar == nStartChar)
		{
			if (nLen <= nStartChar)
				nEndChar++;
			else
			{
				char nCh = StrTxt.GetAt(nStartChar);
				nEndChar += (IsDBCSLeadByte(nCh)) ? 2 : 1;
			}
			SetSel(nStartChar, nEndChar);
		}
		
		BOOL CurCharLeading = (!u_bLeading && IsDBCSLeadByte(nChar));
		if (CurCharLeading)
		{
			u_szBuf[0] = nChar;
			u_szBuf[1] = '\0';
		}
		else 
		{
			if (u_bLeading)
			{
				u_szBuf[1] = nChar;
				u_szBuf[2] = '\0';
			}
			else
			{
				u_szBuf[0] = nChar;
				u_szBuf[1] = '\0';
			}

			ReplaceSel(u_szBuf);
		}

		u_bLeading = CurCharLeading;
	}
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	//if (u_bOverWrite)
	//	SendMessage(WM_KEYDOWN, VK_DELETE, 1L) ;

	if (u_TextLimit != 0 && u_LimitReached != 0)
	{
		GetWindowText(StrTxt);
		if (StrTxt.GetLength() >= u_TextLimit)
			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, u_LimitReached, (LPARAM)this);
	}

	//if (u_LineLimit != 0 && u_bMutiLine)
	//{
		//if (LineLength(-1) >= u_LineLimit)
		//{
		//	CString StrText;
		//	GetWindowText(StrText);
		//	StrText += "\r\n";
		//	SetWindowText(StrText);
		//}
	//}
}

void CGridEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if (u_bKeyDownNoProc)                           //1998-03-20 Add this varible for not processing
	//{                                               //WM_KEY_DOWN message when VK_DOWN comes.
	//	if (nChar == VK_UP || nChar == VK_DOWN)     //OnSelChange() will be called, and then it will disable here.
	//	{											//By WUSC 
	//		u_bKeyDownNoProc = FALSE;                   
	//		return;                   
	//	}
	//}

	BOOL bMove = FALSE;
	u_bRet = FALSE;
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			if (!u_bMutiLine && !GetParent()->IsKindOf(RUNTIME_CLASS(CGridComboEdit)))
        		bMove = TRUE;
			break;

		case VK_RETURN:
			bMove = TRUE;
			u_bRet = TRUE;
			break;
	
		case VK_LEFT:
		case VK_RIGHT:
		{
    		CString EditStr;
    		GetWindowText(EditStr);
			int TotalLength = EditStr.GetLength();
			CPoint CaretPos = CWnd::GetCaretPos();
			int nIndex = CharFromPos(CaretPos);
			int nLineIndex = HIWORD(nIndex);
			int nWordIndex = LOWORD(nIndex);
			if (!u_bMutiLine)
			{
	    		DWORD Selection = GetSel();
	    		int StartPos = LOWORD(Selection);
	    		int StopPos = HIWORD (Selection);
	    		BOOL SelectAll = (StartPos == 0 && StopPos == TotalLength);
    			
				if ((nIndex > TotalLength - 1 && nChar == VK_RIGHT) || 
		    		(nIndex == 0 && nChar == VK_LEFT) || 
					(SelectAll && nChar == VK_RIGHT))
				{	
					bMove = TRUE;
				}
			}
			break;
		}
		
		case VK_PRIOR:
		case VK_NEXT:
			bMove = TRUE;
			break;

		case VK_TAB:
		{
			if (u_bMutiLine)
				break;

			SHORT ch = ::GetKeyState(VK_SHIFT);
			BOOL bShiftDown = (ch & '0x80');
			if (bShiftDown)
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_LEFT, (LPARAM)this);
			else
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_RIGHT, (LPARAM)this);
			return;
		}

		case VK_DELETE:
		case VK_SHIFT:
		case VK_CONTROL:
			break;
			
		default:
			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, (LPARAM)this);
	}

	if (bMove)
	{
		GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, (LPARAM)this);
		return;
    }

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
} 

void CGridEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WM_EDIT_LOSE_FOCUS_MODIFIED, 0, (LPARAM)this);
}

void CGridEdit::SetFontEx(CFont* pFont)
{
	DWORD Margin = GetMargins();
	SetFont(pFont);
	SetMargins(LOWORD(Margin), HIWORD(Margin));
}


/////CUICombo
IMPLEMENT_DYNAMIC(CUICombo, CGridCombo)
BEGIN_MESSAGE_MAP(CUICombo, CGridCombo)
	//{{AFX_MSG_MAP(CUICombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CUICombo::CUICombo()
{
	u_pUnknownBMP = NULL;
}

CUICombo::~CUICombo()
{
	if (u_pUnknownBMP != NULL)
	{
		u_pUnknownBMP->DeleteObject();
		delete u_pUnknownBMP;
	}
}

void CUICombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
    if (lpMIS->itemData == NULL)
    	return;

    //int BMPId = atoi((LPCTSTR)lpMIS->itemData) - 1;
	int BMPId = u_BMPId;
	int nIndex = ::GetIndexFromID(u_pBMPIDArray, BMPId);
	CBitmap* pBMP;
	if (nIndex < 0)
		pBMP = u_pUnknownBMP;
	else	
		pBMP = (CBitmap*)u_pBMPArray->GetAt(nIndex);
	BITMAP BMPBuffer;
	pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
	lpMIS->itemHeight = BMPBuffer.bmHeight + 2; 
}

void CUICombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	//COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data
    char szBMPId[10];
    GetLBText(lpDIS->itemID, szBMPId);
    int BMPId = atoi(szBMPId);
	int nIndex = ::GetIndexFromID(u_pBMPIDArray, BMPId);
    
	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		//CBrush br(cr);
        CBitmap* pBMP;
        ::FillRect(pDC->m_hDC, &lpDIS->rcItem, (HBRUSH)::GetStockObject(WHITE_BRUSH));
        
		if (nIndex < 0)
		{
	  	    if (u_pUnknownBMP == NULL)
   			{
   	    		u_pUnknownBMP = new CBitmap;
   	    		u_pUnknownBMP->LoadBitmap(IDB_UNKNOWN);
   			}	
			pBMP = u_pUnknownBMP;
		}	
		else	
			pBMP = (CBitmap*)u_pBMPArray->GetAt(nIndex);
		BITMAP BMPBuffer;
   		pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
        int rcWidth = lpDIS->rcItem.right - lpDIS->rcItem.left + 1;
        int rcHeight = lpDIS->rcItem.bottom - lpDIS->rcItem.top + 1; 
   		
   		int bmWidth = BMPBuffer.bmWidth;
   		int bmHeight = BMPBuffer.bmHeight;
        RECT RectBMP = {0, 0, bmWidth - 1, bmHeight - 1};              
     
        //CUIGrid* pGrid = (CUIGrid*)GetParent();
		//int PreRow = pGrid->u_PreRow;
		//int PreCol = pGrid->u_PreCol;
		//int Align = pGrid->GetAttribAt(PreRow, PreCol, cAlign);
        int Indent;
        if (u_Align == 0)
        	Indent = u_InitX;
        else
        	Indent = (rcWidth - bmWidth) / 2;
        int nTopGap = max(0, (rcHeight - bmHeight) / 2);	
        RECT RectDraw = {Indent, lpDIS->rcItem.top + nTopGap + 1, Indent + bmWidth, lpDIS->rcItem.top + 1 + nTopGap + bmHeight};
  		::DDBPaint(pDC->m_hDC, &RectDraw, (HBITMAP)pBMP->m_hObject, &RectBMP, NULL);  
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		COLORREF crHilite = RGB(0, 0, 0);
		CBrush br(crHilite);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		COLORREF cr = RGB(255, 255, 255);
		CBrush br(cr);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}
} 

///////////////////////////////////////////////////////////////////////////
//CGridCombo
//FARPROC CGridCombo::lpfnOld;

CGridCombo::CGridCombo()
{  
}

CGridCombo::~CGridCombo()
{
}

IMPLEMENT_DYNAMIC(CGridCombo, CComboBox)
BEGIN_MESSAGE_MAP(CGridCombo, CComboBox)
	//{{AFX_MSG_MAP(CGridCombo)
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridComboEdit message handlers

void CGridCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WM_EDIT_LOSE_FOCUS_MODIFIED, 0, (LPARAM)this);
}

void CGridCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
		{
			int nCount = GetCount();
			if (nCount == 0)
			{
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
				return;
			}

			int nSel = GetCurSel();
			if (nSel == CB_ERR)
				nSel = 0;
			else
			{
				if (nChar == VK_UP)
					nSel--;
				else
					nSel++;
			}
			
			if (nSel < nCount && nSel >= 0)
			{
				SetCurSel(nSel);
				return;
			}

			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
			return;
		}

	
		case VK_LEFT:
		case VK_RIGHT:
		case VK_RETURN:
		case VK_PRIOR:
		case VK_NEXT:
		{
			if (GetDroppedState() && (nChar == VK_UP || nChar == VK_DOWN))
			{
				CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
				return;
			}

			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this)); 
			return;
   		}

		case VK_TAB:
		{
			SHORT ch = ::GetKeyState(VK_SHIFT);
			BOOL bShiftDown = (ch & '0x80');
			if (bShiftDown)
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_LEFT, LPARAM(this));
			else
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_RIGHT, LPARAM(this));
			return;
		}

		case VK_SPACE:
		{
			ShowDropDown(!GetDroppedState());
			break;
		}

		default:
			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
	}

   	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
} 

HBRUSH CGridCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(u_rgbFG);
	return (HBRUSH)u_pBrush->m_hObject;
}


////////////////CGridComboEdit
#define cGap 5

CGridComboEdit::CGridComboEdit()
{  
	u_pEdit = NULL;
	u_ColNo = 1;
	u_bFirst = TRUE;
	u_pColWidthArray = new CWordArray;
	u_pColWidthArray->Add(0);
	u_nCurSel = CB_ERR;
	u_bDestroying = FALSE;
	u_BGColor = RGB(255, 255, 255);
	u_FGColor = RGB(0, 0, 0);
	u_hBgColor = ::CreateSolidBrush(u_BGColor);
	u_bNoPartial = FALSE;
	u_bNoDrawIfOne = TRUE;
	u_bDropDown = TRUE;
}

CGridComboEdit::~CGridComboEdit()
{
	delete u_pEdit;
	delete u_pColWidthArray;
}

IMPLEMENT_DYNAMIC(CGridComboEdit, CComboBox)

BEGIN_MESSAGE_MAP(CGridComboEdit, CComboBox)
	//{{AFX_MSG_MAP(CGridComboEdit)
	ON_WM_CREATE()
	ON_EN_CHANGE(1001, OnEditChange)
	ON_MESSAGE(WM_EDIT_KEY_DOWN, OnEditKeyDown)
	ON_MESSAGE(WM_EDIT_LOSE_FOCUS_MODIFIED, OnLoseFocus)
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorList)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridComboEdit message handlers

void CGridComboEdit::OnEditChange()
{
	SetModify(TRUE);
	GetParent()->SendMessage(WM_GRD_COMBOEDIT_CHANGE, 0, 0);
	if (u_ColNo == 1 || u_bNoPartial)
		return;
	
	CString StrTemp;
	u_pEdit->GetWindowText(StrTemp);
	if (StrTemp == "")
	{
		ShowDropDown(FALSE);
		return;
	}

	int nSize = u_pComboArray->GetSize();
	CString StrCombo;
	int nStrEditSize = StrTemp.GetLength();
	int nIndex = -1;
	for (int i = 0; i < nSize; i++)
	{
		StrCombo = ::GetRowColAt(u_pComboArray, i, 0);
		int nStrComboSize = StrCombo.GetLength();
		if (nStrComboSize < nStrEditSize)
			continue;
		if (StrCombo.Left(nStrEditSize) == StrTemp)
		{
			nIndex = i;
			break;
		}
	}
	u_nCurSel = nIndex;
	if (nIndex == -1)
	{
		::SendMessage(u_hwndList, LB_SETCURSEL, (WPARAM)-1, 0L);
		ShowDropDown(FALSE);
	}
	else 
	{
		ShowDropDown(TRUE);
		::SendMessage(u_hwndList, LB_SETCURSEL, nIndex, 0L);
	}
}

extern int nComboBtnWidth;

int CGridComboEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	//long nStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	InitCombo();

	return 0;
}

void CGridComboEdit::InitCombo()
{
	if (u_bDropDown)
	{
		HWND hWndEdit = ::GetWindow(m_hWnd, GW_CHILD);
		u_pEdit = new CGridEdit;
		u_pEdit->SubclassWindow(hWndEdit);

		if (nComboBtnWidth < 0)
		{
			CRect rcCombo;
			GetWindowRect(rcCombo);

			CRect rcEdit;
			u_pEdit->GetWindowRect(rcEdit);
			nComboBtnWidth = rcCombo.right - rcEdit.right - 4;
		}
	}
}

void CGridComboEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   	if (u_bDropDown)
	{
   		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	if (nChar != VK_LEFT && nChar != VK_RIGHT && nChar != VK_TAB)
   		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
		{
		//	int nCount = GetCount();
		//	if (nCount == 0)
		//	{
		//		GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
		//		return;
		//	}

		//	int nSel = GetCurSel();
		//	if (nSel == CB_ERR)
		//		nSel = 0;
		//	else
		//	{
		//		if (nChar == VK_UP)
		//			nSel--;
		//		else
		//			nSel++;
		//	}
			
		//	if (nSel < nCount && nSel >= 0)
		//	{
		//		SetCurSel(nSel);
		//		return;
		//	}

		//	GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
			return;
		}

	
		case VK_LEFT:
		case VK_RIGHT:
		case VK_RETURN:
		case VK_PRIOR:
		case VK_NEXT:
		{
			if (GetDroppedState() && (nChar == VK_UP || nChar == VK_DOWN))
			{
				CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
				return;
			}

			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this)); 
			return;
   		}

		case VK_TAB:
		{
			SHORT ch = ::GetKeyState(VK_SHIFT);
			BOOL bShiftDown = (ch & '0x80');
			if (bShiftDown)
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_LEFT, LPARAM(this));
			else
				GetParent()->SendMessage(WM_EDIT_KEY_DOWN, VK_RIGHT, LPARAM(this));
			return;
		}

		case VK_SPACE:
		{
			ShowDropDown(!GetDroppedState());
			break;
		}

		default:
			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, nChar, LPARAM(this));
	}
} 


LRESULT CGridComboEdit::OnEditKeyDown(WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_UP || wParam == VK_DOWN)
	{
		int nCount = GetCount();
		if (nCount == 0)
		{
			GetParent()->SendMessage(WM_EDIT_KEY_DOWN, wParam, LPARAM(this));
			return 0L;
		}

		int nSel = GetCurSel();
		//if (nSel == CB_ERR)
		//	nSel = 0;
		//else
		//{
		//	if (wParam == VK_UP)
		//		nSel--;
		//	else
		//		nSel++;
		//}
		
		if (nSel < nCount && nSel >= 0)
		{
			//SetCurSel(nSel);
			GetParent()->SendMessage(WM_COMBO_SEL_CHANGE, 0, 0);
			return 0L;
		}
	}

	GetParent()->SendMessage(WM_EDIT_KEY_DOWN, wParam, LPARAM(this));
	return 0L;
}

LRESULT CGridComboEdit::OnLoseFocus(WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = GetFocus();

	if (pWnd != NULL)
		if (pWnd->m_hWnd == u_hwndList || pWnd == this)
			return 0L;

	GetParent()->SendMessage(WM_EDIT_LOSE_FOCUS_MODIFIED, 0, (LPARAM)this);
	return 0L;
}

void CGridComboEdit::SetFontEx(CFont* pFont)
{
	SetFont(pFont);
	if (u_pEdit != NULL)
	{
		DWORD Margin = u_pEdit->GetMargins();
		u_pEdit->SetMargins(LOWORD(Margin), HIWORD(Margin));
	}
}


LRESULT CGridComboEdit::OnCtlColorList(WPARAM wParam, LPARAM lParam)
{
	HDC hDC = (HDC)wParam;
	::SetTextColor(hDC, u_FGColor);
	
	if (u_ColNo == 1 && u_bNoDrawIfOne)
		return (long)u_hBgColor;

    if (u_bFirst)
	{
        u_hwndList = (HWND)lParam ;     // HWND is 32 bits
	    ReCalcWidth();
		u_bFirst = FALSE;
	}

	CRect rcWindow;
	::GetWindowRect(u_hwndList, rcWindow);

	::SetWindowPos(u_hwndList, NULL, 0, 0, u_nMaxWidth, rcWindow.Height(), 
 				   SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER);

	return (long)u_hBgColor;
}

void CGridComboEdit::ReCalcWidth()
{
	HDC hDC = ::GetDC(u_hwndList);
	
	CRect rcWindow;
	::GetWindowRect(u_hwndList, rcWindow);

	u_nMaxWidth = rcWindow.Width();
	int nSize = u_pComboArray->GetSize();
	CString StrCombo;
	int i = 0;
	int j = 0;
	for (i = 0; i < nSize; i++)
	{
		for (j = 0; j < u_ColNo; j++)
		{
			StrCombo = ::GetRowColAt(u_pComboArray, i, j);
			SIZE sz;
			int nLen = StrCombo.GetLength();
			::GetTextExtentPoint32(hDC, StrCombo, nLen, &sz);
			int nWidth = u_pColWidthArray->GetAt(j);
			u_pColWidthArray->SetAt(j, (unsigned short)max(nWidth, sz.cx));
		}
	}

	int nTotal = 0;
	for (i = 0; i < u_ColNo; i++)
		nTotal += u_pColWidthArray->GetAt(i) + cGap;
	u_nMaxWidth = max(u_nMaxWidth, nTotal);
	
	int nMin, nMax;
	::GetScrollRange(u_hwndList, SB_VERT, &nMin, &nMax);
	if (nMin != 0 || nMax != 0)
		u_nMaxWidth += ::GetSystemMetrics(SM_CXVSCROLL);

	::ReleaseDC(u_hwndList, hDC);
}

void CGridComboEdit::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	pDC->SetBkMode(TRANSPARENT);

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		DrawSelect(pDC, lpDIS->rcItem, u_BGColor);
		pDC->SetTextColor(u_FGColor);
		
		lpDIS->rcItem.left += 3;
		DrawListText(pDC, lpDIS->itemID, &(lpDIS->rcItem));
		return;
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		DrawSelect(pDC, lpDIS->rcItem, ::GetSysColor(COLOR_HIGHLIGHT)); 
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		lpDIS->rcItem.left += 3;
		DrawListText(pDC, lpDIS->itemID, &(lpDIS->rcItem));
		return;
	}

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		pDC->SetTextColor(u_FGColor);
		lpDIS->rcItem.left += 3;
		DrawListText(pDC, lpDIS->itemID, &(lpDIS->rcItem));
	}
}

void CGridComboEdit::DrawListText(CDC* pDC, int nIndex, LPRECT prc)
{
	int Count = 0;
	RECT rc = *prc;
	rc.right = 0;
	CString StrCombo;

	for (int j = 0; j < u_ColNo; j++)
	{
		StrCombo = ::GetRowColAt(u_pComboArray, nIndex, j);
		
		int nWidth;
		if (u_bFirst)
		{
			SIZE sz;
			int nLen = StrCombo.GetLength();
			::GetTextExtentPoint32(pDC->m_hDC, StrCombo, nLen, &sz);
			nWidth = sz.cx;
		}
		else
			nWidth = u_pColWidthArray->GetAt(j);
		
		rc.left = rc.right + cGap;
		rc.right = rc.left + nWidth;	
		pDC->DrawText(StrCombo, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

void CGridComboEdit::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = u_Height;
}

void CGridComboEdit::DrawSelect(CDC* pDC, CRect rc, COLORREF rgb)
{
	CPen Pen1;

   	CPen* OldPen;
	Pen1.CreatePen(PS_SOLID,1,rgb);
   	OldPen = pDC->SelectObject((CPen*)&Pen1);    
  
   	CBrush BrushBk;
	BrushBk.CreateSolidBrush(rgb);

	CBrush* pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->SetROP2(R2_COPYPEN);
   	pDC->Rectangle(rc);  
   	pDC->SelectObject((CPen*)OldPen);
   	Pen1.DeleteObject(); 

   	pDC->SelectObject(pOldBrush);
	BrushBk.DeleteObject();
}

void CGridComboEdit::SetColNo(int nCol)
{
	u_ColNo = nCol;
	u_pColWidthArray->RemoveAll();
	for (int i = 0; i < nCol ; i++)
		u_pColWidthArray->Add(0);
	u_bFirst = TRUE;
}

void CGridComboEdit::SetWindowTextAndSel(CString StrText)
{
	SetWindowText(StrText);
	int n;
	if ((n = FindStringExact(-1, StrText)) != CB_ERR)
		SetCurSel(n);
}

void CGridComboEdit::OnDestroy()
{
	u_bDestroying = TRUE;

	CComboBox::OnDestroy();
}

HBRUSH CGridComboEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	pDC->SetTextColor(u_FGColor);
	pDC->SetBkColor(u_BGColor);

	return u_hBgColor;
}

void CGridComboEdit::SetBGColor(COLORREF Color)
{
	u_BGColor = Color;
	::DeleteObject(u_hBgColor);
	u_hBgColor = ::CreateSolidBrush(Color);
}

void CGridComboEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	CWnd* pWnd = GetFocus();
	if (pWnd != NULL)
		if (pWnd->m_hWnd == u_hwndList || pWnd == this)
			return;

	GetParent()->SendMessage(WM_EDIT_LOSE_FOCUS_MODIFIED, 0, (LPARAM)this);
}

