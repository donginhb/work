#include <streams.h>     // DirectShow (includes windows.h)
#include <initguid.h>    // declares DEFINE_GUID to declare an EXTERN_C const.
#include "null_AV.h"

// Using this pointer in constructor
#pragma warning(disable:4355 4127)

//
// CreateInstance
//
// Provide the way for COM to create a CAVNullRenderer object
CUnknown * WINAPI CAVNullRenderer::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CheckPointer(phr,NULL);
    
    CAVNullRenderer *pNewObject = 
        new CAVNullRenderer(punk, phr);

    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance

CAVNullRenderer::CAVNullRenderer(LPUNKNOWN pUnk,HRESULT *phr)
	: m_Input(this, phr, L"Input"),
	  CBaseFilter(NAME("NullRenderer filter"), pUnk, this, CLSID_AVNullRenderer)
{ 
}

CAVNullRenderer::~CAVNullRenderer()
{
}

int CAVNullRenderer::GetPinCount()
{
	return 1;
}

CBasePin *CAVNullRenderer::GetPin(int n)
{
    ASSERT(n == 0);
    if(n != 0)
    {
        return NULL;
    }

    return &m_Input;
}

HRESULT CAVNullRenderer::PresetMediaType(const GUID *ptype, const GUID *psubtype)
{
	m_Input.m_intype = GUID_NULL;
	m_Input.m_insubtype = GUID_NULL;

	if(ptype) m_Input.m_intype = *ptype;
	if(psubtype) m_Input.m_insubtype = *psubtype;

	return S_OK;
}

CAVNullRendererInputPin::CAVNullRendererInputPin(CAVNullRenderer *pFilter, HRESULT *phr, LPCWSTR pPinName)
	:CBaseInputPin(NAME("Null Input Pin"), pFilter, pFilter, phr, pPinName)
{
	m_pFilter = pFilter;

	m_intype = GUID_NULL;
	m_insubtype = GUID_NULL;
}

CAVNullRendererInputPin::~CAVNullRendererInputPin()
{
}

HRESULT CAVNullRendererInputPin::BreakConnect()
{
    // Check we have a valid connection

    if(m_mt.IsValid() == FALSE)
    {
        // Don't return an error here, because it could lead to 
        // ASSERT failures when rendering media files in GraphEdit.
        return S_FALSE;
    }

    m_pFilter->Stop();

    // Reset the CLSIDs of the connected media type

    m_mt.SetType(&GUID_NULL);
    m_mt.SetSubtype(&GUID_NULL);
    return CBaseInputPin::BreakConnect();

} // BreakConnect

HRESULT CAVNullRendererInputPin::CheckMediaType(const CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);

	if((m_intype!=GUID_NULL)&&(pmt->majortype!=m_intype))
		return E_INVALIDARG;
	
	if((m_insubtype!=GUID_NULL)&&(pmt->subtype!=m_insubtype))
		return E_INVALIDARG;

	return NOERROR;
}
