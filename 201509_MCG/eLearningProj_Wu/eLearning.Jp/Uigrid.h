
#ifndef _CUIGrid
#define _CUIGrid

#define	tMaxScrollRange			32000
#define cGridMaxCol 			1024
#define cGridDefaultColWidth	100
#define cGridInitX				5
#define cGridInitY				1
#define cEditGapTop				3 
#define cEditGapBottom			3
#define cComboExtraHeight		40
#define cGridMaxBMP     		10
#define cGridSpace				6
#define cComboCount				10
#define cAttribNo				10

//Grid Selection Type
#define cGridNonSel		0 
#define cGridRowSel		1
#define cGridHeadSel	2 

//Grid Cell Type
#define cGridText			0
#define cGridBMP			1
#define cGridCombo      	2
#define cGridComboEdit		3  
#define cGridBMPCombo   	4
#define cGridNote			5 
#define	cGridEditBtn		6
#define	cGridComboBtn		7
#define cGridComboEditBtn   8
#define cGridComboDB        15
#define cGridComboEditDB	16
#define cGridPassword		17
#define cGridHyperLink		18
#define cGridHyperBMP		19
#define cGridLast			31

//SeqType
#define cSeqTextCombo		0
#define cSeqBMPCombo		1
#define cSeqDBCombo			2
#define	cSeqRGB				3
#define cSeqAllowedChar		4

//BMPType
#define cBMP				0
#define cBMPBtn				1

//Alignment
#define cLeftAlign			0
#define cCenterAlign		1
#define cRightAlign			2

//BackGround
#define cDefColor			254
#define cGrayColor			253
#define cDefValue			254
 
//Attrib
#define cType				0 
#define cIndex				1
#define cEditable			2
#define cAlign				3
#define cBGColor			4
#define cFGColor			5
#define cTextLimit			6
#define cLimitReached		7
#define cAllowedChar		8

//Color
#define cVLineColor			6
#define cHLineColor			7
#define cSelColor			8
#define cSelRowColor		9
					    
#define SB_ABSOLUTE			999
#define ID_GRID_EDIT		33000
#define ID_GRID_COMBOEDIT	33001
#define ID_GRID_COMBO		33003
#define ID_GRID_BMP_COMBO	33004
#define	cPageSize			50

/////////////////////////////////////////////////////////////////////////////
// CUIGrid window
#define WM_GRD_NOTE				(WM_USER + 119)
#define WM_GRD_DBLCLICK         (WM_USER + 120)
#define WM_GRD_UPDATE           (WM_USER + 121)
#define WM_GRD_CLICK			(WM_USER + 122) 
#define WM_GRD_SORT				(WM_USER + 123)
#define WM_RBUTTON_DRAGDROP     (WM_USER + 125) 
#define WM_GRD_SWAP				(WM_USER + 126) 
#define WM_GRD_COMBO_SEL_CHANGE (WM_USER + 127) 
#define WM_GRD_UPDATED          (WM_USER + 128) 
#define WM_GRD_EDIT_CHANGE      (WM_USER + 129)
#define WM_GRD_BTN_DOWN		    (WM_USER + 130)
#define WM_GRD_CLICKED			(WM_USER + 131) 
#define WM_GRD_ROW_SELECTED		(WM_USER + 132) 
//#define WM_GRD_ROW_SELECTING	(WM_USER + 133)
#define WM_GRD_COL_WIDTH_CHANGE (WM_USER + 134)
#define WM_GRD_FETCHDATA		(WM_USER + 135)
#define WM_GRD_SET_HEADWIDTH	(WM_USER + 136)
#define WM_GRD_SET_ROWHEIGHT	(WM_USER + 137)
#define WM_GRD_CTRL_ON			(WM_USER + 138)
#define WM_GRD_NEED_DATA		(WM_USER + 139)
#define WM_GRD_KEY_DOWN			(WM_USER + 140)
#define WM_GRD_LEAVE_CELL       (WM_USER + 141)

class CUICombo; 
class CGridEdit;              
class CDBObject;
//class CGridCombo;
class CGridComboEdit;
class CUIGrid : public CWnd
{ 
	DECLARE_DYNCREATE(CUIGrid)
// Construction
public:
	CUIGrid();

// Attributes
public:

// Operations
public:
                                                                   
// Implementation
public:
	virtual ~CUIGrid();

protected:
	// Generated message map functions
	//{{AFX_MSG(CUIGrid)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnEditKeyDown(WPARAM wParam, LPARAM lParam);   
	afx_msg void OnEditChange(); 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnComboSelChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnComboEditChange(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnCtrlLoseFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP() 

protected:  
	CString GetValueMap(CString StrMap);
	int		GetBMPIndexFromID(int nID);
	void 	DrawGridLines(CDC* pDC, int StartCol, int EndCol, int Bound,
						  CRect RectGrid, int RightEdge, long StartLine);
	void 	DrawGridContent(CDC* pDC, CObArray* pStrArray, int StartCol, 
							int EndCol, int Bound, long StartLine, 
							BOOL bPrinting, long* pFrameTop);
						  
    void 	DrawSelect(CDC* , int); 
    int 	GetPosXIndex(int);
	CRect 	PointToRect(CPoint); 
	void	PointToRowCol(CPoint point, int& nRow, int& nCol);
	void 	DrawNextBMP(int nRow, int nCol, CString StrCurrent, int nIndex); 
	void 	DrawBtnDown(CDC* pDC, LPRECT lpRect);
	void 	DrawBtnEdge(CDC* pDC, LPRECT lpRect);
	void 	MoveControlPos();  

	void	SetComboContent(CComboBox* pCombo, int nIndex, CString GridCell); 
	void 	SetBMPComboContent(int nIndex, CString GridCell);
	void 	SetDBComboContent(CComboBox* pCombo, int nIndex, CString GridCell);

	void	SetLinePos(int*);                                        
	void	DragLinePos(CPoint);
	void 	TitleButtonPressed(CPoint point);
	int		GetRightEdge(BOOL WithScrollBar = TRUE); 
	void 	DragTitle(CPoint point);
	void 	SelectRows(CPoint point); 
	void 	DrawSelRegion(CDC* pDC, long BeginLine, long EndLine, BOOL BtnUp);  
	void	DrawSelLines(CDC* pDC, int BeginLine, int EndLine, BOOL BtnUp);
	void 	DrawSelByPos(long InitRow, long PreRow, long CurRow); 
	int 	GetMaxHorzRange();
	int 	GetStartCol(int ColGroup); 
	int 	GetEndCol(int StartCol);
	int 	AddColWidth(int StartCol, int EndCol);
    void 	ClickCell(CPoint point, int ClickedRow, int ClickedCol, BOOL bForced = FALSE);
    void 	DragCell(CPoint point, int ClickedRow, int ClickedCol);
    BOOL 	NotCare(CPoint point, BOOL bCheckRight);  
	void 	GetNextEditableCell(int nDir, int StartRow, 
								int StartCol, int* EndRow, int* EndCol);
	void	DrawBMP(CDC*, CRect, int, int bPrinting=FALSE, int align = 3);
	void 	DrawNoteBtn(CDC* pDC, CRect RectCell, BOOL Enable, BOOL Up, 
						int bPrinting, int Pos = 1, int nIndex = -1);
	BOOL 	IsCellCombo(int nCell);
	BOOL 	IsCellComboOrEdit(int nCell);
	BOOL 	NeedFrame(int nCell);
    
    //Scroll Functions
	void 	OnScroll(int, UINT, long);
	long	ScrollToVirtual(int i);	
	int		VirtualToScroll(long l);
	
	CStringArray* GetANewRow(int nRow);
	BOOL	ValidRowCol(int nRow, int nCol);
	void	SetAt(int nRow, int nCol, CString Str);
	
	virtual void PreSetDataAt(int nRow, int nCol, CString& Str){};
	virtual CString PostGetDataAt(int nRow, int nCol, CString Str){return Str;};
	virtual void	PostSetColNo(int nCol);
	virtual CString AddAttribNew(int nRow, int nCol, CString StrContent);
	virtual CString AddAttribSet(int nRow, int nCol, CString StrContent);
	virtual BOOL PreInsertOper(int nIndex, CStringArray* pArray);
	virtual void PostInsertOper(){};
	virtual BOOL PreDelOper(){return TRUE;};
	virtual BOOL PreDelAtOper(long nIndex){return TRUE;};
	virtual BOOL PostDelOper(){return TRUE;};
	virtual BOOL PreDeleteAllOper();
	virtual void OnGrdUpdated(int PreRow, int PreCol){};
	virtual BOOL PreGrdUpdate(int PreRow, int PreCol, CString PreValue, CString PostValue){return TRUE;};
	virtual BOOL NeedData(long nRow);
	virtual void OnTitleButtonPressed(int nIndex);
	virtual void OnColSwap(int SourceX, int DestX);
	//virtual void OnDelKeyDown();
    virtual void KeyDown(UINT);
	virtual void PostLButtonDown(int ClickedRow, int ClickedCol);
	virtual CString SetHeadString(long i);
	virtual void ExtraDrawHeadSel(CDC* pDC, CRect rcFixed, long i);
	virtual void PreRemoveSelRow(int nIndex){};
	virtual void PreInsertSelRow(int nIndex, long lRow){};
	virtual void PasteData(CStringArray* pStrArray){};
	virtual CBrush* GetDefBrush(int nCol);
	void	SetHorzScrollRange();
	void	SetVertScrollRange();
	COLORREF GetColorFromIndex(int nIndex);
	void	PaintWithBrush(CDC* pDC, COLORREF rgb, CRect RectFill);
	virtual CBrush* GetBrush(int nRow, int nCol);
	COLORREF GetCellColor(int nRow, int nCol, int nAttrib);
	void	SetFonts();
	void 	SetDefaultRowWidth();
	void 	SetDynaHeadWidth();
	void	OnMoveCursorKeyDown(UINT nChar);
	void	SwapLong(long& a, long& b);
	void	SwapInt(int& a, int& b);
	int		GetAheadSpace();
	void	FetchNewPage();
	int		GetColGroup(int i);
	BOOL	GetSelPos(long nPos, int& nIndex);
	void	SelOrDeselARow(long nRow, BOOL bDraw = TRUE);
	CStringArray* CopyStrArrayNoAttrib(CStringArray* pSourceArray);
	void	OnEditCopy();
	void	OnEditPaste();
	CWnd*	GetMSGWnd() {return (u_pMSGWnd == NULL) ? GetParent() : u_pMSGWnd;}; 
	CString GetStrOfLen(int nLen,  char ch);
	void CheckCursor(CPoint point);
	void LeaveTrack(); 
	void	SetRowsAndScrollBar();
	int GetNextBMPIndex(CString CurrentShow, int Index);

public:
	virtual int GetGridData(long Start, int Size, CObArray* pArray, BOOL bSel = TRUE);
	virtual void OnGrdUpdate(int PreRow, int PreCol, 
				 BOOL bWidthData = FALSE, CString StrCell = ""){};
	void	SetTotalColNo(int nColNum);
	int 	GetTotalColNo(){return u_Cols;};
	void	SetRowHeight(int RowWidth);
	void	SetColWidth(int nIndex, short nValue);
	void	SetColWidth(CString StrCol, char Del = '\\');
	int		GetColWidth(int nIndex);
	
	BOOL	Create(CRect RectGrid, CWnd* pParent, UINT nGridID);
		
	//Insert A Row
	void	InsertRowAt(int nIndex, int nRowNo);
	void	DeleteAllRows();
	void	DeleteRows(long nIndex, long nEndIndex);
	void	AppendRow(int nRowNo = 1);
	void	AppendCol(int nColNo = 1);
	CObArray* ReplaceGridBuffer(CObArray* pData, BOOL bTransform = FALSE);
	void	CopyGridBuffer(CObArray* pData);

	//Get And Set Data Functions
	void	SetDataAt(int nRow, int nCol, CString StrData);
	void	SetAttribAt(int nRow, int nCol, int nType, char nValue);
	//void	SetCellAt(int nRow, int nCol, CString StrData, char nType, 
	//			      char nIndex, char nEditable, char Alignment,
	//				  char nColorBackGround, char nColorForeGround);
	CString GetDataAt(int nRow, int nCol);
	CString GetDataCtrlAt(int nRow, int nCol);
	CString GetCtrlContent();
	char	GetAttribAt(int nRow, int nCol, int nType);
	CString	GetCellAt(int nRow, int nCol);

	//Combo,... Sequence Function
	void	SetSeqAt(int nType, int nIndex, CString Value);
	CString GetSeqAt(int nType, int nIndex);
	void	RemoveSeqAt(int nType, int nIndex);
	int		GetSeqSize(int nType);

	//BMP functions
	void	SetBMPAt(int nType, int nIndex, CBitmap* pBMP, int nID = -1);
	void	RemoveBMPAt(int nType, int nIndex);
	int		GetBMPSize(int nType);
	void	SetBMPFromID(CWordArray* pBMPIDArray, CString ResFilePath = "");
	CBitmap* GetBMPAt(int nType, int nIndex);

    //Scroll Functions 
	void 	IncVScrollPos();
    void 	SetVScrollPos(long VPos, BOOL bGo = TRUE);
    long	GetVScrollPos(){return u_VirtualScrollPos;};

	long	GetDataSize();
	int		GetFrameSize() {return u_FrameSize;};
	void 	SetFrameSize(int x) {u_FrameSize = x;};
	long	GetFrameTop()	{return u_FrameTop;};
	void 	SetFrameTop(long x) {u_FrameTop = x;};	
	int		GetMaxVertRange()	{return u_MaxVertScrollRange;};

	long	GetVirtualDataSize() {return u_VirtualDataSize;};
	void	SetVirtualDataSize(long x) {u_VirtualDataSize = x;};

	CFont*	GetGridFont() {return u_pFont;};
	CFont*	SetGridFont(CFont* pFont);
    CDBObject*	GetDBObj() {return u_pDBObj;};
	void	SetDBObj(CDBObject* pDBObj) {u_pDBObj = pDBObj;};
	void	SetColor(int nType, COLORREF rgb);
	COLORREF GetColor(int nType);

	void	RePaint();
	void	RePaintCell(int nRow, int nCol);
	void 	RePaintRow(int Row, int EndRow);
	void	RePaintCol(int nCol);

    BOOL 	IsCellClicked(int* pPreRow = NULL, int* pPreCol = NULL);
    void	SetNoSelect();
	BOOL 	UpdateGrdData();
	void	ClickRowCol(int nRow, int nCol);
    BOOL	IsRowInBuffer(long nRow);
	int		GetRowHeight() {return u_RowWidth;};
	int		GetHeadWidth() {return u_HeadWidth;};
	CRect 	RowColToRect(int nRow, int nCol);

	long 	GetLastScrollPos();	
	void 	EraseSel(int Row, int Col);				
	void 	DrawASel(CDC* pDC, BOOL bObligated = FALSE);
	void 	DrawSel(CDC* pDC, int Row, int Col, BOOL bShorter = FALSE);

    void 	LButtonDown(CPoint point, BOOL Ext = TRUE, BOOL NotSetFocus = FALSE);
    void    OnFilePrint(); 
  	void 	OnDraw(CDC* dc, CObArray* pStrArray, long* pFrameTop, 
  				   int bPrinting = FALSE, CSize *pSize = NULL, 
  				   int bPageNo = 2, int yP1Start = 0);

	void	SetFixedRowLabel(CString StrRow, CString Del = "\\");
	CString GetFixedRowLabelAt(int nIndex);
	void	SetFixedRowLabelAt(int nIndex, CString StrRow);
	void	SetFixedColLabel(CString StrCol, CString Del = "\\");
	CString GetFixedColLabelAt(int nIndex);
	void	SetFixedColLabelAt(int nIndex, CString StrCol);

	void	SetEditOverWrite(BOOL bFlag);
	void	SetHeadWidth(int HeadWidth);

	int		GetGridRowsShown(){return u_Rows;};
	BOOL	IsRowVisible(long nRow);
	
	void	DrawSelRows(CDC* pDC, long StartLine, long EndLine, BOOL bBtnUp);
	void	WriteSelCellBack();

	void	SetGridSelType(int SelType, BOOL Multi = TRUE, BOOL Toggle = FALSE);
	int		GetGridSelType() {return u_SelType;};
	BOOL	IsMultiSel() {return u_bMultiSel;};
	BOOL	IsToggleSel() {return u_bToggleSel;};
	void	SetVirtualPage(BOOL bPage){u_bVirtualPage = bPage;};
	void	SetFixedColor(COLORREF rgb){u_rgbFixed = rgb;};
	void	AddValueMap(CString StrMap, CString StrValue);

protected:
	static int			u_GridNo;
	static CString		u_GridWndClass;
	static CFont*		u_pDefaultFont;
    static CBitmap*		u_pNoteEnBMP;
    static CBitmap*		u_pNoteDisBMP;
    static CBitmap*		u_pUnknownBMP;
	
	int 			u_Rows;
	int 			u_Cols;
	CWordArray		u_ColWidth;

	COLORREF		u_rgbVLine;     
	COLORREF		u_rgbHLine;
    COLORREF		u_rgbBG;
	COLORREF		u_rgbSel;
	COLORREF		u_rgbSelRows;

    BOOL			u_bDynaHeadWidth;
	BOOL			u_bDefRowWidth;
	CBrush*			u_pDefBrush;
	CString			u_StrDefAttrib;
	CObArray* 		u_pStrArray;
	CGridEdit*		u_pEdit; 
	CGridEdit*		u_pPassEdit; 
    CGridComboEdit*	u_pCombo;
    CGridComboEdit*	u_pComboEdit;
	int 			u_PreCellType;
    int				u_ScrollPos; 
    int 			u_BMPModified;
    CWnd*			u_pComboOrEdit;
    int 			u_ComboGap;
	long			u_VirtualScrollPos;
    long			u_VirtualLastScrollPos;
    long			u_FrameTop;
	int 			u_MaxVertScrollRange;
    CString			u_PreCellStr;
	CPoint			u_LButtonPoint;
	BOOL			u_bNoUpdate;

	CStringArray*   u_pTableFieldArray;
    CStringArray* 	u_pComboArray;
    CStringArray*	u_pBMPArray;
	CStringArray*	u_pColorArray;
	CStringArray*	u_pAllowedArray;
	
	CStringArray*	u_pValueMapArray;

    CObArray*		u_pBMP;
    CObArray*		u_pBtnBMP;
	
	CString			u_DataBeforeUpdate;

	int				u_FrameSize;
   	long			u_VirtualDataSize;
    CUICombo*		u_pBMPCombo;

    CFont*			u_pFont;
    CDBObject*		u_pDBObj;
	CObArray*		u_pBrushArray;

	int 			u_RowWidth;	 	
	int 			u_HeadWidth;
	int 			u_SelType;   //cGridNonSel, cGridRowSel, cGridHeadSel 

	CStringArray*	u_pFixedRow;
	CStringArray*	u_pFixedCol;
    
	CDWordArray*	u_pSelArray;
	CWordArray*		u_pBMPIDArray;
	//BOOL			u_bFirst;
	BOOL			u_bToggleSel;
   	BOOL			u_bMultiSel;
	BOOL			u_CtrlShown;
	BOOL			u_bOverWrite;
	BOOL			u_bLButtonUp;
    BOOL			u_Dragable; 
    BOOL			u_bGrdUpdated;
    BOOL			u_bVirtualPage;

	COLORREF		u_rgbFixed;
	CObArray*		u_pComboStrArray;
	int				u_nFieldCount;
	int				u_nComboMajor;
	int				u_PreOverRow;
	int				u_PreOverCol;
	BOOL			m_bLeaveTrack;

public:
   	char			u_GridType;
	BOOL 			u_bCellMark;
   	BOOL			u_bSort;
   	BOOL			u_bDragLine;
   	BOOL			u_bDragTitle;
   	BOOL			u_bDragCell;
   	BOOL			u_bHScrollable;
   	BOOL			u_bVScrollable;
    BOOL			u_bFixedButtoned;
	BOOL			u_bBtnNullEnable;
    BOOL			u_bFieldSizeChanged;
    BOOL			u_bBtnClickable;
	BOOL 			u_bEditable;  
	BOOL			u_bWithFixedRow;
	BOOL			u_bWithFixedCol;
	BOOL			u_bAutoCol;
	BOOL			u_bCopyable;
	BOOL			u_bPasteable;
	BOOL			u_bDrawSel;

	CWnd*			u_pMSGWnd;

	int 			u_PreCol;
	int 			u_PreRow; 
    long			u_VirtualPreRow;

	long 			u_SelectedRow;
    long			u_EndSelectedRow;
	
    int				u_GridInitX;
	int				u_TitleAlign;
	COLORREF		u_rgbFG;
	BOOL			u_bPaintBk;
	BOOL			u_bMultilineEdit;
};
                 
#endif                 
/////////////////////////////////////////////////////////////////////////////
