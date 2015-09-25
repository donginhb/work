#include "stdafx.h"
#include "resource.h"
#include "CScorm.h"
#include "cmkapi.h"
#define  Scormini "Scorm.ini"

//void  CScorm::MakeintoScorm(CString strSrcCourseDir, CString strCourse, BOOL bOracle)
void  CScorm::MakeintoScorm(CString strSrcCourseDir, CString strCourse, int bOracle, int naEnrich)
{
	WIN32_FIND_DATA FileData; 
	BOOL fFinished = FALSE; 
	HANDLE hSearch; 
	
	//CString strScormPath = ::GetSysPath() + "scorm\\";

	CString strScormPath;

	if (bOracle == 1) // Oracle 5
	{
		strScormPath = ::GetSysPath() + "scorm\\oracle_5\\";
	}
	else if(bOracle == 2) // Old Oracle
	{
		strScormPath = ::GetSysPath() + "scorm\\oracle\\";
	}
	else if(bOracle == 0) // normal
	{
		strScormPath = ::GetSysPath() + "scorm\\";
	}


	hSearch = FindFirstFile(strScormPath + "*.xsd", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		MessageBox(NULL, "No .XSD files found.", "No XSD", MB_OK); 
		fFinished=TRUE;
	} 

	while (!fFinished) 
	{
		CopyFile(strScormPath + FileData.cFileName, strSrcCourseDir + FileData.cFileName, FALSE);

		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				MessageBox(NULL, "Couldn't find next file.", 
					"No more files", MB_OK); 
			} 
		}

	}

	FindClose(hSearch); 
  
	fFinished = FALSE; 
	hSearch = FindFirstFile(strScormPath + "*.xsx", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		fFinished=TRUE;
	} 

	while (!fFinished) 
	{
		CopyFile(strScormPath + FileData.cFileName, strSrcCourseDir + FileData.cFileName, FALSE);

		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				MessageBox(NULL, "Couldn't find next file.", 
					"No more files", MB_OK); 
			} 
		}

	}

	FindClose(hSearch); 

	xGetIniFileData(strSrcCourseDir + Scormini, strSrcCourseDir + "Course.ini");
	//::DeleteFile(strSrcCourseDir + Scormini);
	//::DeleteFile(strSrcCourseDir + "Course.ini");

	//add by shien 
	if(naEnrich = 0)
	{
		::DeleteFile(strSrcCourseDir + Scormini);
		::DeleteFile(strSrcCourseDir + "Course.ini");
	}

	if (!::FileExists(strScormPath + "imsmanifest.xml"))
	{ 
		MessageBox(NULL, "File [imsmanifest.xml] not found.", "No XML", MB_OK); 
		return;
	}
	CopyFile(strScormPath + "imsmanifest.xml", strSrcCourseDir + "imsmanifest.xml", FALSE);
	ProduceIMS(strSrcCourseDir + "imsmanifest.xml");

	if (bOracle != 1) // Oracle 5
	{
		if (!::FileExists(strScormPath + "mcg.xml"))
		{ 
			MessageBox(NULL, "File [mcg.xml] not found.", "No XML", MB_OK); 
			return;
		} 
		CopyFile(strScormPath + "mcg.xml", strSrcCourseDir + "mcg.xml", FALSE);
		ProduceMCGxml(strSrcCourseDir + "mcg.xml");
	}

}

void CScorm::xGetIniFileData(CString ScormIniFile, CString strCourseIniFile)
{
	GetPrivateProfileString("General","Manifestidentity","SingleCourseManifest",General_Manifestidentity,
	   sizeof(General_Manifestidentity),ScormIniFile);

	GetPrivateProfileString("General","OrganizationsDefault","Photon Computer",General_OrganizationsDefault,
	   sizeof(General_OrganizationsDefault),ScormIniFile);

	GetPrivateProfileString("General","OrganizationsIdentity","  ",General_OrganizationsIdentity,
	   sizeof(General_OrganizationsIdentity),ScormIniFile);

	GetPrivateProfileString("Title","Name","  ",Title_Name, sizeof(Title_Name),ScormIniFile);

	GetPrivateProfileString("Title","Subtitle","  ",Title_Subtitle,
	   sizeof(Title_Subtitle),ScormIniFile);

	GetPrivateProfileString("General","Keyword","  ",Title_Keyword,
	   sizeof(Title_Keyword),ScormIniFile);

	GetPrivateProfileString("Information","Date","  ",m_szDate, sizeof(m_szDate), strCourseIniFile);
	GetPrivateProfileString("Information","Duration","  ",m_szTime, sizeof(m_szTime), strCourseIniFile);
	GetPrivateProfileString("Information","Advisor","  ",m_szAuthor, sizeof(m_szAuthor), strCourseIniFile);
}

void CScorm::ProduceIMS(CString sTempFileName)
{
	CString FileData;
	int IFileIndex;

	CFile f;

	//let's open file and read it into CString (u can use any buffer to read though
	if (f.Open(sTempFileName, CFile::modeRead | CFile::shareDenyNone)) 
	{
		int nLength = f.GetLength();
		char* pBuffer = new char[nLength + 1];
		UINT n = f.Read(pBuffer, nLength);
		pBuffer[n] = '\0';
		f.Close();
		FileData = pBuffer;
	}
	else
	{
		return;
	}

//    IFileIndex=FileData.Replace("#1",General_Manifestidentity);
	IFileIndex=FileData.Replace("#2",General_OrganizationsDefault);
   	IFileIndex=FileData.Replace("#3",General_OrganizationsIdentity);
   	IFileIndex=FileData.Replace("#4",Title_Name);
	IFileIndex=FileData.Replace("#5",Title_Subtitle);
	//add by shien
	IFileIndex=FileData.Replace("#6",m_szDate);

	if (f.Open(sTempFileName, CFile::modeWrite | CFile::modeCreate)) 
	{
		f.Write(FileData, FileData.GetLength());
		f.Close();
	}
	else
	{
		return;
	}
}

void CScorm::ProduceMCGxml(CString sTempFileName)
{
	CString FileData;
	int IFileIndex;

	CFile f;

	//let's open file and read it into CString (u can use any buffer to read though
	if (f.Open(sTempFileName, CFile::modeRead | CFile::shareDenyNone)) 
	{
		int nLength = f.GetLength();
		char* pBuffer = new char[nLength + 1];
		UINT n = f.Read(pBuffer, nLength);
		pBuffer[n] = '\0';
		f.Close();
		FileData = pBuffer;
	}
	else
	{
		return;
	}

    IFileIndex=FileData.Replace("%TITLE%",Title_Name);
    CString strLan;
	strLan.LoadString(IDS_SCORM_LANGUAGE);
    IFileIndex=FileData.Replace("%LAN%", strLan);
    IFileIndex=FileData.Replace("%DATE%", m_szDate);
    IFileIndex=FileData.Replace("%TIME%", m_szTime);
    IFileIndex=FileData.Replace("%DESC%", General_Manifestidentity);
    IFileIndex=FileData.Replace("%AUTHOR%", m_szAuthor);
    IFileIndex=FileData.Replace("%KEYWORD%", Title_Keyword);
    IFileIndex=FileData.Replace("%CORP%", General_OrganizationsIdentity);

	if (f.Open(sTempFileName, CFile::modeWrite | CFile::modeCreate)) 
	{
		f.Write(FileData, FileData.GetLength());
		f.Close();
	}
	else
	{
		return;
	}
}

