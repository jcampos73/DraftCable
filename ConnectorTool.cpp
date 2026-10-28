#include "stdafx.h"
#include "ConnectorTool.h"

IMPLEMENT_SERIAL(CConnectorTool, CAbstractTool, 1)

CConnectorTool::CConnectorTool()
	: m_pWire(NULL), m_ptSegmentStart(0, 0), m_bMouseDown(FALSE),
	  m_rectUpdate(0, 0, 0, 0)
{
	m_Status = ddcStatusBeginConnect;
	pDoc = NULL;
	m_pObListSel = NULL;
}

CConnectorTool::~CConnectorTool()
{
	Cancel();
}

CRect CConnectorTool::MoveTo(UINT nFlags, CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	if (IsDrawing())
		__DoMoveTo(nFlags, point);
	return m_rectUpdate;
}

CRect CConnectorTool::MouseDown(UINT /*nFlags*/, CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	if (pDoc == NULL || pDoc->m_pObArray == NULL)
		return m_rectUpdate;

	// An externally deleted preview must not leave the tool in Connecting.
	if (m_Status == ddcStatusConnecting && !IsDrawing())
	{
		m_pWire = NULL;
		m_Status = ddcStatusBeginConnect;
	}
	m_bMouseDown = TRUE;
	__DoState(point, ddcEventMouseDown);
	return m_rectUpdate;
}

CRect CConnectorTool::MouseUp(CPoint point)
{
	m_rectUpdate.SetRectEmpty();
	if (!m_bMouseDown || pDoc == NULL || pDoc->m_pObArray == NULL)
		return m_rectUpdate;

	m_bMouseDown = FALSE;
	__DoState(point, ddcEventMouseUp);
	return m_rectUpdate;
}

void CConnectorTool::__DoState(CPoint point, EventType eventType)
{
	switch (m_Status)
	{
	case ddcStatusBeginConnect:
		__DoStateBeginConnect(point, eventType);
		break;
	case ddcStatusConnecting:
		__DoStateConnecting(point, eventType);
		break;
	}

	if (eventType == ddcEventMouseDown)
		m_ptMouseDownPrev = point;
}

void CConnectorTool::__DoStateBeginConnect(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseDown:
		if (m_pObListSel != NULL)
			m_rectUpdate = DeselectAll();
		__DoBeginSegment(point);
		__DoConnectShapes(point, SHAPECONTAINER_CONNECTING_PIN_UNIT);
		m_Status = ddcStatusConnecting;
		break;
	}
}

void CConnectorTool::__DoStateConnecting(CPoint point, EventType eventType)
{
	// Match the active view's placement cycle: press keeps tracing, release
	// confirms a segment. Do not run continuation on both events.
	if (eventType != ddcEventMouseUp || !IsDrawing())
		return;

	__DoMoveTo(0, point);
	// Starting with a press/release at the same point must not create a
	// zero-length segment or connect its end back to the starting pin.
	if (point == m_ptSegmentStart)
		return;

	BOOL bWireEnd = __DoConnectShapes(point, SHAPECONTAINER_CONNECTING_PIN_WIRE);
	CShapeWire* pSh = m_pWire;
	pSh->OnLButtonUp(0, point);
	pSh->m_RectLast = pSh->m_Rect;
	__DoAddToUpdateRect(pSh);
	pDoc->SetModifiedFlag(TRUE);
	m_pWire = NULL;

	if (bWireEnd)
	{
		m_Status = ddcStatusBeginConnect;
	}
	else
	{
		// Continue from the actual previous segment, even if another object
		// has since been added to the document.
		CShapeContainer* previousEnd = (CShapeContainer*)
			(*pSh)[SHAPECONTAINER_CONNECTING_PIN_WIRE];
		__DoBeginSegment(point, previousEnd);
	}
}

BOOL CConnectorTool::__DoConnectShapes(CPoint point, int indexPin)
{
	if (m_pWire == NULL)
		return FALSE;

	int index;
	pDoc->LastObject(index);
	CShape* pSh = (CShape*)pDoc->PrevObject(index);
	while (pSh != NULL)
	{
		// Skip the provisional wire; do not stop the entire search at it.
		if (pSh != m_pWire && pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer)))
		{
			CShapeContainer* pShContConnect = NULL;
			CShapeContainer* pShContainer = (CShapeContainer*)pSh;
			if (pShContainer->PtInRect(&point, &pShContConnect) &&
				pShContConnect != NULL)
			{
				__DoAddToUpdateRect(pShContConnect->GetParent());
				__DoLinkPin(indexPin, pShContConnect);
				return TRUE;
			}
		}
		pSh = (CShape*)pDoc->PrevObject(index);
	}
	return FALSE;
}

void CConnectorTool::__DoBeginSegment(CPoint point, CShapeContainer* sourcePin)
{
	// ID_PLACE_WIRE already prepares a wire outside the document. Reuse it
	// on the first press instead of allocating a second provisional object.
	CShape* prepared = pDoc->m_pSh;
	if (m_Status == ddcStatusBeginConnect && prepared != NULL &&
		prepared->IsKindOf(RUNTIME_CLASS(CShapeWire)) &&
		prepared->m_Mode == _DRAFTDRAW_MODE_ACTIVE &&
		prepared->m_TypeSelect == _DRAFTDRAW_SEL_SIZING_RECT &&
		__DoFindShapeIndex(prepared) < 0)
		m_pWire = (CShapeWire*)prepared;
	else
		m_pWire = new CShapeWire(NULL, 0, pDoc->cmdDeque);

	pDoc->m_pSh = m_pWire;
	m_pWire->m_pCursorArray = pDoc->m_CursorArray;
	m_ptSegmentStart = point;
	m_pWire->OnLButtonDown(0, point);
	m_pWire->m_RectLast = m_pWire->m_Rect;
	// Register once in the active sheet, retaining an explicit preview pointer.
	pDoc->AddObject(m_pWire);
	__DoUpdatePinRects();
	if (sourcePin != NULL)
		__DoLinkPin(SHAPECONTAINER_CONNECTING_PIN_UNIT, sourcePin);
	__DoAddToUpdateRect(m_pWire);
}

void CConnectorTool::__DoMoveTo(UINT nFlags, CPoint point)
{
	// The provisional segment follows the cursor with or without a button held.
	__DoAddToUpdateRect(m_pWire);
	m_pWire->OnMouseMove(nFlags, point);
	__DoUpdatePinRects();
	__DoAddToUpdateRect(m_pWire);
}

void CConnectorTool::__DoLinkPin(int indexPin, CShapeContainer* targetPin)
{
	CShapeContainer* wirePin = (CShapeContainer*)(*m_pWire)[indexPin];
	m_pWire->LinkShapes(targetPin, wirePin);
	if (indexPin == SHAPECONTAINER_CONNECTING_PIN_UNIT)
		__DoSelectCableFromPin(targetPin);
	// Preserve the legacy cleanup of the parent's temporary connection.
	targetPin->GetParent()->m_pshChildConn = NULL;
}

void CConnectorTool::__DoSelectCableFromPin(CShapeContainer* sourcePin)
{
	// An explicit choice on this segment takes precedence over defaults.
	if (m_pWire == NULL || sourcePin == NULL || !m_pWire->m_strTypeIdent.IsEmpty())
		return;

	CShape* parent = sourcePin->GetParent();
	if (parent != NULL && parent->IsKindOf(RUNTIME_CLASS(CShapeWire)))
	{
		// Continuations and starts at existing wire ends retain the cable type.
		m_pWire->m_strTypeIdent = parent->m_strTypeIdent;
		return;
	}

	if (!sourcePin->IsKindOf(RUNTIME_CLASS(CShapePin)) || TABLE_PIN_LABEL == NULL)
		return;
	int type = ((CShapePin*)sourcePin)->m_TypePin;
	if (type < 0 || type >= AfxGetConnectorCount())
		return;
	CString connector = TABLE_PIN_LABEL[type];
	if (connector.IsEmpty())
		return;
	if (connector.Right(4).CompareNoCase(_T(".txt")) != 0)
		connector += _T(".txt");

	CString config = g_sDCableBaseDir + _T("ConnectorCableDefaults.ini");
	TCHAR configPath[MAX_PATH];
	DWORD length = GetFullPathName(config, MAX_PATH, configPath, NULL);
	if (length == 0 || length >= MAX_PATH)
		return;
	TCHAR value[MAX_PATH];
	length = GetPrivateProfileString(_T("DefaultCable"), connector, _T(""),
		value, MAX_PATH, configPath);
	if (length == 0 || length >= MAX_PATH - 1)
		return;
	CString cable = value;
	cable.TrimLeft();
	cable.TrimRight();
	if (cable.IsEmpty() || cable.FindOneOf(_T("\\/:*?")) >= 0 ||
		cable.Right(4).CompareNoCase(_T(".txt")) != 0)
		return;

	// Use the actual file name casing, also used by the existing cable list.
	WIN32_FIND_DATA data;
	HANDLE file = FindFirstFile(AfxGetCablePath() + cable, &data);
	if (file == INVALID_HANDLE_VALUE)
		return;
	FindClose(file);
	if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		m_pWire->m_strTypeIdent = data.cFileName;
}

void CConnectorTool::__DoUpdatePinRects()
{
	// Same pin rectangles as wire drawing, also available before the next paint.
	for (int i = 0; i < 2; ++i)
	{
		CShapePin* pin = (CShapePin*)(*m_pWire)[i];
		CPoint point = i == SHAPECONTAINER_CONNECTING_PIN_UNIT
			? m_pWire->m_Rect.TopLeft() : m_pWire->m_Rect.BottomRight();
		pin->m_Rect = CRect(point, CSize(5, 5));
		CPoint connectionPoint = point -
			CPoint(pin->m_szSize.cx / 2, pin->m_szSize.cy / 2);
		pin->m_rectConect = CRect(connectionPoint, pin->m_szSize) -
			m_pWire->m_Rect.TopLeft();
	}
}

int CConnectorTool::__DoFindShapeIndex(CShape* shape) const
{
	if (shape == NULL || pDoc == NULL || pDoc->m_pObArray == NULL)
		return -1;
	for (int i = 0; i < pDoc->m_pObArray->GetSize(); ++i)
		if ((*pDoc->m_pObArray)[i] == shape)
			return i;
	return -1;
}

BOOL CConnectorTool::IsDrawing() const
{
	return m_Status == ddcStatusConnecting && __DoFindShapeIndex(m_pWire) >= 0;
}

CRect CConnectorTool::Cancel()
{
	m_rectUpdate.SetRectEmpty();
	m_bMouseDown = FALSE;
	int index = __DoFindShapeIndex(m_pWire);
	if (index >= 0)
	{
		__DoAddToUpdateRect(m_pWire);
		for (int i = 0; i < 2; ++i)
		{
			CShape* pin = (*m_pWire)[i];
			if (pin->m_pshConn != NULL)
				__DoAddToUpdateRect(pin->m_pshConn);
		}
		m_pWire = NULL;
		// Delete this exact preview, not the last object in the stack.
		// CShapeWire's destructor releases the reciprocal pin connections.
		pDoc->DeleteObject(index);
	}
	m_pWire = NULL;
	m_Status = ddcStatusBeginConnect;
	return m_rectUpdate;
}

void CConnectorTool::__DoAddToUpdateRect(CShape* shape)
{
	CRect rect;
	shape->GetSizingRect(rect);
	rect.NormalizeRect();
	rect.InflateRect(DCABLE_PADDINGX_DEFAULT, DCABLE_PADDINGY_DEFAULT);
	CRect sheet(CPoint(0, 0), pDoc->GetSize());
	sheet.DeflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
	rect.IntersectRect(rect, sheet);
	m_rectUpdate.UnionRect(m_rectUpdate, rect);
}
