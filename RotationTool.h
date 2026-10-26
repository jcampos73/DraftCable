#pragma once
#include "AbstractTool.h"

class CRotationTool : public CAbstractTool
{
public:
    CRotationTool();
    DECLARE_SERIAL(CRotationTool);
    virtual ~CRotationTool();

    virtual CRect MouseDown(UINT nFlags, CPoint point);
    virtual CRect MoveTo(UINT nFlags, CPoint point);
    virtual CRect MouseUp(CPoint point);

private:
    BOOL m_bDragging;
    BOOL m_bChanged;

    void AddUpdateRect(CRect& updateRect, CShape* shape);
    CRect FinishDrag();
};
