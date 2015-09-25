#ifndef _LOCKSERVER_H_
#define _LOCKSERVER_H_

interface IEasyCapture;
class CLockServer {
private:
	IEasyCapture *m_pEasyCapture;
public:
	BOOL m_fSuccess;
public:
	CLockServer() : m_fSuccess(FALSE) {
		HRESULT hval = CoInitialize(NULL);		
		if(SUCCEEDED(hval)) {
			hval = CoCreateInstance(CLSID_EasyCapture, NULL, CLSCTX_INPROC_SERVER, IID_IEasyCapture, (void**)&m_pEasyCapture);
			if(SUCCEEDED(hval))	{
				hval = m_pEasyCapture->Open(CAPTUREKERNALMODE_MIX);
				if(FAILED(hval)) {
					m_pEasyCapture->Release();
					m_pEasyCapture = NULL;
				}
			}
		}
		m_fSuccess = SUCCEEDED(hval);
	};
	~CLockServer() {
		if(m_pEasyCapture!=NULL) {
			m_pEasyCapture->Close();
			m_pEasyCapture->Release();
			m_pEasyCapture = NULL;
		}
		CoUninitialize();
	};
};
#endif