#ifndef _HELPERWAV_H_
#define _HELPERWAV_H_

#include <wtypes.h>
#include <mmsystem.h>

interface IEasyManager;
typedef struct _tagWAVWRITER {
	HMMIO			hmmioOut;
	MMCKINFO		ckOut;
	MMCKINFO		ckOutRIFF;
	MMIOINFO		mmioinfoOut;
	WAVEFORMATEX	*pwfx;
	IEasyManager	*pManager;
} WAVWRITER, *LPWAVWRITER;

BOOL WINAPI WavWriterCreate_PCM(LPWAVWRITER pWavWriter, WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate, LPSTR szFile);
BOOL WINAPI WavWriterSetEasyManager(LPWAVWRITER pWavWriter, LPVOID pManager);
BOOL WINAPI WavWriterStartDataWrite(LPWAVWRITER pWavWriter);
BOOL WINAPI WavWriterWriteData(LPWAVWRITER pWavWriter, LPBYTE pBuffer, DWORD cbBuffer);
BOOL WINAPI WavWriterAppendMute(LPWAVWRITER pWavWriter, UINT umsDuration);
BOOL WINAPI WavWriterAppendFile(LPWAVWRITER pWavWriter, UINT umsStart, UINT umsStop, LPSTR szFile);
BOOL WINAPI WavWriterClose(LPWAVWRITER pWavWriter);

typedef struct _tagWAVREADER {
	HMMIO		 hmmio;
	WAVEFORMATEX *pwfx;
	MMCKINFO	 ckRiff;
	MMCKINFO	 ckData;
} WAVREADER, *LPWAVREADER;

BOOL WINAPI WavReaderOpen(LPWAVREADER pWavReader, LPSTR szFile);
BOOL WINAPI WavReaderStartDataRead(LPWAVREADER pWavReader);
BOOL WINAPI WavReaderReadSerialData(LPWAVREADER pWavReader, UINT uLength, LPBYTE pBuffer, UINT *pdwRefSize);
BOOL WINAPI WavReaderReadDirectData(LPWAVREADER pWavReader, UINT uStartPos, UINT uLength, LPBYTE pBuffer, UINT *pdwRefSize);
BOOL WINAPI WavReaderClose(LPWAVREADER pWavReader);

#endif