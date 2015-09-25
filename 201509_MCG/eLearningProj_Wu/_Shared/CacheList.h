#ifndef _CACHELIST_H_
#define _CACHELIST_H_

// Each cache SAMPLE should be eithor MediaSample or NSSBuffer!!
interface INSSBuffer;
interface IMediaSample;
typedef struct _tagCacheSample {
	DWORD			dwID; // for easy debug !!
	long			lPriority;
	long			lType; // 0: Audio, 1: Video, 2: Script!!
// Only use for MediaSample
	IMediaSample	*pSample;
// Only use for NssBuffer
	INSSBuffer		*pBuffer;
	REFERENCE_TIME	rtTime;
	DWORD			dwFlags;
} CACHESAMPLE;

typedef struct {
	long			m_lAudioSampleCount, m_lAudioBufferCount;
	long			m_lVideoSampleCount, m_lVideoBufferCount;
	long			m_lScriptSampleCount, m_lScriptBufferCount;
} CACHEINFO;

class CCacheSampleList {
private:
	CCritSec		m_ListCrit;     // Serialize lists	
	long			m_lMaxCount;
	DWORD			m_dwCurrID;
public:
	CAMEvent		m_evList;
	LPBYTE			m_pSafeBuffer;
	CGenericList<CACHESAMPLE> m_ListFree;
    CGenericList<CACHESAMPLE> m_ListFilled;

	CACHEINFO		m_CacheInfo;
	REFERENCE_TIME	m_rtLastTime;
public:
	CCacheSampleList(long lCount, HRESULT *phr);
	~CCacheSampleList();	
public:
	void ResetTimer();
	REFERENCE_TIME GetTime() { return m_rtLastTime; };
	DWORD GetCount() { 
		CAutoLock	autolock(&m_ListCrit);
		return (m_CacheInfo.m_lAudioSampleCount + m_CacheInfo.m_lAudioBufferCount +
				m_CacheInfo.m_lVideoSampleCount + m_CacheInfo.m_lVideoBufferCount +
				m_CacheInfo.m_lScriptSampleCount + m_CacheInfo.m_lScriptBufferCount);
	};				
	BOOL GetCacheInfo(CACHEINFO *ci) { 
		CAutoLock	autolock(&m_ListCrit);
		if(ci==NULL) return FALSE; 
		memcpy(ci, &m_CacheInfo, sizeof(CACHEINFO));
		return TRUE; 
	};
	BOOL AddSample(long lPriority, long lType, IMediaSample *psample, DWORD *pdwID=NULL);
	BOOL AddBuffer(long lPriority, long lType, INSSBuffer *pnssbuffer, REFERENCE_TIME rtTime, DWORD dwFlags, DWORD *pdwID=NULL);
	void RemoveAll();
	BOOL RemoveHead(CACHESAMPLE *);
    HANDLE GetWaitHandle() {  return (HANDLE)m_evList; }
	BOOL WaitForSamples(DWORD dwMili=INFINITE);	
private:
	CACHESAMPLE *TryAddNode(long lPriority);
};

#endif