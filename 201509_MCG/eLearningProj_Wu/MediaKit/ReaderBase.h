#ifndef _WMBASE_H_
#define _WMBASE_H_

#include <windef.h>

#undef TIMEUNIT
#define TIMEUNIT	10000000L
class CReaderBase: public IWMReaderCallback, public IWMReaderCallbackAdvanced
{
public:
	HWND	m_hWndProgress, m_hWndAbort;
	long	*m_pbAbort;
	long	m_lMsgProgress;
	BOOL	m_fAutoProgressEnabled;
public:
	BOOL		m_fEOF, m_fManualDeliver;
	QWORD		m_qwUserTime, m_qwStartTime;
	BOOL		m_fUserCheckStop;
	QWORD		m_qwUserForceStopTime;

	HANDLE		m_hEventAsync, m_hTimeAsync;
	HRESULT		m_hvalAsync;
public:
    IWMReader			*m_pReader;
    IWMReaderAdvanced	*m_pReaderAdvanced;
	IWMProfile			*m_pProfile;
	IWMHeaderInfo		*m_pHeaderInfo;

	BOOL		m_fVideoExisted, m_fScriptExisted;
	BOOL		m_fAudioCompressed, m_fVideoCompressed;
	WORD		m_wStreamAudio, m_wStreamVideo, m_wStreamScript;
	DWORD		m_dwOutputAudio, m_dwOutputVideo, m_dwOutputScript;
	QWORD		m_qwLength;	
	QWORD		m_qwStart, m_qwStop;
public:
	CReaderBase();
	~CReaderBase();

	BOOL Open(WCHAR *pwchWMX);
	QWORD GetTotalDeliverTime() { return m_qwUserTime; };
	BOOL SetUserClock(BOOL fUserProvided);
	BOOL SetSelectedStreams(BOOL fAudio, BOOL fVideo, BOOL fScript);
	BOOL ConfigAudioOutputCompressed();
	BOOL ConfigVideoOutputCompressed();
	BOOL ConfigScriptOutputCompressed();

	BOOL ConfigAudioOutputFormat(WM_MEDIA_TYPE *pwmt);
	BOOL ConfigVideoOutputFormat(WM_MEDIA_TYPE *pwmt);

	BOOL ConfigAudioOutputSimpleUncompressed(WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate);
	BOOL ConfigVideoOutputSimpleUncompressed(WORD wBitCount, DWORD biCompression);	
	BOOL ConfigVideoOutputSimpleUncompressedDim(DWORD dwWidth, DWORD dwHeight);	

	WM_MEDIA_TYPE *GetAudioOutputFormat();
	WM_MEDIA_TYPE *GetVideoOutputFormat();
	WM_MEDIA_TYPE *GetScriptOutputFormat();
	void FreeFormatAllocated(WM_MEDIA_TYPE *);

	BOOL Run_PlayMode_Start(QWORD qwStart, QWORD qwDuration, float dRate);
	BOOL Run_PlayMode_Resume();
	BOOL Run_PlayMode_Pause();	
	BOOL Run_PlayMode_Stop();

	virtual BOOL Run_AutoMode(QWORD qwStart, QWORD qwDuration);
	virtual BOOL Run_ManualMode(QWORD qwStart);
	virtual BOOL AutoDeliverMore(QWORD qwDuration);
	virtual BOOL ManualDeliverTo(QWORD qwTime);
	void Close();

	BOOL WaitForStop(HANDLE hEvent, DWORD dwmsWait);
	BOOL CopyAllHeaders(IWMHeaderInfo* pWriterHeaderInfo);
	BOOL CopyScript(IWMHeaderInfo* pWriterHeaderInfo);
	BOOL CopyMarker(BSTR bstrOutput);	
	void EnableAutoProgressReport(BOOL fEnabled) { m_fAutoProgressEnabled = fEnabled; };
public:
	BOOL SetupProgressHandler(HWND hwnd, long lMsg) { 
		m_hWndProgress = hwnd; 
		m_lMsgProgress = lMsg;
		return TRUE; 
	};	
	BOOL SetupQueryAbort(HWND hwnd, long *pbAbort) { 
		m_hWndAbort = hwnd; 
		m_pbAbort = pbAbort; 
		return TRUE; 
	};	
private:
	void ResetVariables();	
public:
	virtual HRESULT STDMETHODCALLTYPE OnStatus(
            /* [in] */ WMT_STATUS Status,
            /* [in] */ HRESULT hr,
            /* [in] */ WMT_ATTR_DATATYPE dwType,
            /* [in] */ BYTE __RPC_FAR *pValue,
			/* [in] */ void __RPC_FAR *pvContext);
	virtual HRESULT STDMETHODCALLTYPE OnTime( 
            /* [in] */ QWORD qwCurrentTime,
            /* [in] */ void __RPC_FAR *pvContext);
public:
	virtual HRESULT STDMETHODCALLTYPE OnSample( 
            /* [in] */ DWORD dwOutputNum,
            /* [in] */ QWORD qwSampleTime,
            /* [in] */ QWORD qwSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext) =0;
	virtual HRESULT STDMETHODCALLTYPE OnStreamSample( 
            /* [in] */ WORD wStreamNum,
            /* [in] */ QWORD cnsSampleTime,
            /* [in] */ QWORD cnsSampleDuration,
            /* [in] */ DWORD dwFlags,
            /* [in] */ INSSBuffer __RPC_FAR *pSample,
            /* [in] */ void __RPC_FAR *pvContext) =0;
public:
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

#endif