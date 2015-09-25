#ifndef _WRITERBASE_H_
#define _WRITERBASE_H_

#include <windef.h>

class CWriterBase {
public:
	IWMWriter			*m_pWriter;
	IWMWriterAdvanced	*m_pWriterAdvanced;
	IWMHeaderInfo		*m_pHeaderInfo;

	BOOL	m_fVideoExisted, m_fScriptExisted;
	DWORD	m_dwInputAudio, m_dwInputVideo, m_dwInputScript;
	WORD	m_wStreamAudio, m_wStreamVideo, m_wStreamScript;
public:
	CWriterBase();
	~CWriterBase();

	BOOL Create(WCHAR *pwchFile, IWMProfile *pProfile);
	BOOL Create(WCHAR *pwchFile, BSTR bstrProfile);
	void Close();

	WM_MEDIA_TYPE *GetAudioInputFormat();
	WM_MEDIA_TYPE *GetVideoInputFormat();
	WM_MEDIA_TYPE *GetScriptInputFormat();
	void FreeFormatAllocated(WM_MEDIA_TYPE *);

	
	BOOL ConfigAudioInputCodecNoNeeded();
	BOOL ConfigVideoInputCodecNoNeeded();
	BOOL ConfigScriptInputCodecNoNeeded();

	BOOL ConfigAudioInputFormat(WM_MEDIA_TYPE *pwmt);
	BOOL ConfigVideoInputFormat(WM_MEDIA_TYPE *pwmt);	

	BOOL ConfigAudioInputSimpleUncompressed(WORD wBitsPerSample, WORD wChannels, DWORD dwSampleRate);
	BOOL ConfigVideoInputSimpleUncompressed(WORD wBitCount, DWORD biCompression);
	BOOL ConfigVideoInputBitmapFormat(BITMAPINFOHEADER *pbih);

	BOOL BeginWriting();
	BOOL EndWriting();
};
#endif