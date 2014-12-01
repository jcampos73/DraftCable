// PartPView.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PartPView.h"
#include "DialogPlacePart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartPView

IMPLEMENT_DYNCREATE(CPartPView, CView)

CPartPView::CPartPView()
{
	m_pShape=NULL;
}

CPartPView::~CPartPView()
{
	if(m_pShape){

		delete(m_pShape);
	}

}


BEGIN_MESSAGE_MAP(CPartPView, CView)
	//{{AFX_MSG_MAP(CPartPView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartPView operations

void CPartPView::SetPlacePartDlg(CWnd *pPlacePartDlg)
{
	m_pPlacePartDlg = pPlacePartDlg;
}

/////////////////////////////////////////////////////////////////////////////
// CPartPView drawing

void CPartPView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if(m_pShape){

		//MAP MODE
		int prev_mapm=pDC->SetMapMode(MM_ANISOTROPIC);

		CRect rect;
		GetClientRect(&rect);

		//Control for zoom - operation
		float scale_x=m_pShape->m_Rect.Width()+rect.Width();
		scale_x/=rect.Width();
		float scale_y=m_pShape->m_Rect.Height()+rect.Height();
		scale_y/=rect.Height();
		int scale=(scale_x>scale_y?scale_x:scale_y);

		CSize m_szWext=CSize(scale,scale);
		CSize m_szVPext=CSize(1,1);


		//Control for zoom + operation
		if(scale==1){


			scale_x=rect.Width();
			scale_x/=m_pShape->m_Rect.Width();
			scale_y=rect.Height();
			scale_y/=m_pShape->m_Rect.Height();
			scale=(scale_x>scale_y?scale_y:scale_x);


			m_szWext=CSize(1,1);
			m_szVPext=CSize(scale,scale);

			//Zoomed shape are sensible to offsets in origin:
			//shape goes out of preview screen
			m_pShape->m_Rect.OffsetRect(-m_pShape->m_Rect.TopLeft());

		}

		pDC->SetWindowExt(m_szWext);
		pDC->SetViewportExt(m_szVPext);



		m_pShape->OnDraw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPartPView diagnostics

#ifdef _DEBUG
void CPartPView::AssertValid() const
{
	CView::AssertValid();
}

void CPartPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPartPView message handlers


int CPartPView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// This method needs to be overridden to prevent an assertion
	// As described here
	// http://www.codeguru.com/cpp/w-d/dislog/article.php/c5007/Creating-a-View-on-a-Dialog.htm
	return MA_ACTIVATE;
}


void CPartPView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CView::OnLButtonDown(nFlags, point);

	/*
	NMHDR nmhdr;
	nmhdr.code = LVN_BEGINDRAG;
	nmhdr.idFrom = IDC_LISTPART;
	nmhdr.hwndFrom = GetDlgItem(IDC_LISTPART)->m_hWnd;
	*/

	/*
	NM_LISTVIEW NMListView;
	NMListView.iItem = 0;
	NMListView.ptAction = point;
	::PostMessage(m_pPlacePartDlg->m_hWnd, WM_NOTIFY, IDC_LISTPART, (LPARAM)&NMListView);
	*/

	//Does not work: asserts
	/*
	NM_LISTVIEW NMListView;
	NMListView.iItem = 0;
	NMListView.ptAction = point;
	LRESULT result;
	((CDialogPlacePart*)m_pPlacePartDlg)->OnBegindragListpart((NMHDR*)&NMListView, &result);
	*/

	CView::OnLButtonDown(nFlags, point);

#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL_

	int nDragIndex = ((CDialogPlacePart*)m_pPlacePartDlg)->DoGetLibraryAndPart();

	if (nDragIndex >= 0){

		//This routine sets the parameters for a Drag and Drop operation.
		//It sets some variables to track the Drag/Drop as well
		// as creating the drag image to be shown during the drag.

		//// Save the index of the item being dragged in m_nDragIndex
		////  This will be used later for retrieving the info dragged
		((CDialogPlacePart*)m_pPlacePartDlg)->m_nDragIndex = nDragIndex;

		//// Create a drag image
		POINT pt;
		int nOffset = -10; //offset in pixels for drag image
		pt.x = nOffset;
		pt.y = nOffset;

		/*((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragImage =
			m_lcPart.CreateDragImage(((CDialogPlacePart*)m_pPlacePartDlg)->m_nDragIndex, &pt);*/
		ASSERT(((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragImage); //make sure it was created
		//We will call delete later (in LButtonUp) to clean this up

		CBitmap bitmap;
		/*
		if (m_lcPart.GetSelectedCount() > 1) //more than 1 item in list is selected
			//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
			//bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);
			bitmap.LoadBitmap(IDB_BITMAP_BOX);
		else
			bitmap.LoadBitmap(IDB_BITMAP_BOX);
		*/

		((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragImage->Replace(0, &bitmap, &bitmap);

		//// Change the cursor to the drag image
		////	(still must perform DragMove() in OnMouseMove() to show it moving)
		((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
		/*((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);*/

		//// Set dragging flag and others
		((CDialogPlacePart*)m_pPlacePartDlg)->m_bDragging = TRUE;	//we are in a drag and drop operation
		((CDialogPlacePart*)m_pPlacePartDlg)->m_nDropIndex = -1;	//we don't have a drop index yet
		/*((CDialogPlacePart*)m_pPlacePartDlg)->m_pDragList = &m_lcPart;*/ //make note of which list we are dragging from
		/*((CDialogPlacePart*)m_pPlacePartDlg)->m_pDropWnd = &m_lcPart;*/	//at present the drag list is the drop list

		//// Capture all mouse messages
		SetCapture();

	}

#endif
}


void CPartPView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CPartPView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}
