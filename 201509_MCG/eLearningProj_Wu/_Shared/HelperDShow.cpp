#include <afxwin.h>
#include <streams.h>
#include <dshowasf.h>
#include <qedit.h>
#include <Atlbase.h>

#include "HelperDShow.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

HRESULT MediaFileGetDuration(BSTR bstrMedia, double *pdDuration)
{
	CComPtr< IMediaDet > pDet;
	HRESULT hr = CoCreateInstance(CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER, IID_IMediaDet, (void**)&pDet );
    if( FAILED( hr ) ) return E_FAIL;

    hr = pDet->put_Filename( bstrMedia );
    if( FAILED( hr ) ) return E_FAIL;

    long lStreamCount = 0;
    hr = pDet->get_OutputStreams( &lStreamCount );
    if( FAILED( hr ) ) return E_FAIL;

	double dLen=0, dTemp=0;
	for(long ii=0; ii<lStreamCount; ii++) {
		hr = pDet->put_CurrentStream(ii);
		if(SUCCEEDED(hr)) {
			hr = pDet->get_StreamLength(&dTemp);
			if(SUCCEEDED(hr)&&(dTemp>dLen)) dLen = dTemp;
		}
	}

	*pdDuration = dLen;

	return S_OK;
}

void WaitForGraphCompletion(IGraphBuilder *pGraph, long msTimeOut)
{
    HRESULT hr;
    LONG lEvCode = 0;
    IMediaEvent *pEvent;

    pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent);

    do
    {
        MSG Message;

        while(PeekMessage(&Message, NULL, 0, 0, TRUE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }

        hr = pEvent->WaitForCompletion(msTimeOut, &lEvCode);

    } while(lEvCode == 0);

    pEvent->Release();
}


HRESULT FindPinOnFilter( IBaseFilter * pFilter, PIN_DIRECTION PinDir, DWORD dwPin, BOOL fConnected, IPin ** ppPin )
{
	HRESULT			hr = S_OK;
	IEnumPins *		pEnumPin = NULL;
	IPin *			pConnectedPin = NULL;
	PIN_DIRECTION	PinDirection;
	ULONG			ulFetched;
	DWORD			nFound = 0;

	ASSERT( pFilter != NULL );
	*ppPin = NULL;

	hr = pFilter->EnumPins( &pEnumPin );
	if(SUCCEEDED(hr))
	{
		while ( S_OK == ( hr = pEnumPin->Next( 1L, ppPin, &ulFetched ) ) )
		{
			hr = (*ppPin)->ConnectedTo( &pConnectedPin );
			if (pConnectedPin)
			{
				pConnectedPin->Release();
				pConnectedPin = NULL;
			}
			if ( ( ( VFW_E_NOT_CONNECTED == hr ) && !fConnected ) ||
				 ( ( S_OK                == hr ) &&  fConnected ) )
			{
				hr = (*ppPin)->QueryDirection( &PinDirection );
				if ( ( S_OK == hr ) && ( PinDirection == PinDir ) )
				{
					if ( nFound == dwPin ) break;
					nFound++;
				}
			}
			(*ppPin)->Release();
		}
	}
	pEnumPin->Release();
	return hr;
} // FindPinOnFilter

HRESULT GetPin(IBaseFilter *pFilter, DWORD dwPin, IPin **ppPin)
{
    IEnumPins *pins=NULL;

    *ppPin = NULL;
    HRESULT hr = pFilter->EnumPins(&pins);
    if(FAILED(hr)) return hr;

    if(dwPin > 0) {
        hr = pins->Skip(dwPin);
        if(FAILED(hr)) {            
            pins->Release();
            return hr;
        }
    }

    DWORD n;
    hr = pins->Next(1, ppPin, &n);

    pins->Release();
    return hr;
}

HRESULT SetNoClock(IFilterGraph *graph)
{
    // Keep a useless clock from being instantiated....
    IMediaFilter *graphF=NULL;
    HRESULT hr = graph->QueryInterface(IID_IMediaFilter, (void **) &graphF);

    if(SUCCEEDED(hr)) {
        hr = graphF->SetSyncSource(NULL);
        graphF->Release();
    }

    return hr;
}

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker * pMoniker=NULL;
    IRunningObjectTable *pROT=NULL;
    if (FAILED(GetRunningObjectTable(0, &pROT))) {
        return E_FAIL;
    }
    WCHAR wsz[256];
    wsprintfW(wsz, L"FilterGraph %08p pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
    return hr;
}

void RemoveFromRot(DWORD pdwRegister)
{
	IRunningObjectTable *pROT=NULL;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}

BOOL QuerySystemDeviceEnumMoniker(REFIID gCategory, IEnumMoniker **ppEmMoniker)
{
	HRESULT hval=0;

    ICreateDevEnum *pCreateDevEnum=NULL;
    hval = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hval!=S_OK) return FALSE;
    
    hval = pCreateDevEnum->CreateClassEnumerator(gCategory, ppEmMoniker, 0);
    if(pCreateDevEnum!=NULL) pCreateDevEnum->Release();
    if(hval!=S_OK) return FALSE;
	
    return TRUE;
}

BOOL SelectClock(IFilterGraph *pFG, IBaseFilter *pBaseFilter)
{
	if((pFG==NULL)||(pBaseFilter==NULL)) return FALSE;

    HRESULT hval=0;
    IReferenceClock *pRefClock=NULL;
    IMediaFilter *pMediaFilter=NULL;
    
    hval = pBaseFilter->QueryInterface(IID_IReferenceClock, (void**)&pRefClock);
	if(FAILED(hval)) return FALSE;
    hval = pFG->QueryInterface(IID_IMediaFilter, (void**)&pMediaFilter);
	if(SUCCEEDED(hval)) {    
		// set the clock of the specified filter on the filtergraph builder
        hval = pMediaFilter->SetSyncSource(pRefClock);
    }
	RELEASE(pMediaFilter);
	RELEASE(pRefClock);

    return (SUCCEEDED(hval));
}       

void BreakConnectAll(IFilterGraph *pFG, IBaseFilter *pBF)
{	
    IPin *pP=NULL, *pTo=NULL;
    ULONG u=0;
    IEnumPins *pins=NULL;
    PIN_INFO pininfo;
	WORD 	 ii=0;
       
    HRESULT hr = pBF->EnumPins(&pins);
    pins->Reset();
    while(hr==S_OK) {
		hr = pins->Next(1, &pP, &u);
        if((hr==S_OK)&&(pP!=NULL)) {
            pP->ConnectedTo(&pTo);
            if(pTo!=NULL) {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr==S_OK) {
                    if(pininfo.dir==PINDIR_INPUT) {
                        BreakConnectAll(pFG, pininfo.pFilter);
                        hr = pFG->Disconnect(pTo);
                        hr = pFG->Disconnect(pP);
                    }
                    RELEASE(pininfo.pFilter);
                }
                RELEASE(pTo);
            }
            RELEASE(pP);
        }
    }

    RELEASE(pins);
}

void DisconnectAll(IFilterGraph *pFG, IBaseFilter *pBF, IBaseFilter **ppReserveds, WORD wReservedCount)
{
	ASSERT(pFG!=NULL);
	ASSERT(pBF!=NULL);

    IPin *pP=NULL, *pTo=NULL;
    ULONG u=0;
    IEnumPins *pins=NULL;
    PIN_INFO pininfo;
	WORD 	 ii=0;
       
    HRESULT hr = pBF->EnumPins(&pins);
    pins->Reset();
    while(hr==S_OK) {
		hr = pins->Next(1, &pP, &u);
        if((hr==S_OK)&&(pP!=NULL)) {
            pP->ConnectedTo(&pTo);
            if(pTo!=NULL) {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr==S_OK) {
                    if(pininfo.dir==PINDIR_INPUT) {
                        DisconnectAll(pFG, pininfo.pFilter, ppReserveds, wReservedCount);
                        hr = pFG->Disconnect(pTo);
                        hr = pFG->Disconnect(pP);
                        
						for(ii=0; ii<wReservedCount; ii++) //check reserved!!
							if(pininfo.pFilter==ppReserveds[ii]) break;

						if(ii>=wReservedCount) // NoMatch Reserved
                            hr = pFG->RemoveFilter(pininfo.pFilter);
                    }
                    ASSERT(pininfo.pFilter!=NULL);
                    RELEASE(pininfo.pFilter);
                }
                ASSERT(pTo!=NULL);
                RELEASE(pTo);
            }
            ASSERT(pP!=NULL);
            RELEASE(pP);
        }
    }

    RELEASE(pins);
}

HRESULT GetPinDir( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr< IEnumPins > pEnum;
    *ppPin = NULL;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) 
        return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;
    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;
        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;
                break;
            }
            iNum--;
        } // if
        pPin->Release();
    } // while

    return hr;
}


IPin * GetInPin( IBaseFilter * pFilter, int Num )
{
    IPin *pComPin=NULL;
    GetPinDir(pFilter, PINDIR_INPUT, Num, &pComPin);
    return pComPin;
}


IPin * GetOutPin( IBaseFilter * pFilter, int Num )
{
    IPin *pComPin=NULL;
    GetPinDir(pFilter, PINDIR_OUTPUT, Num, &pComPin);
    return pComPin;
}


HRESULT FindOtherSplitterPin(IPin *pPinIn, GUID guid, int nStream, IPin **ppSplitPin)
{
    DbgLog((LOG_TRACE,1,TEXT("FindOtherSplitterPin")));

    CheckPointer(ppSplitPin, E_POINTER);
    CComPtr< IPin > pPinOut;
    pPinOut = pPinIn;

    while(pPinOut)
    {
        PIN_INFO ThisPinInfo;
        pPinOut->QueryPinInfo(&ThisPinInfo);
        if(ThisPinInfo.pFilter) ThisPinInfo.pFilter->Release();

        pPinOut = NULL;
        CComPtr< IEnumPins > pEnumPins;
        ThisPinInfo.pFilter->EnumPins(&pEnumPins);
        if(!pEnumPins)
        {
            return NULL;
        }

        // look at every pin on the current filter...
        //
        ULONG Fetched = 0;
        while(1)
        {
            CComPtr< IPin > pPin;
            Fetched = 0;
            ASSERT(!pPin); // is it out of scope?
            pEnumPins->Next(1, &pPin, &Fetched);
            if(!Fetched)
            {
                break;
            }

            PIN_INFO pi;
            pPin->QueryPinInfo(&pi);
            if(pi.pFilter) pi.pFilter->Release();

            // if it's an input pin...
            //
            if(pi.dir == PINDIR_INPUT)
            {
                // continue searching upstream from this pin
                //
                pPin->ConnectedTo(&pPinOut);

                // a pin that supports the required media type is the
                // splitter pin we are looking for!  We are done
                //
            }
            else
            {
                CComPtr< IEnumMediaTypes > pMediaEnum;
                pPin->EnumMediaTypes(&pMediaEnum);
                if(pMediaEnum)
                {
                    Fetched = 0;
                    AM_MEDIA_TYPE *pMediaType;
                    pMediaEnum->Next(1, &pMediaType, &Fetched);
                    if(Fetched)
                    {
                        if(pMediaType->majortype == guid)
                        {
                            if(nStream-- == 0)
                            {
                                DeleteMediaType(pMediaType);
                                *ppSplitPin = pPin;
                                (*ppSplitPin)->AddRef();
                                DbgLog((LOG_TRACE,1,TEXT("Found SPLIT pin")));
                                return S_OK;
                            }
                        }
                        DeleteMediaType(pMediaType);
                    }
                }
            }

            // go try the next pin

        } // while
    }
    ASSERT(FALSE);
    return E_FAIL;
}