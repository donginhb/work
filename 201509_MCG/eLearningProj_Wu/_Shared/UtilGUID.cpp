#include <afxwin.h>
#include <objbase.h>

#include "UtilGUID.h"

BOOL GetFormattedGuid(GUID &guid, CString& cst)
{
	cst.Format("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",		
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );
	return TRUE;
}

BOOL GenGuidString(CString &cst)
{
	GUID guid=GUID_NULL;
	CoCreateGuid(&guid);
	if(guid==GUID_NULL) return FALSE;

	return GetFormattedGuid(guid, cst);
}