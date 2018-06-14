#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

//#define DRAFTCABLE_MFC_CONTAINER_WRAP

IMPLEMENT_SERIAL(CSelectionTool,CAbstractTool, 1)

CSelectionTool::CSelectionTool()
{
	m_Status = ddcStatusNothingSelected;
}

CSelectionTool::~CSelectionTool()
{
}

CRect CSelectionTool::MoveTo(CPoint point)
{
	//If selection rectangle is being drawn...
	if (m_Status == ddcStatusCreatingSelectionRect)
	{
		m_rect = CRect(m_rect.TopLeft(), point);
		m_rect.NormalizeRect();
	}

	return m_rect;
}

CRect CSelectionTool::MouseDown(CPoint point)
{
	CRect RectMult(0, 0, 0, 0);
	int status = ddcStatusNothingSelected;
	//Select shape in point
	//We need a iterator to go though all the shapes in that region of the screen

#ifndef DRAFTCABLE_MFC_CONTAINER_WRAP

	//if (m_Status != ddcStatusCreatingSelectionRect)
	//{
	//	DeselectAll();
	//}
	//else
	//{
	//Check all he shapes to see if point is inside any
	//int index;
	//CShape *pSh = (CShape *)pDoc->LastObject(index);
	//pSh = (CShape *)pDoc->PrevObject(index);
	//while (status == ddcStatusNothingSelected && pSh != NULL){
	//	//Test if shape is selected

	//	if (pSh->PtInRect(point)){
	//		status = ddcStatusSomeSelected;
	//	}

	//	pSh = (CShape *)pDoc->PrevObject(index);
	//}//End while

	//if (status == ddcStatusNothingSelected)
	//{
	//	m_Status = ddcStatusNothingSelected;
	//	DeselectAll();
	//}
	//}

	//1.Iterate all shapes
	int index;
	CShape *pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);
	while (status == ddcStatusNothingSelected && pSh != NULL){
		//Test if shape is selected

		if (pSh->PtInRect(point))
		{
			status = ddcStatusSomeSelected;

			//pSh->OnLButtonDown(0, point);
			if (pSh->IsSelected()){

			}
			else
			{
				RectMult = DeselectAll();
				if (m_pObListSel->Find(pSh) == false){
					m_pObListSel->AddHead(pSh);
				}
				pSh->OnLButtonDown(0, point);

				RectMult.UnionRect(RectMult, pSh->m_Rect);
			}
		}

		pSh = (CShape *)pDoc->PrevObject(index);
	}//End while

#endif

#ifdef DRAFTCABLE_MFC_CONTAINER_WRAP
	//With for...
	//for (ListIter it = ListIter(m_pObList); it != ListIter(); ++it)
	ListIter it = ListIter(m_pObList);
	while (status == ddcStatusNothingSelected && it != ListIter())
	{
		//Test if shape is selected
		((CShape*)(*it))->OnLButtonDown(0, point);
		if (((CShape*)(*it))->IsSelected()){
			status = ddcStatusSomeSelected;
		}
		//Next shape
		++it;
	}
#endif	
	//If no shape selected start a selection rectangle
	if (status == ddcStatusNothingSelected)
	{
	//	status = ddcStatusCreatingSelectionRect;
	//	m_Status = ddcStatusCreatingSelectionRect;

		RectMult = DeselectAll();

	//	//-----------------------
	//	if (pDoc->m_pSh){
	//		if (pDoc->m_pSh != (CShape*)((*pDoc->m_pObArray)[pDoc->m_pObArray->GetSize() - 1])){
	//			delete(pDoc->m_pSh);
	//		}
	//	}
	//	pDoc->m_pSh = new(CShapeRect);
	//	pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;
	//	//-----------------------
	//	pDoc->m_iToolSel = _TOOLSELECTMUL_DRAFTCABLE;
	//	//m_RectMul = CRect(0, 0, 0, 0);
	//	CShapeRect *pShRect = (CShapeRect *)pDoc->m_pSh;
	//	pShRect->m_bGroup = TRUE;
	//	pShRect->m_bDrawDashedRect = TRUE;
	//	pDoc->AddObject(pDoc->m_pSh);
	//	pDoc->m_pSh->OnLButtonDown(0, point);
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
		CShape *pSh = (CShape *)pDoc->FirstObject(index);

		int sizeObArray = pDoc->m_pObArray->GetSize();

		pSh->OnLButtonDown(0, pDoc->m_pSh->m_Rect);
		//Add to selected array
		_AddToSelectedArray(pSh);

		//Iterate shapes
		while (index<sizeObArray - 1){
			pSh = (CShape *)pDoc->NextObject(index);
			pSh->OnLButtonDown(0, pDoc->m_pSh->m_Rect);
			//Add to selected array
			_AddToSelectedArray(pSh);
		}
		pSh = (CShape *)pDoc->NextObject(index);

		pSh->OnLButtonUp(0, point);
		pSh->OnLButtonDown(0, pSh->m_Rect);

		RectMult.UnionRect(RectMult, pSh->m_Rect);

		pDoc->DeleteObject(-1);
		pDoc->m_iToolSel = _TOOLSELECT_DRAFTCABLE;

		if (!RectMult.IsRectEmpty()){
			RectMult.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
		}

		m_Status = ddcStatusNothingSelected;
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

void CSelectionTool::_AddToSelectedArray(CShape *pSh)
{
	if (pSh->IsSelected()){
		POSITION pos = m_pObListSel->Find(pSh);
		if (pos == NULL)
			m_pObListSel->AddTail(pSh);
	}
}

