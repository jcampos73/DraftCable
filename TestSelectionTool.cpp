#include "stdafx.h"
#include "TestSelectionTool.h"

#include "SelectionTool.h"

CTestSelectionTool::CTestSelectionTool()
{
}


CTestSelectionTool::~CTestSelectionTool()
{
}

void CTestSelectionTool::Event_WithValidPoint_MovesShape()
{
	CPoint point;
	CSelectionTool selectionTool;

	selectionTool.MouseDown(point);
}