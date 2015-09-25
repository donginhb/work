class CScorm {
public:
  	//void MakeintoScorm(CString strSrcCourseDir, CString strCourse, BOOL bOracle);
  	void MakeintoScorm(CString strSrcCourseDir, CString strCourse, int bOracle, int naEnrich);
private:
	void xGetIniFileData(CString ScormIniFile, CString strCourseIni);
	void ProduceIMS(CString sTempFileName);
	void ProduceMCGxml(CString sTempFileName);
	
	char General_Manifestidentity[512];
    char General_OrganizationsDefault[512];
	char General_OrganizationsIdentity[512];
	char Title_Name[512];
	char Title_Keyword[512];
    char Title_Subtitle[512];
	char m_szDate[512];
	char m_szTime[512];
	char m_szAuthor[512];
	CString m_strContent;
};

  