// ListCtrlCR.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "ListCtrlCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCR

CListCtrlCR::CListCtrlCR()
{
}

CListCtrlCR::~CListCtrlCR()
{
}


BEGIN_MESSAGE_MAP(CListCtrlCR, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlCR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCR message handlers

int CALLBACK CListCtrlCR::CompareProcAlphDesc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   return strcmp(strItem2, strItem1);
}

