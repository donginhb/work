// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application15 wrapper class

class _Application15 : public COleDispatchDriver
{
public:
	_Application15() {}		// Calls COleDispatchDriver default constructor
	_Application15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application15(const _Application15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	LPDISPATCH GetCOMAddIns();
	CString GetProductCode();
	LPDISPATCH GetLanguageSettings();
	long GetShowWindowsInTaskbar();
	void SetShowWindowsInTaskbar(long nNewValue);
	long GetFeatureInstall();
	void SetFeatureInstall(long nNewValue);
	LPDISPATCH GetFileDialog(long Type);
	long GetDisplayGridLines();
	void SetDisplayGridLines(long nNewValue);
	long GetAutomationSecurity();
	void SetAutomationSecurity(long nNewValue);
	LPDISPATCH GetNewPresentation();
	long GetDisplayAlerts();
	void SetDisplayAlerts(long nNewValue);
	long GetShowStartupDialog();
	void SetShowStartupDialog(long nNewValue);
	LPDISPATCH GetAutoCorrect();
	LPDISPATCH GetOptions();
	BOOL GetDisplayDocumentInformationPanel();
	void SetDisplayDocumentInformationPanel(BOOL bNewValue);
	LPDISPATCH GetAssistance();
	long GetActiveEncryptionSession();
	LPDISPATCH GetFileConverters();
	LPDISPATCH GetSmartArtLayouts();
	LPDISPATCH GetSmartArtQuickStyles();
	LPDISPATCH GetSmartArtColors();
	LPDISPATCH GetProtectedViewWindows();
	LPDISPATCH GetActiveProtectedViewWindow();
	BOOL GetIsSandboxed();
	void StartNewUndoEntry();
	long GetFileValidation();
	void SetFileValidation(long nNewValue);
	LPDISPATCH OpenThemeFile(LPCTSTR themeFileName);
	BOOL GetChartDataPointTrack();
	void SetChartDataPointTrack(BOOL bNewValue);
	long GetDisplayGuides();
	void SetDisplayGuides(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow wrapper class

class SlideShowWindow15 : public COleDispatchDriver
{
public:
	SlideShowWindow15() {}		// Calls COleDispatchDriver default constructor
	SlideShowWindow15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowWindow15(const SlideShowWindow15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	LPDISPATCH GetSlideNavigation();
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowWindows wrapper class

class SlideShowWindows15 : public COleDispatchDriver
{
public:
	SlideShowWindows15() {}		// Calls COleDispatchDriver default constructor
	SlideShowWindows15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowWindows15(const SlideShowWindows15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowView wrapper class

class SlideShowView15 : public COleDispatchDriver
{
public:
	SlideShowView15() {}		// Calls COleDispatchDriver default constructor
	SlideShowView15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowView15(const SlideShowView15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void GotoSlide(long Index, long ResetSlide);
	void GotoNamedShow(LPCTSTR SlideShowName);
	void EndNamedShow();
	void ResetSlideTime();
	void Exit();
	long GetCurrentShowPosition();
	void GotoClick(long Index);
	long GetClickIndex();
	long GetClickCount();
	BOOL FirstAnimationIsAutomatic();
	LPDISPATCH Player(const VARIANT& ShapeId);
	long GetMediaControlsVisible();
	float GetMediaControlsLeft();
	float GetMediaControlsTop();
	float GetMediaControlsWidth();
	float GetMediaControlsHeight();
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings wrapper class

class SlideShowSettings15 : public COleDispatchDriver
{
public:
	SlideShowSettings15() {}		// Calls COleDispatchDriver default constructor
	SlideShowSettings15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowSettings15(const SlideShowSettings15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	long GetShowScrollbar();
	void SetShowScrollbar(long nNewValue);
	long GetShowPresenterView();
	void SetShowPresenterView(long nNewValue);
	long GetShowMediaControls();
	void SetShowMediaControls(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Presentations wrapper class

class Presentations15 : public COleDispatchDriver
{
public:
	Presentations15() {}		// Calls COleDispatchDriver default constructor
	Presentations15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Presentations15(const Presentations15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(long WithWindow);
	LPDISPATCH Open(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow);
	void CheckOut(LPCTSTR FileName);
	BOOL CanCheckOut(LPCTSTR FileName);
	LPDISPATCH Open2007(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow, long OpenAndRepair);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class

class PageSetup15 : public COleDispatchDriver
{
public:
	PageSetup15() {}		// Calls COleDispatchDriver default constructor
	PageSetup15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PageSetup15(const PageSetup15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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

class Slides15 : public COleDispatchDriver
{
public:
	Slides15() {}		// Calls COleDispatchDriver default constructor
	Slides15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Slides15(const Slides15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH FindBySlideID(long SlideID);
	long InsertFromFile(LPCTSTR FileName, long Index, long SlideStart, long SlideEnd);
	LPDISPATCH Range(const VARIANT& Index);
	LPDISPATCH Paste(long Index);
	LPDISPATCH AddSlide(long Index, LPDISPATCH pCustomLayout);
};
/////////////////////////////////////////////////////////////////////////////
// _Slide wrapper class

class _Slide15 : public COleDispatchDriver
{
public:
	_Slide15() {}		// Calls COleDispatchDriver default constructor
	_Slide15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Slide15(const _Slide15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	LPDISPATCH GetComments();
	LPDISPATCH GetDesign();
	void SetDesign(LPDISPATCH newValue);
	void MoveTo(long toPos);
	LPDISPATCH GetTimeLine();
	void ApplyTemplate(LPCTSTR FileName);
	LPDISPATCH GetCustomLayout();
	void SetCustomLayout(LPDISPATCH newValue);
	void ApplyTheme(LPCTSTR themeName);
	LPDISPATCH GetThemeColorScheme();
	void ApplyThemeColorScheme(LPCTSTR themeColorSchemeName);
	long GetBackgroundStyle();
	void SetBackgroundStyle(long nNewValue);
	LPDISPATCH GetCustomerData();
	void PublishSlides(LPCTSTR SlideLibraryUrl, BOOL Overwrite, BOOL UseSlideOrder);
	void MoveToSectionStart(long toSection);
	long GetSectionIndex();
	long GetHasNotesPage();
	void ApplyTemplate2(LPCTSTR FileName, LPCTSTR VariantGUID);
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class Shapes15 : public COleDispatchDriver
{
public:
	Shapes15() {}		// Calls COleDispatchDriver default constructor
	Shapes15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shapes15(const Shapes15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
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
	LPDISPATCH Range(const VARIANT& Index);
	long GetHasTitle();
	LPDISPATCH AddTitle();
	LPDISPATCH GetTitle();
	LPDISPATCH GetPlaceholders();
	LPDISPATCH AddOLEObject(float Left, float Top, float Width, float Height, LPCTSTR ClassName, LPCTSTR FileName, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link);
	LPDISPATCH AddPlaceholder(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH Paste();
	LPDISPATCH AddTable(long NumRows, long NumColumns, float Left, float Top, float Width, float Height);
	LPDISPATCH PasteSpecial(long DataType, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link);
	LPDISPATCH AddMediaObject2(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height);
	LPDISPATCH AddMediaObjectFromEmbedTag(LPCTSTR EmbedTag, float Left, float Top, float Width, float Height);
	LPDISPATCH AddSmartArt(LPDISPATCH Layout, float Left, float Top, float Width, float Height);
	LPDISPATCH AddChart2(long Style, long Type, float Left, float Top, float Width, float Height, BOOL NewLayout);
	LPDISPATCH AddPicture2(LPCTSTR FileName, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height, long compress);
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class Shape15 : public COleDispatchDriver
{
public:
	Shape15() {}		// Calls COleDispatchDriver default constructor
	Shape15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shape15(const Shape15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	CString GetAlternativeText();
	void SetAlternativeText(LPCTSTR lpszNewValue);
	long GetHasTable();
	LPDISPATCH GetTable();
	long GetChild();
	LPDISPATCH GetParentGroup();
	long GetId();
	LPDISPATCH GetCustomerData();
	LPDISPATCH GetTextFrame2();
	long GetHasChart();
	long GetShapeStyle();
	void SetShapeStyle(long nNewValue);
	long GetBackgroundStyle();
	void SetBackgroundStyle(long nNewValue);
	LPDISPATCH GetSoftEdge();
	LPDISPATCH GetGlow();
	LPDISPATCH GetReflection();
	LPDISPATCH GetChart();
	long GetHasSmartArt();
	LPDISPATCH GetSmartArt();
	void ConvertTextToSmartArt(LPDISPATCH Layout);
	CString GetTitle();
	void SetTitle(LPCTSTR lpszNewValue);
	LPDISPATCH GetMediaFormat();
	void PickupAnimation();
	void ApplyAnimation();
	void UpgradeMedia();
};
/////////////////////////////////////////////////////////////////////////////
// TextFrame wrapper class

class TextFrame15 : public COleDispatchDriver
{
public:
	TextFrame15() {}		// Calls COleDispatchDriver default constructor
	TextFrame15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextFrame15(const TextFrame15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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

class TextRange15 : public COleDispatchDriver
{
public:
	TextRange15() {}		// Calls COleDispatchDriver default constructor
	TextRange15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextRange15(const TextRange15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	LPDISPATCH PasteSpecial(long DataType, long DisplayAsIcon, LPCTSTR IconFileName, long IconIndex, LPCTSTR IconLabel, long Link);
};
/////////////////////////////////////////////////////////////////////////////
// _Presentation wrapper class

class _Presentation15 : public COleDispatchDriver
{
public:
	_Presentation15() {}		// Calls COleDispatchDriver default constructor
	_Presentation15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Presentation15(const _Presentation15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	long GetDefaultLanguageID();
	void SetDefaultLanguageID(long nNewValue);
	LPDISPATCH GetCommandBars();
	long GetEnvelopeVisible();
	void SetEnvelopeVisible(long nNewValue);
	long GetVBASigned();
	long GetSnapToGrid();
	void SetSnapToGrid(long nNewValue);
	float GetGridDistance();
	void SetGridDistance(float newValue);
	LPDISPATCH GetDesigns();
	void Merge(LPCTSTR Path);
	void CheckIn(BOOL SaveChanges, const VARIANT& Comments, const VARIANT& MakePublic);
	BOOL CanCheckIn();
	LPDISPATCH GetSignatures();
	long GetRemovePersonalInformation();
	void SetRemovePersonalInformation(long nNewValue);
	void EndReview();
	CString GetPasswordEncryptionProvider();
	CString GetPasswordEncryptionAlgorithm();
	long GetPasswordEncryptionKeyLength();
	BOOL GetPasswordEncryptionFileProperties();
	void SetPasswordEncryptionOptions(LPCTSTR PasswordEncryptionProvider, LPCTSTR PasswordEncryptionAlgorithm, long PasswordEncryptionKeyLength, BOOL PasswordEncryptionFileProperties);
	CString GetPassword();
	void SetPassword(LPCTSTR lpszNewValue);
	CString GetWritePassword();
	void SetWritePassword(LPCTSTR lpszNewValue);
	LPDISPATCH GetPermission();
	LPDISPATCH GetSharedWorkspace();
	LPDISPATCH GetSync();
	void SendFaxOverInternet(LPCTSTR Recipients, LPCTSTR Subject, BOOL ShowMessage);
	LPDISPATCH GetDocumentLibraryVersions();
	LPDISPATCH GetContentTypeProperties();
	void RemoveDocumentInformation(long Type);
	void CheckInWithVersion(BOOL SaveChanges, const VARIANT& Comments, const VARIANT& MakePublic, const VARIANT& VersionType);
	void ExportAsFixedFormat(LPCTSTR Path, long FixedFormatType, long Intent, long FrameSlides, long HandoutOrder, long OutputType, long PrintHiddenSlides, LPDISPATCH PrintRange, long RangeType, LPCTSTR SlideShowName, BOOL IncludeDocProperties, 
		BOOL KeepIRMSettings, BOOL DocStructureTags, BOOL BitmapMissingFonts, BOOL UseISO19005_1, const VARIANT& ExternalExporter);
	LPDISPATCH GetServerPolicy();
	LPDISPATCH GetWorkflowTasks();
	LPDISPATCH GetWorkflowTemplates();
	void LockServerFile();
	LPDISPATCH GetDocumentInspectors();
	BOOL GetHasVBProject();
	LPDISPATCH GetCustomXMLParts();
	BOOL GetFinal();
	void SetFinal(BOOL bNewValue);
	void ApplyTheme(LPCTSTR themeName);
	LPDISPATCH GetCustomerData();
	LPDISPATCH GetResearch();
	void PublishSlides(LPCTSTR SlideLibraryUrl, BOOL Overwrite, BOOL UseSlideOrder);
	CString GetEncryptionProvider();
	void SetEncryptionProvider(LPCTSTR lpszNewValue);
	LPDISPATCH GetSectionProperties();
	LPDISPATCH GetCoauthoring();
	void MergeWithBaseline(LPCTSTR withPresentation, LPCTSTR baselinePresentation);
	BOOL GetInMergeMode();
	void AcceptAll();
	void RejectAll();
	void EnsureAllMediaUpgraded();
	LPDISPATCH GetBroadcast();
	BOOL GetHasNotesMaster();
	BOOL GetHasHandoutMaster();
	void Convert2(LPCTSTR FileName);
	long GetCreateVideoStatus();
	void CreateVideo(LPCTSTR FileName, BOOL UseTimingsAndNarrations, long DefaultSlideDuration, long VertResolution, long FramesPerSecond, long Quality);
	void ApplyTemplate2(LPCTSTR FileName, LPCTSTR VariantGUID);
	BOOL GetChartDataPointTrack();
	void SetChartDataPointTrack(BOOL bNewValue);
	void ExportAsFixedFormat2(LPCTSTR Path, long FixedFormatType, long Intent, long FrameSlides, long HandoutOrder, long OutputType, long PrintHiddenSlides, LPDISPATCH PrintRange, long RangeType, LPCTSTR SlideShowName, BOOL IncludeDocProperties, 
		BOOL KeepIRMSettings, BOOL DocStructureTags, BOOL BitmapMissingFonts, BOOL UseISO19005_1, BOOL IncludeMarkup, const VARIANT& ExternalExporter);
	LPDISPATCH GetGuides();
};
/////////////////////////////////////////////////////////////////////////////
// PublishObjects wrapper class

class PublishObjects15 : public COleDispatchDriver
{
public:
	PublishObjects15() {}		// Calls COleDispatchDriver default constructor
	PublishObjects15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PublishObjects15(const PublishObjects15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// PublishObject wrapper class

class PublishObject15 : public COleDispatchDriver
{
public:
	PublishObject15() {}		// Calls COleDispatchDriver default constructor
	PublishObject15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PublishObject15(const PublishObject15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
