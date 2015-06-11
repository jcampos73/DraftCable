#include "stdafx.h"
#include "SelectionTool.h"
#include "ShArrayIterator.h"

CSelectionTool::CSelectionTool()
{
	//Container lst(*m_pObList);
	//ListIter m_pIter(m_pObList);


}


CSelectionTool::~CSelectionTool()
{
}

void CSelectionTool::MoveTo(CPoint point)
{

}

void CSelectionTool::MouseDown(CPoint point)
{
	//Select shape in point
	//We need a iterator to go though all the shapes in that region of the screen

	//1.Iterate all shapes
	for (ListIter it = ListIter(m_pObList); it != ListIter(); ++it)
	{
		
	}


	//If no shape selected start a selection rectangle
	
}

void CSelectionTool::MouseUp(CPoint point)
{

}
