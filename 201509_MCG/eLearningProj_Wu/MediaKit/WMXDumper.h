#ifndef _WMXDUMPER_H_
#define _WMXDUMPER_H_

#include "ReaderBase.h"
#include "WriterBase.h"
interface IEasyManager;
class CWMXDumper : public CReaderBase
{
private:
	BOOL			m_bInitialScript;
	BSTR			m_bstrInitialType, m_bstrInitialData;
	QWORD			m_qwLastAudioSampleTime;
	QWORD			m_qwOffset;
	CWriterBase		*m_pWriterBase;
	QWORD			m_qwDumpedAudioDuration;
	IEasyManager	*m_pAudioManager, *m_pVideoManager;
public:
	CWMXDumper();
	~CWMXDumper();

	BOOL Run_AutoMode(QWORD qwStart, QWORD qwDuration);
	BOOL Run_ManualMode(QWORD qwStart);
	BOOL AddInitialScript();
	void SetManager(IEasyManager *pAudioManager, IEasyManager *pVideoManager) {
		m_pAudioManager = pAudioManager; 
		m_pVideoManager = pVideoManager;
	};
	QWORD GetAudioDefaultDuration() { return m_qwDumpedAudioDuration; }
	QWORD GetLastAudioSampleTime() { return m_qwLastAudioSampleTime; }
	void SetInitialScript(BOOL bSet, BSTR bstrType, BSTR bstrData);
	void SetOffset(QWORD qwOffset) { m_qwOffset = qwOffset; }
	void SetWriterBase(CWriterBase *pWriterBase) {
		m_pWriterBase = pWriterBase;
	}
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