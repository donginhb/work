#include <afxwin.h>
#include <streams.h>
#include <dshowasf.h>
#include <dvdmedia.h>
#include <qedit.h>
#include <atlbase.h>
#include <wmsysprf.h>

#include "KeyProvider.h"
#include "HelperDShow.h"
#include "HelperProfile.h"
#include "HelperKit.h"

#include "Media2WMX.h"
#include "inftee_AV.h"
#include "null_AV.h"

#define MUTE_SAMPLERATE			8000
#define MUTE_BITSPERSAMPLE		16

#undef TIMEUNIT
#define TIMEUNIT		(LONGLONG)10000000

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

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
QWORD g_WMXGetFileDuration(WCHAR *bstrFile)
{	
	HRESULT hr=0;
	HRESULT hvalAsync = 0;
    HANDLE hEventAsync = CreateEvent( NULL, FALSE, FALSE, WM_DURATION_ASYNC_EVENT );
	if(NULL == hEventAsync) return FALSE;
	
	CDurationReaderCallback	callback;
	
	callback.m_hEventAsync = hEventAsync;
	callback.m_phvalAsync = &hvalAsync;
	
	QWORD		qwDuration=0;
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
					hr = pHeaderInfo->GetAttributeByName(&wStreamNum,  g_wszWMDuration, &type, (BYTE *)&qwDuration, &cbLength ) ;
					
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
	
	return qwDuration;
}

BOOL GetMediaFileParameter(WCHAR *wchFile, BOOL *pfAudio, AUDIO_PARAMS &apm, BOOL *pfVideo, VIDEO_PARAMS &vpm)
{
	CComPtr< IMediaDet > pDet;
	HRESULT hr = CoCreateInstance(CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER, IID_IMediaDet, (void**)&pDet );
    if( FAILED( hr ) ) return NULL;

    hr = pDet->put_Filename( wchFile );
    if( FAILED( hr ) ) return NULL;

    long lStreamCount = 0;
    hr = pDet->get_OutputStreams( &lStreamCount );
    if( FAILED( hr ) ) return NULL;

	*pfAudio = FALSE;
	*pfVideo = FALSE;

	AM_MEDIA_TYPE amt;    
    for( int ii = 0 ; ii < lStreamCount ; ii++ ) {
        memset( &amt, 0, sizeof( amt ) );

        hr = pDet->put_CurrentStream( ii );
        if( FAILED( hr ) ) return NULL;

        hr = pDet->get_StreamMediaType( &amt );
        if( FAILED( hr ) ) return NULL;

		BOOL fOK=TRUE;
		if( !(*pfVideo) && (amt.majortype == MEDIATYPE_Video) ) {            
			fOK = PF_GetVideoStreamParameter(*(WM_MEDIA_TYPE*)(&amt), vpm);
			if(fOK) *pfVideo = TRUE;
		}
		else if( !(*pfAudio) && (amt.majortype == MEDIATYPE_Audio) ) {			
			fOK = PF_GetAudioStreamParameter(*(WM_MEDIA_TYPE*)(&amt), apm);
			if(fOK) *pfAudio = TRUE;
		}

        FreeMediaType( amt );

		if(!fOK) return NULL;
    }

	return TRUE;
}

CMedia2WMX::CMedia2WMX()
{
	m_hWndProgress = NULL;
	m_hWndAbort = NULL;
	m_plAbort = NULL;
	m_lMsgProgress = 0;

	m_pKeyProvider = NULL;
	m_dwDebugRegister = 0;
	m_pWMPM = NULL;

	memset(&m_Data, 0, sizeof(m_Data));
}

CMedia2WMX::~CMedia2WMX()
{
	Close();
}

BOOL CMedia2WMX::WMV2WMV_MBR(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut)
{
	if(!Open()) return FALSE;	
	
	IWMProfile *pTmpProfile = PF_LoadFromPRX(m_pWMPM, bstrProfile);
	if(pTmpProfile==NULL) return FALSE;
	
	IWMProfile *pProfile = PF_Subtract(m_pWMPM, pTmpProfile, TRUE, FALSE);
	RELEASE(pTmpProfile);
	if(pProfile==NULL) return FALSE;
	
	BOOL fMute = IsMediaMute(bstrIn);
	BOOL fOK = ConvertWMV_MBR(bstrIn, bstrOut, fMute, pProfile);
	
	Close();
	return fOK;
}

BOOL CMedia2WMX::Media2WMV(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut)
{
	if(!Open()) return FALSE;	

	IWMProfile *pTmpProfile = PF_LoadFromPRX(m_pWMPM, bstrProfile);
	if(pTmpProfile==NULL) return FALSE;

	IWMProfile *pProfile = PF_Subtract(m_pWMPM, pTmpProfile, TRUE, FALSE);
	RELEASE(pTmpProfile);
	if(pProfile==NULL) return FALSE;

	BOOL fMute = IsMediaMute(bstrIn);
	BOOL fOK = ConvertWMV(bstrIn, bstrOut, fMute, pProfile);

	Close();
	return fOK;
}

BOOL CMedia2WMX::Media2WMV_Param(BSTR bstrIn, DWORD dwAudioBitrate, DWORD dwVideoBitrate, BSTR bstrOut, BSTR bstrOutProfile)
{
	if(!Open()) return FALSE;	

	BOOL fMute=FALSE;
	IWMProfile *pProfile=NULL;
	if(!MediaBuildProfile(bstrIn, dwAudioBitrate, dwVideoBitrate, &fMute, &pProfile))
		return FALSE;

	if((bstrOutProfile!=NULL)&&(CString(bstrOutProfile)!="")) {
		if(S_OK!=PF_SavePRX(m_pWMPM, pProfile, bstrOutProfile))
			return FALSE;
	}
	
	BOOL fOK = ConvertWMV(bstrIn, bstrOut, fMute, pProfile);

	RELEASE(pProfile);

	Close();
	return fOK;
}

BOOL CMedia2WMX::Media2WMA(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut)
{
	if(!Open()) return FALSE;

//	if(IsMediaMute(bstrIn))
//		return FALSE;

	IWMProfile *pTmpProfile = PF_LoadFromPRX(m_pWMPM, bstrProfile);
	if(pTmpProfile==NULL) return FALSE;

	IWMProfile *pProfile = PF_Subtract(m_pWMPM, pTmpProfile, FALSE, FALSE);
	RELEASE(pTmpProfile);
	if(pProfile==NULL) return FALSE;


	AUDIO_PARAMS	apm;
	VIDEO_PARAMS	vpm;
	SCRIPT_PARAMS	spm;

	if(!PF_GetParameter(pProfile, apm, vpm, spm)) {
		RELEASE(pProfile);
		return FALSE;
	}
	if((apm.dwBitrate==0)||(vpm.dwBitrate!=0)) {
		RELEASE(pProfile);
		return FALSE; // No Audio Only Stream
	}
	
	BOOL fOK = ConvertWMA(bstrIn, bstrOut, pProfile);

	RELEASE(pProfile);

	Close();
	return fOK;
}

BOOL CMedia2WMX::Media2WMA_Param(BSTR bstrIn, DWORD dwAudioBitrate, BSTR bstrOut, BSTR bstrOutProfile)
{
	if(!Open()) return FALSE;

	BOOL fMute=FALSE;
	IWMProfile *pProfile=NULL;
	if(!MediaBuildProfile(bstrIn, dwAudioBitrate, 0, &fMute, &pProfile))
		return FALSE;

	if(fMute) {
		RELEASE(pProfile);
		return FALSE;
	}

	if((bstrOutProfile!=NULL)&&(CString(bstrOutProfile)!="")) {
		if(S_OK!=PF_SavePRX(m_pWMPM, pProfile, bstrOutProfile)) {
			RELEASE(pProfile);
			return FALSE;
		}
	}
	
	BOOL fOK = ConvertWMA(bstrIn, bstrOut, pProfile);

	RELEASE(pProfile);

	Close();
	return fOK;
}
//////////////////////////////////////////////////////////////////////////////////////
//

BOOL CMedia2WMX::Open()
{
	Close();

	if(!BuildKeyProvider()) return FALSE;

	HRESULT	hval=0;
	IWMProfileManager *pPM0=NULL;

	hval = E_FAIL;
    hval = WMCreateProfileManager(&pPM0);
    if(SUCCEEDED(hval)) {
		hval = pPM0->QueryInterface(IID_IWMProfileManager2, (void**)&m_pWMPM);
		RELEASE(pPM0);
	}

	if(FAILED(hval)) {
		Close();
		return FALSE;
	}

	return TRUE;
}

void CMedia2WMX::Close()
{
	FreeAll();
	RELEASE(m_pWMPM);
 
	FreeKeyProvider();
	m_dwDebugRegister = 0;
}

BOOL CMedia2WMX::MediaBuildProfile(BSTR bstrIn, DWORD dwAudioBitrate, DWORD dwVideoBitrate, BOOL *pbMute, IWMProfile **ppProfile)
{
	if(m_pWMPM==NULL) return FALSE; // Not Open Yet!!

	BOOL bAudio=FALSE, bVideo=FALSE;
	AUDIO_PARAMS	apm;
	VIDEO_PARAMS	vpm;

	if(!GetMediaFileParameter(bstrIn, &bAudio, apm, &bVideo, vpm)) 
		return FALSE;
	if((!bAudio)&&(!bVideo)) return FALSE; // No AV Stream

	*pbMute = !bAudio; // Mute!!

	if(!bAudio) { // For Mute!!		
		apm.dwFormatTag = CODEC_AUDIO_MSAUDIO;
		apm.dwChannels = 1;
		apm.dwSampleRate = MUTE_SAMPLERATE;
		apm.dwBitrate =  5000; // Minimum!!
		// ORG: apm.dwChannels * MUTE_BITSPERSAMPLE * apm.dwSampleRate
	}
	
	if(bAudio) {
		if((apm.dwBitrate==0)||(dwAudioBitrate<apm.dwBitrate)) apm.dwBitrate = dwAudioBitrate;
		PF_CorrectAPM(apm);
	}

	if(bVideo) {		
		if((vpm.dwBitrate==0)||(dwVideoBitrate<vpm.dwBitrate)) vpm.dwBitrate = dwVideoBitrate;	
		PF_CorrectVPM(vpm);
	}
	
	if(bVideo)	*ppProfile = PF_Generate(m_pWMPM, &apm, NULL, 1, &vpm);
	else *ppProfile = PF_Generate(m_pWMPM, &apm, NULL, 0, NULL);

	if(*ppProfile==NULL) return FALSE;	

	return TRUE;
}

BOOL CMedia2WMX::IsMediaMute(BSTR bstrMedia)
{
	BOOL bAudio=FALSE, bVideo=FALSE;
	AUDIO_PARAMS apm;
	VIDEO_PARAMS vpm;

	if(!GetMediaFileParameter(bstrMedia, &bAudio, apm, &bVideo, vpm)) 
		return FALSE;

	return !bAudio; // Mute!!
}

BOOL CMedia2WMX::ConvertWMV_MBR(BSTR bstrIn, BSTR bstrOut, BOOL fMute, IWMProfile *pProfile)
{
	if(pProfile==NULL) return FALSE;
	
	FreeAll();	
	
	BOOL fOK=FALSE;
	if(!CreateFilterGraph()) goto ERROR_2ASF;
	if(!MakeWMVWriterGraph_MBR(bstrIn, bstrOut, pProfile)) goto ERROR_2ASF;
//	if(!ProcessGraphInfo(fMute)) goto ERROR_2ASF; has done in previous function!!
	if(!GoConvert()) goto ERROR_2ASF;
	
	fOK = TRUE;
ERROR_2ASF:
	FreeAll();
	
	return fOK;
}

BOOL CMedia2WMX::ConvertWMV(BSTR bstrIn, BSTR bstrOut, BOOL fMute, IWMProfile *pProfile)
{
	if(pProfile==NULL) return FALSE;

	FreeAll();	

	BOOL fOK=FALSE;
	if(!CreateFilterGraph()) goto ERROR_2ASF;
	if(!MakeWMVWriterGraph(bstrIn, bstrOut, pProfile)) goto ERROR_2ASF;
	if(!ProcessGraphInfo(fMute)) goto ERROR_2ASF;
	if(!GoConvert()) goto ERROR_2ASF;

	fOK = TRUE;
ERROR_2ASF:
	FreeAll();

	return fOK;
}

BOOL CMedia2WMX::ConvertWMA(BSTR bstrIn, BSTR bstrOut, IWMProfile *pProfile)
{
	if(pProfile==NULL) return FALSE;

	FreeAll();	

	BOOL fOK=FALSE;
	if(!CreateFilterGraph()) goto ERROR_2ASF;
	if(!MakeWMAWriterGraph(bstrIn, bstrOut, pProfile)) goto ERROR_2ASF;
	if(!ProcessGraphInfo(FALSE)) goto ERROR_2ASF;
	if(!GoConvert()) goto ERROR_2ASF;

	fOK = TRUE;
ERROR_2ASF:
	FreeAll();

	return fOK;
}

/////////////////////////////////////////////////////////////////

BOOL CMedia2WMX::CreateFilterGraph()
{
	HRESULT hval=0;

	if(m_Data.pFG==NULL) {
		hval = CoCreateInstance((REFCLSID)CLSID_FilterGraph, NULL, CLSCTX_INPROC,
					(REFIID)IID_IGraphBuilder, (void **)&m_Data.pFG);
		if(FAILED(hval)) return FALSE;
	}

#ifdef _DEBUG  
	hval = AddToRot(m_Data.pFG, &m_dwDebugRegister);
#endif

// Setup KeyProvider for WMFormatSDK!!	
	IObjectWithSite* pObjectWithSite=NULL;
	hval = m_Data.pFG->QueryInterface(IID_IObjectWithSite, (void**)&pObjectWithSite);
	if(FAILED(hval)) return FALSE;
	hval = pObjectWithSite->SetSite((IUnknown*)m_pKeyProvider);
	RELEASE(pObjectWithSite);
	if(FAILED(hval)) return FALSE;

	return SUCCEEDED(hval);
}

BOOL CMedia2WMX::MakeWMVWriterGraph_MBR(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfRefer)
{
	HRESULT hval=0;

	m_Data.l2TotalLength = g_WMXGetFileDuration(bstrIn);

	CAVNullRenderer *pAudioNull = new CAVNullRenderer(NULL, &hval);
	if(FAILED(hval)) return FALSE;	
	pAudioNull->AddRef();

	pAudioNull->PresetMediaType(&MEDIATYPE_Audio, &MEDIASUBTYPE_PCM);

	hval = m_Data.pFG->AddFilter(pAudioNull, L"AudioNull");
	if(FAILED(hval)) return FALSE;

	CAVNullRenderer *pVideoNull = new CAVNullRenderer(NULL, &hval);
	if(FAILED(hval)) return FALSE;	
	pVideoNull->AddRef();

	pVideoNull->PresetMediaType(&MEDIATYPE_Video, &MEDIASUBTYPE_RGB24);

	hval = m_Data.pFG->AddFilter(pVideoNull, L"VideoNull");
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pFG->RenderFile(bstrIn, NULL);
	if(FAILED(hval)) return FALSE;			

    hval = m_Data.pFG->QueryInterface(IID_IMediaSeeking, (void **)&m_Data.pMS);
	if(FAILED(hval)) return FALSE;

// After expand the media file graph

	CAVTee *pAudioTee = new CAVTee(TEXT("AudioTee"), NULL, &hval);
	if(FAILED(hval)) return FALSE;	
	pAudioTee->AddRef();

	pAudioTee->PresetMediaType(&MEDIATYPE_Audio, &MEDIASUBTYPE_PCM);

	hval = m_Data.pFG->AddFilter(pAudioTee, L"AudioTee");
	if(FAILED(hval)) return FALSE;

	CAVTee *pVideoTee = new CAVTee(TEXT("VideoTee"), NULL, &hval);
	if(FAILED(hval)) return FALSE;	
	pVideoTee->AddRef();

	pVideoTee->PresetMediaType(&MEDIATYPE_Video, &MEDIASUBTYPE_RGB24);

	hval = m_Data.pFG->AddFilter(pVideoTee, L"VideoTee");
	if(FAILED(hval)) return FALSE;

	{
		// Audio Part
		IPin *pIn=NULL, *pOut=NULL;

		pIn = GetInPin(pAudioNull, 0);
		hval = pIn->ConnectedTo(&pOut);
		if(FAILED(hval)) return FALSE;

		m_Data.pFG->Disconnect(pIn);
		m_Data.pFG->Disconnect(pOut);

		pIn->Release();

		pIn = GetInPin(pAudioTee, 0);

		hval = m_Data.pFG->ConnectDirect(pOut, pIn, NULL);
		pIn->Release();
		pOut->Release();
		if(FAILED(hval)) return FALSE;

		m_Data.pFG->RemoveFilter(pAudioNull);

	// Video Part
		pIn = GetInPin(pVideoNull, 0);
		hval = pIn->ConnectedTo(&pOut);
		if(FAILED(hval)) return FALSE;

		m_Data.pFG->Disconnect(pIn);
		m_Data.pFG->Disconnect(pOut);

		pIn->Release();

		pIn = GetInPin(pVideoTee, 0);

		hval = m_Data.pFG->ConnectDirect(pOut, pIn, NULL);
		pIn->Release();
		pOut->Release();
		if(FAILED(hval)) return FALSE;

		m_Data.pFG->RemoveFilter(pVideoNull);
	}

    hval = CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pAsfMux);
	if(FAILED(hval)) return FALSE;
	
	hval = m_Data.pAsfMux->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
	if(FAILED(hval)) {
		hval = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pWriter);
		if(FAILED(hval)) return FALSE;
		
		hval = m_Data.pWriter->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
		if(FAILED(hval)) return FALSE;
	}
	
	hval = m_Data.pFileSink->SetFileName(bstrOut, NULL);
	if(FAILED(hval)) return FALSE;
	
	hval = m_Data.pFG->AddFilter(m_Data.pAsfMux, L"Mux");
	if(FAILED(hval)) return FALSE;
	
    // Set interleaving mode to FULL
    // !!! ASF won't support this, but that's okay
	hval = m_Data.pAsfMux->QueryInterface(IID_IConfigInterleaving, (void **)&m_Data.pConfigInterleaving);
	if(SUCCEEDED(hval)) {
		hval = m_Data.pConfigInterleaving->put_Mode(INTERLEAVE_FULL);
		// Do'nt Check Error Here!!
	}
	
    hval = m_Data.pAsfMux->QueryInterface(IID_IConfigAsfWriter, (void **)&m_Data.pConfigAsfWriter);
	if(FAILED(hval)) return FALSE;
	
	hval = m_Data.pConfigAsfWriter->ConfigureFilterUsingProfile(pwmpfRefer);
	if(FAILED(hval)) return FALSE;
	
	if(m_Data.pWriter!=NULL) {
		hval = m_Data.pFG->AddFilter(m_Data.pWriter, L"Writer");
		if(FAILED(hval)) return FALSE;
		
		IPin *pMuxOut=NULL, *pWriterIn=NULL;
		hval = FindPinOnFilter(m_Data.pAsfMux, PINDIR_OUTPUT, 0, FALSE, &pMuxOut);
		if(SUCCEEDED(hval)) {
			hval = FindPinOnFilter(m_Data.pWriter, PINDIR_INPUT, 0, FALSE, &pWriterIn);
			if(SUCCEEDED(hval)) {
				hval = m_Data.pFG->ConnectDirect(pMuxOut, pWriterIn, NULL);
				RELEASE(pWriterIn);
			}
			RELEASE(pMuxOut);
		}
		if(FAILED(hval)) return FALSE;
	}
	
	hval = m_Data.pConfigAsfWriter->SetIndexMode(FALSE);
	if(FAILED(hval)) return FALSE;

	int ii=0, jj=0;
	for(ii=0; ii<1000; ii++) {
		IPin *pOut = GetOutPin(pAudioTee, ii);
		if(pOut==NULL) break;

		hval = E_FAIL;
		for(jj=0; jj<1000; jj++) {
			IPin *pIn = GetInPin(m_Data.pAsfMux, jj);
			if(pIn==NULL) break;

			IPin *pTmpOut = NULL;
			hval = pIn->ConnectedTo(&pTmpOut);
			if(pTmpOut) pTmpOut->Release();
			if(SUCCEEDED(hval)) continue; // has connected

			hval = m_Data.pFG->ConnectDirect(pOut, pIn, NULL);
			pIn->Release();
			if(SUCCEEDED(hval)) break; // connect one!!
		}

		pOut->Release();

		if(hval==E_FAIL) break; // no other can connect!!
	}
	
	for(ii=0; ii<1000; ii++) {
		IPin *pOut = GetOutPin(pVideoTee, ii);
		if(pOut==NULL) break;
		
		hval = E_FAIL;
		for(jj=0; jj<1000; jj++) {
			IPin *pIn = GetInPin(m_Data.pAsfMux, jj);
			if(pIn==NULL) break;
			
			IPin *pTmpOut = NULL;
			hval = pIn->ConnectedTo(&pTmpOut);
			if(pTmpOut) pTmpOut->Release();
			if(SUCCEEDED(hval)) continue; // has connected

			hval = m_Data.pFG->ConnectDirect(pOut, pIn, NULL);
			pIn->Release();
			if(SUCCEEDED(hval)) break; // connect one!!
		}
		
		pOut->Release();
		
		if(hval==E_FAIL) break; // no other can connect!!
	}
	
	SetNoClock(m_Data.pFG);

	if(pAudioTee) {
		pAudioTee->Release();
		pAudioTee = NULL;
	}

	if(pVideoTee) {
		pVideoTee->Release();
		pVideoTee = NULL;
	}

	if(pAudioNull) {
		pAudioNull->Release();
		pAudioNull = NULL;
	}

	if(pVideoNull) {
		pVideoNull->Release();
		pVideoNull = NULL;
	}
	
	return TRUE;
}

BOOL CMedia2WMX::MakeWMVWriterGraph(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfRefer)
{
	HRESULT hval=0;

    hval = CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pAsfMux);
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pAsfMux->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
	if(FAILED(hval)) {
		hval = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pWriter);
		if(FAILED(hval)) return FALSE;

		hval = m_Data.pWriter->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
		if(FAILED(hval)) return FALSE;
	}

	hval = m_Data.pFileSink->SetFileName(bstrOut, NULL);
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pFG->AddFilter(m_Data.pAsfMux, L"Mux");
	if(FAILED(hval)) return FALSE;

    // Set interleaving mode to FULL
    // !!! ASF won't support this, but that's okay
	hval = m_Data.pAsfMux->QueryInterface(IID_IConfigInterleaving, (void **)&m_Data.pConfigInterleaving);
	if(SUCCEEDED(hval)) {
		hval = m_Data.pConfigInterleaving->put_Mode(INTERLEAVE_FULL);
		// Do'nt Check Error Here!!
	}

    hval = m_Data.pAsfMux->QueryInterface(IID_IConfigAsfWriter, (void **)&m_Data.pConfigAsfWriter);
	if(FAILED(hval)) return FALSE;
	
	hval = m_Data.pConfigAsfWriter->ConfigureFilterUsingProfile(pwmpfRefer);
	if(FAILED(hval)) return FALSE;

	if(m_Data.pWriter!=NULL) {
		hval = m_Data.pFG->AddFilter(m_Data.pWriter, L"Writer");
		if(FAILED(hval)) return FALSE;

		IPin *pMuxOut=NULL, *pWriterIn=NULL;
		hval = FindPinOnFilter(m_Data.pAsfMux, PINDIR_OUTPUT, 0, FALSE, &pMuxOut);
		if(SUCCEEDED(hval)) {
			hval = FindPinOnFilter(m_Data.pWriter, PINDIR_INPUT, 0, FALSE, &pWriterIn);
			if(SUCCEEDED(hval)) {
				hval = m_Data.pFG->ConnectDirect(pMuxOut, pWriterIn, NULL);
				RELEASE(pWriterIn);
			}
			RELEASE(pMuxOut);
		}
		if(FAILED(hval)) return FALSE;
	}

	hval = m_Data.pConfigAsfWriter->SetIndexMode(FALSE);
	if(FAILED(hval)) return FALSE;	

	hval = m_Data.pFG->RenderFile(bstrIn, NULL);
	if(FAILED(hval)) return FALSE;	

	SetNoClock(m_Data.pFG);

	return TRUE;
}

BOOL CMedia2WMX::MakeWMAWriterGraph(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfRefer)
{
	HRESULT hval=0;

    hval = CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pAsfMux);
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pAsfMux->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
	if(FAILED(hval)) {
		hval = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_Data.pWriter);
		if(FAILED(hval)) return FALSE;

		hval = m_Data.pWriter->QueryInterface(IID_IFileSinkFilter, (void **)&m_Data.pFileSink);
		if(FAILED(hval)) return FALSE;
	}

	hval = m_Data.pFileSink->SetFileName(bstrOut, NULL);
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pFG->AddFilter(m_Data.pAsfMux, L"Mux");
	if(FAILED(hval)) return FALSE;

    // Set interleaving mode to FULL
    // !!! ASF won't support this, but that's okay
	hval = m_Data.pAsfMux->QueryInterface(IID_IConfigInterleaving, (void **)&m_Data.pConfigInterleaving);
	if(SUCCEEDED(hval)) {
		hval = m_Data.pConfigInterleaving->put_Mode(INTERLEAVE_FULL);
		// Do'nt Check Error Here!!
	}

    hval = m_Data.pAsfMux->QueryInterface(IID_IConfigAsfWriter, (void **)&m_Data.pConfigAsfWriter);
	if(FAILED(hval)) return FALSE;
	
	hval = m_Data.pConfigAsfWriter->ConfigureFilterUsingProfile(pwmpfRefer);
	if(FAILED(hval)) return FALSE;

	if(m_Data.pWriter!=NULL) {
		hval = m_Data.pFG->AddFilter(m_Data.pWriter, L"Writer");
		if(FAILED(hval)) return FALSE;

		IPin *pMuxOut=NULL, *pWriterIn=NULL;
		hval = FindPinOnFilter(m_Data.pAsfMux, PINDIR_OUTPUT, 0, FALSE, &pMuxOut);
		if(SUCCEEDED(hval)) {
			hval = FindPinOnFilter(m_Data.pWriter, PINDIR_INPUT, 0, FALSE, &pWriterIn);
			if(SUCCEEDED(hval)) {
				hval = m_Data.pFG->ConnectDirect(pMuxOut, pWriterIn, NULL);
				RELEASE(pWriterIn);
			}
			RELEASE(pMuxOut);
		}
		if(FAILED(hval)) return FALSE;
	}

	hval = m_Data.pConfigAsfWriter->SetIndexMode(FALSE);
	if(FAILED(hval)) return FALSE;	

	hval = m_Data.pFG->RenderFile(bstrIn, NULL);
	if(FAILED(hval)) return FALSE;

	IVideoWindow *pVW=NULL;
	hval = m_Data.pFG->QueryInterface(IID_IVideoWindow, (void**)&pVW);
	if(SUCCEEDED(hval)) { // Hide Video!!
		pVW->put_AutoShow(OAFALSE);
		pVW->put_Visible(OAFALSE);
		RELEASE(pVW);
	}

	SetNoClock(m_Data.pFG);

	return TRUE;
}

BOOL CMedia2WMX::ProcessGraphInfo(BOOL fMute)
{
	HRESULT hval=0;

    hval = m_Data.pFG->QueryInterface(IID_IMediaSeeking, (void **)&m_Data.pMS);
	if(FAILED(hval)) return FALSE;

	hval = m_Data.pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	if(SUCCEEDED(hval)) {
		hval = m_Data.pMS->GetDuration(&m_Data.l2TotalLength);
	}
	if(FAILED(hval)) return FALSE;

	if(fMute) {
		if(!AddMuteAudioToGraph(m_Data.pFG, m_Data.l2TotalLength, MUTE_SAMPLERATE, MUTE_BITSPERSAMPLE))
			return FALSE;
	}
	
    return TRUE;
}

#include "WaveFile.h"
BOOL CMedia2WMX::AddMuteAudioToGraph(IGraphBuilder *pFG, LONGLONG l2Length, DWORD dwSamplesPerSec, WORD wBitsPerSample)
{	
	WAVEFORMATEX	wfx;

	memset(&wfx, 0, sizeof(wfx));

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = dwSamplesPerSec;
	wfx.wBitsPerSample = wBitsPerSample;
	wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample)/8;
	wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

	UINT	uBlockSize=0, uSuccessSize=0;
	BYTE	*pBlock=(BYTE*)malloc(wfx.nAvgBytesPerSec);
	if(pBlock==NULL) return FALSE;
	memset(pBlock, 0, wfx.nAvgBytesPerSec);
	
	HRESULT			hval=0;
	CMyWaveFile		cwf;

	hval = cwf.Open("~tmp.wav", &wfx, WAVEFILE_WRITE);
	if(FAILED(hval)) return FALSE;

	hval = S_OK;
	for(LONGLONG ii=0; ii<l2Length; ii+=TIMEUNIT) {
		uBlockSize = wfx.nAvgBytesPerSec;
		if((ii+TIMEUNIT)>l2Length) {
			uBlockSize = (ULONG)(wfx.nAvgBytesPerSec * (l2Length-ii)/TIMEUNIT);
			uBlockSize += uBlockSize%wfx.nBlockAlign;			
		}
		hval = cwf.Write(uBlockSize, pBlock, &uSuccessSize);
		if(FAILED(hval)||(uBlockSize!=uSuccessSize)) {
			hval = E_FAIL;
			break;
		}
	}

	cwf.Close();
	free(pBlock);
	pBlock = NULL;

	m_Data.fAddAudioFile = TRUE;
	
	if(FAILED(hval)) return FALSE;

	hval = pFG->RenderFile(L"~tmp.wav", NULL);
	return SUCCEEDED(hval);
}

BOOL CMedia2WMX::GoConvert()
{	
	HRESULT hval=0;
	
	IMediaControl *pMC=NULL;
    hval = m_Data.pFG->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if(FAILED(hval)) return FALSE;

    IMediaEvent *pME=NULL;
	hval = m_Data.pFG->QueryInterface(IID_IMediaEvent, (void **) &pME);
	if(FAILED(hval)) {
		RELEASE(pMC);
		return FALSE;
	}

	hval = pMC->Run();
	if(FAILED(hval)) {
		RELEASE(pMC);
		RELEASE(pME);
		return FALSE;
	}
	
    LONG lEvCode = 0;
    do {
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
		
		LONGLONG llCurr=0;
		if(SUCCEEDED(m_Data.pMS->GetCurrentPosition(&llCurr))) {
			if(IsWindow(m_hWndProgress)) {
				SendMessage(m_hWndProgress, m_lMsgProgress, 0, MAKELONG(llCurr/TIMEUNIT, m_Data.l2TotalLength/TIMEUNIT));
			}	
		}
		if(IsWindow(m_hWndAbort)&&(m_plAbort!=NULL)&&((*m_plAbort)!=0)) {
			break;
		}

        hval = pME->WaitForCompletion(250, &lEvCode);

		if((E_ABORT!=hval)&&FAILED(hval)) 
			break;

    } while(lEvCode == 0);    
	
	hval = pMC->Stop();	
	RELEASE(pMC);
	RELEASE(pME);
	if(FAILED(hval)) return FALSE;

	if(lEvCode!=EC_COMPLETE) 
		return FALSE;

	return TRUE;
}

BOOL CMedia2WMX::BuildKeyProvider()
{
	m_pKeyProvider = new CKeyProvider();
	if(m_pKeyProvider==NULL) return FALSE;
	m_pKeyProvider->AddRef(); // Donot let COM try to free

	return TRUE;
}

void CMedia2WMX::FreeKeyProvider()
{
	RELEASE(m_pKeyProvider);
}

void CMedia2WMX::FreeFilterGraph()
{
	RELEASE(m_Data.pMS);

	RELEASE(m_Data.pAsfMux);
	RELEASE(m_Data.pWriter);	
	RELEASE(m_Data.pFileSink);
	RELEASE(m_Data.pConfigInterleaving);
	RELEASE(m_Data.pConfigAsfWriter);	

#ifdef _DEBUG
	if(m_Data.pFG!=NULL)
		RemoveFromRot(m_dwDebugRegister);
#endif

	RELEASE(m_Data.pFG);
}

void CMedia2WMX::FreeAll()
{
	FreeFilterGraph();

	if(m_Data.fAddAudioFile) DeleteFile("~tmp.wav");

	memset(&m_Data, 0, sizeof(m_Data));
}