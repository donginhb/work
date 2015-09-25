#include "stdafx.h"
#include "BitmapMenu.h"

#define cGap 10

CBitmapMenu::CBitmapMenu()
{
	m_pFont = NULL;
	m_szText.cx = 100;
	m_szText.cy = 20;
}

CBitmapMenu::~CBitmapMenu()
{
   Detach() ;
   ASSERT(m_hMenu == NULL) ;
}

void CBitmapMenu::SetMenuFont(CFont* pFont)
{
	m_pFont = pFont;

	int nSize = GetMenuItemCount();
	CString strMenu;
	CSize szText;
	m_szText.cx = m_szText.cy = 0;
	for (int i = 0; i < nSize; i++)
	{
		GetMenuString(i, strMenu, MF_BYPOSITION);
		szText = GetDrawSize(strMenu);

		if (szText.cx > m_szText.cx)
			m_szText.cx = szText.cx;

		if (szText.cy > m_szText.cy)
			m_szText.cy = szText.cy;
	}
};

int CBitmapMenu::GetMenuIndex(UINT nID)
{
	int nSize = GetMenuItemCount();
	int nIndex = 0;
	for (int i = 0; i < nSize; i++)
	{
		UINT nMenuID = GetMenuItemID(i);
		if (nMenuID == nID)
			return nIndex;

		if (nMenuID != 0)
			nIndex++;
	}

	return -1;
}

BOOL CBitmapMenu::Attach(HMENU hMenu)
{
	BOOL bRet = CMenu::Attach(hMenu);	

	int nSize = GetMenuItemCount();
	for (int i = 0; i < nSize; i++)
		ModifyMenu(i, MF_BYPOSITION | MF_OWNERDRAW, GetMenuItemID(i));

	return bRet;
}

CSize CBitmapMenu::GetDrawSize(CString str)
{
	CClientDC dc(AfxGetMainWnd());
	CFont* pOldFont = dc.SelectObject(m_pFont);
	CSize szRet = dc.GetOutputTabbedTextExtent(str, 0, NULL);
	dc.SelectObject(pOldFont);

	return szRet;
}

void CBitmapMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	IMAGEINFO imageInfo;
	m_ImageList.GetImageInfo(0, &imageInfo);
	CSize size = CRect(imageInfo.rcImage).Size();

	if (lpMIS->itemID != 0)
		lpMIS->itemHeight = max(size.cy, m_szText.cy) + 2;
	else
		lpMIS->itemHeight = 9;

	lpMIS->itemWidth = size.cx + m_szText.cx + 2 + cGap;
}

void CBitmapMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// The remainder of this function pertains to menus.
	if (lpDIS->CtlType != ODT_MENU) return;

	// Get information about the menu item to be drawn.
	BOOL bDisabled = ((lpDIS->itemState & ODS_GRAYED) != 0);
	BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) != 0);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcDraw = lpDIS->rcItem;
	
	CRect rcBitmap;
	rcBitmap.top = (rcDraw.Height() - m_szBitmap.cy) / 2 + rcDraw.top;
	rcBitmap.bottom = rcBitmap.top + m_szBitmap.cy;
	rcBitmap.left = 2;
	rcBitmap.right = rcBitmap.left + m_szBitmap.cx;

	CRect rcText = rcDraw;
	rcText.left = rcBitmap.right + 4;
	rcText.right = rcDraw.right;

	// The item is disabled, so draw a monochrome bitmap.
	if (bDisabled)	
	{
		//DrawDisabled(pDC, lpDIS->itemID, lpDIS->rcItem);
		return;
	}

	// If the item is selected, draw a raised button.
	//if (bSelected) 
	//	DrawButton(pDC, rcBitmap);


	if (lpDIS->itemID == 0)
	{
		DrawSeparator(pDC, rcDraw);
		return;
	}

	CString strText;
	GetMenuString(lpDIS->itemID, strText, MF_BYCOMMAND);

	CFont* pOldFont = pDC->SelectObject(m_pFont);
	pDC->SetBkMode(TRANSPARENT);

	int nIndex = GetMenuIndex(lpDIS->itemID);

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		m_ImageList.Draw(pDC, nIndex, CPoint(rcBitmap.left, rcBitmap.top), ILD_TRANSPARENT);
		
		pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		pDC->DrawText(strText, rcText, DT_LEFT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE);
	}

	if (lpDIS->itemState & ODS_CHECKED)
	{
      // Menu item is checked.
	}

	CRect rcFill = rcText;
	rcFill.left = rcBitmap.right + 1;
	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
        DrawButton(pDC, rcBitmap, TRUE);

		CBrush br(RGB(0, 0, 128));
		pDC->FillRect(rcFill, &br);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->DrawText(strText, rcText, DT_VCENTER | DT_SINGLELINE);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
        DrawButton(pDC, rcBitmap, FALSE);

		CBrush br(::GetSysColor(COLOR_MENU));
		pDC->FillRect(rcFill, &br);
		pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		pDC->DrawText(strText, rcText, DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);
}

void CBitmapMenu::DrawSeparator(CDC *pDC, CRect rcSep)
{
	CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen* pOldPen = pDC->SelectObject(&penDkGray);
	int nMiddle = (rcSep.top + rcSep.bottom) / 2;
	pDC->MoveTo(0, nMiddle - 1);
	pDC->LineTo(rcSep.left, nMiddle - 1);
	pDC->LineTo(rcSep.right, nMiddle - 1);

	CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
	pDC->SelectObject(&penWhite);
	pDC->MoveTo(0, nMiddle);
	pDC->LineTo(rcSep.left, nMiddle);
	pDC->LineTo(rcSep.right, nMiddle);

	pDC->SelectObject(pOldPen);
}

void CBitmapMenu::DrawButton(CDC *pDC, CRect rcBitmap, BOOL bDraw)
{
	if (bDraw)
	{
		// Draw the lighted side of the button.
		CPen penWhite(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
		CPen* pOldPen = pDC->SelectObject(&penWhite);
		pDC->MoveTo(rcBitmap.left, rcBitmap.bottom);
		pDC->LineTo(rcBitmap.left, rcBitmap.top);
		pDC->LineTo(rcBitmap.right, rcBitmap.top);

		// Draw the darkened side of the button.
		CPen penDkGray(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
		pDC->SelectObject(&penDkGray);
		pDC->LineTo(rcBitmap.right, rcBitmap.bottom);
		pDC->LineTo(rcBitmap.left, rcBitmap.bottom);

		pDC->SelectObject(pOldPen);
	}
	else	
	{
		// Draw the lighted side of the button.
		CPen penMenu(PS_SOLID, 1, ::GetSysColor(COLOR_MENU));
		CPen* pOldPen = pDC->SelectObject(&penMenu);
		pDC->MoveTo(rcBitmap.left, rcBitmap.bottom);
		pDC->LineTo(rcBitmap.left, rcBitmap.top);
		pDC->LineTo(rcBitmap.right, rcBitmap.top);
		pDC->LineTo(rcBitmap.right, rcBitmap.bottom);
		pDC->LineTo(rcBitmap.left, rcBitmap.bottom);

		pDC->SelectObject(pOldPen);
	}

	return;
}

//
// Draw an image in a disabled state by converting a color image to a 
// monochrome image. Pixels in the color image that are part of the 
// background or have the same color as the button highlight color are
// mapped to the monochrome background. All other pixels are mapped to
// the monochrome foreground.
//

void CBitmapMenu::DrawDisabled(CImageList *imageList, CDC *pDC, 
							  int imageID, CPoint &position, 
							  CSize &size)
{
	// Create a color bitmap.
	CWindowDC windowDC(0);
	CDC colorDC;
	colorDC.CreateCompatibleDC(0);
	CBitmap colorBmp;
	colorBmp.CreateCompatibleBitmap(&windowDC, size.cx, size.cy);
	CBitmap *oldColorBmp = colorDC.SelectObject(&colorBmp);

	// Create a monochrome bitmap.
	CDC monoDC;
	monoDC.CreateCompatibleDC(0);
	CBitmap monoBmp;
	monoBmp.CreateCompatibleBitmap(&monoDC, size.cx, size.cy);
	CBitmap *oldMonoBmp = monoDC.SelectObject(&monoBmp);

	// Copy the toolbar button to the color bitmap, make all transparent 
	// areas the same color as the button highlight color.
	imageList->DrawIndirect(&colorDC, imageID, CPoint(0,0), 
							size, CPoint(0,0), ILD_NORMAL, 
							SRCCOPY, GetSysColor(COLOR_BTNHIGHLIGHT));

	// Copy the color bitmap into the monochrome bitmap. Pixels that 
	// have the button highlight color are mapped to the background.
	colorDC.SetBkColor(GetSysColor(COLOR_BTNHIGHLIGHT));
	monoDC.BitBlt(0, 0, size.cx, size.cy, &colorDC, 0, 0, SRCCOPY);

	// Draw the monochrome bitmap onto the menu.
	pDC->BitBlt(position.x, position.y, size.cx, size.cy, 
			   &monoDC, 0, 0, SRCCOPY);

	// Delete the color DC and bitmap.
	colorDC.SelectObject(oldColorBmp);
	colorDC.DeleteDC();
	colorBmp.DeleteObject();

	// Delete the monochrome DC and bitmap.
	monoDC.SelectObject(oldMonoBmp);
	monoDC.DeleteDC();
	monoBmp.DeleteObject();

	return;
}

void CBitmapMenu::CreateImage()
{
	int nSize = m_dwArrayID.GetSize();
	if (nSize > 0)
	{
		UINT nID = m_dwArrayID[0];
		CBitmap bitmap;
		bitmap.LoadBitmap(nID);
	
		BITMAP b;
		bitmap.GetBitmap(&b);
		int nBmpWidth = b.bmWidth;
		int nBmpHeight = b.bmHeight;

		m_ImageList.Create(nBmpWidth, nBmpHeight, ILC_COLORDDB | ILC_MASK, nSize, 1);

		m_szBitmap.cx = nBmpWidth;
		m_szBitmap.cy = nBmpHeight;

		for (int i = 0; i < nSize; i++)
		{
			UINT nID = m_dwArrayID[i];
			CBitmap bmp;
			bmp.LoadBitmap(nID);
			m_ImageList.Add(&bmp, RGB(255, 0, 255));
		}
	}
}

void CBitmapMenu::AddImage(UINT nID)
{
	m_dwArrayID.Add(nID);
}

