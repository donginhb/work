#ifndef _MEDIA2WMX_H_
#define _MEDIA2WMX_H_

typedef struct {
// Filter Graph Builder
	IGraphBuilder			*pFG;

// Writer Part
	IBaseFilter				*pAsfMux;
	IBaseFilter				*pWriter;
	IFileSinkFilter			*pFileSink;
	IConfigInterleaving		*pConfigInterleaving;
	IConfigAsfWriter		*pConfigAsfWriter;
	IBaseFilter				*pAsfReader;

// Infomation
	IMediaSeeking			*pMS;
	LONGLONG				l2TotalLength;
	BOOL					fAddAudioFile;
} AA_STRUCT;

class CKeyProvider;
class CMedia2WMX {
public:
	HWND				m_hWndProgress, m_hWndAbort;
	long				*m_plAbort;
	long				m_lMsgProgress;
public:
	DWORD				m_dwDebugRegister;
	CKeyProvider		*m_pKeyProvider;
	AA_STRUCT			m_Data;
	IWMProfileManager2	*m_pWMPM;
public:
	CMedia2WMX();
	~CMedia2WMX();
public:
	BOOL Open();
	void Close();	
public:
	BOOL SetupProgressHandler(HWND hwnd, long lMsg) { m_hWndProgress = hwnd; m_lMsgProgress = lMsg; return TRUE; };
	BOOL SetupQueryAbort(HWND hwnd, long *plAbort) { m_hWndAbort = hwnd; m_plAbort = plAbort; return TRUE; };
public:
	BOOL Media2WMV(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut);
	BOOL Media2WMV_Param(BSTR bstrIn, DWORD dwAudioBitRate, DWORD dwVideoBitRate, BSTR bstrOut, BSTR bstrOutProfile);
	BOOL Media2WMA(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut);
	BOOL Media2WMA_Param(BSTR bstrIn, DWORD dwAudioBitRate, BSTR bstrOut, BSTR bstrOutProfile);
	BOOL WMV2WMV_MBR(BSTR bstrIn, BSTR bstrProfile, BSTR bstrOut);
	BOOL IsMediaMute(BSTR bstrMedia);
private:
	BOOL CreateFilterGraph();
	BOOL ConvertWMV_MBR(BSTR bstrIn, BSTR bstrOut, BOOL fMute, IWMProfile *pProfile);
	BOOL ConvertWMV(BSTR bstrIn, BSTR bstrOut, BOOL fMute, IWMProfile *pProfile);
	BOOL ConvertWMA(BSTR bstrIn, BSTR bstrOut, IWMProfile *pProfile);
	BOOL MakeWMVWriterGraph(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfAVStreams);
	BOOL MakeWMAWriterGraph(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfAStream);
	BOOL MakeWMVWriterGraph_MBR(BSTR bstrIn, BSTR bstrOut, IWMProfile *pwmpfAVStreams);
	BOOL ProcessGraphInfo(BOOL fMute);
	BOOL GoConvert();
	BOOL AddMuteAudioToGraph(IGraphBuilder *pFG, LONGLONG l2Length, DWORD dwSamplesPerSec, WORD dwBitsPerSample);
	void FreeFilterGraph();
	void FreeAll();

	BOOL MediaBuildProfile(BSTR bstrIn, DWORD dwAudioBitrate, DWORD dwVideoBitrate, BOOL *pbMute, IWMProfile **ppProfile);
	
	BOOL BuildKeyProvider();
	void FreeKeyProvider();
};

#endif