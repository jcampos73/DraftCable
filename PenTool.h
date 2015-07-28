#pragma once
class CPenTool
{
public:
	CPenTool();
	~CPenTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
};

