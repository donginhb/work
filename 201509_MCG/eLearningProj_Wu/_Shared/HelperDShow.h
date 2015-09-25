#ifndef _HELPERDSHOW_H_
#define _HELPERDSHOW_H_

HRESULT MediaFileGetDuration(BSTR bstrMedia, double *pdDuration);
// System Device Enum
BOOL QuerySystemDeviceEnumMoniker(REFIID gCategory, IEnumMoniker **ppEmMoniker);

// Filter Graph
void WaitForGraphCompletion(IGraphBuilder *pGraph, long msTimeOut);
HRESULT FindPinOnFilter( IBaseFilter * pFilter, PIN_DIRECTION PinDir, DWORD dwPin, BOOL fConnected, IPin ** ppPin );
HRESULT GetPin(IBaseFilter *pFilter, DWORD dwPin, IPin **ppPin);
HRESULT GetPinDir( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
IPin * GetInPin( IBaseFilter * pFilter, int Num );
IPin * GetOutPin( IBaseFilter * pFilter, int Num );
BOOL SelectClock(IFilterGraph *pFG, IBaseFilter *pBaseFilter);
void DisconnectAll(IFilterGraph *pFG, IBaseFilter *pBF, IBaseFilter **ppReserveds, WORD wReservedCount);
HRESULT SetNoClock(IFilterGraph *pFG);
// DEBUG
HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveFromRot(DWORD pdwRegister);

#endif