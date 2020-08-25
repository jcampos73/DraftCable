#pragma once
#include "AbstractTool.h"
class CConnectorTool : public CAbstractTool
{
public:
	CConnectorTool();
	DECLARE_SERIAL(CConnectorTool);
	~CConnectorTool();
public:
	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseDown(UINT nFlags, CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum Status { ddcStatusBeginConnect = 0, ddcStatusConnecting } m_Status;
	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginConnect(CPoint point, EventType eventType);
	void __DoStateConnecting(CPoint point, EventType eventType);
	BOOL __DoConnectShapes(CPoint point);
};