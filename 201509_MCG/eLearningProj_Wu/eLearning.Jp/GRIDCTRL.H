#define WM_EDIT_KEY_DOWN				(WM_USER + 100) 
#define WM_GRD_COMBOEDIT_CHANGE			(WM_USER + 101)
#define WM_EDIT_LOSE_FOCUS_MODIFIED     (WM_USER + 102)
#define WM_COMBO_SEL_CHANGE				(WM_USER + 103)

/////////////////////////////////////////////////////////////////////////////
// CGridEdit window
class CGridEdit : public CEdit
{
	DECLARE_DYNAMIC(CGridEdit)
	
	// Construction
public:
	CGridEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CGridEdit();

protected:
	// Generated message map functions
	//{{AFX_MSG(CGridEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags); 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()  

public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL b3D = TRUE);			
	void SetOverWrite(BOOL bFlag) {u_bOverWrite = bFlag;};
	void SetTextLimit(int n) {u_TextLimit = n;};
	void SetLimitReached(int n) {u_LimitReached = n;};
	void SetAllowedChar(CString Str) {u_AllowedStr = Str;};
	void SetFontEx(CFont* pFont);
	//void SetKeyDownNoProc() {u_bKeyDownNoProc = TRUE;};

protected:
	static BOOL u_bOverWrite;
	int u_TextLimit;
	int u_LimitReached;
	CString u_AllowedStr;
	BOOL u_bLeading;
	char u_szBuf[3];
	BOOL u_bRet;
	//BOOL u_bKeyDownNoProc;

public:
	BOOL u_bEditable;
	BOOL u_bMutiLine;
};

class CGridCombo : public CComboBox
{
	DECLARE_DYNAMIC(CGridCombo)

// Construction
public:
	CGridCombo();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CGridCombo();

protected:
	// Generated message map functions
	//{{AFX_MSG(CGridComboEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CBrush* u_pBrush;
	COLORREF u_rgbFG;
};                      

class CUICombo : public CGridCombo
{
	DECLARE_DYNAMIC(CUICombo)

protected:
	//{{AFX_MSG(CGridCombo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
// Implementation
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	CUICombo();
	virtual ~CUICombo();

public:
	CObArray*	u_pBMPArray;
	CWordArray*	u_pBMPIDArray;
	int			u_BMPId;	//Added for the abnormal value of 
							//lpMIS->itemData in function MeasureItem
							//after porting	
	CBitmap*	u_pUnknownBMP;
	int			u_InitX;
	int			u_Align;
};

/////////////////////////////////////////////////////////////////////////////
// CGridComboEdit

class CGridComboEdit : public CComboBox
{
	DECLARE_DYNAMIC(CGridComboEdit)

	// Construction
public:
	CGridComboEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CGridComboEdit();

protected:
	// Generated message map functions
	//{{AFX_MSG(CGridComboEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditChange();
	afx_msg LRESULT OnEditKeyDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoseFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtlColorList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CGridEdit*	u_pEdit;
	BOOL		u_bFirst;
	int			u_ColNo;
	CWordArray* u_pColWidthArray;
	HWND		u_hwndList;
	int			u_nMaxWidth;
	int			u_nCurSel;
	BOOL		u_bDestroying;
	COLORREF	u_BGColor;
	COLORREF	u_FGColor;
	HBRUSH		u_hBgColor;
	BOOL		u_bNoPartial;
	BOOL		u_bDropDown;

protected:
	void DrawSelect(CDC* pDC, CRect rc, COLORREF rgb);
	void DrawListText(CDC* pDC, int nIndex, LPRECT prc);

public:
	int u_Height;
	CObArray* u_pComboArray;
	BOOL u_bNoDrawIfOne;

public:
	void SetOverWrite(BOOL bFlag) {if (u_pEdit != NULL) u_pEdit->SetOverWrite(bFlag);};
	void SetTextLimit(int n) {if (u_pEdit!= NULL) u_pEdit->SetTextLimit(n);};
	void SetLimitReached(int n) {if (u_pEdit!= NULL) u_pEdit->SetLimitReached(n);};
	void SetAllowedChar(CString Str) {if (u_pEdit!= NULL) u_pEdit->SetAllowedChar(Str);};
	void SetFontEx(CFont* pFont);
	void SetModify(BOOL bValue){if (u_pEdit!= NULL) u_pEdit->SetModify(bValue);};
	BOOL GetModify(){if (u_pEdit!= NULL) return u_pEdit->GetModify(); return TRUE;};
	//void SetKeyDownNoProc() {if (u_pEdit!= NULL) u_pEdit->SetKeyDownNoProc();};
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	void SetColNo(int nCol);
	void ReCalcWidth();
	void SetWindowTextAndSel(CString StrText);
	BOOL IsDestroying() {return u_bDestroying;};
	void SetBGColor(COLORREF Color);
	void SetFGColor(COLORREF Color) {u_FGColor = Color;};
	void SetPartial(BOOL bFlag) {u_bNoPartial = !bFlag;};
	BOOL GetPartial() {return !u_bNoPartial;};
	void SetDropDown(BOOL b) {u_bDropDown = b;};
	BOOL GetDropDown() {return u_bDropDown;};
	void InitCombo();
};

