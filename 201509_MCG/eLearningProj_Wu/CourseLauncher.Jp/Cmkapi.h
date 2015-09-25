int		 WINAPI GetLeftNumPart(CString strSrc);
BOOL     WINAPI GetTokStr(const char* pSource, 
		  			      CString& StrDest, int& Start, CString Delimeter);
BOOL 	 WINAPI	FileExists(const char* filename);
BOOL	 WINAPI GetDir(CString strDir, CStringArray& strArray);
BOOL	 WINAPI GetFiles(CString strDir, CString strExt, CStringArray& strArray);
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
CString  WINAPI ConvertTimeToStr(LONGLONG qwtime);
CString  WINAPI ConvertSecToStr(long nSeconds);
BOOL	 WINAPI IsDigit(CString str);
BOOL	 WINAPI IsValidTime(CString strTime);
BOOL	 WINAPI SelectImageFile(CString strTitle, CString strDefPath, CString& strSelPath);
BOOL	 WINAPI SelectCtrlFile(CString strTitle, CString strDefPath, CString& strSelPath);
CString  WINAPI GetDir(LPCTSTR szPath);
CString  WINAPI GetFileName(LPCTSTR szPath);
CString  WINAPI GetFileExt(LPCTSTR szPath);
BOOL	 WINAPI IsFullPath(LPCTSTR szPath);
LONGLONG WINAPI ConvertStrToTime(CString strTime);
BOOL	 WINAPI SureDeleteFile(LPCTSTR strPath);
BOOL     WINAPI CopyTree(LPCTSTR szSrcPath, LPCTSTR szDestPath, CStringArray* pExcludeFile = NULL, BOOL bWritable = TRUE);
BOOL     WINAPI IsValidNumber(CString strNum);
CSize	 WINAPI GetBmpDim(UINT nID);
BOOL	 WINAPI SafeCopyFile(CString strSrc, CString strDest, BOOL bFailIfExists = FALSE); 
BOOL	 WINAPI CopyDir(CString strSrcPath, CString strDestPath); 
LONGLONG WINAPI GetVolFreeSpace(CString strVol); 
BOOL	 WINAPI CopyFileToClipboard(CString strFile); 
void	 WINAPI SortArray(CStringArray& strArray);
int		 WINAPI GetFileCount(LPCTSTR szSrcPath, CStringArray* pExcludeFile = NULL);
BOOL	 WINAPI TryToMakeDir(CString strPath, CWnd* pWnd);
void	 WINAPI SetFileWritable(LPCTSTR strPath);
BOOL	 WINAPI UpdateFileByTag(CString strPath, CString strFile, 
			 					CStringArray& strsTag, CStringArray& strsReplace, 
								BOOL bReplaceToLast = FALSE);
BOOL	 WINAPI SetTreeWritable(LPCTSTR szSrcPath);
BOOL	 WINAPI IsFileWritable(LPCTSTR strPath);
BOOL	 WINAPI GetFolder(CString* strSelectedFolder,
						  const char* lpszTitle,
						  const HWND hwndOwner, 
				          const char* strRootFolder, 
				          const char* strStartFolder);
CString WINAPI GetFilePath(LPCTSTR szPath);

