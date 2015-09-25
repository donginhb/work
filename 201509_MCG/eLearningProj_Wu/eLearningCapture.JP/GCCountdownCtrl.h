// ****************************************************************************
// ****************************************************************************
//
//                                CGCCountdownCtrl
//
//                                Countdown Control
//
//                                    Version 1.0
//
//                           Developed By: George Chastain
//                                    11/10/99
//
// Derived from the CGECColorStatic class, this class provides the ability to
// quickly include a countdown time display.  Since it is publicly derived from
// the CGECColorStatic class, all the CGECColorStatic class' methods are 
// available for controlling the look of the countdown control.
//
// What does this class allow you to do?
//
// -- Specify any time in Days, Hours, Minutes and Seconds start a countdown
//    from.  
//
// -- Change the format Style of the control to either DD:HH:MM:SS, HH:MM:SS, MM:SS or
//    just SS.  Note:  DD = Number of Days remaining, HH = Number of Hours 
//    remaining, MM = Number of Minutes remaining and SS = Number of Seconds
//    remaining.
//
// -- Specify if you want to stop at zero or continue counting negative.
//    You may count negative only if the specified style for the control
//    states a format style of DD:HH:MM:SS so that an "infinite" negative time
//    may be supported.
//
// -- Specify a function to call when the Countdown Control reaches the Zero Point.
//
// -- Stop the countdown at any point and then resume the countdown from the
//    point at which it was stopped.
//
// ****************************************************************************
// ****************************************************************************

#if !defined(__CGCCountdownCtrl__)
#define __CGCCountdownCtrl__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GCColorStatic.h"

class CGCCountdownCtrl : public CGCColorStatic
{
public:
	enum {	CD_SECONDS	= 0x0001,		// Display SS (seconds) only.
			CD_MINUTES	= 0x0010,		// Display MM:SS (minutes::seconds) only.
			CD_HOURS	= 0x0100,		// Display HH:MM:SS (hours:minutes:seconds) only.
			CD_DAYS		= 0x1000};		// Display DD:HH:MM:SS (days:hours:minutes:seconds).


	// Set() - Sets the number of Days, Hours, Minutes and Seconds at which the countdown
	//         is to begin.
	//         Returns TRUE if successful.
	//
	BOOL Set(const UINT Days = 0,						// How many days are we 
														// to countdown from?
														// Ignored if Style != CD_DAYS.
		
		     const UINT Hours = 1,						// How many hours are we 
														// to countdown from?
														// Ignored if Style != CD_DAYS
														// or CD_HOURS.

			 const UINT Minutes = 0,					// How many minutes are we
														// to countdown from?
														// Ignored if Style != CD_DAYS,
														// CD_HOURS, or CD_MINUTES.

			 const UINT Seconds = 0,					// How many seconds are we 
														// to countdown from?
														// Seconds are ALWAYS displayed
														// at a minimum.

			 const BOOL TerminateAtZero = TRUE,			// Should it terminate at Zero?
														// Only used when Style = CD_DAYS.  
														// Other styles imply 
														// TerminateAtZero = TRUE by design.

			 const UINT Style = CD_DAYS,				// Will the Countdown Control 
														// display DD:HH:MM:SS, 
														// HH:MM:SS, MM:SS, or SS only?
														// Possible Styles:
														// CD_SECONDS displays SS only.
														// CD_MINUTES displays MM:SS only.
														// CD_HOURS displays HH:MM:SS only.
														// CD_DAYS displays DD:HH:MM:SS.

			 void (*fn_pointer)(UINT CtrlID) = NULL);	// Pointer to a function to 
														// call when the Countdown 
														// Control reaches 0.


	// Start() - Must be called to initialize and start the Countdown Timer.
	//           Returns FALSE if a failure occurred starting the Countdown Timer.
	//           May also return FALSE if the Countdown Control was set to terminate
	//           at zero and the zero point has aleady been reached.
	//
	BOOL Start();


	// Stop() - Allows client to suspend countdown.  The countdown may be restarted
	// at the point at which the countdown was suspended by calling Start() again.
	//
	void Stop();	
   	BOOL m_bZeroPointPassed;		// TRUE means We have passed the Zero Point.


	// EndCountAtZero() - Allows client to change setting of the TerminateAtZero flag
	// passed in when Set() was called.  Returns previous state of this setting.
	//
	BOOL EndCountAtZero(const BOOL bZero = TRUE);
   	BOOL m_bZeroPointReached;		// TRUE means I have reached by Zero Point.
 

	CGCCountdownCtrl();
	~CGCCountdownCtrl();
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
private:

	// Variables used when calculating time from total number of seconds 
	// for formatted display.
	int m_diff;
	int m_Seconds;
	int m_Minutes;
	int m_Hours;
	int m_Days;

	// Internal style flags.
	enum {	CD_DHMS = CD_DAYS | CD_HOURS | CD_MINUTES | CD_SECONDS,
			CD_HMS	= CD_HOURS | CD_MINUTES | CD_SECONDS,
			CD_MS	= CD_MINUTES | CD_SECONDS,
			CD_S	= CD_SECONDS};

	UINT m_uiStyle;					// Do I display DD:HH:MM:SS, HH:MM:SS, MM:SS or just SS?
	BOOL m_bStopAtZero;				// Do I stop when I reach Zero Point?

	void (*m_fncptr)(UINT CtrlID);	// Function to call when I reach Zero Point.

	int m_iTotalSeconds;			// Total time in seconds in my countdown.


	BOOL m_bStopFlag;				// TRUE means Stop countdown now!


	void Decrement();				// Function to decrement/increment counter.


	BOOL m_bTimerFlag;				// A timer process exists.

	CString m_FormattedTime;		// Timer time formatted for display.

	void FormatTime();				// Function to format m_iTotalSeconds for display.

	// Class private message.
	// DO NOT USE THE VALUE OF (WM_APP + 1)!!! That value is reserved for 
	// a private message utilized by the CGECColorStatic class.
	//
	enum {CNTDWN_UPDATE_TIMER_ID = WM_APP + 2};
};

#endif