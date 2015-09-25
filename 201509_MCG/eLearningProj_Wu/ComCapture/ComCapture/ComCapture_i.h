

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Apr 21 18:04:30 2015
 */
/* Compiler settings for ComCapture.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __ComCapture_i_h__
#define __ComCapture_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEasyCapture_FWD_DEFINED__
#define __IEasyCapture_FWD_DEFINED__
typedef interface IEasyCapture IEasyCapture;
#endif 	/* __IEasyCapture_FWD_DEFINED__ */


#ifndef __EasyCapture_FWD_DEFINED__
#define __EasyCapture_FWD_DEFINED__

#ifdef __cplusplus
typedef class EasyCapture EasyCapture;
#else
typedef struct EasyCapture EasyCapture;
#endif /* __cplusplus */

#endif 	/* __EasyCapture_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_ComCapture_0000_0000 */
/* [local] */ 

typedef /* [public][public][public][public][public][public] */ 
enum __MIDL___MIDL_itf_ComCapture_0000_0000_0001
    {	CAPTUREDEVICETYPE_AUDIO	= 0,
	CAPTUREDEVICETYPE_VIDEO	= 1
    } 	CAPTUREDEVICETYPE;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_ComCapture_0000_0000_0002
    {	CAPTURESESSION_CAM	= 0,
	CAPTURESESSION_AUDIO	= 1,
	CAPTURESESSION_SCREEN	= 2,
	CAPTURESESSION_IPCAM	= 3
    } 	CAPTURESESSION;



extern RPC_IF_HANDLE __MIDL_itf_ComCapture_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ComCapture_0000_0000_v0_0_s_ifspec;

#ifndef __IEasyCapture_INTERFACE_DEFINED__
#define __IEasyCapture_INTERFACE_DEFINED__

/* interface IEasyCapture */
/* [unique][nonextensible][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEasyCapture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E6D0286B-CEDE-4B43-8764-099AEBAAC38B")
    IEasyCapture : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            long lReserved) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceTableUpdate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceTableGetCount( 
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceTableGetName( 
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lIndex,
            /* [out] */ BSTR *pbstr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceOpen( 
            /* [in] */ CAPTURESESSION eSession,
            /* [in] */ long lVideoIndex,
            /* [in] */ long lAudioIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceClose( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceFormatTableGetCount( 
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lDeviceIndex,
            /* [out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceFormatTableGetFormat( 
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lDeviceIndex,
            /* [in] */ long lFormatIndex,
            /* [out] */ BSTR *pbstr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VideoFrameRateTableGetCount( 
            /* [in] */ long lDeviceIndex,
            /* [out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VideoFrameRateTableGetFrameRate( 
            /* [in] */ long lDeviceIndex,
            /* [in] */ long lFrameRateIndex,
            /* [out] */ BSTR *pbstr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPCameraOpen( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrLogin,
            /* [in] */ BSTR bstrPassword) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PreviewPrepare( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CapturePrepare( 
            /* [in] */ BSTR bstrFile,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ long lAudioTimeshift,
            /* [in] */ VARIANT_BOOL vbSyncPreview) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoStart( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoStop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoPause( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoResume( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisplayOpen( 
            /* [in] */ long lwnd,
            /* [in] */ long xx,
            /* [in] */ long yy,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisplayClose( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupUserMsgHandler( 
            /* [in] */ long lWnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendIDScript( 
            /* [in] */ BSTR bstrID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRecordVolumeLevel( 
            /* [out] */ long *plLevel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCurrentCount( 
            /* [out] */ long *plCurrentCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsDigitalCamActive( 
            /* [out] */ VARIANT_BOOL *pvb) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeviceTableSearchName( 
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ BSTR bstrName,
            /* [out] */ long *plIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEasyCaptureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEasyCapture * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEasyCapture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEasyCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEasyCapture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEasyCapture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEasyCapture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEasyCapture * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IEasyCapture * This,
            long lReserved);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceTableUpdate )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceTableGetCount )( 
            IEasyCapture * This,
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceTableGetName )( 
            IEasyCapture * This,
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lIndex,
            /* [out] */ BSTR *pbstr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceOpen )( 
            IEasyCapture * This,
            /* [in] */ CAPTURESESSION eSession,
            /* [in] */ long lVideoIndex,
            /* [in] */ long lAudioIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceClose )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceFormatTableGetCount )( 
            IEasyCapture * This,
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lDeviceIndex,
            /* [out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceFormatTableGetFormat )( 
            IEasyCapture * This,
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ long lDeviceIndex,
            /* [in] */ long lFormatIndex,
            /* [out] */ BSTR *pbstr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VideoFrameRateTableGetCount )( 
            IEasyCapture * This,
            /* [in] */ long lDeviceIndex,
            /* [out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VideoFrameRateTableGetFrameRate )( 
            IEasyCapture * This,
            /* [in] */ long lDeviceIndex,
            /* [in] */ long lFrameRateIndex,
            /* [out] */ BSTR *pbstr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IPCameraOpen )( 
            IEasyCapture * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrLogin,
            /* [in] */ BSTR bstrPassword);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PreviewPrepare )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CapturePrepare )( 
            IEasyCapture * This,
            /* [in] */ BSTR bstrFile,
            /* [in] */ BSTR bstrProfile,
            /* [in] */ long lAudioTimeshift,
            /* [in] */ VARIANT_BOOL vbSyncPreview);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoStart )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoStop )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoPause )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoResume )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisplayOpen )( 
            IEasyCapture * This,
            /* [in] */ long lwnd,
            /* [in] */ long xx,
            /* [in] */ long yy,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisplayClose )( 
            IEasyCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetupUserMsgHandler )( 
            IEasyCapture * This,
            /* [in] */ long lWnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendIDScript )( 
            IEasyCapture * This,
            /* [in] */ BSTR bstrID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRecordVolumeLevel )( 
            IEasyCapture * This,
            /* [out] */ long *plLevel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentCount )( 
            IEasyCapture * This,
            /* [out] */ long *plCurrentCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsDigitalCamActive )( 
            IEasyCapture * This,
            /* [out] */ VARIANT_BOOL *pvb);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeviceTableSearchName )( 
            IEasyCapture * This,
            /* [in] */ CAPTUREDEVICETYPE eType,
            /* [in] */ BSTR bstrName,
            /* [out] */ long *plIndex);
        
        END_INTERFACE
    } IEasyCaptureVtbl;

    interface IEasyCapture
    {
        CONST_VTBL struct IEasyCaptureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEasyCapture_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEasyCapture_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEasyCapture_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEasyCapture_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEasyCapture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEasyCapture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEasyCapture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEasyCapture_Open(This,lReserved)	\
    ( (This)->lpVtbl -> Open(This,lReserved) ) 

#define IEasyCapture_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IEasyCapture_DeviceTableUpdate(This)	\
    ( (This)->lpVtbl -> DeviceTableUpdate(This) ) 

#define IEasyCapture_DeviceTableGetCount(This,eType,plCount)	\
    ( (This)->lpVtbl -> DeviceTableGetCount(This,eType,plCount) ) 

#define IEasyCapture_DeviceTableGetName(This,eType,lIndex,pbstr)	\
    ( (This)->lpVtbl -> DeviceTableGetName(This,eType,lIndex,pbstr) ) 

#define IEasyCapture_DeviceOpen(This,eSession,lVideoIndex,lAudioIndex)	\
    ( (This)->lpVtbl -> DeviceOpen(This,eSession,lVideoIndex,lAudioIndex) ) 

#define IEasyCapture_DeviceClose(This)	\
    ( (This)->lpVtbl -> DeviceClose(This) ) 

#define IEasyCapture_DeviceFormatTableGetCount(This,eType,lDeviceIndex,plCount)	\
    ( (This)->lpVtbl -> DeviceFormatTableGetCount(This,eType,lDeviceIndex,plCount) ) 

#define IEasyCapture_DeviceFormatTableGetFormat(This,eType,lDeviceIndex,lFormatIndex,pbstr)	\
    ( (This)->lpVtbl -> DeviceFormatTableGetFormat(This,eType,lDeviceIndex,lFormatIndex,pbstr) ) 

#define IEasyCapture_VideoFrameRateTableGetCount(This,lDeviceIndex,plCount)	\
    ( (This)->lpVtbl -> VideoFrameRateTableGetCount(This,lDeviceIndex,plCount) ) 

#define IEasyCapture_VideoFrameRateTableGetFrameRate(This,lDeviceIndex,lFrameRateIndex,pbstr)	\
    ( (This)->lpVtbl -> VideoFrameRateTableGetFrameRate(This,lDeviceIndex,lFrameRateIndex,pbstr) ) 

#define IEasyCapture_IPCameraOpen(This,bstrURL,bstrLogin,bstrPassword)	\
    ( (This)->lpVtbl -> IPCameraOpen(This,bstrURL,bstrLogin,bstrPassword) ) 

#define IEasyCapture_PreviewPrepare(This)	\
    ( (This)->lpVtbl -> PreviewPrepare(This) ) 

#define IEasyCapture_CapturePrepare(This,bstrFile,bstrProfile,lAudioTimeshift,vbSyncPreview)	\
    ( (This)->lpVtbl -> CapturePrepare(This,bstrFile,bstrProfile,lAudioTimeshift,vbSyncPreview) ) 

#define IEasyCapture_DoStart(This)	\
    ( (This)->lpVtbl -> DoStart(This) ) 

#define IEasyCapture_DoStop(This)	\
    ( (This)->lpVtbl -> DoStop(This) ) 

#define IEasyCapture_DoPause(This)	\
    ( (This)->lpVtbl -> DoPause(This) ) 

#define IEasyCapture_DoResume(This)	\
    ( (This)->lpVtbl -> DoResume(This) ) 

#define IEasyCapture_DisplayOpen(This,lwnd,xx,yy,lWidth,lHeight)	\
    ( (This)->lpVtbl -> DisplayOpen(This,lwnd,xx,yy,lWidth,lHeight) ) 

#define IEasyCapture_DisplayClose(This)	\
    ( (This)->lpVtbl -> DisplayClose(This) ) 

#define IEasyCapture_SetupUserMsgHandler(This,lWnd)	\
    ( (This)->lpVtbl -> SetupUserMsgHandler(This,lWnd) ) 

#define IEasyCapture_SendIDScript(This,bstrID)	\
    ( (This)->lpVtbl -> SendIDScript(This,bstrID) ) 

#define IEasyCapture_GetRecordVolumeLevel(This,plLevel)	\
    ( (This)->lpVtbl -> GetRecordVolumeLevel(This,plLevel) ) 

#define IEasyCapture_GetCurrentCount(This,plCurrentCount)	\
    ( (This)->lpVtbl -> GetCurrentCount(This,plCurrentCount) ) 

#define IEasyCapture_IsDigitalCamActive(This,pvb)	\
    ( (This)->lpVtbl -> IsDigitalCamActive(This,pvb) ) 

#define IEasyCapture_DeviceTableSearchName(This,eType,bstrName,plIndex)	\
    ( (This)->lpVtbl -> DeviceTableSearchName(This,eType,bstrName,plIndex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEasyCapture_INTERFACE_DEFINED__ */



#ifndef __ComCaptureLib_LIBRARY_DEFINED__
#define __ComCaptureLib_LIBRARY_DEFINED__

/* library ComCaptureLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ComCaptureLib;

EXTERN_C const CLSID CLSID_EasyCapture;

#ifdef __cplusplus

class DECLSPEC_UUID("90678C8F-19AA-4725-874C-3D749A8F6D3C")
EasyCapture;
#endif
#endif /* __ComCaptureLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


