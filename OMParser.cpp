// OMParser.cpp : implementation of the OMParser class & cmddeque class
// Object model parser
// 25/11/2004
// This parser will be able to process
// 'begin
// 'end
// blocks, like those used in SQL store procedures, to execute blocks of statements.

#include "stdafx.h"
#include "OMParser.h"

//This interfaces will be substituted by LPDISPATCH
#include "DraftDraw.h"
#include "DraftDrawDoc.h"

omobject g_OMObjectTable[]={
	{ddcParam,"parameter",0},
	{ddcBlockBegin,"begin",0},
	{ddcBlockEnd,"end",0},
	{ddcVar,"var",0},
	{ddcNew,"new",0},
	{ddcObject1ActiveSheet,"ActiveSheet",1},
	{ddcObject1ActiveDocument,"ActiveDocument",1},
	{ddcObject4Shapes,"Shapes",4},
	{ddcObject4Selection,"Selection",4},
	{ddcObject5AddShape,"AddShape",5},
	{ddcObject5SelectShape,"Select",5},
	{ddcObject5DeleteShape,"Delete",5},
	{ddcObject5ShapeRange,"ShapeRange",5},
	{ddcObject6IncrementLeft,"IncrementLeft",6},
	{ddcObject6IncrementTop,"IncrementTop",6},
	{ddcObject6ScaleWidth,"ScaleWidth",6},
	{ddcObject6ScaleHeight,"ScaleHeight",6},
	{ddcObjectNull,"",-1}
};

//Construtor
cmddeque::cmddeque(int nSize /*= 1024*/){

	m_nIndexCommQue=0;
	m_nIndexModified0=-1;
	m_bBackwards=false;
	m_nSize=nSize;
}

//Add a command to the queue
void cmddeque::push_cmd(std::string strComm,std::string strCommUndo,bool bModified /*=false*/){

	//Check index is not at the end
	if(m_nIndexCommQue!=m_dqCommands.size()){
		int idata=0;
		if(m_bBackwards){
			idata=m_dqCommands.size()-m_nIndexCommQue-1;
		}
		else{
			idata=m_dqCommands.size()-m_nIndexCommQue;
		}
		if(idata>0){
			std::deque<std::string>::iterator it=m_dqCommands.end()-idata;
			m_dqCommands.erase(it,m_dqCommands.end());
			it=m_dqCommandsUndo.end()-idata;
			m_dqCommandsUndo.erase(it,m_dqCommandsUndo.end());
			m_nIndexCommQue=m_dqCommands.size();
		}
		m_bBackwards=false;
	}

	//Check size
	//Causes exception
	check_size();

	//Add to command queues
	m_dqCommands.push_back(strComm);
	m_dqCommandsUndo.push_back(strCommUndo);

	//Set modified flag
	if(bModified){
		m_nIndexModified0=m_nIndexCommQue;
	}

	//Increment index
	m_nIndexCommQue+=1;

	//31/07/2005
	//Parse command
	theApp.m_OMParser.OMParse(strComm);
}

//Clear both command queues
void cmddeque::clear(){
	m_dqCommands.clear();
	m_dqCommandsUndo.clear();
	m_nIndexCommQue=0;
}

void cmddeque::check_size(){

	int nSize=m_dqCommands.size()>m_dqCommandsUndo.size()?m_dqCommands.size():m_dqCommandsUndo.size();
	while(nSize>m_nSize){

		//Declarations
		OMParser omParser;
		int idata2=0;

		//Parse undo commands
		std::deque<std::string>::iterator it=m_dqCommandsUndo.end();
		int nBlock=1;
		while(it>m_dqCommandsUndo.begin()&&nBlock>0){
			it--;
			nBlock=omParser.OMParse(*it,OMPF_JUSTSINTACTICAL);
			idata2++;
		}

		//Free memory
		int i;
		for(i=0;i<idata2 && m_dqCommands.size()>0;i++){
			m_dqCommands.pop_front();
		}

		//Free memory
		for(i=0;i<idata2 && m_dqCommandsUndo.size()>0;i++){
			m_dqCommandsUndo.pop_front();
		}

		//Recalculate size
		nSize=m_dqCommands.size()>m_dqCommandsUndo.size()?m_dqCommands.size():m_dqCommandsUndo.size();
		m_nIndexCommQue=nSize;
	}
}

//Check if document has been modified
bool cmddeque::is_modified(){

	if(!m_bBackwards){

		return (m_nIndexModified0!=-1&&m_nIndexCommQue>m_nIndexModified0);
	}
	else{

		return (m_nIndexModified0!=-1&&m_nIndexCommQue>=m_nIndexModified0);
	}
}

//Reset modified flag
void cmddeque::reset_modifiedflag(){

	if(!m_bBackwards){

		if(m_nIndexModified0!=-1){
			m_nIndexModified0=m_nIndexCommQue;
		}
	}
	else if(m_nIndexModified0!=-1){
		//Declarations
		OMParser omParser;
		int idata2=0;

		//Parse redo commands
		std::deque<std::string>::iterator it=m_dqCommands.begin()+m_nIndexCommQue;
		int nBlock=1;
		while(it<m_dqCommands.end()&&nBlock>0){
			it++;
			nBlock=omParser.OMParse(*it,OMPF_JUSTSINTACTICAL);
			idata2++;
		}

		m_nIndexModified0=m_nIndexCommQue+idata2;
	}
}

//Constructor
OMParser::OMParser (){

	m_nBlockDeep=0;
}

//Return codes (ms-dos convention):
//0	OK
//1 Error
int OMParser::OMParse (std::string sCommand,int flags/*=0*/){

	//Local variables
	std::list<omobject> liTargetObject;
	omobject *omObj;

	//LEXICAL ANALYSER
	//Break into tokens source command.
	//In this simple interpreter we only have two types of tokens:
	//objects
	//operators: .()=
	//**************************************************************************
	omobject Ob;
	std::string sCommBuf=sCommand;
	int nStart=0;
	if(sCommBuf.length()>0 && sCommBuf[0]=='\"'){
		nStart=sCommBuf.find_first_of("\",1");
	}
	std::size_t nIndex=sCommBuf.find_first_of(".()=",nStart);
	while(nIndex!=std::string::npos){
		std::string sOb=sCommBuf.substr(0,nIndex);
		if(sCommBuf[nIndex]==')'){
			sOb="("+sOb;
			if(nIndex<sCommBuf.length()-1){
				nIndex++;
			}
		}
		sCommBuf=sCommBuf.substr(nIndex+1,sCommBuf.length()-nIndex-1);
		
		//Obtain commands from name
		IdFromName(sOb,&Ob);
		//Add to list to process
		liTargetObject.push_back(Ob);
		if(sCommBuf.length()>0 && sCommBuf[0]=='\"'){
			nStart=sCommBuf.find_first_of("\"",1);
		}
		nIndex=sCommBuf.find_first_of(".()=",nStart);
	}
	//**************************************************************************
	IdFromName(sCommBuf,&Ob);
	liTargetObject.push_back(Ob);

	int size = liTargetObject.size();

	//Set up initial level context
	//**************************************************************************
	int nObLevelContext=1;
	//**************************************************************************

	//INTERPRETER
	//Target object selector
	//Interpreter
	//**************************************************************************

	//This point will be substituted by LPDISPATCH
	CDraftDrawDoc *pDoc=theApp.GetActiveDocument();
	int iObPrev=ddcObjectNull;
	CShape *pShTarget=NULL;

	//Iterate tokens obtained by lexical analyzer
	std::deque<CDBVariant*> *lpParametersPrev=NULL;//lpParameters;
	std::list<omobject>::iterator it;
	for(it=liTargetObject.begin();it!=liTargetObject.end();++it){

		std::deque<CDBVariant*> *lpParameters=NULL;
		try{
			lpParameters=(std::deque<CDBVariant*> *)(*it).lpParam;
		}
		catch(CException* pe)
		{
			char msg[255];
			pe->GetErrorMessage(msg, 255);
			TRACE("Exception in OMParse: " + CString(msg));
		}

		int iData=(*it).iObject;
		switch(iData){
		case ddcObjectNull:
			//m_nBlockDeep=0;
			//return 0;
			break;
		case ddcParam:
			iData=lpParameters->size();
			switch(iObPrev){
			case ddcObject5AddShape:{
				std::deque<CDBVariant*> *lpParam=NULL;
				if(lpParameters->size()>6){
					//It will add a shape that addmit parameters as polylines, etc.
					CDBVariant *dbvValue=(*lpParameters)[6];
					CString str=*(*lpParameters)[6]->m_pstring;
					str.TrimLeft(" ");
					str.TrimRight(" ");
					if(m_sblTable.find(std::string(str))!=m_sblTable.end()){
						lpParam=(std::deque<CDBVariant*> *)m_sblTable[std::string(str)]->lpParam;
					}
				}
				if(!(flags&OMPF_JUSTSINTACTICAL))pDoc->AddShape((*lpParameters)[0]->m_lVal,
					(*lpParameters)[1]->m_lVal,
					(*lpParameters)[2]->m_lVal,
					(*lpParameters)[3]->m_lVal,
					(*lpParameters)[4]->m_lVal,
					(*lpParameters)[5]->m_lVal,
					lpParam);
				}break;
			case ddcObject5SelectShape:
				if(!(flags&OMPF_JUSTSINTACTICAL))pDoc->SelectShapes((*lpParameters)[0]->m_lVal,(*lpParametersPrev)[0]->m_lVal);
				break;
			case ddcObject6IncrementLeft:
				if(!(flags&OMPF_JUSTSINTACTICAL)){
					if(pShTarget==NULL){
						pDoc->MoveShapes((*lpParameters)[0]->m_lVal,0);
					}
					else{
						pShTarget->Move((*lpParameters)[0]->m_lVal,0,FALSE);
					}
				}
				break;
			case ddcObject6IncrementTop:
				if(!(flags&OMPF_JUSTSINTACTICAL)){
					if(pShTarget==NULL){
						pDoc->MoveShapes(0,(*lpParameters)[0]->m_lVal);
					}
					else{
						pShTarget->Move(0,(*lpParameters)[0]->m_lVal,FALSE);
					}
				}
				break;
			case ddcObject6ScaleWidth:
				if(!(flags&OMPF_JUSTSINTACTICAL))pDoc->ScaleShapes((*lpParameters)[0]->m_lVal,0,(*lpParameters)[2]->m_lVal);
				break;
			case ddcObject6ScaleHeight:
				if(!(flags&OMPF_JUSTSINTACTICAL))pDoc->ScaleShapes(0,(*lpParameters)[0]->m_lVal,(*lpParameters)[2]->m_lVal);
				break;
			//25/07/2005
			case ddcVar:
				if(!(flags&OMPF_JUSTSINTACTICAL)){
					omObj->lpParam=new std::deque<CDBVariant*>;
						*((std::deque<CDBVariant*>*)omObj->lpParam)=*lpParameters;
				}
				break;
			//02/10/2005
			case ddcObject4Shapes:{
				int idata=(*lpParameters)[0]->m_lVal;
				int nIndex=pDoc->FindShape( (*lpParameters)[0]->m_lVal );
				if(nIndex>=0) pShTarget=(CShape *)(*pDoc->m_pObArray)[nIndex];
				}break;
			}
			lpParametersPrev=lpParameters;
			break;
		case ddcBlockBegin:
			m_nBlockDeep++;
			return m_nBlockDeep;
			break;
		case ddcBlockEnd:
			m_nBlockDeep--;
			return m_nBlockDeep;
			break;
		case ddcObject1ActiveSheet:
			break;
		case ddcObject5AddShape:
			iObPrev=ddcObject5AddShape;
			break;
		case ddcObject5SelectShape:
			iObPrev=ddcObject5SelectShape;
			break;
		case ddcObject5DeleteShape:
			//iData=lpParametersPrev->size();
			if(!(flags&OMPF_JUSTSINTACTICAL))pDoc->DeleteShape((*lpParametersPrev)[0]->m_lVal);
			break;
		case ddcObject6IncrementLeft:
			iObPrev=ddcObject6IncrementLeft;
			break;
		case ddcObject6IncrementTop:
			iObPrev=ddcObject6IncrementTop;
			break;
		case ddcObject6ScaleWidth:
			iObPrev=ddcObject6ScaleWidth;
			break;
		case ddcObject6ScaleHeight:
			iObPrev=ddcObject6ScaleHeight;
			break;
		//25/07/2005
		case ddcVar:{
			std::deque<CDBVariant*> *lpParameters=(std::deque<CDBVariant*> *)(*it).lpParam;
			iObPrev=ddcVar;
			CDBVariant *variant=(*lpParameters)[0];
			CString str=*variant->m_pstring;
			std::map<std::string,omobject*>::iterator it=m_sblTable.find(std::string(str));
			if(it!=m_sblTable.end()){
				omObj=it->second;
			}
			else{
				omObj=new omobject;
				strncpy(omObj->sLabel,str,MAX_PATH);
				m_sblTable[std::string(str)]=omObj;
			}
			}break;
		case ddcNew:
			break;
		//02/10/2005
		case ddcObject4Shapes:
			iObPrev=ddcObject4Shapes;
			break;
		}//end switch

	}//end for

	
	//**************************************************************************


	//FINAL CLEAN-UP
//OMPARSER_CLEANUP:
	for(it=liTargetObject.begin();it!=liTargetObject.end();++it){

		std::deque<CDBVariant*> *lpParameters=(std::deque<CDBVariant*> *)(*it).lpParam;

		if(lpParameters){

			delete(lpParameters);
		}
	}


	//Return code
	return m_nBlockDeep;
}

//Obtain commands from name
BOOL OMParser::IdFromName(std::string sName,omobject *lpOb){

	int nIndex=0;
	BOOL bResult=FALSE;
	lpOb->lpParam=NULL;

	//Check '('
	if(sName.length()>0 && sName[0]=='('){
		//prepare storage for parameters
		lpOb->lpParam=new std::deque<CDBVariant*>;

		//remove '('
		sName=sName.substr(1,sName.length()-1);

		//store parameters
		CDBVariant *dbvValue;
		int nIndex=sName.find_first_of(",");
		std::string sValue;
		if(nIndex!=std::string::npos){
			sValue=sName.substr(0,nIndex);
			sName=sName.substr(nIndex+1,sName.length()-nIndex-1);
		}
		else{
			sValue=sName;
			sName="";
		}
		while(sValue.length()>0){

			dbvValue=new CDBVariant;
			if(sValue.find_first_not_of(" -+0123456789")==std::string::npos){
				dbvValue->m_dwType=DBVT_LONG;
				dbvValue->m_lVal=atoi(sValue.c_str());
			}
			else{
				dbvValue->m_dwType=DBVT_STRING;
				CString *pstr=new CString(sValue.c_str());
				pstr->TrimLeft("\"");
				pstr->TrimRight("\"");
				dbvValue->m_pstring=pstr;
				
			}
			((std::deque<CDBVariant*>*)lpOb->lpParam)->push_back(dbvValue);

			nIndex=sName.find_first_of(",");
			if(nIndex!=std::string::npos){
				sValue=sName.substr(0,nIndex);
				sName=sName.substr(nIndex+1,sName.length()-nIndex-1);
			}
			else{
				sValue=sName;
				sName="";
			}
		}

		//prepare for normal flow
		sName="parameter";
	}


	//Objects normal flow
	std::string sParam;
	int index1=sName.find(" ");
	if(index1!=std::string::npos){
		int index2=sName.find_first_not_of(" ",index1);
		if(index2!=std::string::npos){
			sParam=sName.substr(index2,sName.length()-index2);
		}
		sName=sName.substr(0,index1);
	}
	while(g_OMObjectTable[nIndex].iObject!=ddcObjectNull){
		if(strcmp(g_OMObjectTable[nIndex].sLabel,sName.c_str())==0){
			bResult=TRUE;
			break;
		}
		nIndex++;
	}
	lpOb->iObject=g_OMObjectTable[nIndex].iObject;
	strcpy(lpOb->sLabel,g_OMObjectTable[nIndex].sLabel);
	lpOb->iLevel=g_OMObjectTable[nIndex].iLevel;
	if(sParam.length()>0){

		CDBVariant *dbvValue=new CDBVariant;
		lpOb->lpParam=new std::deque<CDBVariant*>;
		dbvValue->m_dwType=DBVT_STRING;
		dbvValue->m_pstring=new CString(sParam.c_str());
		((std::deque<CDBVariant*>*)lpOb->lpParam)->push_back(dbvValue);
	}

	//Return result
	return bResult;
}