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
private:
	enum Status { ddcStatusDrawingNone = 0, ddcStatusPolyDrawingIni, ddcStatusPolyDrawing } m_Status;
	LinkType m_LinkType;
};

