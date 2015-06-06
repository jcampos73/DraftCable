#pragma once
class CAbstractTool
{
public:
	virtual void MoveTo(CPoint point) = 0;
	virtual void MouseDown(CPoint point) = 0;
	virtual void MouseUp(CPoint point) = 0;
};

