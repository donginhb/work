#include <afxwin.h>
#include <wmsdkidl.h>

#include "MultiWMXAudio2Wav.h"
#include "WriterBase.h"


CMultiWMXAudio2Wav::CMultiWMXAudio2Wav()
{
	m_pWavWriter = NULL;
	m_dwTimeOffset = 0;
	m_qwStartTime = 0;
	m_qwStopTime = 0;
	
	EnableAutoProgressReport(FALSE);
}

CMultiWMXAudio2Wav::~CMultiWMXAudio2Wav()
{
}

BOOL CMultiWMXAudio2Wav::AddWMXAudio(BSTR bstrSource, long lmsStart, long lmsStop)
{
	if(m_pWavWriter==NULL)
		return FALSE;

	if(!Open(bstrSource))
		return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, FALSE, FALSE);

	if(!ConfigAudioOutputSimpleUncompressed(m_pWavWriter->pwfx->wBitsPerSample, m_pWavWriter->pwfx->nChannels, m_pWavWriter->pwfx->nSamplesPerSec))
		return FALSE;	

	m_qwStartTime = (LONGLONG)lmsStart * 10000L;
	m_qwStopTime = (LONGLONG)lmsStop * 10000L;
	// No actual reason for seek has bugs so it is just a dirty fix!!
	if(!Run_AutoMode(0, m_qwStopTime))
		return FALSE;
	
	m_dwTimeOffset += lmsStop - lmsStart;
	ReportMultiProgress(m_hWndProgress, m_lMsgProgress, (QWORD)m_dwTimeOffset*10000);

	Close();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CMultiWMXAudio2Wav::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputAudio)
		return S_OK;

	if(qwSampleTime<m_qwStartTime)
		return S_OK;
	
	if(qwSampleTime>=m_qwStopTime)
		return S_OK;
	
	if(qwSampleTime+qwSampleDuration>m_qwStopTime)
		return S_OK;

	if(m_pWavWriter==NULL)
		return E_FAIL;

	if(m_fUserCheckStop&&((qwSampleTime+qwSampleDuration)>m_qwUserForceStopTime))
		return S_OK;

	DWORD	dwLength=0;
	LPBYTE	pBuffer=NULL;
	UINT	uRetSize=0;
	if(FAILED(pSample->GetBufferAndLength(&pBuffer, &dwLength)))
		return E_FAIL;

	dwLength = dwLength / m_pWavWriter->pwfx->nBlockAlign * m_pWavWriter->pwfx->nBlockAlign;

	if(!WavWriterWriteData(m_pWavWriter, pBuffer, dwLength))
		return E_FAIL;
	
	return S_OK;
}
	
HRESULT CMultiWMXAudio2Wav::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}
