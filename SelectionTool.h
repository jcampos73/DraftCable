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
private:
	enum{ ddcStatusNothingSelected = 0, ddcStatusSomeSelected, ddcStatusCreatingSelectionRect } m_Status;
	CObList* m_pObList;
};

