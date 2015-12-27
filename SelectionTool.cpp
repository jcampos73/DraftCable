#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

//#define DRAFTCABLE_MFC_CONTAINER_WRAP

CSelectionTool::CSelectionTool()
{
	m_Status = ddcStatusNothingSelected;
}

CSelectionTool::~CSelectionTool()
{
}

void CSelectionTool::MoveTo(CPoint point)
{
	//If selection rectangle is being drawn...
	if (m_Status == ddcStatusCreatingSelectionRect)
	{
		m_rect = CRect(m_rect.TopLeft(), point);
		m_rect.NormalizeRect();
	}
}

void CSelectionTool::MouseDown(CPoint point)
{
	int status = ddcStatusNothingSelected;
	//Select shape in point
	//We need a iterator to go though all the shapes in that region of the screen

#ifndef DRAFTCABLE_MFC_CONTAINER_WRAP
	//1.Iterate all shapes
	int index;
	CShape *pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);
	while (status == ddcStatusNothingSelected && pSh != NULL){
		//Test if shape is selected

		pSh->OnLButtonDown(0, point);
		if (pSh->IsSelected()){
			status = ddcStatusSomeSelected;
			if (m_pObListSel->Find(pSh)==false){
				//Deselect all
				DeselectAll();
				m_pObListSel->AddHead(pSh);
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
		status = ddcStatusCreatingSelectionRect;
	}
}

void CSelectionTool::MouseUp(CPoint point)
{
	int index;
	int status = ddcStatusNothingSelected;

	if (m_Status == ddcStatusCreatingSelectionRect)
	{
		CShape *pSh = (CShape *)pDoc->LastObject(index);
		pSh = (CShape *)pDoc->PrevObject(index);

		while (pSh != NULL){
			//Test if shape is selected
			pSh->OnLButtonDown(0, m_rect);
			if (pSh->IsSelected()){
				status = ddcStatusSomeSelected;
				m_pObListSel->AddHead(pSh);
			}

			pSh = (CShape *)pDoc->PrevObject(index);
		}

		m_Status = static_cast<Status>(status);
	}
}

