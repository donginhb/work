#include <afxwin.h>
#include "MultiWMXCopyWav.h"

#include <uuids.h>

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

CMultiWMXCopyWav::CMultiWMXCopyWav()
{
	m_qwSegmentTime = 0;
	m_pWriterBase = NULL;
	m_pWavReader = NULL;
	m_qwAudioDuration = 0;

	EnableAutoProgressReport(FALSE);
}

CMultiWMXCopyWav::~CMultiWMXCopyWav()
{
}

BOOL CMultiWMXCopyWav::AppendWavReader(BOOL fSpaceVideo, UINT &uPosWave, UINT uSize, WAVREADER *pWavReader)
{
	HRESULT hr= 0;
	LPBYTE pBuffer=NULL;
	INSSBuffer *pNSSBuffer=NULL;
	UINT	uStop = uPosWave + uSize;
	UINT	uRetSize=0, uCopySize=0;
	QWORD	qwTime=m_qwSegmentTime;	
	while(uPosWave<uStop) {
		uCopySize = uStop - uPosWave;
		if(uCopySize>pWavReader->pwfx->nAvgBytesPerSec) uCopySize = pWavReader->pwfx->nAvgBytesPerSec;
		uCopySize = uCopySize / pWavReader->pwfx->nBlockAlign * pWavReader->pwfx->nBlockAlign;

		if(uCopySize<pWavReader->pwfx->nBlockAlign) break;

		hr = m_pWriterBase->m_pWriter->AllocateSample(uCopySize, &pNSSBuffer);
		if(FAILED(hr)) return FALSE;
		
		pNSSBuffer->GetBuffer(&pBuffer);

		if(!WavReaderReadDirectData(pWavReader, uPosWave, uCopySize, pBuffer, &uRetSize)) {
			RELEASE(pNSSBuffer);
			return FALSE;
		}		

		hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputAudio, qwTime, 0, pNSSBuffer);
		RELEASE(pNSSBuffer);
		if(FAILED(hr)) return FALSE;		

		uPosWave += uCopySize;
		qwTime += (QWORD)(uCopySize * 1000.0 / pWavReader->pwfx->nAvgBytesPerSec) * 10000L;

		ReportMultiProgress(m_hWndProgress, m_lMsgProgress, qwTime);
	}

	m_qwSegmentTime = qwTime;	

	return TRUE;
}

BOOL CMultiWMXCopyWav::AddWMXFile(BSTR bstrWMX, DWORD dwmsStart, DWORD dwmsStop, WAVREADER *pWavReader, UINT &uPosWave)
{
	if(m_pWriterBase==NULL)
		return FALSE;

	m_pWavReader = pWavReader;

	if(!Open(bstrWMX)) return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, FALSE);

	ConfigAudioOutputSimpleUncompressed(pWavReader->pwfx->wBitsPerSample, pWavReader->pwfx->nChannels, pWavReader->pwfx->nSamplesPerSec);	
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();

	BOOL fError = FALSE;
	m_uPosWave = uPosWave;
	m_qwStartTime = (LONGLONG)dwmsStart * 10000L;

	if(dwmsStop>=(DWORD)(m_qwLength/10000))
		dwmsStop = (DWORD)(m_qwLength/10000);

	m_qwStopTime = (LONGLONG)dwmsStop * 10000L;

	if(Run_ManualMode(m_qwStartTime)) {
		WaitForStop(m_hTimeAsync, INFINITE);
		DWORD dwmsTime = dwmsStart;
		DWORD dwmsDeliverSize=0;
		while(dwmsTime<dwmsStop) {
			dwmsDeliverSize = dwmsStop - dwmsTime;
			if(dwmsDeliverSize>1000) dwmsDeliverSize = 1000; // one second!!

			dwmsTime += dwmsDeliverSize;

			if(!ManualDeliverTo((QWORD)dwmsTime * 10000L)) { // also change m_qwSegmentTime!!
				fError = TRUE;
				break;
			}

			ReportMultiProgress(m_hWndProgress, m_lMsgProgress, m_qwSegmentTime + (QWORD)(dwmsTime-dwmsStart)*10000);

			WaitForStop(m_hTimeAsync, INFINITE);
		}
	}

	m_pReader->Stop();

	Close();

	m_qwSegmentTime += (QWORD)(dwmsStop - dwmsStart) * 10000L;
	uPosWave = m_uPosWave;

	return (!fError);
}

///////////////////////////////////////////////////////////////////////
//
HRESULT CMultiWMXCopyWav::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{	
	if(qwSampleTime<m_qwStartTime) return S_OK;	
	if(dwOutputNum!=m_dwOutputAudio) return S_OK;

// to add this period seems to be strange but indeed, it makes better results.
	if(qwSampleTime>=m_qwStopTime)
		return S_OK;
	if((qwSampleTime+qwSampleDuration)>m_qwStopTime) 
		return S_OK;
	
	DWORD dwLength=0;
	LPBYTE pBuffer=NULL;
	HRESULT hr = pSample->GetBufferAndLength(&pBuffer, &dwLength);
	if(FAILED(hr)) return hr;

	if(dwOutputNum==m_dwOutputAudio) {
		m_qwAudioDuration = qwSampleDuration;

		DWORD dwNewLength=0;
		dwNewLength = dwLength / m_pWavReader->pwfx->nBlockAlign *m_pWavReader->pwfx->nBlockAlign;
		ASSERT(dwNewLength==dwLength);
		
		UINT uRetSize=0;
		if(!WavReaderReadDirectData(m_pWavReader, m_uPosWave, dwLength, pBuffer, &uRetSize))
			return S_OK;
		
		m_uPosWave += dwNewLength;

		hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputAudio, m_qwSegmentTime + qwSampleTime - m_qwStartTime, dwFlags, pSample);
		if(FAILED(hr)) return hr;
	}

	return S_OK;
}
	
HRESULT CMultiWMXCopyWav::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	if(cnsSampleTime<m_qwStartTime) return S_OK;	

	WORD	wWriterStream=0;
	if(wStreamNum==m_wStreamVideo) wWriterStream = m_pWriterBase->m_wStreamVideo;
	else if(wStreamNum==m_wStreamScript) wWriterStream = m_pWriterBase->m_wStreamScript;
	else return S_OK;

	HRESULT hr = m_pWriterBase->m_pWriterAdvanced->WriteStreamSample(wWriterStream, 
			m_qwSegmentTime + cnsSampleTime - m_qwStartTime, 0, cnsSampleDuration, dwFlags, pSample);

	return S_OK;
}
