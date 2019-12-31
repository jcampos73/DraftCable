#include "stdafx.h"

#include "PenTool.h"

IMPLEMENT_SERIAL(CPenTool, CAbstractTool, 1)

CPenTool::CPenTool()
{
	m_LinkType = ddcLinkTypeNone;
	m_Status = ddcStatusNothingDrawing;
}

CPenTool::CPenTool(LinkType linkType)
{
	m_LinkType = linkType;
	m_Status = ddcStatusNothingDrawing;
}


CPenTool::~CPenTool()
{
}

CRect CPenTool::MoveTo(UINT nFlags, CPoint point)
{
	int index;
	CShape *pShMove = NULL;
	CRect rect_union = CRect(0, 0, 0, 0);

	CShape *pSh = (CShape *)pDoc->FirstObject(index);
	pSh = (CShape *)pDoc->NextObject(index);

	while (pSh){
		POSITION pos = m_pObListSel->Find(pSh);
		if (pSh->IsSelected()
			|| (pSh->m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT && pDoc->m_iToolType == _TOOLTYPECHAIN_DRAFTCABLE)
			){

			pShMove = pSh;
			int prev_mode = pSh->m_Mode;
			CRect prev_rect = pSh->m_Rect;
			pSh->m_Mode = 0;

			if (pDoc->m_iToolSel != _TOOLROTATE_DRAFTCABLE){
				pSh->OnMouseMove(0, point);
				//Check if shape is beeing moved out of sheet
				//2014/04/10
				//This is not correct since if shape is placed initially out (or partially out) of design area will not work
				//-------------------------------------------
				//if (!pSh->m_Rect.IsRectEmpty()){
				//	CRect rect1;
				//	rect1.IntersectRect(rcSheet, pSh->m_Rect);
				//	if (rect1 != pSh->m_Rect){
				//		//pSh->m_Rect=prev_rect;
				//		//2014/04/10
				//		//As a solution to above comment, we apply following code
				//		DoPreventShpOutSchem(pSh, rcSheet);
				//	}
				//}
				//-------------------------------------------
				//Recover saved rectangle
				pSh->m_Mode = prev_mode;
			}
			else{
				pSh->OnRotate(0, point);
			}
			CRect rect;
			pSh->GetSizingRect(rect);

			rect.NormalizeRect();

			__DoAddToUpdateRect(rect_union, rect);
		}

		//Iterate next shape
		pSh = (CShape *)pDoc->NextObject(index);

	}//End while

	return rect_union;
}

CRect CPenTool::MouseDown(CPoint point)
{
	__DoState(point, ddcEventMouseDown);

	m_ptMouseDownPrev = point;

	return CRect(0, 0, 0, 0);
}

CRect CPenTool::MouseUp(CPoint point)
{
	__DoState(point, ddcEventMouseUp);

	return CRect(0, 0, 0, 0);
}

void CPenTool::__DoState(CPoint point, EventType eventType)
{
	switch (m_Status){
		case ddcStatusNothingDrawing:
			{
				DeselectAll();
				CShape *pSh = GetCurrentShape();
				//If poly set status to...
				if (pSh->IsPoly()){
					m_Status = ddcStatusBeginPolyDrawing;
				}
				else
				{
					m_Status = ddcStatusPenDrawing;
					_DoAddNewShapeToStack(0, point);
				}
			}
			break;
		case ddcStatusBeginPolyDrawing:
			__DoStateBeginPoly(point, eventType);
			break;
		case ddcStatusPenDrawing:
		case ddcStatusPolyDrawing:
			__DoStateDrawing(point, eventType);
			break;
	}


	if (eventType == ddcEventMouseDown){
		m_ptMouseDownPrev = point;
	}
	else if (eventType == ddcEventMouseUp){
		m_Status = ddcStatusNothingDrawing;
	}
}

void CPenTool::__DoStateBeginPoly(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseUp:
		if (m_ptMouseDownPrev != point){
			m_Status = ddcStatusPolyDrawing;
		}
		break;
	}
}

void CPenTool::__DoStateDrawing(CPoint point, EventType eventType)
{
	CShape *pSh = NULL;
	if (m_ptMouseDownPrev != point){
		switch (m_Status)
		{
		case ddcStatusPenDrawing:
			////We are done
			//pSh = __DoCreateNewItem(point);
			//pSh->OnLButtonDown(0, point);
			break;
		case ddcStatusPolyDrawing:
			//Pass event to shape
			pSh = GetCurrentShape();
			pSh->OnLButtonDown(0, point);
			break;
		}
	}
}

BOOL CPenTool::__DoAddToUpdateRect(CRect& rectUpdate, CRect rectToAdd)
{
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
