  #include <streams.h>          // quartz, includes windows
#pragma warning(disable: 4511 4512)

#include <initguid.h>
#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "WMXDSVideo.h"

//
// initialise the static instance count.
//!=
int CWMXDSVideo::m_nInstanceCount = 0;

CWMXDSVideo::CWMXDSVideo(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	: CTransInPlaceFilter (tszName, punk, CLSID_WMXDSVideo, phr)
{
    m_nThisInstance = ++m_nInstanceCount; // Useful for debug, no other purpose
}

CWMXDSVideo::~CWMXDSVideo()
{
}

CUnknown * WINAPI CWMXDSVideo::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    CWMXDSVideo *pNewObject = new CWMXDSVideo(NAME("WMXDSVideo Filter"), punk, phr);
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
STDMETHODIMP CWMXDSVideo::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CAutoLock cs(&m_cSharedState);

    CheckPointer(ppv,E_POINTER);

    return CTransInPlaceFilter::NonDelegatingQueryInterface(riid, ppv);
} // NonDelegatingQueryInterface

// Transform
//
// Override CTransInPlaceFilter method.
// Convert the input sample into the output sample.

HRESULT CWMXDSVideo::Transform(IMediaSample *pSample)
{
	CAutoLock cs(&m_cSharedState);

    return NOERROR;
} // Transform


// CheckInputType
//
// Override CTransformFilter method.
// Part of the Connect process.
// Ensure that we do not get connected to formats that we can't handle.
HRESULT CWMXDSVideo::CheckInputType(const CMediaType *pmt)
{
    CAutoLock cs(&m_cSharedState);

    DisplayType("CheckInputType", pmt);

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
} // CheckInputType