#ifndef _WMXSCRIPTDUMP_H_
#define _WMXSCRIPTDUMP_H_

#include "ReaderBase.h"

class CWMXScriptDump : public CReaderBase {
private:
	CStringArray	*m_pCmdArray, *m_pTextArray;
	CDWordArray		*m_pTimeArray;
public:
	CWMXScriptDump();
	~CWMXScriptDump();
public:
	BOOL Script2Array(BSTR bstrWMX, CStringArray &CmdArray, CStringArray &TextArray, CDWordArray &TimeArray);
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