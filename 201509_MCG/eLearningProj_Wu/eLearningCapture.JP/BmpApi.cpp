#include "stdafx.h"
#include "bmpapi.h"   

#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)  
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

void WINAPI InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr,
                                  DWORD dwWidth,
                                  DWORD dwHeight,
                                  int nBPP)
{
   memset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));

   lpBmInfoHdr->biSize      = sizeof (BITMAPINFOHEADER);
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if (nBPP <= 1)
      nBPP = 1;
   else if (nBPP <= 4)
      nBPP = 4;
   else if (nBPP <= 8)
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}

HANDLE WINAPI BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP             Bitmap;
   BITMAPINFOHEADER   bmInfoHdr;
   LPBITMAPINFOHEADER lpbmInfoHdr;
   LPSTR              lpBits;
   HDC                hMemDC;
   HANDLE             hDIB;
   HPALETTE           hOldPal = NULL;

      // Do some setup -- make sure the Bitmap passed in is valid,
      //  get info on the bitmap (like its height, width, etc.),
      //  then setup a BITMAPINFOHEADER.

   if (!hBitmap)
      return NULL;

   if (!::GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
      return NULL;

   ::InitBitmapInfoHeader (&bmInfoHdr, 
                           Bitmap.bmWidth, 
                           Bitmap.bmHeight, 
                           Bitmap.bmPlanes * Bitmap.bmBitsPixel);


      // Now allocate memory for the DIB.  Then, set the BITMAPINFOHEADER
      //  into this memory, and find out where the bitmap bits go.

   hDIB = ::GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) + 
            ::PaletteSize ((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

   if (!hDIB)
      return NULL;

   lpbmInfoHdr  = (LPBITMAPINFOHEADER) ::GlobalLock (hDIB);
   *lpbmInfoHdr = bmInfoHdr;
   lpBits       = ::FindDIBBits ((LPSTR) lpbmInfoHdr);


      // Now, we need a DC to hold our bitmap.  If the app passed us
      //  a palette, it should be selected into the DC.

   hMemDC       = ::GetDC (NULL);

   if (hPal)
    {
      hOldPal = ::SelectPalette (hMemDC, hPal, TRUE);
      ::RealizePalette (hMemDC);
    }

      // We're finally ready to get the DIB.  Call the driver and let
      //  it party on our bitmap.  It will fill in the color table,
      //  and bitmap bits of our global memory block.

   if (!::GetDIBits (hMemDC, 
                     hBitmap, 
                     0, 
                     Bitmap.bmHeight, 
                     lpBits, 
                     (LPBITMAPINFO) lpbmInfoHdr, 
                     DIB_RGB_COLORS))
      {
        ::GlobalUnlock (hDIB);
        ::GlobalFree (hDIB);
        hDIB = NULL;
      }
   else
      ::GlobalUnlock (hDIB);


      // Finally, clean up and return.

   if (hOldPal)
      ::SelectPalette (hMemDC, hOldPal, TRUE);

   ::ReleaseDC (NULL, hMemDC);

   return hDIB;
}

/*************************************************************************
 *
 * PaintDIB()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HDIB hDIB        - handle to global memory with a DIB spec
 *                    in it followed by the DIB bits
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HANDLE    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP

   ::GlobalUnlock((HGLOBAL) hDIB);

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

BOOL WINAPI DDBPaint(HDC hDC, 
					 LPRECT lpDCRect,
					 HBITMAP hDDB,
					 LPRECT lpDDBRect,
					 HPALETTE hPal)
{
   HDC hMemDC;
   HBITMAP hOldBitmap;
   HPALETTE hOldPal1 = NULL;
   HPALETTE hOldPal2 = NULL;
   
   hMemDC = ::CreateCompatibleDC(hDC);
   
   if (!hMemDC)
   	return FALSE;
   
   if (hPal)
   {
   	hOldPal1 = ::SelectPalette(hMemDC, hPal, FALSE);
   	hOldPal2 = ::SelectPalette(hDC, hPal, FALSE);
   }	                                      
   
   hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hDDB);
   
   ::SetStretchBltMode(hDC, COLORONCOLOR);

   if ((RECTWIDTH(lpDCRect) == RECTWIDTH(lpDDBRect)) &&
       (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect)))
      BitBlt(hDC,
      		 lpDCRect->left,
      		 lpDCRect->top,
      		 lpDCRect->right - lpDCRect->left,
      		 lpDCRect->bottom - lpDCRect->top,
      		 hMemDC,
      		 lpDDBRect->left,
      		 lpDDBRect->top,
      		 SRCCOPY);
   else
      StretchBlt(hDC,
      		 	 lpDCRect->left,
      		 	 lpDCRect->top,
      		  	 lpDCRect->right - lpDCRect->left,
      		 	 lpDCRect->bottom - lpDCRect->top,
      		 	 hMemDC,
      		 	 lpDDBRect->left,
      		 	 lpDDBRect->top,
    		 	 lpDDBRect->right - lpDDBRect->left,
      		 	 lpDDBRect->bottom - lpDDBRect->top,
      		 	 SRCCOPY);
   
   ::SelectObject(hMemDC, hOldBitmap);
   
   if (hOldPal1)
   	::SelectPalette(hMemDC, hOldPal1, FALSE);
   
   if (hOldPal2)
   	::SelectPalette(hDC, hOldPal2, FALSE);
			  
   ::DeleteDC(hMemDC); 
   return TRUE;   
}

CBitmap* WINAPI GetBitmap(LPSTR pStr)
{
	CBitmap* pBMP = new CBitmap;
    LPBITMAPINFOHEADER lpDIBHdr = (LPBITMAPINFOHEADER)pStr;
    LPSTR lpDIBBits = ::FindDIBBits((LPSTR)lpDIBHdr);
    HDC hDC;
    if (lpDIBHdr->biBitCount != 1)
    	hDC = ::GetDC(NULL);
    else
    	hDC = ::CreateCompatibleDC(NULL);
    ::RealizePalette(hDC);
    HBITMAP hBMP;//, hBMP1;
    hBMP = ::CreateDIBitmap(hDC, 
    						lpDIBHdr, 
    						CBM_INIT, 
    						lpDIBBits,
    	 					(LPBITMAPINFO)lpDIBHdr, 
    	 					DIB_RGB_COLORS); 

    if (lpDIBHdr->biBitCount == 1)
    	::DeleteDC(hDC);
    else
    	::ReleaseDC(NULL, hDC);
    pBMP->m_hObject = hBMP;		
    return pBMP;
}

 
CBitmap* WINAPI CreateFillingBMP(COLORREF rgbFill, UINT Width, UINT Height)
{
    CBitmap* pBMP = new CBitmap;
    pBMP->CreateBitmap(Width, Height, 1, 8, NULL);
    
    CDC dc;
    dc.CreateCompatibleDC(NULL); 
    CBitmap* pOldBMP = dc.SelectObject((CBitmap*)pBMP);  
    
	CBrush brush1;
    brush1.CreateSolidBrush(rgbFill);
	CBrush* pOldBrush = dc.SelectObject(&brush1);
    
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen1);

	dc.Rectangle(CRect(0, 0, Width - 1, Height - 1));
    
	dc.SelectObject(pOldPen);
	pen1.DeleteObject();

	dc.SelectObject(pOldBrush);
	brush1.DeleteObject();

	dc.SelectObject(pOldBMP); 
    dc.DeleteDC();
    return pBMP;
}
 
CBitmap* WINAPI CreateRectBMP(COLORREF rgbFill, UINT Width, UINT Height)
{
    CBitmap* pBMP = new CBitmap;
    pBMP->CreateBitmap(Width, Height, 1, 1, NULL);
    
    CDC dc;
    dc.CreateCompatibleDC(NULL); 
    CBitmap* pOldBMP = dc.SelectObject((CBitmap*)pBMP);  
    
	CBrush* pBrush = new CBrush;
	pBrush->CreateSolidBrush(rgbFill);
	dc.FillRect(CRect(0, 0, Width, Height), pBrush);
	pBrush->DeleteObject();
	delete pBrush;
		
	dc.SelectObject(pOldBMP); 
    dc.DeleteDC();
    return pBMP;
}

void WINAPI DrawCBitmap(CDC* pDC, CBitmap* pBMP, CPoint point)
{
   	HBITMAP hBMP = (HBITMAP)pBMP->m_hObject; 

   	BITMAP BMPBuffer;
   	pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
   	int bmWidth = BMPBuffer.bmWidth;
   	int bmHeight = BMPBuffer.bmHeight;

   	RECT RectTemp = {0, 0, bmWidth, bmHeight};

   	RECT RectDraw = {point.x , point.y, point.x + bmWidth, point.y + bmHeight};
	::DDBPaint(pDC->m_hDC, &RectDraw, hBMP, &RectTemp, NULL);  
} 


/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HANDLE hDIB, CPalette* pPal)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return FALSE;

   lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion = 0x0300;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = pPal->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bResult;
}

#define	HIMETRIC_INCH	2540

CBitmap* WINAPI LoadFile(CString strFile)
{
	CFile			cFile;
	CFileException	e;
	BOOL bResult = FALSE;
	IPicture* pPicture;
	if (cFile.Open(strFile, CFile::modeRead | CFile::typeBinary, &e))
	{
		BYTE* pBuff = new BYTE[cFile.GetLength()];

		if (cFile.Read(pBuff, cFile.GetLength()) > 0)
		{
			if (LoadFromBuffer(pBuff, cFile.GetLength(), &pPicture))
				bResult = TRUE;
		}

		delete [] pBuff;
	}
	if (!bResult)
		return NULL;

	long hmWidth;
	long hmHeight;
	pPicture->get_Width(&hmWidth);
	pPicture->get_Height(&hmHeight);

	HDC hDC = ::GetDC(NULL);
	int cx	= MulDiv(hmWidth,	::GetDeviceCaps(hDC, LOGPIXELSX), HIMETRIC_INCH);
	int cy	= MulDiv(hmHeight,	::GetDeviceCaps(hDC, LOGPIXELSY), HIMETRIC_INCH);
	::ReleaseDC(NULL, hDC);

    CDC dc;
    dc.CreateCompatibleDC(NULL); 
	
	int nPixel = dc.GetDeviceCaps(BITSPIXEL);
     CBitmap* pBMP = new CBitmap;
    pBMP->CreateBitmap(cx, cy, 1, nPixel, NULL);

	CBitmap* pOldBMP = dc.SelectObject((CBitmap*)pBMP);  
	CBitmap* pRet = NULL;
	if (pPicture->Render(dc.m_hDC, 0, 0, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
		pRet = pBMP;

	dc.SelectObject(pOldBMP); 
    dc.DeleteDC();

	if (pPicture != NULL)
	{
		pPicture->Release();
		pPicture = NULL;
	}

	return pBMP;
}

BOOL WINAPI LoadFromBuffer(BYTE* pBuff, int nSize, IPicture** pPicture)
{
	BOOL bResult = FALSE;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pBuff, nSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr;
		if ((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)pPicture)) == S_OK)
			bResult = TRUE;
			
		pStream->Release();
	}

	return bResult;
}
