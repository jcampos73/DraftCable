#include "stdafx.h"
#include "RotationTool.h"

IMPLEMENT_SERIAL(CRotationTool, CAbstractTool, 1)

CRotationTool::CRotationTool()
    : m_bDragging(FALSE), m_bChanged(FALSE)
{
    pDoc = NULL;
    m_pObListSel = NULL;
}

CRotationTool::~CRotationTool()
{
    FinishDrag();
}

CRect CRotationTool::MouseDown(UINT /*nFlags*/, CPoint point)
{
    CRect updateRect = FinishDrag();
    if (m_pObListSel == NULL || m_pObListSel->IsEmpty())
        return updateRect;

    // Rotate the existing selection; do not select or insert document objects.
    m_bDragging = TRUE;
    m_bChanged = FALSE;
    POSITION pos = m_pObListSel->GetHeadPosition();
    while (pos != NULL)
    {
        CShape* shape = (CShape*)m_pObListSel->GetNext(pos);
        AddUpdateRect(updateRect, shape);
        shape->m_Point = point;
        shape->m_RectLast = shape->m_Rect;
        shape->m_Mode = _DRAFTDRAW_MODE_ACTIVE;
    }
    return updateRect;
}

CRect CRotationTool::MoveTo(UINT nFlags, CPoint point)
{
    CRect updateRect(0, 0, 0, 0);
    if (!m_bDragging || !(nFlags & MK_LBUTTON) || m_pObListSel == NULL)
        return updateRect;

    POSITION pos = m_pObListSel->GetHeadPosition();
    while (pos != NULL)
    {
        CShape* shape = (CShape*)m_pObListSel->GetNext(pos);
        AddUpdateRect(updateRect, shape);
        float previousAngle = shape->m_fAngle;
        shape->OnRotate(nFlags, point);
        if (shape->m_fAngle != previousAngle)
            m_bChanged = TRUE;
        AddUpdateRect(updateRect, shape);
    }
    return updateRect;
}

CRect CRotationTool::MouseUp(CPoint /*point*/)
{
    return FinishDrag();
}

CRect CRotationTool::FinishDrag()
{
    CRect updateRect(0, 0, 0, 0);
    if (!m_bDragging)
        return updateRect;

    m_bDragging = FALSE;
    if (m_pObListSel != NULL)
    {
        POSITION pos = m_pObListSel->GetHeadPosition();
        while (pos != NULL)
        {
            CShape* shape = (CShape*)m_pObListSel->GetNext(pos);
            AddUpdateRect(updateRect, shape);
            shape->m_Mode = _DRAFTDRAW_MODE_IDLE;
            shape->m_RectLast = shape->m_Rect;
        }
    }
    if (m_bChanged && pDoc != NULL)
        pDoc->SetModifiedFlag(TRUE);
    m_bChanged = FALSE;
    return updateRect;
}

void CRotationTool::AddUpdateRect(CRect& updateRect, CShape* shape)
{
    CRect rect;
    shape->GetSizingRect(rect);
    rect.NormalizeRect();
    rect.InflateRect(DCABLE_PADDINGX_DEFAULT, DCABLE_PADDINGY_DEFAULT);
    updateRect.UnionRect(updateRect, rect);
}
