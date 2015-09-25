// ColGrid.cpp : implementation file
//

#include "stdafx.h"
//#include "eLearning.h"
#include "ColGrid.h"
#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColGrid

CColGrid::CColGrid()
{
	u_pColAttrib = new CStringArray;
	//u_pColAttrib->Add(u_StrDefAttrib);
	
	u_pColInit = new CStringArray;
	//u_pColInit->Add("");
    u_bLastRowStar = FALSE;
	u_bInsertable = TRUE;
	u_bDeletable = TRUE;
}

CColGrid::~CColGrid()
{
	delete u_pColAttrib;
	delete u_pColInit;
}

BEGIN_MESSAGE_MAP(CColGrid, CUIGrid)
	//{{AFX_MSG_MAP(CColGrid)
	ON_MESSAGE(WM_GRD_COMBOEDIT_CHANGE, OnComboEditChange)
	ON_EN_CHANGE(ID_GRID_EDIT, OnEditChange)
	ON_CBN_SELCHANGE(ID_GRID_COMBO, OnSelChange)
	ON_CBN_SELCHANGE(ID_GRID_COMBOEDIT, OnSelChange)
	ON_CBN_SELCHANGE(ID_GRID_BMP_COMBO, OnSelChange)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColGrid::PostSetColNo(int nCol)
{
	int i;
	if (u_Cols > nCol)
	{
		for (i = 0; i < u_Cols - nCol; i++)
		{
			u_pColAttrib->RemoveAt(u_Cols - i - 1);
			u_pColInit->RemoveAt(u_Cols - i - 1);
		}
	}
	else
	{
		for (i = 0; i < nCol - u_Cols; i++)
		{
			CString StrAttrib = u_StrDefAttrib;
			if (u_Cols != 0)
				StrAttrib = u_pColAttrib->GetAt(u_Cols - 1);
			u_pColAttrib->Add(StrAttrib);
			u_pColInit->Add("");
		}
	}
}

void CColGrid::SetColAttribAt(int nCol, int nType, char nValue, BOOL bReflect)
{
	if (nCol < 0 || nCol >= u_Cols)
		return;

	CString StrCell = u_pColAttrib->GetAt(nCol);
	StrCell.SetAt(nType, nValue + 1);
	u_pColAttrib->SetAt(nCol, StrCell);

	if (bReflect)
	{
		int nSize = u_pStrArray->GetSize();
		for (int i = 0; i < nSize; i++)
			SetAttribAt(i, nCol, nType, nValue);
	}
}

/*
void CColGrid::SetColAttribAt(int nCol, char nType, char nIndex, 
							  char nEditable, char nAlign,
							  char nBGColor, char nFGColor)
{
	if (nCol < 0 || nCol >= u_Cols)
		return;

	CString StrAttrib;
	StrAttrib.Format("%c%c%c%c%c%c%c%c%c%c", 
		nType + 1, nIndex + 1, nEditable + 1, 
		nAlign + 1, nBGColor + 1, nFGColor + 1, 1, 1, 1, 1);
	u_pColAttrib->SetAt(nCol, StrAttrib);
}

CString CColGrid::GetColAttribAt(int nCol)
{
	if (nCol < 0 || nCol >= u_Cols)
		return u_StrDefAttrib;

	return u_pColAttrib->GetAt(nCol);
}
*/

char CColGrid::GetColAttribAt(int nCol, int nType)
{
	if (nCol < 0 || nCol >= u_Cols)
		return -1;

	CString StrCell = u_pColAttrib->GetAt(nCol);
	return StrCell.GetAt(nType) - 1;
}

CString CColGrid::AddAttribNew(int nRow, int nCol, CString StrContent)
{
	return u_pColAttrib->GetAt(nCol) + u_pColInit->GetAt(nCol);
}

CString CColGrid::AddAttribSet(int nRow, int nCol, CString StrContent)
{
	StrContent = GetValueMap(StrContent);
	return u_pColAttrib->GetAt(nCol) + StrContent;
}

void CColGrid::SetColInitAt(int nCol, CString StrInit)
{
	if (nCol < 0 || nCol >= u_Cols)
		return;

	u_pColInit->SetAt(nCol, StrInit);
}

void CColGrid::PostLButtonDown(int ClickedRow, int ClickedCol)
{
	if (GetDataAt(ClickedRow, ClickedCol) != "")
		CheckToAddLastRow(ClickedRow, ClickedCol);
}

void CColGrid::CheckToAddLastRow(int ClickedRow, int ClickedCol)
{
	if (ClickedRow + u_FrameTop == GetDataSize() - 1 && u_bLastRowStar)
	{
		int nHeadWidth = u_HeadWidth;
		AppendRow();
		CRect rcCell = RowColToRect(ClickedRow + 1, ClickedCol);
		if (rcCell.IsRectEmpty())
			OnScroll(SB_VERT, SB_LINEDOWN, 0);
		if (nHeadWidth == u_HeadWidth)
			RePaintRow(ClickedRow, ClickedRow + 1);
		else
			RePaint();
	}
} 

void CColGrid::OnEditChange()
{
	CUIGrid::OnEditChange();

   	CString StrEdit;
   	u_pComboOrEdit->GetWindowText(StrEdit);
	if (StrEdit != "")
		CheckToAddLastRow(u_PreRow, u_PreCol);
}

LRESULT CColGrid::OnComboEditChange(WPARAM wParam, LPARAM lParam)
{
	CUIGrid::OnComboEditChange(wParam, lParam);

   	CString StrEdit;
   	u_pComboOrEdit->GetWindowText(StrEdit);
	if (StrEdit != "")
		CheckToAddLastRow(u_PreRow, u_PreCol);
	return 0L;
}

CString CColGrid::SetHeadString(long i)
{
	if (i == GetDataSize() - 1 && u_bLastRowStar)
   		return "*";
	return CUIGrid::SetHeadString(i);
}

void CColGrid::DelSelected()
{
	if (!u_bDeletable || !u_bEditable)
		return;

	if (u_bToggleSel)
	{
		int nSize = u_pSelArray->GetSize();
		for (int i = 0; i < nSize; i++)
		{
			long nPos = u_pSelArray->GetAt(i);
			DeleteRows(nPos, nPos);
		}
	}
	else
	{
		if (u_SelectedRow < 0)
			return;

		if (u_bLastRowStar && u_EndSelectedRow == GetDataSize() - 1)
			u_EndSelectedRow--;
		if (u_EndSelectedRow >= u_SelectedRow)
			DeleteRows(u_SelectedRow, u_EndSelectedRow);
		
		u_SelectedRow = -1;
		u_EndSelectedRow = -1;
	}

	RePaint();
}

void CColGrid::InsertAtSelectedRow()
{
	if (!u_bInsertable || !u_bEditable)
		return;

	if (u_SelectedRow != -1 && 
		u_SelectedRow == u_EndSelectedRow)
	{
		int nIndex = u_SelectedRow;
		if (!IsRowVisible(nIndex))
			SetVScrollPos(max(0, u_SelectedRow - u_Rows / 2 + 1));

		InsertRowAt(nIndex, 1);
		OnScroll(SB_HORZ, SB_TOP, 0);
		ClickRowCol(nIndex, 0);
		RePaint();
	}
}

void CColGrid::OnSelChange()
{
  	CString StrEdit;
   	CComboBox* pCombo = (CComboBox*)u_pComboOrEdit;
	int nSel = pCombo->GetCurSel();
	if (nSel != CB_ERR)
	{
		pCombo->GetLBText(nSel, StrEdit);
		if (StrEdit != "")
			CheckToAddLastRow(u_PreRow, u_PreCol);
	}
	CUIGrid::OnComboSelChange();
}

void CColGrid::KeyDown(UINT nChar)
{
    switch (nChar)
	{
		case VK_DELETE:
			//DelSelected();
			break;
		
		case VK_INSERT:
			//InsertAtSelectedRow();
			break;
	}
	CUIGrid::KeyDown(nChar);
}

CBrush* CColGrid::GetDefBrush(int nCol)
{
	unsigned char nIndex = GetColAttribAt(nCol, cBGColor);
	
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

/*
CBrush* CColGrid::GetBrush(int nRow, int nCol)
{
	if (!ValidRowCol(nRow, nCol))
		return u_pDefBrush;

	unsigned char nIndex = GetColAttribAt(nCol, cBGColor);
	
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
*/


