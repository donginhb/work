// DlgPenProperty.cpp : implementation file
//

#include "stdafx.h"
#include "elearningcapture.h"
#include "DlgPenProperty.h"
#include "cmkapi.h"
#include "..\elearning.jp\bmpapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPenProperty dialog


CDlgPenProperty::CDlgPenProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPenProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPenProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPenProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPenProperty)
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_cbPen);
	DDX_Control(pDX, IDC_COMBO_IMAGE, m_cbPointer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPenProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgPenProperty)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPenProperty message handlers

void CDlgPenProperty::OnOK() 
{
	CString strIniPath = ::GetSysPath() + "Elearning.ini";

	CString str;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	pCombo->GetWindowText(str);
	::WritePrivateProfileString("PenProperty", "Width", str, strIniPath);

	CString strR, strG, strB;
	COLORREF color = m_cbPen.GetColor();
	int r = GetRValue(color);
	strR.Format("%d", r);

	int g = GetGValue(color);
	strG.Format("%d", g);

	int b = GetBValue(color);
	strB.Format("%d", b);

	::WritePrivateProfileString("PenProperty", "ColorR", strR, strIniPath);
	::WritePrivateProfileString("PenProperty", "ColorG", strG, strIniPath);
	::WritePrivateProfileString("PenProperty", "ColorB", strB, strIniPath);
	
	int nIndex = m_cbPointer.GetCurSel();
	if (nIndex >= 0)
	{
		m_cbPointer.GetLBText(nIndex, str);
		::WritePrivateProfileString("PenProperty", "Pointer", str, strIniPath);
	}

	CDialog::OnOK();
}

CBitmap* CDlgPenProperty::ConvertBMP(CBitmap* pBMP) 
{
    CDC dc;
    dc.CreateCompatibleDC(NULL); 
	
	BITMAP b;
	pBMP->GetBitmap(&b);
	int cx = b.bmWidth;
	int cy = b.bmHeight;

	int nPixel = dc.GetDeviceCaps(BITSPIXEL);
    CBitmap* pNewBMP = new CBitmap;
    pNewBMP->CreateBitmap(cx, cy, 1, nPixel, NULL);

	CBitmap* pOldBMP = dc.SelectObject((CBitmap*)pNewBMP);  

	::FillRect(dc.m_hDC, CRect(0, 0, cx, cy), (HBRUSH)::GetStockObject(WHITE_BRUSH));

	CImageList imageList;
	imageList.Create(cx, cy, ILC_COLORDDB | ILC_MASK, 1, 1);
	imageList.Add(pBMP, RGB(255, 0, 255));
	imageList.Draw(&dc, 0, CPoint(0, 0), ILD_TRANSPARENT);

	dc.SelectObject(pOldBMP); 
    dc.DeleteDC();

	return pNewBMP;
}

BOOL CDlgPenProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//CString strPath = ::GetSysPath() + "Layout\\Pointers\\";
	//::GetFiles(strPath, "bmp", m_strArrayFile);
	//::SortArray(m_strArrayFile);
	//for (int i = 0; i < m_strArrayFile.GetSize(); i++)
	//{
	//	CString strFile = m_strArrayFile.GetAt(i);
	//	CString sPath = strPath + strFile;
	//	CBitmap* pBMP = ::LoadFile(sPath);
	//	CBitmap* pNewBMP = ConvertBMP(pBMP);
	//	m_BMPArray.Add(pNewBMP);
	//	pBMP->DeleteObject();
	//	delete pBMP;
	//}

	for (UINT i = IDB_PTR_FIRST; i <= IDB_PTR_LAST; i++)
	{
		CString str;
		str.Format("%d", i);
		m_strArrayID.Add(str);

		CBitmap* pBMP = new CBitmap;
		pBMP->LoadBitmap(i);
		CBitmap* pNewBMP = ConvertBMP(pBMP);
		m_BMPArray.Add(pNewBMP);
		pBMP->DeleteObject();
		delete pBMP;
	}

	m_cbPointer.m_pBMPArray = &m_BMPArray;
	m_cbPointer.m_pBMPIDArray = &m_strArrayID;

	for (int j = 0; j < m_strArrayID.GetSize(); j++)
		m_cbPointer.AddString(m_strArrayID[j]);

	CString strIniPath = ::GetSysPath() + "Elearning.ini";

	char szBuf[256];
	::GetPrivateProfileString("PenProperty", "Width", "2px", szBuf, 256, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	int nIndex = pCombo->FindString(-1, szBuf);
	if (nIndex >= 0)
		pCombo->SetCurSel(nIndex);

	CString strR, strG, strB;
	::GetPrivateProfileString("PenProperty", "ColorR", "0", szBuf, 256, strIniPath);
	strR = szBuf;
	::GetPrivateProfileString("PenProperty", "ColorG", "0", szBuf, 256, strIniPath);
	strG = szBuf;
	::GetPrivateProfileString("PenProperty", "ColorB", "0", szBuf, 256, strIniPath);
	strB = szBuf;
	COLORREF color = RGB(atoi(strR), atoi(strG), atoi(strB));
	m_cbPen.SetColor(color);

	::GetPrivateProfileString("PenProperty", "Pointer", "0", szBuf, 256, strIniPath);
	if (atoi(szBuf) == 0)
		sprintf(szBuf, "%d", IDB_PTR_FIRST);

	nIndex = m_cbPointer.FindString(-1, szBuf);
	if (nIndex >= 0)
		m_cbPointer.SetCurSel(nIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPenProperty::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for (int i = 0; i < m_BMPArray.GetSize(); i++)
	{
		CBitmap* pBMP = (CBitmap*)m_BMPArray.GetAt(i);
		pBMP->DeleteObject();
		delete pBMP;
	}
}

