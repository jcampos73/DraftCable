#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

IMPLEMENT_SERIAL(CSelectionTool,CAbstractTool, 1)

CSelectionTool::CSelectionTool()
{
	m_Status = ddcStatusNothingSelected;
}

CSelectionTool::~CSelectionTool()
{
}

CRect CSelectionTool::MoveTo(UINT nFlags, CPoint point)
{
	CRect RectMult(0, 0, 0, 0);

	if (nFlags & MK_LBUTTON)
	{
		//If selection rectangle is being drawn...
		if (m_Status == ddcStatusCreatingSelectionRect)
		{
			m_rect = CRect(m_rect.TopLeft(), point);
			m_rect.NormalizeRect();
		}

		POSITION pos = m_pObListSel->GetHeadPosition();
		while (pos != NULL) {

			CShape* pSh = (CShape*)m_pObListSel->GetNext(pos);

			int prev_mode = pSh->m_Mode;
			CRect prev_rect = pSh->m_Rect;
			pSh->m_Mode = 0;

			pSh->OnMouseMove(0, point);

			CRect rect;
			pSh->GetSizingRect(rect);

			rect.NormalizeRect();

			RectMult.UnionRect(RectMult, rect);

			_DoAddToUpdateRect(RectMult, rect);
		}
	}

	return RectMult;
}

CRect CSelectionTool::MouseDown(UINT nFlags, CPoint point)
{
	CRect RectMult(0, 0, 0, 0);
	int status = ddcStatusNothingSelected;

	__DoState(point, ddcEventMouseDown);

	m_ptMouseDownPrev = point;

	//Iterate all shapes
	int index;
	CShape *pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);
	while (status == ddcStatusNothingSelected && pSh != NULL){

		if (pSh->PtInRect(point))
		{
			status = ddcStatusDragging;

			//Select shape
			if (!pSh->IsSelected()){

				if (!(nFlags & MK_CONTROL))
				{
					RectMult = DeselectAll();
				}
				if (m_pObListSel->Find(pSh) == false){
					m_pObListSel->AddHead(pSh);
				}
				pSh->OnLButtonDown(0, point);

				RectMult.UnionRect(RectMult, pSh->m_Rect);
			}
			else if(nFlags & MK_CONTROL)
			{
				//Deselect
				pSh->Unselect();
				POSITION pos = m_pObListSel->Find(pSh);
				if (pos)
				{
					m_pObListSel->RemoveAt(pos);
				}
			}
		}

		pSh = (CShape *)pDoc->PrevObject(index);
	}//End while

	RectMult = _DoCreateSelectionRectangle(status, point, RectMult);

	return RectMult;
}

CRect CSelectionTool::_DoCreateSelectionRectangle(int status, CPoint point, CRect RectMult)
{
	//If no shape selected start a selection rectangle
	if (status == ddcStatusNothingSelected)
	{
		m_Status = ddcStatusCreatingSelectionRect;

		m_rect = CRect(point, point);

		RectMult = DeselectAll();

		//Create selection rectangle
		//-----------------------
		__DeleteSafeBufferShape();

		//Create selection rectangle
		_CreateSelectionRect(point);
		//-----------------------

		pDoc->m_iToolSel = _TOOLSELECTMUL_DRAFTCABLE;

	}
	else
	{
		m_Status = ddcStatusDragging;
	}

	return RectMult;
}

CRect CSelectionTool::MouseUp(CPoint point)
{
	int index;
	CRect RectMult(0, 0, 0, 0);
	int status = ddcStatusNothingSelected;

	if (m_Status == ddcStatusCreatingSelectionRect)
	{
		if (m_rect.IsRectEmpty() == false)
		{
			CShape *pSh = (CShape *)pDoc->FirstObject(index);

			int sizeObArray = pDoc->m_pObArray->GetSize();

			pSh->OnLButtonDown(0, pDoc->m_pSh->m_Rect);
			//Add to selected array
			_AddToSelectedArray(pSh);

			//Iterate shapes
			while (index < sizeObArray - 1){
				pSh = (CShape *)pDoc->NextObject(index);
				pSh->OnLButtonDown(0, pDoc->m_pSh->m_Rect);
				//Add to selected array
				_AddToSelectedArray(pSh);
			}
			pSh = (CShape *)pDoc->NextObject(index);

			pSh->OnLButtonUp(0, point);
			pSh->OnLButtonDown(0, pSh->m_Rect);

			if (pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
				int debug_here = 1;
			}

			RectMult.UnionRect(RectMult, pSh->m_Rect);
			_RemoveFromSelectedArray(pSh);

			pDoc->DeleteObject(-1);
			pDoc->m_iToolSel = _TOOLSELECT_DRAFTCABLE;

			if (!RectMult.IsRectEmpty()){
				RectMult.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
			}

			m_Status = ddcStatusNothingSelected;
		}
		else
		{
			CShape *pSh = (CShape *)pDoc->FirstObject(index);

			if (pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
				_RemoveFromSelectedArray(pSh);

				pDoc->DeleteObject(-1);

				m_Status = ddcStatusNothingSelected;
			}
		}
	}
	else
	{
		POSITION pos = m_pObListSel->GetHeadPosition();
		while (pos != NULL){
			//Deselect shape
			CShape* pSh = (CShape*)m_pObListSel->GetNext(pos);

			RectMult.UnionRect(RectMult, pSh->m_Rect);

			CRect rect2 = pSh->m_RectLast;

			rect2.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);

			RectMult.UnionRect(RectMult, rect2);

			pSh->OnLButtonUp(0, point);
		}
	}

	return RectMult;
}

void CSelectionTool::__DoState(CPoint point, EventType eventType)
{
	switch (m_Status) {
		case ddcStatusDragging:
		break;
	}
}

void CSelectionTool::_AddToSelectedArray(CShape *pSh)
{
	if (pSh->IsSelected()){
		POSITION pos = m_pObListSel->Find(pSh);
		if (pos == NULL)
		{
			m_pObListSel->AddTail(pSh);
		}
	}
}

void CSelectionTool::_RemoveFromSelectedArray(CShape *pSh)
{
	POSITION pos = m_pObListSel->Find(pSh);
	if (pos != NULL)
	{
		m_pObListSel->RemoveAt(pos);
	}
}

void CSelectionTool::__DeleteSafeBufferShape()
{
	if (pDoc->m_pSh){
		if (pDoc->m_pSh != (CShape*)((*pDoc->m_pObArray)[pDoc->m_pObArray->GetSize() - 1])){
			delete(pDoc->m_pSh);
		}
	}
}

void CSelectionTool::_CreateSelectionRect(CPoint point)
{
	//Create selection rectangle
	pDoc->m_pSh = new(CShapeRect);
	pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;
	CShapeRect *pShRect = (CShapeRect *)pDoc->m_pSh;
	pShRect->m_bGroup = TRUE;
	pShRect->m_bDrawDashedRect = TRUE;
	pDoc->AddObject(pDoc->m_pSh);
	pDoc->m_pSh->OnLButtonDown(0, point);
	_AddToSelectedArray(pDoc->m_pSh);
}

BOOL CSelectionTool::_DoAddToUpdateRect(CRect& rectUpdate, CRect rectToAdd)
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

