#define		cComboBtnWidth	nComboBtnWidth
#define		strtokex(s,d)		StrTrace(s,d)

BOOL WINAPI         GetToken(const char* pSource, char* pDest, int* Start, char Delimeter = '\\');
void	WINAPI		RemoveAndDelete(CObArray* pArray, int nIndex);
void	WINAPI 		ReleaseObArray(CObArray*); 
//CString WINAPI 		FilterSpace(CString Str);  
//void 	WINAPI 		CenterWindow(CWnd*); 
//CString WINAPI		GetItem(CString itemList, int itemno);
//CString WINAPI		StripComma(CString StrSource);
//CString WINAPI		AddCommaToNumber(const char* pszBuffer);
//CString WINAPI		FloatToAscii(double fVal, int d_leng, int p_leng);
//void	WINAPI		ReleasePtrArray(CPtrArray* pReleasedArray);
//CStringArray* WINAPI CopyStrArray(CStringArray* pSourceArray);
void	WINAPI 		DrawPanel(HDC, LPRECT, BOOL bExclusive = FALSE);
void	WINAPI 		DrawRect(HDC hDC, LPRECT lpRect, BOOL bExclusive = FALSE);
int		WINAPI		GetIndexFromID(CWordArray* pIDArray, int nID);
//void	WINAPI		WritePrivateProfileInt(const char* szSec, const char* szEntry, int nValue, const char* szAppIni);
//COLORREF WINAPI		GetColorRef(CString szBuffer);	
//CString WINAPI		GetColorStr(COLORREF rgb);
// next for unit trans
//int WINAPI UnitsToPt(CString tPos,  int nUnitsOption, int nLog);
//char * WINAPI ftoa(double fValue);
//CString WINAPI PtToUnits(CString tPos,  int nUnitsOption, int nLog);
//CString WINAPI TrResolution(CString OldValue, int OldCaps, int NewCaps);
//CRect WINAPI GetPrnRect(CString PosX, CString PosY, CString Width, CString Height, int nUnitsOption, int nLog);

//void WINAPI DrawEditFrame(CDC* pDC, CRect rcEdit, COLORREF color); 
//int WINAPI SelectStr(CString StrSelected, CString StrSel);

//char*  WINAPI LeadingSpaceBypass(char *CheckStr);
//char*  WINAPI LeadingSpaceBypass(CString& CheckStr);
//char*  WINAPI EndingSpaceBypass(char *CheckStr);
//char*  WINAPI EndingSpaceBypass(CString& CheckStr);

//int WINAPI GetNextAppear(const char* pSource, int Start, char ch);
//BOOL WINAPI GetNextNonSpaceIndex(const char* pSource, int* Start);

extern	char *pTracingString;
char * WINAPI StrTrace(char* pSource, 
					   char *Delimeter, char *Delimeter1=NULL, char *pLastToken=NULL);
CString WINAPI GetRowColAt(CObArray* pArray, int nRow, int nCol);
//CString  WINAPI N1000ToChinese(int SrcVal, int Digs);
//CString  WINAPI NumToChinese(long SrcVal, int Digs);
//BOOL WINAPI	InstallCheck();
//CString WINAPI MakeInstallKey(long volval);
//void WINAPI GetFullPath(char* pBuffer, int nLen);
//CString WINAPI TrFDateToVDate(CString SrcStr, BOOL bNoTransIfNotDate = FALSE);
//CString WINAPI TrVDateToFDate(CString SrcStr);
//int	WINAPI IsRightDate(int year, int month, int day);
//BOOL WINAPI IsDigit(CString Str);
//CString WINAPI GetNoStr(int n, int nDigit);
//CString WINAPI GetDateStr(CTime ct);
//CString WINAPI GetTimeStr(CTime ct);
//CString StrListGetAt(CStringList* pList, int ListNo);
//int IDCheckSum(CString CheckSumType, CString KeyDigits);
//CString WINAPI GetFirstTokenBySpace(CString StrSrc);
//CString WINAPI ByPassQuote(CString StrSrc);

