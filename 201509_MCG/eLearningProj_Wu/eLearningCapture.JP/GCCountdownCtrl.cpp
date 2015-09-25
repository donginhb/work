#include "stdafx.h"
#include "GCCountdownCtrl.h"


#define SECONDS_PER_DAY	86400
#define SECONDS_PER_HOUR 3600

CGCCountdownCtrl::CGCCountdownCtrl() : CGCColorStatic()
{
	m_bTimerFlag = FALSE;
	m_bStopAtZero = FALSE;
	m_bStopFlag = FALSE;

	m_bZeroPointReached = FALSE;
	m_bZeroPointPassed = FALSE;
	m_iTotalSeconds = 0;

	m_uiStyle = 0x1111;
	m_Days = 0;
	m_Hours = 0;
	m_Minutes = 0;
	m_diff = 0;
	m_Seconds = 0;
}


CGCCountdownCtrl::~CGCCountdownCtrl()
{
}


BEGIN_MESSAGE_MAP(CGCCountdownCtrl, CGCColorStatic)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CGCCountdownCtrl::OnTimer(UINT nIDEvent) 
{
	// The CGCCountdownCtrl class is derived from the CGCColorStatic class.
	// Since the CGCColorStatic class provides the capability to flash the background
	// or text using timer events, we must distinguish between CGCColorStatic timer events
	// and the CGCCountdownCtrl's clock update timer event. Verify that the current timer
	// event is the CGCCountdownCtrl class' update event.  If it isn't route it on to the
	// parent class' OnTimer() event handler because that may be one of the 
	// CGCColorStatic class' flashing timer events.
	
	if (nIDEvent != CNTDWN_UPDATE_TIMER_ID) CGCColorStatic::OnTimer(nIDEvent);

	// If we are here then the current timer event is the clock's update event.

	// A race condition exists in which the CGCCountdownCtrl can reach the 0 stop point
	// but be unable to kill the timer process before the timer causes the countdown
	// to continue past the Zero Point.  A boolean flag is used to signal when the timer
	// is to stop so that it will not go too far.
	
	if (m_bStopFlag)
	{
		// Kill the timer.
		if (m_bTimerFlag) KillTimer(CNTDWN_UPDATE_TIMER_ID);

		m_bStopFlag = FALSE;	// Reset Stop Flag.
		m_bTimerFlag = FALSE;	// Resent Timer Flag.
		return;
	}

	Decrement();	// decrement/increment m_iTotalSeconds.

	FormatTime();	// Format m_iTotalSeconds for display.

	SetWindowText(m_FormattedTime);		// Display the time via inherited method.
}



inline void CGCCountdownCtrl::FormatTime()
{
	// Break down m_iTotalSeconds into DD, HH, MM and SS for display.

	m_Days = (int)( m_iTotalSeconds / SECONDS_PER_DAY );

	m_diff = (m_Days*SECONDS_PER_DAY);

	m_Seconds = m_iTotalSeconds - m_diff;

	m_Hours =  (int)( m_Seconds / SECONDS_PER_HOUR );

	m_diff = m_Hours* SECONDS_PER_HOUR;

	m_Seconds -= m_diff;

	m_Minutes = (int) (m_Seconds / 60);

	m_diff = m_Minutes * 60;

	m_Seconds -= m_diff;

	switch (m_bZeroPointPassed)
	{
		case FALSE:
			if ( m_uiStyle == CD_DHMS )
					m_FormattedTime.Format("%02d:%02d:%02d:%02d",
						                   m_Days,
									       m_Hours,
										   m_Minutes,
										   m_Seconds);
			else if (m_uiStyle == CD_HMS)
					m_FormattedTime.Format("%02d:%02d:%02d",
									       m_Hours,
										   m_Minutes,
										   m_Seconds);
			else if (m_uiStyle == CD_MS)
					m_FormattedTime.Format("%02d:%02d",
										   m_Minutes,
										   m_Seconds);
			else 
				m_FormattedTime.Format("%02d",m_Seconds);

			break;
		case TRUE:
			if ( m_uiStyle == CD_DHMS )
					m_FormattedTime.Format("-%02d:%02d:%02d:%02d",
						                   m_Days,
									       m_Hours,
										   m_Minutes,
										   m_Seconds);
			else if (m_uiStyle == CD_HMS)
						m_FormattedTime.Format("-%02d:%02d:%02d",
										       m_Hours,
											   m_Minutes,
											   m_Seconds);
			else if (m_uiStyle == CD_MS)
						m_FormattedTime.Format("-%02d:%02d",
											   m_Minutes,
											   m_Seconds);

			else 
				m_FormattedTime.Format("%02d",m_Seconds);
			break;
	}

}


BOOL CGCCountdownCtrl::Start()
{
	if (m_bZeroPointReached && ((m_uiStyle & CD_DAYS) != CD_DAYS)) return FALSE;

	UINT id = this->SetTimer(CNTDWN_UPDATE_TIMER_ID,1000,NULL);
	if (id == 0) return FALSE;
	m_bTimerFlag = TRUE;
	return TRUE;
}



void CGCCountdownCtrl::Stop()
{
	m_bStopFlag = TRUE;
}


BOOL CGCCountdownCtrl::Set(const UINT Days,
							const UINT Hours, 
							const UINT Minutes, 
							const UINT Seconds,
							const BOOL TerminateAtZero,
							const UINT Style,
							void (*fn_pointer)(UINT CtrlID))
{
	if ( (Style & CD_DAYS) == CD_DAYS )
		m_uiStyle = CD_DHMS;
	else if ( (Style & CD_HOURS) == CD_HOURS)
		m_uiStyle = CD_HMS;
	else if ( (Style & CD_MINUTES) == CD_MINUTES)
		m_uiStyle = CD_MS;
	else
		m_uiStyle = CD_S;


	m_bStopAtZero = TerminateAtZero;

	if ( m_uiStyle != CD_DHMS )	m_bStopAtZero = TRUE;

	UINT tmpDays = Days;
	UINT tmpHours = Hours;
	UINT tmpMinutes = Minutes;

	if ( (m_uiStyle & CD_DAYS) != CD_DAYS ) tmpDays = 0;
	if ( (m_uiStyle & CD_HOURS) != CD_HOURS ) tmpHours = 0;
	if ( (m_uiStyle & CD_MINUTES) != CD_MINUTES ) tmpMinutes = 0;

	// You have to specify SOMETHING?!
	if ((tmpDays == 0) && 
		(tmpHours == 0) && 
		(tmpMinutes == 0) && 
		(Seconds == 0)) return FALSE;

	if (m_bTimerFlag) m_bStopFlag = TRUE;

	m_fncptr = fn_pointer;

	// Compute time in total number of seconds.  This is easier to work with.

	m_iTotalSeconds = (tmpDays * 24 * 60 * 60) + 
		              (tmpHours * 60 * 60) + 
					  (tmpMinutes * 60) + 
					  Seconds;

	FormatTime();						// Format it for display.

	SetWindowText(m_FormattedTime);		// Display the time.

	return TRUE;
}



inline void CGCCountdownCtrl::Decrement()
{
	if (! m_bZeroPointReached)
	{
		// If we haven't reached the Zero Point, continue counting down...

		m_iTotalSeconds -= 1;

		if (m_iTotalSeconds == 0)
		{
			m_bZeroPointReached = TRUE;		// Zero Point reached.

			if (m_bStopAtZero)	m_bStopFlag = TRUE;

			if (m_fncptr)		// If set, execute function, passing it my control ID.
				(*m_fncptr)(this->GetDlgCtrlID());
		}

	}
	else
	{
		// We have passed Zero Point, begin counting up.

		m_bZeroPointPassed = TRUE;
		m_iTotalSeconds += 1;
	}

}

BOOL CGCCountdownCtrl::EndCountAtZero(const BOOL bZero)
{
	BOOL prev = m_bStopAtZero;
	m_bStopAtZero = bZero;
	return prev;
}


