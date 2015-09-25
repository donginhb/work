// namegrd.cpp : implementation file
//
                  
#include "stdafx.h"
//#include "eLearning.h"
#include "resource.h"
#include "uigrid.h" 
#include "grdapi.h"                        
#include "bmpapi.h"
//#include "uiresour.h"
#include "gridctrl.h"
//#include "dbobject.h"
#include <string.h> 
#include <stdlib.h>
#include <math.h> 
#include <stdio.h>
#include <memory.h>
#include "Memdc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif 

//#define tField "Äæ¦E
#define tField ""

int			CUIGrid::u_GridNo = 0;
CString		CUIGrid::u_GridWndClass = "";
CFont*		CUIGrid::u_pDefaultFont = NULL;
CBitmap*	CUIGrid::u_pNoteEnBMP = NULL;
CBitmap*	CUIGrid::u_pNoteDisBMP = NULL;
CBitmap*	CUIGrid::u_pUnknownBMP = NULL;

IMPLEMENT_DYNCREATE(CUIGrid, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CUIGrid

CUIGrid::CUIGrid()
{   
	u_bDrawSel = TRUE;

	u_GridNo++;
	u_rgbFG = ::GetSysColor(COLOR_WINDOWTEXT);
    u_rgbVLine = ::GetSysColor(COLOR_BTNFACE);
	u_rgbHLine = ::GetSysColor(COLOR_BTNFACE);

	u_rgbBG = ::GetSysColor(COLOR_WINDOW);
	u_pDefBrush = new CBrush;
	u_pDefBrush->CreateSolidBrush(u_rgbBG);

	u_bDefRowWidth = TRUE;
	
	u_Rows = -1;
	u_Cols = 0;

	u_SelectedRow = -1;
    u_EndSelectedRow = -1;
	
	u_SelType = cGridNonSel; 
	u_bEditable = TRUE;
	u_PreRow = -1;
	u_PreCol = -1;
	u_VirtualPreRow = -1; 
    u_HeadWidth = 40;
    u_pStrArray = new CObArray;
    
	u_pComboArray = NULL;
    u_pBMPArray = NULL;
	u_pTableFieldArray = NULL;
	u_pColorArray = NULL;
	
	u_pBMP = NULL; 
	u_bWithFixedRow = FALSE;
	u_bWithFixedCol = FALSE;
	u_pFixedRow = NULL;
	u_pFixedCol = NULL;
    u_ScrollPos = 0; 
    u_bCellMark = FALSE; 
    u_bSort = FALSE;
    u_bDragLine = TRUE;
    u_bDragTitle = FALSE;
    u_Dragable = FALSE; 
    u_bDragCell = FALSE; 
    u_bMultiSel = TRUE;
    u_bHScrollable = TRUE;
    u_CtrlShown = FALSE;
    u_bFieldSizeChanged = FALSE;      
    u_bNoUpdate = FALSE;

	u_pBMPCombo = NULL;
	u_pEdit = NULL;
	u_pPassEdit = NULL;
	u_pCombo = NULL;
	u_pComboEdit = NULL;
    
    u_pBtnBMP = NULL;
    u_bBtnNullEnable = FALSE;
    u_GridInitX = cGridInitX;
    u_bFixedButtoned = TRUE;
    u_TitleAlign = 0;
    u_bBtnClickable = TRUE;
    if (u_pDefaultFont == NULL)
	{
	    u_pDefaultFont = new CFont;
		u_pDefaultFont->CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
								   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								   PROOF_QUALITY, FF_ROMAN, //"MS UI Gothic");
#ifdef _JAPANESE
	"MS UI Gothic");
#else
	"Arial");
#endif


	}
	u_pFont = u_pDefaultFont;
    u_bVirtualPage = FALSE;  
    u_FrameSize = cPageSize;
    u_FrameTop = 0;
    u_VirtualScrollPos = 0;
	u_VirtualLastScrollPos = 0;
    SetHeadWidth(60);
	u_bLButtonUp = TRUE;
	u_pBrushArray = NULL;
	u_bOverWrite = FALSE;
	u_pBMPIDArray = NULL;
	u_pAllowedArray = NULL;
	u_pSelArray = NULL;
	u_bToggleSel = FALSE;
	//u_bFirst = TRUE;
	u_VirtualDataSize = 0;
	u_rgbFixed = ::GetSysColor(COLOR_WINDOW);

	u_StrDefAttrib.Format("%c%c%c%c%c%c%c%c%c%c", 
		cGridText + 1, 1, 2, cLeftAlign + 1, 
		cDefColor + 1, cDefColor + 1, 1, 1, cDefValue + 1, 1);
	
	u_pComboStrArray = NULL;
	u_bAutoCol = FALSE;
	u_pValueMapArray = NULL;
	u_bCopyable = TRUE;
	u_bPasteable = TRUE;
	u_pMSGWnd = NULL;
	u_PreOverRow = -1;
	u_PreOverCol = -1;
	m_bLeaveTrack = FALSE;
	u_rgbSel = ::GetSysColor(COLOR_WINDOWTEXT);
	u_bVScrollable = TRUE;
	u_rgbSelRows = ::GetSysColor(COLOR_WINDOWTEXT);
	u_bPaintBk = TRUE;
	u_bMultilineEdit = FALSE;
}                                    

CUIGrid::~CUIGrid()
{ 
    u_GridNo--;
	if (u_GridNo == 0)
	{
		u_pDefaultFont->DeleteObject();
		delete u_pDefaultFont;
		u_pDefaultFont = NULL;

   		if (u_pNoteEnBMP != NULL)
   		{
   			u_pNoteEnBMP->DeleteObject();
   			delete u_pNoteEnBMP;
			u_pNoteEnBMP = NULL;
		}
   		
   		if (u_pNoteDisBMP != NULL)
   		{
   			u_pNoteDisBMP->DeleteObject();
   			delete u_pNoteDisBMP;
			u_pNoteDisBMP = NULL;
		}

   		if (u_pUnknownBMP != NULL)
   		{
   			u_pUnknownBMP->DeleteObject();
   			delete u_pUnknownBMP;
			u_pUnknownBMP = NULL;
		}
	}

	delete u_pComboArray; 
    delete u_pTableFieldArray;
   	delete u_pBMPArray;
	delete u_pColorArray;
	delete u_pAllowedArray;
	if (u_pBMP != NULL)
	{
		::ReleaseObArray(u_pBMP);
	    delete u_pBMP;  
    }

	if (u_pBtnBMP != NULL)
	{
		::ReleaseObArray(u_pBtnBMP);
	    delete u_pBtnBMP;  
    }

	if (u_pBrushArray != NULL)
	{
		::ReleaseObArray(u_pBrushArray);
		delete u_pBrushArray;
	}

	u_pDefBrush->DeleteObject();
	delete u_pDefBrush;

	if (u_pComboStrArray != NULL)
	{
		::ReleaseObArray(u_pComboStrArray);
		delete u_pComboStrArray;
	}

	delete u_pEdit;
	delete u_pPassEdit;
	delete u_pComboEdit; 
	delete u_pCombo; 
	delete u_pBMPCombo;

	::ReleaseObArray(u_pStrArray);
	delete u_pStrArray;

	delete u_pFixedRow;
	delete u_pFixedCol;
	delete u_pBMPIDArray;
	delete u_pSelArray;
	delete u_pValueMapArray;
}

BEGIN_MESSAGE_MAP(CUIGrid, CWnd)
	//{{AFX_MSG_MAP(CUIGrid)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN() 
	ON_MESSAGE(WM_EDIT_KEY_DOWN, OnEditKeyDown)
	ON_EN_CHANGE(ID_GRID_EDIT, OnEditChange)
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(ID_GRID_COMBO, OnComboSelChange)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_MESSAGE(WM_GRD_COMBOEDIT_CHANGE, OnComboEditChange)
	ON_WM_CREATE()
	ON_MESSAGE(WM_EDIT_LOSE_FOCUS_MODIFIED, OnCtrlLoseFocus)
	ON_CBN_SELCHANGE(ID_GRID_BMP_COMBO, OnComboSelChange)
	ON_CBN_SELCHANGE(ID_GRID_COMBOEDIT, OnComboSelChange)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIGrid message handlers

BOOL CUIGrid::IsCellCombo(int nCell)
{
    return nCell == cGridComboEdit   || nCell == cGridComboEditBtn ||
  		   nCell == cGridCombo       || nCell == cGridComboBtn     ||
  		   nCell == cGridBMPCombo    || nCell == cGridComboDB      ||
  		   nCell == cGridComboEditDB;
}

BOOL CUIGrid::IsCellComboOrEdit(int nCell)
{
    return (nCell != cGridBMP && nCell != cGridHyperBMP);
}

BOOL CUIGrid::NeedFrame(int nCell)
{
	BOOL bCond1, bCond2;
	bCond1 = nCell == cGridBMP || nCell == cGridText || nCell == cGridNote;
	bCond2 = nCell == cGridCombo   || nCell == cGridComboEdit ||
			 nCell == cGridComboDB || nCell == cGridComboEditDB || nCell == cGridPassword;
	bCond2 = bCond2 && !u_CtrlShown; 
	return bCond1 || bCond2;
}    				 

void CUIGrid::OnDestroy()
{
	CWnd::OnDestroy(); 
    
	if (u_PreRow >= 0 && u_PreCol >= 0)
	{
        if (IsCellComboOrEdit(u_PreCellType) && u_CtrlShown)            
   			UpdateGrdData();
   	}		        	
}

void CUIGrid::OnDraw(CDC* pDC, CObArray* pStrArray, long* pFrameTop,
					 int bPrinting, CSize *pSize, int nPageNo, int yP1Start)
// nPageNo is 1-based
{   
    if (pStrArray == NULL)
    	return;

    int i;		
    int RightEdge;
    long StartLine; 
	CPoint oldOrigin;
    CRect RectGrid;
	int OldRows;

    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;

   	long DataSize = GetDataSize();
	BOOL IsPageMode = (MM_TEXT != pDC->GetMapMode());
    
    if (IsPageMode) //Print or PrintPreView
    {
     	ASSERT(pSize != NULL);	// You cannot use Page mode w/o supplying a page size
		
		OldRows = u_Rows;

		CSize PageSize;
		PageSize = *pSize;

		if(nPageNo <= 1)
		{
			oldOrigin = pDC->SetWindowOrg(int(-PageSize.cx * 0.08),	-yP1Start);
			RectGrid.SetRect(0, 0, int(PageSize.cx * 0.8), 0);
			StartLine = 0;
			u_Rows = int(PageSize.cy * 0.9 - yP1Start) / u_RowWidth;
		}
		else
		{
			oldOrigin = pDC->SetWindowOrg(int(-PageSize.cx * 0.08), 
				int(-PageSize.cy * 0.055));
			RectGrid.SetRect(0, 0, int(PageSize.cx * 0.8), 0);
			StartLine = (nPageNo - 2) * (u_Rows - 1)
				+ int(PageSize.cy * 0.9 - yP1Start) / u_RowWidth - 1;	// line count on page 1
			u_Rows = int(PageSize.cy * 0.85) / u_RowWidth;
		}
		RectGrid.bottom = RectGrid.top + u_RowWidth * u_Rows;
		RightEdge = int(PageSize.cx * 0.8);
    }                   
    else // not page mode
    {
		GetClientRect(RectGrid);
			
        if (u_SelectedRow > DataSize - 1)
        	u_SelectedRow = -1;
        if (u_EndSelectedRow > DataSize - 1)
        	u_EndSelectedRow = u_SelectedRow;
        	
		RightEdge = GetRightEdge();
		StartLine = u_VirtualScrollPos;
    }
    
	//if (!bPrinting)
	{
		CRect RectBound;
		RectBound.left = RectGrid.left - 1;
		RectBound.right = RectGrid.right + 3;
		RectBound.top = RectGrid.top - 1;
		RectBound.bottom = RectGrid.bottom + 3;
		pDC->IntersectClipRect(RectBound);
    }
    
    //////////////// Create or Select GDI Object ////////////////////
    
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetROP2(R2_COPYPEN);
    CFont* OldFont;
    int StartCol;
    if (IsPageMode) //Print or PrintPreView
   	    StartCol = 0;
	else
	{
	    OldFont = pDC->SelectObject(u_pFont);
	    StartCol = GetStartCol(GetScrollPos(SB_HORZ));
	}
   	int Bound = (int)min(DataSize - StartLine, u_Rows - IntFixedRow);

    int EndCol = u_Cols - 1;
	int TotalColWidth = GetAheadSpace();
    for (i = StartCol; i < u_Cols; i++)
    {
         TotalColWidth += u_ColWidth[i];
         if (TotalColWidth > RectGrid.Width())
         {
             EndCol = i;
             break;
         }
    }              

    if (IsPageMode)      //If Printing, Erase the content at first.
    {
		::FillRect(pDC->m_hDC, (LPRECT)RectGrid, 
   	  			   (HBRUSH)::GetStockObject(WHITE_BRUSH));
    }
    
	DrawGridLines(pDC, StartCol, EndCol, Bound, RectGrid, 
				  RightEdge, StartLine);    
    DrawGridContent(pDC, pStrArray, StartCol, EndCol, Bound, 
    				StartLine, bPrinting, pFrameTop);
                                
    if (EndCol == u_Cols - 1 && RightEdge < RectGrid.right)
    {
	  	CRect RectFill;
    	RectFill.left = RightEdge + 1;
    	RectFill.right = RectGrid.right;
    	RectFill.top = 0;
    	RectFill.bottom = RectGrid.bottom;
		
		pDC->FillRect(RectFill, u_pDefBrush);

	    CPen* pPenBound = new CPen;
	    pPenBound->CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWTEXT)); 
	   	CPen* pOldPen = pDC->SelectObject(pPenBound);    
		pDC->MoveTo(RightEdge, 0);
		pDC->LineTo(RightEdge, RectFill.bottom);
        pDC->SelectObject(pOldPen);
        pPenBound->DeleteObject();
		delete pPenBound;
    }

    if (IsPageMode) //Print or PrintPreView
    {
    	if (bPrinting)
    	{
    		CRect RectFill;
    		RectFill.left = RectGrid.right;
    		RectFill.right = RectFill.left + u_ColWidth[EndCol];
    		RectFill.top = RectGrid.top;
    		RectFill.bottom = RectGrid.bottom;
			::FillRect(pDC->m_hDC, (LPRECT)RectFill, 
   		   					(HBRUSH)::GetStockObject(WHITE_BRUSH));
        }

    	pDC->MoveTo(RectGrid.left - 1, RectGrid.top - 1);
    	pDC->LineTo(RectGrid.left - 1, RectGrid.bottom);
    	pDC->LineTo(RectGrid.right + 1, RectGrid.bottom);
    	pDC->LineTo(RectGrid.right + 1, RectGrid.top - 1); 
    	pDC->LineTo(RectGrid.left - 1, RectGrid.top - 1); 
        
    	pDC->SetWindowOrg(oldOrigin);

		u_Rows = OldRows; //Print function might change the value of u_Rows
    }	
    else
    {
    	// Draw Select /////////////////////////////////////////
    
		if (u_SelType != cGridNonSel)
		{
			if (u_bToggleSel && u_bMultiSel)
				DrawSelRows(pDC, StartLine, StartLine + u_Rows, FALSE);			
			else 
				DrawSelRegion(pDC, u_SelectedRow, u_EndSelectedRow, FALSE);  
		}
		
    	DrawASel(pDC);
    	
	    pDC->SelectObject(OldFont);
    }
}

void CUIGrid::PaintWithBrush(CDC* pDC, COLORREF rgb, CRect RectFill)
{
	 CBrush* pBrush = new CBrush;
	 pBrush->CreateSolidBrush(rgb);
	 pDC->FillRect((LPRECT)RectFill, pBrush);
	 pBrush->DeleteObject();
	 delete pBrush;
}

void CUIGrid::DrawASel(CDC* pDC, BOOL bObligated)
{
	if (u_PreRow >= 0 && u_PreCol >= 0)
	{ 
	    BOOL bNotShown = (u_PreRow != -1) && (u_PreCol != -1) && !u_CtrlShown;
	    BOOL bNeedFrame = NeedFrame(u_PreCellType) || bNotShown || bObligated;
		BOOL bDrawSel = u_PreCellType != cGridEditBtn && 
						u_PreCellType != cGridHyperLink &&
						u_PreCellType != cGridHyperBMP &&
						bNeedFrame;
		if (bDrawSel)
			DrawSel(pDC, u_PreRow, u_PreCol);
		else if (u_PreCellType== cGridEditBtn)
			DrawSel(pDC, u_PreRow, u_PreCol, TRUE);
	}
}

void CUIGrid::DrawGridContent(CDC* pDC, CObArray* pStrArray, int StartCol, 
							  int EndCol, int Bound, long StartLine,
							  BOOL bPrinting, long* pFrameTop)
{
    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
    
    BOOL IsPageMode = (pDC->GetMapMode() != MM_TEXT);
	
	///////////// Write Text or Draw Icon ////////////////////////
	for (int i = 0; i <= u_Rows; i++)
   	{  
   	    int nIndex = i;
		if (i < Bound)
   	    {
            nIndex = (int)(StartLine  - *pFrameTop + i); 
            if (nIndex >= pStrArray->GetSize() || nIndex < 0)
            {
				GetMSGWnd()->SendMessage(WM_GRD_NEED_DATA, (WPARAM)this, StartLine + i); 
            	nIndex = (int)(StartLine  - *pFrameTop + i);
       		}
        }
             
        int ColPos = GetAheadSpace();
        int j;    
   		for (j = StartCol; j < EndCol + 1; j++)	   
  		{ 
  			if (j != StartCol)
  				ColPos += u_ColWidth[j - 1];

            int PosY = (i + IntFixedRow) * u_RowWidth - 1;

			CRect RectInside;
			if (j == 0 && !u_bWithFixedCol && u_SelType != cGridHeadSel)
				RectInside.left = ColPos;
			else
				RectInside.left = ColPos + 1;	
			RectInside.right = ColPos + u_ColWidth[j];
  			RectInside.top = PosY + 1;
			RectInside.bottom = RectInside.top + u_RowWidth - 1;
    		
            if (!IsPageMode)
            {
				CBrush* pBrush;
				if (i < Bound)
					pBrush = GetBrush(nIndex, j);
				else 
					pBrush = GetDefBrush(j);
				if (u_ColWidth[j] !=0)
					pDC->FillRect((LPRECT)RectInside, pBrush);
            }
            
            if (i >= Bound)
            	continue;

    	  	CRect RectTemp; 
            CString StrShown = GetDataAt(nIndex, j);
            char CellType = GetAttribAt(nIndex, j, cType);
            char Alignment = GetAttribAt(nIndex, j, cAlign);
			pDC->SetTextColor(GetCellColor(nIndex, j, cFGColor));
			
    	  	switch (CellType)
    	  	{   
                case cGridBMPCombo:
       	  		case cGridBMP:
    	  		{   
    	  	  	    int nID = atoi((const char*)StrShown);
					int nIdx = ::GetIndexFromID(u_pBMPIDArray, nID);
					DrawBMP(pDC, RectInside, nIdx, bPrinting, Alignment);
    	  	 	    break;
    	  	 	}    

				case cGridHyperBMP:
    	  		{   
					int nID, nIdx;
					if (u_PreOverRow == nIndex && u_PreOverCol == j)
					{
			 			int n = GetAttribAt(nIndex, j, cIndex);
						nIdx = GetNextBMPIndex(StrShown, n);
					}
					else
					{
						nID = atoi((const char*)StrShown);
						nIdx = ::GetIndexFromID(u_pBMPIDArray, nID);
					}
					DrawBMP(pDC, RectInside, nIdx, bPrinting, Alignment);
    	  	 	    break;
    	  	 	}    

				case cGridPassword:
				case cGridComboEdit:
    	  	 	case cGridCombo:
   	  			case cGridText:
   	  			case cGridHyperLink:
   	  			case cGridComboDB:
   	  			case cGridComboEditDB:
   	  			{
   	  				UINT nDrawMode = DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE;
   	  				switch (Alignment)
   	  				{
   	  				    case cLeftAlign:
   	  				       	nDrawMode |= DT_LEFT;
   	  				    	break;
   	  				    case cCenterAlign:
   	  				    	nDrawMode |= DT_CENTER;
   	  				    	break;
   	  				    case cRightAlign:
   	  				    	nDrawMode |= DT_RIGHT;
   	  				    	break;
   	  				}
   	  				
   	  				RectTemp.SetRect(ColPos + u_GridInitX, cGridInitY + PosY,              
   	  								 RectInside.right - u_GridInitX, RectInside.bottom);
   	  				
					int nLen = strlen((const char*)StrShown);
					if (CellType == cGridPassword)
					{
						pDC->DrawText((const char*)GetStrOfLen(nLen, '*'), 
						    nLen, RectTemp, nDrawMode);
					}
					else if (CellType == cGridHyperLink)
					{
						if (u_PreOverRow == nIndex && u_PreOverCol == j)
							pDC->SetTextColor(RGB(255, 0, 0));
						pDC->DrawText((const char*)StrShown, nLen,
   	  							RectTemp, nDrawMode);
					}
					else
					{
						pDC->DrawText((const char*)StrShown, nLen,
   	  						RectTemp, nDrawMode);
					}

   	  				break;
                }
                
    	  	 	case cGridEditBtn:
                {
	    			int Width = RectInside.Width() - u_GridInitX * 2;
    				int BtnWidth = min(Width, u_RowWidth);

   	  				RectTemp.SetRect(ColPos + u_GridInitX * 3 + BtnWidth, 
   	  					 cGridInitY + PosY,	RectInside.right, RectInside.bottom);
   	  				pDC->DrawText((const char*)StrShown, strlen((const char*)StrShown),
   	  					RectTemp, DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);

                    BOOL BtnEnable = u_bBtnNullEnable || StrShown.GetLength() > 0;
                    DrawNoteBtn(pDC, RectInside, BtnEnable, TRUE, FALSE, 0,
                    			GetAttribAt(i, j, cIndex));
   	  				break;	
   	  			}

  	  			case cGridNote:
   	  			{                           
                    DrawNoteBtn(pDC, RectInside, StrShown.GetLength() > 0, TRUE
                    			, FALSE, Alignment);
                }	
   	  		}	
   	   	}   
   	}
}

CString CUIGrid::GetStrOfLen(int nLen,  char ch)
{
	CString Str = "";
	for (int i = 0; i < nLen; i++)
		Str += ch;
	return Str;
}

void CUIGrid::DrawGridLines(CDC* pDC, int StartCol, int EndCol,
							int Bound, CRect RectGrid, int RightEdge, long StartLine)
{
    CBrush* OldBrush; 
    CBrush NewBrush;
	CString StrTemp;
    BOOL IsPageMode = (pDC->GetMapMode() != MM_TEXT);
    if (IsPageMode)    //Print or PrintPreView         
	    NewBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	else
   	    NewBrush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    OldBrush = pDC->SelectObject(&NewBrush);   

    CPen PenV, PenH, Pen3;// Pen4; 
    CPen* OldPen;
	PenV.CreatePen(PS_SOLID, 1, u_rgbVLine);
	PenH.CreatePen(PS_SOLID, 1, u_rgbHLine);
    Pen3.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWTEXT)); 
 
    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;

    // Draw Main Horz. Lines /////////////////////////////
    int Initial = 0;
    if (u_SelType == cGridHeadSel || (u_bWithFixedCol && u_bFixedButtoned))
    	Initial = u_HeadWidth + 1;
    
   	OldPen = pDC->SelectObject((CPen*)&PenH);
	int i = 0;
    for (i = 0; i < u_Rows; i++)
    {
        pDC->MoveTo(RectGrid.left + Initial, u_RowWidth * (i + 1) - 1);
        pDC->LineTo(RightEdge, u_RowWidth * (i + 1) - 1);
    }          
    
    // Draw Main Vert. Lines //////////////////////////////
    pDC->SelectObject((CPen*)&PenV);
	int WidthCount = GetAheadSpace();
    for (i = StartCol; i < EndCol; i++)
    {
        WidthCount += u_ColWidth[i];	
		pDC->MoveTo(WidthCount, 0);
		pDC->LineTo(WidthCount, RectGrid.bottom);
    }  

    // Draw Left Side //////////////////////////
    UINT nFormat = DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE;
	switch (u_TitleAlign)
	{
		case 2:
			nFormat |= DT_RIGHT;
			break;

		case 1: 
			nFormat |= DT_CENTER;
			break;
		
		default:
			nFormat |= DT_LEFT;
	}

    if (u_bWithFixedCol || u_SelType == cGridHeadSel)
    {                                        
        int Interval = u_HeadWidth;
        		
        CRect rcFixed;
        rcFixed.SetRect(-1, -1 ,Interval, RectGrid.bottom); 
        if (!(u_bWithFixedCol && !u_bFixedButtoned))
           	pDC->Rectangle(rcFixed);
        else
        	::FillRect(pDC->m_hDC, rcFixed,(HBRUSH)::GetStockObject(WHITE_BRUSH));

		RECT RectTemp;
       
       	if (u_bWithFixedCol && !u_bFixedButtoned)
        	pDC->SelectObject((CPen*)&PenH); 
        else
   	        pDC->SelectObject((CPen*)&Pen3); 

      	for (i = IntFixedRow; i < u_Rows + IntFixedRow; i++)
	    {   
	    	int PosY = u_RowWidth * (i + 1) - 1;
	    	
   			pDC->MoveTo(0, PosY);
   			pDC->LineTo(Interval, PosY); 
	        
	        if ((u_SelType == cGridHeadSel || u_bWithFixedCol) 
	        	&& !(i == 0 && u_bWithFixedRow))
	        {
	         	int AbsLine = (int)(u_SelectedRow - StartLine + IntFixedRow);
	         	int AbsEndLine = (int)(u_EndSelectedRow - StartLine + IntFixedRow); 
	         	if (!(!u_bMultiSel && i == AbsLine) && 
	         		!(u_bMultiSel && u_SelectedRow != -1 && i >= AbsLine && i <= AbsEndLine)
	         		&& !IsPageMode)	         	
	         	{
	         	 	RectTemp.left = 0; 
	         		RectTemp.top =	u_RowWidth * i;
	         		RectTemp.right = Interval - 1;
	         		RectTemp.bottom = PosY - 1;

	         		if (!(u_bWithFixedCol && !u_bFixedButtoned))
	         			DrawBtnEdge(pDC, &RectTemp); 
	         	}	
	        }
	        
            if (u_bWithFixedCol)
	        {
                RectTemp.top += 2;
                RectTemp.left = u_GridInitX;
                RectTemp.right -= u_GridInitX;
				
				StrTemp = GetFixedColLabelAt(StartLine + i - IntFixedRow);
   	  			pDC->DrawText(StrTemp, StrTemp.GetLength(),	&RectTemp, nFormat);
	        }
	    } 
		
       	if (u_bWithFixedCol && !u_bFixedButtoned)
        	pDC->SelectObject((CPen*)&PenV); 
	    pDC->MoveTo(Interval, 0);
		pDC->LineTo(Interval, RectGrid.bottom);
        
        long DataSize = GetDataSize();
        if (u_SelType == cGridHeadSel)
        {   
        	long i; 
        	for (i = StartLine; i < Bound + StartLine; i++)
        	{
				StrTemp = SetHeadString(i);
				rcFixed.left = 0;
		   	   	rcFixed.top = u_RowWidth * (int)(i - StartLine + IntFixedRow);;
		   	   	rcFixed.right = rcFixed.left + u_HeadWidth - u_GridInitX;
		   	   	rcFixed.bottom = rcFixed.top + u_RowWidth;
		   	   	pDC->DrawText((const char*)StrTemp, -1, rcFixed, DT_VCENTER | DT_SINGLELINE | DT_RIGHT); 
        		ExtraDrawHeadSel(pDC, rcFixed, i);
			}
        }	
    }
 
    //  Draw Top Side ///////////////////////////////

   	if (u_bWithFixedRow)
    {   
        pDC->SelectObject((CPen*)&PenH);          

		CBrush FixedBrush;
		if (!u_bFixedButtoned)
		{
			FixedBrush.CreateSolidBrush(u_rgbFixed);
			pDC->SelectObject(FixedBrush);
		}
		pDC->Rectangle(-1, -1, RightEdge + 1, u_RowWidth);
		if (!u_bFixedButtoned)
		{
			FixedBrush.DeleteObject();
			pDC->SelectObject(NewBrush);
		}
        
   		if (u_bFixedButtoned)
			pDC->SelectObject((CPen*)&Pen3); 
		else        
			pDC->SelectObject((CPen*)&PenV); 
      	
        int WidthCount = 0;
		for (i = StartCol - 1; i < EndCol; i++)
	    { 
	       	if (i == StartCol - 1)
	       	{
        		if ((u_SelType == cGridHeadSel || u_bWithFixedCol)
					&& u_bWithFixedRow)
        		{
            		WidthCount = u_HeadWidth;
   	       			pDC->MoveTo(WidthCount, 0);
		   			pDC->LineTo(WidthCount, u_RowWidth - 1);
        		}
        		else
        			WidthCount = 0;
	       	}
            else    
            {
	       		WidthCount += u_ColWidth[i];
				pDC->MoveTo(WidthCount, 0);
	   			pDC->LineTo(WidthCount, u_RowWidth - 1);
            }
            
            CRect RectTemp;
            RectTemp.left = WidthCount + 1;
            RectTemp.top = 0;
            RectTemp.right = WidthCount + u_ColWidth[i + 1] - 1;
            RectTemp.right = min(RectTemp.right, RightEdge);
            RectTemp.bottom = RectTemp.top + u_RowWidth - 2;
            if (!IsPageMode && u_bFixedButtoned)
            	DrawBtnEdge(pDC, &RectTemp);
                                               
            //RectTemp.top  =  cGridInitY;
            RectTemp.left += u_GridInitX - 1;
            RectTemp.bottom++;
				
			StrTemp = GetFixedRowLabelAt(i + 1);
			pDC->SetTextColor(u_rgbFG);
			pDC->DrawText(StrTemp, StrTemp.GetLength(),	RectTemp, 
						  DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);
		}

		pDC->SelectObject((CPen*)&Pen3); 
		pDC->MoveTo(0, u_RowWidth - 1);
		pDC->LineTo(RightEdge, u_RowWidth - 1);
    }  
    
    ///////// Release or Deselect GDI Object /////////////////////////
    
    pDC->SelectObject(OldBrush);
    NewBrush.DeleteObject();  

    pDC->SelectObject((CPen*)OldPen);
    PenH.DeleteObject(); 
	PenV.DeleteObject(); 
    Pen3.DeleteObject();
}

void CUIGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	u_bLButtonUp = FALSE;
    LButtonDown(point, FALSE);
	CWnd::OnLButtonDown(nFlags, point);
	//CheckCursor(point);
} 

void CUIGrid::LButtonDown(CPoint point, BOOL Ext, BOOL NotSetFocus)
{
	if(u_Dragable && !Ext)
	{
		DragLinePos(point);
		u_Dragable = FALSE;
	    return;
	} 

	if (!NotSetFocus)
	{
		CString Str;
		if (u_pComboEdit != NULL)
		{
			u_pComboEdit->GetWindowText(Str);
			u_pComboEdit->ShowDropDown(FALSE);
			u_pComboEdit->SetWindowText(Str);
		}
		SetFocus();
		if (GetFocus() != this)
			return;
	}

    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
    
    int PosY = point.y / u_RowWidth;
	int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);
    
    if (PosY == 0 && u_bWithFixedRow 
    	&& !((point.x < u_HeadWidth && u_SelType == cGridHeadSel)
    	      || point.x > GetRightEdge()))
    {
        TitleButtonPressed(point);
        return;
    }

	if (NotCare(point, !Ext))
	{
		return;   
    }
    
    UpdateWindow();         //for note edit
    
    //These parameters are used to send message WM_GRD_UPDATED
    u_bGrdUpdated = FALSE;
    int PreRow = u_PreRow;
    int PreCol = u_PreCol;

    int ClickedRow = PosY + StartLine - IntFixedRow;
    int ClickedCol = GetPosXIndex(point.x);

    if (ClickedRow >= 0 && ClickedCol >= 0)
    {
 		GetMSGWnd()->SendMessage(WM_GRD_CLICK, (WPARAM)this,
    							 MAKELONG(ClickedCol, ClickedRow));
    }
    
    switch (u_SelType)
    {
     	case cGridHeadSel:	
     	{   
   	        if (point.x <= u_HeadWidth)
   			{   
     	    	WriteSelCellBack();

				if (u_bToggleSel && u_bMultiSel)
					SelOrDeselARow(ClickedRow + u_FrameTop);
				else
					SelectRows(point);
				
   	    	    if (u_bGrdUpdated)
			    {
			    	OnGrdUpdated(PreRow, PreCol);

					DWORD lParam = MAKELONG(PreCol, PreRow);
			    	GetMSGWnd()->SendMessage(WM_GRD_UPDATED, (WPARAM)this, lParam); 
			    }
     	    	return;
     	    }
     	    break;	
     	}
     	    
     	case cGridRowSel:
   		{       
     	    WriteSelCellBack();

			if (u_bToggleSel && u_bMultiSel)
				SelOrDeselARow(ClickedRow + u_FrameTop);
			else
				SelectRows(point);

			return;
        }
        
      	case cGridNonSel:	
            break;
    }       
    
    //Unselect the region
 	CDC* pDC = GetDC();
	if (u_SelType != cGridNonSel)
	{
 		if (!u_bMultiSel)
			PreRemoveSelRow(-1);

		if (!u_bMultiSel || !u_bToggleSel)
			DrawSelRegion(pDC, u_SelectedRow, u_EndSelectedRow, TRUE);
		else 
		{
			BOOL bRePaint = FALSE;
			if (u_pSelArray->GetSize() != 0)
				bRePaint = TRUE;

			PreRemoveSelRow(-1);
			long nPos = u_VirtualScrollPos;
			DrawSelRows(pDC, nPos, nPos + u_Rows, TRUE);
			if (bRePaint)
				RePaint();
		}
	}

 	if (u_PreRow >= 0 && u_PreCol >= 0)        
    	EraseSel(u_PreRow, u_PreCol);

    if (u_VirtualPreRow >= 0 && u_PreCol >= 0)
    {
  		if (u_CtrlShown)
  		{
  			if (IsCellCombo(u_PreCellType))
  				((CComboBox*)u_pComboOrEdit)->ShowDropDown(FALSE);

			UpdateGrdData();
            UpdateWindow();
		}
	}  

    u_PreCellStr = GetCellAt(ClickedRow, ClickedCol);
    u_PreCellType = GetAttribAt(ClickedRow, ClickedCol, cType);
   	if (u_PreCellType == cGridEditBtn)
   		DrawSel(pDC, ClickedRow, ClickedCol, TRUE);
   	else if (u_PreCellType != cGridNote && u_PreCellType != cGridHyperLink && u_PreCellType != cGridHyperBMP)
  		DrawSel(pDC, ClickedRow, ClickedCol);

	ReleaseDC(pDC);

    u_PreRow = ClickedRow;
    u_PreCol = ClickedCol;
    u_VirtualPreRow = u_PreRow + u_FrameTop;
    u_SelectedRow = -1; 
    u_EndSelectedRow = -1;

	if (!Ext && (u_bDragCell || u_PreCellType == cGridEditBtn ||
				 u_PreCellType == cGridNote))
	{
		DragCell(point, ClickedRow, ClickedCol);
	}
	else
		ClickCell(point, ClickedRow, ClickedCol);	
	
    //send message WM_GRD_UPDATED
    if (u_bGrdUpdated)
    {
    	OnGrdUpdated(PreRow, PreCol);
		
		DWORD lParam = MAKELONG(PreCol, PreRow);
    	GetMSGWnd()->SendMessage(WM_GRD_UPDATED, (WPARAM)this, lParam); 
    }
    
    GetMSGWnd()->SendMessage(WM_GRD_CLICKED, (WPARAM)this
   				, MAKELONG(ClickedCol, ClickedRow));
	
	PostLButtonDown(ClickedRow, ClickedCol);
}

void CUIGrid::PostLButtonDown(int ClickedRow, int ClickedCol)
{
}

void CUIGrid::DragCell(CPoint point, int ClickedRow, int ClickedCol)
{
  	CString StrContent = GetDataAt(ClickedRow, ClickedCol);
    char GridType = GetAttribAt(ClickedRow, ClickedCol, cType);
	char nIndex = GetAttribAt(ClickedRow, ClickedCol, cIndex);
    CRect RectCell;
    BOOL bNoteInRect = FALSE;
    BOOL DragFlag = FALSE;
  	int MoveCount = 0;
	BOOL bBtnRight = (GridType == cGridEditBtn 
				     || GridType == cGridComboBtn
    				 || GridType == cGridComboEditBtn);
    if (u_bBtnClickable && (GridType == cGridNote || bBtnRight))
    {
        RectCell = RowColToRect(ClickedRow, ClickedCol);
        RectCell.left ++;
        RectCell.top ++;

	    int Width = RectCell.Width() - u_GridInitX * 2;
    	int BtnWidth = min(Width, u_RowWidth);
	    CRect RectBtn = RectCell;
	    RectBtn.top += 2;
	    RectBtn.bottom -= 2;
        char Align = GetAttribAt(ClickedRow, ClickedCol, cAlign);
	    if (GridType == cGridNote && Align != 0)
	        RectBtn.left += (Width - BtnWidth) / 2;
	    else
        	RectBtn.left += u_GridInitX;	
	    RectBtn.right = RectBtn.left + BtnWidth;
        if (RectBtn.PtInRect(point))
        {
	        CDC* pDC = GetDC();
	        if (GridType == cGridNote)                         
	        {
	        	DrawNoteBtn(pDC, RectCell, StrContent.GetLength() > 0, 
							FALSE, FALSE, Align);
	        }
	        else
	        {
	        	BOOL bCanPress = TRUE;
				if (!bCanPress)
	        	{
	        		ReleaseDC(pDC);
                    return;
	        	}
	        	DrawNoteBtn(pDC, RectCell, bCanPress, FALSE, FALSE, 0, nIndex);
	        }
	        ReleaseDC(pDC);  
	        bNoteInRect = TRUE;
        }
    }

  	::SetCapture(m_hWnd);
    for (;;)
    {
    	::WaitMessage();  
    	
    	MSG msg;
    	if (u_bLButtonUp || 
    	    ::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
    	{      
        	if (msg.message == WM_LBUTTONUP || u_bLButtonUp)
        	{
                ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
              	::ReleaseCapture(); 
                
                CRect RectCell = RowColToRect(ClickedRow, ClickedCol);
                
                CPoint Pt;
                if (u_bLButtonUp)
                	Pt = u_LButtonPoint;
                else
                {	
                	POINTS points = MAKEPOINTS(msg.lParam);
					Pt.x = points.x;
                	Pt.y = points.y;
                }
                	
                if (bBtnRight && bNoteInRect)
                {
                   	if (bNoteInRect && u_bBtnClickable)
                    {
				        RectCell.top++;
				        RectCell.left++;
				        CDC* pDC = GetDC();
				        DrawNoteBtn(pDC, RectCell, TRUE, TRUE, FALSE, 0, nIndex);
				        ReleaseDC(pDC);
	            		
	            		GetMSGWnd()->SendMessage(WM_GRD_BTN_DOWN, (WPARAM)this
	            						, MAKELONG(ClickedCol, ClickedRow));
                   	}
                   	return;
                }
                else if (RectCell.PtInRect(Pt) || GridType == cGridNote)            
                {
                  	if (GridType == cGridNote)
                  	{
                  		if (bNoteInRect)
                    		ClickCell(point, ClickedRow, ClickedCol, TRUE);
                    }
                    else
                    	ClickCell(point, ClickedRow, ClickedCol, FALSE);
                    	
                    return;
                }
                    
   	            CRect RectGrid;
                GetWindowRect(RectGrid);
                Pt.x += RectGrid.left;
                Pt.y += RectGrid.top;
                
                CPoint CellPos;
                CellPos.x = ClickedCol;
                CellPos.y = ClickedRow;                     
                
                CObArray pObArray;
                pObArray.Add((CObject*)&Pt);
                pObArray.Add((CObject*)&StrContent); 
                pObArray.Add((CObject*)&CellPos);
                GetMSGWnd()->SendMessage(WM_RBUTTON_DRAGDROP, (WPARAM)this
                					, (LPARAM)&pObArray);

				break;
    	  	}  
    	  	else if (msg.message == WM_MOUSEMOVE)
    	  	{
                if (GridType != cGridNote && u_bDragCell)
                {
                    if (!bNoteInRect)
                    {
		                if (!DragFlag)
		                {
		                	if (MoveCount > 0)
		                		DragFlag = TRUE;
		    	            MoveCount++; 
		    	            continue;
		    	        }
		  	  	    	::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAG_DROP));
                    }
                }
    	  	}
    	}
	}
} 

void CUIGrid::ClickCell(CPoint point, int ClickedRow, int ClickedCol, BOOL bForced)
{
	if (!u_bEditable)
    	return;

	if (!GetAttribAt(ClickedRow, ClickedCol, cEditable) && !bForced)
        return;
        
    CString GridCell = GetDataAt(ClickedRow, ClickedCol);
        
    char nIndex = GetAttribAt(ClickedRow, ClickedCol, cIndex);
	char CellType = GetAttribAt(ClickedRow, ClickedCol, cType);
	char nTextLimited = GetAttribAt(ClickedRow, ClickedCol, cTextLimit);
	char nLimitReached = GetAttribAt(ClickedRow, ClickedCol, cLimitReached);
	char nAllowSeq = GetAttribAt(ClickedRow, ClickedCol, cAllowedChar);
	
	CString StrAllowedText = "";
	if (u_pAllowedArray != NULL)
		if (nAllowSeq >= 0 && nAllowSeq < u_pAllowedArray->GetSize())
			StrAllowedText = u_pAllowedArray->GetAt(nAllowSeq);

	CRect ComboEditRect = PointToRect(point); 
    int Width = ComboEditRect.Width() - 1 - u_GridInitX * 2;
	int BtnWidth = min(Width, u_RowWidth);
    switch (CellType)
    {
    	case cGridBMP:
    	{
    	 	if (u_PreRow == ClickedRow && u_PreCol == ClickedCol)
            {
	 		    u_BMPModified = TRUE;
	 			DrawNextBMP(ClickedRow, ClickedCol, GridCell, nIndex); 
	 			
				OnGrdUpdate(u_PreRow, u_PreCol);
				
				DWORD lParam = MAKELONG(u_PreCol, u_PreRow);
	   			GetMSGWnd()->SendMessage(WM_GRD_UPDATE, (WPARAM)this, lParam);
	   			
				u_bGrdUpdated = TRUE;
	 		}
	 		else                     
	 		{
	    	    u_BMPModified = FALSE;
				CDC* pDC = GetDC();
	    		DrawSel(pDC, ClickedRow, ClickedCol);
				ReleaseDC(pDC);
	    	}
	    	break;	
        }
            
		case cGridText:
    	case cGridEditBtn:
        {
            if (CellType == cGridEditBtn)
           		if (BtnWidth < u_RowWidth)
           			return;
            if (u_pEdit == NULL)
            {
		       	u_pEdit = new CGridEdit;
		       	RECT rc = {0, -100, 100, -10};
		       	DWORD dwStyle = WS_CHILD | ES_LEFT | ES_AUTOHSCROLL;
				if (u_bMultilineEdit)
					dwStyle |= ES_MULTILINE;
				BOOL bCreate = u_pEdit->Create(dwStyle, rc, this, ID_GRID_EDIT, FALSE); 
		       	u_pEdit->SetFont(u_pFont);
		    }   	
    		u_pEdit->SetTextLimit(nTextLimited);
			u_pEdit->SetLimitReached(nLimitReached);
			u_pEdit->SetAllowedChar(StrAllowedText);
			u_pComboOrEdit = u_pEdit;

            if (CellType == cGridText)
			{
				ComboEditRect.left += 1;//u_GridInitX;           
			}
			else
				ComboEditRect.left += (BtnWidth + u_GridInitX * 3);
			ComboEditRect.top += 1;//cEditGapTop;
			ComboEditRect.bottom = ComboEditRect.top + u_RowWidth - 1;// - cEditGapBottom; 
			ComboEditRect.right -= 1; 
       		u_pEdit->SetWindowText(GridCell);

            break;
        }
                
		case cGridPassword:
        {
            if (u_pPassEdit == NULL)
            {
		       	u_pPassEdit = new CGridEdit;
		       	RECT rc = {0, -100, 100, -10};
		       	BOOL bCreate = u_pPassEdit->Create(WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD,// | ES_WANTRETURN | ES_AUTOVSCROLL | ES_MULTILINE,
				                               rc, this, ID_GRID_EDIT, FALSE); 
		       	u_pPassEdit->SetFont(u_pFont);
		    }   	
    		u_pPassEdit->SetTextLimit(nTextLimited);
			u_pPassEdit->SetLimitReached(nLimitReached);
			u_pPassEdit->SetAllowedChar(StrAllowedText);
			u_pComboOrEdit = u_pPassEdit;

			ComboEditRect.left += u_GridInitX;           
			ComboEditRect.top += cEditGapTop;
			ComboEditRect.bottom = ComboEditRect.top + u_RowWidth - cEditGapBottom; 
			ComboEditRect.right -= 1; 
       		u_pPassEdit->SetWindowText(GridCell);

            break;
        }

		case cGridComboEdit:
    	case cGridCombo: 
    	case cGridBMPCombo:
    	case cGridComboDB:
    	case cGridComboEditDB:
		{
			if (CellType == cGridComboEdit || CellType == cGridComboEditDB)
			{
                if (u_pComboEdit == NULL)
                {
					u_pComboEdit = new CGridComboEdit;
					u_pComboEdit->u_Height = u_RowWidth - u_ComboGap;
					u_pComboEdit->Create(WS_CHILD | WS_TABSTOP | 
						CBS_DROPDOWN | WS_VSCROLL | ES_AUTOHSCROLL | 
						CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
						CRect(0,0,0,0), this, ID_GRID_COMBOEDIT); 
				   	u_pComboEdit->SetFont(u_pFont);
					u_pComboEdit->u_bNoDrawIfOne = FALSE;
                }
    			u_pComboEdit->SetTextLimit(nTextLimited);
				u_pComboEdit->SetLimitReached(nLimitReached);
				u_pComboEdit->SetAllowedChar(StrAllowedText);
				u_pComboOrEdit = u_pComboEdit;
              	if (CellType == cGridComboEditDB)
              		SetDBComboContent(u_pComboEdit, nIndex, GridCell);
              	else	
              		SetComboContent(u_pComboEdit, nIndex, GridCell);
            }
			else if (CellType == cGridCombo || CellType == cGridComboDB)
			{
				if (u_pCombo == NULL)
                {
				    u_pCombo = new CGridComboEdit;
					u_pCombo->u_Height = u_RowWidth - u_ComboGap;
					u_pCombo->SetDropDown(FALSE);
				   	u_pCombo->Create(WS_CHILD | WS_TABSTOP | 
									 CBS_DROPDOWNLIST | WS_VSCROLL | ES_AUTOHSCROLL | 
									 CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
									 CRect(0,0,0,0), this, ID_GRID_COMBO); 
				   	u_pCombo->SetFont(u_pFont);
					u_pCombo->u_bNoDrawIfOne = FALSE;
                }
				u_pComboOrEdit = u_pCombo;
               	u_pCombo->SetBGColor(GetCellColor(ClickedRow, ClickedCol, cBGColor));
				u_pCombo->SetFGColor(GetCellColor(ClickedRow, ClickedCol, cFGColor));
				if (CellType == cGridCombo)
               		SetComboContent(u_pCombo, nIndex, GridCell);
               	else
               		SetDBComboContent(u_pCombo, nIndex, GridCell);	
            }
			else
			{
				if (u_pBMPCombo == NULL)
				{    
				    u_pBMPCombo = new CUICombo;
				   	u_pBMPCombo->u_pBMPArray = u_pBMP;
					u_pBMPCombo->u_pBMPIDArray = u_pBMPIDArray;
					u_pBMPCombo->u_InitX = u_GridInitX;
				   	u_pBMPCombo->Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL
				   					 | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS,
									 CRect(0,0,0,0), this, ID_GRID_BMP_COMBO);
				}
				u_pBMPCombo->u_Align = GetAttribAt(ClickedRow, ClickedCol, cAlign);
  				u_pBMPCombo->u_pBrush = GetBrush(ClickedRow, ClickedCol);
				u_pBMPCombo->u_rgbFG = GetCellColor(ClickedRow, ClickedCol, cFGColor);
				SetBMPComboContent(nIndex, GridCell);
				u_pComboOrEdit = u_pBMPCombo;	
            }

			((CComboBox*)u_pComboOrEdit)->SetItemHeight(-1, u_RowWidth - u_ComboGap);				  
       		int Count = ((CComboBox*)u_pComboOrEdit)->GetCount(); 
       		if (Count > cComboCount)
       			Count = cComboCount;
    		int Height = ((CComboBox*)u_pComboOrEdit)->GetItemHeight(0);
   			ComboEditRect.bottom = ComboEditRect.top 
   								+ Count * Height + cComboExtraHeight; 
            ComboEditRect.right++; 
            EraseSel(u_PreRow, u_PreCol); 
            break;
        }
		case cGridNote:
		{
			char Align = GetAttribAt(ClickedRow, ClickedCol, cAlign);
	        ComboEditRect.top++;
	        ComboEditRect.left++; 
	        CDC* pDC = GetDC();
	        DrawNoteBtn(pDC, ComboEditRect, GridCell.GetLength() > 0, TRUE, FALSE, Align);
			DrawSel(pDC, ClickedRow, ClickedCol);	
		    ReleaseDC(pDC);
	
        	GetMSGWnd()->SendMessage(WM_GRD_NOTE, (WPARAM)this, MAKELONG(ClickedCol, ClickedRow));
		    break;
		}
   	}		
	
	if (CellType != cGridBMP && CellType != cGridNote && CellType != cGridHyperLink && CellType != cGridHyperBMP)
	{
   		u_pComboOrEdit->SetWindowPos(&wndTop, ComboEditRect.left, ComboEditRect.top,
			ComboEditRect.Width(), ComboEditRect.Height(), SWP_NOACTIVATE);
		if (CellType == cGridText || CellType == cGridEditBtn)
			u_pEdit->SetSel(0, -1, TRUE);
		else if (CellType == cGridPassword)
			u_pPassEdit->SetSel(0, -1, TRUE);

		u_pComboOrEdit->SetFocus();
        u_pComboOrEdit->ShowWindow(SW_SHOW);

		u_CtrlShown = TRUE;	 
	}
	
	GetMSGWnd()->SendMessage(WM_GRD_CTRL_ON, (WPARAM)this, MAKELONG(ClickedCol, ClickedRow));
}

BOOL CUIGrid::NotCare(CPoint point, BOOL bCheckRight)
{
    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
    
    int PosY = point.y / u_RowWidth;
    int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);
    
    if (StartLine + PosY >= u_pStrArray->GetSize() + IntFixedRow)
    	return TRUE;

    if (point.x < u_HeadWidth && u_SelType == cGridHeadSel)
    { 
    	if (u_bWithFixedRow && point.y < u_RowWidth) 
    		return TRUE;  
    	return FALSE;	
    }
    if (point.x < u_HeadWidth && u_bWithFixedCol) 
    	return TRUE;

    if (point.x > GetRightEdge() && bCheckRight) 
      	return TRUE; 

    return FALSE;  	
}

void CUIGrid::SetComboContent(CComboBox* pCombo, int Index, CString GridCell)
{
	pCombo->ResetContent();
    if (pCombo == u_pComboEdit)
    	pCombo->SetWindowText(GridCell);
    
	if (u_pComboArray == NULL)
		return;
	if (Index < 0 || Index >= u_pComboArray->GetSize())
		return;
	CString ComboStr = u_pComboArray->GetAt(Index);

	char* pToken = new char[1024];
	if (u_pComboStrArray == NULL)
		u_pComboStrArray = new CObArray;
	else
		::ReleaseObArray(u_pComboStrArray);
	u_nFieldCount = -1;
	int nCount = 0;
	while (::GetToken(ComboStr, pToken, &nCount, '\\'))
	{
		char* p; 
		if ((p = strtokex(pToken, ",")) == NULL)
			p = pToken;
		CStringArray* pStrArray = new CStringArray;
		u_pComboStrArray->Add(pStrArray);
		for (;;)
		{
			pStrArray->Add(p);
			if ((p = strtokex(NULL, ",")) == NULL)
			{
				if (u_nFieldCount == -1)
				{
					u_nFieldCount = pStrArray->GetSize();
					u_nComboMajor = u_nFieldCount - 1;
				}
				break;
			}
		}
	}

	nCount = u_pComboStrArray->GetSize();
	CString StrTemp; 
	for (int i = 0; i < nCount; i++)
	{
		StrTemp = ::GetRowColAt(u_pComboStrArray, i, u_nComboMajor);
		pCombo->AddString(StrTemp);
		if (pCombo == u_pCombo && GridCell == StrTemp)
       		pCombo->SetCurSel(i);
	}

	((CGridComboEdit*)pCombo)->u_pComboArray = u_pComboStrArray;
	((CGridComboEdit*)pCombo)->SetColNo(u_nFieldCount);
	
	delete [] pToken;
}

void CUIGrid::SetDBComboContent(CComboBox* pCombo, int Index, CString GridCell)
{
/*	
	pCombo->ResetContent();
    if (pCombo == u_pComboEdit)
    	pCombo->SetWindowText(GridCell);
            	
   	if (u_pTableFieldArray == NULL)
		return;
	if (Index < 0 || Index >= u_pTableFieldArray->GetSize())
		return;

	CString StrTableField = u_pTableFieldArray->GetAt(Index);
            
    char Token[255];
    int Count = 0;              
    int i = 0;
    CString StrTableName;
    CString StrFieldName;

    ::GetToken((const char*)StrTableField, Token, &Count);
   	StrTableName = Token;
    	
    ::GetToken((const char*)StrTableField, Token, &Count);
    StrFieldName = Token;
    	
    CObArray* pData = new CObArray;

    BeginWaitCursor();
    u_pDBObj->Query(StrTableName, "DISTINCT " + StrFieldName, "", "", NULL, pData);
    EndWaitCursor();
    
    int nCount = pData->GetSize();
    CString StrCand;
    for (i = 0; i < nCount; i++)
    {
    	CStringArray* pStrList = (CStringArray*)pData->GetAt(i);
    	StrCand = pStrList->GetAt(0);
		//StrCand.TrimRight();
    	pCombo->AddString(StrCand);
        if (pCombo == u_pCombo && GridCell == StrCand)
        	pCombo->SetCurSel(i);
    }
    
    ::ReleaseObArray(pData);
    delete pData;	
*/
}

void CUIGrid::SetBMPComboContent(int Index, CString GridCell)
{
    u_pBMPCombo->ResetContent();

   	if (u_pBMPArray == NULL)
		return;
	if (Index < 0 || Index >= u_pBMPArray->GetSize())
		return;
	
    CString ComboStr = u_pBMPArray->GetAt(Index);
            
    char Token[20];
    int Count = 0;              
    int i = 0;
    while(::GetToken((const char*)ComboStr, Token, &Count))
    {
    	int nId = atoi(Token);
		u_pBMPCombo->u_BMPId = nId;   //Done for the abnormal condition of MeasureItem
    	u_pBMPCombo->AddString((LPCTSTR)Token);
        if (strcmp((const char*)GridCell, Token) == 0)
        	u_pBMPCombo->SetCurSel(i);
        i++;	
    }	
}

int CUIGrid::GetNextBMPIndex(CString CurrentShow, int Index)
{
    CString BMPCycle = u_pBMPArray->GetAt(Index);

    char* pToken = new char[BMPCycle.GetLength()];
   	int Count = 0; 
   	while(::GetToken((const char*)BMPCycle, pToken, &Count))
   	{
   		if (strcmp(pToken, (const char*)CurrentShow) == 0)
   		{   
   			if (Count == (int)strlen((const char*)BMPCycle))
   				Count = 0;
   		 	::GetToken((const char*)BMPCycle, pToken, &Count);
   		 	break;
   		}
   	}	          

   	int BMPIndex = atoi(pToken);
	delete [] pToken;

	return BMPIndex;
}

void CUIGrid::DrawNextBMP(int nRow, int nCol, CString CurrentShow, int Index)
{
   	int BMPIndex = GetNextBMPIndex(CurrentShow, Index);
	CString strTmp;
	strTmp.Format("%d", BMPIndex);
    //Update Current BMP
	SetDataAt(nRow, nCol, strTmp);
		
    CRect CellRect = RowColToRect(nRow, nCol);    

    RECT RectInside = {CellRect.left + 1,
       				   CellRect.top + 1,
       				   CellRect.right,
       				   CellRect.bottom};

	CDC* pDC = GetDC();                         
    ::FillRect(pDC->m_hDC, &RectInside, 
    		   (HBRUSH)::GetStockObject(WHITE_BRUSH));
    DrawBMP(pDC, CellRect, BMPIndex);
    ReleaseDC(pDC);
}

void CUIGrid::DrawSelect(CDC* pDC, int SelectedRow)
{
    if (SelectedRow < 0 || (SelectedRow == 0 && u_bWithFixedRow)) 
    	return;
    	
  	CPen Pen1;

   	CPen* OldPen;
	Pen1.CreatePen(PS_SOLID, 1, u_rgbSelRows);
   	OldPen = pDC->SelectObject((CPen*)&Pen1);    
  
   	CBrush BrushBk;
	BrushBk.CreateSolidBrush(u_rgbSelRows);

	CBrush* pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->SetROP2(R2_NOTXORPEN);
	
    int InitPos = 0;
    if (u_SelType == cGridHeadSel ||u_bWithFixedCol)
    	InitPos = u_HeadWidth + 1;
        
   	pDC->Rectangle(InitPos, 
   				   SelectedRow * u_RowWidth, 
   				   GetRightEdge(), 
				   (SelectedRow + 1) * u_RowWidth);  

   	pDC->SelectObject((CPen*)OldPen);
   	Pen1.DeleteObject(); 

   	pDC->SelectObject(pOldBrush);
	BrushBk.DeleteObject();
}

BOOL CUIGrid::UpdateGrdData()
{
	if (!u_CtrlShown)
		return FALSE;
	if (u_VirtualPreRow < 0 || u_PreCol < 0)
		return FALSE; 	
	if (!IsCellComboOrEdit(u_PreCellType))
		return FALSE;	
	
	BOOL DataUpdated = FALSE;

  	BOOL bComboNoMatch = FALSE;
  	CString EdString;
  	if (u_PreCellType == cGridCombo 
  		|| u_PreCellType == cGridComboBtn
  		|| u_PreCellType == cGridComboDB)
  	{
  	    int nCurSel = u_pCombo->GetCurSel();  // ABY Lu
  	    if (nCurSel != CB_ERR)
  	    	u_pCombo->GetLBText(nCurSel, EdString);
  	    else
  	    	bComboNoMatch = TRUE;	
  	} 
  	else if (u_PreCellType == cGridBMPCombo)
  	{
  	    int nCurSel = u_pBMPCombo->GetCurSel();  // ABY Lu
  	    if (nCurSel != CB_ERR)
  	    	u_pBMPCombo->GetLBText(nCurSel, EdString);
  	    else
  	    	bComboNoMatch = TRUE;	
  	} 
  	else if (u_PreCellType == cGridComboEdit)
	{
		BOOL bPartial = TRUE;
		CString StrCombo;
		CString StrMatch;
		int nSize = u_pComboStrArray->GetSize();
  		u_pComboEdit->GetWindowText(EdString);
		int nStrEditSize = EdString.GetLength();
		int nIndex = -1;
		if (EdString != "")
		{
			for (int i = 0; i < nSize; i++)
			{
				StrCombo = ::GetRowColAt(u_pComboStrArray, i, 0);
				int nStrComboSize = StrCombo.GetLength();
				if (nStrComboSize < nStrEditSize)
					continue;
				StrMatch = (bPartial) ? StrCombo.Left(nStrEditSize) : StrCombo;
				if (StrMatch == EdString)
				{
					nIndex = i;
					break;
				}
			}
		}

		if (nIndex != -1)
		{
			int j = u_nFieldCount - 1;
			EdString = ::GetRowColAt(u_pComboStrArray, nIndex, j);
		}
	}
	else
  	{
  		u_pComboOrEdit->GetWindowText(EdString);
    }
            
	CString GridCell = (const char*)u_PreCellStr + cAttribNo;

	char CellType = u_PreCellStr.GetAt(cType) - 1;
   	if (CellType == cGridText || CellType == cGridPassword ||
		(!bComboNoMatch || CellType == cGridEditBtn))  
   	{
   	    if (EdString != GridCell)
		{
    		u_DataBeforeUpdate = GridCell;
			if (PreGrdUpdate(u_PreRow, u_PreCol, u_DataBeforeUpdate, EdString))
			{
				if (u_PreRow >= 0 && u_PreCol >= 0)
					SetDataAt(u_PreRow, u_PreCol, EdString);
  	    		DataUpdated = TRUE;
  	    	
				OnGrdUpdate(u_PreRow, u_PreCol);
			
				DWORD lParam = MAKELONG(u_PreCol, u_PreRow);
  	    		GetMSGWnd()->SendMessage(WM_GRD_UPDATE, (WPARAM)this, lParam); 
  	    	
				u_bGrdUpdated = TRUE;
			}
		}
		EraseSel(u_PreRow, u_PreCol);	
	}	
 
	if (IsCellCombo(u_PreCellType))
		((CComboBox*)u_pComboOrEdit)->ShowDropDown(FALSE);

    u_pComboOrEdit->ShowWindow(SW_HIDE);
    u_CtrlShown = FALSE;
            
	int PreRow = u_PreRow;
	int PreCol = u_PreCol;

	u_PreRow = -1;
  	u_PreCol = -1;   	   
	u_VirtualPreRow = -1;

   	GetMSGWnd()->SendMessage(WM_GRD_LEAVE_CELL, (WPARAM)this,
   							 MAKELONG(PreCol, PreRow));
    
    return DataUpdated;
}      

int CUIGrid::GetPosXIndex(int PosX)
{
    int Count = GetAheadSpace();
    int StartCol = GetStartCol(GetScrollPos(SB_HORZ));
    int i;
    for (i = StartCol; i < u_Cols; i++)
    {
      	if (PosX >= Count && PosX <= Count + (int)u_ColWidth[i])
        	return i;
       	Count += u_ColWidth[i];
    }
    return -1;
}  

void CUIGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar == GetScrollBarCtrl(SB_VERT)); 
	OnScroll(SB_VERT, nSBCode, nPos);
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CUIGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar == GetScrollBarCtrl(SB_HORZ)); 
	OnScroll(SB_HORZ, nSBCode, nPos);
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CUIGrid::OnScroll(int nBar, UINT nSBCode, long nPos)
{
    ASSERT(nBar == SB_VERT || nBar == SB_HORZ);
    BOOL bHorz = (nBar == SB_HORZ);
    
    int zOrig, z; 
    int zMax, zMin;
    int nOneUnit;
    long VirtualScrollPosOrig = u_VirtualScrollPos;
     
    zOrig = z = GetScrollPos(nBar);
    GetScrollRange(nBar, &zMin, &zMax);
    nOneUnit = 1;
    ASSERT(zMin == 0);
    
    if (zMax <= 0)
    {
       ASSERT(z == 0);
       return;
    }         
    
	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
	int Interval;
	if (!bHorz)
	{
   		Interval = u_Rows - IntFixedRow;   
	    
	    int HorzMin, HorzMax;
	    GetScrollRange(SB_HORZ, &HorzMin, &HorzMax);  
	    if (!(HorzMin == 0 && HorzMax == 0))
	    	Interval -= 1;
    }   
    else
    	Interval = 3;
    
    switch (nSBCode)
    {
    case SB_TOP:
    		z = 0;
    		if (!bHorz)
    		  	u_VirtualScrollPos = 0;
    		break;
    		
    case SB_BOTTOM:
    		z = zMax;
     		if (!bHorz)
    		  	u_VirtualScrollPos = u_VirtualLastScrollPos;
    		break;
 
    case SB_LINEUP:
    		if (!bHorz)
    		{
    			u_VirtualScrollPos -= nOneUnit;
    			z = VirtualToScroll(u_VirtualScrollPos);
    		}
            else
            	z -= nOneUnit; 
    		break;
 
    case SB_LINEDOWN:
    		if (!bHorz)
    		{
    			u_VirtualScrollPos += nOneUnit;
    			z = VirtualToScroll(u_VirtualScrollPos);
    		}
            else
            	z += nOneUnit; 
    		break;       
    
    case SB_PAGEUP:
    		if (!bHorz)
    		{
    			u_VirtualScrollPos -= Interval;
    			z = VirtualToScroll(u_VirtualScrollPos);
    		}
            else
            	z -= Interval; 

    		break;
	
	case SB_PAGEDOWN:
     		if (!bHorz)
    		{
    			u_VirtualScrollPos += Interval;
    			z = VirtualToScroll(u_VirtualScrollPos);
    		}
            else
            	z += Interval; 

    		break;
	
	case SB_THUMBPOSITION: 
    		z = (int)nPos; 
           	if (!bHorz)
           	{
           		if (z != zMax)
            		u_VirtualScrollPos = ScrollToVirtual(z);
    			else
            		u_VirtualScrollPos = u_VirtualLastScrollPos;
    		}
    		break;  
    
    case SB_ABSOLUTE:
    		if (!bHorz)
    		{
    			z = VirtualToScroll(nPos);
    			u_VirtualScrollPos = nPos;
    		}
			else
				z = nPos;
    		break;
    
    default:
    		return;
    }
    
    if (z < 0)
    	z = 0;
    else if (z > zMax)
    	z = zMax;

    u_bNoUpdate= TRUE;

	if (u_VirtualScrollPos < 0)
		u_VirtualScrollPos = 0;
	else if (u_VirtualScrollPos > u_VirtualLastScrollPos)
		u_VirtualScrollPos = u_VirtualLastScrollPos;
    
    BOOL bNeedToMoveCtrl = FALSE;
    if ((bHorz && z != zOrig) || 
    	(!bHorz && u_VirtualScrollPos != VirtualScrollPosOrig))
    {
       	SetScrollPos(nBar, z, TRUE);
       
       	if (!bHorz)
       	{
       		u_ScrollPos = z;                                 
       		BOOL bScrollUp = u_VirtualScrollPos < u_FrameTop;
       		BOOL bScrollDown = u_VirtualScrollPos + u_Rows - IntFixedRow 
       						   > u_FrameTop + u_FrameSize;

            if (u_bVirtualPage && (bScrollUp || bScrollDown)) 
				FetchNewPage();
        }
           
       	CRect ClientRect;
       	GetClientRect(ClientRect);
        
   		if (u_PreRow >= 0 && u_PreCol >= 0 && 
			u_PreCellType!= cGridBMP && u_PreCellType!= cGridHyperBMP && 
			u_CtrlShown)
        {
  			if (IsCellCombo(u_PreCellType))
  				((CComboBox*)u_pComboOrEdit)->ShowDropDown(FALSE);
   			
			u_pComboOrEdit->ShowWindow(SW_HIDE);

   			bNeedToMoveCtrl = TRUE;	
        }
        
       	if (nBar == SB_VERT)
        {
    		int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
       		ClientRect.top += IntFixedRow * u_RowWidth;
       		int Delta = (int)(VirtualScrollPosOrig - u_VirtualScrollPos);
       		ScrollWindow(0, Delta * u_RowWidth, ClientRect, ClientRect);
       	}
       	else
       	{
   			ClientRect.left += GetAheadSpace() + 1;

       		int i; 
       		int TotalCount = 0;
       		int BeginIndex = GetStartCol(min(zOrig, z));// + IntFixedCol;
       		int EndIndex = GetStartCol(max(zOrig, z));//+ IntFixedCol;
	       	for (i = BeginIndex; i < EndIndex; i++)
	       		TotalCount += u_ColWidth[i];
            
       		TotalCount = (zOrig - z > 0) ? TotalCount : -TotalCount;
       		
       		ScrollWindow(TotalCount , 0, ClientRect, ClientRect);
       	}

       	if (bNeedToMoveCtrl)
       		MoveControlPos();        
       	UpdateWindow(); 
    }   

    u_bNoUpdate= FALSE;
}                                      

void CUIGrid::FetchNewPage()
{
	long lRow = max(0, u_VirtualScrollPos - (u_FrameSize - u_Rows) / 2);
    
	NeedData(lRow);
	GetMSGWnd()->SendMessage(WM_GRD_FETCHDATA, 0, lRow);

    u_FrameTop = lRow;
	if (u_VirtualPreRow >= u_FrameTop 
		&& u_VirtualPreRow <= u_FrameTop + u_FrameSize)
	{
	    u_PreRow = (int)(u_VirtualPreRow - u_FrameTop);
	}
	else
	{
	    u_PreRow = -1;	
  		if (IsCellCombo(u_PreCellType))
  			((CComboBox*)u_pComboOrEdit)->ShowDropDown(FALSE);
		if (u_CtrlShown)
		   	u_pComboOrEdit->ShowWindow(SW_HIDE);
	}
}

long CUIGrid::ScrollToVirtual(int i)
{
    long DataSize = GetDataSize();
    if (DataSize == 0)
    	return 0;
	long Divisor = min(DataSize,tMaxScrollRange);
	return (long)(i * ((double)DataSize / Divisor));
}

int	CUIGrid::VirtualToScroll(long l)
{	
    long DataSize = GetDataSize();
    if (DataSize == 0)
    	return 0;
	return (int)(l * ((double)min(DataSize, tMaxScrollRange) / DataSize));
}

void CUIGrid::SetVScrollPos(long VPos, BOOL bGo)
{
    if (bGo)
    	OnScroll(SB_VERT, SB_ABSOLUTE, VPos); 
    else
    {
		int z = VirtualToScroll(VPos);
		u_VirtualScrollPos = VPos;      
		u_ScrollPos = z;
      	SetScrollPos(SB_VERT, z, TRUE);
    }	
}

void CUIGrid::IncVScrollPos()
{
    OnScroll(SB_VERT, SB_LINEDOWN, 0);
}

void CUIGrid::DrawBMP(CDC* pDC, CRect RectCell, int Index, int bPrinting, int Align)
{
	if (u_pBMP == NULL)
		return;

	if (Index >= u_pBMP->GetSize())
    	return;
    	
    CBitmap* pBMP;
   	if (Index >= 0)
   		pBMP = ((CBitmap*)u_pBMP->GetAt(Index));
   	else
   	{
   	    if (u_pUnknownBMP == NULL)
   	    {
   	    	u_pUnknownBMP = new CBitmap;
   	    	u_pUnknownBMP->LoadBitmap(IDB_UNKNOWN);
   	    }	
   	    pBMP = u_pUnknownBMP;
   	}
    	
   	HBITMAP hBMP = (HBITMAP)pBMP->m_hObject; 

   	BITMAP BMPBuffer;
   	pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
   	int bmWidth = BMPBuffer.bmWidth;
   	int bmHeight = BMPBuffer.bmHeight;
    int DrawWidth = bmWidth;
    int DrawHeight = bmHeight;
    int RectWidth = RectCell.Width();
    int RectHeight = RectCell.Height();

    if (bmHeight > RectHeight - 2)
    {
        DrawHeight = RectHeight - 2;
        DrawWidth = (int)(bmWidth * ((float)DrawHeight / bmHeight));
    }
    if (DrawWidth > RectWidth - 2)
    {
    	DrawWidth = max(0, RectWidth - 2);
    }
    	 
    int Pos;
    switch (Align)
    {
        case 1:
        case 3:
     		Pos = RectCell.left + (RectWidth - DrawWidth) / 2;
     		break;
     	case 2:     //to be added;
     	case 0:
     		Pos = RectCell.left + u_GridInitX;
     		break;	
    }
    
    int XPos = max(0, Pos);                                    
    Pos = RectCell.top + (RectCell.Height() - DrawHeight) / 2;
    int YPos = max(0, Pos);
               
    int Delta = (bPrinting) ? 1 : 0;
   	RECT RectTemp = {0, 0, bmWidth, bmHeight};
   	RECT RectDraw = {XPos, 
					 YPos, 
   					 XPos + DrawWidth + Delta, 
					 YPos + DrawHeight + Delta};

  	if (bPrinting)
  	{   
  	    HANDLE hDIB = ::BitmapToDIB(hBMP, NULL);
  		::PaintDIB(pDC->m_hDC, &RectDraw, hDIB, &RectTemp, NULL);
  		::GlobalFree(hDIB);  
  	}
  	else
  		::DDBPaint(pDC->m_hDC, &RectDraw, hBMP, &RectTemp, NULL);  

}  


void CUIGrid::DrawNoteBtn(CDC* pDC, CRect RectCell, BOOL Enable, 
						  BOOL Up, int bPrinting, int nPos, int nIndex)
{

    if (RectCell.right <= RectCell.left)
    	return;
    	
    int Width = RectCell.Width() - ((nPos == 1 || nPos == 3) ? 0 : (u_GridInitX * 2));
    int BtnWidth = min(Width, u_RowWidth);
    if (BtnWidth < 0)
    	return;
    	
    RectCell.top += 3;
    RectCell.bottom -= 2;
    if (nPos == 1 || nPos == 3)
    	RectCell.left += (Width - BtnWidth) / 2;
    else if (nPos == 0)
    	RectCell.left += u_GridInitX;
    else
    	RectCell.left = RectCell.right - u_GridInitX - BtnWidth;	
    		
    RectCell.right = RectCell.left + BtnWidth;

   	CBitmap* pBMP;
	if (nIndex >= 0 && Enable)
	{
        if (u_pBtnBMP == NULL)
		{
		   	if (u_pNoteEnBMP == NULL)
    		{
    			u_pNoteEnBMP = new CBitmap;
    			u_pNoteEnBMP->LoadBitmap(IDB_NOTE_EN);     
    		}	
			pBMP = u_pNoteEnBMP;
		}
		else
			pBMP = (CBitmap*)u_pBtnBMP->GetAt(nIndex);	
    }
    if (nIndex < 0 && Enable)
    {
    	if (u_pNoteEnBMP == NULL)
    	{
    		u_pNoteEnBMP = new CBitmap;
    		u_pNoteEnBMP->LoadBitmap(IDB_NOTE_EN);     
    	}	
    	pBMP = u_pNoteEnBMP;
    }
    if (!Enable)
    {
    	if (u_pNoteDisBMP == NULL)
    	{
    		u_pNoteDisBMP = new CBitmap;          
    		u_pNoteDisBMP->LoadBitmap(IDB_NOTE_DIS);
    	}	
    	pBMP = u_pNoteDisBMP;
    }

   	HBITMAP hBMP = (HBITMAP)pBMP->m_hObject;
   	BITMAP BMPBuffer;
   	pBMP->GetObject(sizeof(BITMAP), &BMPBuffer); 
   	int bmWidth = BMPBuffer.bmWidth;
   	int bmHeight = BMPBuffer.bmHeight;

    int RectWidth = RectCell.Width();
    if (bmWidth > RectWidth - 4)
    	bmWidth = RectWidth - 4;
    	 
    int Pos = RectCell.left + (RectWidth - bmWidth) / 2;
    int XPos = max(1, Pos);                                    
    Pos = RectCell.top + (RectCell.Height() - bmHeight) / 2;
    int YPos = max(1, Pos);
               
   	RECT RectTemp = {0, 0, bmWidth, bmHeight};
   	RECT RectDraw = {XPos, YPos, 
   					 XPos + bmWidth, YPos + bmHeight};

    ::FillRect(pDC->m_hDC, RectCell, (HBRUSH)::GetStockObject(LTGRAY_BRUSH));
    if (!Up)
    {
        RectDraw.top++;
        RectDraw.bottom++;
        RectDraw.left++;
        RectDraw.right++;
    }
    
  	if (bPrinting)
  	{   
  	    RectDraw.bottom++;   //Use StretchBlt in ::PaintDIB
  	    RectDraw.right++;    //Use StretchBlt in ::PaintDIB
  	    HANDLE hDIB = ::BitmapToDIB(hBMP, NULL);
  		::PaintDIB(pDC->m_hDC, &RectDraw, hDIB, &RectTemp, NULL);
  		::GlobalFree(hDIB);  
  	}
  	else
  		::DDBPaint(pDC->m_hDC, &RectDraw, hBMP, &RectTemp, NULL);  

    ::DrawRect(pDC->m_hDC, RectCell);
    if (Up)
    {
	    RectCell.right -= 2;
	    RectCell.bottom -= 2;
	    RectCell.top++;
	    RectCell.left++;
	    DrawBtnEdge(pDC, RectCell);
    }
    else
    {
	    RectCell.top += 2;
	    RectCell.left += 2;
	    RectCell.bottom -= 2;
	    RectCell.right -= 2;
    	DrawBtnDown(pDC, RectCell);  
    }
}  

CRect CUIGrid::PointToRect(CPoint point)
{
    int StartCol = GetStartCol(GetScrollPos(SB_HORZ)); 
    int i;
    int Count = GetAheadSpace();
       
    CRect RectTemp;
    for (i = StartCol; i < u_Cols; i++)
    {
      	if (point.x >= Count && point.x <= Count + (int)u_ColWidth[i])
       	{
       	 	RectTemp.right = Count + u_ColWidth[i];
       	 	RectTemp.left = Count;  
       	 	break; 
       	}         
       	Count += u_ColWidth[i];
    } 
    
    RectTemp.top = point.y / u_RowWidth * u_RowWidth - 1;
    RectTemp.bottom = RectTemp.top + u_RowWidth;
    
    return RectTemp;
}  

void CUIGrid::PointToRowCol(CPoint point, int& nRow, int& nCol)
{
    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
    
    int PosY = point.y / u_RowWidth;
	int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);

    nRow = PosY + StartLine - IntFixedRow;
	if (nRow >= GetDataSize())
	{
		nRow = -1;
		nCol = -1;
	}
    else
		nCol = GetPosXIndex(point.x);

	return;
}  

CRect CUIGrid::RowColToRect(int nRow, int nCol)
{   
	CRect rcRet;
	rcRet.SetRectEmpty();
	
	int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);
	if (nRow < StartLine)
		return rcRet;	

    int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;

	CRect rcClient;
	GetClientRect(rcClient);
	int YTopPos = (nRow - StartLine + IntFixedRow) * u_RowWidth - 1; 
	int YBottomPos = YTopPos + u_RowWidth;
	if (rcClient.bottom < YTopPos)
		return rcRet;

   	int StartPos = GetScrollPos(SB_HORZ);
   	int StartCol = GetStartCol(StartPos);
   	int EndCol = GetEndCol(StartCol);     
   	if (nCol < StartCol || nCol > EndCol)
		return rcRet;

	int i;  
    int Count = GetAheadSpace();
    for (i = StartCol; i < nCol; i++)
       	Count += u_ColWidth[i];
    
 	rcRet.left = Count;  
 	rcRet.right = Count + u_ColWidth[nCol];
    rcRet.top = YTopPos;
    rcRet.bottom = YBottomPos;
    
    return rcRet;
}

void CUIGrid::EraseSel(int Row, int Col)
{
 	CRect RectTemp = RowColToRect(Row, Col);
 	
 	if (RectTemp.IsRectEmpty())
 		return;
    
    RectTemp.bottom++;
    RectTemp.right++;
    
    InvalidateRect(RectTemp);
	//RedrawWindow(RectTemp, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void CUIGrid::DrawSel(CDC* pDC, int Row, int Col, BOOL bShorter)
{   
	if (!u_bDrawSel)
		return;

	CRect RectTemp = RowColToRect(Row, Col);
 	
 	if (RectTemp.IsRectEmpty())
 		return;
    
 	if (bShorter)
    {
	    int Width = RectTemp.Width() - 1 - u_GridInitX * 2;
	  	int BtnWidth = min(Width, u_RowWidth);
		RectTemp.left += (BtnWidth + u_GridInitX * 2);
    }

 	CPen Pen1;
 	CPen* OldPen;
 	Pen1.CreatePen(PS_SOLID, 1, u_rgbSel);
	OldPen = pDC->SelectObject(&Pen1); 	

    pDC->MoveTo(RectTemp.left, RectTemp.top);
  	pDC->LineTo(RectTemp.left, RectTemp.bottom);	
    pDC->LineTo(RectTemp.right, RectTemp.bottom);
   	pDC->LineTo(RectTemp.right, RectTemp.top);
	pDC->LineTo(RectTemp.left, RectTemp.top);    
	
    pDC->SelectObject(OldPen);
    Pen1.DeleteObject();
} 

void CUIGrid::DrawBtnDown(CDC* pDC, LPRECT lpRect)
{   
	pDC->SetROP2(R2_COPYPEN);
	
   	CPen Pen1;
	Pen1.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNFACE));
	
	CPen Pen2;
    Pen2.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
    
	CPen* OldPen;
	OldPen = pDC->SelectObject(&Pen2);

	pDC->MoveTo(lpRect->left - 1, lpRect->bottom);
	pDC->LineTo(lpRect->left - 1, lpRect->top - 1);
	pDC->LineTo(lpRect->right, lpRect->top - 1);
	
	pDC->SelectObject(&Pen1); 
	pDC->LineTo(lpRect->right, lpRect->bottom);
	pDC->LineTo(lpRect->left- 1, lpRect->bottom);

	pDC->SelectObject(OldPen);

    Pen1.DeleteObject(); 
    Pen2.DeleteObject();
}

void CUIGrid::DrawBtnEdge(CDC* pDC, LPRECT lpRect)
{
   	::DrawPanel(pDC->m_hDC, lpRect); 
}
 

void CUIGrid::MoveControlPos()
{
    if (u_PreRow >= 0 && u_PreCol >= 0) 
    {     
        CRect RectTemp = RowColToRect(u_PreRow, u_PreCol);
        if (RectTemp.IsRectEmpty())
       		return;

	   	switch (u_PreCellType)
	   	{
	   		case cGridText:
	   		case cGridPassword:
			case cGridEditBtn:
		   	{
				RectTemp.top += cEditGapTop;
				RectTemp.bottom = RectTemp.top + u_RowWidth - cEditGapBottom; 
	        	RectTemp.left += u_GridInitX;           
        		RectTemp.right -= 1; 
	        	if (u_PreCellType== cGridEditBtn)
	        	{
				    int Width = RectTemp.Width() - 1 - u_GridInitX * 2;
			    	int BtnWidth = min(Width, u_RowWidth);
			    	RectTemp.left += (BtnWidth + u_GridInitX * 2);
	        	}		
                break;
	      	}
      		
      		default:
      		{   
      			CComboBox* pCombo = (CComboBox*)u_pComboOrEdit;
      			int Count = pCombo->GetCount();
      			int Height = pCombo->GetItemHeight(0);
   				RectTemp.bottom = RectTemp.top + Count * Height + cComboExtraHeight; 
      		}
        }
   		u_pComboOrEdit->SetWindowPos(&wndTop, RectTemp.left, RectTemp.top,
   			RectTemp.Width(), RectTemp.Height(), SWP_SHOWWINDOW);
   		u_CtrlShown = TRUE;  
   		u_pComboOrEdit->SetFocus();
    }                                                        
}

BOOL CUIGrid::IsCellClicked(int* pPreRow, int* pPreCol)
{
    if (u_PreRow != -1 && u_PreCol != -1
        && !((u_PreCellType== cGridText || u_PreCellType== cGridPassword)
		     && u_bCellMark)) 
    {
     	if (pPreRow != NULL)
     		*pPreRow = u_PreRow;
     	if (pPreCol != NULL)
     		*pPreCol = u_PreCol;
     	return TRUE;
    }	            
    return FALSE;
}

void CUIGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    GetMSGWnd()->SendMessage(WM_GRD_DBLCLICK, (WPARAM)this, 0);
	CWnd::OnLButtonDblClk(nFlags, point);  
} 


void CUIGrid::OnPaint()
{
	CPaintDC ptdc(this);
	CMemDC_MCG dc(&ptdc);
	//if (u_bFirst)
	//{
	//	SetRowsAndScrollBar();		
	//	u_bFirst = FALSE;
	//}

	OnDraw(&dc, u_pStrArray, &u_FrameTop);
}

int CUIGrid::AddColWidth(int StartCol, int EndCol)
{
    int Count = GetAheadSpace();
    for (int i = StartCol; i <= EndCol; i++)  
    {
        if (i == 0 && u_bWithFixedCol)
        	continue;
        Count += u_ColWidth[i];
    }
    return Count;    
}

void CUIGrid::KeyDown(UINT nChar)
{
    switch (nChar)
	{
		case VK_PRIOR:
			OnScroll(SB_VERT, SB_PAGEUP, 0);
			break;

		case VK_NEXT:
			OnScroll(SB_VERT, SB_PAGEDOWN, 0);
			break;

		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN: 
		case VK_RETURN:
			OnMoveCursorKeyDown(nChar);
			break;
	}
	GetMSGWnd()->SendMessage(WM_GRD_KEY_DOWN, (WPARAM)this, nChar);
}

void CUIGrid::OnMoveCursorKeyDown(UINT nChar)
{
    if (u_PreRow < 0 || u_PreCol < 0)
		return;

	BOOL Dragable = u_Dragable;
    u_Dragable = FALSE;
    
    int NextRow, NextCol;
	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
   	int MaxRow = u_pStrArray->GetSize() - 1;
	int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);
   	int StartPos = GetScrollPos(SB_HORZ);
   	int StartCol = GetStartCol(StartPos);
   	int EndCol = GetEndCol(StartCol);     
	long nDataSize = GetDataSize();

    CRect RectClient;
    GetClientRect(RectClient); 
    
    int MinHorzRange = 0;
    int MaxHorzRange = GetMaxHorzRange();
    
    if (nChar == VK_LEFT)                             //left arrow
    {
   	 	int ReturnCol = GetStartCol(0);
   	 	if (u_PreCol > ReturnCol)
   	 	{
            GetNextEditableCell(VK_LEFT, u_PreRow, u_PreCol, &NextRow, &NextCol);
            if (u_bHScrollable && MaxHorzRange != 0)
			{
				int ColCount = GetColGroup(NextCol);
				OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount);
			}
		}	
        else if (u_PreCol == ReturnCol)
        {	
            if (u_PreRow > 0)
	        {
	   	    	GetNextEditableCell(VK_LEFT, u_PreRow, 0, &NextRow, &NextCol);
   		    	if (NextRow == StartLine - 1)
                    OnScroll(SB_VERT, SB_LINEUP, 0);
	    	}
	    	else
        	{
				u_Dragable = Dragable;
				return;
            }	
	    	
            if (u_bHScrollable && MaxHorzRange != 0)
            {	
	            int i; 
	            BOOL NeedScroll = FALSE;
	            for (i = u_Cols - 2; i >= 0; i--)
	            {
	                if (AddColWidth(i, u_Cols - 1) > RectClient.Width())
	                {
	                	NeedScroll = TRUE;
	                	break;            
	                }	
	            } 
	            if (NeedScroll)
	            {
					if (NextCol <= i)
						i = NextCol - 1;
					int ColCount = GetColGroup(i + 1);
		    		OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount); 
	            }
            }
        } 
    }
   	else if (nChar == VK_UP)              //up arrow
	{   
		if (u_bVirtualPage)
		{
			if (u_PreRow == StartLine)
			{
				if (u_VirtualPreRow != 0)
					OnScroll(SB_VERT, SB_LINEUP, 0);
				else
				{
 					u_Dragable = Dragable;
 					return;
 				}	
 			}                                  
			NextCol = u_PreCol;
 			NextRow = u_PreRow - 1;
		}
		else
		{
            if (u_PreRow == 0)
			{
				if (u_PreCol == 0)
				{
 					u_Dragable = Dragable;
 					return;
 				}	
				GetNextEditableCell(VK_UP, 0, u_PreCol, &NextRow, &NextCol);
			}
			else
				GetNextEditableCell(VK_UP, u_PreRow, u_PreCol, &NextRow, &NextCol);

			if (!IsRowVisible(NextRow))
			{
				int nPos = max(0, NextRow - u_Rows / 2);
				OnScroll(SB_VERT, SB_THUMBPOSITION, nPos);
			}

			if (NextCol < StartCol && u_bHScrollable && MaxHorzRange != 0)
			{
				int ColCount = GetColGroup(NextCol);
				OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount);
			}
		}
	}
	else if (nChar == VK_RIGHT || nChar == VK_RETURN)                 //right arrow
	{   
        GetNextEditableCell(VK_RIGHT, u_PreRow, u_PreCol, &NextRow, &NextCol); 
        if (u_PreRow != NextRow)
        {	
      	    if (!IsRowVisible(u_FrameTop + u_PreRow + 1))
				OnScroll(SB_VERT, SB_LINEDOWN, 0);

            if (u_bHScrollable && MaxHorzRange != 0)
			{
				int ColCount = GetColGroup(NextCol);
				OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount);
			}
        }
        else if (u_bHScrollable && MaxHorzRange != 0)
        {    
            int LastRow, LastCol;
            if (EndCol == 0)
            {
                LastCol = 0;
                LastRow = u_PreRow;
            }
            else
                GetNextEditableCell(VK_LEFT, u_PreRow, EndCol,
            				        &LastRow, &LastCol);
            				        
            if (AddColWidth(StartCol, EndCol) > RectClient.Width() &&
				u_PreCol >= LastCol && u_PreRow == LastRow &&
				u_bHScrollable && MaxHorzRange != 0)
	        {                                   
	            int i;
				if (NextCol != EndCol)
					i = NextCol - 1;
				else
				{
					for (i = EndCol - 1; i > StartCol; i--)
					{
						if (AddColWidth(i, EndCol) > RectClient.Width())
	                		break;
					}     
				}

	            if (StartCol == EndCol)
	                i++;
				int ColCount = GetColGroup(i + 1);
	            OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount);
	        }    
        }
    }
	else if (nChar == VK_DOWN)                                  //down arrow
	{
		if (u_bVirtualPage)
		{
			if (u_VirtualPreRow == nDataSize - 1)
			{
				u_Dragable = Dragable;
				return;
			}	

			if (!IsRowVisible(u_FrameTop + u_PreRow + 1))
			{                          
	       		OnScroll(SB_VERT, SB_LINEDOWN, 0);

				if (u_PreRow == MaxRow) 
				{
					u_Dragable = Dragable;
					return;
				}	
			}                                  
 			
			NextCol = u_PreCol;
 			NextRow = u_PreRow + 1;
		}
		else
		{
            if (u_PreRow == nDataSize - 1)
			{
				if (u_PreCol == u_Cols - 1)
				{
 					u_Dragable = Dragable;
 					return;
 				}	
				GetNextEditableCell(VK_DOWN, nDataSize - 1, u_PreCol, &NextRow, &NextCol);
			}
			else
				GetNextEditableCell(VK_DOWN,u_PreRow, u_PreCol, &NextRow, &NextCol);

			if (!IsRowVisible(NextRow))
			{
				int nPos = max(0, NextRow - u_Rows / 2);
				OnScroll(SB_VERT, SB_THUMBPOSITION, nPos);
			}

			if (NextCol >= EndCol && u_bHScrollable && MaxHorzRange != 0)
			{
				int ColCount = GetColGroup(NextCol);
				OnScroll(SB_HORZ, SB_THUMBPOSITION, ColCount);
			}
		}
	}

    if (u_PreRow != NextRow || u_PreCol != NextCol)
		ClickRowCol(NextRow, NextCol);
}

int CUIGrid::GetColGroup(int i)
{
	int ColCount = 0;
	for (int j = 0; j < i; j++) 
	{
	    if (u_ColWidth[j] != 0)
	        ColCount++; 
	}
	return ColCount;
}

void CUIGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'C' || nChar == 'c')
	{
		SHORT ch = ::GetKeyState(VK_CONTROL);
		BOOL bCtrlDown = (ch & '0x80');
		if (bCtrlDown)
		{
			OnEditCopy();
			CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}
	}
	else if (nChar == 'V' || nChar == 'v')
	{
		SHORT ch = ::GetKeyState(VK_CONTROL);
		BOOL bCtrlDown = (ch & '0x80');
		if (bCtrlDown)
		{
			OnEditPaste();
			CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}
	} 

	KeyDown(nChar);
    
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


LRESULT CUIGrid::OnEditKeyDown(WPARAM wParam, LPARAM lParam)
{
    KeyDown(LOWORD(wParam));
    return 0L;
} 

void CUIGrid::SetNoSelect()
{
	u_SelectedRow = -1;
	u_EndSelectedRow = -1;

    UpdateGrdData();
        
   	u_PreRow = -1;
	u_PreCol = -1;
	u_VirtualPreRow = -1;
	
	if (u_pSelArray != NULL)
		u_pSelArray->RemoveAll();
}


void CUIGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	//if (!u_bDragLine)                 
	//{
	//   CWnd::OnMouseMove(nFlags, point);
	//	return;
	//}
	
	LeaveTrack();

	int nPos = GetScrollPos(SB_HORZ);
	int StartCol = GetStartCol(nPos);
	int nStart = 0;
	if (StartCol != 0)
		if (u_ColWidth[StartCol - 1] == 0)
			nStart = 1; 

	if (u_Cols - StartCol + nStart == 0)
	{
	    CWnd::OnMouseMove(nFlags, point);
		return;
	}

	HCURSOR Cur;
	u_Dragable = FALSE; 
	
	if (u_bDragLine)                 
	{
		int* LineCood = new int[u_Cols - StartCol + nStart];    
		SetLinePos(LineCood);
		int i;
		for (i = 0; i < u_Cols - StartCol + nStart; i++)
		{   
			int j = LineCood[i];
			if (abs(LineCood[i] - point.x) <= 2)
			{
				Cur = AfxGetApp()->LoadCursor(IDC_CURSOR_DRAG_SIDE);
				::SetCursor(Cur); 
            
				u_Dragable = TRUE;
				break;
			}
		}
		delete LineCood;
	}

    if (u_Dragable == FALSE) 
    {
		int nRow, nCol;
		PointToRowCol(point, nRow, nCol);
		int nType = GetAttribAt(nRow, nCol, cType);
		if (nType == cGridHyperLink || nType == cGridHyperBMP)
		{
			if (u_PreOverRow != nRow || u_PreOverCol != nCol)
			{
				CRect rc = RowColToRect(u_PreOverRow, u_PreOverCol);
				u_PreOverRow = nRow;
				u_PreOverCol = nCol;
				RedrawWindow(rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				rc = RowColToRect(nRow, nCol);
				RedrawWindow(rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
		}
		else
		{
			if (u_PreOverRow >= 0 && u_PreOverCol >= 0)
			{
				CRect rc = RowColToRect(u_PreOverRow, u_PreOverCol);
				u_PreOverRow = -1;
				u_PreOverCol = -1;
				RedrawWindow(rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}

			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

int CUIGrid::GetAheadSpace()
{
	if (u_SelType == cGridHeadSel || u_bWithFixedCol)
		return u_HeadWidth;
	return 0;
}

void CUIGrid::SetLinePos(int* pLinePos)
{
	int nPos = GetScrollPos(SB_HORZ);
	int StartCol = GetStartCol(nPos);
	int nStart = 0;
	if (StartCol != 0)
		if (u_ColWidth[StartCol - 1] == 0)
			nStart = 1; 
	
	int nOffset = 0;
	if (nStart == 0 && StartCol <= u_Cols - 1) 
		nOffset = u_ColWidth[StartCol];
	pLinePos[0] = GetAheadSpace() + nOffset;
	for (int i = 1; i < u_Cols - StartCol + nStart; i++)
		pLinePos[i] = pLinePos[i - 1] + u_ColWidth[StartCol + i - nStart];
} 

void CUIGrid::DragLinePos(CPoint point)
{   
    HCURSOR Cur = AfxGetApp()->LoadCursor(IDC_CURSOR_DRAG_SIDE);
    ::SetCursor(Cur);  
        
    HDC hDC = ::GetDC(m_hWnd);
    HPEN hCurrentPen = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
    HPEN hOldPen = (HPEN)SelectObject(hDC, hCurrentPen);
    ::SetROP2(hDC, R2_XORPEN);
	    
    POINT PrePoint;
    PrePoint.x = -1;//point.x;
    PrePoint.y = point.y;
	    
    CRect RectClient;
    GetClientRect(RectClient);
    
    int TotalCount = GetAheadSpace();
    int MaxColWidth = RectClient.Width() - TotalCount - 2;
    
    int StartCol = GetStartCol(GetScrollPos(SB_HORZ));
    int DragedLine;
    int i;                 
    if (u_bWithFixedCol && abs(TotalCount - point.x) <= 2)
    {
        if (u_ColWidth[StartCol] == 0 && point.x > TotalCount)
        	DragedLine = StartCol + 1;
        else
        {
        	DragedLine = StartCol;	
        	TotalCount = 0;
        }	
    }
    else
    {
    	for (i = StartCol; i <= u_Cols; i++)
    	{
	       	if (abs(TotalCount - point.x) <= 2)
        	{   
            	if (i <= u_Cols - 1)
            	{
            		if (u_ColWidth[i] == 0 && point.x - TotalCount > 0)
            			DragedLine = i;
            	  	else
            		{	
          				if (i > 0)
						{
							TotalCount -= u_ColWidth[i - 1];
          					DragedLine = i - 1;
						}
          			}	
            	}
            	else
            	{	
          			if (i > 0)
					{
						TotalCount -= u_ColWidth[i - 1];
          				DragedLine = i - 1;
					}
          		}	
          		break;	  	
   	  		}
			TotalCount += u_ColWidth[i];
   		}
    }
    
    if (DragedLine == 0 && u_bWithFixedCol)
    	MaxColWidth += u_HeadWidth;
    	
  	::SetCapture(m_hWnd);
    for (;;)
    {
    	::WaitMessage();  
    	
    	MSG msg;
    	if (::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
    	{      
        	if (msg.message == WM_LBUTTONUP)
        	{   
        	    POINTS points = MAKEPOINTS(msg.lParam);
				PrePoint.x = points.x;
        	     
        	    ::MoveToEx(hDC, PrePoint.x, 0, NULL);
        	    ::LineTo(hDC, PrePoint.x, RectClient.bottom);
    	    	    
       	    	int ColWidth = min(PrePoint.x - TotalCount, MaxColWidth);
       	    	if (PrePoint.x - TotalCount > 0)
       	    	    u_ColWidth[DragedLine] = ColWidth;                                                 
       	    	else
       	    	{                                                     
	       	    	if (DragedLine > 0 && u_ColWidth[DragedLine] == 0)
	       	    	{
	       	    		DragedLine--;
	                    TotalCount -= u_ColWidth[DragedLine];
	                }
	                ColWidth = min(PrePoint.x - TotalCount, MaxColWidth);    
        	    	u_ColWidth[DragedLine] = max(0, ColWidth);
				}    

                UpdateGrdData();
				SetHorzScrollRange();
                RePaint();

			    u_bFieldSizeChanged = TRUE;
                    
			    ::DeleteObject(hCurrentPen);
			    ::SelectObject(hDC, hOldPen);
			    ::ReleaseDC(m_hWnd, hDC);
				break;
    	  	}
    	   	else if (msg.message == WM_MOUSEMOVE)
    	   	{
    	  	    if (PrePoint.x != -1)
    	   	    {
    	   	    	::MoveToEx(hDC, PrePoint.x, 0, NULL);
    	   	    	::LineTo(hDC, PrePoint.x, RectClient.bottom);
    	   	    }	

    	   	    POINTS points = MAKEPOINTS(msg.lParam);
				::MoveToEx(hDC, points.x, 0, NULL);
    	   	    ::LineTo(hDC, points.x, RectClient.bottom);
    	    	    
				PrePoint.x = points.x;
    	   	    PrePoint.y = points.y;
    	   	}
    	}
	}
	::ReleaseCapture();
	GetMSGWnd()->SendMessage(WM_GRD_COL_WIDTH_CHANGE, (WPARAM)this, 0);   
	return;
}

void CUIGrid::DragTitle(CPoint point)
{
  	CRect RectBtn = PointToRect(point);
  	
  	BOOL DragFlag = FALSE;
  	int MoveCount = 0;
  	::SetCapture(m_hWnd);
    for (;;)
    {
    	::WaitMessage();  
    	
    	MSG msg;
    	if (::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
    	{      
        	if (msg.message == WM_LBUTTONUP)
        	{
                ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
                
                if (!(DragFlag && u_bDragTitle) && u_bSort)
                {
	               	int nIndex = GetPosXIndex(point.x);
					OnTitleButtonPressed(nIndex);
					GetMSGWnd()->SendMessage(WM_GRD_SORT, (WPARAM)this, nIndex);
                   	break;
                }
                	
                if (!u_bDragTitle)
					break;

				CRect RectClient;
                GetClientRect(RectClient);
				RectClient.left += GetAheadSpace();
                RectClient.right = GetRightEdge();

                CPoint Pt;
				POINTS points = MAKEPOINTS(msg.lParam);
                Pt.x = points.x;
                Pt.y = points.y;

                int SourceX = GetPosXIndex(point.x);
                int DestX = GetPosXIndex(points.x);

                if (!RectClient.PtInRect(Pt))
                {
	                CRect RectGrid;
	                GetWindowRect(RectGrid);
	                
	                Pt.x += RectGrid.left;
	                Pt.y += RectGrid.top;
	                
	                CPoint CellPos;
	                CellPos.x = SourceX;
	                CellPos.y = -1;                     
	                
	                CObArray pObArray;
	                pObArray.Add((CObject*)&Pt);
	                pObArray.Add(NULL); 
	                pObArray.Add((CObject*)&CellPos);
	                GetMSGWnd()->SendMessage(WM_RBUTTON_DRAGDROP, (WPARAM)this, (LPARAM)&pObArray);
                    break;
                }
                
                if (SourceX == DestX)
                	break;
                
                OnColSwap(SourceX, DestX);
				GetMSGWnd()->SendMessage(WM_GRD_SWAP, (WPARAM)this, MAKELONG(SourceX, DestX));
                
				break;
    	  	}  
    	  	else if (msg.message == WM_MOUSEMOVE)
    	  	{
                if (!DragFlag)
                {
                	if (MoveCount > 1)
                		DragFlag = TRUE;
    	            MoveCount++; 
    	            continue;
    	        }
				if (u_bDragTitle)
    	  			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAG_DROP));
    	  	}
    	}
	}
	::ReleaseCapture();
} 

void CUIGrid::TitleButtonPressed(CPoint point)
{   
    if (!u_bDragTitle && !u_bSort)
    	return;
    	
    SetNoSelect();
    UpdateWindow();
    
    CRect RectBtn = PointToRect(point);
    RectBtn.left += 2;
    RectBtn.top += 2;
    RectBtn.right -= 1;
    RectBtn.bottom -= 1;
    
    CDC* pDC = GetDC();
    DrawBtnDown(pDC, RectBtn);
    ReleaseDC(pDC);
    
    DragTitle(point);

	//InvalidateRect(RectBtn);
	RedrawWindow(RectBtn, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return;
} 


int CUIGrid::GetRightEdge(BOOL WithScrollBar)       
{
    int TotalCount = GetAheadSpace();
    int StartCol = GetStartCol(GetScrollPos(SB_HORZ));
    int i;
    for (i = StartCol; i < u_Cols; i++)
    	TotalCount += u_ColWidth[i];
    	
 	CRect RectClient;
	GetClientRect(RectClient);

	if (!WithScrollBar)
		RectClient.right = RectClient.right - RectClient.left - 2;

    if (TotalCount > RectClient.right)
    	return RectClient.right;
    else
    	return TotalCount;	
}


void CUIGrid::SelectRows(CPoint point)
{
    // if the grid editable and the edit control is now on,
    // it should be invisible after a new line selected,
    // and the content should be written back.	 
    WriteSelCellBack();
		
   	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;

   	int PosY = point.y / u_RowWidth;
	long StartLine = u_VirtualScrollPos;
    long InitSelRow = PosY + StartLine - IntFixedRow;
	long CurSelRow, PreSelRow;	

   	CDC* pDC = GetDC();
	DrawSelRegion(pDC, u_SelectedRow, u_EndSelectedRow, TRUE);
	
	if (u_bMultiSel)
	{
		SHORT ch = ::GetKeyState(VK_SHIFT);
		BOOL bShiftDown = (ch & '0x80');
		if (bShiftDown)
		{
			if (InitSelRow < u_SelectedRow)
				u_SelectedRow = InitSelRow;
			else 
				u_EndSelectedRow = InitSelRow;
			DrawSelRegion(pDC, u_SelectedRow, u_EndSelectedRow, FALSE);
			ReleaseDC(pDC);

			int nSelRow = u_SelectedRow - u_FrameTop;
			int nSelEndRow = u_EndSelectedRow - u_FrameTop;
			GetMSGWnd()->SendMessage(WM_GRD_ROW_SELECTED, (WPARAM)this, 
									 MAKELONG(nSelRow, nSelEndRow));
			return;
		}
	}

	DrawSelect(pDC, PosY);
    if (u_SelType == cGridHeadSel)
    {
    	CRect RectTemp;
    	RectTemp.left = 1;
    	RectTemp.right = u_HeadWidth - 1;
    	RectTemp.top = PosY * u_RowWidth + 1;
    	RectTemp.bottom = RectTemp.top + u_RowWidth - 3;
    	DrawBtnDown(pDC, RectTemp);  
    }
    
    ReleaseDC(pDC);
                
	if (!u_bMultiSel) 
	{
		if (u_SelectedRow >= 0)
			SelOrDeselARow(u_SelectedRow, FALSE);
		SelOrDeselARow(InitSelRow, FALSE);
		u_SelectedRow = InitSelRow;
		u_EndSelectedRow = InitSelRow;

		int nSelRow = InitSelRow - u_FrameTop;
		GetMSGWnd()->SendMessage(WM_GRD_ROW_SELECTED, (WPARAM)this, MAKELONG(nSelRow, nSelRow));

		return;
	}
	
	CRect RectClient;
	GetClientRect(RectClient);

    int MinPos, MaxPos;
    MinPos = 0;
    MaxPos = GetMaxVertRange();

	int BeginScrollY = (u_bWithFixedRow) ? u_RowWidth : 0;                					
 	PreSelRow = InitSelRow;
  	long DataSize = GetDataSize();
  	long MaxIndex = DataSize - 1; 
  	
  	::SetCapture(m_hWnd);

    for (;;)
    {
    	::WaitMessage();
    	
    	MSG msg;
    	if (::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
    	{      
        	if (msg.message != WM_LBUTTONUP && msg.message != WM_MOUSEMOVE)
        		continue;
        		
            StartLine = u_VirtualScrollPos;
            int DataEnd = (int)(MaxIndex - StartLine + 1 + IntFixedRow) * u_RowWidth - 1;
            int DragBound = min(RectClient.bottom, DataEnd);
            POINTS points = MAKEPOINTS(msg.lParam);
			int YPos = points.y;  

        	if (msg.message == WM_LBUTTONUP)
        	{   
                int MaxRangeY = max(0, GetMaxVertRange() - cGridSpace);
                if (YPos > DragBound && StartLine >= MaxRangeY)
                	CurSelRow = DataSize - 1;
                else if (YPos < BeginScrollY && StartLine == MinPos)
                	CurSelRow = 0;
                else
                {
   	                int CurY;
                	if (YPos < 0)
                	{
                		if (u_bWithFixedRow)
                			CurY = 0;
                		else
                			CurY = -1;
                	}
                	else if (YPos > RectClient.bottom)
                		CurY = RectClient.bottom / u_RowWidth + 1;
                	else
                		CurY = YPos / u_RowWidth;

                	CurSelRow = CurY + StartLine - IntFixedRow; 
                }
				DrawSelByPos(InitSelRow, PreSelRow, CurSelRow);
				u_EndSelectedRow = max(CurSelRow, InitSelRow);
				u_SelectedRow = min(CurSelRow, InitSelRow);
                
				break;
    	  	}
    	   	else if (msg.message == WM_MOUSEMOVE)
    	   	{
                if (YPos > DragBound && StartLine == MaxPos)
                {
                    if (CurSelRow != MaxIndex && PreSelRow < MaxIndex)            
                       	DrawSelByPos(InitSelRow, PreSelRow, MaxIndex);
                    
                    CurSelRow = MaxIndex;
                    PreSelRow = CurSelRow;

                    continue;
                }           
                
                if (YPos < BeginScrollY && StartLine == MinPos)
                {
                    if (CurSelRow != 0)
                    {
                    	DrawSelByPos(InitSelRow, PreSelRow, 0);
                    	CurSelRow = 0;
                    	PreSelRow = 0;
                    }	
                	continue;
                }
                
                int CurY; 
                if (YPos < 0)
                {
                	if (u_bWithFixedRow)
                		CurY = 0;
                	else
                		CurY = -1;
                }
                else if (YPos > RectClient.bottom)
                	CurY = RectClient.bottom / u_RowWidth + 1;
                else
                	CurY = YPos / u_RowWidth;

                CurSelRow = CurY + StartLine - IntFixedRow; 
				u_EndSelectedRow = max(CurSelRow, InitSelRow);
				u_SelectedRow = min(CurSelRow, InitSelRow);

                if (CurSelRow != PreSelRow)
                {
					int NeedScroll = 0;
					if (YPos > RectClient.bottom)
					{
						NeedScroll = 1;
						DrawSelByPos(InitSelRow, PreSelRow, CurSelRow - 1);
					}	
					else if (YPos < BeginScrollY)
					{
						NeedScroll = 2;                       
						DrawSelByPos(InitSelRow, PreSelRow, CurSelRow + 1);
                    }
                    
					if (NeedScroll == 0)
					{  
						DrawSelByPos(InitSelRow, PreSelRow, CurSelRow);
						PreSelRow = CurSelRow;
                    }
                    else
					{
					    for (;;)
					    {
    						int CurYPos;
    						
    						MSG msg;
    						if (::PeekMessage(&msg, NULL, WM_MOUSEFIRST, 
    												WM_MOUSELAST, PM_REMOVE))
    						{      
                                points = MAKEPOINTS(msg.lParam);
								CurYPos = points.y; 

        						if (msg.message == WM_LBUTTONUP)
        						{
								    ::ReleaseCapture();
									int nSelRow = u_SelectedRow - u_FrameTop;
									int nSelEndRow = u_EndSelectedRow - u_FrameTop;
									GetMSGWnd()->SendMessage(WM_GRD_ROW_SELECTED, (WPARAM)this, 
															 MAKELONG(nSelRow, nSelEndRow));
									return;
        						}
                                else if (msg.message == WM_MOUSEMOVE)
                                {
                                    if ((NeedScroll == 1 && CurYPos < RectClient.bottom) ||
                                        (NeedScroll == 2 && CurYPos > BeginScrollY))
                                    {
                                        PreSelRow = CurSelRow;
                                       	break;
                                    }   	
                                }
                            } 

   	                    	StartLine = u_VirtualScrollPos;
       	                    if ((NeedScroll == 1 && StartLine == MaxPos) ||
       	                        (NeedScroll == 2 && StartLine == MinPos))
       	                    {
       	                    	PreSelRow = CurSelRow;
       	                    	break;                       
                            }
                            
                			if (NeedScroll == 1)
                			{
                				BOOL bNormalScroll = TRUE;
                				if (u_Rows + StartLine - IntFixedRow > DataSize - 1)
                					bNormalScroll = FALSE;
                				CurSelRow = min(u_Rows + StartLine - IntFixedRow, DataSize - 1); 
                				u_EndSelectedRow = max(CurSelRow, InitSelRow);
								u_SelectedRow = min(CurSelRow, InitSelRow); 
								if (u_SelectedRow == CurSelRow && bNormalScroll)
									DrawSelRegion(pDC, CurSelRow - 1, CurSelRow - 1, TRUE);
                            	OnScroll(SB_VERT, SB_LINEDOWN, 0);
                			}
                			else
                            {
                                CurSelRow = StartLine - 1;
                                u_EndSelectedRow = max(CurSelRow, InitSelRow);
								u_SelectedRow = min(CurSelRow, InitSelRow); 
								if (u_EndSelectedRow == CurSelRow)
									DrawSelRegion(pDC, CurSelRow + 1, CurSelRow + 1, TRUE);
								OnScroll(SB_VERT, SB_LINEUP, 0);		
                            }
					    }
					}
		   	   	}    
    	   	}
    	}
	}
	::ReleaseCapture();

    return;
}

void CUIGrid::WriteSelCellBack()
{
	if (IsCellComboOrEdit(u_PreCellType) && u_CtrlShown)
	{
		// Update the temporary buffer
    	UpdateGrdData();
		UpdateWindow();  
    }

	if (u_PreRow >= 0 && u_PreCol >= 0)
	{
	    EraseSel(u_PreRow, u_PreCol);

        u_PreRow = -1;
		u_PreCol = -1;
		u_VirtualPreRow = -1;  
	} 
}

void CUIGrid::DrawSelRegion(CDC* pDC, long BeginLine, long EndLine, BOOL BtnUp)
{
    if (BeginLine == -1)
    	return;
    long DataSize = GetDataSize();
    EndLine = min(EndLine, DataSize - 1);

   	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;        
    int BeginGap = (int)(BeginLine - u_VirtualScrollPos);
    int BeginSel = max(0, BeginGap + IntFixedRow);
    
    int EndSel;
    if (!u_bMultiSel)
        EndSel = BeginSel;
	else          
	{
	    int EndGap = (int)(EndLine - u_VirtualScrollPos);
		EndSel = EndGap + IntFixedRow;
		EndSel = min(EndSel, u_Rows);
	}
		
    DrawSelLines(pDC, BeginSel, EndSel, BtnUp);
}

void CUIGrid::DrawSelLines(CDC* pDC, int BeginLine, int EndLine, BOOL BtnUp)
{
	CRect RectTemp;
	if (u_SelType == cGridHeadSel)
	{
		if (BtnUp) 
			RectTemp.left = 0; 
		else
			RectTemp.left = 1;
			
		RectTemp.right = u_HeadWidth - 1; 
    }
    
 	int i;
 	for (i = BeginLine; i <= EndLine; i++)
 	{
        if (i == 0 && u_bWithFixedRow)
        	continue;
        	
 	 	DrawSelect(pDC, i);

 	    if (u_SelType == cGridHeadSel)
 	    {
 	    	if (BtnUp)
 	    	{
	 	    	RectTemp.top = i * u_RowWidth;
	 	    	RectTemp.bottom = RectTemp.top + u_RowWidth - 2;
	 	    	DrawBtnEdge(pDC, RectTemp);
 	    	}
 	    	else
 	    	{
 	    		RectTemp.top = i * u_RowWidth + 1;
	    		RectTemp.bottom = RectTemp.top + u_RowWidth - 3;
 	    		DrawBtnDown(pDC, RectTemp);
 	    	}
 	    }	
	}	
}

void CUIGrid::DrawSelByPos(long InitRow, long PreRow, long CurRow)
{
	CDC* pDC = GetDC();
	
	if (CurRow > PreRow && PreRow >= InitRow)
		DrawSelRegion(pDC, PreRow + 1, CurRow, FALSE);
	else if (PreRow > CurRow && CurRow >= InitRow)
       	DrawSelRegion(pDC, CurRow + 1, PreRow, TRUE);
	else if (CurRow < PreRow && PreRow <= InitRow)
       	DrawSelRegion(pDC, CurRow, PreRow - 1, FALSE);
	else if (PreRow < CurRow && CurRow <= InitRow)
		DrawSelRegion(pDC, PreRow, CurRow - 1, TRUE);
	else if (CurRow < InitRow && InitRow < PreRow)
	{
       	DrawSelRegion(pDC, CurRow, InitRow - 1, FALSE);
       	DrawSelRegion(pDC, InitRow + 1, PreRow, TRUE);
    }
	else if (CurRow > InitRow && InitRow > PreRow)
	{                
	   	DrawSelRegion(pDC, InitRow + 1, CurRow, FALSE);
       	DrawSelRegion(pDC, PreRow, InitRow - 1, TRUE);
	}
                        
	ReleaseDC(pDC);
} 

int CUIGrid::GetMaxHorzRange()
{
	int ColCount = 0;
	for (int i = 0; i < u_Cols; i++)
	{
		if (u_ColWidth[i] != 0)
			ColCount++;
	}
    return max(ColCount - 1, 0);
}

int CUIGrid::GetStartCol(int ColGroup)
{
	int ColCount = 0;
	for (int i = 0; i <= ColGroup; i++)
	{
	    for (;ColCount < u_Cols;)
	    {
	    	if (u_ColWidth[ColCount] == 0)
	    		ColCount++;
	    	else
	    		break;	
	    }    
	    if (i > 0)
			ColCount++;
	}
	return ColCount;
} 

void CUIGrid::GetNextEditableCell(int nDir, int StartRow, 
								  int StartCol, int* EndRow, int* EndCol)
{
	int nCol = StartCol;
	int nRow = StartRow; 
	int nDataSize = u_pStrArray->GetSize();
	if (nDir == VK_RIGHT)
	{   
        for (;;)
		{
	   	    nCol = (nCol + 1) % u_Cols;
	   	    if (nCol == 0)
	   	    {
	   	        if (nRow == nDataSize - 1)
	   	        {
					nRow = StartRow; 
					nCol = StartCol;
					break;
	   	        }
	   	        else nRow++;
	   	    }
		    if (u_ColWidth[nCol] != 0 && GetAttribAt(nRow, nCol, cEditable))
		        break;
		} 
	}
	else if (nDir == VK_LEFT)
	{
        for (;;)
		{
	   	    nCol = (nCol == 0) ? u_Cols - 1: nCol - 1;
	   	    if (nCol == u_Cols - 1)
	   	    {
	   	        if (nRow == 0)
	   	        {
					nRow = StartRow; 
					nCol = StartCol;
					break;
	   	        }
	   	        else nRow--;
	   	    }	
		    if (u_ColWidth[nCol] != 0 && GetAttribAt(nRow, nCol, cEditable))
		        break;
		} 
	}
	else if (nDir == VK_UP)
	{
        for (;;)
		{
	   	    nRow = (nRow == 0) ? nDataSize - 1: nRow - 1;
	   	    if (nRow == nDataSize - 1)
	   	    {
	   	        if (nCol == 0)
	   	        {
					nRow = StartRow; 
					nCol = StartCol;
					break;
	   	        }
	   	        else nCol--;
	   	    }	
		    if (u_ColWidth[nCol] != 0 && GetAttribAt(nRow, nCol, cEditable))
		        break;
		} 
	}
	else if (nDir == VK_DOWN)
	{
		for (;;)
		{
	   	    nRow = (nRow + 1) % nDataSize;
	   	    if (nRow == 0)
	   	    {
	   	        if (nCol == u_Cols - 1)
	   	        {
					nRow = StartRow; 
					nCol = StartCol;
					break;
	   	        }
	   	        else nCol++;
	   	    }	
		    if (u_ColWidth[nCol] != 0 && GetAttribAt(nRow, nCol, cEditable))
		        break;
		} 
	}

    *EndRow = nRow;		  
    *EndCol = nCol;
}

int CUIGrid::GetEndCol(int StartCol)
{
	int	TotalColWidth = GetAheadSpace();

	CRect RectGrid;
	GetClientRect(RectGrid); 
	for (int i = StartCol; i < u_Cols; i++)
	{
		TotalColWidth += u_ColWidth[i];
    	if (TotalColWidth > RectGrid.Width() + 2)
        	return i;
	}  
	return u_Cols - 1;                                                
} 

void CUIGrid::OnComboSelChange()
{
	GetMSGWnd()->SendMessage(WM_GRD_COMBO_SEL_CHANGE, (WPARAM)this, (LPARAM)u_pComboOrEdit);
}

void CUIGrid::OnEditChange()
{
   	CString StrEdit;
   	u_pComboOrEdit->GetWindowText(StrEdit);

	if(u_PreCellType== cGridEditBtn)
    {
        int Strlen = StrEdit.GetLength();
        
        CRect RectCell;
        RectCell = RowColToRect(u_PreRow, u_PreCol);
        RectCell.left ++;
        RectCell.top ++;
        
    	char nIndex = GetAttribAt(u_PreRow, u_PreCol, cIndex);

        CDC* pDC = GetDC();
   	    if (Strlen == 0 && !u_bBtnNullEnable)
   	    	DrawNoteBtn(pDC, RectCell, FALSE, TRUE, FALSE, 0, nIndex);
		else if (Strlen == 1 && !u_bBtnNullEnable)
			DrawNoteBtn(pDC, RectCell, TRUE, TRUE, FALSE, 0, nIndex);
		ReleaseDC(pDC);
    }
    GetMSGWnd()->SendMessage(WM_GRD_EDIT_CHANGE, (WPARAM)this, (LPARAM)u_pComboOrEdit);
}

void CUIGrid::RePaint()
{
	//InvalidateRect(NULL, FALSE);
   	//UpdateWindow(); 
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void CUIGrid::RePaintCell(int nRow, int nCol)
{
    CRect RepaintRect = RowColToRect(nRow, nCol);
    if (!RepaintRect.IsRectEmpty())
	{
		//InvalidateRect(RepaintRect, FALSE);
		//UpdateWindow();
		RedrawWindow(RepaintRect, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CUIGrid::RePaintCol(int nCol)
{


}

void CUIGrid::RePaintRow(int nRow, int nEndRow)
{
	if (nEndRow >= 0 && nEndRow < nRow)
		SwapInt(nRow, nEndRow);

	int StartLine = (int)(u_VirtualScrollPos - u_FrameTop);
	if (nEndRow >= 0 && nEndRow < StartLine)
		return;

    CRect RectInval;
    GetClientRect(RectInval);

    int IntFixedRow = (!u_bWithFixedRow) ? 0 : 1;
	int nAbsLine = max((int)(nRow - StartLine), 0) + IntFixedRow;
    RectInval.top = nAbsLine * u_RowWidth;
	if (RectInval.top >= RectInval.bottom)
		return;

	if (nEndRow >= 0)
	{
		int nAbsEndLine = (int)(nEndRow - StartLine) + IntFixedRow + 1;
		RectInval.bottom = min(nAbsEndLine * u_RowWidth, RectInval.bottom);
	}
	
    //InvalidateRect(RectInval); 
    //UpdateWindow();
	RedrawWindow(RectInval, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
} 

void CUIGrid::ClickRowCol(int Row, int Col)  
{
    CRect RectClick = RowColToRect(Row, Col);
	CRect rcClient;
	GetClientRect(rcClient);
	if (RectClick.IsRectEmpty() || RectClick.bottom > rcClient.bottom)
	{
		SetVScrollPos(Row, TRUE);
		RectClick = RowColToRect(Row, Col);
		if (RectClick.IsRectEmpty())
			OnScroll(SB_HORZ, SB_ABSOLUTE, Col); 
		RectClick = RowColToRect(Row, Col);
	}
	
	if (RectClick.IsRectEmpty())
		return;

    CPoint pt;
    pt.x = RectClick.left + 1;
    pt.y = RectClick.top + 1;
    LButtonDown(pt, TRUE);
}

COLORREF CUIGrid::GetCellColor(int nRow, int nCol, int nAttrib)
{
	unsigned char chColor = GetAttribAt(nRow, nCol, nAttrib);
	if (chColor == cDefColor)
		return (nAttrib == cFGColor) ? u_rgbFG : u_rgbBG;
	else if (chColor == cGrayColor)
		return ::GetSysColor(COLOR_BTNFACE);
	return GetColorFromIndex(chColor);
}

HBRUSH CUIGrid::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (!u_bPaintBk)
		return NULL;
	pDC->SetTextColor(GetCellColor(u_PreRow, u_PreCol, cFGColor));
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetBrush(u_PreRow, u_PreCol)->m_hObject;
} 

long CUIGrid::GetDataSize()
{
    if (u_bVirtualPage)
    	return u_VirtualDataSize;
    else
    	return u_pStrArray->GetSize();
}

void CUIGrid::SetDynaHeadWidth()
{
	if (!::IsWindow(m_hWnd))
		return;

	CSize cs;
    CDC* pDC = GetDC();

	if (u_bWithFixedCol)
	{
		if (u_pFixedCol != NULL)
		{
			int cx = 0;
			int nSize = u_pFixedCol->GetSize();
			for (int i = 0; i < nSize; i++)
			{
				CString StrLabel = u_pFixedCol->GetAt(i);
				cs = pDC->GetTextExtent(StrLabel);
				cx = max(cx, cs.cx);
			}
			u_HeadWidth = cx + u_GridInitX * 2;
		}
	}
	else if (u_SelType == cGridHeadSel)
	{
		long DataSize = GetDataSize();
		char szBuffer[256];
		ltoa(DataSize, szBuffer, 10); 
		cs = pDC->GetTextExtent(szBuffer, strlen(szBuffer));
		u_HeadWidth = max(cs.cx + u_GridInitX * 2 + 10, 50);
	}

	ReleaseDC(pDC);

	GetMSGWnd()->SendMessage(WM_GRD_SET_HEADWIDTH, (WPARAM)this, u_HeadWidth);
}

BOOL CUIGrid::IsRowInBuffer(long nRow)
{
   	int DataSize = u_pStrArray->GetSize();
   	if (nRow >= u_FrameTop && nRow < u_FrameTop + DataSize)
    	return TRUE;
    return FALSE;	
}

void CUIGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
    u_bLButtonUp = TRUE;	 //This flag is used to prevent the loss of 
    u_LButtonPoint = point;	 //WM_LBUTTONUP message;
    
	CWnd::OnLButtonUp(nFlags, point);
	//CheckCursor(point);
}

void CUIGrid::PostSetColNo(int nCol)
{
}

void CUIGrid::SetTotalColNo(int nCol)
{
	if (nCol <= 0 || nCol > cGridMaxCol)
		return;
	if (u_Cols == nCol)
		return;

	int i;
	int nLastWidth = cGridDefaultColWidth;
	if (u_Cols > 0)
		nLastWidth = u_ColWidth[u_Cols - 1];
	if (u_Cols > nCol)
	{
		for (i = 0; i < u_Cols - nCol; i++)
		{
			u_ColWidth.RemoveAt(u_Cols - i - 1);
			if (u_pFixedRow != NULL)
				u_pFixedRow->RemoveAt(u_Cols - i - 1);
		}
	}
	else
	{
		CString StrTemp;
		for (i = 0; i < nCol - u_Cols; i++)
		{
			u_ColWidth.Add(nLastWidth);
			if (u_pFixedRow != NULL)
			{
				StrTemp.Format("%s %d", tField, u_Cols + i + 1);
				u_pFixedRow->Add(StrTemp);
			}
		}
	}
	
	PostSetColNo(nCol);

	u_Cols = nCol;

	SetHorzScrollRange();
}

void CUIGrid::SetColWidth(CString StrCol, char Del)
{
	char* pToken = new char[StrCol.GetLength() + 1];
	int Count= 0;
	int i = 0;
	while (::GetToken(StrCol, pToken, &Count, Del)) 
		SetColWidth(i++, atoi(pToken));		
	delete [] pToken;
	
	SetHorzScrollRange();
}

void CUIGrid::SetColWidth(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= u_Cols)
		return;
	if (nValue < 0)
		return;	

	u_ColWidth[nIndex] = nValue;

	SetHorzScrollRange();
}

BOOL CUIGrid::Create(CRect RectGrid, CWnd* pParent, UINT nGridID) 
{
	if (u_GridWndClass == "")
	{
		u_GridWndClass = AfxRegisterWndClass(CS_DBLCLKS, NULL,
								(HBRUSH) ::GetStockObject(WHITE_BRUSH)); 
	}

	BOOL bSuccess = CWnd::CreateEx(WS_EX_CLIENTEDGE, u_GridWndClass, "Grid", 
						WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WS_VSCROLL, 
						RectGrid, pParent, nGridID);

	return bSuccess;
}

CObArray* CUIGrid::ReplaceGridBuffer(CObArray* pData, BOOL bTransForm)
{
	int i, j;
	int nCount = pData->GetSize();
	if (nCount > 0)
	{
		CStringArray* pArray = (CStringArray*)pData->GetAt(0);
		if (u_bAutoCol) 
			SetTotalColNo(pArray->GetSize());
	}

	for (i = 0; i < nCount; i++)
	{
		CStringArray* pArray = (CStringArray*)pData->GetAt(i);
		for (j = 0; j < u_Cols; j++)
		{
			CString StrData = pArray->GetAt(j);
			StrData = AddAttribSet(i, j, StrData);
			pArray->SetAt(j, StrData);
		}
	}
	
	if (bTransForm)
	{
		for (i = 0; i < nCount; i++)
		{
			CStringArray* pArray = (CStringArray*)u_pStrArray->GetAt(i);
			for (j = 0; j < u_Cols; j++)
				pArray->SetAt(j, (const char*)pArray->GetAt(j) + cAttribNo);
		}
	}

	CObArray* pOld = u_pStrArray;
	u_pStrArray = pData;
	
 	SetVertScrollRange();

	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();
	
	return pOld;
}

void CUIGrid::CopyGridBuffer(CObArray* pData)
{
	::ReleaseObArray(pData);
	int nSize = u_pStrArray->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CStringArray* pStrArray = (CStringArray*)u_pStrArray->GetAt(i);
		pStrArray = CopyStrArrayNoAttrib(pStrArray);
		pData->Add(pStrArray);
	}
}

CString CUIGrid::AddAttribNew(int nRow, int nCol, CString StrContent)
{
	return u_StrDefAttrib + "";
}

CString CUIGrid::AddAttribSet(int nRow, int nCol, CString StrContent)
{
	StrContent = GetValueMap(StrContent);
	return u_StrDefAttrib + StrContent;
}

CStringArray* CUIGrid::GetANewRow(int nRow)
{
	CStringArray* pRetRow = new CStringArray;
	for (int i = 0; i < u_Cols; i++)
		pRetRow->Add(AddAttribNew(nRow, i, ""));
	return pRetRow;
}

BOOL CUIGrid::PreInsertOper(int nIndex, CStringArray* pArray)
{
	return TRUE;
}

void CUIGrid::InsertRowAt(int nIndex, int nRowNo)
{
	int nSize = u_pStrArray->GetSize();
	if (nIndex < 0 || nIndex >= nSize)
		return;
	for (int i = 0; i < nRowNo; i++)
	{
		CStringArray* pAddedArray = GetANewRow(nIndex + i);
		if (PreInsertOper(nIndex + i, pAddedArray))
		{
			u_pStrArray->InsertAt(nIndex, pAddedArray);
			if (u_bVirtualPage)
				u_VirtualDataSize++;
		}
	}

	SetVertScrollRange();

	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();

	PostInsertOper();
}

void CUIGrid::SwapLong(long& a, long& b)
{
	long c = a;
	a = b;
	b = c;
}

void CUIGrid::SwapInt(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

void CUIGrid::DeleteRows(long nIndex, long nEndIndex)
{
	long nSize = GetDataSize();
	if (nIndex > nEndIndex)
		SwapLong(nIndex, nEndIndex);
	if (nIndex >= nSize || nEndIndex < 0)
		return;
	if (nIndex < 0)
		nIndex = 0;
	if (nEndIndex >= nSize)
		nEndIndex = nSize - 1;

	if (!PreDelOper())
		return;

	int nRelIndex = (int)(nIndex - u_FrameTop);
	for (long i = nIndex; i <= nEndIndex; i++)
	{
		if (PreDelAtOper(i))
		{
			if (IsRowInBuffer(nIndex))
				::RemoveAndDelete(u_pStrArray, nRelIndex);
			if (u_bVirtualPage)
				u_VirtualDataSize--;
		}
	}

	SetVertScrollRange();

	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();

	PostDelOper();
}

BOOL CUIGrid::PreDeleteAllOper()
{
	return TRUE;
}

void CUIGrid::DeleteAllRows()
{
	if (PreDeleteAllOper())
	{
		::ReleaseObArray(u_pStrArray);
		if (u_bVirtualPage)
			u_VirtualDataSize = 0;
	}

	SetVertScrollRange();

	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();
}

void CUIGrid::AppendRow(int nRowNo)
{
	for (int i = 0; i < nRowNo; i++)
	{
		int nGridSize = u_pStrArray->GetSize();
		CStringArray* pAddedArray = GetANewRow(nGridSize);
		if (PreInsertOper(nGridSize, pAddedArray))
		{
			u_pStrArray->Add(pAddedArray);
			if (u_bVirtualPage)
				u_VirtualDataSize++;
		}
	}

	SetVertScrollRange();

	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();
	PostInsertOper();
}

void CUIGrid::AppendCol(int nColNo)
{
	CStringArray* pArray = (CStringArray*)u_pStrArray->GetAt(0);

	int nCols = GetTotalColNo();
	SetTotalColNo(nCols + nColNo);
	int nSize = u_pStrArray->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CStringArray* pArray = (CStringArray*)u_pStrArray->GetAt(i);
		for (int j = 0; j < nColNo; j++)
			pArray->Add(AddAttribNew(i, j + nCols, ""));
	}
}

void CUIGrid::SetDataAt(int nRow, int nCol, CString StrData)
{
	if (!ValidRowCol(nRow, nCol))
		return;
	
	PreSetDataAt(nRow, nCol, StrData);
	StrData = GetValueMap(StrData);
	
	CString StrCell = GetCellAt(nRow, nCol);

	u_DataBeforeUpdate = StrCell.Mid(cAttribNo);
    SetAt(nRow, nCol, StrCell.Left(cAttribNo) + StrData);
}

void CUIGrid::SetAttribAt(int nRow, int nCol, int nType, char nValue)
{
	if (!ValidRowCol(nRow, nCol))
		return;

	CString StrCell = GetCellAt(nRow, nCol);
	StrCell.SetAt(nType, nValue + 1);
	SetAt(nRow, nCol, StrCell);
}

CString CUIGrid::GetDataAt(int nRow, int nCol)
{
	if (!ValidRowCol(nRow, nCol))
		return "";
	
	CString StrCell = GetCellAt(nRow, nCol);
	int nLen = StrCell.GetLength() - cAttribNo;
	 
	return PostGetDataAt(nRow, nCol, StrCell.Right(nLen));
}

CString CUIGrid::GetDataCtrlAt(int nRow, int nCol)
{
   	if (!ValidRowCol(nRow, nCol))
		return "";

    if (u_PreRow == nRow && u_PreCol == nCol && 
    	IsCellComboOrEdit(u_PreCellType) && u_CtrlShown)
    {
        return GetCtrlContent();
    }
    else
		return GetDataAt(nRow, nCol);
} 

CString CUIGrid::GetCtrlContent()
{
  	CString StrTemp = "";
  	switch (u_PreCellType)
  	{
  		case cGridText:
  		case cGridPassword:
		case cGridEditBtn:
  		case cGridComboEdit:
  		case cGridComboEditDB:
  			u_pComboOrEdit->GetWindowText(StrTemp);
        	break;
        
        case cGridCombo:
        case cGridBMPCombo:
        case cGridComboBtn:
        case cGridComboDB:
      	{
      	    CComboBox* pCombo = (CComboBox*)u_pComboOrEdit;
			int nSel = pCombo->GetCurSel();
			if (nSel != CB_ERR)
      			pCombo->GetLBText(nSel, StrTemp);
            break;
        }
    }        
    char szBuffer[64];
    if (u_PreCellType == cGridBMPCombo)
    	StrTemp = itoa(atoi((const char*)StrTemp), szBuffer, 10);
    return StrTemp;
}


char CUIGrid::GetAttribAt(int nRow, int nCol, int nType)
{
	if (!ValidRowCol(nRow, nCol))
		return -1;

	CString StrCell = GetCellAt(nRow, nCol);
	return StrCell.GetAt(nType) - 1;
}

CString	CUIGrid::GetCellAt(int nRow, int nCol)
{
	if (!ValidRowCol(nRow, nCol))
		return "";

    CStringArray* pStrList = (CStringArray*)u_pStrArray->GetAt(nRow);
    return pStrList->GetAt(nCol);
}

BOOL CUIGrid::ValidRowCol(int nRow, int nCol)
{
	int nTotalRow = u_pStrArray->GetSize();
	if (nRow < 0 || nRow >= nTotalRow)
		return FALSE;
	CStringArray* pStr = (CStringArray*)u_pStrArray->GetAt(nRow);
	int nTotalCol = pStr->GetSize();
	if (nCol < 0 || nCol >= nTotalCol)
		return FALSE;
	return TRUE;
}

void CUIGrid::SetAt(int nRow, int nCol, CString Str)
{
    CStringArray* pStrList = (CStringArray*)u_pStrArray->GetAt(nRow);
    pStrList->SetAt(nCol, Str);
}

void CUIGrid::SetHeadWidth(int HeadWidth)
{
	if (HeadWidth < 0 && u_bDynaHeadWidth)
		return;

	if (HeadWidth < 0)
	{
		SetDynaHeadWidth();
		u_bDynaHeadWidth = TRUE;
	}
	else
	{
		u_HeadWidth = HeadWidth;
		u_bDynaHeadWidth = FALSE;
	}
}

void CUIGrid::SetVertScrollRange() 
{
   	if (!::IsWindow(m_hWnd))
		return;

	if (u_Rows < 0)
		return;

	if (!u_bVScrollable)
	{
		ShowScrollBar(SB_VERT, FALSE);
		return;
	}

	u_VirtualLastScrollPos = GetLastScrollPos();

    int RealLastScrollPos = VirtualToScroll(u_VirtualLastScrollPos);

    u_MaxVertScrollRange = max(0, RealLastScrollPos);
    SetScrollRange(SB_VERT, 0, u_MaxVertScrollRange, TRUE);   

    if (RealLastScrollPos > 0)
    {
        if (u_ScrollPos > RealLastScrollPos)
            u_ScrollPos = RealLastScrollPos;
    
    	SetScrollPos(SB_VERT, u_ScrollPos);
    }   
    else
    {
		u_ScrollPos = 0;
		u_VirtualScrollPos = 0;
	}
}

long CUIGrid::GetLastScrollPos()
{
    long DataSize = GetDataSize();
    int IntFixedRow = (!u_bWithFixedRow) ? 0 : 1;
  	long LastScrollPos = (long)(DataSize - u_Rows + IntFixedRow + 1);

   	if (LastScrollPos >= 0) //||
  		LastScrollPos += min(cGridSpace, u_Rows - 3);
  	else 
  		LastScrollPos = 0;

    return LastScrollPos;
}

void CUIGrid::SetHorzScrollRange()
{
   	if (!::IsWindow(m_hWnd))
		return;
	
	if (!u_bHScrollable)
    {
		SetScrollRange(SB_HORZ, 0, 0);	
		return;
	}

	int	TotalColWidth = GetAheadSpace();
    int ZeroCount = 0;
    int FirstOverRow = -1;
	
	CRect RectGrid;
	GetClientRect(RectGrid);

    for (int i = u_Cols - 1; i >= 0; i--)
    {
    	TotalColWidth += u_ColWidth[i];
        if (TotalColWidth > RectGrid.Width() + 2 && FirstOverRow == -1)
            FirstOverRow = i;
    	if (u_ColWidth[i] == 0)
    	    ZeroCount++; 
    }                                                  

    if (FirstOverRow != -1)
		SetScrollRange(SB_HORZ, 0, u_Cols - 1 - ZeroCount);
	else
    	SetScrollRange(SB_HORZ, 0, 0);	
}

void CUIGrid::SetSeqAt(int nType, int nIndex, CString Value)
{
	if (nIndex < 0)
		return;

	CStringArray* pArray;
	switch (nType)
	{	
		case cSeqTextCombo:
			if (u_pComboArray == NULL)
				u_pComboArray = new CStringArray;
			pArray = u_pComboArray;
			break;
		
		case cSeqBMPCombo:
			if (u_pBMPArray == NULL)
				u_pBMPArray = new CStringArray;
			pArray = u_pBMPArray;
			break;

		case cSeqDBCombo:
			if (u_pTableFieldArray == NULL)
				u_pTableFieldArray = new CStringArray;
			pArray = u_pTableFieldArray;
			break;

		case cSeqRGB:
			if (u_pColorArray == NULL)
				u_pColorArray = new CStringArray;
			pArray = u_pColorArray;

			if (u_pBrushArray == NULL)
				u_pBrushArray = new CObArray;
			u_pBrushArray->SetAtGrow(nIndex, NULL);

			break;
		
		case cSeqAllowedChar:
			if (u_pAllowedArray == NULL)
				u_pAllowedArray = new CStringArray;
			pArray = u_pAllowedArray;

			break;
	}
	pArray->SetAtGrow(nIndex, Value);
}

CString CUIGrid::GetSeqAt(int nType, int nIndex)
{
	if (nIndex < 0)
		return "";

	CStringArray* pArray;
	switch (nType)
	{	
		case cSeqTextCombo:
			if (u_pComboArray == NULL)
				return "";
			pArray = u_pComboArray;
			break;
			
		case cSeqBMPCombo:
			if (u_pBMPArray == NULL)
				return "";
			pArray = u_pBMPArray;
			break;

		case cSeqDBCombo:
			if (u_pTableFieldArray == NULL)
				return "";
			pArray = u_pTableFieldArray;
			break;

		case cSeqRGB:
			if (u_pColorArray == NULL)
				return "";
			pArray = u_pColorArray;
			break;
		
		case cSeqAllowedChar:
			if (u_pAllowedArray == NULL)
				return "";
			pArray = u_pAllowedArray;

			break;
	}
	if (nIndex >= pArray->GetSize())
		return "";

	return pArray->GetAt(nIndex);
}

int CUIGrid::GetSeqSize(int nType)
{
	CStringArray* pArray;
	switch (nType)
	{	
		case cSeqTextCombo:
			pArray = u_pComboArray;
			break;
		
		case cSeqBMPCombo:
			pArray = u_pBMPArray;
			break;

		case cSeqDBCombo:
			pArray = u_pTableFieldArray;
			break;

		case cSeqRGB:
			pArray = u_pColorArray;
			break;

		case cSeqAllowedChar:
			pArray = u_pAllowedArray;
			break;
	}
	if (pArray == NULL)
		return 0;
	else
		return pArray->GetSize();
}

void CUIGrid::RemoveSeqAt(int nType, int nIndex)
{
	CStringArray* pArray;
	switch (nType)
	{	
		case cSeqTextCombo:
			pArray = u_pComboArray;
			break;
		
		case cSeqBMPCombo:
			pArray = u_pBMPArray;
			break;

		case cSeqDBCombo:
			pArray = u_pTableFieldArray;
			break;

		case cSeqRGB:
			pArray = u_pColorArray;
			break;

		case cSeqAllowedChar:
			pArray = u_pAllowedArray;
			break;
	}
	if (pArray == NULL)
		return;
	if (nIndex >= pArray->GetSize())
		return;
	if (nIndex < 0)
	{
		pArray->RemoveAll();
		if (nType == cSeqRGB)
			::ReleaseObArray(u_pBrushArray);
	}
	else
	{
		pArray->RemoveAt(nIndex);
		if (nType == cSeqRGB)
			::RemoveAndDelete(u_pBrushArray, nIndex);
	}	
}

COLORREF CUIGrid::GetColorFromIndex(int nIndex)
{
	if (u_pColorArray == NULL)
		return ::GetSysColor(COLOR_WINDOW);
	if (nIndex < 0 || nIndex >= u_pColorArray->GetSize())
		return ::GetSysColor(COLOR_WINDOW);
	
	CString StrColor = u_pColorArray->GetAt(nIndex);
	
	int r, g, b;
	char* pToken = new char[StrColor.GetLength()];
	int nCount = 0;
	::GetToken(StrColor, pToken, &nCount);
	r = atoi(pToken);

	::GetToken(StrColor, pToken, &nCount);
	g = atoi(pToken);

	::GetToken(StrColor, pToken, &nCount);
	b = atoi(pToken);

	delete [] pToken;

	return RGB(r, g, b);
}

void CUIGrid::SetBMPAt(int nType, int nIndex, CBitmap* pBMP, int nID)
{
	if (nIndex < 0)
		return;

	CObArray* pArray;
	switch (nType)
	{	
		case cBMP:
			if (u_pBMP == NULL)
				u_pBMP = new CObArray;
			pArray = u_pBMP;
			
			if (u_pBMPIDArray == NULL)
				u_pBMPIDArray = new CWordArray;
			if (nID < 0)
				nID = nIndex;
			u_pBMPIDArray->SetAtGrow(nIndex, nID);

			break;
		
		case cBMPBtn:
			if (u_pBtnBMP == NULL)
				u_pBtnBMP = new CObArray;
			pArray = u_pBtnBMP;
			break;

	}
	pArray->SetAtGrow(nIndex, pBMP);
}

CBitmap* CUIGrid::GetBMPAt(int nType, int nIndex)
{
	if (nIndex < 0)
		return NULL;

	CObArray* pArray;
	switch (nType)
	{	
		case cBMP:
			if (u_pBMP == NULL)
				return NULL;
			if (nIndex >= u_pBMP->GetSize())
				return NULL;
			pArray = u_pBMP;
			break;
		
		case cBMPBtn:
			if (u_pBtnBMP == NULL)
				return NULL;
			if (nIndex >= u_pBtnBMP->GetSize())
				return NULL;
			pArray = u_pBtnBMP;
			break;

	}
	return (CBitmap*)pArray->GetAt(nIndex);
}


void CUIGrid::RemoveBMPAt(int nType, int nIndex)
{
	CObArray* pArray;
	switch (nType)
	{	
		case cBMP:
			pArray = u_pBMP;
			break;
		
		case cBMPBtn:
			pArray = u_pBtnBMP;
			break;
	}
	if (pArray == NULL)
		return;
	if (nIndex >= pArray->GetSize())
		return;
	if (nIndex < 0)
		::ReleaseObArray(pArray);
	else
		::RemoveAndDelete(pArray, nIndex);
	
	if (nType == cBMP)
	{
		if (nIndex < 0)
			u_pBMPIDArray->RemoveAll();
		else
			u_pBMPIDArray->RemoveAt(nIndex);
	}
}

int	CUIGrid::GetBMPSize(int nType)
{
	CObArray* pArray;
	switch (nType)
	{	
		case cBMP:
			pArray = u_pBMP;
			break;
		
		case cBMPBtn:
			pArray = u_pBtnBMP;
			break;
	}
	if (pArray == NULL)
		return 0;
	else
		return pArray->GetSize();
}

void CUIGrid::SetBMPFromID(CWordArray* pBMPIDArray, CString ResFilePath)
{
/*	if (u_pBMP == NULL)
		u_pBMP = new CObArray;
    int BMPCount = pBMPIDArray->GetSize();

    CUIResource* pRes;
    if (ResFilePath != "")
    	pRes = new CUIResource(ResFilePath);

	if (u_pBMPIDArray == NULL)
		u_pBMPIDArray = new CWordArray;

    for (int i = 0; i < BMPCount; i++)
    {
		CBitmap* pBMP = NULL;
		UINT Id = pBMPIDArray->GetAt(i);
		u_pBMPIDArray->Add(Id);

		if (ResFilePath != "")
        {
    		LPSTR pStr = pRes->GetResValue(Id);
			if (pStr != NULL)
        		pBMP = ::GetBitmap(pStr);
			delete [] pStr;
        }
        else
		{
	      	pBMP = new CBitmap;
        	pBMP->LoadBitmap(Id);
        }	 
        u_pBMP->Add(pBMP);
    }

	if (ResFilePath != "")
        delete pRes;
*/
}

CBrush* CUIGrid::GetDefBrush(int nCol)
{
	return u_pDefBrush;
}

CBrush* CUIGrid::GetBrush(int nRow, int nCol)
{
	if (!ValidRowCol(nRow, nCol))
		return u_pDefBrush;

	unsigned char nIndex = GetAttribAt(nRow, nCol, cBGColor);
	
	if (nIndex == cDefColor)
		return u_pDefBrush;

	if (u_pBrushArray == NULL)
		return NULL;

	CBrush* pBrush;
	if ((pBrush = (CBrush*)u_pBrushArray->GetAt(nIndex)) == NULL)
	{
		pBrush = new CBrush;
		pBrush->CreateSolidBrush(GetColorFromIndex(nIndex));
		u_pBrushArray->SetAt(nIndex, pBrush);
	}
	return pBrush;
}

void CUIGrid::SetColor(int nType, COLORREF rgb)
{
	switch (nType)
	{
		case cFGColor:
			u_rgbFG = rgb;
			break;

		case cVLineColor:
			u_rgbVLine = rgb;
			break;

		case cHLineColor:
			u_rgbHLine = rgb;
			break;

		case cSelColor:
			u_rgbSel = rgb;
			break;

		case cSelRowColor:
			u_rgbSelRows = rgb;
			break;

		case cBGColor:
			u_rgbBG = rgb;
			
			u_pDefBrush->DeleteObject();
			delete u_pDefBrush;
			u_pDefBrush = new CBrush;
			u_pDefBrush->CreateSolidBrush(rgb);
			
			break;
	}
}

COLORREF CUIGrid::GetColor(int nType)
{
	COLORREF RefRet;
	switch (nType)
	{
		case cFGColor:
			RefRet = u_rgbFG;
			break;

		case cVLineColor:
			RefRet = u_rgbVLine;
			break;

		case cHLineColor:
			RefRet = u_rgbHLine;
			break;

		case cBGColor:
			RefRet = u_rgbBG;
			break;
	}
	return RefRet;
}

void CUIGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	//if (u_bFirst)
	//	return;

	if (!::IsWindow(m_hWnd))
		return;
	
	SetRowsAndScrollBar();
 }

CFont* CUIGrid::SetGridFont(CFont* pNewFont)
{
	CFont* pOldFont = u_pFont;
	u_pFont = pNewFont;
	SetFonts();
	if (u_bDefRowWidth && ::IsWindow(m_hWnd))
		SetDefaultRowWidth();
	return pOldFont;
}

void CUIGrid::SetFonts()
{
	if (u_pEdit != NULL)
		u_pEdit->SetFontEx(u_pFont);
	if (u_pPassEdit != NULL)
		u_pPassEdit->SetFontEx(u_pFont);
	if (u_pComboEdit != NULL)
		u_pComboEdit->SetFontEx(u_pFont); 
    if (u_pCombo != NULL)
		u_pCombo->SetFontEx(u_pFont);
}

void CUIGrid::SetDefaultRowWidth()
{
	CComboBox* pCombo = new CComboBox;
   	pCombo->Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWN | WS_VSCROLL | ES_AUTOHSCROLL,
					 CRect(0,0,300,300), this, ID_GRID_COMBOEDIT); 
   	pCombo->SetFont(u_pFont);  
   	CRect rcSize;
   	pCombo->GetWindowRect(rcSize); 
    u_RowWidth = rcSize.Height() - 1;
    int nHeightStatic = pCombo->GetItemHeight(-1);
    u_ComboGap = rcSize.Height() - nHeightStatic;
    pCombo->DestroyWindow();
    delete pCombo;

	GetMSGWnd()->SendMessage(WM_GRD_SET_ROWHEIGHT, (WPARAM)this, u_RowWidth);
}

void CUIGrid::SetRowHeight(int RowWidth)
{
	if (RowWidth < 0 && u_bDefRowWidth)
		return;

	if (RowWidth < 0)
	{
		SetDefaultRowWidth();
		u_bDefRowWidth = TRUE;
	}
	else
	{
		u_RowWidth = RowWidth;
		u_bDefRowWidth = FALSE;
	}
	
	if (::IsWindow(m_hWnd))
	{
		CRect rcClient;
		GetClientRect(rcClient);
		u_Rows = (rcClient.Height() + u_RowWidth - 1) / u_RowWidth;
	}
}

BOOL CUIGrid::NeedData(long nRow)
{
	return TRUE;
}

void CUIGrid::OnTitleButtonPressed(int nIndex)
{
};

void CUIGrid::SetFixedRowLabel(CString StrRow, CString Del)
{
	u_bWithFixedRow = TRUE;

	if (u_pFixedRow != NULL)
		u_pFixedRow->RemoveAll();

	int	 i = 0;
	char *pToken = StrTrace((char *)(const char*)StrRow,
							(char *)(const char*)Del); 
	while (pToken != NULL) 
	{
		SetFixedRowLabelAt(i++, pToken);
		pToken = StrTrace(NULL,(char *)(const char*)Del);
	}
}

CString CUIGrid::GetFixedRowLabelAt(int nIndex)
{
	if (!u_bWithFixedRow)
		return "";
	if (u_pFixedRow == NULL)
		return "";
	if (nIndex < 0 || nIndex >= u_pFixedRow->GetSize())
		return "";
	return u_pFixedRow->GetAt(nIndex);
}

void CUIGrid::SetFixedRowLabelAt(int nIndex, CString StrRow)
{
	if (!u_bWithFixedRow)
		return;
	if (nIndex < 0 || nIndex >= u_Cols)
		return;
	if (u_pFixedRow == NULL)
		u_pFixedRow = new CStringArray;
	u_pFixedRow->SetAtGrow(nIndex, StrRow);
}

void CUIGrid::SetFixedColLabel(CString StrCol, CString Del)
{
	u_bWithFixedCol = TRUE;

	if (u_pFixedCol != NULL)
		u_pFixedCol->RemoveAll();

	int  i = 0;
	char *pToken = StrTrace((char *)(const char*)StrCol,
							(char *)(const char*)Del); 
	while (pToken != NULL) 
	{
		SetFixedColLabelAt(i++, pToken);
		pToken = StrTrace(NULL,(char *)(const char*)Del);
	}
}

CString CUIGrid::GetFixedColLabelAt(int nIndex)
{
	if (!u_bWithFixedCol)
		return "";
	if (u_pFixedCol == NULL)
		return "";
	if (nIndex < 0 || nIndex >= u_pFixedCol->GetSize())
		return "";
	return u_pFixedCol->GetAt(nIndex);
}

void CUIGrid::SetFixedColLabelAt(int nIndex, CString StrCol)
{
	if (!u_bWithFixedCol)
		return;
	if (nIndex < 0)
		return;
	if (u_pFixedCol == NULL)
		u_pFixedCol = new CStringArray;
	u_pFixedCol->SetAtGrow(nIndex, StrCol);
}

void CUIGrid::OnColSwap(int SourceX, int DestX)
{
}

LRESULT CUIGrid::OnComboEditChange(WPARAM wParam, LPARAM lParam)
{
    GetMSGWnd()->SendMessage(WM_GRD_COMBOEDIT_CHANGE, (WPARAM)this, (LPARAM)u_pComboOrEdit);
	return 0L;
}

LRESULT CUIGrid::OnCtrlLoseFocus(WPARAM wParam, LPARAM lParam)
{
	if (!u_bNoUpdate)
		UpdateGrdData();
	return 0L;
}

CString CUIGrid::SetHeadString(long i)
{
	char szBuffer[64];
	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
 	ltoa(i + 1, szBuffer, 10);
	return szBuffer;	
}

void CUIGrid::ExtraDrawHeadSel(CDC* pDC, CRect rcFixed, long i)
{
}

void CUIGrid::SetEditOverWrite(BOOL bFlag)
{
	u_bOverWrite = bFlag;
	if (u_pEdit != NULL)
		u_pEdit->SetOverWrite(bFlag);
	if (u_pPassEdit != NULL)
		u_pPassEdit->SetOverWrite(bFlag);
	if (u_pComboEdit != NULL)
		u_pComboEdit->SetOverWrite(bFlag);
}

int CUIGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (u_bDefRowWidth)
		SetDefaultRowWidth();
	if (u_bDynaHeadWidth)
		SetDynaHeadWidth();
	
	return 0;
}

BOOL CUIGrid::IsRowVisible(long nRow)
{
	int IntFixedRow = (u_bWithFixedRow) ? 1 : 0;
	BOOL bRet = (nRow < u_VirtualScrollPos);
	bRet = bRet || (nRow >= u_VirtualScrollPos + u_Rows - 1 - IntFixedRow);
	return !bRet;
}

int	CUIGrid::GetColWidth(int nIndex)
{
	if (nIndex < 0 || nIndex >= u_Cols)
		return -1;
	return u_ColWidth[nIndex];
}

void CUIGrid::SetGridSelType(int SelType, BOOL bMultiSel, BOOL bToggle)
{
	if (SelType < cGridNonSel || SelType > cGridHeadSel)
		return;
	u_SelType = SelType;
	u_bMultiSel = bMultiSel;

	PreRemoveSelRow(-1);
	u_bToggleSel = bToggle;
	delete u_pSelArray;
	u_pSelArray = NULL;
	if (bToggle || !bMultiSel)
		u_pSelArray = new CDWordArray;
}

BOOL CUIGrid::GetSelPos(long nPos, int& nIndex)
{
	int nCount = u_pSelArray->GetSize();
	int nUpper = nCount - 1;
	int nLower = 0;
	BOOL bFound = FALSE;
	while (nUpper >= nLower)
	{
		int nMiddle = (nUpper + nLower) / 2;
		long n = u_pSelArray->GetAt(nMiddle);
		if (n == nPos)
		{
			nIndex = nMiddle;
			return TRUE;
		}
		else if (n > nPos)
			nUpper = nMiddle - 1;
		else
			nLower = nMiddle + 1;
	}
	nIndex = nLower;
	return bFound;
}

void CUIGrid::SelOrDeselARow(long lRow, BOOL bDraw)
{
	int nIndex;
	
	CDC* pDC = GetDC();
	
	if (GetSelPos(lRow, nIndex))
	{
		PreRemoveSelRow(nIndex);
		u_pSelArray->RemoveAt(nIndex);
		if (bDraw)
			DrawSelRegion(pDC, lRow, lRow, TRUE);	
	}	
	else
	{
		PreInsertSelRow(nIndex, lRow);		
		u_pSelArray->InsertAt(nIndex, lRow);
		
		int nSelRow = lRow - u_FrameTop;
		
		if (bDraw)
			DrawSelRegion(pDC, lRow, lRow, FALSE);	
	}
	
	ReleaseDC(pDC);
}

void CUIGrid::DrawSelRows(CDC* pDC, long StartLine, long EndLine, BOOL bBtnUp)			
{
	int nStartIndex;
	GetSelPos(StartLine, nStartIndex);

	int nEndIndex;
	GetSelPos(EndLine, nEndIndex);

	for (int i = nStartIndex; i < nEndIndex; i++)
	{
		long n = u_pSelArray->GetAt(i);
		DrawSelRegion(pDC, n, n, bBtnUp);	
	}
}

void CUIGrid::SetRowsAndScrollBar()
{
	CRect rcClient;
	GetClientRect(rcClient);
	u_Rows = (rcClient.Height() + u_RowWidth - 1) / u_RowWidth;

	SetVertScrollRange();
	SetHorzScrollRange();
}

CStringArray* CUIGrid::CopyStrArrayNoAttrib(CStringArray* pSourceArray)
{
    if (pSourceArray == NULL)
    	return NULL;
    
    // Copy to temporary buffer 
    CStringArray* pDestArray = new CStringArray;
  	
    int Bound = pSourceArray->GetSize(); 
	const char* p; 
	for (int i = 0 ; i < Bound ; i++)
	{
        p = (const char*)pSourceArray->GetAt(i);
		if (i < u_Cols)
			pDestArray->Add(p + cAttribNo);
		else
			pDestArray->Add(p);
	}
    return pDestArray;
}

int CUIGrid::GetGridData(long Start, int Size, CObArray* pArray, BOOL bSel)
{
	int n = u_pStrArray->GetSize();
	for (long i = Start - 1; i < Start + Size - 1; i++)
	{
		if (i >= n)
			break;

		CStringArray* pStrArray = (CStringArray*)u_pStrArray->GetAt(i);
		pArray->Add(CopyStrArrayNoAttrib(pStrArray));
	}
	return pArray->GetSize();
}

void CUIGrid::AddValueMap(CString StrMap, CString StrValue)
{
	if (u_pValueMapArray == NULL)
		u_pValueMapArray = new CStringArray;
	u_pValueMapArray->Add(StrMap);
	u_pValueMapArray->Add(StrValue);
}

CString CUIGrid::GetValueMap(CString StrMap)
{
	if (u_pValueMapArray == NULL)
		return StrMap;
	int nSize = u_pValueMapArray->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (u_pValueMapArray->GetAt(i++) == StrMap)
			return u_pValueMapArray->GetAt(i);
	}
	return StrMap;
}

void CUIGrid::OnEditPaste()
{
    if (!u_bPasteable)
		return;

	if (OpenClipboard())
	{
		HANDLE hMem = ::GetClipboardData(CF_TEXT);
		LPTSTR pStr = (LPTSTR)::GlobalLock(hMem);
		CStringArray* pStrArray = new CStringArray;
		char* p = strtokex(pStr, "\n");
		CString StrTemp;
		while (p != NULL)
		{
			StrTemp = p;
			int nLen = StrTemp.GetLength();
			if (StrTemp[nLen - 1] == '\r')
				StrTemp = StrTemp.Left(nLen - 1);
			pStrArray->Add(StrTemp);
			p = strtokex(NULL, "\n");
		}
		::GlobalUnlock(hMem);
   		::CloseClipboard();

		PasteData(pStrArray);

		delete pStrArray;
	}
}

void CUIGrid::OnEditCopy()
{
	if (!u_bCopyable)
		return;

	if (OpenClipboard())
	{
		BeginWaitCursor();
		
		long nDataSize = GetDataSize();
		CObArray* pObArray = new CObArray;
		GetGridData(1, nDataSize, pObArray, TRUE);
		CStringArray* pStrArray = new CStringArray;
		
		CString StrTemp;
		while (pObArray->GetSize() > 0)
		{
			CStringArray* pStr = (CStringArray*)pObArray->GetAt(0);
			StrTemp = "";
			int nSize = pStr->GetSize();
			for (int i = 0; i < nSize; i++)
			{
				if (i != nSize - 1)
					StrTemp += pStr->GetAt(i) + "\t";
				else
					StrTemp += pStr->GetAt(i) + "\r\n";
			}
			pStrArray->Add(StrTemp);
			pObArray->RemoveAt(0);
			delete pStr;
		}
		delete pObArray;
		
       	DWORD nCount = 0;
		int nSize = pStrArray->GetSize();
		int i = 0;
		for (i = 0; i < nSize; i++)
			nCount += pStrArray->GetAt(i).GetLength();

		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE,(DWORD)nCount + 1);
	    char* lpszText = (char*)GlobalLock(hData);

        nCount = 0;
	    for (i = 0; i < nSize; i++)
	    {
	        CString StrTemp = pStrArray->GetAt(i);
	        int StrLen = StrTemp.GetLength();
            memcpy(lpszText + nCount, (LPCTSTR)StrTemp, StrLen);
            nCount += StrLen;
	    }
        lpszText[nCount] = '\0';
			
	    GlobalUnlock(hData);
        ::EmptyClipboard();
	    ::SetClipboardData(CF_TEXT, hData);
    	::CloseClipboard();

		delete pStrArray;

		EndWaitCursor();
	}
}

void CUIGrid::CheckCursor(CPoint point)
{
	int nRow, nCol;
	PointToRowCol(point, nRow, nCol);
	int nType = GetAttribAt(nRow, nCol, cType);
	if (nType == cGridHyperLink || nType == cGridHyperBMP)
		::SetCursor(AfxGetApp()->LoadCursor(MCG_IDC_HAND));
	else
   		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

LRESULT CUIGrid::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bLeaveTrack = FALSE;
	if (u_PreOverRow >= 0 && u_PreOverCol >= 0)
	{
		CRect rc = RowColToRect(u_PreOverRow, u_PreOverCol);
		u_PreOverRow  = -1;
		u_PreOverCol = -1;
		RedrawWindow(rc, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return 0;
}

void CUIGrid::LeaveTrack() 
{
	if (!m_bLeaveTrack)
	{
		m_bLeaveTrack = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
	}
}
