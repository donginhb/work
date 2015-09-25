#include <streams.h>
#include <initguid.h>
#include "inftee_AV.h"
#include <tchar.h>
#include <stdio.h>
#define INFTEE_MAX_PINS 1000

// Using this pointer in constructor
#pragma warning(disable:4355 4127)

//
// CreateInstance
//
// Creator function for the class ID
//
CUnknown * WINAPI CAVTee::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CAVTee(NAME("Infinite Tee Filter"), pUnk, phr);
}


//
// Constructor
//
CAVTee::CAVTee(TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr) :
    m_OutputPinsList(NAME("Tee Output Pins list")),
    m_lCanSeek(TRUE),
    m_pAllocator(NULL),
    m_NumOutputPins(0),
    m_NextOutputPinNumber(0),
    m_Input(NAME("Input Pin"), this, phr, L"Input"),
    CBaseFilter(NAME("Tee filter"), pUnk, this, CLSID_AVTee)
{
    ASSERT(phr);

    // Create a single output pin at this time
    InitOutputPinsList();

    CAVTeeOutputPin *pOutputPin = CreateNextOutputPin(this);

    if (pOutputPin != NULL )
    {
        m_NumOutputPins++;
        m_OutputPinsList.AddTail(pOutputPin);
    }
}


//
// Destructor
//
CAVTee::~CAVTee()
{
    InitOutputPinsList();
}


HRESULT CAVTee::PresetMediaType(const GUID *ptype, const GUID *psubtype)
{
	m_Input.m_intype = GUID_NULL;
	m_Input.m_insubtype = GUID_NULL;

	if(ptype) m_Input.m_intype = *ptype;
	if(psubtype) m_Input.m_insubtype = *psubtype;

	return S_OK;
}

//
// GetPinCount
//
int CAVTee::GetPinCount()
{
    return (1 + m_NumOutputPins);
}


//
// GetPin
//
CBasePin *CAVTee::GetPin(int n)
{
    if (n < 0)
        return NULL ;

    // Pin zero is the one and only input pin
    if (n == 0)
        return &m_Input;

    // return the output pin at position(n - 1) (zero based)
    return GetPinNFromList(n - 1);
}


//
// InitOutputPinsList
//
void CAVTee::InitOutputPinsList()
{
    POSITION pos = m_OutputPinsList.GetHeadPosition();

    while(pos)
    {
        CAVTeeOutputPin *pOutputPin = m_OutputPinsList.GetNext(pos);
        ASSERT(pOutputPin->m_pOutputQueue == NULL);
        pOutputPin->Release();
    }

    m_NumOutputPins = 0;
    m_OutputPinsList.RemoveAll();

} // InitOutputPinsList


//
// CreateNextOutputPin
//
CAVTeeOutputPin *CAVTee::CreateNextOutputPin(CAVTee *pTee)
{
    WCHAR szbuf[20];             // Temporary scratch buffer
    m_NextOutputPinNumber++;     // Next number to use for pin
    HRESULT hr = NOERROR;

    swprintf(szbuf, L"Output%d\0", m_NextOutputPinNumber);

    CAVTeeOutputPin *pPin = new CAVTeeOutputPin(NAME("Tee Output"), pTee,
                                            &hr, szbuf,
                                            m_NextOutputPinNumber);

    if (FAILED(hr) || pPin == NULL) 
    {
        delete pPin;
        return NULL;
    }

    pPin->AddRef();
    return pPin;

} // CreateNextOutputPin


//
// DeleteOutputPin
//
void CAVTee::DeleteOutputPin(CAVTeeOutputPin *pPin)
{
    ASSERT(pPin);
    if (!pPin)
        return;
        
    POSITION pos = m_OutputPinsList.GetHeadPosition();

    while(pos) 
    {
        POSITION posold = pos;         // Remember this position
        CAVTeeOutputPin *pOutputPin = m_OutputPinsList.GetNext(pos);

        if (pOutputPin == pPin) 
        {
            // If this pin holds the seek interface release it
            if (pPin->m_bHoldsSeek) 
            {
                InterlockedExchange(&m_lCanSeek, FALSE);
                pPin->m_bHoldsSeek = FALSE;
                pPin->m_pPosition->Release();
            }

            m_OutputPinsList.Remove(posold);
            ASSERT(pOutputPin->m_pOutputQueue == NULL);
            delete pPin;

            m_NumOutputPins--;
            IncrementPinVersion();
            break;
        }
    }

} // DeleteOutputPin


//
// GetNumFreePins
//
int CAVTee::GetNumFreePins()
{
    int n = 0;
    POSITION pos = m_OutputPinsList.GetHeadPosition();

    while(pos) 
    {
        CAVTeeOutputPin *pOutputPin = m_OutputPinsList.GetNext(pos);

        if (pOutputPin && pOutputPin->m_Connected == NULL)
            n++;
    }

    return n;

} // GetNumFreePins


//
// GetPinNFromList
//
CAVTeeOutputPin *CAVTee::GetPinNFromList(int n)
{
    // Validate the position being asked for
    if (n >= m_NumOutputPins)
        return NULL;

    // Get the head of the list
    POSITION pos = m_OutputPinsList.GetHeadPosition();

    n++;       // Make the number 1 based

    CAVTeeOutputPin *pOutputPin=0;
    while(n) 
    {
        pOutputPin = m_OutputPinsList.GetNext(pos);
        n--;
    }

    return pOutputPin;

} // GetPinNFromList


//
// Stop
//
// Overriden to handle no input connections
//
STDMETHODIMP CAVTee::Stop()
{
    CBaseFilter::Stop();
    m_State = State_Stopped;

    return NOERROR;
}


//
// Pause
//
// Overriden to handle no input connections
//
STDMETHODIMP CAVTee::Pause()
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = CBaseFilter::Pause();

    if (m_Input.IsConnected() == FALSE) 
    {
        m_Input.EndOfStream();
    }

    return hr;
}


//
// Run
//
// Overriden to handle no input connections
//
STDMETHODIMP CAVTee::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = CBaseFilter::Run(tStart);

    if (m_Input.IsConnected() == FALSE) 
    {
        m_Input.EndOfStream();
    }

    return hr;
}

//
// CAVTeeInputPin constructor
//
CAVTeeInputPin::CAVTeeInputPin(TCHAR   *pName,
                           CAVTee    *pTee,
                           HRESULT *phr,
                           LPCWSTR pPinName) :
    CBaseInputPin(pName, pTee, pTee, phr, pPinName),
    m_pTee(pTee),
    m_bInsideCheckMediaType(FALSE)
{
	m_intype = GUID_NULL;
	m_insubtype = GUID_NULL;
		
    ASSERT(pTee);
}


#ifdef DEBUG
//
// CAVTeeInputPin destructor
//
CAVTeeInputPin::~CAVTeeInputPin()
{
    DbgLog((LOG_TRACE,2,TEXT("CAVTeeInputPin destructor")));
    ASSERT(m_pTee->m_pAllocator == NULL);
}
#endif


#ifdef DEBUG
//
// DisplayMediaType -- (DEBUG ONLY)
//
void DisplayMediaType(TCHAR *pDescription, const CMediaType *pmt)
{
    ASSERT(pmt);
    if (!pmt)
        return;
        
    // Dump the GUID types and a short description

    DbgLog((LOG_TRACE,2,TEXT("")));
    DbgLog((LOG_TRACE,2,TEXT("%s"),pDescription));
    DbgLog((LOG_TRACE,2,TEXT("")));
    DbgLog((LOG_TRACE,2,TEXT("Media Type Description")));
    DbgLog((LOG_TRACE,2,TEXT("Major type %s"),GuidNames[*pmt->Type()]));
    DbgLog((LOG_TRACE,2,TEXT("Subtype %s"),GuidNames[*pmt->Subtype()]));
    DbgLog((LOG_TRACE,2,TEXT("Subtype description %s"),GetSubtypeName(pmt->Subtype())));
    DbgLog((LOG_TRACE,2,TEXT("Format size %d"),pmt->cbFormat));

    // Dump the generic media types */

    DbgLog((LOG_TRACE,2,TEXT("Fixed size sample %d"),pmt->IsFixedSize()));
    DbgLog((LOG_TRACE,2,TEXT("Temporal compression %d"),pmt->IsTemporalCompressed()));
    DbgLog((LOG_TRACE,2,TEXT("Sample size %d"),pmt->GetSampleSize()));


} // DisplayMediaType
#endif


//
// CheckMediaType
//
HRESULT CAVTeeInputPin::CheckMediaType(const CMediaType *pmt)
{
    CAutoLock lock_it(m_pLock);

    // If we are already inside checkmedia type for this pin, return NOERROR
    // It is possble to hookup two of the tee filters and some other filter
    // like the video effects sample to get into this situation. If we don't
    // detect this situation, we will carry on looping till we blow the stack

    if (m_bInsideCheckMediaType == TRUE)
        return NOERROR;

    m_bInsideCheckMediaType = TRUE;

	if((m_intype!=GUID_NULL)&&(pmt->majortype!=m_intype))
		return E_INVALIDARG;
	
	if((m_insubtype!=GUID_NULL)&&(pmt->subtype!=m_insubtype))
		return E_INVALIDARG;
	
    HRESULT hr = NOERROR;

#ifdef DEBUG
    // Display the type of the media for debugging perposes
    DisplayMediaType(TEXT("Input Pin Checking"), pmt);
#endif

    // The media types that we can support are entirely dependent on the
    // downstream connections. If we have downstream connections, we should
    // check with them - walk through the list calling each output pin

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n) 
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);

        if (pOutputPin != NULL) 
        {
            if (pOutputPin->m_Connected != NULL) 
            {
                // The pin is connected, check its peer
                hr = pOutputPin->m_Connected->QueryAccept(pmt);
                if (hr != NOERROR) 
                {
                    m_bInsideCheckMediaType = FALSE;
                    return VFW_E_TYPE_NOT_ACCEPTED;
                }
            }
        } 
        else 
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }

        n--;
    }

    // Either all the downstream pins have accepted or there are none.
    m_bInsideCheckMediaType = FALSE;
    return NOERROR;

} // CheckMediaType


//
// SetMediaType
//
HRESULT CAVTeeInputPin::SetMediaType(const CMediaType *pmt)
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;

    // Make sure that the base class likes it
    hr = CBaseInputPin::SetMediaType(pmt);
    if (FAILED(hr))
        return hr;

    ASSERT(m_Connected != NULL);
    return NOERROR;

} // SetMediaType


//
// BreakConnect
//
HRESULT CAVTeeInputPin::BreakConnect()
{
    // Release any allocator that we are holding
    if (m_pTee->m_pAllocator)
    {
        m_pTee->m_pAllocator->Release();
        m_pTee->m_pAllocator = NULL;
    }

    return NOERROR;

} // BreakConnect


//
// NotifyAllocator
//
STDMETHODIMP
CAVTeeInputPin::NotifyAllocator(IMemAllocator *pAllocator, BOOL bReadOnly)
{
    CheckPointer(pAllocator,E_FAIL);
    CAutoLock lock_it(m_pLock);

    // Free the old allocator if any
    if (m_pTee->m_pAllocator)
        m_pTee->m_pAllocator->Release();

    // Store away the new allocator
    pAllocator->AddRef();
    m_pTee->m_pAllocator = pAllocator;

    // Notify the base class about the allocator
    return CBaseInputPin::NotifyAllocator(pAllocator,bReadOnly);

} // NotifyAllocator


//
// EndOfStream
//
HRESULT CAVTeeInputPin::EndOfStream()
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_pTee->m_NumOutputPins);
    HRESULT hr = NOERROR;

    // Walk through the output pins list, sending the message downstream

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n) 
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if (pOutputPin != NULL) 
        {
            hr = pOutputPin->DeliverEndOfStream();
            if (FAILED(hr))
                return hr;
        } 
        else 
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }
    return(NOERROR);

} // EndOfStream


//
// BeginFlush
//
HRESULT CAVTeeInputPin::BeginFlush()
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_pTee->m_NumOutputPins);
    HRESULT hr = NOERROR;

    // Walk through the output pins list, sending the message downstream

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n) 
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if (pOutputPin != NULL) 
        {
            hr = pOutputPin->DeliverBeginFlush();
            if (FAILED(hr))
                return hr;
        } 
        else 
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }

    return CBaseInputPin::BeginFlush();

} // BeginFlush


//
// EndFlush
//
HRESULT CAVTeeInputPin::EndFlush()
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_pTee->m_NumOutputPins);
    HRESULT hr = NOERROR;

    // Walk through the output pins list, sending the message downstream

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n)
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if(pOutputPin != NULL)
        {
            hr = pOutputPin->DeliverEndFlush();
            if(FAILED(hr))
                return hr;
        }
        else
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }

    return CBaseInputPin::EndFlush();

} // EndFlush


//
// NewSegment
//
                    
HRESULT CAVTeeInputPin::NewSegment(REFERENCE_TIME tStart,
                                 REFERENCE_TIME tStop,
                                 double dRate)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_pTee->m_NumOutputPins);
    HRESULT hr = NOERROR;

    // Walk through the output pins list, sending the message downstream

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n)
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if(pOutputPin != NULL)
        {
            hr = pOutputPin->DeliverNewSegment(tStart, tStop, dRate);
            if(FAILED(hr))
                return hr;
        }
        else
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }

    return CBaseInputPin::NewSegment(tStart, tStop, dRate);

} // NewSegment


//
// Receive
//
HRESULT CAVTeeInputPin::Receive(IMediaSample *pSample)
{
    ASSERT(pSample);
    CAutoLock lock_it(m_pLock);

    // Check that all is well with the base class
    HRESULT hr = NOERROR;
    hr = CBaseInputPin::Receive(pSample);
    if(hr != NOERROR)
        return hr;

    // Walk through the output pins list, delivering to each in turn

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n)
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if(pOutputPin != NULL)
        {
            hr = pOutputPin->Deliver(pSample);
            if(hr != NOERROR)
                return hr;
        }
        else
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }

    return NOERROR;

} // Receive


//
// Completed a connection to a pin
//
HRESULT CAVTeeInputPin::CompleteConnect(IPin *pReceivePin)
{
    ASSERT(pReceivePin);
    
    HRESULT hr = CBaseInputPin::CompleteConnect(pReceivePin);
    if(FAILED(hr))
    {
        return hr;
    }

    // Force any output pins to use our type

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n)
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if(pOutputPin != NULL)
        {
            // Check with downstream pin
            if(pOutputPin->m_Connected != NULL)
            {
                if(m_mt != pOutputPin->m_mt)
                    m_pTee->ReconnectPin(pOutputPin, &m_mt);
            }
        }
        else
        {
            // We should have as many pins as the count says we have
            ASSERT(FALSE);
        }
        n--;
    }

    return S_OK;
}


//
// CAVTeeOutputPin constructor
//
CAVTeeOutputPin::CAVTeeOutputPin(TCHAR *pName,
                             CAVTee *pTee,
                             HRESULT *phr,
                             LPCWSTR pPinName,
                             int PinNumber) :
    CBaseOutputPin(pName, pTee, pTee, phr, pPinName) ,
    m_pOutputQueue(NULL),
    m_bHoldsSeek(FALSE),
    m_pPosition(NULL),
    m_pTee(pTee),
    m_cOurRef(0),
    m_bInsideCheckMediaType(FALSE)
{
    ASSERT(pTee);
}



#ifdef DEBUG
//
// CAVTeeOutputPin destructor
//
CAVTeeOutputPin::~CAVTeeOutputPin()
{
    ASSERT(m_pOutputQueue == NULL);
}
#endif


//
// NonDelegatingQueryInterface
//
// This function is overwritten to expose IMediaPosition and IMediaSelection
// Note that only one output stream can be allowed to expose this to avoid
// conflicts, the other pins will just return E_NOINTERFACE and therefore
// appear as non seekable streams. We have a LONG value that if exchanged to
// produce a TRUE means that we have the honor. If it exchanges to FALSE then
// someone is already in. If we do get it and error occurs then we reset it
// to TRUE so someone else can get it.
//
STDMETHODIMP
CAVTeeOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    ASSERT(ppv);

    *ppv = NULL;
    HRESULT hr = NOERROR;

    // See what interface the caller is interested in.
    if(riid == IID_IMediaPosition || riid == IID_IMediaSeeking)
    {
        if(m_pPosition)
        {
            if(m_bHoldsSeek == FALSE)
                return E_NOINTERFACE;
            return m_pPosition->QueryInterface(riid, ppv);
        }
    }
    else
    {
        return CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
    }

    CAutoLock lock_it(m_pLock);
    ASSERT(m_pPosition == NULL);
    IUnknown *pMediaPosition = NULL;

    // Try to create a seeking implementation
    if(InterlockedExchange(&m_pTee->m_lCanSeek, FALSE) == FALSE)
        return E_NOINTERFACE;

    // Create implementation of this dynamically as sometimes we may never
    // try and seek. The helper object implements IMediaPosition and also
    // the IMediaSelection control interface and simply takes the calls
    // normally from the downstream filter and passes them upstream

    hr = CreatePosPassThru(GetOwner(),
                           FALSE,
                           (IPin *)&m_pTee->m_Input,
                           &pMediaPosition);

    if(pMediaPosition == NULL)
    {
        InterlockedExchange(&m_pTee->m_lCanSeek, TRUE);
        return E_OUTOFMEMORY;
    }

    if(FAILED(hr))
    {
        InterlockedExchange(&m_pTee->m_lCanSeek, TRUE);
        pMediaPosition->Release();
        return hr;
    }

    m_pPosition = pMediaPosition;
    m_bHoldsSeek = TRUE;
    return NonDelegatingQueryInterface(riid, ppv);

} // NonDelegatingQueryInterface


//
// NonDelegatingAddRef
//
// We need override this method so that we can do proper reference counting
// on our output pin. The base class CBasePin does not do any reference
// counting on the pin in RETAIL.
//
// Please refer to the comments for the NonDelegatingRelease method for more
// info on why we need to do this.
//
STDMETHODIMP_(ULONG) CAVTeeOutputPin::NonDelegatingAddRef()
{
    CAutoLock lock_it(m_pLock);

#ifdef DEBUG
    // Update the debug only variable maintained by the base class
    m_cRef++;
    ASSERT(m_cRef > 0);
#endif

    // Now update our reference count
    m_cOurRef++;
    ASSERT(m_cOurRef > 0);
    return m_cOurRef;

} // NonDelegatingAddRef


//
// NonDelegatingRelease
//
// CAVTeeOutputPin overrides this class so that we can take the pin out of our
// output pins list and delete it when its reference count drops to 1 and there
// is atleast two free pins.
//
// Note that CreateNextOutputPin holds a reference count on the pin so that
// when the count drops to 1, we know that no one else has the pin.
//
// Moreover, the pin that we are about to delete must be a free pin(or else
// the reference would not have dropped to 1, and we must have atleast one
// other free pin(as the filter always wants to have one more free pin)
//
// Also, since CBasePin::NonDelegatingAddRef passes the call to the owning
// filter, we will have to call Release on the owning filter as well.
//
// Also, note that we maintain our own reference count m_cOurRef as the m_cRef
// variable maintained by CBasePin is debug only.
//
STDMETHODIMP_(ULONG) CAVTeeOutputPin::NonDelegatingRelease()
{
    CAutoLock lock_it(m_pLock);

#ifdef DEBUG
    // Update the debug only variable in CBasePin
    m_cRef--;
    ASSERT(m_cRef >= 0);
#endif

    // Now update our reference count
    m_cOurRef--;
    ASSERT(m_cOurRef >= 0);

    // if the reference count on the object has gone to one, remove
    // the pin from our output pins list and physically delete it
    // provided there are atealst two free pins in the list(including
    // this one)

    // Also, when the ref count drops to 0, it really means that our
    // filter that is holding one ref count has released it so we
    // should delete the pin as well.

    if(m_cOurRef <= 1)
    {
        int n = 2;                     // default forces pin deletion
        if(m_cOurRef == 1)
        {
            // Walk the list of pins, looking for count of free pins
            n = m_pTee->GetNumFreePins();
        }

        // If there are two free pins, delete this one.
        // NOTE: normall
        if(n >= 2)
        {
            m_cOurRef = 0;
#ifdef DEBUG
            m_cRef = 0;
#endif
            m_pTee->DeleteOutputPin(this);
            return(ULONG) 0;
        }
    }

    return(ULONG) m_cOurRef;

} // NonDelegatingRelease


//
// DecideBufferSize
//
// This has to be present to override the PURE virtual class base function
//
HRESULT CAVTeeOutputPin::DecideBufferSize(IMemAllocator *pMemAllocator,
                                        ALLOCATOR_PROPERTIES * ppropInputRequest)
{
    return NOERROR;

} // DecideBufferSize


//
// DecideAllocator
//
HRESULT CAVTeeOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    CheckPointer(pPin,E_POINTER);
    CheckPointer(ppAlloc,E_POINTER);
    ASSERT(m_pTee->m_pAllocator != NULL);

    *ppAlloc = NULL;

    // Tell the pin about our allocator, set by the input pin.
    HRESULT hr = NOERROR;
    hr = pPin->NotifyAllocator(m_pTee->m_pAllocator,TRUE);
    if(FAILED(hr))
        return hr;

    // Return the allocator
    *ppAlloc = m_pTee->m_pAllocator;
    m_pTee->m_pAllocator->AddRef();
    return NOERROR;

} // DecideAllocator


//
// CheckMediaType
//
HRESULT CAVTeeOutputPin::CheckMediaType(const CMediaType *pmt)
{
    CAutoLock lock_it(m_pLock);

    // If we are already inside checkmedia type for this pin, return NOERROR
    // It is possble to hookup two of the tee filters and some other filter
    // like the video effects sample to get into this situation. If we
    // do not detect this, we will loop till we blow the stack

    if(m_bInsideCheckMediaType == TRUE)
        return NOERROR;

    m_bInsideCheckMediaType = TRUE;
    HRESULT hr = NOERROR;

#ifdef DEBUG
    // Display the type of the media for debugging purposes
    DisplayMediaType(TEXT("Output Pin Checking"), pmt);
#endif

    // The input needs to have been conneced first
    if(m_pTee->m_Input.m_Connected == NULL)
    {
        m_bInsideCheckMediaType = FALSE;
        return VFW_E_NOT_CONNECTED;
    }

    // Make sure that our input pin peer is happy with this
    hr = m_pTee->m_Input.m_Connected->QueryAccept(pmt);
    if(hr != NOERROR)
    {
        m_bInsideCheckMediaType = FALSE;
        return VFW_E_TYPE_NOT_ACCEPTED;
    }

    // Check the format with the other outpin pins

    int n = m_pTee->m_NumOutputPins;
    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();

    while(n)
    {
        CAVTeeOutputPin *pOutputPin = m_pTee->m_OutputPinsList.GetNext(pos);
        if(pOutputPin != NULL && pOutputPin != this)
        {
            if(pOutputPin->m_Connected != NULL)
            {
                // The pin is connected, check its peer
                hr = pOutputPin->m_Connected->QueryAccept(pmt);
                if(hr != NOERROR)
                {
                    m_bInsideCheckMediaType = FALSE;
                    return VFW_E_TYPE_NOT_ACCEPTED;
                }
            }
        }
        n--;
    }

    m_bInsideCheckMediaType = FALSE;
    return NOERROR;

} // CheckMediaType


//
// EnumMediaTypes
//
STDMETHODIMP CAVTeeOutputPin::EnumMediaTypes(IEnumMediaTypes **ppEnum)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(ppEnum);

    // Make sure that we are connected
    if(m_pTee->m_Input.m_Connected == NULL)
        return VFW_E_NOT_CONNECTED;

    // We will simply return the enumerator of our input pin's peer
    return m_pTee->m_Input.m_Connected->EnumMediaTypes(ppEnum);

} // EnumMediaTypes


//
// SetMediaType
//
HRESULT CAVTeeOutputPin::SetMediaType(const CMediaType *pmt)
{
    CAutoLock lock_it(m_pLock);

#ifdef DEBUG
    // Display the format of the media for debugging purposes
    DisplayMediaType(TEXT("Output pin type agreed"), pmt);
#endif

    // Make sure that we have an input connected
    if(m_pTee->m_Input.m_Connected == NULL)
        return VFW_E_NOT_CONNECTED;

    // Make sure that the base class likes it
    HRESULT hr = NOERROR;
    hr = CBaseOutputPin::SetMediaType(pmt);
    if(FAILED(hr))
        return hr;

    return NOERROR;

} // SetMediaType


//
// CompleteConnect
//
HRESULT CAVTeeOutputPin::CompleteConnect(IPin *pReceivePin)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_Connected == pReceivePin);
    HRESULT hr = NOERROR;

    hr = CBaseOutputPin::CompleteConnect(pReceivePin);
    if(FAILED(hr))
        return hr;

    // If the type is not the same as that stored for the input
    // pin then force the input pins peer to be reconnected

    if(m_mt != m_pTee->m_Input.m_mt)
    {
        hr = m_pTee->ReconnectPin(m_pTee->m_Input.m_Connected, &m_mt);
        if(FAILED(hr))
        {
            return hr;
        }
    }

    // Since this pin has been connected up, create another output pin. We
    // will do this only if there are no unconnected pins on us. However
    // CompleteConnect will get called for the same pin during reconnection

    int n = m_pTee->GetNumFreePins();
    ASSERT(n <= 1);
    if(n == 1 || m_pTee->m_NumOutputPins == INFTEE_MAX_PINS)
        return NOERROR;

    // No unconnected pins left so spawn a new one

    CAVTeeOutputPin *pOutputPin = m_pTee->CreateNextOutputPin(m_pTee);
    if(pOutputPin != NULL)
    {
        m_pTee->m_NumOutputPins++;
        m_pTee->m_OutputPinsList.AddTail(pOutputPin);
        m_pTee->IncrementPinVersion();
    }

    // At this point we should be able to send some
    // notification that we have sprung a new pin

    return NOERROR;

} // CompleteConnect


//
// Active
//
// This is called when we start running or go paused. We create the
// output queue object to send data to our associated peer pin
//
HRESULT CAVTeeOutputPin::Active()
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;

    // Make sure that the pin is connected
    if(m_Connected == NULL)
        return NOERROR;

    // Create the output queue if we have to
    if(m_pOutputQueue == NULL)
    {
        m_pOutputQueue = new COutputQueue(m_Connected, &hr, TRUE, FALSE);
        if(m_pOutputQueue == NULL)
            return E_OUTOFMEMORY;

        // Make sure that the constructor did not return any error
        if(FAILED(hr))
        {
            delete m_pOutputQueue;
            m_pOutputQueue = NULL;
            return hr;
        }
    }

    // Pass the call on to the base class
    CBaseOutputPin::Active();
    return NOERROR;

} // Active


//
// Inactive
//
// This is called when we stop streaming
// We delete the output queue at this time
//
HRESULT CAVTeeOutputPin::Inactive()
{
    CAutoLock lock_it(m_pLock);

    // Delete the output queus associated with the pin.
    if(m_pOutputQueue)
    {
        delete m_pOutputQueue;
        m_pOutputQueue = NULL;
    }

    CBaseOutputPin::Inactive();
    return NOERROR;

} // Inactive


//
// Deliver
//
HRESULT CAVTeeOutputPin::Deliver(IMediaSample *pMediaSample)
{
    CheckPointer(pMediaSample,E_POINTER);

    // Make sure that we have an output queue
    if(m_pOutputQueue == NULL)
        return NOERROR;

    pMediaSample->AddRef();
    return m_pOutputQueue->Receive(pMediaSample);

} // Deliver


//
// DeliverEndOfStream
//
HRESULT CAVTeeOutputPin::DeliverEndOfStream()
{
    // Make sure that we have an output queue
    if(m_pOutputQueue == NULL)
        return NOERROR;

    m_pOutputQueue->EOS();
    return NOERROR;

} // DeliverEndOfStream


//
// DeliverBeginFlush
//
HRESULT CAVTeeOutputPin::DeliverBeginFlush()
{
    // Make sure that we have an output queue
    if(m_pOutputQueue == NULL)
        return NOERROR;

    m_pOutputQueue->BeginFlush();
    return NOERROR;

} // DeliverBeginFlush


//
// DeliverEndFlush
//
HRESULT CAVTeeOutputPin::DeliverEndFlush()
{
    // Make sure that we have an output queue
    if(m_pOutputQueue == NULL)
        return NOERROR;

    m_pOutputQueue->EndFlush();
    return NOERROR;

} // DeliverEndFlish

//
// DeliverNewSegment
//
HRESULT CAVTeeOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, 
                                         REFERENCE_TIME tStop,  
                                         double dRate)
{
    // Make sure that we have an output queue
    if(m_pOutputQueue == NULL)
        return NOERROR;

    m_pOutputQueue->NewSegment(tStart, tStop, dRate);
    return NOERROR;

} // DeliverNewSegment


//
// Notify
//
STDMETHODIMP CAVTeeOutputPin::Notify(IBaseFilter *pSender, Quality q)
{
    // We pass the message on, which means that we find the quality sink
    // for our input pin and send it there

    POSITION pos = m_pTee->m_OutputPinsList.GetHeadPosition();
    CAVTeeOutputPin *pFirstOutput = m_pTee->m_OutputPinsList.GetNext(pos);

    if(this == pFirstOutput)
    {
        if(m_pTee->m_Input.m_pQSink!=NULL)
        {
            return m_pTee->m_Input.m_pQSink->Notify(m_pTee, q);
        }
        else
        {
            // No sink set, so pass it upstream
            HRESULT hr;
            IQualityControl * pIQC;

            hr = VFW_E_NOT_FOUND;
            if(m_pTee->m_Input.m_Connected)
            {
                m_pTee->m_Input.m_Connected->QueryInterface(IID_IQualityControl,(void**)&pIQC);

                if(pIQC!=NULL)
                {
                    hr = pIQC->Notify(m_pTee, q);
                    pIQC->Release();
                }
            }
            return hr;
        }
    }

    // Quality management is too hard to do
    return NOERROR;

} // Notify