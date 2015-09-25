/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 30 10:27:50 2012
 */
/* Compiler settings for C:\MCG5.0\eLearningProj_Wu\MediaFile\MediaFile.idl:
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

#ifndef __MediaFile_h__
#define __MediaFile_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMediaConvert_FWD_DEFINED__
#define __IMediaConvert_FWD_DEFINED__
typedef interface IMediaConvert IMediaConvert;
#endif 	/* __IMediaConvert_FWD_DEFINED__ */


#ifndef __MediaConvert_FWD_DEFINED__
#define __MediaConvert_FWD_DEFINED__

#ifdef __cplusplus
typedef class MediaConvert MediaConvert;
#else
typedef struct MediaConvert MediaConvert;
#endif /* __cplusplus */

#endif 	/* __MediaConvert_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_MediaFile_0000 */
/* [local] */ 

typedef /* [public][public] */ struct  __MIDL___MIDL_itf_MediaFile_0000_0001
    {
    DWORD dwBitrate;
    DWORD dwFormatTag;
    DWORD dwSampleRate;
    DWORD dwChannels;
    }	MF_AUDIOPARAMS;

typedef /* [public][public] */ struct  __MIDL___MIDL_itf_MediaFile_0000_0002
    {
    DWORD dwFourCC;
    DWORD dwBitrate;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwFPS;
    DWORD dwQuality;
    DWORD dwSecPerKey;
    }	MF_VIDEOPARAMS;



extern RPC_IF_HANDLE __MIDL_itf_MediaFile_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_MediaFile_0000_v0_0_s_ifspec;

#ifndef __IMediaConvert_INTERFACE_DEFINED__
#define __IMediaConvert_INTERFACE_DEFINED__

/* interface IMediaConvert */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMediaConvert;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FD1C87EB-98A9-4D23-A4CE-203D9D83FBF2")
    IMediaConvert : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupProgressHandler( 
            /* [in] */ long lwnd,
            /* [in] */ long lMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupQueryAbort( 
            /* [in] */ long lWndAbortActive,
            /* [in] */ long __RPC_FAR *lpbAbort) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MediaGetDuration( 
            /* [in] */ BSTR bstrMedia,
            /* [out] */ double __RPC_FAR *pdmsDuration) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Media2WMV_Param( 
            /* [in] */ BSTR bstrMedia,
            /* [in] */ long lAudioKiloBPS,
            /* [in] */ long lVideoKiloBPS,
            /* [in] */ BSTR bstrOutWMX,
            /* [in] */ BSTR bstrOutProfile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Media2WMV( 
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutWMX) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Media2WMA( 
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutWMA) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Media2WMA_Param( 
            /* [in] */ BSTR bstrMedia,
            /* [in] */ long lKiloBPS,
            /* [in] */ BSTR bstrOutWMA,
            /* [in] */ BSTR bstrOutProfile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MediaGetParams( 
            /* [in] */ BSTR bstrMedia,
            /* [out] */ MF_AUDIOPARAMS __RPC_FAR *pam,
            /* [out] */ MF_VIDEOPARAMS __RPC_FAR *pvm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WMV2WMV_MBR( 
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrMBRProfile,
            /* [in] */ BSTR bstrOutput) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaConvertVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaConvert __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaConvert __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMediaConvert __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetupProgressHandler )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ long lwnd,
            /* [in] */ long lMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetupQueryAbort )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ long lWndAbortActive,
            /* [in] */ long __RPC_FAR *lpbAbort);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MediaGetDuration )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [out] */ double __RPC_FAR *pdmsDuration);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Media2WMV_Param )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [in] */ long lAudioKiloBPS,
            /* [in] */ long lVideoKiloBPS,
            /* [in] */ BSTR bstrOutWMX,
            /* [in] */ BSTR bstrOutProfile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Media2WMV )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutWMX);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Media2WMA )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ BSTR bstrOutWMA);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Media2WMA_Param )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [in] */ long lKiloBPS,
            /* [in] */ BSTR bstrOutWMA,
            /* [in] */ BSTR bstrOutProfile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MediaGetParams )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [out] */ MF_AUDIOPARAMS __RPC_FAR *pam,
            /* [out] */ MF_VIDEOPARAMS __RPC_FAR *pvm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WMV2WMV_MBR )( 
            IMediaConvert __RPC_FAR * This,
            /* [in] */ BSTR bstrMedia,
            /* [in] */ BSTR bstrMBRProfile,
            /* [in] */ BSTR bstrOutput);
        
        END_INTERFACE
    } IMediaConvertVtbl;

    interface IMediaConvert
    {
        CONST_VTBL struct IMediaConvertVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaConvert_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaConvert_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaConvert_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaConvert_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMediaConvert_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMediaConvert_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMediaConvert_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMediaConvert_SetupProgressHandler(This,lwnd,lMsg)	\
    (This)->lpVtbl -> SetupProgressHandler(This,lwnd,lMsg)

#define IMediaConvert_SetupQueryAbort(This,lWndAbortActive,lpbAbort)	\
    (This)->lpVtbl -> SetupQueryAbort(This,lWndAbortActive,lpbAbort)

#define IMediaConvert_MediaGetDuration(This,bstrMedia,pdmsDuration)	\
    (This)->lpVtbl -> MediaGetDuration(This,bstrMedia,pdmsDuration)

#define IMediaConvert_Media2WMV_Param(This,bstrMedia,lAudioKiloBPS,lVideoKiloBPS,bstrOutWMX,bstrOutProfile)	\
    (This)->lpVtbl -> Media2WMV_Param(This,bstrMedia,lAudioKiloBPS,lVideoKiloBPS,bstrOutWMX,bstrOutProfile)

#define IMediaConvert_Media2WMV(This,bstrMedia,bstrProfile,bstrOutWMX)	\
    (This)->lpVtbl -> Media2WMV(This,bstrMedia,bstrProfile,bstrOutWMX)

#define IMediaConvert_Media2WMA(This,bstrMedia,bstrProfile,bstrOutWMA)	\
    (This)->lpVtbl -> Media2WMA(This,bstrMedia,bstrProfile,bstrOutWMA)

#define IMediaConvert_Media2WMA_Param(This,bstrMedia,lKiloBPS,bstrOutWMA,bstrOutProfile)	\
    (This)->lpVtbl -> Media2WMA_Param(This,bstrMedia,lKiloBPS,bstrOutWMA,bstrOutProfile)

#define IMediaConvert_MediaGetParams(This,bstrMedia,pam,pvm)	\
    (This)->lpVtbl -> MediaGetParams(This,bstrMedia,pam,pvm)

#define IMediaConvert_WMV2WMV_MBR(This,bstrMedia,bstrMBRProfile,bstrOutput)	\
    (This)->lpVtbl -> WMV2WMV_MBR(This,bstrMedia,bstrMBRProfile,bstrOutput)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_SetupProgressHandler_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ long lwnd,
    /* [in] */ long lMsg);


void __RPC_STUB IMediaConvert_SetupProgressHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_SetupQueryAbort_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ long lWndAbortActive,
    /* [in] */ long __RPC_FAR *lpbAbort);


void __RPC_STUB IMediaConvert_SetupQueryAbort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_MediaGetDuration_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [out] */ double __RPC_FAR *pdmsDuration);


void __RPC_STUB IMediaConvert_MediaGetDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_Media2WMV_Param_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [in] */ long lAudioKiloBPS,
    /* [in] */ long lVideoKiloBPS,
    /* [in] */ BSTR bstrOutWMX,
    /* [in] */ BSTR bstrOutProfile);


void __RPC_STUB IMediaConvert_Media2WMV_Param_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_Media2WMV_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutWMX);


void __RPC_STUB IMediaConvert_Media2WMV_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_Media2WMA_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [in] */ BSTR bstrProfile,
    /* [in] */ BSTR bstrOutWMA);


void __RPC_STUB IMediaConvert_Media2WMA_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_Media2WMA_Param_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [in] */ long lKiloBPS,
    /* [in] */ BSTR bstrOutWMA,
    /* [in] */ BSTR bstrOutProfile);


void __RPC_STUB IMediaConvert_Media2WMA_Param_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_MediaGetParams_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [out] */ MF_AUDIOPARAMS __RPC_FAR *pam,
    /* [out] */ MF_VIDEOPARAMS __RPC_FAR *pvm);


void __RPC_STUB IMediaConvert_MediaGetParams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMediaConvert_WMV2WMV_MBR_Proxy( 
    IMediaConvert __RPC_FAR * This,
    /* [in] */ BSTR bstrMedia,
    /* [in] */ BSTR bstrMBRProfile,
    /* [in] */ BSTR bstrOutput);


void __RPC_STUB IMediaConvert_WMV2WMV_MBR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaConvert_INTERFACE_DEFINED__ */



#ifndef __MEDIAFILELib_LIBRARY_DEFINED__
#define __MEDIAFILELib_LIBRARY_DEFINED__

/* library MEDIAFILELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MEDIAFILELib;

EXTERN_C const CLSID CLSID_MediaConvert;

#ifdef __cplusplus

class DECLSPEC_UUID("5C366CA7-C3F5-4383-866B-CB8ECCAFE5C3")
MediaConvert;
#endif
#endif /* __MEDIAFILELib_LIBRARY_DEFINED__ */

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
