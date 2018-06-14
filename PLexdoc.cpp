// PLexdoc.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDrawDoc.h"
#include "PLexdoc.h"

#include "dxf.h"

//std template objects
#include <ios>
#include <iostream>


#include <fstream>
#include <sstream>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLexdoc

IMPLEMENT_DYNCREATE(CPLexdoc, CDocument)

CPLexdoc::CPLexdoc()
{
	m_pParent=NULL;
}

BOOL CPLexdoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPLexdoc::~CPLexdoc()
{
}


BEGIN_MESSAGE_MAP(CPLexdoc, CDocument)
	//{{AFX_MSG_MAP(CPLexdoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLexdoc diagnostics

#ifdef _DEBUG
void CPLexdoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPLexdoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPLexdoc serialization

void CPLexdoc::Serialize(CArchive& ar)
{

	//debug code:testing std c++ library performance
	//==========================================================================
	std::fstream fsFile;
	std::string buffer;

	/*
	int ihandle=_fileno( stdin );
	*/

	//Syncronize archive with standart input
	FILE *file_new=_wfreopen( ar.GetFile( )->GetFilePath(), _T("r"), stdin );
	ios_base::sync_with_stdio(true);

	//std::fstream does not have attach
	//micosoft's iostream class library does have attatch
	/*
	std::ifstream myFile1(ar.GetFile( )->m_hFile  );  
	fsFile.attach( ar.GetFile( )->m_hFile );
	*/

	dxfgroup group;

	/*
	cin>>buffer;
	*/



	//use attach stream to a allready open file
	//==========================================================================

	//This funtion read contens of dxf file and produces tokens as output
	//Will use two layers:
	//1.Physical layer: processes raw data from file
	//1.1. Uses same names used in MFC CFile::Open,CFile::Read,etc
	//2.Link layer: producess tokens
	//2.1. Uses same names as class CDocument
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here

		//while(CFiledxf::ReadGroup()){
		//cin.width(1);???
		//Main variables, can be member variales
		//======================================================================
		streambuf *pstreambuf=cin.rdbuf();	//aux microsoft stream
		string sSection="";					//physical layer state variable
		string sEntity="";					//physical layer state variable
		list<float> listCoor;				//coor storing list
		string sCoorbuf;//("pru");					//coor storing alternative
		stringstream *sstreamCoor=new stringstream(sCoorbuf);
basic_stringbuf<char> *strbuf=sstreamCoor->rdbuf();
string str=strbuf->str();
		string sToken="";					//tokens
		DWORD dwTokenFlag;					//token flags
		float Xmax,Ymax,Zmax;
		//======================================================================

/*
SEQEND    No fields. This entity marks the end of vertices (Vertex 
          type name) for a Polyline, or the end of Attribute 
          entities (Attrib type name) for an Insert entity that has 
          Attributes (indicated by 66 group present and nonzero in 
          Insert entity).*/
		string sSequence="";		//end with SEQEND
		string sSubSequence="";

		//MAIN LOOP
		while(!cin.eof()){


			


			//Read a group
			//======================================================================

			cin>>group.group_code;

			int code_index=GetCodeIndex(group.group_code);

			int char_data=pstreambuf->snextc();

			/*
			if(char_data=='"'){
				char_data=pstreambuf->snextc();
			}
			*/


			if(char_data==10){//beware empty line!

				switch(code_index){
				case 0:
					group.idata=0;
					break;
				case 1:
					group.fdata=0.0;
					break;
				case 2:
					group.sdata="";
					break;
				}

			}else{


				//check empty string
				/*
				if((group.group_code)==1){
					cin.delbuf
					continue;
				}
				*/

				switch(code_index){
				case 0:
					cin>>group.sdata;
					group.idata=atoi(group.sdata.c_str());
					break;
				case 1:
					cin>>group.sdata;
					group.fdata=atof(group.sdata.c_str());
					break;
				case 2:
					cin>>group.sdata;
					char_data=pstreambuf->sgetc();
					while(char_data==32){
						string sdata;
						cin>>sdata;
						group.sdata+=" ";
						group.sdata+=sdata;
						char_data=pstreambuf->sgetc();
					}
					if(char_data!=10){
						AfxAbort();
					}

					break;
				default://unknown code
					cin>>group.sdata;
					break;
				}

			}

			//======================================================================


			//Physical layer Automata
			//======================================================================
			//======================================================================

			//Input
			if((group.group_code==0)&&(group.sdata=="SECTION")){
				sSection="";
			}
			if((group.group_code==0)||(group.group_code==9)){
				sEntity="";
			}
			if((group.group_code==0)&&(group.sdata=="SEQEND")){
				sSequence="";
			}
			if((group.group_code==0)&&(group.sdata=="EOF")){
				sSequence="EOF";
			}

			//Jump
			//======================================================================
			//Jump sublayer reconizes patterns in input stream and produces a
			//state jump.
			//Link layer has as many state variables as signals has input stream:
			//1.SECTIONS SIGNAL
			//2.ENTITIES SIGNAL
			//3.SEQUENCES SIGNAL
			//======================================================================
			//State variables:
			//sSection
			if((!sSection.size())&&(group.group_code==2)){
				sSection=group.sdata;
			}
			if((!sEntity.size())&&((group.group_code==0)||(group.group_code==9))){
				sEntity=group.sdata;
			}
			if(/*(!sSequence.size())&&*/(group.group_code==0)&&(group.sdata=="POLYLINE")){
				sSequence=group.sdata;
			}
			//Debug
/*
				if((group.group_code==100)||(group.group_code==0)){
					sSubSequence=group.sdata;
					if(sSequence=="AcDbTrace")sSequence="";
				}
				if(sSubSequence=="AcDbTrace"){
					sToken=sSubSequence;
					sSequence=sToken;
					if(group.group_code!=100){
						*sstreamCoor<<" ";
						*sstreamCoor<<group.sdata;
					}
				}
*/
			//Output: filter for next layer

			//Debug code
			if(group.sdata=="$DIMBLK"){
				int i=0;
			}
			if(group.sdata=="AcDbXrecord"){
				int i=0;
			}
			//======================================================================
			//======================================================================


			//Link layer Automata
			//Has an implicit syntactical analysis.
			//======================================================================
			if(sSection=="HEADER"){

				if(sEntity=="$EXTMAX"){

					switch(group.group_code){
					case 10:
						Xmax=group.fdata;
						break;
					case 20:
						Ymax=group.fdata;
						break;
					case 30:
						Zmax=group.fdata;
						break;
					}

				}


			}
			else if(sSection=="CLASSES"){

				int i=0;


			}
			else if((sSection=="ENTITIES")||(sSection=="BLOCKS")){
				if(sSequence=="POLYLINE"){
					if(sEntity=="POLYLINE"){
						sToken=sSequence;
						dwTokenFlag=0;
					}
					if(group.group_code==70){
						dwTokenFlag=group.idata;
					}
					if(sEntity=="VERTEX"){
						//Now program will store polyline data to produce output
						int i=0;
						switch(group.group_code){
						case 10:
						case 20:
						case 30:

							*sstreamCoor<<" ";
							*sstreamCoor<<group.sdata;
							break;

						}
					}
				}
			}
			/*
			else if(sSection=="BLOCKS"){

				int i=0;


			}
			*/
			else{

				int i=0;

			}

			//Output: filter for next layer
			if(sToken.size()&&(sToken!=sSequence)){
				sToken=sSequence;
				//Scale shapes
				//==============================================================
				float xmax=400.0;
				float ymax=400.0;
				if(m_pParent){
					POSITION pos=m_pParent->GetFirstViewPosition();
					CView *pview=m_pParent->GetNextView(pos);
					if(pview->IsKindOf(RUNTIME_CLASS(CTreeView))){
						pview=m_pParent->GetNextView(pos);
					}
					CRect rect;
					pview->GetClientRect(rect);
					xmax=rect.Width();
					ymax=rect.Height();
				}
				//==============================================================
				//Generate shapes
				string sdata;
				*sstreamCoor>>sdata;int x1=atof(sdata.c_str())*xmax/Xmax;
				*sstreamCoor>>sdata;int y1=ymax-    atof(sdata.c_str())  *ymax/Ymax;//-200.0;
				*sstreamCoor>>sdata;int z1=atof(sdata.c_str());
				int x0=x1;
				int y0=y1;
				int z0=z1;
				do{
					*sstreamCoor>>sdata;int x2=atof(sdata.c_str())*xmax/Xmax;
					*sstreamCoor>>sdata;int y2=ymax-    atof(sdata.c_str())*ymax/Ymax;//-200.0;
//debuf
if(y2<0){
	int i=0;
}
					*sstreamCoor>>sdata;int z2=atof(sdata.c_str());
					CShapeLine *pShLine=new CShapeLine();pShLine->m_Rect=CRect(CPoint(x1,y1),CPoint(x2,y2));
					m_pObArray->/*m_obaShape.*/Add(pShLine);
					x1=x2;
					y1=y2;
					z1=z2;
				}while(!sstreamCoor->eof());
				//Closing polyline
				if(dwTokenFlag&0x01){
					CShapeLine *pShLine=new CShapeLine();pShLine->m_Rect=CRect(CPoint(x1,y1),CPoint(x0,y0));
					m_pObArray->/*m_obaShape.*/Add(pShLine);
				}
				delete(sstreamCoor);
				sstreamCoor=new stringstream(sCoorbuf);
				//sCoorbuf="";
			}
			//======================================================================

			if(sSequence=="EOF"){
				return;
			}
		}




	}
}

/////////////////////////////////////////////////////////////////////////////
// PLexdoc commands

int CPLexdoc::GetCodeIndex(int group_code){

	int i=0;
	while(TableGroup_code_ranges[i][0]>=0){

		if((TableGroup_code_ranges[i][1]>=group_code)&&
			(TableGroup_code_ranges[i][0]<=group_code)){

			return TableGroup_code_ranges[i][2];
		}


	i++;}

	return -1;
}