#include "stdafx.h"
#include <ios>
#include <sstream>

#include "Tipography.h"

#include "ShapeUnit.h"

//Dialogs
#include "DraftDraw.h"
#include "CableEdit.h"
#include "PartEdit.h"
#include "DialogPartProp.h"
#include "DialogText.h"






char **TABLE_PIN_LABEL;

//First implementation of TABLE_PIN_LABEL, before was unified with
//afxConnectorList in AfxInitGlobal
//keeped as reference
/*
char TABLE_PIN_LABEL[][256]={
"CANON_CD30",
"RJ45_516"
};
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeUnit
/*IMPLEMENT_DYNCREATE(CShapeUnit, CShapeContainer)*/
IMPLEMENT_SERIAL(CShapeUnit, CShapeContainer, 1)

CShapeUnit::CShapeUnit(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapeContainer(lpRect,nId,cmddq)
{
	m_uiShapeType=ddcShapeUnit;
	m_bNoResize=TRUE;
	m_bFlagFileOpen=FALSE;
	//06/03/2005
	m_bFlagPartEdit=FALSE;

	m_uiPartnumber=1;
	mf_cbPatchPEdit=SDisplayPatchPProp;//NULL;
	m_PointspCount=0;//0 polylines
	m_LabelsCount=0;//0 labels


	m_Rect=CRect(0,0,100,100);

	//m_ShapeNumber=16;
m_ShapeNumber=0;

	//m_Alfa=20.0*PI/180;
	//m_Beta=60.0*PI/180;
	m_d=0.10;
	m_a1=0.70/4/*16*/;
	m_b1=0.60;
	m_a2=0.60/16;
	m_b2=0.60/16;

	/*
	for(int i=0;i<16;i++){

		CShapePin *psh=new CShapePin(i);
		psh->m_Mode=1;
		m_obarrShapearr.Add(psh);
	}
	*/

	m_fontFont1.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                  // lpszFacename

#define _MAXPOINTBUF_DRAFTCABLE		256
	m_pPBuffer=(CPoint *)GlobalAlloc(GMEM_ZEROINIT,sizeof(CPoint)*_MAXPOINTBUF_DRAFTCABLE);
}

//Copy constructor
CShapeUnit::CShapeUnit(CShapeUnit& ShUnit):CShapeContainer(ShUnit){

}

CShapeUnit::~CShapeUnit()
{
	//when somebody deletes the shape connection handlers
	//must appear in units or wires connected to deleted wire
	for(int i=0;i<m_obarrShapearr.GetSize();i++){
		CShape *pSh=(CShape *)m_obarrShapearr[i];
		if(!pSh->IsKindOf(RUNTIME_CLASS(CShapePin))){
			continue;
		}
		CShapePin *pPin=(CShapePin *)pSh;
		if(pPin->m_bConect){
			((CShapePin *)pPin->m_pshChildConn)->Disconnect(CPoint(1,1),TRUE);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CShapePin implementation
CShape& CShapeUnit::operator=( const CShape& Sh ){

	//03/01/2005
	CShapeContainer::operator =(Sh);


	//Previous code
	CRuntimeClass* prt = Sh.GetRuntimeClass();
	ASSERT( strcmp( prt->m_lpszClassName, "CShapeUnit" )  == 0 );

	CShapeUnit *pShUnit=(CShapeUnit *)&Sh;

	m_sUnitName=pShUnit->m_sUnitName;

	m_uiPartnumber=pShUnit->m_uiPartnumber;

	//03/01/2005

	m_Rect=pShUnit->m_Rect;

	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	m_PointspCount=pShUnit->m_PointspCount;
	for(int indexpline=0;indexpline<pShUnit->m_PointspCount;indexpline++){
		int idata=pShUnit->m_pPoints[indexpline][0];
		m_pPoints[indexpline]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
		for(int i=0;i<pShUnit->m_pPoints[indexpline][0]*2+1;i++){
			m_pPoints[indexpline][i]=pShUnit->m_pPoints[indexpline][i];
		}
	}

	m_ShapeNumber = pShUnit->m_ShapeNumber;
	m_obarrShapearr.SetSize(pShUnit->m_obarrShapearr.GetSize());
	for(int i=0;i<pShUnit->m_obarrShapearr.GetSize();i++){
		CRuntimeClass *pRuntimeClass=pShUnit->m_obarrShapearr[i]->GetRuntimeClass( );
		CShape* pSh1 = (CShape*)pRuntimeClass->CreateObject();

		m_obarrShapearr[i]=pShUnit->m_obarrShapearr[i];

		if(pShUnit->m_obarrShapearr[i]->IsKindOf(RUNTIME_CLASS(CShapePin))){

			m_obarrShapearr[i]=new CShapePin(*((CShapePin*)pShUnit->m_obarrShapearr[i]));

			//20/01/2004
			((CShapePin*)m_obarrShapearr[i])->SetParent(this);

			//20/01/2004
			//This is very important. In this design shapes allways are created in drag mode (neither selected neither normal).
			((CShapePin*)m_obarrShapearr[i])->Select(FALSE);

			//There is a problem with this assignement.
			/*
			*pSh1=*((CShape*)pShUnit->m_obarrShapearr[i]);
			m_obarrShapearr[i]=pSh1;
			*/

			CShapePin *pShPin=(CShapePin *)m_obarrShapearr[i];
		}
	}//end for

	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	m_LabelsCount=pShUnit->m_LabelsCount;
	for(int indexlabel=0;indexlabel<pShUnit->m_LabelsCount;indexlabel++){
		m_pLabels[indexlabel]=new label(*pShUnit->m_pLabels[indexlabel]);
	}


	if(!m_bNoResize){
		m_pPoints0=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
		for(int indexpline=0;indexpline<pShUnit->m_PointspCount;indexpline++){
			int idata=pShUnit->m_pPoints0[indexpline][0];
			m_pPoints0[indexpline]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
			for(int i=0;i<pShUnit->m_pPoints0[indexpline][0]*2+1;i++){
				m_pPoints0[indexpline][i]=pShUnit->m_pPoints0[indexpline][i];
			}
		}
	}

	return *this;
}
CShape& CShapeUnit::operator++( ){
	m_uiPartnumber++;
	//LoadUnit(m_sUnitName);

	//03/01/2005
	CString strFile=m_sUnitName;
	//get libray name
	CString strLib=strFile.Left(strFile.Find('.'));
	//remove library name
	strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

	CString str=strFile.Left(strFile.Find('.'));
	str+="%i";
	m_strIdent.Format(str,m_uiPartnumber);


	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CShapeUnit message handlers

void CShapeUnit::OnLButtonUp(UINT nFlags, CPoint point){

	if(m_Mode==_DRAFTDRAW_MODE_SEL&&!m_bNoResize){
		m_Rect.NormalizeRect();
		Resize();
	}

	CShapeContainer::OnLButtonUp(nFlags,point);
}

void CShapeUnit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CShape::OnLButtonDown(nFlags,point);
	if(!m_Mode){
		m_Mode=1;

		CString strFile=m_sUnitName;
		//get libray name
		CString strLib=strFile.Left(strFile.Find('.'));
		//remove library name
		strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

		//Check if part is of type TB
		CString str=strFile;

		int nIndex=str.Find('.');

		if(nIndex>=0){
			str=str.Left(nIndex);
		}

//Temporally dissabled
/*
		if(str.CompareNoCase("TB")==0){

			if(mf_cbPatchPEdit!=NULL){

				(*mf_cbPatchPEdit)(m_uiShapeId);
				return;
			}
		}
*/


		//Else edit part normally
		CPartEdit dPart;
		dPart.m_pShape=this;
		dPart.m_strName=m_strIdent;
		//wire identifier has priority
		if(m_strIdent.IsEmpty()){
			for(int i=0;i<m_LabelsCount;i++){
				CString str=*m_pLabels[i]->sname;
				str.MakeUpper();
				if(str.CompareNoCase("TITLE=")==0){
					dPart.m_strName=*m_pLabels[i]->slabel;
					break;
				}
			}

		}
		if(dPart.DoModal()==IDOK){
			//Search for a title label
			//====================================
			int i;
			for(i=0;i<m_LabelsCount;i++){
				CString str=*m_pLabels[i]->sname;
				str.MakeUpper();
				if(str.CompareNoCase("TITLE=")==0){//if(str.Find("TITLE")>=0){
					*m_pLabels[i]->slabel=dPart.m_strName;
					*m_pLabels[i]->rect=CRect(m_pLabels[i]->rect->TopLeft(),CSize(0,0));
					break;
				}
			}
			//====================================
			if(i==m_LabelsCount){
				m_strIdent=dPart.m_strName;
				m_bVertical=dPart.m_bVertical;
				m_sFont=dPart.m_sFont;
				m_iSize=dPart.m_iSize;
			}
		}
	}
}

BOOL CShapeUnit::IsTB(){
	CString strFile = m_sUnitName;
	//get libray name
	CString strLib = strFile.Left(strFile.Find('.'));
	//remove library name
	strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

	//Check if part is of type TB
	CString str=strFile;

	int nIndex=str.Find('.');

	if(nIndex>=0){
		str=str.Left(nIndex);
	}

	if(str.CompareNoCase("TB")==0){
		return TRUE;
	}

	return FALSE;
}

//Drawing code
void CShapeUnit::DoDraw(CDC *pDC, CRect rect1){

	//Resize while drawing shape
	if(m_Mode==_DRAFTDRAW_MODE_SEL&&!m_bNoResize){
		Resize();
	}

    //---------------------------------------
    //|                                     |
    //|                                     |
    //|                                     |   b
    //|                                     |
    //|                                     |
    //---------------------------------------
	//
	//<------------------- a --------------->

	//Erase rect
	//	pDC->FillSolidRect(&m_Rect,RGB(255,255,255));

	//Draw text
	int i;
	for(i=0; i<m_LabelsCount;i++){

		//Resize label to fit text
		if(m_pLabels[i]->rect->IsRectEmpty()){

			//Save previous font
			CFont *prev_font;
			//Check if label font is null
			if(m_pLabels[i]->font==NULL){
				if(m_pLabels[i]->iSize<=10){
					prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));
				}
				else{
					prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_NORMAL));
				}
			}
			else{
				//If it is not null select font in shape
				prev_font=pDC->SelectObject(m_pLabels[i]->font);
			}

			//Check if it is a vertical label
			if(m_pLabels[i]->bver){

				CString stlabel="";
				CSize szac=CSize(0,0);

				for(int j=0;j<m_pLabels[i]->slabel->GetLength();j++){
					
					//Add line breaks except for 1st character
					if(j)stlabel+="\n";
					stlabel+=m_pLabels[i]->slabel[0][j];

					//Get space occupied by single character
					CString stlabel1=m_pLabels[i]->slabel[0][j];
					SIZE sz;
					GetTextExtentPoint32(pDC->m_hDC, stlabel1, lstrlen(stlabel1), &sz);

					//Accumulate total occupied space
					szac+=CSize(0,sz.cy);
					if(szac.cx<sz.cx){
						szac.cx=sz.cx;
					}
				}

				*m_pLabels[i]->slabel=stlabel;
				*m_pLabels[i]->rect=CRect(m_pLabels[i]->rect->TopLeft(),szac);
			}
			else{

				SIZE sz;
				GetTextExtentPoint32(pDC->m_hDC, *m_pLabels[i]->slabel, lstrlen(*m_pLabels[i]->slabel), &sz);
				*m_pLabels[i]->rect=CRect(m_pLabels[i]->rect->TopLeft(),sz);
			}

			pDC->SelectObject(prev_font);
		}

		//debug
		//pDC->FrameRect(m_pLabels[i]->rect,&CBrush(RGB(255,0,0)));

		CString stlabel="";

		//redundant block?
		if(m_pLabels[i]->bver){
			stlabel=*m_pLabels[i]->slabel;
		}
		else{
			stlabel=*m_pLabels[i]->slabel;
		}

		if(m_pLabels[i]->style&WS_VISIBLE){
			CFont *prev_font;
			if(m_pLabels[i]->font==NULL){
				if(m_pLabels[i]->iSize<=10){
					prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));
				}
				else{
					prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_NORMAL));
				}
			}
			else{
				prev_font=pDC->SelectObject(m_pLabels[i]->font);
			}

			pDC->DrawText( stlabel,*m_pLabels[i]->rect+rect1.TopLeft(), DT_CENTER);
			pDC->SelectObject(prev_font);
		}

	}//end draw text

	//Draw pin
	for(i=0;i<m_obarrShapearr.GetSize();i++){

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(i);

		//Save original rectangle
		CRect rect=psh->m_Rect;

		if(psh->IsKindOf(RUNTIME_CLASS(CShapePin))){
			//This only a patch to avoid leaving things back when dragging
			psh->m_Rect+=m_Rect.TopLeft();
		}
		else{
			psh->m_Rect+=rect1.TopLeft();
		}

		psh->OnDraw(pDC);

		//Restore original rectangle
		psh->m_Rect=rect;
	}

	//Draw lines
	for(i=0; i<m_PointspCount;i++){

		//Get pointer to polylines buffer
		LPPOINT lpPoints=( LPPOINT)&m_pPoints[i][1];
		int nCount= m_pPoints[i][0];

		//Iterate polylines
		for(int j=0;j<nCount;j++){

			m_pPBuffer[j]=CPoint(lpPoints[j])+rect1.TopLeft();
		}

		//Draw polyline using MFC
		pDC->Polyline(m_pPBuffer, nCount);
	}
}

void CShapeUnit::OnDraw(CDC *pDC){

	//2014-09-25
	//CShapeUnit is not calling its base case CShapeContainer for drawing because is overlapping some
	//of its functionality (mainly polyline implementation) and should be solved

	//Call base class for drawing sizing handles
	if(m_Mode==_DRAFTDRAW_MODE_SEL){

		//Redraw shape in current place
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			DoDraw(pDC,m_RectLast);
		}

		//Draw shape while dragging
		DoDraw(pDC,m_Rect);

		//To draw selection handlers
		CShape::OnDraw(pDC);
	}
	else if(!m_Rect.IsRectEmpty()){

		//Normal draw
		DoDraw(pDC,m_Rect);

		//To draw selection handlers
		CShape::OnDraw(pDC);
	}
}

void CShapeUnit::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShapeContainer::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
        //archive << m_name << m_number;
		
		archive << m_sUnitName;

		//Calcule count

		//27/03/2005
		/*
		int nCounterNoCont=0;
		for(int i=0;i<m_obarrShapearr.GetSize();i++){
			CShape *pSh=(CShape *)m_obarrShapearr[i];
			if(!m_obarrShapearr[i]->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
				nCounterNoCont++;
			}
		}
		*/


		int nCount=m_PointspCount+/*m_obarrShapearr.GetSize()+*/m_LabelsCount/*+1*//*frm rectangle*/;
		if(nCount||m_obarrShapearr.GetSize())//!!! nCount=0 => Part edit!
		archive<<nCount;
		//archive<<&CShapeFrmRect(m_Rect);
		//CObArray aShPoly;
		//aShPoly.SetSize(m_PointspCount);

		if(m_bNoResize){
			g_aShBuffer.SetSize(g_aShBuffer.GetSize()+m_PointspCount+m_LabelsCount);
		}
		//24/01/2005
		else{
			g_aShBuffer.SetSize(g_aShBuffer.GetSize()+2*m_PointspCount+m_LabelsCount);
		}


		int indexpline;
		for(indexpline=0;indexpline<m_PointspCount;indexpline++){
			g_aShBuffer[g_aShBufIndex+indexpline]=new CShapePolyline();
			int idata=m_pPoints[indexpline][0];
			((CShapePolyline*)g_aShBuffer[g_aShBufIndex+indexpline])->Create((LPPOINT)&m_pPoints[indexpline][1],idata);
			archive<<g_aShBuffer[g_aShBufIndex+indexpline];
		}
		g_aShBufIndex+=indexpline;

		//CObArray aShLabel;
		//aShLabel.SetSize(m_LabelsCount);
		int indexlabel;
		for(indexlabel=0;indexlabel<m_LabelsCount;indexlabel++){
			label lbl=*m_pLabels[indexlabel];
			//*lbl.rect=CRect((CRect(lbl.rect)-m_Rect.TopLeft()).TopLeft(),CSize(0,0));
			g_aShBuffer[g_aShBufIndex+indexlabel]=new CShapeLabel(&lbl);
			archive<<g_aShBuffer[g_aShBufIndex+indexlabel];
		}
		g_aShBufIndex+=indexlabel;
		//Clean up.
		/*
		for(indexpline=0;indexpline<m_PointspCount;indexpline++){
			delete(aShPoly[indexpline]);
		}
		for(indexlabel=0;indexlabel<m_LabelsCount;indexlabel++){
			delete(aShLabel[indexlabel]);
		}
		*/

		//24/03/2004
		//From second version on a secondary buffer will be stored
		if(!m_bNoResize){
			int nCount=m_PointspCount/*+1*//*frm rectangle*/;
			if(nCount)//!!!
			archive<<nCount;

			for(indexpline=0;indexpline<m_PointspCount;indexpline++){
				g_aShBuffer[g_aShBufIndex+indexpline]=new CShapePolyline();
				int idata=m_pPoints0[indexpline][0];
				((CShapePolyline*)g_aShBuffer[g_aShBufIndex+indexpline])->Create((LPPOINT)&m_pPoints0[indexpline][1],idata);
				archive<<g_aShBuffer[g_aShBufIndex+indexpline];
			}
			g_aShBufIndex+=indexpline;
		}
	}
    else{

		archive >> m_sUnitName;

		//allocate memory
		m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
		m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);

		int indexpline=0;
		int indexlabel=0;

		//loop
		int nCount=0;
		if(!m_bFlagPartEdit)
		archive>>nCount;
		//archive>>nCount;
		for(int i=0;i<nCount;i++){

			CShape *pSh;
			archive >> pSh;

			if(pSh->IsKindOf(RUNTIME_CLASS(CShapePolyline))){
				CShapePolyline *pShPoly=(CShapePolyline *)pSh;
				int idata=pShPoly->GetSize();
				m_pPoints[indexpline]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
				m_pPoints[indexpline][0]=idata;
				pShPoly->GetPoints((LPPOINT)&m_pPoints[indexpline][1],idata);
				indexpline++;
			}
			else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeEllipse))){
				m_obarrShapearr.Add(pSh);
			}
			else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){
				//m_Rect=pSh->m_Rect;
				//07/01/2006
				//When CShapeUnit's are placed into schematics frame rectangle
				//must have the origin at 0,0
				m_Rect=CRect(CPoint(0,0),pSh->m_Rect.Size());
			}
			else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){
				m_pLabels[indexlabel]=new label( ((CShapeLabel *)pSh)->m_Label );
				indexlabel++;
			}
			else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
				((CShapeContainer*)pSh)->m_pShParent=this;
				//27/03/2005
				//Moved to parent class: CShapeContainer
				//Has been leaved here for compatibilty with parts in libraries.
				m_obarrShapearr.Add(pSh);
			}
			else{
				//27/03/2005
				//Moved to parent class: CShapeContainer
				//Has been leaved here for compatibilty with parts in libraries.
				m_obarrShapearr.Add(pSh);
			}
		}//loop end

		m_PointspCount=indexpline;
		m_LabelsCount=indexlabel;
		m_ShapeNumber=m_obarrShapearr.GetSize();

		//1.04
		//Flag m_bNoResize is serialized from version 1.04 on.
		//If tiny shapes are serialized filling buffers here can affect scalling.
		//It would be better serializing entire scalling buffer!!!
		//1.04
		float fVer0=1.04;
		float fData=m_fVer-fVer0;
		if(fData>=0){
			if(!m_bNoResize){
				
				m_pPoints0=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
				int indexpline=0;
				int nCount=0;
				if(!m_bFlagPartEdit)
				archive>>nCount;
				//archive>>nCount;
				for(int i=0;i<nCount;i++){


					CShape *pSh;
					archive >> pSh;

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapePolyline))){
						CShapePolyline *pShPoly=(CShapePolyline *)pSh;
						int idata=pShPoly->GetSize();
						m_pPoints0[indexpline]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
						m_pPoints0[indexpline][0]=idata;
						pShPoly->GetPoints((LPPOINT)&m_pPoints0[indexpline][1],idata);
						indexpline++;
					}

				}
			}
		}//end if version >= 1.04

	}//end else IsStorimg

}

void CShapeUnit::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeUnit::SerializeSvg(CSvgioFile &svgfile)
{
	CShapeContainer::SerializeSvg(svgfile);

	if (m_bNoResize){
		g_aShBuffer.SetSize(g_aShBuffer.GetSize() + m_PointspCount + m_LabelsCount);
	}
	else{
		g_aShBuffer.SetSize(g_aShBuffer.GetSize() + 2 * m_PointspCount + m_LabelsCount);
	}

	int indexpline;
	for (indexpline = 0; indexpline<m_PointspCount; indexpline++){
		g_aShBuffer[g_aShBufIndex + indexpline] = new CShapePolyline();
		int idata = m_pPoints[indexpline][0];
		((CShapePolyline*)g_aShBuffer[g_aShBufIndex + indexpline])->Create((LPPOINT)&m_pPoints[indexpline][1], idata);

		//Save previous bounding rectangle
		CRect rect_prev = ((CShapePolyline*)g_aShBuffer[g_aShBufIndex + indexpline])->m_Rect;
		//Offset shape with parent
		((CShapePolyline*)g_aShBuffer[g_aShBufIndex + indexpline])->m_Rect += this->m_Rect.TopLeft();
		//Serialize
		((CShapePolyline*)g_aShBuffer[g_aShBufIndex + indexpline])->SerializeSvg(svgfile);
		//Restore previous rectangle
		((CShapePolyline*)g_aShBuffer[g_aShBufIndex + indexpline])->m_Rect = rect_prev;
	}
	g_aShBufIndex += indexpline;

	int indexlabel;
	for (indexlabel = 0; indexlabel<m_LabelsCount; indexlabel++){
		label lbl = *m_pLabels[indexlabel];
		g_aShBuffer[g_aShBufIndex + indexlabel] = new CShapeLabel(&lbl);

		//Save previous bounding rectangle
		CRect rect_prev = ((CShapeLabel*)g_aShBuffer[g_aShBufIndex + indexlabel])->m_Label.rect;
		//Offset shape with parent
		*((CShapeLabel*)g_aShBuffer[g_aShBufIndex + indexlabel])->m_Label.rect += this->m_Rect.TopLeft();
		//Serialize
		((CShapeLabel*)g_aShBuffer[g_aShBufIndex + indexlabel])->SerializeSvg(svgfile);
		//Restore previous rectangle
		*((CShapeLabel*)g_aShBuffer[g_aShBufIndex + indexlabel])->m_Label.rect = rect_prev;
	}
	g_aShBufIndex += indexlabel;
}

void CShapeUnit::SerializeDdw(CDdwioFile &ddwfile)
{
	ddwfile.WriteUnit(m_sUnitName,&m_Rect);
}

void CShapeUnit::Resize(){
	//return;
	if(m_Rect!=m_Rect0){
		CRect rectTemp=m_Rect;
		//rectTemp.NormalizeRect();
		//debug delete
		if(rectTemp!=m_Rect){
			int i=0;
		}
		float scale_x=rectTemp.Width();
		scale_x/=m_Rect0.Width();
		float scale_y=rectTemp.Height();
		scale_y/=m_Rect0.Height();
		for(int j=0;j<m_PointspCount;j++){
			for(int i=1;i<m_pPoints[j][0]*2+1;i+=2){
				float fdata_x=m_pPoints0[j][i];
				fdata_x*=scale_x;
				float fdata_y=m_pPoints0[j][i+1];
				fdata_y*=scale_y;
				m_pPoints[j][i]=fdata_x;
				m_pPoints[j][i+1]=fdata_y;
			}
		}
	}
}

//Create buffers for rescale
void CShapeUnit::SetupBuffers(LPRECT rect0){
	m_Rect0=rect0;
	m_pPoints0=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*m_PointspCount);
	for(int i=0;i<m_PointspCount;i++){
		int idata=m_pPoints[i][0];
		m_pPoints0[i]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
		memcpy(m_pPoints0[i],m_pPoints[i],(2*idata+1)*sizeof(int));
	}
}

//Release rescale buffer
void CShapeUnit::ReleaseBuffers(){
	for(int i=0;i<m_PointspCount;i++){
		GlobalFree(m_pPoints0[i]);
	}
	GlobalFree(m_pPoints0);
}

BOOL CShapeUnit::OnCommand( WPARAM wParam, LPARAM lParam ){

	switch(LOWORD(wParam)){
	case ID_EDIT_PROPERTIES:
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){

			if(!g_db.IsOpen()){
				TCHAR sConnect[1024];
				g_GetConnectString(sConnect,1024);
				//Connect to database
				g_db.OpenEx(sConnect);
			}

			//Delete contents of temporal table
			g_db.ExecuteSQL("DELETE FROM tbPartTemp");

			//Insert properties in db
			CString str;
			CString strSQL="INSERT INTO tbPartTemp (cNombre,cValor) VALUES ('%s','%i')";


			str.Format(strSQL,"ID",m_uiShapeId);
			g_db.ExecuteSQL(str);
			str.Format(strSQL,"Width0",m_Rect0.Width());
			g_db.ExecuteSQL(str);
			str.Format(strSQL,"Height0",m_Rect0.Height());
			g_db.ExecuteSQL(str);
			str.Format(strSQL,"Width",m_Rect.Width());
			g_db.ExecuteSQL(str);
			str.Format(strSQL,"Height",m_Rect.Height());
			g_db.ExecuteSQL(str);
#define DRAFTCABLE_PARTPROP_PROGRAMATICALLY			5

			int i;
			for(i=0;i<m_LabelsCount;i++){
				str=*m_pLabels[i]->sname;
				str.MakeUpper();
				if(0){//(str.Find("TITLE")>=0){
					strSQL="INSERT INTO tbPartTemp (cNombre,cValor) VALUES ('%s','%s')";
					str.Format(strSQL,"Title",*m_pLabels[i]->slabel);
					g_db.ExecuteSQL(str);
					//break;
				}
				else if(str.GetLength()>0){
					CString str1;
					strSQL="INSERT INTO tbPartTemp (cNombre,cValor) VALUES ('%s','%s')";
					str1.Format(strSQL,str,*m_pLabels[i]->slabel);
					g_db.ExecuteSQL(str1);
				}
			}

			/*
			CRecordset rsPartTemp(&db);
			rsPartTemp.m_strFilter="bRack<>0";
			rsPartTemp.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");
			*/

			g_db.Close();

			CDialogPartProp dialog;
			/*dialog.m_sText.Format("Id=%i",m_uiShapeId);*/
			if(dialog.DoModal()==IDOK){

				if(!g_db.IsOpen()){
					TCHAR sConnect[1024];
					g_GetConnectString(sConnect,1024);
					//Connect to database
					g_db.OpenEx(sConnect);
				}

				strSQL="SELECT cNombre,cValor FROM tbPartTemp ORDER BY iId";
				CRecordset rsTemp(&g_db);

				rsTemp.Open(CRecordset::forwardOnly,strSQL);

				int nCounter=0;
				i=0;
				bool bFlagModified=false;
				while(!rsTemp.IsBOF()&&!rsTemp.IsEOF()){

					if(nCounter>=DRAFTCABLE_PARTPROP_PROGRAMATICALLY&&i<m_LabelsCount){
						///local var
						CString strName,strValue;
						str=*m_pLabels[i]->sname;
						str.MakeUpper();

						rsTemp.GetFieldValue("cNombre",strName);

						if(str.Compare(strName)==0){
							str=*m_pLabels[i]->slabel;
							rsTemp.GetFieldValue("cValor",strValue);
							if(str.Compare(strValue)){
								*m_pLabels[i]->slabel=strValue;
								*m_pLabels[i]->rect=CRect(m_pLabels[i]->rect->TopLeft(),CSize(0,0));

								//Process parameters
								if(strName.Compare("NSIDES")==0){
									bFlagModified=true;
								}
								else if(strName.Compare("DEEP")==0){
									bFlagModified=true;
								}

							}
						}

						i++;
					}

					rsTemp.MoveNext();
					nCounter++;
				}

				if(bFlagModified){

					int idata,idata1;

					for(int i=0;i<m_LabelsCount;i++){
						CString str=*m_pLabels[i]->sname;
						str.MakeUpper();
						if(str.Compare("NSIDES")==0){

							idata=atoi(*m_pLabels[i]->slabel);
						}
						else if(str.Compare("DEEP")==0){

							idata1=atoi(*m_pLabels[i]->slabel);
						}
					}

					switch(m_uiShapeType){
					case ddcShapeNPolygon:
						{

						//do some cleaning
						ReleaseBuffers();
						GlobalFree(m_pPoints[0]);
						//allocate new space
						m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+2));
						//recalculate shape
						m_pPoints[0][0]=idata+1;
						m_PointspCount=1;
						CShapePolyline ShPoly;
						ShPoly.Create(CRect(0,0,150,150),idata);
						ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],idata+1);
						//allocate new space
						SetupBuffers(CRect(0,0,150,150));
						Resize();
						}break;
					case ddcShapeNCross:
						{

						//do some cleaning
						ReleaseBuffers();
						GlobalFree(m_pPoints[0]);
						//allocate new space
						m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(3*idata+2));
						//recalculate shape
						m_pPoints[0][0]=3*idata+1;
						m_PointspCount=1;
						CShapePolyline ShPoly;
						ShPoly.CreateNCross(CRect(0,0,150,150),idata,idata1);
						ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],3*idata+1);
						//allocate new space
						SetupBuffers(CRect(0,0,150,150));
						Resize();
						}break;
					case ddcShapeNGear:
						{

						//do some cleaning
						ReleaseBuffers();
						GlobalFree(m_pPoints[0]);
						//allocate new space
						m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(4*idata+2));
						//recalculate shape
						m_pPoints[0][0]=4*idata+1;
						m_PointspCount=1;
						CShapePolyline ShPoly;
						ShPoly.CreateNGear(CRect(0,0,150,150),idata,idata1);
						ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],4*idata+1);
						//allocate new space
						SetupBuffers(CRect(0,0,150,150));
						Resize();
						}break;
					case ddcShapeNStar:
						{

						//do some cleaning
						ReleaseBuffers();
						GlobalFree(m_pPoints[0]);
						//allocate new space
						m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(2*idata+2));
						//recalculate shape
						m_pPoints[0][0]=2*idata+1;
						m_PointspCount=1;
						CShapePolyline ShPoly;
						ShPoly.CreateNStar(CRect(0,0,150,150),idata,idata1);
						ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],2*idata+1);
						//allocate new space
						SetupBuffers(CRect(0,0,150,150));
						Resize();
						}break;

					}

				}
			}

			return TRUE;
		}
		break;
	}

	return CShapeContainer::OnCommand( wParam, lParam );
	//return FALSE;
}

BOOL CShapeUnit::CreateNGear(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CShapePolyline ShPoly;
	int idata=nCount;
	int idata1=percDeep;
	int uiShapeType=ddcShapeNGear;

	m_bNoResize=FALSE;

	m_uiShapeType=uiShapeType;
	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	CString str;str.Format("%i",idata);
	label *lbl=new label(str,0,CRect(0,0,0,0),"NSIDES");
	m_pLabels[0]=lbl;
	m_PointspCount=1;

	str.Format("%i",idata1);
	lbl=new label(str,0,CRect(0,20,0,0),"DEEP");
	m_pLabels[1]=lbl;
	m_LabelsCount=2;

	m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(4*idata+2));//storage per number of vertex+1 to close the polygon
	m_pPoints[0][0]=4*idata+1;
	BOOL result=ShPoly.CreateNGear(CRect(0,0,150,150),idata,idata1);
	ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],4*idata+1);
	return result;
}

BOOL CShapeUnit::CreateNCross(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CShapePolyline ShPoly;
	int idata=nCount;
	int idata1=percDeep;
	int uiShapeType=ddcShapeNCross;

	m_bNoResize=FALSE;

	m_uiShapeType=uiShapeType;
	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	CString str;str.Format("%i",idata);
	label *lbl=new label(str,0,CRect(0,0,0,0),"NSIDES");
	m_pLabels[0]=lbl;
	m_PointspCount=1;

	str.Format("%i",idata1);
	lbl=new label(str,0,CRect(0,20,0,0),"DEEP");
	m_pLabels[1]=lbl;
	m_LabelsCount=2;

	m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(3*idata+2));//storage per number of vertex+1 to close the polygon
	m_pPoints[0][0]=3*idata+1;
	BOOL result=ShPoly.CreateNCross(CRect(0,0,150,150),idata,idata1);
	ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],3*idata+1);
	return result;
}

BOOL CShapeUnit::CreateNStar(LPRECT lpRect,int nCount,float percDeep /*=25.0*/){

	CShapePolyline ShPoly;
	int idata=nCount;
	int idata1=percDeep;
	int uiShapeType=ddcShapeNStar;

	m_bNoResize=FALSE;

	m_uiShapeType=uiShapeType;
	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	CString str;str.Format("%i",idata);
	label *lbl=new label(str,0,CRect(0,0,0,0),"NSIDES");
	m_pLabels[0]=lbl;
	
	m_PointspCount=1;


	str.Format("%i",idata1);
	lbl=new label("15",0,CRect(0,20,0,0),"DEEP");
	m_pLabels[1]=lbl;
	m_LabelsCount=2;

	m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(2*idata+2));//storage per number of vertex+1 to close the polygon
	m_pPoints[0][0]=2*idata+1;
	BOOL result=ShPoly.CreateNStar(CRect(0,0,150,150),idata,idata1);
	ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],2*idata+1);
	return result;
}

BOOL CShapeUnit::Create(LPRECT lpRect,int nCount,int uiShapeType){

	CShapePolyline ShPoly;
	int idata=nCount;

	m_uiShapeType=uiShapeType;
	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	CString str;str.Format("%i",idata);
	label *lbl=new label(str,0,CRect(0,0,0,0),"NSIDES");
	m_pLabels[0]=lbl;
	
	m_PointspCount=1;


	m_LabelsCount=1;

	m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+2));//storage per number of vertex+1 to close the polygon
	m_pPoints[0][0]=idata+1;
	BOOL result=ShPoly.Create(CRect(0,0,150,150),idata);//ShPoly.Create(CRect(100,100,250,250),idata);
	ShPoly.GetPoints((LPPOINT)&m_pPoints[0][1],idata+1);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CShapePin
/*IMPLEMENT_DYNCREATE(CShapePin, CShapeContainer)*/
IMPLEMENT_SERIAL(CShapePin, CShapeContainer, 1)

CShapePin::CShapePin()
{
	//1st
	//Flags,counters,rects
	m_bNoResize=TRUE;
	m_bFixed=FALSE;
	m_bJack=FALSE;
	m_ShapeNumber=1;
	m_rectPin=CRect(CPoint(0,0),CSize(20,20));
	m_Rect=CRect(CPoint(0,0),CSize(20,20));

	//2nd
	m_szSize=CSize(5,5);

	//3rd
	m_bConect=FALSE;
	m_TypePin=-1;

}

CShapePin::CShapePin(UINT uiPinnumber,UINT uiPos,DWORD dwStyle,cmddeque *cmddq /*=NULL*/):CShapeContainer(0,0,cmddq)
{
	//1st
	//Flags,counters,basic rects
	m_bNoResize=TRUE;
	m_bFixed=FALSE;
	m_bJack=FALSE;
	m_ShapeNumber=1;
	m_rectPin=CRect(CPoint(0,0),CSize(20,20));
	m_Rect=CRect(CPoint(0,0),CSize(20,20));

	//2nd
	m_uiPinnumber=uiPinnumber;
	m_uiPos=uiPos;
	m_szSize=CSize(5,5);
	m_dwStyle=dwStyle;
	if(m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
		m_strIdent.Format("J%i",m_uiPinnumber);
	}
	else{
		m_strIdent.Format("P%i",m_uiPinnumber);
	}

	//3rd
	m_bConect=FALSE;

	//4th
	m_fontFont1.CreateFont(
		8,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                  // lpszFacename

}

//Copy constructor
CShapePin::CShapePin(CShapePin& ShPin):CShapeContainer(ShPin){

	//1st
	//Flags,counters,basic rects
	m_bNoResize=TRUE;
	m_bFixed=FALSE;
	m_bJack=FALSE;
	m_ShapeNumber=1;
	m_rectPin=ShPin.m_rectPin;//CRect(CPoint(0,0),CSize(20,20));
	m_Rect=ShPin.m_Rect;//CRect(CPoint(0,0),CSize(20,20));

	//2nd
	m_uiPinnumber=ShPin.m_uiPinnumber;
	m_uiPos=ShPin.m_uiPos;
	m_szSize=CSize(5,5);
	m_dwStyle=ShPin.m_dwStyle;
	if(m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
		m_strIdent.Format("J%i",m_uiPinnumber);
	}
	else{
		m_strIdent.Format("P%i",m_uiPinnumber);
	}

	//3rd
	m_bConect=FALSE;

	//4th
	m_fontFont1.CreateFont(
		8,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                  // lpszFacename
	m_TypePin = ShPin.m_TypePin;
}

CShapePin::~CShapePin()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapePin implementation

CShape& CShapePin::operator=( const CShape& Sh ){

	CRuntimeClass* prt = Sh.GetRuntimeClass();
	ASSERT( strcmp( prt->m_lpszClassName, "CShapePin" )  == 0 );

	CShapePin *pShPin=(CShapePin *)&Sh;

	//1st
	//Flags,counters,rects
	m_rectPin=pShPin->m_rectPin;
	//03/01/2005
	m_Rect=pShPin->m_Rect;

	//2nd
	m_uiPinnumber=pShPin->m_uiPinnumber;
	m_uiPos=pShPin->m_uiPos;
	m_dwStyle=pShPin->m_dwStyle;
	if(m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
		m_strIdent.Format("J%i",m_uiPinnumber);
	}
	else{
		m_strIdent.Format("P%i",m_uiPinnumber);
	}

	//3rd
	m_bConect=pShPin->m_bConect;

	//4th
	m_TypePin=pShPin->m_TypePin;

	return *this;
}

CShape& CShapePin::operator++( ){
	m_uiPinnumber++;
	if(m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
		m_strIdent.Format("J%i",m_uiPinnumber);
	}
	else{
		m_strIdent.Format("P%i",m_uiPinnumber);
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CShapePin message handlers

BOOL CShapePin::Disconnect(CPoint pointr,BOOL bDiscon /*=FALSE*/){
	//First implementation disconnection ocurs if dragging in x and y simul
	if((!pointr.x)&&(!pointr.y)){
		return FALSE;
	}
	else if(bDiscon||((pointr.x!=0)&&(pointr.y!=0))){
		m_bConect=FALSE;
		m_pshConn=NULL;
		m_pshChildConn=NULL;
		return TRUE;
	}
	else if((!pointr.x)&&(!GetParent()->m_Rect.Width())){
		if( ((CShapeContainer*)GetParent())->m_obarrShapearr[0]==this ){
			if(GetParent()->m_Rect.top>=GetParent()->m_Rect.bottom){
				CRect *pRect=&GetParent()->m_Rect;
				CRect rect1(0,-pointr.y,0,0);
				*pRect+=rect1;//inflate rect
			}
			else{
				CRect *pRect=&GetParent()->m_Rect;
				CRect rect1(0,-pointr.y,0,0);
				*pRect+=rect1;//inflate rect
			}
		}
		else{
			if(GetParent()->m_Rect.top>=GetParent()->m_Rect.bottom){
				CRect *pRect=&GetParent()->m_Rect;
				CRect rect1(0,0,0,+pointr.y);
				*pRect+=rect1;//inflate rect
			}
			else{
				CRect *pRect=&GetParent()->m_Rect;
				CRect rect1(0,0,0,+pointr.y);
				*pRect+=rect1;//inflate rect
			}
		}
		return FALSE;
	}
	else if((!pointr.y)&&(!GetParent()->m_Rect.Height())){
		if( ((CShapeContainer*)GetParent())->m_obarrShapearr[0]==this ){
			if(GetParent()->m_Rect.right>=GetParent()->m_Rect.left){
				GetParent()->m_Rect+=CRect(-pointr.x,0,0,0);	//inflate rect

			}
			else{
				GetParent()->m_Rect+=CRect(-pointr.x,0,0,0);	//inflate rect
			}
		}
		else{
			if(GetParent()->m_Rect.right>=GetParent()->m_Rect.left){
				GetParent()->m_Rect+=CRect(0,0,+pointr.x,0);	//inflate rect

			}
			else{
				GetParent()->m_Rect+=CRect(0,0,+pointr.x,0);	//inflate rect
			}
		}
		return FALSE;
	}
	else{
		m_pshConn=NULL;
		m_pshChildConn=NULL;
		m_bConect=FALSE;
		return TRUE;
	}
}

void CShapePin::RotatePin(){

	switch(m_uiPos){
	case _DRAFTDRAW_SEL_RESIZING_RECT_N:
		m_uiPos=_DRAFTDRAW_SEL_RESIZING_RECT_W;
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_E:
		m_uiPos=_DRAFTDRAW_SEL_RESIZING_RECT_N;
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_S:
		m_uiPos=_DRAFTDRAW_SEL_RESIZING_RECT_E;
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_W:
		m_uiPos=_DRAFTDRAW_SEL_RESIZING_RECT_S;
		break;
	}
}

BOOL CShapePin::OnCommand( WPARAM wParam, LPARAM lParam ){

	switch(LOWORD(wParam)){
	case ID_ROTATE:
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			RotatePin();
			return TRUE;
		}
		break;
	}

	return FALSE;
}

void CShapePin::OnDraw(CDC *pDC){

	CShape::OnDraw(pDC);

	if(!m_Rect.IsRectEmpty()){

		int offset_x;
		int offset_y;
		CFont *def_font;
		CString strlabel1;
		CString strlabel2;
		CPoint point;
		CRect rect=m_rectPin+m_Rect.TopLeft();
		CRect rect1;
		CRect rect2;


		switch(m_dwStyle&SHAPEUNIT_PINTYPE_MASK){
		case PIN_UNIT://unit pins

			//Create rectangles and labels needed for drawing
			_DoCreateRectUnitPin(point, rect1, rect2, strlabel1, strlabel2);

			//border rectangle
			pDC->Rectangle(rect);

			/*
			switch(m_uiPos){
			case _DRAFTDRAW_SEL_RESIZING_RECT_N:
			case _DRAFTDRAW_SEL_RESIZING_RECT_S:
				point=rect.TopLeft()+CPoint(0,rect.Height()*0.5);
				rect1=CRect(rect.TopLeft(),point+CPoint(rect.Width(),0));
				rect1.DeflateRect(0,rect1.Height()*0.10);
				rect2=CRect(point,rect.BottomRight());
				rect2.DeflateRect(0,rect2.Height()*0.10);
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_E:
			case _DRAFTDRAW_SEL_RESIZING_RECT_W:
				point=rect.TopLeft()+CPoint(rect.Width()*0.5,0);
				rect1=CRect(rect.TopLeft(),point+CPoint(0,rect.Height()));
				rect1.DeflateRect(rect1.Width()*0.10,rect1.Height()*0.10);
				rect2=CRect(point,rect.BottomRight());
				rect2.DeflateRect(rect2.Width()*0.10,rect2.Height()*0.10);
				break;
			}

			//label
			switch(m_uiPos){
			case _DRAFTDRAW_SEL_RESIZING_RECT_E:
			case _DRAFTDRAW_SEL_RESIZING_RECT_S:
				strlabel1.Format("J%i",m_uiPinnumber);
				strlabel2.Format("P%i",m_uiPinnumber);
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_W:
			case _DRAFTDRAW_SEL_RESIZING_RECT_N:
				strlabel2.Format("J%i",m_uiPinnumber);
				strlabel1.Format("P%i",m_uiPinnumber);
				break;
			}
				

			if(m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
				CString str=strlabel1;
				strlabel1=strlabel2;
				strlabel2=str;
			}
			*/

			//Draw pin

			def_font=pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));

			pDC->DrawText(strlabel1,rect1,DT_CENTER);
			pDC->DrawText(strlabel2,rect2,DT_CENTER);

			pDC->SelectObject(def_font);

			//divisory line
			switch(m_uiPos){
			case _DRAFTDRAW_SEL_RESIZING_RECT_N:
			case _DRAFTDRAW_SEL_RESIZING_RECT_S:
				pDC->MoveTo(point);
				pDC->LineTo(point+CPoint(rect.Width(),0));
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_E:
			case _DRAFTDRAW_SEL_RESIZING_RECT_W:
				pDC->MoveTo(point);
				pDC->LineTo(point+CPoint(0,rect.Height()));
				break;
			}
			//conecting point

			offset_x=rect.Width()*1.0;
			offset_y=rect.Height()*1.0;

			switch(m_uiPos){

			case _DRAFTDRAW_SEL_RESIZING_RECT_N:
				offset_x=0;
				offset_y=-offset_y;
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_E:
				offset_y=0;
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_S:
				offset_x=0;
				break;
			case _DRAFTDRAW_SEL_RESIZING_RECT_W:
				offset_x=-offset_x;
				offset_y=0;
				break;
			}

			//debuggin
			//******************************************************************
			/*
			{
			CString str;
			CPoint point=rect.CenterPoint()+CPoint(2*offset_x,2*offset_y)-CPoint(m_szSize.cx*.5,m_szSize.cy*.5);
			str.Format("%02i",m_uiPinnumber);
			pDC->TextOut(point.x,point.y,str);
			}
			*/
			//******************************************************************

			//rectangle of pin(absolute offset allready added) + connecting pin offset + 1/2 connection point size
			point=rect.CenterPoint()+CPoint(offset_x,offset_y)-CPoint(m_szSize.cx*.5,m_szSize.cy*.5);

			m_rectConect=CRect(point,m_szSize)  -GetParent()->m_Rect.TopLeft();

			if(!m_bConect){
				pDC->Rectangle(m_rectConect  +GetParent()->m_Rect.TopLeft());
			}

		break;

		case PIN_WIRE://wire pins

			//debuggin
			//******************************************************************
			/*
			{
			CString str;
			CPoint point=rect.TopLeft()+CPoint(m_szSize.cx*.5,m_szSize.cy*.5);
			str.Format("%02i",m_uiPinnumber);
			pDC->TextOut(point.x,point.y,str);
			}
			*/
			//******************************************************************
			

			point=rect.TopLeft()-CPoint(m_szSize.cx*.5,m_szSize.cy*.5);
			m_rectConect=CRect(point,m_szSize)  -GetParent()->m_Rect.TopLeft();

			if(!m_bConect){
				pDC->Rectangle(m_rectConect  +GetParent()->m_Rect.TopLeft());
			}

		break;


		}//end switch style

		//to draw selections
		CShape::OnDraw(pDC);

	}//rect is not empty

}

void CShapePin::OnMouseMove(UINT nFlags, CPoint point){
	CShape *pSh=m_pshConn;
	CShapePin *pSh1=(CShapePin *)m_pshChildConn;
	CShape *pShparent=GetParent();
	if(pShparent)
	switch(pShparent->m_TypeSelect){
	case  _DRAFTDRAW_SEL_MOVING_RECT:
		if((pSh1)&&(m_bConect)){
			if(pSh1->Disconnect(point)){
				//This allows wire streching
				m_pshConn=NULL;
				m_pshChildConn=NULL;
				m_bConect=FALSE;//disconnect if other side disconnect too
			}
		}
		else{
			m_pshConn=NULL;
			m_pshChildConn=NULL;
			m_bConect=FALSE;
		}
		break;
	}
	CShapeContainer::OnMouseMove(nFlags,point);
}

void CShapePin::OnLButtonDown(UINT nFlags, CPoint point){


	if((m_Mode)&&(m_rectConect.PtInRect(CPoint(point  -GetParent()->m_Rect.TopLeft()) ))
		//10/12/2004
		//To prevent pins to get connected when part edition mode is active.
		&&GetParent()!=this){

		m_bConect=true;
		m_pshChildConn=this;
		return;
	}

	//wire pin should be set to fixed to!
	//check this asap!
	if(!m_bFixed)
	CShape::OnLButtonDown(nFlags,point);
}

void CShapePin::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CShape::OnLButtonDown(nFlags,point);
	if(!m_Mode){
		m_Mode=1;


		CString sConnect=TABLE_PIN_LABEL[m_TypePin];

		//sConnect+=".txt";

		CCableEdit dialog(sConnect,TRUE);

		//07/01/2006
		CString strName;
		if(m_bJack)
			strName.Format("J%i",m_uiPinnumber);
		else
			strName.Format("P%i",m_uiPinnumber);
		dialog.m_strName=m_strIdent;//strName;

		if(dialog.DoModal()==IDOK){

			m_TypePin=dialog.m_iType;
			//07/01/2006
			//Get pin number
			dialog.m_strName.MakeUpper();
			char c;
			int n;
			if(sscanf(dialog.m_strName,"%c%i",&c,&n)!=EOF){
				if(c=='J'){
					m_bJack=TRUE;
					m_uiPinnumber=n;
				}
				else{
					m_bJack=FALSE;
					m_uiPinnumber=n;
				}
				m_strIdent=dialog.m_strName;
			}
			else{
				m_bJack=FALSE;
				m_uiPinnumber=1;
				m_strIdent="P1";
			}
		}

	}
}

void CShapePin::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShapeContainer::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		archive<<m_uiPinnumber;
		archive<<m_uiPos;
		archive<<m_dwStyle;
		archive<<m_TypePin;
		archive<<m_rectPin;
	}
    else{
		archive>>m_uiPinnumber;
		archive>>m_uiPos;
		archive>>m_dwStyle;
		archive>>m_TypePin;
		archive>>m_rectPin;
	}

}

void CShapePin::SerializeDdw(CDdwioFile &ddwfile){

	ddwfile.WritePin(m_Rect.TopLeft(),
		m_uiPinnumber,
		m_uiPos,
		m_dwStyle,
		m_TypePin);
}

void CShapePin::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapePin::SerializeSvg(CSvgioFile &svgfile)
{
	int offset_x;
	int offset_y;
	CFont *def_font;
	CString strlabel1;
	CString strlabel2;
	CPoint point;
	CRect rect = m_rectPin + m_Rect.TopLeft();
	CRect rect1;
	CRect rect2;

	//Create rectangles and labels needed for drawing
	_DoCreateRectUnitPin(point, rect1, rect2, strlabel1, strlabel2);

	svgfile.WriteRectangle(rect);

	svgfile.WriteText(CPoint(rect1.TopLeft().x, rect1.BottomRight().y), strlabel1, AfxGetFont(AFX_FONT_SMALL));
	svgfile.WriteText(CPoint(rect2.TopLeft().x, rect2.BottomRight().y), strlabel2, AfxGetFont(AFX_FONT_SMALL));

	//divisory line
	switch (m_uiPos){
	case _DRAFTDRAW_SEL_RESIZING_RECT_N:
	case _DRAFTDRAW_SEL_RESIZING_RECT_S:
		svgfile.WriteMoveTo(point);
		svgfile.WriteLineTo(point + CPoint(rect.Width(), 0));
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_E:
	case _DRAFTDRAW_SEL_RESIZING_RECT_W:
		svgfile.WriteMoveTo(point);
		svgfile.WriteLineTo(point + CPoint(0, rect.Height()));
		break;
	}
	//conecting point
}

//Return a pointer to a shape that can be connect, but doesn´t connect anaything.
BOOL CShapePin::PtInRect(LPPOINT point,CShapeContainer** lpSh){

	CPoint point0=GetParent()->m_Rect.TopLeft();
	if((m_Mode)&&(m_rectConect.PtInRect(CPoint(CPoint(*point) - point0) ))
		//10/12/2004
		//To prevent pins to get connected when part edition mode is active.
		&&GetParent()!=this){

		*lpSh=this;
		return TRUE;
	}

	return FALSE;
}

void CShapePin::_DoCreateRectUnitPin(CPoint& point, CRect& rect1, CRect& rect2, CString& strlabel1, CString& strlabel2)
{
	//Local variables
	CRect rect = m_rectPin + m_Rect.TopLeft();

	switch (m_uiPos){
	case _DRAFTDRAW_SEL_RESIZING_RECT_N:
	case _DRAFTDRAW_SEL_RESIZING_RECT_S:
		point = rect.TopLeft() + CPoint(0, rect.Height()*0.5);
		rect1 = CRect(rect.TopLeft(), point + CPoint(rect.Width(), 0));
		rect1.DeflateRect(0, rect1.Height()*0.10);
		rect2 = CRect(point, rect.BottomRight());
		rect2.DeflateRect(0, rect2.Height()*0.10);
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_E:
	case _DRAFTDRAW_SEL_RESIZING_RECT_W:
		point = rect.TopLeft() + CPoint(rect.Width()*0.5, 0);
		rect1 = CRect(rect.TopLeft(), point + CPoint(0, rect.Height()));
		rect1.DeflateRect(rect1.Width()*0.10, rect1.Height()*0.10);
		rect2 = CRect(point, rect.BottomRight());
		rect2.DeflateRect(rect2.Width()*0.10, rect2.Height()*0.10);
		break;
	}

	//label
	switch (m_uiPos){
	case _DRAFTDRAW_SEL_RESIZING_RECT_E:
	case _DRAFTDRAW_SEL_RESIZING_RECT_S:
		strlabel1.Format("J%i", m_uiPinnumber);
		strlabel2.Format("P%i", m_uiPinnumber);
		break;
	case _DRAFTDRAW_SEL_RESIZING_RECT_W:
	case _DRAFTDRAW_SEL_RESIZING_RECT_N:
		strlabel2.Format("J%i", m_uiPinnumber);
		strlabel1.Format("P%i", m_uiPinnumber);
		break;
	}

	if (m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
		CString str = strlabel1;
		strlabel1 = strlabel2;
		strlabel2 = str;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CShapeWire
/*IMPLEMENT_DYNCREATE(CShapeWire, CShapeContainer)*/
IMPLEMENT_SERIAL(CShapeWire, CShapeContainer, 1)

CShapeWire::CShapeWire(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapeContainer(lpRect,nId,cmddq)
{

	m_uiShapeType=ddcShapeWire;
	m_ShapeNumber=2;
	m_iSize=10;

	//add pin

	for(int i=0;i<2;i++){

		CShapePin *psh=new CShapePin(i,0,1);
		psh->m_Mode=1;
		psh->SetParent(this);
		psh->m_uiShapeId=i+1;
		m_obarrShapearr.Add(psh);
	}

	m_bLineResize=TRUE;
}

CShapeWire::~CShapeWire()
{
	//when somebody deletes the shape connection handlers
	//must appear in units or wires connected to deleted wire
	for(int i=0;i<m_obarrShapearr.GetSize();i++){
		CShape *pSh=(CShape *)m_obarrShapearr[i];
		if(!pSh->IsKindOf(RUNTIME_CLASS(CShapePin))){
			continue;
		}
		CShapePin *pPin=(CShapePin *)pSh;
		if(pPin->m_bConect){
			((CShapePin *)pPin->m_pshChildConn)->Disconnect(CPoint(1,1),TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeWire message handlers

void CShapeWire::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CShape::OnLButtonDown(nFlags,point);
	if(!m_Mode){
		m_Mode=1;

		//Check for a bridge
		CShape *pShFirst=GetFirstChainItem();
		CShape *pSh=pShFirst;
		CShape *pShLast=NULL;
		CShape *pShPrev=NULL;
		while(pSh){
			pShLast=pSh;
			GetNextChainItem(pSh, pShPrev);
			if(pShLast==pSh){
				TRACE("TRACE: End less loop!\n");
				goto WIRE_EDIT_NORMAL;
			}
			//Keep prev chain item
			pShPrev=pSh;
		}

		if(pShLast){
			CString sConnect = "";
			CShape *pSh=(CShape *)((CShapeContainer*)pShFirst)->m_obarrShapearr.GetAt(0);
			
			//check pin is of type 'TB'
			if(((CShapePin*)pSh->m_pshChildConn)->m_TypePin>=0){
				sConnect=TABLE_PIN_LABEL[((CShapePin*)pSh->m_pshChildConn)->m_TypePin];
				if(sConnect.CompareNoCase("TB.txt")==0){

					int uiShFirst=pSh->m_pshConn->m_uiShapeId;

					if(m_strIdent!=""){
						SDisplayPatchPProp(m_strIdent);
						return;
					}
					else{
						goto WIRE_EDIT_NORMAL;
					}
				}//end if compare TB
			}

			pSh=(CShape *)((CShapeContainer*)pShLast)->m_obarrShapearr.GetAt(1);
			
			//check pin is of type 'TB'
			if(((CShapePin*)pSh->m_pshChildConn)->m_TypePin>=0){
				sConnect=TABLE_PIN_LABEL[((CShapePin*)pSh->m_pshChildConn)->m_TypePin];
				if(sConnect.CompareNoCase("TB.txt")==0){

					int uiShLast=pSh->m_pshConn->m_uiShapeId;

					if(m_strIdent!=""){
						SDisplayPatchPProp(m_strIdent);
						return;
					}
					else{
						goto WIRE_EDIT_NORMAL;
					}

				}//end if compare TB
			}
		}

WIRE_EDIT_NORMAL:
		//Normal behaviour
		CString str="cable516.txt";
		if(!m_strTypeIdent.IsEmpty()){
			str=m_strTypeIdent;
		}
		CCableEdit dCable(str);

		if(!m_strIdent.IsEmpty()){
			dCable.m_strName=m_strIdent;
		}

		if(dCable.DoModal()==IDOK){
			m_strIdent=dCable.m_strName;
			m_strTypeIdent=dCable.m_strFile;
		}
	}
}

void CShapeWire::OnDraw(CDC *pDC){

	/*if((!m_Mode)&&(!m_Rect.IsRectEmpty())){*/

		CShape::OnDraw(pDC);
	/*}*/
	/*else*/	/*if(!m_Rect.IsRectEmpty())*/{

		//Change attributes
		CPen *prevColor;
		CPen *newPen;
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			newPen=new CPen( PS_GEOMETRIC , 1, RGB(0,0,255) );
			prevColor=pDC->SelectObject(newPen);
		}

		//Draw wire
		pDC->MoveTo(m_Rect.TopLeft());
		pDC->LineTo(m_Rect.BottomRight());

		//Draw 1st end
		CPoint point=m_Rect.TopLeft();

		CShape *psh=(CShape *)m_obarrShapearr.GetAt(0);

		psh->m_Rect=CRect(point,CSize(5,5));

		psh->OnDraw(pDC);

		//Draw 2nd end
		point=m_Rect.BottomRight();

		psh=(CShape *)m_obarrShapearr.GetAt(1);

		psh->m_Rect=CRect(point,CSize(5,5));

		psh->OnDraw(pDC);

		//draw label if exists
		if(m_ShapeNumber>2){
			psh=(CShape *)m_obarrShapearr.GetAt(2);
			if(psh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){
				CShapeLabel *pLabel=(CShapeLabel *)psh;
				CRect rectn=m_Rect;rectn.NormalizeRect();
				if(rectn.Height()>=rectn.Width()){//vertical wire
					psh->m_Rect=CRect(m_Rect.TopLeft(),CSize(20,80));
					pLabel->m_Label.bver=true;
				}
				else{//horizontal wire
					psh->m_Rect=CRect(m_Rect.TopLeft(),CSize(80,20));
					pLabel->m_Label.bver=false;
				}
			}
			psh->OnDraw(pDC);
		}

		//Change attributes
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			prevColor=pDC->SelectObject(prevColor);
			delete(newPen);
		}

		//to draw selections
		CShape::OnDraw(pDC);

	}



}

void CShapeWire::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeWire::Serialize( CArchive& archive )
{
	//dbug
	if(m_strIdent.CompareNoCase("1234")==0){
		int i=0;
	}

    // call base class function first
    // base class is CObject in this case
    CShapeContainer::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
        //archive << m_name << m_number;
	}
    else{
		/*
		int nCount;
		CShape *pSh;
		archive>>nCount;
        archive >> pSh;
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
			m_Rect=pSh->m_Rect;
		}
		*/
	}

}

void CShapeWire::SerializeDdw(CDdwioFile &ddwfile)
{
	ddwfile.WriteUnit("*wire",&m_Rect,std::string(m_strIdent+","+m_strTypeIdent).c_str());
}

void CShapeWire::RepositionPins(){

	//1st end
	CPoint point=m_Rect.TopLeft();

	CShape *psh=(CShape *)m_obarrShapearr.GetAt(0);

	psh->m_Rect=CRect(point,CSize(5,5));
	((CShapePin *)psh)->m_rectConect=CRect(point,CSize(5,5));

	//2nd end
	point=m_Rect.BottomRight();

	psh=(CShape *)m_obarrShapearr.GetAt(1);

	psh->m_Rect=CRect(point,CSize(5,5));
	((CShapePin *)psh)->m_rectConect=CRect(point,CSize(5,5));
}

CObject *CShapeUnit::LoadUnit(LPCTSTR lpUnitName,CArchive *ar)
{
	//Get unit name
	CString strFile=lpUnitName;
	//Get libray name
	CString strLib=strFile.Left(strFile.Find('.'));
	//Remove library name
	strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

	m_sUnitName=lpUnitName;
	CString str=strFile.Left(strFile.Find('.'));
	str+="%i";
	m_strIdent.Format(str,m_uiPartnumber);

	//To load parts from db
	//CArchiveDb ardb(NULL,CArchive::load   );
	//CDatabase db;
	std::stringstream* sout;
	BOOL bFlagDb=TRUE;//Set to true if part is stored in database
	BOOL bFlagDbEnable=TRUE;//Set to true if database storage is enabled

	BOOL bStaticBehave=FALSE;
	if(ar){

		bStaticBehave=TRUE;
	}
	else{

		//Reconnect to database
		if(!g_db.IsOpen()){
			TCHAR sConnect[1024];
			g_GetConnectString(sConnect,1024);
			g_db.OpenEx(sConnect);
		}

		CRecordset rsConnector(&g_db);

		rsConnector.m_strFilter="nNamePart='"+CString(strFile)+"' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
		rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");
		
		CString str;
		if(!rsConnector.IsEOF()){

			//Check if part is stored in binary format
			//******************************************************************
			//******************************************************************
			CDBVariant dbTextBin;
			rsConnector.GetFieldValue("bTextBin",str);

			if(str.Compare("1")==0){

				BYTE *pStorage = NULL;

				CDBVariant dbTextPart;
				rsConnector.GetFieldValue("bTextPart",dbTextPart);

				if(dbTextPart.m_dwType!=DBVT_BINARY){
					return NULL;
				}

				//SIZE_T sizet=GlobalSize(dbTextPart.m_pbinary);
				pStorage=(BYTE *)GlobalLock(dbTextPart.m_pbinary->m_hData);

				CMemFile file(pStorage, dbTextPart.m_pbinary->m_dwDataLength, 0);
				CFileException fe;
				//file.Attach(pStorage, dbTextPart.m_pbinary->m_dwDataLength, 0);

				CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
				//loadArchive.m_pDocument = this;
				loadArchive.m_bForceFlat = TRUE;

				TRY
				{
					Serialize(loadArchive);     // load main contents
					loadArchive.Close();
					file.Close();
				}
				CATCH_ALL(e)
				{
					file.Abort();   // will not throw an exception
					// remove item because it didn't fully loaded
					/*GetDocument()->RemoveItem(pItem);*/
					THROW_LAST();
				}
				END_CATCH_ALL

				rsConnector.Close();

				m_sUnitName=lpUnitName;

				return this;
			}
			//******************************************************************
			//******************************************************************


			bFlagDb=TRUE;

			rsConnector.GetFieldValue("nTextPart",str);
		}


		sout= new std::stringstream(ios_base::in);

		sout->str(std::string(str));

	}


	//Allocate memory
	m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
	m_pLabels=(label **)GlobalAlloc(GMEM_ZEROINIT,sizeof(label*)*_DEFAULTMAXPLINE_DRAFTCABLE);

	//CStdioFile funit;

	int indexpline=0;
	int indexlabel=0;
	int indexpin=0;

	CObject *pObject=NULL;

	if((m_bFlagFileOpen==FALSE)&&(bStaticBehave==FALSE)){
		if(bFlagDb&&bFlagDbEnable){

		}
		else{

			m_bFlagFileOpen=TRUE;
			if(!funit.Open(strFile/*lpUnitName*/,CFile::modeRead|CFile::typeText)){
				::AfxMessageBox("No se puede cargar.",MB_ICONSTOP|MB_OK);
				return NULL;
			}
		}
	}

	CString stlabel;
	CString stline;
	std::string stdline;

	if(bStaticBehave==FALSE){

		if(bFlagDb&&bFlagDbEnable){

			*sout>>stdline;
			stline=CString(stdline.c_str());
			if(stline.IsEmpty() || sout->eof()){
				::AfxMessageBox("Archivo vacío.",MB_ICONSTOP|MB_OK);
				return NULL;
			}
		}
		else{
			if(!funit.ReadString(stline)){
				::AfxMessageBox("Archivo vacío.",MB_ICONSTOP|MB_OK);
				return NULL;
			}
		}
	}
	else{

		ar->ReadString(stline);
	}

	//LABEL LOOP
	BOOL bFlagFileEnd = TRUE;
	while(stline.IsEmpty()==FALSE && bFlagFileEnd){

//CString strdebug=stline;
//::AfxMessageBox(strdebug,MB_ICONINFORMATION|MB_OK);

		//process polyline
		if(!stline.CompareNoCase("[polyline]")){

			//POLYLINE LOOP
			
			if(bFlagDb&&bFlagDbEnable){
				*sout>>stdline;
				stline=CString(stdline.c_str());
				bFlagFileEnd=!sout->eof();//stdline.length()>0;
			}
			else{
				bFlagFileEnd=funit.ReadString(stline);
			}
			
			while(bFlagFileEnd){

//CString strdebug=stline;
//::AfxMessageBox(strdebug,MB_ICONINFORMATION|MB_OK);

				if(stline[0]=='['){
					break;
				}

				CString stdata=CutString(stline);

				int idata=atoi(stdata.GetBuffer(255));

//CString strdebug;
//strdebug.Format("%i",idata);
//::AfxMessageBox(strdebug,MB_ICONINFORMATION|MB_OK);

				m_pPoints[indexpline]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));
if(m_pPoints[indexpline]==NULL){
	//strdebug="NULL";
	//::AfxMessageBox(strdebug,MB_ICONSTOP|MB_OK);
}
				m_pPoints[indexpline][0]=idata;

				stdata=CutString(stline);
				int index=1;
				while(stdata.IsEmpty()==FALSE){

					idata=atoi(stdata.GetBuffer(255));

					m_pPoints[indexpline][index]=idata;

					index++;

					stdata=CutString(stline);
				}

				indexpline++;

			
				if(bFlagDb&&bFlagDbEnable){
					*sout>>stdline;
					stline=CString(stdline.c_str());
					bFlagFileEnd=!sout->eof();//stdline.length()>0;
				}
				else{
					bFlagFileEnd=funit.ReadString(stline);
				}
			
			
			}
		
		}//end if process polyline

		//process label
		if(!stline.CompareNoCase("[label]")){

			if(bStaticBehave==FALSE){
				if(bFlagDb&&bFlagDbEnable){
					*sout>>stdline;
					stline=CString(stdline.c_str());
				}
				else{
					funit.ReadString(stline);
				}
			}
			else{

				ar->ReadString(stline);
			}

			//LABEL LOOP
			while(stline.IsEmpty()==FALSE){

				if(stline[0]=='['){
					break;
				}


				label *pLabel;
				if(!bStaticBehave){

					pLabel=(label *)GlobalAlloc(GMEM_ZEROINIT,sizeof(label));
					pLabel->style=WS_VISIBLE;//24/03/2005
					m_pLabels[indexlabel]=pLabel;
				}
				else{

					pObject=new CShapeLabel;
					pLabel=&((CShapeLabel*)pObject)->m_Label;
				}



				//read rectangle
				CString stdata=CutString(stline);
				int idata=atoi(stdata.GetBuffer(255));
				int l=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int t=idata;
				stdata=CutString(stline);
				stdata=CutString(stline);
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int r=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int b=idata;

				pLabel->rect=new CRect(l,t,l,t);//new CRect(l,t,r,b);
				

				if(bStaticBehave==TRUE){

					((CShapeLabel*)pObject)->m_Rect=pLabel->rect;
				}

				//read text
				if(bStaticBehave==FALSE){
					if(bFlagDb&&bFlagDbEnable){
						*sout>>stdline;
						stline=CString(stdline.c_str());
					}
					else{
						funit.ReadString(stline);
					}
					//funit.ReadString(stline);
				}
				else{

					ar->ReadString(stline);
				}

				pLabel->slabel=new CString(CutString(stline));
				pLabel->sname=new CString(CutString(stline));

				//read flag vertical
				if(bStaticBehave==FALSE){
					if(bFlagDb&&bFlagDbEnable){
						*sout>>stdline;
						stline=CString(stdline.c_str());
						if(sout->eof()){
							stline = "";
						}
					}
					else{
						funit.ReadString(stline);
					}
					//funit.ReadString(stline);
				}
				else{

					ar->ReadString(stline);
				}

				idata=atoi(CutString(stline).GetBuffer(255));
				pLabel->bver=idata;
				idata=atoi(CutString(stline).GetBuffer(255));
				pLabel->iSize=idata;

				//read font
/*
pLabel->font=new CFont;


pLabel->font->CreateFont(
   12,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_NORMAL,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "Arial");                 // lpszFacename

*/


				LOGFONT lf;
				ZeroMemory(&lf, sizeof(LOGFONT));

				if(pLabel->iSize>0){
					lf.lfHeight=-pLabel->iSize-3;
				}
				else{
					lf.lfHeight=-13;
				}
				lf.lfWeight=400;
				lf.lfOutPrecision=3;
				lf.lfClipPrecision=2;
				lf.lfQuality=1;
				lf.lfPitchAndFamily=34;
				strcpy(lf.lfFaceName,"Arial");

				pLabel->font=new CFont();
				pLabel->font->CreateFontIndirect(&lf);



				//next label
				indexlabel++;

				if(bStaticBehave==FALSE){
					if(bFlagDb&&bFlagDbEnable){
						*sout>>stdline;
						stline=CString(stdline.c_str());
						if(sout->eof()){
							stline = "";
						}
					}
					else{
						funit.ReadString(stline);
					}


				}
				else{

					ar->ReadString(stline);
					break;
				}

			}
		}
		
		//process pin
		//=================================================================
		if(!stline.CompareNoCase("[pin]")){

			if(bFlagDb&&bFlagDbEnable){
				*sout>>stdline;
				stline=CString(stdline.c_str());
				bFlagFileEnd=stdline.length()>0;
			}
			else{
				bFlagFileEnd=funit.ReadString(stline);
			}
			
			while(bFlagFileEnd){


			//PIN LOOP


				if(stline[0]=='[') break;

				//read pin
				CString stdata=CutString(stline);
				int idata=atoi(stdata.GetBuffer(255));
				int x=idata;							//X location
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int y=idata;							//Y location

				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				UINT uiPinnumber=idata;					//Pin number
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				UINT uiPinpos=idata;					//Pin position
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				DWORD dwStyle=idata;					//Style
				/*stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				BOOL bJack=idata;					
				dwStyle|=SHAPEUNIT_PINTYPE_JACK*bJack;	//Jack flag
				*/
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int iIndexType=idata;					//Index of pin type

				CShapePin *pShpin=new CShapePin(uiPinnumber,uiPinpos,dwStyle);
				pShpin->m_bFixed=TRUE;
				pShpin->m_Mode=1;
				pShpin->m_rectPin+=CPoint(x,y);
				pShpin->m_TypePin=iIndexType;

				pShpin->SetParent(this);
				pShpin->m_uiShapeId=indexpin+1;
				m_obarrShapearr.Add(pShpin);
				//next pin
				indexpin++;


				if(bFlagDb&&bFlagDbEnable){
					*sout>>stdline;
					stline=CString(stdline.c_str());
					bFlagFileEnd=!sout->eof();//bFlagFileEnd=stdline.length()>0;
				}
				else{
					bFlagFileEnd=funit.ReadString(stline);
				}




			}
		}
		//=================================================================

		
		//process rectangle
		if(!stline.CompareNoCase("[rectangle]")){

			if(bFlagDb&&bFlagDbEnable){
				*sout>>stdline;
				stline=CString(stdline.c_str());
				bFlagFileEnd=!sout->eof();//stdline.length()>0;
			}
			else{
				bFlagFileEnd=funit.ReadString(stline);
			}
			
			while(bFlagFileEnd){

			//RECTANGLE LOOP

				if(stline[0]=='[') break;

				//read rectangle
				CString stdata=CutString(stline);
				int idata=atoi(stdata.GetBuffer(255));
				int x1=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int y1=idata;

				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int x2=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int y2=idata;

				m_Rect=CRect(x1,y1,x2,y2);


				if(bFlagDb&&bFlagDbEnable){
					*sout>>stdline;
					stline=CString(stdline.c_str());
					bFlagFileEnd=!sout->eof();//stdline.length()>0;
				}
				else{
					bFlagFileEnd=funit.ReadString(stline);
				}

			}//end while
		}// end if rectangle
		
		//process ellipse
		if(!stline.CompareNoCase("[ellipse]")){

			if(bFlagDb&&bFlagDbEnable){
				*sout>>stdline;
				stline=CString(stdline.c_str());
				bFlagFileEnd=!sout->eof();//stdline.length()>0;
			}
			else{
				bFlagFileEnd=funit.ReadString(stline);
			}
			
			while(bFlagFileEnd){


			//ELLIPSE LOOP


				if(stline[0]=='[') break;

				//read rectangle
				CString stdata=CutString(stline);
				int idata=atoi(stdata.GetBuffer(255));
				int x1=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int y1=idata;

				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int x2=idata;
				stdata=CutString(stline);
				idata=atoi(stdata.GetBuffer(255));
				int y2=idata;

				//m_Rect=CRect(x1,y1,x2,y2);
				CShapeEllipse *pSh=new CShapeEllipse();
				pSh->m_Rect=CRect(x1,y1,x2,y2);
				pSh->m_Mode=1;
				m_obarrShapearr.Add(pSh);

				//Fill color
				stdata=CShapeUnit::CutString(stline);
				if(!stdata.IsEmpty()){
					pSh->m_bTransparent=FALSE;
					char   *stopstring;
					int idata=strtol(stdata,&stopstring,16);
					pSh->m_crFill=idata;
				}


				if(bFlagDb&&bFlagDbEnable){
					*sout>>stdline;
					stline=CString(stdline.c_str());
					bFlagFileEnd=!sout->eof();//stdline.length()>0;
				}
				else{
					bFlagFileEnd=funit.ReadString(stline);
				}

			}//end while
		}//end if ellipse

		if(bStaticBehave==TRUE){

			break;
		}

	}//end of label loop


	m_PointspCount=indexpline;
	m_LabelsCount=indexlabel;
	m_ShapeNumber=indexpin;


	//Close file
	if((stline.IsEmpty()==TRUE)&&(bStaticBehave==FALSE)){
		if(bFlagDb&&bFlagDbEnable){

		}
		else{
			funit.Close();
			m_bFlagFileOpen=FALSE;
		}
	}


	//Close db
	if(!bStaticBehave){

		//db.Close();
	}


	//::AfxMessageBox("Cargado.",MB_ICONINFORMATION|MB_OK);
	return pObject;
}

CString CShapeUnit::CutString(CString &strtext)
{
	//Cut a rows into separate string in iteration manner
	//ALLOWED SEPARATORS: ','

	//FINDS FIRST SEPARATOR
	int index=strtext.Find(',');
/*
	int index=strtext.Find(';');
	int index1=strtext.Find('\t');

	if((index1>=0)&&(index1<index)||(index<0)) index=index1;

	index1=strtext.Find(' ');

	if((index1>=0)&&(index1<index)||(index<0)) index=index1;
*/
	//DELETES REPEATED SEPARATORS
/*
	while((index<strtext.GetLength()-1)&&
		((strtext[index+1]==';')||
		(strtext[index+1]=='\t')||
		(strtext[index+1]==' '))){
*/
	while((index<strtext.GetLength()-1)&&
		(strtext[index+1]==',')){

		strtext.Delete(index,1);
	}


	//CUT STRING
	if(index==-1){
		//simple row
		CString strfield=strtext;
		strtext="";
		return strfield;
	}
	else if(index==0){
		//deletes initial separators
		strtext=strtext.Right(strtext.GetLength()-index-1);
		return CutString(strtext);
	}
	else{
		//cut left most string of a row
		CString strfield=strtext.Left(index);
		strtext=strtext.Right(strtext.GetLength()-index-1);
		return strfield;
	}


}

/////////////////////////////////////////////////////////////////////////////
// CShapeLabel
/*IMPLEMENT_DYNCREATE(CShapeLabel, CShape)*/
IMPLEMENT_SERIAL(CShapeLabel, CShape, 1)

CShapeLabel::CShapeLabel(label *lpLbl/*=NULL*/,LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{

	m_uiShapeType=ddcShapeLabel;
	m_bIni=FALSE;
	m_bNoResize=TRUE;
	//m_Rect=CRect(0,0,100,100);
	m_Rect=CRect(0,0,0,0);

	if(lpLbl){

		m_Label=*lpLbl;
	}
	else{

		m_Label.font=NULL;//!!!

		m_Label.rect=new CRect(0,0,0,0);//!!!
	}
	/*
	m_Label.font=new CFont();
	m_Label.font->CreateFont(
	   12,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   "Arial");                 // lpszFacename
	*/
}

BOOL CShapeLabel::Create(LPRECT lpRect,LPCTSTR lpszText,int nSize,BOOL bVer){

	m_Label.slabel=new CString(lpszText);
	m_Label.sname=new CString(lpszText);
	m_Label.iSize=nSize;
	m_Label.bver=bVer;
	m_Rect=lpRect;

	//Create font
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight=-m_Label.iSize-3;
	lf.lfWeight=400;
	lf.lfOutPrecision=3;
	lf.lfClipPrecision=2;
	lf.lfQuality=1;
	lf.lfPitchAndFamily=34;
	strcpy(lf.lfFaceName,"Arial");

	m_Label.font=new CFont();
	m_Label.font->CreateFontIndirect(&lf);

	//This is done becouse create will be called mainly from command interpreter
	m_bIni=TRUE;

	return TRUE;
}

CShapeLabel::~CShapeLabel()
{
	if(m_Label.font!=NULL){

		delete(m_Label.font);
	}
}

void CShapeLabel::SetModified()
{
	m_bIni=FALSE;
	CPoint pt=m_Rect.TopLeft();
	m_Rect=CRect(pt,pt);
}

/////////////////////////////////////////////////////////////////////////////
// CShapeLabel implementation
CShape& CShapeLabel::operator=( const CShape& Sh ){

	CRuntimeClass* prt = Sh.GetRuntimeClass();
	ASSERT( strcmp( prt->m_lpszClassName, "CShapeLabel" )  == 0 );

	CShape::operator =(Sh);

	CShapeLabel *pShLabel=(CShapeLabel *)&Sh;

	m_Label.bver=pShLabel->m_Label.bver;
	m_Label.slabel=new CString(*pShLabel->m_Label.slabel);

	LOGFONT lf;
	pShLabel->m_Label.font->GetLogFont(&lf);
	m_Label.font = new CFont();
	m_Label.font->CreateFontIndirect(&lf);

	m_Rect = CRect(pShLabel->m_Rect.TopLeft(), pShLabel->m_Rect.TopLeft());

	return *this;
}
CShape& CShapeLabel::operator++( ){

	CString sLabel=*m_Label.slabel,sCount;

	int i=sLabel.FindOneOf("0123456789");

	int count=0;
	if(i>0){
		count=atoi(sLabel.Right(sLabel.GetLength()-i));
		sLabel=sLabel.Left(i);
	}
	count++;
	sCount.Format("%i",count);

	*m_Label.slabel=sLabel+sCount;
	return *this;
}

void CShapeLabel::ResizeAuto(CDC *pDC)
{
	if (m_Rect.IsRectEmpty()){

		//Save font
		CFont *prev_font;
		if (m_Label.font == NULL){
			if (m_Label.iSize <= 10){
				prev_font = pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));
			}
			else{
				prev_font = pDC->SelectObject(AfxGetFont(AFX_FONT_NORMAL));
			}
		}
		else{
			prev_font = pDC->SelectObject(m_Label.font);
		}

		if (m_Label.bver){

			//Local variables
			CString stlabel = "";
			CSize szac = CSize(0, 0);

			//Check if label has not been initialized
			if (!m_bIni){
				for (int i = 0; i<m_Label.slabel->GetLength(); i++){

					if (i)stlabel += "\n";
					stlabel += m_Label.slabel[0][i];
				}
				*m_Label.slabel = stlabel;
				//Set label as initialized
				m_bIni = TRUE;
			}

			//Calculate bounding rectangle
			for (int i = 0; i<m_Label.slabel->GetLength(); i++){

				CString stlabel1 = m_Label.slabel[0][i];
				SIZE sz;
				GetTextExtentPoint32(pDC->m_hDC, stlabel1, lstrlen(stlabel1), &sz);

				szac += CSize(0, sz.cy);
				if (szac.cx<sz.cx){
					szac.cx = sz.cx;
				}
			}

			//Set bounding rectangle
			m_Rect = CRect(m_Rect.TopLeft(), szac);
		}
		else{
			//Check if label has not been initialized
			if (!m_bIni){
				m_Label.slabel->Replace("\n", "");
				m_bIni = TRUE;
			}

			//Calculate bounding rectangle
			SIZE sz;
			GetTextExtentPoint32(pDC->m_hDC, *m_Label.slabel, lstrlen(*m_Label.slabel), &sz);

			//Set bounding rectangle
			m_Rect = CRect(m_Rect.TopLeft(), sz);
		}

		//Restore font
		pDC->SelectObject(prev_font);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeLabel message handlers

void CShapeLabel::RotateLabel(){

	m_Label.bver=!m_Label.bver;
	m_Rect=CRect(m_Rect.TopLeft(),CSize(m_Rect.Height(),m_Rect.Width()));
}

BOOL CShapeLabel::OnCommand( WPARAM wParam, LPARAM lParam ){

	switch(LOWORD(wParam)){
	case ID_ROTATE:
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			RotateLabel();
			return TRUE;
		}
		break;
	}

	return FALSE;
}

void CShapeLabel::OnDraw(CDC *pDC){

	if((!m_Mode) && m_Rect.IsRectEmpty()){
		//Resize label to fit text
		ResizeAuto(pDC);
	}
	else if((!m_Mode)&&(!m_Rect.IsRectEmpty())){

		CShape::OnDraw(pDC);
	}
	else if(!m_Rect.IsRectEmpty()){
		//Draw connection with related wire
		if(m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT)
		if(m_pshConn){
			CPen penDotted( PS_DOT, 1, RGB(0,0,0) );
			CPen *prevPen=pDC->SelectObject(&penDotted);
			CPoint ptOff=m_Rect.CenterPoint()-m_RectLast.CenterPoint();
			pDC->MoveTo(m_Rect.CenterPoint());
			CPoint pt=m_pshConn->m_Rect.CenterPoint()+ptOff;
			pDC->LineTo(pt);
			pDC->Rectangle(m_Rect);
			pDC->SelectObject(prevPen);
		}

		CString stlabel="";

		//Redundant block?
		if(m_Label.bver){
			stlabel=*m_Label.slabel;
		}
		else{
			stlabel=*m_Label.slabel;
		}

		//Save previous font
		CFont *prev_font;
		if(m_Label.font==NULL){
			if(m_Label.iSize<=10){
				prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));
			}
			else{
				prev_font=pDC->SelectObject(AfxGetFont(AFX_FONT_NORMAL));
			}
		}
		else{
			prev_font=pDC->SelectObject(m_Label.font);
		}

		//18/11/2004
		//Draw text fills the rectangle with background color if background mode is OPAQUE
		//All this mode changes can be taken up to a higher level that paint or label in a block,
		//for example.
		//COLORREF clrprev=pDC->SetBkColor( RGB(0,128,0) );
		int nBkMode=pDC->SetBkMode( TRANSPARENT );
		pDC->DrawText(stlabel,&m_Rect,DT_CENTER);
		//pDC->SetBkColor( clrprev );
		pDC->SetBkMode( nBkMode );

		//Restore font
		pDC->SelectObject(prev_font);

		//to draw selections
		CShape::OnDraw(pDC);

	}//end !m_Rect.IsRectEmpty
}

void CShapeLabel::Serialize( CArchive& archive )
{

    // call base class function first
    // base class is CObject in this case
	DoConntoId();
    CShape::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
		m_Label.font->Serialize(archive);

		archive<<m_Label.bver;
		archive<<m_Label.iSize;
		archive<<*m_Label.rect;
		archive<<*m_Label.slabel;
		archive<<*m_Label.sname;
		//1.04
		archive<<m_Label.style;
	}
    else{
        //archive >> m_name >> m_number;
		m_Label.font=new CFont;
		m_Label.font->Serialize(archive);

		archive>>m_Label.bver;
		archive>>m_Label.iSize;
		m_Label.rect=new CRect;
		archive>>*m_Label.rect;
		m_Label.slabel=new CString;
		archive>>*m_Label.slabel;
		m_Label.sname=new CString;
		archive>>*m_Label.sname;
		//1.04
		float fVer0=1.04;
		float fData=m_fVer-fVer0;
		if(fData>=0){
			archive>>m_Label.style;
		}


		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));

		if(m_Label.iSize>0){
			lf.lfHeight=-m_Label.iSize-3;
		}
		else{
			lf.lfHeight=-13;
		}
		lf.lfWeight=400;
		lf.lfOutPrecision=3;
		lf.lfClipPrecision=2;
		lf.lfQuality=1;
		lf.lfPitchAndFamily=34;
		strcpy(lf.lfFaceName,"Arial");

		m_Label.font=new CFont();
		m_Label.font->CreateFontIndirect(&lf);
	}

}

void CShapeLabel::SerializeDdw(CDdwioFile &ddwfile)
{
	m_Label.rect=new CRect(m_Rect);
	ddwfile.WriteLabel(&m_Label);
}

void CShapeLabel::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeLabel::SerializeSvg(CSvgioFile &svgfile)
{
	CPoint p1 = CPoint(m_Label.rect->TopLeft().x, m_Label.rect->CenterPoint().y);

	svgfile.WriteText(p1, *m_Label.slabel, m_Label.font);
}

void CShapeLabel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CShape::OnLButtonDown(nFlags,point);
	if(!m_Mode){
		m_Mode=1;
		//Edit label
		CDialogText dlgText;
		dlgText.m_bVertical = m_Label.bver;
		dlgText.m_sAlias = *m_Label.slabel;
		dlgText.m_sName = "";//*m_Label.sname;
		dlgText.m_iSize = m_Label.iSize;

		if(dlgText.DoModal()==IDOK){
			m_Label.bver=dlgText.m_bVertical;
			delete(m_Label.slabel);
			delete(m_Label.sname);
			m_Label.slabel=new CString(dlgText.m_sAlias);
			if(dlgText.m_sName.GetLength()==0){
				m_Label.sname=new CString(dlgText.m_sAlias);
			}
			else{
				m_Label.sname=new CString(dlgText.m_sName);
			}
			m_Label.iSize=dlgText.m_iSize;

			LOGFONT lf;
			ZeroMemory(&lf, sizeof(LOGFONT));

			lf.lfHeight=-dlgText.m_iSize-3;
			lf.lfWeight=400;
			lf.lfOutPrecision=3;
			lf.lfClipPrecision=2;
			lf.lfQuality=1;
			lf.lfPitchAndFamily=34;
			strcpy(lf.lfFaceName,"Arial");

			delete(m_Label.font);
			m_Label.font=new CFont();
			m_Label.font->CreateFontIndirect(&lf);

			SetModified();
		}
	}
}

void CShapeLabel::OnMouseMove(UINT nFlags, CPoint point)
{
	CShape::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CShapeFrmRect

/*IMPLEMENT_DYNCREATE(CShapeFrmRect, CShape)*/
IMPLEMENT_SERIAL(CShapeFrmRect, CShape, 1)

CShapeFrmRect::CShapeFrmRect(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShape(lpRect,nId,cmddq)
{

	m_Type=1;

	m_Pen.CreatePen(PS_SOLID|PS_DASH,1,RGB(0,0,0));
}

CShapeFrmRect::~CShapeFrmRect()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeRect message handlers
//use this function to select
/*
void CShapeFrmRect::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Point=point;
	m_Rect=CRect(point,point);
}
*/
/*
void CShapeFrmRect::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}
*/

void CShapeFrmRect::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShape::Serialize( archive );

	// now do the stuff for our specific class
	/*
    if( archive.IsStoring() )
        archive << m_Rect;
    else
        archive >> m_Rect;
	*/
}

void CShapeFrmRect::SerializeDdw(CDdwioFile &ddwfile)
{

	ddwfile.WriteRect(&m_Rect);
}

void CShapeFrmRect::OnDraw(CDC *pDC)
{

	if(!m_Mode){

		CShape::OnDraw(pDC);
	}

	else if(!m_Rect.IsRectEmpty()){

		if(m_VectorProyect==CPoint3D(0,1,0)){

			CRect rect=CRect(
				m_Rect.TopLeft().x,
				0/*m_Rect.TopLeft().y*/,
				m_Rect.BottomRight().x,
				/*m_Rect.TopLeft().y*/-m_Height);

			pDC->Rectangle(rect+m_PointOrg);
			return;
		}

		CPen *prev_pen=pDC->SelectObject(&m_Pen);
		pDC->Rectangle(m_Rect);
		pDC->SelectObject(prev_pen);

		//to draw selections
		CShape::OnDraw(pDC);

	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeJunction

IMPLEMENT_DYNCREATE(CShapeJunction, CShapeContainer)

CShapeJunction::CShapeJunction():CShapeContainer()
{
	m_Type=1;
	m_bNoResize=TRUE;
	m_Rect=CRect(0,0,20,20);
}

CShapeJunction::~CShapeJunction()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapeJunction implementation

CShape& CShapeJunction::operator=( const CShape& Sh ){

	CRuntimeClass* prt = Sh.GetRuntimeClass();
	ASSERT( strcmp( prt->m_lpszClassName, "CShapeJunction" )  == 0 );

	CShapeJunction *pShJunction=(CShapeJunction *)&Sh;

	return *this;
}
CShape& CShapeJunction::operator++( ){

	return *this;
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeJunction message handlers
//use this function to select
/*
void CShapeJunction::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Point=point;
	m_Rect=CRect(point,point);
}
*/
/*
void CShapeJunction::OnLButtonUp(UINT nFlags, CPoint point)
{
	//m_Rect=CRect(point,point);
}
*/

void CShapeJunction::SerializeDdw(CDdwioFile &ddwfile)
{
	/*ddwfile.WriteRect(&m_Rect);*/
}

void CShapeJunction::OnDraw(CDC *pDC)
{

	if(!m_Mode){

		CShape::OnDraw(pDC);
	}

	else if(!m_Rect.IsRectEmpty()){

		/*if(m_VectorProyect==CPoint3D(0,1,0)){

			CRect rect=CRect(
				m_Rect.TopLeft().x,
				0,
				m_Rect.BottomRight().x,
				-m_Height);

			pDC->Rectangle(rect+m_PointOrg);
			return;
		}*/

		/*
		CPen *prev_pen=pDC->SelectObject(&m_Pen);
		pDC->Rectangle(m_Rect);
		pDC->SelectObject(prev_pen);
		*/
		pDC->MoveTo(m_Rect.TopLeft());
		pDC->LineTo(m_Rect.BottomRight());

		//to draw selections
		CShape::OnDraw(pDC);

	}
}

/////////////////////////////////////////////////////////////////////////////
// CShapeBus

/*IMPLEMENT_DYNCREATE(CShapeBus, CShape)*/
IMPLEMENT_SERIAL(CShapeBus, CShapePolyline, 1)

CShapeBus::CShapeBus(LPRECT lpRect/*=NULL*/,UINT nId/*=0*/,cmddeque *cmddq /*=NULL*/):CShapePolyline(lpRect,nId,cmddq)
{

	m_bLineResize=TRUE;
}

CShapeBus::~CShapeBus()
{
}

/*
BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShapeBus message handlers
//use this function to select


void CShapeBus::OnDraw(CDC *pDC)
{
	CPen *prevColor;
	CPen *newPen;

	newPen=new CPen( PS_GEOMETRIC , 4, RGB(0,0,255) );
	prevColor=pDC->SelectObject(newPen);

	CShapePolyline::OnDraw(pDC);

	pDC->SelectObject(prevColor);
	delete(newPen);
}

void CShapeBus::SerializeGbr(CGbrioFile &gbrfile)
{

}

void CShapeBus::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CShapePolyline::Serialize( archive );

    // now do the stuff for our specific class

    if( archive.IsStoring() ){
        //archive << m_name << m_number;
	}
    else{
		/*
		int nCount;
		CShape *pSh;
		archive>>nCount;
        archive >> pSh;
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
			m_Rect=pSh->m_Rect;
		}
		*/
	}

}

void CShapeBus::SerializeDdw(CDdwioFile &ddwfile)
{
	int count=m_dwarrPointarr.GetSize();

	CPoint *p=new CPoint[count];

	for(int i=0;i<count;i++){

		p[i]=CPoint(m_dwarrPointarr[i])+m_Rect.TopLeft();
	}

	ddwfile.WriteUnit("*bus",p,count,std::string(m_strIdent+","+m_strTypeIdent).c_str());

	delete(p);
	
}
