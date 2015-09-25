#include <stdio.h>
#include <wmsdkidl.h>

#include "WMXDumper.h"
#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#include "..\VFXManager\VFXMan.h"
#include "..\VFXPlugIn\VFXEffect.h"

CWMXDumper::CWMXDumper()
{	
	m_qwOffset = 0;
	m_qwLastAudioSampleTime = 0;
	m_pWriterBase = NULL;
	m_bInitialScript = FALSE;
	m_bstrInitialType = NULL;
	m_bstrInitialData = NULL;
	m_pAudioManager = NULL;
	m_pVideoManager = NULL;

	m_qwDumpedAudioDuration = 0;
}

CWMXDumper::~CWMXDumper()
{
	if(m_bstrInitialType) {
		SysFreeString(m_bstrInitialType); 
		m_bstrInitialType = NULL;
	}
	if(m_bstrInitialData) {
		SysFreeString(m_bstrInitialData);
		m_bstrInitialData = NULL;
	}
}

void CWMXDumper::SetInitialScript(BOOL bSet, BSTR bstrType, BSTR bstrData)
{
	m_bInitialScript = bSet;
	if(m_bstrInitialType) {
		SysFreeString(m_bstrInitialType); 
		m_bstrInitialType = NULL;
	}
	if(m_bstrInitialData) {
		SysFreeString(m_bstrInitialData);
		m_bstrInitialData = NULL;
	}
	
	m_bstrInitialType = SysAllocString(bstrType);
	m_bstrInitialData = SysAllocString(bstrData);
}

BOOL CWMXDumper::AddInitialScript()
{
	if(m_pWriterBase==NULL)
		return FALSE;

	HRESULT hr=0;
	QWORD cnsSampleTime = m_qwOffset, cnsSampleDuration = 50000;
	DWORD dwCopySize=wcslen(m_bstrInitialType)*2+2+wcslen(m_bstrInitialData)*2+2;
	INSSBuffer *pNssBuffer=NULL;

	hr = m_pWriterBase->m_pWriter->AllocateSample(dwCopySize, &pNssBuffer);
	if(SUCCEEDED(hr)&&(pNssBuffer!=NULL)) {
		pNssBuffer->SetLength(dwCopySize);

		LPBYTE pBuffer=NULL;
		hr = pNssBuffer->GetBuffer(&pBuffer);
		if(SUCCEEDED(hr)&&(pBuffer!=NULL)) {
			memset(pBuffer, 0, dwCopySize);
			memcpy(pBuffer, m_bstrInitialType, wcslen(m_bstrInitialType)*2);
			memcpy(pBuffer+(wcslen(m_bstrInitialType)*2+2), m_bstrInitialData, wcslen(m_bstrInitialData)*2);
		}
		hr = m_pWriterBase->m_pWriterAdvanced->WriteStreamSample(m_pWriterBase->m_wStreamScript, cnsSampleTime, 0, cnsSampleDuration, 0, pNssBuffer);
	}
	RELEASE(pNssBuffer);
	return SUCCEEDED(hr);
}

BOOL CWMXDumper::Run_ManualMode(QWORD qwStart)
{
	if(m_pWriterBase!=NULL) {
		if(m_bInitialScript) AddInitialScript();
	}
	return CReaderBase::Run_ManualMode(qwStart);
}

BOOL CWMXDumper::Run_AutoMode(QWORD qwStart, QWORD qwDuration)
{
	if(m_pWriterBase!=NULL) {
		if(m_bInitialScript) AddInitialScript();
	}
	return CReaderBase::Run_AutoMode(qwStart, qwDuration);
}

HRESULT CWMXDumper::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration, DWORD dwFlags, 
						  INSSBuffer *pSample, void *pvContext)
{
	if(m_pWriterBase== NULL)
		return E_FAIL;

	BOOL fWrite=FALSE;
	DWORD dwWriterInputNum=0;
	QWORD qwTime = qwSampleTime + m_qwOffset - m_qwStartTime;
	if(dwOutputNum==m_dwOutputAudio) {
		m_qwDumpedAudioDuration = qwSampleDuration;

		fWrite = TRUE;
		dwWriterInputNum = m_pWriterBase->m_dwInputAudio;

		if((qwSampleTime+qwSampleDuration)>m_qwLastAudioSampleTime)
			m_qwLastAudioSampleTime = qwSampleTime + qwSampleDuration;
		if(m_pAudioManager) {
			LPBYTE pBuffer=NULL;
			DWORD dwLen=0;
			pSample->GetBufferAndLength(&pBuffer, &dwLen);
			m_pAudioManager->RunSample((LONG)(qwTime/10000), pBuffer, dwLen);
		}
	}
	else if(m_pWriterBase->m_fVideoExisted&&(dwOutputNum==m_dwOutputVideo)) {
		fWrite = TRUE;
		dwWriterInputNum = m_pWriterBase->m_dwInputVideo;
		if(m_pVideoManager) {
			LPBYTE pBuffer=NULL;
			DWORD dwLen=0;
			pSample->GetBufferAndLength(&pBuffer, &dwLen);
			m_pVideoManager->Run((LONG)(qwTime/10000), pBuffer, dwLen);
		}
	}
	else if(m_pWriterBase->m_fScriptExisted&&(dwOutputNum==m_dwOutputScript)) {
		fWrite = TRUE;
		dwWriterInputNum = m_pWriterBase->m_dwInputScript;
	}

	if(fWrite) {		
		return m_pWriterBase->m_pWriter->WriteSample(dwWriterInputNum, qwTime, 0, pSample);
	}

	return S_OK;
}

HRESULT CWMXDumper::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
						DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	if(m_pWriterBase== NULL)
		return E_FAIL;

	BOOL fWrite=FALSE;
	WORD wWriterStreamNum=0;
	if(wStreamNum==m_wStreamAudio) {
		m_qwDumpedAudioDuration = cnsSampleDuration;

		fWrite = TRUE;
		wWriterStreamNum = m_pWriterBase->m_wStreamAudio;
		if((cnsSampleTime+cnsSampleDuration)>m_qwLastAudioSampleTime)
			m_qwLastAudioSampleTime = cnsSampleTime+cnsSampleDuration;
	}
	else if(m_pWriterBase->m_fVideoExisted&&(wStreamNum==m_wStreamVideo)) {
		fWrite = TRUE;
		wWriterStreamNum = m_pWriterBase->m_wStreamVideo;
	}
	else if(m_pWriterBase->m_fScriptExisted&&(wStreamNum==m_wStreamScript)) {
		fWrite = TRUE;
		wWriterStreamNum = m_pWriterBase->m_wStreamScript;
	}

	if(fWrite) {
		cnsSampleTime = cnsSampleTime + m_qwOffset - m_qwStartTime;
		return m_pWriterBase->m_pWriterAdvanced->WriteStreamSample(wWriterStreamNum, cnsSampleTime, 0, cnsSampleDuration, dwFlags, pSample);
	}

	return S_OK;
}
