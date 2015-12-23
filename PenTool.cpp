#include "stdafx.h"

#include "PenTool.h"


CPenTool::CPenTool(LinkType linkType = ddcLinkTypeNone)
{
	m_LinkType = linkType;
	m_Status = ddcStatusNothingDrawing;
}


CPenTool::~CPenTool()
{
}

void CPenTool::MoveTo(CPoint point)
{

}

void CPenTool::MouseDown(CPoint point)
{
	__DoState(point, ddcEventMouseDown);

	m_ptMouseDownPrev = point;
}

void CPenTool::MouseUp(CPoint point)
{
	__DoState(point, ddcEventMouseUp);
}

void CPenTool::__DoState(CPoint point, EventType eventType)
{
	switch (m_Status){
		case ddcStatusNothingDrawing:
			DeselectAll();
			CShape *pSh;
			//If poly set status to...
			if (pSh->IsPoly()){
				m_Status = ddcStatusBeginPolyDrawing;
			}
			else
			{
				m_Status = ddcStatusPenDrawing;
			}
			break;
		case ddcStatusBeginPolyDrawing:
			__DoStateBeginPoly(point, eventType);
			break;
		case ddcStatusPenDrawing:
		case ddcStatusPolyDrawing:
			__DoStateDrawing(point, eventType);
			break;
	}


	if (eventType == ddcEventMouseDown){
		m_ptMouseDownPrev = point;
	}
}

void CPenTool::__DoStateBeginPoly(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseUp:
		if (m_ptMouseDownPrev != point){
			m_Status = ddcStatusPolyDrawing;
		}
		break;
	}
}

void CPenTool::__DoStateDrawing(CPoint point, EventType eventType)
{
	if (m_ptMouseDownPrev != point){
		switch (eventType)
		{
		case ddcStatusPenDrawing:
			//We are done
			break;
		case ddcStatusPolyDrawing:
			//Pass event to shape
			break;
		}
	}
}
