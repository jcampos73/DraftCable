#pragma once
#include "afx.h"

/*
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
<path d="M30,1h40l29,29v40l-29,29h-40l-29-29v-40z" stroke="#000" fill="none"/>
<path d="M31,3h38l28,28v38l-28,28h-38l-28-28v-38z" fill="#a23"/>
<text x="50" y="68" font-size="48" fill="#FFF" text-anchor="middle"><![CDATA[410]]></text>
</svg>

<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="400px" height="150px"
xmlns="http://www.w3.org/2000/svg" version="1.1">
<desc>Example NewCoordSys - New user coordinate system</desc>
<g fill="none" stroke="black" stroke-width="3" >
<!-- Draw the axes of the original coordinate system -->
<line x1="0" y1="1.5" x2="400" y2="1.5" />
<line x1="1.5" y1="0" x2="1.5" y2="150" />
</g>

*/
class CSvgioFile :
	public CStdioFile
{
protected:
	const CString _ENDL_SVG = "\r\n";

	const CString _HEADER_TPL_SVG = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%ipx\" height=\"%ipx\">\r\n";
	const CString _PATH_TPL_SVG = "<path d=\"%sZ\" stroke=\"#000\" fill=\"none\"/>\r\n";
	const CString _TEXT_TPL_SVG = "<text x=\"%i\" y=\"%i\" font-family=\"sans - serif\" font-size=\"%ipx\" fill=\"black\">%s</text>\r\n";
	const CString _END_TPL_SVG = "</svg>\r\n";
	CString coorStr = "";
public:
	CArchive * m_par;
	void WriteMoveTo(CPoint point);
	void WriteLineTo(CPoint point);
	void OpenPath();
	void ClosePath();
	void WriteText(CPoint point, CString text, CFont * pFont);
	CSvgioFile(CArchive *par, CSize szDesign =  CSize(600, 450));
	~CSvgioFile();
};

