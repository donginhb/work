#include "stdafx.h"
//#include "grdapi.h"
#include "bmpapi.h"
//#include "myfile.h"

//#include <io.h>
//#define tNormalBufferSize 1024

void WINAPI DrawPanel(HDC hDC,LPRECT lpRect, BOOL bExclusive)
{   
	if (bExclusive)                
		::SetROP2(hDC, R2_XORPEN);
	else	
		::SetROP2(hDC, R2_COPYPEN);
	
   	HPEN Pen1;
	Pen1 = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	HPEN OldPen;
	OldPen = (HPEN)::SelectObject(hDC, Pen1);

	::MoveToEx(hDC, lpRect->left, lpRect->bottom, NULL);
	::LineTo(hDC, lpRect->left, lpRect->top);
	::LineTo(hDC, lpRect->right, lpRect->top); 
     
    HPEN Pen2;
	Pen2=::CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	(HPEN)::SelectObject(hDC, Pen2);
	::LineTo(hDC,lpRect->right, lpRect->bottom);
	::LineTo(hDC,lpRect->left, lpRect->bottom);

	::SelectObject(hDC, OldPen);

    ::DeleteObject(Pen1);
    ::DeleteObject(Pen2);
} 

void WINAPI DrawRect(HDC hDC, LPRECT lpRect, BOOL bExclusive)
{   
   	HPEN Pen1;                                      
	if (!bExclusive)
	{
		::SetROP2(hDC, R2_COPYPEN);
		Pen1 = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    }
    else
    {
		::SetROP2(hDC, R2_XORPEN);
		Pen1 = ::CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
    }
    	
	HPEN OldPen;
	OldPen = (HPEN)::SelectObject(hDC, Pen1);

	::MoveToEx(hDC, lpRect->left, lpRect->bottom - 1, NULL);
	::LineTo(hDC, lpRect->left, lpRect->top);
	::LineTo(hDC, lpRect->right - 1, lpRect->top); 
	::LineTo(hDC, lpRect->right - 1, lpRect->bottom - 1); 
	::LineTo(hDC, lpRect->left, lpRect->bottom - 1);
    
	::SelectObject(hDC, OldPen);

    ::DeleteObject(Pen1);
} 

BOOL WINAPI GetToken(const char* pSource, char* pDest, int* Start, char Delimeter)
{
    int Len = strlen(pSource);
    
    if (*Start >= Len || *Start < 0)
    	return FALSE;

    int i;             
    int Count;
    BOOL LeadByteAppear = FALSE; 
    for (i = *Start; i < Len; i++)
    {
        if (i != Len - 1)
        {
        	if (LeadByteAppear)
        	{
            	LeadByteAppear = FALSE;
            	continue;
        	}
        	else if (IsDBCSLeadByte(pSource[i]))
        	{
            	LeadByteAppear = TRUE;
            	continue;
        	}
        }
        
     	if (i == Len - 1 || pSource[i] == Delimeter)
     	{  	
           	if (pSource[i] == Delimeter)
           		Count = i - *Start;
           	else 
           	    Count = i - *Start + 1;
           	    
           	strncpy(pDest, pSource + *Start, Count);
            pDest[Count] = '\0';
            *Start = i + 1;
            return TRUE;;
        } 
    }                 
    return FALSE;
}

void WINAPI RemoveAndDelete(CObArray* pArray, int nIndex)
{
	CObject* pList;
    if ((pList = pArray->GetAt(nIndex)) == NULL)
    {
		pArray->RemoveAt(nIndex);
		//wangwang20080223
		return;
	}
	pArray->RemoveAt(nIndex);
    if (pList->IsKindOf(RUNTIME_CLASS(CGdiObject)))
       	((CGdiObject*)pList)->DeleteObject();
	if (pList!=NULL)
	{
      //delete pList;  
	  pList=NULL;
	//	//wangwang
	//  pList=NULL;
	}
}

void WINAPI ReleaseObArray(CObArray* pReleasedArray)
{ 
    int ArraySize;

    if (pReleasedArray == NULL)
    	return;
    ArraySize = pReleasedArray->GetSize();
    for (int i = 0; i < ArraySize; i++)
   		::RemoveAndDelete(pReleasedArray, 0);
}

int WINAPI GetIndexFromID(CWordArray* pIDArray, int nID)
{
	if (pIDArray == NULL)
		return -1;
	int nSize = pIDArray->GetSize();
	for (int i = 0; i < nSize; i++)
		if (pIDArray->GetAt(i) == nID)
			return i;
	return -1;
}


//  -------------------------------------------------------------------
//  Function Name: GetToken
//  Purpose      : Get a String delimited by token
//  Global       :
//      Input    : pParsingString - pointer to begin of parsing string
//      Output   : pParsingString - advance a size of Token
//  Local        :
//      Argument : none
//      Return   : NULL  - no Token traced
//                 Token - pointer to traced token.
//  -------------------------------------------------------------------
char *pTracingString=NULL;
char * WINAPI StrTrace(char* pSource, 
					   char *Delimeter, char *Delimeter1, char *pLastToken)
{
	static	char	LastToken[3]="\0\0";
	static	CString StrToken;
	static	char 	StrChars[2];
    StrChars[0] = ';';
    StrChars[1] = '\0';
    if (pSource != NULL)
	{
		pTracingString = pSource;
		if (pLastToken != NULL)
			LastToken[0] = pLastToken[0];
//		LastToken[0] = '\0';
	}
    if (pTracingString == NULL) return NULL;
	if (Delimeter == NULL) 			// get the rest string.
	{
		if (pLastToken != NULL)
			pLastToken[0] = LastToken[0];
		return pTracingString;  
	}
	if ((int)Delimeter == -1) 		// get last token.
		return LastToken;  
	if (Delimeter[0] == '\0')       // skip current token.
	{
        StrChars[0] = *pTracingString;
		pTracingString++;  
	    while (*pTracingString == ' ')
	        pTracingString++; 
    	return StrChars;
	}

    int	    nSize = strlen(Delimeter);


    LastToken[0] = '\0';
	StrToken.Empty();

	if (*pTracingString == '\0') return NULL;

//  Following if stmt will discard the white space between " and " 
//  while strtokex(NULL,""") called at CallCmdExecute.  July.14,1997. R.T.Sheu.
	if (Delimeter[0] != '"' && Delimeter[0] != '\'')
	while (*pTracingString == ' ')
		pTracingString++; 

#ifdef	TokenStringTrace
    BOOL	bFirst = FALSE;
	int		nSize1 = 0;
	if (Delimeter1 != NULL) nSize1 = strlen(Delimeter1);
 	if (strncmp(pTracingString,Delimeter,nSize) == 0)
 	{
 		pTracingString += strlen(Delimeter);
		if (nSize > 1 && strcmp(Delimeter,Delimeter1) == 0)
			bFirst == TRUE;
 	}
#else
	int i;
//	for (i = 0; i < nSize; i++)
//	{
//		if (*pTracingString == Delimeter[i])
//		{
//			pTracingString++;
//			break;
//		}
//	}   // reserver the leading token for the following parsing. 1997.6.18. R.T.S
#endif
 	
    BOOL LeadByteAppear = FALSE; 
    while (*pTracingString != '\0')
    {
    	if (LeadByteAppear)
        	LeadByteAppear = FALSE;
    	else 
    	if (IsDBCSLeadByte(*pTracingString))
        	LeadByteAppear = TRUE;
        else
#ifdef	TokenStringTrace
     	if (strncmp(pTracingString,Delimeter,nSize) == 0)
            break;
        else
        {
			if (nSize1 > 0)
	     	if (strncmp(pTracingString,Delimeter1,nSize1) == 0)
	            break;
	    }
#else
		for (i = 0; i < nSize; i++)
		{
			if (*pTracingString == Delimeter[i])
			{
				LastToken[0] = *pTracingString;
				pTracingString++; // bypass current traced token. June.18,1997 R.T.S
    			return (char*)(char const *)StrToken;
			}
		}
#endif
            
        StrChars[0] = *pTracingString;
        StrToken += StrChars;
        pTracingString++;
    }
    //if (pSource != NULL)
	//	return NULL;
	return (char *)(const char *)StrToken;
}

int nComboBtnWidth = -1;

CString WINAPI GetRowColAt(CObArray* pArray, int nRow, int nCol)
{
	if (pArray == NULL)
		return "";
	if (nRow < 0 || nRow >= pArray->GetSize())
		return "";
	CStringArray* p = (CStringArray*)pArray->GetAt(nRow);
	if (nCol < 0 || nCol >= p->GetSize())
		return "";
	
	return p->GetAt(nCol);
}

