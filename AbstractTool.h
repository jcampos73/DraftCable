#pragma once
#include "DraftDrawDoc.h"
class CAbstractTool
{
public:
	CAbstractTool();
	~CAbstractTool();
public:
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	enum EventType { ddcEventMouseDown, ddcEventMouseUp };
	CPoint m_ptMouseDownPrev;
	CDraftDrawDoc *pDoc;
	CObList *m_pObList;
	CObList *m_pObListSel;

	virtual void DeselectAll();
	virtual CShape* __DoCreateNewItem();
	virtual void __DoState(CPoint point, EventType eventType);
};