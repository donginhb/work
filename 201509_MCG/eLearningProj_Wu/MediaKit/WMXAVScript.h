#ifndef _WMXAVSCRIPT_H_
#define _WMXAVSCRIPT_H_

#include "ReaderBase.h"
#include "WriterBase.h"

class CWMXAVScript : public CReaderBase {
private:
	CWriterBase		m_WriterBase;
	CStringArray	*m_pCmdArray, *m_pTextArray;
	CDWordArray		*m_pTimeArray;
public:
	CWMXAVScript();
	~CWMXAVScript();
public:
	BOOL MakeAVScript(BSTR bstrWMX, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutWMX, CStringArray &CmdArray, CStringArray &TextArray, CDWordArray &TimeArray);
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