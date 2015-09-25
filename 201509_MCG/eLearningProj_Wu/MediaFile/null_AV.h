#ifndef _NULL_AV_H_
#define _NULL_AV_H_

// {31AF8C60-FCF9-47b7-A9F9-1ED1B9F39CD6}
DEFINE_GUID(CLSID_AVNullRenderer, 
0x31af8c60, 0xfcf9, 0x47b7, 0xa9, 0xf9, 0x1e, 0xd1, 0xb9, 0xf3, 0x9c, 0xd6);

class CAVNullRendererInputPin;
class CAVNullRenderer;

class CAVNullRendererInputPin : public CBaseInputPin
{
    friend class CAVNullRender;
public:
	GUID	m_intype, m_insubtype;
private:

    CAVNullRenderer *m_pFilter;         // The filter that owns us

public:

    CAVNullRendererInputPin(CAVNullRenderer *pFilter,
                   HRESULT *phr,
                   LPCWSTR pPinName);
    ~CAVNullRendererInputPin();

	HRESULT BreakConnect();

    // Check that we can support this input type
    HRESULT CheckMediaType(const CMediaType *pmt);

}; // CAVNullRendererInputPin

class CAVNullRenderer
    : public CBaseFilter, public CCritSec
{

public:

    DECLARE_IUNKNOWN;

    // Constructor - just calls the base class constructor
    CAVNullRenderer(LPUNKNOWN pUnk,HRESULT *phr);
	virtual ~CAVNullRenderer();

    // Return the pins that we support
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

public:
    int GetPinCount();
    CBasePin *GetPin(int n);

	HRESULT PresetMediaType(const GUID *ptype, const GUID *psubtype);
private:
	CAVNullRendererInputPin m_Input;   // Handles pin interfaces
};

#endif