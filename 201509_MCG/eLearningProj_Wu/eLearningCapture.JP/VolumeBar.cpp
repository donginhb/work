// VolumeBar.cpp : implementation file
//

#include "stdafx.h"
#include "VolumeBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeBar

CVolumeBar::CVolumeBar()
{
	volume = MAX_VOLUME / 2;
	isInteracting = false;
	isMouseOnThumb = false;
	isVertical = false;

	bkColor = RGB(255,255,255);
	barColor = RGB(173, 231, 0);
}

CVolumeBar::~CVolumeBar()
{

}


BEGIN_MESSAGE_MAP(CVolumeBar, CStatic)
	//{{AFX_MSG_MAP(CVolumeBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeBar message handlers

#include "memdc.h"

void CVolumeBar::OnPaint() 
{
	if (GetStyle() & TBS_HORZ) {
		isVertical = false;
	}
	if (GetStyle() & TBS_VERT) {
		isVertical = true;
	}

	if (isVertical) {
	
		CPaintDC dc0(this); // device context for painting
		CMemDC dc(&dc0);

		CBrush *pOldBrush = dc.GetCurrentBrush();

		CPen *pOldPen;
		CPen nullPen(PS_NULL, 0, RGB(0,0,0));
		pOldPen = dc.SelectObject( &nullPen );

		
		
		CRect rect;
		GetClientRect(&rect);

		CBrush bkBrush(bkColor);
		dc.FillRect( &rect, &bkBrush);


		int h = rect.Height();
		rect.top += h / 12;
		rect.bottom -= h / 12;


		

		
		// Draw the triangle (whole)
		CPoint pts[3];
		pts[0].x = -rect.Height() / 6 + rect.Width()/2;
		pts[0].y = rect.top;

		pts[1].x = rect.Width()/2;
		pts[1].y = rect.top;

		pts[2].x = rect.Width()/2;
		pts[2].y = rect.bottom;


		CBrush gray(RGB(231,227,231));
		dc.SelectObject(&gray);
		dc.Polygon( pts, 3);

		float pos = volume / ((float) MAX_VOLUME + 1);

		// Draw the triangle (volume)

		pts[0].x = (int)(-(1-pos) * rect.Height() / 6 + rect.Width()/2);
		pts[0].y = (int)(rect.top + pos *  rect.Height());

		pts[1].x = rect.Width()/2;
		pts[1].y = (int)(rect.top + pos *  rect.Height());

		pts[2].x = rect.Width()/2;
		pts[2].y = rect.bottom;

				
		CBrush barBrush(barColor);
				
		dc.SelectObject(&barBrush);
		dc.Polygon( pts, 3);
		
		CBrush dark_gray(RGB(189,190,189));
		CRect r( -1  + rect.Width()/2, rect.top, 0  + rect.Width()/2,rect.bottom);	
		dc.FillRect( &r, &dark_gray);	
		
		CBrush darkest_gray(RGB(90,93,90));
		CRect r2( 1  + rect.Width()/2, rect.top, 2  + rect.Width()/2,rect.bottom);	
		dc.FillRect( &r2, &darkest_gray);
		
		CBrush darker_gray(RGB(128,128,128));
		CRect r3( 0  + rect.Width()/2, rect.top, 1  + rect.Width()/2,rect.bottom);	
		dc.FillRect( &r3, &darker_gray);	

		

		CBrush light_gray(RGB(222, 219, 222));

		// Draws the thumb
		CPen darkPen( PS_SOLID, 1, RGB(128,128,128));
		dc.SelectObject( &darkPen);
		if (isInteracting) {
			dc.SelectObject(&barBrush);
		} else {
			dc.SelectObject(&light_gray);
		}
		thumbRect = CRect( (int)(rect.Width()/2 - rect.Height() / 12), (int)(rect.top + pos *  rect.Height() - rect.Height() / 12),
					(int)(rect.Width()/2 + rect.Height() / 12), (int)(rect.top + pos *  rect.Height() + rect.Height() / 12));
		dc.Ellipse(&thumbRect);
		

		if (isMouseOnThumb || isInteracting) {
			CRect r( thumbRect.left +  thumbRect.Width()/4, thumbRect.top + thumbRect.Height()/2-1, 
					 thumbRect.right - thumbRect.Width()/4, thumbRect.top + thumbRect.Height()/2+1); 
			if (isInteracting) {
				dc.FillRect(&r, &darker_gray);
			} else {
				dc.FillRect(&r, &barBrush);
			}
		}

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	} else {

		CPaintDC dc0(this); // device context for painting
		CMemDC dc(&dc0);

		CBrush *pOldBrush = dc.GetCurrentBrush();

		CPen *pOldPen;
		CPen nullPen(PS_NULL, 0, RGB(0,0,0));
		pOldPen = dc.SelectObject( &nullPen );

		
		CRect rect;
		GetClientRect(&rect);

		CBrush bkBrush(bkColor);
		dc.FillRect( &rect, &bkBrush);
	

		int w = rect.Width();
		
		rect.left += w / 12;
		rect.right -= w / 12;


		// Draw the triangle (whole)
		CPoint pts[3];
		pts[0].x = rect.right;
		pts[0].y = -rect.Width() / 6 + rect.Height()/2;
		
		pts[1].x = rect.right;
		pts[1].y = rect.Height()/2;
		
		pts[2].x = rect.left;
		pts[2].y = rect.Height()/2;
		

		CBrush gray(RGB(231,227,231));
		pOldBrush = dc.SelectObject(&gray);
		dc.Polygon( pts, 3);
		dc.SelectObject(pOldBrush);

		float pos = volume / ((float) MAX_VOLUME + 1);

		// Draw the triangle (volume)

		pts[0].x = (int)(rect.left + pos *  rect.Width());
		pts[0].y = (int)(-pos * rect.Width() / 6 + rect.Height()/2);
		
		pts[1].x = (int)(rect.left + pos *  rect.Width());
		pts[1].y = rect.Height()/2;
		
		pts[2].x = rect.left;
		pts[2].y = rect.Height()/2;
		
		
		
		CBrush barBrush(barColor);
		dc.SelectObject(&barBrush);


		dc.Polygon( pts, 3);
		
		CBrush dark_gray(RGB(189,190,189));
		CRect r( rect.left, -1  + rect.Height()/2, rect.right, 0  + rect.Height()/2);	
		dc.FillRect( &r, &dark_gray);	
		
		CBrush darkest_gray(RGB(90,93,90));
		CRect r2( rect.left, 1  + rect.Height()/2, rect.right, 2  + rect.Height()/2);	
		dc.FillRect( &r2, &darkest_gray);
		
		
		CBrush darker_gray(RGB(128,128,128));
		CRect r3( rect.left, 0  + rect.Height()/2,rect.right, 1  + rect.Height()/2);	
		dc.FillRect( &r3, &darker_gray);	

		

		CBrush light_gray(RGB(222, 219, 222));

		// Draws the thumb
		CPen darkPen( PS_SOLID, 1, RGB(128,128,128));
		dc.SelectObject( &darkPen);
		if (isInteracting) {
			pOldBrush = dc.SelectObject(&barBrush);
		} else {
			pOldBrush = dc.SelectObject(&light_gray);
		}
		thumbRect = CRect( 
					(int)(rect.left + pos *  rect.Width() - rect.Width() / 12),
					(int)(rect.Height()/2 - rect.Width() / 12), 
					(int)(rect.left + pos *  rect.Width() + rect.Width() / 12),
					(int)(rect.Height()/2 + rect.Width() / 12));
					
		
		
		dc.Ellipse(&thumbRect);
		

		if (isMouseOnThumb || isInteracting) {
			CRect r( thumbRect.left + thumbRect.Width()/2-1, thumbRect.top +  thumbRect.Height()/4, 
					 thumbRect.left + thumbRect.Width()/2+1, thumbRect.bottom - thumbRect.Height()/4 ); 
			if (isInteracting) {
				dc.FillRect(&r, &darker_gray);
			} else {
				dc.FillRect(&r, &barBrush);
			}
		}

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
		
	}
		
	// Do not call CSliderCtrl::OnPaint() for painting messages
}


BOOL CVolumeBar::OnEraseBkgnd(CDC* pDC) 
{
	//CStatic::OnEraseBkgnd(pDC);

	return true;
}



unsigned long CVolumeBar::GetVolume()
{
	if (isVertical) {
		return MAX_VOLUME - volume;	
	} else {
		return volume;
	}
 



}

void CVolumeBar::SetVolume(unsigned long v)
{
	if (isVertical) {
		volume = MAX_VOLUME - v;	
	} else {
		volume = v;
	}
	Invalidate();
}

void CVolumeBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	/* wangwang
	if (!isInteracting) {
		isInteracting = true;
		isMouseOnThumb = true;
		SetCapture();
		UpdateVolume(point);
	}
	*/
	CStatic::OnLButtonDown(nFlags,point);
}

void CVolumeBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/* wangwang
	if (isInteracting) {
		isInteracting = false;
		ReleaseCapture();
		UpdateVolume(point);
		Invalidate();
	}
	*/
	CStatic::OnLButtonUp(nFlags,point);
}

void CVolumeBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (isInteracting) {
		UpdateVolume(point);
	}

	if (thumbRect.PtInRect(point)) {
		if (!isMouseOnThumb) {
			isMouseOnThumb = true;
			Invalidate();
		}
	} else {
		if (isMouseOnThumb) {
			isMouseOnThumb = false;
			Invalidate();
		}
	}
	CStatic::OnMouseMove(nFlags,point);
}

void CVolumeBar::UpdateVolume(CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	float vol;
	
	if (isVertical) {
		int h = rect.Height();
		vol = ( (point.y - h/12) / (float) (rect.Height() - 2 * h / 12) * MAX_VOLUME);
	} else {
		int w = rect.Width();	
		vol = ( (point.x - w/12) / (float) (rect.Width() - 2 * w / 12) * MAX_VOLUME);
	}


	if (vol < 0) vol = 0;
	if (vol > MAX_VOLUME) vol = MAX_VOLUME;

	unsigned long oldVolume = volume;
	volume = (unsigned int) vol;

	if (oldVolume != volume) Invalidate();
}

void CVolumeBar::SetOrientation(bool vertical)
{
	isVertical = vertical;
	Invalidate();
}

bool CVolumeBar::GetOrientation()
{
	return isVertical;
}




