#pragma once
#include "DraftDrawDoc.h"
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
	virtual CRect MouseDown(CPoint point);
	virtual CRect MouseUp(CPoint point);
protected:
	enum EventType { ddcEventMouseDown, ddcEventMouseUp };
	CPoint m_ptMouseDownPrev;

	virtual CRect DeselectAll();
	virtual CShape* GetCurrentShape();
	virtual void _DoAddNewShapeToStack(UINT nFlags, CPoint point);
	virtual CShape* __DoCreateNewItem(CPoint point);
	virtual void __DoState(CPoint point, EventType eventType);
};