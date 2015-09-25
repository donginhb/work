#include <stdio.h>
#include <wmsdkidl.h>
#include "ReaderBase.h"

#include "HelperKit.h"
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#define TIMEPERIOD	500

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))

CReaderBase::CReaderBase()
{
	m_hWndProgress = NULL;
	m_hWndAbort = NULL;
	m_pbAbort = NULL;
	m_lMsgProgress = 0;
	m_fAutoProgressEnabled = TRUE;
	m_qwStartTime = 0;

	ResetVariables();
}

CReaderBase::~CReaderBase()
{
	Close();
}

void CReaderBase::ResetVariables()
{
	m_fManualDeliver = FALSE;
	m_fEOF = FALSE;
	m_qwUserTime = 0;
	m_fUserCheckStop = FALSE;
	m_qwUserForceStopTime = 0;

	m_qwStartTime = 0;

	m_hEventAsync = NULL;
	m_hTimeAsync = NULL;
	m_hvalAsync = 0;

	m_fVideoExisted = FALSE;
	m_fScriptExisted = FALSE;
	m_fAudioCompressed = FALSE;
	m_fVideoCompressed = FALSE;
	m_qwLength = 0;

	m_wStreamAudio = 0;
	m_wStreamVideo = 0;
	m_wStreamScript = 0;

	m_dwOutputAudio = 0;
	m_dwOutputVideo = 0;
	m_dwOutputScript = 0;

    m_pReader = NULL;
    m_pReaderAdvanced = NULL;
	m_pProfile = NULL;
	m_pHeaderInfo = NULL;
}

BOOL MyGenGuidString(char szStr[64])
{
	GUID guid=GUID_NULL;
	CoCreateGuid(&guid);
	if(guid==GUID_NULL) 
		return FALSE;	

	sprintf(szStr, "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return TRUE;
}

BOOL RetrieveOutputNumber(IWMReader *pReader, DWORD *pdwOutputAudio, DWORD *pdwOutputVideo, DWORD *pdwOutputScript, BOOL *pfVideoExisted, BOOL *pfScriptExisted)
{
	if(pReader==NULL)
		return FALSE;

	DWORD dwCount=0;
	HRESULT hr = pReader->GetOutputCount(&dwCount);
	if(FAILED(hr)) return FALSE;

	GUID	guid=GUID_NULL;
	IWMOutputMediaProps *pProps=NULL;
	for(DWORD ii=0; ii<dwCount; ii++) {
		hr = pReader->GetOutputProps(ii, &pProps);
		if(FAILED(hr)) break;

		hr = pProps->GetType(&guid);
		if(FAILED(hr)) break;

		RELEASE(pProps);

		if(guid==WMMEDIATYPE_Audio) {
			if(pdwOutputAudio!=NULL)
				*pdwOutputAudio = ii;
		}
		else if(guid==WMMEDIATYPE_Video) {
			if(pfVideoExisted!=NULL)
				*pfVideoExisted = TRUE;
			if(pdwOutputVideo!=NULL)
				*pdwOutputVideo = ii;
		}
		else if(guid==WMMEDIATYPE_Script) {
			if(pfScriptExisted!=NULL)
				*pfScriptExisted = TRUE;
			if(pdwOutputScript!=NULL)
				*pdwOutputScript = ii;
		}		
	}
	RELEASE(pProps);
	return TRUE;
}

BOOL RetrieveProfileStreamInfo(IWMProfile *pProfile, WORD *pwStreamAudio, WORD *pwStreamVideo, WORD *pwStreamScript, BOOL *pfVideoExisted, BOOL *pfScriptExisted)
{
	if(pProfile==NULL)
		return FALSE;

	DWORD dwStreams=0;
    HRESULT hr = pProfile->GetStreamCount( &dwStreams );
	if ( FAILED( hr ) ) return FALSE;

	if(pfVideoExisted!=NULL) *pfVideoExisted = FALSE;
	if(pfScriptExisted!=NULL) *pfScriptExisted = FALSE;

	GUID	guid=GUID_NULL;
	WORD	wStreamNumber=0;
	IWMStreamConfig *pStream = NULL ;
	for ( DWORD ii = 0; ii < dwStreams; ii++ ) {
		hr = pProfile->GetStream( ii, &pStream );
		if ( FAILED( hr ) ) break;
		
		hr = pStream->GetStreamType(&guid);
		if ( FAILED( hr ) ) break;
        
        hr = pStream->GetStreamNumber( &wStreamNumber ) ;
        if ( FAILED( hr ) ) break;

		RELEASE( pStream );

		if(guid==WMMEDIATYPE_Audio) {
			if(pwStreamAudio!=NULL)
				*pwStreamAudio = wStreamNumber;
		}
		else if(guid==WMMEDIATYPE_Video) {
			if(pfVideoExisted!=NULL)
				*pfVideoExisted = TRUE;
			if(pwStreamVideo!=NULL)
				*pwStreamVideo = wStreamNumber;
		}
		else if(guid==WMMEDIATYPE_Script) {
			if(pfScriptExisted!=NULL)
				*pfScriptExisted = TRUE;
			if(pwStreamScript!=NULL)
				*pwStreamScript = wStreamNumber;
		}		
	}
	RELEASE( pStream );

	return TRUE;
}

BOOL CReaderBase::Open(WCHAR *pwchWMX)
{
	char szGUID[64];
	if(!MyGenGuidString(szGUID)) 
		return FALSE;

    m_hEventAsync = CreateEvent( NULL, FALSE, FALSE, szGUID);
	if(NULL == m_hEventAsync) return FALSE;

	if(!MyGenGuidString(szGUID)) 
		return FALSE;

    m_hTimeAsync = CreateEvent( NULL, FALSE, FALSE, szGUID);
	if(NULL == m_hTimeAsync) return FALSE;

	BOOL fOK=FALSE;

	HRESULT hr = WMCreateReader( NULL, 0, &m_pReader ) ;
	if ( FAILED( hr ) ) goto ERROR_DO;

	hr = m_pReader->QueryInterface( IID_IWMReaderAdvanced, ( VOID ** )&m_pReaderAdvanced );
	if ( FAILED( hr ) ) goto ERROR_DO;

	hr = m_pReader->Open( pwchWMX, this, NULL );
	if ( FAILED ( hr ) ) goto ERROR_DO;

	WaitForSingleObject( m_hEventAsync, INFINITE );
	if ( FAILED( m_hvalAsync ) ) goto ERROR_DO;

	hr = m_pReader->QueryInterface( IID_IWMProfile, ( VOID ** )&m_pProfile );
	if ( FAILED( hr ) ) goto ERROR_DO;

	hr = m_pReader->QueryInterface( IID_IWMHeaderInfo, ( VOID ** )&m_pHeaderInfo );
	if ( FAILED( hr ) ) goto ERROR_DO;

	if(!RetrieveProfileStreamInfo(m_pProfile, &m_wStreamAudio, &m_wStreamVideo, &m_wStreamScript,
		&m_fVideoExisted, &m_fScriptExisted))
		goto ERROR_DO;

	if(!RetrieveOutputNumber(m_pReader, &m_dwOutputAudio, &m_dwOutputVideo, &m_dwOutputScript, NULL, NULL))
		goto ERROR_DO;

	{
		WORD wStreamNum = 0 ;
		WMT_ATTR_DATATYPE type ;			
		WORD cbLength = sizeof(m_qwLength) ;
		hr = m_pHeaderInfo->GetAttributeByName(&wStreamNum,  g_wszWMDuration, &type, (BYTE *)&m_qwLength, &cbLength ) ;
		if(FAILED(hr)) goto ERROR_DO;
	}

	fOK = TRUE;

ERROR_DO:
	if(!fOK) Close();
	
	return fOK;
}

void CReaderBase::Close()
{
	if(m_pReader!=NULL) m_pReader->Close();

	RELEASE( m_pHeaderInfo) ;
    RELEASE( m_pProfile ) ;
    RELEASE( m_pReader ) ;
//	RELEASE( m_pReaderAdvanced ) ;
    
	if(m_hEventAsync!=NULL) {
		CloseHandle( m_hEventAsync ) ;
		m_hEventAsync = NULL;
	}

	if(m_hTimeAsync!=NULL) {
		CloseHandle( m_hTimeAsync ) ;
		m_hTimeAsync = NULL;
	}

	ResetVariables();
}

BOOL CReaderBase::WaitForStop(HANDLE hEvent, DWORD dwmsWait)
{
	if(!IsWindow(m_hWndProgress)&&!IsWindow(m_hWndAbort)) {
		WaitForSingleObject( hEvent, dwmsWait );
		return TRUE;
	}

	do {
		if(m_fEOF) break;
		if(m_fAutoProgressEnabled&&IsWindow(m_hWndProgress)) 
			SendMessage(m_hWndProgress, m_lMsgProgress, 0, MAKELONG((WORD)(m_qwUserTime/TIMEUNIT), (WORD)(m_qwLength/TIMEUNIT)));

		MSG	msg;
		if(IsWindow(m_hWndAbort)) {
			while (PeekMessage(&msg, m_hWndAbort, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		while (PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if((m_pbAbort!=NULL)&&((*m_pbAbort)!=0)) {
			m_qwUserForceStopTime = m_qwUserTime;
			m_fUserCheckStop = TRUE; //Let Data Flush!!			
		}
		if(WAIT_OBJECT_0==WaitForSingleObject( hEvent, TIMEPERIOD ))
			break;
	} while(TRUE);

	return TRUE;
}

BOOL CReaderBase::Run_PlayMode_Start(QWORD qwStart, QWORD qwDuration, float dRate)
{
	if(!SetUserClock(FALSE))
		return FALSE;

	if(m_pReader==NULL) return FALSE;

	m_qwStartTime = qwStart;
	HRESULT hr = m_pReader->Start(qwStart, qwDuration, dRate, NULL);
	return SUCCEEDED(hr);
}

BOOL CReaderBase::Run_PlayMode_Pause()
{
	if(m_pReader==NULL) 
		return FALSE;

	HRESULT hr = m_pReader->Pause();
	return SUCCEEDED(hr);
}

BOOL CReaderBase::Run_PlayMode_Resume()
{
	if(m_pReader==NULL) 
		return FALSE;

	HRESULT hr = m_pReader->Resume();
	return SUCCEEDED(hr);
}

BOOL CReaderBase::Run_PlayMode_Stop()
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	HRESULT hr = m_pReader->Stop();
	return SUCCEEDED(hr);
}

BOOL CReaderBase::Run_ManualMode(QWORD qwStart)
{
	if(!SetUserClock(TRUE))
		return FALSE;

	HRESULT hr = 0;

	m_qwStartTime = qwStart;
	m_fManualDeliver = TRUE;
	m_fUserCheckStop = FALSE;
	
	m_qwUserTime = qwStart;
	hr = m_pReader->Start(qwStart, 0, 1.0, 0 );
	if ( FAILED( hr ) ) return FALSE;

	return TRUE;
}

BOOL CReaderBase::Run_AutoMode(QWORD qwStart, QWORD qwDuration)
{
	if(!SetUserClock(TRUE))
		return FALSE;
	
	HRESULT hr = 0;

	m_qwStartTime = qwStart;
	m_fManualDeliver = FALSE;
	m_fUserCheckStop = (qwDuration!=0);
	m_qwUserForceStopTime = qwStart + qwDuration;

	m_qwUserTime = qwStart;
	hr = m_pReader->Start(qwStart, 0, 1.0, 0 );
	if ( FAILED( hr ) ) return FALSE;

	WaitForStop(m_hEventAsync, INFINITE);

	hr = m_pReader->Stop( ); // Stop Merge
	if ( FAILED( hr ) ) 
		return FALSE;	

	if( FAILED( m_hvalAsync ) ) 
		return FALSE;

	return TRUE;
}

BOOL CReaderBase::ManualDeliverTo(QWORD qwTime)
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	m_qwUserTime = qwTime;
	HRESULT hval = m_pReaderAdvanced->DeliverTime( m_qwUserTime );

	return TRUE;
}

BOOL CReaderBase::AutoDeliverMore(QWORD qwDuration)
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	if(!m_fEOF) {
		if(m_fUserCheckStop) {
			if(m_qwUserForceStopTime<=m_qwUserTime) {
				m_fEOF = TRUE;
				m_hvalAsync = S_OK;
				SetEvent( m_hEventAsync );
				return TRUE;
			}
			else {
				if((m_qwUserTime+qwDuration)>=m_qwUserForceStopTime)
					qwDuration = m_qwUserForceStopTime - m_qwUserTime;					
			}
		}
		m_qwUserTime += qwDuration;
		m_pReaderAdvanced->DeliverTime( m_qwUserTime );
	}

	return TRUE;
}

BOOL CReaderBase::SetUserClock(BOOL fUserProvided)
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	HRESULT hr = m_pReaderAdvanced->SetUserProvidedClock( fUserProvided );
	return SUCCEEDED(hr);
}

BOOL CReaderBase::SetSelectedStreams(BOOL fAudio, BOOL fVideo, BOOL fScript)
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	HRESULT hr = m_pReaderAdvanced->SetManualStreamSelection(TRUE);
	if(FAILED(hr)) return FALSE;

	WORD					streamnums[3];
	WMT_STREAM_SELECTION	streamsels[3];

	WORD	wCount=0;
	streamnums[wCount] = m_wStreamAudio;
	streamsels[wCount] = fAudio?WMT_ON:WMT_OFF;
	wCount++;

	if(m_fVideoExisted) {
		streamnums[wCount] = m_wStreamVideo;
		streamsels[wCount] = fVideo?WMT_ON:WMT_OFF;
		wCount++;
	}
	if(m_fScriptExisted) {
		streamnums[wCount] = m_wStreamScript;
		streamsels[wCount] = fScript?WMT_ON:WMT_OFF;
		wCount++;
	}

	hr = m_pReaderAdvanced->SetStreamsSelected(wCount, streamnums, streamsels);

	return SUCCEEDED(hr);
}

BOOL CReaderBase::ConfigAudioOutputCompressed()
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;
	HRESULT hr = m_pReaderAdvanced->SetReceiveStreamSamples( m_wStreamAudio, TRUE);
	if ( FAILED( hr ) ) return FALSE;

	return TRUE;
}

BOOL CReaderBase::ConfigVideoOutputCompressed()
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	if(!m_fVideoExisted) return FALSE;

	HRESULT hr = m_pReaderAdvanced->SetReceiveStreamSamples( m_wStreamVideo, TRUE);
	if ( FAILED( hr ) ) return FALSE;

	return TRUE;
}

BOOL CReaderBase::ConfigScriptOutputCompressed()
{
	if(m_pReaderAdvanced==NULL)
		return FALSE;

	if(!m_fScriptExisted) return FALSE;

	HRESULT hr = m_pReaderAdvanced->SetReceiveStreamSamples( m_wStreamScript, TRUE);
	if ( FAILED( hr ) ) return FALSE;

	return TRUE;
}

BOOL CReaderBase::ConfigAudioOutputFormat(WM_MEDIA_TYPE *pwmt)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = m_pReader->GetOutputProps(m_dwOutputAudio, &pProps);
	if(FAILED(hr)) return FALSE;

	hr = pProps->SetMediaType(pwmt);
	RELEASE(pProps);
	if(FAILED(hr)) return FALSE;

	return TRUE;
}

BOOL CReaderBase::ConfigVideoOutputFormat(WM_MEDIA_TYPE *pwmt)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = m_pReader->GetOutputProps(m_dwOutputVideo, &pProps);
	if(FAILED(hr)) return FALSE;

	hr = pProps->SetMediaType(pwmt);
	RELEASE(pProps);
	if(FAILED(hr)) return FALSE;

	return TRUE;
}

BOOL CReaderBase::ConfigAudioOutputSimpleUncompressed(WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = m_pReader->GetOutputProps(m_dwOutputAudio, &pProps);
	if(FAILED(hr)) return FALSE;

	DWORD cbType=0;
	hr = pProps->GetMediaType(NULL, &cbType);
	if(SUCCEEDED(hr)) {
		hr = E_FAIL;
		WM_MEDIA_TYPE *pwmftmp = (WM_MEDIA_TYPE*)malloc(cbType);		
		if(pwmftmp!=NULL) {
			hr = pProps->GetMediaType(pwmftmp, &cbType);
			if(SUCCEEDED(hr)) {
				WAVEFORMATEX *pwih=(WAVEFORMATEX *)pwmftmp->pbFormat;

				pwih->wFormatTag = WAVE_FORMAT_PCM;
				pwih->wBitsPerSample = wBitsPerSample;
				pwih->nSamplesPerSec = dwSampleRate;
				pwih->nChannels = wChannels;
				pwih->nBlockAlign = (pwih->nChannels * pwih->wBitsPerSample)/8;
				pwih->nAvgBytesPerSec = (pwih->nSamplesPerSec * pwih->nBlockAlign);
				pwih->cbSize = 0; // Extended Data
				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pReader->SetOutputProps(m_dwOutputAudio, pProps);
				}
			}
			free(pwmftmp);
		}
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

BOOL CReaderBase::ConfigVideoOutputSimpleUncompressed(WORD wBitCount, DWORD biCompression)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = m_pReader->GetOutputProps(m_dwOutputVideo, &pProps);
	if(FAILED(hr)) return FALSE;

	DWORD cbType=0;
	hr = pProps->GetMediaType(NULL, &cbType);
	if(SUCCEEDED(hr)) {
		hr = E_FAIL;
		WM_MEDIA_TYPE *pwmftmp = (WM_MEDIA_TYPE*)malloc(cbType);		
		if(pwmftmp!=NULL) {
			hr = pProps->GetMediaType(pwmftmp, &cbType);
			if(SUCCEEDED(hr)) {
				WMVIDEOINFOHEADER *pvih=(WMVIDEOINFOHEADER *)pwmftmp->pbFormat;
				pvih->bmiHeader.biBitCount = wBitCount;
				pvih->bmiHeader.biPlanes = 1;
				pvih->bmiHeader.biCompression = biCompression;
				pvih->bmiHeader.biSizeImage = DIBSIZE(pvih->bmiHeader);

				pwmftmp->bFixedSizeSamples = TRUE;
				pwmftmp->bTemporalCompression = FALSE;
				pwmftmp->pUnk = NULL;
				pwmftmp->lSampleSize = pvih->bmiHeader.biSizeImage;

				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pReader->SetOutputProps(m_dwOutputVideo, pProps);
				}
			}
			free(pwmftmp);
		}
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

BOOL CReaderBase::ConfigVideoOutputSimpleUncompressedDim(DWORD dwWidth, DWORD dwHeight)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = m_pReader->GetOutputProps(m_dwOutputVideo, &pProps);
	if(FAILED(hr)) return FALSE;

	DWORD cbType=0;
	hr = pProps->GetMediaType(NULL, &cbType);
	if(SUCCEEDED(hr)) {
		hr = E_FAIL;
		WM_MEDIA_TYPE *pwmftmp = (WM_MEDIA_TYPE*)malloc(cbType);		
		if(pwmftmp!=NULL) {
			hr = pProps->GetMediaType(pwmftmp, &cbType);
			if(SUCCEEDED(hr)) {
				WMVIDEOINFOHEADER *pvih=(WMVIDEOINFOHEADER *)pwmftmp->pbFormat;
				pvih->bmiHeader.biWidth = (LONG)dwWidth;
				pvih->bmiHeader.biHeight = (LONG)dwHeight;
				pvih->bmiHeader.biSizeImage = DIBSIZE(pvih->bmiHeader);

				pvih->rcSource.left = 0;
				pvih->rcSource.right = (LONG)dwWidth;
				pvih->rcSource.top = 0;
				pvih->rcSource.bottom = (LONG)dwHeight;
				
				pvih->rcTarget.left = 0;
				pvih->rcTarget.right = (LONG)dwWidth;
				pvih->rcTarget.top = 0;
				pvih->rcTarget.bottom = (LONG)dwHeight;

				pwmftmp->bFixedSizeSamples = TRUE;
				pwmftmp->bTemporalCompression = FALSE;
				pwmftmp->pUnk = NULL;
				pwmftmp->lSampleSize = pvih->bmiHeader.biSizeImage;

				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pReader->SetOutputProps(m_dwOutputVideo, pProps);
				}
			}
			free(pwmftmp);
		}
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

WM_MEDIA_TYPE *GetOutputFormat(IWMReader *pReader, DWORD dwOutput)
{
	if(pReader==NULL) return NULL;

	WM_MEDIA_TYPE *pwmftmp=NULL;

	IWMOutputMediaProps *pProps=NULL;
	HRESULT hr = pReader->GetOutputProps(dwOutput, &pProps);
	if(FAILED(hr)) return NULL;

	DWORD cbType=0;
	hr = pProps->GetMediaType(NULL, &cbType);
	if(SUCCEEDED(hr)) {
		hr = E_FAIL;
		pwmftmp = (WM_MEDIA_TYPE*)malloc(cbType);		
		if(pwmftmp!=NULL) {
			hr = pProps->GetMediaType(pwmftmp, &cbType);
			if(FAILED(hr)) {
				free(pwmftmp);
				return NULL;
			}
		}
	}
	if(FAILED(hr)) return NULL;

	return pwmftmp;
}

WM_MEDIA_TYPE *CReaderBase::GetAudioOutputFormat()
{
	return GetOutputFormat(m_pReader, m_dwOutputAudio);
}

WM_MEDIA_TYPE *CReaderBase::GetVideoOutputFormat()
{
	if(!m_fVideoExisted)
		return NULL;

	return GetOutputFormat(m_pReader, m_dwOutputVideo);
}

WM_MEDIA_TYPE *CReaderBase::GetScriptOutputFormat()
{
	if(!m_fScriptExisted)
		return NULL;

	return GetOutputFormat(m_pReader, m_dwOutputScript);
}


void CReaderBase::FreeFormatAllocated(WM_MEDIA_TYPE *pwmt)
{
	if(pwmt!=NULL)
		free(pwmt);
}

BOOL CReaderBase::CopyAllHeaders(IWMHeaderInfo* pWriterHeaderInfo)
{
	HRESULT hr = WMX_CopyAllHeaders(m_pHeaderInfo, pWriterHeaderInfo);
	return SUCCEEDED(hr);
}

BOOL CReaderBase::CopyScript(IWMHeaderInfo* pWriterHeaderInfo)
{
	HRESULT hr = WMX_CopyScript(m_pHeaderInfo, pWriterHeaderInfo);
	return SUCCEEDED(hr);
}

BOOL CReaderBase::CopyMarker(BSTR bstrOutput)
{
	HRESULT hr = WMX_CopyMarker(m_pHeaderInfo, bstrOutput);
	return SUCCEEDED(hr);
}

///////////////////////////////////////////////////////////////////////////////////
// Another Thread Function!!

HRESULT CReaderBase::OnStatus(WMT_STATUS Status, HRESULT hr, WMT_ATTR_DATATYPE dwType, 
						  BYTE __RPC_FAR *pValue, void __RPC_FAR *pvContext)
{
	HRESULT hval=0;
	switch ( Status ) {
	case WMT_OPENED:
		m_qwUserTime = 0;
		m_hvalAsync = hr;
		SetEvent( m_hEventAsync );
		break;
	case WMT_STARTED: 
		{
			BOOL fUserClock=FALSE;
			hval = m_pReaderAdvanced->GetUserProvidedClock(&fUserClock);
			if(fUserClock) {
				if(m_fManualDeliver) {
					m_hvalAsync = hr;
					SetEvent( m_hTimeAsync );
				}
				else AutoDeliverMore(TIMEUNIT);
			}
		}
		break;
	case WMT_STOPPED:
		break;
	case WMT_END_OF_SEGMENT:
		m_hvalAsync = hr;
		break;
	case WMT_EOF:
		m_fEOF = TRUE;
		m_hvalAsync = hr;
		SetEvent( m_hEventAsync );
		if(m_fManualDeliver) 
			SetEvent( m_hTimeAsync );
		break;
	}

	return S_OK;
}

HRESULT CReaderBase::OnTime(QWORD qwCurrentTime, void __RPC_FAR *pvContext)
{
	if(m_fEOF) {
		m_hvalAsync = S_OK;
		SetEvent(m_hEventAsync);
	}
	else if(m_fManualDeliver) {
		m_hvalAsync = S_OK;
		SetEvent(m_hTimeAsync);
	} 
	else AutoDeliverMore(TIMEUNIT);

	return S_OK;
}
