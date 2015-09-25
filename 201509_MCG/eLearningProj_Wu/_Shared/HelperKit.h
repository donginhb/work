#ifndef _HELPERKIT_H_
#define _HELPERKIT_H_

#include <wmsdkidl.h>

#define CODEC_AUDIO_MSAUDIO                 353   // Microsoft WMAudio
#define CODEC_AUDIO_ACELP                   304   // Sipro.net ACELP

//#define CODEC_VIDEO_SCREENCAP7				827544397L
#define CODEC_VIDEO_SCREENCAP				0x3253534D // MSS2 // 859066445L //Update by CCH 920130
#define CODEC_VIDEO_CAMERACAP				0x31564D57 // WMV1 0x33564D57 // WMV3

//#define CODEC_VIDEO_MPEG43                  MAKEFOURCC( 'M', 'P', '4', '3' )
//#define CODEC_VIDEO_WMV1                    MAKEFOURCC( 'W', 'M', 'V', '1' )

#define DEFAULT_SCRIPT_STREAM_BANDWIDTH     2048

// ASF Supported Audio Bit Rate
#define BPS_AUDIO_5K		5000L
#define BPS_AUDIO_6K		6000L
#define BPS_AUDIO_6_5K		6500L
#define BPS_AUDIO_8K		8000L
#define BPS_AUDIO_8_5K		8500L
#define BPS_AUDIO_10K		10000L
#define BPS_AUDIO_12K		12000L
#define BPS_AUDIO_16K		16000L
#define BPS_AUDIO_20K		20000L
#define BPS_AUDIO_22K		22000L
#define BPS_AUDIO_32K		32000L
#define BPS_AUDIO_40K		40000L
#define BPS_AUDIO_48K		48000L
#define BPS_AUDIO_64K		64000L
#define BPS_AUDIO_80K		80000L
#define BPS_AUDIO_96K		96000L
#define BPS_AUDIO_128K		128000L
#define BPS_AUDIO_160K		160000L
#define BPS_AUDIO_192K		192000L

typedef struct {
    DWORD dwBitrate;
    DWORD dwFormatTag;
    DWORD dwSampleRate;
    DWORD dwChannels;
} AUDIO_PARAMS;

typedef struct {
    DWORD dwFourCC;
    DWORD dwBitrate;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwFPS;
    DWORD dwQuality;
    DWORD dwSecPerKey;
} VIDEO_PARAMS;

typedef struct {
    DWORD dwBitrate;
} SCRIPT_PARAMS;


HRESULT WMX_CopyAllHeaders( IWMHeaderInfo* pRHeaderInfo, IWMHeaderInfo* pWHeaderInfo );
HRESULT WMX_CopyScript( IWMHeaderInfo *pReaderHeaderInfo, IWMHeaderInfo* pWriterHeaderInfo);
HRESULT WMX_CopyMarker(IWMHeaderInfo *pReaderHeaderInfo, WCHAR * outfile);

BOOL PF_GetStreamCount(IWMProfile *pwmpf, WORD &wAudio, WORD &wVideo, WORD &wScript);
IWMProfile *PF_LoadFromPRX(IWMProfileManager2 *pwmpm, BSTR bstrPrx);
IWMProfile *PF_Subtract(IWMProfileManager2 *pwmpm, IWMProfile *pwmpf, BOOL fVideo, BOOL fScript);
HRESULT PF_SavePRX(IWMProfileManager2 *pWMPM, IWMProfile *pProfile, WCHAR *pwchFile);
IWMProfile *PF_Lower(IWMProfileManager2 *pWMPM, IWMProfile *pProfile, BOOL fAudio, DWORD dwAudioSampleRate, DWORD dwAudioChannels, DWORD dwAudioBitrate, BOOL fVideo, DWORD dwVideoSecPerKey, DWORD dwVideoQuality, WORD wFPS, DWORD dwVideoBitrate, BOOL fScript, DWORD dwScriptBitrate);

void PF_CorrectAPM(AUDIO_PARAMS &apm);
void PF_CorrectVPM(VIDEO_PARAMS &vpm);
void PF_CorrectSPM(SCRIPT_PARAMS &spm);

BOOL PF_GetParameter(IWMProfile *pwmpf, AUDIO_PARAMS &AudioParams, VIDEO_PARAMS &VideoParams, SCRIPT_PARAMS &ScriptParams);

IWMProfile *PF_Generate(IWMProfileManager2 *pWMPM, 
					  AUDIO_PARAMS *pAudioParams,
                      SCRIPT_PARAMS *pScriptParams,
                      WORD cVideoStreams,
                      VIDEO_PARAMS *prgVideoParams);

BOOL PF_GetAudioStreamParameter(WM_MEDIA_TYPE &amt, AUDIO_PARAMS &apm);
BOOL PF_GetVideoStreamParameter(WM_MEDIA_TYPE &amt, VIDEO_PARAMS &vpm);
BOOL PF_GetAudioPropsParameter(IWMMediaProps *pmp, AUDIO_PARAMS &apm);
BOOL PF_GetVideoPropsParameter(IWMMediaProps *pmp, VIDEO_PARAMS &vpm);
#endif