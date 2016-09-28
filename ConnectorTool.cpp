#include "stdafx.h"

#include "ConnectorTool.h"

IMPLEMENT_SERIAL(CConnectorTool, CAbstractTool, 1)

CConnectorTool::CConnectorTool()
{
}


CConnectorTool::~CConnectorTool()
{
}

void CConnectorTool::MoveTo(CPoint point)
{

}

void CConnectorTool::MouseDown(CPoint point)
{
	__DoState(point, ddcEventMouseDown);
}

void CConnectorTool::MouseUp(CPoint point)
{
	__DoState(point, ddcEventMouseUp);
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
		m_Status = ddcStatusConnecting;
		break;
	}
}

void CConnectorTool::__DoStateConnecting(CPoint point, EventType eventType)
{
	if (m_ptMouseDownPrev != point){
		//Try to connect destiny. If not connected then
		if (!__DoConnectShapes(point)){
			__DoCreateNewItem(point);
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

	while (pSh && bConnectionTmp == FALSE){
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