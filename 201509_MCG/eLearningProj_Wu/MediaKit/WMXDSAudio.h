#ifndef _WMXDSAUDIO_H_
#define _WMXDSAUDIO_H_

// {4A9DF9C7-1F5D-4e81-A68F-CED88A428EAB}
DEFINE_GUID(CLSID_WMXDSAudio, 
			0x4a9df9c7, 0x1f5d, 0x4e81, 0xa6, 0x8f, 0xce, 0xd8, 0x8a, 0x42, 0x8e, 0xab);

class CWMXDSAudio: public CTransInPlaceFilter
{
private:
	CCritSec		m_cSharedState;
	
    // If there are multiple instances of this filter active, it's
    // useful for debug messages etc. to know which one this is.
    // This variable has no other purpose.
    static int			m_nInstanceCount;                   // total instances
    int					m_nThisInstance;
public:
    CWMXDSAudio(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
	~CWMXDSAudio();
public:
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	
    DECLARE_IUNKNOWN;
	
    HRESULT CheckInputType(const CMediaType *mtIn);
	
	// --- CTransInPlaceFilter Overrides --
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);	
	
    // Overrides the PURE virtual Transform of CTransInPlaceFilter base class
    // This is where the "real work" is done.
    HRESULT Transform(IMediaSample *pSample);	
};


#endif