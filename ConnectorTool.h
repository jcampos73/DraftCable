#pragma once
#include "AbstractTool.h"
class CConnectorTool : public CAbstractTool
{
public:
	CConnectorTool();
	DECLARE_SERIAL(CConnectorTool);
	~CConnectorTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	enum Status { ddcStatusBeginConnect = 0, ddcStatusConnecting } m_Status;
	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginConnect(CPoint point, EventType eventType);
	void __DoStateConnecting(CPoint point, EventType eventType);
	BOOL __DoConnectShapes(CPoint point);
};