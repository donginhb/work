#pragma once
#include "atltypes.h"


extern class CCtrlPanelWnd;

// CTWnd

class CTWnd : public CWnd
{
	DECLARE_DYNAMIC(CTWnd)

public:
	CTWnd();
	virtual ~CTWnd();

protected:
	DECLARE_MESSAGE_MAP()
	CCtrlPanelWnd* panelWnd;
public:
	BOOL CreateEx(CCtrlPanelWnd* panelWnd, const RECT& rect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
protected:
	bool isMouseDown;
	CPoint lastPoint;
};


