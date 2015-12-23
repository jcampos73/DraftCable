#include "stdafx.h"
#include "DraftDrawDoc.h"

#include "AbstractTool.h"


CAbstractTool::CAbstractTool()
{
}

CAbstractTool::~CAbstractTool()
{
}

void CAbstractTool::MoveTo(CPoint point) {}
void CAbstractTool::MouseDown(CPoint point) {}
void CAbstractTool::MouseUp(CPoint point) {}

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

CShape* CAbstractTool::__DoCreateNewItem()
{
	//Create a new shape
	CShape* pShCopy = NULL;
	CShape* pSh = (CShape*)m_pObList->GetTail();
	CRuntimeClass* pRuntimeClass = pSh->GetRuntimeClass();

	CString sName = CString(pRuntimeClass->m_lpszClassName);
	if (sName.Compare("CShape") && sName.Compare("CShapeContainer")){

		//copy & increment
		CShape* pShCopy = (CShape*)pRuntimeClass->CreateObject();
		*pShCopy = *pSh;
		(*pShCopy)++;

		m_pObList->AddTail(pShCopy);

		/*
		//Beware! This mechanism is not very good. It would be better a suitable '=' operator.
		pSh1->pcmdDeque = pDoc->cmdDeque;

		pDoc->m_pSh = pSh1;
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

void CAbstractTool::__DoState(CPoint point, EventType eventType)
{
}