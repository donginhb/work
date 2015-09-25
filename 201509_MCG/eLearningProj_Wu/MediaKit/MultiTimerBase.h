#ifndef _MULTITIMERBASE_H_
#define _MULTITIMERBASE_H_

class CMultiTimerBase {
public:
	QWORD	m_qwTotalDuration;
public:
	CMultiTimerBase() { m_qwTotalDuration=0; }
	~CMultiTimerBase() { }
public:
	void SetupTotalDuration(QWORD qwDuration) { m_qwTotalDuration = qwDuration; }
	BOOL ReportMultiProgress(HWND hwnd, long lMsg, QWORD qwNow);
};

#endif