#ifndef _WMX2MEDIA_H_
#define _WMX2MEDIA_H_

#include "WaveLib\HelperWav.h"
#include "mmsystem.h"
#include "ReaderBase.h"

class CWMX2Media : public CReaderBase {
private:
	WAVWRITER	m_WavWriter;

public:
	CWMX2Media();
	~CWMX2Media();
public:
	BOOL MakeWave(BSTR bstrSource, WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate, BSTR bstrOutput);
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