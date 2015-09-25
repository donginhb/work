#ifndef _MULTIWMXWAV2AUDIO_H_
#define _MULTIWMXWAV2AUDIF_H_

#include <wmsdkidl.h>
#include <mmsystem.h>
#include "MultiTimerBase.h"
#include "ReaderBase.h"
#include "WriterBase.h"
#include "MyCritSec.h"
#include "WaveLib\HelperWav.h"
interface IEasyManager;
class CMultiWMXWav2Audio : public CReaderBase, public CMultiTimerBase {
private:
	CMyCritSec		m_csSample;
	UINT			m_uPosWave;
	QWORD			m_qwSegmentTime, m_qwStartTime, m_qwStopTime;
	QWORD			m_qwAudioDuration;

	WAVREADER		*m_pWavReader;
	CWriterBase		*m_pWriterBase;
	IEasyManager	*m_pManager;
private:
	BITMAPINFOHEADER	m_bihWriter, m_bihReader;
/*
private:
	HDC				m_hMemDC;
	HBITMAP			m_hbmp, m_holdbmp;
	LPBYTE			m_pdibbuffer;
*/
public:
	CMultiWMXWav2Audio();
	~CMultiWMXWav2Audio();

	BOOL SetWriterBase(CWriterBase *pWriterBase) { m_pWriterBase = pWriterBase; return TRUE; };
	BOOL SetEasyManager(IEasyManager *pManager) { m_pManager = pManager; return TRUE; };
public:
	BOOL AddWMXFile(BSTR bstrWMX, DWORD dwmsStart, DWORD dwmsStop, WAVREADER *pWavReader, UINT &uPosWave);
	BOOL AppendWavReader(BOOL fSpaceVideo, UINT &uPosWave, UINT uSize, WAVREADER *pWavReader);
	BOOL AppendSpaceVideo(QWORD	qwTime);
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