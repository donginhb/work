#include "HelperBitmapOP.h"
#include "HelperLTools.h"

BOOL CopyBufferToBitmapHandle(LPBYTE pBuffer, DWORD cbBuffer, BITMAPHANDLE &bmh)
{
	if(!bmh.Flags.Allocated)
		return FALSE;

	L_AccessBitmap(&bmh);

	long ii=0, lOffset=0;
	for(ii=0; ii<bmh.Height; ii++) {
		if((DWORD)(lOffset+(long)bmh.BytesPerLine)<=cbBuffer)
			L_PutBitmapRow(&bmh, pBuffer+lOffset, ii, bmh.BytesPerLine);
		lOffset += bmh.BytesPerLine;
	}

	L_ReleaseBitmap(&bmh);

	return TRUE;
}

BOOL CopyBitmapHandleToBuffer(BITMAPHANDLE &bmh, LPBYTE pBuffer, DWORD cbBuffer)
{
	if(!bmh.Flags.Allocated)
		return FALSE;

	L_AccessBitmap(&bmh);

	long ii=0, lOffset=0;
	for(ii=0; ii<bmh.Height; ii++) {
		if((DWORD)(lOffset+(long)bmh.BytesPerLine)<=cbBuffer) {
			L_GetBitmapRow(&bmh, pBuffer+lOffset, ii, bmh.BytesPerLine);
		}
		lOffset += bmh.BytesPerLine;
	}

	L_ReleaseBitmap(&bmh);
	return TRUE;
}

// pBuffer -> bmh = pBuffer
BOOL ProcessingBuffer(ENUMPROC eProc, BITMAPHANDLE &bmh, LPBYTE pBuffer, DWORD cbBuffer, long lRatio_1000)
{
	LPBYTE pLine = (LPBYTE)malloc(bmh.BytesPerLine);
	if(pLine==NULL) return FALSE;

	L_AccessBitmap(&bmh);

	WORD wBytesPerPixel=bmh.BitsPerPixel/8;
	long lOffset=0;
	int cr=0, rr=0, gg=0, bb=0, rr2=0, gg2=0, bb2=0;
	for(DWORD ii=0; ii<(DWORD)bmh.Height; ii++) {
		L_GetBitmapRow(&bmh, pLine, ii, bmh.BytesPerLine);
		for(DWORD jj=0; jj<(DWORD)bmh.BytesPerLine; jj+=wBytesPerPixel) {
			switch(bmh.BitsPerPixel) {
			case 16:
				cr = *(WORD*)(pBuffer+lOffset+jj);
				rr = RGB555_GETRED(cr); gg = RGB555_GETGREEN(cr); bb = RGB555_GETBLUE(cr);

				cr = *(WORD*)(pLine+jj);
				rr2 = RGB555_GETRED(cr); gg2 = RGB555_GETGREEN(cr); bb2 = RGB555_GETBLUE(cr);
				break;
			case 24:
				rr = pBuffer[lOffset+jj]; gg = pBuffer[lOffset+jj+1]; bb = pBuffer[lOffset+jj+2];
				rr2 = pLine[jj]; gg2 = pLine[jj+1]; bb2 = pLine[jj+2];
				break;
			}


			switch(eProc) {
			case PROC_ADD_BMH_2_BUF:
				rr2 = rr + rr2 * lRatio_1000 / 1000;
				gg2 = gg + gg2 * lRatio_1000 / 1000;
				bb2 = bb + bb2 * lRatio_1000 / 1000;
				break;
			case PROC_DEL_BMH_2_BUF:
				rr2 = rr - rr2 * lRatio_1000 / 1000;
				gg2 = gg - gg2 * lRatio_1000 / 1000;
				bb2 = bb - bb2 * lRatio_1000 / 1000;
				break;
			case PROC_GRAYSCALE:
				rr2 = (rr + gg + bb)/3;
				gg2 = rr2; bb2 = rr2;
				break;
			default:
				rr2 = (rr2 - rr) * lRatio_1000 / 1000 + rr;
				gg2 = (gg2 - gg) * lRatio_1000 / 1000 + gg;
				bb2 = (bb2 - bb) * lRatio_1000 / 1000 + bb;
				break;
			}

			if(rr2<0) rr2 = 0; if(gg2<0) gg2 = 0; if(bb2<0) bb2 = 0;
			if(rr2>255) rr2 = 255; if(gg2>255) gg2 = 255; if(bb2>255) bb2 = 255;

			switch(bmh.BitsPerPixel) {
			case 16: *(WORD*)(pBuffer+lOffset+jj) = RGB555_MAKE(rr2, gg2, bb2); break;
			case 24: pBuffer[lOffset+jj] = rr2; pBuffer[lOffset+jj+1] = gg2; pBuffer[lOffset+jj+2] = bb2; break;
			}
		}
		lOffset += bmh.BytesPerLine;
	}

	L_ReleaseBitmap(&bmh);

	free(pLine);
	return TRUE;
}
