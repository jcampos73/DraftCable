///////////////////////////////////////////////////////////////////////////////
//DraftCable IO Module
//Classes to serialize/deserialize shapes in several formats.
//Following shapes included:
//
//Version:
//	1.00.0000
//	05/01/2006
//(Ed./Rev.)Author:
//	00/00	J.Campos	05/01/2006

//////////////////////////////////////////////////////////////////////
// Implementation of the CGbrioFile class.
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "GbrioFile.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGbrioFile::CGbrioFile(CArchive *par)
{

	m_par=par;
}

CGbrioFile::~CGbrioFile()
{

}

void CGbrioFile::WriteLineTo(CPoint point)
{
	CString strCommand;
	strCommand.Format("X%05iY%05i*\n",point.x,point.y);

	m_par->WriteString(strCommand);
}

void CGbrioFile::WriteMoveTo(CPoint point)
{

	CString strCommand1="D02*\n";
	CString strCommand2;
	strCommand2.Format("X%05iY%05i*\n",point.x,point.y);
	CString strCommand3="D01*\n";

	m_par->WriteString(strCommand1);
	m_par->WriteString(strCommand2);
	m_par->WriteString(strCommand3);


}

//////////////////////////////////////////////////////////////////////
// Implementation of the CDdwioFile class.
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDdwioFile::CDdwioFile(CArchiveDb *par)
{
	m_stCommandLast="";
	m_par=par;

	m_bOrg=FALSE;
}

CDdwioFile::~CDdwioFile()
{

}

void CDdwioFile::WritePin(POINT point,UINT uiPinnumber,UINT uiPos,DWORD dwStyle,int iPinType/* = 0*/){

	//WRITE COMMAND
	CString strCommand="[pin]\n";

	CPoint rpoint=CPoint(point)-m_POrg;
	if(m_stCommandLast.CompareNoCase("[pin]")){

		m_stCommandLast="[pin]";
		m_par->WriteString(strCommand);
	}

	strCommand.Format("%i,%i,%i,%i,%i,%i\n",
		rpoint.x,rpoint.y,uiPinnumber,uiPos,dwStyle,iPinType);

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteLabel(label *lb){

	//SET ORIGIN
	if(!m_bOrg){

		m_bOrg=TRUE;
		m_POrg=lb->rect->TopLeft();

	}

	//WRITE COMMAND
	CString strCommand="[label]\n";

	if(m_stCommandLast.CompareNoCase("[label]")){

		m_stCommandLast="[label]";
		m_par->WriteString(strCommand);
	}

	strCommand="";
	CString stfield;

	CPoint p[4];
	p[0]=lb->rect->TopLeft();//0;
	p[2]=lb->rect->BottomRight();//-m_Rect.TopLeft();

	p[1]=CPoint(p[2].x,p[0].y);
	p[3]=CPoint(p[0].x,p[2].y);

	int i;
	for(i=0;i<3;i++){

		CPoint point=CPoint(p[i])-m_POrg;

		stfield.Format("%i,%i,",point.x,point.y);
		strCommand+=stfield;

	}

	CPoint point=CPoint(p[i])-m_POrg;

	stfield.Format("%i,%i\n",point.x,point.y);

	strCommand+=stfield;

	m_par->WriteString(strCommand);


	//---------
	std::string ststr=*lb->slabel;
	int idata=ststr.find("\n");
	while(idata!=std::string::npos){
		ststr=ststr.substr(0,idata)+ststr.substr(idata+1,ststr.length()-idata-1);
		idata=ststr.find("\n");
	}
	//---------


	strCommand=ststr.c_str();//*lb->slabel;
	strCommand+=",";
	strCommand+=ststr.c_str();//*lb->slabel;
	strCommand+="\n";

	m_par->WriteString(strCommand);

	strCommand.Format("%i,%i\n",lb->bver,lb->iSize);

	m_par->WriteString(strCommand);

}
void CDdwioFile::WritePolyline(LPPOINT lpPoints, int nCount )
{
	//SET ORIGIN
	if(!m_bOrg){

		m_bOrg=TRUE;

		m_POrg=lpPoints[0];

		for(int i=1;i<nCount;i++){

			if(lpPoints[i].x<m_POrg.x){

				m_POrg=lpPoints[i];

			}
			else if(lpPoints[i].x==m_POrg.x){

				if(lpPoints[i].y<m_POrg.y){

					m_POrg=lpPoints[i];

				}

			}
		}

	}

	CString strCommand="[polyline]\n";

	if(m_stCommandLast.CompareNoCase("[polyline]")){

		m_stCommandLast="[polyline]";
		m_par->WriteString(strCommand);
	}

	strCommand.Format("%i,",nCount);
	CString stfield;

	int i;
	for(i=0;i<nCount-1;i++){

		CPoint point=CPoint(lpPoints[i])-m_POrg;

		stfield.Format("%i,%i,",point.x,point.y);
		strCommand+=stfield;

	}

	CPoint point=CPoint(lpPoints[i])-m_POrg;

	stfield.Format("%i,%i\n",point.x,point.y);

	strCommand+=stfield;

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteRect(CRect *rect){

	//SET ORIGIN
	if(!m_bOrg){

		m_bOrg=TRUE;

		m_POrg=rect->TopLeft();
	}

	//COMMAND
	CString strCommand="[rectangle]\n";

	if(m_stCommandLast.CompareNoCase("[rectangle]")){

		m_stCommandLast="[rectangle]";
		m_par->WriteString(strCommand);
	}


	//WRITE DATA

	CPoint point1=rect->TopLeft()-m_POrg;
	CPoint point2=rect->BottomRight()-m_POrg;
	
	strCommand.Format("%i,%i,%i,%i\n",
		point1.x,
		point1.y,
		point2.x,
		point2.y);

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteEllipse(CRect *rect,LPCOLORREF lpcrFill/*=NULL*/){

	//SET ORIGIN
	if(!m_bOrg){

		m_bOrg=TRUE;

		m_POrg=rect->TopLeft();
	}

	//COMMAND
	CString strCommand="[ellipse]\n";

	if(m_stCommandLast.CompareNoCase("[ellipse]")){

		m_stCommandLast="[ellipse]";
		m_par->WriteString(strCommand);
	}


	//WRITE DATA

	CPoint point1=rect->TopLeft()-m_POrg;
	CPoint point2=rect->BottomRight()-m_POrg;
	
	if(!lpcrFill){
		strCommand.Format("%i,%i,%i,%i\n",
			point1.x,
			point1.y,
			point2.x,
			point2.y);
	}
	else{
		strCommand.Format("%i,%i,%i,%i,0x%08x\n",
			point1.x,
			point1.y,
			point2.x,
			point2.y,
			*lpcrFill);
	}

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteUnit(CString name,CRect *rect,LPCTSTR lpszLabel){

	//SET ORIGIN
	//Units are written as members of whole drawing so org is not changed!
	if(!m_bOrg){

		m_bOrg=TRUE;

		m_POrg=CPoint(0,0);//rect->TopLeft();
	}


	//COMMAND
	CString strCommand="["+name+"]\n";

	if(m_stCommandLast.CompareNoCase("["+name+"]")){

		m_stCommandLast="["+name+"]";
		m_par->WriteString(strCommand);
	}


	//WRITE DATA

	CPoint point1=rect->TopLeft()-m_POrg;
	CPoint point2=rect->BottomRight()-m_POrg;
	
	if(lpszLabel){
		strCommand.Format("%i,%i,%i,%i,%s\n",
			point1.x,
			point1.y,
			point2.x,
			point2.y,
			lpszLabel);
	}
	else{
		strCommand.Format("%i,%i,%i,%i\n",
			point1.x,
			point1.y,
			point2.x,
			point2.y);
	}

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteConnect(int *pConn,int nCount){

	//SET ORIGIN
	//No apply, becouse only logic connections are written.


	//COMMAND
	CString strCommand="[*connection]\n";

	if(m_stCommandLast.CompareNoCase("[*connection]")){

		m_stCommandLast="[*connection]";
		m_par->WriteString(strCommand);
	}


	//WRITE DATA
	
	strCommand.Format("%i,%i,%i,%i\n",
		pConn[0],
		pConn[1],
		pConn[2],
		pConn[3]);

	m_par->WriteString(strCommand);
}

void CDdwioFile::WriteSheet(LPCTSTR lpszLabel){

	//COMMAND
	CString name="*SHEET";

	if(lpszLabel!=NULL){
		name=lpszLabel;
	}

	CString strCommand="["+name+"]\n";

	if(m_stCommandLast.CompareNoCase("["+name+"]")){

		m_stCommandLast="["+name+"]";
		m_par->WriteString(strCommand);
	}
}

//This overloaded function is to write buses DDW files.
void CDdwioFile::WriteUnit(CString name,LPPOINT lpPoints, int nCount,LPCTSTR lpszLabel){

	//SET ORIGIN
	//Units are written as members of whole drawing so org is not changed!
	if(!m_bOrg){

		m_bOrg=TRUE;

		m_POrg=CPoint(0,0);//rect->TopLeft();
	}


	//COMMAND
	CString strCommand="["+name+"]\n";

	if(m_stCommandLast.CompareNoCase("["+name+"]")){

		m_stCommandLast="["+name+"]";
		m_par->WriteString(strCommand);
	}

	//WRITE DATA
	strCommand.Format("%i,",nCount);
	CString stfield;

	int i=0;
	for(i=0;i<nCount-1;i++){

		CPoint point=CPoint(lpPoints[i])-m_POrg;

		stfield.Format("%i,%i,",point.x,point.y);
		strCommand+=stfield;

	}

	CPoint point=CPoint(lpPoints[i])-m_POrg;

	stfield.Format("%i,%i",point.x,point.y);

	strCommand+=stfield;

	m_par->WriteString(strCommand);

	//WRITE LABEL
	if(lpszLabel){
		strCommand.Format(",%s\n",
			lpszLabel);
	}
	else{
		strCommand="\n";
	}

	m_par->WriteString(strCommand);

}