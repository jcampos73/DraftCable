#include "stdafx.h"
#include "DraftDrawDoc.h"

#include "AbstractTool.h"

IMPLEMENT_SERIAL(CAbstractTool, CObject, 1)

CAbstractTool::CAbstractTool()
{
	//m_pObListSel = new CObList();
}

CAbstractTool::~CAbstractTool()
{
	//delete(m_pObListSel);
}

void CAbstractTool::MoveTo(CPoint point) {}
void CAbstractTool::MouseDown(CPoint point) {}
void CAbstractTool::MouseUp(CPoint point) {}

//Deselect all the shapes
void CAbstractTool::DeselectAll(){
	//Deselect all
	POSITION pos = m_pObListSel->GetHeadPosition();
	while (pos != NULL){
		//Deselect shape
		CShape* pSh = (CShape*)m_pObListSel->GetNext(pos);
		pSh->Unselect();
	}
	//Add to selected
	m_pObListSel->RemoveAll();
}

//Get current shape for operations
CShape* CAbstractTool::GetCurrentShape()
{
	int index;
	CShape *pSh;

	pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);
	return pSh;
}

//Create new item and add to stack
CShape* CAbstractTool::__DoCreateNewItem(CPoint point)
{
	//Create a new shape
	int index;
	CShape* pShCopy = NULL;
	CShape *pSh = (CShape *)pDoc->LastObject(index);
	pSh = (CShape *)pDoc->PrevObject(index);
	CRuntimeClass* pRuntimeClass = pSh->GetRuntimeClass();

	CString sName = CString(pRuntimeClass->m_lpszClassName);
	if (sName.Compare("CShape") && sName.Compare("CShapeContainer")){

		//copy & increment
		CShape* pShCopy = (CShape*)pRuntimeClass->CreateObject();
		//Add this to copy method:
		//pcmdDeque
		//m_pCursorArray
		*pShCopy = *pSh;
		pShCopy->pcmdDeque = pDoc->cmdDeque;

		pDoc->m_pSh = pShCopy;
		pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;

		(*pShCopy)++;

		if ((pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))) ||
			(pSh->IsKindOf(RUNTIME_CLASS(CShapePin))) ||
			(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel)))
			){

			if (pDoc->m_pSh->m_bNoResize){
				pDoc->AddObject(pDoc->m_pSh);
				pDoc->m_pSh->OnLButtonDown(0, point);
			}

		}

		::SetCursor(pDoc->m_CursorArray[CURSOR_DRAW]);

		/*
		//Beware! This mechanism is not very good. It would be better a suitable '=' operator.
		pShCopy->pcmdDeque = pDoc->cmdDeque;

		pDoc->m_pSh = pShCopy;
		pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;

		if ((pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))) ||
		(pSh->IsKindOf(RUNTIME_CLASS(CShapePin))) ||
		(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel)))
		){

			if (pDoc->m_pSh->m_bNoResize){
				pDoc->AddObject(pDoc->m_pSh);
				pDoc->m_pSh->OnLButtonDown(0, point);
			}

		}

		::SetCursor(pDoc->m_CursorArray[CURSOR_DRAW]);
		*/
	}

	return pShCopy;
}

//Status machine
void CAbstractTool::__DoState(CPoint point, EventType eventType)
{
	//To be overriden
}