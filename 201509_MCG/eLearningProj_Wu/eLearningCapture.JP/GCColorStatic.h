#if !defined(AFX_COLORSTATIC_H__CA34AF76_8DFF_11D3_A770_00105AA7390A__INCLUDED_)
#define AFX_COLORSTATIC_H__CA34AF76_8DFF_11D3_A770_00105AA7390A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CGCColorStatic.h : header file
//
// ****************************************************************************
// ****************************************************************************
//                             CLASS CGCColorStatic
//
//                        Makes a good status indicator!
//
//                       Developed By: George E. Chastain
//                              Software Consultant
//                                 Date: 11/1/99
//
// In my line of work, I often need status indicators with a wide variety of
// capabilities.  These include all types of color capabilities as well as
// different flashing capabilities.  I developed this class to be an all-encompasing
// general-purpose status display control.  It should Fulfill almost any need. 
//
// I did not develop this control to support rotated text for two reasons:
//
// 1. Rarely (if ever) do you need to display rotated text.  This is usually
//    not recommended in MMIs.  Especially for status indicators that need to
//    display critical status information.
//
// 2. I don't know how to do it. :-)  At least not without implementing a full owner-drawn
//    control.
//
//
//
// What does this class allow you to do?
//
// -- Define an On and an Off state.  This class allows the client to
//    create a static field of a specific color that never changes, or 
//    the client can define an ON/SET and and OFF/UNSET state by calling different
//    interfaces to define different colors for the two states.
//
// -- Specify the color to be used for the background in the On/Set state.
//
// -- Specify the color to be used for the background in the Off/Unset state.
//
// -- Specify the color to be used for the text in the On/Set state.
//
// -- Specify the color to be used for the text in the Off/Unset state.
//
// -- Specify a bitmap pattern to use for the background's On/Set state.
//
// -- Specify a bitmap pattern to use for the background's Off/Unset state.
//
// -- Specify a hatch pattern to use for the background.
//
// -- Define the font to use for the text.
//
// -- Bold face
//
// -- Italics
//
// -- Underline
//
// -- Symmetrical Flashing of the Background color between ON and OFF states 
//    (on and off background color).  This flash mode makes the duration of the
//    "off" state the same as the duration of the "on" state.
//
// -- Asymmetrical Flashing of the Background color between ON and OFF states.
//    This flash mode allows the client to set the duration of the "on" and "off"
//    states to different time lengths.  For example, the control can remain in the
//    "ON" state for 2 seconds and off 1 second.  Any interval may be defined by the
//    client.
//
// -- Pulsed Flashing of the Background color between ON and OFF states.
//    This flash mode allows the client to set a sequence of symmetrical flashes
//    of a specified duration, followed by a steady "on" state for a client-specified
//    amount of time. For example, the client could have the control flash 4 quick times
//    followed by a longer, "on" state, followed by 4 more quick flashes, and so on.
//
// -- Flash Text between between ON and OFF states (on and off text colors).
//
//    Flashing text is mutually exclusive of flashing background.
//
// -- With all types of flashing, the client may specify the final state desired when
//    the flashing is terminated.  In many cases with status indicators, it is desired
//    that the indicator flash for some period of time and then remain in a specific
//    state.  This class provides a convenient way of ensuring that it is left in either
//    the "on" or "off" state as specified by the client when flashing was initiated.
//
// -- Asymmetrical flashing of the background only.  Where the "ON" state is displayed
//    for a specified amount of time followed by the "OFF" state for a different
//    amount of time and so on.
//
// -- Display a border or remove it.
//
// -- Display the control as sunken or not.
//
// -- Display the control as sunken with a border.
//
// -- Display the control with a modal style or not.
//
// -- ****** Display a colored frame with a user-specified color.  This color is independant of
//    the other colors used by the control.
//
//
// NOTES:
//
// When the client calls the FlashBackground() method or the FlashBackgroundAsym() method, 
// the control will alternate between the "ON" and the "OFF" states.  That is, 
// the control will be displayed with the "ON" background color and the "ON" text color, 
// then it will be displayed with the "OFF" background color and the "OFF" text color.  
// This will alternate until the FlashBackground() method is called again with 
// the appropriate parameters to terminate the flashing.
//
// When the client calls the FlashText() method, the control will keep the background color
// set to whatever it was when the FlashText() method was called.  The text will alternate
// between the text's "OFF" color and the text's "ON" color.  When the client calls 
// FlashText() again with the appropriate parameters to terminate the flashing, the control
// will be left in either the "ON" or "OFF" state according to those parameters.
// When calling FlashText(), the on/off state of the text and the on/off state
// of the background become out of sync.  If the client does not need an "on"/"off" condition,
// this is ok.  Note however, that when the FlashBackground() or the FlashText()
// methods are called, the on/off state of the text is resynced automatically with
// the on/off state of the background.  Also, calling SetOn(TRUE) or SetOn(FALSE)
// will resync the background and text states.

//
// See the descriptions for FlashBackground(), FlashBackgroundAsym(), 
// FlashBackgroundPulsee() and FlashText() methods below for further
// details.
//
// This class is derived (public) from the MFC class CStatic so all of its methods
// are available for use.
//
// This class makes use of the reflected WM_CTLCOLOR message to handle its own color change.
// See the ON_WM_CTLCOLOR_REFLECT() macro in the message map in GECColorStatic.cpp.
//
// How to use:
//
// 1.  Include this class in your project.
// 2.  Place the static control where desired on the dialog using the resource
//     editor (or dynamically create it if you're a sick puppy).  
// 3.  Use Class Wizard to define a variable for the static control of class 
//     CGCColorStatic.
//
// 
//
// ****************************************************************************
// ****************************************************************************



class CGCColorStatic : public CStatic
{
public:
	// Define default colors.  These are available Publicly for reference.

	enum {	DEFAULT_BACKGROUND_ON_COLOR = RGB(255,255,255),	// Default background ON state.
			DEFAULT_BACKGROUND_OFF_COLOR = RGB(0,0,0),		// Default background OFF color.
			DEFAULT_TEXT_ON_COLOR = RGB(0,0,0),				// Default text ON color.
			DEFAULT_TEXT_OFF_COLOR = RGB(255,255,255),		// Default text OFF color.
			DEFAULT_COLOR_FRAME_COLOR = RGB(255,0,0)};		// Default color frame color.
private:


	enum {	FLASH_TIMER_ID = 1};					// Id of flash timer.

	// Define a private message to be used to draw/redraw the color frame as needed.
	enum {WM_GECCOLORSTATIC_DRAWFRAME = WM_APP+1};	

	// The private type FLASH_TYPE is used to identify the flash state of the control.

	enum FLASH_TYPE {BACKGROUND, ASYM_BACKGROUND, PULSED_BACKGROUND, TEXT, NONE};

public:
	CGCColorStatic(const BOOL bOn = TRUE);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCColorStatic)
	//}}AFX_VIRTUAL

public:

		// SetOn() -- If bOn is TRUE, sets the control's background color to the
		// background's ON color (as specified by a call to SetBackgroundOnColor()),
		// and the text color to the text's ON color (as specified by a call
		// to SetTextOnColor()).
		// If bOn is FALSE, it sets the control's background color to the
		// background's OFF color (as specified by a call to SetBackgroundOffColor()),
		// and the text color to the text's OFF color (as specified by a call to
		// SetTextOffColor()).
		// Returns previous state.
		//
	BOOL SetOn(const BOOL bOn = TRUE);

		// SetBackgroundOnColor() -- Sets new background color. Returns previous 
		// background color.  This color is used as the "on" color when flashing 
		// the background. If the previous "on" color was a pattern, then 
		// DEFAULT_BACKGROUND_ON_COLOR is returned as the previous color.
		//
	COLORREF SetBackgroundOnColor(const COLORREF backColor = DEFAULT_BACKGROUND_ON_COLOR);


		// SetBackgroundOffColor() -- Used to define the "off" color used 
		// when flashing the background color of the control. Returns the 
		// previous "off" color. If the previous "off" color was a pattern, then 
		// DEFAULT_BACKGROUND_OFF_COLOR is returned as the previous
		// color.
		//
	COLORREF SetBackgroundOffColor(const COLORREF offColor = DEFAULT_BACKGROUND_OFF_COLOR);


		// SetBackgroundOnPattern() -- Used to set the "on" background color to 
		// the specified pattern.  Returns the previous color.  If the previous 
		// "on" background color was a pattern,then DEFAULT_BACKGROUND_ON_COLOR
		// is returned as the previous color.
		//
	COLORREF SetBackgroundOnPattern(CBitmap* pattern);

		// SetBackgroundOffPattern() -- Used to set the "off" background color to 
		// the specified pattern.  Returns the previous color.  If the previous 
		// "off" background color was a pattern,then DEFAULT_BACKGROUND_OFF_COLOR
		// is returned as the previous color.
		//
	COLORREF SetBackgroundOffPattern(CBitmap* pattern);


		// SetHatch() -- Sets the control the use a Hatch Pattern.
		// The control will utilize the color set by a call to 
		// SetBackgroundOnColor() as the color of the hatch pattern.  
		// The color set by a call to SetBackgroundOffColor()
		// is used to fill in the area between the lines in the hatch pattern.
		//
		// The HatchType parameter may be set to any of these Windows-defined patterns:
		//
		// HS_BDIAGONAL		Downward hatch (left to right) at 45 degrees
		// HS_CROSS			Horizontal and vertical crosshatch
		// HS_DIAGCROSS		Crosshatch at 45 degrees
		// HS_FDIAGONAL		Upward hatch (left to right) at 45 degrees
		// HS_HORIZONTAL	Horizontal hatch
		// HS_VERTICAL		Vertical hatch
		//
	BOOL SetHatch(const BOOL bOn = TRUE, 
		          const UINT HatchType = HS_VERTICAL);

		// SetTextOnColor() -- Sets new text color.  Returns previous text color.
		//
	COLORREF SetTextOnColor(const COLORREF textColor = DEFAULT_TEXT_ON_COLOR);
	

		// SetTextOffColor() -- Set the text's OFF color.
		//
	COLORREF SetTextOffColor(const COLORREF offColor = DEFAULT_TEXT_OFF_COLOR);


		// GetBackgroundOnColor() -- Returns the current background ON 
		// color as a COLORREF value.
		// If the current background "on" color is a pattern, the returned color
		// will be black (0).
		//
	COLORREF GetBackgroundOnColor() const;

		// GetBackgroundOnColor() -- Returns current background ON color as 
		// separate red, green and blue components.
		// If the current background "on" color is a pattern, the returned color
		// will be black (0,0,0).
		//
	void GetBackgroundOnColor(BYTE& Red, BYTE& Green, BYTE& Blue) const;


		// GetBackgroundOffColor() -- Returns the current background OFF 
		// color as a COLORREF value.
		// If the current background "off" color is a pattern, the returned color
		// will be black (0).
		//
	COLORREF GetBackgroundOffColor() const;

		// GetBackgroundOffColor() -- Returns current background OFF color as 
		// separate red, green and blue components.
		// If the current background "off" color is a pattern, the returned color
		// will be black (0,0,0).
		//
	void GetBackgroundOffColor(BYTE& Red, BYTE& Green, BYTE& Blue) const;

		// SetWindowText() -- Used to set/change the text displayed in the control.
		// See comments to this override in the implementation for details on why
		// this was overridden.
		//
	void SetWindowText( LPCTSTR lpszString );


		// GetTextOnColor() -- Returns the current text color as a COLORREF value.
		//
	COLORREF GetTextOnColor() const;

		// GetTextOffColor() -- Returns the OFF color for the text.
		//
	COLORREF GetTextOffColor() const;

		// GetTextOffColor() -- Returns the OFF color for the text as separate red, 
		// green, and blue components.
		//
	void GetTextOffColor(BYTE& Red, BYTE& Green, BYTE& Blue) const;


		// GetTextOnColorRGB() -- Returns the current text color as separate RGB values.
		//
	void GetTextOnColor(BYTE& Red, BYTE& Green, BYTE& Blue) const;

		// TextOn() -- Allows manual setting of the text to the ON or OFF state.
		// If bOn == TRUE, text is displayed in the text's "ON" color.  If bOn == FALSE,
		// the text is displayed in the text's "OFF" color.
		// Returns previous state.
		//
		// Note using this interface places the on/off state of the text and 
		// the on/off state of the background out of sync.  If the client does not 
		// need an "on"/"off" condition, this is ok.  Note however, that when 
		// the FlashBackground() or the FlashText() methods are called, 
		// the on/off states are resynced automatically by those methods.
		//
	BOOL TextOn(const BOOL bOn = TRUE);


		// FlashBackground() -- Flashes the background of the static text field.
		// bFlash      --  TRUE if you want to flash it. FALSE to stop it.
		// duration    --  Interval of flash in seconds that you want to flash it.  
		//                 If you want to flash at a rate of say once every half-second,
		//                 set duration to 0.5.
		// bFinalState --  When turning the flash off (that is, when bFlash is FALSE), 
		//                 this flag forces the background to be left in the desired 
		//                 "on" (TRUE) or "off" (FALSE) state.  The default is to leave it "on".
		//                 This also forces the text to be in the same state, keeping
		//                 the on/off state of the text in sync with the on/off state of 
		//                 the background.
		//
		// The color specified by a call to SetBackgroundOnColor() is used as 
		// the "on" state color. The "off" state color is set by a call to 
		// SetBackgroundOffColor(). Returns TRUE if successful.
		// 
		// Flashing the background does NOT affect the hatch color if the control 
		// has been set to Hatch Mode by a call to the SetHatch() method.  The 
		// hatch pattern will ALWAYS utilize the background's ON color.  When the 
		// background flashes, as the background changes to the ON state, it will 
		// appear as if the hatch pattern vanishes and then reappears as the control's 
		// background transitions to the OFF state.
		//
	BOOL FlashBackground(const BOOL bFlash, 
						 const BOOL bFinalState = TRUE,
		                 const float duration = 1.0);			// Default 1 second

		// FlashBackgroundAsym() -- Flashes the background of the static text field
		// asymmetrically.  That is, the client may specify an "ON" time and and "OFF" 
		// time.  The background will flash with the "ON" state displayed for OnDuration
		// time followed by the "OFF" state for the OffDuration time.  Both OnDuration
		// and OffDuration parameters are specified in seconds. The bFinalState parameter
		// is used in exactly the same way as for the FlashBackground() method above.
		//
		// The colors are treated in the same manner as for FlashBackground() 
		// defined above.
		//	 
		// Flashing the background does NOT affect the hatch color if the control 
		// is been set to Hatch Mode by a call to the SetHatch() method.  The 
		// hatch pattern will ALWAYS utilize the background's ON color.  When the 
		// background flashes, as the background changes to the ON state, it will 
		// appear as if the hatch pattern vanishes and then reappears as the 
		// control's background transitions to the OFF state.
		//
		//
	BOOL FlashBackgroundAsym(const BOOL bFlash, 
							 const BOOL bFinalState = TRUE,
		                     const float OnDuration = 1.0,		// Default 1 second
							 const float OffDuration = 0.5);		// Default 0.5 second
	

		// FlashBackgroundPulsed() -- Used to Pulse Flash the background of the 
		// static control. For example, you could flash the background 4 quick times 
		// and then hold in the "on" state for a longer period of time then flash 4 
		// quick times again and so on.
		// 
		// bFlash        --  TRUE if you want to flash it. FALSE to stop it.
		// pulseDuration --  Interval of flash in seconds at which you wish to 
		//					 flash the pulsed sequence.  
		//                   If you want to pulse-flash at a rate of say once every 
		//					 half-second, set duration to 0.5.
		// pulseRep      --  The number of times (repititions) at which you want 
		//					 the pulse flash to occur.
		// onDuration    --  The number of seconds the control is to remain in 
		//					 the "on" state before repeating the pulse sequence.
		// bFinalState   --  When turning the flash off (that is, when bFlash is FALSE), 
		//                   this flag forces the background to be left in the desired 
		//                   "on" (TRUE) or "off" (FALSE) state.  The default is to 
		//					 leave it "on".  This also forces the text to be in the 
		//					 same state, keeping the on/off state of the text in 
		//					 sync with the on/off state of the background.
		//
		// The color specified by a call to SetBackgroundOnColor() is used as the 
		// "on" state color.  The "off" state color is set by a call to 
		// SetBackgroundOffColor().  Returns TRUE if successful.
		//
		// 
		// Flashing the background does NOT affect the hatch color if the control 
		// is been set to Hatch Mode by a call to the SetHatch() method.  The 
		// hatch pattern will ALWAYS utilize the background's ON color.  When the 
		// background flashes, as the background changes to the ON state, it will 
		// appear as if the hatch pattern vanishes and then reappears as the control's 
		// background transitions to the OFF state.
		//
	BOOL FlashBackgroundPulsed(const BOOL bFlash, 
							   const BOOL bFinalState = TRUE,
		                       const float pulseDuration = 0.5, 
							   const int pulseRep = 4, 
							   const float onDuration = 2.0);
	
		// FlashText() -- Flashes the text between the text's ON color and its OFF color.
		// You could make the OFF color of the text the same as the currently displayed
		// color of the background (typically the ON color) and the text would flash
		// on/off screen.
		// Returns TRUE if successful.  Note that, unlike the bFinalState parameter 
		// used by the FlashBackground() and FlashBackgroundAsym() methods, the
		// value of bFinaState here applies ONLY to the final state of the text.
		// That is, the text could be left in the "ON" state while the background is
		// in the "OFF" state if that is what is desired.  
		//
		// Flashing of the text has no affect on the background color or the background's
		// hatch pattern if the control was set to Hatch Mode by a call to the 
		// SetHatch() method.
		//
	BOOL FlashText(const BOOL bFlash, 
				   const BOOL bFinalState = FALSE,
		           const float duration = 1.0);			// Default 1 second.


		// GetStates() -- Gets the current state of the background and the text.
		//                If bBackgroundState == TRUE, the background color is set to its
		//				  "ON" or "SET" color state.
		//				  If bTextState == TRUE, the text color is set to its
		//				  "ON" or "SET" color state.
		// 
		//                Note that since the FlashText() method allows the
		//                client to get the on/off state of the text color
		//                out of sync with the background color, it is possible for
		//                GetStates() to return different states for the background and
		//                the text if FlashText() has been in use.
		//				  The client can call SetOn() with the appropriate flag to
		//                resync the on/off states of the text and background colors.
		//				  Further, calling FlashBackground(), FlashBackgroundAsym(),
		//				  or FlashBackgroundPulsed() will also resync the background
		//				  and text color states.
		//
	void GetStates(BOOL& bBackgroundState, BOOL& bTextState) const;


		// SetBorder() -- If bBorder == TRUE, display a border.  If == FALSE, do not.
		// Returns TRUE if successful.
		//
	BOOL SetBorder(const BOOL bBorder = TRUE);

		// SetSunken() -- If bSunken == TRUE, display as sunken.  If == FALSE, do not.
		// Returns TRUE if successful.
		//
	BOOL SetSunken(const BOOL bSunken = TRUE);

		// SetModalFrame() -- if bModal == TRUE, display modal frame style.  
		// If == FALSE, do not.
		// Returns TRUE if successful.
		//
	BOOL SetModalFrame(const BOOL bModal = TRUE);

		// SetColorFrame() -- Call to create a colored frame around the control.
		// I made the color frame mutually exclusive of the modal and sunken frames. 
		// They just do not look nice together.
		// When using the modal or sunken frames, the client area that is actually 
		// visible is decreased in size.  If you were to add the color frame with the 
		// modal or sunken frames, and draw the color frame so that it did not overlay 
		// the modal or sunken frame, the usable area would be even smaller.  
		// Therefore, as a design decision, I made the color frame border type 
		// mutually exclusive with the modal and sunken types.
		// Overlaying the color frame on the modal or sunken frame looks bad.
		//
		// The color frame is drawn inside the edges of the control.  If the color
		// frame were drawn along the edge of the control or outside the edge
		// of the control, the control take up more area on the screen.  If there 
		// are multiple indicators next to each other, you would have the color frame 
		// of one control being drawn on top of the adjoinging control.  Therefore, 
		// I paint the frame inside the edges of the control.  Remember this when 
		// selecting fonts because the color frame makes the area availble for 
		// displaying text a little smaller.
		//
	void SetColorFrame(const BOOL bOn = TRUE, 
		               const int Width = 2,			// Width of color frame in device units (pixels).
					   const COLORREF FrameColor = DEFAULT_COLOR_FRAME_COLOR);  // Color of the colored frame.

		// SetBold() -- Sets the text to boldface or normal.  Not, the font weight is
		// set to 400 (normal) if On = FALSE or to 700 (bold) if On = TRUE.
		//
	void SetBold(const BOOL On = TRUE);

		// SetItalic() -- Sets the text to italic.
		//
	void SetItalic(const BOOL On = TRUE);

		// SetUnderline() -- Sets the text to underline.
		//
	void SetUnderline(const BOOL On = TRUE);

		// SetPointFont() -- Allows the client to set the font for the control 
		// by specifying the point size and the font name.
		// Returns TRUE if successful.
		//
	BOOL SetPointFont(const int PointSize, const LPCTSTR fontname);
	
		// SetFontIndirect() sets the static control to use the specified font.
		// For a description of how to use the input parameter, refer to the 
		// MFC documentation for CFont::CreateFontIndirect().
		// Returns TRUE if successful.
		//
	BOOL SetFontIndirect(const LOGFONT* lpLogFont);

		// SetPointFontIndirect() sets the static control to use the specified font.
		// For a description of how to use the input parameter, refer to the MFC
		// documentation for CFont::CreatePointFontIndirect().
		// Returns TRUE if successful.
		//
	BOOL SetPointFontIndirect(const LOGFONT* lpLogFont);

	LOGFONT GetLogFont() const;

		// SetFontWeight() -- More control that SetBold().  Use to set the font weight
		// from 1 to 1000.  The actual affect of this call depends on the font in use.
		// Returns TRUE if successful.
		//
	BOOL SetFontWeight(const int Weight);



	virtual ~CGCColorStatic();

	// Generated message map functions
protected:
	afx_msg LRESULT OnDrawFrame(WPARAM wp, LPARAM lp);
	//{{AFX_MSG(CGCColorStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_uiTimerID;

	BOOL m_bHatchMode;						// Is control in Hatch Mode?
	int m_iHatchType;						// Type of hatch pattern if m_bHatchMode == TRUE.

	BOOL m_bTextOn;							// Is text in ON state?
	BOOL m_bBackgroundOn;					// Is background in ON state?

	COLORREF m_CurrentBackgroundOnColor;	// Background's ON state color.
	COLORREF m_CurrentBackgroundOffColor;	// Background's OFF state color.

	CBrush m_brOnBrush;						// Background's ON/SET brush.
	CBrush m_brOffBrush;					// Background's OFF/UNSET brush.

	COLORREF m_CurrentTextOnColor;			// Text's ON/SET state color.
	COLORREF m_CurrentTextOffColor;			// Text's OFF/UNSET state color.

	BOOL m_bTimerFlag;						// Is a flash timer running?

	CFont m_Font;							// The font to use in the control.

	FLASH_TYPE m_FlashTypeFlag;				// Are we doing Symmetrical Background Flashing,
											// Asymmetrical Background Flashing, Text Flashing
											// or neither?

	// Variables used for Asymmetrical Background Flashing

	float m_fOnDuration;					// "ON" duration time for Asymmetrical Flashing. milliseconds.
	float m_fOffDuration;					// "OFF" duration time for Asymmetrical Flashing. milliseconds.

	// Variables used for Pulsed Background Flashing

	int m_pulseCounter;						// Current pulse of the pulse sequence.
	float m_pulseOnDuration;				// Duration in milliseconds of each pulse flash (on or off state)
	int m_pulseRep;							// The number of times the control is to pulse flash.
	float m_pulseDuration;					// The time in milliseconds that the stead "on" condition
											// is to remain between the pulsed sequences.

	int m_ColorFrameWidth;					// Width in device units of the color frame when used.
	COLORREF m_ColorFrameColor;				// Color to use when drawing a color frame.
	BOOL m_bColorFrameFlag;					// Flag indicating if a color frame is to be displayed.

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__CA34AF76_8DFF_11D3_A770_00105AA7390A__INCLUDED_)
