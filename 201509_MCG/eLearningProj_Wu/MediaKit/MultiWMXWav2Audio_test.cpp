#include <afxwin.h>
#include "MultiWMXWav2Audio.h"
#include "l_bitmap.h"

#include "..\VFXManager\VFXMan.h"
#include <uuids.h>

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

CMultiWMXWav2Audio::CMultiWMXWav2Audio()
{
	m_qwSegmentTime = 0;
	m_pWriterBase = NULL;
	m_pWavReader = NULL;
	m_pManager = NULL;
	m_qwAudioDuration = 0;

	EnableAutoProgressReport(FALSE);
/*
	m_hMemDC = NULL;
	m_hbmp = NULL;
	m_holdbmp = NULL;
	m_pdibbuffer = NULL;
*/
}

CMultiWMXWav2Audio::~CMultiWMXWav2Audio()
{
}

BOOL CMultiWMXWav2Audio::AppendSpaceVideo(QWORD qwTime)
{
	INSSBuffer *pNSSBuffer=NULL;
	LPBYTE pBuffer=NULL;
	WM_MEDIA_TYPE *pmt = m_pWriterBase->GetVideoInputFormat();
	if(pmt==NULL) return FALSE;
	WMVIDEOINFOHEADER *pvih = (WMVIDEOINFOHEADER *)pmt->pbFormat;
	DWORD dwSizeImage = pvih->bmiHeader.biSizeImage;
	m_pWriterBase->FreeFormatAllocated(pmt);

	HRESULT hr = m_pWriterBase->m_pWriter->AllocateSample(dwSizeImage, &pNSSBuffer);
	if(FAILED(hr)) return FALSE;

	pNSSBuffer->GetBuffer(&pBuffer);
	memset(pBuffer, 0, dwSizeImage);

	hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputVideo, qwTime, WM_SF_CLEANPOINT|WM_SF_DISCONTINUITY, pNSSBuffer);
	RELEASE(pNSSBuffer);
	if(FAILED(hr)) return FALSE;

	return TRUE;
}

BOOL CMultiWMXWav2Audio::AppendWavReader(BOOL fSpaceVideo, UINT &uPosWave, UINT uSize, WAVREADER *pWavReader)
{
	if(fSpaceVideo) 
		AppendSpaceVideo(m_qwSegmentTime);

	HRESULT hr= 0;
	LPBYTE pBuffer=NULL;
	INSSBuffer *pNSSBuffer=NULL;
	UINT	uStop = uPosWave + uSize;
	UINT	uRetSize=0, uCopySize=0;
	QWORD	qwTime=m_qwSegmentTime;	
	while(uPosWave<uStop) {
		uCopySize = uStop - uPosWave;
		if(uCopySize>pWavReader->pwfx->nAvgBytesPerSec) uCopySize = pWavReader->pwfx->nAvgBytesPerSec;
		uCopySize = uCopySize / pWavReader->pwfx->nBlockAlign * pWavReader->pwfx->nBlockAlign;

		if(uCopySize<pWavReader->pwfx->nBlockAlign) break;

		hr = m_pWriterBase->m_pWriter->AllocateSample(uCopySize, &pNSSBuffer);
		if(FAILED(hr)) return FALSE;
		
		pNSSBuffer->GetBuffer(&pBuffer);

		if(!WavReaderReadDirectData(pWavReader, uPosWave, uCopySize, pBuffer, &uRetSize)) {
			RELEASE(pNSSBuffer);
			return FALSE;
		}		

		hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputAudio, qwTime, 0, pNSSBuffer);
		RELEASE(pNSSBuffer);
		if(FAILED(hr)) return FALSE;		

		uPosWave += uCopySize;
		qwTime += (QWORD)(uCopySize * 1000.0 / pWavReader->pwfx->nAvgBytesPerSec) * 10000L;
		
		ReportMultiProgress(m_hWndProgress, m_lMsgProgress, qwTime);
	}

	m_qwSegmentTime = qwTime;	

	return TRUE;
}

BOOL CMultiWMXWav2Audio::AddWMXFile(BSTR bstrWMX, DWORD dwmsStart, DWORD dwmsStop, WAVREADER *pWavReader, UINT &uPosWave)
{
	if(m_pWriterBase==NULL)
		return FALSE;

	m_pWavReader = pWavReader;

	if(!Open(bstrWMX)) return FALSE;

	SetUserClock(TRUE);
	SetSelectedStreams(TRUE, TRUE, FALSE);

	ConfigAudioOutputSimpleUncompressed(pWavReader->pwfx->wBitsPerSample, pWavReader->pwfx->nChannels, pWavReader->pwfx->nSamplesPerSec);	
	ConfigVideoOutputSimpleUncompressed(16, 0);	
	
	if(m_pWriterBase->m_fVideoExisted) {
		WM_MEDIA_TYPE *pmt = m_pWriterBase->GetVideoInputFormat();
		if(pmt==NULL) return FALSE;
		GUID guid = pmt->subtype;
		WMVIDEOINFOHEADER *pvih = (WMVIDEOINFOHEADER *)pmt->pbFormat;
		memcpy(&m_bihWriter, &pvih->bmiHeader, sizeof(BITMAPINFOHEADER));	
		m_pWriterBase->FreeFormatAllocated(pmt);

		ENUMVIDEOSUBTYPE eVideoSubtype=VIDEOSUBTYPE_24;
		if(guid==WMMEDIASUBTYPE_RGB565) eVideoSubtype = VIDEOSUBTYPE_565;
		else if(guid==WMMEDIASUBTYPE_RGB555) eVideoSubtype = VIDEOSUBTYPE_555;
		else if(guid==WMMEDIASUBTYPE_RGB24) eVideoSubtype = VIDEOSUBTYPE_24;
		else if(guid==WMMEDIASUBTYPE_RGB32) eVideoSubtype = VIDEOSUBTYPE_32;
		if(m_pManager!=NULL) m_pManager->SetupInfo(m_bihWriter.biWidth, m_bihWriter.biHeight, eVideoSubtype, NULL);	

		pmt = GetVideoOutputFormat();
		if(pmt==NULL) return FALSE;
		pvih = (WMVIDEOINFOHEADER *)pmt->pbFormat;
		memcpy(&m_bihReader, &pvih->bmiHeader, sizeof(BITMAPINFOHEADER));
		FreeFormatAllocated(pmt);
	}

/*
	if(0!=memcmp(&m_bihWriter, &m_bihReader, sizeof(BITMAPINFOHEADER))) {
		HDC hdc = GetDC(NULL);
		if(hdc!=NULL) {
			m_hMemDC = CreateCompatibleDC(hdc);
			if(m_hMemDC!=NULL) {
				m_hbmp = CreateDIBSection(m_hMemDC, (BITMAPINFO*)&m_bihWriter, DIB_RGB_COLORS, (void**)&m_pdibbuffer, NULL, 0);
				if(m_hbmp!=NULL)
					m_holdbmp = (HBITMAP)SelectObject(m_hMemDC, m_hbmp);
			}
			ReleaseDC(NULL, hdc);
		}
	}
*/
	BOOL fError = FALSE;
	m_uPosWave = uPosWave;
	m_qwStartTime = (LONGLONG)dwmsStart * 10000L;

	if(dwmsStop>=(DWORD)(m_qwLength/10000))
		dwmsStop = (DWORD)(m_qwLength/10000);

	m_qwStopTime = (LONGLONG)dwmsStop * 10000L;

	if(Run_ManualMode(m_qwStartTime)) {
		WaitForStop(m_hTimeAsync, INFINITE);
		DWORD dwmsTime = dwmsStart;
		DWORD dwmsDeliverSize=0;
		while(dwmsTime<dwmsStop) {
			dwmsDeliverSize = dwmsStop - dwmsTime;
			if(dwmsDeliverSize>1000) dwmsDeliverSize = 1000; // one second!!

			dwmsTime += dwmsDeliverSize;

			if(!ManualDeliverTo((QWORD)dwmsTime * 10000L)) { // also change m_qwSegmentTime!!
				fError = TRUE;
				break;
			}

			ReportMultiProgress(m_hWndProgress, m_lMsgProgress, m_qwSegmentTime + (QWORD)(dwmsTime-dwmsStart)*10000);

			WaitForStop(m_hTimeAsync, INFINITE);
		}
	}

	m_pReader->Stop();

	Close();

	m_qwSegmentTime += (QWORD)(dwmsStop - dwmsStart) * 10000L;
	uPosWave = m_uPosWave;

/*
	if(m_hMemDC!=NULL) {
		SelectObject(m_hMemDC, m_holdbmp);
		DeleteDC(m_hMemDC);		
	}
	if(m_hbmp!=NULL) DeleteObject(m_hbmp);
*/
	return (!fError);
}

///////////////////////////////////////////////////////////////////////
//
HRESULT CMultiWMXWav2Audio::OnSample(DWORD dwOutputNum, QWORD qwSampleTime, QWORD qwSampleDuration,
							 DWORD dwFlags, INSSBuffer *pSample, void *pvContext) 
{
	if(qwSampleTime<m_qwStartTime) return S_OK;
	if((dwOutputNum!=m_dwOutputAudio)&&(dwOutputNum!=m_dwOutputVideo)) return S_OK;

// to add this period seems to be strange but indeed, it makes better results.
	if(qwSampleTime<m_qwStartTime)
		return S_OK;

	if(qwSampleTime>=m_qwStopTime)
		return S_OK;

	if((qwSampleTime+qwSampleDuration)>m_qwStopTime) 
		return S_OK;
	
	HRESULT hr = 0;

	DWORD dwLength=0;
	LPBYTE pBuffer=NULL;
	hr = pSample->GetBufferAndLength(&pBuffer, &dwLength);
	if(FAILED(hr)) return hr;

	if(dwOutputNum==m_dwOutputAudio) {
		m_qwAudioDuration = qwSampleDuration;
		
		DWORD dwNewLength=0;
		dwNewLength = dwLength / m_pWavReader->pwfx->nBlockAlign * m_pWavReader->pwfx->nBlockAlign;
		ASSERT(dwNewLength==dwLength);
		
		UINT uRetSize=0;
		if(!WavReaderReadDirectData(m_pWavReader, m_uPosWave, dwNewLength, pBuffer, &uRetSize))
			return S_OK;
		QWORD qwWaveTime = (QWORD)1000 * 10000 * m_uPosWave / m_pWavReader->pwfx->nAvgBytesPerSec;
		QWORD qwPlayTime = m_qwSegmentTime + qwSampleTime - m_qwStartTime;
		ASSERT((qwWaveTime<qwPlayTime+5000000)&&(qwPlayTime<qwWaveTime+5000000));

		// maybe, use qwPlayTime to re-compute m_uPosWave and dwNewLength can decrease the shifting
		// but in this test, no 0.5 sec difference, and can be acceptence
		// except audio wave has shifted after building out wave (current use D-Show in audio.)

		hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputAudio, qwPlayTime, dwFlags, pSample);
		if(FAILED(hr)) return hr;

		m_uPosWave += dwNewLength;
	}
	else {
		BITMAPHANDLE bmh;

		long lRet = L_CreateBitmap(&bmh, TYPE_USER, m_bihReader.biWidth, m_bihReader.biHeight, 
		m_bihReader.biBitCount, ORDER_BGR, NULL, BOTTOM_LEFT, pBuffer, m_bihReader.biSizeImage);

		if(lRet==SUCCESS) {
			lRet = L_SizeBitmap(&bmh, m_bihWriter.biWidth, m_bihWriter.biHeight, SIZE_NORMAL);
			if(lRet==SUCCESS) {
				INSSBuffer *pNSSBuffer=NULL;
				hr = m_pWriterBase->m_pWriter->AllocateSample(m_bihWriter.biSizeImage, &pNSSBuffer);
				if(SUCCEEDED(hr)) {
					LPBYTE pNewBuffer=NULL;
					pNSSBuffer->GetBuffer(&pNewBuffer);
					DWORD dwWidthBytes = WIDTHBYTES(m_bihWriter.biWidth*m_bihWriter.biBitCount);
					DWORD dwOffset=0;
					long ii=0;
					while((ii<m_bihWriter.biHeight)&&(dwOffset<m_bihWriter.biSizeImage)) {
						L_GetBitmapRow(&bmh, pNewBuffer + dwOffset, ii, dwWidthBytes);
						ii++;
						dwOffset += dwWidthBytes;
					}
					QWORD qwPlayTime = m_qwSegmentTime + qwSampleTime - m_qwStartTime;
					if(m_pManager!=NULL) m_pManager->Run((long)(qwPlayTime/10000L), pNewBuffer, m_bihWriter.biSizeImage); 
					hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputVideo, qwPlayTime, dwFlags, pNSSBuffer);
					if(FAILED(hr)) return hr;

					RELEASE(pNSSBuffer);
				}
			}
			L_FreeBitmap(&bmh);
		}

/*
		if(m_hMemDC==NULL) { // no need convert!!
			hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputVideo, m_qwSegmentTime + qwSampleTime - m_qwStartTime, dwFlags, pSample);
			if(FAILED(hr)) return hr;
		}
		else {
			INSSBuffer *pNSSBuffer=NULL;
			hr = m_pWriterBase->m_pWriter->AllocateSample(m_bihWriter.biSizeImage, &pNSSBuffer);
			if(SUCCEEDED(hr)) {
				StretchDIBits(m_hMemDC, 0, 0, m_bihWriter.biWidth, m_bihWriter.biHeight,
					0, 0, m_bihReader.biWidth, m_bihReader.biHeight, pBuffer, (BITMAPINFO*)&m_bihReader, DIB_RGB_COLORS, SRCCOPY);
				LPBYTE pNewBuffer=NULL;
				pNSSBuffer->GetBuffer(&pNewBuffer);
				memcpy(pNewBuffer, m_pdibbuffer, m_bihWriter.biSizeImage);
				hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputVideo, m_qwSegmentTime + qwSampleTime - m_qwStartTime, dwFlags, pNSSBuffer);
				if(FAILED(hr)) return hr;
				RELEASE(pNSSBuffer);
			}
		}
*/
//		hr = m_pWriterBase->m_pWriter->WriteSample(m_pWriterBase->m_dwInputVideo, m_qwSegmentTime + qwSampleTime - m_qwStartTime, dwFlags, pSample);
//		if(FAILED(hr)) return hr;
	}	
	return S_OK;
}
	
HRESULT CMultiWMXWav2Audio::OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration,
							DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	return S_OK;
}
