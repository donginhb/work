// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "s_msppt15.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Application15 properties

/////////////////////////////////////////////////////////////////////////////
// _Application15 operations

LPDISPATCH _Application15::GetPresentations()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetActivePresentation()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetSlideShowWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application15::GetPath()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application15::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application15::GetCaption()
{
	CString result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application15::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Application15::GetBuild()
{
	CString result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application15::GetVersion()
{
	CString result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application15::GetOperatingSystem()
{
	CString result;
	InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application15::GetActivePrinter()
{
	CString result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Application15::GetCreator()
{
	long result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetVbe()
{
	LPDISPATCH result;
	InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Application15::Help(LPCTSTR HelpFile, long ContextID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 HelpFile, ContextID);
}

void _Application15::Quit()
{
	InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

float _Application15::GetLeft()
{
	float result;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application15::SetLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application15::GetTop()
{
	float result;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application15::SetTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application15::GetWidth()
{
	float result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application15::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application15::GetHeight()
{
	float result;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application15::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Application15::GetWindowState()
{
	long result;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application15::GetVisible()
{
	long result;
	InvokeHelper(0x7ee, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application15::GetActive()
{
	long result;
	InvokeHelper(0x7f0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::Activate()
{
	InvokeHelper(0x7f1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Application15::GetCOMAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application15::GetProductCode()
{
	CString result;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetLanguageSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application15::GetShowWindowsInTaskbar()
{
	long result;
	InvokeHelper(0x7f8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetShowWindowsInTaskbar(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application15::GetFeatureInstall()
{
	long result;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetFeatureInstall(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Application15::GetFileDialog(long Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Type);
	return result;
}

long _Application15::GetDisplayGridLines()
{
	long result;
	InvokeHelper(0x7fe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetDisplayGridLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application15::GetAutomationSecurity()
{
	long result;
	InvokeHelper(0x7ff, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetAutomationSecurity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ff, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Application15::GetNewPresentation()
{
	LPDISPATCH result;
	InvokeHelper(0x800, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application15::GetDisplayAlerts()
{
	long result;
	InvokeHelper(0x801, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetDisplayAlerts(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x801, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application15::GetShowStartupDialog()
{
	long result;
	InvokeHelper(0x802, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetShowStartupDialog(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Application15::GetAutoCorrect()
{
	LPDISPATCH result;
	InvokeHelper(0x804, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x805, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Application15::GetDisplayDocumentInformationPanel()
{
	BOOL result;
	InvokeHelper(0x808, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application15::SetDisplayDocumentInformationPanel(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x808, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Application15::GetAssistance()
{
	LPDISPATCH result;
	InvokeHelper(0x809, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application15::GetActiveEncryptionSession()
{
	long result;
	InvokeHelper(0x80a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetFileConverters()
{
	LPDISPATCH result;
	InvokeHelper(0x80b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetSmartArtLayouts()
{
	LPDISPATCH result;
	InvokeHelper(0x80c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetSmartArtQuickStyles()
{
	LPDISPATCH result;
	InvokeHelper(0x80d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetSmartArtColors()
{
	LPDISPATCH result;
	InvokeHelper(0x80e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetProtectedViewWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x80f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application15::GetActiveProtectedViewWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x810, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Application15::GetIsSandboxed()
{
	BOOL result;
	InvokeHelper(0x811, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application15::StartNewUndoEntry()
{
	InvokeHelper(0x813, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Application15::GetFileValidation()
{
	long result;
	InvokeHelper(0x814, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetFileValidation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x814, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Application15::OpenThemeFile(LPCTSTR themeFileName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x815, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		themeFileName);
	return result;
}

BOOL _Application15::GetChartDataPointTrack()
{
	BOOL result;
	InvokeHelper(0x816, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application15::SetChartDataPointTrack(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x816, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Application15::GetDisplayGuides()
{
	long result;
	InvokeHelper(0x817, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application15::SetDisplayGuides(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x817, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow operations

LPDISPATCH SlideShowWindow15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow15::GetView()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow15::GetPresentation()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowWindow15::GetIsFullScreen()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float SlideShowWindow15::GetLeft()
{
	float result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow15::SetLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow15::GetTop()
{
	float result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow15::SetTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow15::GetWidth()
{
	float result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow15::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow15::GetHeight()
{
	float result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow15::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long SlideShowWindow15::GetActive()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow15::Activate()
{
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH SlideShowWindow15::GetSlideNavigation()
{
	LPDISPATCH result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowWindows properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowWindows operations

long SlideShowWindows15::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindows15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindows15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindows15::Item(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowView properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowView operations

LPDISPATCH SlideShowView15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView15::GetZoom()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView15::GetSlide()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView15::GetPointerType()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView15::SetPointerType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowView15::GetState()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView15::SetState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowView15::GetAcceleratorsEnabled()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView15::SetAcceleratorsEnabled(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float SlideShowView15::GetPresentationElapsedTime()
{
	float result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float SlideShowView15::GetSlideElapsedTime()
{
	float result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowView15::SetSlideElapsedTime(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH SlideShowView15::GetLastSlideViewed()
{
	LPDISPATCH result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView15::GetAdvanceMode()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView15::GetPointerColor()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView15::GetIsNamedShow()
{
	long result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString SlideShowView15::GetSlideShowName()
{
	CString result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void SlideShowView15::DrawLine(float BeginX, float BeginY, float EndX, float EndY)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 BeginX, BeginY, EndX, EndY);
}

void SlideShowView15::EraseDrawing()
{
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::First()
{
	InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::Last()
{
	InvokeHelper(0x7e2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::Next()
{
	InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::Previous()
{
	InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::GotoSlide(long Index, long ResetSlide)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, ResetSlide);
}

void SlideShowView15::GotoNamedShow(LPCTSTR SlideShowName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SlideShowName);
}

void SlideShowView15::EndNamedShow()
{
	InvokeHelper(0x7e7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::ResetSlideTime()
{
	InvokeHelper(0x7e8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView15::Exit()
{
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SlideShowView15::GetCurrentShowPosition()
{
	long result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView15::GotoClick(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

long SlideShowView15::GetClickIndex()
{
	long result;
	InvokeHelper(0x7ed, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long SlideShowView15::GetClickCount()
{
	long result;
	InvokeHelper(0x7ee, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL SlideShowView15::FirstAnimationIsAutomatic()
{
	BOOL result;
	InvokeHelper(0x7ef, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView15::Player(const VARIANT& ShapeId)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x7f0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&ShapeId);
	return result;
}

long SlideShowView15::GetMediaControlsVisible()
{
	long result;
	InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float SlideShowView15::GetMediaControlsLeft()
{
	float result;
	InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float SlideShowView15::GetMediaControlsTop()
{
	float result;
	InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float SlideShowView15::GetMediaControlsWidth()
{
	float result;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float SlideShowView15::GetMediaControlsHeight()
{
	float result;
	InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings operations

LPDISPATCH SlideShowSettings15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings15::GetPointerColor()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings15::GetNamedSlideShows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowSettings15::GetStartingSlide()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetStartingSlide(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetEndingSlide()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetEndingSlide(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetAdvanceMode()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetAdvanceMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH SlideShowSettings15::Run()
{
	LPDISPATCH result;
	InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowSettings15::GetLoopUntilStopped()
{
	long result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetLoopUntilStopped(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowType()
{
	long result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowWithNarration()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowWithNarration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowWithAnimation()
{
	long result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowWithAnimation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString SlideShowSettings15::GetSlideShowName()
{
	CString result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetSlideShowName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long SlideShowSettings15::GetRangeType()
{
	long result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetRangeType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowScrollbar()
{
	long result;
	InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowScrollbar(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7df, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowPresenterView()
{
	long result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowPresenterView(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings15::GetShowMediaControls()
{
	long result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings15::SetShowMediaControls(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// Presentations properties

/////////////////////////////////////////////////////////////////////////////
// Presentations operations

long Presentations15::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations15::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Presentations15::Add(long WithWindow)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		WithWindow);
	return result;
}

LPDISPATCH Presentations15::Open(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, ReadOnly, Untitled, WithWindow);
	return result;
}

void Presentations15::CheckOut(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

BOOL Presentations15::CanCheckOut(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

LPDISPATCH Presentations15::Open2007(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow, long OpenAndRepair)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, ReadOnly, Untitled, WithWindow, OpenAndRepair);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// PageSetup properties

/////////////////////////////////////////////////////////////////////////////
// PageSetup operations

LPDISPATCH PageSetup15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PageSetup15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PageSetup15::GetFirstSlideNumber()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetFirstSlideNumber(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float PageSetup15::GetSlideHeight()
{
	float result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetSlideHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float PageSetup15::GetSlideWidth()
{
	float result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetSlideWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long PageSetup15::GetSlideSize()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetSlideSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PageSetup15::GetNotesOrientation()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetNotesOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PageSetup15::GetSlideOrientation()
{
	long result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup15::SetSlideOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// Slides properties

/////////////////////////////////////////////////////////////////////////////
// Slides operations

long Slides15::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides15::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Slides15::FindBySlideID(long SlideID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		SlideID);
	return result;
}

long Slides15::InsertFromFile(LPCTSTR FileName, long Index, long SlideStart, long SlideEnd)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FileName, Index, SlideStart, SlideEnd);
	return result;
}

LPDISPATCH Slides15::Range(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x7d6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Slides15::Paste(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Slides15::AddSlide(long Index, LPDISPATCH pCustomLayout)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index, pCustomLayout);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Slide properties

/////////////////////////////////////////////////////////////////////////////
// _Slide operations

LPDISPATCH _Slide15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetHeadersFooters()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetSlideShowTransition()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetColorScheme()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::SetColorScheme(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Slide15::GetBackground()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Slide15::GetName()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Slide15::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Slide15::GetSlideID()
{
	long result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide15::GetPrintSteps()
{
	long result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::Select()
{
	InvokeHelper(0x7db, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Slide15::Cut()
{
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Slide15::Copy()
{
	InvokeHelper(0x7dd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Slide15::GetLayout()
{
	long result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::SetLayout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Slide15::Duplicate()
{
	LPDISPATCH result;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::Delete()
{
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Slide15::GetTags()
{
	LPDISPATCH result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Slide15::GetSlideIndex()
{
	long result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide15::GetSlideNumber()
{
	long result;
	InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide15::GetDisplayMasterShapes()
{
	long result;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::SetDisplayMasterShapes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Slide15::GetFollowMasterBackground()
{
	long result;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::SetFollowMasterBackground(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Slide15::GetNotesPage()
{
	LPDISPATCH result;
	InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::Export(LPCTSTR FileName, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FilterName, ScaleWidth, ScaleHeight);
}

LPDISPATCH _Slide15::GetComments()
{
	LPDISPATCH result;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide15::GetDesign()
{
	LPDISPATCH result;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::SetDesign(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _Slide15::MoveTo(long toPos)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 toPos);
}

LPDISPATCH _Slide15::GetTimeLine()
{
	LPDISPATCH result;
	InvokeHelper(0x7ef, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::ApplyTemplate(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7f0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

LPDISPATCH _Slide15::GetCustomLayout()
{
	LPDISPATCH result;
	InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::SetCustomLayout(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7f2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _Slide15::ApplyTheme(LPCTSTR themeName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7f3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 themeName);
}

LPDISPATCH _Slide15::GetThemeColorScheme()
{
	LPDISPATCH result;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::ApplyThemeColorScheme(LPCTSTR themeColorSchemeName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7f5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 themeColorSchemeName);
}

long _Slide15::GetBackgroundStyle()
{
	long result;
	InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::SetBackgroundStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Slide15::GetCustomerData()
{
	LPDISPATCH result;
	InvokeHelper(0x7f7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide15::PublishSlides(LPCTSTR SlideLibraryUrl, BOOL Overwrite, BOOL UseSlideOrder)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x7f8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SlideLibraryUrl, Overwrite, UseSlideOrder);
}

void _Slide15::MoveToSectionStart(long toSection)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 toSection);
}

long _Slide15::GetSectionIndex()
{
	long result;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide15::GetHasNotesPage()
{
	long result;
	InvokeHelper(0x7fb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide15::ApplyTemplate2(LPCTSTR FileName, LPCTSTR VariantGUID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x7fc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, VariantGUID);
}


/////////////////////////////////////////////////////////////////////////////
// Shapes properties

/////////////////////////////////////////////////////////////////////////////
// Shapes operations

LPDISPATCH Shapes15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shapes15::GetCreator()
{
	long result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shapes15::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPUNKNOWN Shapes15::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::AddCallout(long Type, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddConnector(long Type, float BeginX, float BeginY, float EndX, float EndY)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, BeginX, BeginY, EndX, EndY);
	return result;
}

LPDISPATCH Shapes15::AddCurve(const VARIANT& SafeArrayOfPoints)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&SafeArrayOfPoints);
	return result;
}

LPDISPATCH Shapes15::AddLabel(long Orientation, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Orientation, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddLine(float BeginX, float BeginY, float EndX, float EndY)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		BeginX, BeginY, EndX, EndY);
	return result;
}

LPDISPATCH Shapes15::AddPicture(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, LinkToFile, SaveWithDocument, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddPolyline(const VARIANT& SafeArrayOfPoints)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&SafeArrayOfPoints);
	return result;
}

LPDISPATCH Shapes15::AddShape(long Type, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddTextEffect(long PresetTextEffect, LPCTSTR Text, LPCTSTR FontName, float FontSize, long FontBold, long FontItalic, float Left, float Top)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_BSTR VTS_R4 VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PresetTextEffect, Text, FontName, FontSize, FontBold, FontItalic, Left, Top);
	return result;
}

LPDISPATCH Shapes15::AddTextbox(long Orientation, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Orientation, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::BuildFreeform(long EditingType, float X1, float Y1)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EditingType, X1, Y1);
	return result;
}

void Shapes15::SelectAll()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Shapes15::Range(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

long Shapes15::GetHasTitle()
{
	long result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::AddTitle()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::GetTitle()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::GetPlaceholders()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::AddOLEObject(float Left, float Top, float Width, float Height, LPCTSTR ClassName, LPCTSTR FileName, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Width, Height, ClassName, FileName, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Link);
	return result;
}

LPDISPATCH Shapes15::AddPlaceholder(long Type, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7da, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::Paste()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes15::AddTable(long NumRows, long NumColumns, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7dd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		NumRows, NumColumns, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::PasteSpecial(long DataType, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4;
	InvokeHelper(0x7de, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DataType, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Link);
	return result;
}

LPDISPATCH Shapes15::AddMediaObject2(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, LinkToFile, SaveWithDocument, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddMediaObjectFromEmbedTag(LPCTSTR EmbedTag, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EmbedTag, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddSmartArt(LPDISPATCH Layout, float Left, float Top, float Width, float Height)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Layout, Left, Top, Width, Height);
	return result;
}

LPDISPATCH Shapes15::AddChart2(long Style, long Type, float Left, float Top, float Width, float Height, BOOL NewLayout)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Style, Type, Left, Top, Width, Height, NewLayout);
	return result;
}

LPDISPATCH Shapes15::AddPicture2(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height, long compress)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, LinkToFile, SaveWithDocument, Left, Top, Width, Height, compress);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Shape properties

/////////////////////////////////////////////////////////////////////////////
// Shape operations

LPDISPATCH Shape15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetCreator()
{
	long result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Shape15::Apply()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::Delete()
{
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::Flip(long FlipCmd)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FlipCmd);
}

void Shape15::IncrementLeft(float Increment)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Increment);
}

void Shape15::IncrementRotation(float Increment)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Increment);
}

void Shape15::IncrementTop(float Increment)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Increment);
}

void Shape15::PickUp()
{
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::RerouteConnections()
{
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::ScaleHeight(float Factor, long RelativeToOriginalSize, long fScale)
{
	static BYTE parms[] =
		VTS_R4 VTS_I4 VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Factor, RelativeToOriginalSize, fScale);
}

void Shape15::ScaleWidth(float Factor, long RelativeToOriginalSize, long fScale)
{
	static BYTE parms[] =
		VTS_R4 VTS_I4 VTS_I4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Factor, RelativeToOriginalSize, fScale);
}

void Shape15::SetShapesDefaultProperties()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Shape15::Ungroup()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Shape15::ZOrder(long ZOrderCmd)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ZOrderCmd);
}

LPDISPATCH Shape15::GetAdjustments()
{
	LPDISPATCH result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetAutoShapeType()
{
	long result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetAutoShapeType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Shape15::GetBlackWhiteMode()
{
	long result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetBlackWhiteMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Shape15::GetCallout()
{
	LPDISPATCH result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetConnectionSiteCount()
{
	long result;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Shape15::GetConnector()
{
	long result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetConnectorFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetFill()
{
	LPDISPATCH result;
	InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetGroupItems()
{
	LPDISPATCH result;
	InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

float Shape15::GetHeight()
{
	float result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Shape15::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x6d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Shape15::GetHorizontalFlip()
{
	long result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float Shape15::GetLeft()
{
	float result;
	InvokeHelper(0x6f, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Shape15::SetLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x6f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Shape15::GetLine()
{
	LPDISPATCH result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetLockAspectRatio()
{
	long result;
	InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetLockAspectRatio(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x71, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString Shape15::GetName()
{
	CString result;
	InvokeHelper(0x73, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Shape15::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x73, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH Shape15::GetNodes()
{
	LPDISPATCH result;
	InvokeHelper(0x74, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

float Shape15::GetRotation()
{
	float result;
	InvokeHelper(0x75, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Shape15::SetRotation(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x75, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Shape15::GetPictureFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetShadow()
{
	LPDISPATCH result;
	InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetTextEffect()
{
	LPDISPATCH result;
	InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetTextFrame()
{
	LPDISPATCH result;
	InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetThreeD()
{
	LPDISPATCH result;
	InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

float Shape15::GetTop()
{
	float result;
	InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Shape15::SetTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Shape15::GetType()
{
	long result;
	InvokeHelper(0x7c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Shape15::GetVerticalFlip()
{
	long result;
	InvokeHelper(0x7d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT Shape15::GetVertices()
{
	VARIANT result;
	InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

long Shape15::GetVisible()
{
	long result;
	InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float Shape15::GetWidth()
{
	float result;
	InvokeHelper(0x80, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Shape15::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x80, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Shape15::GetZOrderPosition()
{
	long result;
	InvokeHelper(0x81, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetOLEFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetLinkFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetPlaceholderFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetAnimationSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetActionSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetTags()
{
	LPDISPATCH result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Shape15::Cut()
{
	InvokeHelper(0x7d9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::Copy()
{
	InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::Select(long Replace)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7db, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Replace);
}

LPDISPATCH Shape15::Duplicate()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetMediaType()
{
	long result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Shape15::GetHasTextFrame()
{
	long result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Shape15::GetAlternativeText()
{
	CString result;
	InvokeHelper(0x83, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Shape15::SetAlternativeText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x83, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long Shape15::GetHasTable()
{
	long result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetTable()
{
	LPDISPATCH result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetChild()
{
	long result;
	InvokeHelper(0x88, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetParentGroup()
{
	LPDISPATCH result;
	InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetId()
{
	long result;
	InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetCustomerData()
{
	LPDISPATCH result;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetTextFrame2()
{
	LPDISPATCH result;
	InvokeHelper(0x91, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetHasChart()
{
	long result;
	InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Shape15::GetShapeStyle()
{
	long result;
	InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetShapeStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x96, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Shape15::GetBackgroundStyle()
{
	long result;
	InvokeHelper(0x97, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Shape15::SetBackgroundStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x97, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Shape15::GetSoftEdge()
{
	LPDISPATCH result;
	InvokeHelper(0x98, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetGlow()
{
	LPDISPATCH result;
	InvokeHelper(0x99, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetReflection()
{
	LPDISPATCH result;
	InvokeHelper(0x9a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetChart()
{
	LPDISPATCH result;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shape15::GetHasSmartArt()
{
	long result;
	InvokeHelper(0x9b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shape15::GetSmartArt()
{
	LPDISPATCH result;
	InvokeHelper(0x9c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Shape15::ConvertTextToSmartArt(LPDISPATCH Layout)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x9d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Layout);
}

CString Shape15::GetTitle()
{
	CString result;
	InvokeHelper(0x9e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Shape15::SetTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH Shape15::GetMediaFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Shape15::PickupAnimation()
{
	InvokeHelper(0x7e7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::ApplyAnimation()
{
	InvokeHelper(0x7e8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape15::UpgradeMedia()
{
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// TextFrame properties

/////////////////////////////////////////////////////////////////////////////
// TextFrame operations

LPDISPATCH TextFrame15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long TextFrame15::GetCreator()
{
	long result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextFrame15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

float TextFrame15::GetMarginBottom()
{
	float result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetMarginBottom(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float TextFrame15::GetMarginLeft()
{
	float result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetMarginLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float TextFrame15::GetMarginRight()
{
	float result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetMarginRight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float TextFrame15::GetMarginTop()
{
	float result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetMarginTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long TextFrame15::GetOrientation()
{
	long result;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long TextFrame15::GetHasText()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextFrame15::GetTextRange()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextFrame15::GetRuler()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long TextFrame15::GetHorizontalAnchor()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetHorizontalAnchor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long TextFrame15::GetVerticalAnchor()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetVerticalAnchor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long TextFrame15::GetAutoSize()
{
	long result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetAutoSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long TextFrame15::GetWordWrap()
{
	long result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextFrame15::SetWordWrap(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void TextFrame15::DeleteText()
{
	InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// TextRange properties

/////////////////////////////////////////////////////////////////////////////
// TextRange operations

long TextRange15::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::GetActionSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long TextRange15::GetStart()
{
	long result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long TextRange15::GetLength()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float TextRange15::GetBoundLeft()
{
	float result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float TextRange15::GetBoundTop()
{
	float result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float TextRange15::GetBoundWidth()
{
	float result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float TextRange15::GetBoundHeight()
{
	float result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::Paragraphs(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7da, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::Sentences(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7db, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::Words(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::Characters(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7dd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::Lines(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7de, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::Runs(long Start, long Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, Length);
	return result;
}

LPDISPATCH TextRange15::TrimText()
{
	LPDISPATCH result;
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString TextRange15::GetText()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void TextRange15::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH TextRange15::InsertAfter(LPCTSTR NewText)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7e1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		NewText);
	return result;
}

LPDISPATCH TextRange15::InsertBefore(LPCTSTR NewText)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7e2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		NewText);
	return result;
}

LPDISPATCH TextRange15::InsertDateTime(long DateTimeFormat, long InsertAsField)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7e3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DateTimeFormat, InsertAsField);
	return result;
}

LPDISPATCH TextRange15::InsertSlideNumber()
{
	LPDISPATCH result;
	InvokeHelper(0x7e4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::InsertSymbol(LPCTSTR FontName, long CharNumber, long Unicode)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7e5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FontName, CharNumber, Unicode);
	return result;
}

LPDISPATCH TextRange15::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH TextRange15::GetParagraphFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long TextRange15::GetIndentLevel()
{
	long result;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextRange15::SetIndentLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void TextRange15::Select()
{
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextRange15::Cut()
{
	InvokeHelper(0x7ea, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextRange15::Copy()
{
	InvokeHelper(0x7eb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextRange15::Delete()
{
	InvokeHelper(0x7ec, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH TextRange15::Paste()
{
	LPDISPATCH result;
	InvokeHelper(0x7ed, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void TextRange15::ChangeCase(long Type)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type);
}

void TextRange15::AddPeriods()
{
	InvokeHelper(0x7ef, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextRange15::RemovePeriods()
{
	InvokeHelper(0x7f0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH TextRange15::Find(LPCTSTR FindWhat, long After, long MatchCase, long WholeWords)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7f1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FindWhat, After, MatchCase, WholeWords);
	return result;
}

LPDISPATCH TextRange15::Replace(LPCTSTR FindWhat, LPCTSTR ReplaceWhat, long After, long MatchCase, long WholeWords)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7f2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FindWhat, ReplaceWhat, After, MatchCase, WholeWords);
	return result;
}

void TextRange15::RotatedBounds(float* X1, float* Y1, float* X2, float* Y2, float* X3, float* Y3, float* x4, float* y4)
{
	static BYTE parms[] =
		VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4;
	InvokeHelper(0x7f3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X1, Y1, X2, Y2, X3, Y3, x4, y4);
}

long TextRange15::GetLanguageID()
{
	long result;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void TextRange15::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void TextRange15::RtlRun()
{
	InvokeHelper(0x7f5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextRange15::LtrRun()
{
	InvokeHelper(0x7f6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH TextRange15::PasteSpecial(long DataType, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4;
	InvokeHelper(0x7f7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DataType, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Link);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Presentation properties

/////////////////////////////////////////////////////////////////////////////
// _Presentation operations

LPDISPATCH _Presentation15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSlideMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetTitleMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetHasTitleMaster()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::AddTitleMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::ApplyTemplate(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

CString _Presentation15::GetTemplateName()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetNotesMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetHandoutMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSlides()
{
	LPDISPATCH result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetColorSchemes()
{
	LPDISPATCH result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetExtraColors()
{
	LPDISPATCH result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSlideShowSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetFonts()
{
	LPDISPATCH result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetTags()
{
	LPDISPATCH result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetDefaultShape()
{
	LPDISPATCH result;
	InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetBuiltInDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetCustomDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetVBProject()
{
	LPDISPATCH result;
	InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetReadOnly()
{
	long result;
	InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Presentation15::GetFullName()
{
	CString result;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Presentation15::GetName()
{
	CString result;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Presentation15::GetPath()
{
	CString result;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetSaved()
{
	long result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetSaved(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation15::GetLayoutDirection()
{
	long result;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetLayoutDirection(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Presentation15::NewWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7ed, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::FollowHyperlink(LPCTSTR Address, LPCTSTR SubAddress, BOOL NewWindow, BOOL AddHistory, LPCTSTR ExtraInfo, long Method, LPCTSTR HeaderInfo)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(0x7ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Address, SubAddress, NewWindow, AddHistory, ExtraInfo, Method, HeaderInfo);
}

void _Presentation15::AddToFavorites()
{
	InvokeHelper(0x7ef, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation15::GetPrintOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::PrintOut(long From, long To, LPCTSTR PrintToFile, long Copies, long Collate)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 From, To, PrintToFile, Copies, Collate);
}

void _Presentation15::Save()
{
	InvokeHelper(0x7f3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Presentation15::SaveAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileFormat, EmbedTrueTypeFonts);
}

void _Presentation15::SaveCopyAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileFormat, EmbedTrueTypeFonts);
}

void _Presentation15::Export(LPCTSTR Path, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, FilterName, ScaleWidth, ScaleHeight);
}

void _Presentation15::Close()
{
	InvokeHelper(0x7f7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation15::GetContainer()
{
	LPDISPATCH result;
	InvokeHelper(0x7f9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetDisplayComments()
{
	long result;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetDisplayComments(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation15::GetFarEastLineBreakLevel()
{
	long result;
	InvokeHelper(0x7fb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetFarEastLineBreakLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Presentation15::GetNoLineBreakBefore()
{
	CString result;
	InvokeHelper(0x7fc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetNoLineBreakBefore(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7fc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Presentation15::GetNoLineBreakAfter()
{
	CString result;
	InvokeHelper(0x7fd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetNoLineBreakAfter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7fd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Presentation15::UpdateLinks()
{
	InvokeHelper(0x7fe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation15::GetSlideShowWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7ff, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetFarEastLineBreakLanguage()
{
	long result;
	InvokeHelper(0x800, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetFarEastLineBreakLanguage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x800, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation15::GetDefaultLanguageID()
{
	long result;
	InvokeHelper(0x802, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetDefaultLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Presentation15::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x803, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetEnvelopeVisible()
{
	long result;
	InvokeHelper(0x809, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetEnvelopeVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x809, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation15::GetVBASigned()
{
	long result;
	InvokeHelper(0x80b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetSnapToGrid()
{
	long result;
	InvokeHelper(0x80d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetSnapToGrid(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x80d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float _Presentation15::GetGridDistance()
{
	float result;
	InvokeHelper(0x80e, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetGridDistance(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x80e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Presentation15::GetDesigns()
{
	LPDISPATCH result;
	InvokeHelper(0x80f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::Merge(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x810, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

void _Presentation15::CheckIn(BOOL SaveChanges, const VARIANT& Comments, const VARIANT& MakePublic)
{
	static BYTE parms[] =
		VTS_BOOL VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x811, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges, &Comments, &MakePublic);
}

BOOL _Presentation15::CanCheckIn()
{
	BOOL result;
	InvokeHelper(0x812, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSignatures()
{
	LPDISPATCH result;
	InvokeHelper(0x813, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetRemovePersonalInformation()
{
	long result;
	InvokeHelper(0x814, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetRemovePersonalInformation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x814, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Presentation15::EndReview()
{
	InvokeHelper(0x817, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _Presentation15::GetPasswordEncryptionProvider()
{
	CString result;
	InvokeHelper(0x81b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Presentation15::GetPasswordEncryptionAlgorithm()
{
	CString result;
	InvokeHelper(0x81c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Presentation15::GetPasswordEncryptionKeyLength()
{
	long result;
	InvokeHelper(0x81d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Presentation15::GetPasswordEncryptionFileProperties()
{
	BOOL result;
	InvokeHelper(0x81e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetPasswordEncryptionOptions(LPCTSTR PasswordEncryptionProvider, LPCTSTR PasswordEncryptionAlgorithm, long PasswordEncryptionKeyLength, BOOL PasswordEncryptionFileProperties)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL;
	InvokeHelper(0x81f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PasswordEncryptionProvider, PasswordEncryptionAlgorithm, PasswordEncryptionKeyLength, PasswordEncryptionFileProperties);
}

CString _Presentation15::GetPassword()
{
	CString result;
	InvokeHelper(0x820, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetPassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x820, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Presentation15::GetWritePassword()
{
	CString result;
	InvokeHelper(0x821, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetWritePassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x821, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Presentation15::GetPermission()
{
	LPDISPATCH result;
	InvokeHelper(0x822, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSharedWorkspace()
{
	LPDISPATCH result;
	InvokeHelper(0x823, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetSync()
{
	LPDISPATCH result;
	InvokeHelper(0x824, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::SendFaxOverInternet(LPCTSTR Recipients, LPCTSTR Subject, BOOL ShowMessage)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x825, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipients, Subject, ShowMessage);
}

LPDISPATCH _Presentation15::GetDocumentLibraryVersions()
{
	LPDISPATCH result;
	InvokeHelper(0x826, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetContentTypeProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x827, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::RemoveDocumentInformation(long Type)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x82e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type);
}

void _Presentation15::CheckInWithVersion(BOOL SaveChanges, const VARIANT& Comments, const VARIANT& MakePublic, const VARIANT& VersionType)
{
	static BYTE parms[] =
		VTS_BOOL VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x82f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges, &Comments, &MakePublic, &VersionType);
}

void _Presentation15::ExportAsFixedFormat(LPCTSTR Path, long FixedFormatType, long Intent, long FrameSlides, long HandoutOrder, long OutputType, long PrintHiddenSlides, LPDISPATCH PrintRange, long RangeType, LPCTSTR SlideShowName, BOOL IncludeDocProperties, 
		BOOL KeepIRMSettings, BOOL DocStructureTags, BOOL BitmapMissingFonts, BOOL UseISO19005_1, const VARIANT& ExternalExporter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_DISPATCH VTS_I4 VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_VARIANT;
	InvokeHelper(0x830, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, FixedFormatType, Intent, FrameSlides, HandoutOrder, OutputType, PrintHiddenSlides, PrintRange, RangeType, SlideShowName, IncludeDocProperties, KeepIRMSettings, DocStructureTags, BitmapMissingFonts, UseISO19005_1, &ExternalExporter);
}

LPDISPATCH _Presentation15::GetServerPolicy()
{
	LPDISPATCH result;
	InvokeHelper(0x831, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetWorkflowTasks()
{
	LPDISPATCH result;
	InvokeHelper(0x832, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetWorkflowTemplates()
{
	LPDISPATCH result;
	InvokeHelper(0x833, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::LockServerFile()
{
	InvokeHelper(0x834, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation15::GetDocumentInspectors()
{
	LPDISPATCH result;
	InvokeHelper(0x835, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Presentation15::GetHasVBProject()
{
	BOOL result;
	InvokeHelper(0x836, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetCustomXMLParts()
{
	LPDISPATCH result;
	InvokeHelper(0x837, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Presentation15::GetFinal()
{
	BOOL result;
	InvokeHelper(0x838, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetFinal(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x838, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Presentation15::ApplyTheme(LPCTSTR themeName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x839, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 themeName);
}

LPDISPATCH _Presentation15::GetCustomerData()
{
	LPDISPATCH result;
	InvokeHelper(0x83a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetResearch()
{
	LPDISPATCH result;
	InvokeHelper(0x83b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::PublishSlides(LPCTSTR SlideLibraryUrl, BOOL Overwrite, BOOL UseSlideOrder)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x83c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SlideLibraryUrl, Overwrite, UseSlideOrder);
}

CString _Presentation15::GetEncryptionProvider()
{
	CString result;
	InvokeHelper(0x83d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetEncryptionProvider(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x83d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Presentation15::GetSectionProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x83f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation15::GetCoauthoring()
{
	LPDISPATCH result;
	InvokeHelper(0x840, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation15::MergeWithBaseline(LPCTSTR withPresentation, LPCTSTR baselinePresentation)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x841, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 withPresentation, baselinePresentation);
}

BOOL _Presentation15::GetInMergeMode()
{
	BOOL result;
	InvokeHelper(0x842, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Presentation15::AcceptAll()
{
	InvokeHelper(0x843, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Presentation15::RejectAll()
{
	InvokeHelper(0x844, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Presentation15::EnsureAllMediaUpgraded()
{
	InvokeHelper(0x845, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation15::GetBroadcast()
{
	LPDISPATCH result;
	InvokeHelper(0x846, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Presentation15::GetHasNotesMaster()
{
	BOOL result;
	InvokeHelper(0x847, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Presentation15::GetHasHandoutMaster()
{
	BOOL result;
	InvokeHelper(0x848, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Presentation15::Convert2(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x849, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

long _Presentation15::GetCreateVideoStatus()
{
	long result;
	InvokeHelper(0x84a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation15::CreateVideo(LPCTSTR FileName, BOOL UseTimingsAndNarrations, long DefaultSlideDuration, long VertResolution, long FramesPerSecond, long Quality)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x84b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, UseTimingsAndNarrations, DefaultSlideDuration, VertResolution, FramesPerSecond, Quality);
}

void _Presentation15::ApplyTemplate2(LPCTSTR FileName, LPCTSTR VariantGUID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x84c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, VariantGUID);
}

BOOL _Presentation15::GetChartDataPointTrack()
{
	BOOL result;
	InvokeHelper(0x84d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Presentation15::SetChartDataPointTrack(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x84d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Presentation15::ExportAsFixedFormat2(LPCTSTR Path, long FixedFormatType, long Intent, long FrameSlides, long HandoutOrder, long OutputType, long PrintHiddenSlides, LPDISPATCH PrintRange, long RangeType, LPCTSTR SlideShowName, BOOL IncludeDocProperties, 
		BOOL KeepIRMSettings, BOOL DocStructureTags, BOOL BitmapMissingFonts, BOOL UseISO19005_1, BOOL IncludeMarkup, const VARIANT& ExternalExporter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_DISPATCH VTS_I4 VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_VARIANT;
	InvokeHelper(0x84e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, FixedFormatType, Intent, FrameSlides, HandoutOrder, OutputType, PrintHiddenSlides, PrintRange, RangeType, SlideShowName, IncludeDocProperties, KeepIRMSettings, DocStructureTags, BitmapMissingFonts, UseISO19005_1, IncludeMarkup, 
		&ExternalExporter);
}

LPDISPATCH _Presentation15::GetGuides()
{
	LPDISPATCH result;
	InvokeHelper(0x84f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// PublishObjects properties

/////////////////////////////////////////////////////////////////////////////
// PublishObjects operations

long PublishObjects15::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH PublishObjects15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PublishObjects15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PublishObjects15::Item(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// PublishObject properties

/////////////////////////////////////////////////////////////////////////////
// PublishObject operations

LPDISPATCH PublishObject15::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PublishObject15::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PublishObject15::GetHTMLVersion()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetHTMLVersion(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PublishObject15::GetSourceType()
{
	long result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetSourceType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PublishObject15::GetRangeStart()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetRangeStart(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PublishObject15::GetRangeEnd()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetRangeEnd(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString PublishObject15::GetSlideShowName()
{
	CString result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetSlideShowName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long PublishObject15::GetSpeakerNotes()
{
	long result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetSpeakerNotes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString PublishObject15::GetFileName()
{
	CString result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void PublishObject15::SetFileName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void PublishObject15::Publish()
{
	InvokeHelper(0x7da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
