// ChildFrmPP.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "DraftDraw.h"

#include "ChildFrmPP.h"
#include "LeftView.h"
#include "DraftDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFramePP

IMPLEMENT_DYNCREATE(CChildFramePP, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFramePP, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFramePP)
	//ON_WM_SETCURSOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	//ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	//ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFramePP construction/destruction

CChildFramePP::CChildFramePP()
{
	// TODO: add member initialization code here
	
	m_bActivated=FALSE;
}

CChildFramePP::~CChildFramePP()
{
}

//BOOL CChildFramePP::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
//	CCreateContext* pContext)
//{
	// create splitter window
/*	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CDraftDrawView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}*/

BOOL CChildFramePP::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFramePP diagnostics

#ifdef _DEBUG
void CChildFramePP::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFramePP::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
BOOL CChildFramePP::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
/*	
	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
*/
}

void CChildFramePP::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class

	//Knowledge Base/Visual C++
	//How to Create a Minimized/Maximized MDI Child Window
	//ID: Q142317

    if (!m_bActivated) 
    {
        m_bActivated = TRUE;
        nCmdShow = SW_SHOWNORMAL;
    }	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

void CChildFramePP::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CView *pView=GetActiveView( );

	if(pView->DestroyWindow()!=TRUE){
		return;
	}
	
	CMDIChildWnd::OnClose();
}
