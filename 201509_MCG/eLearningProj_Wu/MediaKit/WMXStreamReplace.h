#ifndef _WMXSTREAMREPLACE_H_
#define _WMXSTREAMREPLACE_H_

class CWMXStreamReplace {
private:
	HWND	m_hWndProgress, m_hWndAbort;
	long	*m_pbAbort;
	long	m_lMsgProgress;
public:
	CWMXStreamReplace();
	~CWMXStreamReplace();

	BOOL MakeReplace(BSTR bstrWMXSource, BSTR bstrWMXReplace, BOOL fAudio, BOOL fVideo, BOOL fScript, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput);
public:
	BOOL SetupProgressHandler(HWND hwnd, long lMsg) { 
		m_hWndProgress = hwnd; 
		m_lMsgProgress = lMsg;
		return TRUE; 
	};	
	BOOL SetupQueryAbort(HWND hwnd, long *pbAbort) { 
		m_hWndAbort = hwnd; 
		m_pbAbort = pbAbort; 
		return TRUE; 
	};	
};
#endif