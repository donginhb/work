#include "HelperWav.h"
#include "cch_Wave.h"

#include "..\..\VFXManager\VFXMan.h"
#include "..\..\VFXManager\VFXMan_i.c"
#include "..\..\VFXPlugIn\VFXEffect.h"


BOOL WINAPI WavWriterCreate_PCM(LPWAVWRITER pWavWriter, WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate, LPSTR szFile)
{
	if(pWavWriter==NULL)
		return FALSE;

	pWavWriter->pwfx = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	if(pWavWriter->pwfx==NULL) return FALSE;

	memset(pWavWriter->pwfx, 0, sizeof(WAVEFORMATEX));

	pWavWriter->pwfx->wFormatTag = WAVE_FORMAT_PCM;
	pWavWriter->pwfx->wBitsPerSample = wBitsPerSample;
	pWavWriter->pwfx->nChannels = wChannels;
	pWavWriter->pwfx->nSamplesPerSec = dwSampleRate;
	pWavWriter->pwfx->nBlockAlign = (WORD)((DWORD)pWavWriter->pwfx->nChannels * (DWORD)pWavWriter->pwfx->wBitsPerSample)/8;	
	pWavWriter->pwfx->nAvgBytesPerSec = pWavWriter->pwfx->nSamplesPerSec * pWavWriter->pwfx->nBlockAlign;

	HRESULT hr = WaveCreateFile(szFile, &pWavWriter->hmmioOut, pWavWriter->pwfx, &pWavWriter->ckOut, &pWavWriter->ckOutRIFF);
	return (hr==NOERROR);
}

BOOL WINAPI WavWriterSetEasyManager(LPWAVWRITER pWavWriter, LPVOID pManager)
{
	pWavWriter->pManager = (IEasyManager*)pManager;
/*
	IUnknown *pUnknown = (IUnknown*)pManager;

	HRESULT hr = pUnknown->QueryInterface(IID_IEasyManager, (void**)&pWavWriter->pManager);
	if(FAILED(hr)) return FALSE;
*/
	return TRUE;
}

BOOL WINAPI WavWriterStartDataWrite(LPWAVWRITER pWavWriter)
{
	if(pWavWriter==NULL)
		return FALSE;

	HRESULT hr = WaveStartDataWrite(&pWavWriter->hmmioOut, &pWavWriter->ckOut, &pWavWriter->mmioinfoOut);
	return (hr==NOERROR);
}

BOOL WINAPI WavWriterWriteData(LPWAVWRITER pWavWriter, LPBYTE pBuffer, DWORD cbBuffer)
{
	if(pWavWriter==NULL)
		return FALSE;

	if(pWavWriter->pManager!=NULL) {
		HRESULT hr = ((IEasyManager*)pWavWriter->pManager)->RunSample(0, pBuffer, cbBuffer);
		if(FAILED(hr)) return FALSE;
	}

	UINT uRetSize=0;
	HRESULT hr = WaveWriteFile(pWavWriter->hmmioOut, cbBuffer, pBuffer, &pWavWriter->ckOut, &uRetSize, &pWavWriter->mmioinfoOut);
	return (hr==NOERROR);
}

BOOL WINAPI WavWriterAppendMute(LPWAVWRITER pWavWriter, UINT umsDuration)
{
	if(pWavWriter==NULL)
		return FALSE;

	UINT uTotalSize = (UINT)(umsDuration / 1000.0 * pWavWriter->pwfx->nAvgBytesPerSec);	
	if(uTotalSize<pWavWriter->pwfx->nBlockAlign) return TRUE;

	LPBYTE pBuffer = (LPBYTE)malloc(pWavWriter->pwfx->nAvgBytesPerSec);
	if(pBuffer==NULL) return FALSE;

	memset(pBuffer, 0, pWavWriter->pwfx->nAvgBytesPerSec);

	UINT uSize=0, uCopySize = 0;
	while(uSize<uTotalSize) {
		uCopySize = uTotalSize - uSize;
		if(uCopySize>pWavWriter->pwfx->nAvgBytesPerSec) uCopySize = pWavWriter->pwfx->nAvgBytesPerSec;
		uCopySize = uCopySize / pWavWriter->pwfx->nBlockAlign * pWavWriter->pwfx->nBlockAlign;
		if(uCopySize<pWavWriter->pwfx->nBlockAlign) break;

		WavWriterWriteData(pWavWriter, pBuffer, uCopySize);
		uSize += uCopySize;
	}

	free(pBuffer);
	return TRUE;
}

BOOL WINAPI WavWriterAppendFile(LPWAVWRITER pWavWriter, UINT umsStart, UINT umsStop, LPSTR szFile)
{
	WAVREADER WavReader;
	if(!WavReaderOpen(&WavReader, szFile))
		return FALSE;

	if((pWavWriter->pwfx->wFormatTag!=WavReader.pwfx->wFormatTag)&&(WavReader.pwfx->wFormatTag!=0)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	UINT uOffsetSize=sizeof(pWavWriter->pwfx->wFormatTag);
	if(0!=memcmp((LPBYTE)pWavWriter->pwfx+uOffsetSize, (LPBYTE)WavReader.pwfx+uOffsetSize, sizeof(WAVEFORMATEX)-uOffsetSize)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	if(!WavReaderStartDataRead(&WavReader)) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	LPBYTE pBuffer = (LPBYTE)malloc(WavReader.pwfx->nAvgBytesPerSec);
	if(pBuffer==NULL) {
		WavReaderClose(&WavReader);
		return FALSE;
	}

	if(umsStop>=WavReader.ckData.cksize) umsStop = WavReader.ckData.cksize - 1;

	BOOL fError = FALSE;
	UINT ii=umsStart;
	UINT uRetSize=0, uActualSize=0;
	while(ii<=umsStop) {
		uActualSize = umsStop - ii + 1;
		if(uActualSize>WavReader.pwfx->nAvgBytesPerSec) uActualSize = WavReader.pwfx->nAvgBytesPerSec;
		uActualSize = uActualSize / WavReader.pwfx->nBlockAlign * WavReader.pwfx->nBlockAlign;
		if(uActualSize<WavReader.pwfx->nBlockAlign) break;

		if(!WavReaderReadDirectData(&WavReader, ii, uActualSize, pBuffer, &uRetSize)) {
			fError = TRUE;
			break;
		}

		if(!WavWriterWriteData(pWavWriter, pBuffer, uRetSize)) {
			fError = TRUE;
			break;
		}

		ii += uActualSize;
	}

	WavReaderClose(&WavReader);
	free(pBuffer);

	return (!fError);
}

BOOL WINAPI WavWriterClose(LPWAVWRITER pWavWriter)
{
	if(pWavWriter==NULL)
		return FALSE;

	if(pWavWriter->pManager!=NULL) {
// We may leak a memory for that!!
//		pWavWriter->pManager->Release();
		pWavWriter->pManager = NULL;
	}

	HRESULT hr = WaveCloseWriteFile(&pWavWriter->hmmioOut, &pWavWriter->ckOut, &pWavWriter->ckOutRIFF, &pWavWriter->mmioinfoOut, 0);
	free(pWavWriter->pwfx); pWavWriter->pwfx = NULL;
	return (hr==NOERROR);
}


BOOL WINAPI WavReaderOpen(LPWAVREADER pWavReader, LPSTR szFile)
{
	if(pWavReader==NULL) 
		return FALSE;

	HRESULT hr = WaveOpenFile(szFile, &pWavReader->hmmio, &pWavReader->pwfx, &pWavReader->ckRiff);
	return (hr==NOERROR);
}

BOOL WINAPI WavReaderStartDataRead(LPWAVREADER pWavReader)
{
	if(pWavReader==NULL) 
		return FALSE;

	HRESULT hr = WaveStartDataRead(&pWavReader->hmmio, &pWavReader->ckData, &pWavReader->ckRiff);
	return (hr==NOERROR);
}

BOOL WINAPI WavReaderReadSerialData(LPWAVREADER pWavReader, UINT uLength, LPBYTE pBuffer, UINT *puRefSize)
{
	if(pWavReader==NULL) 
		return FALSE;

	HRESULT hr = WaveReadFile(pWavReader->hmmio, uLength, pBuffer, &pWavReader->ckData, puRefSize);
	return (hr>=0);
}

BOOL WINAPI WavReaderReadDirectData(LPWAVREADER pWavReader, UINT uStartPos, UINT uLength, LPBYTE pBuffer, UINT *puRetSize)
{
	if(pWavReader==NULL) 
		return FALSE;	

	HRESULT hr = WaveDirectReadFile(pWavReader->hmmio, uStartPos, uLength, pBuffer, &pWavReader->ckData, puRetSize);
	return (hr>=0);
}

BOOL WINAPI WavReaderClose(LPWAVREADER pWavReader)
{
	if(pWavReader==NULL) 
		return FALSE;

	HRESULT hr = WaveCloseReadFile(&pWavReader->hmmio, &pWavReader->pwfx);
	return (hr==NOERROR);
}