// WndSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "WndSplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndSplitter

CWndSplitter::CWndSplitter()
{
}

CWndSplitter::~CWndSplitter()
{
}


BEGIN_MESSAGE_MAP(CWndSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CWndSplitter)
	//ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndSplitter message handlers

BOOL CWndSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;

	/*return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);*/
}

BOOL CWndSplitter::CreateView(int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	//This code tried to create a right pane with execell tabs.
	//It is woth less.
/*
	if(row==0&&col==1){

		return m_wndFolderFrame.Create(this,
			pViewClass, pContext,
			IDR_FOLDERTABS);

	}
*/
	
	return CSplitterWnd::CreateView(row, col, pViewClass, sizeInit, pContext);
}
