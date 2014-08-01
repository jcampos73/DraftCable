#include "stdafx.h"
#include "SvgioFile.h"


CSvgioFile::CSvgioFile(CArchive *par, CSize szDesign /*= CSize(600, 450)*/)
{
	CString strCommand1;
	
	m_par = par;

	strCommand1.Format(_HEADER_TPL_SVG, szDesign.cx, szDesign.cy);
	m_par->WriteString(strCommand1);
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

void CSvgioFile::WriteText(CPoint point, CString text, CFont * pFont)
{
	CString strCommand1;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	strCommand1.Format(_TEXT_TPL_SVG, point.x, point.y, -lf.lfHeight-3 ,text);

	m_par->WriteString(strCommand1);
}
