#include <streams.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <initguid.h>
#include "WMXDSAudio.h"
#include "resource.h"

// Setup data
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
//
// CreateInstance
//
// This goes in the factory template table to create new instances
//
CUnknown * WINAPI CWMXDSAudioFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CWMXDSAudioFilter(pUnk, phr);

} // CreateInstance

STDMETHODIMP CWMXDSAudioFilter::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv,E_POINTER);
/*
    if (riid == IID_IWMXDSAudioConfig) 
        return GetInterface((IWMXDSAudioConfig *)this, ppv);
*/
    return CBaseRenderer::NonDelegatingQueryInterface(riid,ppv);

} // NonDelegatingQueryInterface

//
// Constructor
//
// Create the filter, WMXDSAudio window, and input pin
//
#pragma warning(disable:4355)
//
CWMXDSAudioFilter::CWMXDSAudioFilter(LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseRenderer(CLSID_WMXDSAudio, NAME("WMXDSAudio"), pUnk, phr)
{
} // (Constructor)


//
// Destructor
//
CWMXDSAudioFilter::~CWMXDSAudioFilter()
{
} // (Destructor)

//
// CheckMediaType
//
// Check that we can support a given proposed type
//
HRESULT CWMXDSAudioFilter::CheckMediaType(const CMediaType *pmt)
{
    if (pmt->majortype != MEDIATYPE_Audio) {
        return E_INVALIDARG;
    }

    if (pmt->formattype != FORMAT_WaveFormatEx) {
        return E_INVALIDARG;
    }

	WAVEFORMATEX *pwfx = (WAVEFORMATEX *) pmt->Format();
	
    if (pwfx == NULL)
        return E_INVALIDARG;
	
    if (pwfx->wFormatTag != WAVE_FORMAT_PCM) {
        return E_INVALIDARG;
    }

	if (pwfx->wBitsPerSample != 16) {
		return E_INVALIDARG;
	}

	if (pwfx->nChannels != 2) {
		return E_INVALIDARG;
	}

	if (pwfx->nSamplesPerSec != 44100) {
		return E_INVALIDARG;
	}

    return NOERROR;

} // CheckMediaType

HRESULT CWMXDSAudioFilter::DoRenderSample(IMediaSample *pMediaSample)
{
    CAutoLock lock(this);

	return S_OK;
}
