#ifndef _WMXNORMALIZE_H_
#define _WMXNORMALIZE_H_

class CReaderBase;
class CWriterBase;
class CWMXNormalize : public CReaderBase {
private:
	CWriterBase		*m_pWriterBase;
	
	BOOL	m_fPass_ReadPeak;
	double	m_dMultiplyRatio;
	short	m_sMin, m_sMax;
public:
	CWMXNormalize();
	~CWMXNormalize();

public:
	BOOL AudioGetPeak(WCHAR *pwchWFX, long *plPeakMin, long *plPeakMax, long *plMin, long *plMax);
	BOOL AudioMultiply(WCHAR *pwchWFX, WCHAR *pwchOutput, WCHAR *pwchProfile, long lRatio_100, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker);
	BOOL AudioNormalize(WCHAR *pwchWFX, WCHAR *pwchOutput, WCHAR *pwchProfile, long lPercentage_100, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker);
public:
	HRESULT STDMETHODCALLTYPE OnSample( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ QWORD qwSampleTime,
            /* [in] */ QWORD qwSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext) ;
	HRESULT STDMETHODCALLTYPE OnStreamSample( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ QWORD cnsSampleTime,
            /* [in] */ QWORD cnsSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext) ;
};
#endif