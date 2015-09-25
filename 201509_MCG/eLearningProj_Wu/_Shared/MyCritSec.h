#ifndef _MYCRITSEC_H_
#define _MYCRITSEC_H_
// wrapper for whatever critical section we have
class CMyCritSec {
	
    // make copy constructor and assignment operator inaccessible
	
    CMyCritSec(const CMyCritSec &refCritSec);
    CMyCritSec &operator=(const CMyCritSec &refCritSec);
	
    CRITICAL_SECTION m_CritSec;
	
public:
    CMyCritSec() {
		InitializeCriticalSection(&m_CritSec);
    };
	
    ~CMyCritSec() {
		DeleteCriticalSection(&m_CritSec);
    };
	
    void Lock() {
		EnterCriticalSection(&m_CritSec);
    };
	
    void Unlock() {
		LeaveCriticalSection(&m_CritSec);
    };
};

// locks a critical section, and unlocks it automatically
// when the lock goes out of scope
class CMyAutoLock {
	
    // make copy constructor and assignment operator inaccessible
	
    CMyAutoLock(const CMyAutoLock &refAutoLock);
    CMyAutoLock &operator=(const CMyAutoLock &refAutoLock);
	
protected:
    CMyCritSec * m_pLock;
	
public:
    CMyAutoLock(CMyCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };
	
    ~CMyAutoLock() {
        m_pLock->Unlock();
    };
};
#endif