// DlgInputCaption.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgInputCaption.h"
#include "macro.h"
#include "cmkapi.h"
#include "CaptionAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCaption dialog


CDlgInputCaption::CDlgInputCaption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputCaption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputCaption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strCaption = "";
	m_strIniPath = MACRO_INI_PATH;
	m_strSection = "Caption";
}


void CDlgInputCaption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputCaption)
	DDX_Control(pDX, IDC_BUTTON_COLOR_BG, m_cbBG);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FG, m_cbFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputCaption, CDialog)
	//{{AFX_MSG_MAP(CDlgInputCaption)
	ON_BN_CLICKED(IDC_RADIO_TRANSPARENT, OnRadioTransparent)
	ON_BN_CLICKED(IDC_RADIO_OPAQUE, OnRadioOpaque)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_BN_CLICKED(IDC_CHECK_BOLD, OnCheckBold)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, OnCheckItalic)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, OnCheckUnderline)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_FG, OnButtonColorFg)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_BG, OnButtonColorBg)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, OnSelchangeComboFont)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, OnSelchangeComboFontSize)
	ON_MESSAGE(CPN_SELENDOK, OnColorSelected)
	ON_MESSAGE(WM_FONTCHANGED, OnFontChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputCaption message handlers

BOOL CDlgInputCaption::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cboFont.SubclassDlgItem(IDC_COMBO_FONT,this);
	m_cboFont.Initialize();

	CString strIniPath = m_strIniPath;
	char szBuf[512];

#ifdef _JAPANESE
	::GetPrivateProfileString(m_strSection, "FontName", "MS UI Gothic", szBuf, 512, strIniPath);
#else
	::GetPrivateProfileString(m_strSection, "FontName", "Arial", szBuf, 512, strIniPath);
#endif
	
	int nIndex = m_cboFont.FindString(-1, szBuf);
	if (nIndex >= 0)
		m_cboFont.SetCurSel(nIndex);

	::GetPrivateProfileString(m_strSection, "FontSize", "12", szBuf, 512, strIniPath);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT_SIZE);
	nIndex = pCombo->FindString(-1, szBuf);
	if (nIndex >= 0)
		pCombo->SetCurSel(nIndex);

	::GetPrivateProfileString(m_strSection, "FontBold", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_BOLD))->SetCheck(atoi(szBuf));

	::GetPrivateProfileString(m_strSection, "FontItalic", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_ITALIC))->SetCheck(atoi(szBuf));

	::GetPrivateProfileString(m_strSection, "FontUnderline", "0", szBuf, 512, strIniPath);
	((CButton*)GetDlgItem(IDC_CHECK_UNDERLINE))->SetCheck(atoi(szBuf));

	CString strTmp;
	COLORREF crWhite = RGB(255, 255, 255);
	strTmp.Format("%ld", crWhite);
	::GetPrivateProfileString(m_strSection, "TextColor", strTmp, szBuf, 512, strIniPath);
	m_cbFG.SetColor(COLORREF(atol(szBuf)));

	COLORREF crBlack = RGB(0, 0, 0);
	strTmp.Format("%ld", crBlack);
	::GetPrivateProfileString(m_strSection, "BKColor", strTmp, szBuf, 512, strIniPath);
	m_cbBG.SetColor(COLORREF(atol(szBuf)));

	::GetPrivateProfileString(m_strSection, "BKMode", "2", szBuf, 512, strIniPath);
	int nTmp = atoi(szBuf);
	if (nTmp == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT))->SetCheck(1);
		m_cbBG.EnableWindow(FALSE);
	}
	else 
		((CButton*)GetDlgItem(IDC_RADIO_OPAQUE))->SetCheck(1);

	::GetPrivateProfileString(m_strSection, "Position", "1", szBuf, 512, strIniPath);
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


	::GetPrivateProfileString(m_strSection, "V_INDENT", "3", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_V_INDENT)->SetWindowText(szBuf);

	::GetPrivateProfileString(m_strSection, "H_INDENT", "3", szBuf, 512, strIniPath);
	GetDlgItem(IDC_EDIT_H_INDENT)->SetWindowText(szBuf);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CAPTION);
	if (m_strCaption != "")
	{
		pEdit->SetWindowText(m_strCaption);
		pEdit->SetSel(0, -1);
	}
	pEdit->SetFocus();

	ApplyBak();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputCaption::OnOK() 
{
	if (!Apply())
		return;

	CDialog::OnOK();
}

BOOL CDlgInputCaption::Apply() 
{
	CString strCaption;
	GetDlgItem(IDC_EDIT_CAPTION)->GetWindowText(strCaption);
	if (strCaption == "")
	{
		W_SHOW_WARNING(IDS_NO_SUB_INPUT, this);
		return FALSE;
	}
	
	m_strCaption = strCaption;

	m_cboFont.GetWindowText(m_strFontName);
	GetDlgItem(IDC_COMBO_FONT_SIZE)->GetWindowText(m_strFontSize);

	m_strItalic = "0";
	int n = ((CButton*)GetDlgItem(IDC_CHECK_ITALIC))->GetCheck();
	if (n)
		m_strItalic = "-1";

	m_strBold = "0";
	n = ((CButton*)GetDlgItem(IDC_CHECK_BOLD))->GetCheck();
	if (n)
		m_strBold = "-1";

	m_strUnderline = "0";
	n = ((CButton*)GetDlgItem(IDC_CHECK_UNDERLINE))->GetCheck();
	if (n)
		m_strUnderline = "-1";

	m_strBkMode = "2";
	n = ((CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT))->GetCheck();
	if (n)
		m_strBkMode = "1";

	m_strTextColor.Format("%ld", m_cbFG.GetColor());
	m_strBkColor.Format("%ld", m_cbBG.GetColor());

	GetDlgItem(IDC_EDIT_V_INDENT)->GetWindowText(m_strVIndent);
	GetDlgItem(IDC_EDIT_H_INDENT)->GetWindowText(m_strHIndent);

	int nCheck0 = ((CButton*)GetDlgItem(IDC_RADIO_UP))->GetCheck();
	int nCheck1 = ((CButton*)GetDlgItem(IDC_RADIO_MIDDLE))->GetCheck();
	int nCheck2 = ((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck();
	int nCheck3 = ((CButton*)GetDlgItem(IDC_RADIO_LEFT))->GetCheck();
	int nCheck4 = ((CButton*)GetDlgItem(IDC_RADIO_CENTER))->GetCheck();
	int nCheck5 = ((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->GetCheck();
	int nPos = nCheck0 * 6 + nCheck1 * 3 + nCheck2 * 0 + nCheck3 * 0 + nCheck4 * 1 + nCheck5 * 2;
	m_strPosition.Format("%d", nPos);

	return TRUE;
}

BOOL CDlgInputCaption::ApplyBak() 
{
	CString strCaption;
	GetDlgItem(IDC_EDIT_CAPTION)->GetWindowText(strCaption);
	
	m_strCaptionBak = strCaption;

	m_cboFont.GetWindowText(m_strFontName);
	GetDlgItem(IDC_COMBO_FONT_SIZE)->GetWindowText(m_strFontSizeBak);

	m_strItalicBak = "0";
	int n = ((CButton*)GetDlgItem(IDC_CHECK_ITALIC))->GetCheck();
	if (n)
		m_strItalicBak = "-1";

	m_strBoldBak = "0";
	n = ((CButton*)GetDlgItem(IDC_CHECK_BOLD))->GetCheck();
	if (n)
		m_strBoldBak = "-1";

	m_strUnderlineBak = "0";
	n = ((CButton*)GetDlgItem(IDC_CHECK_UNDERLINE))->GetCheck();
	if (n)
		m_strUnderlineBak = "-1";

	m_strBkModeBak = "2";
	n = ((CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT))->GetCheck();
	if (n)
		m_strBkModeBak = "1";

	m_strTextColorBak.Format("%ld", m_cbFG.GetColor());
	m_strBkColorBak.Format("%ld", m_cbBG.GetColor());

	GetDlgItem(IDC_EDIT_V_INDENT)->GetWindowText(m_strVIndentBak);
	GetDlgItem(IDC_EDIT_H_INDENT)->GetWindowText(m_strHIndentBak);

	int nCheck0 = ((CButton*)GetDlgItem(IDC_RADIO_UP))->GetCheck();
	int nCheck1 = ((CButton*)GetDlgItem(IDC_RADIO_MIDDLE))->GetCheck();
	int nCheck2 = ((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck();
	int nCheck3 = ((CButton*)GetDlgItem(IDC_RADIO_LEFT))->GetCheck();
	int nCheck4 = ((CButton*)GetDlgItem(IDC_RADIO_CENTER))->GetCheck();
	int nCheck5 = ((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->GetCheck();
	int nPos = nCheck0 * 6 + nCheck1 * 3 + nCheck2 * 0 + nCheck3 * 0 + nCheck4 * 1 + nCheck5 * 2;
	m_strPositionBak.Format("%d", nPos);

	return TRUE;
}

void CDlgInputCaption::OnRadioTransparent() 
{
	m_cbBG.EnableWindow(FALSE);
	RedrawText();
}

void CDlgInputCaption::OnRadioOpaque() 
{
	m_cbBG.EnableWindow(TRUE);
	RedrawText();
}

void CDlgInputCaption::OnApply() 
{
	CCaptionAll dlg;
	if (dlg.DoModal() != IDOK)
		return;
	if (Apply())
		EndDialog(ID_APPLY);
}

void CDlgInputCaption::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcDC;
	GetDlgItem(IDC_STATIC_DC)->GetWindowRect(rcDC);
	ScreenToClient(rcDC);
	
//	::DrawPanel(dc.m_hDC, rcDC, TRUE);

	dc.SetROP2(R2_COPYPEN);
	
   	CPen pen1;
	
	pen1.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	CPen* pOldPen;
	pOldPen = (CPen*)dc.SelectObject(&pen1);

	dc.MoveTo(rcDC.left, rcDC.bottom);
	dc.LineTo(rcDC.left, rcDC.top);
	dc.LineTo(rcDC.right, rcDC.top); 
	//(HPEN)::SelectObject(hDC, OldPen);
     
   	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
    dc.SelectObject(&pen2);
    //::MoveTo(hDC,lpRect->right, lpRect->top);
	dc.LineTo(rcDC.right, rcDC.bottom);
	dc.LineTo(rcDC.left, rcDC.bottom);

    pen1.DeleteObject();
    pen2.DeleteObject();

	CString strCaption;
	GetDlgItem(IDC_EDIT_CAPTION)->GetWindowText(strCaption);

    CString strSize;
	GetDlgItem(IDC_COMBO_FONT_SIZE)->GetWindowText(strSize);
	LOGFONT LogFont;
	LogFont.lfHeight = atol(strSize) * 4 / 3;
    LogFont.lfWidth = 0;
    LogFont.lfEscapement = 0;
    LogFont.lfOrientation = 0;
    CButton* pCheckBold;
    CButton* pCheckItalic;
    CButton* pCheckUnderline;
	pCheckBold = (CButton*)GetDlgItem(IDC_CHECK_BOLD);
	if (pCheckBold->GetCheck())
		LogFont.lfWeight = FW_BOLD;
	else
		LogFont.lfWeight = FW_NORMAL;

	pCheckItalic = (CButton*)GetDlgItem(IDC_CHECK_ITALIC);
	pCheckUnderline = (CButton*)GetDlgItem(IDC_CHECK_UNDERLINE);
    LogFont.lfItalic = pCheckItalic->GetCheck();
    LogFont.lfUnderline = pCheckUnderline->GetCheck();
    LogFont.lfStrikeOut = 0;
    LogFont.lfCharSet = DEFAULT_CHARSET;
    LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    LogFont.lfQuality = PROOF_QUALITY;
    LogFont.lfPitchAndFamily = FF_ROMAN;
	CString strFont;
	GetDlgItem(IDC_COMBO_FONT)->GetWindowText(strFont);
	strncpy((char*)LogFont.lfFaceName, strFont, sizeof LogFont.lfFaceName);
    
	dc.SetTextColor(m_cbFG.GetColor());
	CButton* pBkTransparent;
	pBkTransparent = (CButton*)GetDlgItem(IDC_RADIO_TRANSPARENT);
	if (pBkTransparent->GetCheck())
		dc.SetBkMode(TRANSPARENT);
	else
	{
		dc.SetBkMode(OPAQUE);
		dc.SetBkColor(m_cbBG.GetColor());
	}

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&LogFont);
	CFont* pOldFont;
	pOldFont = dc.SelectObject(&fontDraw);

	CPen pen3;
	pen3.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWTEXT));
    dc.SelectObject(&pen3);
	rcDC.left += 5;
	rcDC.right -= 5;
	dc.DrawText(strCaption, strCaption.GetLength(), rcDC, DT_VCENTER | DT_SINGLELINE | DT_LEFT);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}

void CDlgInputCaption::OnChangeEditCaption() 
{
	RedrawText();
}

void CDlgInputCaption::RedrawText() 
{
	CRect rcDC;
	GetDlgItem(IDC_STATIC_DC)->GetWindowRect(rcDC);
	ScreenToClient(rcDC);

    RedrawWindow(rcDC);
}

void CDlgInputCaption::OnCheckBold() 
{
	RedrawText();
}

void CDlgInputCaption::OnCheckItalic() 
{
	RedrawText();
}

void CDlgInputCaption::OnCheckUnderline() 
{
	RedrawText();
}

void CDlgInputCaption::OnButtonColorFg() 
{
}

void CDlgInputCaption::OnButtonColorBg() 
{
}

void CDlgInputCaption::OnSelchangeComboFont() 
{
	RedrawText();
}

void CDlgInputCaption::OnSelchangeComboFontSize() 
{
	RedrawText();
}

LRESULT CDlgInputCaption::OnColorSelected(WPARAM wParam, LPARAM lParam)
{
	RedrawText();
	return 0L;
}

LRESULT CDlgInputCaption::OnFontChanged(WPARAM wParam, LPARAM lParam)
{
	RedrawText();
	return 0L;
}
