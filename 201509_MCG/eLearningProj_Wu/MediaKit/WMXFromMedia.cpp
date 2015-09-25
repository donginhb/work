#include <afxwin.h>
#include "WMXFromMedia.h"

CWMXFromMedia::CWMXFromMedia()
{
	m_ulPos = 0;
	memset(&m_WavReader, 0, sizeof(m_WavReader));
}

CWMXFromMedia::~CWMXFromMedia()
{
}

BOOL CWMXFromMedia::WMXFromWav(BSTR bstrWMX, BSTR bstrProfile, BSTR bstrWav, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput)
{
	BOOL		fOK=FALSE;
	CString		szFile(bstrWav);

	if(!WavReaderOpen(&m_WavReader, (LPTSTR)(LPCSTR)szFile))
		return FALSE;

	m_ulPos = 0;

	if(!WavReaderStartDataRead(&m_WavReader))
		goto ERROR_FROMWAV;

	if(!m_WriterBase.Create(bstrOutput, bstrProfile))
		goto ERROR_FROMWAV;

	if(!m_WriterBase.ConfigAudioInputSimpleUncompressed(m_WavReader.pwfx->wBitsPerSample, m_WavReader.pwfx->nChannels, m_WavReader.pwfx->nSamplesPerSec))
		goto ERROR_FROMWAV;

	m_WriterBase.ConfigVideoInputCodecNoNeeded();
	m_WriterBase.ConfigScriptInputCodecNoNeeded();

	if(!Open(bstrWMX))
		goto ERROR_FROMWAV;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, TRUE);

	ConfigAudioOutputSimpleUncompressed(m_WavReader.pwfx->wBitsPerSample, m_WavReader.pwfx->nChannels, m_WavReader.pwfx->nSamplesPerSec);
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();

	if(fCopyHeader) CopyAllHeaders(m_WriterBase.m_pHeaderInfo);
	if(fCopyScript) CopyScript(m_WriterBase.m_pHeaderInfo);

	if(!m_WriterBase.BeginWriting())
		goto ERROR_FROMWAV;

	fOK = Run_AutoMode(0, 0);

	m_WriterBase.EndWriting();
	if(!fOK) goto ERROR_FROMWAV;

	fOK = TRUE;
ERROR_FROMWAV:
	
	m_WriterBase.Close();

	if(fCopyMarker) CopyMarker(bstrOutput);

	WavReaderClose(&m_WavReader);
	Close();
	
	return fOK;
}

///////////////////////////////////////////////////////////////////////
//
HRESULT CWMXFromMedia::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputAudio)
		return S_OK;

	DWORD	dwLength=0;
	LPBYTE	pBuffer=NULL;
	UINT	uRetSize=0;
	if(FAILED(pSample->GetBufferAndLength(&pBuffer, &dwLength)))
		return E_FAIL;

	DWORD dwNewLength = dwLength*m_WavReader.pwfx->nBlockAlign/m_WavReader.pwfx->nBlockAlign;
	ASSERT(dwLength == dwNewLength);
	if(!WavReaderReadDirectData(&m_WavReader, m_ulPos, dwNewLength, pBuffer, &uRetSize))
		return E_FAIL;

	pSample->SetLength(dwNewLength);
	m_ulPos += dwNewLength;

	return m_WriterBase.m_pWriter->WriteSample(m_WriterBase.m_dwInputAudio, qwSampleTime, 0, pSample);	
}
	
HRESULT CWMXFromMedia::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	WORD wStream=0;
	if(wStreamNum==m_wStreamAudio) return S_OK;
	if(wStreamNum==m_wStreamVideo) wStream = m_WriterBase.m_wStreamVideo;
	if(wStreamNum==m_wStreamScript) wStream = m_WriterBase.m_wStreamScript;

	return m_WriterBase.m_pWriterAdvanced->WriteStreamSample(wStream, cnsSampleTime, 0, cnsSampleDuration, dwFlags, pSample);
}
