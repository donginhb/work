#include <afxwin.h>
#include <wmsdk.h>

#include "Index.h"

#define TIMEUNIT	(LONGLONG)10000000

#define WM_INDEX_ASYNC_EVENT _T( "C6729684-EE62-44e0-858A-4176EB11087D" )

#undef RELEASE
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}

CIndex::CIndex()
{
	m_phvalAsync = NULL ;
	m_hEventAsync = NULL;

	m_fEOF = FALSE;
	m_dwProgress = 0;

	m_hWndProgress = NULL;
}

CIndex::~CIndex()
{
}

BOOL CIndex::MakeIndex(WCHAR *outfile)
{
	if ( NULL == outfile ) return FALSE;

	HRESULT		hr=0, hrIndexAsync=0;
	BOOL		fOK=FALSE;
	IWMIndexer	*pIndexer=NULL ;	

// Event for the asynchronous calls
	HANDLE hIndexEventAsync = CreateEvent( NULL, FALSE, FALSE, WM_INDEX_ASYNC_EVENT );
	if ( NULL == hIndexEventAsync ) return FALSE;

	m_hEventAsync = hIndexEventAsync;
	m_phvalAsync = &hrIndexAsync;
	
	hr = WMCreateIndexer(&pIndexer) ;
	if( FAILED( hr ) ) goto ERROR_MAKEINDEX;

	hr = pIndexer->StartIndexing(outfile , this, NULL) ;
	if( FAILED( hr ) ) goto ERROR_MAKEINDEX;

	if(IsWindow(m_hWndProgress)) {
		do {
			if(m_fEOF) break;
			if(IsWindow(m_hWndProgress))
				SendMessage(m_hWndProgress, m_lMsgProgress, 0, MAKELONG(m_dwProgress, 100));
			WaitForSingleObject( hIndexEventAsync, 250 );
		} while(TRUE);
	}
	else WaitForSingleObject( hIndexEventAsync, INFINITE );		
	ASSERT(m_fEOF);

	if ( FAILED( hrIndexAsync ) ) goto ERROR_MAKEINDEX;

	fOK = TRUE;
ERROR_MAKEINDEX:
	RELEASE( pIndexer) ;
	if(hIndexEventAsync!=NULL) CloseHandle( hIndexEventAsync );

	return fOK;
}

#include "InfoReader.h"
BOOL CIndex::IndexExisted(BSTR bstrFile)
{
	CInfoReader inforeader;
	if(!inforeader.Open(bstrFile))
		return FALSE;

	IWMHeaderInfo		*pHeaderInfo=NULL;

	BOOL fOK=FALSE;
	HRESULT	hr = inforeader.m_pReader->QueryInterface( IID_IWMHeaderInfo, ( VOID ** )&pHeaderInfo );
	if(SUCCEEDED(hr)) {
		WMT_ATTR_DATATYPE	type;
		BYTE				value[4];
		WORD				cbLength=4;
		WORD				wStreamNum=0;
		hr = pHeaderInfo->GetAttributeByName( &wStreamNum, g_wszWMSeekable, &type, value, &cbLength ) ;
		if(SUCCEEDED(hr)) {
			fOK = (value[0] != 0);
		}
	}
	RELEASE(pHeaderInfo);
	
	inforeader.Close();

	return fOK;
}

//////////////////////////////////////////////////////////////////////////
// Implement Interface

HRESULT CIndex::OnStatus( 
							/* [in] */ WMT_STATUS Status,
							/* [in] */ HRESULT hr,
							/* [in] */ WMT_ATTR_DATATYPE dwType,
							/* [in] */ BYTE __RPC_FAR *pValue,
							/* [in] */ void __RPC_FAR *pvContext)
{
	switch ( Status ) {
	case WMT_STARTED:
		m_dwProgress = 0;
		m_fEOF = FALSE;
		break;

	case WMT_CLOSED:
		m_fEOF = TRUE;
		*m_phvalAsync = hr;
		SetEvent(m_hEventAsync) ;
		break ;

	case WMT_INDEX_PROGRESS:
		if(dwType==WMT_TYPE_DWORD)
			m_dwProgress = *(DWORD*)pValue;
		break ;
	}
	return S_OK;
}
