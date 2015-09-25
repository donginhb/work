#ifndef _MultiWMXMixer2Wav_H_
#define _MultiWMXMixer2Wav_H_

#include "WaveLib\HelperWav.h"
#include "MultiTimerBase.h"
#include "ReaderBase.h"

class CMultiWMXMixer2Wav : public CReaderBase, public CMultiTimerBase {
private:
	WAVWRITER	*m_pWavWriter;
	WAVREADER	*m_pWavReader;
	LPBYTE		m_pMixerBuffer;
	UINT		m_uPosWork;
public:
	CMultiWMXMixer2Wav();
	~CMultiWMXMixer2Wav();
public:
	BOOL SetWriter(WAVWRITER *pWavWriter);
	BOOL MixerWMXAudio(BSTR bstrSource, long lmsStart, long lmsStop, WAVREADER *pWavReader, UINT &uPosWave);
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