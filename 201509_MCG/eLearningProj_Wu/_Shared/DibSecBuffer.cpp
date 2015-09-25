#include <afxwin.h>
#include "DibSecBuffer.h"

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))

CDibSectionBuffer::CDibSectionBuffer()
{	
	m_hdcMem = NULL;
	m_hbmp = NULL;
	m_hbmpold = NULL;
	m_pBuffer = NULL;
	m_RowIndices = NULL;
	memset(&m_bmiHeader, 0, sizeof(m_bmiHeader));
}

CDibSectionBuffer::~CDibSectionBuffer()
{
	Close();
}

BOOL CDibSectionBuffer::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
	Close();

	HDC hdc = ::GetDC(NULL);
	if(hdc==NULL) return FALSE;

	BOOL fOK=FALSE;
	if(16<=GetDeviceCaps(hdc, BITSPIXEL)) {		
		m_hdcMem = CreateCompatibleDC(hdc);
		fOK = (m_hdcMem!=NULL);
	}	
	::ReleaseDC(NULL, hdc);
	if(!fOK) return FALSE;

	memset(&m_bmiHeader, 0, sizeof(m_bmiHeader));
	m_bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmiHeader.biWidth = dwWidth;
	m_bmiHeader.biHeight = dwHeight;	
	m_bmiHeader.biBitCount = wBitCount;
	m_bmiHeader.biPlanes = 1;
	m_bmiHeader.biSizeImage = DIBSIZE(m_bmiHeader);

	m_hbmp = CreateDIBSection(m_hdcMem, (LPBITMAPINFO)&m_bmiHeader, DIB_RGB_COLORS, (void**)&m_pBuffer, NULL, NULL);
	if(m_hbmp==NULL) return FALSE;

	m_hbmpold = (HBITMAP)SelectObject(m_hdcMem, m_hbmp);

	m_RowIndices = (DWORD*)malloc(dwHeight*sizeof(DWORD));
	for(DWORD ii=0; ii<dwHeight; ii++)
		m_RowIndices[ii] = ii * DIBWIDTHBYTES(m_bmiHeader);

	return TRUE;
}

void CDibSectionBuffer::Clear()
{
	if(m_pBuffer!=NULL)
		memset(m_pBuffer, 0, m_bmiHeader.biSizeImage);
}

void CDibSectionBuffer::Close()
{
	if(m_RowIndices!=NULL) {
		free(m_RowIndices);
		m_RowIndices = NULL;
	}

	if(m_hdcMem!=NULL) 
		SelectObject(m_hdcMem, m_hbmpold);

	m_hbmpold = NULL;

	if(m_hbmp!=NULL) {
		DeleteObject(m_hbmp);
		m_hbmp = NULL;
	}

	if(m_hdcMem!=NULL) {
		DeleteDC(m_hdcMem);
		m_hdcMem = NULL;
	}	
}