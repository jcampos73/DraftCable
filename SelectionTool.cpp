#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

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
		//Just call external method to inflate rectangle
	}
}

void CSelectionTool::MouseDown(CPoint point)
{
	int status = ddcStatusNothingSelected;
	//Select shape in point
	//We need a iterator to go though all the shapes in that region of the screen

	//1.Iterate all shapes
	//for (ListIter it = ListIter(m_pObList); it != ListIter(); ++it)
	ListIter it = ListIter(m_pObList);
	do
	{
		//Test if shape is selected
		//CShape *pSh = (CShape *)it;
		//it->OnLButtonDown(0, point);
		//Next shape
		++it;
	}
	while(it != ListIter());

	//If no shape selected start a selection rectangle
	if (status == ddcStatusNothingSelected)
	{
		status = ddcStatusCreatingSelectionRect;
	}
}

void CSelectionTool::MouseUp(CPoint point)
{
	if (m_Status == ddcStatusCreatingSelectionRect)
	{

	}
}
