/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 30 10:28:26 2012
 */
/* Compiler settings for C:\MCG5.0\eLearningProj_Wu\MediaKit\MediaKit.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MediaKit_h__
#define __MediaKit_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IWMXUtility_FWD_DEFINED__
#define __IWMXUtility_FWD_DEFINED__
typedef interface IWMXUtility IWMXUtility;
#endif 	/* __IWMXUtility_FWD_DEFINED__ */


#ifndef __WMXUtility_FWD_DEFINED__
#define __WMXUtility_FWD_DEFINED__

#ifdef __cplusplus
typedef class WMXUtility WMXUtility;
#else
typedef struct WMXUtility WMXUtility;
#endif /* __cplusplus */

#endif 	/* __WMXUtility_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_MediaKit_0000 */
/* [local] */ 

typedef /* [public][public][public][public] */ 
enum __MIDL___MIDL_itf_MediaKit_0000_0001
    {	IMAGEFORMAT_KIT_BMP	= 0,
	IMAGEFORMAT_KIT_JPG	= 1,
	IMAGEFORMAT_KIT_PCX	= 2,
	IMAGEFORMAT_KIT_TIF	= 3,
	IMAGEFORMAT_KIT_PNG	= 4
    }	ENUMIMAGEFORMAT_KIT;

typedef /* [public][public][public][public][public] */ struct  __MIDL___MIDL_itf_MediaKit_0000_0002
    {
    DWORD dwBitrate;
    DWORD dwFormatTag;
    DWORD dwSampleRate;
    DWORD dwChannels;
    }	AUDIOPARAM;

typedef /* [public][public][public][public][public] */ struct  __MIDL___MIDL_itf_MediaKit_0000_0003
    {
    DWORD dwFourCC;
    DWORD dwBitrate;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwFPS;
    DWORD dwQuality;
    DWORD dwSecPerKey;
    }	VIDEOPARAM;

typedef /* [public][public][public][public][public] */ struct  __MIDL___MIDL_itf_MediaKit_0000_0004
    {
    DWORD dwBitrate;
    }	SCRIPTPARAM;

typedef /* [public][public][public] */ struct  __MIDL___MIDL_itf_MediaKit_0000_0005
    {
    AUDIOPARAM ainfo;
    VIDEOPARAM vinfo;
    SCRIPTPARAM sinfo;
    }	PROFILEPARAMS;



extern RPC_IF_HANDLE __MIDL_itf_MediaKit_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_MediaKit_0000_v0_0_s_ifspec;

#ifndef __IWMXUtility_INTERFACE_DEFINED__
#define __IWMXUtility_INTERFACE_DEFINED__

/* interface IWMXUtility */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMXUtility;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B86F4B76-BE82-4952-A26B-91F83866AD4C")
    IWMXUtility : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupProgressHandler( 
            /* [in] */ long lWnd,
            /* [in] */ long lMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupQueryAbort( 
            /* [in] */ long lWndAbortActive,
            /* [in] */ long __RPC_FAR *lpbAbort) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXAudio2Wav( 
            /* [in] */ BSTR bstrAudioList,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrMixerTimers,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXAudioFromWav( 
            /* [in] */ BSTR bstrWMXList,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrWave,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXDirectCopy( 
            /* [in] */ BSTR bstrList,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXCopyWav( 
            /* [in] */ BSTR bstrList,
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrOrgProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Wav2WMX( 
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WavFromWMX( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsWMXProtected( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbProtected) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsWMXProfileStreamCompatible( 
            /* [in] */ BSTR bstrWMX1,
            /* [in] */ BSTR bstrWMX2,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbAudio,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbVideo,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbScript) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsWMXProfileCompatible( 
            /* [in] */ BSTR bstrWMX1,
            /* [in] */ BSTR bstrWMX2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsWMXIndexExisted( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbExisted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWMXDuration( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ double __RPC_FAR *pdmsLength) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWMXProfileInfo( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ AUDIOPARAM __RPC_FAR *painfo,
            /* [out] */ VIDEOPARAM __RPC_FAR *pvinfo,
            /* [out] */ SCRIPTPARAM __RPC_FAR *psinfo) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWMXAudioPeak( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ long __RPC_FAR *plPeakMin,
            /* [out] */ long __RPC_FAR *plPeakMax,
            /* [out] */ long __RPC_FAR *plMin,
            /* [out] */ long __RPC_FAR *plMax) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PRXGetParams( 
            /* [in] */ BSTR bstrPRX,
            /* [in] */ PROFILEPARAMS __RPC_FAR *pParam) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PRXSubtract( 
            /* [in] */ BSTR bstrPRX1,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrPRX2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PRXLower( 
            /* [in] */ BSTR bstrPRX,
            /* [in] */ VARIANT_BOOL vbAudioLower,
            /* [in] */ long lAudioSampleRate,
            /* [in] */ long lAudioChannels,
            /* [in] */ long lAudioKiloBPS,
            /* [in] */ VARIANT_BOOL vbVideoLower,
            /* [in] */ long lVideoSecPerKey,
            /* [in] */ long lVideoQuality,
            /* [in] */ long lVideoFPS,
            /* [in] */ long lVideoKiloBPS,
            /* [in] */ VARIANT_BOOL vbScriptLower,
            /* [in] */ long lScriptKiloBPS,
            /* [in] */ BSTR bstrOutPRX) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PRXFromWMX( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ long lFPS,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutPrx) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PRXGen( 
            /* [in] */ PROFILEPARAMS __RPC_FAR *pParam,
            /* [in] */ BSTR bstrGenPRX) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXMakeIndex( 
            /* [in] */ BSTR bstrWMX) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudioMultiply( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lRatio_100,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudioNormalize( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lPercentage_100,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudioReplace( 
            /* [in] */ BSTR bstrFile,
            /* [in] */ BSTR bstrReplace,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudioFromWav( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrWav,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXConvert( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXCopy( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SpecWMXDumpScript( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrOutTable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SpecWMXMakeAVScript( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutWMX,
            /* [in] */ BSTR bstrOutTable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXMixer2Wav( 
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrLists,
            /* [in] */ BSTR bstrOutWav) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXFromImage( 
            /* [in] */ BSTR bstrImage,
            /* [in] */ long lmsDuration,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXFromMuteAudio( 
            /* [in] */ long lmsDuration,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXGetCoveredImages( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrImagePrev,
            /* [in] */ BSTR bstrImageNext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXGetNextImage( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrOutImage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXGetPrevImage( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrOutImage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMX_DBG_GetSampleCount( 
            /* [in] */ BSTR bstrWMX,
            /* [out] */ long __RPC_FAR *plAudio,
            /* [out] */ long __RPC_FAR *plVideo,
            /* [out] */ long __RPC_FAR *plScript) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXDirectAppend( 
            /* [in] */ BSTR bstrWMXNameTable,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutWMX) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXOutput( 
            /* [in] */ BSTR bstrMain,
            /* [in] */ BSTR bstrAudioReplace,
            /* [in] */ BSTR bstrAudioMix,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudio2Wav( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutWav) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXDirectTrim( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXConvertTrim( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MBRPrxGen( 
            /* [in] */ long lA1kbps,
            /* [in] */ long lV1kps,
            /* [in] */ long lwV1,
            /* [in] */ long lhV1,
            /* [in] */ long lA2kbps,
            /* [in] */ long lV2kps,
            /* [in] */ long lwV2,
            /* [in] */ long lhV2,
            /* [in] */ long lA3kbps,
            /* [in] */ long lV3kps,
            /* [in] */ long lwV3,
            /* [in] */ long lhV3,
            /* [in] */ BSTR bstrPrxOut) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiWMXExport( 
            /* [in] */ BSTR bstrList,
            /* [in] */ IUnknown __RPC_FAR *pAudioManager,
            /* [in] */ IUnknown __RPC_FAR *pVideoManager,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXReplaceAudio_Wav( 
            /* [in] */ BSTR bstrFile,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrReplace,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMXAudioMixReplace2Wav( 
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrReplaceList,
            /* [in] */ BSTR bstrMixList,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutputWav) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMXUtilityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWMXUtility __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWMXUtility __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWMXUtility __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetupProgressHandler )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ long lWnd,
            /* [in] */ long lMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetupQueryAbort )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ long lWndAbortActive,
            /* [in] */ long __RPC_FAR *lpbAbort);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXAudio2Wav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrAudioList,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrMixerTimers,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXAudioFromWav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMXList,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrWave,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXDirectCopy )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrList,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXCopyWav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrList,
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrOrgProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Wav2WMX )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WavFromWMX )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsWMXProtected )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbProtected);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsWMXProfileStreamCompatible )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX1,
            /* [in] */ BSTR bstrWMX2,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbAudio,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbVideo,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbScript);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsWMXProfileCompatible )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX1,
            /* [in] */ BSTR bstrWMX2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsWMXIndexExisted )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pvbExisted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWMXDuration )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ double __RPC_FAR *pdmsLength);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWMXProfileInfo )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ AUDIOPARAM __RPC_FAR *painfo,
            /* [out] */ VIDEOPARAM __RPC_FAR *pvinfo,
            /* [out] */ SCRIPTPARAM __RPC_FAR *psinfo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWMXAudioPeak )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ long __RPC_FAR *plPeakMin,
            /* [out] */ long __RPC_FAR *plPeakMax,
            /* [out] */ long __RPC_FAR *plMin,
            /* [out] */ long __RPC_FAR *plMax);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PRXGetParams )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrPRX,
            /* [in] */ PROFILEPARAMS __RPC_FAR *pParam);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PRXSubtract )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrPRX1,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrPRX2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PRXLower )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrPRX,
            /* [in] */ VARIANT_BOOL vbAudioLower,
            /* [in] */ long lAudioSampleRate,
            /* [in] */ long lAudioChannels,
            /* [in] */ long lAudioKiloBPS,
            /* [in] */ VARIANT_BOOL vbVideoLower,
            /* [in] */ long lVideoSecPerKey,
            /* [in] */ long lVideoQuality,
            /* [in] */ long lVideoFPS,
            /* [in] */ long lVideoKiloBPS,
            /* [in] */ VARIANT_BOOL vbScriptLower,
            /* [in] */ long lScriptKiloBPS,
            /* [in] */ BSTR bstrOutPRX);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PRXFromWMX )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ long lFPS,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutPrx);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PRXGen )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ PROFILEPARAMS __RPC_FAR *pParam,
            /* [in] */ BSTR bstrGenPRX);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXMakeIndex )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudioMultiply )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lRatio_100,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudioNormalize )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lPercentage_100,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudioReplace )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrFile,
            /* [in] */ BSTR bstrReplace,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudioFromWav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrWav,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXConvert )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXCopy )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SpecWMXDumpScript )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrOutTable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SpecWMXMakeAVScript )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ VARIANT_BOOL vbCopyHeader,
            /* [in] */ VARIANT_BOOL vbCopyScript,
            /* [in] */ VARIANT_BOOL vbCopyMarker,
            /* [in] */ BSTR bstrOutWMX,
            /* [in] */ BSTR bstrOutTable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXMixer2Wav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWav,
            /* [in] */ BSTR bstrLists,
            /* [in] */ BSTR bstrOutWav);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXFromImage )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrImage,
            /* [in] */ long lmsDuration,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXFromMuteAudio )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ long lmsDuration,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXGetCoveredImages )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrImagePrev,
            /* [in] */ BSTR bstrImageNext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXGetNextImage )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrOutImage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXGetPrevImage )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
            /* [in] */ long lmsTime,
            /* [in] */ BSTR bstrOutImage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMX_DBG_GetSampleCount )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [out] */ long __RPC_FAR *plAudio,
            /* [out] */ long __RPC_FAR *plVideo,
            /* [out] */ long __RPC_FAR *plScript);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXDirectAppend )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMXNameTable,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutWMX);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXOutput )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrMain,
            /* [in] */ BSTR bstrAudioReplace,
            /* [in] */ BSTR bstrAudioMix,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudio2Wav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutWav);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXDirectTrim )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXConvertTrim )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ long lmsStart,
            /* [in] */ long lmsStop,
            /* [in] */ VARIANT_BOOL vbVideo,
            /* [in] */ VARIANT_BOOL vbScript,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MBRPrxGen )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ long lA1kbps,
            /* [in] */ long lV1kps,
            /* [in] */ long lwV1,
            /* [in] */ long lhV1,
            /* [in] */ long lA2kbps,
            /* [in] */ long lV2kps,
            /* [in] */ long lwV2,
            /* [in] */ long lhV2,
            /* [in] */ long lA3kbps,
            /* [in] */ long lV3kps,
            /* [in] */ long lwV3,
            /* [in] */ long lhV3,
            /* [in] */ BSTR bstrPrxOut);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MultiWMXExport )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrList,
            /* [in] */ IUnknown __RPC_FAR *pAudioManager,
            /* [in] */ IUnknown __RPC_FAR *pVideoManager,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXReplaceAudio_Wav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrFile,
            /* [in] */ IUnknown __RPC_FAR *pManager,
            /* [in] */ BSTR bstrReplace,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMXAudioMixReplace2Wav )( 
            IWMXUtility __RPC_FAR * This,
            /* [in] */ BSTR bstrWMX,
            /* [in] */ BSTR bstrReplaceList,
            /* [in] */ BSTR bstrMixList,
            /* [in] */ long lBitsPerSample,
            /* [in] */ long lChannels,
            /* [in] */ long lSampleRate,
            /* [in] */ BSTR bstrOutputWav);
        
        END_INTERFACE
    } IWMXUtilityVtbl;

    interface IWMXUtility
    {
        CONST_VTBL struct IWMXUtilityVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMXUtility_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMXUtility_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMXUtility_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMXUtility_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMXUtility_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMXUtility_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMXUtility_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMXUtility_SetupProgressHandler(This,lWnd,lMsg)	\
    (This)->lpVtbl -> SetupProgressHandler(This,lWnd,lMsg)

#define IWMXUtility_SetupQueryAbort(This,lWndAbortActive,lpbAbort)	\
    (This)->lpVtbl -> SetupQueryAbort(This,lWndAbortActive,lpbAbort)

#define IWMXUtility_MultiWMXAudio2Wav(This,bstrAudioList,pManager,bstrMixerTimers,lBitsPerSample,lChannels,lSampleRate,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXAudio2Wav(This,bstrAudioList,pManager,bstrMixerTimers,lBitsPerSample,lChannels,lSampleRate,bstrOutput)

#define IWMXUtility_MultiWMXAudioFromWav(This,bstrWMXList,pManager,bstrWave,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXAudioFromWav(This,bstrWMXList,pManager,bstrWave,bstrProfile,bstrOutput)

#define IWMXUtility_MultiWMXDirectCopy(This,bstrList,vbVideo,vbScript,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXDirectCopy(This,bstrList,vbVideo,vbScript,bstrOutput)

#define IWMXUtility_MultiWMXCopyWav(This,bstrList,bstrWav,bstrOrgProfile,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXCopyWav(This,bstrList,bstrWav,bstrOrgProfile,bstrOutput)

#define IWMXUtility_Wav2WMX(This,bstrWav,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> Wav2WMX(This,bstrWav,bstrProfile,bstrOutput)

#define IWMXUtility_WavFromWMX(This,bstrWMX,lBitsPerSample,lChannels,lSampleRate,bstrOutput)	\
    (This)->lpVtbl -> WavFromWMX(This,bstrWMX,lBitsPerSample,lChannels,lSampleRate,bstrOutput)

#define IWMXUtility_IsWMXProtected(This,bstrWMX,pvbProtected)	\
    (This)->lpVtbl -> IsWMXProtected(This,bstrWMX,pvbProtected)

#define IWMXUtility_IsWMXProfileStreamCompatible(This,bstrWMX1,bstrWMX2,pvbAudio,pvbVideo,pvbScript)	\
    (This)->lpVtbl -> IsWMXProfileStreamCompatible(This,bstrWMX1,bstrWMX2,pvbAudio,pvbVideo,pvbScript)

#define IWMXUtility_IsWMXProfileCompatible(This,bstrWMX1,bstrWMX2)	\
    (This)->lpVtbl -> IsWMXProfileCompatible(This,bstrWMX1,bstrWMX2)

#define IWMXUtility_IsWMXIndexExisted(This,bstrWMX,pvbExisted)	\
    (This)->lpVtbl -> IsWMXIndexExisted(This,bstrWMX,pvbExisted)

#define IWMXUtility_GetWMXDuration(This,bstrWMX,pdmsLength)	\
    (This)->lpVtbl -> GetWMXDuration(This,bstrWMX,pdmsLength)

#define IWMXUtility_GetWMXProfileInfo(This,bstrWMX,painfo,pvinfo,psinfo)	\
    (This)->lpVtbl -> GetWMXProfileInfo(This,bstrWMX,painfo,pvinfo,psinfo)

#define IWMXUtility_GetWMXAudioPeak(This,bstrWMX,plPeakMin,plPeakMax,plMin,plMax)	\
    (This)->lpVtbl -> GetWMXAudioPeak(This,bstrWMX,plPeakMin,plPeakMax,plMin,plMax)

#define IWMXUtility_PRXGetParams(This,bstrPRX,pParam)	\
    (This)->lpVtbl -> PRXGetParams(This,bstrPRX,pParam)

#define IWMXUtility_PRXSubtract(This,bstrPRX1,vbVideo,vbScript,bstrPRX2)	\
    (This)->lpVtbl -> PRXSubtract(This,bstrPRX1,vbVideo,vbScript,bstrPRX2)

#define IWMXUtility_PRXLower(This,bstrPRX,vbAudioLower,lAudioSampleRate,lAudioChannels,lAudioKiloBPS,vbVideoLower,lVideoSecPerKey,lVideoQuality,lVideoFPS,lVideoKiloBPS,vbScriptLower,lScriptKiloBPS,bstrOutPRX)	\
    (This)->lpVtbl -> PRXLower(This,bstrPRX,vbAudioLower,lAudioSampleRate,lAudioChannels,lAudioKiloBPS,vbVideoLower,lVideoSecPerKey,lVideoQuality,lVideoFPS,lVideoKiloBPS,vbScriptLower,lScriptKiloBPS,bstrOutPRX)

#define IWMXUtility_PRXFromWMX(This,bstrWMX,vbVideo,lFPS,vbScript,bstrOutPrx)	\
    (This)->lpVtbl -> PRXFromWMX(This,bstrWMX,vbVideo,lFPS,vbScript,bstrOutPrx)

#define IWMXUtility_PRXGen(This,pParam,bstrGenPRX)	\
    (This)->lpVtbl -> PRXGen(This,pParam,bstrGenPRX)

#define IWMXUtility_WMXMakeIndex(This,bstrWMX)	\
    (This)->lpVtbl -> WMXMakeIndex(This,bstrWMX)

#define IWMXUtility_WMXAudioMultiply(This,bstrWMX,lRatio_100,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXAudioMultiply(This,bstrWMX,lRatio_100,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)

#define IWMXUtility_WMXAudioNormalize(This,bstrWMX,lPercentage_100,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXAudioNormalize(This,bstrWMX,lPercentage_100,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)

#define IWMXUtility_WMXAudioReplace(This,bstrFile,bstrReplace,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutput)	\
    (This)->lpVtbl -> WMXAudioReplace(This,bstrFile,bstrReplace,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutput)

#define IWMXUtility_WMXAudioFromWav(This,bstrWMX,bstrWav,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXAudioFromWav(This,bstrWMX,bstrWav,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)

#define IWMXUtility_WMXConvert(This,bstrWMX,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXConvert(This,bstrWMX,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrProfile,bstrOutput)

#define IWMXUtility_WMXCopy(This,bstrWMX,vbVideo,vbScript,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutput)	\
    (This)->lpVtbl -> WMXCopy(This,bstrWMX,vbVideo,vbScript,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutput)

#define IWMXUtility_SpecWMXDumpScript(This,bstrWMX,bstrOutTable)	\
    (This)->lpVtbl -> SpecWMXDumpScript(This,bstrWMX,bstrOutTable)

#define IWMXUtility_SpecWMXMakeAVScript(This,bstrWMX,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutWMX,bstrOutTable)	\
    (This)->lpVtbl -> SpecWMXMakeAVScript(This,bstrWMX,vbCopyHeader,vbCopyScript,vbCopyMarker,bstrOutWMX,bstrOutTable)

#define IWMXUtility_MultiWMXMixer2Wav(This,bstrWav,bstrLists,bstrOutWav)	\
    (This)->lpVtbl -> MultiWMXMixer2Wav(This,bstrWav,bstrLists,bstrOutWav)

#define IWMXUtility_WMXFromImage(This,bstrImage,lmsDuration,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXFromImage(This,bstrImage,lmsDuration,bstrProfile,bstrOutput)

#define IWMXUtility_WMXFromMuteAudio(This,lmsDuration,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXFromMuteAudio(This,lmsDuration,bstrProfile,bstrOutput)

#define IWMXUtility_WMXGetCoveredImages(This,bstrWMX,eFormat,lmsTime,bstrImagePrev,bstrImageNext)	\
    (This)->lpVtbl -> WMXGetCoveredImages(This,bstrWMX,eFormat,lmsTime,bstrImagePrev,bstrImageNext)

#define IWMXUtility_WMXGetNextImage(This,bstrWMX,eFormat,lmsTime,bstrOutImage)	\
    (This)->lpVtbl -> WMXGetNextImage(This,bstrWMX,eFormat,lmsTime,bstrOutImage)

#define IWMXUtility_WMXGetPrevImage(This,bstrWMX,eFormat,lmsTime,bstrOutImage)	\
    (This)->lpVtbl -> WMXGetPrevImage(This,bstrWMX,eFormat,lmsTime,bstrOutImage)

#define IWMXUtility_WMX_DBG_GetSampleCount(This,bstrWMX,plAudio,plVideo,plScript)	\
    (This)->lpVtbl -> WMX_DBG_GetSampleCount(This,bstrWMX,plAudio,plVideo,plScript)

#define IWMXUtility_MultiWMXDirectAppend(This,bstrWMXNameTable,vbVideo,vbScript,bstrOutWMX)	\
    (This)->lpVtbl -> MultiWMXDirectAppend(This,bstrWMXNameTable,vbVideo,vbScript,bstrOutWMX)

#define IWMXUtility_MultiWMXOutput(This,bstrMain,bstrAudioReplace,bstrAudioMix,pManager,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXOutput(This,bstrMain,bstrAudioReplace,bstrAudioMix,pManager,bstrProfile,bstrOutput)

#define IWMXUtility_WMXAudio2Wav(This,bstrWMX,lmsStart,lmsStop,lBitsPerSample,lChannels,lSampleRate,bstrOutWav)	\
    (This)->lpVtbl -> WMXAudio2Wav(This,bstrWMX,lmsStart,lmsStop,lBitsPerSample,lChannels,lSampleRate,bstrOutWav)

#define IWMXUtility_WMXDirectTrim(This,bstrWMX,lmsStart,lmsStop,vbVideo,vbScript,bstrOutput)	\
    (This)->lpVtbl -> WMXDirectTrim(This,bstrWMX,lmsStart,lmsStop,vbVideo,vbScript,bstrOutput)

#define IWMXUtility_WMXConvertTrim(This,bstrWMX,lmsStart,lmsStop,vbVideo,vbScript,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXConvertTrim(This,bstrWMX,lmsStart,lmsStop,vbVideo,vbScript,bstrProfile,bstrOutput)

#define IWMXUtility_MBRPrxGen(This,lA1kbps,lV1kps,lwV1,lhV1,lA2kbps,lV2kps,lwV2,lhV2,lA3kbps,lV3kps,lwV3,lhV3,bstrPrxOut)	\
    (This)->lpVtbl -> MBRPrxGen(This,lA1kbps,lV1kps,lwV1,lhV1,lA2kbps,lV2kps,lwV2,lhV2,lA3kbps,lV3kps,lwV3,lhV3,bstrPrxOut)

#define IWMXUtility_MultiWMXExport(This,bstrList,pAudioManager,pVideoManager,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> MultiWMXExport(This,bstrList,pAudioManager,pVideoManager,bstrProfile,bstrOutput)

#define IWMXUtility_WMXReplaceAudio_Wav(This,bstrFile,pManager,bstrReplace,bstrProfile,bstrOutput)	\
    (This)->lpVtbl -> WMXReplaceAudio_Wav(This,bstrFile,pManager,bstrReplace,bstrProfile,bstrOutput)

#define IWMXUtility_WMXAudioMixReplace2Wav(This,bstrWMX,bstrReplaceList,bstrMixList,lBitsPerSample,lChannels,lSampleRate,bstrOutputWav)	\
    (This)->lpVtbl -> WMXAudioMixReplace2Wav(This,bstrWMX,bstrReplaceList,bstrMixList,lBitsPerSample,lChannels,lSampleRate,bstrOutputWav)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_SetupProgressHandler_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ long lWnd,
    /* [in] */ long lMsg);


void __RPC_STUB IWMXUtility_SetupProgressHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_SetupQueryAbort_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ long lWndAbortActive,
    /* [in] */ long __RPC_FAR *lpbAbort);


void __RPC_STUB IWMXUtility_SetupQueryAbort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXAudio2Wav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrAudioList,
    /* [in] */ IUnknown __RPC_FAR *pManager,
    /* [in] */ BSTR bstrMixerTimers,
    /* [in] */ long lBitsPerSample,
    /* [in] */ long lChannels,
    /* [in] */ long lSampleRate,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXAudio2Wav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXAudioFromWav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMXList,
    /* [in] */ IUnknown __RPC_FAR *pManager,
    /* [in] */ BSTR bstrWave,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXAudioFromWav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXDirectCopy_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrList,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXDirectCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXCopyWav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrList,
    /* [in] */ BSTR bstrWav,
    /* [in] */ BSTR bstrOrgProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXCopyWav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_Wav2WMX_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWav,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_Wav2WMX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WavFromWMX_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lBitsPerSample,
    /* [in] */ long lChannels,
    /* [in] */ long lSampleRate,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WavFromWMX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_IsWMXProtected_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pvbProtected);


void __RPC_STUB IWMXUtility_IsWMXProtected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_IsWMXProfileStreamCompatible_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX1,
    /* [in] */ BSTR bstrWMX2,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pvbAudio,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pvbVideo,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pvbScript);


void __RPC_STUB IWMXUtility_IsWMXProfileStreamCompatible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_IsWMXProfileCompatible_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX1,
    /* [in] */ BSTR bstrWMX2);


void __RPC_STUB IWMXUtility_IsWMXProfileCompatible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_IsWMXIndexExisted_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pvbExisted);


void __RPC_STUB IWMXUtility_IsWMXIndexExisted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_GetWMXDuration_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ double __RPC_FAR *pdmsLength);


void __RPC_STUB IWMXUtility_GetWMXDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_GetWMXProfileInfo_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ AUDIOPARAM __RPC_FAR *painfo,
    /* [out] */ VIDEOPARAM __RPC_FAR *pvinfo,
    /* [out] */ SCRIPTPARAM __RPC_FAR *psinfo);


void __RPC_STUB IWMXUtility_GetWMXProfileInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_GetWMXAudioPeak_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ long __RPC_FAR *plPeakMin,
    /* [out] */ long __RPC_FAR *plPeakMax,
    /* [out] */ long __RPC_FAR *plMin,
    /* [out] */ long __RPC_FAR *plMax);


void __RPC_STUB IWMXUtility_GetWMXAudioPeak_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_PRXGetParams_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrPRX,
    /* [in] */ PROFILEPARAMS __RPC_FAR *pParam);


void __RPC_STUB IWMXUtility_PRXGetParams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_PRXSubtract_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrPRX1,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrPRX2);


void __RPC_STUB IWMXUtility_PRXSubtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_PRXLower_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrPRX,
    /* [in] */ VARIANT_BOOL vbAudioLower,
    /* [in] */ long lAudioSampleRate,
    /* [in] */ long lAudioChannels,
    /* [in] */ long lAudioKiloBPS,
    /* [in] */ VARIANT_BOOL vbVideoLower,
    /* [in] */ long lVideoSecPerKey,
    /* [in] */ long lVideoQuality,
    /* [in] */ long lVideoFPS,
    /* [in] */ long lVideoKiloBPS,
    /* [in] */ VARIANT_BOOL vbScriptLower,
    /* [in] */ long lScriptKiloBPS,
    /* [in] */ BSTR bstrOutPRX);


void __RPC_STUB IWMXUtility_PRXLower_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_PRXFromWMX_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ long lFPS,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrOutPrx);


void __RPC_STUB IWMXUtility_PRXFromWMX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_PRXGen_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ PROFILEPARAMS __RPC_FAR *pParam,
    /* [in] */ BSTR bstrGenPRX);


void __RPC_STUB IWMXUtility_PRXGen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXMakeIndex_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX);


void __RPC_STUB IWMXUtility_WMXMakeIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudioMultiply_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lRatio_100,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXAudioMultiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudioNormalize_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lPercentage_100,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXAudioNormalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudioReplace_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrFile,
    /* [in] */ BSTR bstrReplace,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXAudioReplace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudioFromWav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ BSTR bstrWav,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXAudioFromWav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXConvert_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXConvert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXCopy_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_SpecWMXDumpScript_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ BSTR bstrOutTable);


void __RPC_STUB IWMXUtility_SpecWMXDumpScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_SpecWMXMakeAVScript_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ VARIANT_BOOL vbCopyHeader,
    /* [in] */ VARIANT_BOOL vbCopyScript,
    /* [in] */ VARIANT_BOOL vbCopyMarker,
    /* [in] */ BSTR bstrOutWMX,
    /* [in] */ BSTR bstrOutTable);


void __RPC_STUB IWMXUtility_SpecWMXMakeAVScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXMixer2Wav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWav,
    /* [in] */ BSTR bstrLists,
    /* [in] */ BSTR bstrOutWav);


void __RPC_STUB IWMXUtility_MultiWMXMixer2Wav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXFromImage_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrImage,
    /* [in] */ long lmsDuration,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXFromImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXFromMuteAudio_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ long lmsDuration,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXFromMuteAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXGetCoveredImages_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
    /* [in] */ long lmsTime,
    /* [in] */ BSTR bstrImagePrev,
    /* [in] */ BSTR bstrImageNext);


void __RPC_STUB IWMXUtility_WMXGetCoveredImages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXGetNextImage_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
    /* [in] */ long lmsTime,
    /* [in] */ BSTR bstrOutImage);


void __RPC_STUB IWMXUtility_WMXGetNextImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXGetPrevImage_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ ENUMIMAGEFORMAT_KIT eFormat,
    /* [in] */ long lmsTime,
    /* [in] */ BSTR bstrOutImage);


void __RPC_STUB IWMXUtility_WMXGetPrevImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMX_DBG_GetSampleCount_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [out] */ long __RPC_FAR *plAudio,
    /* [out] */ long __RPC_FAR *plVideo,
    /* [out] */ long __RPC_FAR *plScript);


void __RPC_STUB IWMXUtility_WMX_DBG_GetSampleCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXDirectAppend_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMXNameTable,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrOutWMX);


void __RPC_STUB IWMXUtility_MultiWMXDirectAppend_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXOutput_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrMain,
    /* [in] */ BSTR bstrAudioReplace,
    /* [in] */ BSTR bstrAudioMix,
    /* [in] */ IUnknown __RPC_FAR *pManager,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudio2Wav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lmsStart,
    /* [in] */ long lmsStop,
    /* [in] */ long lBitsPerSample,
    /* [in] */ long lChannels,
    /* [in] */ long lSampleRate,
    /* [in] */ BSTR bstrOutWav);


void __RPC_STUB IWMXUtility_WMXAudio2Wav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXDirectTrim_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lmsStart,
    /* [in] */ long lmsStop,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXDirectTrim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXConvertTrim_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ long lmsStart,
    /* [in] */ long lmsStop,
    /* [in] */ VARIANT_BOOL vbVideo,
    /* [in] */ VARIANT_BOOL vbScript,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXConvertTrim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MBRPrxGen_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ long lA1kbps,
    /* [in] */ long lV1kps,
    /* [in] */ long lwV1,
    /* [in] */ long lhV1,
    /* [in] */ long lA2kbps,
    /* [in] */ long lV2kps,
    /* [in] */ long lwV2,
    /* [in] */ long lhV2,
    /* [in] */ long lA3kbps,
    /* [in] */ long lV3kps,
    /* [in] */ long lwV3,
    /* [in] */ long lhV3,
    /* [in] */ BSTR bstrPrxOut);


void __RPC_STUB IWMXUtility_MBRPrxGen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_MultiWMXExport_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrList,
    /* [in] */ IUnknown __RPC_FAR *pAudioManager,
    /* [in] */ IUnknown __RPC_FAR *pVideoManager,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_MultiWMXExport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXReplaceAudio_Wav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrFile,
    /* [in] */ IUnknown __RPC_FAR *pManager,
    /* [in] */ BSTR bstrReplace,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IWMXUtility_WMXReplaceAudio_Wav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMXUtility_WMXAudioMixReplace2Wav_Proxy( 
    IWMXUtility __RPC_FAR * This,
    /* [in] */ BSTR bstrWMX,
    /* [in] */ BSTR bstrReplaceList,
    /* [in] */ BSTR bstrMixList,
    /* [in] */ long lBitsPerSample,
    /* [in] */ long lChannels,
    /* [in] */ long lSampleRate,
    /* [in] */ BSTR bstrOutputWav);


void __RPC_STUB IWMXUtility_WMXAudioMixReplace2Wav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMXUtility_INTERFACE_DEFINED__ */



#ifndef __MEDIAKITLib_LIBRARY_DEFINED__
#define __MEDIAKITLib_LIBRARY_DEFINED__

/* library MEDIAKITLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MEDIAKITLib;

EXTERN_C const CLSID CLSID_WMXUtility;

#ifdef __cplusplus

class DECLSPEC_UUID("3FD8AA90-1BB1-4FCB-90A3-0BB11AB9DA9A")
WMXUtility;
#endif
#endif /* __MEDIAKITLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
