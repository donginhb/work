#ifndef _MULTIWMXAUDIO2WAV_H_
#define _MULTIWMXAUDIO2WAV_H_

#include "WaveLib\HelperWav.h"
#include "MultiTimerBase.h"
#include "ReaderBase.h"

class CMultiWMXAudio2Wav : public CReaderBase, public CMultiTimerBase {
private:
	WAVWRITER	*m_pWavWriter;
	DWORD		m_dwTimeOffset;
	QWORD		m_qwStartTime, m_qwStopTime;
public:
	CMultiWMXAudio2Wav();
	~CMultiWMXAudio2Wav();
public:
	BOOL SetWriter(WAVWRITER *pWavWriter) { m_pWavWriter = pWavWriter; return TRUE;};
	BOOL AddWMXAudio(BSTR bstrSource, long lmsStart, long lmsStop);
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