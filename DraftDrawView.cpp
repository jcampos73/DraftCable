// DraftDrawView.cpp : implementation of the CDraftDrawView class
//

#include "stdafx.h"
#include "Tipography.h"

#include "ShapeUnit.h"
#include "DraftDraw.h"
#include "DraftDrawDoc.h"
#include "DraftDrawView.h"

//Support
#include "fileCore.h"

//Dialogs
#include "DialogGraphDen.h"

//Tools
#include "SelectionTool.h"
#include "PenTool.h"
#include "ConnectorTool.h"

#define TEST_PENTOOL
#define TEST_SELTOOL
//#define TEST_CONNTOOL

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Buffer2 is used for ersaing shape in its original position after droping in another place
#define DCABLE_BUFFER2_OFFSET_X		DCABLE_GRIDX_DEFAULT*3
#define DCABLE_BUFFER2_OFFSET_Y		DCABLE_GRIDY_DEFAULT*3

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView

IMPLEMENT_DYNCREATE(CDraftDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDraftDrawView, CScrollView)
	//{{AFX_MSG_MAP(CDraftDrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_SNAP_TOGRID, OnSnapTogrid)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_ESCAPE, OnEscape)
	ON_WM_TIMER()
	ON_COMMAND(ID_ORDER_BOTTOM, OnOrderBottom)
	ON_COMMAND(ID_ORDER_TOP, OnOrderTop)
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_SNAP_TOGRID2, OnSnapTogrid2)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_GRAPH_DEN, OnViewGraphDen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PART, OnUpdateBtEditPart)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RACK, OnUpdateBtEditRack)
	ON_NOTIFY(FTN_TABCHANGED, 1, OnChangedTab)
	//ON_COMMAND(ID_SELECT_NET, OnSelectNet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView construction/destruction

CDraftDrawView::CDraftDrawView()
{
	//Construction code here
	m_bFlagIniUpdate=FALSE;
	m_FlagBuffer1=false;
	m_bSaveDC=FALSE;
	m_RectDraw=CRect(0,0,0,0);
	m_RectDraw2=CRect(0,0,0,0);
	m_bmpCanvas1=new CBitmap();
	m_bmpCanvas2=new CBitmap();
	m_bForceTransfer=FALSE;
}

CDraftDrawView::~CDraftDrawView()
{
}

BOOL CDraftDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView drawing

void CDraftDrawView::OnDraw(CDC* pDC)
{
	//Local variables
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize m_szGrid=pDoc->m_szGrid;
	int buffer2_offset_x=0;
	int buffer2_offset_y=0;
	BOOL bFlagTransfer = FALSE;

	/*
	CRect rectUpdate;
	if (GetUpdateRect(NULL)){
		GetUpdateRect(rectUpdate);
		m_RectDraw.UnionRect(m_RectDraw, rectUpdate);
	}*/

	//04/06/2005
	//Code to save drawing in a file.
	CDC *pDCMemory=NULL;
	HBITMAP hOldBitmapMem;
	if(m_bSaveDC){

		//Create a compatible Device Context
		pDCMemory=new CDC();
		pDCMemory->CreateCompatibleDC(pDC);

		//Create a bitmap compatible with the screen DC
		HBITMAP hBitmap = CreateCompatibleBitmap(pDC->m_hDC, pDoc->GetSize().cx, pDoc->GetSize().cy);

		//Select new bitmap into memory DC
		hOldBitmapMem = (HBITMAP)SelectObject(pDCMemory->m_hDC, hBitmap);

		pDC=pDCMemory;

		pDC->FillRect(CRect(CPoint(0,0),pDoc->GetSize()),&CBrush(RGB(255,255,255)));
	}

	///////////////////////////////////////////////////////////////////
	//27/03/2005
	//Code to test rotations.
	//This code works properly.
	//Transformations can be used to: rotate, scale, reflect, shear (deform throw an axis).
	//The idea is to change transformation for each shape being drawn according
	//with its parameters.

	XFORM xForm;

	//Comented becouse produces strange behaviour of grid when zoom x 2
	/*
	SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	*/		

	///////////////////////////////////////////////////////////////////

	//Prepare buffer for drawing in
	PrepareBuffer(pDoc, pDC);

	//CListCtrl& refCtrl = GetListCtrl();
	//refCtrl.InsertItem(0, _T("Item!"));


	//MAP MODE
	int prev_mapm=pDC->SetMapMode(MM_ANISOTROPIC);

	//Window and viewport control
	if(!IsPrinting(pDC)){
		pDC->SetWindowExt(m_szWext);
		pDC->SetViewportExt(m_szVPext);
	}
	else{
		pDC->SetWindowExt(CSize(1,1));
		pDC->SetViewportExt(CSize(1,1));

		//Scale:
		//ixd,iyd are the equivalent in window pixels for 1 inch in printer
		//Since we are using logical units of 0.2 mm, ixd*2 must be =254 and
		//iyd*2 must be =254
		//then the scaling is (ixd/ixp)*(ixd*2/254)=2*xp/254 and
		//(iyd/iyp)*(iyd*2/254)=2*yp/254
		//(ixd/ixp) --> calculates the ratio screen resolution to printer resolution
		//(ixd*2/254) --> calculates the ratio screen resolution to drawing logical units

        // get the printer's pixel resolution
        int ixp=pDC->GetDeviceCaps(LOGPIXELSX);
        int iyp=pDC->GetDeviceCaps(LOGPIXELSY);

        // get the current window's resolution
        CDC* pddc = GetDC();
        int ixd=pddc->GetDeviceCaps(LOGPIXELSX);
        int iyd=pddc->GetDeviceCaps(LOGPIXELSY);

        // scale rect2 up by the ratio of the resolutions
        int width  = MulDiv(2,ixp, 254);
        int height = MulDiv(2,iyp, 254);
		int scale=min(width,height);

		//standard case: drawing is zoomed in for printing
		if(scale>0){
			pDC->SetViewportExt(CSize(scale,scale));
		}
		//less frequent: drawing is zoomed out for printing
		else{
			width  = MulDiv(1,254,2*ixp);
			height = MulDiv(1,254,2*iyp);
			scale=max(width,height);
			pDC->SetWindowExt(CSize(scale,scale));
		}
	}

	//ERASE
	if(!m_RectDraw.IsRectEmpty()&&m_FlagBuffer1){

		//Erase
		pDC1->FillSolidRect(&CRect(CPoint(0,0),m_RectDraw.Size()),RGB(255,255,255));

		//For erasing shape in original location
		if(!m_RectDraw2.IsRectEmpty()){
			CPoint p=CPoint(buffer2_offset_x,buffer2_offset_y);
			pDC2->FillSolidRect(&CRect(p,m_RectDraw2.Size()),RGB(255,255,255));
		}
	}

	//GRID
	int origin_x;
	int limit_x;
	int origin_y;
	int limit_y;

	if((m_bGrid)&&(m_bGridTemp)&&(!IsPrinting(pDC)) ){

		CSize szDesign=pDoc->GetSize();

		CPoint pscrollpos=GetScrollPosition( );
		CRect rect;
		GetClientRect(&rect);


//#define _DRAFTCABLE_DRAW_BORDER_CLIENT
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
		origin_x=pscrollpos.x*m_xScale;
		limit_x=(pscrollpos.x+rect.BottomRight().x)*m_xScale;
		origin_y=pscrollpos.y*m_yScale;
		limit_y=(pscrollpos.y+rect.BottomRight().y)*m_yScale;
#else
		origin_x=DCABLE_GRIDX_DEFAULT;
		limit_x=szDesign.cx;//+1;
		origin_y=DCABLE_GRIDX_DEFAULT;
		limit_y=szDesign.cy;//+1;
#endif


		if(limit_x>(szDesign.cx-DCABLE_GRIDX_DEFAULT * 1.5)){
			limit_x=szDesign.cx-DCABLE_GRIDX_DEFAULT * 1.5;
		}

		if(limit_y>(szDesign.cy-DCABLE_GRIDY_DEFAULT * 1.5)){
			limit_y=szDesign.cy-DCABLE_GRIDY_DEFAULT * 1.5;
		}

		//Full repaint
		CPoint ptOrg = Snap(CPoint(origin_x, origin_y));
		/*
		CPoint ptLimit=Snap(CPoint(limit_x,limit_y));
		*/
		CPoint ptLimit = CPoint(limit_x, limit_y);

		CRect rect_tot = CRect(ptOrg, ptLimit);
		if(m_RectDraw.IsRectNull()){

			DrawGrid(pDC, &rect_tot, FALSE);
		}
		else{

			//GRID: PARTIAL REDRAW
			//Uncoment to debug update rectangle
			//pDC->FrameRect(m_RectDraw,&CBrush(RGB(0,0,0)));

			CRect rect;// = CRect(CPoint(0, 0), m_RectDraw.Size());

			rect.IntersectRect(m_RectDraw, rect_tot);

			//Move rectangle to origen for buffer draw
			rect -= rect.TopLeft();

			//DrawGrid(pDC,&m_RectDraw);
			DrawGrid(pDC1,&rect,TRUE);

			if(!m_RectDraw2.IsRectEmpty()){
				CPoint p=CPoint(buffer2_offset_x,buffer2_offset_y);
				CRect rect2=CRect(p, m_RectDraw2.Size());

				//DrawGrid(pDC1,&rect2,TRUE);
				DrawGrid(pDC2,&rect2,TRUE);
			}

			//Set flag to indicate draw in buffer
			bFlagTransfer = TRUE;

		}//end if m_RectDraw null
	}//end if grid

	////////////////////////////////////////////////////////////////////////////
	//
	// 1ST LOOP
	//
	// 1. Draw not selected shapes outside update area.
	// 2. Draw not selected shapes inside update area. Draw them in secondary buffer.

	int index;

	//Prepare interaction
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//Prepare pen for drawing
	int p=min(max(m_szVPext.cx,m_szVPext.cy),MAX_NUM_PEN_IN_VIEW)-1;
	CPen *prev_pen=pDC->SelectObject(&arrPen[p]);

	//Loop
	//while(false){
	while(pSh!=NULL){

		//Local variables
		BOOL bFlagModeChanged=FALSE;
		int mode_prev;
		CRect rect_prev;

		//11/05/2005
		//Rotate.
		DoWorldTransform(pSh,pDC->m_hDC);

		//07/12/2004
		//Check if shape allows partial redraw (like polylines).
		//If so, keep mode & rectangle.
		//Then calculate temporal rectangle and change mode.
		if(pSh->m_Mode == _DRAFTDRAW_MODE_SEL &&
			pSh->m_TypeSelect == _DRAFTDRAW_SEL_SIZING_RECT &&
			pSh->m_bAllowsPartialDraw){
			//Keep mode & rectangle
			mode_prev=pSh->m_Mode;
			rect_prev=pSh->m_Rect;

			//Update rectangle and mode (is for drawing the part of a polyline is not under selection: i.e. allready drawn)
			CRect rect;
			pSh->GetRectTemp(rect);
			pSh->m_Rect=rect;
			pSh->m_Mode=_DRAFTDRAW_MODE_DRW;
			bFlagModeChanged=TRUE;
		}

		//Check if the shape is not selected
		if(pSh->m_Mode != _DRAFTDRAW_MODE_SEL){

			//if pSh->m_Rect intersect m_RectDraw it has to been drawn in pDC1
			CRect rect1,rectInter,rectInter1;

			//26/01/2005
			//To decide if a shape should be redrawn we inflate redraw rectangle by a unit:
			//there are line at the border of the unit that will be erased when moving
			//a shape over.
			rect1=m_RectDraw;
			rect1.InflateRect(DCABLE_GRIDX_DEFAULT*3,DCABLE_GRIDY_DEFAULT*3,DCABLE_GRIDX_DEFAULT*3,DCABLE_GRIDY_DEFAULT*3);//MAGIC NUMBER!

			//Intersection rectangle to decide if a shape must be redrawn.
			rectInter.IntersectRect(rect1,pSh->m_Rect);

			//Inflate rectangle for lines, wires, buses, etc.
			//this piece of code is executed also if rectangle has negative with/height, is it correct?
			if(pSh->m_Rect.IsRectEmpty()){
				CRect rect=pSh->m_Rect;
				rect.NormalizeRect();
				rect.InflateRect(1,1,1,1);
				rectInter1.IntersectRect(rect1,rect);
			}

			//Normal draw
			//if(rectInter.IsRectEmpty()){
			if(m_RectDraw.IsRectEmpty()){

				//07/12/2004
				//This is not very clean!
				//Rectangle must be set now to 0 because in normal draw points of polyline
				//are added an offset equal to shape m_Rect.
				//Polyline points already have offset becouse polyline points are normalized
				//after polyline drawing has ended.
				if(bFlagModeChanged){
					pSh->m_Rect=CRect(0,0,0,0);
				}

				//Draw the shape
				pSh->OnDraw(pDC);
			}
			//Draw in buffer
			else if(//Normal shapes
					(!rectInter.IsRectEmpty()&&!pSh->m_Rect.IsRectEmpty())||
					//Lines, wires, etc
					(!rectInter1.IsRectEmpty()&&pSh->m_Rect.IsRectEmpty())){

				//Store bounding rectangle without normalization
				CRect rect_abs=pSh->m_Rect;

				//Set shape relative to m_RectDraw
				CPoint ptXYRelative=CPoint(pSh->m_Rect.TopLeft()-m_RectDraw.TopLeft());
				pSh->m_Rect=CRect(ptXYRelative,pSh->m_Rect.Size());

				//Change marker array
				int i=0;
				while(pSh->m_MarkerArray[i]){

					//Offset sizing handle
					ptXYRelative=CPoint(pSh->m_MarkerArray[i]->TopLeft()-m_RectDraw.TopLeft());
					*pSh->m_MarkerArray[i]=CRect(ptXYRelative,pSh->m_MarkerArray[i]->Size());

					//Increment index
					i++;
				}

				//07/12/2004
				//This is not very clean!
				//Rectangle must be offset by -m_Rect.TopLeft() because in normal draw points of polyline
				//are added an offset equal to shape m_Rect.
				//Polyline points already have offset becouse polyline points are normalized
				//after polyline drawing has ended.
				if(bFlagModeChanged){
					pSh->m_Rect-=rect_abs.TopLeft();
				}

				//Draw shape in buffer
				pSh->OnDraw(pDC1);

				//Restore previous values
				pSh->m_Rect=rect_abs;
				//Restore marker array
				i=0;
				while(pSh->m_MarkerArray[i]){

					//Offset sizing handle
					ptXYRelative=CPoint(pSh->m_MarkerArray[i]->TopLeft()+m_RectDraw.TopLeft());
					*pSh->m_MarkerArray[i]=CRect(ptXYRelative,pSh->m_MarkerArray[i]->Size());

					//Increment index
					i++;
				}

				//Set flag to indicate draw in buffer
				bFlagTransfer=TRUE;
			}

			//Check for redrawing of not selected shape in original position of shape moved
			if( (!m_RectDraw2.IsRectEmpty())  ){

				//Temporal rectangle
				rect1=m_RectDraw2;

				//Intersection rectangle to decide if a shape must be redrawn.
				rectInter.IntersectRect(rect1,pSh->m_Rect);

				//Inflate rectangle for lines, wires, buses, etc.
				//this piece of code is executed also if rectangle has negative with/height, is it correct?
				if(pSh->m_Rect.IsRectEmpty()){
					CRect rect=pSh->m_Rect;
					rect.NormalizeRect();
					rect.InflateRect(1,1,1,1);
					rectInter1.IntersectRect(rect1,rect);
				}

				//Normal draw
				if((rectInter.IsRectEmpty()&&!pSh->m_Rect.IsRectEmpty())||
					(rectInter1.IsRectEmpty()&&pSh->m_Rect.IsRectEmpty())){

				}
				else
				{
					//Change coor. relative to m_RectDraw2
					CRect rect_abs=pSh->m_Rect;
					CPoint ptXYRelative=CPoint(pSh->m_Rect.TopLeft()-m_RectDraw2.TopLeft());
					ptXYRelative.x+=buffer2_offset_x;
					ptXYRelative.y+=buffer2_offset_y;
					pSh->m_Rect=CRect(ptXYRelative,pSh->m_Rect.Size());

					//Change marker array
					int i=0;
					while(pSh->m_MarkerArray[i]){

						//Offset sizing handle
						ptXYRelative=CPoint(pSh->m_MarkerArray[i]->TopLeft()-m_RectDraw2.TopLeft());
						ptXYRelative.x+=buffer2_offset_x;
						ptXYRelative.y+=buffer2_offset_y;
						*pSh->m_MarkerArray[i]=CRect(ptXYRelative,pSh->m_MarkerArray[i]->Size());

						//Increment index
						i++;
					}

					//Draw shape in buffer
					//pSh->OnDraw(pDC1);
					pSh->OnDraw(pDC2);

					//Restore previous values
					pSh->m_Rect=rect_abs;
					//Restore marker array
					i=0;
					while(pSh->m_MarkerArray[i]){

						//Offset sizing handle
						ptXYRelative=CPoint(pSh->m_MarkerArray[i]->TopLeft()+m_RectDraw2.TopLeft());
						ptXYRelative.x-=buffer2_offset_x;
						ptXYRelative.y-=buffer2_offset_y;
						*pSh->m_MarkerArray[i]=CRect(ptXYRelative,pSh->m_MarkerArray[i]->Size());

						//Increment index
						i++;
					}

				}

			}//end if m_RectDraw2 is not empty
		}

		//Restore values
		if(bFlagModeChanged){
			pSh->m_Rect=rect_prev;
			pSh->m_Mode=mode_prev;
			bFlagModeChanged=FALSE;
		}

		pSh=(CShape *)pDoc->NextObject(index);
	}//end while

	pDC->SelectObject(prev_pen);

	////////////////////////////////////////////////////////////////////////////
	//
	// 2ND LOOP
	//
	// 1. Draw selected shapes inside update area. Draw them in secondary buffer.

	pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//while(false){
	while(pSh!=NULL){

		pSh->SetOrg(CPoint(0,0));
		pSh->SetProyect(0,0,1);

		//If shape is not selected continue to next shape
		if(pSh->m_Mode){
			pSh=(CShape *)pDoc->NextObject(index);
			continue;
		}

		int x=pSh->m_Rect.TopLeft().x;
		int y=pSh->m_Rect.TopLeft().y;
		//Calculate shape update rect offset
		CPoint ptXYRelative=CPoint(pSh->m_Rect.TopLeft()-m_RectDraw.TopLeft());

		BOOL FlagLoadBuffer1=TRUE;//FALSE;

		if(FlagLoadBuffer1){

			//15/05/2005
			pSh->m_RectAux=pSh->m_Rect;
			pSh->m_RectLastAux=pSh->m_RectLast;

			CRect rect_abs=pSh->m_Rect;

			pSh->m_Rect=CRect(ptXYRelative,pSh->m_Rect.Size());

			//Store infomation last rectangle
			int x_last=pSh->m_RectLast.TopLeft().x;
			int y_last=pSh->m_RectLast.TopLeft().y;
			CPoint ptXYRelativeLast=CPoint(pSh->m_RectLast.TopLeft()-m_RectDraw.TopLeft());
			pSh->m_RectLast=CRect(ptXYRelativeLast,pSh->m_RectLast.Size());

			//Draw shape
			pSh->OnDraw(pDC1);

			//Restore previous values
			//pSh->m_Rect=rect_abs;
			//Above line is commented and changed by below line becouse some shapes change rectangle size first time
			//they are drawn
			pSh->m_Rect=CRect(rect_abs.TopLeft(),pSh->m_Rect.Size());

			pSh->m_RectLast=CRect(CPoint(x_last,y_last),pSh->m_RectLast.Size());

			//It is for frame erasing it is not used
			m_PointPrev=CPoint(-1,-1);
			//m_PointPrev=CPoint(x,y);
		}// end if FlagLoadBuffer1

				

		//Copy from buffer
		if(m_FlagBuffer1==true){//ver

			DWORD dwRop=MERGEPAINT;

			//Calculate erase region
			//Frame erasing causes less blinking
			//==========================================================
//#define DCABLE_FRAME_ERASING_ENABLE
#ifdef DCABLE_FRAME_ERASING_ENABLE
			int result;

			if((m_PointPrev.x>=0)&&(m_PointPrev!=CPoint(x,y))){

				CRect rect_prev=CRect(m_PointPrev,pSh->m_Rect.Size());
				CRect rect_union;
				rect_union.UnionRect(rect_prev,pSh->m_Rect);
				CRect rect_erase;

				if(rect_union.top<pSh->m_Rect.top){
					rect_erase=CRect(rect_union.left,rect_union.top,rect_union.right,pSh->m_Rect.top);

							result=pDC->BitBlt(
								m_PointPrev.x,//destination
								m_PointPrev.y,
								pSh->m_Rect.Width(),//rect_erase.Width(),//pSh->m_Rect.Width(),
								pSh->m_Rect.Height(),//rect_erase.Height(),//pSh->m_Rect.Height(),
								pDC1,
								0,//source
								0,
								dwRop  );

						}

						if(rect_union.bottom>pSh->m_Rect.bottom){
							rect_erase=CRect(rect_union.left,pSh->m_Rect.bottom,rect_union.right,rect_union.bottom);

							result=pDC->BitBlt(
								m_PointPrev.x,//destination
								m_PointPrev.y,
								pSh->m_Rect.Width(),//rect_erase.Width(),//pSh->m_Rect.Width(),
								pSh->m_Rect.Height(),//rect_erase.Height(),//pSh->m_Rect.Height(),
								pDC1,
								0,//source
								0,
								dwRop  );

						}

						if(rect_union.left<pSh->m_Rect.left){
							rect_erase=CRect(rect_union.left,pSh->m_Rect.top,pSh->m_Rect.left,pSh->m_Rect.bottom);

							result=pDC->BitBlt(
								m_PointPrev.x,//destination
								m_PointPrev.y,
								pSh->m_Rect.Width(),//rect_erase.Width(),//pSh->m_Rect.Width(),
								pSh->m_Rect.Height(),//rect_erase.Height(),//pSh->m_Rect.Height(),
								pDC1,
								0,//source
								0,
								dwRop  );

						}

						if(rect_union.right>pSh->m_Rect.right){
							rect_erase=CRect(pSh->m_Rect.right,pSh->m_Rect.top,rect_union.right,pSh->m_Rect.bottom);

							result=pDC->BitBlt(
								m_PointPrev.x,//destination
								m_PointPrev.y,
								pSh->m_Rect.Width(),//rect_erase.Width(),//pSh->m_Rect.Width(),
								pSh->m_Rect.Height(),//rect_erase.Height(),//pSh->m_Rect.Height(),
								pDC1,
								0,//source
								0,
								dwRop  );

						}

					}
#endif
					//==========================================================

					bFlagTransfer=TRUE;

					m_PointPrev=CPoint(x,y);
				}

				pSh=(CShape *)pDoc->NextObject(index);

				continue;

		//======================================================================

		pSh->OnDraw(pDC);

		//Code to draw front projection of shapes
		/*
		if(pSh->m_Type == _DRAFTDRAW_ELEVATION_DRAW_ENABLE){

			pSh->SetOrg(CPoint(0,m_SeparatorHeight+100));
			pSh->SetProyect(0,1,0);

			pSh->OnDraw(pDC);
		}
		*/

		//Next shape
		pSh=(CShape *)pDoc->NextObject(index);

	}//end while second loop


	//TRANSFER FROM BUFFER
	if(bFlagTransfer || m_bForceTransfer){
		m_bForceTransfer=FALSE;
		//Operation to perform
		DWORD dwRop;
		//dwRop=SRCAND;//OR!
		dwRop=SRCCOPY;//Move buffer to screen, overwrite.
		//dwRop=MERGEPAINT;

		//Second transfer to erase shape in original position
		if(!m_RectDraw2.IsRectEmpty()){
			pDC->BitBlt(
				m_RectDraw2.TopLeft().x,//destination
				m_RectDraw2.TopLeft().y,
				m_RectDraw2.Width(),
				m_RectDraw2.Height(),
				pDC2,
				buffer2_offset_x,//source
				buffer2_offset_y,
				dwRop
				);
		}//end if m_RectDraw2 is empty

		//First transfer
		int result = pDC->BitBlt(
			m_RectDraw.TopLeft().x,//destination
			m_RectDraw.TopLeft().y,
			m_RectDraw.Width(),
			m_RectDraw.Height(),
			pDC1,
			0,//source
			0,
			dwRop
			);

	}//end if bFlagTransfer


	//RESTORE TRANSFORM
	xForm.eM11 = (FLOAT) 1.0; 
	xForm.eM12 = (FLOAT) 0.0; 
	xForm.eM21 = (FLOAT) 0.0; 
	xForm.eM22 = (FLOAT) 1.0; 
	xForm.eDx  = (FLOAT) 0.0; 
	xForm.eDy  = (FLOAT) 0.0; 
	SetWorldTransform(pDC->m_hDC, &xForm); 

	//SEPARATOR

#ifdef _DRAWSEPARATOR_DRAFTCABLE
	CPen *prev_pen=pDC->SelectObject(&PenThick);
	
	CRect rect;
		
	GetClientRect(&rect);

	pDC->MoveTo(rect.TopLeft().x,m_SeparatorHeight);
	pDC->LineTo(rect.BottomRight().x,m_SeparatorHeight);

	pDC->SelectObject(prev_pen);
#endif

	//BORDER
	/*
	xForm.eM11 = (FLOAT) 1.0; 
	xForm.eM12 = (FLOAT) 0.0; 
	xForm.eM21 = (FLOAT) 0.0; 
	xForm.eM22 = (FLOAT) 1.0; 
	xForm.eDx  = (FLOAT) 0.0; 
	xForm.eDy  = (FLOAT) 0.0; 
	SetWorldTransform(pDC->m_hDC, &xForm);
	*/
	DrawBorder(pDC);

	//04/06/2005
	//Save drawing in a file.
	if(m_bSaveDC){

		//HBITMAP hBitmap=CopyDocToBitmap(CRect(0,0,1500,500),pDC->m_hDC );
		HBITMAP hBitmap = (HBITMAP)SelectObject(pDC->m_hDC, hOldBitmapMem);
		//HBITMAP hBitmap=(HBITMAP)pDC->GetCurrentBitmap()->m_hObject;

		HPALETTE hPal;
		if (hBitmap)
		hPal = GetSystemPalette();
		else
		hPal = NULL;

		//Commom save dialog
		static TCHAR BASED_CODE szFilter[] = _T("Windows bmp (*.bmp)|*.bmp|Gif (*.gif)|*.gif|All Files (*.*)|*.*||");

		CString strPath=pDoc->GetPathName();
		savebmp(hBitmap,hPal,pDoc->saCodecs[pDoc->nFilterIndex-1].c_str(),strPath);

		delete(pDC);

		m_bSaveDC=FALSE;
	}


	//Clear update rectangle
	m_RectDraw=CRect(0,0,0,0);
	m_RectDraw2=CRect(0,0,0,0);

	//End of OnDraw
}

void CDraftDrawView::OnInitialUpdate()
{
	//This flag is used becouse I don't why OnInitialUpdate function is called twice
	if(!m_bFlagIniUpdate){
		m_bFlagIniUpdate=TRUE;//Set to false so it is executed again

		//Call base class
		CScrollView::OnInitialUpdate();

		//Get a pointer to document class, just in case we need access some members
		CDraftDrawDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		//adjust scrolling characteristics
		SetScrollSizes(MM_TEXT,pDoc->GetSize(),
			CSize(SCROLL_SIZE_PAGE,SCROLL_SIZE_PAGE),
			CSize(SCROLL_SIZE_PAGE,SCROLL_SIZE_PAGE));
		m_ptScrollPrev=GetScrollPosition( );

		//Initialize grid flags
		m_bGrid=TRUE;//Draw grid or not flag
		m_bGridTemp=TRUE;//Used to disable grid temporally on scroll (is not used)

		//Initialize zoom status
		m_szWext=CSize(1,1);
		m_szVPext=CSize(1,1);
		m_xScale=1.0;
		m_yScale=1.0;

		//Calculate separator height. It is not currently used
		CRect rect;
		GetClientRect(&rect);
		m_SeparatorHeight=rect.CenterPoint().y;

		//Creates a thick pen (not currently used, was used for separator)
		//BOOL result=PenThick.CreatePen(PS_GEOMETRIC,4,RGB(0,0,255));

		//Its used to draw with a thicker pen when we increment zoom
		int i;
		int thick=1;
		for(i=0;i<MAX_NUM_PEN_IN_VIEW;i++){
			BOOL result=arrPen[i].CreatePen(PS_GEOMETRIC,thick,RGB(0,0,0));
			if(i>1){
				thick<<=1;
			}
		}

		//CFolderFrame::ShowControls lets you hide or show all the controls (folder tab and scroll bars).
		//It's handy for SDI apps where the frame starts out empty—that is, with no document/view. For DibView, this results in pDIB==NULL,
		//in which case CDIBView::OnInitialUpdate passes CFolderFrame::hide to hide the controls; otherwise it passes CFolderFrame::bestFit,
		//which instructs the folder frame to make the folder tab exactly as wide as necessary to display all the tabs,
		//and use the rest of the window width for the scroll bar. 
		/*
		GetFolderFrame()->ShowControls(pDIB ?
		   CFolderFrame::bestFit : CFolderFrame::hide);
		*/
		GetFolderFrame()->ShowControls(
		   CFolderFrame::bestFit);


	}
	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView printing

BOOL CDraftDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDraftDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDraftDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView diagnostics

#ifdef _DEBUG
void CDraftDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDraftDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDraftDrawDoc* CDraftDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDraftDrawDoc)));
	return (CDraftDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawView message handlers
void CDraftDrawView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CDraftDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Message handler follows
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect_union = CRect(0, 0, 0, 0);			//Rectangle for repainting

	//Get size of grid
	CSize m_szGrid = pDoc->m_szGrid;

	//Store hit point for pan tool
	m_PointLBD0 = point;

	//=======================================================
	//MAPPING ENGINE
	//Convert to a function
	//=======================================================

	//Scale point
	CPoint point_ns = point;
	ClientToSchematic(&point);

	//Snap to grid
	SnapToGrid(&point);
	//=======================================================

	//New state pattern tests
#ifdef TEST_SELTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CSelectionTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		CRect rect_union = ((CAbstractTool*)pDoc->m_tooSel)->MouseDown(nFlags, point);
		if (!rect_union.IsRectEmpty())
		{
			_SetUpdateRect(rect_union);
		}
		return;
	}
#endif
#ifdef TEST_PENTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CPenTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		((CAbstractTool*)pDoc->m_tooSel)->MouseDown(nFlags, point);
		return;
	}
#endif
#ifdef TEST_CONNTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CConnectorTool))) {
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		((CAbstractTool*)pDoc->m_tooSel)->MouseDown(point);
		return;
	}
#endif

	//=======================================================
	//INITIALIZE
	//=======================================================
	SetConnectPrev(NULL);
	//=======================================================

	//=======================================================
	//ZOOM CURSOR ENGINE
	//=======================================================
	if (pDoc->m_iToolSel == _TOOLSELECT_ZOOMPLUS){
		CPoint pscrollpos = point;
		DoZoomIn(point);
		return;
	}
	else if (pDoc->m_iToolSel == _TOOLSELECT_ZOOMMINUS){
		CPoint pscrollpos = point;
		DoZoomOut(point);
		return;
	}

	//=======================================================

	//=======================================================
	//PLACE IN CHAIN ENGINE
	//=======================================================
	if (pDoc->m_iToolType == _TOOLTYPECHAIN_DRAFTCABLE){

		//15/08/2004
		//This line has been commented becouse it caused a curious bug:
		//A 0 length wire was introduced at the end of each wire segment.
		//		OnLButtonUp(nFlags,point_ns);
		return;
	}

	//=======================================================

	//=======================================================
	//SELECTING-DESELECTING ENGINE
	//=======================================================
	if (pDoc->m_iToolSel == _TOOLPLACE_DRAFTCABLE ||
		pDoc->m_iToolSel == _TOOLTYPECHAININI_DRAFTCABLE){
		POSITION pos = m_ObListSel.GetHeadPosition();
		while (pos != NULL)
		{
			CShape* pSh = (CShape*)m_ObListSel.GetNext(pos);
			pSh->Unselect();
		}
	}

	//Flag for multiple select operation:
	//1.Normal: shape stack is walked from last to first
	//2.Inverse: when multiple select tool is selected stack is walked backwards
	int DRAFTCABLE_SELECT_INV = 0;// (pDoc->m_iToolSel == _TOOLSELECTMUL_DRAFTCABLE);
	//CShape *pSh=new CShape();
	//add a member function to perform this action
	//pSh->m_TypeSelect=pDoc->m_pSh->m_TypeSelect;
	int idata_deb = pDoc->m_pObArray->GetSize();
	bool flag_return = false;
	bool flag_no_deselect = false;
	bool flag_atleast_one_selected = false;
	int index;
	int index_keep = -1;
	//CShape *pShconn = NULL;
	//CShape *pShchild = NULL;
	CShape *pSh;
	CShape *pShSelected = NULL;
	if (DRAFTCABLE_SELECT_INV){
		pSh = (CShape *)pDoc->FirstObject(index);
		pSh = (CShape *)pDoc->NextObject(index);
	}
	else{
		pSh = (CShape *)pDoc->LastObject(index);
		pSh = (CShape *)pDoc->PrevObject(index);
	}

	//debug
	if (pDoc->m_iToolSel == _TOOLSELECTMUL_DRAFTCABLE){
		int _dummy = 1;
	}

	//1st LOOP
	//SELECTING/CONNECTING LOOP
	//Tasks:
	//1.Put shapes in selection state.
	//Filters:
	//_TOOLSELECT_DRAFTCABLE-->To select shapes
	//19/01/2005: new connecting mechanism
	BOOL bConnectionTmp = FALSE;//set when a connection is pending
	CShapeContainer *pShContConnect = NULL;
	//=======================================================
	if ((pDoc->m_iToolSel == _TOOLSELECT_DRAFTCABLE) ||
		(pDoc->m_iToolSel == _TOOLSELECTMUL_DRAFTCABLE) ||
		(pDoc->m_iToolSel == _TOOLPLACE_DRAFTCABLE))

		while (pSh){

			int TypeSelectLast = pSh->m_TypeSelect;

			//To connect
			if (pDoc->m_iToolSel == _TOOLPLACE_DRAFTCABLE &&
				pDoc->m_iToolType == _TOOLTYPECHAININI_DRAFTCABLE){
				//bConnectionTmp =_DoGetConnectionTmp(&point, &pShContConnect, pSh, bConnectionTmp)
				if ((pSh->GetRuntimeClass())->IsDerivedFrom(RUNTIME_CLASS(CShapeContainer))){

					CShapeContainer *pShContainer = (CShapeContainer *)pSh;
					pShContainer->m_bConnectMake = TRUE;

					//19/01/2005: new connecting mechanism
					if (!bConnectionTmp)
						bConnectionTmp = pShContainer->PtInRect(&point, &pShContConnect);
				}
			}

			//To select
			if (pDoc->m_iToolSel != _TOOLPLACE_DRAFTCABLE
				){

				pSh->OnLButtonDown(nFlags, point);
				
				if (pSh->m_Mode == _DRAFTDRAW_MODE_SEL){

					POSITION pos = m_ObListSel.Find(pSh);
					if (pos == NULL)
						//m_ObListSel.AddTail(pSh);
						int _dummy = 1;
					else
						flag_no_deselect = true;

					pShSelected = pSh;
					flag_atleast_one_selected = true;
					flag_return = true;

					break;
				}

			}

			if (DRAFTCABLE_SELECT_INV){
				pSh = (CShape *)pDoc->NextObject(index);
			}
			else{
				pSh = (CShape *)pDoc->PrevObject(index);
			}
		}//end while
	//=======================================================

	//2nd LOOP
	//DESELECTING LOOP
	//Tasks:
	//1.Put shapes beeing deselected in normal draw.
	//=======================================================

	//New de-selecting mechanism
	if ((pDoc->m_iToolSel == _TOOLSELECT_DRAFTCABLE && flag_no_deselect==false)
		|| pDoc->m_iToolSel == _TOOLPLACE_DRAFTCABLE
		//If we are in multiple selection mode
		//&& pDoc->m_iToolSel != _TOOLSELECTMUL_DRAFTCABLE
		|| (pDoc->m_iToolSel == _TOOLSELECTMUL_SELECTED_DRAFTCABLE && pShSelected==NULL)
		){
		POSITION pos = m_ObListSel.GetHeadPosition();
		while (pos != NULL)
		{
			CShape* pSh = (CShape*)m_ObListSel.GetNext(pos);
			if (pSh != pShSelected){
				int _dummy = 0;
				pSh->Unselect();
				//flag_return=true;
				//Inflate update rectangle
				CRect rect = pSh->m_Rect;
				rect.InflateRect(DCABLE_GRIDX_DEFAULT * 4, DCABLE_GRIDY_DEFAULT * 4);//MAGIC NUMBER

				//Intersect with sheet
				//rect.IntersectRect(rect, rcSheet);

				//Add to update rectangle
				rect_union.UnionRect(rect_union, rect);
			}
		}

		m_ObListSel.RemoveAll();
	}

	if (pShSelected != NULL){
		POSITION pos = m_ObListSel.Find(pShSelected);
		if (pos == NULL)
			m_ObListSel.AddTail(pShSelected);
	}
	//=======================================================

	//STATUS MACHINE
	//=======================================================
	if(pDoc->m_iToolSel==_TOOLSELECT_DRAFTCABLE &&
		pShSelected == NULL){
		//-----------------------
		if(pDoc->m_pSh){
			if(pDoc->m_pSh != (CShape*)((*pDoc->m_pObArray)[pDoc->m_pObArray->GetSize()-1])){
				delete(pDoc->m_pSh);
			}
		}
		pDoc->m_pSh=new(CShapeRect);
		pDoc->m_pSh->m_pCursorArray=pDoc->m_CursorArray;
		//-----------------------
		pDoc->m_iToolSel=_TOOLSELECTMUL_DRAFTCABLE;
		m_RectMul=CRect(0,0,0,0);
		CShapeRect *pShRect=(CShapeRect *)pDoc->m_pSh;
		pShRect->m_bGroup=TRUE;
		pShRect->m_bDrawDashedRect = TRUE;
		pDoc->AddObject(pDoc->m_pSh);
		pDoc->m_pSh->OnLButtonDown(nFlags, point);
	}
	else if (pDoc->m_iToolSel == _TOOLPLACE_DRAFTCABLE){
		//Connect
		//19/01/2005: new connecting mechanism
		if(bConnectionTmp){
			if(pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
				CShapeContainer *pShCont1=(CShapeContainer *)pDoc->m_pSh;
				CShape *pSh=(*pShCont1)[0];
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

					CShapeContainer *pShCont2=(CShapeContainer *)pSh;
					pShCont1->LinkShapes(pShContConnect,pShCont2);

					//Store connection point to prevent releasing mouse button up in same point where pressed it
					SetConnectPrev(point);

					//Previous mechanism to 19/01/2005 used temporal pointers in m_pChildConn. Delete this line ASAP. 
					pShContConnect->GetParent()->m_pshChildConn=NULL;
				}
			}
		}

		//Add new shape
		_DoAddNewShapeToStack(nFlags, point);

	}
	//=======================================================

	//=======================================================
	//DRAWING ENGINE
	//=======================================================
	if (rect_union.IsRectEmpty()==FALSE){

		CRect rect = rect_union;

		//debug
		if (rect.IsRectEmpty()){
			int i = 0;
		}

		//Scale update rectangle to current coordinates
		CPoint point1 = CPoint(rect.TopLeft().x / m_xScale, rect.TopLeft().y / m_yScale) - GetScrollPosition();
		//CPoint point1=rect.TopLeft();
		CSize size = CSize(rect.Width() / m_xScale, rect.Height() / m_yScale);

		m_RectDraw = rect;
		InvalidateRect(CRect(point1, size), FALSE);
	}
	//=======================================================

	//Call base class
	CScrollView::OnLButtonDown(nFlags, point);
}

void CDraftDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//Local variable
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BOOL bRedrawAll = FALSE;

	//=======================================================
	//MAPPING ENGINE
	//=======================================================

	//Scale point
	CPoint point_ns=point;
	ClientToSchematic(&point);

	//Snap to grid
	SnapToGrid(&point);
	//=======================================================

#ifdef TEST_SELTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CSelectionTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		CRect rect_union = ((CAbstractTool*)pDoc->m_tooSel)->MouseUp(point);
		if (!rect_union.IsRectEmpty())
		{
			_SetUpdateRect(rect_union);
		}
		return;
	}
#endif
#ifdef TEST_PENTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CPenTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		((CAbstractTool*)pDoc->m_tooSel)->MouseUp(point);
		//return;
	}
#endif
#ifdef TEST_CONNTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CConnectorTool))) {
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		((CAbstractTool*)pDoc->m_tooSel)->MouseUp(point);
		return;
	}
#endif

	//Calculate sheet area
	//=======================================================
	CSize szDesign=pDoc->GetSize();
	CRect rcSheet=CRect(CPoint(0,0),szDesign);
	rcSheet.DeflateRect(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT);
	//=======================================================

	//Rectangle for repainting
	CRect rect_union=CRect(0,0,0,0);
	CRect rect_union2=CRect(0,0,0,0);

	//=======================================================
	//SELECTING-DESELECTING ENGINE
	//=======================================================

	//CONNECTING LOOP
	int index;
	CShape *pShconn=NULL;
	CShape *pShchild=NULL;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	int indexPin = SHAPECONTAINER_CONNECTING_PIN_WIRE;

	//19/01/2005: New connecting mechanism
	BOOL bConnectionTmp=FALSE;//set when a connection is pending
	CShapeContainer *pShContConnect=NULL;

	//STATUS MACHINE CONNECT
	//Execution end here if we are creating a new wire left click in the unit pin and release the button in the same point
	if(pDoc->m_iToolSel==_TOOLPLACE_DRAFTCABLE && pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeWire)))
		//Loop
		while(index<pDoc->m_pObArray->GetSize()){

		//Care should be taken not to select unselected shapes
		int ModeLast=pSh->m_Mode;
		int TypeLast=pSh->m_TypeSelect;

		//Do prepare for connecting
		if (_DoGetConnectionTmp(pSh, point, nFlags,
			bConnectionTmp,
			indexPin,
			&pShContConnect
			) == TRUE){
			if (pDoc->m_iToolType == _TOOLTYPECHAININI_DRAFTCABLE){
				pDoc->m_iToolType = _TOOLTYPECHAIN_DRAFTCABLE;
			}
			//If the private member return TRUE we return now
			return;
		}

		//Next shape
		pSh=(CShape *)pDoc->NextObject(index);
	}//end while and if (...) IsKindOf(RUNTIME_CLASS(CShapeWire)) (...)

	//STATUS MACHINE SELECT
	if(pDoc->m_iToolSel==_TOOLPLACE_DRAFTCABLE
		|| pDoc->m_iToolSel==_TOOLSELECT_DRAFTCABLE
		|| pDoc->m_iToolSel==_TOOLROTATE_DRAFTCABLE
		){
		pSh=(CShape *)pDoc->FirstObject(index);
		pSh=(CShape *)pDoc->NextObject(index);
		CShape *pShSel=NULL;
		while(pSh){
			if(pSh->IsSelected()){
				pShSel=pSh;
				//Keep former position rectangle to erase
				CRect rect2;
				rect2=pSh->m_RectLast;
				rect2.InflateRect(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT);
				rect_union2.UnionRect(rect_union2,rect2);
				//Pass event to shape
				pSh->OnLButtonUp(nFlags, point);
				//Current position to erase
				CRect rect;
				pSh->GetSizingRect(rect);
				bRedrawAll |= _DoAddToUpdateRect(rect_union, rect);
			}
			pSh=(CShape *)pDoc->NextObject(index);
		}
		pSh=pShSel;
	}
	//Disable rect for multiple selections
	else if ((pDoc->m_iToolSel == _TOOLSELECTMUL_DRAFTCABLE)
		&& (m_RectMul.IsRectEmpty())
		){

		int sizeObArray = pDoc->m_pObArray->GetSize();

		pSh->OnLButtonDown(nFlags, pDoc->m_pSh->m_Rect);
		//Add to selected array
		_AddToSelectedArray(pSh);

		//Iterate shapes
		while (index<sizeObArray - 1){
			pSh = (CShape *)pDoc->NextObject(index);
			pSh->OnLButtonDown(nFlags, pDoc->m_pSh->m_Rect);
			//Add to selected array
			_AddToSelectedArray(pSh);
		}
		pSh = (CShape *)pDoc->NextObject(index);

		pSh->OnLButtonUp(nFlags, point);
		pSh->OnLButtonDown(nFlags, pSh->m_Rect);

		m_RectMul = pSh->m_Rect;

#ifdef DRAFTDRAW_RECTMUL_DELETE
		pDoc->DeleteObject(-1);
		pDoc->m_iToolSel = _TOOLSELECT_DRAFTCABLE;
#endif

		if (!m_RectMul.IsRectEmpty()){
			m_RectDraw = m_RectMul;
			m_RectDraw.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
			InvalidateRect(m_RectDraw, FALSE);
		}

		return;
	}

	//Connection pin index for shape under selection is 1 for wires
	BOOL bFlagWireEnd=FALSE;
	//Connect
	//19/01/2005: new connecting mechanism
	if(bConnectionTmp){
		if(pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
			CShapeContainer *pShCont1=(CShapeContainer *)pDoc->m_pSh;
			CShape *pSh=(*pShCont1)[indexPin];
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

				CShapeContainer *pShCont2=(CShapeContainer *)pSh;
				pShCont1->LinkShapes(pShContConnect,pShCont2);

				//pDoc->m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
				pDoc->m_iToolType=_TOOLTYPECHAININI_DRAFTCABLE;
				bFlagWireEnd=TRUE;

				//Previous mechanism to 19/01/2005 used temporal pointers in m_pChildConn. Delete this line ASAP. 
				pShContConnect->GetParent()->m_pshChildConn=NULL;
			}
		}
	}

	//=======================================================
	//PLACE IN CHAIN ENGINE
	//=======================================================
	if(pDoc->m_iToolSel==_TOOLPLACE_DRAFTCABLE){

		if((pDoc->m_iToolType==_TOOLTYPECHAININI_DRAFTCABLE)||
			(pDoc->m_iToolType==_TOOLTYPECHAIN_DRAFTCABLE)){

			pSh=(CShape *)pDoc->LastObject(index);
			//Create a new shape
			pDoc->m_pSh=new CShapeWire(NULL,0,pDoc->cmdDeque);
			pDoc->m_pSh->m_pCursorArray=pDoc->m_CursorArray;

			//Connect
			if(!bFlagWireEnd){

				pDoc->AddObject(pDoc->m_pSh);
				pDoc->m_iToolType=_TOOLTYPECHAIN_DRAFTCABLE;

				pShconn=NULL;
			
				int idata1=pSh->m_Mode;
				int idata2=pSh->m_TypeSelect;
				//15/08/2004
				//Added to prevent pines get connected on selections.
				//---------------------------------------------------
				if( (pSh->GetRuntimeClass())->IsDerivedFrom(RUNTIME_CLASS(CShapeContainer))   ){

					CShapeContainer *pShContainer=(CShapeContainer *)pSh;
					pShContainer->m_bConnectMake=TRUE;
				}
				//---------------------------------------------------

				//19/01/2005: new connecting mechanism
				//connection pin index for shape under selection is 1 for wires
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){
					CShapeContainer *pShContainer=(CShapeContainer *)pSh;

					bConnectionTmp=pShContainer->PtInRect(&point,&pShContConnect);
					if(bConnectionTmp){
						indexPin = SHAPECONTAINER_CONNECTING_PIN_UNIT;
					}
				}

				//Connect
				if(bConnectionTmp){
					if(pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
						CShapeContainer *pShCont1=(CShapeContainer *)pDoc->m_pSh;
						CShape *pSh=(*pShCont1)[indexPin];
						if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

							CShapeContainer *pShCont2=(CShapeContainer *)pSh;
							pShCont1->LinkShapes(pShContConnect,pShCont2);

							//pDoc->m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
							//pDoc->m_iToolType=_TOOLTYPECHAININI_DRAFTCABLE;
							//bFlagWireEnd=TRUE;

							//Previous mechanism to 19/01/2005 used temporal pointers in m_pChildConn. Delete this line ASAP. 
							pShContConnect->GetParent()->m_pshChildConn=NULL;
						}
					}
				}

				//Set new shape in sizing mode
				pDoc->m_pSh->OnLButtonDown(nFlags, point);
			}
			else{
				pDoc->m_iToolType=_TOOLTYPECHAININI_DRAFTCABLE;
			}
			//}//end point!=ptConnectPrev
		}//end if _TOOLTYPECHAININI_DRAFTCABLE
		else if(pDoc->m_iToolType==_TOOLTYPEPOLY_DRAFTCABLE){

			//Current position to erase
			CRect rect;
			pSh->GetSizingRect(rect);
			pSh->GetRectUpdatePlace(rect);
			bRedrawAll |= _DoAddToUpdateRect(rect_union, rect);

			//Pass event to shape
			/*pDoc->m_pSh->OnLButtonDown(nFlags, point);*/
		}
		//REPEAT PREVIOUS PLACEMENT
		else if(pDoc->m_iToolType==_TOOLTYPENORMAL_DRAFTCABLE){
			pSh=(CShape *)pDoc->LastObject(index);

			//Zoom area control engine is placed here for convenience
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeZoomRect))){

				//Calculate first values
				CSize szViewport=CSize(pSh->m_Rect.Width(),pSh->m_Rect.Height());//szDesign asigned to zoom rectangle
				CRect rectWindow;
				GetClientRect(&rectWindow);
				m_szWext=CSize(1,1);//default value

				//Calculate
				_DoCalculateWindowViewPortExtend(szViewport, rectWindow);

				//Execute zoom
				_DoZoom(pSh);

				//Delete zoom shape and set tool select automatically
				pDoc->DeleteObject(-1);
				pDoc->m_iToolSel=_TOOLSELECT_DRAFTCABLE;

				bRedrawAll = TRUE;
			}
			//Repeat placement engine
			else{

				//Create a new shape
				CRuntimeClass* pRuntimeClass=pSh->GetRuntimeClass( );

				CString sName=CString(pRuntimeClass->m_lpszClassName);
				if(sName.Compare(_T("CShape"))&&sName.Compare(_T("CShapeContainer"))){

					//copy & increment
					CShape* pSh1 = (CShape*)pRuntimeClass->CreateObject();
					*pSh1=*pSh;
					//(*pSh1)++;
					++(*pSh1);

					//Beware! This mechanism is not very good. It would be better a suitable '=' operator.
					pSh1->pcmdDeque=pDoc->cmdDeque;

					pDoc->m_pSh=pSh1;
					pDoc->m_pSh->m_pCursorArray=pDoc->m_CursorArray;

					if( (pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit)))||
						(pSh->IsKindOf(RUNTIME_CLASS(CShapePin)))||
						(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel)))
						){
						
						//CShape* pShDebug=(CShape*)((CShapeUnit*)pSh)->m_obarrShapearr[0];

						if(pDoc->m_pSh->m_bNoResize){
							pDoc->AddObject(pDoc->m_pSh);
							pDoc->m_pSh->OnLButtonDown(0, point);
						}

						//pShDebug=(CShape*)((CShapeUnit*)pSh)->m_obarrShapearr[0];
						//pDoc->m_pSh->m_Rect+=point;
					}

					::SetCursor(pDoc->m_CursorArray[CURSOR_DRAW]);
				}
			}
		}/* end _TOOLTYPENORMAL_DRAFTCABLE*/
	}

	//=======================================================
	//DRAWING ENGINE
	//=======================================================

	//debug
	int dummy_=pDoc->m_pObArray->GetSize();

	if(bRedrawAll){
		//Puts m_RectDraw to empty rect (OnMouseMove could set m_RectDraw)
		m_RectDraw=CRect(0,0,0,0);
		m_RectDraw2=CRect(0,0,0,0);
		Invalidate();
	}
	else{
		CRect rect=rect_union;
		rect.UnionRect(rect,m_RectPrev);

		/*
		//Scale update rectangle to current coordinates
		CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
		//CPoint point1=rect.TopLeft();
		CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);
		*/
		CRect rectScaled = _GetScaledUpdateRect(rect);

		//Scale update rectangle to current coordinates
		CRect rect2=rect_union2;
		CPoint point2=CPoint(rect2.TopLeft().x/m_xScale,rect2.TopLeft().y/m_yScale)-GetScrollPosition();
		CSize size2=CSize(rect2.Width()/m_xScale,rect2.Height()/m_yScale);

		//rect=CRect(point1,size);
		m_RectDraw=rect;
		m_RectPrev=rect_union;//keep last union rect
		m_RectDraw2=rect_union2;

//#define DCABLE_ONMOUSEMOVE_ERASING_ENABLE
#ifdef DCABLE_ONMOUSEMOVE_ERASING_ENABLE
		InvalidateRect(CRect(point1,size),TRUE);
		//RedrawWindow(CRect(point1,size));
#else
		//Delete curren position
		InvalidateRect(rectScaled/*CRect(point1, size)*/, FALSE);
		//Delete in original position
		InvalidateRect(CRect(point2,size2),FALSE);
#endif
	}

	//Call base class
	CScrollView::OnLButtonUp(nFlags, point);
}

void CDraftDrawView::_DoPanMove(CPoint point)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint pscrollpos = GetScrollPosition();

	int nMapMode;
	SIZE sizeTotal;
	SIZE sizePage;
	SIZE sizeLine;
	GetDeviceScrollSizes(nMapMode, sizeTotal, sizePage, sizeLine);

	//Move this line to right position
	CSize szOffset = point - m_PointLBD0;

	int x_offset = min(sizeTotal.cx, pscrollpos.x + szOffset.cx);
	int y_offset = min(sizeTotal.cy, pscrollpos.y + szOffset.cy);
	x_offset = max(0, pscrollpos.x + szOffset.cx);
	y_offset = max(0, pscrollpos.y + szOffset.cy);

	SetScrollPos(SB_HORZ, x_offset, TRUE);
	SetScrollPos(SB_VERT, y_offset, TRUE);

	CSize szDesign = pDoc->GetSize();
	CRect rcSheet = CRect(CPoint(0, 0), szDesign);
	//rcSheet.DeflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
	m_RectDraw = rcSheet;
	Invalidate(FALSE);

	return;
}

void CDraftDrawView::_DoMappingEngine(CPoint& point)
{
	//Scale point
	CPoint point_ns = point;
	ClientToSchematic(&point);

	//Snap to grid
	SnapToGrid((LPPOINT)&point);

	//Keep coor
	g_iX = point.x;
	g_iY = point.y;
}

void CDraftDrawView::GetSheetArea(CRect& rcSheet)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize szDesign = pDoc->GetSize();
	rcSheet = CRect(CPoint(0, 0), szDesign);
	rcSheet.DeflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
}

void CDraftDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	//Local variables
	int index;
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize m_szGrid=pDoc->m_szGrid;
	CShape *pSh;
	CShape *pShMove = NULL;
	CRect rect_union = CRect(0, 0, 0, 0);			//Rectangle for repainting

	//Pan tool controler below
	if ((nFlags&MK_LBUTTON) &&
		(pDoc->m_iToolSel == _TOOLSELECT_PAN)){
		_DoPanMove(point);
		return;
	}

	//MAPPING ENGINE: scale and snap
	//=======================================================
	_DoMappingEngine(point);
	//=======================================================
#ifdef TEST_SELTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CSelectionTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		CRect rect_union = ((CAbstractTool*)pDoc->m_tooSel)->MoveTo(nFlags, point);
		if (!rect_union.IsRectEmpty())
		{
			_SetUpdateRect(rect_union);
		}
		return;
	}
#endif
#ifdef TEST_PENTOOL
	if (((CAbstractTool*)pDoc->m_tooSel)->IsKindOf(RUNTIME_CLASS(CPenTool))){
		((CAbstractTool*)pDoc->m_tooSel)->m_pObListSel = &this->m_ObListSel;
		CRect rect_union = ((CAbstractTool*)pDoc->m_tooSel)->MoveTo(nFlags, point);
		if (!rect_union.IsRectEmpty())
		{
			_SetUpdateRect(rect_union);
		}
		return;
	}
#endif

	//Calculate sheet area
	//=======================================================
	CRect rcSheet;
	GetSheetArea(rcSheet);
	//=======================================================

	pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//If this if is uncommented place in chain (wires) doesn't work properly
	//thus a second ored clause is added on 25/10/2003
	if((nFlags & MK_LBUTTON)
		|| (pDoc->m_iToolSel==_TOOLPLACE_DRAFTCABLE)
		)
	{
		while(pSh){
			POSITION pos = m_ObListSel.Find(pSh);
			if (pSh->IsSelected()
				|| (pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT && pDoc->m_iToolType==_TOOLTYPECHAIN_DRAFTCABLE)
				){
				
				pShMove=pSh;
				int prev_mode=pSh->m_Mode;
				CRect prev_rect=pSh->m_Rect;
				pSh->m_Mode=0;

				if(pDoc->m_iToolSel!=_TOOLROTATE_DRAFTCABLE){
					pSh->OnMouseMove(nFlags, point);
					//Check if shape is beeing moved out of sheet
					//2014/04/10
					//This is not correct since if shape is placed initially out (or partially out) of design area will not work
					//-------------------------------------------
					if(!pSh->m_Rect.IsRectEmpty()){
						CRect rect1;
						rect1.IntersectRect(rcSheet,pSh->m_Rect);
						if(rect1!=pSh->m_Rect){
							//pSh->m_Rect=prev_rect;
							//2014/04/10
							//As a solution to above comment, we apply following code
							DoPreventShpOutSchem(pSh, rcSheet);
						}
					}
					//-------------------------------------------
					//Recover saved rectangle
					pSh->m_Mode=prev_mode;
				}
				else{
					pSh->OnRotate(nFlags, point);
				}
				CRect rect;
				pSh->GetSizingRect(rect);

				rect.NormalizeRect();

				_DoAddToUpdateRect(rect_union, rect);
			}

			//Iterate next shape
			pSh=(CShape *)pDoc->NextObject(index);

		}//End while
	}//End if


	//=======================================================
	//DRAWING ENGINE
	//=======================================================

	//pShMove is used as flag only
	if(pShMove>0){

		_SetUpdateRect(rect_union);
	}//End if something is being
	//=======================================================

	CScrollView::OnMouseMove(nFlags, point);
}

void CDraftDrawView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//Get pointer to document
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int index;
	int index_keep=-1;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);
	while(pSh){
		if((pSh->m_Mode==_DRAFTDRAW_MODE_SEL)&&
			(pSh->m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT)){

			index_keep=index-1;
			break;

		}

		if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){

			index_keep=index-1;
			break;
		}

		pSh=(CShape *)pDoc->NextObject(index);

	}

	switch(nChar){

	case 36://INI

		RedrawWindow();
		break;

	case 46://DEL

		/*if(index_keep!=-1){
			
			pDoc->DeleteObject(index_keep);
		}*/
		break;

	case 27://ESC
		//2014/07/23
		//this code is never called because of
		//ON_COMMAND(ID_ESCAPE, OnEscape)
		//so is commented
		/*
		if(index_keep!=-1){

			if(pDoc->m_iToolType==_TOOLTYPEPOLY_DRAFTCABLE){

				pSh->OnKeyDown(nChar,nRepCnt,nFlags);
				pSh->m_Mode=_DRAFTDRAW_MODE_DRW;				
			}
			else{

				//pSh->m_Mode=_DRAFTDRAW_MODE_DRW;
				pDoc->DeleteObject(index_keep);
				pDoc->m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			}
		}
		*/
		break;

	default://other

		if(pSh){
			pSh->OnKeyDown(nChar, nRepCnt, nFlags);
			CRect rect=pSh->m_Rect;
			m_RectDraw=rect;
			CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
			//CPoint point1=rect.TopLeft();
			CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);
			InvalidateRect(CRect(point1,size),FALSE);
		}
		break;
	}


	//RedrawWindow();
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDraftDrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HCURSOR hCursor=::GetCursor();
	if((hCursor==pDoc->m_CursorArray[8])||(hCursor==pDoc->m_CursorArray[0])||(hCursor==pDoc->m_CursorArray[1])){

		return TRUE;
	}


	pDoc->SetCursor();
	return TRUE;
	//return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CDraftDrawView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	//=======================================================
	//SELECTING-DESELECTING ENGINE
	//=======================================================
	//Flag for multiple select operation:
	//1.Normal: shape stack is walked from last to first
	//2.Inverse: when multiple select tool is selected stack is walked backwards
	int DRAFTCABLE_SELECT_INV=(pDoc->m_iToolSel==_TOOLSELECTMUL_DRAFTCABLE);
int idata_deb=pDoc->m_pObArray->GetSize();
	bool flag_return=false,flag_return1=false;
	int index;
	int index_keep=-1;
	CShape *pShconn=NULL;
	CShape *pShchild=NULL;
	CShape *pSh;
	if(DRAFTCABLE_SELECT_INV){
		pSh=(CShape *)pDoc->FirstObject(index);
		pSh=(CShape *)pDoc->NextObject(index);
	}
	else{
		pSh=(CShape *)pDoc->LastObject(index);
		pSh=(CShape *)pDoc->PrevObject(index);
	}


	//SELECTING/CONNECTING LOOP
	//=======================================================
	while(pSh){

		pSh->OnLButtonDown(nFlags, point);

		if(!pSh->m_Mode){

			flag_return=true;
			if(DRAFTCABLE_SELECT_INV){
				index_keep=index-1;
			}
			else{
				index_keep=index+1;
			}
			break;
		}

		//check temp conection
		if(pSh->m_pshChildConn){

			pShconn=pSh;
			pShchild=pSh->m_pshChildConn;

		}

		if(DRAFTCABLE_SELECT_INV){
			pSh=(CShape *)pDoc->NextObject(index);
		}
		else{
			pSh=(CShape *)pDoc->PrevObject(index);
		}
	}
	//=======================================================

	//old code follows, delete!
	/*
	bool flag_return=false;
	int index;
	int index_keep=-1;
	CShape *pShconn=NULL;
	CShape *pShchild=NULL;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//SELECTING LOOP
	
	while(pSh){

		pSh->OnLButtonDown(nFlags, point);

		if(!pSh->m_Mode){

			pDoc->DeleteObject(index-1);
			pDoc->AddObject(pSh);
			flag_return=true;
			index_keep=pDoc->m_pObArray->GetSize()-1;//index-1;
			break;
		}


		pSh=(CShape *)pDoc->NextObject(index);
	}


	pSh=(CShape *)pDoc->LastObject(index);
	pSh->OnLButtonUp(nFlags, point);
	*/

	if(pSh)
		pSh->OnLButtonUp(nFlags, point);
	//RedrawWindow();

	//CONTEXT MENU DISPLAYING
    CMenu menu;
    CMenu *submenu;
    // Load the menu.
    menu.LoadMenu(IDR_CONTEXT);
    // Get the pop-up menu.
    submenu = menu.GetSubMenu(0);



#ifdef _DEBUG
submenu->AppendMenu(MF_STRING,ID_DEBUG_EDIT_SHPINFO,_T("Info"));
#endif



    // Convert to screen coordinates.
    ClientToScreen(&point);
	//Disable some commands
	//submenu->EnableMenuItem(ID_SELECT_NET,MF_GRAYED);
	//==================================================
	CShape *pShSel=NULL;
	m_bFlagSelNet=FALSE;
	m_bFlagEditTB=FALSE;
	BOOL bFlagOther=FALSE;
	for(int i=0;i<pDoc->m_pObArray->GetSize();i++){
		CShape *pSh=(CShape *)(*pDoc->m_pObArray)[i];
		if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire)) ){
				//submenu->EnableMenuItem(ID_SELECT_NET,MF_GRAYED);
				m_bFlagSelNet=TRUE;
				if(!pSh->m_strIdent.IsEmpty()){
					m_bFlagEditTB=TRUE;
					m_strEditID=pSh->m_strIdent;
				}
			}
			else{
				bFlagOther=TRUE;
			}
		}
	}
	if(bFlagOther){
		bFlagOther=FALSE;
	}
	//==================================================

    // Post the menu.
    submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
            point.x,    point.y,
            AfxGetApp()->m_pMainWnd,NULL);

	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CDraftDrawView::OnZoomIn() 
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Calculate first values
	CRect rect;
	GetClientRect(&rect);
	CPoint pscrollpos=GetScrollPosition();
	CSize szDesign=pDoc->GetSize();
	szDesign=CSize(szDesign.cx/m_xScale,szDesign.cy/m_yScale);
	CRect rcDesign(CPoint(0,0),szDesign);

	//Calculate zoom center point
	CPoint point=rect.CenterPoint();
	(point.x>rcDesign.CenterPoint().x?point.x=rcDesign.CenterPoint().x/m_xScale:point.x);
	(point.y>rcDesign.CenterPoint().y?point.y=rcDesign.CenterPoint().y/m_yScale:point.y);
	pscrollpos=CPoint(pscrollpos.x*m_xScale,pscrollpos.y*m_yScale);
	point=CPoint(point.x*m_xScale,point.y*m_yScale)+pscrollpos;

	//Execute zoom
	DoZoomIn(point);
}

void CDraftDrawView::DoZoomIn(CPoint point) 
{
	//Calcule first values
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize szDesign=pDoc->GetSize();
	CPoint pscrollpos=GetScrollPosition();
	//pscrollpos=CPoint(pscrollpos.x*m_xScale,pscrollpos.y*m_yScale);
	CPoint point1=CPoint(point.x/m_xScale,point.y/m_yScale);
	CSize offset=point1-pscrollpos;
	//offset=CSize(offset.cx*m_xScale,offset.cy*m_yScale);

	//Calcule
	if(m_szWext.cx>1){
		m_szWext-=CSize(1,1);
		SetScrollSizes(MM_TEXT,
			CSize(szDesign.cx/m_szWext.cx,szDesign.cy/m_szWext.cx),
			CSize(SCROLL_SIZE_PAGE/m_szWext.cx,SCROLL_SIZE_PAGE/m_szWext.cx),
			CSize(SCROLL_SIZE_LINE/m_szWext.cx,SCROLL_SIZE_LINE/m_szWext.cx));
		m_xScale=m_szWext.cx;
		m_yScale=m_szWext.cy;
	}
	else{
		m_szVPext+=CSize(1,1);
		SetScrollSizes(MM_TEXT,
			CSize(szDesign.cx*m_szVPext.cx,szDesign.cy*m_szVPext.cx),
			CSize(SCROLL_SIZE_PAGE*m_szVPext.cx,SCROLL_SIZE_PAGE*m_szVPext.cx),
			CSize(SCROLL_SIZE_LINE*m_szVPext.cx,SCROLL_SIZE_LINE*m_szVPext.cx));
		m_xScale=1.0/m_szVPext.cx;
		m_yScale=1.0/m_szVPext.cy;
	}	

	//Offset origin so as to cursor is as the same point of drawing.
	pscrollpos.x=(point.x/m_xScale-offset.cx);
	pscrollpos.y=(point.y/m_yScale-offset.cy);

	//Offset gain: align zoom center with center of screen.
	//Comment this block if you don't want this behaviour.
	//Try it!
	CRect rect;
	//--------------------------------------------------------------------------
	pscrollpos.x=point.x/m_xScale;
	pscrollpos.y=point.y/m_yScale;
	GetClientRect(&rect);
	point=rect.CenterPoint();
	pscrollpos=-CPoint(point.x,point.y)+pscrollpos;//no scale needed for center point!
	//--------------------------------------------------------------------------

	//Final check:
	//origin is not offset if design fits in window.
	szDesign=CSize(szDesign.cx/m_xScale,szDesign.cy/m_yScale);
	GetClientRect(&rect);
	(rect.Width()>szDesign.cx?pscrollpos.x=0:pscrollpos.x);
	(rect.Height()>szDesign.cy?pscrollpos.y=0:pscrollpos.y);

	//Execute offset
	ScrollToPosition(pscrollpos);

	//Redraw window
	RedrawWindow();
}

void CDraftDrawView::OnZoomOut() 
{
	//Calculate first values
	CRect rect;
	GetClientRect(&rect);
	CPoint pscrollpos=GetScrollPosition();

	//Calculate zoom center point
	CPoint point=rect.CenterPoint();
	pscrollpos=CPoint(pscrollpos.x*m_xScale,pscrollpos.y*m_yScale);
	point=CPoint(point.x*m_xScale,point.y*m_yScale)+pscrollpos;

	//Execute zoom
	DoZoomOut(point);
}

void CDraftDrawView::DoZoomOut(CPoint point) 
{
	//Calcule first values
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize szDesign=pDoc->GetSize();
	CPoint pscrollpos=GetScrollPosition();
	//pscrollpos=CPoint(pscrollpos.x*m_xScale,pscrollpos.y*m_yScale);
	CPoint point1=CPoint(point.x/m_xScale,point.y/m_yScale);
	CSize offset=point1-pscrollpos;

	//Calcule
	if(m_szVPext.cx>1){
		m_szVPext-=CSize(1,1);
		SetScrollSizes(MM_TEXT,
			CSize(szDesign.cx*m_szVPext.cx,szDesign.cy*m_szVPext.cx),
			CSize(100*m_szVPext.cx,100*m_szVPext.cx),
			CSize(10*m_szVPext.cx,10*m_szVPext.cx));

		m_xScale=1.0/m_szVPext.cx;
		m_yScale=1.0/m_szVPext.cy;
	}
	else{
		m_szWext+=CSize(1,1);
		SetScrollSizes(MM_TEXT,
			CSize(szDesign.cx/m_szWext.cx,szDesign.cy/m_szWext.cx),
			CSize(100/m_szWext.cx,100/m_szWext.cx),
			CSize(10/m_szWext.cx,10/m_szWext.cx));

		m_xScale=m_szWext.cx;
		m_yScale=m_szWext.cy;
	}

	//Offset origin so as to cursor is as the same point of drawing.
	pscrollpos.x=(point.x/m_xScale-offset.cx);
	pscrollpos.y=(point.y/m_yScale-offset.cy);

	//Offset gain: align zoom center with center of screen.
	//Comment this block if you don't want this behaviour.
	//Try it!
	//--------------------------------------------------------------------------
	pscrollpos.x=point.x/m_xScale;
	pscrollpos.y=point.y/m_yScale;
	CRect rect;
	GetClientRect(&rect);
	point=rect.CenterPoint();
	pscrollpos=-CPoint(point.x,point.y)+pscrollpos;//no scale needed for center point!
	//--------------------------------------------------------------------------

	//Final check:
	//origin is not offset if design fits in window.
	szDesign=CSize(szDesign.cx/m_xScale,szDesign.cy/m_yScale);
	GetClientRect(&rect);
	(rect.Width()>szDesign.cx?pscrollpos.x=0:pscrollpos.x);
	(rect.Height()>szDesign.cy?pscrollpos.y=0:pscrollpos.y);

	//Execute offset.
	ScrollToPosition(pscrollpos);

	//Redraw window.
	RedrawWindow();
}

void CDraftDrawView::OnZoomAll() 
{
	//Calcule first values
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize szDesign=pDoc->GetSize();

	CRect rect;
	GetClientRect(&rect);

	//Calcule
	CSize szWext;
	if(szDesign.cx>rect.Width()){

		szWext.cx=(szDesign.cx+rect.Width())/rect.Width();
	}

	if(szDesign.cy>rect.Height()){

		szWext.cy=(szDesign.cy+rect.Height())/rect.Height();
	}

	//Process
	(szWext.cx>szWext.cy?m_szWext.cx=m_szWext.cy=szWext.cx:m_szWext.cx=m_szWext.cy=szWext.cy);

	m_szVPext=CSize(1,1);

	//Execute zoom
	SetScrollSizes(MM_TEXT,
		CSize(szDesign.cx/m_szWext.cx,szDesign.cy/m_szWext.cx),
		CSize(100/m_szWext.cx,100/m_szWext.cx),
		CSize(10/m_szWext.cx,10/m_szWext.cx));

	//Recalcule scales.
	m_xScale=m_szWext.cx;
	m_yScale=m_szWext.cy;


	//Redraw window
	RedrawWindow();
}

void CDraftDrawView::OnSnapTogrid() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_szGrid.cx=10;
	pDoc->m_szGrid.cy=10;

	m_bGrid=!m_bGrid;
	RedrawWindow();
}

void CDraftDrawView::OnSnapTogrid2() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_szGrid.cx=2;
	pDoc->m_szGrid.cy=2;

	m_bGrid=!m_bGrid;
	RedrawWindow();
}

void CDraftDrawView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	if(0&&m_bGrid){
		m_bGridTemp=FALSE;
		SetTimer(1,100,NULL);
	}
}

void CDraftDrawView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);

/*
CRect rect;
GetScrollRect(rect);
if(!rect.IsRectNull()){
	m_RectDraw=rect;
	RedrawWindow(&m_RectDraw);
}
*/

	if(0&&m_bGrid){
		m_bGridTemp=FALSE;
		SetTimer(1,100,NULL);
	}
}

void CDraftDrawView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize m_szGrid=pDoc->m_szGrid;

	//=======================================================
	//MAPPING ENGINE
	//=======================================================

	//SCALE POINT
	point=CPoint(m_xScale*point.x,m_yScale*point.y)+
		CPoint(m_xScale*GetScrollPosition().x,m_yScale*GetScrollPosition().y);

	//SNAP TO GRID
	if(m_bGrid){
		
		point.x=((int)((point.x+m_szGrid.cx*.5)/m_szGrid.cx))*m_szGrid.cx;
		point.y=((int)((point.y+m_szGrid.cy*.5)/m_szGrid.cy))*m_szGrid.cy;

	}
	//=======================================================

int idata_deb=pDoc->m_pObArray->GetSize();
	bool flag_return=false,flag_return1=false;
	int index;
	int index_keep=-1;
	CShape *pShconn=NULL;
	CShape *pShchild=NULL;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//=======================================================
	//SELECTING/CONNECTING LOOP
	//=======================================================

	if(pDoc->m_iToolSel==_TOOLSELECT_DRAFTCABLE || pDoc->m_iToolSel== _TOOLSELECTMUL_DRAFTCABLE)
	while(pSh){

		CString sName=pSh->m_strIdent;

		pSh->OnLButtonDblClk(nFlags, point);

		if(sName!=pSh->m_strIdent &&
			!pDoc->m_bFlagPartEdit){

			//check shape of type unit
			ASSERT(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer)));
			CShapeContainer *pShCont=(CShapeContainer *)pSh;

			//create new label
			CShapeLabel *pShlabel=new CShapeLabel();
			pShlabel->m_pCursorArray=pDoc->m_CursorArray;
			pShlabel->m_Mode=1;
			pShlabel->m_Label.bver=pShCont->m_bVertical;//true;
			pShlabel->m_Label.slabel=new CString(pSh->m_strIdent);
			pShlabel->m_Label.sname=new CString(pSh->m_strIdent);
			pShlabel->m_Label.iSize=pShCont->m_iSize;

			//13/03/2005
			//Process wire
			//A mechanism should be incorpored to decide is user has changed label apearance
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){

				CShapeWire *pShWire=(CShapeWire *)pSh;
				pShlabel->m_Label.bver=(pShWire->m_Rect.Height()>pShWire->m_Rect.Width());

			}

			LOGFONT lf;
			ZeroMemory(&lf, sizeof(LOGFONT));

			if(pShlabel->m_Label.iSize>0){
				lf.lfHeight=-pShlabel->m_Label.iSize-3;
			}
			else{
				lf.lfHeight=-13;
			}
			lf.lfWeight=400;
			lf.lfOutPrecision=3;
			lf.lfClipPrecision=2;
			lf.lfQuality=1;
			lf.lfPitchAndFamily=34;
			wcscpy(lf.lfFaceName,_T("Arial"));

			pShlabel->m_Label.font=new CFont();
			pShlabel->m_Label.font->CreateFontIndirect(&lf);

			//Calculate rect size
			SIZE sz;
			CDC *pDC=GetDC( );
			CFont *pFont=pDC->SelectObject(pShlabel->m_Label.font);

			if(pShlabel->m_Label.bver){

				CString stlabel=_T("");
				CSize szac=CSize(0,0);

				for(int i=0;i<pShlabel->m_Label.slabel->GetLength();i++){
					
					if(i)stlabel+=_T("\n");
					stlabel+=pShlabel->m_Label.slabel[0][i];


					CString stlabel1=pShlabel->m_Label.slabel[0][i];

					SIZE sz;
					GetTextExtentPoint32(pDC->m_hDC, stlabel1, lstrlen(stlabel1), &sz);

					szac+=CSize(0,sz.cy);
					if(szac.cx<sz.cx){
						szac.cx=sz.cx;
					}
				}

				*pShlabel->m_Label.slabel=stlabel;

				pShlabel->m_Rect=CRect(pSh->m_Rect.TopLeft(),szac);
			}
			else{

				GetTextExtentPoint32(pDC->m_hDC, *pShlabel->m_Label.slabel, pShlabel->m_Label.slabel->GetLength(), &sz);
				
				CRect rect=CRect(pSh->m_Rect.TopLeft(),sz);
				pShlabel->m_Rect=rect;

			}

			//13/03/2005
			//Possition wire label
			//This is an approximate algorithm: conver to a more accure one and promote to function!
			//After that link with wire repostion throw command interpreter.
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){

				if(pShlabel->m_Label.bver){
					CPoint point=pSh->m_Rect.CenterPoint()-CSize(pShlabel->m_Rect.Width()*.5,pShlabel->m_Rect.Height()*.5);
					point-=CSize(pShlabel->m_Rect.Size().cx * 2,0);
					pShlabel->m_Rect=CRect(point,pShlabel->m_Rect.Size());
				}
				else{
					CPoint point=pSh->m_Rect.CenterPoint()-CSize(pShlabel->m_Rect.Width()*.5,pShlabel->m_Rect.Height()*.5);
					point+=CSize(0,pShlabel->m_Rect.Size().cy * 2);
					pShlabel->m_Rect=CRect(point,pShlabel->m_Rect.Size());
				}
			}



			pDC->SelectObject(pFont);

			//Connect label and wire
			//==================================================================
			pShlabel->m_pshConn=pSh;
			pShlabel->m_pshChildConn=pSh;
			pShlabel->m_bConect=TRUE;
			pSh=pShlabel->m_pshConn;
			pSh=pShlabel->m_pshChildConn;
			//==================================================================
			if(!pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){
				pDoc->AddObject(pShlabel);
			}
			else{
				pDoc->AddObject(pShlabel);
				/*
				CShapeWire *pWire=(CShapeWire *)pSh;
				pWire->m_obarrShapearr.Add(pShlabel);
				pWire->m_ShapeNumber=pWire->m_obarrShapearr.GetSize();
				*/
				//Track all cable length
				//==============================================================
				for(int i=0;i<2;i++){
					CShape *pSh1=pSh;
					CShapeWire *pShWire=(CShapeWire *)pSh1;
					CString strIdent=pSh1->m_strIdent;
					CString	strTypeIdent=pSh1->m_strTypeIdent;
					CShape *pSh1Ant=NULL;
					do{
						if(pSh1Ant==pSh1){
							break;
						}

						pSh1Ant=pSh1;


						pShWire=(CShapeWire *)pSh1;
						CShape *pSh=(CShape *)pShWire->m_obarrShapearr[i];
CShape *pShdeb1=(CShape *)pShWire->m_obarrShapearr[0];
CShape *pShdeb2=(CShape *)pShWire->m_obarrShapearr[1];
						if((pSh)&&(pSh->IsKindOf(RUNTIME_CLASS(CShapePin)))){
							CShapePin *pShPin=(CShapePin *)pSh;
							if(pShPin->m_pshChildConn){
								pSh1=((CShapePin *)(pShPin->m_pshChildConn))->GetParent();
							}
							else{
								pSh1=NULL;
							}
							if((pShPin->m_bConect)&&(pSh1)&&(pSh1->IsKindOf(RUNTIME_CLASS(CShapeWire))) ){
								pSh1->m_strIdent=strIdent;
								pSh1->m_strTypeIdent=strTypeIdent;
							}
						}
						else{
							pSh1=NULL;
						}
					}while((pSh1)&&(pSh1->IsKindOf(RUNTIME_CLASS(CShapeWire))) );
				}
				//==============================================================
			}//end of if wire

			//m_RectDraw=pShlabel->m_Rect;
			CRgn rgn;
			rgn.CreateRectRgnIndirect( pShlabel->m_Rect );
			InvalidateRgn(&rgn);
		}

		pSh=(CShape *)pDoc->NextObject(index);
	}//end while
	else if(pDoc->m_iToolSel==_TOOLPLACE_DRAFTCABLE && pDoc->m_iToolType==_TOOLTYPEPOLY_DRAFTCABLE){
		CDraftDrawDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
	
		CRect rect;

		int index;
		int index_keep=-1;
		CShape *pSh=(CShape *)pDoc->FirstObject(index);
		pSh=(CShape *)pDoc->NextObject(index);

		//Iterate all the shapes
		while(pSh){
			if((pSh->m_Mode==_DRAFTDRAW_MODE_SEL)&&
				(pSh->m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT)){
				rect=pSh->m_Rect;
				index_keep=index-1;
				break;
			}

			//Next shape
			pSh=(CShape *)pDoc->NextObject(index);
		}

		if(index_keep!=-1){
			if(pDoc->m_iToolType==_TOOLTYPEPOLY_DRAFTCABLE){
				pSh->OnLButtonDblClk(nFlags, point);
				pSh->OnKeyDown(27,0,0);
				pSh->m_Mode=_DRAFTDRAW_MODE_DRW;							
			}
		}

		//Put selection tool
		AfxGetMainWnd()->PostMessage(WM_COMMAND,ID_BUTTON_SELECT);

		//=======================================================
		//DRAWING ENGINE
		//=======================================================
		if(index_keep!=-1){

			CRect rect_union = CRect(0, 0, 0, 0);

			pSh->GetRectUpdatePlace(rect);
			_DoAddToUpdateRect(rect_union, rect);		

			rect = rect_union;
			rect.UnionRect(rect, m_RectPrev);

			CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
			CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);
			//rect=CRect(point1,size);
			m_RectDraw = rect;
			m_RectPrev = rect_union;//keep last union rect

			InvalidateRect(CRect(point1,size),FALSE);

			/*
			CRect rect=rect_union;

			rect.UnionRect(rect, m_RectPrev);

			//Scale update rectangle to current coordinates
			CPoint point1 = CPoint(rect.TopLeft().x / m_xScale, rect.TopLeft().y / m_yScale) - GetScrollPosition();
			//CPoint point1=rect.TopLeft();
			CSize size = CSize(rect.Width() / m_xScale, rect.Height() / m_yScale);

			//Scale update rectangle to current coordinates
			CRect rect2 = rect_union2;
			CPoint point2 = CPoint(rect2.TopLeft().x / m_xScale, rect2.TopLeft().y / m_yScale) - GetScrollPosition();
			CSize size2 = CSize(rect2.Width() / m_xScale, rect2.Height() / m_yScale);

			//rect=CRect(point1,size);
			m_RectDraw = rect;
			m_RectPrev = rect_union;//keep last union rect
			m_RectDraw2 = rect_union2;

			//#define DCABLE_ONMOUSEMOVE_ERASING_ENABLE
#ifdef DCABLE_ONMOUSEMOVE_ERASING_ENABLE
			InvalidateRect(CRect(point1, size), TRUE);
			//RedrawWindow(CRect(point1,size));
#else
			//Delete curren position
			InvalidateRect(CRect(point1, size), FALSE);
			//Delete in original position
			InvalidateRect(CRect(point2, size2), FALSE);
#endif			
			
			*/
		}
		//=======================================================
		//=======================================================
	}
	//=======================================================
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDraftDrawView::OnEscape() 
{
	//Local variables
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;
	int index;
	int index_keep=-1;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);

	//Iterate all the shapes
	while(pSh){
		if((pSh->m_Mode==_DRAFTDRAW_MODE_SEL)&&
			(pSh->m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT)){
			rect=pSh->m_Rect;
			index_keep=index-1;
			break;
		}
		if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			rect=pSh->m_Rect;
			index_keep=index-1;
			break;
		}
		//Next shape
		pSh=(CShape *)pDoc->NextObject(index);
	}

	//Changing shape status
	if(index_keep!=-1){
		//End polyline drawing delting current segment
		if(pDoc->m_iToolType==_TOOLTYPEPOLY_DRAFTCABLE){
			pSh->OnKeyDown(27,0,0);
			pSh->m_Mode=_DRAFTDRAW_MODE_DRW;
		}
		else{
			if (pSh->m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT){
				//Deselect shape when user hits <ESC>
				pSh->Select(FALSE);
			}
			else{
				//pSh->m_Mode=_DRAFTDRAW_MODE_DRW;
				//Deletes current segment
				pDoc->DeleteObject(index_keep);
				//pDoc->m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			}
		}
	}

	//15/08/2004
	//Put selection tool
	AfxGetMainWnd()->PostMessage(WM_COMMAND,ID_BUTTON_SELECT);

	//=======================================================
	//DRAWING ENGINE
	//=======================================================
	if(index_keep!=-1){
		//Calculate update area

		CRect rect_union = CRect(0, 0, 0, 0);

		_DoAddToUpdateRect(rect_union, rect);

		rect = rect_union;

		CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
		CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);

		m_RectDraw=rect;
		InvalidateRect(CRect(point1,size),TRUE);
	}
	//=======================================================
	//=======================================================
	
}

void CDraftDrawView::OnRotate() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRgn rgn1,rgn2;
	rgn1.CreateRectRgn(0,0,0,0);
	BOOL bRedraw=FALSE;

	int index;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);
	while(pSh){

		if(pSh->OnCommand(MAKEWPARAM(ID_ROTATE,0),0)){
			CRect rect=pSh->m_Rect;

			rect.InflateRect(DCABLE_GRIDX_DEFAULT<<1,DCABLE_GRIDY_DEFAULT<<1);

			rgn2.CreateRectRgnIndirect(rect);
			rgn1.CombineRgn(&rgn1,&rgn2,RGN_OR);
			bRedraw=TRUE;
		}
		pSh=(CShape *)pDoc->NextObject(index);
	}

	if(bRedraw){
		RedrawWindow(NULL,&rgn1);
	}
}

BOOL CDraftDrawView::OnCommandShape(WPARAM wParam, LPARAM lParam){
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRgn rgn1;
	rgn1.CreateRectRgn(0,0,0,0);
	BOOL bRedraw=FALSE;

	int index;
	CShape *pSh=(CShape *)pDoc->FirstObject(index);
	pSh=(CShape *)pDoc->NextObject(index);
	CShape* pShPrev = NULL;
	while(pSh){

		//Local regions for update
		CRgn rgn2;
		CRgn rgn3;

		//Filter for duplicated shapes in stack like polylines
		if (pShPrev == pSh){
			pShPrev = pSh;
			pSh = (CShape *)pDoc->NextObject(index);
			continue;
		}

		//Store previous rectangle
		CRect rectPrev = pSh->m_Rect;
		if(pSh->OnCommand(wParam,lParam)){

			CRect rect=pSh->m_Rect;
			rect.NormalizeRect();
			rect.InflateRect(DCABLE_GRIDX_DEFAULT<<1,DCABLE_GRIDY_DEFAULT<<1);

			CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
			CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);

			rgn2.CreateRectRgnIndirect(CRect(point1,size));
			rgn1.CombineRgn(&rgn1,&rgn2,RGN_OR);

			//Previous region
			rect = rectPrev;
			rect.NormalizeRect();
			rect.InflateRect(DCABLE_GRIDX_DEFAULT << 1, DCABLE_GRIDY_DEFAULT << 1);
			point1 = CPoint(rect.TopLeft().x / m_xScale, rect.TopLeft().y / m_yScale) - GetScrollPosition();
			size = CSize(rect.Width() / m_xScale, rect.Height() / m_yScale);
			rgn3.CreateRectRgnIndirect(CRect(point1, size));
			rgn1.CombineRgn(&rgn1, &rgn3, RGN_OR);

			//Redraw flag
			bRedraw=TRUE;
		}

		pShPrev = pSh;
		pSh=(CShape *)pDoc->NextObject(index);
	}

	if(bRedraw){
		RedrawWindow(NULL,&rgn1);
	}

	return TRUE;
}

BOOL CDraftDrawView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pExtra==NULL){
		switch(nID){
		//Commands that are handled by shapes
		case ID_ROTATE:
		case ID_DEBUG_EDIT_SHPINFO:
		case ID_EDIT_PROPERTIES:
		case ID_IMG_FLIPHORZ:
		case ID_IMG_FLIPVERT:
		case ID_EDIT_FORMATSHAPE:
		case ID_ROTATE_ROTATE90CW:
		case ID_ROTATE_ROTATE90CCW:
			return OnCommandShape(MAKEWPARAM(nID,0),0);
			break;

		case ID_SUP:{
			//Rectangle for repainting
			CRect rect_union=CRect(0,0,0,0);

			CDraftDrawDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			int index;

			CShape *pSh=(CShape *)pDoc->FirstObject(index);
			int index_last=index;
			pSh=(CShape *)pDoc->NextObject(index);

			while(pSh){

				if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
					CRect rect;
					pSh->GetSizingRect(rect);
					rect.NormalizeRect();
					//Inflate update rectangle
					rect.InflateRect(DCABLE_GRIDX_DEFAULT*4,DCABLE_GRIDY_DEFAULT*4);//MAGIC NUMBER
					//Add to update rectangle
					rect_union.UnionRect(rect_union,rect);

					pDoc->DeleteObject(index_last);
					//Delete from selected shapes
					DeleteSelected(pSh);
					index--;
					//break;


				}

				index_last=index;

				pSh=(CShape *)pDoc->NextObject(index);

			}

			CRect rect=rect_union;
			//Scale update rectangle to current coordinates
			CPoint point1=CPoint(rect.TopLeft().x/m_xScale,rect.TopLeft().y/m_yScale)-GetScrollPosition();
			//CPoint point1=rect.TopLeft();
			CSize size=CSize(rect.Width()/m_xScale,rect.Height()/m_yScale);				
			//RedrawWindow();
			m_bForceTransfer=TRUE;
			m_RectDraw=rect;
			InvalidateRect(CRect(point1,size),FALSE);

			return TRUE;
			}break;
		case ID_SELECT_NET:
			if(m_bFlagSelNet==TRUE){
				OnSelectNet();
			}
			break;
		case ID_EDIT_TB:
			if(m_bFlagEditTB==TRUE){
				CDraftDrawDoc* pDoc = GetDocument();
				ASSERT_VALID(pDoc);
				pDoc->DisplayPatchPProp(m_strEditID);
			}
			break;
		}
	}
	else{
		//Returning true in this case enables menu item.
		switch(nID){
		case ID_SELECT_NET:
			//Selected net must to be enabled when a wire segment is selected.
			if(m_bFlagSelNet==TRUE){
				return TRUE;
			}
			break;
		case ID_EDIT_TB:
			if(m_bFlagEditTB==TRUE){
				return TRUE;
			}
			break;
		case ID_ROTATE:
		case ID_DEBUG_EDIT_SHPINFO:
		case ID_EDIT_PROPERTIES:
		case ID_IMG_FLIPHORZ:
		case ID_IMG_FLIPVERT:
		case ID_EDIT_FORMATSHAPE:
		case ID_ROTATE_ROTATE90CW:
		case ID_ROTATE_ROTATE90CCW:
			return TRUE;
			break;
		}
	}

	return CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDraftDrawView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_bGridTemp=TRUE;
	RedrawWindow();
	CScrollView::OnTimer(nIDEvent);
}

//Snap a point to the grid (if is ennabled)
CPoint CDraftDrawView::Snap(CPoint point)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize m_szGrid=pDoc->m_szGrid;

	//SNAP TO GRID
	CPoint ptRes=point;
	if(m_bGrid){
		ptRes.x=((int)((point.x+m_szGrid.cx*.5)/m_szGrid.cx))*m_szGrid.cx;
		ptRes.y=((int)((point.y+m_szGrid.cy*.5)/m_szGrid.cy))*m_szGrid.cy;
	}
	return ptRes;
}

//Draw grid
void CDraftDrawView::DrawGrid(CDC *pDC,LPRECT lpRect,BOOL inBuffer){

	//Local variables
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize m_szGrid=pDoc->m_szGrid;
	int origin_x1=lpRect->left;
	int limit_x1=lpRect->right;
	int origin_y1=lpRect->top;
	int limit_y1=lpRect->bottom;

	//Get limits for draw
	CPoint ptOrg1=Snap(CPoint(origin_x1,origin_y1));
	CPoint ptLimit1=CPoint(limit_x1,limit_y1);

	//Get viewport and window extend
	int vp=min(max(m_szVPext.cx,m_szVPext.cy),MAX_NUM_PEN_IN_VIEW)-1;
	int w=max(m_szWext.cx,m_szWext.cy);
	int grd=min(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT);

	//Check if grid point will be too close
	if(w>2){
		return;
	}

	//With viewport greater than 1, draw a square
	if(vp>1 || (vp==1 && inBuffer==FALSE)){
		CBrush black_brush(RGB(0,0,0));
		CBrush *p_brush=pDC->SelectObject(&black_brush);
		for(int i=ptOrg1.x;i<=ptLimit1.x;i+=m_szGrid.cx){
		for(int j=ptOrg1.y;j<=ptLimit1.y;j+=m_szGrid.cy){
			pDC->Rectangle(i,j,i+vp,j+vp);
		}}
		pDC->SelectObject(p_brush);
	}
	//Normal draw
	else
	{
		for(int i=ptOrg1.x;i<=ptLimit1.x;i+=m_szGrid.cx){
		for(int j=ptOrg1.y;j<=ptLimit1.y;j+=m_szGrid.cy){

			pDC->SetPixel(CPoint(i,j),RGB(0,0,0));
		}}
	}
}

//This function is under develop and tries to draw a grip in the
//frame between and inner and outer given rectangles
void CDraftDrawView::DrawGrid(CDC *pDC,LPRECT lpRectOut,LPRECT lpRectIn){

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize m_szGrid=pDoc->m_szGrid;

	int origin_x1;
	int limit_x1;
	int origin_y1;
	int limit_y1;
	int origin_x2;
	int limit_x2;
	int origin_y2;
	int limit_y2;


	for(int k=0;k<3;k++){

		switch(k){
		case 0:
			origin_x1=lpRectOut->left;
			limit_x1=lpRectOut->right;
			origin_y1=lpRectOut->top;
			limit_y1=lpRectIn->top;
			origin_x2=0;
			limit_x2=0;
			origin_y2=0;
			limit_y2=0;
			break;
		case 1:
			origin_x1=lpRectOut->left;
			limit_x1=lpRectIn->left;
			origin_y1=lpRectIn->top;
			limit_y1=lpRectIn->bottom;
			origin_x2=lpRectIn->right;
			limit_x2=lpRectOut->right;
			origin_y2=lpRectIn->top;
			limit_y2=lpRectIn->bottom;
			break;
		case 2:
			origin_x1=lpRectOut->left;
			limit_x1=lpRectOut->right;
			origin_y1=lpRectIn->bottom;
			limit_y1=lpRectOut->bottom;
			origin_x2=0;
			limit_x2=0;
			origin_y2=0;
			limit_y2=0;
			break;
		}



		CPoint ptOrg1=Snap(CPoint(origin_x1,origin_y1));
		CPoint ptLimit1=Snap(CPoint(limit_x1,limit_y1));
		CPoint ptOrg2=Snap(CPoint(origin_x2,origin_y2));
		CPoint ptLimit2=Snap(CPoint(limit_x2,limit_y2));

	//if(m_RectDraw.IsRectNull())//this line erases grid when moving a shape
		int i;
		for(i=ptOrg1.x;i<ptLimit1.x;i+=m_szGrid.cx){
		for(int j=ptOrg1.y;j<ptLimit1.y;j+=m_szGrid.cy){

			pDC->SetPixel(CPoint(i,j),RGB(0,0,0));
		}}

	//if(m_RectDraw.IsRectNull())//this line erases grid when moving a shape
		for(i=ptOrg2.x;i<ptLimit2.x;i+=m_szGrid.cx){
		for(int j=ptOrg2.y;j<ptLimit2.y;j+=m_szGrid.cy){

			pDC->SetPixel(CPoint(i,j),RGB(0,0,0));
		}}

	}//end k

}

void CDraftDrawView::OnOrderBottom() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Order(0);	
}

void CDraftDrawView::OnOrderTop() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Order(1);
}

void CDraftDrawView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	SetScrollSizes(MM_TEXT,pDoc->GetSize(),CSize(100,100),CSize(10,10));

	//Sended when changing to or from edit part
	switch(lHint){
		case UPDATE_HINT_NEWDOC:
			RedrawWindow();
			GetFolderFrame()->GetFolderTabCtrl().SelectItem(pDoc->GetSheet());
			break;

		case UPDATE_HINT_REGION:
			if(pHint&&pHint->IsKindOf(RUNTIME_CLASS(CRgn))){
				CRgn *rgn=(CRgn *)pHint;

				RedrawWindow(NULL,rgn);
			}
			break;

		case UPDATE_HINT_SHEET_SIZE_CHANGE:{
			//10/05/2005
			//This implementation does not cope with the problem of placing shapes bigger than the sheet...
			if(m_bmpCanvas1){

				delete(m_bmpCanvas1);
			}
			m_bmpCanvas1=new CBitmap();

			//Prepare memory alocation
			CSize size=pDoc->GetSize();
			__int32 *Bits=new __int32[size.cx*size.cy];
			memset(Bits,0xff,size.cx*size.cy*sizeof(__int32));

			//Create bitmap
			m_bmpCanvas1->CreateBitmap( size.cx, size.cy, 1, 32, Bits );

			delete(Bits);

			pDC1->SelectObject(m_bmpCanvas1);

			//2nd buffer
			if(m_bmpCanvas2){

				delete(m_bmpCanvas2);
			}
			m_bmpCanvas2=new CBitmap();

			//Prepare memory alocation
			Bits=new __int32[size.cx*size.cy];
			memset(Bits,0xff,size.cx*size.cy*sizeof(__int32));

			//Create bitmap
			m_bmpCanvas2->CreateBitmap( size.cx, size.cy, 1, 32, Bits );

			delete(Bits);

			pDC2->SelectObject(m_bmpCanvas2);

			//Invalidate window
			Invalidate();

			}
			break;

		case UPDATE_HINT_SAVE_BMP:
			m_bSaveDC=TRUE;
			break;
	}
	
}

void CDraftDrawView::OnUpdateBtEditPart(CCmdUI *pCmdUI){

	((CDraftDrawApp *)AfxGetApp())->OnUpdateBtEditPart(pCmdUI);
}

void CDraftDrawView::OnUpdateBtEditRack(CCmdUI *pCmdUI){

	((CDraftDrawApp *)AfxGetApp())->OnUpdateBtEditRack(pCmdUI);
}

void CDraftDrawView::OnSelectNet() 
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CShape *pSh;

	int i;
	for(i=0;i<pDoc->m_pObArray->GetSize();i++){
		CShape *pSh1=(CShape *)(*pDoc->m_pObArray)[i];
		if(pSh1->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			if(pSh1->IsKindOf(RUNTIME_CLASS(CShapeWire)) ){
				pSh=pSh1;	
				break;
			}
		}
	}
	if(i>=pDoc->m_pObArray->GetSize()){
		//security: none found now
		return;
	}

	//Track all cable length
	//This loop should be transformed in wire iterators
	//to be exported to the rest of the program
	//==============================================================

	for(i=0;i<2;i++){
		CShape *pSh1=pSh;
		CShapeWire *pShWire=(CShapeWire *)pSh1;
		CString strIdent=pSh1->m_strIdent;
		CString	strTypeIdent=pSh1->m_strTypeIdent;
		CShape *pSh1Ant=NULL;
		do{
			if(pSh1Ant==pSh1){
				break;
			}

			pSh1Ant=pSh1;


			pShWire=(CShapeWire *)pSh1;
			CShape *pSh=(CShape *)pShWire->m_obarrShapearr[i];
CShape *pShdeb1=(CShape *)pShWire->m_obarrShapearr[0];
CShape *pShdeb2=(CShape *)pShWire->m_obarrShapearr[1];
			if((pSh)&&(pSh->IsKindOf(RUNTIME_CLASS(CShapePin)))){
				CShapePin *pShPin=(CShapePin *)pSh;
				if(pShPin->m_pshChildConn){
					pSh1=((CShapePin *)(pShPin->m_pshChildConn))->GetParent();
				}
				else{
					pSh1=NULL;
				}
				if((pShPin->m_bConect)&&(pSh1)&&(pSh1->IsKindOf(RUNTIME_CLASS(CShapeWire))) ){
					pSh1->m_Mode=_DRAFTDRAW_MODE_DRW;
					pSh1->m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;
					PlaceSelectRect(pSh1->m_MarkerArray,&pSh1->m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
				}
			}
			else{
				pSh1=NULL;
			}
		}while((pSh1)&&(pSh1->IsKindOf(RUNTIME_CLASS(CShapeWire))) );
	}
	//==============================================================

	Invalidate();
}

//Draw border of design area
void CDraftDrawView::DrawBorder(CDC *pDC){

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bFlag1=TRUE;

	//Added 28/11/2004
	//Special black and yellow border for part edit
	CBrush brush;
	//CBrush *brush_prev;
	if(pDoc->m_bFlagPartEdit){
		IMAGEINFO info;
		g_ImageListPatterns.GetImageInfo(0,&info);
		//CBitmap *bm=CBitmap::FromHandle(info.hbmImage);
		CBitmap bm;
		bm.LoadBitmap(IDB_PATTERN1_48);
		brush.CreatePatternBrush(&bm);
		//brush_prev=pDC->SelectObject(&brush);
	}

	CSize szDesign=pDoc->GetSize();

	CPoint pscrollpos=GetScrollPosition( );
	CRect rect;
	GetClientRect(&rect);

	//07/12/2004
	//This define refers to drawing the border just in client area or viewport of scroll view.
	//There is a problem in drawing the border just in the view port, becouse class CScrollView
	//automatically manages off screen area, so border is not properly drawn when user
	//scrolls the screen.
	//This problem is solved drawing the border for all the document, not only for the screen.
	//But, even if you draw the entire border, you can not use a pattern brush (bitmap) to fill
	//the border, becouse it seems as if Windows drawing engine restars the pattern at
	//view port beginning (is for optimization?).
//#define _DRAFTCABLE_DRAW_BORDER_CLIENT//only draw border in viewport
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
	int origin_x=pscrollpos.x*m_xScale;
	int limit_x=(pscrollpos.x+rect.BottomRight().x)*m_xScale;
	int origin_y=pscrollpos.y*m_yScale;
	int limit_y=(pscrollpos.y+rect.BottomRight().y)*m_yScale;
#else
	int origin_x=0;
	int limit_x=szDesign.cx+1;
	int origin_y=0;
	int limit_y=szDesign.cy+1;
#endif

	BOOL bLimitx=FALSE;
	if(limit_x>szDesign.cx){
		limit_x=szDesign.cx;
		bLimitx=TRUE;
	}
#define _DRAFTCABLE_DRAW_BORDER_COOR
	//Draw bottom border
	if(limit_y>szDesign.cy){
		limit_y=szDesign.cy;

		//Draw marks
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
		int idata=(origin_x/DCABLE_BORDERX_MARK+1)*DCABLE_BORDERX_MARK;
		int x_offset=idata-origin_x%DCABLE_BORDERX_MARK;
		int nIndex=origin_x/DCABLE_BORDERX_MARK+1;
		for(int j=x_offset;j<limit_x;j+=DCABLE_BORDERX_MARK){
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALL);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			if(j>0&&j<origin_y+DCABLE_BORDERY_MARK){
				rect=CRect(origin_x,limit_y-DCABLE_GRIDY_DEFAULT,j,limit_y);
			}
			else{
				rect=CRect(j-DCABLE_BORDERX_MARK,limit_y-DCABLE_GRIDY_DEFAULT,j,limit_y);
			}
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//if(bFlag1){
				//bFlag1=FALSE;
				pDC->FillRect(rect,&brush);
				//pDC->Rectangle(rect);
				//}
			}
			CString str;str.Format(_T("%i"),nIndex);
			pDC->TextOut(rect.CenterPoint().x,rect.TopLeft().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#else
		int nIndex=1;
		int j;
		for(j=origin_x+DCABLE_BORDERX_MARK;j<limit_x;j+=DCABLE_BORDERX_MARK){
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALL);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			rect=CRect(j-DCABLE_BORDERX_MARK,limit_y-DCABLE_GRIDY_DEFAULT,j,limit_y);
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//if(bFlag1){
				//bFlag1=FALSE;
				//rect.DeflateRect(1,1);
				if(nIndex%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
				//pDC->Rectangle(rect);
				//}
			}
			CString str;str.Format(_T("%i"),nIndex);
			pDC->SetBkMode(TRANSPARENT);
			pDC->TextOut(rect.CenterPoint().x,rect.TopLeft().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#endif
			pDC->MoveTo(j,limit_y);
			pDC->LineTo(j,limit_y-DCABLE_GRIDY_DEFAULT);
		}
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
		//if(j%DCABLE_BORDERX_MARK){
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);

			rect=CRect(j-DCABLE_BORDERX_MARK,limit_y-DCABLE_GRIDY_DEFAULT,limit_x,limit_y);

			//We are editing a part
			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if(nIndex%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			//pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
		//}
#endif
		pDC->MoveTo(origin_x,limit_y);
		pDC->LineTo(limit_x,limit_y);
		pDC->MoveTo(origin_x+DCABLE_GRIDX_DEFAULT,limit_y-DCABLE_GRIDY_DEFAULT);
		pDC->LineTo(limit_x-DCABLE_GRIDX_DEFAULT,limit_y-DCABLE_GRIDY_DEFAULT);
	}

	//Draw right border
	if(bLimitx){

		//Draw marks
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
		int idata=(origin_y/DCABLE_BORDERY_MARK+1)*DCABLE_BORDERY_MARK;
		int y_offset=idata-origin_y%DCABLE_BORDERY_MARK;
		char nIndex='A'+origin_y/DCABLE_BORDERY_MARK;
		for(int j=y_offset;j<limit_y;j+=DCABLE_BORDERY_MARK){
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			if(j>0&&j<origin_y+DCABLE_BORDERY_MARK){
				rect=CRect(limit_x-DCABLE_GRIDX_DEFAULT,origin_y,limit_x,j);
			}
			else{
				rect=CRect(limit_x-DCABLE_GRIDX_DEFAULT,j-DCABLE_BORDERY_MARK,limit_x,j);
			}
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#else
		char nIndex='A';
		int j;
		for(j=origin_y+DCABLE_BORDERY_MARK;j<limit_y;j+=DCABLE_BORDERY_MARK){
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			rect=CRect(limit_x-DCABLE_GRIDX_DEFAULT,j-DCABLE_BORDERY_MARK,limit_x,j);
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
			}
			CString str;str.Format(_T("%c"),nIndex);
			pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#endif
			pDC->MoveTo(limit_x,j);
			pDC->LineTo(limit_x-DCABLE_GRIDX_DEFAULT,j);
		}
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
		//if(j%DCABLE_BORDERY_MARK){
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);

			rect=CRect(limit_x-DCABLE_GRIDX_DEFAULT,j-DCABLE_BORDERY_MARK,limit_x,limit_y);

			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			//pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
		//}
#endif

		pDC->MoveTo(limit_x,limit_y);
		pDC->LineTo(limit_x,origin_y);
		pDC->MoveTo(limit_x-DCABLE_GRIDX_DEFAULT,limit_y-DCABLE_GRIDY_DEFAULT);
		pDC->LineTo(limit_x-DCABLE_GRIDX_DEFAULT,origin_y+DCABLE_GRIDY_DEFAULT);
	}

	//Draw left border
	if(origin_x==0){
		//Draw marks
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
		int idata=(origin_y/DCABLE_BORDERY_MARK+1)*DCABLE_BORDERY_MARK;
		int y_offset=idata-origin_y%DCABLE_BORDERY_MARK;
		char nIndex='A'+origin_y/DCABLE_BORDERY_MARK;
		for(int j=y_offset;j<limit_y;j+=DCABLE_BORDERY_MARK){
			//Draw rectangle
			//******************************************************************
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			if(j>0&&j<origin_y+DCABLE_BORDERY_MARK){
				rect=CRect(origin_x,origin_y,origin_x+DCABLE_GRIDX_DEFAULT,j);
			}
			else{
				rect=CRect(origin_x,j-DCABLE_BORDERY_MARK,origin_x+DCABLE_GRIDX_DEFAULT,j);
			}
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#else
		char nIndex='A';
		int j;
		for(j=origin_y+DCABLE_BORDERY_MARK;j<limit_y;j+=DCABLE_BORDERY_MARK){
			//Draw rectangle
			//******************************************************************
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			rect=CRect(origin_x,j-DCABLE_BORDERY_MARK,origin_x+DCABLE_GRIDX_DEFAULT,j);
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(1,1);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
#endif
			pDC->MoveTo(origin_x,j);
			pDC->LineTo(origin_x+DCABLE_GRIDX_DEFAULT,j);
			//******************************************************************
		}
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
		//if(j%DCABLE_BORDERY_MARK){
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);

			rect=CRect(origin_x,j-DCABLE_BORDERY_MARK,origin_x+DCABLE_GRIDX_DEFAULT,limit_y);

			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			//pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
		//}
#endif

		pDC->MoveTo(origin_x,origin_y);
		pDC->LineTo(origin_x,limit_y);
		pDC->MoveTo(origin_x+DCABLE_GRIDX_DEFAULT,origin_y+DCABLE_GRIDY_DEFAULT);
		pDC->LineTo(origin_x+DCABLE_GRIDX_DEFAULT,limit_y-DCABLE_GRIDY_DEFAULT);
	}

	//Draw top border
	if(origin_y==0){
		//Draw marks
#ifdef _DRAFTCABLE_DRAW_BORDER_CLIENT
		int idata=(origin_x/DCABLE_BORDERX_MARK+1)*DCABLE_BORDERX_MARK;
		int x_offset=idata-origin_x%DCABLE_BORDERX_MARK;
		int nIndex=origin_x/DCABLE_BORDERX_MARK+1;
		for(int j=x_offset;j<limit_x;j+=DCABLE_BORDERX_MARK){
			//Draw rectangle
			//******************************************************************
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALL);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			if(j>0&&j<origin_y+DCABLE_BORDERY_MARK){
				rect=CRect(origin_x,origin_y,j,origin_y+DCABLE_GRIDY_DEFAULT);
			}
			else{
				rect=CRect(j-DCABLE_BORDERX_MARK,origin_y,j,origin_y+DCABLE_GRIDY_DEFAULT);
			}
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//if(bFlag1){
				//bFlag1=FALSE;
				pDC->FillRect(rect,&brush);
				//pDC->Rectangle(rect);
				//}
			}
			CString str;str.Format(_T("%i"),nIndex);
			pDC->TextOut(rect.CenterPoint().x,rect.TopLeft().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
			//******************************************************************
#else
		int nIndex=1;
		int j;
		for(j=origin_x+DCABLE_BORDERX_MARK;j<limit_x;j+=DCABLE_BORDERX_MARK){
			//Draw rectangle
			//******************************************************************
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
			CFont *pFont=AfxGetFont(AFX_FONT_SMALL);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);
			rect=CRect(j-DCABLE_BORDERX_MARK,origin_y,j,origin_y+DCABLE_GRIDY_DEFAULT);
			//Added 28/11/2004
			//Draws pattern border for part edition
			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%i"),nIndex);
			pDC->TextOut(rect.CenterPoint().x,rect.TopLeft().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
#endif
			//******************************************************************
#endif
			pDC->MoveTo(j,origin_y);
			pDC->LineTo(j,origin_y+DCABLE_GRIDY_DEFAULT);
		}
#ifdef _DRAFTCABLE_DRAW_BORDER_COOR
		//if(j%DCABLE_BORDERX_MARK){
			CFont *pFont=AfxGetFont(AFX_FONT_SMALLVER);
			CRect rect;
			CFont *prevFont=pDC->SelectObject(pFont);

			rect=CRect(j-DCABLE_BORDERX_MARK,origin_y,limit_x,origin_y+DCABLE_GRIDY_DEFAULT);

			if(pDoc->m_bFlagPartEdit){
				//rect.DeflateRect(2,2);
				if((nIndex-'A')%2){
					pDC->FillSolidRect(rect,RGB(255,255,0));
				}
				else{
					pDC->FillSolidRect(rect,RGB(0,0,0));
				}
				pDC->SetBkColor(RGB(255,255,255));
				//pDC->FillRect(rect,&brush);
			}
			CString str;str.Format(_T("%c"),nIndex);
			//pDC->TextOut(rect.TopLeft().x,rect.CenterPoint().y,str);
			pDC->SelectObject(prevFont);
			nIndex++;
		//}
#endif

		pDC->MoveTo(limit_x,origin_y);
		pDC->LineTo(origin_x,origin_y);
		pDC->MoveTo(limit_x-DCABLE_GRIDX_DEFAULT,origin_y+DCABLE_GRIDY_DEFAULT);
		pDC->LineTo(origin_x+DCABLE_GRIDX_DEFAULT,origin_y+DCABLE_GRIDY_DEFAULT);
	}

	//Added 28/11/2004
	if(pDoc->m_bFlagPartEdit){
		//pDC->SelectObject(&brush_prev);
	}
}

//void CDraftDrawView::OnChangedTab(NMFOLDERTAB* nmtab, LRESULT* pRes)
void CDraftDrawView::OnChangedTab(NMHDR* nmtab, LRESULT* pRes)
{
	int i=0;
	//OnChangedFolder(nmtab->iItem);

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//pDoc->ChangeSheet(nmtab->iItem);
	pDoc->ChangeSheet(nmtab->idFrom);

	Invalidate();
}

BOOL CDraftDrawView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(0&&m_bGrid){
		m_bGridTemp=FALSE;
		SetTimer(1,100,NULL);
	}
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

//Calculate scroll rect using member m_ptScrollPrev
void CDraftDrawView::GetScrollRect(CRect &rectUpdate){

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int origin_x;
	int limit_x;
	int origin_y;
	int limit_y;

	CPoint pscrollpos=GetScrollPosition( );
	CRect rect;
	GetClientRect(&rect);

/*
int nMapMode;
SIZE sizeTotal;
SIZE sizePage;
SIZE sizeLine;
GetDeviceScrollSizes( nMapMode, sizeTotal, sizePage, sizeLine );
*/

	origin_x=pscrollpos.x*m_xScale;
	limit_x=(pscrollpos.x+rect.BottomRight().x)*m_xScale;
	origin_y=pscrollpos.y*m_yScale;
	limit_y=(pscrollpos.y+rect.BottomRight().y)*m_yScale;

	CSize szDesign=pDoc->GetSize();

	if(limit_x>szDesign.cx){
		limit_x=szDesign.cx;
	}

	if(limit_y>szDesign.cy){
		limit_y=szDesign.cy;
	}

	//Calculate offset
	if(m_ptScrollPrev.x==pscrollpos.x){
		rectUpdate=CRect(CPoint(pscrollpos.x,rect.bottom-pscrollpos.y),CSize(rect.BottomRight().x,m_ptScrollPrev.y-pscrollpos.y));
	}
	else{
		rectUpdate=CRect(CPoint(pscrollpos.x,pscrollpos.y),CSize(m_ptScrollPrev.x-pscrollpos.x,rect.BottomRight().y));
	}
	rectUpdate.NormalizeRect();

	//Update memory
	m_ptScrollPrev=pscrollpos;
}

//Code to test rotations.
//This code works properly.
//Transformations can be used to: rotate, scale, reflect, shear (deform throw an axis).
//The idea is to change transformation for each shape being drawn according
//with its parameters.
void CDraftDrawView::DoWorldTransform(CShape *pSh,HDC hdc){


	//Sample
	// Rotate 30 degrees counterclockwise.
	/*
    xForm.eM11 = (FLOAT) 0.8660; 
    xForm.eM12 = (FLOAT) 0.5000; 
    xForm.eM21 = (FLOAT) -0.5000; 
    xForm.eM22 = (FLOAT) 0.8660; 
    xForm.eDx  = (FLOAT) 0.0; 
    xForm.eDy  = (FLOAT) 0.0; 
    SetWorldTransform(pDC->m_hDC, &xForm); 
	*/

	XFORM x;
	CPoint pivot=pSh->m_Rect.CenterPoint();

	/* translate objects from pivot point to origin. */
	x.eM11 = (float)1.0;
	x.eM12 = (float)0.0;
	x.eM21 = (float)0.0;
	x.eM22 = (float)1.0;
	x.eDx = (float)-pivot.x;
	x.eDy = (float)-pivot.y;
	SetWorldTransform (hdc, &x);

	/* rotate object about origin. */
	x.eM11 = pSh->m_eM11;
	x.eM12 = pSh->m_eM12;
	x.eM21 = pSh->m_eM21;
	x.eM22 = pSh->m_eM22;
	x.eDx = (float)0.0;
	x.eDy = (float)0.0;
	ModifyWorldTransform (hdc, &x, MWT_RIGHTMULTIPLY);

	/* translate objects back to pivot point. */
	x.eM11 = (float)1.0;
	x.eM12 = (float)0.0;
	x.eM21 = (float)0.0;
	x.eM22 = (float)1.0;
	x.eDx = (float)pivot.x;
	x.eDy = (float)pivot.y;
	ModifyWorldTransform (hdc, &x, MWT_RIGHTMULTIPLY);
}

BOOL CDraftDrawView::IsPrinting(CDC *pDC){

	if(m_bSaveDC){

		return TRUE;
	}

	return pDC->IsPrinting();
}
void CDraftDrawView::OnViewGraphDen() 
{
	// TODO: Add your command handler code here
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDialogGraphDen dlgGraph;
	dlgGraph.m_pShArray=pDoc->m_pShArray;
	if(dlgGraph.DoModal()){};
}

//Transform a point in client coordinates to schematic coordinates
void CDraftDrawView::ClientToSchematic(LPPOINT lpPoint ) const{

	//SCALE POINT
	CPoint point_ns=*lpPoint;
	*lpPoint=CPoint(m_xScale*lpPoint->x,m_yScale*lpPoint->y)+
		CPoint(m_xScale*GetScrollPosition().x,m_yScale*GetScrollPosition().y);

}

//Snap to grid. Coordinates are schematic.
void CDraftDrawView::SnapToGrid(LPPOINT lpPoint ){

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize m_szGrid=pDoc->m_szGrid;

	//SNAP TO GRID
	if(m_bGrid){		
		lpPoint->x=((int)((lpPoint->x+m_szGrid.cx*.5)/m_szGrid.cx))*m_szGrid.cx;
		lpPoint->y=((int)((lpPoint->y+m_szGrid.cy*.5)/m_szGrid.cy))*m_szGrid.cy;
	}
}

//Snap to grid. Coordinates are schematic.
void CDraftDrawView::SnapToGridFloor(LPPOINT lpPoint ){

	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize m_szGrid=pDoc->m_szGrid;

	//SNAP TO GRID
	if(m_bGrid){		
		lpPoint->x=((int)((lpPoint->x)/m_szGrid.cx))*m_szGrid.cx;
		lpPoint->y=((int)((lpPoint->y)/m_szGrid.cy))*m_szGrid.cy;
	}
}

//Prepare a buffer for drawing operations (avoid blinking)
void CDraftDrawView::PrepareBuffer(CDraftDrawDoc* pDoc,CDC* pDC)
{
	if(m_FlagBuffer1==false){

		//Get document size
		CSize size=pDoc->GetSize();

		//Create a buffer and set to white
		__int32 *Bits=new __int32[size.cx*size.cy];
		memset(Bits,0xff,size.cx*size.cy*sizeof(__int32));

		//Create bitmap 1 plane and 32 bit
		m_bmpCanvas1->CreateBitmap( size.cx, size.cy, 1, 32, Bits );

		//Delete temporal buffer
		delete(Bits);

		int result=pDC->GetDeviceCaps(/*TECHNOLOGY*/RASTERCAPS);

		//Construct device context
		pDC1=new CDC;

		//Initialize device context
		result=pDC1->CreateCompatibleDC( pDC );
		result=pDC1->GetDeviceCaps(/*TECHNOLOGY*/RASTERCAPS);
		pDC1->SelectObject(m_bmpCanvas1);

		//Create a buffer and set to white
		Bits=new __int32[size.cx*size.cy];
		memset(Bits,0xff,size.cx*size.cy*sizeof(__int32));

		//Create bitmap 1 plane and 32 bit
		m_bmpCanvas2->CreateBitmap( size.cx, size.cy, 1, 32, Bits );

		//2nd buffer
		pDC2=new CDC;

		//Initialize device context
		result=pDC2->CreateCompatibleDC( pDC );
		result=pDC2->GetDeviceCaps(/*TECHNOLOGY*/RASTERCAPS);
		pDC2->SelectObject(m_bmpCanvas2);

		//Set buffer as initiallized
		m_FlagBuffer1=true;

		m_PointPrev=CPoint(-1,-1);

	}// end if m_FlagBuffer1
}

//Delete an object from the list of select object
void CDraftDrawView::DeleteSelected(CObject *pOb){
	POSITION pos=m_ObListSel.Find(pOb);
	while(pos!=NULL)
	{
		m_ObListSel.RemoveAt(pos);
		pos=m_ObListSel.Find(pOb);
	}
}

//Prevent shape out of schematic
void CDraftDrawView::DoPreventShpOutSchem(CShape *pSh, CRect rcSheet){
	//Horizontal
	if(pSh->m_Rect.right>rcSheet.right){
		pSh->m_Rect -= CPoint(pSh->m_Rect.right-rcSheet.right, 0);
		CPoint pt=pSh->m_Rect.TopLeft();
		SnapToGridFloor(&pt);
		pSh->m_Rect=new CRect(pt, pSh->m_Rect.Size());
	}
	else if(pSh->m_Rect.left<rcSheet.left){
		pSh->m_Rect += CPoint(rcSheet.left-pSh->m_Rect.left, 0);
	}
	//Vertical
	if(pSh->m_Rect.bottom>rcSheet.bottom){
		pSh->m_Rect -= CPoint(0,pSh->m_Rect.bottom-rcSheet.bottom);
		CPoint pt=pSh->m_Rect.TopLeft();
		SnapToGridFloor(&pt);
		pSh->m_Rect=new CRect(pt, pSh->m_Rect.Size());
	}
	else if(pSh->m_Rect.top<rcSheet.top){
		pSh->m_Rect += CPoint(0,rcSheet.top-pSh->m_Rect.top);
	}
}

//pSh				Pointer to shape yo test
//point				point to test
//nFlags			passed to OnLButtonDown for example
//bConnectionTmp	set if a temp connection is being made
//indexPin			index of pDoc->m_pSh being connected
//pShContConnect	shape that pDoc->m_pSh is being connected to
BOOL CDraftDrawView::_DoGetConnectionTmp(CShape *pSh, CPoint point, UINT nFlags,
	BOOL& bConnectionTmp,
	int& indexPin,
	CShapeContainer** pShContConnect
	)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Different treatment should be done to wires and units:
	//Wires: hit point = point
	//Units: all unit hit point should be tested.
	if ((pSh->GetRuntimeClass())->IsDerivedFrom(RUNTIME_CLASS(CShapeContainer))){

		//BOOL _DoGetConnectionTmp(CShape *pSh);
		CShapeContainer *pShContainer = (CShapeContainer *)pSh;

		if (pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){
			//19/01/2005: new connecting mechanism
			if (!bConnectionTmp){
				//Get previous connecting point
				CPoint ptConnectPrev = GetConnectPrev();
				//If point is same that previous one, do not reconnect on itself
				if (point != ptConnectPrev){
					//Test connection
					bConnectionTmp = pShContainer->PtInRect(&point, pShContConnect);
					if (bConnectionTmp){
						indexPin = SHAPECONTAINER_CONNECTING_PIN_WIRE;
					}
				}
				else{
					return TRUE;
				}
			}
		}
		else if (pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
			CShapeUnit *pShUnit = (CShapeUnit *)pDoc->m_pSh;
			//This code is suspect not been completelly implemented

			//==========================================================================

			for (int i = 0; i<pShUnit->m_obarrShapearr.GetSize(); i++){

				CShape *psh = (CShape *)pShUnit->m_obarrShapearr.GetAt(i);

				if (psh->IsKindOf(RUNTIME_CLASS(CShapePin))){

					CShapePin *pShPin = (CShapePin *)psh;

					if (!pShPin->m_pshChildConn){

						//Connection rectangles are relative to container.
						CPoint hitPoint = CPoint(pShPin->m_rectConect.CenterPoint() + pShUnit->m_Rect.TopLeft());

						pShContainer->m_bConnectMake = TRUE;
						pSh->OnLButtonDown(nFlags, hitPoint);

						if (pSh->m_pshChildConn){
							pShUnit->m_pshChildConn = pShPin->m_pshChildConn;
							indexPin = SHAPECONTAINER_CONNECTING_PIN_UNIT;
						}
					}
				}
			}

			//==========================================================================
		}
	}

	return FALSE;
}

//Add a new shape to stack
void CDraftDrawView::_DoAddNewShapeToStack(UINT nFlags, CPoint point)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//This should be changed to sub-method
	int index = pDoc->m_ObArray.GetUpperBound();
	if (index > -1){
		if ((CShape*)pDoc->m_ObArray[index] != pDoc->m_pSh
			|| pDoc->m_iToolType == _TOOLTYPEPOLY_DRAFTCABLE){
			if ((CShape*)pDoc->m_ObArray[index] != pDoc->m_pSh
				)
				pDoc->AddObject(pDoc->m_pSh);
			pDoc->m_pSh->OnLButtonDown(nFlags, point);
		}
	}
	else{
		pDoc->AddObject(pDoc->m_pSh);
		pDoc->m_pSh->OnLButtonDown(nFlags, point);
	}
}

BOOL  CDraftDrawView::_DoGetConnectionTmp(LPPOINT point, CShapeContainer** pShContConnect, CShape* pSh, BOOL bConnectionTmp)
{
	if ((pSh->GetRuntimeClass())->IsDerivedFrom(RUNTIME_CLASS(CShapeContainer))){

		CShapeContainer *pShContainer = (CShapeContainer *)pSh;
		pShContainer->m_bConnectMake = TRUE;

		//19/01/2005: new connecting mechanism
		if (!bConnectionTmp)
			bConnectionTmp = pShContainer->PtInRect(point, pShContConnect);
	}

	return bConnectionTmp;
}

void CDraftDrawView::_DoCalculateWindowViewPortExtend(CSize viewport, CRect window)
{
	CSize szVPext;
	if (viewport.cx < window.Width()){
		szVPext.cx = window.Width() / viewport.cx;
	}
	else{
		szVPext.cx = 1;
		float fdata = viewport.cx;
		fdata /= window.Width();
		m_szWext.cx = ceil(fdata);
	}

	if (viewport.cy < window.Height()){
		szVPext.cy = window.Height() / viewport.cy;
	}
	else{
		szVPext.cy = 1;
		float fdata = viewport.cy;
		fdata /= window.Height();
		m_szWext.cy = ceil(fdata);
	}

	//Process results
	if (m_szWext.cx == 1 && m_szWext.cy == 1){
		(szVPext.cx<szVPext.cy ? m_szVPext.cx = m_szVPext.cy = szVPext.cx : m_szVPext.cx = m_szVPext.cy = szVPext.cy);
	}
	else{

		m_szVPext = CSize(1, 1);
		(m_szWext.cx<m_szWext.cy ? m_szWext.cx = m_szWext.cy : m_szWext.cy = m_szWext.cx);
	}
}

void CDraftDrawView::_DoZoom(CShape* pSh)
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;

	CSize szDesign = pDoc->GetSize();

	SetScrollSizes(MM_TEXT,
		CSize(szDesign.cx*m_szVPext.cx, szDesign.cy*m_szVPext.cx),
		CSize(100 * m_szVPext.cx, 100 * m_szVPext.cx),
		CSize(10 * m_szVPext.cx, 10 * m_szVPext.cx));

	//Recalcule scales.
	if (m_szWext.cx == 1 && m_szWext.cy == 1){
		m_xScale = 1.0 / m_szVPext.cx;
		m_yScale = 1.0 / m_szVPext.cy;
	}
	else{
		m_xScale = m_szWext.cx;
		m_yScale = m_szWext.cy;
	}

	//Origin offset
	//Drawing is shift so as to center of viewport aligns with
	//zoom rectangle center.
	//int origin_x=pscrollpos.x*m_xScale;
	//int origin_y=pscrollpos.y*m_yScale;
	CPoint pscrollpos;
	pscrollpos.x = pSh->m_Rect.CenterPoint().x / m_xScale;
	pscrollpos.y = pSh->m_Rect.CenterPoint().y / m_yScale;
	GetClientRect(&rect);
	CPoint point = rect.CenterPoint();
	pscrollpos = -CPoint(point.x, point.y) + pscrollpos;//no scale needed for center point!

	//Final check:
	//origin is not offset if design fits in window.
	szDesign = pDoc->GetSize();
	szDesign = CSize(szDesign.cx / m_xScale, szDesign.cy / m_yScale);
	(rect.Width()>szDesign.cx ? pscrollpos.x = 0 : pscrollpos.x);
	(rect.Height()>szDesign.cy ? pscrollpos.y = 0 : pscrollpos.y);

	//Execute offset
	ScrollToPosition(pscrollpos);
}

BOOL CDraftDrawView::_DoAddToUpdateRect(CRect& rectUpdate, CRect rectToAdd)
{
	//Local variable
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Calculate sheet area
	//=======================================================
	CSize szDesign = pDoc->GetSize();
	CRect rcSheet = CRect(CPoint(0, 0), szDesign);
	rcSheet.DeflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
	//=======================================================

	rectToAdd.NormalizeRect();
	//Inflate update rectangle
	rectToAdd.InflateRect(DCABLE_PADDINGX_DEFAULT, DCABLE_PADDINGY_DEFAULT);
	//Intersect with sheet
	rectToAdd.IntersectRect(rectToAdd, rcSheet);
	//Add to update rectangle
	rectUpdate.UnionRect(rectUpdate, rectToAdd);
	return FALSE;
}

void CDraftDrawView::_SetUpdateRect(CRect rectUpdate)
{
	CRect rect = rectUpdate;

	rect.UnionRect(rect, m_RectPrev);

	//Scale update rectangle to current coordinates
	CPoint point1 = CPoint(rect.TopLeft().x / m_xScale, rect.TopLeft().y / m_yScale) - GetScrollPosition();
	CSize size = CSize(rect.Width() / m_xScale, rect.Height() / m_yScale);

	m_RectDraw = rect;
	m_RectPrev = rectUpdate;//keep last union rect

	InvalidateRect(CRect(point1, size), FALSE);
}

CRect CDraftDrawView::_GetUpdateRect()
{
	return m_RectDraw;
}

CRect CDraftDrawView::_GetScaledUpdateRect(CRect rect)
{
	//Scale update rectangle to current coordinates
	CPoint point = CPoint(rect.TopLeft().x / m_xScale, rect.TopLeft().y / m_yScale) - GetScrollPosition();

	CSize size = CSize(rect.Width() / m_xScale, rect.Height() / m_yScale);

	return CRect(point, size);
}

void CDraftDrawView::_AddToSelectedArray(CShape *pSh)
{
	if (pSh->IsSelected()){
		POSITION pos = m_ObListSel.Find(pSh);
		if (pos == NULL)
			m_ObListSel.AddTail(pSh);
	}
}