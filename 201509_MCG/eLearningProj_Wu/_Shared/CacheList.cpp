#include <streams.h>
#include <wmsdk.h>

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

#define THRESHOLD	((REFERENCE_TIME)3000*10000)

#include "CacheList.h"

CCacheSampleList::CCacheSampleList(long lCount, HRESULT *phr) :
	m_lMaxCount(lCount),
	m_ListFree(NAME("Free buffers"), 0),
    m_ListFilled(NAME("Pending, full, buffers"), 0),
	m_evList(TRUE), m_dwCurrID(0),
	m_rtLastTime(0)
{
	CAutoLock	autolock(&m_ListCrit);

	m_evList.Reset();

	if(m_lMaxCount<=0) {
		*phr = E_INVALIDARG;
		return ;
	}

	m_pSafeBuffer = (LPBYTE)malloc(m_lMaxCount*sizeof(CACHESAMPLE));
	if(m_pSafeBuffer==NULL) {
		*phr = E_OUTOFMEMORY;
		return ;
	}
	memset(m_pSafeBuffer, 0, m_lMaxCount*sizeof(CACHESAMPLE));

	for(long ii=0; ii<m_lMaxCount; ii++) {
		CACHESAMPLE *pCacheSample = (CACHESAMPLE *)(m_pSafeBuffer + ii*sizeof(CACHESAMPLE));
		ASSERT(pCacheSample!=NULL);

		POSITION pos = m_ListFree.AddHead(pCacheSample);
		if(pos==NULL) {
			*phr = E_OUTOFMEMORY;
			return ;
		}
	}
	memset(&m_CacheInfo, 0, sizeof(m_CacheInfo));
	*phr = S_OK;
}

CCacheSampleList::~CCacheSampleList()
{
	RemoveAll();
	m_ListFree.RemoveAll();

	if(m_pSafeBuffer!=NULL) {
		free(m_pSafeBuffer);
		m_pSafeBuffer = NULL;
	}
}

CACHESAMPLE *CCacheSampleList::TryAddNode(long lPriority)
{
	POSITION pos=NULL;
	CACHESAMPLE *pCacheSample = m_ListFree.RemoveHead();
	if(pCacheSample==NULL) {
		CACHESAMPLE *pTempCache=NULL;
		pos = m_ListFilled.GetTailPosition();	
		while(pos!=NULL) {			
			pTempCache=m_ListFilled.Get(pos);
			ASSERT(pTempCache!=NULL);
			if(pTempCache->lPriority<lPriority) {
				DbgLog((LOG_TRACE, 1, "[Swap] a sample... CacheCount(%d)", m_ListFilled.GetCount()));
				pTempCache = m_ListFilled.Remove(pos);
				if(pTempCache->pSample!=NULL) {
					switch(pTempCache->lType) {
					case 0: m_CacheInfo.m_lAudioSampleCount--; break;
					case 1: m_CacheInfo.m_lVideoSampleCount--; break;
					case 2: m_CacheInfo.m_lScriptSampleCount--; break;
					}					
				}
				else if(pTempCache->pBuffer!=NULL) {
					switch(pTempCache->lType) {
					case 0: m_CacheInfo.m_lAudioBufferCount--; break;
					case 1: m_CacheInfo.m_lVideoBufferCount--; break;
					case 2: m_CacheInfo.m_lScriptBufferCount--; break;
					}
				}
				RELEASE(pTempCache->pSample);
				RELEASE(pTempCache->pBuffer);

				pCacheSample = pTempCache;
				break;
			}
			else pos = m_ListFilled.Prev(pos);
		}
	}

	if(pCacheSample==NULL) {
		DbgLog((LOG_TRACE, 1, "[Skip] a sample... CacheCount(%d)", m_ListFilled.GetCount()));
		return NULL;
	}
	else {
		pos = m_ListFilled.AddTail(pCacheSample);
		ASSERT(pos!=NULL);
	}	

	return pCacheSample;
}

BOOL CCacheSampleList::WaitForSamples(DWORD dwMili)
{
	return m_evList.Wait(dwMili);
}

//////////////////////////////////////////////////////////////////////////////
// export and sharelock!!
void CCacheSampleList::ResetTimer()
{
	CAutoLock	autolock(&m_ListCrit);

	m_rtLastTime = 0;
}

BOOL CCacheSampleList::AddSample(long lPriority, long lType, IMediaSample *pSample, DWORD *pdwID)
{
	CAutoLock	autolock(&m_ListCrit);

	m_dwCurrID++;
	if(pdwID!=NULL) *pdwID = m_dwCurrID;

	if(pSample==NULL) return FALSE;	

	REFERENCE_TIME rtStart=0, rtEnd=0;
	// no time stamp can be used, then increasely!!
	if(FAILED(pSample->GetTime(&rtStart, &rtEnd))) 
		m_rtLastTime += (REFERENCE_TIME)10;
	else if(rtStart>m_rtLastTime) m_rtLastTime = rtStart;
	else if(rtStart<m_rtLastTime-THRESHOLD) {
		DbgLog((LOG_TRACE, 1, "[Skip] add sample... late CacheCount(%d)", m_ListFilled.GetCount()));
		return FALSE;
	}

	CACHESAMPLE *pCacheSample = TryAddNode(lPriority);
	if(pCacheSample==NULL)
		return FALSE; // full and skip!!

	memset(pCacheSample, 0, sizeof(CACHESAMPLE));
	pCacheSample->dwID = m_dwCurrID;
	pCacheSample->lPriority = lPriority;
	pCacheSample->lType = lType;
	pCacheSample->pSample = pSample;
	pCacheSample->pSample->AddRef();

	switch(lType) {
	case 0: m_CacheInfo.m_lAudioSampleCount++; break;
	case 1: m_CacheInfo.m_lVideoSampleCount++; break;
	case 2: m_CacheInfo.m_lScriptSampleCount++; break;
	}
	
	if(m_ListFilled.GetCount()==1) // Each time far from empty ...
		m_evList.Set();

	return TRUE;
}

BOOL CCacheSampleList::AddBuffer(long lPriority, long lType, INSSBuffer *pBuffer, REFERENCE_TIME rtTime, DWORD dwFlags, DWORD *pdwID)
{
	CAutoLock	autolock(&m_ListCrit);

	m_dwCurrID++;
	if(pdwID!=NULL) *pdwID = m_dwCurrID;

	if(pBuffer==NULL) return FALSE;

	// no time info can be used, then increasely!!
	if(rtTime==0) m_rtLastTime += 10;
	else if(rtTime>m_rtLastTime) m_rtLastTime = rtTime;
	else if(rtTime<m_rtLastTime-THRESHOLD) {
		DbgLog((LOG_TRACE, 1, "[Skip] add buffer... late CacheCount(%d)", m_ListFilled.GetCount()));
		return FALSE; // skip!!
	}

	CACHESAMPLE *pCacheSample = TryAddNode(lPriority);
	if(pCacheSample==NULL)
		return FALSE;

	memset(pCacheSample, 0, sizeof(CACHESAMPLE));
	pCacheSample->dwID = m_dwCurrID;
	pCacheSample->lPriority = lPriority;
	pCacheSample->lType = lType;
	pCacheSample->rtTime = rtTime;
	pCacheSample->dwFlags = dwFlags;
	pCacheSample->pBuffer = pBuffer;
	pCacheSample->pBuffer->AddRef();

	switch(lType) {
	case 0: m_CacheInfo.m_lAudioBufferCount++; break;
	case 1: m_CacheInfo.m_lVideoBufferCount++; break;
	case 2: m_CacheInfo.m_lScriptBufferCount++; break;
	}
	
	if(m_ListFilled.GetCount()==1) // Each time far from empty ...
		m_evList.Set();

	return TRUE;
}

void CCacheSampleList::RemoveAll()
{
	CAutoLock	autolock(&m_ListCrit);

	CACHESAMPLE *pCacheSample=NULL;
	while((pCacheSample=m_ListFilled.RemoveHead())&&(pCacheSample!=NULL)) {
		RELEASE(pCacheSample->pSample);
		RELEASE(pCacheSample->pBuffer);
		m_ListFree.AddHead(pCacheSample);
	}
	memset(&m_CacheInfo, 0, sizeof(m_CacheInfo));

	m_evList.Reset();
}

BOOL CCacheSampleList::RemoveHead(CACHESAMPLE *pcp)
{
	CAutoLock	autolock(&m_ListCrit);

	CACHESAMPLE *pCacheSample=m_ListFilled.RemoveHead();
	if(pCacheSample==NULL)
		return FALSE;

	memcpy(pcp, pCacheSample, sizeof(CACHESAMPLE));

	m_ListFree.AddHead(pCacheSample);	

	switch(pcp->lType) {
	case 0: 
		if(pcp->pSample!=NULL) {
			m_CacheInfo.m_lAudioSampleCount--;
			ASSERT(m_CacheInfo.m_lAudioSampleCount>=0);
		}
		else if(pcp->pBuffer!=NULL) {
			m_CacheInfo.m_lAudioBufferCount--; 
			ASSERT(m_CacheInfo.m_lAudioBufferCount>=0); 
		}
		break;
	case 1: 
		if(pcp->pSample!=NULL) {
			m_CacheInfo.m_lVideoSampleCount--;
			ASSERT(m_CacheInfo.m_lVideoSampleCount>=0);
		}
		else if(pcp->pBuffer!=NULL) {
			m_CacheInfo.m_lVideoBufferCount--; 
			ASSERT(m_CacheInfo.m_lVideoBufferCount>=0); 
		}
		break;
	case 2: 
		if(pcp->pSample!=NULL) {
			m_CacheInfo.m_lScriptSampleCount--;
			ASSERT(m_CacheInfo.m_lScriptSampleCount>=0);
		}
		else if(pcp->pBuffer!=NULL) {
			m_CacheInfo.m_lScriptBufferCount--; 
			ASSERT(m_CacheInfo.m_lScriptBufferCount>=0); 
		}
	}

	if(0==m_ListFilled.GetCount())
		m_evList.Reset();

	return TRUE;
}
