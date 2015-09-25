#ifndef _CWMXAudioReplaceWav_H_
#define _CWMXAudioReplaceWav_H_

#include "ReaderBase.h"
#include "WriterBase.h"
//#include "MyCritSec.h"
interface IEasyManager;
class CWMXAudioReplaceWav : public CReaderBase
{
private:
	IEasyManager	*m_pManager;
	UINT		m_uPosWave;
	WAVREADER	m_WavReader;
	CWriterBase m_WriterBase;
public:
	CWMXAudioReplaceWav();
	~CWMXAudioReplaceWav();

	virtual BOOL AutoDeliverMore(QWORD qwDuration);
	BOOL MakeReplace(BSTR bstrFile, IEasyManager *pManager, BSTR bstrReplace, BSTR bstrProfile, BSTR bstrOutput);
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