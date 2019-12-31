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

CRect CAbstractTool::MoveTo(UINT nFlags, CPoint point) { return new CRect(0, 0, 0, 0); }
CRect CAbstractTool::MouseDown(CPoint point) { return new CRect(0, 0, 0, 0); }
CRect CAbstractTool::MouseUp(CPoint point) { return new CRect(0, 0, 0, 0); }

//Deselect all the shapes
CRect CAbstractTool::DeselectAll(){
	//Deselect all
	POSITION pos = m_pObListSel->GetHeadPosition();
	CRect RectMult(0, 0, 0, 0);

	while (pos != NULL){
		//Deselect shape
		CShape* pSh = (CShape*)m_pObListSel->GetNext(pos);

		CRect rect2 = pSh->m_Rect;

		rect2.InflateRect(DCABLE_GRIDX_DEFAULT, DCABLE_GRIDY_DEFAULT);

		RectMult.UnionRect(RectMult, rect2);

		pSh->Unselect();
	}
	//Add to selected
	m_pObListSel->RemoveAll();

	return RectMult;
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

void CAbstractTool::_DoAddNewShapeToStack(UINT nFlags, CPoint point)
{
	//This should be changed to sub-method
	int index = pDoc->m_ObArray.GetUpperBound();
	if (index > -1){
		if ((CShape*)pDoc->m_ObArray[index] != pDoc->m_pSh
			|| pDoc->m_iToolType == _TOOLTYPEPOLY_DRAFTCABLE){
			if ((CShape*)pDoc->m_ObArray[index] != pDoc->m_pSh
				)
				pDoc->AddObject(pDoc->m_pSh);
			pDoc->m_pSh->OnLButtonDown(nFlags, point);
		}
	}
	else{
		pDoc->AddObject(pDoc->m_pSh);
		pDoc->m_pSh->OnLButtonDown(nFlags, point);
	}
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
	if (sName.Compare(_T("CShape")) && sName.Compare(_T("CShapeContainer"))){

		//copy & increment
		CShape* pShCopy = (CShape*)pRuntimeClass->CreateObject();
		//Add this to copy method:
		//pcmdDeque
		//m_pCursorArray
		*pShCopy = *pSh;
		pShCopy->pcmdDeque = pDoc->cmdDeque;

		pDoc->m_pSh = pShCopy;
		pDoc->m_pSh->m_pCursorArray = pDoc->m_CursorArray;

		//(*pShCopy)++;
		++(*pShCopy);

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

	}

	return pShCopy;
}

//Status machine
void CAbstractTool::__DoState(CPoint point, EventType eventType)
{
	//To be overriden
}