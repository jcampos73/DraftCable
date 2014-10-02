///////////////////////////////////////////////////////////////////////////////
//Shapes Module
//Classes of shapes.
//Following shapes included:
//
//Version:
//	1.00.0000
//	07/01/2006
//(Ed./Rev.)Author:
//	00/00	J.Campos	07/01/2006

// Shape.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "Gdiplusgraphics.h"

#include "DraftDraw.h"
#include "Shape.h"
//#include "ShapeUnit.h"

#include "EllipseCore.h"

//Dialogs
#include "DialogDebugShpInfo.h"
#include "DialogFillShape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Constructor template
/*

1.Main variables defaults

2.Flags defaults

3.Variables for 3D

4.Buffers/Pointers

5.Initialize pointer to parse function

6.Initialize rect & id

*/

/////////////////////////////////////////////////////////////////////////////
// CShape
/*IMPLEMENT_DYNCREATE(CShape, CObject)*/
IMPLEMENT_SERIAL(CShape, CObject, 1)

CShape::CShape(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/)
{
	//Main variables defaults
	m_strIdent="";
	m_strTypeIdent="";
	m_uiShapeId=0;
	m_Type=0;
	m_TypeSelect=0;
	m_Mode=0;

	//2.Flags defaults
	m_bNoResize=FALSE;
	m_bLineResize=FALSE;
	m_bTransparent=TRUE;
	m_bAllowsPartialDraw=FALSE;
	m_bConect=FALSE;
	m_bDrawDashedRect=TRUE;

	//3.Variables for 3D
	m_Height=20;
	m_VectorProyect=CPoint3D(0,0,1);
	m_PointOrg=CPoint(0,0);

	//4.Buffers/Pointers
	m_pshChildConn=NULL;
	m_pshConn=NULL;
	ZeroMemory(m_MarkerArray,sizeof(int)*_DRAFTDRAW_SEL_MARKERMAX);

	//5.Initialize pointer to parse function
	/*pfParser=NULL;*/
	pcmdDeque=cmddq;
	m_uiShapeType=ddcShape;

	//6.Initialize rect, id, angle
	if(lpRect){
		m_Rect=lpRect;
	}
	if(nId){
		m_uiShapeId=nId;
	}
	m_fAngle=0.0;					//Rotation angle of shape
	m_eM11=(FLOAT)1.0;				//Cosine of rotation angle
	m_eM12=(FLOAT)0;				//Sine of the rotation angle
	m_eM21=(FLOAT)0;				//Negative sine of the rotation angle
	m_eM22=(FLOAT)1.0;				//Cosine of rotation angle
	m_crFill = RGB(255, 255, 255);
	m_crFillBgnd = RGB(255, 255, 255);
}

//Copy constructor
CShape::CShape(CShape& Sh){
	m_strIdent="";
	m_strTypeIdent="";
	m_uiShapeId=0;

	m_Type=0;
	m_TypeSelect=0;
	m_Mode=0;

	//Flags
	m_bNoResize=FALSE;
	m_bLineResize=FALSE;
	m_bTransparent=TRUE;
	m_bAllowsPartialDraw=FALSE;
	m_bConect=Sh.m_bConect;
	m_bDrawDashedRect=TRUE;

	m_Height=20;

	m_VectorProyect=CPoint3D(0,0,1);

	m_PointOrg=CPoint(0,0);

	m_pshChildConn=NULL;
	m_pshConn=NULL;

	ZeroMemory(m_MarkerArray,sizeof(int)*_DRAFTDRAW_SEL_MARKERMAX);

	//Initialize pointer to parse function
	/*pfParser=NULL;*/
	pcmdDeque=Sh.pcmdDeque;
	m_uiShapeType=ddcShape;

	//Initialize rect & id

	m_Rect=Sh.m_Rect;

	m_uiShapeId=Sh.m_uiShapeId;

	m_fAngle=Sh.m_fAngle;				//Rotation angle of shape
	m_eM11=(FLOAT)Sh.m_eM11;			//Cosine of rotation angle
	m_eM12=(FLOAT)Sh.m_eM12;			//Sine of the rotation angle
	m_eM21=(FLOAT)Sh.m_eM21;			//Negative sine of the rotation angle
	m_eM22=(FLOAT)Sh.m_eM22;			//Cosine of rotation angle
}

CShape::~CShape()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShape implementation
CShape& CShape::operator=( const CShape& Sh ){
	//03/01/2005
	/*
	m_Mode=Sh.m_Mode;
	m_TypeSelect=Sh.m_TypeSelect;
	m_bConect=Sh.m_bConect;
	*/
	m_Rect0=Sh.m_Rect0;
	m_bNoResize=Sh.m_bNoResize;
	m_uiShapeType=Sh.m_uiShapeType;

	m_fAngle=Sh.m_fAngle;				//Rotation angle of shape
	m_eM11=(FLOAT)Sh.m_eM11;			//Cosine of rotation angle
	m_eM12=(FLOAT)Sh.m_eM12;			//Sine of the rotation angle
	m_eM21=(FLOAT)Sh.m_eM21;			//Negative sine of the rotation angle
	m_eM22=(FLOAT)Sh.m_eM22;			//Cosine of rotation angle

	return *this;
}
CShape& CShape::operator++( ){
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers

BOOL CShape::OnCommand( WPARAM wParam, LPARAM lParam ){

	if (m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT){
		switch (LOWORD(wParam)){
		case ID_IMG_FLIPHORZ:
			Flip(0.0);
			return TRUE;
			break;
		case ID_IMG_FLIPVERT:
			Flip(90.0);
			return TRUE;
			break;
		case ID_ROTATE:
			Rotate(30.0);
			return TRUE;
			break;
		}

		//Format shape
		switch (LOWORD(wParam)){
		case ID_EDIT_FORMATSHAPE:
			CDialogFillShape pdlgFill = new CDialogFillShape();
			if (pdlgFill.DoModal() == IDOK){
				m_crFill = pdlgFill.m_crCurrent;
				m_crFillBgnd = pdlgFill.m_crCurrentBgnd;
				m_bTransparent = FALSE;
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

CShape *CShape::SetConnection(CShape *pSh,int index){

	return NULL;
}

CObject *CShape::LoadUnit(LPCTSTR lpUnitName,CArchive *ar){


	return NULL;

}
void CShape::OnLButtonDblClk(UINT nFlags, CPoint point)
{
}
void CShape::OnLButtonDown(UINT nFlags, CRect rect)
{
	//Normal shapes
	if(!m_Rect.IsRectEmpty()){
		CRect rectInter;
		rectInter.IntersectRect(m_Rect,rect);
		if(rectInter==m_Rect){
			OnLButtonDown(nFlags, m_Rect.CenterPoint());
			OnLButtonUp(nFlags, m_Rect.CenterPoint());
		}
		else{
			int i=0;
		}
	}
	//Line shapes
	else{
		CPoint top_left=m_Rect.TopLeft();
		CPoint bottom_right=m_Rect.BottomRight();
		if(rect.PtInRect(top_left)&&rect.PtInRect(bottom_right)){
			OnLButtonDown(nFlags, m_Rect.CenterPoint());
			OnLButtonUp(nFlags, m_Rect.CenterPoint());
		}
		else{
			int i=0;
		}
	}
}
void CShape::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i;

	//Transform
	//==========================================================================
	if(m_eM11!=1.0||m_eM21!=0.0){
		point-=m_Rect.CenterPoint();
		int x=point.x*m_eM11-point.y*m_eM21;
		int y=point.x*m_eM21+point.y*m_eM11;
		point=CPoint(x,y);
		point+=m_Rect.CenterPoint();
	}
	//==========================================================================

	m_Point=point;

	if(m_Mode==_DRAFTDRAW_MODE_DRW){

		i=0;
		if(!m_bNoResize)
		while(m_MarkerArray[i]){

			//15/08/2004
			//This line copes with the fact that line are allway drawn from NW corner to SE corner.
			//When line is completely horizontal or vertical, NE marker is hit first than SE,
			//becouse NW<SE.
			//SW/NW case has been added as a safety measure.
			if(!m_bLineResize||(i!=_DRAFTDRAW_SEL_RESIZING_RECT_NE && i!=_DRAFTDRAW_SEL_RESIZING_RECT_SW)){

				if(m_MarkerArray[i]->PtInRect(point)==TRUE){

					m_Mode=_DRAFTDRAW_MODE_SEL;
					m_RectLast=m_Rect;
					m_TypeSelect=_DRAFTDRAW_SEL_RESIZING_RECT;
					m_VectorSelect=i;
					SetCursor(m_pCursorArray[i]);
					return;
				}
			}

			i++;
		}

			CRect rect=m_Rect;
			rect.NormalizeRect();

		if(rect.PtInRect(point)==TRUE){

			//Added 28/11/2004
			//Add to command queue shape creation
			//******************************************************************

			if(pcmdDeque){
				if(m_Mode!=_DRAFTDRAW_MODE_SEL){
					CString str;
					str.Format("ActiveSheet.Shapes(%i).Select(1)",
						m_uiShapeId);
					std::string strComm=str;
					
					str.Format("ActiveSheet.Shapes(%i).Select(0)",m_uiShapeId);
					std::string strCommUndo=str;

					pcmdDeque->push_cmd(strComm,strCommUndo);
				}
			}

			//******************************************************************
			m_Mode=_DRAFTDRAW_MODE_SEL;
			m_RectLast=m_Rect;
			m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;

			//if(!m_bNoResize)
				PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);

			SetCursor(m_pCursorArray[8]);
		}
		else if(rect.IsRectEmpty()==TRUE){
/*
			CRect rect=m_Rect;
			rect.NormalizeRect();
*/
			CPoint point1=rect.TopLeft();
			CPoint point2=rect.BottomRight();

			if((point1.x==point2.x)&&
			(point.x==point1.x)&&
			(point.y>=point1.y)&&
			(point.y<point2.y)){

				m_Mode=_DRAFTDRAW_MODE_SEL;
				m_RectLast=m_Rect;
				m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;

				//if(!m_bNoResize)
					PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);

				SetCursor(m_pCursorArray[8]);
			}
			else if((point.y==point1.y)&&
			(point.y==point2.y)&&
			(point.x>=point1.x)&&
			(point.x<point2.x)){

				m_Mode=_DRAFTDRAW_MODE_SEL;
				m_RectLast=m_Rect;
				m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;

				//if(!m_bNoResize)
					PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);

				SetCursor(m_pCursorArray[8]);
			}
			else{
				int i=0;
			}
		}//end if m_Rect empty
	}
	else
	switch(m_TypeSelect){

	case _DRAFTDRAW_SEL_SIZING_RECT:

		m_Point=point;

		if(!m_bNoResize){
		m_Rect=CRect(point,point);
		}
		else{
		//m_Rect+=point;
		}

		break;

	case _DRAFTDRAW_SEL_MOVING_RECT:


		break;


	}


}
//use this function to switch between selecting and drawing
void CShape::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
	// code afecting bounding rectangle can be place in base class
	//CShape::OnLButtonUp(UINT nFlags, CPoint point);


	//m_Mode=_DRAFTDRAW_MODE_DRW;

	SetCursor(m_pCursorArray[9]);
	if(!m_Mode){
	switch(m_TypeSelect){

	case _DRAFTDRAW_SEL_SIZING_RECT:

		m_Mode=_DRAFTDRAW_MODE_DRW;
		//Added 27/11/2004
		//Add to command queue shape creation
		//**********************************************************************
		if(pcmdDeque&&!IsPoly()){
			CString str;
			CStringArray saData;
			GetData(saData);
			if(saData.GetSize()==0){

				str.Format("ActiveSheet.Shapes.AddShape(%i, %i, %i, %i, %i, %i).Select",
					m_uiShapeType,
					m_Rect.left,
					m_Rect.top,
					m_Rect.right,
					m_Rect.bottom,
					m_uiShapeId);
				std::string strComm=str;
				
				str.Format("ActiveSheet.Shapes(%i).Select.Delete",m_uiShapeId);
				std::string strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo,true);
			}
			else{
				pcmdDeque->push_cmd("begin","end",true);

				str="";
				for(int i=0;i<saData.GetSize();i++){
					CString strAux;
					if(str.GetLength()>0){
						str+=",";
					}
					strAux.Format("%s",saData[i]);
					//Autodetect strings
					if(std::string(strAux).find_first_not_of(" -+0123456789")!=std::string::npos){
						strAux="\""+strAux+"\"";
					}
					str+=strAux;
				}
				str="var data=new Array("+str+")";
				std::string strComm=str;
				std::string strCommUndo="";
				pcmdDeque->push_cmd(strComm,strCommUndo);

				str.Format("ActiveSheet.Shapes.AddShape(%i, %i, %i, %i, %i, %i, data).Select",
					m_uiShapeType,
					m_Rect.left,
					m_Rect.top,
					m_Rect.right,
					m_Rect.bottom,
					m_uiShapeId);
				strComm=str;
				str.Format("ActiveSheet.Shapes(%i).Select.Delete",m_uiShapeId);
				strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo);

				pcmdDeque->push_cmd("end","begin");
			}
		}
		//**********************************************************************
		break;

	case _DRAFTDRAW_SEL_RESIZING_RECT:

		m_Mode=_DRAFTDRAW_MODE_DRW;
		m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;
		m_Rect.NormalizeRect();
		//Added 29/11/2004
		//Add to command queue shape resizing
		//**********************************************************************
		if(pcmdDeque){

			//Choose scale mode
			
			CRect rectOffset=m_Rect;
			rectOffset-=m_RectLast;

			int nScaleH=ddcScaleFromTopLeft;
			int nScaleV=ddcScaleFromTopLeft;
			int incrementH=0;
			int incrementV=0;
			if(m_Rect.left>m_Rect.right)
				incrementH=m_Rect.left-m_RectLast.left;
			if(m_Rect.top>m_Rect.bottom)
				incrementV=m_Rect.top-m_RectLast.top;
			int idataH=m_Rect.right-m_RectLast.right-incrementH;
			int idataV=m_Rect.bottom-m_RectLast.bottom-incrementV;

			if(m_Rect.right==m_RectLast.right){
				nScaleH=ddcScaleFromBottomRight;
				idataH=m_Rect.left-m_RectLast.left;
			}

			if(m_Rect.bottom==m_RectLast.bottom){
				nScaleV=ddcScaleFromBottomRight;
				idataV=m_Rect.top-m_RectLast.top;
			}

			//Restore last rectangle to rescale
			m_Rect=m_RectLast;

			//Push commands
			pcmdDeque->push_cmd("begin","end",true);

			CString str;

			if(incrementH!=0){
				str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
					m_uiShapeId,
					incrementH);
				std::string strComm=str;

				str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
					m_uiShapeId,
					-incrementH);
				std::string strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo);
			}

			if(incrementV!=0){
				str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
					m_uiShapeId,
					incrementV);
				std::string strComm=str;

				str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
					m_uiShapeId,
					-incrementV);
				std::string strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo);
			}

			str.Format("Selection.ShapeRange.ScaleWidth(%i,%i,%i)",
				idataH,
				ddcFalse,
				nScaleH);
			std::string strComm=str;

			str.Format("Selection.ShapeRange.ScaleWidth(%i,%i,%i)",
				-idataH,
				ddcFalse,
				nScaleH);
			std::string strCommUndo=str;

			pcmdDeque->push_cmd(strComm,strCommUndo);

			str.Format("Selection.ShapeRange.ScaleHeight(%i,%i,%i)",
				idataV,
				ddcFalse,
				nScaleV);
			strComm=str;

			str.Format("Selection.ShapeRange.ScaleHeight(%i,%i,%i)",
				-idataV,
				ddcFalse,
				nScaleV);
			strCommUndo=str;

			pcmdDeque->push_cmd(strComm,strCommUndo);

			pcmdDeque->push_cmd("end","begin");
		}
		//**********************************************************************
		//Added 27/01/2004
		//Resize shape in screen array
		//**********************************************************************
		//**********************************************************************
		break;

	case _DRAFTDRAW_SEL_MOVING_RECT:

		m_Mode=_DRAFTDRAW_MODE_DRW;
		CRect rect=m_Rect;
		//31/01/2005
		m_Rect=m_RectLast;
		//Added 29/11/2004
		//Add to command queue shape movement
		//**********************************************************************
		if(pcmdDeque){

			pcmdDeque->push_cmd("begin","end",true);

			int idata=rect.left-m_RectLast.left;
			CString str;
			str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
				m_uiShapeId,
				idata);
			std::string strComm=str;

			str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
				m_uiShapeId,
				-idata);
			std::string strCommUndo=str;

			pcmdDeque->push_cmd(strComm,strCommUndo);

			idata=rect.top-m_RectLast.top;

			str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
				m_uiShapeId,
				idata);
			strComm=str;

			str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
				m_uiShapeId,
				-idata);
			strCommUndo=str;

			pcmdDeque->push_cmd(strComm,strCommUndo);

			pcmdDeque->push_cmd("end","begin");
		}
		//**********************************************************************
		//Added 27/01/2004
		//Move shape in screen array
		//**********************************************************************
		//**********************************************************************
		break;



	}}
	else{
		switch(m_TypeSelect){

		case _DRAFTDRAW_SEL_SIZING_RECT:

			break;

		case _DRAFTDRAW_SEL_MOVING_RECT:

			//01/10/2005
			//Program enter this case if shape is selected in a multiple selection
			//and the selection is moved.
			CRect rect=m_Rect;
			m_Rect=m_RectLast;
			//Add to command queue shape movement
			//**********************************************************************

			if(pcmdDeque){

				pcmdDeque->push_cmd("begin","end",true);

				int idata=rect.left-m_RectLast.left;
				CString str;
				str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
					m_uiShapeId,
					idata);
				std::string strComm=str;

				str.Format("ActiveSheet.Shapes(%i).IncrementLeft(%i)",
					m_uiShapeId,
					-idata);
				std::string strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo);

				idata=rect.top-m_RectLast.top;

				str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
					m_uiShapeId,
					idata);
				strComm=str;

				str.Format("ActiveSheet.Shapes(%i).IncrementTop(%i)",
					m_uiShapeId,
					-idata);
				strCommUndo=str;

				pcmdDeque->push_cmd(strComm,strCommUndo);

				pcmdDeque->push_cmd("end","begin");
			}

			//**********************************************************************
			m_RectLast=m_Rect;
			break;


		}
	}
	/*
	int i=0;
	while(m_MarkerArray[i]){

		delete(m_MarkerArray[i]);
		m_MarkerArray[i]=NULL;

		i++;
	}
	*/
	
}

//use this function to recalculate while resizing
void CShape::OnRotate(UINT nFlags, CPoint point)
{
	CPoint vector_offset=CPoint(point-m_Point);
	m_Point=point;
	if(vector_offset.x > 0){
		Rotate(5.0);
	}
	else if(vector_offset.x < 0){
		Rotate(-5.0);
	}
}

//use this function to recalculate while resizing
void CShape::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	m_Rect=CRect(m_Point,point);
	m_Rect.NormalizeRect();
	*/

	if(m_Mode==_DRAFTDRAW_MODE_SEL){


		switch(m_TypeSelect){

			case _DRAFTDRAW_SEL_MOVING_RECT:

				m_Rect+=CPoint(point-m_Point);
				m_Point=point;
				//if(!m_bNoResize)
					PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
				break;
			case _DRAFTDRAW_SEL_SIZING_RECT:

				if(!m_bNoResize){
				m_Rect=CRect(m_Point,point);
				if(!m_bLineResize)
					m_Rect.NormalizeRect();
				}
				else{
				m_Rect+=CPoint(point-m_Point);
				m_Point=point;
				}

				break;

			case _DRAFTDRAW_SEL_RESIZING_RECT:

				CPoint vector_offset=CPoint(point-m_Point);
				m_Point=point;
				float vector_x;
				float vector_y;
				bool flag_movex=false;
				bool flag_movey=false;

				switch(m_VectorSelect){
					case _DRAFTDRAW_SEL_RESIZING_RECT_NW:
						vector_x=-1;
						vector_y=-1;
						flag_movex=true;
						flag_movey=true;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_NE:
						vector_x=1;
						vector_y=-1;
						flag_movey=true;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_SE:
						vector_x=1;
						vector_y=1;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_SW:
						vector_x=-1;
						vector_y=1;
						flag_movex=true;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_N:
						vector_x=0;
						vector_y=-1;
						flag_movey=true;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_E:
						vector_x=1;
						vector_y=0;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_S:
						vector_x=0;
						vector_y=1;
						break;
					case _DRAFTDRAW_SEL_RESIZING_RECT_W:
						vector_x=-1;
						vector_y=0;
						flag_movex=true;
						break;
				}

				vector_x*=vector_offset.x;
				vector_y*=vector_offset.y;

				//if(vector_x<0) vector_x=0.0;
				//if(vector_y<0) vector_y=0.0;

				if(flag_movex){
					
					if(vector_x<0){

						vector_offset.x=-vector_offset.x;
					}

					

					m_Rect+=CPoint(-vector_x/**vector_offset.x/abs(vector_offset.x)*/,
						0);
				}

				if(flag_movey){
			

					if(vector_y<0){

						vector_offset.y=-vector_offset.y;
					}
					

					m_Rect+=CPoint(0,
						-vector_y/**vector_offset.y/abs(vector_offset.y)*/);
				}

				//rect: l,t,r,b
				m_Rect+=CRect(0,0,vector_x,vector_y);

				//repaint markers
				//if(!m_bNoResize)
				PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
				break;
		}
	}
}

void CShape::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){

	switch(nChar){

	case 27://ESC

		if(IsPoly()){
			CString str;
			CStringArray saData;
			GetData(saData);

			pcmdDeque->push_cmd("begin","end",true);

			str="";
			for(int i=0;i<saData.GetSize();i++){
				CString strAux;
				if(str.GetLength()>0){
					str+=",";
				}
				strAux.Format("%s",saData[i]);
				str+=strAux;
			}
			str="var data=new Array("+str+")";
			std::string strComm=str;
			std::string strCommUndo="";
			pcmdDeque->push_cmd(strComm,strCommUndo);

			str.Format("ActiveSheet.Shapes.AddShape(%i, %i, %i, %i, %i, %i, data).Select",
				m_uiShapeType,
				m_Rect.left,
				m_Rect.top,
				m_Rect.right,
				m_Rect.bottom,
				m_uiShapeId);
			strComm=str;
			str.Format("ActiveSheet.Shapes(%i).Select.Delete",m_uiShapeId);
			strCommUndo=str;

			pcmdDeque->push_cmd(strComm,strCommUndo);

			pcmdDeque->push_cmd("end","begin");

		}
		break;
	}
}

void CShape::OnDraw(CDC *pDC)
{
	if(m_Mode == _DRAFTDRAW_MODE_SEL){

		//Check if it is a line, polyline or whatever that is drawn using segments
		if(!m_bLineResize){

			CRect rectTemp=m_Rect;
			rectTemp.NormalizeRect();

			//debug delete
			if(m_Rect!=rectTemp){
				int i=0;
			}

			//if(!rectTemp.IsRectEmpty()){

				//Local variables
				SIZE size;
				size.cx=1;
				size.cy=1;
				CPoint p1;
				CPoint p2;

				//Draw shape in current position
				//-------------------------------------------------------------
				if(m_fAngle==0.0){
					//Get points to draw shape in its current position
					p1=m_RectLast.TopLeft();
					p2=m_RectLast.BottomRight();

					//Draw a rectangle in shape's current position
					pDC->MoveTo(p1);
					pDC->LineTo(p2.x,p1.y);
					pDC->LineTo(p2);
					pDC->LineTo(p1.x,p2.y);
					pDC->LineTo(p1);

					if(m_MarkerArray[0]){
						int i=0;
						CPoint ptOffset=m_MarkerArray[0]->CenterPoint()-m_Rect.TopLeft();
						while(m_MarkerArray[i]){

							CRect rect=*m_MarkerArray[i];
							rect-=ptOffset;
							pDC->Rectangle(rect);

							i++;
						}
					}
				}
				//-------------------------------------------------------------

				//Draw drag rectangle
				//-------------------------------------------------------------
				if(m_bDrawDashedRect){
				//Get a dashed pen
					CPen dashed(PS_DASH,1,RGB(0,0,0));
					CPen *prev_pen=pDC->SelectObject(&dashed);

					p1=rectTemp.TopLeft();
					p2=rectTemp.BottomRight();

					pDC->MoveTo(p1);
					pDC->LineTo(p2.x,p1.y);
					pDC->LineTo(p2);
					pDC->LineTo(p1.x,p2.y);
					pDC->LineTo(p1);

					//Select previuos pen
					pDC->SelectObject(prev_pen);
				}
				//-------------------------------------------------------------

			//}
		}
		else{
			//If we are not drawing the line proceed with this logic
			if(m_TypeSelect/*m_bLineResize*/ != _DRAFTDRAW_SEL_SIZING_RECT){

				//Local variables
				SIZE size;
				size.cx=1;
				size.cy=1;
				CPoint p1;
				CPoint p2;

				//Draw shape in current position
				//-------------------------------------------------------------
				if(m_fAngle==0.0){
					//Get points to draw shape in its current position
					p1=m_RectLast.TopLeft();
					p2=m_RectLast.BottomRight();

					//Draw a rectangle in shape's current position
					pDC->MoveTo(p1);
					pDC->LineTo(p2.x,p1.y);
					pDC->LineTo(p2);
					pDC->LineTo(p1.x,p2.y);
					pDC->LineTo(p1);

					if(m_MarkerArray[0]){
						int i=0;
						CPoint ptOffset=m_MarkerArray[0]->CenterPoint()-m_Rect.TopLeft();
						while(m_MarkerArray[i]){

							CRect rect=*m_MarkerArray[i];
							rect-=ptOffset;
							pDC->Rectangle(rect);

							i++;
						}
					}
				}
				//-------------------------------------------------------------
			}

			if(m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT){
			CPoint P1=m_Rect.TopLeft();
			CPoint P2=m_Rect.BottomRight();
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			}
		}
	}//end if mode _DRAFTDRAW_MODE_SEL
	else if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){

		int i=0;
		while(m_MarkerArray[i]){

			//Draw sizing handle
			pDC->Rectangle(m_MarkerArray[i]);

			//Debug
			/*
			CRect rc=this->m_Rect;
			pDC->Rectangle(rc);
			*/

			//Increment index
			i++;
		}

	}
}

BOOL CShape::IsEmpty()
{
	return m_Rect.IsRectEmpty();
}

void CShape::GetSizingRect( LPRECT lpRect ) const{

	*lpRect=m_Rect;
}

void CShape::GetSizingRect( CRect &rect ) const{

	if(m_fAngle==0.0){
		rect=m_Rect;
	}
	else{
		rect=m_Rect;
		GraphicsPath path;
		Rect pathRect = Rect(rect.left,rect.top,rect.Width(),rect.Height());
		path.AddRectangle(pathRect);
		Matrix matrix;
		matrix.Translate(-rect.CenterPoint().x, -rect.CenterPoint().y, MatrixOrderAppend);
		matrix.Rotate(m_fAngle, MatrixOrderAppend);
		matrix.Translate(rect.CenterPoint().x, rect.CenterPoint().y, MatrixOrderAppend);
		path.GetBounds(&pathRect,&matrix);
		rect=CRect(pathRect.GetLeft(),pathRect.GetTop(),pathRect.GetRight(),pathRect.GetBottom());
	}
}

//Select shape
void CShape::Select(BOOL bSelect/*=TRUE*/){
	if(bSelect){
		m_Mode=_DRAFTDRAW_MODE_DRW;
		m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;
		PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
	}
	else{
		m_Mode=_DRAFTDRAW_MODE_DRW;
		m_TypeSelect=_DRAFTDRAW_SEL_RESIZING_RECT;
	}
}

//Is shape selected?
BOOL CShape::IsSelected(){

	return ((m_Mode==_DRAFTDRAW_MODE_SEL)|(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT)|(m_TypeSelect==_DRAFTDRAW_SEL_RESIZING_RECT));
}

//Unselect shape
void CShape::Unselect(){
	this->m_Mode=_DRAFTDRAW_MODE_DRW;
	this->m_TypeSelect=_DRAFTDRAW_SEL_SIZING_RECT;
}

//Move shape
void CShape::Move(int x,int y,BOOL bAbsolute/*=FALSE*/){

	if(!bAbsolute){
		m_Rect.OffsetRect(x,y);
	}
	else{
		m_Rect.OffsetRect(x-m_Rect.left,y-m_Rect.top);
	}

	if(IsSelected()){
		PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
	}
}

//Move & Scale shape
void CShape::Move(int left,int top,int right,int bottom,BOOL bAbsolute/*=FALSE*/){

	if(!bAbsolute){
		m_Rect+=CRect(left,top,right,bottom);
	}
	else{
		m_Rect=CRect(left,top,right,bottom);
	}

	if(IsSelected()){
		PlaceSelectRect(m_MarkerArray,&m_Rect,( ( (CDraftDrawApp*)AfxGetApp() )->GetActiveViewProp()).cx*_DRAFTDRAW_SEL_SIZEREL);//_DRAFTDRAW_SEL_SIZE);
	}
}

//Angle of flip axis in degrees: 0.0 = horizontal, 90.0 = vertical, etc.
void CShape::Flip(float fAngle){

	float fAngNorm=fmod((double)fAngle,180.0);

	if(fAngNorm==0.0){
		m_Rect=CRect(m_Rect.right,m_Rect.top,m_Rect.left,m_Rect.bottom);
	}
	else if(fAngNorm==90.0){
		m_Rect=CRect(m_Rect.left,m_Rect.bottom,m_Rect.right,m_Rect.top);
	}
	else{
	}
}

void CShape::DoConntoId(){

	if(m_bConect){
		m_uiConn=m_pshConn->m_uiShapeId;
		m_uiChildConn=m_pshChildConn->m_uiShapeId;
	}
}

void CShape::DoPasteConnections(std::map<int,int> *pmapIdtoId,mapShapeIdtoObj_t *pmapIdtoObj){

	if(m_bConect){
		int new_id=m_uiConn;

		if(pmapIdtoId){

			new_id=(*pmapIdtoId)[m_uiConn];
		}

		m_pshConn=(CShape*)(*pmapIdtoObj)[new_id];


		if(pmapIdtoId){

			new_id=(*pmapIdtoId)[m_uiChildConn];
		}

		m_pshChildConn=(CShape*)(*pmapIdtoObj)[new_id];
	}

}

void CShape::Rotate(float fAngle){

	//Increment angle
	m_fAngle=fmod((double)(m_fAngle+fAngle),360.0);

	//Calcule cos and sin
	float cos1=cos(m_fAngle*PI/180.0);
	float sin1=sin(m_fAngle*PI/180.0);

	FLOAT eM11=cos1;
	FLOAT eM12=sin1;
	m_eM11=eM11;
	m_eM12=eM12;
	m_eM21=-m_eM12;
	m_eM22=m_eM11;
}

void CShape::DoWorldTransform(HDC hdc){

	XFORM x;
	CPoint pivot=this->m_RectLast.CenterPoint();

	/* translate objects from pivot point to origin. */
	x.eM11 = (float)1.0;
	x.eM12 = (float)0.0;
	x.eM21 = (float)0.0;
	x.eM22 = (float)1.0;
	x.eDx = (float)-pivot.x;
	x.eDy = (float)-pivot.y;
	SetWorldTransform (hdc, &x);

	/* rotate object about origin. */
	x.eM11 = this->m_eM11;
	x.eM12 = this->m_eM12;
	x.eM21 = this->m_eM21;
	x.eM22 = this->m_eM22;
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

//Do filling of shape: solid, gradient...
void CShape::DoFill(CDC* pDC, LPRECT lpRect /*=NULL*/)
{
	CRect rect = m_Rect;
	if (lpRect != NULL){
		rect = *lpRect;
	}
	GraphicsPath gfxPath;
	Gdiplus::Rect tmpRect(rect.left, rect.top, rect.Width(), rect.Height());
	gfxPath.AddRectangle(tmpRect);

	CPoint point1 = rect.TopLeft();
	CPoint point2 = CPoint(rect.TopLeft().x, rect.BottomRight().y);

	if (rect.Height() > rect.Width())
	{
		point1 = rect.TopLeft();
		point2 = CPoint(rect.BottomRight().x, rect.TopLeft().y);
	}

	//Fill the path
	Graphics grf(pDC->m_hDC);
	LinearGradientBrush lgb(Point(point1.x, point1.y),
		Point(point2.x, point2.y),
		Color(GetRValue(m_crFillBgnd), GetGValue(m_crFillBgnd), GetBValue(m_crFillBgnd)),
		Color(GetRValue(m_crFill), GetGValue(m_crFill), GetBValue(m_crFill))
		);
	lgb.SetBlendBellShape(.5f, 1.0f);//SetBlendTriangularShape(.5f, 1.0f);
	grf.FillPath(&lgb, &gfxPath);
	//grf.FillRectangle(&lgb, tmpRect);
}

//Do filling of shape: solid, gradient...
void CShape::DoFill(CDC* pDC, void* gfxPath, CPoint point1, CPoint point2)
{
	//Fill the path
	Graphics grf(pDC->m_hDC);
	LinearGradientBrush lgb(Point(point1.x, point1.y),
		Point(point2.x, point2.y),
		Color(GetRValue(m_crFillBgnd), GetGValue(m_crFillBgnd), GetBValue(m_crFillBgnd)),
		Color(GetRValue(m_crFill), GetGValue(m_crFill), GetBValue(m_crFill))
		);
	lgb.SetBlendBellShape(.5f, 1.0f);//SetBlendTriangularShape(.5f, 1.0f);
	grf.FillPath(&lgb, (GraphicsPath*)gfxPath);
	//grf.FillRectangle(&lgb, tmpRect);
}

//Split rectangle in two rectangles of same size, following vector
int CShape::SplitRect(CRect rect, LPPOINT vect, CRect(&arrRect)[2])
{
	CPoint point;
	CRect rect1, rect2;

	if (vect->x == 0 && vect->y != 0){
		point = rect.TopLeft() + CPoint(0, rect.Height()*0.5);
		rect1 = CRect(rect.TopLeft(), point + CPoint(rect.Width(), 0));
		//rect1.DeflateRect(0, rect1.Height()*0.10);
		rect2 = CRect(point, rect.BottomRight());
		//rect2.DeflateRect(0, rect2.Height()*0.10);
	}
	else if (vect->x != 0 && vect->y == 0){
		point = rect.TopLeft() + CPoint(rect.Width()*0.5, 0);
		rect1 = CRect(rect.TopLeft(), point + CPoint(0, rect.Height()));
		//rect1.DeflateRect(rect1.Width()*0.10, rect1.Height()*0.10);
		rect2 = CRect(point, rect.BottomRight());
		//rect2.DeflateRect(rect2.Width()*0.10, rect2.Height()*0.10);
	}

	arrRect[0] = rect1;
	arrRect[1] = rect2;

	return 2;
}

/////////////////////////////////////////////////////////////////////////////
// CShapeRect

/*IMPLEMENT_DYNCREATE(CShapeRect, CShape)*/
IMPLEMENT_SERIAL(CShapeRect, CShape, 1)

CShapeRect::CShapeRect(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{
	//CShape::CShape(lpRect,nId,cmddq);
	m_uiShapeType=ddcShapeRectangle;
	m_Type=1;
	m_bGroup=FALSE;
	m_bDrawDashedRect=FALSE;
}

CShapeRect::~CShapeRect()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeRect message handlers
//use this function to select
/*
void CShapeRect::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Point=point;
	m_Rect=CRect(point,point);
}
*/
/*
void CShapeRect::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}
*/


void CShapeRect::OnDraw(CDC *pDC)
{
	if(m_Mode==_DRAFTDRAW_SEL_SIZING_RECT){
		if(m_bGroup){
			//Don't draw anything in this case
		}
		else{
			//Draw in current position
			if(m_TypeSelect!=_DRAFTDRAW_SEL_RESIZING_RECT){
				pDC->Rectangle(m_RectLast);
			}

			pDC->Rectangle(m_Rect);
		}

		CShape::OnDraw(pDC);
	}
	else if(!m_Rect.IsRectEmpty()){

		if(m_VectorProyect==CPoint3D(0,1,0)){

			CRect rect=CRect(
				m_Rect.TopLeft().x,
				0/*m_Rect.TopLeft().y*/,
				m_Rect.BottomRight().x,
				/*m_Rect.TopLeft().y*/-m_Height);

			pDC->Rectangle(rect+m_PointOrg);
			return;
		}

		if(m_bGroup){
			pDC->FrameRect(&m_Rect,&CBrush(RGB(0,0,0)));
		}
		else{
#ifdef DCABLE_SHAPE_FILL_PROTOTYPE
			CRect arrRect[2];
			//void Func(int(&myArray)[100]);
			//SplitRect(m_Rect, &CPoint(0, 1), arrRect);
			//DoFill(pDC, arrRect[0]);
			//DoFill(pDC, arrRect[1]);
			DoFill(pDC, m_Rect);
			pDC->FrameRect(&m_Rect,&CBrush(RGB(0,0,0)));
#else
			pDC->Rectangle(m_Rect);
#endif
		}

		//to draw selections
		CShape::OnDraw(pDC);

	}
}


/////////////////////////////////////////////////////////////////////////////
// CShapeEllipse
/*IMPLEMENT_DYNCREATE(CShapeEllipse, CShape)*/
IMPLEMENT_SERIAL(CShapeEllipse, CShape, 1)

CShapeEllipse::CShapeEllipse(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapeEllipse;
	m_Type=1;

}

CShapeEllipse::~CShapeEllipse()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers
/*
void CShapeEllipse::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Rect=CRect(point,point);
}

void CShapeEllipse::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}

void CShapeEllipse::OnMouseMove(UINT nFlags, CPoint point)
{

	m_Rect=CRect(m_Rect.TopLeft(),point);

	m_Rect.NormalizeRect();
}
*/
void CShapeEllipse::OnDraw(CDC *pDC)
{
	if(!m_Mode){
		//Draw in current position

		if(m_TypeSelect!=_DRAFTDRAW_SEL_RESIZING_RECT){
			pDC->Ellipse(m_RectLast);
		}

		pDC->Ellipse(m_Rect);

		CShape::OnDraw(pDC);
	}

	else if(!m_Rect.IsRectEmpty()){

		if(m_VectorProyect==CPoint3D(0,1,0)){

			CRect rect=CRect(
				m_Rect.TopLeft().x,
				0/*m_Rect.TopLeft().y*/,
				m_Rect.BottomRight().x,
				/*m_Rect.TopLeft().y*/-m_Height);

			pDC->Rectangle(rect+m_PointOrg);
			return;
		}

		if(m_bTransparent){
			pDC->Ellipse(m_Rect);

		}
		else{
			CBrush brush(m_crFill);
			CBrush *prev_brush=pDC->SelectObject(&brush);
			pDC->Ellipse(m_Rect);
			pDC->SelectObject(prev_brush);
		}

		//To draw selections
		CShape::OnDraw(pDC);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CShapeLine
/*IMPLEMENT_DYNCREATE(CShapeLine, CShape)*/
IMPLEMENT_SERIAL(CShapeLine, CShape, 1)

CShapeLine::CShapeLine(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapeLine;
	m_Type=1;
	m_bLineResize=TRUE;
}

CShapeLine::~CShapeLine()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers
/*
void CShapeLine::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Rect=CRect(point,point);
}
*/
/*
void CShapeLine::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}
*/
/*
void CShapeLine::OnMouseMove(UINT nFlags, CPoint point)
{

	m_Rect=CRect(m_Rect.TopLeft(),point);

	m_Rect.NormalizeRect();
}
*/

void CShapeLine::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
        //archive << m_name << m_number;
	}
    else{
		/*
		int nCount;
		CShape *pSh;
		archive>>nCount;
        archive >> pSh;
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
			m_Rect=pSh->m_Rect;
		}
		*/
	}

}

void CShapeLine::SerializeDdw(CDdwioFile &ddwfile)
{
	CPoint p[2];
	p[0]=m_Rect.TopLeft();//0;
	p[1]=m_Rect.BottomRight();//-m_Rect.TopLeft();

	ddwfile.WritePolyline(p,2);
}

void CShapeLine::SerializeSvg(CSvgioFile &svgfile)
{
	CPoint p[2];
	p[0] = m_Rect.TopLeft();//0;
	p[1] = m_Rect.BottomRight();//-m_Rect.TopLeft();

	svgfile.OpenPath();
	svgfile.WriteMoveTo(p[0]);
	svgfile.WriteLineTo(p[1]);
	svgfile.ClosePath();
}
void CShapeLine::OnDraw(CDC *pDC)
{

	//if(!m_Rect.IsRectEmpty()){


		if(m_VectorProyect==CPoint3D(0,1,0)){

			CPoint point1=CPoint(m_Rect.TopLeft().x,0);
			point1+=m_PointOrg;


			CPoint point2=CPoint(m_Rect.BottomRight().x,point1.y);

			pDC->MoveTo(point1);
			pDC->LineTo(point2);

			return;
		}

		pDC->MoveTo(m_Rect.TopLeft());
		pDC->LineTo(m_Rect.BottomRight());

		//to draw selections
		CShape::OnDraw(pDC);
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeRule
IMPLEMENT_DYNCREATE(CShapeRule, CShape)

CShapeRule::CShapeRule()
{
	m_Type=1;
	m_bLineResize=TRUE;
	m_alfa=2*PI*20/360;
}

CShapeRule::~CShapeRule()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers

void CShapeRule::OnMouseMove(UINT nFlags, CPoint point)
{
	CShape::OnMouseMove(nFlags, point);

	if(m_Mode==_DRAFTDRAW_MODE_SEL){

		int height=m_Rect.Height();
		int width=m_Rect.Width();
		m_dfD=sqrt((double)(width*width+height*height));
		m_fScale=DCABLE_GRIDX_DEFAULT*2.0;
		m_fScale/=m_dfD;

		m_offset1=(width*cos(m_alfa)+height*sin(m_alfa))*m_fScale;
		m_offset2=(height*cos(m_alfa)-width*sin(m_alfa))*m_fScale;
		m_offset3=(width*cos(m_alfa)-height*sin(m_alfa))*m_fScale;
		m_offset4=(height*cos(m_alfa)+width*sin(m_alfa))*m_fScale;

	}
}

void CShapeRule::OnDraw(CDC *pDC)
{

	//if(!m_Rect.IsRectEmpty()){


		if(m_VectorProyect==CPoint3D(0,1,0)){

			CPoint point1=CPoint(m_Rect.TopLeft().x,0);
			point1+=m_PointOrg;


			CPoint point2=CPoint(m_Rect.BottomRight().x,point1.y);

			pDC->MoveTo(point1);
			pDC->LineTo(point2);
			pDC->Ellipse(CRect(point1-CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT),point1+CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT)));
			pDC->Ellipse(CRect(point2-CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT),point2+CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT)));
			return;
		}

		CPoint point1=m_Rect.TopLeft();
		CPoint point2=m_Rect.BottomRight();

		pDC->MoveTo(m_Rect.TopLeft());
		pDC->LineTo(m_Rect.BottomRight());
/*
		pDC->Ellipse(CRect(point1-CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT),point1+CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT)));
		pDC->Ellipse(CRect(point2-CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT),point2+CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT)));
*/

		CPoint pt=m_Rect.TopLeft();
		CPoint pt11=pt+CSize(m_offset1,m_offset2);
		CPoint pt12=pt+CSize(m_offset3,m_offset4);
		pt=m_Rect.BottomRight();
		CPoint pt21=pt-CSize(m_offset3,m_offset4);
		CPoint pt22=pt-CSize(m_offset1,m_offset2);

		//if(!m_Mode==_DRAFTDRAW_MODE_SEL){
		pDC->MoveTo(pt11);
		pDC->LineTo(m_Rect.TopLeft());
		pDC->LineTo(pt12);
		pDC->MoveTo(pt21);
		pDC->LineTo(m_Rect.BottomRight());
		pDC->LineTo(pt22);
		//}

		//Draw label
		CFont font;
		font.CreateFont(
			16,							// nHeight
			0,							// nWidth
			0,							// nEscapement
			0,							// nOrientation
			FW_NORMAL,					// nWeight
			FALSE,						// bItalic
			FALSE,						// bUnderline
			0,							// cStrikeOut
			ANSI_CHARSET,				// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,			// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			"Arial");					// lpszFacename
		CString strLabel;
		int height=m_Rect.Height();
		int width=m_Rect.Width();
		strLabel.Format("%.0f(%i,%i)",m_dfD,width,height);
		CFont *font_prev=pDC->SelectObject(&font);
		SIZE sz;
		GetTextExtentPoint32(pDC->m_hDC, strLabel, strLabel.GetLength(), &sz);
		pt=m_Rect.TopLeft()+CSize((width-sz.cx)*.5,(height-sz.cy)*.5);
		pDC->DrawText(strLabel,CRect(pt,sz),DT_CENTER);
		pDC->FrameRect(CRect(pt,sz),&CBrush(RGB(0,0,0)));
		pDC->SelectObject(font_prev);

		//to draw selections
		CShape::OnDraw(pDC);
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeArc

IMPLEMENT_SERIAL(CShapeArc, CShape, 1)

CShapeArc::CShapeArc(LPRECT lpRect,UINT nId,cmddeque *cmddq):CShape(lpRect,nId,cmddq)
{
	m_uiShapeType = ddcShapeArc;
	m_Type = 1;

	b = 20;
	m_Alfa = 0.0;
	m_Alfap = 0.0;

	//Start angle for drawing ellipse arc
	m_angleStart = 30.0;
	m_angleSweep = -180.0;

	//Use Gdiplus to draw the ellipse arc
	m_bGdiplus = true;//false;
}

BOOL CShapeArc::Create(LPPOINT lpPoint1, LPPOINT lpPoint2, BOOL bGdiplus /*= FALSE*/)
{
	m_angleStart = 0;
	m_angleSweep = 90;
	m_bGdiplus = bGdiplus;

	if (lpPoint1->y < lpPoint2->y){
		m_angleSweep = -90;
	}

	if (lpPoint1->x < lpPoint2->x){
		m_angleStart = 180;
	}

	return TRUE;
}

CShapeArc::~CShapeArc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers


void CShapeArc::OnMouseMove(UINT nFlags, CPoint point)
{

	CShape::OnMouseMove(nFlags,point);
	//m_Rect=CRect(m_Point,point);

	//m_Rect.NormalizeRect();

	m_Point2=point;

	m_P1=m_Rect.TopLeft();//m_Point;
	m_P2=m_Rect.BottomRight();//m_Point2;

	//m_Alfa=0.0;
	
	if(m_Alfa!=0.0){

		m_Alfap=m_Alfa;
		//RotateAxis(m_P1,m_P2,b,m_Alfap);
	}

}
/*
void CShapeArc::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}
*/
void CShapeArc::OnLButtonDown(UINT nFlags, CPoint point)
{

	CShape::OnLButtonDown(nFlags,point);

	m_P1=m_Rect.TopLeft();//point;
	m_P2=m_Rect.BottomRight();//point;
	m_Point2=point;
	
}

void CShapeArc::OnDraw(CDC *pDC)
{

	//to draw selections
	CShape::OnDraw(pDC);

	if((!m_Mode)&&(!m_Rect.IsRectEmpty())){

		CPoint P1=m_Rect.TopLeft();//m_Point;
		CPoint P2=m_Rect.BottomRight();//m_Point2;

		pDC->MoveTo(P1);
		pDC->LineTo(P2);

		//module of main axis (2a)
		float mod=sqrt((double)((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y)));
		//calcule b axis
		float vector_x=(P2.y-P1.y)*b/mod;
		float vector_y=(P1.x-P2.x)*b/mod;
		//calcule midle point
		CPoint point_mid=CPoint((P1.x+P2.x)*.5,(P1.y+P2.y)*.5);

		//calcule extremes of second axis (2b)
		//CPoint P3=CPoint(point_mid.x+vector_x,point_mid.y+vector_y);
		//CPoint P4=CPoint(point_mid.x-vector_x,point_mid.y-vector_y);
		//calcule extremes of second axis (2b) with rotation
		float cosAB=vector_x*cos(2*PI*m_Alfa/360)+vector_y*sin(2*PI*m_Alfa/360);
		float senAB=vector_y*cos(2*PI*m_Alfa/360)-vector_x*sin(2*PI*m_Alfa/360);
		CPoint P3=CPoint(point_mid.x+cosAB,point_mid.y+senAB);
		CPoint P4=CPoint(point_mid.x-cosAB,point_mid.y-senAB);

		//draw handlers
		pDC->MoveTo(P3);
		pDC->LineTo(P4);
		pDC->MoveTo(m_P1);
		pDC->LineTo(m_P2);

	}
	else if (!m_Rect.IsRectEmpty()){

		CPoint P1 = m_Rect.TopLeft();//m_Point;
		CPoint P2 = m_Rect.BottomRight();//m_Point2;

		//CRect rect;
		//BoundRect(P1,P2,10,rect);

		if (!m_bGdiplus){
			m_Alfap = m_Alfa;
			RotateAxis(P1, P2, b, m_Alfap);

			CPoint point_array[100];
			DrawEllipse(point_array, 100, P1, P2, b, m_Alfap);

			pDC->Polyline(point_array, 100);
		}
		else{
			GraphicsPath gfxPath;
			float angleStart = m_angleStart;//2 * PI* m_angleStart / 360;
			float angleSweep = m_angleSweep;//2 * PI* m_angleSweep / 360;
			gfxPath.AddArc(m_Rect.TopLeft().x,
				m_Rect.TopLeft().y,
				m_Rect.Width(),
				m_Rect.Height(),
				angleStart, angleSweep);
			//Draw the path
			Graphics grf(pDC->m_hDC);
			Pen blackPen(Color::Black, 1);
			grf.DrawPath(&blackPen, &gfxPath);
		}

		//pDC->Arc(&rect,P1,P2);
		//pDC->Rectangle(&rect);
	}
}

void CShapeArc::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){

	int i;

	switch(nChar){

	case 38:

		if(b<200) b+=10;
		break;

	case 40:

		if(b>10) b-=10;
		break;

	case 37://left arrow

		m_Alfa-=1;
		//m_P1=m_Point;
		//m_P2=m_Point2;
		//m_Alfap=m_Alfa;
		//RotateAxis(m_P1,m_P2,b,m_Alfap);
		m_angleStart = fmod(m_angleStart - 10.0, 360.0);
		break;

	case 39://right arrow

		m_Alfa+=1;
		//m_P1=m_Point;
		//m_P2=m_Point2;
		//m_Alfap=m_Alfa;
		//RotateAxis(m_P1,m_P2,b,m_Alfap);
		m_angleStart = fmod(m_angleStart + 10.0, 360);
		break;

	default:

		i=0;
		break;

	}

}

/////////////////////////////////////////////////////////////////////////////
// CShapeContainer
/*IMPLEMENT_DYNCREATE(CShapeContainer, CShape)*/
IMPLEMENT_SERIAL(CShapeContainer, CShape, 1)

CShapeContainer::CShapeContainer(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{

	m_bConnectMake=FALSE;	//Must be set to TRUE before calling OnLButtonDown to try to connect

	m_ShapeNumber=10;
	SetParent(NULL);

}

//Copy constructor
CShapeContainer::CShapeContainer(CShapeContainer& ShCont):CShape(ShCont){


	m_bConnectMake=FALSE;	//Must be set to TRUE before calling OnLButtonDown to try to connect

	m_ShapeNumber=10;
	SetParent(NULL);
}

CShapeContainer::~CShapeContainer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapeContainer implementation

CShape& CShapeContainer::operator=( const CShape& Sh ){
	//03/01/2005
	CShape::operator =(Sh);

	return *this;
}

CShape& CShapeContainer::operator++( ){
	return *this;
}

CShape*& CShapeContainer::operator []( int nIndex ){

	ASSERT(nIndex<m_obarrShapearr.GetSize());

	CObject*& pOb=m_obarrShapearr[nIndex];
	return ((CShape *&)pOb);
}

CShape* CShapeContainer::operator []( int nIndex ) const{

	ASSERT(nIndex<m_obarrShapearr.GetSize());

	CObject* pOb=m_obarrShapearr[nIndex];
	return ((CShape *)pOb);
}

/////////////////////////////////////////////////////////////////////////////
// CShapeContainer message handlers

CShape *CShapeContainer::GetParent(){

	if(m_pShParent==NULL){
		return this;
	}

	return m_pShParent;
}

void CShapeContainer::SetParent(CShape *pSh){

	m_pShParent=pSh;
}

BOOL CShapeContainer::OnCommand( WPARAM wParam, LPARAM lParam ){

	switch(LOWORD(wParam)){
	case ID_DEBUG_EDIT_SHPINFO:
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			CDialogDebugShpInfo dialog;
			dialog.m_sText.Format("Id=%i",m_uiShapeId);
			dialog.DoModal();
			return TRUE;
		}
		break;
	}

	return CShape::OnCommand( wParam, lParam );
	//return FALSE;
}

void CShapeContainer::SerializeGbr(CGbrioFile &gbrfile){

}

void CShapeContainer::SerializeSvg(CSvgioFile &svgfile){
	//Iretare child shapes
	for (int i = 0; i<m_obarrShapearr.GetSize(); i++){

		//Get pointer to current child shape
		CShape *psh = (CShape *)m_obarrShapearr.GetAt(i);

		//Save previous bounding rectangle
		CRect rect_prev = psh->m_Rect;

		//Offset shape with parent
		psh->m_Rect += this->m_Rect.TopLeft();

		//Serialize to file
		psh->SerializeSvg(svgfile);

		//Restore previous rectangle
		psh->m_Rect = rect_prev;
	}
}

//Connection is allways implemented in CShapeContainer
//Connection between 2 Units apear only in between pins as follows:
//        Shape1                            Shape2
//          |                                 |
//          pin                             pin
//            m_pshConn<------------->m_pshConn
//            m_pshChildConn<--->m_pshChildConn

CShape *CShapeContainer::SetConnection(CShape *pSh,int index){
	//configure pins
	if(pSh!=NULL)
	for(int i=0;i<m_obarrShapearr.GetSize();i++){
	CShapeContainer *pShpin=(CShapeContainer *)m_obarrShapearr.GetAt(i);
	if(m_pshChildConn==pShpin->m_pshChildConn){

		pShpin->m_pshChildConn=pSh;
		pShpin->m_pshConn=m_pshConn;
		pShpin->m_bConect=true;
		m_pshChildConn=NULL;
		m_pshConn=NULL;
		return NULL;
	}
	}

	CShapeContainer *pShpin=(CShapeContainer *)m_obarrShapearr.GetAt(index);
//debug
//******************************************************************************
/*
CShapePin *pShpinpin=(CShapePin *)pShpin;
int iData=m_obarrShapearr.GetSize();
for(int i=0;i<iData;i++){
	CRuntimeClass *pRT=(m_obarrShapearr.GetAt(i))->GetRuntimeClass();
	pShpinpin=(CShapePin *)m_obarrShapearr.GetAt(i);
}
*/
//******************************************************************************


	pShpin->m_pshChildConn=m_pshChildConn;
	pShpin->m_pshConn=m_pshConn;
	pShpin->m_bConect=true;
	m_pshChildConn=NULL;
	m_pshConn=NULL;
	return pShpin; 
}

void CShapeContainer::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		DoConntoId();
        //archive << m_name << m_number;
		int nCount=m_obarrShapearr.GetSize();
		archive<<nCount;
		for(int i=0;i<nCount;i++){
			archive<<m_obarrShapearr[i];
		}
		archive<<m_bConect;
		//archive<<(DWORD)m_pshConn;
		//archive<<(DWORD)m_pshChildConn;
		archive<<m_uiConn;
		archive<<m_uiChildConn;

	}
    else{
		int nCount;

		archive>>nCount;
		if(nCount){
			for(int i=0;i<m_obarrShapearr.GetSize();i++){
				delete(m_obarrShapearr[i]);
			}
			m_obarrShapearr.RemoveAll();
		}
		//archive>>nCount;
		for(int i=0;i<nCount;i++){
			CShape *pSh;
			archive >> pSh;
			if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
				CShapeContainer *pShCont=(CShapeContainer *)pSh;
				pShCont->m_pShParent=this;
			}
			m_obarrShapearr.Add(pSh);
		}


		/*
		if(nCount){
			CShapePin *pShPin0=(CShapePin *)m_obarrShapearr[0];
			CShapePin *pShPin1=(CShapePin *)m_obarrShapearr[1];
		}
		*/

		archive>>m_bConect;
		//DWORD dwData;
		//archive>>dwData;
		//m_pshConn=(CShape*)dwData;
		//archive>>dwData;
		//m_pshChildConn=(CShape*)dwData;
		archive>>m_uiConn;
		archive>>m_uiChildConn;

		/*
		int nCount;
		CShape *pSh;
		archive>>nCount;
        archive >> pSh;
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
			m_Rect=pSh->m_Rect;
		}
		*/
	}

	//1.05
	float fVer0=1.05;
	float fData=m_fVer-fVer0;
	if(fData>=0){

		m_iaParameter.Serialize(archive);
	}
}

void CShapeContainer::SerializeDdw(CDdwioFile &ddwfile)
{
	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			if(psh->m_pshChildConn){

				int p[4];
				p[0]=m_uiShapeId-1;
				p[1]=psh->m_uiShapeId;
				p[2]=psh->m_pshConn->m_uiShapeId-1;
				p[3]=psh->m_pshChildConn->m_uiShapeId;
				ddwfile.WriteConnect(p,4);
			}
		}
	}
}

void CShapeContainer::OnDraw(CDC *pDC)
{
	CShape::OnDraw(pDC);
}

void CShapeContainer::OnMouseMove(UINT nFlags, CPoint point){

	//Iterate throw child shape to disconnect them, stretch, or create
	//new shapes between
	//==========================================================================
	for(int i=0;i<m_obarrShapearr.GetSize();i++){
		//To child shapes relative position is passes in point
		CShape *pSh=(CShape *)m_obarrShapearr[i];
		CPoint pointr=point-m_Point;
		pSh->OnMouseMove(nFlags,pointr);
	}
	//==========================================================================
	CShape::OnMouseMove(nFlags, point);
}

void CShapeContainer::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}

void CShapeContainer::OnLButtonUp(UINT nFlags, CPoint point){

	/*
	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(!psh->m_pshChildConn){

			psh->OnLButtonDown(nFlags, point);
			if(psh->m_pshChildConn){

				m_pshChildConn=psh->m_pshChildConn;
			}
		}

	}
	*/

	CShape::OnLButtonUp(nFlags,point);
}

void CShapeContainer::OnLButtonDown(UINT nFlags, CPoint point){

	if(m_bConnectMake){//if(1){//if(m_bConnectMake){
		for(int i=0;i<m_obarrShapearr.GetSize();i++){

			CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

			if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			if(!psh->m_pshChildConn){

				psh->OnLButtonDown(nFlags, point);
				if(psh->m_pshChildConn){

					m_pshChildConn=psh->m_pshChildConn;
					//Added 15/08/2004 to prevent shapes getting selected
					//when connecting to a pin.
					//---------------------------------------------------
					m_bConnectMake=FALSE;
					return;	//remove either this return or m_bConnectMake flag,
							//is dangeous two exit point and a behavior flag.
					//---------------------------------------------------
				}
			}
			}
		}
		m_bConnectMake=FALSE;
	}

	CShape::OnLButtonDown(nFlags, point);
}

void CShapeContainer::GetSizingRect( LPRECT lpRect ) const{

	CShape::GetSizingRect(lpRect);
}

void CShapeContainer::GetRectTemp(CRect &rect){

}

void CShapeContainer::GetRectUpdatePlace(CRect &rect){

}

//Return a pointer to a shape that can be connect, but doesn´t connect anaything.
BOOL CShapeContainer::PtInRect(LPPOINT point,CShapeContainer** lpSh){

	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShCont=(CShapeContainer *)psh;

			if(pShCont->PtInRect(point,lpSh)){

				return TRUE;
			}
		}
	}

	return FALSE;
}

//Check if a shape container is connected
BOOL CShapeContainer::IsConnected(){

	return m_bConect;
}

//Link two shapes.
void CShapeContainer::LinkShapes(CShapeContainer* lpShCont1,
								CShapeContainer* lpShCont2){

	//Assert at least one shape is contained in this.
	CShape *pParent1=lpShCont1->GetParent();
	CShape *pParent2=lpShCont2->GetParent();
	ASSERT(pParent1==this||pParent2==this);
	
	//Child pointer vs Child pointer
	lpShCont1->m_pshChildConn=lpShCont2;
	lpShCont2->m_pshChildConn=lpShCont1;

	//Parent pointer vs Parent pointer
	lpShCont1->m_pshConn=pParent2;
	lpShCont2->m_pshConn=pParent1;

	//Mark shapes as linked
	lpShCont1->m_bConect=TRUE;
	lpShCont2->m_bConect=TRUE;
}

//Unlink two shapes.
void CShapeContainer::UnlinkShapes(CShapeContainer* lpShCont1,
								CShapeContainer* lpShCont2){

	//Assert shapes are connected.
	CShape *pParent1=lpShCont1->GetParent();
	CShape *pParent2=lpShCont2->GetParent();
	ASSERT(lpShCont1->m_pshChildConn==lpShCont2);
	ASSERT(lpShCont2->m_pshChildConn==lpShCont1);
	ASSERT(lpShCont1->m_pshConn==pParent1);
	ASSERT(lpShCont2->m_pshConn==pParent2);

	//Child pointers
	lpShCont1->m_pshChildConn=NULL;
	lpShCont2->m_pshChildConn=NULL;

	//Parent pointers
	lpShCont1->m_pshConn=NULL;
	lpShCont2->m_pshConn=NULL;

	//Mark shapes as unlinked
	lpShCont1->m_bConect=FALSE;
	lpShCont2->m_bConect=FALSE;
}

POSITION CShapeContainer::GetFirstConnectedItemPosition() const{

	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShCont=(CShapeContainer *)psh;

			if(pShCont->IsConnected()){

				return (POSITION)(i+1);
			}
		}
	}

	return NULL;
}

int CShapeContainer::GetNextConnectedItem(POSITION& pos) const{

	int nIndex=((int)pos)-1;
	for(int i=nIndex+1;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShCont=(CShapeContainer *)psh;

			if(pShCont->IsConnected()){

				pos=(POSITION)(i+1);
				return nIndex;
			}
		}
	}

	pos=NULL;
	return nIndex;
}

//------------------------------------------------------------------------
//FUNCTION: DoPasteConnections
//Rebind shape connections based in shape ID.
//pmapIdtoId = pointer to map of shape ID(old) to shape ID(new), can be NULL
//pmapIdtoObj = pointer to map of shape ID(new) to object pointer
void CShapeContainer::DoPasteConnections(std::map<int,int> *pmapIdtoId,mapShapeIdtoObj_t *pmapIdtoObj){
	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShCont=(CShapeContainer *)psh;
			BOOL bConnect=pShCont->m_bConect;

			if(((CShapeContainer *)psh)->m_bConect){
				int new_id=psh->m_uiConn;

				if(pmapIdtoId){

					new_id=(*pmapIdtoId)[psh->m_uiConn];
				}

				CShapeContainer *pShCont2=(CShapeContainer *)(*pmapIdtoObj)[new_id];


				if(psh->m_uiChildConn>=0){
					pShCont2=(CShapeContainer *)pShCont2->m_obarrShapearr[psh->m_uiChildConn];


					LinkShapes((CShapeContainer *)psh,pShCont2);
				}
			}
		}

	}
}

void CShapeContainer::DoConntoId(BOOL bOwn /*=FALSE*/,BOOL bChild /*=TRUE*/){

	//wire to label loop
	if(bOwn&&m_pshChildConn){
		m_uiConn=m_pshConn->m_uiShapeId;
		m_uiChildConn=-1;
		for(int i2=0;i2<((CShapeContainer*)m_pshConn)->m_obarrShapearr.GetSize();i2++){

			CShape *psh2=(CShape *)((CShapeContainer*)m_pshConn)->m_obarrShapearr.GetAt(i2);

			if(psh2==m_pshChildConn){

				m_uiChildConn=i2;
				break;
			}
		}
		ASSERT(m_uiChildConn>=0);
	}


	//wire to wire loop or wire to unit loop
	if(bChild)
	for(int i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		if(psh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShCont=(CShapeContainer *)psh;

			BOOL bConnect=pShCont->m_bConect;

			if(psh->m_pshChildConn){
				psh->m_uiConn=psh->m_pshConn->m_uiShapeId;

				psh->m_uiChildConn=-1;
				for(int i2=0;i2<((CShapeContainer*)psh->m_pshConn)->m_obarrShapearr.GetSize();i2++){

					CShape *psh2=(CShape *)((CShapeContainer*)psh->m_pshConn)->m_obarrShapearr.GetAt(i2);

					if(psh2==psh->m_pshChildConn){

						psh->m_uiChildConn=i2;
						break;
					}
				}
				ASSERT(psh->m_uiChildConn>=0);
			}
		}

	}
}

//Encuentra el índice de una shape en la shape padre.
int CShapeContainer::GetIndex(){

	CShape *pSh=GetParent();

	if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

		CShapeContainer *pShCont=(CShapeContainer *)pSh;

		for(int i=0;i<pShCont->m_obarrShapearr.GetSize();i++){

			if(pShCont->m_obarrShapearr[i]==pSh){

				return i;
			}
		}
		return -1;
	}
	else{

		return -1;
	}
}

//Encuentra el primer segmento de un cable.
CShape *CShapeContainer::GetFirstChainItem() const{

	//Local variables
	CShape *pSh=(CShape*)this;
	CShape *pSh0=(CShape*)this;
	int counter=0;

	if(m_obarrShapearr.GetSize()>0){
		CShape *psh=(CShape*)this;
		while( psh && psh->IsKindOf(RUNTIME_CLASS(CShapeWire))&&((CShapeContainer*)psh)->m_obarrShapearr.GetSize()>0 ){

			pSh=psh;
			psh=(CShape *)((CShapeContainer*)psh)->m_obarrShapearr.GetAt(0);
			psh=psh->m_pshConn;

			//2013/04/22
			//No use for this code
			/*
			if( (!psh->IsKindOf(RUNTIME_CLASS(CShapeWire))) && ((CShapeContainer*)psh)->m_obarrShapearr.GetSize()>0 ){
				for(int i=0;i<((CShapeContainer*)psh)->m_obarrShapearr.GetSize();i++){
					CShape *pshtest=(CShape *)((CShapeContainer*)psh)->m_obarrShapearr.GetAt(i);
					pshtest=pshtest->m_pshConn;
					if(pshtest==pSh0){
						psh=(CShape *)((CShapeContainer*)pSh0)->m_obarrShapearr.GetAt(1);
						psh=psh->m_pshConn;
						counter++;
						break;
					}
				}
			}
			*/
		}
	}

	return pSh;
}

//Encuentra el siguiente segmento de un cable.
void CShapeContainer::GetNextChainItem(CShape *& pSh,CShape* pShPrev) const{

	pSh=(CShape*)this;
	if(m_obarrShapearr.GetSize()>0){

		CShape *psh=(CShape *)((CShapeContainer*)pSh)->m_obarrShapearr.GetAt(1);
		//Debug
		CShape *psh0=(CShape *)((CShapeContainer*)pSh)->m_obarrShapearr.GetAt(0);
		psh0=psh0->m_pshConn;

		psh=psh->m_pshConn;
		
		//20/04/2013
		//Commented: causes loop
		/*
		if(pShPrev != NULL && psh==pShPrev){
			psh=psh0;
		}
		*/

		if(psh != NULL && psh->IsKindOf(RUNTIME_CLASS(CShapeWire))){
			pSh=psh;
		}
		else{
			pSh=NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeBoard

CShapeBoard::CShapeBoard()
{
	m_ShapeNumber=10;
	m_Alfa=20.0*PI/180;
	m_Beta=60.0*PI/180;
	m_d=0.20;
}

CShapeBoard::~CShapeBoard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapeBoard message handlers
//use this function to select
/*
void CShapeBoard::OnLButtonDown(UINT nFlags, CPoint point)
{


	// code afecting bounding rectangle can be placed in base class
	//CShape::OnLButtonDown(UINT nFlags, CPoint point);
	int i;

	if(m_Mode==_DRAFTDRAW_MODE_DRW){

		if(m_Rect.PtInRect(point)==TRUE){

			m_TypeSelect=_DRAFTDRAW_SEL_MOVING_RECT;

			PlaceSelectRect(m_MarkerArray,&m_Rect,_DRAFTDRAW_SEL_SIZE);

		}
	}
	else
	switch(m_TypeSelect){

	case _DRAFTDRAW_SEL_MOVING_RECT:

		while(m_MarkerArray[i]){

			if(m_MarkerArray[i]->PtInRect(point)==TRUE){

				m_TypeSelect=_DRAFTDRAW_SEL_RESIZING_RECT;
				m_VectorSelect=i;
			}

			i++;
		}

		break;


	}
}
//use this function release selection
void CShapeBoard::OnLButtonUp(UINT nFlags, CPoint point)
{


	// code afecting bounding rectangle can be place in base class
	//CShape::OnLButtonUp(UINT nFlags, CPoint point);
	m_Mode=_DRAFTDRAW_MODE_DRW;
	int i=0;
	while(m_MarkerArray[i]){

		delete(m_MarkerArray[i]);
		m_MarkerArray[i]=NULL;

		i++;
	}



}
//use this function to recalculate while resizing
void CShapeBoard::OnMouseMove(UINT nFlags, CPoint point)
{
	// code afecting bounding rectangle can be placed in base class
	//CShape::OnMouseMove(UINT nFlags, CPoint point);

	if(m_Mode==_DRAFTDRAW_MODE_SEL){
	switch(m_TypeSelect){

	case _DRAFTDRAW_SEL_SIZING_RECT:

		m_Rect=CRect(m_Point,point);
		m_Rect.NormalizeRect();

		break;

	case _DRAFTDRAW_SEL_RESIZING_RECT:

		CPoint vector_offset=CPoint(point-m_Point);
		float vector_x;
		float vector_y;
		bool flag_move=false;
		


		switch(m_VectorSelect){

		case _DRAFTDRAW_SEL_RESIZING_RECT_NW:
			vector_x=-0.7071;
			vector_y=-0.7071;
			flag_move=true;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_NE:
			vector_x=0.7071;
			vector_y=-0.7071;
			flag_move=true;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_SE:
			vector_x=0.7071;
			vector_y=0.7071;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_SW:
			vector_x=-0.7071;
			vector_y=0.7071;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_N:
			vector_x=0;
			vector_y=-1;
			flag_move=true;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_E:
			vector_x=1;
			vector_y=0;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_S:
			vector_x=0;
			vector_y=-1;
			break;
		case _DRAFTDRAW_SEL_RESIZING_RECT_W:
			vector_x=1;
			vector_y=0;
			flag_move=true;
			break;

		}

		vector_x*=vector_offset.x;
		vector_y*=vector_offset.x;

		if(vector_x<0) vector_x=0.0;
		if(vector_y<0) vector_y=0.0;

		if(flag_move){

			if(vector_offset.x>0){

				vector_offset.x=0.0;
			}

			if(vector_offset.y>0){

				vector_offset.y=0.0;
			}

			m_Rect+=CPoint(vector_x*vector_offset.x/abs(vector_offset.x),
				vector_y*vector_offset.y/abs(vector_offset.y));
		}

		//rect: l,t,b,r
		m_Rect+=CRect(0,0,vector_y,vector_x);

		//repaint markers
		PlaceSelectRect(m_MarkerArray,&m_Rect,_DRAFTDRAW_SEL_SIZE);
		break;

	}



	}
}
*/
void CShapeBoard::OnDraw(CDC *pDC){

	if((!m_Mode)&&(!m_Rect.IsRectEmpty())){

		CShape::OnDraw(pDC);
	}
	else if(!m_Rect.IsRectEmpty()){

		//draws using PolyBezier
		//needs calculating P1,P2,P3,P4,P5,P6,p7
        //---------------------------------------
        //|             xxxxxxxxxxxx|xxx        |
        //|   xxx              alfap|betap    x |
        //|xxxalfa                  |      betax|   b
        //|   xxx                   |         x |
        //|             xxxxxxxxxxxx|xxx        |
        //---------------------------------------
		//
		//<------------------- a --------------->
		//                           <---- d---->

		int a=m_Rect.Width();
		int b=m_Rect.Height();
		int d=m_d*a;
		

		for(int i=0;i<m_ShapeNumber;i++){


			CPoint p[10];

			float fdata=i;
			fdata/=(m_ShapeNumber-1);

			int h=b*fdata;
			float alfa=m_Alfa*(0.5-fdata)*2;
			float alfap=0.0;
			float beta=m_Beta*(0.5-fdata)*2;
			float betap=0.0;



			p[0]=CPoint(m_Rect.TopLeft().x,m_Rect.CenterPoint().y);

			p[1]=p[0]+CPoint((a-d)/3,(d-a)*tan(alfa)/3);

			p[3]=CPoint(m_Rect.BottomRight().x-d,m_Rect.TopLeft().y+h);

			p[2]=p[3]+CPoint((d-a)/3,(a-d)*tan(alfap)/3);

			p[4]=p[3]+CPoint(d/3,d*tan(betap)/3);

			p[6]=CPoint(m_Rect.BottomRight().x,m_Rect.CenterPoint().y);

			p[5]=p[6]+CPoint(-d/3,-d*tan(beta)/3);


			pDC->PolyBezier(p,7);
			//pDC->Polyline(p,7);

			//to draw selections
			CShape::OnDraw(pDC);
		}

	}



}

/////////////////////////////////////////////////////////////////////////////
// CShapePolyline

/*IMPLEMENT_DYNCREATE(CShapePolyline, CShapeContainer)*/
IMPLEMENT_SERIAL(CShapePolyline, CShapeContainer, 1)

CShapePolyline::CShapePolyline(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapeContainer(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapePolyline;
	m_Type=1;
	m_bLineResize=TRUE;//resizing as a rectangle
	m_bAllowsPartialDraw=TRUE;

	m_ShapeNumber=0;
	m_bClosed=FALSE;
}

CShapePolyline::~CShapePolyline()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapePolyline message handlers
void CShapePolyline::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){

	int i;
	int min_x,min_y,max_x,max_y;

	switch(nChar){

	case 27://ESC

		if(m_dwarrPointarr.GetSize()){

			CPoint point=CPoint(m_dwarrPointarr.GetAt(0));

			min_x=point.x;
			min_y=point.y;
			max_x=point.x;
			max_y=point.y;

			for(i=1;i<m_dwarrPointarr.GetSize();i++){

				point=CPoint(m_dwarrPointarr.GetAt(i));

				if(point.x<min_x){

					min_x=point.x;
				}
				if(point.y<min_y){

					min_y=point.y;
				}
				if(point.x>max_x){

					max_x=point.x;
				}
				if(point.y>max_y){

					max_y=point.y;
				}

			}

			m_Rect=CRect(min_x,min_y,max_x,max_y);
			m_Rect0=m_Rect;

			CPoint point_org=m_Rect.TopLeft();

			//Polyline normalization
			m_dwarrPointarr0.RemoveAll();
			for(i=0;i<m_dwarrPointarr.GetSize();i++){

				point=CPoint(m_dwarrPointarr.GetAt(i))-point_org;

				m_dwarrPointarr.SetAt(i,MAKELONG(point.x,point.y));
				m_dwarrPointarr0.Add(MAKELONG(point.x,point.y));
			}

			//Set flag to ended
			m_bClosed=TRUE;
		}

		break;
	}

	CShapeContainer::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CShapePolyline::OnLButtonDown(UINT nFlags, CPoint point){

	DWORD dwPoint=MAKELONG(point.x,point.y);

	if((m_Mode==_DRAFTDRAW_MODE_SEL)&&(m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT)){
		if(m_dwarrPointarr.GetSize()>1){

			m_dwarrPointarr.Add(dwPoint);

			CShape::OnLButtonUp(nFlags,point);

			m_Mode=_DRAFTDRAW_MODE_SEL;
		}
		else{

			m_dwarrPointarr.Add(dwPoint);
		}
	}

	CShape::OnLButtonDown(nFlags,point);
}

void CShapePolyline::OnLButtonUp(UINT nFlags, CPoint point){

	if((m_Mode==_DRAFTDRAW_MODE_SEL)&&(m_TypeSelect==_DRAFTDRAW_SEL_SIZING_RECT)){
/*
		if(m_dwarrPointarr.GetSize()<=1){
			DWORD dwPoint=MAKELONG(point.x,point.y);

			m_dwarrPointarr.Add(dwPoint);

			CShape::OnLButtonUp(nFlags,point);


			m_Mode=_DRAFTDRAW_MODE_SEL;
//			CShape::OnLButtonDown(nFlags,point);
		}
*/
		DWORD status=GetStatus();
		CShape::OnLButtonUp(nFlags,point);
		SetStatus(status);

	}
	else if((m_Mode==_DRAFTDRAW_MODE_SEL)&&(m_TypeSelect==_DRAFTDRAW_SEL_RESIZING_RECT)){
		if(m_Rect!=m_Rect0){
			CRect rectTemp=m_Rect;
			m_Rect.NormalizeRect();


			float scale_x=m_Rect.Width();
			scale_x/=m_Rect0.Width();
			float scale_y=m_Rect.Height();
			scale_y/=m_Rect0.Height();
			for(int i=0;i<m_dwarrPointarr.GetSize();i++){
				float fdata_x=LOWORD(m_dwarrPointarr0[i]);
				fdata_x*=scale_x;
				if(rectTemp.left != m_Rect.left){
					fdata_x=m_Rect.Width() - fdata_x;
				}

				float fdata_y=HIWORD(m_dwarrPointarr0[i]);
				fdata_y*=scale_y;

				if(rectTemp.bottom != m_Rect.bottom){
					fdata_y=m_Rect.Height() - fdata_y;
				}

				m_dwarrPointarr[i]=MAKELONG(fdata_x,fdata_y);
			}
			CShape::OnLButtonUp(nFlags,point);
			return;
		}
	}
	else{

		CShape::OnLButtonUp(nFlags,point);
		return;
	}

	
}

void CShapePolyline::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_dwarrPointarr.GetSize()>2){

		m_dwarrPointarr.RemoveAt(m_dwarrPointarr.GetSize()-1);

		m_dwarrPointarr.Add(m_dwarrPointarr[0]);

		CShape::OnLButtonUp(nFlags,point);
	}
}

void CShapePolyline::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShapeContainer::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		archive << m_dwarrPointarr.GetSize();
		for(int i=0;i<m_dwarrPointarr.GetSize();i++){
			archive << m_dwarrPointarr[i];
		}
	}
    else{
		int nCount;
		archive >> nCount;
		for(int i=0;i<nCount;i++){
			DWORD dwData;
			archive >> dwData;
			m_dwarrPointarr.Add(dwData);
			m_dwarrPointarr0.Add(dwData);
			if(m_Rect0.IsRectNull()){
				m_Rect0=m_Rect;
			}
		}
	}

}

void CShapePolyline::SerializeXml(CXMLArchive& archive)
{
	//Get pointer to current node
	CXMLArchiveNode* curNodePtr = archive.GetCurrentNode();

	//Sample path line in svg file
	//<path d="M 995 1040 L 995 910 Z" stroke="#000" fill="none"/>

	CString d = archive.m_xmlDocPtr->GetAttrib("d");
	CString stroke = archive.m_xmlDocPtr->GetAttrib("stroke");
	CString fill = archive.m_xmlDocPtr->GetAttrib("fill");

	//Parse d
	//Tokenize
	CString resToken;
	int curPos = 0;
	resToken = d.Tokenize(_T(" "), curPos);
	CString x_str = "";
	CString y_str = "";

	//Point counter
	int countPoint = 0;
	CArray<CPoint, CPoint> arrPoints;
	while (resToken != _T(""))
	{
		
		if (resToken.MakeUpper() == "M" ||
			resToken.MakeUpper() == "L" ||
			resToken.MakeUpper() == "Z"){
			if (countPoint > 0){
			arrPoints.Add(CPoint(atoi(x_str), atoi(y_str)));
				x_str = "";
				y_str = "";
			}
			countPoint++;
		}
		else if (x_str == "")
		{
			x_str = resToken;
		}
		else
		{
			y_str = resToken;
		}


		resToken = d.Tokenize(_T(" "), curPos);
	};

	/*
	int idata = CDraftDrawDoc::Split(d, " ", NULL, 0);
	LPTSTR *sa = new LPTSTR[idata];
	CDraftDrawDoc::Split(d, " ", sa, idata);

	CString x_str = "";
	CString y_str = "";
	int count = 0;
	CArray<CPoint, CPoint> arrPoints;
	for (int i = 0; i < idata; i++)
	{
		if (CString(sa[i]).MakeUpper() == "M" ||
			CString(sa[i]).MakeUpper() == "L" ||
			CString(sa[i]).MakeUpper() == "Z"){
			arrPoints.Add(CPoint(atoi(x_str), atoi(y_str)));
			x_str = "";
			y_str = "";
			count++;
		}
		else if (x_str == "")
		{
			x_str = sa[i];
		}
		else
		{
			y_str = sa[i];
		}
	}//end for
	*/

	//Create polyline
	//Prepare point array
	LPPOINT pPoints = new POINT[countPoint];

	for (int i = 0; i < arrPoints.GetCount(); i++)
	{
		CPoint point = arrPoints.GetAt(i);
		pPoints[i] = point;
	}

	//Create polyline
	Create(pPoints, arrPoints.GetCount());
}

void CShapePolyline::SerializeDdw(CDdwioFile &ddwfile)
{
	int count=m_dwarrPointarr.GetSize();

	CPoint *p=new CPoint[count];

	for(int i=0;i<count;i++){

		p[i]=CPoint(m_dwarrPointarr[i])+m_Rect.TopLeft();
	}

	ddwfile.WritePolyline(p,count);

	delete(p);
}

void CShapePolyline::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapePolyline::SerializeSvg(CSvgioFile &svgfile)
{
	int count = m_dwarrPointarr.GetSize();

	CPoint *p = new CPoint[count];

	for (int i = 0; i < count; i++){

		p[i] = CPoint(m_dwarrPointarr[i]) + m_Rect.TopLeft();
	}

	if (count > 1){
		svgfile.OpenPath();
		svgfile.WriteMoveTo(p[0]);
		for (int i = 1; i < count; i++){
			svgfile.WriteLineTo(p[i]);
		}
		svgfile.ClosePath();
	}

	delete(p);
}

void CShapePolyline::OnDraw(CDC *pDC){

	if(m_Mode==_DRAFTDRAW_MODE_SEL){

		//07/12/2004
		//This draw in selected mode was commented since a buffer is used to draw shapes.
		//See 07/12/2004 notes in 1ST LOOP of function CDrafDrawView::OnDraw().

		if((m_dwarrPointarr.GetSize()) &&(m_TypeSelect!=_DRAFTDRAW_SEL_SIZING_RECT)){

			if(m_TypeSelect==_DRAFTDRAW_SEL_RESIZING_RECT){
				if(m_Rect!=m_Rect0){
					float scale_x=m_Rect.Width();
					scale_x/=m_Rect0.Width();
					float scale_y=m_Rect.Height();
					scale_y/=m_Rect0.Height();
					for(int i=0;i<m_dwarrPointarr.GetSize();i++){
						float fdata_x=LOWORD(m_dwarrPointarr0[i]);
						fdata_x*=scale_x;
						float fdata_y=HIWORD(m_dwarrPointarr0[i]);
						fdata_y*=scale_y;
						m_dwarrPointarr[i]=MAKELONG(fdata_x,fdata_y);
					}
				}
			}

			pDC->MoveTo(m_Rect.TopLeft()+CPoint(m_dwarrPointarr.GetAt(0)));

			for(int i=1;i<m_dwarrPointarr.GetSize();i++){

				pDC->LineTo(m_Rect.TopLeft()+CPoint(m_dwarrPointarr.GetAt(i)));
			}

			//Draw in current position
			if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
				pDC->MoveTo(m_RectLast.TopLeft()+CPoint(m_dwarrPointarr.GetAt(0)));

				for(int i=1;i<m_dwarrPointarr.GetSize();i++){

					pDC->LineTo(m_RectLast.TopLeft()+CPoint(m_dwarrPointarr.GetAt(i)));
				}
			}
		}

		CShape::OnDraw(pDC);
	}
	else{

		if(m_dwarrPointarr.GetSize()){

			GraphicsPath gfxPath;

			pDC->MoveTo(m_Rect.TopLeft()+CPoint(m_dwarrPointarr.GetAt(0)));

			CPoint prevPoint = m_Rect.TopLeft() + CPoint(m_dwarrPointarr.GetAt(0));

			//Aux point for gradients
			CPoint point = prevPoint;
			CPoint point1 = prevPoint;
			CPoint point2 = prevPoint;
			CPoint point_left = prevPoint;
			CPoint point_top = prevPoint;
			CPoint point_bottom = prevPoint;
			CPoint point_right = prevPoint;

			for(int i=1;i<m_dwarrPointarr.GetSize();i++){

				point = m_Rect.TopLeft() + CPoint(m_dwarrPointarr.GetAt(i));

				pDC->LineTo(point);

				//Gradient
				gfxPath.AddLine(prevPoint.x, prevPoint.y, point.x, point. y);
				prevPoint = point;
				point_left = (point.x < point_left.x ? point : point_left);
				point_right = (point.x > point_right.x ? point : point_right);
				point_top = (point.y < point_top.y ? point : point_top);
				point_bottom = (point.y > point_bottom.y ? point : point_bottom);
			}
			/*
			if (m_dwarrPointarr.GetSize()>1){
				point2 = m_Rect.TopLeft() + CPoint(m_dwarrPointarr.GetAt(1));
			}
			*/

			if (point_right.x - point_left.x < point_bottom.y - point_top.y){
				point1 = point_left;
				point2 = CPoint(point_right.x, point_left.y);
			}
			else
			{
				point1 = point_top;
				point2 = CPoint(point_top.x, point_bottom.y);
			}

			if (m_bTransparent == FALSE)
			{
				this->DoFill(pDC, &gfxPath, point1, point2);
			}
		}

		CShape::OnDraw(pDC);

	}

}

void CShape::SetProyect(CPoint3D point)
{
	m_VectorProyect=point;
}

void CShape::SetProyect(int x,int y,int z)
{
	m_VectorProyect=CPoint3D(x,y,z);
}

/////////////////////////////////////////////////////////////////////////////
// Point3D
CPoint3D::CPoint3D()
{
	x=0;
	y=0;
	z=0;
}


CPoint3D::CPoint3D(int x1,int y1,int z1)
{
	x=x1;
	y=y1;
	z=z1;
}

CPoint3D::~CPoint3D()
{
}

BOOL CPoint3D::operator==(CPoint3D point){

if	((x!=point.x)||
	(y!=point.y)||
	(z!=point.z)){


	return FALSE;
}


return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers


void CShape::SetOrg(CPoint org)
{
	m_PointOrg=org;
}

void CShape::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );

    // now do the stuff for our specific class
    if( archive.IsStoring() ){
		//CString sVer="1.00";
		//CString sVer="1.01";	//added m_uiShapeId;
		//CString sVer="1.02";	//added m_strTypeIdent;
		//CString sVer="1.03";	//added m_bConect,m_uiConn,m_uiConnChild
		//CString sVer="1.04";	//added m_bNoResize,m_uiShapeType,label::style,m_Rect0,CShapeUnit::m_pPoints0
		//CString sVer="1.05";	//added 'TB' serialization in CDocument
		//CString sVer="1.06";	//added 'TB' serialization in CDocument with support for 'TB' associated to wires
		CString m_sVer="1.07";	//serialization of fill colors (front & background)
		m_fVer=atof(m_sVer);
		archive << m_sVer;
		int idata=9;
		archive << idata;
        archive << m_Rect;
		archive << m_Mode;
		archive << m_strIdent;
		archive << m_uiShapeId;
		archive << m_strTypeIdent;
		//1.03
		archive << m_bConect;
		archive << m_uiConn;
		archive << m_uiChildConn;
		//1.04
		archive << m_bNoResize;
		archive << m_Rect0;
		archive << m_uiShapeType;
		//1.05
		//'TB' serialization in CDocument
		//1.07
		archive << m_bTransparent;
		archive << m_crFill;
		archive << m_crFillBgnd;
	}
    else{
		//CString sVer;
		//float fVer;
		int nCount;
		archive >> m_sVer;
		m_fVer=atof(m_sVer);
		archive >> nCount;
		archive >> m_Rect;
		archive >> m_Mode;
		archive >> m_strIdent;
		//1.01
		float fVer0=1.01;
		float fData=m_fVer-fVer0;
		if(fData>=0){
			archive >> m_uiShapeId;
		}
		//1.02
		fVer0=1.02;
		fData=m_fVer-fVer0;
		if(fData>=0){
			archive >> m_strTypeIdent;
		}
		//1.03
		fVer0=1.03;
		fData=m_fVer-fVer0;
		if(fData>=0){
			archive >> m_bConect;
			archive >> m_uiConn;
			archive >> m_uiChildConn;

		}
		//1.04
		fVer0=1.04;
		fData=m_fVer-fVer0;
		if(fData>=0){
			archive >> m_bNoResize;
			archive >> m_Rect0;
			archive >> m_uiShapeType;
		}
		//1.05
		//'TB' serialization in CDocument
		//1.07
		fVer0 = 1.07;
		fData = m_fVer - fVer0;
		if (fData >= 0){
			archive >> m_bTransparent;
			archive >> m_crFill;
			archive >> m_crFillBgnd;
		}
	}
}

void CShape::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShape::SerializeDdw(CDdwioFile &ddwfile)
{

}

void CShape::SerializeSvg(CSvgioFile &svgfile)
{

}

void CShape::GetRectTemp(CRect &rect){

}

void CShape::GetRectUpdatePlace(CRect &rect){

}

void CShapeRect::Serialize( CArchive& archive )
{

    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		//
	}
    else{
        //archive >> m_name >> m_number;
	}

}

void CShapeRect::SerializeGbr(CGbrioFile &gbrfile)
{
	CPoint p1=m_Rect.TopLeft();
	CPoint p3=m_Rect.BottomRight();

	CPoint p2=CPoint(p3.x,p1.y);
	CPoint p4=CPoint(p1.x,p3.y);

	gbrfile.WriteMoveTo(p1);
	gbrfile.WriteLineTo(p2);
	gbrfile.WriteLineTo(p3);
	gbrfile.WriteLineTo(p4);
	gbrfile.WriteLineTo(p1);
}

void CShapeRect::SerializeSvg(CSvgioFile &svgfile)
{
	CPoint p1 = m_Rect.TopLeft();
	CPoint p3 = m_Rect.BottomRight();

	CPoint p2 = CPoint(p3.x, p1.y);
	CPoint p4 = CPoint(p1.x, p3.y);

	svgfile.OpenPath();
	svgfile.WriteMoveTo(p1);
	svgfile.WriteLineTo(p2);
	svgfile.WriteLineTo(p3);
	svgfile.WriteLineTo(p4);
	svgfile.WriteLineTo(p1);
	svgfile.ClosePath();
}

void CShapeRect::SerializeDdw(CDdwioFile &ddwfile)
{
	CPoint p[5];
	p[0]=m_Rect.TopLeft();//0;
	p[2]=m_Rect.BottomRight();//-m_Rect.TopLeft();

	p[1]=CPoint(p[2].x,p[0].y);
	p[3]=CPoint(p[0].x,p[2].y);

	p[4]=p[0];

	ddwfile.WritePolyline(p,5);
}

void CShapeBoard::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeEllipse::Serialize( CArchive& archive )
{

    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		//
	}
    else{
        //archive >> m_name >> m_number;
	}

}

void CShapeEllipse::SerializeDdw(CDdwioFile &ddwfile)
{
	CPoint p[5];
	p[0]=m_Rect.TopLeft();//0;
	p[2]=m_Rect.BottomRight();//-m_Rect.TopLeft();

	p[1]=CPoint(p[2].x,p[0].y);
	p[3]=CPoint(p[0].x,p[2].y);

	p[4]=p[0];

	if(m_bTransparent){
		ddwfile.WriteEllipse(&m_Rect);
	}
	else{
		ddwfile.WriteEllipse(&m_Rect,&m_crFill);
	}
}

void CShapeEllipse::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeArc::SerializeGbr(CGbrioFile &gbrfile)
{
	CPoint P1=m_P1;//m_Point;//m_Rect.TopLeft();

	CPoint P2=m_P2;//m_Point2;//m_Rect.BottomRight();


	//CRect rect;
	//BoundRect(P1,P2,10,rect);

	m_Alfap=m_Alfa;
	RotateAxis(P1,P2,b,m_Alfap);


	CPoint point_array[100];
	DrawEllipse(point_array,100,P1,P2,b,m_Alfap);

	gbrfile.WriteMoveTo(point_array[0]);
	for(int i=1;i<100;i++){

		gbrfile.WriteLineTo(point_array[i]);
	}

}

//Create a polyline from an array of points
BOOL CShapePolyline::Create(LPPOINT lpPoints, int nCount)
{
	int x_min=lpPoints[0].x,y_min=lpPoints[0].y;
	int x_max=lpPoints[0].x,y_max=lpPoints[0].y;
	int i;
	for(int i=0;i<nCount;i++){
		if(lpPoints[i].x<x_min){
			x_min=lpPoints[i].x;
		}
		else if(lpPoints[i].x>x_max){
			x_max=lpPoints[i].x;
		}
		if(lpPoints[i].y<y_min){
			y_min=lpPoints[i].y;
		}
		else if(lpPoints[i].y>y_max){
			y_max=lpPoints[i].y;
		}
	}
	/*m_Rect=CRect(CPoint(0,0),CPoint(1,1));*//*CRect(lpPoints[0],lpPoints[nCount-1]);*/
	if(x_min==x_max) x_max+=10;
	if(y_min==y_max) y_max+=10;
	m_Rect=CRect(CPoint(x_min,y_min),CPoint(x_max,y_max));
m_Rect0=m_Rect;
	for(i=0;i<nCount;i++){
		CPoint point=lpPoints[i];
		point-=m_Rect.TopLeft();
		m_dwarrPointarr.Add(MAKELONG(point.x,point.y));
		m_dwarrPointarr0.Add(MAKELONG(point.x,point.y));
	}

	m_Mode=_DRAFTDRAW_MODE_DRW;//!

	return TRUE;
}

//Create a rectangle
BOOL CShapePolyline::Create(LPRECT lpRect){

	CPoint ptArray[5];

	ptArray[0]=CPoint(lpRect->left,lpRect->top);
	ptArray[1]=CPoint(lpRect->right,lpRect->top);
	ptArray[2]=CPoint(lpRect->right,lpRect->bottom);
	ptArray[3]=CPoint(lpRect->left,lpRect->bottom);
	ptArray[4]=CPoint(lpRect->left,lpRect->top);

	return Create(ptArray,5);
}

//Creates a polygon
BOOL CShapePolyline::Create(LPRECT lpRect,int nCount){

	CPoint *ptArray=new CPoint[nCount+1];

	CreatePointArray(lpRect,ptArray,nCount);
/*
	float alfa=2.0*PI/nCount;
	CSize offset=CRect(lpRect).CenterPoint();
	float a=CRect(lpRect).Width()/2;
	float b=CRect(lpRect).Height()/2;
	for(int i=0;i<nCount;i++){

		CPoint pt1=CPoint(a*cos(alfa*i),b*sin(alfa*i));
		ptArray[i]=pt1+offset;
	}
	ptArray[i]=CPoint(a,0)+offset;
*/
	BOOL result=Create(ptArray,nCount+1);
	delete(ptArray);
	return result;
}

BOOL CShapePolyline::CreateNStar(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CPoint *ptArray1=new CPoint[nCount+1];
	CPoint *ptArray2=new CPoint[nCount+1];
	CPoint *ptArray=new CPoint[2*nCount+1];

	CreatePointArray(lpRect,ptArray1,nCount);

	CRect rect(lpRect);
	float fdata1=rect.Width();
	fdata1=fdata1*percDeep/200.0;
	float fdata2=rect.Height();
	fdata2=fdata2*percDeep/200.0;

	rect.DeflateRect(fdata1,fdata2);
	CreatePointArray(rect,ptArray2,nCount,180.0/nCount);

	int i;
	for(i=0;i<nCount;i++){

		ptArray[2*i]=ptArray1[i];
		ptArray[2*i+1]=ptArray2[i];
	}
	ptArray[2*i]=ptArray1[i];

	BOOL result=Create(ptArray,2*nCount+1);

	delete(ptArray1);
	delete(ptArray2);
	delete(ptArray);
	return result;

}

BOOL CShapePolyline::CreateNGear(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CPoint *ptArray1=new CPoint[2*nCount+1];
	CPoint *ptArray2=new CPoint[2*nCount+1];
	CPoint *ptArray=new CPoint[4*nCount+1];

	CreatePointArray(lpRect,ptArray1,2*nCount);
	
	CRect rect(lpRect);
	float fdata1=rect.Width();
	fdata1=fdata1*percDeep/200.0;
	float fdata2=rect.Height();
	fdata2=fdata2*percDeep/200.0;
	
	rect.DeflateRect(fdata1,fdata2);
	CreatePointArray(rect,ptArray2,2*nCount);

	int i;
	for(i=0;i<nCount;i++){

		ptArray[4*i]=ptArray1[2*i];
		ptArray[4*i+1]=ptArray2[2*i];
		ptArray[4*i+2]=ptArray2[2*i+1];
		ptArray[4*i+3]=ptArray1[2*i+1];
	}
	ptArray[4*i]=ptArray1[2*i];

	BOOL result=Create(ptArray,4*nCount+1);

	delete(ptArray1);
	delete(ptArray2);
	delete(ptArray);
	return result;

}

BOOL CShapePolyline::CreateNCross(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CPoint *ptArray1=new CPoint[2*nCount+1];
	CPoint *ptArray2=new CPoint[nCount+1];
	CPoint *ptArray=new CPoint[3*nCount+1];

	float fOffset=CreatePointArray(lpRect,ptArray1,2*nCount,0.0,TRUE);
	
	CRect rect(lpRect);
	float fdata1=rect.Width();
	fdata1=fdata1*percDeep/200.0;
	float fdata2=rect.Height();
	fdata2=fdata2*percDeep/200.0;
	
	rect.DeflateRect(fdata1,fdata2);
	CreatePointArray(rect,ptArray2,nCount,90.0/nCount+fOffset,FALSE);

	int i;
	for(i=0;i<nCount;i++){

		ptArray[3*i]=ptArray1[2*i];
		ptArray[3*i+1]=ptArray2[i];
		ptArray[3*i+2]=ptArray1[2*i+1];
	}
	ptArray[3*i]=ptArray1[2*i];

	BOOL result=Create(ptArray,3*nCount+1);

	delete(ptArray1);
	delete(ptArray2);
	delete(ptArray);
	return result;

}

float CShapePolyline::CreatePointArray(LPRECT lpRect,LPPOINT ptArray,int nCount,float degOffset/*=0.0*/,BOOL bAlign /*=TRUE*/){

	//Initial default offset
	float degOff0=0.0;
	if(bAlign){
		degOff0=-90.0;

		//Check if number of sides is 4 divisible
		if(nCount/4*4==nCount){
			//If so, offset=(1/2)*(360.0/sides)
			degOff0=180.0/nCount;
		}
	}

	float degOff=(degOffset+degOff0)*PI/180.0;
	float alfa=2.0*PI/nCount;
	CSize offset=CRect(lpRect).CenterPoint();
	float a=CRect(lpRect).Width()/2;
	float b=CRect(lpRect).Height()/2;
	int i;
	for(i=0;i<nCount;i++){

		CPoint pt1=CPoint(a*cos(alfa*i+degOff),b*sin(alfa*i+degOff));
		ptArray[i]=pt1+offset;
	}
	ptArray[i]=CPoint(a*cos(alfa*i+degOff),b*sin(alfa*i+degOff))+offset;//CPoint(a,0)+offset;
	return degOff0;
}

//Return nCount of polyline in array
int CShapePolyline::GetPoints(LPPOINT lpPoints,int nCount){

	int i;
	for(i=0;i<m_dwarrPointarr.GetSize()&&i<nCount;i++){
		lpPoints[i]=CPoint(m_dwarrPointarr[i])+m_Rect.TopLeft();
	}
	return i;
}

//Return number of points in polyline
int CShapePolyline::GetSize(){

	return m_dwarrPointarr.GetSize();
}

void CShapePolyline::GetRectTemp(CRect &rect){

	int i;
	int min_x,min_y,max_x,max_y;

	if(m_dwarrPointarr.GetSize()){

		CPoint point=CPoint(m_dwarrPointarr.GetAt(0));

		min_x=point.x;
		min_y=point.y;
		max_x=point.x;
		max_y=point.y;

		//Get bounding rectangle
		for(i=1;i<m_dwarrPointarr.GetSize();i++){

			point=CPoint(m_dwarrPointarr.GetAt(i));

			if(point.x<min_x){

				min_x=point.x;
			}
			if(point.y<min_y){

				min_y=point.y;
			}
			if(point.x>max_x){

				max_x=point.x;
			}
			if(point.y>max_y){

				max_y=point.y;
			}

		}

		rect=CRect(min_x,min_y,max_x,max_y);

/*
		CPoint point_org=rect.TopLeft();

		for(i=0;i<m_dwarrPointarr.GetSize();i++){

			point=CPoint(m_dwarrPointarr.GetAt(i))-point_org;

			m_dwarrPointarr.SetAt(i,MAKELONG(point.x,point.y));

		}
*/
	}
	else{
		rect=CRect(0,0,0,0);
	}

}

void CShapePolyline::GetRectUpdatePlace(CRect &rect){
	CRect rectTot=new CRect(0, 0, 0, 0);
	CPoint point1, point2;

	if(m_dwarrPointarr.GetSize()>=2){
		//This if to check if the polyline is closed
		if(m_bClosed==TRUE){
			//This part is executed when polyline is complete and has been normalized
			point1=CPoint(m_dwarrPointarr.GetAt(0));
			point1+=m_Rect.TopLeft();
			point2=CPoint(m_dwarrPointarr.GetAt(1));
			point2+=m_Rect.TopLeft();
			rect=CRect(point1, point2);
			rect.NormalizeRect();
			rectTot.UnionRect(rectTot,rect);

			//Last segment of polyline
			point1=CPoint(m_dwarrPointarr.GetAt(m_dwarrPointarr.GetSize()-2));
			point1+=m_Rect.TopLeft();
			point2=CPoint(m_dwarrPointarr.GetAt(m_dwarrPointarr.GetSize()-1));
			point2+=m_Rect.TopLeft();
			rect=CRect(point1, point2);
			rect.NormalizeRect();
			rectTot.UnionRect(rectTot,rect);
			rect = rectTot;

			return;
		}
		//If it is not complete, then is not normalized
		point1=CPoint(m_dwarrPointarr.GetAt(m_dwarrPointarr.GetSize()-2));
		point2=CPoint(m_dwarrPointarr.GetAt(m_dwarrPointarr.GetSize()-1));
		rect=CRect(point1, point2);
		rect.NormalizeRect();
		rectTot.UnionRect(rectTot,rect);
		rect = rectTot;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeZoomRect

IMPLEMENT_DYNCREATE(CShapeZoomRect, CShape)

CShapeZoomRect::CShapeZoomRect()
{
	CShape::CShape();
	m_Type=1;
	m_bGroup=FALSE;
}

CShapeZoomRect::~CShapeZoomRect()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeRect message handlers
//use this function to select


void CShapeZoomRect::OnDraw(CDC *pDC)
{

	if(!m_Mode){

		CShape::OnDraw(pDC);
	}

	else if(!m_Rect.IsRectEmpty()){

		if(m_VectorProyect==CPoint3D(0,1,0)){

			CRect rect=CRect(
				m_Rect.TopLeft().x,
				0/*m_Rect.TopLeft().y*/,
				m_Rect.BottomRight().x,
				/*m_Rect.TopLeft().y*/-m_Height);

			pDC->Rectangle(rect+m_PointOrg);
			return;
		}

		if(m_bGroup){
			pDC->FrameRect(&m_Rect,&CBrush(RGB(0,0,0)));
		}
		else{
			pDC->Rectangle(m_Rect);
		}

		//to draw selections
		CShape::OnDraw(pDC);

	}
}


/////////////////////////////////////////////////////////////////////////////
// CShapePicture

/*IMPLEMENT_DYNCREATE(CShapePicture, CShape)*/
IMPLEMENT_SERIAL(CShapePicture, CShape, 1)

CShapePicture::CShapePicture(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{

	m_uiShapeType=ddcShapePicture;
	m_Type=1;
	m_bGroup=FALSE;

	gpPicture=NULL;
}

CShapePicture::~CShapePicture()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapePicture message handlers
//use this function to select


void CShapePicture::OnDraw(CDC *pDC)
{

	if(!m_Mode){

		CShape::OnDraw(pDC);
	}

	else if(!m_Rect.IsRectEmpty()){


		if (gpPicture)
		{
			// get width and height of picture
			long hmWidth;
			long hmHeight;
			gpPicture->get_Width(&hmWidth);
			gpPicture->get_Height(&hmHeight);
			// convert himetric to pixels
			int nWidth	= m_Rect.Width();//MulDiv(hmWidth, GetDeviceCaps(pDC->m_hDC, LOGPIXELSX), HIMETRIC_INCH);
			int nHeight	= m_Rect.Height();//MulDiv(hmHeight, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), HIMETRIC_INCH);
			//RECT rc;
			//GetClientRect(hWnd, &rc);
			// display picture using IPicture::Render
			gpPicture->Render(pDC->m_hDC, m_Rect.TopLeft().x, m_Rect.TopLeft().y, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, &m_Rect);
			//gpPicture->Render(hdc, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, &rc);
		}


		//to draw selections
		CShape::OnDraw(pDC);

	}
}

// This function loads a file into an IStream.
void CShapePicture::LoadPictureFile(LPCTSTR szFile)
{
	// open file
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	_ASSERTE(INVALID_HANDLE_VALUE != hFile);

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	_ASSERTE(-1 != dwFileSize);

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	_ASSERTE(SUCCEEDED(hr) && pstm);

	// Create IPicture from image file
	if (gpPicture)
		gpPicture->Release();
	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
	_ASSERTE(SUCCEEDED(hr) && gpPicture);	
	pstm->Release();

	//InvalidateRect(ghWnd, NULL, TRUE);

	m_sPath=szFile;
}

// This allows to choose a file.
void CShapePicture::ChooseFile(HWND hWnd)
{
	// get file name to open
	TCHAR szFile[MAX_PATH];
	ZeroMemory(szFile, MAX_PATH);
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	ofn.hwndOwner	= hWnd;
	ofn.lpstrFilter	= _T("Supported Files Types(*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf)\0*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf\0Bitmaps (*.bmp)\0*.bmp\0GIF Files (*.gif)\0*.gif\0JPEG Files (*.jpg)\0*.jpg\0Icons (*.ico)\0*.ico\0Enhanced Metafiles (*.emf)\0*.emf\0Windows Metafiles (*.wmf)\0*.wmf\0\0");
	ofn.lpstrTitle	= _T("Open Picture File");
	ofn.lpstrFile	= szFile;
	ofn.nMaxFile	= MAX_PATH;
	if (IDOK == GetOpenFileName(&ofn))
		// load the file
		LoadPictureFile(szFile);



}

void CShapePicture::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

    // now do the stuff for our specific class
}

/////////////////////////////////////////////////////////////////////////////
// CShapeCurve

/*IMPLEMENT_DYNCREATE(CShapeCurve, CShape)*/
IMPLEMENT_SERIAL(CShapeCurve, CShape, 1)

CShapeCurve::CShapeCurve(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapePolyline(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapeCurve;
	//m_Type=1;
	//m_bGroup=FALSE;

}

CShapeCurve::~CShapeCurve()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeCurve message handlers
//use this function to select

void CShapeCurve::OnDraw(CDC* pDC){

	if(!m_Mode){

		CShapePolyline::OnDraw(pDC);
	}
	else {

		if(m_dwarrPointarr.GetSize()){

//Status DrawCurve(
//	const Pen *pen,
//    const Point *points,
//    INT count,
//    INT offset,
//    INT numberOfSegments,
//    REAL tension
//);

			Pen blackPen(Color::Black, 1);
			Point *points=new Point[m_dwarrPointarr.GetSize()];
			int offset = 0;
			int segments = m_dwarrPointarr.GetSize()-1;
			REAL tension = 0.5;

			for(int i=0;i<m_dwarrPointarr.GetSize();i++){

				CPoint pt(m_dwarrPointarr.GetAt(i));
				pt+=m_Rect.TopLeft();
				points[i]=Point(pt.x,pt.y);
			}

			Graphics grf(pDC->m_hDC);
			grf.DrawCurve(&blackPen, points, m_dwarrPointarr.GetSize(), offset, segments, tension);
			//To close the curve use below method
			//grf.DrawClosedCurve(&blackPen, points, m_dwarrPointarr.GetSize(), tension);

			delete(points);

//			pDC->MoveTo(m_Rect.TopLeft()+CPoint(m_dwarrPointarr.GetAt(0)));

//			for(int i=1;i<m_dwarrPointarr.GetSize();i++){

//				pDC->LineTo(m_Rect.TopLeft()+CPoint(m_dwarrPointarr.GetAt(i)));
//			}

		}

		CShape::OnDraw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapePolyArc
/*
private void DrawRoundedRectangle(Graphics gfx, Rectangle Bounds, int CornerRadius, Pen DrawPen, Color FillColor)
{
    int strokeOffset = Convert.ToInt32(Math.Ceiling(DrawPen.Width));
    Bounds = Rectangle.Inflate(Bounds, -strokeOffset, -strokeOffset);

    DrawPen.EndCap = DrawPen.StartCap = LineCap.Round;

    GraphicsPath gfxPath = new GraphicsPath();
    gfxPath.AddArc(Bounds.X, Bounds.Y, CornerRadius, CornerRadius, 180, 90);
    gfxPath.AddArc(Bounds.X + Bounds.Width - CornerRadius, Bounds.Y, CornerRadius, CornerRadius, 270, 90);
    gfxPath.AddArc(Bounds.X + Bounds.Width - CornerRadius, Bounds.Y + Bounds.Height - CornerRadius, CornerRadius, CornerRadius, 0, 90);
    gfxPath.AddArc(Bounds.X, Bounds.Y + Bounds.Height - CornerRadius, CornerRadius, CornerRadius, 90, 90);
    gfxPath.CloseAllFigures();

    gfx.FillPath(new SolidBrush(FillColor), gfxPath);
    gfx.DrawPath(DrawPen, gfxPath);
}
*/
/*IMPLEMENT_DYNCREATE(CShapePolyArc, CShape)*/
IMPLEMENT_SERIAL(CShapePolyArc, CShape, 1)

CShapePolyArc::CShapePolyArc(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapePolyline(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapePolyarc;
	//m_Type=1;
	//m_bGroup=FALSE;

}

CShapePolyArc::~CShapePolyArc()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapePolyArc message handlers
//use this function to select

void CShapePolyArc::OnDraw(CDC* pDC){

	if(!m_Mode){

		CShapePolyline::OnDraw(pDC);
	}
	else {

		if(m_dwarrPointarr.GetSize()){

			int iSizeArr=m_dwarrPointarr.GetSize();

			if(m_dwarrPointarr.GetSize()>=2 && m_dwarrPointarr[0]==m_dwarrPointarr[m_dwarrPointarr.GetSize()-1]){
				iSizeArr--;
			}

			Pen blackPen(Color::Black, 1);
			Point *points=new Point[iSizeArr];
				double startAngle=0;
				double amplitude=90;

			GraphicsPath gfxPath;
			for(int i=0;i<iSizeArr;i++){

				CPoint pt(m_dwarrPointarr.GetAt(i));
				pt+=m_Rect.TopLeft();

				CPoint pt_prev;
				CPoint pt_next;
				double h_next=0;
				double w_next=0;
				//We need more than two points to calcule angles
				if(iSizeArr>=2){

					int index1=i-1;
					int index2=(i+1)%iSizeArr;

					index1=index1<0 ? index1+iSizeArr:index1;
					index2=index2<0 ? index2+iSizeArr:index2;

					pt_prev=m_dwarrPointarr.GetAt(index1);
					pt_next=m_dwarrPointarr.GetAt(index2);

					pt_prev+=m_Rect.TopLeft();
					pt_next+=m_Rect.TopLeft();

					CPoint v2=pt_prev-pt;
					CPoint v1=pt_next-pt;

					h_next=v1.y;
					w_next=v1.x;

					//Calcule amplitude of angle
					double prod_esc= v1.x*v2.x + v1.y*v2.y;
					double module = sqrt((double)(v1.x*v1.x + v1.y*v1.y)) * sqrt((double)(v2.x*v2.x + v2.y*v2.y));
					amplitude=acos(prod_esc/module) * 180.0 / PI;
					//amplitude=amplitude<90.0 ? 180.0-amplitude : amplitude;
					amplitude=180.0-amplitude;
				}
				else if(i>=1){
					pt_prev=m_dwarrPointarr.GetAt(i-1);
					pt_prev+=m_Rect.TopLeft();
				}
				else
				{
					pt_prev=m_dwarrPointarr.GetAt(iSizeArr-1);
					pt_prev+=m_Rect.TopLeft();
				}

				//Corner radious
				int CornerRadiusX=10;
				int CornerRadiusY=10;
				CRect Bounds(pt.x-CornerRadiusX,
					pt.y-CornerRadiusY,
					pt.x+CornerRadiusX,
					pt.y+CornerRadiusY);

				//int strokeOffset = Convert.ToInt32(Math.Ceiling(DrawPen.Width));
				//Rectangle.Inflate(Bounds, -strokeOffset, -strokeOffset);
				//DrawPen.EndCap = DrawPen.StartCap = LineCap.Round;

				//For debugging drawing rectangles
				switch(i){
					case 0:
						startAngle=180;
						break;
					case 1:
						startAngle=270;
						break;
					case 2:
						startAngle=0;
						break;
					case 3:
						startAngle=90;
						break;
				}

				double h=pt.y-pt_prev.y;
				double w=pt.x-pt_prev.x;

				//Check if we are drawing CCW
				double tg=h/w;
				if(w==0 && h>=0){
					startAngle = 90.0;
				}
				else if(w==0 && h<0){
					startAngle = 270.0;
				}
				else if(w<0 && h==0){
					startAngle = 180.0;
				}
				else{
					startAngle = atan(tg) * 180.0 / PI;
					//  | 
					//-----
					//  | 1
					if(w<0 && h<0){
						//Check CCW
						if(h_next>=0 && w_next<=0){
							//Turning CCW
							amplitude=-amplitude;
						}
						else if(h_next<0 &&  w_next<0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next<startAngle){
								//Turning CCW
								amplitude=-amplitude;
							}
							else{
								startAngle+=180.0;
							}
						}
						else if(h_next>0 &&  w_next>0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next>startAngle){
								//Turning CCW
								amplitude=-amplitude;
							}
							else{
								startAngle+=180.0;
							}
						}
						else{
							startAngle+=180.0;
						}
					}
					//  | 1
					//-----
					//  |
					else if(w<0 && h>0){
						//Check CCW
						if(h_next>=0 && w_next>=0){
							//Turning CCW
							amplitude=-amplitude;
						}
						else if(h_next>0 &&  w_next<0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next<startAngle){
								//Turning CCW
								amplitude=-amplitude;
							}
							else{
								startAngle+=180.0;
							}
						}
						else if(h_next<0 &&  w_next>0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next>startAngle){
								//Turning CCW
								amplitude=-amplitude;
							}
							else{
								startAngle+=180.0;
							}
						}
						else{
							startAngle+=180.0;
						}
					}
					//1 | 
					//-----
					//  |
					else if(w>0 && h>0){
						//Check CCW
						if(h_next<=0 && w_next>=0){
							//Turning CCW
							startAngle+=180.0;
							amplitude=-amplitude;
						}
						else if(h_next>0 &&  w_next>0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next<startAngle){
								//Turning CCW
								startAngle+=180.0;
								amplitude=-amplitude;
							}
						}
						else if(h_next<0 &&  w_next<0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next>startAngle){
								//Turning CCW
								startAngle+=180.0;
								amplitude=-amplitude;
							}
						}
					}
					//  | 
					//-----
					//1 |
					else if(w>0 && h<0){
						//Check CCW
						if(h_next<=0 && w_next<=0){
							//Turning CCW
							startAngle+=180.0;
							amplitude=-amplitude;
						}
						else if(h_next<0 &&  w_next>0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next<startAngle){
								//Turning CCW
								startAngle+=180.0;
								amplitude=-amplitude;
							}
						}
						else if(h_next>0 &&  w_next<0){
							double startAngle_next=atan(h_next/w_next) * 180.0 / PI;
							if(startAngle_next>startAngle){
								//Turning CCW
								startAngle+=180.0;
								amplitude=-amplitude;
							}
						}
					}
				}
				startAngle = startAngle - 90.0;

				//Add a corner
				gfxPath.AddArc(Bounds.TopLeft().x, Bounds.TopLeft().y, Bounds.Width(), Bounds.Height(), startAngle, amplitude);

			}//End for m_dwarrPointarr

			//Link all the arcs
			//x upper left
			//y upper right corner
			//width of rectangle
			//height of ractangle
			//start angle from x clock wise
			//angle amplitude from start angle
			//if(m_bClosed){
			if(m_dwarrPointarr.GetSize()>=2 && m_dwarrPointarr[0]==m_dwarrPointarr[m_dwarrPointarr.GetSize()-1]){
				gfxPath.CloseAllFigures();
			}

			//Draw the path
			Graphics grf(pDC->m_hDC);
			grf.DrawPath(&blackPen, &gfxPath);

			//Delete the buffer
			delete(points);

		}//End if m_dwarrPointarr.GetSize()

		CShape::OnDraw(pDC);
	}//End else if !m_mode 

}//End OnDraw
