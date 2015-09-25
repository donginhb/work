#include <streams.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <initguid.h>
#include "WMXDSVideo.h"
#include "resource.h"

// Setup data
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
//
// CreateInstance
//
// This goes in the factory template table to create new instances
//
CUnknown * WINAPI CWMXDSVideoFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CWMXDSVideoFilter(pUnk, phr);

} // CreateInstance

STDMETHODIMP CWMXDSVideoFilter::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv,E_POINTER);
/*
    if (riid == IID_IWMXDSVideoConfig) 
        return GetInterface((IWMXDSVideoConfig *)this, ppv);
*/
    return CBaseRenderer::NonDelegatingQueryInterface(riid,ppv);

} // NonDelegatingQueryInterface

//
// Constructor
//
// Create the filter, WMXDSVideo window, and input pin
//
#pragma warning(disable:4355)
//
CWMXDSVideoFilter::CWMXDSVideoFilter(LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseRenderer(CLSID_WMXDSVideo, NAME("WMXDSVideo"), pUnk, phr)
{
} // (Constructor)


//
// Destructor
//
CWMXDSVideoFilter::~CWMXDSVideoFilter()
{
} // (Destructor)

//
// CheckMediaType
//
// Check that we can support a given proposed type
//
HRESULT CWMXDSVideoFilter::CheckMediaType(const CMediaType *pmt)
{
    if (pmt->majortype != MEDIATYPE_Video) 
        return VFW_E_TYPE_NOT_ACCEPTED;
	
    if (pmt->formattype != FORMAT_VideoInfo)
        return VFW_E_TYPE_NOT_ACCEPTED;
	
	VIDEOINFOHEADER *pvih=(VIDEOINFOHEADER *)pmt->pbFormat;
	if (pvih->bmiHeader.biBitCount!=24) 
		return VFW_E_TYPE_NOT_ACCEPTED;
	
    if (pvih->bmiHeader.biCompression != 0) 
		return VFW_E_TYPE_NOT_ACCEPTED;

    return NOERROR;
} // CheckMediaType

HRESULT CWMXDSVideoFilter::DoRenderSample(IMediaSample *pMediaSample)
{
    CAutoLock lock(this);

	return S_OK;
}
