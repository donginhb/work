#include <afxwin.h>
#include <wmsdkidl.h>

#include "WMXAVScript.h"

CWMXAVScript::CWMXAVScript()
{
	m_pCmdArray = NULL;
	m_pTextArray = NULL;
	m_pTimeArray = NULL;
}

CWMXAVScript::~CWMXAVScript()
{
}

BOOL CWMXAVScript::MakeAVScript(BSTR bstrWMX, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutWMX, CStringArray &CmdArray, CStringArray &TextArray, CDWordArray &TimeArray)
{
	CmdArray.RemoveAll();
	TextArray.RemoveAll();
	TimeArray.RemoveAll();

	m_pCmdArray = &CmdArray;
	m_pTextArray = &TextArray;
	m_pTimeArray = &TimeArray;

	if(!Open(bstrWMX))
		return FALSE;

	if(!m_WriterBase.Create(bstrOutWMX, m_pProfile))
		return FALSE;

	BOOL fOK=FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, TRUE);

	ConfigAudioOutputCompressed();
	ConfigVideoOutputCompressed();

	m_WriterBase.ConfigAudioInputCodecNoNeeded();
	m_WriterBase.ConfigVideoInputCodecNoNeeded();

	if(fCopyHeader) CopyAllHeaders(m_WriterBase.m_pHeaderInfo);
	if(fCopyScript) CopyScript(m_WriterBase.m_pHeaderInfo);

	if(!m_WriterBase.BeginWriting())
		return FALSE;

	fOK = Run_AutoMode(0, 0);
		
	m_WriterBase.EndWriting();
	m_WriterBase.Close();

	if(fCopyMarker) CopyMarker(bstrOutWMX);

	Close();
	return fOK;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CWMXAVScript::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputScript)
		return S_OK;

	DWORD	dwLength=0;
	BYTE	*pBuffer=NULL;
	if(S_OK==pSample->GetBufferAndLength(&pBuffer, &dwLength)) {
		CString cstCmd((WCHAR*)pBuffer);
		DWORD dwIndex=0;
		while((dwIndex<dwLength)&&(pBuffer[dwIndex]!=0)) dwIndex+=2;
		if(dwIndex+2<dwLength) {
			CString cstText((WCHAR*)(pBuffer+dwIndex+2));
			if(m_pCmdArray!=NULL) m_pCmdArray->Add(cstCmd);
			if(m_pTextArray!=NULL) m_pTextArray->Add(cstText);
			if(m_pTimeArray!=NULL) m_pTimeArray->Add((DWORD)(qwSampleTime/10000));
		}
	}

	return S_OK;
}
	
HRESULT CWMXAVScript::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return m_WriterBase.m_pWriterAdvanced->WriteStreamSample(wStreamNum, cnsSampleTime, 0, cnsSampleDuration, dwFlags, pSample);	
}
