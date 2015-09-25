// DlgAdjustVolAndVideo.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgAdjustVolAndVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustVolAndVideo dialog


CDlgAdjustVolAndVideo::CDlgAdjustVolAndVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdjustVolAndVideo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdjustVolAndVideo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nContrastValue = -1;
	m_nLightValue = -1;
	m_nVolumeValue = -1;
}


void CDlgAdjustVolAndVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdjustVolAndVideo)
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
	DDX_Control(pDX, IDC_SLIDER_LIGHT, m_sliderLight);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContrast);
	DDX_Control(pDX, IDC_STATIC_VOLUME, m_volume);
	DDX_Control(pDX, IDC_STATIC_LIGHT, m_light);
	DDX_Control(pDX, IDC_STATIC_CONTRAST, m_contrast);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAdjustVolAndVideo, CDialog)
	//{{AFX_MSG_MAP(CDlgAdjustVolAndVideo)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustVolAndVideo message handlers

BOOL CDlgAdjustVolAndVideo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_sliderVolume.SetRange(0, 200);
	if (m_nVolumeValue >= 0)
		m_sliderVolume.SetPos(m_nVolumeValue);
	else	
		m_sliderVolume.SetPos(100);

	m_sliderLight.SetRange(0, 200);
	m_sliderLight.SetPos(100);
	if (m_nLightValue >= 0)
		m_sliderLight.SetPos(m_nLightValue);
	else	
		m_sliderLight.SetPos(100);
	
	m_sliderContrast.SetRange(0, 200);
	if (m_nContrastValue >= 0)
		m_sliderContrast.SetPos(m_nContrastValue);
	else	
		m_sliderContrast.SetPos(100);

	SetTimer(1, 200, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAdjustVolAndVideo::OnOK() 
{
	m_nContrastValue = m_sliderContrast.GetPos();
	m_nLightValue = m_sliderLight.GetPos();
	m_nVolumeValue = m_sliderVolume.GetPos();
	
	CDialog::OnOK();
}

void CDlgAdjustVolAndVideo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		m_nContrastValue = m_sliderContrast.GetPos();
		m_nLightValue = m_sliderLight.GetPos();
		m_nVolumeValue = m_sliderVolume.GetPos();
		CString strVolume;
		strVolume.Format("%d\%%", m_nVolumeValue - 100);
		m_volume.SetWindowTextA(strVolume);
		CString strLight;
		strLight.Format("%d", m_nLightValue - 100);
		m_light.SetWindowTextA(strLight);
		CString strContrast;
		strContrast.Format("%d", m_nContrastValue - 100);
		m_contrast.SetWindowTextA(strContrast);
	}

	CDialog::OnTimer(nIDEvent);
}
