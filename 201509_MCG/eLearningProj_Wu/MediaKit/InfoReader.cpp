#include <afxwin.h>
#include <wmsdkidl.h>
#include <l_bitmap.h>

#include "InfoReader.h"

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

CInfoReader::CInfoReader()
{
	m_fNextImageFound = FALSE;
	m_fPrevImageFound = FALSE;
	m_pNextImageFile = NULL;
	m_pPrevImageFile = NULL;
	m_qwAudioDuration = 0;	
	m_lImageFormat = 0;
	m_lImageWidth = 0;
	m_lImageHeight = 0;
	m_fFindImageNext = FALSE;
	m_pNextBuffer = NULL;
	m_pPrevBuffer = NULL;
	m_cbBuffer = 0;

	m_lAudioSampleCount = 0;
	m_lVideoSampleCount = 0;
	m_lScriptSampleCount = 0;
}

CInfoReader::~CInfoReader()
{
	Close();
}

BOOL SaveReaderImage(LPSTR szImage, long lLTFormat, DWORD dwWidth, DWORD dwHeight, WORD wBitCount, LPBYTE pBuffer, DWORD dwLength)
{
	BOOL fUseQF=(lLTFormat==FILE_CMP)||(lLTFormat==FILE_JFIF)||(lLTFormat==FILE_LEAD1JFIF)||
		(lLTFormat==FILE_LEAD2JFIF)||(lLTFormat==FILE_JTIF)||(lLTFormat==FILE_LEAD1JTIF)||
		(lLTFormat==FILE_LEAD2JTIF)||(lLTFormat==FILE_FPX_JPEG_QFACTOR)||(lLTFormat==FILE_EXIF_JPEG);

	BITMAPHANDLE bmh;
	if(1>L_CreateBitmap(&bmh, TYPE_USER, dwWidth, dwHeight, wBitCount, ORDER_BGR, NULL, BOTTOM_LEFT, pBuffer, dwLength))
		return FALSE;

	long lret = L_SaveFile(szImage, &bmh, lLTFormat, 24, fUseQF?2:0, 0, NULL, NULL, NULL);
	L_FreeBitmap(&bmh);
	if(1>lret) return FALSE;

	return TRUE;
}

BOOL CInfoReader::GetVideoImageSize(long &lWidth, long &lHeight)
{
	lWidth = 0; lHeight = 0;
	WM_MEDIA_TYPE *pwmt = GetVideoOutputFormat();
	if(pwmt==NULL) return FALSE;
	WMVIDEOINFOHEADER *pvih=(WMVIDEOINFOHEADER *)pwmt->pbFormat;
	if(pvih!=NULL) {
		lWidth = pvih->bmiHeader.biWidth;
		lHeight = pvih->bmiHeader.biHeight;
	}
	FreeFormatAllocated(pwmt);

	if((lWidth==0)||(lHeight==0))
		return FALSE;

	return TRUE;
}

BOOL CInfoReader::ImageGetting_Init(long lFormat)
{
	if(!m_fVideoExisted)
		return FALSE;

	if(!SetUserClock(TRUE))
		return FALSE;

	if(!SetSelectedStreams(FALSE, TRUE, FALSE))
		return FALSE;

	if(!GetVideoImageSize(m_lImageWidth, m_lImageHeight))
		return FALSE;

	if(!ConfigVideoOutputSimpleUncompressed(24, 0))
		return FALSE;

	m_cbBuffer = m_lImageWidth * m_lImageHeight * 3;
	m_pNextBuffer = (LPBYTE)malloc(m_cbBuffer);
	if(m_pNextBuffer==NULL) return FALSE;
	memset(m_pNextBuffer, 0, m_cbBuffer);
	m_pPrevBuffer = (LPBYTE)malloc(m_cbBuffer);
	if(m_pPrevBuffer==NULL) { free(m_pNextBuffer); return FALSE; };
	memset(m_pPrevBuffer, 0, m_cbBuffer);

	m_lImageFormat = lFormat;

	return TRUE;
}

BOOL CInfoReader::ImageGetting_NextImage(long lmsTime, LPSTR szImage)
{
	m_fFindImageNext = TRUE;
	m_fNextImageFound = FALSE;
	m_pNextImageFile = szImage;

	QWORD qwStartTime=0, qwStopTime=0, qwDeliver=0;

	qwStartTime = (QWORD)lmsTime * 10000;
	qwStopTime = qwStartTime + TIMEUNIT * 5;
	if(qwStopTime>m_qwLength) qwStopTime = m_qwLength;

	if(Run_ManualMode(qwStartTime)) {
		WaitForStop(m_hTimeAsync, INFINITE);
		
		while(qwStartTime<qwStopTime) {
			qwDeliver = qwStopTime-qwStartTime;
			if(qwDeliver>10000000) qwDeliver = 10000000; // CCH 0017
			if(!ManualDeliverTo(qwStartTime+qwDeliver)) 
				break;			
			WaitForStop(m_hTimeAsync, INFINITE);
		
			if(m_fNextImageFound) 
				break;

			qwStartTime += qwDeliver;
		}
	}

	m_pReader->Stop();
	return TRUE;
}

BOOL CInfoReader::ImageGetting_PrevImage(long lmsTime, LPSTR szImage)
{
	m_fFindImageNext = FALSE;
	m_fPrevImageFound = FALSE;
	m_pPrevImageFile = szImage;

	QWORD qwWorkTime=0, qwStartTime=0, qwStopTime=0, qwDeliver=0;
	
	qwStartTime = (QWORD)lmsTime*10000;

	if(qwStartTime<=TIMEUNIT * 5) qwStopTime = 0;
	else qwStopTime = qwStartTime - TIMEUNIT * 5;

	if(qwStartTime<(10000000+10000)) qwWorkTime = 0;
	else qwWorkTime = qwStartTime - (10000000+10000);

	while(qwWorkTime>=qwStopTime) { // should be >= !!!
		if(!Run_ManualMode(qwWorkTime)) 
			break;
		WaitForStop(m_hTimeAsync, INFINITE);

		qwDeliver = qwWorkTime - qwStartTime;
		if(qwDeliver>10000000) qwDeliver = 10000000;
		if(!ManualDeliverTo(qwWorkTime+qwDeliver)) 
			break;			
		WaitForStop(m_hTimeAsync, INFINITE);
		
		if(m_fPrevImageFound) 
			break;

		qwWorkTime -= qwDeliver;
	}

	m_pReader->Stop();

	return TRUE;
}

void CInfoReader::ImageGetting_Exit()
{
	if(m_pNextBuffer!=NULL) free(m_pNextBuffer);
	m_pNextBuffer = NULL;
	if(m_pPrevBuffer!=NULL) free(m_pPrevBuffer);
	m_pPrevBuffer = NULL;
}

BOOL CInfoReader::GetNextImage(long lFormat, long lmsTime, LPSTR szImage)
{
	if(!ImageGetting_Init(lFormat))
		return FALSE;

	BOOL fOK = ImageGetting_NextImage(lmsTime, szImage);
	if(fOK) SaveReaderImage(m_pNextImageFile, m_lImageFormat, m_lImageWidth, m_lImageHeight, 24, m_pNextBuffer, m_cbBuffer);

	ImageGetting_Exit();
	return fOK;
}

BOOL CInfoReader::GetPrevImage(long lFormat, long lmsTime, LPSTR szImage)
{
	if(!ImageGetting_Init(lFormat))
		return FALSE;

	BOOL fOK = ImageGetting_PrevImage(lmsTime, szImage);

	if(fOK) SaveReaderImage(m_pPrevImageFile, m_lImageFormat, m_lImageWidth, m_lImageHeight, 24, m_pPrevBuffer, m_cbBuffer);

	ImageGetting_Exit();
	return fOK;
}

BOOL CInfoReader::GetCoveredImages(long lFormat, long lmsTime, LPSTR szImagePrev, LPSTR szImageNext)
{
	if(!ImageGetting_Init(lFormat))
		return FALSE;
	
	BOOL fOK = ImageGetting_NextImage(lmsTime, szImageNext);
	
	if(fOK) {
		WaitForStop(m_hTimeAsync, 0); // just skip event!!

		SaveReaderImage(m_pNextImageFile, m_lImageFormat, m_lImageWidth, m_lImageHeight, 24, m_pNextBuffer, m_cbBuffer);

		fOK = ImageGetting_PrevImage(lmsTime, szImagePrev);

		SaveReaderImage(m_pPrevImageFile, m_lImageFormat, m_lImageWidth, m_lImageHeight, 24, m_pPrevBuffer, m_cbBuffer);
	}

	ImageGetting_Exit();
	return fOK;
}

BOOL CInfoReader::GetAudioAlignTime(QWORD *pqwAlign)
{
	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, FALSE, FALSE);

	ConfigAudioOutputCompressed();

	if(Run_ManualMode(0)) {
		WaitForStop(m_hTimeAsync, INFINITE);

		QWORD qwDeliver=0, qwTime = 0, qwStop = (QWORD)TIMEUNIT * 5;
		if(qwStop>m_qwLength) qwStop = m_qwLength;
		while(qwTime<qwStop) {
			qwDeliver = qwStop-qwTime;
			if(qwDeliver>TIMEUNIT) qwDeliver = TIMEUNIT;
			if(!ManualDeliverTo(qwTime+qwDeliver)) 
				break;			
			WaitForStop(m_hTimeAsync, INFINITE);

			qwTime += qwDeliver;
			if(m_qwAudioDuration!=0) 
				break;
		}
	}

	m_pReader->Stop();
	
	*pqwAlign = m_qwAudioDuration;

	return (m_qwAudioDuration!=0);
}

BOOL CInfoReader::GetDuration(QWORD *pqwDuration)
{
	if(m_pReader==NULL)
		return FALSE;

	IWMHeaderInfo*   pHeaderInfo=NULL;
	HRESULT hr = m_pReader->QueryInterface(IID_IWMHeaderInfo, (VOID **)&pHeaderInfo);
	if(FAILED(hr)) return FALSE;

    WORD wStreamNum = 0 ;
	WMT_ATTR_DATATYPE type ;
	QWORD qwDuration=0;
	WORD cbLength = sizeof(qwDuration) ;
	hr = pHeaderInfo->GetAttributeByName(&wStreamNum,  g_wszWMDuration, &type, (BYTE *)&qwDuration, &cbLength ) ;
	RELEASE(pHeaderInfo);
	if(FAILED(hr)) return FALSE;

	*pqwDuration = qwDuration;

	return TRUE;
}

BOOL CInfoReader::GetSampleCount(long *plAudio, long *plVideo, long *plScript)
{
	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, TRUE);

	ConfigAudioOutputCompressed();
	ConfigVideoOutputCompressed();
	ConfigScriptOutputCompressed();

	m_lAudioSampleCount = 0;
	m_lVideoSampleCount = 0;
	m_lScriptSampleCount = 0;

	BOOL fOK = Run_AutoMode(0, 0);

	if(plAudio!=NULL) *plAudio = m_lAudioSampleCount;
	if(plVideo!=NULL) *plVideo = m_lVideoSampleCount;
	if(plScript!=NULL) *plScript = m_lScriptSampleCount;

	Close();

	return fOK;
}
/////////////////////////////////////////////////////////////////////////////////////
//
HRESULT CInfoReader::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(dwOutputNum!=m_dwOutputVideo)
		return S_OK;

	if(m_fFindImageNext&&m_fNextImageFound) 
		return S_OK;

// pass2 get prev image
//	if((!m_fFindImageNext)&&m_fPrevImageFound)
//		return S_OK;

	DWORD	dwLength=0;
	BYTE	*pBuffer=NULL;
	if(S_OK==pSample->GetBufferAndLength(&pBuffer, &dwLength)) {
		if(m_fFindImageNext) {
			memcpy(m_pNextBuffer, pBuffer, __min(dwLength, m_cbBuffer));
			m_fNextImageFound = TRUE;
		}
		else {
			memcpy(m_pPrevBuffer, pBuffer, __min(dwLength, m_cbBuffer));
			m_fPrevImageFound = TRUE;
		}
	}

	return S_OK;
}
	
HRESULT CInfoReader::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	if(wStreamNum==m_wStreamAudio) {
		m_qwAudioDuration = cnsSampleDuration;
		m_lAudioSampleCount ++;
	}
	else if(wStreamNum==m_wStreamVideo) {
		m_lVideoSampleCount ++;
	}
	else if(wStreamNum==m_wStreamScript) {
		m_lScriptSampleCount ++;
	}
	return S_OK;
}
