#ifndef _WAVWRITERGUIDS_H_
#define _WAVWRITERGUIDS_H_

// {B1E581DC-8DAC-4f35-8FB4-C32742CBBD71}
DEFINE_GUID(CLSID_WavWriter, 
0xb1e581dc, 0x8dac, 0x4f35, 0x8f, 0xb4, 0xc3, 0x27, 0x42, 0xcb, 0xbd, 0x71);

// {8C6DCEDA-5E70-4820-B9A2-4644576BD649}
DEFINE_GUID(IID_IWavWriterConfig, 
0x8c6dceda, 0x5e70, 0x4820, 0xb9, 0xa2, 0x46, 0x44, 0x57, 0x6b, 0xd6, 0x49);

#include "WaveLib\HelperWav.h"
DECLARE_INTERFACE_(IWavWriterConfig, IUnknown)
{
	STDMETHOD(SetupStartTime)(LONGLONG llStart) PURE;
	STDMETHOD(SetupWavWriter)(/*in*/LPWAVWRITER pWavWriter) PURE;
	STDMETHOD(SetupAudioFormat)(/*in*/long lBitsPerSample, /*[in]*/long lChannels, /*[in]*/long lSampleRate) PURE;	
	STDMETHOD(SetupSizeBound)(/*in*/long lBound) PURE;
};

typedef IWavWriterConfig* LPWAVWRITERCONFIG;

BOOL WMX2Wav(BSTR bstrWMX, long lmsStart, long lmsStop, LPWAVWRITER pWavWriter);
#endif