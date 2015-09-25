#ifndef _INFOREADER_H_
#define _INFOREADER_H_

#include "ReaderBase.h"

class CInfoReader : public CReaderBase {
public:
	BOOL	m_fFindImageNext;
	long	m_lImageFormat;
	BOOL	m_fPrevImageFound, m_fNextImageFound;
	long 	m_lImageWidth, m_lImageHeight;	
	LPSTR	m_pPrevImageFile, m_pNextImageFile;
	LPBYTE	m_pPrevBuffer, m_pNextBuffer;
	DWORD	m_cbBuffer;
	long	m_lAudioSampleCount, m_lVideoSampleCount, m_lScriptSampleCount;

	QWORD	m_qwAudioDuration;
public:
	CInfoReader();
	~CInfoReader();
public:
	BOOL GetCoveredImages(long lFormat, long lmsTime, LPSTR szImagePrev, LPSTR szImageNext);
	BOOL GetNextImage(long lFormat, long lmsTime, LPSTR szImage);
	BOOL GetPrevImage(long lFormat, long lmsTime, LPSTR szImage);
	BOOL GetAudioAlignTime(QWORD *pqwAlign);
	BOOL GetDuration(QWORD *pqwDuration);
	BOOL GetSampleCount(long *plAudio, long *plVideo, long *plScript);
private:
	BOOL ImageGetting_Init(long lFormat);
	BOOL ImageGetting_NextImage(long lmsTime, LPSTR szImage);
	BOOL ImageGetting_PrevImage(long lmsTime, LPSTR szImage);
	void ImageGetting_Exit();
	BOOL GetVideoImageSize(long &lWidth, long &lHeight);
public:
	virtual HRESULT STDMETHODCALLTYPE OnSample( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ QWORD qwSampleTime,
            /* [in] */ QWORD qwSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext);
	virtual HRESULT STDMETHODCALLTYPE OnStreamSample( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ QWORD cnsSampleTime,
            /* [in] */ QWORD cnsSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext);
};
#endif