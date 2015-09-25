#ifndef _WAVWRITER_H_
#define _WAVWRITER_H_

#include "WavWriterGuids.h"

// This is the COM object
class CWavWriterFilter : public IWavWriterConfig, public CBaseRenderer, public CCritSec
{
private:	
	LPWAVWRITER			m_pWavWriter;
	WAVEFORMATEX		m_wfx;
	long				m_lCurrSize, m_lSizeBound;
	LONGLONG			m_llStartTime;
public:
// Implements the IBaseFilter and IMediaFilter interfaces

    DECLARE_IUNKNOWN
		
// Implements the IWavWriterConfig interface
	STDMETHODIMP SetupStartTime(LONGLONG llStart) { m_llStartTime = llStart; return S_OK; }
	STDMETHODIMP SetupWavWriter(LPWAVWRITER pWavWriter) {m_pWavWriter = pWavWriter; return S_OK; }
	STDMETHODIMP SetupAudioFormat(long lBitsPerSample, long lChannels, long lSampleRate);
	STDMETHODIMP SetupSizeBound(long lBound);
// must be overrided member functions!!
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
	virtual HRESULT CheckMediaType(const CMediaType *);
	HRESULT SetMediaType(const CMediaType *pmt);
	
public:

    CWavWriterFilter(LPUNKNOWN pUnk,HRESULT *phr);
    virtual ~CWavWriterFilter();

    // This goes in the factory template table to create new instances
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN, HRESULT *);

    // This goes in the factory template table to create new instances
	STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);
}; // CWavWriterFilter

#endif