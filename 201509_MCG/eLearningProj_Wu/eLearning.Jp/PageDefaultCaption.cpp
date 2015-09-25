// PageDefaultCaption.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "PageDefaultCaption.h"
#include "macro.h"
#include "cmkapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultCaption property page

IMPLEMENT_DYNCREATE(CPageDefaultCaption, CPropertyPage)

CPageDefaultCaption::CPageDefaultCaption() : CPropertyPage(CPageDefaultCaption::IDD)
{
	//{{AFX_DATA_INIT(CPageDefaultCaption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageDefaultCaption::~CPageDefaultCaption()
{
}

void CPageDefaultCaption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDefaultCaption)
	DDX_Control(pDX, IDC_BUTTON_COLOR_BG, m_cbBG);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FG, m_cbFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDefaultCaption, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDefaultCaption)
	ON_BN_CLICKED(IDC_RADIO_OPAQUE, OnRadioOpaque)
	ON_BN_CLICKED(IDC_RADIO_TRANSPARENT, OnRadioTransparent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDefaultCaption message handlers

void CPageDefaultCaption::OnOK() 
{
	CString strIniPath = MACRO_INI_PATH;
	CString strTmp;
	m_cboFont.GetWindowText(strTmp);
	::WritePrivateProfileString("Caption", "FontName", strTmp, strIniPath);
	
	GetDlgItem(IDC_COMBO_FONT_SIZE)->GetWindowText(strTmp);
	::WritePrivateProfileString("Caption", "FontSize", strTmp, strIniPath);

	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOLD))->GetCheck();
	strTmp.Format("%d", nCheck);
	::WritePrivateProfileString("Caption", "FontBold", strTmp, strIniPath);

	nCheck = ((CButton*)GetDlgItem(IDC_CHECK_ITALIC))->GetCheck();
	strTmp.Format("%d", nCheck);
	::WritePrivateProfileString("Caption", "FontItalic", strTmp, strIniPath);

	nCheck = ((CButton*)GetDlgItem(IDC_CHECK_UNDERLINE))->GetCheck();
	strTmp.Format("%d", nCheck);
	::WritePrivateProfileString("Caption", "FontUnderline", strTmp, strIniPath);

	COLORREF cr = m_cbFG.GetColor();
	strTmp.Format("%ld", cr);
	::WritePrivateProfileString("Caption", "TextColor", strTmp, strIniPath);

	cr = m_cbBG.GetColor();
	strTmp.Format("%ld", cr);
	::WritePrivateProfileString("Caption", "BKColor", strTmp, strIniPath);

	nCheck = ((CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT))->GetCheck();
	if (nCheck)
		::WritePrivateProfileString("Caption", "BKMode", "1", strIniPath);
	else
		::WritePrivateProfileString("Caption", "BKMode", "2", strIniPath);

	int nCheck0 = ((CButton*)GetDlgItem(IDC_RADIO_UP))->GetCheck();
	int nCheck1 = ((CButton*)GetDlgItem(IDC_RADIO_MIDDLE))->GetCheck();
	int nCheck2 = ((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck();
	int nCheck3 = ((CButton*)GetDlgItem(IDC_RADIO_LEFT))->GetCheck();
	int nCheck4 = ((CButton*)GetDlgItem(IDC_RADIO_CENTER))->GetCheck();
	int nCheck5 = ((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->GetCheck();
	int nPos = nCheck0 * 6 + nCheck1 * 3 + nCheck2 * 0 + nCheck3 * 0 + nCheck4 * 1 + nCheck5 * 2;
	strTmp.Format("%d", nPos);
	::WritePrivateProfileString("Caption", "Position", strTmp, strIniPath);

	//add by buxiangyi
	CString H_indent, V_indent;
	GetDlgItem(IDC_EDIT_H_INDENT)->GetWindowText(H_indent);
	GetDlgItem(IDC_EDIT_V_INDENT)->GetWindowText(V_indent);
	if (H_indent == "")
	{
		H_indent = "3";

	}
	if (V_indent == "")
	{
		V_indent = "3";

	}
	::WritePrivateProfileString("Caption", "H_INDENT", H_indent, strIniPath);
	::WritePrivateProfileString("Caption", "V_INDENT", V_indent, strIniPath);
	
	CPropertyPage::OnOK();
}

BOOL CPageDefaultCaption::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_cboFont.SubclassDlgItem(IDC_COMBO_FONT,this);
	m_cboFont.Initialize();
	
	CString strIniPath = MACRO_INI_PATH;
	char szBuf[512];
#ifdef _JAPANESE
	::GetPrivateProfileString("Caption", "FontName", "MS UI Gothic", szBuf, 512, strIniPath);
#else
	::GetPrivateProfileString("Caption", "FontName", "Arial", szBuf, 512, strIniPath);
#endif
	int nIndex = m_cboFont.FindString(-1, szBuf);
	if (nIndex >= 0)
		m_cboFont.SetCurSel(nIndex);

	::GetPrivateProfileString("Caption", "FontSize", "12", szBuf, 512, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT_SIZE);
	nIndex = pCombo->FindString(-1, szBuf);
	if (nIndex >= 0)
		pCombo->SetCurSel(nIndex);

	::GetPrivateProfileString("Caption", "FontBold", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_BOLD))->SetCheck(atoi(szBuf));

	::GetPrivateProfileString("Caption", "FontItalic", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_ITALIC))->SetCheck(atoi(szBuf));

	::GetPrivateProfileString("Caption", "FontUnderline", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_UNDERLINE))->SetCheck(atoi(szBuf));

	CString strTmp;
	COLORREF crWhite = RGB(255, 255, 255);
	strTmp.Format("%ld", crWhite);
	::GetPrivateProfileString("Caption", "TextColor", strTmp, szBuf, 512, strIniPath);
	m_cbFG.SetColor(COLORREF(atol(szBuf)));

	COLORREF crBlack = RGB(0, 0, 0);
	strTmp.Format("%ld", crBlack);
	::GetPrivateProfileString("Caption", "BKColor", strTmp, szBuf, 512, strIniPath);
	m_cbBG.SetColor(COLORREF(atol(szBuf)));

	::GetPrivateProfileString("Caption", "BKMode", "2", szBuf, 512, strIniPath);
	int nTmp = atoi(szBuf);
	if (nTmp == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT))->SetCheck(1);
		m_cbBG.EnableWindow(FALSE);
	}
	else 
		((CButton*)GetDlgItem(IDC_RADIO_OPAQUE))->SetCheck(1);

	::GetPrivateProfileString("Caption", "Position", "1", szBuf, 512, strIniPath);
	nTmp = atoi(szBuf);
	int nBase = nTmp / 3;
	int nOffset = nTmp % 3;
	if (nBase == 0)
		((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(1);
	else if (nBase == 1)
		((CButton*)GetDlgItem(IDC_RADIO_MIDDLE))->SetCheck(1);
	else if (nBase == 2)
		((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(1);
	if (nOffset == 0)
		((CButton*)GetDlgItem(IDC_RADIO_LEFT))->SetCheck(1);
	else if (nOffset == 1)
		((CButton*)GetDlgItem(IDC_RADIO_CENTER))->SetCheck(1);
	else if (nOffset == 2)
		((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->SetCheck(1);


	::GetPrivateProfileString("Caption", "V_INDENT", "3", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_V_INDENT)->SetWindowText(szBuf);

	::GetPrivateProfileString("Caption", "H_INDENT", "3", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_H_INDENT)->SetWindowText(szBuf);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDefaultCaption::OnRadioOpaque() 
{
	m_cbBG.EnableWindow(TRUE);
}

void CPageDefaultCaption::OnRadioTransparent() 
{
	m_cbBG.EnableWindow(FALSE);
}
