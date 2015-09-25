#include <afxwin.h>
#include <wmsdkidl.h>

#include "WMXScriptDump.h"

CWMXScriptDump::CWMXScriptDump()
{
	m_pCmdArray = NULL;
	m_pTextArray = NULL;
	m_pTimeArray = NULL;
}

CWMXScriptDump::~CWMXScriptDump()
{
}

BOOL CWMXScriptDump::Script2Array(BSTR bstrWMX, CStringArray &CmdArray, CStringArray &TextArray, CDWordArray &TimeArray)
{
	CmdArray.RemoveAll();
	TextArray.RemoveAll();
	TimeArray.RemoveAll();

	m_pCmdArray = &CmdArray;
	m_pTextArray = &TextArray;
	m_pTimeArray = &TimeArray;

	if(!Open(bstrWMX))
		return FALSE;

	BOOL fOK=FALSE;
	if(m_fScriptExisted) {
		SetUserClock(TRUE);
		SetSelectedStreams(FALSE, FALSE, TRUE);
	
		fOK = Run_AutoMode(0, 0);
		
		Close();
	}

	return fOK;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CWMXScriptDump::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
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
	
HRESULT CWMXScriptDump::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}
