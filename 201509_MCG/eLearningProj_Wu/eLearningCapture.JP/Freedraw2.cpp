#include "afxwin.h"
#include "Freedraw.h"

CFreedraw::CFreedraw()
{
	m_crColor = (COLORREF)0;
	m_lPenWidth = 0;
	m_fOpened = FALSE;
	m_fDrawed = FALSE;
	m_lCurrID = -1;
}

CFreedraw::~CFreedraw()
{
	Close();
}

BOOL CFreedraw::Open(LPSTR szDataFile)
{
	HDC hdc = GetDC(NULL);
	if(hdc==NULL) return FALSE;
	long lWidth = GetDeviceCaps(hdc, HORZRES);
	long lHeight = GetDeviceCaps(hdc, VERTRES);

	m_fOpened = m_cf.Open(szDataFile, CFile::modeCreate|CFile::modeWrite, NULL);
	if(m_fOpened) {
		long	lVer=100;
		m_cf.Write(&lVer, 4);
		m_cf.Write(&lWidth, 4);
		m_cf.Write(&lHeight, 4);
	}

	return m_fOpened;
}

void CFreedraw::Close()
{		
	StopDraw();
	if(m_fOpened) {
		m_cf.Close();
		m_fOpened = FALSE;
	}
}

BOOL CFreedraw::StartDraw(int nID, COLORREF crColor, int nPenWidth)
{
	StopDraw();

	m_lCurrID = nID;
	m_crColor = crColor;
	m_lPenWidth = nPenWidth;

	m_fDrawed = TRUE;

	return TRUE;
}

BOOL CFreedraw::AddPoint(int nTime, WORD wX, WORD wY)
{
	m_TimeArray.Add(nTime);
	m_PointArray.Add(MAKELONG(wX, wY));
	
	return TRUE;
}

void CFreedraw::StopDraw()
{
	if(m_fOpened&&m_fDrawed&&(m_PointArray.GetSize()>0)) {
		long lPointCount=m_TimeArray.GetSize();
		m_cf.Write(&m_lCurrID, 4);
		m_cf.Write(&m_crColor, 4);
		m_cf.Write(&m_lPenWidth, 4);
		m_cf.Write(&lPointCount, 4); // point count!!

		for(int ii=0; ii<lPointCount; ii++) {
			DWORD dwTime = m_TimeArray.GetAt(ii);
			DWORD dwPoint = m_PointArray.GetAt(ii);
			WORD  wX=LOWORD(dwPoint), wY=HIWORD(dwPoint);

			m_cf.Write(&dwTime, 4);			
			m_cf.Write(&wX, 2);
			m_cf.Write(&wY, 2);
		}
		m_cf.Flush();
	}

	m_lCurrID = -1;
	m_crColor = 0;
	m_lPenWidth = 0;
	m_TimeArray.RemoveAll();
	m_PointArray.RemoveAll();
	m_fDrawed = FALSE;
}