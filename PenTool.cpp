#include "stdafx.h"
#include "PenTool.h"


CPenTool::CPenTool(LinkType linkType = ddcLinkTypeNone)
{
	m_LinkType = linkType;
	m_Status = ddcStatusDrawingNone;
}


CPenTool::~CPenTool()
{
}

void CPenTool::MoveTo(CPoint point)
{

}

void CPenTool::MouseDown(CPoint point)
{
	//Deselect all
	if (m_Status == ddcStatusDrawingNone){
		DeselectAll();
	}
}

void CPenTool::MouseUp(CPoint point)
{
	if (m_Status == ddcStatusPolyDrawingIni){
		
	}
}