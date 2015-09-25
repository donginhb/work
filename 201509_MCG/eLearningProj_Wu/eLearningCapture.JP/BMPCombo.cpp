// BMPCombo.cpp : implementation file
//

#include "stdafx.h"
#include "elearningcapture.h"
#include "BMPCombo.h"
#include "..\elearning.jp\bmpapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMPCombo

CBMPCombo::CBMPCombo()
{
}

CBMPCombo::~CBMPCombo()
{
}


BEGIN_MESSAGE_MAP(CBMPCombo, CComboBox)
	//{{AFX_MSG_MAP(CBMPCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*
int CBMPCombo::GetIndexFromID(CString strID)
{
	for (int i = 0; i < m_pBMPIDArray->GetSize(); i++)
	{
		if (m_pBMPIDArray->GetAt(i) == strID)
			return i;
	}

	return -1;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CBMPCombo message handlers
void CBMPCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
    if (lpMIS->itemData == NULL)
    	return;

    //int BMPId = atoi((LPCTSTR)lpMIS->itemData) - 1;
	int nIndex = lpMIS->itemID;
	CBitmap* pBMP;
	if (nIndex >= 0)
	{
		pBMP = (CBitmap*)m_pBMPArray->GetAt(nIndex);
	
		BITMAP BMPBuffer;
		pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
		lpMIS->itemHeight = BMPBuffer.bmHeight + 2; 
	}
}

void CBMPCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	//COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data
    if ((int)(lpDIS->itemID) < 0)
		return;

	//CString str;
    //GetLBText(lpDIS->itemID, str);
    //int BMPId = atoi(szBMPId);
	int nIndex = lpDIS->itemID;
    if (nIndex < 0)
		return;

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		//CBrush br(cr);
        CBitmap* pBMP;
        ::FillRect(pDC->m_hDC, &lpDIS->rcItem, (HBRUSH)::GetStockObject(WHITE_BRUSH));
        
		pBMP = (CBitmap*)m_pBMPArray->GetAt(nIndex);
		BITMAP BMPBuffer;
   		pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
        int rcWidth = lpDIS->rcItem.right - lpDIS->rcItem.left + 1;
        int rcHeight = lpDIS->rcItem.bottom - lpDIS->rcItem.top + 1; 
   		
   		int bmWidth = BMPBuffer.bmWidth;
   		int bmHeight = BMPBuffer.bmHeight;
        RECT RectBMP = {0, 0, bmWidth, bmHeight};              
     
        int Indent;
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
