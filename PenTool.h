#pragma once
#include "AbstractTool.h"
class CPenTool : public CAbstractTool
{
public:
	enum LinkType { ddcLinkTypeNone = 0, ddcLinkTypeLink };
	CPenTool();
	DECLARE_SERIAL(CPenTool);
	CPenTool(LinkType linkType);
	~CPenTool();
public:
	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseDown(CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum Status { ddcStatusNothingDrawing = 0, ddcStatusBeginPolyDrawing, ddcStatusPolyDrawing, ddcStatusPenDrawing } m_Status;
	LinkType m_LinkType;

	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginPoly(CPoint point, EventType eventType);
	void __DoStateDrawing(CPoint point, EventType eventType);
	BOOL __DoAddToUpdateRect(CRect& rectUpdate, CRect rectToAdd);
};

