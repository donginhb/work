#ifndef _LOOPTHREAD_H_
#define _LOOPTHREAD_H_

class CLoopThread {
public:
	HANDLE			m_hThread;
	DWORD			m_dwThreadID;

	BOOL			m_fTryThreadExit;
	BOOL			m_fThreadBegin;
private:	
	CAMEvent		m_evThreadDone;
public:
	CLoopThread();
	~CLoopThread();
public:	
	virtual BOOL ThreadBegin();
	virtual BOOL ThreadTryEnd();
	BOOL WaitReallyEnd(); // after EndWrite ...

// overridable member functions!!
public:
// OnBeforeCreateThread is called in ThreadBegin, 
// we can check other condition here before really entering
	virtual BOOL OnBeforeCreateThread() { return TRUE; };

	virtual BOOL OnEnterThread() { return TRUE; };
	virtual void OnThreadLoop(BOOL *pfControlHandOut, BOOL *pfLoopLocked) { 
		if(pfControlHandOut!=NULL) *pfControlHandOut = TRUE; 
		if(pfLoopLocked!=NULL) *pfLoopLocked = FALSE; 
	};
	virtual BOOL OnExitThread() { return TRUE; };
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam);
};

#endif