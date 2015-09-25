#include <afxwin.h>
#include <wmsdkidl.h>

#include "WMX2Media.h"
#include "WriterBase.h"

CWMX2Media::CWMX2Media()
{
	memset(&m_WavWriter, 0, sizeof(m_WavWriter));
}

CWMX2Media::~CWMX2Media()
{
}

BOOL CWMX2Media::MakeWave(BSTR bstrSource, WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate, BSTR bstrOutput)
{
	if(!Open(bstrSource))
		return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, FALSE, FALSE);

	if(!ConfigAudioOutputSimpleUncompressed(wBitsPerSample, wChannels, dwSampleRate))
		return FALSE;
	
	WM_MEDIA_TYPE *pwmt = GetAudioOutputFormat();
	if(pwmt==NULL) return FALSE;

	HRESULT hr=E_FAIL;
	WAVEFORMATEX *pwfx = (WAVEFORMATEX *)pwmt->pbFormat;

	BOOL fOK=FALSE;
	if(pwfx!=NULL) {
		CString cst(bstrOutput);
		fOK = WavWriterCreate_PCM(&m_WavWriter, pwfx->wBitsPerSample, pwfx->nChannels, pwfx->nSamplesPerSec, (LPSTR)(LPCSTR)cst);
	}
	FreeFormatAllocated(pwmt);

	if(!fOK) return FALSE;

	if(!WavWriterStartDataWrite(&m_WavWriter)) 
		return FALSE;

	if(!Run_AutoMode(0, 0))
		return FALSE;

	Close();

	WavWriterClose(&m_WavWriter);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CWMX2Media::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputAudio)
		return S_OK;


	DWORD	dwLength=0;
	LPBYTE	pBuffer=NULL;
	UINT	uRetSize=0;
	if(FAILED(pSample->GetBufferAndLength(&pBuffer, &dwLength)))
		return E_FAIL;

	if(!WavWriterWriteData(&m_WavWriter, pBuffer, dwLength))
		return E_FAIL;
	return S_OK;
}
	
HRESULT CWMX2Media::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}
