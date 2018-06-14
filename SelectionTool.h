#pragma once
#include "AbstractTool.h"
class CSelectionTool : public CAbstractTool
{
public:
	CSelectionTool();
	DECLARE_SERIAL(CSelectionTool);
	~CSelectionTool();
public:
	virtual CRect MoveTo(CPoint point);
	virtual CRect MouseDown(CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum Status { ddcStatusNothingSelected = 0, ddcStatusSomeSelected, ddcStatusCreatingSelectionRect } m_Status;
	CRect m_rect;

	void _AddToSelectedArray(CShape *pSh);
};

