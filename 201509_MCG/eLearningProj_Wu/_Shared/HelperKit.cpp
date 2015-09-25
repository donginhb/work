#include <afxwin.h>

#include <wmsdk.h>
#include <dshow.h>
#include <dvdmedia.h>

#include "HelperProfile.h"
#include "HelperKit.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#undef TIMEUNIT
#define TIMEUNIT		(LONGLONG)10000000

BOOL PF_GetStreamCount(IWMProfile *pwmpf, WORD &wAudio, WORD &wVideo, WORD &wScript)
{
	HRESULT hval=0;
	DWORD	dwCount=0;

	wAudio = 0; wVideo = 0; wScript = 0;
	hval = pwmpf->GetStreamCount(&dwCount);
	if(FAILED(hval)) return FALSE;

	IWMStreamConfig *pscf=NULL;
	for(DWORD ii=0; ii<dwCount; ii++) {
		hval = pwmpf->GetStream(ii, &pscf);
		if(FAILED(hval)) return FALSE;

		GUID guid=GUID_NULL;
		hval = pscf->GetStreamType(&guid);
		RELEASE(pscf);
		if(FAILED(hval)) return FALSE;

		if(guid==WMMEDIATYPE_Audio) wAudio++;
		else if(guid==WMMEDIATYPE_Video) wVideo++;
		else if(guid==WMMEDIATYPE_Script) wScript++;		
	}

	return TRUE;
}

IWMProfile *PF_LoadFromPRX(IWMProfileManager2 *pwmpm, BSTR bstrPrx)
{
	if(pwmpm==NULL) return NULL;

	CString		cstFile(bstrPrx);
	CFile		cf;

	if(!cf.Open(cstFile, CFile::modeRead)) return NULL;

	long lCount = cf.GetLength();

	CString		cstData;
	LPSTR pstr = cstData.GetBufferSetLength(lCount+2);
	memset(pstr, 0, lCount + 2);
	
	DWORD dwCount = cf.ReadHuge(pstr, lCount);
	cf.Close();

	if(dwCount!=(DWORD)lCount) return NULL;
	
	IWMProfile *pProfile=NULL;
	HRESULT hval = pwmpm->LoadProfileByData((WCHAR*)pstr, &pProfile);
	if(FAILED(hval)) return FALSE;

	return pProfile;

}

IWMProfile *PF_Subtract(IWMProfileManager2 *pwmpm, IWMProfile *pwmpf, BOOL fVideo, BOOL fScript)
{
	if(pwmpm==NULL) return NULL;
	if(pwmpf==NULL)  return NULL;

	HRESULT		hval=0;
	IWMProfile	*pwmpfOut=NULL;

	WMT_VERSION	dwVersion;
	hval = pwmpf->GetVersion(&dwVersion);
	if(FAILED(hval)) return NULL;

	DWORD	dwStreamCount=0;
	hval = pwmpf->GetStreamCount(&dwStreamCount);
	if(FAILED(hval)) return NULL;

	hval = pwmpm->CreateEmptyProfile(dwVersion, &pwmpfOut);
	if(FAILED(hval)) return NULL;

	hval = pwmpfOut->SetName(L"CustomProfile");
	if(FAILED(hval)) {
		RELEASE(pwmpfOut);
		return NULL;
	}

	GUID	guidType=GUID_NULL;
	WORD	wStreamNum=0;
	IWMStreamConfig	*pwmsc=NULL;
	for(DWORD ii=0; ii<dwStreamCount; ii++) {
		hval = pwmpf->GetStream(ii, &pwmsc);
		if(SUCCEEDED(hval)) {
			hval = pwmsc->GetStreamType(&guidType);
			if(SUCCEEDED(hval)) {				
				if((guidType==WMMEDIATYPE_Audio)||
				   (fVideo&&(guidType==WMMEDIATYPE_Video))||
				   (fScript&&(guidType==WMMEDIATYPE_Script))) {
					hval = pwmpfOut->AddStream(pwmsc);
				}
			}
			RELEASE(pwmsc);
			if(FAILED(hval)) {
				RELEASE(pwmpfOut);
				return NULL;
			}
		}
	}

	return pwmpfOut;
}

HRESULT PF_SavePRX(IWMProfileManager2 *pWMPM, IWMProfile *pProfile, WCHAR *pwchFile)
{
	HRESULT hr=0;
	DWORD dwLen=0;
	hr = pWMPM->SaveProfile(pProfile, NULL, &dwLen);
	if(SUCCEEDED(hr)) {
		BYTE *pBuffer = (BYTE*)malloc(dwLen*2); // WCHAR is 2 * char!!!
		hr = E_FAIL;
		if(pBuffer!=NULL) {
			hr = pWMPM->SaveProfile(pProfile, (WCHAR*)pBuffer, &dwLen);
			if(SUCCEEDED(hr)) {
				CString	cst(pwchFile);
				CFile	cf;
				if(cf.Open((LPCSTR)cst, CFile::modeCreate|CFile::modeWrite)) {
					try {
						cf.WriteHuge(pBuffer, dwLen*2);
					} 
					catch(CFileException* theException) {
						theException->Delete();
						hr = E_FAIL;						
					}
					cf.Close();
				}
			}
			free(pBuffer); pBuffer = NULL;
		}
	}

	return hr;
}

IWMProfile *PF_Lower(IWMProfileManager2 *pWMPM, IWMProfile *pProfile, BOOL fAudio, DWORD dwAudioSampleRate, DWORD dwAudioChannels, DWORD dwAudioBitrate, BOOL fVideo, DWORD dwVideoSecPerKey, DWORD dwVideoQuality, WORD wFPS, DWORD dwVideoBitrate, BOOL fScript, DWORD dwScriptBitrate)
{
	AUDIO_PARAMS apm;
	VIDEO_PARAMS vpm;
	SCRIPT_PARAMS spm;

	memset(&apm, 0, sizeof(apm));
	memset(&vpm, 0, sizeof(vpm));
	memset(&spm, 0, sizeof(spm));

	if(!PF_GetParameter(pProfile, apm, vpm, spm))
		return NULL;
	
	if(fAudio) {
		if(apm.dwBitrate>dwAudioBitrate) apm.dwBitrate = dwAudioBitrate;
		apm.dwSampleRate = dwAudioSampleRate;
		apm.dwChannels = dwAudioChannels;
		PF_CorrectAPM(apm);
	}

	if(fVideo&&(vpm.dwBitrate>0)) { // Exist Video Stream!!
		if(vpm.dwBitrate>dwVideoBitrate) vpm.dwBitrate = dwVideoBitrate;
		if((0<wFPS)&&(wFPS<=30)) vpm.dwFPS = wFPS;
		vpm.dwSecPerKey = dwVideoSecPerKey;
		vpm.dwQuality = dwVideoQuality;
		PF_CorrectVPM(vpm);
	}

	if(fScript&&(spm.dwBitrate>0)) {
		if(spm.dwBitrate>dwScriptBitrate) spm.dwBitrate = dwScriptBitrate;
		PF_CorrectSPM(spm);
	}

	return PF_Generate(pWMPM, &apm, (fScript&&(spm.dwBitrate>0))?(&spm):NULL, (fVideo&&(vpm.dwBitrate>0))?1:0, (fVideo&&(vpm.dwBitrate>0))?(&vpm):NULL);
}

/*
IWMProfile *PF_Change(IWMProfileManager2 *pWMPM, IWMProfile *pProfile, BOOL fAudio, DWORD dwAudioBitrate, BOOL fVideo, WORD wFPS, DWORD dwVideoBitrate, BOOL fScript, DWORD dwScriptBitrate)
{
	AUDIO_PARAMS apm;
	VIDEO_PARAMS vpm;
	SCRIPT_PARAMS spm;

	memset(&apm, 0, sizeof(apm));
	memset(&vpm, 0, sizeof(vpm));
	memset(&spm, 0, sizeof(spm));

	if(!PF_GetParameter(pProfile, apm, vpm, spm))
		return NULL;
	
	if(fAudio) {
		apm.dwBitrate = dwAudioBitrate;
		PF_CorrectAPM(apm);
	}

	if(fVideo&&(vpm.dwBitrate>0)) { // Exist Video Stream!!
		vpm.dwBitrate = dwVideoBitrate;
		if((0<wFPS)&&(wFPS<=30)) vpm.dwFPS = wFPS; else vpm.dwFPS = 10; // default!!
		vpm.dwSecPerKey = 1;
		PF_CorrectVPM(vpm);
	}

	if(fScript&&(spm.dwBitrate>0)) {
		spm.dwBitrate = dwScriptBitrate;
		PF_CorrectSPM(spm);
	}

	return PF_Generate(pWMPM, &apm, (fScript&&(spm.dwBitrate>0))?(&spm):NULL, (fVideo&&(vpm.dwBitrate>0))?1:0, (fVideo&&(vpm.dwBitrate>0))?(&vpm):NULL);
}
*/

IWMProfile *PF_Generate(IWMProfileManager2 *pWMPM, 
							  AUDIO_PARAMS *pAudioParams,
                              SCRIPT_PARAMS *pScriptParams,
                              WORD cVideoStreams,
                              VIDEO_PARAMS *prgVideoParams
                              )
{
    if( ( NULL == pAudioParams ) ||         
        ( cVideoStreams > WM_MAX_VIDEO_STREAMS ) ||
        ( cVideoStreams < 0 ) ||
        ( (0 < cVideoStreams ) && (NULL == prgVideoParams ) ) )
    {
        return NULL;
    }

    HRESULT		hr = S_OK;    
	IWMProfile	*pwmpf=NULL;

    do
    {
        hr = pWMPM->CreateEmptyProfile( WMT_VER_7_0, &pwmpf );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pwmpf->SetName( L"Custom Profile" );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pwmpf->SetDescription( L"Custom Profile Description" );
        if( FAILED( hr ) )
        {
            break;
        }

        DWORD dwFlags = 0;
        if( cVideoStreams )
        {
            dwFlags |= AUDIO_FLAGS_AUDIOVIDEO;
        }

        hr = AddAudioStream( pWMPM,
							 pwmpf,
							 pAudioParams->dwFormatTag, 
                             pAudioParams->dwBitrate, 
                             pAudioParams->dwSampleRate, 
                             pAudioParams->dwChannels, 
                             dwFlags                             
                              );
        if( FAILED( hr ) )
        {
            break;
        }

        if( cVideoStreams > 0)
        {
            for( WORD i = 0; ( i < cVideoStreams ) && SUCCEEDED( hr ); i++ )
            {
                hr = AddVideoStream( pWMPM, 
                                     pwmpf,
                                     BASE_STREAM_NUMBER_VIDEO + i,
                                     prgVideoParams[i].dwFourCC,
                                     prgVideoParams[i].dwBitrate,
                                     prgVideoParams[i].dwWidth,
                                     prgVideoParams[i].dwHeight,
                                     prgVideoParams[i].dwFPS,
                                     prgVideoParams[i].dwQuality,
                                     prgVideoParams[i].dwSecPerKey );
            }
            if( FAILED( hr ) )
            {
                break;
            }

            if( cVideoStreams > 1 )
            {
                hr = AddMutexObject( pwmpf, cVideoStreams );
                if( FAILED( hr ) )
                {
                    break;
                }
            }
        }

        if( pScriptParams )
        {
            hr = AddScriptStream( pwmpf, 
                                  BASE_STREAM_NUMBER_VIDEO + cVideoStreams,
                                  pScriptParams->dwBitrate );
            if( FAILED( hr ) )
            {
                break;
            }
        }
    }
    while( FALSE );

    if( FAILED( hr ) )
    {
        RELEASE( pwmpf );
    }
    
    return pwmpf;
}

BOOL PF_GetVideoPropsParameter(IWMMediaProps *pmp, VIDEO_PARAMS &VideoParams)
{
/* Programs Are NO USED for Checking!! SEE MANUAL!!	
	memset(&VideoParams, 0, sizeof(VideoParams));

	IWMVideoMediaProps *pvmp=NULL;
	HRESULT hval = pmp->QueryInterface(IID_IWMVideoMediaProps, (void**)&pvmp);
	if(FAILED(hval)) return FALSE;

	hval = pvmp->GetQuality(&VideoParams.dwQuality);
	if(SUCCEEDED(hval)) {
		LONGLONG	llTime=0;
		hval = pvmp->GetMaxKeyFrameSpacing(&llTime);
		if(SUCCEEDED(hval)) {
			ASSERT(llTime==0); // See The Document!!
			VideoParams.dwSecPerKey = 1; // Deafult!!
		}
	}

	RELEASE(pvmp);
	if(FAILED(hval)) return FALSE;
*/
	DWORD			dwSize=0;
	WM_MEDIA_TYPE	*pmtype=NULL;
	HRESULT hval = pmp->GetMediaType(NULL, &dwSize);
	if(SUCCEEDED(hval)) {
		pmtype = (WM_MEDIA_TYPE	*)malloc(dwSize);		
		if(pmtype!=NULL) {
			hval = pmp->GetMediaType(pmtype, &dwSize);
			if(SUCCEEDED(hval)) {
				if(!PF_GetVideoStreamParameter(*(WM_MEDIA_TYPE*)pmtype, VideoParams))
					hval = E_FAIL;
			}
			free(pmtype);
		}
	}
	
	return SUCCEEDED(hval);
}

BOOL PF_GetAudioPropsParameter(IWMMediaProps *pmp, AUDIO_PARAMS &AudioParams)
{
	memset(&AudioParams, 0, sizeof(AudioParams));

	DWORD			dwSize=0;
	WM_MEDIA_TYPE	*pmtype=NULL;
	HRESULT hval = pmp->GetMediaType(NULL, &dwSize);
	if(SUCCEEDED(hval)) {
		pmtype = (WM_MEDIA_TYPE	*)malloc(dwSize);		
		if(pmtype!=NULL) {
			hval = pmp->GetMediaType(pmtype, &dwSize);
			if(SUCCEEDED(hval)) {
				if(!PF_GetAudioStreamParameter(*(WM_MEDIA_TYPE*)pmtype, AudioParams))
					hval = E_FAIL;
			}
			free(pmtype);
		}
	}
	
	return TRUE;
}

BOOL PF_GetParameter(IWMProfile *pwmpf, AUDIO_PARAMS &AudioParams, VIDEO_PARAMS &VideoParams, SCRIPT_PARAMS &ScriptParams)
{
	memset(&AudioParams, 0, sizeof(AudioParams));
	memset(&VideoParams, 0, sizeof(VideoParams));
	memset(&ScriptParams, 0, sizeof(ScriptParams));

	HRESULT hval=0;
	DWORD	dwCount=0;

	hval = pwmpf->GetStreamCount(&dwCount);
	if(FAILED(hval)||(dwCount<1)) return FALSE;

	DWORD	dwSize=0;	
	IWMStreamConfig *pscf=NULL;
	IWMMediaProps	*pmp=NULL;
	GUID			guid=GUID_NULL;

	for(DWORD ii=0; ii<dwCount; ii++) {
		hval = pwmpf->GetStream(ii, &pscf);
		if(FAILED(hval)) return FALSE;

		hval = pscf->GetStreamType(&guid);
		if(FAILED(hval)) {
			RELEASE(pscf);
			return FALSE;
		}

		DWORD	dwBitRate=0;
		hval = pscf->GetBitrate(&dwBitRate);
		if(FAILED(hval)) {
			RELEASE(pscf);
			return FALSE;
		}

		if(guid==WMMEDIATYPE_Script) {
			ScriptParams.dwBitrate = dwBitRate;
		}
		else {
			hval = pscf->QueryInterface(IID_IWMMediaProps, (void**)&pmp);
			RELEASE(pscf);
			if(FAILED(hval)) return FALSE;

			if(guid==WMMEDIATYPE_Video) {
				VideoParams.dwBitrate = dwBitRate;
				if(!PF_GetVideoPropsParameter(pmp, VideoParams)) {
					RELEASE(pmp);
					return FALSE;
				}
			}
			else if(guid==WMMEDIATYPE_Audio) {
				AudioParams.dwBitrate = dwBitRate;
				if(!PF_GetAudioPropsParameter(pmp, AudioParams)) {
					RELEASE(pmp);
					return FALSE;
				}
			}
			RELEASE(pmp);
		}		
	}

	return TRUE;
}

void PF_CorrectAPM(AUDIO_PARAMS &apm)
{
	if(apm.dwChannels<1) apm.dwChannels = 1;
	if(apm.dwChannels>2) apm.dwChannels = 2;

	if(apm.dwFormatTag==CODEC_AUDIO_ACELP) {
		apm.dwChannels = 1;
		apm.dwSampleRate = 8000;
		if(apm.dwBitrate<=BPS_AUDIO_5K+250) apm.dwBitrate = BPS_AUDIO_5K;
		else if(apm.dwBitrate<=BPS_AUDIO_6_5K+250) apm.dwBitrate = BPS_AUDIO_6_5K;
		else if(apm.dwBitrate<=BPS_AUDIO_8_5K+250) apm.dwBitrate = BPS_AUDIO_8_5K;
		else {
			apm.dwSampleRate = 16000;
			apm.dwBitrate = BPS_AUDIO_16K;
		}
	}
	else if(apm.dwFormatTag==CODEC_AUDIO_MSAUDIO) {
		apm.dwFormatTag = CODEC_AUDIO_MSAUDIO;
		if(apm.dwBitrate<=BPS_AUDIO_5K+250) {
			apm.dwBitrate = BPS_AUDIO_5K;
			apm.dwChannels = 1;
			apm.dwSampleRate = 8000;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_6K+250) {
			apm.dwBitrate = BPS_AUDIO_6K;
			apm.dwChannels = 1;
			apm.dwSampleRate = 8000;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_8K+250) {
			apm.dwBitrate = BPS_AUDIO_8K;
			apm.dwChannels = 1;
			if(apm.dwSampleRate<=8000) apm.dwSampleRate = 8000;
			else apm.dwSampleRate = 11025;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_10K+250) {
			apm.dwBitrate = BPS_AUDIO_10K;
			apm.dwChannels = 1;
			if(apm.dwSampleRate<=11025) apm.dwSampleRate = 11025;
			else apm.dwSampleRate = 16000;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_12K+250) {
			apm.dwBitrate = BPS_AUDIO_12K;
			if(apm.dwSampleRate<=8000) {
				apm.dwChannels = 2;
				apm.dwSampleRate = 8000;
			}
			else {
				apm.dwChannels = 1;
				apm.dwSampleRate = 16000;
			}
		}
		else if(apm.dwBitrate<=BPS_AUDIO_16K+250) {
			apm.dwBitrate = BPS_AUDIO_16K;
			if(apm.dwSampleRate<=16000) apm.dwSampleRate = 16000;
			else {
				apm.dwChannels = 1;
				apm.dwSampleRate = 22050;
			}
		}
		else if(apm.dwBitrate<=BPS_AUDIO_20K+250) {
			apm.dwBitrate = BPS_AUDIO_20K;
			if(apm.dwSampleRate<=16000) {
				apm.dwChannels = 2;
				apm.dwSampleRate = 16000;
			}
			else if(apm.dwSampleRate<=22050) 
				apm.dwSampleRate = 22050;
			else {
				apm.dwChannels = 1;
				apm.dwSampleRate = 32000;
			}
		}
		else if(apm.dwBitrate<=BPS_AUDIO_22K+250) {
			apm.dwBitrate = BPS_AUDIO_22K;
			apm.dwChannels = 2;
			apm.dwSampleRate = 22050;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_32K+250) {
			apm.dwBitrate = BPS_AUDIO_32K;
			if(apm.dwSampleRate<=22050) {
				apm.dwChannels = 2;
				apm.dwSampleRate = 22050;
			}
			else if(apm.dwSampleRate<=32000) {
				apm.dwChannels = 2;
				apm.dwSampleRate = 32000;
			}
			else apm.dwSampleRate = 44100;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_40K+250) {
			apm.dwBitrate = BPS_AUDIO_40K;
			apm.dwChannels = 2;
			apm.dwSampleRate = 32000;
		}
		else if(apm.dwBitrate<=BPS_AUDIO_48K+250) {
			apm.dwBitrate = BPS_AUDIO_48K;
			if(apm.dwSampleRate<=32000) {
				apm.dwChannels = 2;
				apm.dwSampleRate = 32000;
			}
			else apm.dwSampleRate = 44100;
		}
		else {
			apm.dwChannels = 2;
			apm.dwSampleRate = 44100;
			if(apm.dwBitrate<=BPS_AUDIO_64K+250) apm.dwBitrate = BPS_AUDIO_64K;
			else if(apm.dwBitrate<=BPS_AUDIO_80K+250) apm.dwBitrate = BPS_AUDIO_80K;
			else if(apm.dwBitrate<=BPS_AUDIO_96K+250) apm.dwBitrate = BPS_AUDIO_96K;
			else if(apm.dwBitrate<=BPS_AUDIO_128K+250) apm.dwBitrate = BPS_AUDIO_128K;
			else if(apm.dwBitrate<=BPS_AUDIO_160K+250) apm.dwBitrate = BPS_AUDIO_160K;
			else apm.dwBitrate = BPS_AUDIO_192K;
		}
	}
}

void PF_CorrectVPM(VIDEO_PARAMS &vpm)
{
	if(vpm.dwFPS<1) vpm.dwFPS = 10;	// At Least	= 15!!
	vpm.dwWidth += vpm.dwWidth&1; // Must Be Even!!
	vpm.dwHeight += vpm.dwHeight&1; // Must Be Even!!
	if(vpm.dwWidth!=0) {
		if(vpm.dwWidth<32) vpm.dwWidth = 32;
		if(vpm.dwWidth>1600) vpm.dwWidth = 1600;
	}
	if(vpm.dwHeight!=0) {
		if(vpm.dwHeight<32) vpm.dwHeight = 32;
		if(vpm.dwHeight>1600) vpm.dwHeight = 1600;
	}
	if(vpm.dwBitrate<10240) vpm.dwBitrate = 10240; // Minimize Requirement!!
	if(vpm.dwSecPerKey<=0) vpm.dwSecPerKey = 1;
	if(vpm.dwQuality>100) vpm.dwQuality = 100;
}

void PF_CorrectSPM(SCRIPT_PARAMS &spm)
{
	if(spm.dwBitrate<1000) spm.dwBitrate = 1000;
	if(spm.dwBitrate>9999) spm.dwBitrate = 9999;
}

BOOL PF_GetAudioStreamParameter(WM_MEDIA_TYPE &amt, AUDIO_PARAMS &apm)
{
	if(amt.majortype !=WMMEDIATYPE_Audio) return FALSE;
	if(amt.formattype!= WMFORMAT_WaveFormatEx) return FALSE;

	memset(&apm, 0, sizeof(apm));

	WAVEFORMATEX *pwinfo = (WAVEFORMATEX*)amt.pbFormat;
	apm.dwFormatTag = CODEC_AUDIO_MSAUDIO; // Our Default!!
	if(pwinfo->wFormatTag==CODEC_AUDIO_ACELP)
		apm.dwFormatTag = CODEC_AUDIO_ACELP;
	apm.dwBitrate = pwinfo->nAvgBytesPerSec * 8;	
	apm.dwSampleRate = pwinfo->nSamplesPerSec;
	apm.dwChannels = pwinfo->nChannels;

	return TRUE;
}

BOOL PF_GetVideoStreamParameter(WM_MEDIA_TYPE &amt, VIDEO_PARAMS &vpm)
{
	if(amt.majortype !=WMMEDIATYPE_Video) return FALSE;

	memset(&vpm, 0, sizeof(vpm));

	vpm.dwFourCC = CODEC_VIDEO_CAMERACAP; // Our Default!!
	if(amt.formattype==WMFORMAT_VideoInfo) {					
		VIDEOINFOHEADER *pmi=(VIDEOINFOHEADER *)amt.pbFormat;
		if(pmi==NULL) return FALSE;		
		
		vpm.dwWidth = pmi->bmiHeader.biWidth;
		vpm.dwHeight = pmi->bmiHeader.biHeight;						
		vpm.dwBitrate = pmi->dwBitRate;
		if(pmi->AvgTimePerFrame>0) vpm.dwFPS = (DWORD)(TIMEUNIT/pmi->AvgTimePerFrame);
		if(pmi->bmiHeader.biCompression==CODEC_VIDEO_SCREENCAP)
			vpm.dwFourCC = CODEC_VIDEO_SCREENCAP;
	}
	else if(amt.formattype==FORMAT_VideoInfo2) {
		VIDEOINFOHEADER2 *pmi=(VIDEOINFOHEADER2 *)amt.pbFormat;
		if(pmi==NULL) return FALSE;

		vpm.dwWidth = pmi->bmiHeader.biWidth;
		vpm.dwHeight = pmi->bmiHeader.biHeight;						
		vpm.dwBitrate = pmi->dwBitRate;
		if(pmi->AvgTimePerFrame>0) vpm.dwFPS = (DWORD)(TIMEUNIT/pmi->AvgTimePerFrame);
		if(pmi->bmiHeader.biCompression==CODEC_VIDEO_SCREENCAP)
			vpm.dwFourCC = CODEC_VIDEO_SCREENCAP;
	}
	else if(amt.formattype==FORMAT_MPEGVideo) {
		MPEG1VIDEOINFO *pmi=(MPEG1VIDEOINFO *)amt.pbFormat;
		if(pmi==NULL) return FALSE;

		vpm.dwWidth = pmi->hdr.bmiHeader.biWidth;
		vpm.dwHeight = pmi->hdr.bmiHeader.biHeight;
		vpm.dwBitrate = pmi->hdr.dwBitRate;
		if(pmi->hdr.AvgTimePerFrame>0) vpm.dwFPS = (DWORD)(TIMEUNIT/pmi->hdr.AvgTimePerFrame);
		if(pmi->hdr.bmiHeader.biCompression==CODEC_VIDEO_SCREENCAP)
			vpm.dwFourCC = CODEC_VIDEO_SCREENCAP;
	}
	else if(amt.formattype==FORMAT_MPEG2Video) {
		MPEG2VIDEOINFO *pmi=(MPEG2VIDEOINFO *)amt.pbFormat;
		if(pmi==NULL) return FALSE;

		vpm.dwWidth = pmi->hdr.bmiHeader.biWidth;
		vpm.dwHeight = pmi->hdr.bmiHeader.biHeight;
		vpm.dwBitrate = pmi->hdr.dwBitRate;
		if(pmi->hdr.AvgTimePerFrame>0) vpm.dwFPS = (DWORD)(TIMEUNIT/pmi->hdr.AvgTimePerFrame);
		if(pmi->hdr.bmiHeader.biCompression==CODEC_VIDEO_SCREENCAP)
			vpm.dwFourCC = CODEC_VIDEO_SCREENCAP;
	}

	if((vpm.dwFPS<1)||(vpm.dwFPS>30))
		vpm.dwFPS = 30; // Our Default!!
	
	vpm.dwQuality = 0; // Our Default!!
	vpm.dwSecPerKey = 1; // Our Default!!

	return TRUE;
}

HRESULT WMX_CopyHeader(IWMHeaderInfo *pReaderHeaderInfo, IWMHeaderInfo* pWriterHeaderInfo, const WCHAR * pwszName)
{
	if ( NULL == pReaderHeaderInfo || NULL == pWriterHeaderInfo || NULL == pwszName )
	{
		return E_INVALIDARG ;
	}

	WORD nstreamNum = 0, cbLength = 0;
	WMT_ATTR_DATATYPE type ;
	HRESULT hr = S_OK;
	BYTE *pValue = NULL ;
	
	do
	{
		//
		// Get the no of bytes to be allocated for pValue
		//
		hr = pReaderHeaderInfo->GetAttributeByName( &nstreamNum,
													pwszName,
													&type,
													NULL,
													&cbLength
													);
		if ( FAILED( hr ) && hr != ASF_E_NOTFOUND )
		{
			_tprintf( _T( "GetAttributeByName failed for Attribute name %ws (hr=0x%08x).\n" ) , pwszName, hr) ;
			break;
		}
		
		if( cbLength == 0 && hr == ASF_E_NOTFOUND )
		{
			hr = S_OK ;
			break ;
		}

		pValue = new BYTE[ cbLength ];
		if ( NULL == pValue )
		{
			_tprintf( _T( "Unable to allocate memory for the Attribute name %ws" ), pwszName) ;
			hr = E_OUTOFMEMORY;
			break ;
		}
		//
		// Now, Get the value
		//
		hr = pReaderHeaderInfo->GetAttributeByName( &nstreamNum,
													pwszName,
													&type,
													pValue,
													&cbLength );
		if ( FAILED( hr ) )
		{
			_tprintf( _T( "GetAttributeByName failed for Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
			break;
		}
		//
		// Set the attribute
		//
		hr = pWriterHeaderInfo->SetAttribute( nstreamNum,
											  pwszName,
											  type,
											  pValue,
											  cbLength );
		if ( FAILED( hr ) )
		{
			_tprintf( _T("SetAttribute failed for Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
			break;
		}
		
		memset( pValue, 0, cbLength );
		//
		// Double check
		//
		hr = pWriterHeaderInfo->GetAttributeByName( &nstreamNum,
													pwszName,
													&type,
													pValue,
													&cbLength );
		if ( FAILED( hr ) )
		{
			_tprintf( _T("Could not set HeaderInfo for the Attribute name %ws (hr=0x%08x).\n" ), pwszName, hr) ;
			break;
		}
	}
	while(FALSE) ;

	delete[] pValue ;
	pValue = NULL ;

	return hr ;
}

HRESULT WMX_CopyAllHeaders( IWMHeaderInfo* pRHeaderInfo, IWMHeaderInfo* pWHeaderInfo )
{
	HRESULT hr = S_OK ;
	
	//
	// Copy all the header attributes, which can be set, from the
	// input file to the output file.
	// Its illegal to read DRM attributes.
	//
	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMTitle)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMAuthor)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMDescription)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMRating)  ;
	if( FAILED( hr ) )
		return hr ;
	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMCopyright)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMAlbumTitle)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMTrack)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMPromotionURL)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMAlbumCoverURL)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMGenre)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMYear)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMGenreID)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMMCDI)  ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMBannerImageType ) ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMBannerImageData ) ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMBannerImageURL ) ;
	if( FAILED( hr ) )
		return hr ;

	hr = WMX_CopyHeader(pRHeaderInfo, pWHeaderInfo, g_wszWMCopyrightURL ) ;

	return hr ;
}

HRESULT WMX_CopyScript( IWMHeaderInfo *pReaderHeaderInfo, IWMHeaderInfo* pWriterHeaderInfo)
{
	if ( NULL == pReaderHeaderInfo || NULL == pWriterHeaderInfo)
		return E_INVALIDARG ;

	HRESULT hr = S_OK ;
	
 	WORD cScript = 0 ;
	WCHAR *pwszType = NULL ;
	WORD cchTypeLen = 0 ;
	WCHAR *pwszCommand = NULL ;
	WORD cchCommandLen = 0 ;
	QWORD cnsScriptTime = 0 ;
	
	hr = pReaderHeaderInfo->GetScriptCount( &cScript ) ;
	if ( FAILED( hr ) )
	{
		_tprintf( _T("GetScriptCount failed (hr=0x%08x).\n" ), hr) ;
		return hr;
	}


	for( int i = 0 ; i < cScript ; i++)
	{
		//
		// Get the memory reqd for this script
		//
		hr = pReaderHeaderInfo->GetScript( i ,
										   NULL ,
										   &cchTypeLen ,
										   NULL ,
										   &cchCommandLen ,
										   &cnsScriptTime ) ;
		if ( FAILED( hr ) )
		{
			_tprintf( _T("GetScript failed for Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}
		
		pwszType = new WCHAR[cchTypeLen] ;
		pwszCommand = new WCHAR[cchCommandLen] ;
		if( pwszType == NULL || pwszCommand == NULL)
		{
			hr = E_OUTOFMEMORY ;
			break ;
		}
		//
		// Now, get the script
		//
		hr = pReaderHeaderInfo->GetScript( i ,
										   pwszType ,
										   &cchTypeLen ,
										   pwszCommand ,
										   &cchCommandLen ,
										   &cnsScriptTime ) ;
		if ( FAILED( hr ) )
		{
			_tprintf( _T("GetScript failed for Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}
		//
		// Add the script to the writer
		//
		hr = pWriterHeaderInfo->AddScript( pwszType ,
										  pwszCommand ,
										  cnsScriptTime ) ;
		if ( FAILED( hr ) )
		{
			_tprintf( _T("AddScript failed for Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}

		memset( pwszType, 0, cchTypeLen * sizeof(WCHAR));
		memset( pwszCommand, 0, cchCommandLen * sizeof(WCHAR));
		//
		//Double Check
		//
		hr = pWriterHeaderInfo->GetScript( i ,
										   pwszType ,
										   &cchTypeLen ,
										   pwszCommand ,
										   &cchCommandLen ,
										   &cnsScriptTime ) ;
		if ( FAILED( hr ) )
		{
			_tprintf( _T("Could not set Script no %d (hr=0x%08x).\n" ), i, hr) ;
			break ;
		}
		
		delete[] pwszType ;
		delete[] pwszCommand ;
		pwszType = pwszCommand = NULL ;
		
		cchTypeLen = 0;
		cchCommandLen = 0 ;
	}
	
	delete[] pwszType ;
	delete[] pwszCommand ;
	pwszType = pwszCommand = NULL ;
	
	return hr ;
}

HRESULT WMX_CopyMarker(IWMHeaderInfo *pReaderHeaderInfo, WCHAR * outfile)
{
	if ( NULL == pReaderHeaderInfo || NULL == outfile)
		return E_INVALIDARG ;
	
	HRESULT hr = S_OK ;
	WORD cMarker = 0 ;
	IWMMetadataEditor *pEditor = NULL;
	IWMHeaderInfo *pWriterHeaderInfo = NULL;

	do
	{
		hr = pReaderHeaderInfo->GetMarkerCount( &cMarker ) ;
		if ( FAILED( hr ) )
		{
			_tprintf( _T("GetMarkerCount failed (hr=0x%08x).\n" ), hr) ;
			break ;
		}
		//
		// Markers can be copied only by the Metadata Editor.
		// Create an Editor
		//
		hr = WMCreateEditor( &pEditor );
		if ( FAILED( hr ) )
		{
			_tprintf( _T( "Could not create Editor (hr=0x%08x).\n" ), hr );
			break ;
		}

		hr = pEditor->Open( outfile ) ;
		if ( FAILED ( hr ) )
		{
			_tprintf( _T( "Could not open outfile %ws (hr=0x%08x).\n" ), outfile ,hr );
			break ;
		}

		hr = pEditor->QueryInterface( IID_IWMHeaderInfo, ( void ** ) &pWriterHeaderInfo );
		if ( FAILED( hr ) )
		{
			_tprintf( _T( "Could not QI for IWMHeaderInfo (hr=0x%08x).\n" ), hr );
			break ;
		}

		WCHAR *pwszMarkerName = NULL ;
		WORD cchMarkerNameLen = 0 ;
		QWORD cnsMarkerTime = 0;
		for( int i = 0 ; i < cMarker ; i++)
		{
			//
			// Get the memory reqd for this Marker
			//
			hr = pReaderHeaderInfo->GetMarker( i ,
											   NULL ,
											   &cchMarkerNameLen ,
											   &cnsMarkerTime ) ;
			if ( FAILED( hr ) )
			{
				_tprintf( _T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}
			
			pwszMarkerName = new WCHAR[cchMarkerNameLen] ;
			if( pwszMarkerName == NULL )
			{
				hr = E_OUTOFMEMORY ;
				break ;
			}

			hr = pReaderHeaderInfo->GetMarker( i ,
											   pwszMarkerName ,
											   &cchMarkerNameLen ,
											   &cnsMarkerTime ) ;
			if ( FAILED( hr ) )
			{
				_tprintf( _T("GetMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}

			hr = pWriterHeaderInfo->AddMarker( pwszMarkerName ,
											   cnsMarkerTime ) ;
			if ( FAILED( hr ) )
			{
				_tprintf( _T("AddMarker failed for Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}
			
			memset( pwszMarkerName, 0, cchMarkerNameLen * sizeof(WCHAR));
			//
			//Double Check
			//
			hr = pWriterHeaderInfo->GetMarker( i ,
											   pwszMarkerName ,
											   &cchMarkerNameLen ,
											   &cnsMarkerTime ) ;
			if ( FAILED( hr ) )
			{
				_tprintf( _T( "Could not set Marker no %d (hr=0x%08x).\n" ), i, hr) ;
				break ;
			}
			delete[] pwszMarkerName ;
			pwszMarkerName = NULL ;
			cchMarkerNameLen = 0 ;
		}
		delete[] pwszMarkerName ;
		pwszMarkerName = NULL ;

		if( FAILED( hr ) )
			break ;
		
		hr = pEditor->Flush() ;
		if( FAILED( hr ) )
		{
			_tprintf( _T( "Could not Flush the Editor (hr=0x%08x).\n" ), hr) ;
			break ;
		}
	}
	while( FALSE) ;

	hr = pEditor->Close() ;
	if( FAILED( hr ) )
		_tprintf( _T( "Could not close the Editor (hr=0x%08x).\n" ), hr) ;
	
	RELEASE( pWriterHeaderInfo) ;
	RELEASE( pEditor ) ;
	
	return hr ;
}