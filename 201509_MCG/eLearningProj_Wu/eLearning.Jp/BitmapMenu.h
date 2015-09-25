class CBitmapMenu : public CMenu
{
   void Init() ;

// Implementation
   virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

public:
	CBitmapMenu();
	virtual ~CBitmapMenu();
	void SetMenuFont(CFont* pFont);
	BOOL Attach(HMENU hMenu);
	void CreateImage();
	void AddImage(UINT nID);

protected:
	void	DrawButton(CDC *pDC, CRect rcBitmap, BOOL bDraw);
	void	DrawDisabled(CImageList *imageList, CDC *dc, int imageID, CPoint &position, CSize &size);
	CSize	GetDrawSize(CString str);
	int		GetMenuIndex(UINT nID);
	void	DrawSeparator(CDC *pDC, CRect rcSep);

protected:
	CImageList	m_ImageList;
	CFont*		m_pFont;
	CSize		m_szText;
	CSize		m_szBitmap;
	CWordArray	m_dwArrayID;
};

