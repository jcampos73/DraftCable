#pragma once
class CAbstractTool
{
public:
	CAbstractTool();
	~CAbstractTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	CObList* m_pObListSel;
	virtual void DeselectAll();
};