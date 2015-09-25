#include "stdafx.h"
#include "office_ctrl.h"
#include "word9_ctrl.h"
#include "excel9_ctrl.h"

BOOL Word2HTML(LPCTSTR szWord, LPCTSTR szHtml)
{
    _MSWord oWordApp;
    oWordApp.CreateDispatch("Word.Application", NULL);
	if(oWordApp==NULL)
		return FALSE;

	BOOL fOK = FALSE;

    COleVariant vTrue((short)TRUE), vFalse((short)FALSE);
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    try
    {
		Documents oDocs;
		_Document oDoc;
		oDocs = oWordApp.GetDocuments();
		if(oDocs==NULL)
			goto ERROR_WORD2HTML;

		oDoc = oDocs.Open(COleVariant(szWord), 
					vFalse, // Conversion
					vTrue, // ReadOnly
					vFalse, // AddRecentFiles
					COleVariant(""), // Passwd Doc
					COleVariant(""), // Passwd Template
					vFalse, // Revert
					COleVariant(""), // Write Passwd Doc
					COleVariant(""), // Write Passwd Template
					COleVariant((short)0), //Const wdOpenFormatAuto
					vOpt, // Encoding
					vTrue // Visible
					);

		if(oDoc==NULL)
			goto ERROR_WORD2HTML;

		oDoc.SaveAs(COleVariant(szHtml), 			  
					COleVariant((short)8), // Const wdFormatHTML = 8
					vFalse, // LockComments
					COleVariant(""), // Passwd
					vFalse, // AddRecent
					COleVariant(""), // Write Passwd
					vFalse, // ReadOnlyRecommended
					vFalse, // EmbedTrueTypeFonts 
					vFalse, // SaveNativePictureFormat 
					vFalse, // SaveFormsData
					vFalse // SaveAsAOCELetter
					//msoEncodingUTF8// Enocding UTF8
					);

		fOK = TRUE;
	}	

	catch(COleException *e)
	{
#ifdef _DEBUG
		LPVOID lpMsg;

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                     FORMAT_MESSAGE_FROM_SYSTEM | 
                     FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e->m_sc,
                     MAKELANGID(LANG_NEUTRAL, 
                     SUBLANG_DEFAULT),(LPTSTR) &lpMsg,
                     0, NULL);
		::MessageBox(NULL, (LPCTSTR)lpMsg, "COM Error", 
                  MB_OK | MB_SETFOREGROUND);
		::LocalFree( lpMsg );
#endif
		goto ERROR_WORD2HTML;
	}

	catch(COleDispatchException *e)
	{
		#ifdef _DEBUG
				char msg[512];
				sprintf(msg, "Run-time error '%d':\n\n%s", 
					e->m_scError & 0x0000FFFF, e->m_strDescription);
				::MessageBox(NULL, msg, "Server Error", 
					MB_OK | MB_SETFOREGROUND);

		#endif
		goto ERROR_WORD2HTML;
	}

    ERROR_WORD2HTML:
	oWordApp.Quit(vFalse, vOpt, vOpt);

	return fOK;
}

BOOL Excel2HTML(LPCTSTR szExcel, LPCTSTR szHtml)
{
    _MSExcel oExcelApp;
    oExcelApp.CreateDispatch("Excel.Application", NULL);
	if(oExcelApp==NULL)
		return FALSE;

	BOOL fOK = FALSE;

    COleVariant vTrue((short)TRUE), vFalse((short)FALSE);
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    try
    {

		Workbooks oBooks;
		_Workbook oBook;
		oBooks = oExcelApp.GetWorkbooks();
		if(oBooks==NULL)
			goto ERROR_EXCEL2HTML;

		oBook = oBooks.Open(szExcel, 
					COleVariant((short)3), // Update external links: 3 = all !!
					vTrue, // Read Only
					vOpt, // Format 
					COleVariant(""), // Passwd
					COleVariant(""), // Write Passwd
					vTrue, // IgnoreReadOnlyRecommended
					vOpt, // Origin
					vOpt, // Delimiter
					vFalse, // Editable
					vFalse, // Notify
					vOpt, // Convert Index
					vFalse // AddRecent
					);

		if(oBook==NULL)
			goto ERROR_EXCEL2HTML;

		oBook.SaveAs(COleVariant(szHtml), 			  
					COleVariant((short)44), // Const xlHtml = 44 (&H2C)
					COleVariant(""), // Passwd
					COleVariant(""), // Write Passwd
					vFalse, // ReadOnlyRecommended
					vFalse, // CreateBackup
					1, // AccessMode Const xlNoChange = 1
					vOpt, // Const xlOtherSessionChanges = 3
					vOpt, // AddRecent
					vOpt, // TextCodePage
					vOpt // TextVisualLayout  
					);

		fOK = TRUE;
	}	
    
	catch(COleException *e)
	{
#ifdef _DEBUG
		LPVOID lpMsg;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                     FORMAT_MESSAGE_FROM_SYSTEM | 
                     FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e->m_sc,
                     MAKELANGID(LANG_NEUTRAL, 
                     SUBLANG_DEFAULT),(LPTSTR) &lpMsg,
                     0, NULL);
		::MessageBox(NULL, (LPCTSTR)lpMsg, "COM Error", 
                  MB_OK | MB_SETFOREGROUND);
		::LocalFree( lpMsg );
#endif
		goto ERROR_EXCEL2HTML;
	}

	catch(COleDispatchException *e)
	{
#ifdef _DEBUG
		char msg[512];
		sprintf(msg, "Run-time error '%d':\n\n%s", 
			e->m_scError & 0x0000FFFF, e->m_strDescription);
		::MessageBox(NULL, msg, "Server Error", 
			MB_OK | MB_SETFOREGROUND);
#endif
		goto ERROR_EXCEL2HTML;
	}   

ERROR_EXCEL2HTML:
	oExcelApp.Quit();

	return fOK;
}