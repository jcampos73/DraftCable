#pragma once
#include "DraftDrawDoc.h"
#include "MouseState.h"
class CAbstractTool : public CObject
{
public:
	CAbstractTool();
	DECLARE_SERIAL(CAbstractTool);
	~CAbstractTool();
	CDraftDrawDoc *pDoc;
	//CObList *m_pObList;
	CObList *m_pObListSel;

public:
	virtual CRect MoveTo(UINT nFlags, CPoint point);
	virtual CRect MouseDown(UINT nFlags, CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum EventType { ddcEventMouseDown, ddcEventMouseUp };
	CPoint m_ptMouseDownPrev;
	MouseState *m_mouseState;

	virtual CRect DeselectAll();
	virtual CShape* GetCurrentShape();
	virtual void _DoAddNewShapeToStack(UINT nFlags, CPoint point);
	virtual CShape* _DoCreateNewItem(CPoint point);
	virtual void __DoState(CPoint point, EventType eventType);
	virtual void MouseStateDown();
};