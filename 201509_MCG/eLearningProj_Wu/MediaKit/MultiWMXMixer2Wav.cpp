#include <afxwin.h>
#include <wmsdkidl.h>

#include "MultiWMXMixer2Wav.h"
#include "WriterBase.h"


CMultiWMXMixer2Wav::CMultiWMXMixer2Wav()
{
	m_pWavWriter = NULL;
	m_pMixerBuffer = NULL;
	m_uPosWork = 0;
	m_pWavReader = NULL;

	EnableAutoProgressReport(FALSE);
}

CMultiWMXMixer2Wav::~CMultiWMXMixer2Wav()
{
	if(m_pMixerBuffer!=NULL) {
		free(m_pMixerBuffer);
		m_pMixerBuffer = NULL;
	}
}

BOOL CMultiWMXMixer2Wav::SetWriter(WAVWRITER *pWavWriter)
{
	m_pWavWriter = pWavWriter;

	if(m_pMixerBuffer!=NULL) {
		free(m_pMixerBuffer);
		m_pMixerBuffer = NULL;
	}

	if(m_pWavWriter!=NULL) {		
		m_pMixerBuffer = (LPBYTE)malloc(m_pWavWriter->pwfx->nAvgBytesPerSec);
		if(m_pMixerBuffer==NULL)
			return FALSE;
	}
	return TRUE;
}

BOOL CMultiWMXMixer2Wav::MixerWMXAudio(BSTR bstrSource, long lmsStart, long lmsStop, WAVREADER *pWavReader, UINT &uPosWave)
{
	if(m_pWavWriter==NULL)
		return FALSE;

#ifdef _DEBUG
	CString _cst(bstrSource);
#endif
	if(!Open(bstrSource))
		return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, FALSE, FALSE);

	if(!ConfigAudioOutputSimpleUncompressed(m_pWavWriter->pwfx->wBitsPerSample, m_pWavWriter->pwfx->nChannels, m_pWavWriter->pwfx->nSamplesPerSec))
		return FALSE;	

	m_pWavReader = pWavReader;
	m_uPosWork = uPosWave;

	if(!Run_AutoMode((LONGLONG)lmsStart * 10000L, (LONGLONG)(lmsStop-lmsStart)*10000L))
		return FALSE;

	uPosWave = m_uPosWork;	

	Close();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CMultiWMXMixer2Wav::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputAudio)
		return S_OK;

	if(m_pWavWriter==NULL)
		return E_FAIL;

	if(m_pWavReader==NULL)
		return E_FAIL;

	if(m_fUserCheckStop&&((qwSampleTime+qwSampleDuration)>m_qwUserForceStopTime))
		return S_OK;

	DWORD	dwLength=0;
	LPBYTE	pBuffer=NULL;
	UINT	uRetSize=0;
	if(FAILED(pSample->GetBufferAndLength(&pBuffer, &dwLength)))
		return E_FAIL;

	dwLength = dwLength / m_pWavWriter->pwfx->nBlockAlign * m_pWavWriter->pwfx->nBlockAlign;
	if(!WavReaderReadDirectData(m_pWavReader, m_uPosWork, dwLength, m_pMixerBuffer, &uRetSize)) 
		return E_FAIL;

	m_uPosWork += dwLength;

	for(DWORD ii=0; ii<dwLength; ii+=m_pWavWriter->pwfx->nBlockAlign) {
		long lValue=0;
		for(UINT jj=0; jj<m_pWavWriter->pwfx->nChannels; jj++) {
			switch(m_pWavWriter->pwfx->wBitsPerSample) {
			case 8:
				lValue = (long)(pBuffer[ii+jj]) - 128 + (long)(m_pMixerBuffer[ii+jj]) - 128;
				if(lValue>127) lValue = 127;
				if(lValue<-127) lValue = -127;
				pBuffer[ii+jj] = (BYTE)(lValue + 128);
				break;
			case 16:					
				lValue = (long)(*(short*)(pBuffer+ii+(jj<<1))) + (long)(*(short*)(m_pMixerBuffer+ii+(jj<<1)));
				if(lValue>32767L) lValue = 32767L;
				if(lValue<-32767L) lValue = -32767L;
				*(short*)(pBuffer+ii+(jj<<1)) = (short)lValue;
				break;
			}
		}
	}

	if(!WavWriterWriteData(m_pWavWriter, pBuffer, dwLength))
		return E_FAIL;
	
	return S_OK;
}
	
HRESULT CMultiWMXMixer2Wav::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}
