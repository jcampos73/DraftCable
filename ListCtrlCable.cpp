// ListCtrlCable.cpp : implementation file
//

#include "stdafx.h"
/*#include "PruDialogCableEdit.h"*/
#include "ListCtrlCable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCable

CListCtrlCable::CListCtrlCable()
{
}

CListCtrlCable::~CListCtrlCable()
{
}


BEGIN_MESSAGE_MAP(CListCtrlCable, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlCable)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCable message handlers

void CListCtrlCable::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT CListCtrlCable::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int i;

	switch(message){

	case WM_CHAR:

		i=0;
		break;

	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

