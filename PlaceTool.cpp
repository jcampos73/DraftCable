#include "stdafx.h"
#include "PlaceTool.h"


CPlaceTool::CPlaceTool()
{
}


CPlaceTool::~CPlaceTool()
{
}

void CPlaceTool::MoveTo(CPoint point)
{

}

void CPlaceTool::MouseDown(CPoint point)
{

}

void CPlaceTool::MouseUp(CPoint point)
{
	CShape *pSh = __DoCreateNewItem();

	pSh->OnLButtonDown(0, point);
}

