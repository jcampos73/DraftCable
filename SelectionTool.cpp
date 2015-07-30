#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

//#define DRAFTCABLE_MFC_CONTAINER_WRAP

CSelectionTool::CSelectionTool()
{
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

	POSITION pos = m_pObList->GetHeadPosition();
	while (status == ddcStatusNothingSelected && pos != NULL){
		//Test if shape is selected
		CShape* pSh = (CShape*)m_pObList->GetNext(pos);
		pSh->OnLButtonDown(0, point);
		if (pSh->IsSelected()){
			status = ddcStatusSomeSelected;
			if (m_pObListSel->Find(pSh)){
				status = ddcStatusSomeAlreadySelected;
			}
			else{
				//Deselect all
				DeselectAll();
				m_pObListSel->AddHead(pSh);
			}
		}
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
	int status = ddcStatusNothingSelected;
	if (m_Status == ddcStatusCreatingSelectionRect)
	{
		POSITION pos = m_pObList->GetHeadPosition();

		while (pos != NULL){
			//Test if shape is selected
			CShape* pSh = (CShape*)m_pObList->GetNext(pos);
			pSh->OnLButtonDown(0, m_rect);
			if (pSh->IsSelected()){
				status = ddcStatusSomeSelected;
				m_pObListSel->AddHead(pSh);
			}
		}

		m_Status = static_cast<Status>(status);
	}
}
