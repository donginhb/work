#include <stdio.h>
#include <wmsdkidl.h>
#include "HelperKit.h"
#include "WriterBase.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))

BOOL RetrieveProfileStreamInfo(IWMProfile *pProfile, WORD *pwStreamAudio, WORD *pwStreamVideo, WORD *pwStreamScript, BOOL *pfVideoExisted, BOOL *pfScriptExisted);

CWriterBase::CWriterBase()
{
	m_pWriter = NULL;
	m_pWriterAdvanced = NULL;
	m_pHeaderInfo = NULL;

	m_fVideoExisted = FALSE;
	m_fScriptExisted = FALSE;

	m_dwInputAudio = 0;
	m_dwInputVideo = 0;
	m_dwInputScript = 0;

	m_wStreamAudio = 0;
	m_wStreamVideo = 0;
	m_wStreamScript = 0;
}

CWriterBase::~CWriterBase()
{
}

BOOL RetrieveInputNumber(IWMWriter *pWriter, DWORD *pdwInputAudio, DWORD *pdwInputVideo, DWORD *pdwInputScript, BOOL *pfVideoExisted, BOOL *pfScriptExisted)
{
	if(pWriter==NULL)
		return FALSE;

	DWORD dwCount=0;
	HRESULT hr = pWriter->GetInputCount(&dwCount);
	if(FAILED(hr)) return FALSE;

	GUID	guid=GUID_NULL;
	IWMInputMediaProps *pProps=NULL;
	for(DWORD ii=0; ii<dwCount; ii++) {
		hr = pWriter->GetInputProps(ii, &pProps);
		if(FAILED(hr)) break;

		hr = pProps->GetType(&guid);
		if(FAILED(hr)) break;

		RELEASE(pProps);

		if(guid==WMMEDIATYPE_Audio) {
			if(pdwInputAudio!=NULL)
				*pdwInputAudio = ii;
		}
		else if(guid==WMMEDIATYPE_Video) {
			if(pfVideoExisted!=NULL)
				*pfVideoExisted = TRUE;
			if(pdwInputVideo!=NULL)
				*pdwInputVideo = ii;
		}
		else if(guid==WMMEDIATYPE_Script) {
			if(pfScriptExisted!=NULL)
				*pfScriptExisted = TRUE;
			if(pdwInputScript!=NULL)
				*pdwInputScript = ii;
		}		
	}
	RELEASE(pProps);

	return TRUE;
}


BOOL CWriterBase::Create(WCHAR *pwchFile, BSTR bstrProfile)
{
	IWMProfileManager	*pWMPM0=NULL;
	IWMProfileManager2	*pWMPM=NULL;

	HRESULT hr = WMCreateProfileManager(&pWMPM0);
	if(FAILED(hr)) return FALSE;

	hr = pWMPM0->QueryInterface(IID_IWMProfileManager2, (void**)&pWMPM);
	RELEASE(pWMPM0);
	if(FAILED(hr)) return FALSE;

	IWMProfile *pProfile = PF_LoadFromPRX(pWMPM, bstrProfile);
	RELEASE(pWMPM);
	if(pProfile==NULL) return FALSE;

	BOOL fOK = Create(pwchFile, pProfile);
	RELEASE(pProfile);

	return fOK;
}

BOOL CWriterBase::Create(WCHAR *pwchFile, IWMProfile *pProfile)
{	
	HRESULT hr = WMCreateWriter( NULL, &m_pWriter );
	if ( FAILED( hr ) ) return FALSE;

	BOOL fOK=FALSE;

	hr = m_pWriter->QueryInterface( IID_IWMWriterAdvanced, ( VOID ** )&m_pWriterAdvanced );
	if ( FAILED( hr ) ) goto ERROR_WRITE;

	hr = m_pWriter->QueryInterface( IID_IWMHeaderInfo, ( VOID ** )&m_pHeaderInfo );
	if ( FAILED( hr ) ) goto ERROR_WRITE;

	hr = m_pWriter->SetProfile( pProfile );
	if ( FAILED( hr ) ) goto ERROR_WRITE;

	hr = m_pWriter->SetOutputFilename( pwchFile );
	if ( FAILED( hr ) ) goto ERROR_WRITE;

	if(!RetrieveProfileStreamInfo(pProfile, &m_wStreamAudio, &m_wStreamVideo, &m_wStreamScript, &m_fVideoExisted, &m_fScriptExisted))
		goto ERROR_WRITE;

	if(!RetrieveInputNumber(m_pWriter, &m_dwInputAudio, &m_dwInputVideo, &m_dwInputScript, NULL, NULL))
		goto ERROR_WRITE;

	fOK = TRUE;
ERROR_WRITE:
	if(!fOK) {
		Close();
		return FALSE;
	}

	return fOK;
}


WM_MEDIA_TYPE *GetInputFormat(IWMWriter *pWriter, DWORD dwInput)
{
	if(pWriter==NULL) return NULL;

	WM_MEDIA_TYPE *pwmftmp=NULL;

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = pWriter->GetInputProps(dwInput, &pProps);
	if(FAILED(hr)) return NULL;

	if(pProps==NULL) return NULL;

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

WM_MEDIA_TYPE *CWriterBase::GetAudioInputFormat()
{
	return GetInputFormat(m_pWriter, m_dwInputAudio);
}

WM_MEDIA_TYPE *CWriterBase::GetVideoInputFormat()
{
	if(!m_fVideoExisted)
		return NULL;

	return GetInputFormat(m_pWriter, m_dwInputVideo);
}

WM_MEDIA_TYPE *CWriterBase::GetScriptInputFormat()
{
	if(!m_fScriptExisted)
		return NULL;

	return GetInputFormat(m_pWriter, m_dwInputScript);
}

void CWriterBase::FreeFormatAllocated(WM_MEDIA_TYPE *pwmt)
{
	if(pwmt!=NULL)
		free(pwmt);
}

BOOL CWriterBase::ConfigAudioInputCodecNoNeeded()
{
	if(m_pWriter==NULL)
		return FALSE;

	return SUCCEEDED(m_pWriter->SetInputProps( m_dwInputAudio, NULL ));
}

BOOL CWriterBase::ConfigVideoInputCodecNoNeeded()
{
	if(m_pWriter==NULL)
		return FALSE;

	if(m_fVideoExisted) 
		return SUCCEEDED(m_pWriter->SetInputProps( m_dwInputVideo, NULL ));

	return TRUE;
}

BOOL CWriterBase::ConfigScriptInputCodecNoNeeded()
{
	if(m_pWriter==NULL)
		return FALSE;

	if(m_fScriptExisted) 
		return SUCCEEDED(m_pWriter->SetInputProps( m_dwInputScript, NULL ));

	return TRUE;
}

BOOL CWriterBase::ConfigAudioInputFormat(WM_MEDIA_TYPE *pwmt)
{
	if(pwmt==NULL) return ConfigAudioInputCodecNoNeeded();

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = m_pWriter->GetInputProps(m_dwInputAudio, &pProps);
	if(FAILED(hr)) return FALSE;
	hr = pProps->SetMediaType(pwmt);
	RELEASE(pProps);
	if(FAILED(hr)) return FALSE;	

	return TRUE;
}

BOOL CWriterBase::ConfigVideoInputFormat(WM_MEDIA_TYPE *pwmt)
{
	if(pwmt==NULL) return ConfigVideoInputCodecNoNeeded();

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = m_pWriter->GetInputProps(m_dwInputVideo, &pProps);
	if(FAILED(hr)) return FALSE;
	hr = pProps->SetMediaType(pwmt);
	RELEASE(pProps);
	if(FAILED(hr)) return FALSE;

	return TRUE;
}

BOOL CWriterBase::ConfigAudioInputSimpleUncompressed(WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate)
{
	if(m_pWriter==NULL)
		return FALSE;

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = m_pWriter->GetInputProps(m_dwInputAudio, &pProps);
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
				pwih->cbSize = 0; //Extended Data

				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pWriter->SetInputProps(m_dwInputAudio, pProps);
				}
			}
			free(pwmftmp);
		}		
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

BOOL CWriterBase::ConfigVideoInputSimpleUncompressed(WORD wBitCount, DWORD biCompression)
{
	if(m_pWriter==NULL)
		return FALSE;

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = m_pWriter->GetInputProps(m_dwInputVideo, &pProps);
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
				pvih->bmiHeader.biSizeImage = DIBSIZE(pvih->bmiHeader);
				pvih->bmiHeader.biCompression = biCompression;

				pwmftmp->bFixedSizeSamples = TRUE;
				pwmftmp->bTemporalCompression = FALSE;
				pwmftmp->pUnk = NULL;
				pwmftmp->lSampleSize = pvih->bmiHeader.biSizeImage;

				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pWriter->SetInputProps(m_dwInputVideo, pProps);
				}
			}
			free(pwmftmp);
		}
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

BOOL CWriterBase::ConfigVideoInputBitmapFormat(BITMAPINFOHEADER *pbih)
{
	if(m_pWriter==NULL)
		return FALSE;

	IWMInputMediaProps *pProps=NULL;
	HRESULT hr = m_pWriter->GetInputProps(m_dwInputVideo, &pProps);
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
				memcpy(&pvih->bmiHeader, pbih, sizeof(BITMAPINFOHEADER));

				pwmftmp->bFixedSizeSamples = TRUE;
				pwmftmp->bTemporalCompression = FALSE;
				pwmftmp->pUnk = NULL;
				pwmftmp->lSampleSize = pbih->biSizeImage;

				hr = pProps->SetMediaType(pwmftmp);
				if(SUCCEEDED(hr)) {
					hr = m_pWriter->SetInputProps(m_dwInputVideo, pProps);
				}
			}
			free(pwmftmp);
		}
	}
	RELEASE(pProps);

	return SUCCEEDED(hr);
}

BOOL CWriterBase::BeginWriting()
{
	if(m_pWriter==NULL)
		return FALSE;

	return SUCCEEDED(m_pWriter->BeginWriting());
}

BOOL CWriterBase::EndWriting()
{
	if(m_pWriter==NULL)
		return FALSE;

	return SUCCEEDED(m_pWriter->EndWriting());
}

void CWriterBase::Close()
{
	EndWriting();
	
	RELEASE(m_pHeaderInfo);
	RELEASE(m_pWriterAdvanced);
//	RELEASE(m_pWriter);
}