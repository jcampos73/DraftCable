// DraftDrawDoc.cpp : implementation of the CDraftDrawDoc class
//

#include "stdafx.h"
#include <io.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include "UtilsCPP.h"
#include "DraftDraw.h"

#include "DraftDrawDoc.h"
#include "ChildFrm.h"
#include "TBEditView.h"
#include "XMLArchive.h"

//Dialog
#include "NetAlias.h"
#include "DialogPin.h"
#include "DialogPlacePart.h"
#include "DialogPageProperties.h"
#include "DialogPatchPProperties.h"
#include "PageSize.h"
#include "PageGrid.h"
#include "DialogConfig.h"
#include "DialogText.h"
#include "NameDlg.h"
#include "DialogNetList.h"
#include "DialogSaveasDB.h"

#include "Shape.h"
#include "ShapeUnit.h"

//Support
#include "fileCore.h"
#include "GbrioFile.h"
#include "CDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define _ARRAY_STG

#define _DCABLE_DATABASEDLG_TREE

//"18;W311;Flex STP CAT 5E PG;J1;J0D;CHASIS SSIF 1R;CHASIS SSIOL 1R;RACK SICAM;RACK SICAM;RJ 45 M apantalldo;RJ 45 M apantalldo;;cable516.txt;4;\n"
char CABLFILE_LINE[256]={
"18;%s;Flex STP CAT 5E PG;%s;%s;%s;%s;%s;%s;%s;%s;;%s;4;\n"
};
char BRIDGEFILE_LINE[256]={
"%s;%s;%s;%s;\n"
};

CString afxWinwordPath="C:\\Archivos de programa\\Microsoft Office\\Office";

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc

IMPLEMENT_DYNCREATE(CDraftDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CDraftDrawDoc, CDocument)
	//{{AFX_MSG_MAP(CDraftDrawDoc)
	ON_COMMAND(ID_BUTTON_RECT, OnButtonRect)
	ON_COMMAND(ID_BUTTON_ELLIPSE, OnButtonEllipse)
	ON_COMMAND(ID_BUTTON_LINE, OnButtonLine)
	ON_COMMAND(ID_BUTTON_ARC, OnButtonArc)
	ON_COMMAND(ID_BUTTON_BOARD, OnButtonBoard)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_ARCHIVO_GENCABLE, OnArchivoGencable)
	ON_COMMAND(ID_TOOL_OPTION_PAGE, OnToolOptionPage)
	ON_COMMAND(ID_TOOL_CONFIG, OnToolConfig)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_TOOL_SIC, OnToolSic)
	ON_COMMAND(ID_FILE_SAVE_SHEET_CURRENT, OnFileSaveSheetCurrent)
	ON_COMMAND(ID_PLACE_SHEETS, OnPlaceSheets)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_SELECT_NET, OnSelectNet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc construction/destruction

CDraftDrawDoc::CDraftDrawDoc()
{
	// TODO: add one-time construction code here
	CWinApp *app=AfxGetApp();

	m_pObArrayTemp=NULL;
	m_pObArray=NULL;
	m_ObObArray.Add(&m_ObArray);
	m_pObArray=&m_ObArray;
	cmdDeque=&m_cmdDeque;
	m_nCountId=1;			//Counter for id's is incremented by AddObject, InsertObject and never decremented
	m_nCountIdPart=1;		//Counter for id's is incremented by AddObject, InsertObject and never decremented
	m_pnCountId=&m_nCountId;
	m_szDesign=GetDefaultSize();
	//04/12/2004
	//Allocation of maps
	mapShapeIdtoObj_t *pmapShapeIdtoObj=new mapShapeIdtoObj_t;
	m_mapShapeIdtoObjArray.push_back(pmapShapeIdtoObj);
	m_pmapShapeIdtoObj=pmapShapeIdtoObj;
	//26/12/2004
	//Allocation of screen arrays
	CShArray *pShArray=new CShArray(m_szDesign,CSize(100,100),this);		//Beware CShArray must be resized whe resizing schematic.
	m_ShArrayArray.push_back(pShArray);
	m_pShArray=pShArray;
	m_ShArrayPart.Create(m_szDesign,CSize(100,100));


	m_nIndexSheet=0;
	m_bModified1=FALSE;
	m_bFlagPartEdit=FALSE;


	m_CursorArray[8]=app->LoadCursor(IDC_CURSOR_4W);
	m_CursorArray[9]=LoadCursor(NULL,IDC_ARROW);	

	m_CursorArray[0]=app->LoadCursor(IDC_CURSOR_NW);
	m_CursorArray[1]=app->LoadCursor(IDC_CURSOR_NE);
	m_CursorArray[2]=m_CursorArray[0];
	m_CursorArray[3]=m_CursorArray[1];
	m_CursorArray[4]=app->LoadCursor(IDC_CURSOR_NS);
	m_CursorArray[5]=app->LoadCursor(IDC_CURSOR_EW);
	m_CursorArray[6]=m_CursorArray[4];
	m_CursorArray[7]=m_CursorArray[5];

	m_CursorArray[10]=app->LoadCursor(IDC_DRAW);
	m_CursorArray[11]=app->LoadCursor(IDC_PENCIL);

	m_CursorArray[12]=app->LoadCursor(IDC_ZOOM_PLUS);
	m_CursorArray[13]=app->LoadCursor(IDC_ZOOM_MINUS);
	m_CursorArray[14]=app->LoadCursor(IDC_ROTATE);

	m_pDocRack=NULL;
	m_iIdPart=0;

	
}

CDraftDrawDoc::~CDraftDrawDoc()
{
	//Clean up
	int i;
	for(i=0;i<m_obaCables1.GetSize();i++){

		CStringArray *pstaArray=m_obaCables1[i];
		delete(pstaArray);
	}
	for(i=0;i<m_obaCables2.GetSize();i++){

		CStringArray *pstaArray=m_obaCables2[i];
		delete(pstaArray);
	}

	//04/12/2004
	//Clean content of maps.
	for(std::deque<mapShapeIdtoObj_t*>::iterator it=m_mapShapeIdtoObjArray.begin();it!=m_mapShapeIdtoObjArray.end();++it){
		mapShapeIdtoObj_t *pmapShapeIdtoObj=*it;
		delete(pmapShapeIdtoObj);
	}
	//26/12/2004
	//Clean content of screen arrays.
	for(std::deque<CShArray*>::iterator it1=m_ShArrayArray.begin();it1!=m_ShArrayArray.end();++it1){
		CShArray *pShArray=*it1;
		delete(pShArray);
	}
	//17/05/2004
	//Clean 'TB' editor support structures.
	CString iKey;
	void *iValue;
	for(POSITION pos=m_mapStrHWParameter.GetStartPosition();pos!=NULL;){
		m_mapStrHWParameter.GetNextAssoc(pos,iKey,iValue);
		CIntArray *piaParameter=(CIntArray *)iValue;
		delete(piaParameter);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc statical members

//std::string CDraftDrawDoc::m_sFilter = _DRAFTCABLE_DOC_FILTER_STR;//"Draft Draw Files (*.ddw)|*.ddw|Gerber Files (*.gbr)|*.gbr|Cut Files (*.cut)|*.cut|Draft Draw Files 1 (*.dd1)|*.dd1|"; //All Files (*.*)|*.*||;

BOOL CDraftDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	// This function is mainly concerned with template operations.

	m_pObArray=&m_ObArray;
	cmdDeque=&m_cmdDeque;
	m_bFlagPartEdit=FALSE;//no part edition
	m_iTypeDocument=_DOCTYPE_SCHEMATIC;//default doc type is schematic
	m_fVer=0.0;
	m_szGrid=CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT);//initial grid

	m_iToolSel=_TOOLSELECT_DRAFTCABLE;

	//A4 size in 0.1 millimeter
	//m_szDesign=CSize(2970,2100);
	m_szDesign=CSize(1485,1050);

	//Load initial title block: bottom right

	CShapeUnit *pShUnit=new CShapeUnit();
	//pShUnit->LoadUnit("Standard.title.ddw");
	pShUnit->LoadUnit("Standard100.title");

	//Control database status
	if(g_db.IsOpen()){
		g_db.Close();
	}

	pShUnit->m_pCursorArray=m_CursorArray;
	pShUnit->m_Mode=_DRAFTDRAW_MODE_DRW;

	pShUnit->m_Rect+=(m_szDesign-pShUnit->m_Rect.Size()-CSize(DCABLE_GRIDX_DEFAULT,DCABLE_GRIDY_DEFAULT));

	//Add shape to stack
	AddObject(pShUnit);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc serialization

void CDraftDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//Storing code

		//Svg file (under development)
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_SVG){

			//First loop for calculate sheet size dynamically
			CRect rect_tot;
			int index;
			CShape *pSh = (CShape *)FirstObject(index);
			pSh = (CShape *)NextObject(index);
			while (pSh != NULL){

				rect_tot.UnionRect(rect_tot, CRect(CPoint(0,0), pSh->m_Rect.BottomRight()));
				pSh = (CShape *)NextObject(index);
			}

			CSvgioFile svgfile(&ar, rect_tot.Size());

			//Serilize every shapes
			pSh = (CShape *)FirstObject(index);
			pSh = (CShape *)NextObject(index);
			while (pSh != NULL){

				pSh->SerializeSvg(svgfile);
				pSh = (CShape *)NextObject(index);
			}
		}
		//Gerber file.
		//**********************************************************************
		else if(nFilterIndex==_DRAFTCABLE_DOC_FILTER_GERBER){

			CGbrioFile gbrfile(&ar);

			//gbrfile.Open(ar.m_strFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeText);

			int index;
			CShape *pSh=(CShape *)FirstObject(index);
			pSh=(CShape *)NextObject(index);

			while(pSh!=NULL){

				pSh->SerializeGbr(gbrfile);
				pSh=(CShape *)NextObject(index);
			}

		}
		//Normal flow using old CDdwioFile.
		//Parts & Schematics.
		//**********************************************************************
		else if(nFilterIndex==_DRAFTCABLE_DOC_FILTER_DDW){

			for(int i=0;i<GetSheetCount();i++){

				//Check if sheet is to be serialized
				if(m_shList.GetCount()>0){
					if(!m_shList.Find(i)) continue;
				}

				//Add code to iterate shapes in several sheets
				if(GetSheetCount()>1){
					ChangeSheet(i);
				}

				CDdwioFile ddwfile((CArchiveDb *)&ar);

				//gbrfile.Open(ar.m_strFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeText);

				int index;
				CShape *pSh=(CShape *)FirstObject(index);
				pSh=(CShape *)NextObject(index);

				while(pSh!=NULL){

					pSh->SerializeDdw(ddwfile);
					pSh=(CShape *)NextObject(index);
				}

				pSh=(CShape *)FirstObject(index);
				pSh=(CShape *)NextObject(index);

				while(pSh!=NULL){

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
						

						CShapeContainer *pShContainer=(CShapeContainer *)pSh;

						pShContainer->CShapeContainer::SerializeDdw(ddwfile);
					}

					pSh=(CShape *)NextObject(index);
				}

				//Sheet marker
				if(GetSheetCount()>1&&i<GetSheetCount()-1){
					ddwfile.WriteSheet(NULL);
				}
			}
		}
		//Store loop for binary files:
		//Parts & Schematics
		//**********************************************************************
		else if(nFilterIndex==_DRAFTCABLE_DOC_FILTER_DD1){

			//Set global index to 0
			g_aShBufIndex=0;

			//Check if we are editing a part
			if(!m_bFlagPartEdit){
				//Check if we are not serializing all the sheets
				if(GetSheetSerialCount()>0){
					ar<<GetSheetSerialCount();
				}
				else{
					ar<<GetSheetCount();//Send to archive number of sheet
				}
			}

			//Temporal buffer
			CObArray obaBuffer;

			int i;
			for(i=0;i<GetSheetCount();i++){

				//Check if sheet is to be serialized
				if(m_shList.GetCount()>0){
					if(!m_shList.Find(i)) continue;
				}

				//Code to iterate shapes in several sheets
				if(GetSheetCount()>1){
					ChangeSheet(i);
				}

				//If we are editing a part seriaze first a shape unit heading
				if(m_bFlagPartEdit){
					CShapeUnit ShUnit;
					ShUnit.m_Rect=CRect(0,0,100,100);
					ShUnit.m_sUnitName=GetTitle();
					ShUnit.Serialize(ar);
				}

				//Main serialization loop
				int index;
				CShape *pSh=(CShape *)FirstObject(index);
				pSh=(CShape *)NextObject(index);
				int idata=GetSizeObject();
				ar<<idata;
				
				CRect rcFrm=CRect(0,0,0,0);
				int nCounter=0;
				while(pSh!=NULL){
					//27/03/2005
					//Debuging. Delete ASAP.
					//----------------------------------------------------------
					/*
					int debug_cont=0;//for debugging
					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
						CShapeUnit *pShUnit=(CShapeUnit *)pSh;
						if(pShUnit->m_obarrShapearr.GetSize()>1000){
							debug_cont=1;
						}
					}
					if(debug_cont){
						pSh=(CShape *)NextObject(index);
						continue;
					}
					*/
					//----------------------------------------------------------

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
						CShapePolyline *pShPoly = new CShapePolyline();
						CRect rectPrev = pSh->m_Rect;
						pSh->m_Rect -= rcFrm.TopLeft();
						pShPoly->Create((CShapeRect*)pSh);
						pSh->m_Rect = rectPrev;

						obaBuffer.Add(pShPoly);
						ar<<pShPoly;
						pSh = pShPoly;
					}
					else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){
						rcFrm=pSh->m_Rect;
						m_fUTemp=pSh->m_Rect.Height();
						m_fUTemp/=DCABLE_GRID_US;
						//07/01/2006
						//Shapes appeared with offset at placing into schematics.
						//This code is not ok, becouse
						//when re-editing the shape, it is moved towards origin.
						//It is better to do it in CShapeUnit::Serialize when
						//archive is loading.
						//pSh->m_Rect-=rcFrm.TopLeft();
						ar<<pSh;
					}
					else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){
						*((CShapeLabel*)pSh)->m_Label.rect=CRect((pSh->m_Rect-rcFrm.TopLeft()).TopLeft(),CSize(0,0));
						pSh->m_Rect-=rcFrm.TopLeft();
						ar<<pSh;
					}
					else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
						pSh->m_Rect-=rcFrm.TopLeft();
						ar<<pSh;
						pSh->m_Rect+=rcFrm.TopLeft();
						//05/04/2005
						//Added to store path panels if applicable
						//This must be substituted by a function call
						//to determine if a shape is of type 'TB'.
						//======================================================
						CString strFile=((CShapeUnit*)pSh)->m_sUnitName;
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

						if (str.CompareNoCase(_T("TB")) == 0){

							//SerializePatchP(ar,((CShapeUnit*)pSh)->m_uiShapeId);
						}
						//======================================================
					}
					else{
						pSh->m_Rect-=rcFrm.TopLeft();
						ar<<pSh;
						pSh->m_Rect+=rcFrm.TopLeft();
					}

					//07/04/2005
					m_fVer=pSh->m_fVer;


					nCounter++;
					pSh=(CShape *)NextObject(index);
				}//end while loop

				nCounter=nCounter;
			}

			//05/04/2005
			//Save TB's

			float fVer0=1.05;
			float fData=m_fVer-fVer0;
			if(fData>=0){
				int sz=m_obaCables1.GetSize();
				ar<<m_obaCables1.GetSize();
				int i;
				for(i=0;i<m_obaCables1.GetSize();i++){
					CStringArray* pstrA;
					pstrA=m_obaCables1[i];
					ar<<pstrA;
				}
				ar<<m_obaCables2.GetSize();
				for(i=0;i<m_obaCables2.GetSize();i++){
					CStringArray* pstrA;
					pstrA=m_obaCables2[i];
					ar<<pstrA;
				}
				//ar<<&m_mapIdHWIndex;
			}


			//Clean up
			for(i=0;i<obaBuffer.GetSize();i++){
				delete(obaBuffer[i]);
			}
			obaBuffer.RemoveAll();
			for(i=0;i<g_aShBuffer.GetSize();i++){
				delete(g_aShBuffer[i]);
			}
			g_aShBuffer.RemoveAll();

		}
		//Export part as bmp,jpg,gif.
		//Only for parts.
		//**********************************************************************
		else if(nFilterIndex>_DRAFTCABLE_DOC_ARCHIVED_COUNT){

			if(m_bFlagPartEdit){

				//savebmp(hbm,hpal,saCodec[nFilterIndex-1]);
				POSITION pos=GetFirstViewPosition();
				CView *pView=GetNextView(pos);

				if(pView->IsKindOf(RUNTIME_CLASS(CTreeView))){
					pView=GetNextView(pos);
				}

				int index;
				CShape *pSh=(CShape *)FirstObject(index);
				pSh=(CShape *)NextObject(index);
				while(pSh){

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){

						CPoint pt=pSh->m_Rect.TopLeft();

						BOOL bError=!ClientToScreen(pView->m_hWnd,&pt);

						//DWORD dwError=GetLastError();

						HBITMAP hBitmap = CopyScreenToBitmap( &(pSh->m_Rect+(pt-pSh->m_Rect.TopLeft())) );

						HPALETTE hPal;
						if (hBitmap)
						hPal = GetSystemPalette();
						else
						hPal = NULL;

						CFile *pFile=ar.GetFile();
						CString strPath=GetPathName();//pFile->GetFilePath();

						wstring wstr=saCodecs[nFilterIndex-1];

						savebmp(hBitmap,hPal,saCodecs[nFilterIndex-1].c_str(),strPath+"1");

						break;
					}

					pSh=(CShape *)NextObject(index);
				}
			}

		}
	}//End of storing code
	//Loading code here
	else
	{
		//Svg file (under development)
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_SVG){

			//Old implementation for loading from files
			//Just in case XML serialization cannot made work
			/*
			char buffer[256];
			buffer[0] = 0;
			CFile *pfile = ar.GetFile();
			fini = fopen(pfile->GetFilePath(), "rb");
			fgets(buffer, 255, fini);
			*/

			XMLCLASSNODE;
			//CXMLArchive& xmlArchive = static_cast<CXMLArchive&>(ar);

			CXMLArchiveNode* curNodePtr = xmlArchive.GetCurrentNode();
			int numberObjects = curNodePtr->GetNoChildren();
			for (int i = 0; i < numberObjects; i++, curNodePtr->GetNextChildIndex())
			{
				CShapePolyline *path = new CShapePolyline();
				XMLDATA(*path);
				AddObject(path);
			}

			//curNodePtr = xmlArchive.GetCurrentNode();
			xmlArchive.ResetMainPos();
			curNodePtr = xmlArchive.GetCurrentNode();
			numberObjects = curNodePtr->GetNoChildren();
			for (int i = 0; i < numberObjects; i++, curNodePtr->GetNextChildIndex())
			{
				CShapeLabel *text = new CShapeLabel();
				XMLDATA(*text);
				AddObject(text);
			}

			//XMLDATA(m_itemArray);
			//XMLDATA(m_singleItem);
			//XMLENDNODE;
		}
		//Binary format
		//Loading for place is perfomed on CShapeUnit::LoadUnit()
		//**********************************************************************
		else if(nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1){

			int idata=1;
			//02/07/2005
			//Code used for reconstruct connection in document insert operations.
			std::map<int,int> *mapIdtoId=NULL;
			int nIndex0=GetSizeObject();

			//Check sheet count
			if(!m_bFlagPartEdit){
				ar>>idata;//retrieve sheet count
			}

			//Iterate all the sheets
			for(int i=0;i<idata;i++){

				//code to iterate shapes in several sheets
				if(i>0 || nIndex0){
					int nIndex=AddSheet();
					ChangeSheet(nIndex);
					CMDIFrameWnd *pMFrame=(CMDIFrameWnd *)AfxGetMainWnd();
					CChildFrame* pChldFrame=(CChildFrame*)pMFrame->MDIGetActive();
					CString str;
					str.Format("Hoja %i",nIndex+1);
					pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().AddItem(str);
					pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().RecomputeLayout();
					pChldFrame->m_wndFolderFrame.ShowControls(CFolderFrame::bestFit);
					pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().SelectItem(nIndex);
				}

				//This is a trick to bypass unit heading when serializing for part editor.
				if(m_bFlagPartEdit){
					CShapeUnit ShUnit;//CShapeContainer ShUnit;//!!!
					ShUnit.m_bFlagPartEdit=TRUE;
					ShUnit.Serialize(ar);
				}

				//Retrive shape count
				int nCount;
				ar>>nCount;

				//Local variables
				CShape *pSh;
				CRect rcFrm=CRect(0,0,0,0);

				//Shape loading block
				for(int i=0;i<nCount;i++){

					//Load shape
					ar>>pSh;

					//07/04/2005
					m_fVer=pSh->m_fVer;

					//Work with frame rectangle
					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){
						rcFrm=pSh->m_Rect;
					}
					else{
						pSh->m_Rect+=rcFrm.TopLeft();
					}

					pSh->m_pCursorArray=m_CursorArray;
					pSh->pcmdDeque=cmdDeque;

					//10/01/2004
					//Added to store pointer to patch panel editor.
					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
						((CShapeUnit*)pSh)->mf_cbPatchPEdit=SDisplayPatchPProp;
						//05/04/2005
						//Added to store path panels if applicable
						//This must be substituted by a function call
						//to determine if a shape is of type 'TB'.
						//======================================================
						CString strFile=((CShapeUnit*)pSh)->m_sUnitName;
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

						if (str.CompareNoCase(_T("TB")) == 0){

							//SerializePatchP(ar,((CShapeUnit*)pSh)->m_uiShapeId);
						}
						//======================================================
					}

					//02/07/2005
					//Code used for reconstruct connection in document insert operations.
					int prev_id=pSh->m_uiShapeId;

					if(!nIndex0){
						AddObject(pSh,pSh->m_uiShapeId);
					}
					else{
						AddObject(pSh);
					}

					//02/07/2005
					//Code used for reconstruct connection in document insert operations.
					if(pSh->m_uiShapeId!=prev_id){
						if(!mapIdtoId){
							mapIdtoId=new std::map<int,int>;
						}
						(*mapIdtoId)[prev_id]=pSh->m_uiShapeId;
					}
				}

				//05/04/2005
				//Load TB's
				float fVer0=1.05;
				float fData=m_fVer-fVer0;
				if(fData>=0){
					int nCount;
					ar>>nCount;
					for(int i=0;i<nCount;i++){
						CStringArray* pstrA;
						ar>>pstrA;
						m_obaCables1.Add(pstrA);
					}
					ar>>nCount;
					for(int i=0;i<nCount;i++){
						CStringArray* pstrA;
						ar>>pstrA;
						m_obaCables2.Add(pstrA);
					}
					/*
					CObject *pObj;
					ar>>pObj;
					CMap<int,int,int,int>*pMap=(CMap<int,int,int,int>*)pObj;

					int iKey,iValue;
					for(POSITION pos=pMap->GetStartPosition( );pos!=NULL;pMap->GetNextAssoc(pos,iKey,iValue)){
						m_mapIdHWView[iKey]=0;
						m_mapIdHWIndex[iKey]=iValue;
					}
					*/
				}

				//rebuild connections

				int index=nIndex0;
				pSh=(CShape *)FirstObject(index);
				pSh=(CShape *)NextObject(index);
				while(pSh){
				

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
						
						CShapeContainer *pShCont=(CShapeContainer *)pSh;

						//int debug_count=pShCont->m_obarrShapearr.GetSize();

						pShCont->DoPasteConnections(mapIdtoId,m_pmapShapeIdtoObj);

						//debug_count=pShCont->m_obarrShapearr.GetSize();
					}
					else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){

						pSh->DoPasteConnections(mapIdtoId,m_pmapShapeIdtoObj);
					}

					pSh=(CShape *)NextObject(index);
				}
			}

			//Clean up
			//02/07/2005
			//Code used for reconstruct connection in document insert operations.
			if(mapIdtoId){
				delete(mapIdtoId);
			}
		}
		//DXF format import.
		//**********************************************************************
		else if(nFilterIndex==4){

			int nIndex0=m_pObArray->GetUpperBound();
			m_plexProcessor->Serialize(ar);

			int nIndex=m_pObArray->GetUpperBound();

			for(int i=(nIndex0+1);i<=nIndex;i++){
				CShape *pSh=(CShape *)(*m_pObArray)[i];
				pSh->m_pCursorArray=m_CursorArray;
				pSh->m_Mode=1;
			}


			//delete(plexDoc);
		}
		//Normal flow using old CDdwioFile.
		//**********************************************************************
		else if(nFilterIndex==1){

			//Schematics
			if(!m_bFlagPartEdit){

				//old code, commented 17/04/2004
				/*
				CShape *pShp;
				CShapeUnit *pShpUnit=new CShapeUnit();
				pShp=(CShape *)pShpUnit->LoadUnit(ar.m_strFileName,&ar);
				pShp->m_pCursorArray=m_CursorArray;
				pShp->m_Mode=1;
				AddObject(pShp);
				delete pShpUnit;
				UpdateAllViews(NULL);
				*/

				//Local buffer
				char buffer[256];
				buffer[0]=0;

				CFile *pfile=ar.GetFile();

				fini=fopen(pfile->GetFilePath(),"rb");

				fgets(buffer,255,fini);

				char *pchar=strchr(buffer,'\n');
				*pchar=0;

				CShapeUnit *shShapeUnit;
				CShapeWire *shShapeWire;
				CShapeBus *shShapeBus;
				CShapeLabel *shShapeLabel;

				int track=-1;
				CString sTrackName;
				int nIndex;
				while(strlen(buffer)!=0){

					switch(track){

					case 0:
						if(DDXF_Unit(FALSE,buffer,sTrackName,&shShapeUnit)){

							track=0;
							sTrackName=shShapeUnit->m_sUnitName;

							shShapeUnit->m_pCursorArray=m_CursorArray;
							shShapeUnit->m_Mode=1;
							shShapeUnit->mf_cbPatchPEdit=SDisplayPatchPProp;

							nIndex=AddObject(shShapeUnit);
							m_MapShapeIdtoObj[nIndex]=shShapeUnit;
						}
						else{

							track=-1;
						}
						break;

					case 1:
						if(DDXF_Wire(FALSE,buffer,&shShapeWire)){

							track=1;

							shShapeWire->m_pCursorArray=m_CursorArray;
							shShapeWire->m_Mode=1;

							nIndex=AddObject(shShapeWire);
							m_MapShapeIdtoObj[nIndex]=shShapeWire;
						}
						else{

							track=-1;
						}
						break;

					case 4:
						if(DDXF_Bus(FALSE,buffer,&shShapeBus)){

							track=4;

							shShapeBus->m_pCursorArray=m_CursorArray;
							shShapeBus->m_Mode=1;

							nIndex=AddObject(shShapeBus);
							m_MapShapeIdtoObj[nIndex]=shShapeBus;
						}
						else{

							track=-1;
						}
						break;
						
					case 2:
						if(DDXF_Connect(FALSE,buffer,&m_MapShapeIdtoObj)){

							track=2;
						}
						else{

							track=-1;
						}
						break;

					case 3:
						if(DDXF_Label(FALSE,buffer,&shShapeLabel)){

							track=3;

							shShapeLabel->m_pCursorArray=m_CursorArray;
							shShapeLabel->m_Mode=1;

							nIndex=AddObject(shShapeLabel);
							m_MapShapeIdtoObj[nIndex]=shShapeLabel;
						}
						else{

							track=-1;
						}
						break;

					default:
						if(DDXF_Unit(FALSE,buffer,sTrackName,&shShapeUnit)){

							track=0;
							sTrackName=shShapeUnit->m_sUnitName;

							shShapeUnit->m_pCursorArray=m_CursorArray;
							shShapeUnit->m_Mode=1;
							shShapeUnit->mf_cbPatchPEdit=SDisplayPatchPProp;

							nIndex=AddObject(shShapeUnit);
							m_MapShapeIdtoObj[nIndex]=shShapeUnit;
						}
						else if(DDXF_Wire(FALSE,buffer,&shShapeWire)){

							track=1;

							shShapeWire->m_pCursorArray=m_CursorArray;
							shShapeWire->m_Mode=1;

							nIndex=AddObject(shShapeWire);
							m_MapShapeIdtoObj[nIndex]=shShapeWire;
						}
						else if(DDXF_Bus(FALSE,buffer,&shShapeBus)){

							track=4;

							shShapeBus->m_pCursorArray=m_CursorArray;
							shShapeBus->m_Mode=1;

							nIndex=AddObject(shShapeBus);
							m_MapShapeIdtoObj[nIndex]=shShapeBus;
						}
						else if(DDXF_Connect(FALSE,buffer,&m_MapShapeIdtoObj)){

							track=2;
						}
						else if(DDXF_Label(FALSE,buffer,&shShapeLabel)){

							track=3;

							shShapeLabel->m_pCursorArray=m_CursorArray;
							shShapeLabel->m_Mode=1;

							nIndex=AddObject(shShapeLabel);
							m_MapShapeIdtoObj[nIndex]=shShapeLabel;
						}
						else if(DDXF_Sheet(FALSE,buffer)){

							track=-1;

							int nIndex=AddSheet();

							ChangeSheet(nIndex);


							CMDIFrameWnd *pMFrame=(CMDIFrameWnd *)AfxGetMainWnd();

							CChildFrame* pChldFrame=(CChildFrame*)pMFrame->MDIGetActive();

							CString str;
							str.Format("Hoja %i",nIndex+1);
							pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().AddItem(str);

							pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().RecomputeLayout();

							pChldFrame->m_wndFolderFrame.ShowControls(CFolderFrame::bestFit);

							pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().SelectItem(nIndex);
						}
						//Not neccesary to call this function because
						//spare are units by default
						/*
						else if(DDXF_Spare(FALSE,buffer)){

							track=-1;
						}
						*/
						break;
					}
				}//end while

				fclose(fini);

			}
			//Parts
			else{


				char buffer[256];
				buffer[0]=0;


				//To load parts from db
				//CArchiveDb ardb(NULL,CArchive::load   );
				//CDatabase db;
				//std::stringstream g_sout;
				//g_bFlagDb=FALSE;//Set to true if part is stored in database
				g_bFlagDbEnable=TRUE;//Set to true if database storage is enabled


/*
				//21/12/2004
				//Try to find part in database
				if(g_bFlagDbEnable){
					if(!g_db.IsOpen()){
						TCHAR sConnect[1024];

						g_GetConnectString(sConnect,1024);


						//Connect to database

						g_db.OpenEx(sConnect);
					}

					CRecordset rsConnector(&g_db);

					rsConnector.m_strFilter="nNamePart='"+CString(strFile)+"' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
					rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");
					
					CString str;

					if(!rsConnector.IsEOF()){

						bFlagDb=TRUE;

						rsConnector.GetFieldValue("nTextPart",str);
					}


					sout=std::stringstream(ios_base::in);

					sout.str(std::string(str));

				}
*/

				CFile *pfile=ar.GetFile();

				//old code sample 1
				/*
				int handle=_dup(pfile->m_hFile);

				if(errno==EBADF){

					int i=0;

				}
				*/
				//old code sample 2
				/*
				fini=fdopen(pfile->m_hFile,"r");
				*/

				//old code sample 3
				/*
				fini=fopen(pfile->GetFilePath(),"rb");

				fgets(buffer,255,fini);
				char *pchar=strchr(buffer,'\n');
				*pchar=0;
				*/


				CShape *shShapePolyline;
				CShapeFrmRect *shShapeFrmRect;
				CShapeEllipse *shShapeEllipse;
				CShapeLabel *shShapeLabel;
				CShapePin *shShapePin;


				//21/12/2004
				//Open file
				if(g_bFlagDb&&g_bFlagDbEnable){

					g_parTheArch=&ar;
				}
				else{

					fini=fopen(pfile->GetFilePath(),"rb");
					if(!fini){
						::AfxMessageBox("No se puede cargar.",MB_ICONSTOP|MB_OK);
						return;
					}
				}


				//21/12/2004
				//Read string from database or file
				CString stline;
				std::string stdline;
				if(g_bFlagDb&&g_bFlagDbEnable){

					ar.ReadString(stline);
/*
					g_sout>>stdline;
					stline=CString(stdline.c_str());
*/
					if(stline.IsEmpty()){
						::AfxMessageBox("Archivo vacío.",MB_ICONSTOP|MB_OK);
						return;
					}

					strncpy(buffer,stline,255);
					buffer[254]=0;
				}
				else{

					if(!fgets(buffer,255,fini)){
						::AfxMessageBox("Archivo vacío.",MB_ICONSTOP|MB_OK);
						return;
					}

					char *pchar=strchr(buffer,'\n');
					*pchar=0;
				}

				int track=-1;
				while(strlen(buffer)!=0){

					switch(track){

					case 0:
						if(DDXF_Polyline(FALSE,buffer,&shShapePolyline)){

							track=0;

							shShapePolyline->m_pCursorArray=m_CursorArray;
							shShapePolyline->m_Mode=1;
							shShapePolyline->m_Rect+=CPoint(100,100);

							AddObject(shShapePolyline);
						}
						else{

							track=-1;
						}
						break;
					case 1:
						if(DDXF_FrmRect(FALSE,buffer,&shShapeFrmRect)){

							track=1;

							shShapeFrmRect->m_pCursorArray=m_CursorArray;
							shShapeFrmRect->m_Mode=1;
							shShapeFrmRect->m_Rect+=CPoint(100,100);

							AddObject(shShapeFrmRect);
						}
						else{

							track=-1;
						}
						break;

					case 2:
						if(DDXF_Label(FALSE,buffer,&shShapeLabel)){

							track=2;

							shShapeLabel->m_pCursorArray=m_CursorArray;
							shShapeLabel->m_Mode=1;
							shShapeLabel->m_Rect+=CPoint(100,100);

							AddObject(shShapeLabel);
						}
						else{

							track=-1;
						}
						break;
					case 3:
						if(DDXF_Pin(FALSE,buffer,&shShapePin)){

							track=3;

							shShapePin->m_pCursorArray=m_CursorArray;
							shShapePin->m_Mode=1;
							shShapePin->m_Rect+=CPoint(100,100);

							AddObject(shShapePin);
						}
						else{

							track=-1;
						}
						break;
					case 4:
						if(DDXF_Ellipse(FALSE,buffer,&shShapeEllipse)){

							track=4;

							shShapeEllipse->m_pCursorArray=m_CursorArray;
							shShapeEllipse->m_Mode=1;
							shShapeEllipse->m_Rect+=CPoint(100,100);

							AddObject(shShapeEllipse);
						}
						else{

							track=-1;
						}
						break;

					default:
						if(DDXF_Polyline(FALSE,buffer,&shShapePolyline)){

							track=0;

							shShapePolyline->m_pCursorArray=m_CursorArray;
							shShapePolyline->m_Mode=1;
							shShapePolyline->m_Rect+=CPoint(100,100);

							AddObject(shShapePolyline);
						}
						else if(DDXF_FrmRect(FALSE,buffer,&shShapeFrmRect)){

							track=1;

							shShapeFrmRect->m_pCursorArray=m_CursorArray;
							shShapeFrmRect->m_Mode=1;
							shShapeFrmRect->m_Rect+=CPoint(100,100);

							AddObject(shShapeFrmRect);
						}
						else if(DDXF_Label(FALSE,buffer,&shShapeLabel)){

							track=2;

							shShapeLabel->m_pCursorArray=m_CursorArray;
							shShapeLabel->m_Mode=1;
							shShapeLabel->m_Rect+=CPoint(100,100);

							AddObject(shShapeLabel);
						}
						else if(DDXF_Pin(FALSE,buffer,&shShapePin)){

							track=3;

							shShapePin->m_pCursorArray=m_CursorArray;
							shShapePin->m_Mode=1;
							shShapePin->m_Rect+=CPoint(100,100);

							AddObject(shShapePin);
						}
						else if(DDXF_Ellipse(FALSE,buffer,&shShapeEllipse)){

							track=4;

							shShapeEllipse->m_pCursorArray=m_CursorArray;
							shShapeEllipse->m_Mode=1;
							shShapeEllipse->m_Rect+=CPoint(100,100);

							AddObject(shShapeEllipse);
						}
						else if(DDXF_Spare(FALSE,buffer)){

							track=-1;
						}
						break;

					}
				}//end while

				if(!g_bFlagDb||!g_bFlagDbEnable){
					fclose(fini);
				}

			}//end else flag part edit
		}
		//Binary format.
		//Obsolete code.
		//Delete.
		//**********************************************************************
		else if(nFilterIndex==5){

			int nCount;
			CShape *pSh;
			ar>>nCount;
			int nIndex;

			for(int i=0;i<nCount;i++){

				ar>>pSh;

				pSh->m_pCursorArray=m_CursorArray;
				pSh->m_Mode=1;

				nIndex=AddObject(pSh);
				m_MapShapeIdtoObj[nIndex]=pSh;
			}
		}

		//Control database status
		if(g_db.IsOpen()){
			g_db.Close();
		}

	}

	//09/04/2005
	//Load TB's
	float fVer0=1.05;
	float fData=m_fVer-fVer0;
	if(fData>=0){

		//m_mapIdHWView.Serialize(ar);
		m_mapIdHWIndex.Serialize(ar);

		if(ar.IsLoading()){

			int iKey;
			int iValue;
			for(POSITION pos=m_mapIdHWIndex.GetStartPosition();pos!=NULL;){

				m_mapIdHWIndex.GetNextAssoc(pos,iKey,iValue);
				m_mapIdHWView[iKey]=0;
			}
		}
	}
	fVer0=1.06;
	fData=m_fVer-fVer0;
	if(fData>=0){

		//m_mapStrHWIndex.Serialize(ar);
		//m_mapStrHWParameter.Serialize(ar);

		CString iKey;
		void *iValue;
		if(ar.IsStoring()){
			ar<<m_mapStrHWIndex.GetCount();
			POSITION pos;
			for(pos=m_mapStrHWIndex.GetStartPosition();pos!=NULL;){
				m_mapStrHWIndex.GetNextAssoc(pos,iKey,iValue);
				ar<<iKey;
				ar<<(DWORD)iValue;
			}
			ar<<m_mapStrHWParameter.GetCount();
			for(pos=m_mapStrHWParameter.GetStartPosition();pos!=NULL;){
				m_mapStrHWParameter.GetNextAssoc(pos,iKey,iValue);
				ar<<iKey;
				((CIntArray*)iValue)->Serialize(ar);
			}
		}
		else{
			int nCount;
			ar>>nCount;
			CString iKey;
			DWORD iValue;
			CIntArray *iValue1;
			int i;
			for(i=0;i<nCount;i++){
				ar>>iKey;
				ar>>iValue;
				m_mapStrHWIndex[iKey]=(void*)iValue;
			}
			ar>>nCount;
			for(i=0;i<nCount;i++){
				ar>>iKey;
				iValue1=new CIntArray;
				iValue1->Serialize(ar);
				m_mapStrHWParameter[iKey]=(void*)iValue1;
			}
		}

		if(ar.IsLoading()){

			CString iKey;
			void *iValue;
			for(POSITION pos=m_mapStrHWIndex.GetStartPosition();pos!=NULL;){

				m_mapStrHWIndex.GetNextAssoc(pos,iKey,iValue);
				m_mapStrHWView[iKey]=0;
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc diagnostics

#ifdef _DEBUG
void CDraftDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDraftDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawDoc commands

CObject * CDraftDrawDoc::NextObject(int& index )
{
#ifdef _LIST_STG
return m_ObList.GetNext(rPosition);

#endif
#ifdef _ARRAY_STG
if(index>m_pObArray->GetUpperBound()){
	//index++;
	return NULL;
}

	CObject *ob=m_pObArray->GetAt(index);
	index++;
	return ob;
#endif
}

CObject * CDraftDrawDoc::PrevObject(int& index )
{
/*
return m_ObList.GetPrev(rPosition);
*/
if(index<0){
	//index--;
	return NULL;
}

	CObject *ob=m_pObArray->GetAt(index);
	index--;
	return ob;
}

CObject * CDraftDrawDoc::FirstObject(int &index)
{

#ifdef _LIST_STG

	if(m_ObList.IsEmpty()){

		rPosition=NULL;
		return NULL;
	}

	rPosition=m_ObList.GetHeadPosition();
	return m_ObList.GetAt(rPosition);
#endif
#ifdef _ARRAY_STG

	if(m_pObArray->GetSize()==0){

		index=NULL;
		return NULL;

	}

	index=0;
	return m_pObArray->GetAt(0);

#endif
}

CObject * CDraftDrawDoc::EndObject()
{
return NULL;
}

int CDraftDrawDoc::AddObject(CObject *newElement,UINT nId /*=0*/)
{
#ifdef _LIST_STG

	SetModifiedFlag(TRUE);
	return m_ObList.AddTail(newElement);

#endif

#ifdef _ARRAY_STG

	int nIndex=m_pObArray->Add(newElement);
	//((CShape *)newElement)->m_uiShapeId=nIndex+1;

	//Manage command queues
	//All code to push commands into queues is being moved to CShape implementation.
	/*
	CString str;
	str.Format("ActiveSheet.Shapes.AddShape(%i, %i, %i, %i, %i, %i).Select",
		ddcShapeRectangle,
		((CShape *)newElement)->m_Rect.left,
		((CShape *)newElement)->m_Rect.top,
		((CShape *)newElement)->m_Rect.right,
		((CShape *)newElement)->m_Rect.bottom,
		((CShape *)newElement)->m_uiShapeId);
	std::string strCommand=str;
	m_dqCommands.push_back(strCommand);
	
	str.Format("ActiveSheet.Shapes(%i).Select.Delete",((CShape *)newElement)->m_uiShapeId);
    strCommand=str;
	m_dqCommandsUndo.push_back(strCommand);
	m_nIndexCommQue+=1;
	*/

	//26/01/2005
	//Add shape to screen map
/*
#ifdef DCABLE_SHARRAY
	if(newElement->IsKindOf( RUNTIME_CLASS( CShape )) ){
		m_pShArray->AddShape((CShape *)newElement);
	}
#endif
*/

	//05/12/2004
	//Increment counter for shape identification
	if(nId>0){
		(*m_pmapShapeIdtoObj)[nId]=newElement;
		//28/02/2004
		//This bug is very important because when deserializing shapes
		//*m_pnCountId can be corrupted.
		*m_pnCountId=max(*m_pnCountId,nId+1);
	}
	else{
		(*m_pmapShapeIdtoObj)[*m_pnCountId]=newElement;
		((CShape *)newElement)->m_uiShapeId=*m_pnCountId;
		(*m_pnCountId)++;
	}

	//SetModifiedFlag(TRUE);
	return nIndex;
	

#endif
}

void CDraftDrawDoc::DeleteObject(int index, BOOL bDoNotRemoveFromArray /*= FALSE*/)
{
#ifdef _LIST_STG
	if(pos==NULL){

		m_ObList.RemoveTail( );
	}

#endif

#ifdef _ARRAY_STG
	if (index == -1){
		index = m_pObArray->GetUpperBound();
	}

/*
	if(index==-1){

		index=m_pObArray->GetUpperBound();
		CShape *pSh=(CShape *)(*m_pObArray)[index];

		if (pSh == NULL){
			return;
		}

		//26/01/2005
		//Delete shape from screen map
#ifdef DCABLE_SHARRAY
		m_pShArray->DeleteShape((CShape *)pSh);
#endif
		mapShapeIdtoObj_t::iterator it=m_pmapShapeIdtoObj->find(pSh->m_uiShapeId);
		if(it!=m_pmapShapeIdtoObj->end()){
			m_pmapShapeIdtoObj->erase(it);
		}

		//WARNING!
		//======================================================================
		if(m_pSh==pSh){
			m_pSh=NULL;
		}
		//======================================================================

		if (bDoNotRemoveFromArray == FALSE){
			m_pObArray->RemoveAt(index);
		}
		else{
			(*m_pObArray)[index] = NULL;
		}

		//Code for deleting of copies
		for (int i = m_pObArray->GetSize() - 1; i > index; i--){
			CShape *pShTemp = (CShape *)(*m_pObArray)[i];
			if (pShTemp == pSh){
				//Make and ASSERT becouse having copies in array is not good
				if (bDoNotRemoveFromArray == FALSE){
					m_pObArray->RemoveAt(i);
				}
				else
				{
					(*m_pObArray)[i] = NULL;
				}
			}
		}

		delete(pSh);

		return;
	}
*/

	CShape *pSh=(CShape *)(*m_pObArray)[index];

	if (pSh == NULL){
		return;
	}

	//26/01/2005
	//Delete shape from screen map
#ifdef DCABLE_SHARRAY
	m_pShArray->DeleteShape((CShape *)pSh);
#endif
	mapShapeIdtoObj_t::iterator it=m_pmapShapeIdtoObj->find(pSh->m_uiShapeId);
	if(it!=m_pmapShapeIdtoObj->end()){
		m_pmapShapeIdtoObj->erase(it);
	}

	//WARNING!
	//======================================================================
	if(m_pSh==pSh){
		m_pSh=NULL;
	}
	//======================================================================

	if (bDoNotRemoveFromArray == FALSE){
		m_pObArray->RemoveAt(index);
	}
	else{
		(*m_pObArray)[index] = NULL;
	}

	//Code for deleting of copies
	for (int i = m_pObArray->GetSize() - 1; i >= index; i--){
		CShape *pShTemp = (CShape *)(*m_pObArray)[i];
		if (pShTemp == pSh){
			//TRACE becouse having copies in array is not good
			TRACE("Duplicated Shape in m_pObArray at pos %i, while deleting %i\n", i, index);
			if (bDoNotRemoveFromArray == FALSE){
				m_pObArray->RemoveAt(i);
			}
			else
			{
				(*m_pObArray)[i] = NULL;
			}
		}
	}

	//Delete shape
	delete(pSh);

#endif
}


CObject * CDraftDrawDoc::LastObject(int& index)
{
#ifdef _LIST_STG
	if(m_ObList.IsEmpty()){

		return NULL;
	}

	return m_ObList.GetTail();
#endif
#ifdef _ARRAY_STG

	index=m_pObArray->GetUpperBound();
	if(index<0){

		return NULL;
	}
	return m_pObArray->GetAt(index);
#endif
}

int CDraftDrawDoc::InsertObject(int index, CObject *newElement,UINT nId /*=0*/)
{
	m_pObArray->InsertAt(index,newElement);

	//26/01/2005
	//Add shape to screen map
#ifdef DCABLE_SHARRAY
	if(newElement->IsKindOf( RUNTIME_CLASS( CShape )) ){
		m_pShArray->AddShape((CShape *)newElement);
	}
#endif

	//05/12/2004
	//Increment counter for shape identification
	if(nId>0){
		(*m_pmapShapeIdtoObj)[nId]=newElement;
	}
	else{
		(*m_pmapShapeIdtoObj)[*m_pnCountId]=newElement;
		(*m_pnCountId)++;
	}

	//SetModifiedFlag(TRUE);
	return index;

}

int CDraftDrawDoc::GetSizeObject(){

	return m_pObArray->GetSize();
}

void CDraftDrawDoc::OnButtonRect() 
{
	// TODO: Add your command handler code here
	/*
	if(m_pSh){

		delete(m_pSh);
	}
	*/
	m_pSh=new CShapeRect(NULL,0,cmdDeque);
	m_pSh->m_pCursorArray=m_CursorArray;
	
}

void CDraftDrawDoc::DeleteContents() 
{
	//Beware this code is called for parts & schematics!
	//This function is mainly concerned with cleaning all structures allocated in
	//Document constructor.
	//While OnNewDocument must initiallize them acting as template or loading templates
	//indeed?

	m_pSh=NULL;
	m_iCursor=-1;
	m_ptPasteOffset=CPoint(25,25);
	m_szGrid=CSize(10,10);//initial grid
	m_iTypeDocument=_DOCTYPE_SCHEMATIC;
	m_fVer=0.0;
	m_bModified1=FALSE;

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CPLexdoc );
	CObject* pObject = pRuntimeClass->CreateObject();
	ASSERT( pObject->IsKindOf( RUNTIME_CLASS( CPLexdoc ) ) );


	m_plexProcessor=(CPLexdoc *)pObject;
	m_plexProcessor->m_pObArray=m_pObArray;
	m_plexProcessor->m_pParent=this;


	//Delete contents of shape array
	if(m_pObArray!=NULL){
		for(int i=0;i<m_pObArray->GetSize();i++){
			DeleteObject(i, TRUE);
		}
		m_pObArray->RemoveAll();
	}

	//Set command queue to 0
	/*
	m_dqCommands.clear();
	m_dqCommandsUndo.clear();
	m_nIndexCommQue=0;
	*/
	cmdDeque->clear();

	//04/12/2004
	//Clean content of maps.
	std::deque<mapShapeIdtoObj_t*>::iterator it;
	for(it=m_mapShapeIdtoObjArray.begin()+1;it!=m_mapShapeIdtoObjArray.end();++it){
		mapShapeIdtoObj_t *pmapShapeIdtoObj=*it;
		delete(pmapShapeIdtoObj);
	}
	m_mapShapeIdtoObjArray.erase(m_mapShapeIdtoObjArray.begin()+1,m_mapShapeIdtoObjArray.end());
	it=m_mapShapeIdtoObjArray.begin();
	m_pmapShapeIdtoObj=*it;
	m_pmapShapeIdtoObj->clear();
	m_mapShapeIdtoObjPart.clear();
	//If document is of kind part edit document, following OnNewDocument can change m_pmapShapeIdtoObj to m_mapShapeIdtoObjPart.

	//26/01/2005
	//Clean content of screen arrays
	std::deque<CShArray*>::iterator it1;
	for(it1=m_ShArrayArray.begin()+1;it1!=m_ShArrayArray.end();++it1){
		CShArray *pShArray=*it1;
		delete(pShArray);
	}
	m_ShArrayArray.erase(m_ShArrayArray.begin()+1,m_ShArrayArray.end());
	it1=m_ShArrayArray.begin();
	m_pShArray=*it1;
	m_pShArray->RemoveAll();
	m_ShArrayPart.RemoveAll();
	//If document is of kind part edit document, following OnNewDocument can change m_pShArray to m_ShArrayPart.

	//17/05/2004
	//Clean 'TB' editor support structures.
	CString iKey;
	void *iValue;
	for(POSITION pos=m_mapStrHWParameter.GetStartPosition();pos!=NULL;){
		m_mapStrHWParameter.GetNextAssoc(pos,iKey,iValue);
		CIntArray *piaParameter=(CIntArray *)iValue;
		piaParameter->RemoveAll();
	}

	//19/06/2005
	//Remove all sheets from list of serialization.
	DoListSheetSave(NULL);

	//Call base class funtion
	//The default implementation of this function does nothing.
	CDocument::DeleteContents();
}

void CDraftDrawDoc::OnButtonEllipse() 
{
	// TODO: Add your command handler code here
	m_pSh=new CShapeEllipse(NULL,0,cmdDeque);
	m_pSh->m_pCursorArray=m_CursorArray;
}

void CDraftDrawDoc::OnButtonLine() 
{
	// TODO: Add your command handler code here
	m_pSh=new CShapeLine(NULL,0,cmdDeque);
	m_pSh->m_pCursorArray=m_CursorArray;
}

void CDraftDrawDoc::OnButtonArc() 
{
	// TODO: Add your command handler code here
	m_pSh=new CShapeArc();
	m_pSh->m_pCursorArray=m_CursorArray;
}

void CDraftDrawDoc::OnButtonBoard() 
{
	// TODO: Add your command handler code here
	m_pSh=new CShapeBoard();
	m_pSh->m_pCursorArray=m_CursorArray;
}

void CDraftDrawDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
#ifdef DCABLE_PART_DB_BIN
	/*
	if(m_bFlagPartEdit){

		OnDatabaseSave();
		return;
	}
	*/
#endif

	if(GetPathName( ).GetLength()>0){

		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW);
		AfxGetMainWnd()->PostMessage(WM_COMMAND,ID_FILE_OPEN);
		return;
	}


	//static char BASED_CODE szFilter[] = "Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
	static char BASED_CODE szFilter[] = _DRAFTCABLE_DOC_FILTER_STR"All Files (*.*)|*.*||";
	static char BASED_CODE szExt[] = "*.dd1";

	CFileDialog fdialog(
		TRUE,		//open dialog box
		NULL,
		szExt,
		0,			//no flags
		szFilter,
		NULL);

	if(fdialog.DoModal()==IDOK){


		nFilterIndex=fdialog.m_ofn.nFilterIndex;

		OnOpenDocument(fdialog.m_ofn.lpstrFile);

		SetPathName(fdialog.m_ofn.lpstrFile);

		if(!m_bFlagPartEdit)
		ChangeSheet(0);

		UpdateAllViews(NULL,1);
	}
	
}

void CDraftDrawDoc::OnFileSave() 
{
	// TODO: Add your command handler code here

	//Remove all sheets from list of serialization.
	DoListSheetSave(NULL);


#ifdef DCABLE_PART_DB_BIN
	if(m_bFlagPartEdit){

		OnPartSave();
		//OnDatabaseSave();
		return;
	}
#endif


	CString strFile=GetPathName();

	if(strFile.IsEmpty()){

		OnFileSaveAs();
	}
	else{

		OnSaveDocument(strFile);
	}
	
}


void CDraftDrawDoc::OnFileSaveAs() 
{
	//Command handler code here

#ifdef DCABLE_PART_DB_BIN
	if(m_bFlagPartEdit){

		OnPartSaveAs();
		return;
	}
#endif


	//Prepare file extensions filter string
	CStringArray saExtensions;
	static std::string sFilter;
	DoFileExtensions(sFilter,saExtensions);


	CString strFile;

	int result;

	do{

	CString str=this->m_strTitle + ".dd1";
	static char BASED_CODE szExt[] = "*.dd1";

	CFileDialog fdialog(
		FALSE,		//save dialog box
		NULL,
		szExt,
		0,			//no flags
		sFilter.c_str(),
		NULL);

	result=IDYES;

	if(fdialog.DoModal()==IDOK){

		int offset=fdialog.m_ofn.nFileExtension;

		strFile=fdialog.m_ofn.lpstrFile;

		nFilterIndex=fdialog.m_ofn.nFilterIndex;

		if((!offset)||(fdialog.m_ofn.lpstrFile[offset]==0)){
/*
			switch(fdialog.m_ofn.nFilterIndex){

			case 1:

				strFile+=".ddw";
				break;

			case 2:

				strFile+=".gbr";
				break;

			case 3:

				strFile+=".cut";
				break;

			}
*/
			strFile+="."+saExtensions[fdialog.m_ofn.nFilterIndex-1];
			sFilter=saExtensions[fdialog.m_ofn.nFilterIndex-1];
		}


		//check file exists
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;

		hFind = FindFirstFile(strFile, &FindFileData);


		if (hFind != INVALID_HANDLE_VALUE){

			result=AfxMessageBox("File allready exists, overwrite?",MB_YESNOCANCEL,0);

		}

		if(result==IDYES){

			SetPathName(strFile,FALSE);

			OnSaveDocument(strFile);

			SetPathName(strFile,TRUE);
		}

	}

	}while(result==IDNO);

	
}

void CDraftDrawDoc::OnPartSave() 
{
	//Part save command handler code here

	CString strFile=GetPathName1();

	if(strFile.IsEmpty()){

		OnPartSaveAs();
	}
	else{

		OnDatabaseSave(strFile);
	}
	
}

//Implement save part as dialog
void CDraftDrawDoc::OnPartSaveAs() 
{
	// TODO: Add your command handler code here

	CString strFile;

#ifndef _DCABLE_DATABASEDLG_TREE
	CNameDlg dlg;
	dlg.m_Name=strFile;
	if (dlg.DoModal() == IDCANCEL || dlg.m_Name == "")
	{
		return;
	}
	strFile=dlg.m_Name;
#else
	CDialogSaveasDB dlg;
	dlg.m_Name=strFile;
	dlg.m_ofn.lpstrFilter=_DRAFTCABLE_DOC_FILTER_STR;//m_sFilter.c_str();
	if (dlg.DoModal() == IDCANCEL )//|| dlg.m_Name == "")
	{
		return;
	}
	strFile=dlg.m_Name;
#endif

	int result=IDYES;

	//TODO: code to implement check if part exists
	/*
	if (hFind != INVALID_HANDLE_VALUE){

		result=AfxMessageBox("File allready exists, overwrite?",MB_YESNOCANCEL,0);

	}
	*/

	if(result==IDYES){

		SetPathName1(strFile,TRUE);

		OnDatabaseSave(strFile);
	}



	
}

BOOL CDraftDrawDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	HCURSOR hCursor;
	
	if(pExtra==NULL){

		CNetAlias netalias;
		CDialogPin pin;

		int uiShapeType=0;//to pass shape type to second phase
		int idata;		//to pass an integer value from 1st phase to 2nd phase
		POINT *Points;	//to pass a array of pints to 2nd phase

		//1st phase
		switch(nID){
			case IDS_MENU_POLYGON_TRIANG:
				uiShapeType=ddcShapeTriangle;
				idata=3;
				break;
			case IDS_MENU_POLYGON_PENT:
				uiShapeType=ddcShapePentagon;
				idata=5;
				break;
			case IDS_MENU_POLYGON_HEX:
				uiShapeType=ddcShapeHexagon;
				idata=6;
				break;
			case IDS_MENU_POLYGON_OCT:
				uiShapeType=ddcShapeOctogon;
				idata=8;
				break;
			case IDS_MENU_POLYGON_NGEAR:
				uiShapeType=ddcShapeNGear;
				idata=6;
				break;
			case IDS_MENU_POLYGON_NSTAR:
				uiShapeType=ddcShapeNStar;
				idata=6;
				break;
			case IDS_MENU_POLYGON_NPOLYGON:
				uiShapeType=ddcShapeNPolygon;
				idata=7;
				break;
			case IDS_MENU_POLYGON_NCROSS:
				uiShapeType=ddcShapeNCross;
				idata=6;
				break;

			case IDS_MENU_POLYGON_PARALEL1:
				idata=5;
				Points=new POINT[idata];
				Points[0]=CPoint(0,0);
				Points[1]=CPoint(100,0);
				Points[2]=CPoint(150,150);
				Points[3]=CPoint(50,150);
				Points[4]=CPoint(0,0);
				break;
			case IDS_MENU_POLYGON_TRAP:
				idata=5;
				Points=new POINT[idata];
				Points[0]=CPoint(25,0);
				Points[1]=CPoint(125,0);
				Points[2]=CPoint(150,150);
				Points[3]=CPoint(0,150);
				Points[4]=CPoint(25,0);
				break;
			case IDS_MENU_POLYGON_ROMBO:
				idata=5;
				Points=new POINT[idata];
				Points[0]=CPoint(75,0);
				Points[1]=CPoint(150,75);
				Points[2]=CPoint(75,150);
				Points[3]=CPoint(0,75);
				Points[4]=CPoint(75,0);
				break;
			case IDS_MENU_POLYGON_CROSS:
				idata=13;
				Points=new POINT[idata];
				Points[0]=CPoint(0,50);
				Points[1]=CPoint(50,50);
				Points[2]=CPoint(50,0);
				Points[3]=CPoint(100,0);
				Points[4]=CPoint(100,50);
				Points[5]=CPoint(150,50);
				Points[6]=CPoint(150,100);
				Points[7]=CPoint(100,100);
				Points[8]=CPoint(100,150);
				Points[9]=CPoint(50,150);
				Points[10]=CPoint(50,100);
				Points[11]=CPoint(0,100);
				Points[12]=CPoint(0,50);
				break;
			case IDS_MENU_POLYGON_T:
				uiShapeType=ddcShapeT;
				idata=9;
				Points=new POINT[idata];
				Points[0]=CPoint(0,50);
				Points[1]=CPoint(150,50);
				Points[2]=CPoint(150,100);
				Points[3]=CPoint(100,100);
				Points[4]=CPoint(100,150);
				Points[5]=CPoint(50,150);
				Points[6]=CPoint(50,100);
				Points[7]=CPoint(0,100);
				Points[8]=CPoint(0,50);
				break;
			case IDS_MENU_POLYGON_L:
				uiShapeType=ddcShapeL;
				idata=7;
				Points=new POINT[idata];
				Points[0]=CPoint(50,0);
				Points[1]=CPoint(100,0);
				Points[2]=CPoint(100,50);
				Points[3]=CPoint(150,50);
				Points[4]=CPoint(150,100);
				Points[5]=CPoint(50,100);
				Points[6]=CPoint(50,0);
				break;
			case IDS_MENU_POLYGON_STRAIGHT:
				idata=5;
				Points=new POINT[idata];
				Points[0]=CPoint(50,0);
				Points[1]=CPoint(100,0);
				Points[2]=CPoint(100,150);
				Points[3]=CPoint(50,150);
				Points[4]=CPoint(50,0);
				break;

		}

		//2nd phase
		switch(nID){

		case IDS_MENU_POLYGON_TRIANG:
		case IDS_MENU_POLYGON_PENT:
		case IDS_MENU_POLYGON_HEX:
		case IDS_MENU_POLYGON_OCT:
		case IDS_MENU_POLYGON_NGEAR:
		case IDS_MENU_POLYGON_NSTAR:
		case IDS_MENU_POLYGON_NPOLYGON:
		case IDS_MENU_POLYGON_NCROSS:
		{
			//Following members should be placed in a constructor!
			m_pSh=new CShapeUnit(NULL,0,cmdDeque);
			m_pSh->m_bNoResize=FALSE;

			CShapeUnit *pShUnit=(CShapeUnit *)m_pSh;

			//m_pSh=new CShapePolyline();
			CShapePolyline ShPoly;//=(CShapePolyline *)m_pSh;
			if(nID==IDS_MENU_POLYGON_NCROSS){
				int idata1=25;
				pShUnit->CreateNCross(CRect(0,0,150,150),idata,idata1);
			}
			else if(nID==IDS_MENU_POLYGON_NGEAR){
				int idata1=25;
				pShUnit->CreateNGear(CRect(0,0,150,150),idata,idata1);
			}
			else if(nID==IDS_MENU_POLYGON_NSTAR){
				int idata1=40;
				pShUnit->CreateNStar(CRect(0,0,150,150),idata,idata1);

			}
			else{
				pShUnit->Create(CRect(0,0,150,150),idata,uiShapeType);
			}

			pShUnit->SetupBuffers(CRect(0,0,150,150));
			m_pSh->m_pCursorArray=m_CursorArray;

			m_iCursor=10;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			return TRUE;

			}break;

		case IDS_MENU_POLYGON_PARALEL1:
		case IDS_MENU_POLYGON_TRAP:
		case IDS_MENU_POLYGON_ROMBO:
		case IDS_MENU_POLYGON_CROSS:
		case IDS_MENU_POLYGON_T:
		case IDS_MENU_POLYGON_L:
		case IDS_MENU_POLYGON_STRAIGHT:
			{
			//Following members should be placed in a constructor!
			m_pSh=new CShapeUnit(NULL,0,cmdDeque);
			m_pSh->m_bNoResize=FALSE;

			CShapeUnit *pShUnit=(CShapeUnit *)m_pSh;

			pShUnit->m_pPoints=(int **)GlobalAlloc(GMEM_ZEROINIT,sizeof(int*)*_DEFAULTMAXPLINE_DRAFTCABLE);
			pShUnit->m_pPoints[0]=(int *)GlobalAlloc(GMEM_ZEROINIT,sizeof(int)*2*(idata+1));//storage per number of vertex+1 to close the polygon

			pShUnit->m_pPoints[0][0]=idata;
			pShUnit->m_PointspCount=1;

			//m_pSh=new CShapePolyline();
			CShapePolyline ShPoly;//=(CShapePolyline *)m_pSh;
			ShPoly.Create(Points,idata);//ShPoly.Create(CRect(100,100,250,250),idata);
			ShPoly.GetPoints((LPPOINT)&pShUnit->m_pPoints[0][1],idata);
			pShUnit->SetupBuffers(CRect(0,0,150,150));
			m_pSh->m_pCursorArray=m_CursorArray;
			//m_pSh->m_Mode=_DRAFTDRAW_MODE_SEL;//!create polyline puts shape in drawing mode
			m_iCursor=10;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			delete Points;
			return TRUE;

			}break;

		case ID_PLACE_PICTURE:
			m_pSh=new CShapePicture(NULL,0,cmdDeque);
			((CShapePicture *)m_pSh)->ChooseFile(AfxGetMainWnd()->m_hWnd);
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));
			return TRUE;
			break;

		case ID_ZOOM_CURSORPLUS:
			m_iToolSel=_TOOLSELECT_ZOOMPLUS;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			::SetCursor(m_CursorArray[12]);
			return TRUE;
		case ID_ZOOM_CURSORMINUS:
			m_iToolSel=_TOOLSELECT_ZOOMMINUS;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			::SetCursor(m_CursorArray[13]);
			return TRUE;

		case ID_ZOOM_AREA:
			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			hCursor=::SetCursor(m_CursorArray[10]);
			m_iCursor=10;


			m_pSh=new CShapeZoomRect();
			m_pSh->m_pCursorArray=m_CursorArray;
			return TRUE;

		case ID_BUTTON_SELECT:

			m_iToolSel=_TOOLSELECT_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			::SetCursor(m_CursorArray[9]);
			return TRUE;

		case ID_BUTTON_ROTATE:

			m_iToolSel=_TOOLROTATE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			hCursor=::SetCursor(m_CursorArray[14]);
			m_iCursor=14;
			return TRUE;

		case ID_BUTTON_PAN:

			m_iToolSel = _TOOLSELECT_PAN;
			m_iToolType = _TOOLTYPENORMAL_DRAFTCABLE;
			hCursor = ::SetCursor(m_CursorArray[9]);
			m_iCursor = 9;
			return TRUE;

		case ID_BUTTON_POLYLINE:

			m_pSh=new CShapePolyline(NULL,0,cmdDeque);
			m_pSh->m_pCursorArray=m_CursorArray;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPEPOLY_DRAFTCABLE;
			::SetCursor(m_CursorArray[10]);
			m_iCursor=10;
			return TRUE;

		case ID_BUTTON_CURVE:

			m_pSh=new CShapeCurve(NULL,0,cmdDeque);
			m_pSh->m_pCursorArray=m_CursorArray;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPEPOLY_DRAFTCABLE;
			::SetCursor(m_CursorArray[10]);
			m_iCursor=10;
			return TRUE;

		case ID_BUTTON_POLYARC:

			m_pSh=new CShapePolyArc(NULL,0,cmdDeque);
			m_pSh->m_pCursorArray=m_CursorArray;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPEPOLY_DRAFTCABLE;
			::SetCursor(m_CursorArray[10]);
			m_iCursor=10;
			return TRUE;

		case ID_BUTTON_RECT:
		case ID_BUTTON_ELLIPSE:
		case ID_BUTTON_LINE:
		case ID_BUTTON_ARC:
		case ID_BUTTON_BOARD:

			ResetAllDrwOpe();

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			hCursor=::SetCursor(m_CursorArray[10]);
			m_iCursor=10;
			break;

		case ID_BUTTON_RULE:

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			hCursor=::SetCursor(m_CursorArray[10]);
			m_iCursor=10;

			m_pSh=new CShapeRule();
			m_pSh->m_pCursorArray=m_CursorArray;

			return TRUE;
			break;

		case ID_PLACE_PART:
		case ID_PLACE_CHASSIS:
			{

			//Reset all current drawing operations
			ResetAllDrwOpe();

			//Construct place part dialog
			CDialogPlacePart *dialog = new CDialogPlacePart();

#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
			dialog->Create(IDD_PLACEPART2, AfxGetMainWnd());
			dialog->ShowWindow(SW_SHOW);
#else
			if(dialog->DoModal()==IDOK){

				m_iToolSel=_TOOLPLACE_DRAFTCABLE;
				m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

				m_pSh=new CShapeUnit(NULL,0,cmdDeque);
				m_pSh->LoadUnit(dialog->m_sLibrary+"."+dialog->m_sPart);
				m_pSh->m_pCursorArray=m_CursorArray;
				m_iCursor=11;

				AddObject(m_pSh);
				m_pSh->OnLButtonDown(0, CPoint(0,0));
			}

			delete(dialog);
#endif

			}
			return TRUE;
			break;
		case ID_PLACE_NOCON:
			{


			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			m_pSh=new CShapeUnit(NULL,0,cmdDeque);
			m_pSh->LoadUnit("Standard.noconnect.ddw");
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));


			}
			return TRUE;
			break;
		case ID_PLACE_PORT:
			{


			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			m_pSh=new CShapeUnit(NULL,0,cmdDeque);
			m_pSh->LoadUnit("Standard.port_left.ddw");
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));


			}
			return TRUE;
			break;
		case ID_PLACE_PORTRIGHT:
			{


			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			m_pSh=new CShapeUnit(NULL,0,cmdDeque);
			m_pSh->LoadUnit("Standard.port_right.ddw");
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));


			}
			return TRUE;
			break;
		case ID_PLACE_WIRE:

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPECHAININI_DRAFTCABLE;
			m_iCursor=10;

			m_pSh=new CShapeWire(NULL,0,cmdDeque);
			m_pSh->m_pCursorArray=m_CursorArray;

			return TRUE;
			break;
		case ID_PLACE_BUS:

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPEPOLY_DRAFTCABLE;
			m_iCursor=10;

			m_pSh=new CShapeBus(NULL,0,cmdDeque);
			m_pSh->m_pCursorArray=m_CursorArray;

			return TRUE;
			break;
		case ID_PLACE_PIN:

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			if(pin.DoModal()==IDOK){

				//Get parameters from dialog
				DWORD dwStyle=pin.m_iShape;
				UINT uiPinnumber=atoi(pin.m_stNumber.GetBuffer(10));

				//Create new shape pin
				CShapePin *pShpin=new CShapePin(uiPinnumber,_DRAFTDRAW_SEL_RESIZING_RECT_S,dwStyle,cmdDeque);
				pShpin->m_bJack=pin.m_bJack;
				pShpin->m_pCursorArray=m_CursorArray;
				pShpin->m_TypePin=pin.m_TypePin;

				m_pSh=pShpin;
				m_iCursor=11;

				AddObject(m_pSh);
				m_pSh->OnLButtonDown(0, CPoint(0,0));
			}
			return TRUE;
			break;

		case ID_PLACE_NETALIAS:

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			if(netalias.DoModal()==IDOK){

				CShapeLabel *pShlabel=new CShapeLabel();
				pShlabel->m_pCursorArray=m_CursorArray;

				pShlabel->m_Label.bver=netalias.m_bVertical;
				pShlabel->m_Label.slabel=new CString(netalias.m_sAlias);
				pShlabel->m_Label.iSize=netalias.m_iSize;

				m_pSh=pShlabel;
			}
			return TRUE;
			break;
		case ID_PLACE_TEXT:{

			//Reset all current drawing operations
			ResetAllDrwOpe();

			CDialogText dlgText;

			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			if(dlgText.DoModal()==IDOK){

				CShapeLabel *pShlabel=new CShapeLabel(NULL,NULL,0,cmdDeque);
				pShlabel->m_pCursorArray=m_CursorArray;

				pShlabel->m_Label.bver=dlgText.m_bVertical;
				pShlabel->m_Label.slabel=new CString(dlgText.m_sAlias);
				if(dlgText.m_sName.GetLength()==0){
					pShlabel->m_Label.sname=new CString(dlgText.m_sAlias);
				}
				else{
					pShlabel->m_Label.sname=new CString(dlgText.m_sName);
				}
				pShlabel->m_Label.iSize=dlgText.m_iSize;

				//Move this to CShapeLabel
				LOGFONT lf;
				ZeroMemory(&lf, sizeof(LOGFONT));

				lf.lfHeight=-dlgText.m_iSize-3;
				lf.lfWeight=400;
				lf.lfOutPrecision=3;
				lf.lfClipPrecision=2;
				lf.lfQuality=1;
				lf.lfPitchAndFamily=34;
				strcpy(lf.lfFaceName,"Arial");

				pShlabel->m_Label.font=new CFont();
				pShlabel->m_Label.font->CreateFontIndirect(&lf);

				//Calculate size of label
				/*
				POSITION pos=GetFirstViewPosition();
				CView *pView=GetNextView(pos);

				if(pView->IsKindOf(RUNTIME_CLASS(CTreeView))){
					pView=GetNextView(pos);
				}

				SIZE sz;
				CDC* pDC=pView->GetDC();
				GetTextExtentPoint32(pDC->m_hDC, (LPCSTR)pShlabel->m_Label.sname, pShlabel->m_Label.sname->GetLength(), &sz);
				pShlabel->m_Rect=CRect(pShlabel->m_Rect.TopLeft(), sz);
				*/

				m_pSh=pShlabel;
				m_iCursor=11;
				AddObject(m_pSh);
				m_pSh->OnLButtonDown(0, CPoint(0,0));
			}
			return TRUE;
			}break;

		case ID_EDIT_NETALIAS:

			if(m_pObArray==&m_ObArray){

				//Check if a net alias is selected
				//==================================================
				CShapeLabel *pShLabelSel=NULL;
				for(int i=0;i<m_pObArray->GetSize();i++){
					CShape *pSh=(CShape *)(*m_pObArray)[i];
					if((pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT)&&
						(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel)) )){
						pShLabelSel=(CShapeLabel *)pSh;
						break;
					}
				}
				//==================================================

				if(pShLabelSel){
					netalias.m_bVertical=pShLabelSel->m_Label.bver;
					netalias.m_iSize=pShLabelSel->m_Label.iSize;
					netalias.m_sAlias=*pShLabelSel->m_Label.slabel;
					if(netalias.DoModal()==IDOK){

						if(pShLabelSel->m_Label.bver!=netalias.m_bVertical){
							pShLabelSel->OnCommand(MAKEWPARAM(ID_ROTATE,0),0);
						}
						pShLabelSel->m_Label.slabel=new CString(netalias.m_sAlias);
						pShLabelSel->m_Label.iSize=netalias.m_iSize;
						pShLabelSel->m_Rect=CRect(pShLabelSel->m_Rect.TopLeft(),CSize(0,0));//force recalculation of bounding rect

						if(pShLabelSel->m_Label.font){
							delete(pShLabelSel->m_Label.font);
						}

						//Create font
						LOGFONT lf;
						ZeroMemory(&lf, sizeof(LOGFONT));
						lf.lfHeight=-netalias.m_iSize-3;
						lf.lfWeight=400;
						lf.lfOutPrecision=3;
						lf.lfClipPrecision=2;
						lf.lfQuality=1;
						lf.lfPitchAndFamily=34;
						strcpy(lf.lfFaceName,"Arial");

						pShLabelSel->m_Label.font=new CFont();
						pShLabelSel->m_Label.font->CreateFontIndirect(&lf);
					}
				}
			}
			return TRUE;
			break;

		case ID_EDIT_PART:

			if(GetActiveViewType()==DCVIEWTYPE_SCHEMATIC){

				//Check if a unit is selected
				//==================================================
				CShapeUnit *pShUnitSel=NULL;
				for(int i=0;i<m_pObArray->GetSize();i++){
					CShape *pSh=(CShape *)(*m_pObArray)[i];
					if((pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT)&&
						(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit)) )){
						pShUnitSel=(CShapeUnit *)pSh;
						break;
					}
				}
				//==================================================

				/*
				m_pObArray=&m_ObArrayPart;
				cmdDeque=&m_cmdDequePart;
				//04/12/2004
				m_pmapShapeIdtoObj=&m_mapShapeIdtoObjPart;
				*/
				SetActiveViewType(DCVIEWTYPE_PART);
				//SetupCollections(1);

				//Load the unit if selected
				//==================================================
				if(pShUnitSel){
					if(SaveModified()){
						//Delete content of array
						//==================================================
						m_pObArray->RemoveAll();
						//==================================================

						//Get unit name
						CString strFile=pShUnitSel->m_sUnitName;
						//Get libray name
						CString strLib=strFile.Left(strFile.Find('.'));
						//Remove library name
						strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

						//21/12/2004
						//code to load parts from files with extension *.ddw using CDocument::Serialize()
						//this function for the moment it is not able to load parts from database
/*
						nFilterIndex=1;
						CFile file(g_sDCableBaseDir+strFile,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone);
						CArchive ar(&file,CArchive::load);
						Serialize(ar);

						CString str=pShUnitSel->m_sUnitName;
						SetPathName1(str);
						SetTitle(str);
*/
						//Load part from database
						CString str=pShUnitSel->m_sUnitName;
						SetPathName1(str);
						SetTitle(str);
						//This loads unit from database instead from shematic
						//Code have to be added to CShapeUnit to dump shapes to CDraftDrawDoc object array
						OnDatabaseLoad();

					}
				}
				//==================================================

				if(m_pObArray->GetSize()==0){

					
					CShapeFrmRect *pSh=new CShapeFrmRect(NULL,0,cmdDeque);
					pSh->m_Rect=CRect(200,200,400,350);
					pSh->m_Mode=1;
					pSh->m_pCursorArray=m_CursorArray;
					m_pObArray->Add(pSh);
					//SetPathName1("Standard.Nuevo");//Not set to force query user SaveModified>DoFileSave()>DoSave()
					SetTitle("Standard.Nuevo");					

				}
			}
			//Go back to schematic view
			else if(GetActiveViewType()==DCVIEWTYPE_PART){

				/*
				m_pObArray=&m_ObArray;
				cmdDeque=&m_cmdDeque;
				//04/12/2004
				std::deque<mapShapeIdtoObj_t*>::iterator it=m_mapShapeIdtoObjArray.begin();
				m_pmapShapeIdtoObj=(*it);
				*/
				SetActiveViewType(DCVIEWTYPE_SCHEMATIC);
				//SetupCollections(0);
			}

			UpdateAllViews(NULL,1);
			return TRUE;
			break;

		case ID_PLACE_BUSENTRY:
			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			//Old code when junctions had it own class CShapeJunction
			/*
			m_pSh=new CShapeJunction();
			m_pSh->m_pCursorArray=m_CursorArray;
			*/

			m_pSh=new CShapeUnit();
			m_pSh->LoadUnit("Standard.junction.ddw");
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));

			return TRUE;
			break;

		case ID_PLACE_BUSJUNC:
			m_iToolSel=_TOOLPLACE_DRAFTCABLE;
			m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;

			m_pSh=new CShapeUnit();
			m_pSh->LoadUnit("Standard.busjunc.ddw");
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;

			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));

			return TRUE;
			break;
		}
	}
	else{
		switch(nID){

		case IDS_MENU_POLYGON_TRIANG:
		case IDS_MENU_POLYGON_PENT:
		case IDS_MENU_POLYGON_HEX:
		case IDS_MENU_POLYGON_OCT:
		case IDS_MENU_POLYGON_NGEAR:
		case IDS_MENU_POLYGON_NSTAR:
		case IDS_MENU_POLYGON_NPOLYGON:
		case IDS_MENU_POLYGON_NCROSS:

		case IDS_MENU_POLYGON_PARALEL1:
		case IDS_MENU_POLYGON_TRAP:
		case IDS_MENU_POLYGON_ROMBO:
		case IDS_MENU_POLYGON_CROSS:
		case IDS_MENU_POLYGON_T:
		case IDS_MENU_POLYGON_L:
		case IDS_MENU_POLYGON_STRAIGHT:

		case ID_ZOOM_CURSORPLUS:
		case ID_ZOOM_CURSORMINUS:
		case ID_ZOOM_AREA:
		case ID_BUTTON_RULE:
		case ID_BUTTON_POLYLINE:
		case ID_BUTTON_CURVE:
		case ID_BUTTON_POLYARC:
		case ID_PLACE_NOCON:
		case ID_PLACE_PORT:
		case ID_PLACE_PORTRIGHT:

		case ID_PLACE_NETALIAS:
		case ID_PLACE_TEXT:
		case ID_PLACE_PICTURE:
			return TRUE;
			break;

		case ID_PLACE_PART:
			if(m_iTypeDocument==_DOCTYPE_SCHEMATIC){
				return TRUE;
			}
			break;

		case ID_PLACE_CHASSIS:
			if(m_iTypeDocument==_DOCTYPE_RACK){
				return TRUE;
			}
			break;

		case ID_PLACE_PIN:
		case ID_PLACE_WIRE:
		case ID_PLACE_BUS:
		case ID_PLACE_BUSENTRY:
		case ID_PLACE_BUSJUNC:

		case ID_ZOOM_IN:
		case ID_ZOOM_OUT:
		case ID_ZOOM_ALL:
		case ID_EDIT_PART:
		case ID_EDIT_NETALIAS:
		case ID_SNAP_TOGRID:
		case ID_SNAP_TOGRID2:
		case ID_BUTTON_SELECT:
		case ID_BUTTON_ROTATE:
		case ID_BUTTON_PAN:

			return TRUE;
			break;
		}
	}

	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDraftDrawDoc::OnSelectNet() 
{
	// TODO: Add your command handler code here

}

//Generates wiring
void CDraftDrawDoc::OnArchivoGencable() 
{
	//Netlist dialog
	CDialogNetList dlgNetList;
	CString sFileName;
	if(dlgNetList.DoModal()!=IDOK){
		return;
	}
	else{
		sFileName=dlgNetList.m_sFileName;
	}


	//Declarations
	FILE *fcabl=NULL;
	FILE *fbridge=NULL;int nBridgeCounter=0;
	FILE *flog=fopen(g_sDCableBaseDir+"cableado.log","w");
	BOOL bFlagWarn=FALSE;
	int index;
	CStringList *slCables=new CStringList;
	CString strLocation="RACK";


	//First of all search for title
	//==============================================================
	CShape *pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);
	while(pSh){
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
			CString str=pSh->m_strIdent;
			str.MakeUpper();
			if(str.Find("TITLE")>=0){
				CShapeUnit *pShUnit=(CShapeUnit *)pSh;
				for(int i=0;i<pShUnit->m_LabelsCount;i++){
					CString str=*pShUnit->m_pLabels[i]->sname;
					str.MakeUpper();
					if(str.Find("TITLE")>=0){
						strLocation=*pShUnit->m_pLabels[i]->slabel;
						break;
					}
				}
				break;
			}
		}
		pSh=(CShape *)NextObject(index);
	}
	//==============================================================

	//GENERATE NETLIST
	//==============================================================
	pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);
	while(pSh){

		CRuntimeClass* pRuntimeClass=pSh->GetRuntimeClass( );

		CString sName=CString(pRuntimeClass->m_lpszClassName);
		if(!sName.Compare("CShapeWire")){
			if(!pSh->m_strIdent.IsEmpty()){

				CString sIdOrg="",sIdDest="";
				CString sConnOrg="",sConnDest="";
				CString sConnNumOrg="",sConnNumDest="";
				CShape *pShOrg=NULL,*pShDest=NULL;

				//keep a register of written cables
				//============================================
				if(slCables->Find(pSh->m_strIdent)){
					pSh=(CShape *)NextObject(index);
					continue;
				}
				slCables->AddTail(pSh->m_strIdent);
				//============================================

				//bridges
				//============================================
				void *ptr;
				if(m_mapStrHWView.Lookup(pSh->m_strIdent,ptr)){

					CString strBridge,strBridgeTitle;
					if(m_mapStrHWParameter.Lookup(pSh->m_strIdent,ptr)){
						CIntArray *piaParameter=(CIntArray *)ptr;

						std::fstream fout;

						strBridge.Format("%s~bridge%03i.txt",AfxGetCablePath(),nBridgeCounter);
						strBridgeTitle.Format("~bridge%03i.txt",nBridgeCounter);
						nBridgeCounter++;
						fout.open(strBridge,std::ios_base::out);

						for(int i=0;i<piaParameter->GetSize();i+=6){
							int idata1=0;
							int idata2=0;
							if(i+2<piaParameter->GetSize()){
								idata1=(*piaParameter)[i+2];
							}
							if(i+3<piaParameter->GetSize()){
								idata2=(*piaParameter)[i+3];
							}

							fout<<idata1<<";;;"<<idata2<<"\n";
						}

						fout<<"\n";
						fout.close();
					}

					if(!fbridge){
						fbridge=fopen(g_sDCableBaseDir+"puentes.txt","w");
					}

					//Find origin & destination
					CShape *pShFirst=((CShapeContainer*)pSh)->GetFirstChainItem();
					CShape *pShTmp=pShFirst;
					CShape *pShLast=NULL;
					CShape *pShPrev=NULL;
					while(pShTmp){
						pShLast=pShTmp;
						((CShapeContainer*)pShTmp)->GetNextChainItem(pShTmp, pShPrev);
						//Keep prev chain item
						pShPrev=pShTmp;
					}

					if(pShLast){
						CShape *pShTmp=(CShape *)((CShapeContainer*)pShFirst)->m_obarrShapearr.GetAt(0);
						//Check if it is a telephony bridge
						if(((CShapeUnit*)pShTmp->m_pshConn)->IsTB()){
							sIdOrg=pShTmp->m_pshConn->m_strIdent;
							sIdDest=pShTmp->m_pshConn->m_strIdent;
						}
						else{
						pShTmp=(CShape *)((CShapeContainer*)pShLast)->m_obarrShapearr.GetAt(1);
							if(((CShapeUnit*)pShTmp->m_pshConn)->IsTB()){
								sIdOrg=pShTmp->m_pshConn->m_strIdent;
								sIdDest=pShTmp->m_pshConn->m_strIdent;
							}
						}
					}

					//Write to file
					int i=strBridgeTitle.Find('.');
					fprintf(fbridge,BRIDGEFILE_LINE,
						pSh->m_strIdent,
						sIdOrg,
						sIdDest,
						strBridgeTitle.Left(i)
						);
					/*
					continue;
					*/
				}
				//============================================

				//check connections
				//============================================
				CShapeContainer *pShCont00=(CShapeContainer *)pSh;
				int i;
				for(i=0;i<pShCont00->m_ShapeNumber;i++){
					CShape *pSh=(CShape *)pShCont00->m_obarrShapearr[i];
					CRuntimeClass* pRuntimeClass=pSh->GetRuntimeClass( );
					CString sName=CString(pRuntimeClass->m_lpszClassName);
					if(!sName.Compare("CShapePin")){
						CShapeContainer *pShCont=(CShapeContainer *)pSh;
						if(pShCont->m_pshConn){
							CShape *pSh=pShCont->m_pshConn;
							if( pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit)) ){
								sIdOrg=pSh->m_strIdent;
								//Check connector type
								//============================================
								CShapeUnit *pShUnit=(CShapeUnit *)pSh;
								for(int i=0;i<pShUnit->m_ShapeNumber;i++){
									CShape *pSh=(CShape *)pShUnit->m_obarrShapearr[i];
									if((pSh)&&(pSh->IsKindOf(RUNTIME_CLASS(CShapePin)))){
										CShapePin *pShPin=(CShapePin *)pSh;
										if(pShPin->m_pshConn==pShCont00){
											if(pShPin->m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
												sConnNumOrg.Format("J%i",pShPin->m_uiPinnumber);
											}
											else{
												sConnNumOrg.Format("P%i",pShPin->m_uiPinnumber);
											}
											int nDefaultPin = 0;
											if(pShPin->m_TypePin >= 0 && pShPin->m_TypePin < AfxGetConnectorCount()){
												sConnOrg=TABLE_PIN_LABEL[pShPin->m_TypePin];
											}
											else
											{
												sConnOrg=TABLE_PIN_LABEL[nDefaultPin];
											}
											//Check if wire is compatible with pin
											//==================================

											//Check for logical connectors
											BOOL bLogical=AfxIsConnectorLogical(pShPin->m_TypePin);
											
											if((!bLogical)&&AfxGetConnectorPintCount(pShPin->m_TypePin)!=AfxGetCableWireCount(pShCont00->m_strTypeIdent)){
												fprintf(flog,"No coincide el número de pines:%s a %s\n",
													pShCont00->m_strIdent,
													pShUnit->m_strIdent
													);
												bFlagWarn=TRUE;
											}
											
											//==================================
											break;
										}
									}
								}
								//==============================================
							}
						}
					}
				}

				//Search for destination unit
				//Criteria to find end segment of wire: same identifier that start segment
				CShapePin *pShPinDest=NULL;
				pShDest=FindShape(pShCont00,(CShape **)&pShPinDest);

				if(pShDest){
					sIdDest=pShDest->m_strIdent;
					if(pShPinDest){
						if(pShPinDest->m_dwStyle&SHAPEUNIT_PINTYPE_JACK){
							sConnNumDest.Format("J%i",pShPinDest->m_uiPinnumber);
						}
						else{
							sConnNumDest.Format("P%i",pShPinDest->m_uiPinnumber);
						}
						int nDefaultPin = 0;
						if(pShPinDest->m_TypePin>= 0 && pShPinDest->m_TypePin < AfxGetConnectorCount()){
							sConnDest=TABLE_PIN_LABEL[pShPinDest->m_TypePin];
						}
						else{
							sConnDest=TABLE_PIN_LABEL[nDefaultPin];
						}
					}
				}
				//============================================

				if(!fcabl){
					fcabl=fopen(g_sDCableBaseDir+"cableado.txt","w");
				}


				//"18;W311;Flex STP CAT 5E PG;J1;J0D;CHASIS SSIF 1R;CHASIS SSIOL 1R;RACK SICAM;RACK SICAM;RJ 45 M apantalldo;RJ 45 M apantalldo;;cable516.txt;4;\n"
				i=pSh->m_strTypeIdent.Find('.');
				fprintf(fcabl,CABLFILE_LINE,
					pSh->m_strIdent,
					sConnNumOrg,
					sConnNumDest,
					sIdOrg,
					sIdDest,
					strLocation,
					strLocation,
					sConnOrg,
					sConnDest,
					pSh->m_strTypeIdent.Left(i));
			}
			else{
				fprintf(flog,"Cable sin identificar, se omite de netlist.Coordenadas:(%i,%i)-(%i,%i)\n",
					pSh->m_Rect.TopLeft().x,
					pSh->m_Rect.TopLeft().y,
					pSh->m_Rect.BottomRight().x,
					pSh->m_Rect.BottomRight().y
					);
				bFlagWarn=TRUE;
			}
		}

		pSh=(CShape *)NextObject(index);
	}
	//==============================================================

	//GENERATE BRIDGES
	//==============================================================
	/*
	pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);
	while(pSh){
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
			std::map<int,HWND>::iterator it1=m_mapIdHWView.find(pSh->m_uiShapeId);
			if(it1!=m_mapIdHWView.end()){
				CShapeContainer *pShCont=(CShapeContainer *)pSh;
				std::fstream fout;
				fout.open("~bridge.txt",std::ios_base::out);

				for(int i=0;i<pShCont->m_iaParameter.GetSize();i+=6){
					int idata1=0;
					int idata2=0;
					if(i+2<pShCont->m_iaParameter.GetSize()){
						idata1=pShCont->m_iaParameter[i+2];
					}
					if(i+3<pShCont->m_iaParameter.GetSize()){
						idata2=pShCont->m_iaParameter[i+3];
					}

					fout<<idata1<<";;;"<<idata2<<"\n";
				}

				fout.close();
			}
			pSh=(CShape *)NextObject(index);
		}
		pSh=(CShape *)NextObject(index);
	}
	*/
	//==============================================================

	//CHECK WARNINGS & ERRORS
	//==================================================
	if(flog){
		fclose(flog);
	}
	if(bFlagWarn){
		if(::AfxMessageBox("Hay algunos warnings, desea continuar?",MB_YESNO|MB_ICONEXCLAMATION,-1)==IDNO){

			HINSTANCE hinstance=ShellExecute(
				AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
				"open",						//LPCTSTR lpVerb,
				"Notepad.exe",				//LPCTSTR lpFile, 
				"cableado.log",				//LPCTSTR lpParameters, 
				g_sDCableBaseDir,			//LPCTSTR lpDirectory,
				SW_SHOWNORMAL				//INT nShowCmd
			);
			//End
			return;
		}

		HINSTANCE hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			"Notepad.exe",				//LPCTSTR lpFile, 
			"cableado.log",				//LPCTSTR lpParameters, 
			g_sDCableBaseDir,			//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);
	}

	//==================================================


	int nIndex=sFileName.Find('.');
	CString sFileExt;
	if(nIndex>=0){
		sFileExt=sFileName.Right(sFileName.GetLength()-nIndex-1);
	}
	if(!sFileExt.CompareNoCase("doc")){
	//CALL WORD
	//==================================================
	if(fcabl){
		fprintf(fcabl,"\n");
		fclose(fcabl);

		CString sWinwordPath=afxWinwordPath;
		sWinwordPath+="\\WINWORD.EXE";
/*
		HINSTANCE hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			sWinwordPath,//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
			"\""+g_sDCableBaseDir+"plantilla_cabl1.doc"+"\"",				//LPCTSTR lpParameters, 
			NULL,						//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);
*/


		std::string sFilter = "Microsoft Word Document (*.doc)|*.doc||"; //All Files (*.*)|*.*||;
		CStringArray saExtensions;
		saExtensions.Add("doc");


		CFileDialog fdialog(
			FALSE,		//save dialog box
			NULL,
			NULL,
			0,			//no flags
			sFilter.c_str(),
			NULL);

		

		if(fdialog.DoModal()==IDOK){


			int offset=fdialog.m_ofn.nFileExtension;

			CString strFile=fdialog.m_ofn.lpstrFile;

			nFilterIndex=fdialog.m_ofn.nFilterIndex;

			if((!offset)||(fdialog.m_ofn.lpstrFile[offset]==0)){

				strFile+="."+saExtensions[fdialog.m_ofn.nFilterIndex-1];
				sFilter=saExtensions[fdialog.m_ofn.nFilterIndex-1];
			}

			LaunchWord(g_sDCableBaseDir+"plantilla_cabl1.doc",strFile);//g_sDCableBaseDir+"cableado.doc"

		}
	}
	if(fbridge){
		fprintf(fbridge,"\n");
		fclose(fbridge);

		CString sWinwordPath=afxWinwordPath;
		sWinwordPath+="\\WINWORD.EXE";

		std::string sFilter = "Microsoft Word Document (*.doc)|*.doc||"; //All Files (*.*)|*.*||;
		CStringArray saExtensions;
		saExtensions.Add("doc");


		CFileDialog fdialog(
			FALSE,		//save dialog box
			NULL,
			NULL,
			0,			//no flags
			sFilter.c_str(),
			NULL);

		

		if(fdialog.DoModal()==IDOK){


			int offset=fdialog.m_ofn.nFileExtension;

			CString strFile=fdialog.m_ofn.lpstrFile;

			nFilterIndex=fdialog.m_ofn.nFilterIndex;

			if((!offset)||(fdialog.m_ofn.lpstrFile[offset]==0)){

				strFile+="."+saExtensions[fdialog.m_ofn.nFilterIndex-1];
				sFilter=saExtensions[fdialog.m_ofn.nFilterIndex-1];
			}

			LaunchWord(g_sDCableBaseDir+"plantilla_puent.doc",strFile);//g_sDCableBaseDir+"cableado.doc"

		}

	}
	//==================================================
	}
	else if(!sFileExt.CompareNoCase("htm")||!sFileExt.CompareNoCase("html")){
	//CALL HTML GEN
	//==================================================
	if(fcabl){
		fprintf(fcabl,"\n");
		fclose(fcabl);

		::SetCurrentDirectory(g_sDCableBaseDir);

		CString sGeneratorPath=".\\";
		sGeneratorPath+="\\gen_html2.exe";

		HINSTANCE hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			sGeneratorPath,//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
			"\""+g_sDCableBaseDir+"plantilla_cabl1.doc"+"\"",				//LPCTSTR lpParameters, 
			NULL,						//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);

		CString sPathBase=g_sDCableBaseDir;//"C:\\2002\\DraftCable\\Src\\";
		CString sPath="\"file://"+sPathBase+"testfile.htm\"";



		//Search for Internet Explorer Path
		BSTR param2 = SysAllocString(OLESTR("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"));
		BSTR param1 = SysAllocString(OLESTR(""));
		BSTR name1 = SysAllocString(OLESTR("SubKey"));
		BSTR name2 = SysAllocString(OLESTR("Entry"));

		AddArgumentCString(name1,0,param1);
		AddArgumentCString(name2,0,param2);

		VARIANTARG FAR rgvarg[2];

		
		V_BSTRREF(&rgvarg[0]) = &param1;
		V_BSTRREF(&rgvarg[1]) = &param2;


		DISPPARAMS dpNoArgs = {g_aVargs, NULL, 2, 0};
		VARIANT vResult;
		OLECHAR FAR* szFunction;
		//BSTR bstrTemp;

		// IDispatch pointers for Word's objects
		//IDispatch* pDispDocs;      //Documents collection

		// DISPIDs
		DISPID dispid_Docs;        //Documents property of Application 
								   //object

		//Initialize the COM libraries
		::CoInitialize(NULL);

		// Create an instance of the Word application and obtain the 
		// pointer to the application's IDispatch interface.
		CLSID clsid;
		CLSIDFromString(L"UtilsVB.clsRegistro", &clsid);  

		IUnknown* pUnk;
		HRESULT hr = ::CoCreateInstance( clsid, NULL, CLSCTX_SERVER,
										 IID_IUnknown, (void**) &pUnk);
		IDispatch* pDispApp;
		hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pDispApp);

		// Get IDispatch* for the Documents collection object
		szFunction = OLESTR("GetStringValue");


		hr = pDispApp->GetIDsOfNames (IID_NULL, &szFunction, 1, 
									  LOCALE_USER_DEFAULT, &dispid_Docs);

		unsigned int FAR uArgErr;
		hr = pDispApp->Invoke (dispid_Docs, IID_NULL, 
							   LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
							   &dpNoArgs, &vResult, NULL, &uArgErr);
		




		//Execute Internet Explorer
		hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			CString(vResult.bstrVal),//"C:\\Archivos de programa\\Internet Explorer\\IEXPLORE.EXE",//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
			sPath,//"\"file://C:\\2002\\DraftCable\\Src\\testfile.htm\"",				//LPCTSTR lpParameters, 
			NULL,						//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);



		ClearAllArgs();
		pDispApp->Release();
		pUnk->Release();

		//...
		::CoUninitialize();

	}
	if(fbridge){
		fprintf(fbridge,"\n");
		fclose(fbridge);

		::SetCurrentDirectory(g_sDCableBaseDir);

		CString sGeneratorPath=".\\";
		sGeneratorPath+="\\gen_html2.exe";

		HINSTANCE hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			sGeneratorPath,//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
			"-bridges",					//LPCTSTR lpParameters, 
			NULL,						//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);

		CString sPathBase=g_sDCableBaseDir;//"C:\\2002\\DraftCable\\Src\\";
		CString sPath="\"file://"+sPathBase+"bridges.htm\"";



		//Search for Internet Explorer Path
		BSTR param2 = SysAllocString(OLESTR("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"));
		BSTR param1 = SysAllocString(OLESTR(""));
		BSTR name1 = SysAllocString(OLESTR("SubKey"));
		BSTR name2 = SysAllocString(OLESTR("Entry"));

		AddArgumentCString(name1,0,param1);
		AddArgumentCString(name2,0,param2);

		VARIANTARG FAR rgvarg[2];

		
		V_BSTRREF(&rgvarg[0]) = &param1;
		V_BSTRREF(&rgvarg[1]) = &param2;


		DISPPARAMS dpNoArgs = {g_aVargs, NULL, 2, 0};
		VARIANT vResult;
		OLECHAR FAR* szFunction;
		//BSTR bstrTemp;

		// IDispatch pointers for Word's objects
		//IDispatch* pDispDocs;      //Documents collection

		// DISPIDs
		DISPID dispid_Docs;        //Documents property of Application 
								   //object

		//Initialize the COM libraries
		::CoInitialize(NULL);

		// Create an instance of the Word application and obtain the 
		// pointer to the application's IDispatch interface.
		CLSID clsid;
		CLSIDFromString(L"UtilsVB.clsRegistro", &clsid);  

		IUnknown* pUnk;
		HRESULT hr = ::CoCreateInstance( clsid, NULL, CLSCTX_SERVER,
										 IID_IUnknown, (void**) &pUnk);
		IDispatch* pDispApp;
		hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pDispApp);

		// Get IDispatch* for the Documents collection object
		szFunction = OLESTR("GetStringValue");


		hr = pDispApp->GetIDsOfNames (IID_NULL, &szFunction, 1, 
									  LOCALE_USER_DEFAULT, &dispid_Docs);

		unsigned int FAR uArgErr;
		hr = pDispApp->Invoke (dispid_Docs, IID_NULL, 
							   LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
							   &dpNoArgs, &vResult, NULL, &uArgErr);
		




		//Execute Internet Explorer
		hinstance=ShellExecute(
			AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
			"open",						//LPCTSTR lpVerb,
			CString(vResult.bstrVal),//"C:\\Archivos de programa\\Internet Explorer\\IEXPLORE.EXE",//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
			sPath,//"\"file://C:\\2002\\DraftCable\\Src\\testfile.htm\"",				//LPCTSTR lpParameters, 
			NULL,						//LPCTSTR lpDirectory,
			SW_SHOWNORMAL				//INT nShowCmd
		);



		ClearAllArgs();
		pDispApp->Release();
		pUnk->Release();

		//...
		::CoUninitialize();

	}
	//==================================================
	}
	else{
		if(fcabl){
			fprintf(fcabl,"\n");
			fclose(fcabl);
			AfxMessageBox("Formato no soportado",MB_OK|MB_ICONSTOP,-1);
		}
	}


	delete(slCables);
}

BOOL CDraftDrawDoc::SetCursor()
{
	switch(m_iToolSel){
	case _TOOLPLACE_DRAFTCABLE:
		if(m_iCursor>=0){
			//::SetCursor(m_CursorArray[10]);
			::SetCursor(m_CursorArray[m_iCursor]);
		}
		break;
	case _TOOLSELECT_ZOOMPLUS:
		::SetCursor(m_CursorArray[12]);
		m_iCursor=12;
		break;
	case _TOOLSELECT_ZOOMMINUS:
		::SetCursor(m_CursorArray[13]);
		m_iCursor=13;
		break;
	case _TOOLROTATE_DRAFTCABLE:
		::SetCursor(m_CursorArray[14]);
		m_iCursor=14;
		break;
	default:
		::SetCursor(m_CursorArray[9]);
		m_iCursor=9;
		break;
	}

	return TRUE;
}

int CDraftDrawDoc::GetCursor()
{
	return m_iCursor;
}

CShape * CDraftDrawDoc::FindShape(CShape *pShSearch,CShape **pShSearchChild/*=NULL*/)
{
	int index;
	CShape *pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);
	while(pSh){
		if( (pSh->IsKindOf(RUNTIME_CLASS(CShapeWire)))&&
			(pSh!=pShSearch)&&
			(pSh->m_strIdent==pShSearch->m_strIdent)){

			CShapeContainer *pShCont00=(CShapeContainer *)pSh;
			for(int i=0;i<pShCont00->m_ShapeNumber;i++){
				CShape *pSh=(CShape *)pShCont00->m_obarrShapearr[i];
				CRuntimeClass* pRuntimeClass=pSh->GetRuntimeClass( );
				CString sName=CString(pRuntimeClass->m_lpszClassName);
				if(!sName.Compare("CShapePin")){
					CShapeContainer *pShCont=(CShapeContainer *)pSh;
					if(pShCont->m_pshConn){
						CShape *pSh=pShCont->m_pshConn;
						if( pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit)) ){
							//Check connector type
							//============================================
							CShapeUnit *pShUnit=(CShapeUnit *)pSh;
							for(int i=0;i<pShUnit->m_ShapeNumber;i++){
								CShape *pSh=(CShape *)pShUnit->m_obarrShapearr[i];
								if((pSh)&&(pSh->IsKindOf(RUNTIME_CLASS(CShapePin)))){
									CShapePin *pShPin=(CShapePin *)pSh;
									if(pShPin->m_pshConn==pShCont00){
										if(pShSearchChild){
											*pShSearchChild=pShPin;
										}
										break;
									}
								}
							}
							//============================================
							return pSh;
						}
					}
				}
			}
			
		}

		pSh=(CShape *)NextObject(index);
	}

	return NULL;
}

void CDraftDrawDoc::OnToolOptionPage() 
{
	// TODO: Add your command handler code here
	CDialogPageProperties ppDialog("Propiedades Página Plano");

	CPageSize page0;
	CPageGrid page1;

	ppDialog.AddPage(&page0);
	ppDialog.AddPage(&page1);

	page1.m_nWidth=m_szGrid.cx;

	if(ppDialog.DoModal()==IDOK){

		m_szDesign=CSize(page0.m_szSize.cx*5,page0.m_szSize.cy*5);

		m_pShArray->SetSize(m_szDesign,CSize(100,100));

		m_szGrid=CSize(page1.m_nWidth,page1.m_nWidth);

		UpdateAllViews(NULL,3);
	}
}

void CDraftDrawDoc::OnToolConfig() 
{
	// TODO: Add your command handler code here
	CDialogConfig dialogConfig;

	dialogConfig.m_sPath=afxWinwordPath;

	if(dialogConfig.DoModal()==IDOK){
		afxWinwordPath=dialogConfig.m_sPath;
		AfxGetApp()->WriteProfileString("Settings","WinwordPath",afxWinwordPath);
	}
}

CSize CDraftDrawDoc::GetSize(){
	return m_szDesign;
}

CSize CDraftDrawDoc::GetDefaultSize(){
	return CSize(1485,1050);///!!!MAGIC NUMBER
}

void CDraftDrawDoc::Order(int idata){

	for(int i=0;i<m_pObArray->GetSize();i++){
		CShape *pSh=(CShape *)(*m_pObArray)[i];
		if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			m_pObArray->RemoveAt(i);
			if(idata){
				m_pObArray->Add(pSh);
			}
			else{
				m_pObArray->InsertAt(0,pSh);
			}
		}
	}
}
void CDraftDrawDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here

	int index;

	if(m_pObArrayTemp==NULL){
		m_pObArrayTemp=new CObArray;
	}
	else{
		m_pObArrayTemp->RemoveAll();
	}

	m_ptPasteOffset=CPoint(25,25);

	CShape *pSh=(CShape *)LastObject(index);
	pSh=(CShape *)PrevObject(index);
	CRect rect_union=CRect(0,0,0,0);
	int nCount=0;
	while(pSh){
		if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
			m_pObArrayTemp->Add(pSh);
			rect_union.UnionRect(rect_union,pSh->m_Rect);
			nCount++;
		}
		pSh=(CShape *)PrevObject(index);
	}

	//prepare to copy to clipboard

	COleDataSource* pSource = new COleDataSource();

	POSITION pos=GetFirstViewPosition();
	CView *pView=GetNextView(pos);

	if(pView->IsKindOf(RUNTIME_CLASS(CTreeView))){
		pView=GetNextView(pos);
	}

	CPoint pt=rect_union.TopLeft();

	BOOL bError=!ClientToScreen(pView->m_hWnd,&pt);

	HDIB hDIB = CopyScreenToDIB( &(rect_union+(pt-rect_union.TopLeft())) );

	LPVOID lpvMem=::GlobalLock(hDIB);
	
    //CDib& dib = GetDocument()->m_dib;
	CDib dib;
	dib.AttachMemory(lpvMem,FALSE,hDIB);
    if (dib.GetSizeImage() > 0) {
      
      int nHeaderSize =  dib.GetSizeHeader();
      int nImageSize = dib.GetSizeImage();
      HGLOBAL hHeader = ::GlobalAlloc(GMEM_SHARE,
          nHeaderSize + nImageSize);
      LPVOID pHeader = ::GlobalLock(hHeader);
      ASSERT(pHeader != NULL);
      LPVOID pImage = (LPBYTE) pHeader + nHeaderSize;
      memcpy(pHeader, dib.m_lpBMIH, nHeaderSize); 
      memcpy(pImage, dib.m_lpImage, nImageSize);
      // Se supone que el receptor libera la memoria global
      ::GlobalUnlock(hHeader);
      pSource->CacheGlobalData(CF_DIB, hHeader);
	  /*
      if(pSource) {
	      pSource->SetClipboard(); // OLE borra la fuente de datos
	  }
	  */
	  ::GlobalUnlock(hDIB);
      //return ;//pSource;
    }
	
	//copy to clipboard in private format
	CMemFile file;
	CFileException fe;

	// save to Contents stream
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = TRUE;

	TRY
	{
		saveArchive<<nCount;
		CShape *pSh=(CShape *)LastObject(index);
		pSh=(CShape *)PrevObject(index);
		while(pSh){
			if(pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT){
				//this is not neccesary!
				/*
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){
					((CShapeContainer*)pSh)->DoConntoId();
				}
				*/

				saveArchive<<pSh;
			}
			pSh=(CShape *)PrevObject(index);
		}
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		THROW_LAST();
	}
	END_CATCH_ALL

	BYTE *pStorage = NULL;
	saveArchive.Flush();
	DWORD dwFileLen=file.GetLength();
	pStorage=file.Detach();

	if(dwFileLen>0){
		//COleDataSource* pSource = new COleDataSource();
		HGLOBAL hMem = ::GlobalAlloc(GMEM_SHARE,dwFileLen);
		LPVOID pMem = ::GlobalLock(hMem);
		ASSERT(pMem != NULL);
		memcpy(pMem, pStorage, dwFileLen); 
		// Se supone que el receptor libera la memoria global
		::GlobalUnlock(hMem);
		pSource->CacheGlobalData(CF_PRIVATEFIRST, hMem);
		if(pSource) {
		  pSource->SetClipboard(); // OLE borra la fuente de datos
		}
	}
}

void CDraftDrawDoc::OnEditPaste() 
{
	// TODO: Add your command handler code here

	COleDataObject dataObject;

	BOOL bAvail=dataObject.AttachClipboard();

	if(bAvail){

		if(dataObject.IsDataAvailable(CF_PRIVATEFIRST )){

			HGLOBAL hMF;
			if (OpenClipboard(AfxGetMainWnd()->m_hWnd))
			{ 
				hMF = GetClipboardData(CF_PRIVATEFIRST); 

			}
			
			ASSERT(hMF!=NULL);

			DWORD dwSize=::GlobalSize(hMF);
			BYTE *pStorage=(BYTE *)GlobalLock(hMF);

			CMemFile *file;
			file=new CMemFile(pStorage, dwSize, 0);
			CFileException fe;

			CArchive loadArchive(file, CArchive::load | CArchive::bNoFlushOnDelete);
			loadArchive.m_pDocument = this;
			loadArchive.m_bForceFlat = TRUE;

			TRY{
				int nCount;
				loadArchive>>nCount;

				CShape *pSh;
				CRect rcFrm=CRect(0,0,0,0);
				std::map<int,int> mapIdtoId;
				CObArray arrayTemp;
				int i;
				for(i=0;i<nCount;i++){

					loadArchive>>pSh;

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){
						rcFrm=pSh->m_Rect;
					}
					else{
						pSh->m_Rect+=rcFrm.TopLeft();
					}

					pSh->m_pCursorArray=m_CursorArray;
					pSh->pcmdDeque=cmdDeque;

					//10/01/2004
					//Added to store pointer to patch panel editor.
					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
						((CShapeUnit*)pSh)->mf_cbPatchPEdit=SDisplayPatchPProp;
					}

					int prev_id=pSh->m_uiShapeId;
					AddObject(pSh);
					arrayTemp.Add(pSh);
					mapIdtoId[prev_id]=pSh->m_uiShapeId;
				}

				for(i=0;i<arrayTemp.GetSize();i++){

					CShape *pSh=(CShape *)arrayTemp[i];

					if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

						((CShapeContainer*)pSh)->DoPasteConnections(&mapIdtoId,m_pmapShapeIdtoObj);
					}
					else if(pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){


						pSh->DoPasteConnections(&mapIdtoId,m_pmapShapeIdtoObj);
					}
				}

			}
			CATCH_ALL(e)
			{
				file->Abort();   // will not throw an exception
				delete(file);
				THROW_LAST();
			}
			END_CATCH_ALL

			::GlobalUnlock(pStorage);
			CloseClipboard();
			return;
		}
		else if(dataObject.IsDataAvailable(CF_ENHMETAFILE)){

			HGLOBAL hMF;

			//this function fails!
			//hMF=dataObject.GetGlobalData(CF_ENHMETAFILE );

			if (OpenClipboard(AfxGetMainWnd()->m_hWnd))
			{ 
				hMF = GetClipboardData(CF_ENHMETAFILE); 

			}
			
			ASSERT(hMF!=NULL);


			//Create shape
			m_pSh=new CShapePicture();
			CShapePicture *pShPicture=(CShapePicture *)m_pSh;
			m_pSh->m_pCursorArray=m_CursorArray;
			m_iCursor=11;


			//Create picture
			PICTDESC PictDesc;
			PictDesc.cbSizeofstruct=sizeof(PictDesc);
			PictDesc.picType=PICTYPE_ENHMETAFILE;
			PictDesc.emf.hemf=(HENHMETAFILE)hMF;
			::OleCreatePictureIndirect(&PictDesc,IID_IPicture,TRUE,(LPVOID *)&pShPicture->gpPicture);


			//Add shape to stack
			AddObject(m_pSh);
			m_pSh->OnLButtonDown(0, CPoint(0,0));

			CloseClipboard();
			return;
		}
	}


	//Internal representation should be avoided
	if(m_pObArrayTemp!=NULL){
		CShape *pSh=(CShape *)(*m_pObArrayTemp)[0];
		CRect rect0;
		if(pSh){
			rect0=pSh->m_Rect+m_ptPasteOffset;
		}

		CRect rect_union=CRect(0,0,0,0);

		for(int i=m_pObArrayTemp->GetSize()-1;i>=0;i--){
			pSh=(CShape *)(*m_pObArrayTemp)[i];
			CRuntimeClass *runtime=pSh->GetRuntimeClass();
			CObject *pObject=runtime->CreateObject();
			CShape *pSh1=(CShape *)pObject;
			pSh1->m_Rect=pSh->m_Rect+m_ptPasteOffset;
			pSh1->m_Mode=1;
			pSh1->m_pCursorArray=m_CursorArray;
			rect0+=CPoint(DCABLE_GRIDX_DEFAULT*5,DCABLE_GRIDY_DEFAULT*5);
			AddObject(pSh1);

			//Calculate update region
			rect_union.UnionRect(rect_union,pSh1->m_Rect);
		}
		//m_pObArrayTemp->RemoveAll();
		m_ptPasteOffset+=CPoint(DCABLE_GRIDX_DEFAULT*5,DCABLE_GRIDY_DEFAULT*5);

		CRgn rgn;
		rgn.CreateRectRgnIndirect(rect_union);
		UpdateAllViews(NULL,2,&rgn);
	}
}

//Display Patch Panel properties
//Input:
//	strID		Cable identifier
//Documentation on using CreateNewFrame / InitialUpdateFrame can be found on
//Knowlege Base.Visual C++
//"HOWTO: Create Additional Views with CreateNewFrame() Function"
//ID: Q100993
void CDraftDrawDoc::DisplayPatchPProp(UINT nID){

	std::map<int,HWND>::iterator it1=m_mapIdHWView.find(nID);

	if((it1==m_mapIdHWView.end())||(it1->second==0)){

		int nIndex=0;

		//do not create edition dialog if TB exits in memory
		if(it1->second!=0){
			CDialogPatchPProperties dialogPatchP;

			int index;
			CShape *pSh=(CShape *)FirstObject(index);
			pSh=(CShape *)NextObject(index);
			while(pSh){
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){

					dialogPatchP.m_mapCableNum[std::string(pSh->m_strIdent)]=index;
				}

				pSh=(CShape *)NextObject(index);
			}

			if(dialogPatchP.DoModal()==IDOK){

				nIndex=m_obaCables1.Add(dialogPatchP.m_pstraCable1);
				m_obaCables2.Add(dialogPatchP.m_pstraCable2);

			}
			else{
				return;
			}
		}
		else{
			/*
			std::map<int,int>::iterator it2=m_mapIdHWIndex.find(nID);
			nIndex=it2->second;
			*/
			m_mapIdHWIndex.Lookup(nID,nIndex);
		}



		CFrameWnd * pNewFrame =theApp.pMDTPatchP->CreateNewFrame(this,NULL);

		if (pNewFrame == NULL)
			 return ;


		theApp.pMDTPatchP->InitialUpdateFrame(pNewFrame, this);

		HWND hwnd=pNewFrame->GetActiveView()->m_hWnd;


		CTBEditView *pTBEditView=(CTBEditView *)pNewFrame->GetActiveView();

		pTBEditView->m_nID=nID;
		pTBEditView->m_nIndex=nIndex;


		m_mapIdHWView[nID]=hwnd;
		m_mapIdHWIndex[nID]=nIndex;


		UpdateAllViews(NULL);

	}
	else{
		HWND hwnd=it1->second;

		CView *pview=(CView *)CWnd::FromHandle(hwnd);


		CMDIFrameWnd *pFrameWnd=(CMDIFrameWnd *)AfxGetMainWnd();
		CWnd *pWnd=pview->GetParent();
		if(pWnd->IsKindOf( RUNTIME_CLASS( CSplitterWnd ) ) ){
			pWnd=pWnd->GetParent();
		}
		pFrameWnd->MDIActivate(pWnd);
		if(pview->GetParent()->IsIconic()){

			pview->GetParent()->ShowWindow(SW_RESTORE);
		}
		
	}

}

//Display Patch Panel properties
//Input:
//	strID		Cable identifier
void CDraftDrawDoc::DisplayPatchPProp(CString strID){

	//std::map<int,HWND>::iterator it1=m_mapIdHWView.find(nID);

	void *ptr;
	if(!m_mapStrHWView.Lookup(strID,ptr)||ptr==0){
	//if((it1==m_mapIdHWView.end())||(it1->second==0)){

		int nIndex=0;

		//Do not create edition dialog if TB exits in memory
		//thought view has been destroyed
		if(!m_mapStrHWView.Lookup(strID,ptr)){

			//Dialog patch panel properties
			CDialogPatchPProperties dialogPatchP;

			int index;
			CShape *pSh=(CShape *)FirstObject(index);
			pSh=(CShape *)NextObject(index);
			while(pSh){
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){

					dialogPatchP.m_mapCableNum[std::string(pSh->m_strIdent)]=index;
				}

				pSh=(CShape *)NextObject(index);
			}

			//Display path panel dialog
			if(dialogPatchP.DoModal()==IDOK){

				nIndex=m_obaCables1.Add(dialogPatchP.m_pstraCable1);
				m_obaCables2.Add(dialogPatchP.m_pstraCable2);

				void *ptr;
				if(m_mapStrHWParameter.Lookup(strID,ptr)){
					delete(ptr);//delete obsolete 'TB'
				}
				else{
					m_mapStrHWParameter[strID]=new CIntArray;
				}
			}
			else{

				return;
			}
		}
		else{

			m_mapStrHWIndex.Lookup(strID,ptr);
			nIndex=(int)ptr;
		}


		//Create view for TB
		CFrameWnd * pNewFrame =theApp.pMDTPatchP->CreateNewFrame(this,NULL);

		//Check if frame has not been created properly
		if (pNewFrame == NULL)
			 return ;

		theApp.pMDTPatchP->InitialUpdateFrame(pNewFrame, this);

		HWND hwnd=pNewFrame->GetActiveView()->m_hWnd;

		CTBEditView *pTBEditView=(CTBEditView *)pNewFrame->GetActiveView();

		pTBEditView->m_nID=0;
		pTBEditView->m_strID=strID;
		pTBEditView->m_nIndex=nIndex;


		m_mapStrHWView[strID]=hwnd;
		m_mapStrHWIndex[strID]=(void*)nIndex;


		UpdateAllViews(NULL);

	}
	else{
		HWND hwnd=(HWND)ptr;

		CView *pview=(CView *)CWnd::FromHandle(hwnd);


		CMDIFrameWnd *pFrameWnd=(CMDIFrameWnd *)AfxGetMainWnd();
		CWnd *pWnd=pview->GetParent();
		if(pWnd->IsKindOf( RUNTIME_CLASS( CSplitterWnd ) ) ){
			pWnd=pWnd->GetParent();
		}
		pFrameWnd->MDIActivate(pWnd);
		if(pview->GetParent()->IsIconic()){

			pview->GetParent()->ShowWindow(SW_RESTORE);
		}
		
	}

}

//Serialize patch panel
void CDraftDrawDoc::SerializePatchP(CArchive& ar,UINT nID){

	std::map<int,HWND>::iterator it1=m_mapIdHWView.find(nID);

	if((it1!=m_mapIdHWView.end())&&(it1->second!=0)){

		/*
		std::map<int,int>::iterator it2=m_mapIdHWIndex.find(nID);
		int nIndex=it2->second;
		*/
	}
}

//Loads a rack document from an schematic
void CDraftDrawDoc::LoadRack(CDraftDrawDoc *pSchDoc){

	TCHAR sConnect[1024];

	g_GetConnectString(sConnect,1024);


	//Connect to database
	CDatabase db;
	db.OpenEx(sConnect);

	CRecordset rsConnector(&db);

	rsConnector.m_strFilter="bRack<>0";
	rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	int nIndex=1;
	float fCountUMax=0.0;//Maximum number of U's in rack
	CString str;
	CString strLib;
	CString strNameRack;//full qualified rack name (with library path).

	CRecordset rsLib(&db);
	rsConnector.GetFieldValue("iIdLib",str);
	rsLib.m_strFilter="iIdLib="+str;
	rsLib.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");
	rsLib.GetFieldValue("nNameLib",strLib);
	rsLib.Close();

	rsConnector.GetFieldValue("nNamePart",str);
	CString strU;
	rsConnector.GetFieldValue("fU",strU);
	fCountUMax=atof(strU)  /*-30.0*/;


	m_pSh=new CShapeUnit();
	str=/*CString(DCABLE_RACKDIR)+*/str;
	strNameRack=strLib+"."+str;
	m_pSh->LoadUnit(strLib+"."+str);
	m_pSh->m_pCursorArray=m_CursorArray;
	m_iCursor=11;


	m_pSh->m_Rect+=CPoint(50,50);///!!!magic number


	AddObject(m_pSh);
	m_pSh->OnLButtonUp(0, CPoint(0,0));


	//1st Iteration of schematic document
	//Calculate number of rack
	//Calculate number of parts of each weight
	
	int index;
	int i=0;
	CShape *pSh;
	/*
	CShape *pSh=(CShape *)pSchDoc->FirstObject(index);
	pSh=(CShape *)pSchDoc->NextObject(index);
	while(pSh){




		pSh=(CShape *)pSchDoc->NextObject(index);
		i++;
	}
	*/

	//2nd Iteration of schematic document
	//ChangeSheet
	int nIndex_prev=pSchDoc->ChangeSheet(0);
	for(int k=0;k<2;k++){
		pSchDoc->ChangeSheet(0);
		float fCountU=0.0;//Acumulator to count part height
		for(int j=0;j<pSchDoc->GetSheetCount();j++){
			pSchDoc->ChangeSheet(j);


			i=0;
			pSh=(CShape *)pSchDoc->FirstObject(index);
			pSh=(CShape *)pSchDoc->NextObject(index);
			while(pSh){

				if(!pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
					pSh=(CShape *)pSchDoc->NextObject(index);
					continue;
				}

				CShapeUnit *pShUnit=(CShapeUnit *)pSh;

				CString strFile=pShUnit->m_sUnitName;
				//get libray name
				CString strLib=strFile.Left(strFile.Find('.'));
				//remove library name
				strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

				rsConnector.m_strFilter="nNamePart='"+strFile+"' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
				rsConnector.Requery();

				if(rsConnector.IsEOF()){
					pSh=(CShape *)pSchDoc->NextObject(index);
					continue;
				}

				CString str;
				CString strId;
				rsConnector.GetFieldValue("iIdRack",str);
				rsConnector.GetFieldValue("iIdPart",strId);

				if(atoi(str)<=0){
					//2nd query
					rsConnector.m_strFilter="iIdRack="+strId;
					rsConnector.Requery();
					if(rsConnector.IsEOF()){
						pSh=(CShape *)pSchDoc->NextObject(index);
						continue;
					}
					else{
						rsConnector.GetFieldValue("iIdPart",str);
					}
				}


				rsConnector.m_strFilter="iIdPart="+str+"";
				rsConnector.Requery();

				//1st iteration: only compute part height and create new rack as neccesary
				if(k==0){

					//Compute height when available
					//...
					CString strU;
					rsConnector.GetFieldValue("fU",strU);
					fCountU+=atof(strU);
					//Create new rack when available
					//...
					if(fCountU>fCountUMax){
						m_pSh=new CShapeUnit();
						m_pSh->LoadUnit(strNameRack);
						m_pSh->m_pCursorArray=m_CursorArray;
						m_pSh->m_Rect+=CPoint(350,50);///!!!magic number
						AddObject(m_pSh);
						m_pSh->OnLButtonUp(0, CPoint(0,0));
						fCountU=0.0;
					}

					pSh=(CShape *)pSchDoc->NextObject(index);
					i++;
					continue;
				}


				CRecordset rsLib(&db);
				rsConnector.GetFieldValue("iIdLib",str);
				rsLib.m_strFilter="iIdLib="+str;
				rsLib.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");
				rsLib.GetFieldValue("nNameLib",strLib);
				rsLib.Close();

				rsConnector.GetFieldValue("nNamePart",str);


				m_pSh=new CShapeUnit();
				str=/*CString(DCABLE_RACKDIR)+*/str;
				m_pSh->LoadUnit(strLib+"."+str);
				m_pSh->m_pCursorArray=m_CursorArray;
				m_iCursor=11;

				m_pSh->m_Rect+=CPoint(100+i*50,100+i*50);///!!!magic number


				AddObject(m_pSh);
				m_pSh->OnLButtonUp(0, CPoint(0,0));

				pSh=(CShape *)pSchDoc->NextObject(index);
				i++;
			}
		}//end of sheet loop
	}//end of two iteration loop

	pSchDoc->ChangeSheet(nIndex_prev);//restore working sheet

	rsConnector.Close();
	db.Close();
}

//Display patch panel properties
void SDisplayPatchPProp(UINT nID){

	CDraftDrawDoc *pDoc=theApp.GetActiveDocument();

	if(pDoc==NULL){
		return;
	}

	pDoc->DisplayPatchPProp(nID);
}

//Display patch panel properties
void SDisplayPatchPProp(CString strID){

	CDraftDrawDoc *pDoc=theApp.GetActiveDocument();

	if(pDoc==NULL){
		return;
	}

	void *ptr;
	CIntArray *piaParameter;
	if(pDoc->m_mapStrHWIndex.Lookup(strID,ptr)){
		piaParameter=(CIntArray *)ptr;
	}
	else{
		piaParameter=new CIntArray;
		pDoc->m_mapStrHWIndex[strID]=piaParameter;
	}

	pDoc->DisplayPatchPProp(strID);
}

//Serialize patch panel
//not used, delete
void SSerializePatchP(UINT nId){

	CDraftDrawDoc *pDoc=theApp.GetActiveDocument();

	if(pDoc==NULL){
		return;
	}

	//pDoc->SerializePatchP(nId);
}

BOOL CDraftDrawDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(nFilterIndex>_DRAFTCABLE_DOC_ARCHIVED_COUNT){


		if(m_bFlagPartEdit){

			//savebmp(hbm,hpal,saCodec[nFilterIndex-1]);
			POSITION pos=GetFirstViewPosition();
			CView *pView=GetNextView(pos);

			if(pView->IsKindOf(RUNTIME_CLASS(CTreeView))){
				pView=GetNextView(pos);
			}

			int index;
			CShape *pSh=(CShape *)FirstObject(index);
			pSh=(CShape *)NextObject(index);
			while(pSh){

				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){

					CPoint pt=pSh->m_Rect.TopLeft();

					BOOL bError=!ClientToScreen(pView->m_hWnd,&pt);

					//DWORD dwError=GetLastError();

					HBITMAP hBitmap = CopyScreenToBitmap( &(pSh->m_Rect+(pt-pSh->m_Rect.TopLeft())) );

					HPALETTE hPal;
					if (hBitmap)
					hPal = GetSystemPalette();
					else
					hPal = NULL;


					CString strPath=GetPathName();

					wstring wstr=saCodecs[nFilterIndex-1];

					savebmp(hBitmap,hPal,saCodecs[nFilterIndex-1].c_str(),strPath);

					break;
				}

				pSh=(CShape *)NextObject(index);
			}
		}
		else{

			//04/06/2005
			//This method is not correct.
			//Views must save themselves painting in a memory DC.
			/*
			POSITION pos=GetFirstViewPosition();
			CView *pView=GetNextView(pos);

			if(pView->IsKindOf(RUNTIME_CLASS(CTreeView))){
				pView=GetNextView(pos);
			}


			CPoint pt=CPoint(0,0);

			BOOL bError=!ClientToScreen(pView->m_hWnd,&pt);

			//DWORD dwError=GetLastError();

			CRect rect(pt,GetSize());

			HBITMAP hBitmap = CopyScreenToBitmap( &rect );

			HPALETTE hPal;
			if (hBitmap)
			hPal = GetSystemPalette();
			else
			hPal = NULL;


			CString strPath=GetPathName();

			wstring wstr=saCodecs[nFilterIndex-1];

			savebmp(hBitmap,hPal,saCodecs[nFilterIndex-1].c_str(),strPath);
			*/
			UpdateAllViews(NULL,4);//!!!MAGIC NUMBER

		}

		return TRUE;        // success
	}

	CFile* pFile = NULL;
	CFile* pFileOle = NULL;
	CFileException fe;
	LPSTORAGE pStgRoot = NULL;
	LPSTREAM pStream = NULL;

	//Copied from MFC's: doccore.cpp
	//--------------------------------------------------------------------------
	if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1)
	{
//#ifdef DCABLE_COMPOUNDFILE
		//CFileException fe;
		//COleStreamFile* pFile = NULL;
		USES_CONVERSION;
		//LPSTORAGE pStgRoot = NULL;
		int g_nIndent = 0;
		VERIFY(::StgCreateDocfile(T2COLE(lpszPathName),
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
			0, &pStgRoot) == S_OK);
		//Create a new stream
		//LPSTREAM pStream = NULL;
		VERIFY(pStgRoot->CreateStream(T2COLE("maindoc"),
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
			0, 0, &pStream) == S_OK);
		ASSERT(pStream != NULL);
		//construct COleStreamFile
		pFileOle = new COleStreamFile(pStream);
//#else
	}
	else{
		//CFileException fe;
		//CFile* pFile = NULL;
		pFileOle = GetFile(lpszPathName, CFile::modeCreate |
			CFile::modeReadWrite | CFile::shareExclusive, &fe);
	}
//#endif

	if (pFileOle == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CArchiveDb saveArchive(pFileOle, CArchive::store | CArchive::bNoFlushOnDelete);
	if(m_bFlagPartEdit){
		saveArchive.m_bFlagDb=TRUE;
	}
	else{
		saveArchive.m_bFlagDb=FALSE;
	}


	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		Serialize(saveArchive);     // save me
		saveArchive.Close();
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1)
		{
//#ifdef DCABLE_COMPOUNDFILE
			WriteStorage(pStgRoot);		// save persistent objects
			//release method
			delete(pFileOle);
			pStream->Release();
			pStgRoot->Release();
		}
		else
		{
//#else
			ReleaseFile(pFileOle, FALSE);
//#endif
		}
	}
	CATCH_ALL(e)
	{
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1)
		{
//#ifdef DCABLE_COMPOUNDFILE
			//release method
			delete(pFileOle);
			pStream->Release();
			pStgRoot->Release();
		}
		else
		{
//#else
			ReleaseFile(pFileOle, TRUE);
//#endif
		}

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
		//Commented,  see: Microsoft Systems Journal July 1999
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
	//--------------------------------------------------------------------------

	//added by class wizard
	//return CDocument::OnSaveDocument(lpszPathName);
}

int CDraftDrawDoc::Split(CString str,LPTSTR *sa,int max){

	int index = str.Find('|');
	int count=0;
	while(index>=0){
		CString l_str=str.Left(index);
		if(sa!=NULL && count<max){
			strcpy(sa[count],(LPCTSTR)l_str);
		}
		str=str.Right(str.GetLength()-index-1);
		index = str.Find('|');
		count++;
	}
	return count;
}

int CDraftDrawDoc::Split(CString str,CString dmter, LPTSTR *sa, int max){
	CString str0 = str;
	int index = str.Find(dmter);
	int count = 0;
	while (index >= 0){
		CString l_str = str.Left(index);
		if (sa != NULL && count<max){
			strcpy(sa[count], (LPCTSTR)l_str);
		}
		str = str.Right(str.GetLength() - index - 1);
		index = str.Find(dmter);
		count++;
	}
	//This is a difference with the implementation with less arguments
	CString end_str = str0.Right(dmter.GetLength());
	if (end_str != dmter){
		if (sa != NULL && count<max){
			strcpy(sa[count], (LPCTSTR)str);
		}
		count++;
	}
	return count;
}

BOOL CDraftDrawDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//--------------------------------------------------------------------------
	//added by class wizard
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	//--------------------------------------------------------------------------
	
	// TODO: Add your specialized creation code here

	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	//Parse file name to find out filter type.

	std::string str=_DRAFTCABLE_DOC_FILTER_STR;
	CString cstr=_DRAFTCABLE_DOC_FILTER_STR;
	
	//regex("...");
	//int idata=split(str,NULL,0);
	int idata=Split(cstr,NULL,0);

	LPTSTR *sa=new LPTSTR[idata];
	int i;
	for(i=0;i<idata;i++){
		sa[i]=new TCHAR[255];
	}

	//idata=split(str,sa,idata);
	idata=Split(cstr,sa,idata);

	for(i=0;i<idata;i++){
		CString strName(lpszPathName);
		CString strExt="(*."+strName.Right(strName.GetLength()-strName.ReverseFind('.')-1)+")";
		if(CString(sa[i]).Find(strExt)>=0){
			nFilterIndex=(i>>1)+1;
			break;
		}
	}



#ifdef DCABLE_COMPOUNDFILE
	CFileException fe;
	//COleStreamFile* pFile = NULL;
	CFile* pFile = NULL;
	LPSTREAM pStream = NULL;
	LPSTORAGE pStgRoot = NULL;

	if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1){

		//Open storage
		USES_CONVERSION;
		//LPSTORAGE pStgRoot = NULL;
		if (::StgOpenStorage(T2OLE(lpszPathName), NULL,
			STGM_READ | STGM_SHARE_EXCLUSIVE,
			NULL, 0, &pStgRoot) == S_OK){
			ASSERT(pStgRoot != NULL);

		}
		else{

			if (!OnOpenDocument2(lpszPathName)){

				AfxMessageBox("Archivo de almacenamiento no disponible o no legible");
				return FALSE;
			}
			else{
				return TRUE;
			}
		}

		//Enumerate elements in storage
		//LPSTREAM pStream = NULL;
		LPENUMSTATSTG pEnum;
		LPMALLOC pMalloc = NULL;
		STATSTG statstg;
		::CoGetMalloc(MEMCTX_TASK, &pMalloc);
		VERIFY(pStgRoot->EnumElements(0, NULL, 0, &pEnum) == S_OK);
		while (pEnum->Next(1, &statstg, NULL) == NOERROR){

			if (statstg.type == STGTY_STREAM){
				if (_wcsicmp(statstg.pwcsName, T2OLE("maindoc")) == 0){
					//Open stream
					VERIFY(pStgRoot->OpenStream(statstg.pwcsName, NULL,
						STGM_READ | STGM_SHARE_EXCLUSIVE,
						0, &pStream) == S_OK);

					ASSERT(pStream != NULL);
					pMalloc->Free(statstg.pwcsName);
					break;
				}
			}
			pMalloc->Free(statstg.pwcsName);
		}
		//Release method
		pMalloc->Release();
		pEnum->Release();


		if (pStream == NULL){
			AfxMessageBox("Archivo de almacenamiento con formato incorrecto.");
			//Release method
			pStgRoot->Release();
			return FALSE;
		}

		//Create stream file
		pFile = new COleStreamFile(pStream);
	}
	else{
		pFile = GetFile(lpszPathName,
			CFile::modeRead|CFile::shareDenyWrite, &fe);
	}
#else
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
#endif
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive *ploadArchive;
	if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_SVG){
		ploadArchive = new CXMLArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	}
	else
	{
		ploadArchive = new CArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	}
	ploadArchive->m_pDocument = this;
	ploadArchive->m_bForceFlat = FALSE;

	/*
	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	*/
	TRY
	{
		g_bFlagDb=FALSE;//!!!

		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(*ploadArchive);     // load me
		ploadArchive->Close();
#ifdef DCABLE_COMPOUNDFILE
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1){
			ReadStorage(pStgRoot);			// load persisted objects
			delete(pFile);
			//Release method
			pStream->Release();
			pStgRoot->Release();
		}
		else{
			ReleaseFile(pFile, FALSE);
		}
#else
		ReleaseFile(pFile, FALSE);
#endif
		delete(ploadArchive);
	}
	CATCH_ALL(e)
	{
#ifdef DCABLE_COMPOUNDFILE
		if (nFilterIndex == _DRAFTCABLE_DOC_FILTER_DD1){
			delete(pFile);
			//Release method
			pStream->Release();
			pStgRoot->Release();
		}
		else
		{
			ReleaseFile(pFile, TRUE);
		}
#else
		ReleaseFile(pFile, TRUE);
#endif
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//Commented,  see: Microsoft Systems Journal July 1999
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified
	m_iToolSel=_TOOLSELECT_DRAFTCABLE;

	return TRUE;

}

BOOL CDraftDrawDoc::OnOpenDocument2(LPCTSTR lpszPathName) 
{
	//--------------------------------------------------------------------------
	//added by class wizard
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	//--------------------------------------------------------------------------
	
	// TODO: Add your specialized creation code here

	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		g_bFlagDb=FALSE;//!!!

		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();

		ReleaseFile(pFile, FALSE);

	}
	CATCH_ALL(e)
	{

		ReleaseFile(pFile, TRUE);

		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//Commented,  see: Microsoft Systems Journal July 1999
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;

}

int CDraftDrawDoc::AddSheet(){

	CObArray *pObArray=new CObArray();
	m_ObObArray.Add(pObArray);
	//04/12/2004
	mapShapeIdtoObj_t *pmapShapeIdtoObj=new mapShapeIdtoObj_t;
	m_mapShapeIdtoObjArray.push_back(pmapShapeIdtoObj);
	//26/01/2004
	CShArray *pShArray=new CShArray(m_szDesign,CSize(100,100),this);		//Beware CShArray must be resized whe resizing schematic.
	m_ShArrayArray.push_back(pShArray);

	return m_ObObArray.GetSize()-1;
}

int CDraftDrawDoc::ChangeSheet(int nIndex){

	int nIndex_prev=m_nIndexSheet;
	m_nIndexSheet=nIndex;
	m_pObArray=(CObArray *)m_ObObArray[nIndex];
	//04/12/2004
	m_pmapShapeIdtoObj=m_mapShapeIdtoObjArray[nIndex];
	//26/01/2005
	m_pShArray=m_ShArrayArray[nIndex];

	return nIndex_prev;
}

//Get sheet count
int CDraftDrawDoc::GetSheetCount(){

	return m_ObObArray.GetSize();
}

//Return number of sheet that will be serialized
int CDraftDrawDoc::GetSheetSerialCount(){

	return m_shList.GetCount();
}

//Get index beginning with 0 of selected sheet
int CDraftDrawDoc::GetSheet(){

	return m_nIndexSheet;
}


static const TCHAR szContents[] = _T("Contents");
#define INITIAL_ALLOC_SIZE 64000	//max memo field type in MS Access 2000

void CDraftDrawDoc::OnDatabaseSave(CString strFile /*=""*/)
{
	//Locals
	LPLOCKBYTES pLockBytes = NULL;
	BYTE *pStorage = NULL;

	//Check if it is a new file that its being saved
	if(strFile.IsEmpty()){

#ifndef _DCABLE_DATABASEDLG_TREE
		CNameDlg dlg;
		dlg.m_Name=strFile;
		if (dlg.DoModal() == IDCANCEL || dlg.m_Name == "")
		{
			return;
		}
		strFile=dlg.m_Name;
#else
		//Save DB dialog
		CDialogSaveasDB dlg;
		dlg.m_Name=strFile;
		dlg.m_ofn.lpstrFilter=_DRAFTCABLE_DOC_FILTER_STR;
		if (dlg.DoModal() == IDCANCEL )
		{
			return;
		}
		strFile=dlg.m_Name;
#endif
	}

	//Get libray name
	CString strLib=strFile.Left(strFile.Find('.'));
	//Remove library name
	strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

	//Connect to database
	CDatabase db;
	TCHAR sConnect[1024];
	g_GetConnectString(sConnect,1024);

	if (!db.OpenEx(sConnect))
	{
		AfxMessageBox("No se puede conectar con la base de datos.");
		return;
	}

	//Get library ID
	CRecordset rs(&db);

	rs.m_strFilter="nNameLib='"+strLib+"'";
	rs.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");

	CString sIdLib;
	rs.GetFieldValue("iIdLib",sIdLib);

	rs.Close();

	//Select part in library

	rs.m_strFilter="nNamePart='"+strFile+"' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
	rs.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	BOOL bEOF=rs.IsEOF();

	//Check if we and saving a rack part
	CString strQuery;
	CString strRack;
	CString strIdRack;
	strRack.Format("%i",(m_iTypeDocument==_DOCTYPE_RACK));
	if(m_iTypeDocument==_DOCTYPE_RACK && m_pDocRack){
		strIdRack.Format("%i",m_pDocRack->m_iIdPart);
	}
	else{
		strIdRack="0";
	}

	//Set counter to zero: rack chasis part height is calculated during serialization.
	m_fUTemp=0;
	if(bEOF){
		strQuery="INSERT INTO tbPart (nNamePart,iIdRack,fU,bTextPart,bTextBin,bRackType,iIdLib) VALUES ('"+strFile+"',"+strIdRack+",%f,0x%s,1,"+strRack+","+sIdLib+")";
	}
	else{
		strQuery="UPDATE tbPart SET iIdRack="+strIdRack+",fU=%f,bTextPart=0x%s,bTextBin=1,bRackType="+strRack+" WHERE nNamePart='"+strFile+"' AND iIdLib="+sIdLib;

		//For debuging
		//FILE *fout=fopen("log_debugdb.txt","w");
		//std::string str(strQuery);
		//fwrite(str.c_str(),sizeof(char),str.length(),fout);
		//fclose(fout);

	}


	/*rs.m_Name = dlg.m_Name;*/

	/*
	GlobalFree(rs.m_OleObject.m_hData);

	rs.m_OleObject.m_hData = GlobalAlloc(GMEM_MOVEABLE, INITIAL_ALLOC_SIZE);

	if (FAILED(CreateILockBytesOnHGlobal(rs.m_OleObject.m_hData,FALSE,
			   &pLockBytes)))
	{
		AfxMessageBox("Unable to retrieve LockBytes");
		return;
	}
	*/

	/*
	if (FAILED(StgCreateDocfileOnILockBytes(pLockBytes, STGM_DIRECT |
		  STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0,
		  &pStorage)))
	{
	AfxMessageBox("Failure creating storage on LockBytes");
	pLockBytes->Release();
	return;
	}
	*/

	//pStorage = (BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, INITIAL_ALLOC_SIZE);

	CMemFile file;
	CFileException fe;
	//file.Attach(pStorage, INITIAL_ALLOC_SIZE, CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate);
	/*
	{
		AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}
	*/

	//Save to Contents stream
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = TRUE;

	TRY
	{
		nFilterIndex=5;//!!!MAGIC NUMBER

		Serialize(saveArchive);

		// commit the storage
		/*
		SCODE sc = pStorage->Commit(STGC_ONLYIFCURRENT);
		if (sc != S_OK)
		AfxThrowOleException(sc);
		*/
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		THROW_LAST();
	}
	END_CATCH_ALL

	/*
	rs.SetFieldDirty(&rs.m_OleObject);
	rs.SetFieldNull(&rs.m_OleObject, FALSE);
	*/

	/*
	STATSTG statstg;
	pLockBytes->Stat(&statstg,  STATFLAG_NONAME);
	rs.m_OleObject.m_dwDataLength = statstg.cbSize.LowPart;
	*/


	saveArchive.Flush();


	DWORD dwFileLen=file.GetLength();
	pStorage=file.Detach();

	CString strData;
	CString str;

	for(unsigned int i=0;i<dwFileLen;i++){
		str.Format("%02X",pStorage[i]);
		strData+=str;
	}

	//Store part in database
	CString strSQL;
	strSQL.Format(strQuery,m_fUTemp,strData);
	db.ExecuteSQL(strSQL);
	db.Close();

	saveArchive.Close();
	file.Close();


	SetModifiedFlag(FALSE);     // back to unmodified


	/*
	rs.Update();
	rs.Close();
	*/

	/*
	pLockBytes->Release();
	pStorage->Release();
	*/

	//GlobalFree(pStorage);
}

//Load a part from database
void CDraftDrawDoc::OnDatabaseLoad()
{
	BYTE *pStorage = NULL;
	LPLOCKBYTES pLockBytes = NULL;

	//Get part name
	CString strFile=GetPathName1();
	//Get libray name
	CString strLib=strFile.Left(strFile.Find('.'));
	//Remove library name
	strFile=strFile.Right(strFile.GetLength()-strFile.Find('.')-1);

	// Make certain there are objects in the database
	// If there are, bring up dialog for user to select an object
	CDatabase db;
	TCHAR sConnect[1024];

	g_GetConnectString(sConnect,1024);

	if (!db.OpenEx(sConnect))
	{
		AfxMessageBox("Access ODBC Driver not installed");
		return;
	}

	/*
	CObjNameSet rsObjName(&db);

	rsObjName.Open();
	if (rsObjName.IsEOF())
	{
	  rsObjName.Close();
	  AfxMessageBox("No objects exist in the database.");
	  return;
	}

	CObjDlg dlg(&rsObjName);
	if (dlg.DoModal() == IDCANCEL)
	{
	 return;
	}

	rsObjName.Close();
	*/

	CRecordset rs(&db);

	/*CContainerItem * pItem = new CContainerItem;*/

	//rs.m_strFilter = "Name = '" + dlg.m_ObjectName + "'";
	//rs.m_strFilter = "(nNamePart = '" + strFile + "') AND (bTextBin=1) AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
	rs.m_strFilter = "(nNamePart = '" + strFile + "') AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='"+strLib+"')";
	rs.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	CString strType;
	CString strId;
	rs.GetFieldValue("bTextBin",strType);
	rs.GetFieldValue("iIdPart",strId);

	if(rs.IsEOF()){
		return;
	}


	/*
	if (FAILED(CreateILockBytesOnHGlobal(rs.m_OleObject.m_hData, FALSE,
				&pLockBytes)))
	{
	  AfxMessageBox("Unable to retrieve LockBytes");
	  return;
	}

	HRESULT hr = StgOpenStorageOnILockBytes(pLockBytes, NULL,
				STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE  , NULL,
				0, &pStorage);

	if (FAILED(hr))
	{
	 AfxMessageBox("Failure opening storage");
	 pLockBytes->Release();
	 return;
	}
	*/

	CDBVariant dbTextPart;
	CString strName;
	if(atoi(strType)){
	
		rs.GetFieldValue(_T("bTextPart"), dbTextPart);
	
		rs.GetFieldValue(_T("nNamePart"), strName);
		if(dbTextPart.m_dwType!=DBVT_BINARY){
			return;
		}

		//SIZE_T sizet=GlobalSize(dbTextPart.m_pbinary);
		pStorage=(BYTE *)GlobalLock(dbTextPart.m_pbinary->m_hData);
	}
	else{
		rs.GetFieldValue(_T("nTextPart"), dbTextPart);
		rs.GetFieldValue(_T("nNamePart"), strName);
		pStorage=(BYTE *)dbTextPart.m_pstring->GetBuffer(dbTextPart.m_pstring->GetLength());
	}

	/*
	CMemFile file;
	CFileException fe;
	file.Attach(pStorage, INITIAL_ALLOC_SIZE, CFile::modeRead|CFile::shareExclusive);
	*/
	CMemFile *file;
	if(atoi(strType)){
	file=new CMemFile(pStorage, dbTextPart.m_pbinary->m_dwDataLength, 0);
	}
	else{
	file=new CMemFile(pStorage, dbTextPart.m_pstring->GetLength(), 0);
	}
	CFileException fe;

	/*
	{
	 if (fe.m_cause == CFileException::fileNotFound)
		AfxThrowArchiveException(CArchiveException::badSchema);
	 else
		AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}
	*/

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = TRUE;

	TRY
	{
		//Parse part name
		if (!strName.Right(strName.GetLength() - strName.ReverseFind(_T('.')) - 1).CompareNoCase(_T("ddw"))){
			g_bFlagDb=TRUE;
			nFilterIndex=1;
		}
		else{
			nFilterIndex=5;//Binary format
		}

		//Serialization
		Serialize(loadArchive);     // load main contents
		loadArchive.Close();
		file->Close();
		delete(file);

		//!Release previously get buffer.
		if (!strName.Right(strName.GetLength() - strName.ReverseFind(_T('.')) - 1).CompareNoCase(_T("ddw"))){
			g_bFlagDb=FALSE;
			dbTextPart.m_pstring->ReleaseBuffer();
		}

		//Store part identifier as document member
		m_iIdPart=atoi(strId);
	}
	CATCH_ALL(e)
	{
		file->Abort();   // will not throw an exception
		delete(file);
		// remove item because it didn't fully loaded
		/*GetDocument()->RemoveItem(pItem);*/
		THROW_LAST();
	}
	END_CATCH_ALL

	rs.Close();

	db.Close();

	/*pLockBytes->Release();*/

	int index;
	CShape *pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);

	while(pSh!=NULL){

				
		pSh=(CShape *)NextObject(index);
	}

	UpdateAllViews(NULL,1);
}

//Part document parallel framework
//==============================================================================
//==============================================================================
void CDraftDrawDoc::OnNewDocumentPart(){
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	// This function is mainly concerned with template operations.
	// Look template operations performed in SIC dialog: CDialogPlacePart.

	if(GetActiveViewType()==DCVIEWTYPE_SCHEMATIC){
	//if(m_pObArray==&m_ObArray){

		//m_bFlagPartEdit=TRUE;

		//Beware! This code is dublicated in command handler of ID_EDIT_PART.
		//ChangeModality should be implemented ASAP!
		/*
		m_pObArray=&m_ObArrayPart;
		cmdDeque=&m_cmdDequePart;
		//04/12/2004
		m_pmapShapeIdtoObj=&m_mapShapeIdtoObjPart;
		*/
		SetActiveViewType(DCVIEWTYPE_PART);
		//SetupCollections(1);

		if(m_pObArray->GetSize()==0){

			
			CShapeFrmRect *pSh=new CShapeFrmRect(NULL,0,cmdDeque);
			pSh->m_Rect=CRect(200,200,400,350);
			pSh->m_Mode=1;
			pSh->m_pCursorArray=m_CursorArray;
			m_pObArray->Add(pSh);
			

		}
	}

}

BOOL CDraftDrawDoc::OnOpenDocumentPart(LPCTSTR lpszPathName) 
{

	if(m_pObArray==&m_ObArray){

		m_bFlagPartEdit=TRUE;

		m_pObArray=&m_ObArrayPart;

	}

	//Delete content of array
	//==================================================
	m_pObArray->RemoveAll();
	//==================================================

	nFilterIndex=1;
	CFile file(lpszPathName,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone);
	CArchive ar(&file,CArchive::load);
	Serialize(ar);


	return TRUE;
}
//==============================================================================
//==============================================================================

//Get selected object
CObject *CDraftDrawDoc::GetCurSelOb(){

	CShapeUnit *pShUnitSel=NULL;
	for(int i=0;i<m_pObArray->GetSize();i++){
		CShape *pSh=(CShape *)(*m_pObArray)[i];
		if((pSh->m_TypeSelect==_DRAFTDRAW_SEL_MOVING_RECT)&&
			(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit)) )){
			pShUnitSel=(CShapeUnit *)pSh;
			return (*m_pObArray)[i];
		}
	}

	//No object is currently selected.
	return NULL;
}

//Get rack counter part
CString CDraftDrawDoc::GetRackOb(CString strName){

	//Get connecyion string to database
	TCHAR sConnect[1024];
	g_GetConnectString(sConnect,1024);

	//Connect to database
	CDatabase db;
	db.OpenEx(sConnect);

	//Create a recordset to query database
	CRecordset rsConnector(&db);

	//Get the part from database
	rsConnector.m_strFilter="nNamePart='"+strName+"'";
	rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	//If nothing found return empty string
	if(rsConnector.IsEOF()){
		return "";
	}

	//Get id of rack part
	CString str;
	rsConnector.GetFieldValue("iIdRack",str);

	//If we don't have rack part
	if(atoi(str)<=0){
		return "";
	}

	//Get rack part to database
	rsConnector.m_strFilter="iIdPart="+str+"";
	rsConnector.Requery();

	//Get part name
	rsConnector.GetFieldValue("nNamePart",str);

	//Close connection
	rsConnector.Close();
	db.Close();

	//Return part name
	return str;
}

//Add shape
//return value: number of shapes in array
int CDraftDrawDoc::AddShape(DWORD dwType,int left,int top,int right,int bottom,DWORD dwId,std::deque<CDBVariant*> *lpParameters /*=NULL*/){

	CShape *pSh=NULL;
	int idata;		//to pass an integer value from 1st phase to 2nd phase

	switch(dwType){
	case ddcShapeTriangle:
		idata = VERTEX_TRIANGLE;
		break;
	case ddcShapePentagon:
		idata = VERTEX_PENTAGON;
		break;
	case ddcShapeHexagon:
		idata = VERTEX_HEXAGON;
		break;
	case ddcShapeOctogon:
		idata = VERTEX_OCTOGON;
		break;
	}

	switch(dwType){
	case ddcShapeEllipse:
		pSh=new CShapeEllipse(CRect(left,top,right,bottom),dwId);
		break;
	case ddcShapeRectangle:
		pSh=new CShapeRect(CRect(left,top,right,bottom),dwId,cmdDeque);
		break;
	case ddcShapeLine:
		pSh=new CShapeLine(CRect(left,top,right,bottom),dwId,cmdDeque);
		break;
	case ddcShapeNGear:
		pSh=new CShapeUnit(NULL,0,cmdDeque);
		pSh->m_pCursorArray=m_CursorArray;
		pSh->m_Mode=1;
		((CShapeUnit*)pSh)->CreateNGear(CRect(left,top,right,bottom),6,25.0);
		((CShapeUnit*)pSh)->SetupBuffers(CRect(0,0,150,150));
		pSh->m_Rect=CRect(left,top,right,bottom);
		((CShapeUnit*)pSh)->Resize();
		break;
	case ddcShapeNCross:
		pSh=new CShapeUnit(NULL,0,cmdDeque);
		pSh->m_pCursorArray=m_CursorArray;
		pSh->m_Mode=1;
		((CShapeUnit*)pSh)->CreateNCross(CRect(left,top,right,bottom),6,25.0);
		((CShapeUnit*)pSh)->SetupBuffers(CRect(0,0,150,150));
		pSh->m_Rect=CRect(left,top,right,bottom);
		((CShapeUnit*)pSh)->Resize();
		break;
	case ddcShapeNStar:
		pSh=new CShapeUnit(NULL,0,cmdDeque);
		pSh->m_pCursorArray=m_CursorArray;
		pSh->m_Mode=1;
		((CShapeUnit*)pSh)->CreateNStar(CRect(left,top,right,bottom),6,40.0);
		((CShapeUnit*)pSh)->SetupBuffers(CRect(0,0,150,150));
		pSh->m_Rect=CRect(left,top,right,bottom);
		((CShapeUnit*)pSh)->Resize();
		break;
	case ddcShapeTriangle:
	case ddcShapePentagon:
	case ddcShapeHexagon:
	case ddcShapeOctogon:
		pSh=new CShapeUnit(NULL,0,cmdDeque);
		pSh->m_pCursorArray=m_CursorArray;
		pSh->m_Mode=1;
		((CShapeUnit*)pSh)->Create(CRect(left,top,right,bottom),idata,dwType);
		((CShapeUnit*)pSh)->SetupBuffers(CRect(0,0,150,150));
		pSh->m_Rect=CRect(left,top,right,bottom);
		((CShapeUnit*)pSh)->Resize();
		break;
	case ddcShapePolyline:
	case ddcShapeCurve:
		if(lpParameters){
			//Get points
			POINT *points=new POINT[lpParameters->size()];
			int index=0;
			for(std::deque<CDBVariant*>::iterator it=lpParameters->begin();it!=lpParameters->end();it+=2){
				int idata1=(*it)->m_lVal;
				int idata2=(*(it+1))->m_lVal;
				POINT point;
				point.x=idata1;
				point.y=idata2;
				points[index]=point;
				index++;
			}
			//Construct polyline
			switch(dwType){
			case ddcShapePolyline:
				pSh=new CShapePolyline(NULL,0,cmdDeque);
				pSh->m_pCursorArray=m_CursorArray;
				pSh->m_Mode=1;
				((CShapePolyline*)pSh)->Create(points,index);
				break;
			case ddcShapeCurve:
				pSh=new CShapeCurve(NULL,0,cmdDeque);
				pSh->m_pCursorArray=m_CursorArray;
				pSh->m_Mode=1;
				((CShapeCurve*)pSh)->Create(points,index);
				break;
			}
			pSh->m_Rect=CRect(left,top,right,bottom);
			delete(points);
		}
		break;
	case ddcShapeUnit:
		if(lpParameters){
			std::deque<CDBVariant*>::iterator it=lpParameters->begin();
			pSh=new CShapeUnit(CRect(left,top,right,bottom),0,cmdDeque);
			if(it!=lpParameters->end()){
				CString strUnit=*(*it)->m_pstring;
				pSh->LoadUnit(strUnit);
			}
			pSh->m_Rect=CRect(left,top,right,bottom);
		}
		break;
	case ddcShapeLabel:
		if(lpParameters){
			std::deque<CDBVariant*>::iterator it=lpParameters->begin();
			CString strLabel=*(*it)->m_pstring;
			it++;
			int iSize=(*it)->m_lVal;
			it++;
			BOOL bVer=(*it)->m_lVal;
			pSh=new CShapeLabel(NULL,NULL,0,cmdDeque);
			((CShapeLabel*)pSh)->Create(CRect(left,top,right,bottom),strLabel,iSize,bVer);
		}
		break;
	case ddcShapePicture:
		if(lpParameters){
			std::deque<CDBVariant*>::iterator it=lpParameters->begin();
			CString strFileName=*(*it)->m_pstring;
			pSh=new CShapePicture(NULL,0,cmdDeque);
			((CShapePicture*)pSh)->LoadPictureFile(strFileName);
			pSh->m_Rect=CRect(left,top,right,bottom);
		}
		break;
	case ddcShapeWire:
		pSh=new CShapeWire(CRect(left,top,right,bottom),0,cmdDeque);
		break;
	}

	if(pSh!=NULL){
		int nIndex=0;
		pSh->m_pCursorArray=m_CursorArray;
		pSh->m_Mode=1;
		mapShapeIdtoObj_t::iterator it=m_pmapShapeIdtoObj->find(dwId);
		if(it==m_pmapShapeIdtoObj->end()){
			nIndex=AddObject(pSh);
		}
		else{
			pSh=(CShape*)it->second;
		}
		//31/07/2005
		//Add shape to screen map
#ifdef DCABLE_SHARRAY
		m_pShArray->AddShape(pSh);
#endif
		//Beware AddObject asignd a default id (this should obtional)!
		pSh->m_uiShapeId=dwId;
		return nIndex;
	}
	else{
		return -1;
	}
}

//Delete shape by id
void CDraftDrawDoc::DeleteShape(DWORD dwId, BOOL bForce /*=TRUE*/){

	int nIndex;

	mapShapeIdtoObj_t::iterator it=m_pmapShapeIdtoObj->find(dwId);

	if(it!=m_pmapShapeIdtoObj->end()){
		nIndex=FindObject(it->second);
		if(nIndex!=-1){
			DeleteObject(nIndex);
			return;
		}
	}

	//Brute force option
	if(bForce){
		nIndex=FindShape(dwId);
		if(nIndex!=-1){
			DeleteObject(nIndex);
			return;
		}
	}
}

//Find a object in the array
//A more sophisticated shape search function can be implemented,
//that use for example regions in client area where shape are asigned to.
int CDraftDrawDoc::FindObject(CObject *pObFind){

	int index;
	CObject *pOb=FirstObject(index);
	pOb=NextObject(index);
	while(pOb){
		if(pOb==pObFind){
			return index-1;
		}
		pOb=NextObject(index);
	}
	return -1;
}

//Brute force search algorithm
int CDraftDrawDoc::FindShape(UINT nId){

	int index;
	CObject *pOb=FirstObject(index);
	pOb=NextObject(index);
	while(pOb){
		if(((CShape *)pOb)->m_uiShapeId==nId){
			return index-1;
		}
		pOb=NextObject(index);
	}
	return -1;
}

//Select/Deselect shapes
//Latter a criteria can be added to this function to select only certain shapes.
void CDraftDrawDoc::SelectShapes(BOOL bSelect/*=TRUE*/,UINT nId/*=0*/){

	int index;
	CObject *pOb=FirstObject(index);
	pOb=NextObject(index);
	if(nId==0)	while(pOb){
		((CShape *)pOb)->Select(bSelect);
		pOb=NextObject(index);
	}
	else while(pOb){
		if(((CShape *)pOb)->m_uiShapeId==nId){
			((CShape *)pOb)->Select(bSelect);
		}
		pOb=NextObject(index);
	}

}

//Move shapes
void CDraftDrawDoc::MoveShapes(int x,int y,BOOL bAbsolute/*=FALSE*/){

	int index;
	CObject *pOb=FirstObject(index);
	pOb=NextObject(index);
	while(pOb){
		if(((CShape *)pOb)->IsSelected()){
			CRect rectLast=((CShape *)pOb)->m_Rect;
			((CShape *)pOb)->Move(x,y,bAbsolute);
			//30/07/2005
			//Move shape in screen map
#ifdef DCABLE_SHARRAY
			m_pShArray->MoveShape((CShape *)pOb,rectLast);
#endif
		}
		pOb=NextObject(index);
	}
}

//Scale shapes
void CDraftDrawDoc::ScaleShapes(int x,int y,int nScale,BOOL bAbsolute/*=FALSE*/){

	int index;
	CObject *pOb=FirstObject(index);
	pOb=NextObject(index);
	while(pOb){
		if(((CShape *)pOb)->IsSelected()){
			if(!bAbsolute){
				if(nScale==ddcScaleFromTopLeft){
					((CShape *)pOb)->Move(0,0,x,y,FALSE);
				}
				else if(nScale==ddcScaleFromBottomRight){
					((CShape *)pOb)->Move(-x,-y,0,0,FALSE);
				}
			}
			else{
				if(nScale==ddcScaleFromTopLeft){
					int x0=((CShape *)pOb)->m_Rect.left;
					int y0=((CShape *)pOb)->m_Rect.top;
					((CShape *)pOb)->Move(x0,y0,x0+x,y0+y,TRUE);
				}
				else if(nScale==ddcScaleFromBottomRight){
					int x0=((CShape *)pOb)->m_Rect.right;
					int y0=((CShape *)pOb)->m_Rect.bottom;
					((CShape *)pOb)->Move(x0-x,y0-y,x0,y0,TRUE);
				}
			}
		}
		pOb=NextObject(index);
	}
}

void CDraftDrawDoc::OnToolSic() 
{
	//Only a SIC can exist in application.

	CDialogPlacePart pdlgSIC(NULL,1);

	if(pdlgSIC.DoModal()==IDOK){

		m_bFlagPartEdit=TRUE;
		m_pObArray=&m_ObArrayPart;
		cmdDeque=&m_cmdDequePart;

		if(!SaveModified()){

			UpdateAllViews(NULL,1);
			return;
		}

		SetPathName1(pdlgSIC.m_sLibrary+"."+pdlgSIC.m_sPart);
		SetTitle(pdlgSIC.m_sLibrary+"."+pdlgSIC.m_sPart);
		OnDatabaseLoad();
	}

}

//Get path name for part under edition: library.part
const CString& CDraftDrawDoc::GetPathName1( ) const{

	return m_sPathName1;
}

//Set path name for part under edition: library.part
void CDraftDrawDoc::SetPathName1( LPCTSTR lpszPathName, BOOL bAddToMRU /*= TRUE*/ ){

	m_sPathName1=lpszPathName;
}

void CDraftDrawDoc::SetModifiedFlag( BOOL bModified /*= TRUE*/ ){

	if(m_bFlagPartEdit){
		m_bModified1=bModified;
		return;
	}

	//27/03/2005
	if(!bModified){
		cmdDeque->reset_modifiedflag();
	}

	CDocument::SetModifiedFlag(bModified);
}

BOOL CDraftDrawDoc::SaveModified()
{
	if(!m_bFlagPartEdit){
		return CDocument::SaveModified();
	}


	if (!m_bFlagPartEdit&&!IsModified())
		return TRUE;        // ok to continue
	else if(!m_bModified1)
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;

	if (m_sPathName1.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		name = m_sPathName1;
	}


	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		//Default implementation perform following steps:
		//1. BOOL CDocument::DoFileSave() -> Returns false if file is not saved
		//if (dwAttrib & FILE_ATTRIBUTE_READONLY) calls DoSave(NULL)
		//else calls DoSave(m_strPathName)
		//2. BOOL CDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE) -> Returns false if file is not saved
		//if lpszPathName is empty: build file name (with extension) and then call CWinApp::DoPromptFileName(...) -> This function checks if file exists?
		//if continuing call OnSaveDocument(newName)

		//As seen before, this function must implement all seen functionality, in particular:
		//1.Construct save dialog for saving in database.
		//2.Check if part exist in database and promp user.
		OnDatabaseSave();
		/*
		if (!DoFileSave())
			return FALSE;       // don't continue
		*/
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

BOOL CDraftDrawDoc::IsModified( ){

	return cmdDeque->is_modified();
}

//There are two views supported:
//0 ->  schematic
//1 ->  part edition
void CDraftDrawDoc::SetupCollections(UINT nTypeView){

	//This is a good place to put memory when changing back from part to schematic
	//to change to same sheet.
	std::deque<mapShapeIdtoObj_t*>::iterator it;
	switch(nTypeView){
	case DCVIEWTYPE_SCHEMATIC:
		m_pObArray=&m_ObArray;
		cmdDeque=&m_cmdDeque;
		//04/12/2004
		it=m_mapShapeIdtoObjArray.begin();
		m_pmapShapeIdtoObj=(*it);
		m_pnCountId=&m_nCountId;
		break;
	case DCVIEWTYPE_PART:
		m_pObArray=&m_ObArrayPart;
		cmdDeque=&m_cmdDequePart;
		//04/12/2004
		m_pmapShapeIdtoObj=&m_mapShapeIdtoObjPart;
		m_pnCountId=&m_nCountIdPart;
		break;
	}
}

//Type of active view
UINT CDraftDrawDoc::GetActiveViewType(){

	if(m_bFlagPartEdit){
		return DCVIEWTYPE_PART;
	}
	else{
		return DCVIEWTYPE_SCHEMATIC;
	}

}

//Set type of active view
UINT CDraftDrawDoc::SetActiveViewType(UINT nViewType){

	UINT type_prev=DCVIEWTYPE_SCHEMATIC;
	if(m_bFlagPartEdit){
		type_prev=DCVIEWTYPE_PART;
	}
	if(nViewType==type_prev){
		return type_prev;
	}

	m_bFlagPartEdit=(nViewType==DCVIEWTYPE_PART);

	SetupCollections(nViewType);

	return type_prev;
}

//Writes in supplied storage objects that implements persistenece
//called by OnSaveDocument after main data serialization
void CDraftDrawDoc::WriteStorage(LPSTORAGE pStg){

	int index;
	CShape *pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);

	int index2=1;
	while(pSh!=NULL){
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapePicture))){
			CShapePicture *pShPicture=(CShapePicture *)pSh;

			LPPERSISTSTREAM pPersistStream=NULL;
			pShPicture->gpPicture->QueryInterface(IID_IPersistStream,(void**) &pPersistStream);
			ASSERT(pPersistStream != NULL);

			//Create a new stream
			USES_CONVERSION;
			LPSTREAM pStream=NULL;
			CString str;
			str.Format("object%i",index2);
			VERIFY(pStg->CreateStream(T2COLE(str),
				STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE,
				0,0,&pStream)==S_OK);
			ASSERT(pStream!=NULL);

			pPersistStream->Save(pStream, TRUE);
			pPersistStream->Release();
			pStream->Release();


			index2++;

		}
		pSh=(CShape *)NextObject(index);
		
	}

}

//Reads from storage persistent objects
void CDraftDrawDoc::ReadStorage(LPSTORAGE pStg){

	int index;
	CShape *pSh=(CShape *)FirstObject(index);
	pSh=(CShape *)NextObject(index);



	//Enumerate elements in storage
	USES_CONVERSION;
	LPENUMSTATSTG pEnum;
	LPMALLOC pMalloc=NULL;
	STATSTG statstg;
	::CoGetMalloc(MEMCTX_TASK,&pMalloc);
	VERIFY(pStg->EnumElements(0,NULL,0,&pEnum)==S_OK);
	while(pEnum->Next(1,&statstg,NULL)==NOERROR){

		if(statstg.type==STGTY_STREAM){
			if(_wcsicmp(statstg.pwcsName,T2OLE("maindoc"))!=0){
				//Open stream

				LPSTREAM pStream=NULL;

				VERIFY(pStg->OpenStream(statstg.pwcsName, NULL,
					STGM_READ|STGM_SHARE_EXCLUSIVE,
					0,&pStream)==S_OK);

				ASSERT(pStream!=NULL);


				//Search for a ShapePicture that can load object
				while(pSh!=NULL){
					if(pSh->IsKindOf(RUNTIME_CLASS(CShapePicture))){
						CShapePicture *pShPicture=(CShapePicture *)pSh;

						HRESULT hr;
						hr = ::OleLoadPicture(pStream, 0, FALSE, IID_IPicture, (LPVOID *)&pShPicture->gpPicture);
						_ASSERTE(SUCCEEDED(hr) && pShPicture->gpPicture);	

						pStream->Release();
						pStream=NULL;		//to prevent to be released twice;

						//Leave one shape prepared
						pSh=(CShape *)NextObject(index);
						break;
					}
					//Search next
					pSh=(CShape *)NextObject(index);
				}

				//End condition
				if(pSh==NULL){//no more shapes where allocate objects

					pMalloc->Free(statstg.pwcsName);

					if(pStream!=NULL){
						pStream->Release();
					}

					break;
				}

			}
		}
		pMalloc->Free(statstg.pwcsName);
	}
	//Release method
	pMalloc->Release();
	pEnum->Release();




}

//Prepare file extensions for OnFileSaveAs dialog.
void CDraftDrawDoc::DoFileExtensions(std::string& sFilter,CStringArray& saExtensions){

	saCodecs.RemoveAll();

	//static char BASED_CODE szFilter[] = "Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
	sFilter =_DRAFTCABLE_DOC_FILTER_STR;//m_sFilter;//static std::string sFilter = "Draft Draw Files (*.ddw)|*.ddw|Gerber Files (*.gbr)|*.gbr|Cut Files (*.cut)|*.cut|Draft Draw Files 1 (*.dd1)|*.dd1|"; //All Files (*.*)|*.*||;
	saExtensions.Add("ddw");
	saExtensions.Add("gbr");
	saExtensions.Add("cut");
	saExtensions.Add("dxf");
	saExtensions.Add("dd1");
	saExtensions.Add("svg");
	saCodecs.Add(L"none");
	saCodecs.Add(L"none");
	saCodecs.Add(L"none");
	saCodecs.Add(L"none");
	saCodecs.Add(L"none");
	saCodecs.Add(L"none");
	//**************************************************************************
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT  num;        // number of image encoders
	UINT  size;       // size, in bytes, of the image encoder array

	ImageCodecInfo* pImageCodecInfo;

	// How many encoders are there?
	// How big (in bytes) is the array of all ImageCodecInfo objects?
	GetImageEncodersSize(&num, &size);

	// Create a buffer large enough to hold the array of ImageCodecInfo
	// objects that will be returned by GetImageEncoders.
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	// GetImageEncoders creates an array of ImageCodecInfo objects
	// and copies that array into a previously allocated buffer. 
	// The third argument, imageCodecInfo, is a pointer to that buffer. 
	GetImageEncoders(num, size, pImageCodecInfo);

	// Display the graphics file format (MimeType)
	// for each ImageCodecInfo object.
	for(UINT j = 0; j < num; ++j)
	{ 
	  //wprintf(L"%s\n", pImageCodecInfo[j].MimeType);


		ULONG cbAnsi, cCharacters;

		cCharacters = wcslen(pImageCodecInfo[j].FormatDescription)+1;
		(cCharacters<wcslen(pImageCodecInfo[j].FilenameExtension)+1?cCharacters=wcslen(pImageCodecInfo[j].FilenameExtension)+1:cCharacters);
		// Determine number of bytes to be allocated for ANSI string. An
		// ANSI string can have at most 2 bytes per character (for Double
		// Byte Character Strings.)
		cbAnsi = cCharacters*2;


		TCHAR sA[1024];//sA[1024];



		WideCharToMultiByte(CP_ACP, 0, pImageCodecInfo[j].FormatDescription, cCharacters, sA, cbAnsi, NULL, NULL);

				
		sFilter+=sA;
		CString str=sA;
		str.MakeLower();
		saExtensions.Add(str);

		cCharacters=wcslen(pImageCodecInfo[j].FilenameExtension)+1;
		cbAnsi = cCharacters*2;

		WideCharToMultiByte(CP_ACP, 0, pImageCodecInfo[j].FilenameExtension, cCharacters, sA, cbAnsi, NULL, NULL);

		sFilter+=" (";
		sFilter+=sA;
		sFilter+=")|";
		sFilter+=sA;
		sFilter+="|";

		cCharacters=wcslen(pImageCodecInfo[j].MimeType)+1;
		cbAnsi = cCharacters*2;

		WideCharToMultiByte(CP_ACP, 0, pImageCodecInfo[j].MimeType, cCharacters, sA, cbAnsi, NULL, NULL);

		//saCodecs.Add(sA);
		saCodecs.Add(pImageCodecInfo[j].MimeType);
	}

	free(pImageCodecInfo);
	GdiplusShutdown(gdiplusToken);
	//**************************************************************************

	sFilter+="All Files (*.*)|*.*||";
	saExtensions.Add("");
	saCodecs.Add(L"none");
}
void CDraftDrawDoc::OnFileSaveSheetCurrent() 
{
	// TODO: Add your command handler code here
	//Prepare file extensions filter string
	CStringArray saExtensions;
	static std::string sFilter;
	DoFileExtensions(sFilter,saExtensions);






	CString strFile;

	int result;

	do{


	CFileDialog fdialog(
		FALSE,		//save dialog box
		NULL,
		NULL,
		0,			//no flags
		sFilter.c_str(),
		NULL);

	result=IDYES;

	if(fdialog.DoModal()==IDOK){




		int offset=fdialog.m_ofn.nFileExtension;

		strFile=fdialog.m_ofn.lpstrFile;

		nFilterIndex=fdialog.m_ofn.nFilterIndex;

		if((!offset)||(fdialog.m_ofn.lpstrFile[offset]==0)){
			strFile+="."+saExtensions[fdialog.m_ofn.nFilterIndex-1];
			sFilter=saExtensions[fdialog.m_ofn.nFilterIndex-1];
		}


		//check file exists
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;

		hFind = FindFirstFile(strFile, &FindFileData);


		if (hFind != INVALID_HANDLE_VALUE){

			result=AfxMessageBox("File allready exists, overwrite?",MB_YESNOCANCEL,0);

		}

		if(result==IDYES){

			//Create list of sheet to Serialize
			CList<int,int> shList;
			shList.AddTail(GetSheet());
			DoListSheetSave(&shList);

			OnSaveDocument(strFile);

			//Remove all sheets from list of serialization.
			DoListSheetSave(NULL);
		}

	}

	}while(result==IDNO);
	
}

void CDraftDrawDoc::DoListSheetSave(CList<int,int> *pshLst){


	m_shList.RemoveAll();

	if(pshLst==NULL){

		return;	
	}

	POSITION pos = pshLst->GetHeadPosition();
	for (int i=0;i < pshLst->GetCount();i++)
	{
	   int idata= pshLst->GetNext(pos);
	   m_shList.AddTail(idata);
	}

}

void CDraftDrawDoc::OnPlaceSheets() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here

	//Prepare file extensions filter string
	CStringArray saExtensions;
	static std::string sFilter;
	DoFileExtensions(sFilter,saExtensions);






	CString strFile;

	int result;



	CFileDialog fdialog(
		TRUE,		//load dialog box
		NULL,
		NULL,
		0,			//no flags
		sFilter.c_str(),
		NULL);

	result=IDYES;

	if(fdialog.DoModal()==IDOK){

		nFilterIndex=fdialog.m_ofn.nFilterIndex;

		OnInsertDocument(fdialog.m_ofn.lpstrFile);

		UpdateAllViews(NULL,1);

	}

}

BOOL CDraftDrawDoc::OnInsertDocument(LPCTSTR lpszPathName) 
{

	//Parse file name to find out filter type.

	std::string str=_DRAFTCABLE_DOC_FILTER_STR;

	//regex("...");
	int idata=split(str,NULL,0);

	LPTSTR *sa=new LPTSTR[idata];
	int i;
	for(int i=0;i<idata;i++){
		sa[i]=new TCHAR[255];
	}

	idata=split(str,sa,idata);

	for(i=0;i<idata;i++){
		CString strName(lpszPathName);
		CString strExt="(*."+strName.Right(strName.GetLength()-strName.ReverseFind('.')-1)+")";
		if(CString(sa[i]).Find(strExt)>=0){
			nFilterIndex=i+1;
			break;
		}
	}

//This code is only supported for compound files.
#ifdef DCABLE_COMPOUNDFILE

	CFileException fe;
	COleStreamFile* pFile = NULL;
	//Open storage
	USES_CONVERSION;
	LPSTORAGE pStgRoot=NULL;
	if(::StgOpenStorage(T2OLE(lpszPathName), NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,0,&pStgRoot)==S_OK){
		ASSERT(pStgRoot!=NULL);

	}
	else{
		
		AfxMessageBox("Archivo de almacenamiento no disponible o no legible");
		return FALSE;
	}

	//Enumerate elements in storage
	LPSTREAM pStream=NULL;
	LPENUMSTATSTG pEnum;
	LPMALLOC pMalloc=NULL;
	STATSTG statstg;
	::CoGetMalloc(MEMCTX_TASK,&pMalloc);
	VERIFY(pStgRoot->EnumElements(0,NULL,0,&pEnum)==S_OK);
	while(pEnum->Next(1,&statstg,NULL)==NOERROR){

		if(statstg.type==STGTY_STREAM){
			if(_wcsicmp(statstg.pwcsName,T2OLE("maindoc"))==0){
				//Open stream
				VERIFY(pStgRoot->OpenStream(statstg.pwcsName, NULL,
					STGM_READ|STGM_SHARE_EXCLUSIVE,
					0,&pStream)==S_OK);

				ASSERT(pStream!=NULL);
				pMalloc->Free(statstg.pwcsName);
				break;
			}
		}
		pMalloc->Free(statstg.pwcsName);
	}
	//Release method
	pMalloc->Release();
	pEnum->Release();


	if(pStream==NULL){
		AfxMessageBox("Archivo de almacenamiento con formato incorrecto.");
		//Release method
		pStgRoot->Release();
		return FALSE;
	}

	//Create stream file
	pFile=new COleStreamFile(pStream);




	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	SetModifiedFlag();  // dirty due to new document insertion

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		g_bFlagDb=FALSE;//!!!

		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();

		ReadStorage(pStgRoot);					// load persisted objects
		delete(pFile);
		//Release method
		pStream->Release();
		pStgRoot->Release();
	}
	CATCH_ALL(e)
	{

		delete(pFile);
		//Release method
		pStream->Release();
		pStgRoot->Release();

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//Commented,  see: Microsoft Systems Journal July 1999
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

#endif
	return TRUE;
}

CRect CDraftDrawDoc::ResetAllDrwOpe()
{
	//Local variables
	CRect rect;
	int index;
	int index_keep = -1;

	//Iterate all the shapes
	CShape *pSh = (CShape *)this->FirstObject(index);
	pSh = (CShape *)this->NextObject(index);
	while (pSh){
		if ((pSh->m_Mode == _DRAFTDRAW_MODE_SEL) &&
			(pSh->m_TypeSelect == _DRAFTDRAW_SEL_SIZING_RECT)){
			rect = pSh->m_Rect;
			index_keep = index - 1;
			break;
		}
		if (pSh->m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT){
			rect = pSh->m_Rect;
			index_keep = index - 1;
			break;
		}
		//Next shape
		pSh = (CShape *)this->NextObject(index);
	}

	//Changing shape status
	if (index_keep != -1){
		//End polyline drawing delting current segment
		if (this->m_iToolType == _TOOLTYPEPOLY_DRAFTCABLE){
			pSh->OnKeyDown(27, 0, 0);
			pSh->m_Mode = _DRAFTDRAW_MODE_DRW;
		}
		else{
			if (pSh->m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT){
				//Deselect shape when user hits <ESC>
				pSh->Select(FALSE);
			}
			else{
				//pSh->m_Mode=_DRAFTDRAW_MODE_DRW;
				//Deletes current segment
				this->DeleteObject(index_keep);
				//pDoc->m_iToolType=_TOOLTYPENORMAL_DRAFTCABLE;
			}
		}
	}

	//Return shape rectangle for update screen
	return rect;
}

void CDraftDrawDoc::DoSerializeDd1(CArchive& ar){

	if (ar.IsStoring())
	{

		//Set global index to 0
		g_aShBufIndex = 0;

		//Check if we are editing a part
		if (!m_bFlagPartEdit){
			//Check if we are not serializing all the sheets
			if (GetSheetSerialCount()>0){
				ar << GetSheetSerialCount();
			}
			else{
				ar << GetSheetCount();//Send to archive number of sheet
			}
		}

		//Temporal buffer
		CObArray obaBuffer;

		int i;
		for (i = 0; i<GetSheetCount(); i++){

			//Check if sheet is to be serialized
			if (m_shList.GetCount()>0){
				if (!m_shList.Find(i)) continue;
			}

			//Code to iterate shapes in several sheets
			if (GetSheetCount()>1){
				ChangeSheet(i);
			}

			//If we are editing a part seriaze first a shape unit heading
			if (m_bFlagPartEdit){
				CShapeUnit ShUnit;
				ShUnit.m_Rect = CRect(0, 0, 100, 100);
				ShUnit.m_sUnitName = GetTitle();
				ShUnit.Serialize(ar);
			}

			//Main serialization loop
			int index;
			CShape *pSh = (CShape *)FirstObject(index);
			pSh = (CShape *)NextObject(index);
			int idata = GetSizeObject();
			ar << idata;

			CRect rcFrm = CRect(0, 0, 0, 0);
			int nCounter = 0;
			while (pSh != NULL){
				//27/03/2005
				//Debuging. Delete ASAP.
				//----------------------------------------------------------
				/*
				int debug_cont=0;//for debugging
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
				CShapeUnit *pShUnit=(CShapeUnit *)pSh;
				if(pShUnit->m_obarrShapearr.GetSize()>1000){
				debug_cont=1;
				}
				}
				if(debug_cont){
				pSh=(CShape *)NextObject(index);
				continue;
				}
				*/
				//----------------------------------------------------------

				if (pSh->IsKindOf(RUNTIME_CLASS(CShapeRect))){
					CShapePolyline *pShPoly = new CShapePolyline();
					pShPoly->Create(pSh->m_Rect - rcFrm.TopLeft());
					obaBuffer.Add(pShPoly);
					ar << pShPoly;
				}
				else if (pSh->IsKindOf(RUNTIME_CLASS(CShapeFrmRect))){
					rcFrm = pSh->m_Rect;
					m_fUTemp = pSh->m_Rect.Height();
					m_fUTemp /= DCABLE_GRID_US;
					//07/01/2006
					//Shapes appeared with offset at placing into schematics.
					//This code is not ok, becouse
					//when re-editing the shape, it is moved towards origin.
					//It is better to do it in CShapeUnit::Serialize when
					//archive is loading.
					//pSh->m_Rect-=rcFrm.TopLeft();
					ar << pSh;
				}
				else if (pSh->IsKindOf(RUNTIME_CLASS(CShapeLabel))){
					*((CShapeLabel*)pSh)->m_Label.rect = CRect((pSh->m_Rect - rcFrm.TopLeft()).TopLeft(), CSize(0, 0));
					pSh->m_Rect -= rcFrm.TopLeft();
					ar << pSh;
				}
				else if (pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){
					pSh->m_Rect -= rcFrm.TopLeft();
					ar << pSh;
					pSh->m_Rect += rcFrm.TopLeft();
					//05/04/2005
					//Added to store path panels if applicable
					//This must be substituted by a function call
					//to determine if a shape is of type 'TB'.
					//======================================================
					CString strFile = ((CShapeUnit*)pSh)->m_sUnitName;
					//get libray name
					CString strLib = strFile.Left(strFile.Find('.'));
					//remove library name
					strFile = strFile.Right(strFile.GetLength() - strFile.Find('.') - 1);

					//Check if part is of type TB
					CString str = strFile;

					int nIndex = str.Find('.');

					if (nIndex >= 0){
						str = str.Left(nIndex);
					}

					if (str.CompareNoCase("TB") == 0){

						//SerializePatchP(ar,((CShapeUnit*)pSh)->m_uiShapeId);
					}
					//======================================================
				}
				else{
					pSh->m_Rect -= rcFrm.TopLeft();
					ar << pSh;
					pSh->m_Rect += rcFrm.TopLeft();
				}

				//07/04/2005
				m_fVer = pSh->m_fVer;


				nCounter++;
				pSh = (CShape *)NextObject(index);
			}//end while loop

			nCounter = nCounter;
		}

		//05/04/2005
		//Save TB's
		float fVer0 = 1.05;
		float fData = m_fVer - fVer0;
		if (fData >= 0){
			ar << m_obaCables1.GetSize();
			int i;
			for (i = 0; i<m_obaCables1.GetSize(); i++){
				CStringArray* pstrA;
				pstrA = m_obaCables1[i];
				ar << pstrA;
			}
			ar << m_obaCables2.GetSize();
			for (i = 0; i<m_obaCables2.GetSize(); i++){
				CStringArray* pstrA;
				pstrA = m_obaCables2[i];
				ar << pstrA;
			}
			//ar<<&m_mapIdHWIndex;
		}

		//Clean up
		for (i = 0; i<obaBuffer.GetSize(); i++){
			delete(obaBuffer[i]);
		}
		obaBuffer.RemoveAll();
		for (i = 0; i<g_aShBuffer.GetSize(); i++){
			delete(g_aShBuffer[i]);
		}
		g_aShBuffer.RemoveAll();

	}//end if storing
	else
	{

	}

}