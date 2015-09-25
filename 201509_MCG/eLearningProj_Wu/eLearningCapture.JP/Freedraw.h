#ifndef _FREEDRAW_H_
#define _FREEDRAW_H_

class CFreedraw {
public:
	CFile		m_cf;
	BOOL		m_fOpened, m_fDrawed;
	COLORREF	m_crColor;
	long		m_lCurrID;
	long		m_lPenWidth;
	CDWordArray	m_TimeArray;
	CDWordArray	m_PointArray;
public:
	CFreedraw();
	~CFreedraw();
public:
	BOOL Open(LPSTR szDataFile);
	BOOL Open(LPSTR szDataFile, CRect sliderect);
	void Close();

	BOOL IsDrawed() { return m_fDrawed; }
	BOOL StartDraw(int nID, COLORREF crColor, int nPenWidth);
	BOOL AddPoint(int nTime, WORD wX, WORD wY);
	void StopDraw();
};

#endif