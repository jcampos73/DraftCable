////////////////////////////////////////////////////////////////
// 1999 Microsoft Systems Journal
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "stdafx.h"
#include "FolderFrame.h"
#include <afxpriv.h> // for WM_INITIALUPDATE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MINSCROLLBARWIDTH = 4*GetSystemMetrics(SM_CXVSCROLL);

IMPLEMENT_DYNAMIC(CFolderFrame, CWnd)
BEGIN_MESSAGE_MAP(CFolderFrame, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

CFolderFrame::CFolderFrame()
{
	m_cxFolderTabCtrl=hide;
}

CFolderFrame::~CFolderFrame()
{
}

//////////////////
// pre-creation: use client edge (assumes Win4)
//
BOOL CFolderFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style |= WS_CLIPCHILDREN;
	cs.style &= ~WS_BORDER;
   return CWnd::PreCreateWindow(cs);
}

//////////////////
// Create window: register class, create view
//
BOOL CFolderFrame::Create(CWnd* pParent, CRuntimeClass* pViewClass,
	CCreateContext* pcc, UINT nIDRes, int cxFolderTabCtrl, DWORD dwStyle)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	static LPCTSTR lpClassName = NULL;
	if (!lpClassName) {
		lpClassName = _T("PxlFolderFrame");
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = NULL; //CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		VERIFY(AfxRegisterClass(&wc));
	}
	m_nIDRes = nIDRes;
	SetFolderTabWidth(cxFolderTabCtrl);

	CRect rc(0,0,0,0);
	if (!CreateEx(0,lpClassName,NULL,dwStyle,rc,pParent,AFX_IDW_PANE_FIRST))
		return FALSE;
	
	CWnd* pView = (CWnd*)pViewClass->CreateObject();
	ASSERT(pView);
	if (!pView->Create(NULL, NULL, dwStyle, rc, this, AFX_IDW_PANE_FIRST, pcc))
		return FALSE;

	pView->SendMessage(WM_INITIALUPDATE);
	return TRUE;
}

//////////////////
// Now create scroll bars and folder tab control
//
int CFolderFrame::OnCreate(LPCREATESTRUCT lpcs)
{
	CRect rc(0,0,0,0);
	VERIFY(m_wndSBHorz.Create(WS_VISIBLE|WS_CHILD|SBS_HORZ,
		rc, this, AFX_IDW_HSCROLL_FIRST));
	VERIFY(m_wndSBVert.Create(WS_VISIBLE|WS_CHILD|SBS_VERT,
		rc, this, AFX_IDW_HSCROLL_FIRST+1));
	VERIFY(m_wndSBBox.Create(WS_VISIBLE|WS_CHILD|SBS_SIZEBOX,
		rc, this,-1));
	
	CFolderTabCtrl& ftc = m_wndFolderTabCtrl;
	VERIFY(ftc.Create(WS_CHILD|WS_VISIBLE, rc, this, 1));
	if (m_nIDRes)
		ftc.Load(m_nIDRes);

	ShowControls(m_cxFolderTabCtrl);

	return 0;
}

//////////////////
// window was sized: move view and scroll bars
//
void CFolderFrame::OnSize(UINT nType, int cx, int cy)
{
	CWnd* pView = GetView();
	if (!pView)
		return;

	CRect rc(0,0,cx,cy);

	// First, size the view.
	// 
	int cxFolderTabCtrl = m_cxFolderTabCtrl;
	if (cxFolderTabCtrl > (rc.Width()- MINSCROLLBARWIDTH))
		// make room for horz scrollbar
		cxFolderTabCtrl = rc.Width()- MINSCROLLBARWIDTH;
	int cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	if (cxFolderTabCtrl>=0) {
		rc.right  -= cxVScroll;
		rc.bottom -= cyHScroll;
	}
	pView->MoveWindow(rc);

	// Next, move the scroll bars: horz, vert and box
	// 
	if (cxFolderTabCtrl<0)
		return;
	rc.left   = rc.right;
	rc.right += cxVScroll;
	m_wndSBVert.MoveWindow(rc);

	rc.left   = max(cxFolderTabCtrl,0);
	rc.right -= cxVScroll;
	rc.top    = rc.bottom;
	rc.bottom+= cyHScroll;
	m_wndSBHorz.MoveWindow(rc);

	if (cxFolderTabCtrl>=0) {
		CRect rc2 = rc;
		rc2.left  = 0;
		rc2.right = cxFolderTabCtrl;
		m_wndFolderTabCtrl.MoveWindow(rc2);
	}

	rc.left = rc.right;
	rc.right+=cxVScroll;
	m_wndSBBox.MoveWindow(rc);
}

//////////////////
// Show controls (scroll bars and folder tab control).
//
void CFolderFrame::ShowControls(int width)
{
	if (width == bestFit)
		width = m_wndFolderTabCtrl.GetDesiredWidth();
	SetFolderTabWidth(width);

	int iShow = m_cxFolderTabCtrl>0 ? SW_SHOW : SW_HIDE;
	m_wndSBVert.ShowWindow(iShow);
	m_wndSBHorz.ShowWindow(iShow);
	m_wndSBBox.ShowWindow(iShow);
	m_wndFolderTabCtrl.ShowWindow(iShow);
	if (iShow==SW_SHOW)
		UpdateBarSize();
}

//////////////////
// Set width of folder tabs.
//
void CFolderFrame::SetFolderTabWidth(int width)
{
	if (width>0) {
		CRect rc;
		GetClientRect(&rc);
if (rc.Width()==0){}
		else if (width > (rc.Width()- MINSCROLLBARWIDTH))
			width = rc.Width()- MINSCROLLBARWIDTH;
	}
	if (width != m_cxFolderTabCtrl) {
		m_cxFolderTabCtrl = width;
		UpdateBarSize();
	}
}

//////////////////
// Update scrollbars after hide/show or width change: Send WM_SIZE to myself.
// This will force MFC to recalc the layout.
//
void CFolderFrame::UpdateBarSize()
{
	CRect rc;
	GetClientRect(&rc);
	SendMessage(WM_SIZE, 0, MAKELPARAM(rc.Width(), rc.Height()));
}

CScrollBar* CFolderFrame::GetScrollBar(int nBar) const
{
	return nBar==SB_HORZ ? const_cast<CScrollBar*>(&m_wndSBHorz)
		: nBar==SB_VERT ? const_cast<CScrollBar*>(&m_wndSBVert) : NULL;
}

//////////////////
// pass scroll messages to view
//
void CFolderFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	GetView()->SendMessage(WM_HSCROLL,
		MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
}
void CFolderFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	GetView()->SendMessage(WM_VSCROLL,
		MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
}

//////////////////
// command routing: pass commands to view and myself
//
BOOL CFolderFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (GetView()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

////////////////////////////////////////////////////////////////
// CFolderView
//
IMPLEMENT_DYNAMIC(CFolderView, CScrollView)
BEGIN_MESSAGE_MAP(CFolderView, CScrollView)
	ON_NOTIFY(FTN_TABCHANGED, 1, OnChangedTab)
END_MESSAGE_MAP()

/////////////////
// Get scrollbar: send message to parent
//
CScrollBar* CFolderView::GetScrollBarCtrl(int nBar) const
{
	return GetFolderFrame()->GetScrollBar(nBar);
}

//////////////////
// Changed tab
//
//void CFolderView::OnChangedTab(NMFOLDERTAB* nmtab, LRESULT* pRes)
void CFolderView::OnChangedTab(NMHDR* nmtab, LRESULT* pRes)
{
	//OnChangedFolder(nmtab->iItem);
	OnChangedFolder(nmtab->idFrom);
}

void CFolderView::OnChangedFolder(int iPage)
{
	// you must override

}

CFolderFrame* CFolderView::GetFolderFrame() const
{
	CFolderFrame* pFolderFrame = (CFolderFrame*)GetParent();
	ASSERT(pFolderFrame);
	ASSERT_KINDOF(CFolderFrame, pFolderFrame);
	return pFolderFrame;
}
