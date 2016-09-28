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
	virtual void MoveTo(CPoint point);
	virtual void MouseDown(CPoint point);
	virtual void MouseUp(CPoint point);
protected:
	enum EventType { ddcEventMouseDown, ddcEventMouseUp };
	CPoint m_ptMouseDownPrev;

	virtual void DeselectAll();
	virtual CShape* GetCurrentShape();
	virtual CShape* __DoCreateNewItem(CPoint point);
	virtual void __DoState(CPoint point, EventType eventType);
};