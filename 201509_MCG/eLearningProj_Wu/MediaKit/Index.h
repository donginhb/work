#ifndef _INDEX_H_
#define _INDEX_H_

class CIndex : public IWMStatusCallback {
public:
	HANDLE		m_hEventAsync ;
	HRESULT		*m_phvalAsync ;

	BOOL		m_fEOF;
	DWORD		m_dwProgress;

private:
	HWND	m_hWndProgress;
	long	m_lMsgProgress;
public:
	CIndex();
	~CIndex();
public:
	BOOL SetupProgressHandler(HWND hwnd, long lMsg) { 
		m_hWndProgress = hwnd; 
		m_lMsgProgress  = lMsg; 
		return TRUE; 
	};
	BOOL MakeIndex(WCHAR *outfile);
	BOOL IndexExisted(BSTR bstrFile);
public: // Implement Interface Methods
	virtual HRESULT STDMETHODCALLTYPE OnStatus( 
            /* [in] */ WMT_STATUS Status,
            /* [in] */ HRESULT hr,
            /* [in] */ WMT_ATTR_DATATYPE dwType,
            /* [in] */ BYTE __RPC_FAR *pValue,
            /* [in] */ void __RPC_FAR *pvContext);
	
	ULONG STDMETHODCALLTYPE AddRef( void ) 
	{     
		return 1;
    }
    
    ULONG STDMETHODCALLTYPE Release( void )
    {
        return 1;
    }

	HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
        {
            if ( riid == IID_IWMStatusCallback )
            {
                *ppvObject = ( IWMStatusCallback * )this;
            }
            else
            {
                return E_NOINTERFACE;
            }
            return S_OK;
        }
};

#endif
