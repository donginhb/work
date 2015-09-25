// DlgGotoSlide.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgGotoSlide.h"
#include "ColGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGotoSlide dialog


CDlgGotoSlide::CDlgGotoSlide(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGotoSlide::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGotoSlide)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pstrArrayTitle = NULL;
}


void CDlgGotoSlide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGotoSlide)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGotoSlide, CDialog)
	//{{AFX_MSG_MAP(CDlgGotoSlide)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_GRD_CTRL_ON, OnHyperLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGotoSlide message handlers

BOOL CDlgGotoSlide::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	m_pGrid = new CColGrid;
	m_pGrid->Create(rcGrid, this, 0);
	m_pGrid->SetTotalColNo(2);
	m_pGrid->SetColAttribAt(0, cType, cGridBMP);
	m_pGrid->SetColAttribAt(1, cType, cGridHyperLink);
	m_pGrid->SetColAttribAt(0, cEditable, 0);
	m_pGrid->SetColAttribAt(1, cEditable, 1);
	m_pGrid->SetColAttribAt(0, cIndex, 0);
	m_pGrid->SetColWidth(0, 24); 
	m_pGrid->SetColWidth(1, rcGrid.Width()); 

	CBitmap *pBmp;
	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_SPACE);
	m_pGrid->SetBMPAt(cBMP, 0, pBmp);

	pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_CHECK);
	m_pGrid->SetBMPAt(cBMP, 1, pBmp);

	m_pGrid->SetSeqAt(cBMP, 0, "0\\1");

	m_pGrid->u_bLastRowStar = FALSE;
	m_pGrid->u_pMSGWnd = this;
	m_pGrid->SetGridSelType(cGridNonSel, TRUE, FALSE);		//MultiSel : True , Toggle : False
	m_pGrid->SetColor(cHLineColor, m_pGrid->GetColor(cBGColor));
	m_pGrid->SetColor(cVLineColor, m_pGrid->GetColor(cBGColor));
	m_pGrid->SetColor(cSelColor, m_pGrid->GetColor(cBGColor));
	m_pGrid->SetColor(cFGColor, RGB(0, 0, 255));
	m_pGrid->SetRowHeight(18);
	m_pGrid->u_bHScrollable = FALSE;
	
	LoadHyperLink();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGotoSlide::LoadHyperLink() 
{
	if (m_pstrArrayTitle == NULL)
		return;

	m_pGrid->DeleteAllRows();
	int nIndex = 0;
	for (int i = 0; i < m_pstrArrayTitle->GetSize(); i++)
	{
		CString strMark = m_pstrArrayTitle->GetAt(i);
		if (strMark == "")
			strMark.LoadString(IDS_UNDEFINED);
		CString strValue;
		strValue.Format("(%d) %s", i + 1, strMark);
		m_pGrid->AppendRow(1);
		m_pGrid->SetDataAt(nIndex, 0, "0");
		m_pGrid->SetDataAt(nIndex++, 1, strValue);
	}

	m_pGrid->SetDataAt(m_nIndex - 1, 0, "1");
	m_pGrid->SetVScrollPos(m_nIndex - 5);

//	m_pGrid->RePaint();

	return;
}

void CDlgGotoSlide::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pGrid->DestroyWindow();
	delete m_pGrid;
}

LRESULT CDlgGotoSlide::OnHyperLink(WPARAM wParam, LPARAM lParam)
{
	int nPreRow = HIWORD(lParam) + 1;
	m_nIndex = nPreRow;
	EndDialog(ID_GO);

	return 0L;
}

void CDlgGotoSlide::OnCancel() 
{
	CDialog::OnCancel();
}
