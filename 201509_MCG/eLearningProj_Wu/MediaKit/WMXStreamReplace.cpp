#include <afxwin.h>
#include <wmsdkidl.h>
#include "WMXStreamReplace.h"

#include "WriterBase.h"
#include "WMXDumper.h"

#include <atlbase.h>

CWMXStreamReplace::CWMXStreamReplace()
{
	m_hWndProgress = NULL;
	m_hWndAbort = NULL;
	m_pbAbort = NULL;
	m_lMsgProgress = 0;
}

CWMXStreamReplace::~CWMXStreamReplace()
{
}

BOOL CWMXStreamReplace::MakeReplace(BSTR bstrWMX, BSTR bstrWMXReplace, BOOL fAudio, BOOL fVideo, BOOL fScript, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput)
{
	CWMXDumper	SourceDumper;
	CWMXDumper	ReplaceDumper;
	CWriterBase	WriterBase;
	QWORD		qwSourceTime=0, qwReplaceTime=0;

	if(!SourceDumper.Open(bstrWMX))
		goto ERROR_REPLACE;

	if(!ReplaceDumper.Open(bstrWMXReplace))
		goto ERROR_REPLACE;

	if(SourceDumper.m_qwLength > ReplaceDumper.m_qwLength) {
		if(IsWindow(m_hWndProgress)) SourceDumper.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
		if(m_pbAbort!=NULL) SourceDumper.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	}
	else {
		if(IsWindow(m_hWndProgress)) ReplaceDumper.SetupProgressHandler(m_hWndProgress, m_lMsgProgress);
		if(m_pbAbort!=NULL) ReplaceDumper.SetupQueryAbort(m_hWndAbort, m_pbAbort);
	}

	if(!WriterBase.Create(bstrOutput, SourceDumper.m_pProfile))
		goto ERROR_REPLACE;

	if(!SourceDumper.Run_ManualMode(0))
		goto ERROR_REPLACE;
	if(!ReplaceDumper.Run_ManualMode(0))
		goto ERROR_REPLACE;

	SourceDumper.ConfigAudioOutputCompressed();
	SourceDumper.ConfigVideoOutputCompressed();
	SourceDumper.ConfigScriptOutputCompressed();
	ReplaceDumper.ConfigAudioOutputCompressed();
	ReplaceDumper.ConfigVideoOutputCompressed();
	ReplaceDumper.ConfigScriptOutputCompressed();

	WriterBase.ConfigAudioInputCodecNoNeeded();
	WriterBase.ConfigVideoInputCodecNoNeeded();
	WriterBase.ConfigScriptInputCodecNoNeeded();

	SourceDumper.SetWriterBase(&WriterBase);
	ReplaceDumper.SetWriterBase(&WriterBase);

	if(fCopyHeader) SourceDumper.CopyAllHeaders(WriterBase.m_pHeaderInfo);
	if(fCopyScript) SourceDumper.CopyScript(WriterBase.m_pHeaderInfo);

	if(!WriterBase.BeginWriting())
		goto ERROR_REPLACE;

	do {
		if(!SourceDumper.m_fEOF) {
			qwSourceTime += TIMEUNIT/5;
			SourceDumper.ManualDeliverTo(qwSourceTime);
			WaitForSingleObject( SourceDumper.m_hTimeAsync, INFINITE );
		}
		if(!ReplaceDumper.m_fEOF) {
			qwReplaceTime += TIMEUNIT/5;
			ReplaceDumper.ManualDeliverTo(qwReplaceTime);
			WaitForSingleObject( ReplaceDumper.m_hTimeAsync, INFINITE );
		}
	} while((!SourceDumper.m_fEOF)||(!ReplaceDumper.m_fEOF));	

ERROR_REPLACE:
	WriterBase.EndWriting();	
	WriterBase.Close();

	if(fCopyMarker) SourceDumper.CopyMarker(bstrOutput);

	SourceDumper.Close();
	ReplaceDumper.Close();	
	
	return TRUE;
}