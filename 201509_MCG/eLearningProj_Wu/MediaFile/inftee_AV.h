//------------------------------------------------------------------------------
// File: InfTee.h
//
// Desc: DirectShow sample code - header file for infinite tee filter.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __AVINFTEE__
#define __AVINFTEE__

// {686EF193-ABDA-465f-9DF8-C01D1E33D23C}
DEFINE_GUID(CLSID_AVTee, 
			0x686ef193, 0xabda, 0x465f, 0x9d, 0xf8, 0xc0, 0x1d, 0x1e, 0x33, 0xd2, 0x3c);

class CAVTee;
class CAVTeeInputPin;
class CAVTeeOutputPin;

// class for the Tee filter's Input pin

class CAVTeeInputPin : public CBaseInputPin
{
    friend class CAVTeeOutputPin;
    CAVTee *m_pTee;                  // Main filter object
    BOOL m_bInsideCheckMediaType;  // Re-entrancy control
public:
	GUID m_intype, m_insubtype;
public:

    // Constructor and destructor
    CAVTeeInputPin(TCHAR *pObjName,
                 CAVTee *pTee,
                 HRESULT *phr,
                 LPCWSTR pPinName);

#ifdef DEBUG
    ~CAVTeeInputPin();
#endif
    // Used to check the input pin connection
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT SetMediaType(const CMediaType *pmt);
    HRESULT BreakConnect();

    // Reconnect outputs if necessary at end of completion
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    STDMETHODIMP NotifyAllocator(IMemAllocator *pAllocator, BOOL bReadOnly);

    // Pass through calls downstream
    STDMETHODIMP EndOfStream();
    STDMETHODIMP BeginFlush();
    STDMETHODIMP EndFlush();
    STDMETHODIMP NewSegment(
                    REFERENCE_TIME tStart,
                    REFERENCE_TIME tStop,
                    double dRate);

    // Handles the next block of data from the stream
    STDMETHODIMP Receive(IMediaSample *pSample);

};


// Class for the Tee filter's Output pins.

class CAVTeeOutputPin : public CBaseOutputPin
{
    friend class CAVTeeInputPin;
    friend class CAVTee;

    CAVTee *m_pTee;                  // Main filter object pointer
    IUnknown    *m_pPosition;      // Pass seek calls upstream
    BOOL m_bHoldsSeek;             // Is this the one seekable stream

    COutputQueue *m_pOutputQueue;  // Streams data to the peer pin
    BOOL m_bInsideCheckMediaType;  // Re-entrancy control
    LONG m_cOurRef;                // We maintain reference counting

public:

    // Constructor and destructor

    CAVTeeOutputPin(TCHAR *pObjName,
                   CAVTee *pTee,
                   HRESULT *phr,
                   LPCWSTR pPinName,
                   INT PinNumber);

#ifdef DEBUG
    ~CAVTeeOutputPin();
#endif

    // Override to expose IMediaPosition
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppvoid);

    // Override since the life time of pins and filters are not the same
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();

    // Override to enumerate media types
    STDMETHODIMP EnumMediaTypes(IEnumMediaTypes **ppEnum);

    // Check that we can support an output type
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT SetMediaType(const CMediaType *pmt);

    // Negotiation to use our input pins allocator
    HRESULT DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc);
    HRESULT DecideBufferSize(IMemAllocator *pMemAllocator,
                             ALLOCATOR_PROPERTIES * ppropInputRequest);

    // Used to create output queue objects
    HRESULT Active();
    HRESULT Inactive();

    // Overriden to create and destroy output pins
    HRESULT CompleteConnect(IPin *pReceivePin);

    // Overriden to pass data to the output queues
    HRESULT Deliver(IMediaSample *pMediaSample);
    HRESULT DeliverEndOfStream();
    HRESULT DeliverBeginFlush();
    HRESULT DeliverEndFlush();
    HRESULT DeliverNewSegment(
                    REFERENCE_TIME tStart,
                    REFERENCE_TIME tStop,
                    double dRate);


    // Overriden to handle quality messages
    STDMETHODIMP Notify(IBaseFilter *pSender, Quality q);
};


// Class for the Tee filter

class CAVTee: public CCritSec, public CBaseFilter
{
    // Let the pins access our internal state
    friend class CAVTeeInputPin;
    friend class CAVTeeOutputPin;
    typedef CGenericList <CAVTeeOutputPin> COutputList;

    // Declare an input pin.
    CAVTeeInputPin m_Input;

    INT m_NumOutputPins;            // Current output pin count
    COutputList m_OutputPinsList;   // List of the output pins
    INT m_NextOutputPinNumber;      // Increases monotonically.
    LONG m_lCanSeek;                // Seekable output pin
    IMemAllocator *m_pAllocator;    // Allocator from our input pin

public:

    CAVTee(TCHAR *pName,LPUNKNOWN pUnk,HRESULT *hr);
    ~CAVTee();

    CBasePin *GetPin(int n);
    int GetPinCount();

    // Function needed for the class factory
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);

    // Send EndOfStream if no input connection
    STDMETHODIMP Run(REFERENCE_TIME tStart);
    STDMETHODIMP Pause();
    STDMETHODIMP Stop();

public:
	// External Interface
	HRESULT PresetMediaType(const GUID *ptype, const GUID *psubtype);
	
protected:

    // The following manage the list of output pins

    void InitOutputPinsList();
    CAVTeeOutputPin *GetPinNFromList(int n);
    CAVTeeOutputPin *CreateNextOutputPin(CAVTee *pTee);
    void DeleteOutputPin(CAVTeeOutputPin *pPin);
    int GetNumFreePins();
};

#endif // __INFTEE__

