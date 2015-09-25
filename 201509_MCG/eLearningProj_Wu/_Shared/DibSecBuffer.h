#ifndef _DIBSECBUFFER_H_
#define _DIBSECBUFFER_H_

#include <wtypes.h>
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

class CDibSectionBuffer {
private:
	HDC					m_hdcMem;
	HBITMAP				m_hbmp, m_hbmpold;
	LPBYTE				m_pBuffer;
	DWORD				*m_RowIndices;
	BITMAPINFOHEADER	m_bmiHeader;	
public:
	CDibSectionBuffer();
	~CDibSectionBuffer();
public:
	BOOL Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount);
	void Close();
	void Clear();	
	HDC GetHdc() { return m_hdcMem; };
	DWORD *GetRowIndices() { return m_RowIndices; };
	LPBITMAPINFOHEADER GetBitmapHeader() { return &m_bmiHeader; };
	LPBYTE GetBuffer() { return m_pBuffer; };
	DWORD GetBufferSize() { return m_bmiHeader.biSizeImage; };	
};

#endif