// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// Collection wrapper class
//

#ifndef _PPT9
#define _PPT9

class _Application9 : public COleDispatchDriver
{
public:
	_Application9() {}		// Calls COleDispatchDriver default constructor
	_Application9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application9(const _Application9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetPresentations();
	LPDISPATCH GetWindows();
	LPDISPATCH GetActiveWindow();
	LPDISPATCH GetActivePresentation();
	LPDISPATCH GetSlideShowWindows();
	LPDISPATCH GetCommandBars();
	CString GetPath();
	CString GetName();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetAssistant();
	LPDISPATCH GetFileSearch();
	LPDISPATCH GetFileFind();
	CString GetBuild();
	CString GetVersion();
	CString GetOperatingSystem();
	CString GetActivePrinter();
	long GetCreator();
	LPDISPATCH GetAddIns();
	LPDISPATCH GetVbe();
	void Help(LPCTSTR HelpFile, long ContextID);
	void Quit();
	// method 'Run' not emitted because of invalid return type or parameter type
	float GetLeft();
	void SetLeft(float newValue);
	float GetTop();
	void SetTop(float newValue);
	float GetWidth();
	void SetWidth(float newValue);
	float GetHeight();
	void SetHeight(float newValue);
	long GetWindowState();
	void SetWindowState(long nNewValue);
	long GetVisible();
	void SetVisible(long nNewValue);
	long GetActive();
	void Activate();
	LPDISPATCH GetAnswerWizard();
	LPDISPATCH GetCOMAddIns();
	CString GetProductCode();
	LPDISPATCH GetDefaultWebOptions();
	LPDISPATCH GetLanguageSettings();
	long GetShowWindowsInTaskbar();
	void SetShowWindowsInTaskbar(long nNewValue);
	long GetFeatureInstall();
	void SetFeatureInstall(long nNewValue);
};

/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow wrapper class

class SlideShowWindow9 : public COleDispatchDriver
{
public:
	SlideShowWindow9() {}		// Calls COleDispatchDriver default constructor
	SlideShowWindow9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowWindow9(const SlideShowWindow9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetView();
	LPDISPATCH GetPresentation();
	long GetIsFullScreen();
	float GetLeft();
	void SetLeft(float newValue);
	float GetTop();
	void SetTop(float newValue);
	float GetWidth();
	void SetWidth(float newValue);
	float GetHeight();
	void SetHeight(float newValue);
	long GetActive();
	void Activate();
};

/////////////////////////////////////////////////////////////////////////////
// SlideShowWindows wrapper class

class SlideShowWindows9 : public COleDispatchDriver
{
public:
	SlideShowWindows9() {}		// Calls COleDispatchDriver default constructor
	SlideShowWindows9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowWindows9(const SlideShowWindows9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(long index);
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowView wrapper class

class SlideShowView9 : public COleDispatchDriver
{
public:
	SlideShowView9() {}		// Calls COleDispatchDriver default constructor
	SlideShowView9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowView9(const SlideShowView9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	long GetZoom();
	LPDISPATCH GetSlide();
	long GetPointerType();
	void SetPointerType(long nNewValue);
	long GetState();
	void SetState(long nNewValue);
	long GetAcceleratorsEnabled();
	void SetAcceleratorsEnabled(long nNewValue);
	float GetPresentationElapsedTime();
	float GetSlideElapsedTime();
	void SetSlideElapsedTime(float newValue);
	LPDISPATCH GetLastSlideViewed();
	long GetAdvanceMode();
	LPDISPATCH GetPointerColor();
	long GetIsNamedShow();
	CString GetSlideShowName();
	void DrawLine(float BeginX, float BeginY, float EndX, float EndY);
	void EraseDrawing();
	void First();
	void Last();
	void Next();
	void Previous();
	void GotoSlide(long index, long ResetSlide);
	void GotoNamedShow(LPCTSTR SlideShowName);
	void EndNamedShow();
	void ResetSlideTime();
	void Exit();
	long GetCurrentShowPosition();
	long GetClickIndex();
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings wrapper class

class SlideShowSettings9 : public COleDispatchDriver
{
public:
	SlideShowSettings9() {}		// Calls COleDispatchDriver default constructor
	SlideShowSettings9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowSettings9(const SlideShowSettings9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetPointerColor();
	LPDISPATCH GetNamedSlideShows();
	long GetStartingSlide();
	void SetStartingSlide(long nNewValue);
	long GetEndingSlide();
	void SetEndingSlide(long nNewValue);
	long GetAdvanceMode();
	void SetAdvanceMode(long nNewValue);
	LPDISPATCH Run();
	long GetLoopUntilStopped();
	void SetLoopUntilStopped(long nNewValue);
	long GetShowType();
	void SetShowType(long nNewValue);
	long GetShowWithNarration();
	void SetShowWithNarration(long nNewValue);
	long GetShowWithAnimation();
	void SetShowWithAnimation(long nNewValue);
	CString GetSlideShowName();
	void SetSlideShowName(LPCTSTR lpszNewValue);
	long GetRangeType();
	void SetRangeType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Presentations wrapper class

class Presentations9 : public COleDispatchDriver
{
public:
	Presentations9() {}		// Calls COleDispatchDriver default constructor
	Presentations9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Presentations9(const Presentations9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& index);
	LPDISPATCH Add(long WithWindow);
	LPDISPATCH Open(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class

class PageSetup9 : public COleDispatchDriver
{
public:
	PageSetup9() {}		// Calls COleDispatchDriver default constructor
	PageSetup9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PageSetup9(const PageSetup9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	long GetFirstSlideNumber();
	void SetFirstSlideNumber(long nNewValue);
	float GetSlideHeight();
	void SetSlideHeight(float newValue);
	float GetSlideWidth();
	void SetSlideWidth(float newValue);
	long GetSlideSize();
	void SetSlideSize(long nNewValue);
	long GetNotesOrientation();
	void SetNotesOrientation(long nNewValue);
	long GetSlideOrientation();
	void SetSlideOrientation(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Slides wrapper class

class Slides9 : public COleDispatchDriver
{
public:
	Slides9() {}		// Calls COleDispatchDriver default constructor
	Slides9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Slides9(const Slides9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& index);
	LPDISPATCH FindBySlideID(long SlideID);
	LPDISPATCH Add(long index, long Layout);
	long InsertFromFile(LPCTSTR FileName, long index, long SlideStart, long SlideEnd);
	LPDISPATCH Range(const VARIANT& index);
	LPDISPATCH Paste(long index);
};
/////////////////////////////////////////////////////////////////////////////
// _Slide wrapper class

class _Slide9 : public COleDispatchDriver
{
public:
	_Slide9() {}		// Calls COleDispatchDriver default constructor
	_Slide9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Slide9(const _Slide9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetShapes();
	LPDISPATCH GetHeadersFooters();
	LPDISPATCH GetSlideShowTransition();
	LPDISPATCH GetColorScheme();
	void SetColorScheme(LPDISPATCH newValue);
	LPDISPATCH GetBackground();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSlideID();
	long GetPrintSteps();
	void Select();
	void Cut();
	void Copy();
	long GetLayout();
	void SetLayout(long nNewValue);
	LPDISPATCH Duplicate();
	void Delete();
	LPDISPATCH GetTags();
	long GetSlideIndex();
	long GetSlideNumber();
	long GetDisplayMasterShapes();
	void SetDisplayMasterShapes(long nNewValue);
	long GetFollowMasterBackground();
	void SetFollowMasterBackground(long nNewValue);
	LPDISPATCH GetNotesPage();
	LPDISPATCH GetMaster();
	LPDISPATCH GetHyperlinks();
	void Export(LPCTSTR FileName, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight);
	LPDISPATCH GetScripts();
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class Shapes9 : public COleDispatchDriver
{
public:
	Shapes9() {}		// Calls COleDispatchDriver default constructor
	Shapes9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shapes9(const Shapes9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH AddCallout(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH AddConnector(long Type, float BeginX, float BeginY, float EndX, float EndY);
	LPDISPATCH AddCurve(const VARIANT& SafeArrayOfPoints);
	LPDISPATCH AddLabel(long Orientation, float Left, float Top, float Width, float Height);
	LPDISPATCH AddLine(float BeginX, float BeginY, float EndX, float EndY);
	LPDISPATCH AddPicture(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height);
	LPDISPATCH AddPolyline(const VARIANT& SafeArrayOfPoints);
	LPDISPATCH AddShape(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH AddTextEffect(long PresetTextEffect, LPCTSTR Text, LPCTSTR FontName, float FontSize, long FontBold, long FontItalic, float Left, float Top);
	LPDISPATCH AddTextbox(long Orientation, float Left, float Top, float Width, float Height);
	LPDISPATCH BuildFreeform(long EditingType, float X1, float Y1);
	void SelectAll();
	LPDISPATCH Range(const VARIANT& index);
	long GetHasTitle();
	LPDISPATCH AddTitle();
	LPDISPATCH GetTitle();
	LPDISPATCH GetPlaceholders();
	LPDISPATCH AddOLEObject(float Left, float Top, float Width, float Height, LPCTSTR ClassName, LPCTSTR FileName, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link);
	LPDISPATCH AddComment(float Left, float Top, float Width, float Height);
	LPDISPATCH AddPlaceholder(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH AddMediaObject(LPCTSTR FileName, float Left, float Top, float Width, float Height);
	LPDISPATCH Paste();
	LPDISPATCH AddTable(long NumRows, long NumColumns, float Left, float Top, float Width, float Height);
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class Shape9 : public COleDispatchDriver
{
public:
	Shape9() {}		// Calls COleDispatchDriver default constructor
	Shape9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shape9(const Shape9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Apply();
	void Delete();
	void Flip(long FlipCmd);
	void IncrementLeft(float Increment);
	void IncrementRotation(float Increment);
	void IncrementTop(float Increment);
	void PickUp();
	void RerouteConnections();
	void ScaleHeight(float Factor, long RelativeToOriginalSize, long fScale);
	void ScaleWidth(float Factor, long RelativeToOriginalSize, long fScale);
	void SetShapesDefaultProperties();
	LPDISPATCH Ungroup();
	void ZOrder(long ZOrderCmd);
	LPDISPATCH GetAdjustments();
	long GetAutoShapeType();
	void SetAutoShapeType(long nNewValue);
	long GetBlackWhiteMode();
	void SetBlackWhiteMode(long nNewValue);
	LPDISPATCH GetCallout();
	long GetConnectionSiteCount();
	long GetConnector();
	LPDISPATCH GetConnectorFormat();
	LPDISPATCH GetFill();
	LPDISPATCH GetGroupItems();
	float GetHeight();
	void SetHeight(float newValue);
	long GetHorizontalFlip();
	float GetLeft();
	void SetLeft(float newValue);
	LPDISPATCH GetLine();
	long GetLockAspectRatio();
	void SetLockAspectRatio(long nNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNodes();
	float GetRotation();
	void SetRotation(float newValue);
	LPDISPATCH GetPictureFormat();
	LPDISPATCH GetShadow();
	LPDISPATCH GetTextEffect();
	LPDISPATCH GetTextFrame();
	LPDISPATCH GetThreeD();
	float GetTop();
	void SetTop(float newValue);
	long GetType();
	long GetVerticalFlip();
	VARIANT GetVertices();
	long GetVisible();
	void SetVisible(long nNewValue);
	float GetWidth();
	void SetWidth(float newValue);
	long GetZOrderPosition();
	LPDISPATCH GetOLEFormat();
	LPDISPATCH GetLinkFormat();
	LPDISPATCH GetPlaceholderFormat();
	LPDISPATCH GetAnimationSettings();
	LPDISPATCH GetActionSettings();
	LPDISPATCH GetTags();
	void Cut();
	void Copy();
	void Select(long Replace);
	LPDISPATCH Duplicate();
	long GetMediaType();
	long GetHasTextFrame();
	LPDISPATCH GetScript();
	CString GetAlternativeText();
	void SetAlternativeText(LPCTSTR lpszNewValue);
	long GetHasTable();
	LPDISPATCH GetTable();
};
/////////////////////////////////////////////////////////////////////////////
// TextFrame wrapper class

class TextFrame9 : public COleDispatchDriver
{
public:
	TextFrame9() {}		// Calls COleDispatchDriver default constructor
	TextFrame9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextFrame9(const TextFrame9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	float GetMarginBottom();
	void SetMarginBottom(float newValue);
	float GetMarginLeft();
	void SetMarginLeft(float newValue);
	float GetMarginRight();
	void SetMarginRight(float newValue);
	float GetMarginTop();
	void SetMarginTop(float newValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	long GetHasText();
	LPDISPATCH GetTextRange();
	LPDISPATCH GetRuler();
	long GetHorizontalAnchor();
	void SetHorizontalAnchor(long nNewValue);
	long GetVerticalAnchor();
	void SetVerticalAnchor(long nNewValue);
	long GetAutoSize();
	void SetAutoSize(long nNewValue);
	long GetWordWrap();
	void SetWordWrap(long nNewValue);
	void DeleteText();
};

/////////////////////////////////////////////////////////////////////////////
// TextRange wrapper class

class TextRange9 : public COleDispatchDriver
{
public:
	TextRange9() {}		// Calls COleDispatchDriver default constructor
	TextRange9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextRange9(const TextRange9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetActionSettings();
	long GetStart();
	long GetLength();
	float GetBoundLeft();
	float GetBoundTop();
	float GetBoundWidth();
	float GetBoundHeight();
	LPDISPATCH Paragraphs(long Start, long Length);
	LPDISPATCH Sentences(long Start, long Length);
	LPDISPATCH Words(long Start, long Length);
	LPDISPATCH Characters(long Start, long Length);
	LPDISPATCH Lines(long Start, long Length);
	LPDISPATCH Runs(long Start, long Length);
	LPDISPATCH TrimText();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	LPDISPATCH InsertAfter(LPCTSTR NewText);
	LPDISPATCH InsertBefore(LPCTSTR NewText);
	LPDISPATCH InsertDateTime(long DateTimeFormat, long InsertAsField);
	LPDISPATCH InsertSlideNumber();
	LPDISPATCH InsertSymbol(LPCTSTR FontName, long CharNumber, long Unicode);
	LPDISPATCH GetFont();
	LPDISPATCH GetParagraphFormat();
	long GetIndentLevel();
	void SetIndentLevel(long nNewValue);
	void Select();
	void Cut();
	void Copy();
	void Delete();
	LPDISPATCH Paste();
	void ChangeCase(long Type);
	void AddPeriods();
	void RemovePeriods();
	LPDISPATCH Find(LPCTSTR FindWhat, long After, long MatchCase, long WholeWords);
	LPDISPATCH Replace(LPCTSTR FindWhat, LPCTSTR ReplaceWhat, long After, long MatchCase, long WholeWords);
	void RotatedBounds(float* X1, float* Y1, float* X2, float* Y2, float* X3, float* Y3, float* x4, float* y4);
	long GetLanguageID();
	void SetLanguageID(long nNewValue);
	void RtlRun();
	void LtrRun();
};
/////////////////////////////////////////////////////////////////////////////
// _Presentation wrapper class

class _Presentation9 : public COleDispatchDriver
{
public:
	_Presentation9() {}		// Calls COleDispatchDriver default constructor
	_Presentation9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Presentation9(const _Presentation9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetSlideMaster();
	LPDISPATCH GetTitleMaster();
	long GetHasTitleMaster();
	LPDISPATCH AddTitleMaster();
	void ApplyTemplate(LPCTSTR FileName);
	CString GetTemplateName();
	LPDISPATCH GetNotesMaster();
	LPDISPATCH GetHandoutMaster();
	LPDISPATCH GetSlides();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetColorSchemes();
	LPDISPATCH GetExtraColors();
	LPDISPATCH GetSlideShowSettings();
	LPDISPATCH GetFonts();
	LPDISPATCH GetWindows();
	LPDISPATCH GetTags();
	LPDISPATCH GetDefaultShape();
	LPDISPATCH GetBuiltInDocumentProperties();
	LPDISPATCH GetCustomDocumentProperties();
	LPDISPATCH GetVBProject();
	long GetReadOnly();
	CString GetFullName();
	CString GetName();
	CString GetPath();
	long GetSaved();
	void SetSaved(long nNewValue);
	long GetLayoutDirection();
	void SetLayoutDirection(long nNewValue);
	LPDISPATCH NewWindow();
	void FollowHyperlink(LPCTSTR Address, LPCTSTR SubAddress, BOOL NewWindow, BOOL AddHistory, LPCTSTR ExtraInfo, long Method, LPCTSTR HeaderInfo);
	void AddToFavorites();
	LPDISPATCH GetPrintOptions();
	void PrintOut(long From, long To, LPCTSTR PrintToFile, long Copies, long Collate);
	void Save();
	void SaveAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts);
	void SaveCopyAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts);
	void Export(LPCTSTR Path, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight);
	void Close();
	LPDISPATCH GetContainer();
	long GetDisplayComments();
	void SetDisplayComments(long nNewValue);
	long GetFarEastLineBreakLevel();
	void SetFarEastLineBreakLevel(long nNewValue);
	CString GetNoLineBreakBefore();
	void SetNoLineBreakBefore(LPCTSTR lpszNewValue);
	CString GetNoLineBreakAfter();
	void SetNoLineBreakAfter(LPCTSTR lpszNewValue);
	void UpdateLinks();
	LPDISPATCH GetSlideShowWindow();
	long GetFarEastLineBreakLanguage();
	void SetFarEastLineBreakLanguage(long nNewValue);
	void WebPagePreview();
	long GetDefaultLanguageID();
	void SetDefaultLanguageID(long nNewValue);
	LPDISPATCH GetCommandBars();
	LPDISPATCH GetPublishObjects();
	LPDISPATCH GetWebOptions();
	LPDISPATCH GetHTMLProject();
	void ReloadAs(long cp);
	long GetEnvelopeVisible();
	void SetEnvelopeVisible(long nNewValue);
	long GetVBASigned();
};
/////////////////////////////////////////////////////////////////////////////
// WebOptions wrapper class

class WebOptions9 : public COleDispatchDriver
{
public:
	WebOptions9() {}		// Calls COleDispatchDriver default constructor
	WebOptions9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	WebOptions9(const WebOptions9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetIncludeNavigation();
	void SetIncludeNavigation(long nNewValue);
	long GetFrameColors();
	void SetFrameColors(long nNewValue);
	long GetResizeGraphics();
	void SetResizeGraphics(long nNewValue);
	long GetShowSlideAnimation();
	void SetShowSlideAnimation(long nNewValue);
	long GetOrganizeInFolder();
	void SetOrganizeInFolder(long nNewValue);
	long GetUseLongFileNames();
	void SetUseLongFileNames(long nNewValue);
	long GetRelyOnVML();
	void SetRelyOnVML(long nNewValue);
	long GetAllowPNG();
	void SetAllowPNG(long nNewValue);
	long GetScreenSize();
	void SetScreenSize(long nNewValue);
	long GetEncoding();
	void SetEncoding(long nNewValue);
	CString GetFolderSuffix();
	void UseDefaultFolderSuffix();
};
/////////////////////////////////////////////////////////////////////////////
// PublishObjects wrapper class

class PublishObjects9 : public COleDispatchDriver
{
public:
	PublishObjects9() {}		// Calls COleDispatchDriver default constructor
	PublishObjects9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PublishObjects9(const PublishObjects9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(long index);
};
/////////////////////////////////////////////////////////////////////////////
// PublishObject wrapper class

class PublishObject9 : public COleDispatchDriver
{
public:
	PublishObject9() {}		// Calls COleDispatchDriver default constructor
	PublishObject9(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PublishObject9(const PublishObject9& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	long GetHTMLVersion();
	void SetHTMLVersion(long nNewValue);
	long GetSourceType();
	void SetSourceType(long nNewValue);
	long GetRangeStart();
	void SetRangeStart(long nNewValue);
	long GetRangeEnd();
	void SetRangeEnd(long nNewValue);
	CString GetSlideShowName();
	void SetSlideShowName(LPCTSTR lpszNewValue);
	long GetSpeakerNotes();
	void SetSpeakerNotes(long nNewValue);
	CString GetFileName();
	void SetFileName(LPCTSTR lpszNewValue);
	void Publish();
};

#endif