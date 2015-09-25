#include <afxwin.h>

#include "..\VFXManager\VFXMan.h"

#include "..\MediaKit\WaveLib\HelperWav.h"
#include "HelperWavSpec.h"

BOOL WavWriterAppendReader(WAVWRITER *pWavWriter, UINT uStart, UINT uSize, LPBYTE pBuffer, UINT cbBuffer, WAVREADER *pWavReader) 
{
	UINT uRetSize=0, uCopySize=0;
	UINT uPos=uStart;
	while(uPos<(uStart+uSize)) {
		uCopySize = uStart+uSize - uPos;
		if(uCopySize>cbBuffer) uCopySize = cbBuffer;
		uCopySize = uCopySize / pWavWriter->pwfx->nBlockAlign * pWavWriter->pwfx->nBlockAlign;
		if(!WavReaderReadDirectData(pWavReader, uPos, uCopySize, pBuffer, &uRetSize))
			return FALSE;
		if(!WavWriterWriteData(pWavWriter, pBuffer, uCopySize))
			return FALSE;
		uPos += uCopySize;
	}
	
	return TRUE;
}

BOOL WavWriterMixerReader(WAVWRITER *pWavWriter, UINT uStart, UINT uSize, LPBYTE pBuffer, UINT cbBuffer, WAVREADER *pWavReader, UINT uMixerStart, LPBYTE pMixerBuffer, UINT cbMixerBuffer, WAVREADER *pMixerWavReader)
{
	BOOL fError=FALSE;
	UINT uRetSize=0, uCopySize=0;
	UINT uPos=uStart, uMixerPos=uMixerStart;
	while((uPos<(uStart+uSize))&&(uMixerPos<(uMixerStart+uSize))) {
		uCopySize = cbBuffer;
		if((uPos+uCopySize)>(uStart+uSize)) uCopySize = uStart + uSize - uPos;
		uCopySize = uCopySize / pWavWriter->pwfx->nBlockAlign * pWavWriter->pwfx->nBlockAlign;
		ASSERT(uMixerPos+uCopySize<=uMixerStart+uSize);
		if(!WavReaderReadDirectData(pWavReader, uPos, uCopySize, pBuffer, &uRetSize)) {
			fError = FALSE;
			break;
		}
		if(!WavReaderReadDirectData(pMixerWavReader, uMixerPos, uCopySize, pMixerBuffer, &uRetSize)) {
			fError = FALSE;
			break;
		}

		for(UINT ii=0; ii<uCopySize; ii+=pWavWriter->pwfx->nBlockAlign) {
			long lValue=0;
			for(UINT jj=0; jj<pWavWriter->pwfx->nChannels; jj++) {
				switch(pWavWriter->pwfx->wBitsPerSample) {
				case 8:
					lValue = (long)(pBuffer[ii+jj]) - 128 + (long)(pMixerBuffer[ii+jj]) - 128;
					if(lValue>127) lValue = 127;
					if(lValue<-127) lValue = -127;
					pBuffer[ii+jj] = (BYTE)(lValue + 128);
					break;
				case 16:					
					lValue = (long)(*(short*)(pBuffer+ii+(jj<<1))) + (long)(*(short*)(pMixerBuffer+ii+(jj<<1)));
					if(lValue>32767L) lValue = 32767L;
					if(lValue<-32767L) lValue = -32767L;
					*(short*)(pBuffer+ii+(jj<<1)) = (short)lValue;
					break;
				default: 
					fError = FALSE;
					break;
				}
			}
		}
		
		if(!WavWriterWriteData(pWavWriter, pBuffer, uCopySize)) {
			fError = FALSE;
			break;
		}
		uPos += uCopySize;
		uMixerPos += uCopySize;
	}

	return (!fError);
}

BOOL WavWriterMixerFile(WAVWRITER *pWavWriter, UINT uStart, UINT uSize, LPBYTE pBuffer, UINT cbBuffer, WAVREADER *pWavReader, UINT uMixerStart, LPBYTE pMixerBuffer, UINT cbMixerBuffer, LPSTR szFile)
{
	WAVREADER MixerWavReader;
	if(!WavReaderOpen(&MixerWavReader, szFile))
		return FALSE;

	if(!WavReaderStartDataRead(&MixerWavReader))
		return FALSE;

	BOOL fOK = WavWriterMixerReader(pWavWriter, uStart, uSize, pBuffer, cbBuffer, pWavReader, uMixerStart, pMixerBuffer, cbMixerBuffer, &MixerWavReader);

	WavReaderClose(&MixerWavReader);

	return fOK;
}

BOOL WavFileMixer(LPSTR szWaveFile, CDWordArray &TimeArray, CStringArray &cstArray, CDWordArray &StartArray, CDWordArray &StopArray, LPSTR szOutput)
{
	WAVREADER WavReader;
	
	if(!WavReaderOpen(&WavReader, szWaveFile))
		return FALSE;

	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nChannels, WavReader.pwfx->nSamplesPerSec, szOutput)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavReaderClose(&WavReader);
		WavWriterClose(&WavWriter);
		return FALSE;
	}

	LPBYTE pBuffer = (LPBYTE)malloc(WavReader.pwfx->nAvgBytesPerSec);
	LPBYTE pMixerBuffer = (LPBYTE)malloc(WavReader.pwfx->nAvgBytesPerSec);

	if((pBuffer==NULL)||(pMixerBuffer==NULL)) {
		if(pBuffer!=NULL) free(pBuffer);
		if(pMixerBuffer!=NULL) free(pMixerBuffer);
		WavReaderClose(&WavReader);
		WavWriterClose(&WavWriter);
		return FALSE;
	}

	BOOL fError = FALSE;
	DWORD dwPrePos=0, dwMixerPos=0, dwMixerSize=0;
	DWORD dwMixerStart=0, dwMixerStop=0;
	for(long ii=0; ii<TimeArray.GetSize(); ii++) {
		DWORD dwTime = TimeArray.GetAt(ii);
		dwMixerPos = (DWORD)(TimeArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwMixerPos = dwMixerPos / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		if(dwPrePos<dwMixerPos) { // dump source from dwPos to dwMixerPos
			if(!WavWriterAppendReader(&WavWriter, dwPrePos, dwMixerPos-dwPrePos, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader)) {
				fError = TRUE;
				break;
			}
			dwPrePos = dwMixerPos;
		}

		dwMixerStart = (DWORD)(StartArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwMixerStop = (DWORD)(StopArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwMixerStart = dwMixerStart / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		dwMixerStop = dwMixerStop / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		
		dwMixerSize = dwMixerStop-dwMixerStart;
		if((dwMixerPos + dwMixerSize)>=WavReader.ckData.cksize) dwMixerSize = WavReader.ckData.cksize - dwMixerPos;
		dwMixerSize = dwMixerSize / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		if(dwMixerSize>=WavReader.pwfx->nBlockAlign) {
			if(!WavWriterMixerFile(&WavWriter, dwMixerPos, dwMixerSize, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader, dwMixerStart, pMixerBuffer, WavReader.pwfx->nAvgBytesPerSec, (LPSTR)(LPCSTR)cstArray.GetAt(ii))) {
				fError = TRUE;
				break;
			}
		}
		dwPrePos = dwMixerPos + dwMixerSize;
	}

	if(!fError) {
		if(dwPrePos<WavReader.ckData.cksize) { // dump source from dwPos to ckSize
			if(!WavWriterAppendReader(&WavWriter, dwPrePos, WavReader.ckData.cksize-dwPrePos, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader)) 
				fError = TRUE;
		}
	}

	WavWriterClose(&WavWriter);	
	WavReaderClose(&WavReader);
	free(pBuffer);
	free(pMixerBuffer);
	
	return !fError;
}

BOOL WavWriterReplacerReader(WAVWRITER *pWavWriter, UINT uStart, UINT uSize, LPBYTE pBuffer, UINT cbBuffer, WAVREADER *pWavReader, UINT uReplacerStart, LPBYTE pReplacerBuffer, UINT cbReplacerBuffer, WAVREADER *pReplacerWavReader)
{
	BOOL fError=FALSE;
	UINT uRetSize=0, uCopySize=0;
	UINT uPos=uStart, uReplacerPos=uReplacerStart;
	while((uPos<(uStart+uSize))&&(uReplacerPos<(uReplacerStart+uSize))) {
		uCopySize = cbBuffer;
		if((uPos+uCopySize)>(uStart+uSize)) uCopySize = uStart + uSize - uPos;
		uCopySize = uCopySize / pWavWriter->pwfx->nBlockAlign * pWavWriter->pwfx->nBlockAlign;
		ASSERT(uReplacerPos+uCopySize<=uReplacerStart+uSize);
		if(!WavReaderReadDirectData(pReplacerWavReader, uReplacerPos, uCopySize, pReplacerBuffer, &uRetSize)) {
			fError = FALSE;
			break;
		}

		memcpy(pBuffer, pReplacerBuffer, uCopySize);
				
		if(!WavWriterWriteData(pWavWriter, pBuffer, uCopySize)) {
			fError = FALSE;
			break;
		}
		uPos += uCopySize;
		uReplacerPos += uCopySize;
	}
	
	return (!fError);
}

BOOL WavWriterReplacerFile(WAVWRITER *pWavWriter, UINT uStart, UINT uSize, LPBYTE pBuffer, UINT cbBuffer, WAVREADER *pWavReader, UINT uReplacerStart, LPBYTE pReplacerBuffer, UINT cbReplacerBuffer, LPSTR szFile)
{
	WAVREADER ReplacerWavReader;
	if(!WavReaderOpen(&ReplacerWavReader, szFile))
		return FALSE;
	
	if(!WavReaderStartDataRead(&ReplacerWavReader))
		return FALSE;
	
	BOOL fOK = WavWriterReplacerReader(pWavWriter, uStart, uSize, pBuffer, cbBuffer, pWavReader, uReplacerStart, pReplacerBuffer, cbReplacerBuffer, &ReplacerWavReader);
	
	WavReaderClose(&ReplacerWavReader);
	
	return fOK;
}

BOOL WavFileReplacer(LPSTR szWaveFile, CDWordArray &TimeArray, CStringArray &cstArray, CDWordArray &StartArray, CDWordArray &StopArray, LPSTR szOutput)
{
	WAVREADER WavReader;
	
	if(!WavReaderOpen(&WavReader, szWaveFile))
		return FALSE;
	
	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}
	
	WAVWRITER WavWriter;
	memset(&WavWriter, 0, sizeof(WavWriter));
	if(!WavWriterCreate_PCM(&WavWriter, WavReader.pwfx->wBitsPerSample, WavReader.pwfx->nChannels, WavReader.pwfx->nSamplesPerSec, szOutput)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}
	
	if(!WavWriterStartDataWrite(&WavWriter)) {
		WavReaderClose(&WavReader);
		WavWriterClose(&WavWriter);
		return FALSE;
	}
	
	LPBYTE pBuffer = (LPBYTE)malloc(WavReader.pwfx->nAvgBytesPerSec);
	LPBYTE pReplacerBuffer = (LPBYTE)malloc(WavReader.pwfx->nAvgBytesPerSec);
	
	if((pBuffer==NULL)||(pReplacerBuffer==NULL)) {
		if(pBuffer!=NULL) free(pBuffer);
		if(pReplacerBuffer!=NULL) free(pReplacerBuffer);
		WavReaderClose(&WavReader);
		WavWriterClose(&WavWriter);
		return FALSE;
	}
	
	BOOL fError = FALSE;
	DWORD dwPrePos=0, dwReplacerPos=0, dwReplacerSize=0;
	DWORD dwReplacerStart=0, dwReplacerStop=0;
	for(long ii=0; ii<TimeArray.GetSize(); ii++) {
		DWORD dwTime = TimeArray.GetAt(ii);
		dwReplacerPos = (DWORD)(TimeArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwReplacerPos = dwReplacerPos / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		if(dwPrePos<dwReplacerPos) { // dump source from dwPos to dwReplacerPos
			if(!WavWriterAppendReader(&WavWriter, dwPrePos, dwReplacerPos-dwPrePos, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader)) {
				fError = TRUE;
				break;
			}
			dwPrePos = dwReplacerPos;
		}
		
		dwReplacerStart = (DWORD)(StartArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwReplacerStop = (DWORD)(StopArray.GetAt(ii) / 1000.0 * WavReader.pwfx->nAvgBytesPerSec);
		dwReplacerStart = dwReplacerStart / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		dwReplacerStop = dwReplacerStop / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		
		dwReplacerSize = dwReplacerStop-dwReplacerStart;
		if((dwReplacerPos + dwReplacerSize)>=WavReader.ckData.cksize) dwReplacerSize = WavReader.ckData.cksize - dwReplacerPos;
		dwReplacerSize = dwReplacerSize / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		if(dwReplacerSize>=WavReader.pwfx->nBlockAlign) {
			if(!WavWriterReplacerFile(&WavWriter, dwReplacerPos, dwReplacerSize, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader, dwReplacerStart, pReplacerBuffer, WavReader.pwfx->nAvgBytesPerSec, (LPSTR)(LPCSTR)cstArray.GetAt(ii))) {
				fError = TRUE;
				break;
			}
		}
		dwPrePos = dwReplacerPos + dwReplacerSize;
	}
	
	if(!fError) {
		if(dwPrePos<WavReader.ckData.cksize) { // dump source from dwPos to ckSize
			if(!WavWriterAppendReader(&WavWriter, dwPrePos, WavReader.ckData.cksize-dwPrePos, pBuffer, WavReader.pwfx->nAvgBytesPerSec, &WavReader)) 
				fError = TRUE;
		}
	}
	
	WavWriterClose(&WavWriter);
	WavReaderClose(&WavReader);
	free(pBuffer);
	free(pReplacerBuffer);
	
	return !fError;
}