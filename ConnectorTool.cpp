#include "stdafx.h"

#include "ConnectorTool.h"

IMPLEMENT_SERIAL(CConnectorTool, CAbstractTool, 1)

CConnectorTool::CConnectorTool()
{
	m_Status = ddcStatusBeginConnect;
}


CConnectorTool::~CConnectorTool()
{
}

CRect CConnectorTool::MoveTo(UINT nFlags, CPoint point)
{
	return CRect(0, 0, 0, 0);
}

CRect CConnectorTool::MouseDown(UINT nFlags, CPoint point)
{
	__DoState(point, ddcEventMouseDown);

	return CRect(0, 0, 0, 0);
}

CRect CConnectorTool::MouseUp(CPoint point)
{
	__DoState(point, ddcEventMouseUp);
	return CRect(0, 0, 0, 0);
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

	if (eventType == ddcEventMouseDown){
		m_ptMouseDownPrev = point;
	}
}

void CConnectorTool::__DoStateBeginConnect(CPoint point, EventType eventType)
{
	switch (eventType)
	{
	case ddcEventMouseDown:
		__DoConnectShapes(point);
		_DoAddNewShapeToStack(0, point);
		m_Status = ddcStatusConnecting;
		break;
	}
}

void CConnectorTool::__DoStateConnecting(CPoint point, EventType eventType)
{
	BOOL bConnectionTmp = FALSE;//set when a connection is pending
	int indexPin = SHAPECONTAINER_CONNECTING_PIN_WIRE;
	CShapeContainer *pShContConnect = NULL;

	if (m_ptMouseDownPrev != point){
		//Try to connect destiny. If not connected then
		if (!__DoConnectShapes(point)){

			int index;
			CShape *pSh = (CShape *)pDoc->LastObject(index);

			//if (pSh->IsSelected()) {
				//pShSel = pSh;
				//Keep former position rectangle to erase
				CRect rect2;
				rect2 = pSh->m_RectLast;
				rect2.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);
				//rect_union2.UnionRect(rect_union2, rect2);
				//Pass event to shape
				pSh->OnLButtonUp(0, point);
				//Current position to erase
				CRect rect;
				pSh->GetSizingRect(rect);
				//bRedrawAll |= _DoAddToUpdateRect(rect_union, rect);
			//}

			//Create a new shape
			pDoc->m_pSh = new CShapeWire(NULL, 0, pDoc->cmdDeque);
			pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;
			pDoc->AddObject(pDoc->m_pSh);

			if (pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))) {
				CShapeContainer *pShContainer = (CShapeContainer *)pSh;

				bConnectionTmp = pShContainer->PtInRect(&point, &pShContConnect);
				if (bConnectionTmp) {
					indexPin = SHAPECONTAINER_CONNECTING_PIN_UNIT;
				}
			}

			//Connect
			if (bConnectionTmp) {
				if (pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))) {
					CShapeContainer *pShCont1 = (CShapeContainer *)pDoc->m_pSh;
					CShape *pSh = (*pShCont1)[indexPin];
					if (pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))) {

						CShapeContainer *pShCont2 = (CShapeContainer *)pSh;
						pShCont1->LinkShapes(pShContConnect, pShCont2);

						//Previous mechanism to 19/01/2005 used temporal pointers in m_pChildConn. Delete this line ASAP. 
						pShContConnect->GetParent()->m_pshChildConn = NULL;
					}
				}
			}

			//Set new shape in sizing mode
			pDoc->m_pSh->OnLButtonDown(0, point);
		}
	}
}

BOOL CConnectorTool::__DoConnectShapes(CPoint point){
	//Local variables
	int index;
	BOOL result = FALSE;
	BOOL bConnectionTmp = FALSE;//set when a connection is pending
	CShapeContainer *pShContConnect = NULL;

	CShape *pSh;
	pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);

	while (pSh
		&& bConnectionTmp == FALSE
		&& pSh != pDoc->m_pSh){

		if ((pSh->GetRuntimeClass())->IsDerivedFrom(RUNTIME_CLASS(CShapeContainer))){

			CShapeContainer *pShContainer = (CShapeContainer *)pSh;

			bConnectionTmp = pShContainer->PtInRect(&point, &pShContConnect);
		}

		pSh = (CShape *)pDoc->PrevObject(index);

	}//end while

	if (bConnectionTmp){
		if (pDoc->m_pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
			CShapeContainer *pShCont1 = (CShapeContainer *)pDoc->m_pSh;
			CShape *pSh = (*pShCont1)[0];
			if (pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

				CShapeContainer *pShCont2 = (CShapeContainer *)pSh;
				pShCont1->LinkShapes(pShContConnect, pShCont2);

				result = TRUE;
			}
		}
	}

	return result;
}