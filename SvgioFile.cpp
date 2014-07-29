#include "stdafx.h"
#include "SvgioFile.h"


CSvgioFile::CSvgioFile(CArchive *par)
{
	m_par = par;
	m_par->WriteString(_HEADER_TPL_SVG);
}


CSvgioFile::~CSvgioFile()
{
	m_par->WriteString(_END_TPL_SVG);
}

void CSvgioFile::WriteLineTo(CPoint point)
{
	CString strCommand1;
	strCommand1.Format("L %i %i ", point.x, point.y);

	coorStr += strCommand1;
}

void CSvgioFile::WriteMoveTo(CPoint point)
{

	CString strCommand1;
	strCommand1.Format("M %i %i ", point.x, point.y);

	coorStr += strCommand1;
}

void CSvgioFile::OpenPath()
{
	coorStr = "";
}

void CSvgioFile::ClosePath()
{
	CString strCommand1;
	strCommand1.Format(_PATH_TPL_SVG, coorStr);

	m_par->WriteString(strCommand1);
}
