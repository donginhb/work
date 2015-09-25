#ifndef _MULTIWMXCOPY_H_
#define _MULTIWMXCOPY_H_

#include "ReaderBase.h"
#include "MultiTimerBase.h"
class CWriterBase;
class CMultiWMXCopy : public CReaderBase, public CMultiTimerBase {
private:
	QWORD			m_qwStartTime, m_qwSegmentTime, m_qwAlignTime;
	QWORD			m_qwMaxTime, m_qwAudioMaxTime, m_qwAudioAdjustTime;
	BOOL			m_fFirstAudioSample;
	CWriterBase		*m_pWriterBase;
public:
	CMultiWMXCopy();
	~CMultiWMXCopy();
public:
	BOOL SetAlignTime(QWORD qwAlign) { m_qwAlignTime = qwAlign; return TRUE; };
	BOOL SetWriterBase(CWriterBase *pWriterBase) { m_pWriterBase = pWriterBase; return TRUE;};
	BOOL AddWMX(BSTR bstrSource, long lmsStart, long lmsStop, BOOL fVideo, BOOL fScript);
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