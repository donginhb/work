int		 WINAPI GetLeftNumPart(CString strSrc);
BOOL     WINAPI GetTokStr(const char* pSource, 
		  			      CString& StrDest, int& Start, CString Delimeter);
BOOL 	 WINAPI	FileExists(const char* filename);
BOOL	 WINAPI GetDir(CString strDir, CStringArray& strArray);
BOOL	 WINAPI GetFiles(CString strDir, CString strExt, CStringArray& strArray);
CString	 WINAPI GetCoursePath(BOOL bConfig = FALSE);
CString	 WINAPI GetSysPath();
BOOL	 WINAPI LoadFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty = TRUE);
BOOL	 WINAPI SaveFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty = TRUE);
BOOL	 WINAPI ValidDir(LPCTSTR szPath);
void	 WINAPI Msg(char *szFormat, ...);
void	 WINAPI DebugOutput(TCHAR *tszErr, ...);
BOOL	 WINAPI FileExists(LPCTSTR strPath, LPCTSTR strFileExt);
BOOL	 WINAPI IsDirEmpty(LPCTSTR strInputPath);
BOOL	 WINAPI DelTree(LPCTSTR szPath, CStringArray* pExcludeFile = NULL);
BOOL	 WINAPI ClearDir(LPCTSTR szPath);
BOOL	 WINAPI IsFileNameValid(LPCTSTR szFileName);
CString  WINAPI ConvertTimeToStr(long qwtime);
CString  WINAPI ConvertSecToStr(long nSeconds);
CString  WINAPI ConvertSecToMinStr(long nSeconds);
BOOL	 WINAPI IsDigit(CString str);
BOOL	 WINAPI IsValidTime(CString strTime);
BOOL	 WINAPI SelectImageFile(CString strTitle, CString strDefPath, CString& strSelPath);
BOOL	 WINAPI SelectCtrlFile(CString strTitle, CString strDefPath, CString& strSelPath);
CString  WINAPI GetDir(LPCTSTR szPath);
CString  WINAPI GetFileName(LPCTSTR szPath);
CString  WINAPI GetFilePath(LPCTSTR szPath);
CString  WINAPI GetFileExt(LPCTSTR szPath);
BOOL	 WINAPI IsFullPath(LPCTSTR szPath);
long	 WINAPI ConvertStrToTime(CString strTime);
BOOL	 WINAPI SureDeleteFile(LPCTSTR strPath);
BOOL     WINAPI CopyTree(LPCTSTR szSrcPath, LPCTSTR szDestPath, CStringArray* pExcludeFile = NULL);
BOOL     WINAPI IsValidNumber(CString strNum);
void	 WINAPI SortArray(CStringArray& strArray);
BOOL	 WINAPI TryToMkDir(CString strPath, BOOL bMsg = 1);
CSize	 WINAPI GetBmpDim(UINT nID);
BOOL	 WINAPI SafeCopyFile(CString strSrc, CString strDest, BOOL bFailIfExists = FALSE); 
BOOL	 WINAPI CopyDir(CString strSrcPath, CString strDestPath); 
LONGLONG WINAPI GetVolFreeSpace(CString strVol); 
BOOL	 WINAPI CopyFileToClipboard(CString strFile); 
int		 WINAPI GetFileCount(LPCTSTR szSrcPath, CStringArray* pExcludeFile);
int		 WINAPI GetBitsPerPixel();
int		 WINAPI GetBitPerPixel(HWND hWnd); 
void	 WINAPI SetFileWritable(LPCTSTR strPath);
BOOL	 WINAPI SetTreeWritable(LPCTSTR szSrcPath);
BOOL	 WINAPI IsFileWritable(LPCTSTR szSrcPath);
BOOL     WINAPI IsValidEmailAddr(CString str);
BOOL	 WINAPI GetFolder(CString* strSelectedFolder,
					      const char* lpszTitle,
				          const HWND hwndOwner, 
				          const char* strRootFolder, 
				          const char* strStartFolder);
BOOL WINAPI CopyHTMLFile(CString strSrcPath, CString strDestPath, BOOL bCopyDotFiles);
BOOL WINAPI GetHTMLEmbededFile(CString strSrcPath, CStringArray& strArrayFile, BOOL& bFrame);
BOOL WINAPI SetFullPath(CString strSrcPath, CStringArray& strArrayFile);
BOOL WINAPI ConverSlashToBackslash(CString& strPath);
void WINAPI CopyArray(CStringArray& strArraySrc, CStringArray& strArrayDest, BOOL bConv);
CString WINAPI GetAssociatedDir(CString strSrcPath);
CString WINAPI GetDotFiles(CString strSrcPath);
BOOL WINAPI CopyAllFilesNeed(CString strSrcPath, CString strDestPath, BOOL bCopyDotFiles);
BOOL WINAPI ConvertSlashToBackslash(CString& strPath);
HWND WINAPI GetChildWndByClassName(HWND hWnd, CString strClass);
void WINAPI DrawResizeBar(CRect& rect);
void WINAPI SetSplitCursor(int nType);
BOOL WINAPI	GetHTMLTitle(CString strSrcPath, CString& strTitle);
int WINAPI GetIndex(CString strPath, CString strAppendix, CString strPrefix);
void WINAPI DrawSunk(HDC hDC,LPRECT lpRect, BOOL bExclusive);
void WINAPI AnsitoUtf_8(char *TheAnsiString,char* Utf_8Code);
CString WINAPI GetVideoPath(CString strCourse);
void WINAPI GetProcessTime(BOOL bStart);






