#pragma once
#include "AbstractTool.h"
class CSelectionTool : public CAbstractTool
{
public:
	CSelectionTool();
	DECLARE_SERIAL(CSelectionTool);
	~CSelectionTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	enum Status { ddcStatusNothingSelected = 0, ddcStatusSomeSelected, ddcStatusCreatingSelectionRect } m_Status;
	CRect m_rect;
};

