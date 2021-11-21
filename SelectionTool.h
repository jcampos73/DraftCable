#pragma once
#include "AbstractTool.h"
class CSelectionTool : public CAbstractTool
{
public:
	CSelectionTool();
	DECLARE_SERIAL(CSelectionTool);
	~CSelectionTool();
public:
	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseDown(UINT nFlags, CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum Status { ddcStatusNothingSelected = 0, ddcStatusDragging, ddcStatusCreatingSelectionRect } m_Status;
	CRect m_rect;

	virtual void __DoState(CPoint point, EventType eventType);
	void _AddToSelectedArray(CShape *pSh);
	CRect _DoCreateSelectionRectangle(int status, CPoint point, CRect RectMult);
	void _RemoveFromSelectedArray(CShape *pSh);
	void _CreateSelectionRect(CPoint point);
	BOOL _DoAddToUpdateRect(CRect& rectUpdate, CRect rectToAdd);

private:
	void __DeleteSafeBufferShape();
};

