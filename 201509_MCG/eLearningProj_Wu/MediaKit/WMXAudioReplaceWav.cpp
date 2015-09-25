#include "stdafx.h"
#include <stdio.h>
#include <wmsdkidl.h>

#include "WaveLib\HelperWav.h"
#include "WriterBase.h"
#include "WMXAudioReplaceWav.h"
#include "..\VFXManager\VFXMan.h"
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

// AllocSample should do it each cycle
// if use global sample, there are some strange problem...

CWMXAudioReplaceWav::CWMXAudioReplaceWav()
{
	m_pManager = NULL;
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

BOOL CWMXAudioReplaceWav::MakeReplace(BSTR bstrFile, IEasyManager *pManager, BSTR bstrReplace, BSTR bstrProfile, BSTR bstrOutput)
{
	m_uPosWave = 0;
	m_pManager = pManager;

	if(!Open(bstrFile))
		return FALSE;
	
	if(!m_WriterBase.Create(bstrOutput, bstrProfile))
		return FALSE;

	CString cstReplace(bstrReplace);
	if(!WavReaderOpen(&m_WavReader, (LPSTR)(LPCSTR)cstReplace))
		return E_FAIL;

	if(!WavReaderStartDataRead(&m_WavReader)) {
		WavReaderClose(&m_WavReader);
		return E_FAIL;
	}
	
	ConfigAudioOutputSimpleUncompressed(m_WavReader.pwfx->wBitsPerSample, m_WavReader.pwfx->nChannels, m_WavReader.pwfx->nSamplesPerSec);
	ConfigVideoOutputSimpleUncompressed(24, BI_RGB);
	ConfigScriptOutputCompressed();
	
	m_WriterBase.ConfigAudioInputSimpleUncompressed(m_WavReader.pwfx->wBitsPerSample, m_WavReader.pwfx->nChannels, m_WavReader.pwfx->nSamplesPerSec);
	m_WriterBase.ConfigVideoInputSimpleUncompressed(24, BI_RGB);
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

	m_pManager = NULL;

	return bret;
}

// each time fill by 1 sec audio buffer
HRESULT CWMXAudioReplaceWav::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration, DWORD dwFlags, 
						  INSSBuffer *pSample, void *pvContext)
{
	HRESULT hr=0;
	if(dwOutputNum==m_dwOutputAudio) {
		DWORD dwSize = (m_WavReader.pwfx->nSamplesPerSec * m_WavReader.pwfx->nChannels * m_WavReader.pwfx->wBitsPerSample / 8);

		LPBYTE pBuffer = NULL;
		INSSBuffer *pNssBuffer=NULL;
		hr = m_WriterBase.m_pWriter->AllocateSample(dwSize, &pNssBuffer);
		if(FAILED(hr)) return E_FAIL;

		pNssBuffer->GetBuffer(&pBuffer);
		ZeroMemory(pBuffer, dwSize);
		
		QWORD qwTime = qwSampleTime;
		DWORD dwPos = (DWORD)(qwTime * m_WavReader.pwfx->nSamplesPerSec * m_WavReader.pwfx->nChannels * m_WavReader.pwfx->wBitsPerSample / 8 / 1000 / 10000);
		while(m_uPosWave < dwPos) {
			UINT uRetSize=0;
			if(!WavReaderReadDirectData(&m_WavReader, m_uPosWave, dwSize, pBuffer, &uRetSize)) 
				break;

			QWORD qwMediaTime = (QWORD)10000 * 1000 * m_uPosWave / (m_WavReader.pwfx->nSamplesPerSec * m_WavReader.pwfx->nChannels * m_WavReader.pwfx->wBitsPerSample / 8);
			if(m_pManager) {
				m_pManager->RunSample((LONG)(qwMediaTime/10000), pBuffer, dwSize);
			}	
			hr = m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, qwMediaTime, 0, pNssBuffer);
			ASSERT(SUCCEEDED(hr));
			m_uPosWave += uRetSize;
		}
		RELEASE(pNssBuffer);

	}
	else if(dwOutputNum==m_dwOutputVideo) {
		if(m_pManager) {
			LPBYTE pBuffer=NULL;
			DWORD dwLen=0;
			pSample->GetBufferAndLength(&pBuffer, &dwLen);
			m_pManager->Run((LONG)(qwSampleTime/10000), pBuffer, dwLen);
		}
		m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputVideo, qwSampleTime, 0, pSample);	
	}

	return S_OK;
}

HRESULT CWMXAudioReplaceWav::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
						DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{	
	return S_OK;
}
