// ColorStatic.cpp : implementation file
// ****************************************************************************
// ****************************************************************************
//                          CLASS CGCColorStatic
//
//						Developed By: George E. Chastain
//								Date: 11/1/99
// ****************************************************************************
// ****************************************************************************

#include "stdafx.h"
#include "GCColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
static int CtlColorCount = 0;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCColorStatic constructor

CGCColorStatic::CGCColorStatic(const BOOL bOn)
{   
	m_bBackgroundOn = bOn;	// Set initial bacground state.
	m_bTextOn = bOn;		// Set initial text state.

	m_bColorFrameFlag = FALSE;	// Is a color frame displayed? 
	m_ColorFrameWidth = 2;		// At what width is the color frame to be displayed, if displayed?

	// Initialize default on/off color variables.
	m_CurrentBackgroundOnColor = DEFAULT_BACKGROUND_ON_COLOR;
	m_CurrentBackgroundOffColor = DEFAULT_BACKGROUND_OFF_COLOR;
	m_CurrentTextOnColor = DEFAULT_TEXT_ON_COLOR;
	m_CurrentTextOffColor = DEFAULT_TEXT_OFF_COLOR;
	m_ColorFrameColor = DEFAULT_BACKGROUND_OFF_COLOR;

	// Create default background ON brush.
	m_brOnBrush.CreateSolidBrush(m_CurrentBackgroundOnColor);

	// Create default background OFF brush.
	m_brOffBrush.CreateSolidBrush(m_CurrentBackgroundOffColor);

//	m_brOffBrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	m_FlashTypeFlag = NONE;	// Default to not flashing.
	m_bTimerFlag = FALSE;	// Indicates if a flashing Timer exists or not.
	m_uiTimerID = 0;


	m_bHatchMode = FALSE;	// Is the background in hatch pattern mode?
	m_iHatchType = -1;		// Initialize hatch pattern.


	// Initialize variables used for Asymmetrical Background Flashing
	m_fOnDuration = 0.0;
	m_fOffDuration = 0.0;

	// Initialize variables used for Pulsed Background Flashing
	m_pulseDuration = 0.0;
	m_pulseRep = 0;
	m_pulseOnDuration = 0.0;
	m_pulseCounter = 0;

}

CGCColorStatic::~CGCColorStatic()
{
	m_brOnBrush.DeleteObject();
	m_brOffBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CGCColorStatic, CStatic)
	ON_MESSAGE( WM_GECCOLORSTATIC_DRAWFRAME, OnDrawFrame )
	//{{AFX_MSG_MAP(CGCColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCColorStatic message handlers



// ****************************************************************************
// ****************************************************************************
// CGCColorStatic::CtlColor()
// Handles the appropriate colors when the control is repainted on screen
// 
//									REMEMBER	
//
// If you handle WM_CTLCOLOR messages in your parent window (the window that
// contains this control) you must be sure to call the parent window's base 
// class OnCtlColor() handler if the parent's OnCtlColor() handler is being
// called for this control.  Otherwise, the OnCtlColor() handler in the 
// parent window will OVERRIDE the OnCtlColor() handler for this control.
//
// Windows is designed that way to give the parent a shot at handling the 
// message before reflecting it back to this control.
// ****************************************************************************

HBRUSH CGCColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	#ifdef _DEBUG
		CtlColorCount += 1;
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s %d\n",">>>>>>>>> CtlColor() called.",CtlColorCount);
	#endif


	// Set the Text Color according to the control's state.

	// It is not necessary to utilize the nCtlColor parameter in this implementation.

	if (m_bTextOn)
		pDC->SetTextColor(m_CurrentTextOnColor);					// Set text to its ON color.
	else		
		pDC->SetTextColor(m_CurrentTextOffColor);					// Set text to its OFF color.

	// Set the fill color according to the background state.

	if (m_bBackgroundOn)
		pDC->SetBkColor(m_CurrentBackgroundOnColor);
	else
		pDC->SetBkColor(m_CurrentBackgroundOffColor);

	// If the control is in Hatch Mode, the control will display the 
	// hatch in the background's ON color and the background behind the 
	// hatch pattern in the background's OFF color by calling the SetBkColor() method
	// of the CDC class above.
	// The SetHatch() method changes the ON brush to a hatch brush.
	// We return the hatch brush when in Hatch Mode.


	if (m_bHatchMode) 
	{
		// I do not set the background mode of the device context to transparent
		// when in Hatch Mode because having the lines passing through underneath
		// the text usually makes the text difficult to read when the control is in 
		// its "off" state.  Therefore I keep the background mode to OPAQUE (default) 
		// to use the background off color as a block on which the text is drawn.
		// This makes it much easier to read.
		//
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
		return static_cast<HBRUSH>(m_brOnBrush.GetSafeHandle());
	}

	// At this point, we must not be in Hatch Mode.

	// Set background mode of the DC to TRANSPARENT so that if a bitmap pattern
	// is being used for the background, it will show beneath the text.
	//
	pDC->SetBkMode(TRANSPARENT);

	

	if (!m_bBackgroundOn)  
	{
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
		return static_cast<HBRUSH>(m_brOffBrush.GetSafeHandle());	// Return background's OFF color.
	}
	else
	{
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
		return static_cast<HBRUSH>(m_brOnBrush.GetSafeHandle());	// Return background's ON color.
	}


}

// ****************************************************************************
// ****************************************************************************




COLORREF CGCColorStatic::SetBackgroundOnColor(const COLORREF backColor)
{

	// Prepare to return previous background ON color.
	COLORREF prevColor = m_CurrentBackgroundOnColor;	

	// Store new background ON color.
	m_CurrentBackgroundOnColor = backColor;				

	// Calling SetBackgroundOnColor() does not change whether or not 
	// a hatch pattern is displayed.  It changes the ON color of the
	// background.  If the control is in Hatch Mode, the hatch pattern
	// will be updated with the new ON color.  If the control is NOT in
	// Hatch Mode, then the solid ON color will be updated.

	if (!m_bHatchMode)
	{
		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetBackgroundOnColor() - creating background On solid brush.");
		#endif

		// We are not in Hatch Mode.  Delete the old brush and create a new
		// SOLID brush with the new "on" color.
		m_brOnBrush.DeleteObject();	  
		m_brOnBrush.CreateSolidBrush(m_CurrentBackgroundOnColor);
	}
	else
	{
		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetBackgroundOnColor() - creating background On hatch brush.");
		#endif

		// We ARE in Hatch Mode.  Delete the old hatch brush and create a new
		// HATCH brush with the new "on" color.
		m_brOnBrush.DeleteObject();
		m_brOnBrush.CreateHatchBrush(m_iHatchType,m_CurrentBackgroundOnColor);
	}
	

	Invalidate(TRUE);					// Force to redraw.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	return prevColor;			// Return previous background ON color.
}



COLORREF CGCColorStatic::SetTextOnColor(const COLORREF textColor)
{
	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetTextOnColor() - Setting text On color.");
	#endif

	COLORREF prevColor = m_CurrentTextOnColor;			// Prepare to return text's previous ON color.
	m_CurrentTextOnColor = textColor;					// Store text's new ON color.

	Invalidate(TRUE);								// Force edit window to repaint.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	return prevColor;
}





COLORREF CGCColorStatic::SetTextOffColor(const COLORREF offColor)
{
	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetTextOffColor() - Setting text Off color.");
	#endif

	COLORREF prevColor = m_CurrentTextOffColor;			// Prepare to return text's previous OFF color.
	m_CurrentTextOffColor = offColor;					// Store text's new OFF color.

	Invalidate(TRUE);								// Force edit window to repaint.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

	return prevColor;									// Return text's previous OFF color.
}







COLORREF CGCColorStatic::GetTextOffColor() const
{
	return m_CurrentTextOffColor;
}



void CGCColorStatic::GetTextOffColor(BYTE &Red, BYTE &Green, BYTE &Blue) const
{
	Red = GetRValue(m_CurrentTextOffColor);
	Green = GetGValue(m_CurrentTextOffColor);
	Blue = GetBValue(m_CurrentTextOffColor);

}



COLORREF CGCColorStatic::GetBackgroundOnColor() const
{
	return m_CurrentBackgroundOnColor;
}

COLORREF CGCColorStatic::GetTextOnColor() const
{
	return m_CurrentTextOnColor;
}



void CGCColorStatic::GetBackgroundOnColor(BYTE & Red, BYTE & Green, BYTE & Blue) const
{
	Red = GetRValue(m_CurrentBackgroundOnColor);
	Green = GetGValue(m_CurrentBackgroundOnColor);
	Blue = GetBValue(m_CurrentBackgroundOnColor);
}

void CGCColorStatic::GetTextOnColor(BYTE & Red, BYTE & Green, BYTE & Blue) const
{
	Red = GetRValue(m_CurrentTextOnColor);
	Green = GetGValue(m_CurrentTextOnColor);
	Blue = GetBValue(m_CurrentTextOnColor);
}



BOOL CGCColorStatic::SetPointFont(const int PointSize, const LPCTSTR fontname)
{
	BOOL bResult = FALSE;


	// Get device context for this edit box.

	CDC* pDC = this->GetDC();

	// Delete previous CGdiObject and free its memory.

	m_Font.DeleteObject();		

	// Create a new Font object.

	bResult = m_Font.CreatePointFont((PointSize*10),fontname,pDC);

	if (! bResult) return bResult;

	// Set the new font for this edit box.

	CStatic::SetFont(&m_Font,TRUE);

	// Release the device context.

	ReleaseDC(pDC);

	return bResult;
}


BOOL CGCColorStatic::SetFontIndirect(const LOGFONT * lpLogFont)
{
	BOOL bResult;


	// Delete previous CGdiObject and free its memory.

	m_Font.DeleteObject();		

	// Create a new Font object.

	bResult = m_Font.CreateFontIndirect(lpLogFont);

	if (! bResult) return bResult;

	// Set the new font for this edit box.

	CStatic::SetFont(&m_Font,TRUE);

	return bResult;
}

BOOL CGCColorStatic::SetPointFontIndirect(const LOGFONT * lpLogFont)
{
	BOOL bResult;

	// Delete previous CGdiObject and free its memory.

	m_Font.DeleteObject();	
	
	// Get device context for this edit box.

	CDC* pDC = this->GetDC();
	
	// Create a new Font object.

	bResult = m_Font.CreatePointFontIndirect(lpLogFont, pDC);

	if (! bResult)
	{
		// Release the device context.
		ReleaseDC(pDC);
		return bResult;
	}

	// Set the new font for this edit box.

	CStatic::SetFont(&m_Font,TRUE);

	// Release the device context.

	ReleaseDC(pDC);

	return bResult;
}


LOGFONT CGCColorStatic::GetLogFont() const
{
	LOGFONT fontstruct;
	CFont* pFont = NULL;

	pFont = this->GetFont();

	pFont->GetLogFont(&fontstruct);
	
	return fontstruct;
}


void CGCColorStatic::SetUnderline(const BOOL On)
{
	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();

	fontstruct.lfUnderline = (BYTE) On;

	SetFontIndirect(&fontstruct);
}

void CGCColorStatic::SetItalic(const BOOL On)
{
	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();

	fontstruct.lfItalic = (BYTE) On;

	SetFontIndirect(&fontstruct);
}

void CGCColorStatic::SetBold(const BOOL On)
{
	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();

	switch (On)
	{
		case TRUE:	
			fontstruct.lfWeight = 700;
			break;
		case FALSE:
			fontstruct.lfWeight = 400;
	}

	SetFontIndirect(&fontstruct);

}


BOOL CGCColorStatic::SetFontWeight(const int Weight)
{
	if ( (Weight < 1) || (Weight > 1000) ) return FALSE;

	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();

	fontstruct.lfWeight = Weight;

	return SetFontIndirect(&fontstruct);
}




BOOL CGCColorStatic::FlashBackground(const BOOL bFlash, 
									  const BOOL bFinalState,
									  const float duration)
{
	if (m_bTimerFlag) KillTimer(m_uiTimerID);	// If a timer is active, kill it.

	if (bFlash)
	{
		// We are turning on flashing of the background.
		m_bTextOn = m_bBackgroundOn;			// Resync on/off states.

		Invalidate(TRUE);					// Force to redraw.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

		m_FlashTypeFlag = BACKGROUND;			// Set flash type to BACKGROUND.
		m_bTimerFlag = TRUE;					// Yes, a timer process exists.
		m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(duration * 1000),NULL);	// Create the timer.
		if (!m_uiTimerID) return FALSE;
	}
	else
	{
		// If we are turning off the flashing, make sure the background is left
		// in the state specified by bFinalState.
		m_bBackgroundOn = bFinalState;			// Set final state of background.
		m_bTextOn = bFinalState;				// Resync text state with background state.
		m_FlashTypeFlag = NONE;					// No flash state exists.

		Invalidate(TRUE);					// Force to redraw.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	}

	return TRUE;
}


BOOL CGCColorStatic::FlashBackgroundAsym(const BOOL bFlash, 
										  const BOOL bFinalState,
										  const float OnDuration, 
										  const float OffDuration) 
{
	if (m_bTimerFlag) KillTimer(m_uiTimerID);		// If a timer is active, kill it.

	if (bFlash)
	{
		m_fOnDuration = OnDuration * 1000;
		m_fOffDuration = OffDuration * 1000;

		// We are turning on flashing of the background.

		SetOn(TRUE);							// Resync text with background and
												// set to on state.  Start asymmetrical
												// flashing out in ON state.


		m_FlashTypeFlag = ASYM_BACKGROUND;		// Set flash type to ASYM_BACKGROUND.
												// This is the Asymmetrical Flash mode.

		m_bTimerFlag = TRUE;					// Yes, a timer process exists.
		m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_fOnDuration),NULL);		// Create the timer.
		if (!m_uiTimerID) return FALSE;
	}
	else
	{
		// If we are turning off the flashing, make sure the background is left
		// in the state specified by bFinalState.
		m_bBackgroundOn = bFinalState;			// Set final state of background.
		m_bTextOn = bFinalState;				// Resync text state with background state.
		m_FlashTypeFlag = NONE;					// No flash state exists.
		Invalidate(TRUE);					// Force repaint.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	}

	return TRUE;
}


BOOL CGCColorStatic::FlashBackgroundPulsed(const BOOL bFlash, 
											const BOOL bFinalState,
											const float pulseDuration,
											const int pulseRep,
											const float onDuration) 
{
	if (m_bTimerFlag) KillTimer(m_uiTimerID);		// If a timer is active, kill it.

	if (bFlash)
	{
		m_pulseDuration = pulseDuration * 1000;
		m_pulseOnDuration = onDuration * 1000;
		m_pulseRep = pulseRep * 2 + 1;

		// We are turning on flashing of the background.

		SetOn(TRUE);							// Resync text with background and
												// set to on state.  Start pulsed
												// flashing out in ON state.


		m_FlashTypeFlag = PULSED_BACKGROUND;	// Set flash type to PULSED_BACKGROUND.
												// This is the Pulsed Flash mode.

		m_bTimerFlag = TRUE;					// Yes, a timer process exists.

		m_pulseCounter += 1;
		m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_pulseDuration),NULL);	// Create the timer.
		if (!m_uiTimerID) return FALSE;
	}
	else
	{
		// If we are turning off the flashing, make sure the background is left
		// in the state specified by bFinalState.
		m_bBackgroundOn = bFinalState;			// Set final state of background.
		m_bTextOn = bFinalState;				// Resync text state with background state.
		m_FlashTypeFlag = NONE;					// No flash state exists.
		Invalidate(TRUE);					// Force repaint.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	}

	return TRUE;
}


BOOL CGCColorStatic::FlashText(const BOOL bFlash, const BOOL bFinalState, const float duration)
{
	if (m_bTimerFlag) KillTimer(m_uiTimerID);	// If a timer is active, kill it.
		
	if (bFlash)
	{
		// We are turning on flashing of the text.

		m_bTextOn = m_bBackgroundOn;			// Resync on/off states.
		Invalidate(TRUE);
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

		m_FlashTypeFlag = TEXT;					// Set flash type to TEXT.
		m_bTimerFlag = TRUE;					// Yes, a timer process exists.

		m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(duration * 1000),NULL);	// Spawn timer.
		if (!m_uiTimerID) return FALSE;
	}
	else
	{
		m_FlashTypeFlag = NONE;					// No flash state exists.
		m_bTextOn = bFinalState;				// Resync text with background final state.
		Invalidate(TRUE);					// Force redraw.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	}

	return TRUE;
}


void CGCColorStatic::OnTimer(UINT nIDEvent) 
{
	// Toggle the state of the appropriate item.
	switch (m_FlashTypeFlag)
	{
		case TEXT:
			// If the flash type is TEXT...

			m_bTextOn = !m_bTextOn;					// Invert state.

			Invalidate(TRUE);					// Force to redraw.
			PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

			break;
		case BACKGROUND:
			// If the flash type is BACKGROUND...

			m_bBackgroundOn = !m_bBackgroundOn;		// Invert state.
			m_bTextOn = !m_bTextOn;					// Invert state.  Yes, I do this here 
													// but not for TEXT above.
			Invalidate(TRUE);					// Force to redraw.
			PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

			break;
		case ASYM_BACKGROUND:
			// If the flash type is Asymmetrical Background...

			KillTimer(m_uiTimerID);	// Kill previous timer.

			// When FlashBackgroundAsym() is called, the control is immediately updated
			// to the "ON" state by that method.  So, we want to start out with 
			// setting the timer for the OnDuration as specified by m_fOnDuration 
			// member variable.
			m_bBackgroundOn = !m_bBackgroundOn;		// Invert state.
			m_bTextOn = !m_bTextOn;					// Invert state.  Yes, I do this here 

			Invalidate(TRUE);
			PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

			switch (m_bBackgroundOn)
			{
				case TRUE:
					m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_fOnDuration),NULL);	// Create the "ON" timer.
					break;
				case FALSE:
					m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_fOffDuration),NULL);	// Create the "OFF" timer.
			}
			break;
		case PULSED_BACKGROUND:

			// If the flash type is Pulsed Background...

			// When FlashBackgroundPulsed() is called, the control is immediately updated
			// to the "ON" state by that method.  So, we want to start out with 
			// setting the timer for the pulse's on duration as specified by m_pulseDuration 
			// member variable.
			m_bBackgroundOn = !m_bBackgroundOn;		// Invert state.
			m_bTextOn = !m_bTextOn;					// Invert state.  Yes, I do this here 

			Invalidate(TRUE);
			PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

			m_pulseCounter += 1;

			if (m_pulseCounter <= m_pulseRep) 
			{
				m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_pulseDuration),NULL);   	// Create a pulse timer.
			}
			else
			{
				m_pulseCounter = 0;
				SetOn();
				m_uiTimerID = SetTimer(FLASH_TIMER_ID,int(m_pulseOnDuration),NULL);	// Create the "ON" timer.
			}

	}		
	CStatic::OnTimer(nIDEvent);
}




COLORREF CGCColorStatic::SetBackgroundOffColor(const COLORREF offColor)
{
	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetBackgroundOffColor() - Creating background Off brush.");
	#endif

	COLORREF prevColor = m_CurrentBackgroundOffColor;			// Prepare to return background's previous OFF color.
	m_CurrentBackgroundOffColor = offColor;						// Store background's new OFF color.
	m_brOffBrush.DeleteObject();								// Delete background's old OFF brush.
	m_brOffBrush.CreateSolidBrush(m_CurrentBackgroundOffColor);	// Create background's new OFF brush.

	if (!m_bBackgroundOn) 
	{
		Invalidate(TRUE);					// Force to redraw.
		PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
	}
	return prevColor;											// Return background's previous OFF color.
}




COLORREF CGCColorStatic::GetBackgroundOffColor() const
{
	return m_CurrentBackgroundOffColor;
}


void CGCColorStatic::GetBackgroundOffColor(BYTE &Red, BYTE &Green, BYTE &Blue) const
{
	Red = GetRValue(m_CurrentBackgroundOffColor);
	Green = GetGValue(m_CurrentBackgroundOffColor);
	Blue = GetBValue(m_CurrentBackgroundOffColor);
}





BOOL CGCColorStatic::SetOn(const BOOL bOn)
{

	#ifdef _DEBUG
		if (bOn)
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetOn() - Setting state to ON");
		else
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">> SetOn() - Setting state to OFF");
	#endif _DEBUG

	BOOL prevState = m_bBackgroundOn;   // Key off background for overall state of control.

	m_bBackgroundOn = bOn;				// Set background to specified on/off state.
	m_bTextOn = m_bBackgroundOn;		// Sync text state to background state.

	Invalidate();

	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);


	return prevState;					// Return previous state.
}



BOOL CGCColorStatic::SetSunken(const BOOL bSunken)
{
	BOOL Result = FALSE;

	if (bSunken)
	{
		if (m_bColorFrameFlag) SetColorFrame(FALSE);	// Color frame and sunken frame mutually exclusive.

		Result = ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
	}
	else
		Result = ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);

	return Result;
}


BOOL CGCColorStatic::SetBorder(const BOOL bBorder)
{
	BOOL Result = FALSE;

	if (bBorder)
		Result = ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
	else
		Result = ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);


	return Result;
}


BOOL CGCColorStatic::SetModalFrame(const BOOL bModal)
{
	BOOL Result = FALSE;

	if (bModal)
	{
		if (m_bColorFrameFlag) SetColorFrame(FALSE);	// Color frame and modal frame mutually exclusive.

		Result = ModifyStyleEx(0,WS_EX_DLGMODALFRAME,SWP_DRAWFRAME);
	}
	else
		Result = ModifyStyleEx(WS_EX_DLGMODALFRAME,0,SWP_DRAWFRAME);
   
	return Result;
}



BOOL CGCColorStatic::TextOn(const BOOL bOn)
{
	BOOL prevState = m_bTextOn;

	if (m_bTextOn == bOn) return m_bTextOn;

	m_bTextOn = bOn;

	Invalidate(TRUE);
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

	return prevState;
}




void CGCColorStatic::GetStates(BOOL &bBackgroundState, BOOL &bTextState) const
{
	bBackgroundState = m_bBackgroundOn;
	bTextState = m_bTextOn;
}






COLORREF CGCColorStatic::SetBackgroundOnPattern(CBitmap * pattern)
{
	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetBackgroundOnPattern() - Creating background on pattern brush");
	#endif

	m_bHatchMode = FALSE;
	COLORREF prev = m_CurrentBackgroundOnColor;

	// Since the bitmap to be used to create the pattern brush may
	// not be a single color, just set the background "on" color to
	// the default background ON color.
	m_CurrentBackgroundOnColor = DEFAULT_BACKGROUND_ON_COLOR;

	m_brOnBrush.DeleteObject();					// Delete the old background "on" brush.
	m_brOnBrush.CreatePatternBrush(pattern);	// Create the new "on" pattern brush.

	Invalidate();		// Repaint the control.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);


	return prev;			// Return previous background ON color.
}



COLORREF CGCColorStatic::SetBackgroundOffPattern(CBitmap * pattern)
{
	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetBackgroundOffPattern() - Creating background off pattern brush");
	#endif

	m_bHatchMode = FALSE;
	COLORREF prev = m_CurrentBackgroundOffColor;

	// Since the bitmap to be used to create the pattern brush may
	// not be a single color, just set the background "off" color to
	// the default background OFF color.
	m_CurrentBackgroundOffColor = DEFAULT_BACKGROUND_OFF_COLOR;

	m_brOffBrush.DeleteObject();				// Delete the old background "on" brush.
	m_brOffBrush.CreatePatternBrush(pattern);	// Create the new "on" pattern brush.

	Invalidate();		// Repaint the control.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

	return prev;			// Return previous background OFF color.
}



BOOL CGCColorStatic::SetHatch(const BOOL bOn, 
							   const UINT HatchType)
{
	if (m_bHatchMode == bOn) return (m_bHatchMode == bOn);

	m_bHatchMode = bOn;

	if (m_bHatchMode)
	{
		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetHatch() - Creating hatch brush");
		#endif

		// If the control is being set to Hatch Mode, delete the old brush
		// used for the background's ON state and create a hatch brush
		// using the same color.  

		m_brOnBrush.DeleteObject();

		m_brOnBrush.CreateHatchBrush(HatchType,m_CurrentBackgroundOnColor);

		// If the previous background OFF brush was a bitmap pattern,
		// we want to delete it and set the background OFF color to the default
		// off color.  It is expected that the client will set a color for the
		// background off color as well as for the background on color that will
		// be used for the hatch pattern.

		LOGBRUSH logicalBrush;
		m_brOffBrush.GetLogBrush(&logicalBrush);
		if ( 
				(logicalBrush.lbStyle == BS_PATTERN)
				||
				(logicalBrush.lbStyle == BS_PATTERN8X8)
		   )
		{
			m_CurrentBackgroundOffColor = DEFAULT_BACKGROUND_OFF_COLOR;
			m_brOffBrush.DeleteObject();
			m_brOffBrush.CreateSolidBrush(DEFAULT_BACKGROUND_OFF_COLOR);
		}

		m_iHatchType = HatchType;	
	}
	else
	{
		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetHatch() - Creating solid brush");
		#endif

		// if Hatch Mode is being turned off, then delete the hatch brush
		// and re-create the solid brush for the background's ON state.
		m_brOnBrush.DeleteObject();
		m_brOnBrush.CreateSolidBrush(m_CurrentBackgroundOnColor);
	}

	Invalidate();		// Force the control to redraw.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);

	return FALSE;
}





void CGCColorStatic::SetColorFrame(const BOOL bOn, const int Width, const COLORREF FrameColor)
{
	m_bColorFrameFlag = bOn;

	if (!bOn)
	{
		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetColorFrame() - Color Frame turned on");
		#endif

		if (m_bBackgroundOn)
			m_ColorFrameColor = m_CurrentBackgroundOnColor;
		else
			m_ColorFrameColor = m_CurrentBackgroundOffColor;
	}
	else
	{
		// I made the color frame mutually exclusive of the modal and sunken frames. They just
		// do not look nice together.
		// When using the modal or sunken frames, the client area that is actually visible
		// is decreased in size.  If you were to add the color frame with the modal or sunken
		// frames, and draw the color frame so that it did not overlay the modal or sunken frame, 
		// the usable area would be even smaller.  Therefore, as a design decision,
		// I made the color frame border type mutually exclusive with the modal and sunken types.
		// Overlaying the color frame on the modal or sunken frame looks bad.

		BOOL Result = ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE,0,SWP_DRAWFRAME);

		m_ColorFrameWidth = Width;

		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>SetColorFrame() - Color Frame turned off");
		#endif

		m_ColorFrameColor = FrameColor;
	}

	Invalidate(TRUE);					// Force to redraw.
	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
}


/*

void CGCColorStatic::OnNcPaint() 
{
	if (m_bColorFrameFlag)
	{
		// Note:  I do not inflate the rectangle at which the color frame is drawn because
		// this would make the control take up more area on the screen.  If there are multiple
		// indicators next to each other, you would have the color frame of one
		// control being drawn on top of the adjoinging control.  Therefore, I paint the
		// frame inside the edges of the control.  Remember this when selecting fonts because
		// the color frame makes the area availble for displaying text a little smaller.

		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>OnNcPaint() - Painting frame");

		CRect rect;
		this->GetWindowRect(&rect);
		CPen pen(PS_INSIDEFRAME , m_ColorFrameWidth,m_ColorFrameColor);
		ScreenToClient(&rect);
		CDC* pDDC = this->GetDC();
		pDDC->SelectObject(pen);
		pDDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDDC->Rectangle(rect);
		ReleaseDC(pDDC);
	}
	else
	{
		_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>OnNcPaint() - Calling base class OnNcPaint()");
		CStatic::OnNcPaint();
	}
	
		CStatic::OnNcPaint();
}
*/


LRESULT CGCColorStatic::OnDrawFrame(WPARAM wp, LPARAM lp)
{
	if (m_bColorFrameFlag)
	{
		// Note:  I do not inflate the rectangle at which the color frame is drawn because
		// this would make the control take up more area on the screen.  If there are multiple
		// indicators next to each other, you would have the color frame of one
		// control being drawn on top of the adjoinging control.  Therefore, I paint the
		// frame inside the edges of the control.  Remember this when selecting fonts because
		// the color frame makes the area availble for displaying text a little smaller.

		#ifdef _DEBUG
			_CrtDbgReport(_CRT_WARN,"GECColorStatic.cpp",__LINE__,"Demo","\n%s\n",">>OnDrawFrame() - Painting frame");
		#endif

		CRect rect;
		this->GetWindowRect(&rect);

		// Do not allow an exception constructing the pen or from creating
		// the device context to kill the client.
		try
		{
			CPen pen(PS_INSIDEFRAME , m_ColorFrameWidth,m_ColorFrameColor);
			ScreenToClient(&rect);
			CClientDC dc(this);
			dc.SelectObject(pen);
			dc.SelectObject(GetStockObject(NULL_BRUSH));
			dc.Rectangle(rect);
		}
		catch (CResourceException)
		{
		}
	}
	return 0;
}


void CGCColorStatic::SetWindowText(LPCTSTR lpszString)
{
	// Had to override this method.  Calling the
	// inherited SetWindowText() resulted in a message being generated causing the
	// background to be repainted.  Since my color frame is drawn inside the edge of the 
	// control, over the client area, this would erase the colored border when a colored
	// border was being used.  Therefore, I had to override this method and force 
	// the redraw of the frame so that it would stay visible.
	//
	//
	CStatic::SetWindowText(lpszString);

	PostMessage(WM_GECCOLORSTATIC_DRAWFRAME);
}


