#pragma once
#include "AbstractTool.h"
class CPlaceTool : public CAbstractTool
{
public:
	CPlaceTool();
	~CPlaceTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	
};