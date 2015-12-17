#pragma once
#include "AbstractTool.h"
class CSelectionTool : public CAbstractTool
{
public:
	CSelectionTool();
	~CSelectionTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
private:
	enum Status { ddcStatusNothingSelected = 0, ddcStatusSomeSelected, ddcStatusCreatingSelectionRect } m_Status;
	CRect m_rect;
};

