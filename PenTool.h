#pragma once
#include "AbstractTool.h"
class CPenTool : public CAbstractTool
{
public:
	enum LinkType { ddcLinkTypeNone = 0, ddcLinkTypeLink };
	CPenTool(LinkType linkType);
	~CPenTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	enum Status { ddcStatusNothingDrawing = 0, ddcStatusBeginPolyDrawing, ddcStatusPolyDrawing, ddcStatusPenDrawing } m_Status;
	LinkType m_LinkType;

	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginPoly(CPoint point, EventType eventType);
	void __DoStateDrawing(CPoint point, EventType eventType);
};

