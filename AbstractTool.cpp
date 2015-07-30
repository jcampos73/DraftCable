#include "stdafx.h"
#include "AbstractTool.h"

CAbstractTool::CAbstractTool()
{
}

CAbstractTool::~CAbstractTool()
{
}

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