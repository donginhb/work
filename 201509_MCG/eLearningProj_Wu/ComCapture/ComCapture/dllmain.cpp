// dllmain.cpp : DllMain の実装

#include "stdafx.h"
#include "resource.h"
#include "ComCapture_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CComCaptureModule _AtlModule;

class CComCaptureApp : public CWinApp
{
public:

// オーバーライド
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CComCaptureApp, CWinApp)
END_MESSAGE_MAP()

CComCaptureApp theApp;

BOOL CComCaptureApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CComCaptureApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
