#ifndef _MULTIWMXCOPYWAV_H_
#define _MULTIWMXCOPYWAV_H_

#include <wmsdkidl.h>
#include <mmsystem.h>
#include "MultiTimerBase.h"
#include "ReaderBase.h"
#include "WriterBase.h"

#include "WaveLib\HelperWav.h"
interface IEasyManager;
class CMultiWMXCopyWav : public CReaderBase, public CMultiTimerBase {
private:
	UINT			m_uPosWave;
	QWORD			m_qwSegmentTime, m_qwStartTime, m_qwStopTime;
	QWORD			m_qwAudioDuration;

	WAVREADER		*m_pWavReader;
	CWriterBase		*m_pWriterBase;
public:
	CMultiWMXCopyWav();
	~CMultiWMXCopyWav();

	BOOL SetWriterBase(CWriterBase *pWriterBase) { m_pWriterBase = pWriterBase; return TRUE; };
public:
	BOOL AddWMXFile(BSTR bstrWMX, DWORD dwmsStart, DWORD dwmsStop, WAVREADER *pWavReader, UINT &uPosWave);
	BOOL AppendWavReader(BOOL fSpaceVideo, UINT &uPosWave, UINT uSize, WAVREADER *pWavReader);	
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