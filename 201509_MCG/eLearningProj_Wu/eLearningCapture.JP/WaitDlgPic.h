// WaitDlgPic.h: CWaitDlgPic クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAITDLGPIC_H__E584E4A0_8584_44D9_9D5C_793BF52F9F88__INCLUDED_)
#define AFX_WAITDLGPIC_H__E584E4A0_8584_44D9_9D5C_793BF52F9F88__INCLUDED_

#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaitDlgPic : public CWnd  
{
public:
	CWaitDlgPic();
	virtual ~CWaitDlgPic();
	BOOL Create();

protected:
	//{{AFX_MSG(CWaitDlgPic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap*	m_Waitbitmap;
	int			m_WbitWidth;
	int			m_WbitHeight;

};

#endif // !defined(AFX_WAITDLGPIC_H__E584E4A0_8584_44D9_9D5C_793BF52F9F88__INCLUDED_)
