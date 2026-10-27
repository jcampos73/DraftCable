#include "stdafx.h"
#include "PolyTool.h"

IMPLEMENT_SERIAL(CPolyTool, CPenTool, 1)

CPolyTool::CPolyTool()
	: m_PolyStatus(ddcStatusBeginPolyDrawing), m_pPoly(NULL),
	  m_nMouseFlags(0), m_rectUpdate(0, 0, 0, 0)
{
	pDoc = NULL;
	m_pObListSel = NULL;
}

CPolyTool::~CPolyTool()
{
	Finish();
}

CRect CPolyTool::MouseDown(UINT nFlags, CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	m_nMouseFlags = nFlags;
	if (pDoc == NULL || pDoc->m_pObArray == NULL)
		return m_rectUpdate;

	if (m_pPoly != NULL && GetPoly() == NULL)
	{
		m_pPoly = NULL;
		m_PolyStatus = ddcStatusBeginPolyDrawing;
	}
	// Reuse PenTool's dispatch and previous-press bookkeeping. The virtual
	// __DoState selects our poly cycle instead of PenTool's normal cycle.
	CPenTool::MouseDown(nFlags, point);
	return m_rectUpdate;
}

CRect CPolyTool::MouseUp(CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	m_nMouseFlags = 0;
	if (GetPoly() != NULL)
		CPenTool::MouseUp(point);
	return m_rectUpdate;
}

CRect CPolyTool::MoveTo(UINT nFlags, CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	CShapePolyline* poly = GetPoly();
	if (poly == NULL || poly->m_Mode != _DRAFTDRAW_MODE_ACTIVE)
		return m_rectUpdate;

	// Trace the current segment even when the mouse button is released.
	__DoAddPolyUpdateRect();
	poly->OnMouseMove(nFlags, point);
	__DoAddPolyUpdateRect();
	return m_rectUpdate;
}

void CPolyTool::__DoState(CPoint point, EventType eventType)
{
	switch (m_PolyStatus)
	{
	case ddcStatusBeginPolyDrawing:
		__DoStateBeginPoly(point, eventType);
		break;
	case ddcStatusPolyDrawing:
		__DoStatePolyDrawing(point, eventType);
		break;
	}
}

void CPolyTool::__DoStateBeginPoly(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseDown:
		if (m_pPoly == NULL)
		{
			CShape* prepared = pDoc->m_pSh;
			if (prepared == NULL ||
				!prepared->IsPoly() ||
				prepared->m_Mode != _DRAFTDRAW_MODE_ACTIVE ||
				prepared->m_TypeSelect != _DRAFTDRAW_SEL_SIZING_RECT)
				return;

			if (m_pObListSel != NULL)
				m_rectUpdate = DeselectAll();
			m_pPoly = (CShapePolyline*)prepared;
			if (GetPoly() == NULL)
				pDoc->AddObject(m_pPoly);
		}
		__DoAddPoint(point);
		break;
	case ddcEventMouseUp:
		__DoReleasePoint(point);
		m_PolyStatus = ddcStatusPolyDrawing;
		break;
	}
}

void CPolyTool::__DoStatePolyDrawing(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseDown:
		__DoAddPoint(point);
		break;
	case ddcEventMouseUp:
		__DoReleasePoint(point);
		break;
	}
}

void CPolyTool::__DoAddPoint(CPoint point)
{
	// A fresh shape has no initialized bounds until its first press.
	if (m_pPoly->m_dwarrPointarr.GetSize() > 0)
		__DoAddPolyUpdateRect();
	m_pPoly->OnLButtonDown(m_nMouseFlags, point);
	__DoAddPolyUpdateRect();
}

void CPolyTool::__DoReleasePoint(CPoint point)
{
	__DoAddPolyUpdateRect();
	// CShapePolyline preserves ACTIVE + SIZING across release. It also owns
	// the point bookkeeping used by curves, polyarcs and buses.
	m_pPoly->OnLButtonUp(m_nMouseFlags, point);
	__DoAddPolyUpdateRect();
}

CRect CPolyTool::MouseDblClk(UINT nFlags, CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	CShapePolyline* poly = GetPoly();
	if (poly == NULL)
		return m_rectUpdate;

	__DoAddPolyUpdateRect();
	// Preserve the legacy sequence: double click, Escape normalization, Idle.
	poly->OnLButtonDblClk(nFlags, point);
	__DoFinishPoly();
	return m_rectUpdate;
}

CRect CPolyTool::Finish()
{
	m_rectUpdate.SetRectEmpty();
	CShapePolyline* poly = GetPoly();
	// ResetAllDrwOpe may already have finalized the shape before deleting
	// this tool. Its now-relative points must not be normalized twice.
	if (poly != NULL && poly->m_Mode == _DRAFTDRAW_MODE_ACTIVE &&
		poly->m_TypeSelect == _DRAFTDRAW_SEL_SIZING_RECT)
		__DoFinishPoly();
	else
	{
		m_pPoly = NULL;
		m_PolyStatus = ddcStatusBeginPolyDrawing;
	}
	return m_rectUpdate;
}

void CPolyTool::__DoFinishPoly()
{
	__DoAddPolyUpdateRect();
	m_pPoly->OnKeyDown(27, 0, 0);
	m_pPoly->m_Mode = _DRAFTDRAW_MODE_IDLE;
	__DoAddPolyUpdateRect();
	m_pPoly = NULL;
	m_PolyStatus = ddcStatusBeginPolyDrawing;
}

CShapePolyline* CPolyTool::GetPoly() const
{
	if (m_pPoly == NULL || pDoc == NULL || pDoc->m_pObArray == NULL)
		return NULL;
	for (int i = 0; i < pDoc->m_pObArray->GetSize(); ++i)
		if ((*pDoc->m_pObArray)[i] == m_pPoly)
			return m_pPoly;
	return NULL;
}

void CPolyTool::__DoAddPolyUpdateRect()
{
	CRect rect;
	m_pPoly->GetSizingRect(rect);
	__DoAddToUpdateRect(m_rectUpdate, rect);
	// Include the preceding fixed segment, as the legacy release path does.
	m_pPoly->GetRectUpdatePlace(rect);
	__DoAddToUpdateRect(m_rectUpdate, rect);
}
