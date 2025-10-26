#include "stdafx.h"
#include "ShapeSelection.h"

/////////////////////////////////////////////////////////////////////////////
// CShapeSelection
IMPLEMENT_SERIAL(CShapeSelection, CShapeRect, 1)

CShapeSelection::CShapeSelection(LPRECT lpRect/*=NULL*/, UINT nId/*=0*/, cmddeque *cmddq /*=NULL*/) :CShapeRect(lpRect, nId, cmddq)
{

}


CShapeSelection::~CShapeSelection()
{
}

void CShapeSelection::OnDraw(CDC *pDC) {
	CShapeRect::OnDraw(pDC);
}