#include <afxwin.h>
#include <wmsdkidl.h>

#include "WriterBase.h"
#include "WMXCopy.h"

CWMXCopy::CWMXCopy() 
{
}
CWMXCopy::~CWMXCopy() 
{
}

BOOL CWMXCopy::MakeCopy(BSTR bstrWMX, BOOL fVideo, BOOL fScript, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput)
{	
	CWriterBase WriterBase;

	if(!Open(bstrWMX)) 
		return FALSE;

	if(!WriterBase.Create(bstrOutput, m_pProfile))
		return FALSE;

	SetWriterBase(&WriterBase);

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, fVideo&&WriterBase.m_fVideoExisted, fScript&&WriterBase.m_fScriptExisted);

	ConfigAudioOutputCompressed();
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();

	WriterBase.ConfigAudioInputCodecNoNeeded();
	WriterBase.ConfigVideoInputCodecNoNeeded();
	WriterBase.ConfigScriptInputCodecNoNeeded();

	// CopyAllHeaders and CopyScript Should Run "Before" BeginWriting!!
	if(fCopyHeader) CopyAllHeaders(WriterBase.m_pHeaderInfo);
	if(fCopyScript) CopyScript(WriterBase.m_pHeaderInfo);

	if(!WriterBase.BeginWriting())
		return FALSE;
	
	BOOL fOK = Run_AutoMode(0, 0);
	
	WriterBase.EndWriting();
	WriterBase.Close();

	if(fCopyMarker) CopyMarker(bstrOutput);

	Close();

	return fOK;
}