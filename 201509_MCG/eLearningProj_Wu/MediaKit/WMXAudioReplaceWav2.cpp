#include "stdafx.h"
#include <stdio.h>
#include <wmsdkidl.h>

#include "WaveLib\HelperWav.h"
#include "WriterBase.h"
#include "WMXAudioReplaceWav.h"
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}


CWMXAudioReplaceWav::CWMXAudioReplaceWav()
{
}

CWMXAudioReplaceWav::~CWMXAudioReplaceWav()
{
}

BOOL CWMXAudioReplaceWav::AutoDeliverMore(QWORD qwDuration)
{
	return CReaderBase::AutoDeliverMore(qwDuration);
/*

	QWORD qwRealDuration = qwDuration;
	if((m_qwUserTime+qwRealDuration)>m_qwLength)
		qwRealDuration = m_qwLength - m_qwUserTime;

	DWORD dwSize = (DWORD)(qwRealDuration/10000) * m_WavReader.pwfx->nSamplesPerSec * m_WavReader.pwfx->nChannels * m_WavReader.pwfx->wBitsPerSample / 8 /1000;

	HRESULT hr = 0;
	INSSBuffer *pNssBuffer=NULL;
	LPBYTE pBuffer=NULL;
	UINT uRetSize=0;
	if(FAILED(m_WriterBase.m_pWriter->AllocateSample(dwSize, &pNssBuffer))) 
		goto ERROR_OUT;

	pNssBuffer->GetBuffer(&pBuffer);
	if(!WavReaderReadDirectData(&m_WavReader, m_uPosWave, dwSize, pBuffer, &uRetSize)) {
		ASSERT(0);
	}

	m_uPosWave += uRetSize;
	hr = m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, m_qwUserTime, 0, pNssBuffer);
	if(FAILED(hr)) // write again!!
		hr = m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, m_qwUserTime, 0, pNssBuffer);
	if(FAILED(hr)) 
		goto ERROR_OUT;

	RELEASE(pNssBuffer);
	return CReaderBase::AutoDeliverMore(qwDuration);
	
ERROR_OUT:
	RELEASE(pNssBuffer);
	m_fEOF = TRUE;
	m_hvalAsync = E_FAIL;
	SetEvent( m_hEventAsync );
	return FALSE;	
*/
}

BOOL CWMXAudioReplaceWav::MakeReplace(BSTR bstrFile, IUnknown *pManager, BSTR bstrReplace, BSTR bstrProfile, BSTR bstrOutput)
{
	m_uPosWave = 0;

	if(!Open(bstrFile))
		return FALSE;

	ConfigAudioOutputCompressed();
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();
	
	if(!m_WriterBase.Create(bstrOutput, bstrProfile))
		return FALSE;

	CString cstReplace(bstrReplace);
	if(!WavReaderOpen(&m_WavReader, (LPSTR)(LPCSTR)cstReplace))
		return E_FAIL;

	if(!WavReaderStartDataRead(&m_WavReader)) {
		WavReaderClose(&m_WavReader);
		return E_FAIL;
	}
	
	m_WriterBase.ConfigAudioInputSimpleUncompressed(m_WavReader.pwfx->wBitsPerSample, m_WavReader.pwfx->nChannels, m_WavReader.pwfx->nSamplesPerSec);
	m_WriterBase.ConfigVideoInputCodecNoNeeded();
	m_WriterBase.ConfigScriptInputCodecNoNeeded();
	
	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, FALSE); // only video is needed!!
	
	if(!m_WriterBase.BeginWriting()) {
		WavReaderClose(&m_WavReader);
		return FALSE;
	}

	
	BOOL bret = Run_AutoMode(0, m_qwLength);

	WavReaderClose(&m_WavReader);

	m_WriterBase.EndWriting();
	m_WriterBase.Close();

	return bret;
}

HRESULT CWMXAudioReplaceWav::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration, DWORD dwFlags, 
						  INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}

HRESULT CWMXAudioReplaceWav::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
						DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	HRESULT hr=0;
	if(wStreamNum==m_wStreamAudio) {
		QWORD qwTime = cnsSampleTime + cnsSampleDuration;
		DWORD dwSize = (DWORD)(qwTime / 10000 * m_WavReader.pwfx->nSamplesPerSec * m_WavReader.pwfx->nChannels * m_WavReader.pwfx->wBitsPerSample / 8 /1000);
		dwSize = dwSize / m_WavReader.pwfx->nBlockAlign * m_WavReader.pwfx->nBlockAlign;
		if(dwSize<m_uPosWave) {
			ASSERT(0);
			return S_OK;
		}

		DWORD dwRealSize = dwSize - m_uPosWave;
		
		INSSBuffer *pNssBuffer=NULL;
		UINT uRetSize=0;
		if(FAILED(m_WriterBase.m_pWriter->AllocateSample(dwRealSize, &pNssBuffer))) 
			return E_FAIL;

		LPBYTE pBuffer=NULL;
		pNssBuffer->GetBuffer(&pBuffer);
		if(!WavReaderReadDirectData(&m_WavReader, m_uPosWave, dwRealSize, pBuffer, &uRetSize)) {
			return S_OK; // no audio!!
		}
		
		m_uPosWave += uRetSize;
		hr = m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, cnsSampleTime, 0, pNssBuffer);
		if(FAILED(hr)) { // write again!!
			hr = m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, m_qwUserTime, 0, pNssBuffer);
			ASSERT(SUCCEEDED(hr));
		}
	
		RELEASE(pNssBuffer);

		if(FAILED(hr))
			return E_FAIL;
	}
	else if(wStreamNum==m_wStreamVideo) {
		hr = m_WriterBase.m_pWriterAdvanced->WriteStreamSample((WORD)m_WriterBase.m_dwInputVideo, cnsSampleTime, 0, cnsSampleDuration, dwFlags, pSample);
	}
		
	return S_OK;
	
}
