#include "stdafx.h"
#include "PlaceTool.h"


CPlaceTool::CPlaceTool()
{
}


CPlaceTool::~CPlaceTool()
{
}

void CPlaceTool::MoveTo(CPoint point)
{

}

void CPlaceTool::MouseDown(CPoint point)
{

}

void CPlaceTool::MouseUp(CPoint point)
{
	CShape *pSh = __DoCreateNewItem();

	pSh->OnLButtonDown(0, point);
}

CShape* CPlaceTool::__DoCreateNewItem()
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