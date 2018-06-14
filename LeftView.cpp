// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "Globals.h"
#include "DraftDraw.h"

#include "DraftDrawDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	//ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style|=(TVS_HASLINES|TVS_HASBUTTONS);

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);

	AddTreeViewItems();
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CDraftDrawDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDraftDrawDoc)));
	return (CDraftDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

BOOL CLeftView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	/*
	return CTreeView::OnSetCursor(pWnd, nHitTest, message);
	*/
}

void CLeftView::AddTreeViewItems(HTREEITEM hitem /*= NULL*/)
{
	//locals
	HANDLE hEnum=NULL;
	//HTREEITEM hHwd,hEquipos;
	HTREEITEM hRoot,hHD,hRadio;
	DWORD dwResult=NO_ERROR;
	//DWORD dwResultEnum;

	//Initialize tree control with root elements
	if(hitem==NULL){

		hRoot=AddOneItem(NULL, _T("All"), 
		(HTREEITEM)TVI_LAST, iMyComp,TRUE);

		hHD=AddOneItem(hRoot, _T("Design"), 
		(HTREEITEM)TVI_LAST, iHardDrv,TRUE);

		//This code shoul be modified to add right view HWND as parameter.
		//DONE
		CWnd *pWnd=GetParent();
		ASSERT(pWnd->IsKindOf( RUNTIME_CLASS( CSplitterWnd ) ) );
		pWnd=((CSplitterWnd *)pWnd)->GetPane(0,1);

		HTREEITEM hSheet=AddOneItem(hHD, _T("Hoja1"), 
		(HTREEITEM)TVI_LAST, iSheet,TRUE,(DWORD)pWnd->m_hWnd);

		AddOneItem(hSheet, _T("Parts"), 
		(HTREEITEM)TVI_LAST, iFolderClose,FALSE);

		hRadio=AddOneItem(hSheet, _T("Wires"), 
		(HTREEITEM)TVI_LAST, iFolderClose,FALSE);

		GetTreeCtrl().SelectSetFirstVisible( hRadio );

	}

}

HTREEITEM CLeftView::AddOneItem(HTREEITEM hParent, LPTSTR szText, HTREEITEM hInsAfter, int iImage, BOOL bChildren, DWORD data/*=0*/)
{
	HTREEITEM hItem;       // return value
	TV_ITEM tvI;           // item structure
	TV_INSERTSTRUCT tvIns; // item insert structure


	// *** set tvI.mask member ***
	if(bChildren)			
	 tvI.mask = TVIF_TEXT | TVIF_IMAGE | 
				TVIF_SELECTEDIMAGE | 
				TVIF_PARAM | 
				TVIF_CHILDREN;//item has children
	else  // item has no children
	 tvI.mask = TVIF_TEXT | TVIF_IMAGE | 
				TVIF_SELECTEDIMAGE | 
				TVIF_PARAM;

	// set text, text length, image 
	tvI.pszText = szText;
	tvI.cchTextMax = lstrlen(szText);
	tvI.iImage = iImage;

	/*indexing temporaly disabled*/
	/*int index=netresArray.Add(data);
	tvI.lParam=index;*/
	tvI.lParam=data;/*0;*/

	// get type of icon to display
	if(bChildren)	 
	// selected a folder, OK to change bitmap
/*
	if(hParent != NULL)	
	  tvI.iSelectedImage = iFolderOpen;
	else  // selected the hard drive icon, 
		  //don't change bitmap!!
*/
	  tvI.iSelectedImage = iImage;
	else
	  tvI.iSelectedImage = iImage;
	tvI.cChildren= 1; // allow one more level down

	// do the insert
	tvIns.item = tvI;
	tvIns.hInsertAfter = hInsAfter;
	tvIns.hParent = hParent;
	hItem = GetTreeCtrl().InsertItem(&tvIns);
	return (hItem); // return (add error trap here)

}
