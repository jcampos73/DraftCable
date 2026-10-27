#pragma once
#include "AbstractTool.h"

class CConnectorTool : public CAbstractTool
{
public:
	CConnectorTool();
	DECLARE_SERIAL(CConnectorTool);
	virtual ~CConnectorTool();

	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseDown(UINT nFlags, CPoint point);
	virtual CRect MouseUp(CPoint point);

	// Cancel only the provisional segment, keeping completed segments.
	// The document and active sheet must remain valid until Cancel returns.
	CRect Cancel();
	BOOL IsDrawing() const;

protected:
	enum Status { ddcStatusBeginConnect = 0, ddcStatusConnecting } m_Status;
	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginConnect(CPoint point, EventType eventType);
	void __DoStateConnecting(CPoint point, EventType eventType);
	BOOL __DoConnectShapes(CPoint point,
		int indexPin = SHAPECONTAINER_CONNECTING_PIN_UNIT);

private:
	// The document owns the preview. Its identity does not depend on stack order.
	CShapeWire* m_pWire;
	CPoint m_ptSegmentStart;
	BOOL m_bMouseDown;
	CRect m_rectUpdate;

	int __DoFindShapeIndex(CShape* shape) const;
	void __DoBeginSegment(CPoint point, CShapeContainer* sourcePin = NULL);
	void __DoMoveTo(UINT nFlags, CPoint point);
	void __DoLinkPin(int indexPin, CShapeContainer* targetPin);
	void __DoUpdatePinRects();
	void __DoAddToUpdateRect(CShape* shape);

	CConnectorTool(const CConnectorTool&);
	CConnectorTool& operator=(const CConnectorTool&);
};
