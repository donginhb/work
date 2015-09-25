#include <streams.h>

#include "LoopThread.h"

CLoopThread::CLoopThread() :
	m_hThread(NULL), m_dwThreadID(0), m_evThreadDone(TRUE), 
	m_fTryThreadExit(FALSE), m_fThreadBegin(FALSE)
{
	m_evThreadDone.Set(); // initially done!!
}

CLoopThread::~CLoopThread()
{
	WaitReallyEnd(); // must wait for done!!
}

BOOL CLoopThread::ThreadBegin()
{
	if(m_fThreadBegin) // has began
		return TRUE;

	m_fTryThreadExit = TRUE;
	m_evThreadDone.Wait(); // wait for last loop if existed!!

	if(!OnBeforeCreateThread())
		return FALSE;

	m_fTryThreadExit = FALSE;
	m_evThreadDone.Reset();
	m_hThread = CreateThread(NULL,						// Security attributes
                            (DWORD) 0,					// Initial stack size
                            CLoopThread::ThreadProc,  // Thread start address
                            (LPVOID) this,				// Thread parameter
                            (DWORD) 0,					// Creation flags
                            &m_dwThreadID);				// Thread identifier
	if(m_hThread==NULL) return FALSE;

	m_fThreadBegin = TRUE;

	return TRUE;
}

BOOL CLoopThread::ThreadTryEnd()
{
	if(!m_fThreadBegin)
		return TRUE;

	m_fTryThreadExit = TRUE;
	m_fThreadBegin = FALSE;

	return TRUE;
}

BOOL CLoopThread::WaitReallyEnd()
{
	m_evThreadDone.Wait();

	return TRUE;
}

////////////////////////////////////////////////////////////
//

DWORD WINAPI CLoopThread::ThreadProc(LPVOID pParam)
{	
	BOOL fControlHandOut=TRUE, fLoopLocked=FALSE;

	CLoopThread *pThread = (CLoopThread *)pParam;
	if(pThread==NULL) goto END_THREADPROC;

	if(!pThread->OnEnterThread())
		goto END_THREADPROC;
	
	do {		
		pThread->OnThreadLoop(&fControlHandOut, &fLoopLocked);
		if(fControlHandOut) {
			MSG	 msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	} while((fLoopLocked)||(!pThread->m_fTryThreadExit));

END_THREADPROC:

	pThread->OnExitThread();

	pThread->m_evThreadDone.Set();

	return 0;
}