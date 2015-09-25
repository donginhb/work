#ifndef _WMXDSAUDIO_H_
#define _WMXDSAUDIO_H_

// {EEC8D01E-95CE-4a1b-98F1-DBDCC9917A26}
DEFINE_GUID(CLSID_WMXDSAudio, 
			0xeec8d01e, 0x95ce, 0x4a1b, 0x98, 0xf1, 0xdb, 0xdc, 0xc9, 0x91, 0x7a, 0x26);

// This is the COM object
class CWMXDSAudioFilter : public CBaseRenderer, public CCritSec
{
public:
// Implements the IBaseFilter and IMediaFilter interfaces

    DECLARE_IUNKNOWN
		
// must be overrided member functions!!
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
	virtual HRESULT CheckMediaType(const CMediaType *);
	
public:

    CWMXDSAudioFilter(LPUNKNOWN pUnk,HRESULT *phr);
    virtual ~CWMXDSAudioFilter();

    // This goes in the factory template table to create new instances
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN, HRESULT *);

    // This goes in the factory template table to create new instances
	STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);
}; // CWMXDSAudioFilter

#endif