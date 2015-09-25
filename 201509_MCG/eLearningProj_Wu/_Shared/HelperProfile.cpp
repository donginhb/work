//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001.  All rights reserved.
//


///////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <wmsdk.h>
#include <mmsystem.h>

#include "HelperProfile.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP SetStreamBasics( IWMStreamConfig *pIWMStreamConfig, 
                              WORD wStreamNum, 
                              LPWSTR pwszStreamName,
                              LPWSTR pwszConnectionName,
                              DWORD dwBitrate,
                              WM_MEDIA_TYPE *pmt )
{
    HRESULT hr = S_OK;
    IWMMediaProps *pIWMMediaProps = NULL;

    do
    {
        hr = pIWMStreamConfig->SetStreamNumber( wStreamNum );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMStreamConfig->SetStreamName( pwszStreamName );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMStreamConfig->SetConnectionName( pwszConnectionName );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMStreamConfig->SetBitrate( dwBitrate );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMStreamConfig->QueryInterface( IID_IWMMediaProps,
                                               (void **) &pIWMMediaProps );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMMediaProps->SetMediaType( pmt );
        if( FAILED( hr ) )
        {
            break;
        }
    }
    while( FALSE );

    RELEASE( pIWMMediaProps );

    return( hr );
}


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP AddAudioStream( IWMProfileManager2 *pIWMProfileManager,
							 IWMProfile *pIWMProfile,
							 DWORD dwFormatTag,
                             DWORD dwBitrate,
                             DWORD dwSampleRate,
                             DWORD dwChannels,
                             DWORD dwFlags
                             )
{
    HRESULT hr = S_OK;

    IWMStreamConfig *pIWMStreamConfig = NULL;
    IWMMediaProps   *pIMP  = NULL;
    IWMCodecInfo *pIWMInfo = NULL;
    WAVEFORMATEX *pSaveWFX = NULL;
    WM_MEDIA_TYPE   *pType = NULL;

    do
    {
        hr = pIWMProfileManager->QueryInterface( IID_IWMCodecInfo,
                                                 (void **) &pIWMInfo );
        if( FAILED( hr ) )
        {
            break;
        }

        DWORD i, j;
        DWORD cCodecs;
        hr = pIWMInfo->GetCodecInfoCount( WMMEDIATYPE_Audio,
                                          &cCodecs );
        if( FAILED( hr ) )
        {
            break;
        }

        for( i = 0; i < cCodecs; i++ )
        {
            DWORD cFormats;
            hr = pIWMInfo->GetCodecFormatCount( WMMEDIATYPE_Audio,
                                                i,
                                                &cFormats );
            if( FAILED( hr ) )
            {
                break;
            }

            for( j = 0; j < cFormats; j++ )
            {
                if( pType )
                {
                    delete pType;
                    pType = NULL;
                }

                DWORD cbType = 0;
                hr = pIWMInfo->GetCodecFormat( WMMEDIATYPE_Audio,
                                               i,
                                               j,
                                               &pIWMStreamConfig );
                if( FAILED( hr ) )
                {
                    break;
                }

                RELEASE( pIMP );
                hr = pIWMStreamConfig->QueryInterface( IID_IWMMediaProps,
                                        ( void ** )&pIMP );
                RELEASE( pIWMStreamConfig );
                if( FAILED( hr ) )
                {
                    break;
                }

                hr = pIMP->GetMediaType( NULL, &cbType );
                if( FAILED( hr ) )
                {
                    break;
                }

                pType = (WM_MEDIA_TYPE *) new BYTE[ cbType ];
                if( !pType )
                {
                    hr = E_OUTOFMEMORY;
                    break;
                }

                hr = pIMP->GetMediaType( pType, &cbType );
                if( FAILED( hr ) )
                {
                    break;
                }

                if( pType->formattype != WMFORMAT_WaveFormatEx )
                {
                    hr = E_FAIL;
                    break;
                }

                WAVEFORMATEX *pWfx = (WAVEFORMATEX *) pType->pbFormat;
                int nDiff = (int) ( pWfx->nAvgBytesPerSec * 8 - dwBitrate );
                if( nDiff < 250 &&
                    nDiff > -250 &&
                    pWfx->nSamplesPerSec == dwSampleRate &&
                    pWfx->nChannels == dwChannels &&
                    pWfx->wFormatTag == dwFormatTag )
                {
                    if( pSaveWFX )
                    {
                        if( dwFlags & AUDIO_FLAGS_AUDIOVIDEO )
                        {
                            if( pWfx->nBlockAlign > pSaveWFX->nBlockAlign )
                            {
                                //
                                // For audio/video configurations, we want to
                                // find the smaller nBlockAlign. In this case,
                                // the nBlockAlign is larger, so we want to
                                // use the old format.
                                //
                                continue;
                            }
                        }
                        else
                        {
                            if( pWfx->nBlockAlign < pSaveWFX->nBlockAlign )
                            {
                                //
                                // Same as above, except we're looking for the
                                // larger nBlockAlign.
                                //
                                continue;
                            }
                        }
                    }

                    //
                    // If we got this far, this is a valid WFX, and the best
                    // we've seen so far. Store it.
                    //
                    delete pSaveWFX;
                    pSaveWFX = (WAVEFORMATEX *) new BYTE[ pType->cbFormat ];
                    if( !pSaveWFX )
                    {
                        hr = E_OUTOFMEMORY;
                        break;
                    }

                    memcpy( pSaveWFX, pWfx, pType->cbFormat );
                }
            }

            if( FAILED( hr ) )
            {
                break;
            }
        }

        if( FAILED( hr ) )
        {
            break;
        }

        if( pSaveWFX )
        {
            //
            // We found a valid WAVEFORMATEX; go ahead and set up the stream.
            //
            WM_MEDIA_TYPE mt;
            
            ZeroMemory( &mt, sizeof( mt ) );
            
            mt.majortype = WMMEDIATYPE_Audio;
            mt.subtype = WMMEDIASUBTYPE_Base;
            mt.subtype.Data1 = pSaveWFX->wFormatTag;

            mt.bFixedSizeSamples = TRUE;
            mt.bTemporalCompression = FALSE;
            mt.lSampleSize = pSaveWFX->nBlockAlign;
            mt.formattype = WMFORMAT_WaveFormatEx;
            mt.pUnk = NULL;
            mt.cbFormat = sizeof( WAVEFORMATEX ) + pSaveWFX->cbSize;
            mt.pbFormat = (BYTE *) pSaveWFX;

            hr = pIWMProfile->CreateNewStream( WMMEDIATYPE_Audio,
                                               &pIWMStreamConfig );
            if( FAILED( hr ) )
            {
                break;
            }
            
            hr = SetStreamBasics( pIWMStreamConfig, 
                                  BASE_STREAM_NUMBER_AUDIO,
                                  L"Audio Stream",
                                  L"Audio",
                                  pSaveWFX->nAvgBytesPerSec * 8,
                                  &mt );
            if( FAILED( hr ) )
            {
                break;
            }

            hr = pIWMProfile->AddStream( pIWMStreamConfig );
            if( FAILED( hr ) )
            {
                break;
            }
        }
        else
        {
            hr = NS_E_AUDIO_CODEC_NOT_INSTALLED;
            break;
        }
    }
    while( FALSE );

    delete pType;
    delete pSaveWFX;
    RELEASE( pIWMInfo );
    RELEASE( pIWMStreamConfig );
    RELEASE( pIMP );

    return( hr );
}


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP AddVideoStream( IWMProfileManager2 *pManager, 
                             IWMProfile *pIWMProfile,
                             WORD wStreamNum,
                             DWORD dwFourCC,
                             DWORD dwBitrate,
                             DWORD dwWidth,
                             DWORD dwHeight,
                             DWORD dwFPS,
                             DWORD dwQuality,
                             DWORD dwSecPerKey )
{
    HRESULT hr = S_OK;

    IWMCodecInfo *pCodecInfo = NULL;
    IWMStreamConfig *pStreamConfig = NULL;
    IWMVideoMediaProps *pProps = NULL;
    WM_MEDIA_TYPE *pMT = NULL;

    do
    {
        hr = pManager->QueryInterface( IID_IWMCodecInfo,
                                       (void **) &pCodecInfo );
        if( FAILED( hr ) )
        {
            break;
        }

        DWORD cCodecs;
        hr = pCodecInfo->GetCodecInfoCount( WMMEDIATYPE_Video, &cCodecs );
        if( FAILED( hr ) )
        {
            break;
        }

        DWORD i;
        for( i = 0; i < cCodecs; i++ )
        {
            DWORD cFormats;
            hr = pCodecInfo->GetCodecFormatCount( WMMEDIATYPE_Video,
                                                  i,
                                                  &cFormats );
            if( FAILED( hr ) )
            {
                break;
            }
                  
            DWORD j;
            for( j = 0; j < cFormats; j++ )
            {
                RELEASE( pStreamConfig );

                hr = pCodecInfo->GetCodecFormat( WMMEDIATYPE_Video,
                                                 i,
                                                 j,
                                                 &pStreamConfig );
                if( FAILED( hr ) )
                {
                    break;
                }

                RELEASE( pProps );
                hr = pStreamConfig->QueryInterface( IID_IWMVideoMediaProps,
                                                    (void **) &pProps );
                if( FAILED( hr ) )
                {
                    break;
                }

                DWORD cbMT;
                hr = pProps->GetMediaType( NULL, &cbMT );
                if( FAILED( hr ) )
                {
                    break;
                }

                delete pMT;
                pMT = (WM_MEDIA_TYPE *) new BYTE[ cbMT ];
                if( !pMT )
                {
                    hr = E_OUTOFMEMORY;
                    break;
                }

                hr = pProps->GetMediaType( pMT, &cbMT );
                if( FAILED( hr ) )
                {
                    break;
                }

                if( pMT->formattype != WMFORMAT_VideoInfo )
                {
                    RELEASE( pStreamConfig );
                    continue;
                }

                WMVIDEOINFOHEADER *pVIH = (WMVIDEOINFOHEADER *) pMT->pbFormat;

                if( pVIH->bmiHeader.biCompression == dwFourCC )
                {
                    break;
                }

                RELEASE( pStreamConfig );
            }

            if( FAILED( hr ) || pStreamConfig )
            {
                break;
            }
        }

        if( FAILED( hr ) )
        {
            break;
        }

        if( !pStreamConfig )
        {
            hr = NS_E_VIDEO_CODEC_NOT_INSTALLED;
            break;
        }

        WMVIDEOINFOHEADER *pVIH = (WMVIDEOINFOHEADER *) pMT->pbFormat;
        pVIH->dwBitRate = dwBitrate;

        pVIH->rcSource.right = dwWidth;
        pVIH->rcSource.bottom = dwHeight;
        pVIH->rcTarget.right = dwWidth;
        pVIH->rcTarget.bottom = dwHeight;
        pVIH->bmiHeader.biWidth = dwWidth;
        pVIH->bmiHeader.biHeight = dwHeight;

        pVIH->AvgTimePerFrame = ((LONGLONG) 10000000 ) / ((LONGLONG) dwFPS );

        hr = pProps->SetQuality( dwQuality );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pProps->SetMaxKeyFrameSpacing( ((LONGLONG) 10000000 ) *
                                            ((LONGLONG) dwSecPerKey ) );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = SetStreamBasics( pStreamConfig, 
                              wStreamNum,
                              L"Video Stream",
                              L"Video",
                              dwBitrate,
                              pMT );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMProfile->AddStream( pStreamConfig );
        if( FAILED( hr ) )
        {
            break;
        }
    }
    while( FALSE );

    RELEASE( pCodecInfo );
    RELEASE( pStreamConfig );
    RELEASE( pProps );
    delete pMT;

    return( hr );
}


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP AddMutexObject( IWMProfile *pIWMProfile, 
                             WORD cVideoStreams )
{
    HRESULT hr = S_OK;

    IWMMutualExclusion *pIWMMutualExclusion = NULL;

    do
    {
        hr = pIWMProfile->CreateNewMutualExclusion( &pIWMMutualExclusion );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMMutualExclusion->SetType( CLSID_WMMUTEX_Bitrate );
        if( FAILED( hr ) )
        {
            break;
        }

        for( WORD i = 0; ( i < cVideoStreams ) && SUCCEEDED( hr ); i++ )
        {
            hr = pIWMMutualExclusion->AddStream( i + BASE_STREAM_NUMBER_VIDEO );
        }
        
        if( FAILED( hr ) )
        {
            break;
        }

        hr = pIWMProfile->AddMutualExclusion( pIWMMutualExclusion );
        if( FAILED( hr ) )
        {
            break;
        }
    }
    while( FALSE );

    RELEASE( pIWMMutualExclusion );

    return( hr );
}


///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP AddScriptStream( IWMProfile *pIWMProfile,
                              WORD wStreamNum,
                              DWORD dwBitrate )
{
    HRESULT hr = S_OK;

    IWMStreamConfig *pIWMStreamConfig = NULL;

    do
    {
        WM_MEDIA_TYPE mt;
        WMSCRIPTFORMAT sf;

        ZeroMemory( &mt, sizeof( mt ) );
        ZeroMemory( &sf, sizeof( sf ) );

        mt.majortype = WMMEDIATYPE_Script;
        mt.subtype = GUID_NULL;
        
        mt.bFixedSizeSamples = FALSE;
        mt.bTemporalCompression = FALSE;
        mt.lSampleSize = 0;
        mt.formattype = WMFORMAT_Script;
        mt.pUnk = NULL;
        mt.cbFormat = sizeof( WMSCRIPTFORMAT );
        mt.pbFormat = (BYTE *) &sf;

        sf.scriptType = WMSCRIPTTYPE_TwoStrings;


        hr = pIWMProfile->CreateNewStream( WMMEDIATYPE_Script, &pIWMStreamConfig );
        if( FAILED( hr ) )
        {
            break;
        }

        hr = SetStreamBasics( pIWMStreamConfig, 
                              wStreamNum, 
                              L"Script Stream", 
                              L"Script", 
                              dwBitrate,
                              &mt );
        if( FAILED( hr ) )
        {
            break;
        }
  
        hr = pIWMProfile->AddStream( pIWMStreamConfig );
        if( FAILED( hr ) )
        {
            break;
        }    
    }
    while( FALSE );

    RELEASE( pIWMStreamConfig );

    return( hr );
}
