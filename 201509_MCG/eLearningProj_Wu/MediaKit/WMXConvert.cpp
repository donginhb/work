#include <afxwin.h>
#include <wmsdkidl.h>

#include "WriterBase.h"
#include "WMXConvert.h"

CWMXConvert::CWMXConvert() 
{
}
CWMXConvert::~CWMXConvert() 
{
}

BOOL CWMXConvert::MakeConvert(BSTR bstrWMX, BSTR bstrProfile, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput)
{	
	CWriterBase WriterBase;

	if(!WriterBase.Create(bstrOutput, bstrProfile))
		return FALSE;

	if(!Open(bstrWMX)) 
		return FALSE;

	SetWriterBase(&WriterBase);

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, m_fVideoExisted&&WriterBase.m_fVideoExisted, m_fScriptExisted&&WriterBase.m_fScriptExisted);

	BOOL fOK=FALSE;
	WM_MEDIA_TYPE *pmt=NULL;

	ConfigAudioOutputSimpleUncompressed(16, 2, 44100); // force to avoid convertion decay!!
	WriterBase.ConfigAudioInputSimpleUncompressed(16, 2, 44100);
	WM_MEDIA_TYPE *pwmt = WriterBase.GetVideoInputFormat();
	if(pwmt) {
		WMVIDEOINFOHEADER *pvih=(WMVIDEOINFOHEADER *)pwmt->pbFormat;
		ConfigVideoOutputSimpleUncompressedDim(pvih->bmiHeader.biWidth, pvih->bmiHeader.biHeight);
		FreeFormatAllocated(pwmt); pwmt = NULL;
	}
	
/*
	pmt = GetAudioOutputFormat();
	if(pmt==NULL) return FALSE;
	fOK = WriterBase.ConfigAudioInputFormat(pmt);
	FreeFormatAllocated(pmt);
	if(!fOK) return FALSE;
*/
	if(WriterBase.m_fVideoExisted&&m_fVideoExisted) {
		pmt = GetVideoOutputFormat();
		if(pmt==NULL) return FALSE;
		fOK = WriterBase.ConfigVideoInputFormat(pmt);
		FreeFormatAllocated(pmt);
		if(!fOK) return FALSE;
	}

	// CopyAllHeaders and CopyScript Should Run "Before" BeginWriting!!
	if(fCopyHeader) CopyAllHeaders(WriterBase.m_pHeaderInfo);
	if(fCopyScript) CopyScript(WriterBase.m_pHeaderInfo);

	if(!WriterBase.BeginWriting())
		return FALSE;
	
	fOK = Run_AutoMode(0, 0);
	
	WriterBase.EndWriting();
	WriterBase.Close();

	if(fCopyMarker) CopyMarker(bstrOutput);

	Close();

	return fOK;
}