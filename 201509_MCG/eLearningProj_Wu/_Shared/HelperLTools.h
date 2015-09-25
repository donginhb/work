#ifndef _HELPERLTOOLS_H_
#define _HELPERLTOOLS_H_
#include <wtypes.h>
#include <l_bitmap.h>
typedef enum {
	PROC_INTERPOLATE_BUF_2_BUF_BMH=0,
	PROC_ADD_BMH_2_BUF=1,
	PROC_DEL_BMH_2_BUF=2,
	PROC_GRAYSCALE=3,
} ENUMPROC;
BOOL ProcessingBuffer(ENUMPROC eProc, BITMAPHANDLE &bmh, LPBYTE pBuffer, DWORD cbBuffer, long lRatio_1000);
BOOL CopyBufferToBitmapHandle(LPBYTE pBuffer, DWORD cbBuffer, BITMAPHANDLE &bmh);
BOOL CopyBitmapHandleToBuffer(BITMAPHANDLE &bmh, LPBYTE pBuffer, DWORD cbBuffer);
#endif