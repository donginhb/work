#ifndef _WMXDS_H_
#define _WMXDS_H_

// can not use mark in/out for seek fail.
class CWriterBase;
BOOL WMXConvertMix(BSTR bstrWMX, LPVOID pAudioManager, LPVOID pVideoManager, CWriterBase *pWriterBase);
BOOL WMXConvertByDS(BSTR bstrWMX, LPVOID pAudioManager, LPVOID pVideoManager, BSTR bstrProfile, BSTR bstrOutput);
BOOL WMXProcess(BSTR bstrWMX, BSTR bstrProfile, LPVOID pManager, DWORD dwmsTime, DWORD dwmsStart, DWORD dwmsStop, 
				CStringArray &ReplaceCstArray, CDWordArray &ReplaceTimeArray, CDWordArray &ReplaceStartArray, CDWordArray &ReplaceStopArray,
				CStringArray &MixCstArray, CDWordArray &MixTimeArray, CDWordArray &MixStartArray, CDWordArray &MixStopArray,
				BSTR bstrOutput);
#endif