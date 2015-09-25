#ifndef _HELPERPROFILE_H_
#define _HELPERPROFILE_H_

#include <wmsdkidl.h>

#define BASE_STREAM_NUMBER_AUDIO    1     // Audio stream numbers start at 1
#define BASE_STREAM_NUMBER_VIDEO    2     // Video stream numbers start at 2

#define AUDIO_FLAGS_AUDIOVIDEO      1

///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP SetStreamBasics( IWMStreamConfig *pIWMStreamConfig, 
                              WORD wStreamNum, 
                              LPWSTR pwszStreamName,
                              LPWSTR pwszConnectionName,
                              DWORD dwBitrate,
                              WM_MEDIA_TYPE *pmt );

STDMETHODIMP AddAudioStream( IWMProfileManager2 *pIWMProfileManager,
							 IWMProfile *pIWMProfile,
							 DWORD dwFormatTag,
                             DWORD dwBitrate,
                             DWORD dwSampleRate,
                             DWORD dwChannels,
                             DWORD flags                             
                             );

STDMETHODIMP AddVideoStream( IWMProfileManager2 *pManager,
                             IWMProfile *pIWMProfile, 
                             WORD wStreamNum,
                             DWORD dwFourCC,
                             DWORD dwBitrate,
                             DWORD dwWidth,
                             DWORD dwHeight,
                             DWORD dwFPS,
                             DWORD dwQuality,
                             DWORD dwSecPerKey );

STDMETHODIMP AddMutexObject( IWMProfile *pIWMProfile, 
                             WORD cVideoStreams );
                             
STDMETHODIMP AddScriptStream( IWMProfile *pIWMProfile,
                              WORD wStreamNum,
                              DWORD dwBitrate );
#endif