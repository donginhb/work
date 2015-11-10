// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "audiodevices.h"


/////////////////////////////////////////////////////////////////////////////
// CAudioDevices properties

short CAudioDevices::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CAudioDevices::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CAudioDevices operations

short CAudioDevices::FindDevice(LPCTSTR strDeviceName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strDeviceName);
	return result;
}

CString CAudioDevices::FindDeviceName(short index)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		index);
	return result;
}

LPDISPATCH CAudioDevices::GetItem(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}