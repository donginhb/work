// HtmlWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "HtmlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd

typedef void (FAR PASCAL* SETSTART)(
	int n
);

HINSTANCE CHtmlWnd::m_hmodHook = NULL;
HHOOK CHtmlWnd::m_hHook = NULL;
CHtmlWnd::CHtmlWnd()
{
	m_pBrowserApp = NULL;
	m_pwndBrowser = NULL;
	m_hWndHTML = NULL;
}

CHtmlWnd::~CHtmlWnd()
{
	if (m_pBrowserApp != NULL)
	{
		m_pBrowserApp->Stop();
		m_pBrowserApp->Release();
		m_pBrowserApp = NULL;
	}

	delete m_pwndBrowser;

	if (m_hmodHook)
	{
		FreeLibrary(m_hmodHook);
		m_hmodHook = NULL;
	}
}


BEGIN_MESSAGE_MAP(CHtmlWnd, CWnd)
	//{{AFX_MSG_MAP(CHtmlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd message handlers

void CHtmlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pBrowserApp != NULL)
	{
		m_pBrowserApp->Stop();
		m_pBrowserApp->Release();
		m_pBrowserApp = NULL;
	}

	m_pwndBrowser->DestroyWindow();
}

void CHtmlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!m_pwndBrowser)
		return;

	if (::IsWindow(m_pwndBrowser->m_hWnd))
	{
		// need to push non-client borders out of the client area
		CRect rect;
		GetClientRect(rect);
		//::AdjustWindowRectEx(rect,
		//	m_pwndBrowser->GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_pwndBrowser->SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

BOOL CHtmlWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
					  DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd,
					  UINT nID, CCreateContext* pContext)
{
	// create the view window itself
	//m_pCreateContext = pContext;
	//if (!CWnd::Create(lpszClassName, lpszWindowName,
	//			dwStyle, rect, pParentWnd,  nID, pContext))
	//{
	//	return FALSE;
	//}
	CRect rc = rect;
	//m_strWindowName = lpszWindowName;
	if (!CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, 
		rect.left, rect.top, rc.Width(), rc.Height(), 
		(pParentWnd) ? pParentWnd->m_hWnd : NULL, (HMENU)nID))
	{
		return FALSE;
	}

	AfxEnableControlContainer();
	
	if (!CreateHTMLWnd())
	{
		DestroyWindow();
		return FALSE;
	}

    if (!m_hmodHook)
    {
		if (!(m_hmodHook = LoadLibrary("HTMLHookProc.dll")))
	    {
            //W_SHOW_ERROR(IDS_CANNOT_LOAD_DLL, this);
			MessageBox("DLL <HTMLHookProc.dll> Not found!");
			return FALSE;
        }
		SetHook();
    }

	return TRUE;
}
	
void CHtmlWnd::StartHook(int nHook)
{
	if (m_hmodHook)
	{
		SETSTART proc = (SETSTART)GetProcAddress(m_hmodHook, "StartHook");
		if (proc != NULL)
			(*proc)(nHook);
	}
}

/*
BOOL CALLBACK CHtmlWnd::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClassName[256];
	GetClassName(hWnd, szClassName, 256);
	if (lstrcmp(szClassName, "Internet Explorer_Server") != 0)
		return TRUE;

	HWND* phWndArray = (HWND*)lParam;
	for (int i = 0; i <30; i++)
	{
		if (phWndArray[i] == NULL)
		{
			phWndArray[i] = hWnd;
			break;
		}
	}

	return TRUE;
}
*/

BOOL CHtmlWnd::CreateHTMLWnd()
{
	if (m_pBrowserApp)
	{
		m_pBrowserApp->Stop();
		m_pBrowserApp->Release();
		m_pBrowserApp = NULL;
	}

	if (m_pwndBrowser)
	{
		m_pwndBrowser->DestroyWindow();
		delete m_pwndBrowser;
	}

	m_pwndBrowser = new CWnd;

	RECT rectClient;
	GetClientRect(&rectClient);
/*
	HWND hWndArray[30];
	for (int i = 0; i < 30; i++)
		hWndArray[i] = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)hWndArray);
*/

	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_pwndBrowser->CreateControl(CLSID_WebBrowser, "",
				WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		return FALSE;
	}

	LPUNKNOWN lpUnk = m_pwndBrowser->GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_pwndBrowser->DestroyWindow();
		return FALSE;
	}


	/*
	HWND hNewWndArray[30];
	for (i = 0; i < 30; i++)
		hNewWndArray[i] = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)hNewWndArray);
	
	HWND hWndHTML = NULL;
	for (i = 0; i < 30; i++)
	{
		HWND hNewWnd = hNewWndArray[i];
		if (hNewWnd == NULL)
			continue;
		BOOL bFound = FALSE;
		HWND hWnd;
		for (int j = 0; j < 30; j++)
		{
			hWnd = hWndArray[j];
			if (hWnd == NULL)
				continue;
			if (hWnd == hNewWnd)
			{
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
		{
			hWndHTML = hNewWnd;
			break;	
		}
	}

	m_hWndHTML = hWndHTML;
*/
	return TRUE;
}

HWND CHtmlWnd::GetIEWnd()
{
	HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);
	while (hWnd)
	{
		char szBuffer[128];
		::GetClassName(hWnd, szBuffer, 128);
		if (strcmp("Internet Explorer_Server", szBuffer) == 0)
			return hWnd;
		hWnd = ::GetWindow(hWnd, GW_CHILD);
	}
	return NULL;
}

CString CHtmlWnd::GetLocationURL() const
{
	ASSERT(m_pBrowserApp != NULL);

	BSTR bstr;
	m_pBrowserApp->get_LocationURL(&bstr);
	CString retVal(bstr);
	return retVal;
}

void CHtmlWnd::Navigate2(LPCTSTR lpszURL, BOOL bReCreate, 
						 DWORD dwFlags /* = 0 */,
						 LPCTSTR lpszTargetFrameName /* = NULL */,
						 LPCTSTR lpszHeaders /* = NULL */,
						 LPVOID lpvPostData /* = NULL */, 
						 DWORD dwPostDataLen /* = 0 */)
{
	if (bReCreate)
		CreateHTMLWnd();
	
	COleSafeArray vPostData;
	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0)
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}

	COleVariant vURL(lpszURL, VT_BSTR);
	COleVariant vHeaders(lpszHeaders, VT_BSTR);
	COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
	COleVariant vFlags((long) dwFlags, VT_I4);

	m_pBrowserApp->Navigate2(vURL,
		vFlags, vTargetFrameName, vPostData, vHeaders);
}

BOOL CHtmlWnd::Create(CWnd* pParentWnd) 
{
	CRect rect; 
	pParentWnd->GetClientRect(rect);
	
	return Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
				  NULL, rect, pParentWnd, 0, NULL);
}

BOOL CHtmlWnd::SetHook()
{   
	if (!m_hmodHook)
		return FALSE;

	if (m_hHook)
		UnSetHook();
 
    if (!m_hHook)
    {
		//if (!(m_hHook = SetWindowsHookEx(WH_GETMESSAGE,
	    //    (HOOKPROC)GetProcAddress(m_hmodHook, "HookProcedure"), m_hmodHook, 0)))  //leu;20060609
        if (!(m_hHook = SetWindowsHookEx(WH_KEYBOARD,
	        (HOOKPROC)GetProcAddress(m_hmodHook, "HookProcedure"), m_hmodHook, 0)))  //leu;20060609
	    {
            return FALSE;
        }
    }

	return TRUE;
};

void CHtmlWnd::UnSetHook()
{
	if (m_hHook)
	{
		::UnhookWindowsHookEx(m_hHook);
		m_hHook = NULL;
	}
};
