// DlgRegister.cpp : implementation file
//

#include "stdafx.h"
#include "elearning.h"
#include "DlgRegister.h"
#include "WebGrab.h"
#include "cmkapi.h"
#include "macro.h"
#include "mainfrm.h"
#include "registry.h"
#include "atlbase.h"

//leu, add begin, for get MAC address
#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Nb30.h>
//leu, add end, for get MAC address

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//leu, add
typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;

ASTAT Adapter;

CString GetMACAddressString();
VOID Convert10To16(unsigned int x, unsigned char *out);
//leu, add

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister dialog


CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRegister)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRegister)
	DDX_Control(pDX, IDC_EDIT_SERIAL1, m_edSerial1);
	DDX_Control(pDX, IDC_EDIT_SERIAL2, m_edSerial2);
	DDX_Control(pDX, IDC_EDIT_SERIAL3, m_edSerial3);
	DDX_Control(pDX, IDC_EDIT_SERIAL4, m_edSerial4);
	DDX_Control(pDX, IDC_EDIT_SERIAL5, m_edSerial5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialog)
	//{{AFX_MSG_MAP(CDlgRegister)
	ON_BN_CLICKED(ID_FAIL_TO_REGISTER, OnFailToRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister message handlers

BOOL CDlgRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strMsg;
	strMsg.LoadString(IDS_READY);
	SetDlgItemText(IDC_STATIC_STATUS, strMsg);
	
	m_edSerial1.SetTextLimit(5);
	m_edSerial2.SetTextLimit(5);
	m_edSerial3.SetTextLimit(5);
	m_edSerial4.SetTextLimit(5);
	m_edSerial5.SetTextLimit(5);
	m_edSerial1.SetLimitReached(0);
	m_edSerial2.SetLimitReached(0);
	m_edSerial3.SetLimitReached(0);
	m_edSerial4.SetLimitReached(0);
	m_edSerial5.SetLimitReached(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRegister::OnOK() 
{
	CString strSerial1, strSerial2, strSerial3, strSerial4, strSerial5;
	CString strLastName, strFirstName, strEmail, strCompany;
    CString MACstr, Verstr; //leu

    //leu, test
    //MACstr = GetMACAddressString();
    //((CELearningApp*)AfxGetApp())->EncodeID(MACstr);
    //Verstr = "mcg50";
    //Verstr = GetEncodedString(Verstr);
    /*
    COleDateTime cur;
    int          ey,eday,em;
    char         c1[3], c2[3];
    CString      CdStr, str1;
    cur = COleDateTime::GetCurrentTime();
    ey = cur.GetYear();
    em = cur.GetMonth();
    eday = cur.GetDay();
    memset(c1, '\0', 3);
    memset(c2, '\0', 3);
    _itoa(eday, c1, 10);
    if (eday < 10)
    {
        c2[0] = '0';
        c2[1] = c1[0];
    }
    else
    {
        c2[0] = c1[0];
        c2[1] = c1[1];
    }
    CdStr = c2;
    str1 = CdStr + "005C" + CdStr;
    ((CELearningApp*)AfxGetApp())->EncodeID(str1);

    CString sBuf;
    TCHAR   tmpc[1024];
    int i1, k1 = 0;
    sBuf="123456789";
    memset(tmpc, '\0', 1024);
    for (i1=2; i1 < sBuf.GetLength() - 2; i1++)
    {
        tmpc[k1] = sBuf.GetAt(i1);
        k1++;
    }
    sBuf.Empty();
    sBuf = tmpc;
    */
    //leu, test

	GetDlgItem(IDC_EDIT_SERIAL1)->GetWindowText(strSerial1);
	if (strSerial1 == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_SERIAL1)->SetFocus();
		return;
	}

	//if (strSerial1 != "MCGP" && strSerial1 != "MCGS" && strSerial1 != "MCGB")
	//{
	//	W_SHOW_ERROR(IDS_INVALID_SERIAL, this);
	//	return;
	//}

	CString strStart = strSerial1.Left(1);
	if ((strStart != "P" && strStart != "N" && PRO_VERSION) ||
		(strStart != "B" && BUNDLE_VERSION))
	{
		W_SHOW_ERROR(IDS_INVALID_SERIAL, this);
		return;
	}

	GetDlgItem(IDC_EDIT_SERIAL2)->GetWindowText(strSerial2);
	if (strSerial2 == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_SERIAL2)->SetFocus();
		return;
	}

	GetDlgItem(IDC_EDIT_SERIAL3)->GetWindowText(strSerial3);
	if (strSerial3 == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_SERIAL3)->SetFocus();
		return;
	}

	GetDlgItem(IDC_EDIT_SERIAL4)->GetWindowText(strSerial4);
	if (strSerial4 == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_SERIAL4)->SetFocus();
		return;
	}

	GetDlgItem(IDC_EDIT_SERIAL5)->GetWindowText(strSerial5);
	if (strSerial5 == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_SERIAL5)->SetFocus();
		return;
	}
	CString strSerial = strSerial1 + strSerial2 + strSerial3 + strSerial4 + strSerial5;	
	if (!ValidSerial(strSerial))
	{
		W_SHOW_WARNING(IDS_INVALID_SERIAL_NO, this);
		return;
	}

	GetDlgItem(IDC_EDIT_LAST_NAME)->GetWindowText(strLastName);
	strLastName.TrimLeft();
	strLastName.TrimRight();
	if (strLastName == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_LAST_NAME)->SetFocus();
		return;
	}
	
	strLastName = GetEncodedString(strLastName);

	GetDlgItem(IDC_EDIT_FIRST_NAME)->GetWindowText(strFirstName);
	strFirstName.TrimLeft();
	strFirstName.TrimRight();
	if (strFirstName == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_FIRST_NAME)->SetFocus();
		return;
	}
	
	strFirstName = GetEncodedString(strFirstName);

	GetDlgItem(IDC_EDIT_EMAIL)->GetWindowText(strEmail);
	strEmail.TrimLeft();
	strEmail.TrimRight();
	if (strEmail == "")
	{
		W_SHOW_WARNING(IDS_NOT_ALL_FILLED, this);
		GetDlgItem(IDC_EDIT_EMAIL)->SetFocus();
		return;
	}
	
	if (!::IsValidEmailAddr(strEmail))
	{
		W_SHOW_WARNING(IDS_INVALID_EMAIL, this);
		((CEdit*)GetDlgItem(IDC_EDIT_EMAIL))->SetSel(0, -1);
		GetDlgItem(IDC_EDIT_EMAIL)->SetFocus();
		return;
	}

    strEmail = GetEncodedString(strEmail); //leu

	GetDlgItem(IDC_EDIT_COMPANY)->GetWindowText(strCompany);
	strCompany.TrimLeft();
	strCompany.TrimRight();

	strCompany = GetEncodedString(strCompany);

	CString strMsg;
	strMsg.LoadString(IDS_CONNECTING_SITE);
	SetDlgItemText(IDC_STATIC_STATUS, strMsg);

	CString strURL;
	strURL.LoadString(IDS_URL);
	
	CString strID;
	((CELearningApp*)AfxGetApp())->GetMachineID(strID);
	((CELearningApp*)AfxGetApp())->EncodeID(strID);

    /********************* leu begin **************************/
    /*
     * register URL parameters
     * 1.Partition ID, from GetMachineID
     * 2.MAC address, new for MCG 5.0,
     * 3.Version, new for MCG 5.0
     * 4.Serial number
     * 5.First name
     * 6.Last name
     * 7.email
     * 8.company
     * strURL = "xxxxxx&macaddr=mmmmmmmm&version=mcg50&serial=ssssss&firstname=ffff&lastname=llll&email=eeee&company=ccccc"
    */
    /*
     * get MAC address
     * 1.need include Netapi32.lib
     * 2.get MAc address, xx(1)-xx(2)-xx(3)-xx(4)-xx(5)-xx(6), convert into string"xx(1)xx(2)xx(3)xx(4)xx(5)xx(6)"
     * 3.encode MAC address
    */
    MACstr = GetMACAddressString();
    ((CELearningApp*)AfxGetApp())->EncodeID(MACstr);

    /*
     * parameter version = mcg50, for security reason the "mcg50" will 
     * rearrange to "005C", inverse direction and remove 'm' & 'g' character
     * 1.the format is <day><005C><day>, so need to get date
     * 2.encode version string by using EncodeID()
    */
    COleDateTime CurrentTime;
    int          Currentdate;
    char         cCurdate1[3],cCurdate2[3];
    CString      CurdateStr;

    memset(cCurdate1, '\0', 3);
    memset(cCurdate2, '\0', 3);
    CurrentTime = COleDateTime::GetCurrentTime();
    Currentdate = CurrentTime.GetDay();
    _itoa(Currentdate, cCurdate1, 10);
    if (Currentdate < 10)
    {
        cCurdate2[0] = '0';
        cCurdate2[1] = cCurdate1[0];
    }
    else
    {
        cCurdate2[0] = cCurdate1[0];
        cCurdate2[1] = cCurdate1[1];
    }

    Verstr = CurdateStr + "005C" + CurdateStr;
    ((CELearningApp*)AfxGetApp())->EncodeID(Verstr);
    /********************* leu end ****************************/

	strURL += strID;

    strURL += "&sANo=" + MACstr;  //leu
    strURL += "&sVNo=" + Verstr;  //leu
	//strURL += "&serial=" + strSerial + "&firstname=" + strFirstName + "&lastname=" + strLastName;  //leu
	//strURL += "&email=" + strEmail + "&company=" + strCompany;  //leu
	strURL += "&sSNo=" + strSerial + "&sFNa=" + strFirstName + "&sLNa=" + strLastName;  //leu
	strURL += "&sEMl=" + strEmail + "&sCOm=" + strCompany;  //leu

	BeginWaitCursor();
	
	CString  strBuffer;
    CWebGrab grab;
    //TCHAR    tmpCarray[1024];  //leu
    //int      i, k;  //leu

    if (grab.GetFile(strURL, strBuffer, _T("WebGrab demo"), GetDlgItem(IDC_STATIC_STATUS)))
	{
        /********************* leu begin **************************/
        /*
         * when get strBuffer, remove first 2 characters and last 2 characters
        */

        /********************* leu end ****************************/

		if (strBuffer.Find("mcg_register_ok:") >= 0)
		{
//			if (!BUNDLE_VERSION)
//			{
				int nIndex = strBuffer.Find("mcg_register_ok:");
				int nEnd;
				for (int i = nIndex + 16; i < strBuffer.GetLength(); i++)
				{
					char ch = strBuffer[i];
					if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z')))
					{
						nEnd = i;
						break;
					}
				}

				CString strID = strBuffer.Mid(nIndex + 16, nEnd - nIndex - 15);

				if (((CELearningApp*)AfxGetApp())->WriteKey(strID))
				{
					strMsg.LoadString(IDS_SUCCESS_REGISTER);
					SetDlgItemText(IDC_STATIC_STATUS, strMsg);

					W_SHOW_MESSAGE(IDS_REG_SUCCESS, this);

					::RegSetKey(MACRO_REGISTRY + CString("\\Serial"), strSerial);

					CDialog::OnOK();
				}
				else
					W_SHOW_ERROR(IDS_REG_FAIL, this);
//			}
//			else
//			{
//				if (((CELearningApp*)AfxGetApp())->AddHour(80))
//				{
//					strMsg.LoadString(IDS_SUCCESS_REGISTER);
//					SetDlgItemText(IDC_STATIC_STATUS, strMsg);
//
//					W_SHOW_MESSAGE(IDS_REG_SUCCESS, this);
//
//					::RegSetKey(MACRO_REGISTRY + CString("\\Serial"), strSerial);
//
//					CDialog::OnOK();
//				}
//				else
//					W_SHOW_ERROR(IDS_REG_FAIL, this);
//			}
		}
		else if (strBuffer.Find("mcg_register_exist") >= 0)
		{
			W_SHOW_ERROR(IDS_REG_EXISTS, this);
		}
		else if (strBuffer.Find("mcg_register_invalid") >= 0)
		{
			W_SHOW_ERROR(IDS_INVALID_SERIAL, this);
		}
		else
			W_SHOW_ERROR(IDS_FAIL_TO_REGISTER, this);
	}
	else
	{
		W_SHOW_ERROR(IDS_FAIL_TO_REGISTER, this);
	}

	EndWaitCursor();

	//CDialog::OnOK();
}

void CDlgRegister::OnFailToRegister() 
{
	CString strURL;
	strURL.LoadString(IDS_VERIFY_URL);
	::ShellExecute(this->m_hWnd, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}

CString CDlgRegister::GetEncodedString(CString strInput)
{	
	CString strTmp, strRet = "";
	for (int i = 0; i < strInput.GetLength(); i++)
	{
		CString str;
		if (::IsDBCSLeadByte(strInput[i]))
		{
			str = strInput.Mid(i, 2);
			i++;
		}
		else
			str = strInput.Mid(i, 1);
			
		CComBSTR bstr = str;
		char szBuf[256];
		int n = WideCharToMultiByte(CP_UTF8, 0, bstr, bstr.Length(), szBuf, 256, NULL, NULL);

		if (n > 1)
		{
			for (int i = 0; i < n; i++)
			{
				strTmp.Format("%02X", (unsigned char) szBuf[i]);
				strRet += "%" + strTmp;
			}
		}
		else
		{
			char ch = szBuf[0];
			strRet += ch;
		}
	}

	return strRet;
}

BOOL CDlgRegister::ValidSerial(CString strSerial)
{
	for (int i = 0; i <= 20; i += 5)
	{
		if (!ValidSerialPart(strSerial, i))
			return FALSE;
	}

	return TRUE;
}

BOOL CDlgRegister::ValidSerialPart(CString strSerial, int nStart)
{
	int nValue = (GetValue(strSerial[nStart]) + GetValue(strSerial[nStart + 1]) * 2 + 
				  GetValue(strSerial[nStart + 2]) * 3 + GetValue(strSerial[nStart + 3]) * 4) % 32;
	int nCheck = GetValue(strSerial[nStart + 4]);
	if (nCheck != nValue)
		return FALSE;

	return TRUE;
}

int CDlgRegister::GetValue(char ch)
{
	if (ch >= 'A' && ch <='H')
		return ch - 'A';
	else if (ch >= 'J' && ch <= 'N')
		return ch - 'J' + 8;
	else if (ch >= 'P' && ch <= 'Z')
		return ch - 'P' + 13;
	else if (ch >= '2' && ch <= '9')
		return ch - '2' + 24;

	return 0;
}

char CDlgRegister::GetDigit(int n)
{
	if (n >= 0 && n <= 7)
		return 'A' + n;
	else if (n >= 8 && n <= 12)
		return 'J' + n - 8;
	else if (n >= 13 && n <= 23)
		return 'P' + n - 13;
	else if (n >= 24 && n <= 31)
		return '2' + n - 24;

	return 'e';
}

/********************* leu begin **************************/
CString GetMACAddressString()
{
    NCB Ncb;
    UCHAR uRetCode;
    CString MACstr;
    LANA_ENUM   lenum;
    int      i;
    unsigned char out_16[3];

    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );
    //printf( "The NCBENUM return code is: 0x%x \n", uRetCode );

    for(i=0; i < lenum.length ;i++)
    {
        memset( &Ncb, 0, sizeof(Ncb) );
        Ncb.ncb_command = NCBRESET;
        Ncb.ncb_lana_num = lenum.lana[i];

        uRetCode = Netbios( &Ncb );
        //printf( "The NCBRESET on LANA %d return code is: 0x%x \n",
        //        lenum.lana[i], uRetCode );

        memset( &Ncb, 0, sizeof (Ncb) );
        Ncb.ncb_command = NCBASTAT;
        Ncb.ncb_lana_num = lenum.lana[i];

        strcpy((char *)Ncb.ncb_callname,  "*               ");
        Ncb.ncb_buffer = (unsigned char *) &Adapter;
        Ncb.ncb_length = sizeof(Adapter);

        uRetCode = Netbios( &Ncb );
        //printf( "The NCBASTAT on LANA %d return code is: 0x%x \n",
        //        lenum.lana[i], uRetCode );
        if ( uRetCode == 0 )
        {
            //printf( "The Ethernet Number on LANA %d is:%02x%02x%02x%02x%02x%02x\n",
            //         lenum.lana[i],
            //         Adapter.adapt.adapter_address[0],
            //         Adapter.adapt.adapter_address[1],
            //         Adapter.adapt.adapter_address[2],
            //         Adapter.adapt.adapter_address[3],
            //         Adapter.adapt.adapter_address[4],
            //         Adapter.adapt.adapter_address[5]);
            MACstr.Empty();
            for (i=0; i < 6; i++)
            {
                memset(out_16, '\0', 3);
                Convert10To16((unsigned int)Adapter.adapt.adapter_address[i], out_16);
                MACstr += out_16[0];
				MACstr += out_16[1];
				MACstr += out_16[2];
            }
        }
    }
    return MACstr;
}

VOID Convert10To16(unsigned int x, unsigned char *out)
{
    char hexchar[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char hex[2]={'0','0'};
    int a, k;

    for (a=0;x>0;a++)
    {
       hex[a]=hexchar[x % 16];
       x=x/16;
    }

    k = 0;
    for (a=1;a>=0;a--)
    {
        out[k] = hex[a];
        k++;
    }
}
/********************* leu end ****************************/
