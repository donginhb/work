// EditAllowed.cpp : implementation file
//

#include "stdafx.h"
//#include "elearning.h"
#include "EditAllowed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditAllowed

CEditAllowed::CEditAllowed()
{
	m_strAllowed = "0123456789-.ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

CEditAllowed::~CEditAllowed()
{
}


BEGIN_MESSAGE_MAP(CEditAllowed, CEdit)
	//{{AFX_MSG_MAP(CEditAllowed)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditAllowed message handlers

void CEditAllowed::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	for (int i = 0; i < m_strAllowed.GetLength(); i++)
	{
		if ((char)nChar == m_strAllowed[i])
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
	}
}
