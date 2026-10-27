#pragma once
#include "PenTool.h"

class CPolyTool : public CPenTool
{
public:
	CPolyTool();
	DECLARE_SERIAL(CPolyTool);
	virtual ~CPolyTool();

	virtual CRect MouseDown(UINT nFlags, CPoint point);
	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseUp(CPoint point);
	CRect MouseDblClk(UINT nFlags, CPoint point);

	// Finish through the shape's existing Escape handler; keep its points.
	// Call before changing sheets or destroying the document.
	CRect Finish();
	CShapePolyline* GetPoly() const;

protected:
	enum PolyStatus { ddcStatusBeginPolyDrawing = 0, ddcStatusPolyDrawing } m_PolyStatus;
	virtual void __DoState(CPoint point, EventType eventType);
	void __DoStateBeginPoly(CPoint point, EventType eventType);
	void __DoStatePolyDrawing(CPoint point, EventType eventType);

private:
	// The document owns the shape. Keep its identity, not its stack position.
	CShapePolyline* m_pPoly;
	UINT m_nMouseFlags;
	CRect m_rectUpdate;

	void __DoAddPoint(CPoint point);
	void __DoReleasePoint(CPoint point);
	void __DoAddPolyUpdateRect();
	void __DoFinishPoly();

	CPolyTool(const CPolyTool&);
	CPolyTool& operator=(const CPolyTool&);
};
