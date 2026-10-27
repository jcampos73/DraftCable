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
		if (pSh->m_Mode == _DRAFTDRAW_MODE_ACTIVE) {

			pShMove = pSh;
			int prev_mode = pSh->m_Mode;
			CRect prev_rect = pSh->m_Rect;
			pSh->m_Mode = _DRAFTDRAW_MODE_ACTIVE;

			pSh->OnMouseMove(0, point);

			//Recover saved rectangle
			pSh->m_Mode = prev_mode;
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

CRect CPenTool::MouseDown(UINT nFlags, CPoint point)
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
			DeselectAll();
			m_Status = ddcStatusPenDrawing;
			_DoAddNewShapeToStack(0, point);
			break;
		case ddcStatusPenDrawing:
			__DoStateDrawing(point, eventType);
			break;
	}

	if (eventType == ddcEventMouseUp){
		m_Status = ddcStatusNothingDrawing;
	}
}

void CPenTool::__DoStateDrawing(CPoint point, EventType eventType)
{
	CShape *pSh = GetCurrentShape();

	if (m_ptMouseDownPrev != point){
		//Keep former position rectangle to erase
		CRect rect2;
		rect2 = pSh->m_RectLast;
		rect2.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
		//Pass event to shape
		pSh->OnLButtonUp(0, point);
		//Current position to erase
		CRect rect;
		pSh->GetSizingRect(rect);
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
