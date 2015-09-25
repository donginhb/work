// dllmain.h : モジュール クラスの宣言

class CComCaptureModule : public ATL::CAtlDllModuleT< CComCaptureModule >
{
public :
	DECLARE_LIBID(LIBID_ComCaptureLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMCAPTURE, "{3DE4D987-C4E5-4E17-B4AC-CFC8050C1305}")
};

extern class CComCaptureModule _AtlModule;
