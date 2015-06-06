#pragma once
#include "AbtsractTool.h"
class CSelectionTool : public CAbstractTool
{
public:
	CSelectionTool();
	~CSelectionTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
};

