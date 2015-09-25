#include <afxwin.h>
#include <wmsdkidl.h>
#include "MultiTimerBase.h"

BOOL CMultiTimerBase::ReportMultiProgress(HWND hwnd, long lMsg, QWORD qwNow) 
{
	if(!IsWindow(hwnd)) return FALSE;
	SendMessage(hwnd, lMsg, 0, MAKELONG((WORD)(qwNow/10000000L), (WORD)(m_qwTotalDuration/10000000L)));
	return TRUE;
}
