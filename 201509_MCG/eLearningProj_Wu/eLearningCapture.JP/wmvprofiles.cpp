// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "wmvprofiles.h"


/////////////////////////////////////////////////////////////////////////////
// CWMVProfiles properties

short CWMVProfiles::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CWMVProfiles::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CWMVProfiles operations

short CWMVProfiles::FindWMVProfile(LPCTSTR strProfile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strProfile);
	return result;
}

CString CWMVProfiles::FindWMVProfileDesc(short index)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		index);
	return result;
}

CString CWMVProfiles::FindWMVProfileName(short index)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		index);
	return result;
}

LPDISPATCH CWMVProfiles::GetItem(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}