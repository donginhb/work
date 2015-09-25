
#include "s_msppt10.h"
//#include "s_msppt9.h"
class CPPT10Ctrl
{
public:
	CPPT10Ctrl();
	~CPPT10Ctrl();

protected:
	_Application10		  m_app;
	Presentations10       m_presentations;
	_Presentation10       m_presentation;
	Slides10              m_slides;
	SlideShowSettings10   m_slideshow;
	SlideShowWindows10    m_slideshowwindows;
	SlideShowWindow10     m_slideshowwindow;
	SlideShowView10		  m_pView;

	//_Application9		  m_app9;
	//Presentations9        m_presentations9;
	//_Presentation9        m_presentation9;
	//Slides9               m_slides9;
	//SlideShowSettings9    m_slideshow9;
	//SlideShowWindows9     m_slideshowwindows9;
	//SlideShowWindow9      m_slideshowwindow9;
	//SlideShowView9		  m_pView9;

	HWND				  m_hWndPPT;

public:	
	BOOL	OpenPresToWin(CString strPath, HWND hWnd, int nWidth, int nHeight);
	BOOL	OpenPPT(CString strPath);
	BOOL	OpenPresentation(CString strPath);

	void	Close(BOOL bQuit = TRUE);
	void	ClosePresentation(BOOL bQuit);
	
	BOOL	Create();
	BOOL	PrevSlide();
	BOOL	NextSlide();
	BOOL	GotoSlide(int n);
	BOOL	FirstSlide();
	BOOL	LastSlide();
	int		GetSlideCount();
	BOOL	GetSliderTitle(CStringArray& strArray);
	int		GetCurrentSlideNumber();
	BOOL	SaveAsJPG(CString strDir);
//	BOOL	WebPublish(CString strDir);	// del itoh 2013/07/04 PPT2013
	BOOL	WebPublish(CString strPath, CString strPPTPath, CString strDir);
	BOOL	SaveOutline(CString strDir);
	BOOL	WithMedia();
	BOOL	SaveMedia(CString strDir);
	BOOL	AppendSlide();
	void	ShowPPT(BOOL bShow);
	HWND	GetPPTWnd() {return m_hWndPPT;};
	BOOL	SetPublishtoDir();
	//add 2014/7/23 kagata BugNo.131
	BOOL	SetThumb(CString strDir, int SlideCnt);

protected:
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	BOOL SaveFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty);
	BOOL GetRunSlideAndGetPPTHandle();
	void deleteDirectory(CString directory_path);
	void PumpMessage();

public:
	int m_nVersion;
	int GetCurrentAnimaNumber();
};
