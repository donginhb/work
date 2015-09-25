CString WINAPI GetFirstHTMLFile(CString strCourse);
BOOL WINAPI WriteTemplateFile(CString strFileName, CString strCourse, 
			 				  CStringArray& strsTag, CStringArray& strsReplace);
BOOL WINAPI UpdateFileByTag(CString strPath, CString strFile, 
			 				CStringArray& strsTag, CStringArray& strsReplace,
							BOOL bReplaceToLast = FALSE);
BOOL WINAPI UpdateFileByStartEnd(CString strPath, CString strFile, 
			 					 CString strStart, CString strEnd, 
								 CStringArray& strsReplace);
CString WINAPI GetMarkSeq(CString strCourse);
BOOL WINAPI ParseForVideo(CString strSrcPath, 
						  CStringArray& strArrayFile, 
						  CStringArray& strArrayOldLine, 
						  CStringArray& strArrayNewLine,
						  CStringArray& strArrayTypes);
BOOL WINAPI ParseForAudio(CString strSrcPath, 
						  CStringArray& strArrayFile, 
						  CStringArray& strArrayOldLine, 
						  CStringArray& strArrayNewLine,
					      CStringArray& strArrayTypes);
CString WINAPI GetFileCreateTime(CString strFileName);
BOOL WINAPI FileContainString(CString strSrcPath, CString strMatch);
void WINAPI ConvertCodePage(CString &str,UINT srcCodePage,UINT destCodePage);

