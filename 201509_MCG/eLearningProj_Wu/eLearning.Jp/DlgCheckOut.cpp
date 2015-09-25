// DlgCheckOut.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgCheckOut.h"
#include "WebGrab.h"
#include "macro.h"
#include "mainfrm.h"
#include "registry.h"  //leu

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//leu, add
extern CString GetMACAddressString();
//leu, add


/////////////////////////////////////////////////////////////////////////////
// CDlgCheckOut dialog


CDlgCheckOut::CDlgCheckOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCheckOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCheckOut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCheckOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCheckOut)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCheckOut, CDialog)
	//{{AFX_MSG_MAP(CDlgCheckOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckOut message handlers

void CDlgCheckOut::OnOK() 
{
    CString MACstr, strSerial;  //leu
	CString strURL;
	strURL.LoadString(IDS_CHECKOUT_URL);

	CString strID;
	((CELearningApp*)AfxGetApp())->GetMachineID(strID);
	((CELearningApp*)AfxGetApp())->EncodeID(strID);
	
	strURL += strID;

    /********************* leu begin **************************/
    /*
     * checkout URL parameters
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

    // get serial number
    if (!::RegGetKey(MACRO_REGISTRY + CString("\\Serial"), strSerial))
	{
		W_SHOW_MESSAGE(IDS_NOT_REGISTERED, this);
		return;
	}

    strURL += "&sANo=" + MACstr;
    strURL += "&sSNo=" + strSerial;
    /********************* leu end ****************************/
	
	CString strMsg;
	strMsg.LoadString(IDS_CONNECTING_SITE);
	SetDlgItemText(IDC_STATIC_STATUS, strMsg);

	BeginWaitCursor();
	
	CString strBuffer;
    CWebGrab grab;
    if (grab.GetFile(strURL, strBuffer, _T("WebGrab demo"), GetDlgItem(IDC_STATIC_STATUS)))
	{
		if (strBuffer.Find("mcg_joseph_ok") >= 0)
		{
			((CELearningApp*)AfxGetApp())->RemoveKey();
			W_SHOW_MESSAGE(IDS_CHECK_OUT_OK, this);
			CDialog::OnOK();

			MACRO_MAINFRM->PostMessage(WM_CLOSE);
		}
		else if (strBuffer.Find("mcg_joseph_not_found") >= 0)
		{

			W_SHOW_ERROR(IDS_NO_FIND_ON_SERVER_CHO, this);

		}
		else
		{
			W_SHOW_ERROR(IDS_FAIL_TO_CHECK_OUT, this);
		}
	}
	else
	{
		W_SHOW_ERROR(IDS_FAIL_TO_CHECK_OUT, this);
	}

	EndWaitCursor();
}
