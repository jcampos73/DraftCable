
#include "stdafx.h"
#include "resource.h"
#include "Globals.h"
#include "ShapeUnit.h"

//Marx MPI support
#include "mpi.h"

/*afxVarname		global variable*/
/*AfxFuncname		global function*/

//Global strings
CString g_sDCableBaseDir=".\\";
CString g_sConnectString;

//Global db conection to load units for example
CDatabase g_db;

//Global gdiplus Token: used for startup and shut down
ULONG_PTR g_gdiplusToken;

//Goblal current part library
CString g_sCurrentLibrary="Standard100";

//Funtionality of following structures should be derived to database
//******************************************************************************	
CStringArray	afxCableList;
CStringArray	afxConnectorList;

CUIntArray		afxCableWireCount;		//holds number of wires for cable
CUIntArray		afxConnectorWireCount;	//holds number o pines for connector

typedef struct conninfotag{
int nCountWire;		//number of pins in the conector / number of wires in the cable
bool bLogical;		//1 -> logical connector (can match any cable with any wires)
bool bTB;			//1 -> TB connector (can reasign pins)
}conninfo;

//Map of cable / conector id's to cable / connector information structures
std::map<int,conninfotag> g_mapIdConnInfo;

//******************************************************************************	


CImageList m_ImageList;
int iHardDrv;
int iFolderClose;
int iMyComp;
int iFolderOpen;
int iSheet;
int iDB;
int iLibrary;
int iPart;
int iPartRack;

int g_iX=0;
int g_iY=0;

//Image list for patterns used in application
CImageList g_ImageListPatterns;

extern CString afxWinwordPath;

void _doFixDataBase();

BOOL LoadProfileSettings(){

	//First of all check hardkey security if it's enabled
#ifdef DCABLE_SECURITY_HARDKEY
/*
Opens a communication channel with a CRYPTO-BOX. Before you call MPI_Open
you need to
• submit access codes via MPI_SubmitPassword;
• set a channel via MPI_SetChannel and
• search for MARX hardware via MPI_SearchFirst.
• For remote access MPI_SetNetworkParameter() needs to be called before
MPI_Open().
*/

	DWORD Identifier=0;
	DWORD ErrorCode;
	char ErrorMessage[1024];
	CString m_sMsg;

	DWORD Reserved=0;
	ErrorCode=MPI_SetChannel("CBU","USB",&Reserved);

	if(ErrorCode){
		MPI_GetLastError (&ErrorCode, ErrorMessage);
		m_sMsg.Format("Error=%i: ",ErrorCode);
		m_sMsg+=ErrorMessage;
/*
		SendDlgItemMessage(IDC_LIST1,LB_ADDSTRING,0,(LPARAM)m_sMsg.GetBuffer(0));
		m_sMsg.ReleaseBuffer();
		UpdateData(FALSE);
*/
		return FALSE;
	}


	char PasswordType[] = "PASSWORD_ID1";
	char AccessMode[] = "READ";
	//CustomerID	69012
	//CustomerName	CRWeb
	//Project		2016
	BYTE Password[] = {0x4C,0x6B,0x4F,0x4E,0x34};
	//BYTE Password[] = {0x64,0x65,0x6D,0x6F};// {"demo"}
	int PasswordLen = sizeof(Password);// {4}
	/*
	{ Password - reference to a byte sequence (array)
	PasswordLen - length of the array (number of bytes)
	}
	*/
	ErrorCode=MPI_SubmitPasswordEx (PasswordType, AccessMode, Password,PasswordLen);

	if(ErrorCode){
		MPI_GetLastError (&ErrorCode, ErrorMessage);
		m_sMsg.Format("Error=%i: ",ErrorCode);
		m_sMsg+=ErrorMessage;
/*
		SendDlgItemMessage(IDC_LIST1,LB_ADDSTRING,0,(LPARAM)m_sMsg.GetBuffer(0));
		m_sMsg.ReleaseBuffer();
		UpdateData(FALSE);
*/
		return FALSE;
	}


	ErrorCode = MPI_Open(Identifier);

	if(ErrorCode){
		MPI_GetLastError (&ErrorCode, ErrorMessage);
		m_sMsg.Format("Error=%i: ",ErrorCode);
		m_sMsg+=ErrorMessage;
/*
		SendDlgItemMessage(IDC_LIST1,LB_ADDSTRING,0,(LPARAM)m_sMsg.GetBuffer(0));
		m_sMsg.ReleaseBuffer();
		UpdateData(FALSE);
*/
		return FALSE;
	}

	ErrorCode = MPI_Close (Identifier);

	if(ErrorCode){
		MPI_GetLastError (&ErrorCode, ErrorMessage);
		m_sMsg.Format("Error=%i: ",ErrorCode);
		m_sMsg+=ErrorMessage;
/*
		SendDlgItemMessage(IDC_LIST1,LB_ADDSTRING,0,(LPARAM)m_sMsg.GetBuffer(0));
		m_sMsg.ReleaseBuffer();
		UpdateData(FALSE);
*/
		return FALSE;
	}

	m_sMsg="Key accessed successfully!";
/*
	SendDlgItemMessage(IDC_LIST1,LB_ADDSTRING,0,(LPARAM)m_sMsg.GetBuffer(0));
	m_sMsg.ReleaseBuffer();
	UpdateData(FALSE);
*/

#endif

	g_sDCableBaseDir=AfxGetApp()->GetProfileString("Settings","BaseDir",".\\");

	g_sConnectString=AfxGetApp()->GetProfileString("Settings","ConnectString","");

	afxWinwordPath=AfxGetApp()->GetProfileString("Settings","WinwordPath","C:\\Archivos de programa\\Microsoft Office\\Office");

	return TRUE;
}

void AfxInitGlobal(){

   // Initialize GDI+.
   GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);


	CreateImagelist();
	CreateImagelistPatterns();

	//**************************************************************************
	//Prepare TABLE_PIN_LABEL
	//At this moment information is redundant in TABLE_PIN_LABEL and afxConnectorList
	//This must be unified when connector and wire sheet will be accessed at a Data Base!!!
	//**************************************************************************

	//Fix multiple 'blank' parts
	_doFixDataBase();

	int nCount=AfxGetCableCount();


	nCount=AfxGetConnectorCount();

	//Allocates memory, check it is deallocated!
	TABLE_PIN_LABEL=new LPSTR[nCount];

	for(int i=0;i<nCount;i++){

		TABLE_PIN_LABEL[i]=new char[MAX_PATH];
	}

	AfxGetConnector(TABLE_PIN_LABEL,nCount,MAX_PATH);
}

int CreateImagelist(){

#define BITMAP_WIDTH	16
#define BITMAP_HEIGHT	16
#define NUM_BITMAPS		4

	m_ImageList.Create(BITMAP_WIDTH,
	  BITMAP_HEIGHT,
	  FALSE,   // list does not include masks
	  NUM_BITMAPS,
	  0 );   // list will not grow

	HICON hIcon;

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_MYCOMP));
	iMyComp=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_HARDDRV));
	iHardDrv=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_FOLDER_CLOSE));
	iFolderClose=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_FOLDER_OPEN));
	iFolderOpen=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_SHEET));
	iSheet=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_DB));
	iDB=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_LIBRARY));
	iLibrary=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_PART));
	iPart=m_ImageList.Add(hIcon);

	hIcon = ::LoadIcon (AfxGetResourceHandle(), 
                   MAKEINTRESOURCE(IDI_PARTRACK));
	iPartRack=m_ImageList.Add(hIcon);

	//
	// Make sure all of the bitmaps were added
	//
	/*
	if (m_ImageList.GetImageCount() < NUM_BITMAPS)
		return -1;
	*/

	return 0;
}

int CreateImagelistPatterns(){

	int bitmap_width	=48;
	int bitmap_height	=48;
	int num_bitmaps		=1;

	g_ImageListPatterns.Create(bitmap_width,
	  bitmap_height,
	  FALSE,   // list does not include masks
	  num_bitmaps,
	  0 );   // list will not grow

	CBitmap bm;

	bm.LoadBitmap(IDB_PATTERN1_48);

	g_ImageListPatterns.Add(&bm,RGB(0,0,0));

	//
	// Make sure all of the bitmaps were added
	//
	/*
	if (m_ImageList.GetImageCount() < num_bitmaps)
		return -1;
	*/

	return 0;
}

CString AfxGetCablePath(){
	return g_sDCableBaseDir+DCABLE_CABLEDIR;
}

int AfxGetCableCount(){

	if(afxCableList.GetSize()==0){
		return AfxGetCables(NULL,0,0);
	}
	else{
		return afxCableList.GetSize();
	}

}

int AfxGetCables(LPTSTR	*lpCableList,int nCount,int nMaxLen){

	if(lpCableList==NULL){

		afxCableList.RemoveAll();

		//Access database
		//**********************************************************************
		//**********************************************************************
		std::string sFileDsn=std::string(g_sDCableBaseDir)+std::string(DCABLE_DATABASEDIR)+std::string("dc.dsn");
		//std::string sConnect="";
		std::string sNameConnector;

		char buffer[MAX_PATH];

		//Connect to database
		CDatabase db;
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);

		db.OpenEx(sConnect);

		CRecordset rsConnector(&db);

		/*
		rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbCables ORDER BY iIdCable ASC");

		while((!rsConnector.IsBOF())&&(!rsConnector.IsEOF())){

			int nIndex=1;
			CString str;
			rsConnector.GetFieldValue(nIndex,str);
			sNameConnector=str;

			if(sNameConnector.find("txt")==std::string::npos){
				sNameConnector+=".txt";
			}

			
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;

			CString sPath=DCABLE_CONNECTORDIR;
			sPath+=sNameConnector.c_str();//"*.txt";

			int nCount=0;

			hFind=FindFirstFile(sPath,&FindFileData);

			if (hFind != INVALID_HANDLE_VALUE) {

				//do{

					if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
						CString sName=FindFileData.cFileName;

					}

					//Count pin number
					//This will not be neccesary when derived to database
					//**********************************************************
					FILE *fin=fopen(sPath,"r");
					
					while(fgets(buffer,MAX_PATH,fin)!=NULL){
						if(strlen(buffer)>1)
							nCount++;
					}
					
					fclose(fin);
					//**********************************************************

				//}while(FindNextFile(hFind,&FindFileData));
			}

			if(hFind){
				::FindClose(hFind);
			}

			//Add cable to list
			afxCableList.Add(sNameConnector.c_str());

			afxCableWireCount.Add(nCount-1);

			conninfo infoConn;
			infoConn.nCountWire=nCount-1;
			rsConnector.GetFieldValue("bLogical",str);
			infoConn.bLogical=atoi(str);
			if(infoConn.bLogical){
				int debug=0;
			}

			nIndex=0;
			rsConnector.GetFieldValue(nIndex,str);
			int idata=atoi(str);
			idata=afxConnectorList.GetSize()-1;
			g_mapIdConnInfo[idata]=infoConn;

			rsConnector.MoveNext();

		}//end database while

		rsConnector.Close();
		db.Close();

		return afxConnectorList.GetSize();
		*/
		//**********************************************************************
		//**********************************************************************

		//Code to access files
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;

		CString sPath = AfxGetCablePath();//DCABLE_CABLEDIR;
		sPath+="*.txt";

		hFind=FindFirstFile(sPath,&FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) {

			do{

				if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
					CString sName=FindFileData.cFileName;
					afxCableList.Add(sName);
					//Count wire number
					//This will not be neccesary when derived to database
					//**********************************************************
					FILE *fin = fopen(AfxGetCablePath() + sName, "r");
					int nCount=0;
					while(fgets(buffer,MAX_PATH,fin)!=NULL){
						if(strlen(buffer)>1)
							nCount++;
					}
					afxCableWireCount.Add(nCount-1);
					fclose(fin);
					//**********************************************************
				}

			}while(FindNextFile(hFind,&FindFileData));

		}
		
		if(hFind){
			::FindClose(hFind);
		}

		return afxCableList.GetSize();

	}
	else{

		int i;
		for(i=0;((i<nCount)&&(i<afxCableList.GetSize()));i++){
			CString sName=afxCableList[i].Left(nMaxLen);
			strcpy(lpCableList[i],sName);
		}

		return i;
	}
}

CString AfxGetConnectorPath(){
	return g_sDCableBaseDir+DCABLE_CONNECTORDIR;
}

//Get connector count
int AfxGetConnectorCount(){

	if(afxConnectorList.GetSize()==0){
		return AfxGetConnector(NULL,0,0);
	}
	else{
		return afxConnectorList.GetSize();
	}

}

//Get connectores
int AfxGetConnector(LPTSTR	*lpConnectorList,int nCount,int nMaxLen){

	//Check if connector list is empty and if so initalizes it
	if(lpConnectorList==NULL){

		//Access database
		//**********************************************************************
		//**********************************************************************
		std::string sFileDsn=std::string(g_sDCableBaseDir)+std::string(DCABLE_DATABASEDIR)+std::string("dc.dsn");
		//std::string sConnect="";
		std::string sNameConnector;

		char buffer[MAX_PATH];

/*
		FILE *fin=fopen(sFileDsn.c_str(),"r");

		//Constructs connection string
		while(fgets(buffer,MAX_PATH,fin)!=NULL){

			std::string sBuffer(buffer);

			if(sBuffer[0]=='['){
				continue;
			}

			int nIndex;
			while((nIndex=sBuffer.find_first_of("[]\n\r")) >=0){
				sBuffer.replace(nIndex,1,"");

			}


			if(sConnect.length()>0){
				sConnect+=";";
			}


			sConnect+=sBuffer;

		}

		fclose(fin);
*/

		//Connect to database
		CDatabase db;
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);

		db.OpenEx(sConnect);

		CRecordset rsConnector(&db);

		rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbConnector ORDER BY iIdConnector ASC");

		while((!rsConnector.IsBOF())&&(!rsConnector.IsEOF())){

			int nIndex=1;
			CString str;
			rsConnector.GetFieldValue(nIndex,str);
			sNameConnector=str;

			if(sNameConnector.find("txt")==std::string::npos){
				sNameConnector+=".txt";
			}		

		//**********************************************************************
		//**********************************************************************


			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;

			CString sPath = AfxGetConnectorPath();//DCABLE_CONNECTORDIR;
			sPath+=sNameConnector.c_str();//"*.txt";


			int nCount=0;


			hFind=FindFirstFile(sPath,&FindFileData);

			if (hFind != INVALID_HANDLE_VALUE) {

				//do{

					if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
						CString sName=FindFileData.cFileName;

					}

					//Count pin number
					//This will not be neccesary when derived to database
					//**********************************************************
					FILE *fin=fopen(sPath,"r");
					
					while(fgets(buffer,MAX_PATH,fin)!=NULL){
						if(strlen(buffer)>1)
							nCount++;
					}

					fclose(fin);
					//**********************************************************

				//}while(FindNextFile(hFind,&FindFileData));

			}
			
			if(hFind){
				::FindClose(hFind);
			}

			//Add cable to list
			afxConnectorList.Add(sNameConnector.c_str());

			afxConnectorWireCount.Add(nCount-1);

			conninfo infoConn;
			infoConn.nCountWire=nCount-1;
			rsConnector.GetFieldValue("bLogical",str);
			infoConn.bLogical=atoi(str);
			if(infoConn.bLogical){
				int debug=0;
			}

			nIndex=0;
			rsConnector.GetFieldValue(nIndex,str);
			int idata=atoi(str);
idata=afxConnectorList.GetSize()-1;
			g_mapIdConnInfo[idata]=infoConn;


			rsConnector.MoveNext();

		}//end database while

		rsConnector.Close();
		db.Close();

		return afxConnectorList.GetSize();

	}
	else{

		int i;
		for(i=0;((i<nCount)&&(i<afxConnectorList.GetSize()));i++){
			CString sName=afxConnectorList[i].Left(nMaxLen);
			strcpy(lpConnectorList[i],sName);
		}

		return i;
	}
}

int AfxGetConnectorPintCount(int iType){

	CString sPath;
	int nDefaultPin = 0;
	if(iType>= 0 && iType < AfxGetConnectorCount()){
		sPath=TABLE_PIN_LABEL[iType];
	}
	else{
		sPath=TABLE_PIN_LABEL[nDefaultPin];
	}

	for(int i=0;i<afxConnectorList.GetSize();i++){
		if(sPath==afxConnectorList[i]){
			int idata=afxConnectorWireCount[i];
			return afxConnectorWireCount[i];
		}
	}

	return 0;
}

int AfxGetCableWireCount(CString strTypeIdent){

	for(int i=0;i<afxCableList.GetSize();i++){
		if(strTypeIdent==afxCableList[i]){
			int idata=afxCableWireCount[i];
			return afxCableWireCount[i];
		}
	}

	return 0;
}

BOOL AfxIsConnectorLogical(int iType){

	BOOL bdata=false;
	std::map<int,conninfotag>::iterator it=g_mapIdConnInfo.find(iType);

	if(it!=g_mapIdConnInfo.end()){
		bdata=it->second.bLogical;
		if(bdata){
			int debug=0;
		}
	}

	return bdata;
}

BOOL AfxIsConnectorTB(int iType){

	BOOL bdata=false;
	std::map<int,conninfotag>::iterator it=g_mapIdConnInfo.find(iType);

	if(it!=g_mapIdConnInfo.end()){
		bdata=it->second.bTB;
		if(bdata){
			int debug=0;
		}
	}

	return bdata;
}

int g_GetConnectString(LPTSTR lpConnStr,int nMaxLen){

#define _DCABLE_CONNSTRING_REGISTER
#ifndef _DCABLE_CONNSTRING_REGISTER
	std::string sFileDsn=std::string(g_sDCableBaseDir)+std::string(DCABLE_DATABASEDIR)+std::string("dc.dsn");
	std::string sConnect="";
	std::string sNameConnector;


	FILE *fin=fopen(sFileDsn.c_str(),"r");


	char buffer[MAX_PATH];
	//Constructs connection string
	while(fgets(buffer,MAX_PATH,fin)!=NULL){

		std::string sBuffer(buffer);

		if(sBuffer[0]=='['){
			continue;
		}

		int nIndex;
		while((nIndex=sBuffer.find_first_of("[]\n\r")) >=0){
			sBuffer.replace(nIndex,1,"");

		}


		if(sConnect.length()>0){
			sConnect+=";";
		}


		sConnect+=sBuffer;

	}

	strcpy(lpConnStr,sConnect.substr(0,nMaxLen-1).c_str());

	fclose(fin);

	return strlen(lpConnStr);
#else

	strncpy(lpConnStr,g_sConnectString,nMaxLen);
	lpConnStr[nMaxLen-1]=0;
	return strlen(lpConnStr);
#endif
}

//Snap to grid. Coordinates are schematic.
void SnapToGrid(LPPOINT lpPoint, CSize szGrid){

	//SNAP TO GRID
	lpPoint->x = ((int)((lpPoint->x + szGrid.cx*.5) / szGrid.cx)) * szGrid.cx;
	lpPoint->y = ((int)((lpPoint->y + szGrid.cy*.5) / szGrid.cy)) * szGrid.cy;
}

// We are not jet including CStringEx in stdafx.h
// so this method is commented
/*
int RegSearchReplace(CStringEx& string, LPCTSTR sSearchExp,
	LPCTSTR sReplaceExp)
{
	int nPos = 0;
	int nReplaced = 0;
	CRegExp r;
	LPTSTR str = (LPTSTR)(LPCTSTR)string;

	r.RegComp(sSearchExp);
	while ((nPos = r.RegFind((LPTSTR)str)) != -1)
	{
		nReplaced++;
		TCHAR *pReplaceStr = r.GetReplaceString(sReplaceExp);

		int offset = str - (LPCTSTR)string + nPos;
		string.Replace(offset, r.GetFindLen(),
			pReplaceStr);

		// Replace might have caused a reallocation
		str = (LPTSTR)(LPCTSTR)string + offset + _tcslen(pReplaceStr);
		delete pReplaceStr;
	}
	return nReplaced;
}
*/

/////////////////////////////////////////////////////////////////////////////
// Private methods

void _doFixDataBase()
{
	//Connect to database
	CDatabase db;
	TCHAR sConnect[1024];

	g_GetConnectString(sConnect, 1024);

	db.OpenEx(sConnect);

	CRecordset rsPart(&db);

	TRY{
		CString query;
		query.Format("SELECT iIdPart FROM tbPart WHERE nNamePart LIKE '%s' ORDER BY iIdPart ASC", "blank");

		rsPart.Open(CRecordset::forwardOnly, query);

		if ((!rsPart.IsBOF()) && (!rsPart.IsEOF())){
			int nIndex = 0;
			CString strIdPart;
			rsPart.GetFieldValue(nIndex, strIdPart);

			CString deleteQuery;
			deleteQuery.Format("DELETE FROM tbPart WHERE nNamePart LIKE '%s' AND iIdPart NOT IN (%s)", "blank", strIdPart);
			db.ExecuteSQL(deleteQuery);
		}
	}
	CATCH_ALL(e)
	{
		char msg[255];
		e->GetErrorMessage(msg, 255);
		int stop = 1;
	}
	END_CATCH_ALL

	rsPart.Close();
	db.Close();
}