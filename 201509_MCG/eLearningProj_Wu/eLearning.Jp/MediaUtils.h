/***************************************************************
 *                      SniperMedia.h    Ver 1.0               * 
 *                      Author        Sniper(Callsign)         *
 *                      Date          2001/1/17                */

//#include "asferr.h"
#include "wmsdkidl.h"

#ifndef SAFE_RELEASE

#define SAFE_RELEASE( x )	\
    if ( NULL != x )		\
	{						\
		x->Release( );		\
		x = NULL;			\
	}

#endif // SAFE_RELEASE

#define   ErrMsg_MemberFunc_1         "Please Open a File with [OPEN] method ! "
#define   ErrMsg_FlushFailed          "Flush failed !"

class  CMediaUtils
{
protected:
	void GetTimeAndParam(CString cs,int* hh,int* mm,double* ss,CString* param);

protected:
	IWMMetadataEditor* m_pEditor;
	IWMHeaderInfo*     m_pHeaderInfo;
	
	CString m_strErr;
	
	BOOL	m_bOpened;
	BOOL	m_bCreated;
	CString m_strFileName;

protected:	
	IWMMetadataEditor* GetMediaEditor();
	IWMHeaderInfo*     GetHeaderInfo();
	BOOL SeparateTimeStr(CString str, int& hh, int& mm, double& ss, CString& strParam);
	BOOL SeparateTimeStr2(CString str, int& hh, int& mm, double& ss, 
						  CString& strParam, CString& strParam2);
	void ConvTime(CString strTime, int& hh, int& mm, double& ss);
	void SortArray(double* pdArrayTime, int nSize, CStringArray& strArray);
	BOOL WriteTimeScript(CStringArray& strArrayStep);

public:
	CMediaUtils();
	~CMediaUtils();

	CString GetErrorString() { return m_strErr;};
	BOOL	Create();

	BOOL MergeOutline(CStringArray& strArrayOutLine, CStringArray& strArrayStep);
	BOOL Open(LPCTSTR FileName); // open a file to AddScript ,RemoveScript ....
	BOOL AddScript(LPCTSTR lpstrType, LPCTSTR lpstrCmd, int hours, int minutes, double secs);
	BOOL AddScript(LPCTSTR lpstrType, LPCTSTR lpstrCmd, LONGLONG llTime);
	BOOL RemoveScript(WORD idx);
	BOOL AddMarker(LPCTSTR lpcMarkerName, int hours, int minutes, double secs);
	BOOL AddMarker(LPCTSTR lpcMarkerName, LONGLONG llTime);
	BOOL RemoveMarker(WORD wIndex);
	BOOL RemoveAllMarkers(BOOL bFlush = TRUE);
	BOOL RemoveAllScripts(BOOL bFlush = TRUE);
	//BOOL TagASF(LPCTSTR lpcLineOutFileName, LPCTSTR lpcStepFileName, BOOL bFlush = TRUE);
	//BOOL TagASF(CStringArray& strArrayOutline, CStringArray& strArrayStep, 
	//		    BOOL bFlush = TRUE);
	BOOL Flush(); 
	BOOL IsMarked();
	BOOL IsScripted();
	BOOL GetMarkerNames(CStringArray* pstrArrayMarkers = NULL, 
						CStringArray* pstrArrayTime = NULL);
	BOOL GetScriptNames(CStringArray* pstrArrayType = NULL, 
					    CStringArray* pstrArrayCmd = NULL, 
						CStringArray* pstrArrayTime = NULL);
	BOOL GetMarkerTimeByName(CString strMarkerName, QWORD& nPos);
	CString ConvertLLTimeToStr(LONGLONG qwtime);
	BOOL TagASFBothWithTime(CStringArray& strArrayOutline, 
							CStringArray& strArrayStep, BOOL bFlush = TRUE);
	BOOL WriteTimeScript();
	//BOOL AdjustTime(CStringArray& strArray);
};



