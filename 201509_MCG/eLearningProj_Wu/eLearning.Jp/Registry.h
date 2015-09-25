BOOL WINAPI RegGetKey(CString StrKey, CString& StrValue);
BOOL WINAPI RegGetKey(CString StrKey, int& nStrValue);

BOOL WINAPI RegSetKey(CString StrKey, CString StrValue);
BOOL WINAPI RegSetKey(CString StrKey, int nStrValue);

BOOL WINAPI RegKeyExists(CString StrKey);
