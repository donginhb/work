#include <stdio.h>
#include <wmsdkidl.h>

#include "ReaderBase.h"
#include "WriterBase.h"

#include "HelperKit.h"
#include "WMXNormalize.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

CWMXNormalize::CWMXNormalize()
{
	m_pWriterBase = NULL;

	m_fPass_ReadPeak = FALSE;
	m_dMultiplyRatio = 1.0;
	m_sMin = 0;
	m_sMax = 0;
}

CWMXNormalize::~CWMXNormalize()
{
}

BOOL CWMXNormalize::AudioGetPeak(WCHAR *pwchWMX, long *plPeakMin, long *plPeakMax, long *plMin, long *plMax)
{
	if(!Open(pwchWMX))
		return FALSE;

	m_fPass_ReadPeak = TRUE;

	*plMin = -32767;
	*plMax = 32767;

	if(!SetSelectedStreams(TRUE, FALSE, FALSE))
		return FALSE;

	if(!ConfigAudioOutputSimpleUncompressed(16, 2, 44100))
		return FALSE;

	m_sMin = 0; m_sMax = 0;

	if(!Run_AutoMode(0, 0))
		return FALSE;

	*plPeakMin = m_sMin;
	*plPeakMax = m_sMax;

	Close();

	return TRUE;
}

/*
HRESULT CWMXNormalize::AdjustProfile(IWMProfile *pProfile)
{
	if(pProfile==NULL)
		return E_FAIL;

	IWMStreamConfig *pStreamConfig=NULL;
	HRESULT hr = pProfile->GetStreamByNumber(m_wStreamVideo, &pStreamConfig);
	if(FAILED(hr)) return hr;

	IWMVideoMediaProps *pProps=NULL;
	hr = pStreamConfig->QueryInterface(IID_IWMVideoMediaProps, (void**)&pProps);
	if(SUCCEEDED(hr)) {
		hr = pProps->SetMaxKeyFrameSpacing(TIMEUNIT);
	}
	if(SUCCEEDED(hr)) {
		DWORD dwSize=0;
		hr = pProps->GetMediaType(NULL, &dwSize);
		if(SUCCEEDED(hr)) {
			hr = E_FAIL;
			WM_MEDIA_TYPE *pwmt = (WM_MEDIA_TYPE *)malloc(dwSize);
			if(pwmt!=NULL) {
				hr = pProps->GetMediaType(pwmt, &dwSize);
				if(SUCCEEDED(hr)) {
					WMVIDEOINFOHEADER *pvih=(WMVIDEOINFOHEADER *)pwmt->pbFormat;
					pvih->AvgTimePerFrame = TIMEUNIT/30;
					hr = pProps->SetMediaType(pwmt);
					if(SUCCEEDED(hr)) 
						hr = pProfile->ReconfigStream(pStreamConfig);
				}
				free(pwmt);
			}
		}
		RELEASE(pProps);
	}	

	RELEASE(pStreamConfig);	

	hr = pProfile->GetStreamByNumber(m_wStreamAudio, &pStreamConfig);
	if(FAILED(hr)) return hr;

	hr = pStreamConfig->QueryInterface(IID_IWMMediaProps, (void**)&pProps);
	if(SUCCEEDED(hr)) {
		DWORD dwSize=0;
		hr = pProps->GetMediaType(NULL, &dwSize);
		if(SUCCEEDED(hr)) {
			hr = E_FAIL;
			WM_MEDIA_TYPE *pwmt = (WM_MEDIA_TYPE *)malloc(dwSize);
			if(pwmt!=NULL) {
				hr = pProps->GetMediaType(pwmt, &dwSize);
				if(SUCCEEDED(hr)) {
					WAVEFORMATEX *pwfx=(WAVEFORMATEX *)pwmt->pbFormat;
					hr = pProps->SetMediaType(pwmt);
					if(SUCCEEDED(hr)) 
						hr = pProfile->ReconfigStream(pStreamConfig);
				}
				free(pwmt);
			}
		}
		RELEASE(pProps);
	}	

	RELEASE(pStreamConfig);	

	return hr;	
}
*/

BOOL CWMXNormalize::AudioMultiply(WCHAR *pwchWMX, WCHAR *pwchOutput, WCHAR *pwchProfile, long lRatio_100, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker)
{
	if(!Open(pwchWMX))
		return FALSE;

	HRESULT hr=0;
	
	m_fPass_ReadPeak = FALSE;

	m_dMultiplyRatio = lRatio_100/100.0;

	if(!SetSelectedStreams(TRUE, TRUE, TRUE))
		return FALSE;

	if(!ConfigAudioOutputSimpleUncompressed(16, 2, 44100))
		return FALSE;

	if(!ConfigVideoOutputCompressed())
		return FALSE;
	if(!ConfigScriptOutputCompressed())
		return FALSE;

	IWMProfileManager *pWMPM0=NULL;
	IWMProfileManager2 *pWMPM=NULL;
	hr = WMCreateProfileManager(&pWMPM0);
	if(FAILED(hr)) return FALSE;

	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if(FAILED(hr)) return FALSE;

	IWMProfile *pProfile = PF_LoadFromPRX(pWMPM, pwchProfile);
	RELEASE(pWMPM);

	if(pProfile==NULL) return FALSE;

	m_pWriterBase = new CWriterBase();
	if(m_pWriterBase==NULL) {
		RELEASE(pProfile);
		return FALSE;
	}
	
	BOOL fOK = m_pWriterBase->Create(pwchOutput, pProfile);
	RELEASE(pProfile);
	if(!fOK) return FALSE;

	if(!m_pWriterBase->ConfigAudioInputSimpleUncompressed(16, 2, 44100))
		return FALSE;

	if(!m_pWriterBase->ConfigVideoInputCodecNoNeeded())
		return FALSE;

	if(!m_pWriterBase->ConfigScriptInputCodecNoNeeded())
		return FALSE;

	if(fCopyHeader) CopyAllHeaders(m_pWriterBase->m_pHeaderInfo);
	if(fCopyScript) CopyScript(m_pWriterBase->m_pHeaderInfo);

	fOK = FALSE;
	if(m_pWriterBase->BeginWriting()) {
		fOK = Run_AutoMode(0, 0);
		m_pWriterBase->EndWriting();
	}

	m_pWriterBase->Close();
	delete m_pWriterBase;
	m_pWriterBase = NULL;

	if(fCopyMarker) CopyMarker(pwchOutput);

	Close();

	return fOK;
}

BOOL CWMXNormalize::AudioNormalize(WCHAR *pwchWMX, WCHAR *pwchOutput, WCHAR *pwchProfile, long lPercentage_100, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker)
{
	long lMinPeak, lMaxPeak, lMin, lMax;
	if(!AudioGetPeak(pwchWMX, &lMinPeak, &lMaxPeak, &lMin, &lMax))
		return FALSE;

	if(-lMin>lMax) lMax = -lMin;
	if(-lMinPeak>lMaxPeak) lMaxPeak = -lMinPeak;

	long lRatio_100 = (long)((double)lPercentage_100*(double)lMax/(double)lMaxPeak);
	if(!AudioMultiply(pwchWMX, pwchOutput, pwchProfile, lRatio_100, fCopyHeader, fCopyScript, fCopyMarker))
		return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//

HRESULT CWMXNormalize::OnSample( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ QWORD qwSampleTime,
            /* [in] */ QWORD qwSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext)
{	
	if(dwOutputNum==m_dwOutputAudio) {
		DWORD	cbBuffer=0;
		LPBYTE	pBuffer=NULL;
		HRESULT hr = pSample->GetBufferAndLength(&pBuffer, &cbBuffer);
		if(FAILED(hr)) return E_FAIL;

		if(m_fPass_ReadPeak) {			
			short sValue=0;
			for(DWORD ii=0; ii<cbBuffer; ii+=2) {
				sValue = *(short*)(pBuffer+ii);
				if(sValue<m_sMin) 
					m_sMin = sValue;
				if(sValue>m_sMax) 
					m_sMax = sValue;
			}
		}
		else {
			long lValue=0; // avoid overflow!!
			for(DWORD ii=0; ii<cbBuffer; ii+=2) {
				lValue = *(short*)(pBuffer+ii);
				lValue = (long)(lValue * m_dMultiplyRatio);
				if(lValue>32767) lValue = 32767;
				if(lValue<-32767) lValue = -32767;
				*(short*)(pBuffer+ii) = (short)lValue;
			}
		}
	}

	if(!m_fPass_ReadPeak) 
		return m_pWriterBase->m_pWriter->WriteSample(dwOutputNum, qwSampleTime, dwFlags, pSample);

	return S_OK;
}

HRESULT CWMXNormalize::OnStreamSample( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ QWORD cnsSampleTime,
            /* [in] */ QWORD cnsSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext)
{
	if(m_fPass_ReadPeak) 
		return S_OK;

	WORD	wWriterStream=0;
	if(wStreamNum==m_wStreamVideo) wWriterStream = m_pWriterBase->m_wStreamVideo;
	else if(wStreamNum==m_wStreamScript) wWriterStream = m_pWriterBase->m_wStreamScript;
	else return S_OK;

	return m_pWriterBase->m_pWriterAdvanced->WriteStreamSample(wWriterStream, cnsSampleTime, NULL, cnsSampleDuration,
				dwFlags, pSample);
}
