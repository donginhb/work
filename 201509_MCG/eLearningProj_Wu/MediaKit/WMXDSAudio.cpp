  #include <streams.h>          // quartz, includes windows
#pragma warning(disable: 4511 4512)

#include <initguid.h>
#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "WMXDSAudio.h"


//
// initialise the static instance count.
//!=
int CWMXDSAudio::m_nInstanceCount = 0;

CWMXDSAudio::CWMXDSAudio(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	: CTransInPlaceFilter (tszName, punk, CLSID_WMXDSAudio, phr)
{
    m_nThisInstance = ++m_nInstanceCount; // Useful for debug, no other purpose
}

CWMXDSAudio::~CWMXDSAudio()
{
}

CUnknown * WINAPI CWMXDSAudio::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    CWMXDSAudio *pNewObject = new CWMXDSAudio(NAME("WMXDSAudio Filter"), punk, phr);
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance

// NonDelegatingQueryInterface
//
// Override CUnknown method.
// Reveal our persistent stream, property pages and IVideoEffect interfaces.
// Anyone can call our private interface so long as they know the secret UUID.
STDMETHODIMP CWMXDSAudio::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CAutoLock cs(&m_cSharedState);

    CheckPointer(ppv,E_POINTER);

    return CTransInPlaceFilter::NonDelegatingQueryInterface(riid, ppv);
} // NonDelegatingQueryInterface

// Transform
//
// Override CTransInPlaceFilter method.
// Convert the input sample into the output sample.

HRESULT CWMXDSAudio::Transform(IMediaSample *pSample)
{
	CAutoLock cs(&m_cSharedState);

    return NOERROR;
} // Transform


// CheckInputType
//
// Override CTransformFilter method.
// Part of the Connect process.
// Ensure that we do not get connected to formats that we can't handle.
HRESULT CWMXDSAudio::CheckInputType(const CMediaType *pmt)
{
    CAutoLock cs(&m_cSharedState);

    DisplayType("CheckInputType", pmt);

    if (pmt->majortype != MEDIATYPE_Audio) 
        return VFW_E_TYPE_NOT_ACCEPTED;

    if (pmt->formattype != FORMAT_WaveFormatEx)
        return VFW_E_TYPE_NOT_ACCEPTED;
	
	WAVEFORMATEX *pwfx=(WAVEFORMATEX *)pmt->pbFormat;

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
} // CheckInputType