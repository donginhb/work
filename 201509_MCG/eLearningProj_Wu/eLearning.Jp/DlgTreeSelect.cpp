// DlgTreeSelect.cpp : implementation file
//

#include "stdafx.h"
#include "eLearning.h"
#include "DlgTreeSelect.h"
#include "ImageContainer.h"
#include "macro.h"
#include "cmkapi.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW


#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTreeSelect dialog

#define cHorzCursor				1
#define cVertCursor				2
#define cSplitBox				2
#define cxSplitter				7
#define cySplitter				7

CDlgTreeSelect::CDlgTreeSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTreeSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTreeSelect)
	//}}AFX_DATA_INIT
	m_pImageContainer = NULL;
	m_bTracking = FALSE;
	m_strDefaultPath = "";
	m_bSelectable = TRUE;
	m_bStretch = FALSE;
	m_strTitle = "";
	m_bResize = FALSE;
	m_bTunneling = FALSE;
	m_bShowFileName = TRUE;
	m_bShowFileExt = TRUE;
	m_nShowType = 0;
	m_nFrameWidth = -1;
	m_nFrameHeight = -1;
	m_nWidth = 580;
	m_nHeight = 480;
	//add by buxaingyi
	m_pPageLayout = FALSE;
	m_pPageImageLogo= FALSE;
	m_pPageImageBackGround= FALSE;
	m_pPageImagePhoto= FALSE;
	m_strShow = "";
}

void CDlgTreeSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTreeSelect)
	//DDX_Text(pDX, IDS_FOLDER, m_strFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTreeSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgTreeSelect)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_DIRTREE, OnItemexpandingDirtree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DIRTREE, OnSelchangedDirtree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_DIRTREE, OnDeleteitemDirtree)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_MESSAGE(WM_PICTURE_SELECTED, OnPictureSelected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTreeSelect message handlers

void CDlgTreeSelect::ResetSize()
{
	CRect rc;
	GetWindowRect(rc);
	rc.right = rc.left + m_nWidth;
	rc.bottom = rc.top + m_nHeight;
	MoveWindow(rc);
}

BOOL CDlgTreeSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ShowWindow(SW_HIDE);
	
	if (m_strTitle != "")
		SetWindowText(m_strTitle);

	if (m_strShow != "")
	{
		CStatic* staShow = (CStatic*)GetDlgItem(IDC_STATIC_FOLDER);
		staShow->SetWindowText(m_strShow);
	}

	CShellTree* pTreeCtl = (CShellTree*)GetDlgItem(IDC_DIRTREE);

	CRect rcTree;
	pTreeCtl->GetWindowRect(rcTree);
	ScreenToClient(rcTree);
	rcTree.right = rcTree.left + m_nWidth / 3;
	pTreeCtl->MoveWindow(rcTree);

	int nWinGap = 0;
	if (m_bSelectable)
	{
		pTreeCtl->EnableImages();
		pTreeCtl->PopulateTree(CSIDL_DRIVES);
		
		m_bTunneling = TRUE;
		pTreeCtl->TunnelTree(m_strDefaultPath);
		m_bTunneling = FALSE;
	
		SelchangedDirtree(m_strDefaultPath);
		
		nWinGap = 3;
	}
	else
	{
		CString strLabel;
		strLabel.LoadString(IDS_SEL_IMAGE);
		CWnd* pLabel = GetDlgItem(IDC_STATIC_FOLDER);
		//pLabel->SetWindowText(strLabel);
		CRect rcLabel;
		pLabel->GetWindowRect(rcLabel);
		ScreenToClient(rcLabel);
		rcLabel.right += 100;
		pLabel->MoveWindow(rcLabel);

		int nTreeWidth = rcTree.Width();
		rcTree.right = rcTree.left;
		pTreeCtl->MoveWindow(rcTree);
		GetDlgItem(IDC_BUTTON_DEFAULT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FOLDER)->ShowWindow(SW_HIDE);
	
		CRect rcWindow;
		GetWindowRect(rcWindow);
		rcWindow.left += nTreeWidth / 3;
		rcWindow.right -= nTreeWidth / 3;
		MoveWindow(rcWindow);
	}

	CRect rcClient;
	GetClientRect(rcClient);
	m_ncxGap = rcTree.left;

	m_pImageContainer = new CImageContainer;
	m_pImageContainer->m_bShowFileName = m_bShowFileName;
	m_pImageContainer->m_bShowFileExt = m_bShowFileExt;
	m_pImageContainer->m_nShowType = m_nShowType;
	m_pImageContainer->m_bSelectable = m_bSelectable;
	if (m_nFrameWidth >= 0 && m_nFrameHeight >= 0)
		m_pImageContainer->SetFrameSize(m_nFrameWidth, m_nFrameHeight);

	CRect rc;
	rc.left = rcTree.right + nWinGap;
	rc.right = rcClient.right - rcTree.left;
	rc.top = rcTree.top;
	rc.bottom = rcTree.bottom;
	m_pImageContainer->Create(WS_VISIBLE | WS_BORDER | WS_CHILD, rc, this);

	CRect rcBtn;
	GetDlgItem(IDCANCEL)->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	m_ncyGap = rcClient.Height() - rcBtn.bottom;
	m_ncyBetween = rcBtn.top - rc.bottom;

	int nBtnHeight = rcBtn.Height();
	int nXOffset = (rcClient.right - m_ncxGap) - rcBtn.right;
	int nYOffset = (rcClient.bottom - m_ncyGap) - rcBtn.bottom;

	rcBtn.left += nXOffset;
	rcBtn.right += nXOffset;
	rcBtn.top += nYOffset;
	rcBtn.bottom += nYOffset;
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rcBtn.left, rcBtn.top, 0, 0, 
								       SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
 
	GetDlgItem(IDOK)->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	rcBtn.left += nXOffset;
	rcBtn.right += nXOffset;
	rcBtn.top += nYOffset;
	rcBtn.bottom += nYOffset;
	GetDlgItem(IDOK)->SetWindowPos(NULL, rcBtn.left, rcBtn.top, 0, 0, 
								       SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

	m_pImageContainer->SetTextFont(MACRO_SYS_FONT);
	m_pImageContainer->SetPath(m_strDefaultPath);

	ResetSize();

	CenterWindow();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTreeSelect::SetFrameSize(int nWidth, int nHeight)
{
	m_nFrameWidth = nWidth;
	m_nFrameHeight = nHeight;
}

void CDlgTreeSelect::SetResizeStyle()
{
	LONG lstyle = GetWindowLong(m_hWnd, GWL_STYLE);
	LONG lExstyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (m_bResize)
	{
		lstyle |= WS_THICKFRAME;
		lstyle &= ~DS_MODALFRAME;
		lExstyle &= ~WS_EX_DLGMODALFRAME;
	}
	else
	{
		lstyle &= ~WS_THICKFRAME;
		lstyle |= DS_MODALFRAME;
		lExstyle |= WS_EX_DLGMODALFRAME;
	}
	SetWindowLong(m_hWnd, GWL_STYLE, lstyle);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lExstyle);
}

void CDlgTreeSelect::OnItemexpandingDirtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CShellTree* pTreeCtl = (CShellTree*)GetDlgItem(IDC_DIRTREE);
	pTreeCtl->OnFolderExpanding(pNMHDR,pResult);
	*pResult = 0;
}

void CDlgTreeSelect::SelchangedDirtree(CString strPath) 
{
	if (m_bTunneling)
		return;

	GetDlgItem(IDC_EDIT_FOLDER)->SetWindowText(strPath);
	if (m_pImageContainer)
		m_pImageContainer->SetPath(strPath);
}

void CDlgTreeSelect::OnSelchangedDirtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CShellTree* pTreeCtl = (CShellTree*)GetDlgItem(IDC_DIRTREE);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString    strTemp;
	if(pTreeCtl->OnFolderSelected(pNMHDR,pResult,strTemp))
		SelchangedDirtree(strTemp); 
	else
		GetDlgItem(IDC_EDIT_FOLDER)->SetWindowText("");

	*pResult = 0;
}

void CDlgTreeSelect::OnDeleteitemDirtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CShellTree* pTreeCtl = (CShellTree*)GetDlgItem(IDC_DIRTREE);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	pTreeCtl->OnDeleteShellItem(pNMHDR, pResult);
	*pResult = 0;
}

void CDlgTreeSelect::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (!::IsWindow(m_hWnd))
		return;

	if (m_pImageContainer == NULL)
		return;

	if (!::IsWindow(m_pImageContainer->m_hWnd))
		return;

	//if (!m_bResize)
	//	return;

	CRect rcClient;
	GetClientRect(rcClient);
	
	CRect rcBtn;
	GetDlgItem(IDCANCEL)->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	int nBtnHeight = rcBtn.Height();
	int nXOffset = (rcClient.right - m_ncxGap) - rcBtn.right;
	int nYOffset = (rcClient.bottom - m_ncyGap) - rcBtn.bottom;

	rcBtn.left += nXOffset;
	rcBtn.right += nXOffset;
	rcBtn.top += nYOffset;
	rcBtn.bottom += nYOffset;
	GetDlgItem(IDCANCEL)->MoveWindow(rcBtn);
 
	GetDlgItem(IDOK)->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	rcBtn.left += nXOffset;
	rcBtn.right += nXOffset;
	rcBtn.top += nYOffset;
	rcBtn.bottom += nYOffset;
	GetDlgItem(IDOK)->MoveWindow(rcBtn);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DEFAULT);
	if (m_bSelectable)
	{
		pWnd->GetWindowRect(rcBtn);
		ScreenToClient(rcBtn);
		rcBtn.top += nYOffset;
		rcBtn.bottom += nYOffset;
		pWnd->MoveWindow(rcBtn);
		pWnd->RedrawWindow();
	}

	GetDlgItem(IDOK)->RedrawWindow();
	GetDlgItem(IDCANCEL)->RedrawWindow();

	CRect rcPos;
	m_pImageContainer->GetWindowRect(rcPos);
	ScreenToClient(rcPos);
	rcPos.right = rcClient.right - m_ncxGap;
	rcPos.bottom = rcClient.bottom - m_ncyGap - nBtnHeight - m_ncyBetween;
	int nBottom = rcPos.bottom;
	m_pImageContainer->MoveWindow(rcPos);

	pWnd = GetDlgItem(IDC_DIRTREE);
	if (m_bSelectable)
	{
		pWnd->GetWindowRect(rcPos);
		ScreenToClient(rcPos);
		rcPos.bottom = nBottom;
		pWnd->MoveWindow(rcPos);
	}

	pWnd = GetDlgItem(IDC_EDIT_FOLDER);
	if (m_bSelectable)
	{
		pWnd->GetWindowRect(rcPos);
		ScreenToClient(rcPos);
		rcPos.right = rcClient.right - m_ncxGap;
		pWnd->MoveWindow(rcPos);
	}
}

void CDlgTreeSelect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pImageContainer->DestroyWindow();
	delete m_pImageContainer;
}

void CDlgTreeSelect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
		return;

	StartTracking(HitTest(point));
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTreeSelect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	StopTracking(TRUE);
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgTreeSelect::OnMouseMove(UINT nFlags, CPoint pt) 
{
	if (!m_bSelectable)
		return;

	if (GetCapture() != this)
		StopTracking(FALSE);

	if (m_bTracking)
	{
		// move tracker to current cursor position

		pt.x -= 1; // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_rectTracker.left != pt.x)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(pt.x - m_rectTracker.left, 0);
			OnInvertTracker(m_rectTracker);
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor

		int ht = HitTest(pt);
		if (ht)
			::SetSplitCursor(cHorzCursor);
		else
		{
			HCURSOR hcr = (HCURSOR)::GetClassLong(m_hWnd, GCL_HCURSOR);
			SetCursor(hcr);
		}
	}
	
	CDialog::OnMouseMove(nFlags, pt);
}

void CDlgTreeSelect::OnInvertTracker(const CRect& rect)
{
	CRect rc = rect;
	ClientToScreen(rc);
	::DrawResizeBar(rc);
}

int CDlgTreeSelect::HitTest(CPoint pt)
{
	ASSERT_VALID(this);

	CRect rcTree;
	GetDlgItem(IDC_DIRTREE)->GetWindowRect(rcTree);
	ScreenToClient(rcTree);

	CRect rcImage;
	m_pImageContainer->GetWindowRect(rcImage);
	ScreenToClient(rcImage);

	CRect rc;
	rc.top = rcImage.top;
	rc.bottom = rcImage.bottom;
	rc.left = rcTree.right;
	rc.right = rcImage.left;
	if (rc.PtInRect(pt))
		return 1;

	return 0;
}

void CDlgTreeSelect::GetHitRect(CRect& rectHit)
{
	ASSERT_VALID(this);

	CRect rcTree;
	GetDlgItem(IDC_DIRTREE)->GetWindowRect(rcTree);
	ScreenToClient(rcTree);

	CRect rcImage;
	m_pImageContainer->GetWindowRect(rcImage);
	ScreenToClient(rcImage);

	rectHit.top = rcImage.top;
	rectHit.bottom = rcImage.bottom;
	rectHit.left = rcTree.right - 1;
	rectHit.right = rcImage.left + 1;
}

void CDlgTreeSelect::StartTracking(int ht)
{
	if (!m_bSelectable)
		return;

	if (!ht)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);
	GetHitRect(m_rectTracker);

	// steal focus and capture
	SetCapture();
	SetFocus();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	OnInvertTracker(m_rectTracker);
	::SetSplitCursor(cHorzCursor);
}

void CDlgTreeSelect::StopTracking(BOOL bAccept)
{
	if (!m_bSelectable)
		return;

	if (!m_bTracking)
		return;

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	m_bTracking = FALSE;

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-1 , -1);
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	CRect rcPos, rcPos1, rcClient;
	GetClientRect(rcClient);

	m_pImageContainer->GetWindowRect(rcPos);
	GetDlgItem(IDC_DIRTREE)->GetWindowRect(rcPos1);
	int nGap = rcPos.left - rcPos1.right;
	ScreenToClient(rcPos);
	rcPos.left = m_rectTracker.right;
	rcPos.right = rcClient.right - m_ncxGap;
	m_pImageContainer->SetWindowPos(NULL, rcPos.left, rcPos.top, 
									rcPos.Width(), rcPos.Height(), 
									SWP_SHOWWINDOW | SWP_NOZORDER);

	ScreenToClient(rcPos1);
	rcPos1.right = rcPos.left - nGap;
	GetDlgItem(IDC_DIRTREE)->SetWindowPos(NULL, rcPos1.left, rcPos1.top, 
									rcPos1.Width(), rcPos1.Height(), 
									SWP_SHOWWINDOW | SWP_NOZORDER);
}

BOOL CDlgTreeSelect::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && pWnd == this && !m_bTracking)
		return TRUE;    // we will handle it in the mouse move

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgTreeSelect::GetInsideRect(CRect& rect) 
{
	ASSERT_VALID(this);

	GetClientRect(rect);
	ASSERT(rect.left == 0 && rect.top == 0);
	rect.right -= (m_ncxGap + 1);
	rect.left += (m_ncxGap - 1);
}

void CDlgTreeSelect::OnButtonDefault() 
{
	CString strFolder = m_strDefaultPath;
	CShellTree* pTreeCtl = (CShellTree*)GetDlgItem(IDC_DIRTREE);

	m_bTunneling = TRUE;
	pTreeCtl->TunnelTree(strFolder);
	m_bTunneling = FALSE;

	SelchangedDirtree(strFolder);
}

void CDlgTreeSelect::OnOK() 
{
	CString strPath;
	if (!m_pImageContainer->GetSelectedFile(strPath))
	{
		//add by buxaingyi
		if (m_pPageLayout == TRUE)
		{
			W_SHOW_WARNING(IDS_NO_IMAGE_SELECTED, this);
			return;
		} 
		else if(m_pPageImageLogo == TRUE)
		{
			W_SHOW_WARNING(IDS_NO_IMAGE_LOGO_SELECTED, this);
			return;
		}
		else if(m_pPageImageBackGround == TRUE)
		{
			W_SHOW_WARNING(IDS_NO_IMAGE_BACKGROUND_SELECTED, this);
			return;
		}
		else if(m_pPageImagePhoto == TRUE)
		{
			W_SHOW_WARNING(IDS_NO_IMAGE_PHOTO_SELECTED, this);
			return;
		}
		
	}
	m_strSelectedFile = strPath;

	CDialog::OnOK();
}

LRESULT CDlgTreeSelect::OnPictureSelected(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0L;
}

void CDlgTreeSelect::ShowFileName(BOOL b)
{ 
	m_bShowFileName = b; 
	m_bShowFileExt = b; 
};

void CDlgTreeSelect::ShowFileExt(BOOL b)
{ 
	m_bShowFileExt = b; 
};

void CDlgTreeSelect::SetShowType(int n)
{ 
	m_nShowType = n; 
};

void CDlgTreeSelect::OnCancel() 
{
	CDialog::OnCancel();
}
