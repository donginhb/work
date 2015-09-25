// WMXExport.cpp : Implementation of CWMXExport
#include "stdafx.h"
#include "MBRPrx.h"

/////////////////////////////////////////////////////////////////////////////
// CWMXExport
#include <wmsdkidl.h>

class CDurationReaderCallback : public IWMReaderCallback, public IWMReaderCallbackAdvanced {
public:
    HANDLE                  m_hEventAsync;
    HRESULT                 *m_phvalAsync;
public:
	CDurationReaderCallback() { m_hEventAsync = NULL; m_phvalAsync = NULL; }
	~CDurationReaderCallback() {}
public: // Implement Interface Methods
	HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if ( riid == IID_IWMReaderCallback ) {
			*ppvObject = ( IWMReaderCallback * )this;
		}
		else if ( riid == IID_IWMReaderCallbackAdvanced ) {
			*ppvObject = ( IWMReaderCallbackAdvanced * )this;
		}
		else {
			return E_NOINTERFACE;
		}

		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef( void ) { return 1; }
	ULONG STDMETHODCALLTYPE Release( void ) { return 1; }

	virtual HRESULT STDMETHODCALLTYPE OnSample( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ QWORD qwSampleTime,
            /* [in] */ QWORD qwSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnStatus( 
            /* [in] */ WMT_STATUS Status,
            /* [in] */ HRESULT hr,
            /* [in] */ WMT_ATTR_DATATYPE dwType,
            /* [in] */ BYTE __RPC_FAR *pValue,
			/* [in] */ void __RPC_FAR *pvContext)
	{
		switch ( Status ) {
		case WMT_OPENED:
			*m_phvalAsync = hr;
			SetEvent( m_hEventAsync );
			break;
		case WMT_STARTED:
			*m_phvalAsync = hr;
			SetEvent( m_hEventAsync );
			break;
		case WMT_EOF:
			*m_phvalAsync = hr;
			SetEvent( m_hEventAsync );
			break;
		}

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnStreamSample( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ QWORD cnsSampleTime,
            /* [in] */ QWORD cnsSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnTime( 
            /* [in] */ QWORD qwCurrentTime,
            /* [in] */ void __RPC_FAR *pvContext)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnStreamSelection( 
            /* [in] */ WORD wStreamCount,
            /* [in] */ WORD __RPC_FAR *pStreamNumbers,
            /* [in] */ WMT_STREAM_SELECTION __RPC_FAR *pSelections,
            /* [in] */ void __RPC_FAR *pvContext)
	{
		return S_OK;
    }

	virtual HRESULT STDMETHODCALLTYPE OnOutputPropsChanged( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ WM_MEDIA_TYPE __RPC_FAR *pMediaType,
            /* [in] */ void __RPC_FAR *pvContext )
    {
        return S_OK;
    }

	virtual HRESULT STDMETHODCALLTYPE AllocateForOutput( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ DWORD cbBuffer,
            /* [out] */ INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [in] */ void __RPC_FAR *pvContext)
    {
        return E_NOTIMPL;
    }

	virtual HRESULT STDMETHODCALLTYPE AllocateForStream( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ DWORD cbBuffer,
            /* [out] */ INSSBuffer __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [in] */ void __RPC_FAR *pvContext)
    {
        return E_NOTIMPL;
    }
};

#define WM_DURATION_ASYNC_EVENT	 _T( "FD8770AC-7EB7-47b5-B239-2CDB3FC5D364" )
double g_WMXGetFileDuration(WCHAR *bstrFile)
{	
	HRESULT hr=0;
	HRESULT hvalAsync = 0;
    HANDLE hEventAsync = CreateEvent( NULL, FALSE, FALSE, WM_DURATION_ASYNC_EVENT );
	if(NULL == hEventAsync) return FALSE;
	
	CDurationReaderCallback	callback;
	
	callback.m_hEventAsync = hEventAsync;
	callback.m_phvalAsync = &hvalAsync;
	
	double		dDuration=0.0;
	IWMReader	*pReader = NULL ;	
	
	hr = WMCreateReader( NULL, 0, &pReader ) ;
	if(SUCCEEDED(hr)) {
		hr = pReader->Open( bstrFile, &callback, NULL );
		if(SUCCEEDED(hr)) {
			WaitForSingleObject( hEventAsync, INFINITE );
			if ( SUCCEEDED( hvalAsync ) ) {
				IWMHeaderInfo*   pHeaderInfo=NULL;
				hr = pReader->QueryInterface(IID_IWMHeaderInfo, (VOID **)&pHeaderInfo);
				if(SUCCEEDED(hr)) {
					WORD wStreamNum = 0 ;
					WMT_ATTR_DATATYPE type ;
					LONGLONG llTime=0;
					WORD cbLength = sizeof(llTime) ;
					hr = pHeaderInfo->GetAttributeByName(&wStreamNum,  g_wszWMDuration, &type, (BYTE *)&llTime, &cbLength ) ;
					if(SUCCEEDED(hr)) dDuration = (double)llTime;
					
					pHeaderInfo->Release();
					pHeaderInfo = NULL;
				}
			}
			pReader->Close();
		}
	}
	
	pReader->Release();
	pReader = NULL;
	
	CloseHandle(hEventAsync);	
	
	return dDuration;
}

///////////////////////////////////////////////////////////////////////////////
//

#include <atlbase.h>
#include <comdef.h>
#include <wmencode.h>
#include <wmencvu.h>
HRESULT AddAudience(IWMEncProfile2 *pPro2, long lAkbps, long lVkbps, long lwV, long lhV)
{
	HRESULT hr;
	IWMEncAudienceObj* pAudnc=NULL;

	hr = pPro2->AddAudience((lAkbps + lVkbps + 9) * 1000, &pAudnc); //overhead <=9
	if(FAILED(hr)) return hr;
	
	hr = pAudnc->put_AudioCodec(0, 0);
	if(FAILED(hr)) return hr;
	
	long lChannel=2, lSampleRate=44100;
	if(lAkbps>=256) {
		lAkbps = 256;
		lSampleRate = 44100;
	}
	else if(lAkbps>=192) {
		lAkbps = 192;
		lSampleRate = 44100;
	}
	else if(lAkbps>=128) {
		lAkbps = 128;
		lSampleRate = 44100;
	}
	else if(lAkbps>=96) {
		lAkbps = 96;
		lSampleRate = 44100;
	}
	else if(lAkbps>=64) {
		lAkbps = 64;
		lSampleRate = 44100;
	}
	else if(lAkbps>=48) {
		lAkbps = 48;
		lSampleRate = 44100;
	}
	else if(lAkbps>=32) {
		lAkbps = 32;
		lSampleRate = 32000;
	}
	else if(lAkbps>=22) {
		lAkbps = 22;
		lSampleRate = 22050;
	}
	else if(lAkbps>=16) {
		lAkbps = 16;
		lSampleRate = 16000;
	}
	else {
		lAkbps = 8;
		lSampleRate = 8000;
		lChannel = 1; // mono
	}
		
	hr = pAudnc->SetAudioConfig(0, (short)lChannel, lSampleRate, lAkbps * 1000, 16);
	if(FAILED(hr)) return hr;		

	hr = pAudnc->put_VideoCodec(0, 0);
	if(FAILED(hr)) return hr;
	
	hr = pAudnc->put_VideoHeight(0, lhV);
	if(FAILED(hr)) return hr;
	
	hr = pAudnc->put_VideoWidth(0, lwV);
	if(FAILED(hr)) return hr;

	hr = pAudnc->put_VideoBitrate(0, lVkbps * 1000);
	if(FAILED(hr)) return hr;
	
	hr = pAudnc->put_VideoBufferSize(0, 5000);
	if(FAILED(hr)) return hr;	
	
	if(pAudnc) {
		pAudnc->Release();
		pAudnc = NULL;
	}

	return S_OK;
}

HRESULT GenMBRPrx(long lA1kbps, long lV1kbps, long lwV1, long lhV1, long lA2kbps, long lV2kbps, long lwV2, long lhV2, long lA3kbps, long lV3kbps, long lwV3, long lhV3, BSTR bstrPrxOut)
{
	HRESULT hr=S_OK;
	IWMEncProfile2* pPro2=NULL;

	hr = CoCreateInstance(CLSID_WMEncProfile2, NULL, CLSCTX_INPROC_SERVER, IID_IWMEncProfile2, (void**)&pPro2);
	if(FAILED(hr)) return hr;

	hr = pPro2->put_ValidateMode(VARIANT_TRUE);
	if(FAILED(hr)) return hr;

	hr = pPro2->put_ProfileName(CComBSTR("MCG MBR Profile"));
	if(FAILED(hr)) return hr;

	hr = pPro2->put_ProfileDescription(CComBSTR("Video profile with multiple audiences"));
	if(FAILED(hr)) return hr;

	hr = pPro2->put_ContentType(17); // audio + video
	if(FAILED(hr)) return hr;

	hr = pPro2->put_VBRMode(WMENC_VIDEO, 0, WMENC_PVM_NONE); // CBR setting
	if(FAILED(hr)) return hr;

	if((lA1kbps!=0)&&(lV1kbps!=0)) {
		hr = AddAudience(pPro2, lA1kbps, lV1kbps, lwV1, lhV1);
		if(FAILED(hr)) return hr;
	}

	if((lA2kbps!=0)&&(lV2kbps!=0)) {
		hr = AddAudience(pPro2, lA2kbps, lV2kbps, lwV2, lhV2);
		if(FAILED(hr)) return hr;
	}

	if((lA3kbps!=0)&&(lV3kbps!=0)) {
		hr = AddAudience(pPro2, lA3kbps, lV3kbps, lwV3, lhV3);
		if(FAILED(hr)) return hr;
	}

	// Validate the settings to make sure the profile has no errors.
	hr = pPro2->Validate();
	if(FAILED(hr)) return hr;

	// Save the profile to a .prx file.
	hr = pPro2->SaveToFile(bstrPrxOut);
	if(FAILED(hr)) return hr;

	if ( pPro2 )
	{
		pPro2->Release();
		pPro2 = NULL;
	}

	return S_OK;
}