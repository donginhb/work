// DlgUseQuota.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgUseQuota.h"
#include "WebGrab.h"
#include "macro.h"
#include "mainfrm.h"
#include "registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//leu, add
extern CString GetMACAddressString();
//leu, add

/////////////////////////////////////////////////////////////////////////////
// CDlgUseQuota dialog


CDlgUseQuota::CDlgUseQuota(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUseQuota::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUseQuota)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgUseQuota::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUseQuota)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUseQuota, CDialog)
	//{{AFX_MSG_MAP(CDlgUseQuota)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUseQuota message handlers

void CDlgUseQuota::OnOK() 
{
    CString MACstr;  //leu
	CString strURL;
	strURL.LoadString(IDS_USE_QUOTA_URL);

	CString strID;
	((CELearningApp*)AfxGetApp())->GetMachineID(strID);
	((CELearningApp*)AfxGetApp())->EncodeID(strID);
	
	strURL += strID;

    /********************* leu begin **************************/
    /*
     * UseQuota URL parameters
     * 1.Partition ID, from GetMachineID
     * 2.MAC address, new for MCG 5.0,
     * 3.Serial number, new for MCG 5.0
    */
    /*
     * get MAC address
     * 1.need include Netapi32.lib
     * 2.get MAc address, xx(1)-xx(2)-xx(3)-xx(4)-xx(5)-xx(6), convert into string"xx(1)xx(2)xx(3)xx(4)xx(5)xx(6)"
     * 3.encode MAC address
    */
    MACstr = GetMACAddressString();
    ((CELearningApp*)AfxGetApp())->EncodeID(MACstr);

    strURL += "&sANo=" + MACstr;
    /********************* leu end ****************************/

	CString strSerial;
	if (!::RegGetKey(MACRO_REGISTRY + CString("\\Serial"), strSerial))
	{
		W_SHOW_MESSAGE(IDS_NOT_REGISTERED, this);
		return;
	}

	//strURL += "&serial=" + strSerial;  //leu
    strURL += "&sSNo=" + strSerial;  //leu

	CString strMsg;
	strMsg.LoadString(IDS_CONNECTING_SITE);
	SetDlgItemText(IDC_STATIC_STATUS, strMsg);

	BeginWaitCursor();
	
	CString strBuffer;
    CWebGrab grab;
    if (grab.GetFile(strURL, strBuffer, _T("WebGrab demo"), GetDlgItem(IDC_STATIC_STATUS)))
	{
		if (strBuffer.Find("mcg_quota_ok") >= 0)
		{
			int nIndex = strBuffer.Find("mcg_quota_ok:");
			int nEnd;
			for (int i = nIndex + 13; i < strBuffer.GetLength(); i++)
			{
				char ch = strBuffer[i];
				if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z')))
				{
					nEnd = i;
					break;
				}
			}

			CString strHour = strBuffer.Mid(nIndex + 13, nEnd - nIndex - 12);
			int nHour = atoi(strHour);
			((CELearningApp*)AfxGetApp())->AddHour(nHour);

			W_SHOW_MESSAGE(IDS_USE_QUOTA_OK, this);
			CDialog::OnOK();
		}
		else if (strBuffer.Find("mcg_quota_not_found") >= 0)
		{
			W_SHOW_ERROR(IDS_FAIL_TO_FIND_QUOTA, this);
		}
		else if (strBuffer.Find("mcg_invalid_serial") >= 0)
		{
			W_SHOW_ERROR(IDS_INVALID_SERIAL_NO, this);
		}
		else
		{
			W_SHOW_ERROR(IDS_FAIL_TO_USE_QUOTA, this);
		}
	}
	else
	{
		W_SHOW_ERROR(IDS_FAIL_TO_USE_QUOTA, this);
	}
	
	EndWaitCursor();
	
	CDialog::OnOK();
}
