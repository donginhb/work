#include <afxwin.h>
#include <wmsdkidl.h>

#include "MultiWMXCopy.h"
#include "WriterBase.h"

CMultiWMXCopy::CMultiWMXCopy()
{
	m_qwAlignTime = 1; // default!!
	m_qwSegmentTime = 0;
	m_qwStartTime = 0;

	m_pWriterBase = NULL;
	m_qwMaxTime = 0;
	m_qwAudioMaxTime = 0;
	m_qwAudioAdjustTime = 0;
	m_fFirstAudioSample = FALSE;

	EnableAutoProgressReport(FALSE);
}

CMultiWMXCopy::~CMultiWMXCopy()
{
}

BOOL CMultiWMXCopy::AddWMX(BSTR bstrSource, long lmsStart, long lmsStop, BOOL fVideo, BOOL fScript)
{
	if(m_pWriterBase==NULL)
		return E_FAIL;

	if(!Open(bstrSource))
		return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, fVideo, fScript);

	ConfigAudioOutputCompressed();
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();

	m_fFirstAudioSample = TRUE;
	m_qwStartTime = (LONGLONG)lmsStart * 10000L;
	m_qwStartTime = m_qwStartTime / m_qwAlignTime * m_qwAlignTime;
	QWORD qwStopTime = (LONGLONG)lmsStop * 10000L;
	qwStopTime = qwStopTime / m_qwAlignTime * m_qwAlignTime;
	QWORD qwDuration = qwStopTime - m_qwStartTime;
	if(!Run_AutoMode(m_qwStartTime, qwDuration))
		return FALSE;

	ReportMultiProgress(m_hWndProgress, m_lMsgProgress, m_qwSegmentTime + qwDuration);

	Close();

/*
	QWORD qwCorrectTime = m_qwAudioMaxTime+m_qwAlignTime-1;
*/
	QWORD qwCorrectTime = m_qwMaxTime+m_qwAlignTime-1;
	qwCorrectTime = qwCorrectTime / m_qwAlignTime * m_qwAlignTime;
	qwDuration = qwCorrectTime;
	
	m_qwSegmentTime += qwDuration;
	m_qwMaxTime = 0;
	m_qwAudioMaxTime = 0;
	m_qwAudioAdjustTime = 0;
	m_fFirstAudioSample = FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//

HRESULT CMultiWMXCopy::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	return S_OK;
}
	
HRESULT CMultiWMXCopy::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	if(m_pWriterBase==NULL)
		return E_FAIL;

	if(cnsSampleTime<m_qwStartTime) 
		return S_OK;

	QWORD	qwSampleTime=cnsSampleTime;
	WORD	wWriterStream=0;
	if(wStreamNum==m_wStreamAudio) {
		wWriterStream = m_pWriterBase->m_wStreamAudio;
		if(m_fFirstAudioSample) {
			m_qwAudioAdjustTime = qwSampleTime - m_qwStartTime;
			m_fFirstAudioSample = FALSE;
		}

		qwSampleTime -= m_qwAudioAdjustTime;
		if((qwSampleTime+cnsSampleDuration-m_qwStartTime)>m_qwAudioMaxTime) 
			m_qwAudioMaxTime = qwSampleTime + cnsSampleDuration - m_qwStartTime;
	}
	else if(wStreamNum==m_wStreamVideo) wWriterStream = m_pWriterBase->m_wStreamVideo;
	else if(wStreamNum==m_wStreamScript) wWriterStream = m_pWriterBase->m_wStreamScript;
	else return S_OK;

	if((cnsSampleTime+cnsSampleDuration-m_qwStartTime)>m_qwMaxTime)
		m_qwMaxTime = cnsSampleTime + cnsSampleDuration - m_qwStartTime;
	
	HRESULT hr = m_pWriterBase->m_pWriterAdvanced->WriteStreamSample(wWriterStream,
			m_qwSegmentTime + cnsSampleTime - m_qwStartTime, 0, cnsSampleDuration, dwFlags, pSample);

	return S_OK;
}
