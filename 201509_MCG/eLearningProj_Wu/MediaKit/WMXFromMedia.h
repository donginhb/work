#ifndef _WMXFROMMEDIA_H_
#define _WMXFROMMEDIA_H_

#include <wmsdkidl.h>
#include <mmsystem.h>
#include "ReaderBase.h"
#include "WriterBase.h"
#include "WaveLib\HelperWav.h"
class CWMXFromMedia : public CReaderBase {
private:
	UINT			m_ulPos;
	WAVREADER		m_WavReader;
	CWriterBase		m_WriterBase;
public:
	CWMXFromMedia();
	~CWMXFromMedia();
public:
	BOOL WMXFromWav(BSTR bstrWMX, BSTR bstrProfile, BSTR bstrWav, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput);
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