// DialogPlacePart.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPlacePart.h"
#include "NameDlg.h"

#include "Importer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPlacePart dialog


CDialogPlacePart::CDialogPlacePart(CWnd* pParent /*=NULL*/,DWORD dwType /*=0*/)
	: CDialog(CDialogPlacePart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPlacePart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int iData=IDD;
	m_dwType=dwType;
	m_bFlagNew=FALSE;//clear new flag
	m_bFlagHdlItemChanged=TRUE;//enables hadling of item changed event
	m_bFlagHdlItemChanged1=TRUE;//disables handling of item changed event for 1 time

	//Create part viewer
	if(iData=IDD_PLACEPART2){

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CPartPView );
		CObject* pObject = pRuntimeClass->CreateObject();
		ASSERT( pObject->IsKindOf( RUNTIME_CLASS( CPartPView ) ) );

		m_pPPView=(CPartPView *)pObject;
	}
}


void CDialogPlacePart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPlacePart)
	DDX_Control(pDX, IDC_LISTPART, m_lcPart);
	DDX_Control(pDX, IDC_COMBO1, m_cbLibrary);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPlacePart, CDialog)
	//{{AFX_MSG_MAP(CDialogPlacePart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTPART, OnItemchangedListpart)
	ON_CBN_SELENDOK(IDC_COMBO1, OnSelendokLibrary)
	ON_BN_CLICKED( IDC_BUTTON_NEW, OnButtonNew )
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LISTPART, OnEndlabeleditListpart)
	ON_BN_CLICKED( IDC_BUTTON_DEL, OnButtonDel )
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_NEWLIB, OnButtonNewlib)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_IMPLIB, &CDialogPlacePart::OnBnClickedButtonImplib)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPlacePart message handlers

BOOL CDialogPlacePart::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Extra initialization
#define DCABLE_PART_DB_DIR
#ifndef DCABLE_PART_DB_DIR

	WIN32_FIND_DATA FindFileData;
	CString sPath=_T("*.ddw");

	CDraftDrawApp *pApp=(CDraftDrawApp *)AfxGetApp();
	switch(pApp->GetActiveDocType()){
		case _DOCTYPE_RACK:
			sPath=_T(".\\Rack\\*.ddw");
			break;
		default:
			sPath=_T("*.ddw");
			break;

	}



	m_lcPart.InsertColumn(0,"Part",LVCFMT_LEFT,100,0);
	m_sPart="";


	HANDLE handle=::FindFirstFile(sPath,&FindFileData);

	if(!handle){

		return TRUE;
	}

	if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){

		m_lcPart.InsertItem(0,FindFileData.cFileName);

	}

	while(::FindNextFile(handle,&FindFileData)){

		if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){

			m_lcPart.InsertItem(0,FindFileData.cFileName);

		}
	}

	::FindClose(handle);

#else
	//Connect to database
	if(!g_db.IsOpen()){
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);

		//Connect to database
		g_db.OpenEx(sConnect);
	}

	//Load libraries first

	CRecordset rsLib(&g_db);

	rsLib.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");

	//Iterate throw libraries
	while(!rsLib.IsEOF()){
		CString str;
		rsLib.GetFieldValue("nNameLib",str);
		int nIndex=m_cbLibrary.AddString(str);
		rsLib.GetFieldValue("iIdLib",str);
		m_cbLibrary.SetItemData(nIndex,atoi(str));
		rsLib.MoveNext();
	}
	
	//Set default selection
	//This can be an user setting
	if(m_cbLibrary.FindString(-1,g_sCurrentLibrary)!=CB_ERR){
		m_cbLibrary.SetCurSel(m_cbLibrary.FindString(-1,g_sCurrentLibrary));
	}
	else{
		m_cbLibrary.SetCurSel(0);
	}
	m_iLibrary=m_cbLibrary.GetItemData(m_cbLibrary.GetCurSel());

	//Load parts then
	UpdatePartList();

#endif

	//Creates part preview window
	/*
	int iData=IDD;
	if(iData=IDD_PLACEPART2){

		m_pPPView->Create(NULL,"MyView",WS_CHILD|WS_VISIBLE|WS_BORDER,CRect(10,60,300,260),this,1000);
	}
	*/

	//Align part viewer with other controls
	CRect rect0,rect, rect1, rect2;
	m_pPPView->GetWindowRect(&rect0);
	m_cbLibrary.GetWindowRect(&rect);
	m_lcPart.GetWindowRect(&rect1);
	CWnd* pButton=GetDlgItem(IDC_BUTTON_NEWLIB);
	pButton->GetWindowRect(&rect2);
	ScreenToClient(&rect);
	ScreenToClient(&rect1);
	ScreenToClient(&rect0);
	m_pPPView->SetWindowPos(0,rect.left,rect1.top,rect2.left-rect.left,rect1.Height(),SWP_NOZORDER);
	
	//Enable buttons
	if(m_dwType==1){
		m_lcPart.ModifyStyle(LVS_SINGLESEL,LVS_EDITLABELS);
		GetDlgItem(IDOK)->SetWindowText("&Editar");
		GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_DEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_NEWLIB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_IMPLIB)->ShowWindow(SW_SHOW);
		SetWindowText("SIC Sistema de Información de Componentes");
		GetDlgItem(IDC_CHECK_ALL)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogPlacePart::UpdatePartList(){

	m_lcPart.DeleteAllItems();

	CRecordset rs(&g_db);

	rs.m_strFilter.Format("iIdLib = %i",m_iLibrary);
	rs.m_strSort="nNamePart DESC";

	if(m_dwType!=1 && SendDlgItemMessage(IDC_CHECK_ALL,BM_GETCHECK)!=BST_CHECKED){
		CDraftDrawApp *pApp=(CDraftDrawApp *)AfxGetApp();
		switch(pApp->GetActiveDocType()){
			case _DOCTYPE_RACK:
				rs.m_strFilter+="AND bRackType = 1";
				break;
			default:
				rs.m_strFilter+="AND bRackType = 0";
				break;

		}
	}

	rs.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	m_lcPart.DeleteAllItems();

	//beware: SetItemData used in UpdatePartList fires LVN_ITEMCHANGED
	m_bFlagHdlItemChanged=FALSE;
	while(!rs.IsEOF()){

		m_lcPart.InsertColumn(0,"Part",LVCFMT_LEFT,100,0);
		CString str;
		rs.GetFieldValue("nNamePart",str);
		m_lcPart.InsertItem(0,str);
		rs.GetFieldValue("iIdPart",str);
		m_lcPart.SetItemData(0,atoi(str));
		rs.MoveNext();
	}
	m_bFlagHdlItemChanged=TRUE;
}

void CDialogPlacePart::OnOK() 
{
	//Check if a part is selected
	POSITION pos = m_lcPart.GetFirstSelectedItemPosition();

	if(pos){

		int nItem = m_lcPart.GetNextSelectedItem(pos);

		CDraftDrawApp *pApp=(CDraftDrawApp *)AfxGetApp();
		switch(pApp->GetActiveDocType()){
			case _DOCTYPE_RACK:
				//23/11/2004
				//Commented because parts are now stored in database
				//m_sPart=_T(".\\Rack\\");
				m_sPart=_T("");
				break;
			default:
				m_sPart=_T("");
				break;

		}

		m_sPart+=m_lcPart.GetItemText(nItem,0);

		int nIndex=m_cbLibrary.GetCurSel();

		m_cbLibrary.GetLBText(nIndex,m_sLibrary);

		CDialog::OnOK();

		return;
	}

	
	CDialog::OnCancel();

}

void CDialogPlacePart::OnItemchangedListpart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//check if this message handling is dissabled
	if(!m_bFlagHdlItemChanged) return;

	//check if this message handling is dissabled only for 1 time
	if(!m_bFlagHdlItemChanged1){
		m_bFlagHdlItemChanged1=TRUE;
		return;
	}
	
	int iData=IDD;
	if(iData=IDD_PLACEPART2)
	if(pNMListView->uNewState==(LVIS_FOCUSED|LVIS_SELECTED)){

		CString str=m_lcPart.GetItemText(pNMListView->iItem,0);

		if(m_pPPView->m_pShape){
			delete(m_pPPView->m_pShape);
			m_pPPView->m_pShape=NULL;
		}

		m_pPPView->m_pShape=new CShapeUnit();

		int nIndex=m_cbLibrary.GetCurSel();
		m_cbLibrary.GetLBText(nIndex,m_sLibrary);

		m_pPPView->m_pShape->LoadUnit(m_sLibrary+"."+str);

		m_pPPView->m_pShape->m_Rect-=(m_pPPView->m_pShape->m_Rect.TopLeft()-CPoint(10,10));
		m_pPPView->m_pShape->m_RectLast-=(m_pPPView->m_pShape->m_Rect.TopLeft()-CPoint(10,10));

		m_pPPView->Invalidate();

		GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	}

	*pResult = 0;
}

void CDialogPlacePart::OnSelendokLibrary() 
{
	// Control notification handler
	int nIndex=m_cbLibrary.GetCurSel();

	m_iLibrary=m_cbLibrary.GetItemData(nIndex);

	//Store in a global variable current selected library
	CString str;
	m_cbLibrary.GetLBText(nIndex, str);
	g_sCurrentLibrary=str;

	//Connect to database
	if(!g_db.IsOpen()){
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);

		//Connect to database
		g_db.OpenEx(sConnect);
	}

	UpdatePartList();
}

void CDialogPlacePart::OnButtonNew( ){

	//This functionality is suitable for custom control.

	//Find next.
	LVFINDINFO lvInfo;
	lvInfo.flags=LVFI_PARTIAL|LVFI_WRAP;
	lvInfo.psz="Nuevo";
	int nIndex=m_lcPart.FindItem(&lvInfo);
	int nIndexLast=-1;//find memory
	int nOrder=0;//ordinal
	int nLenMax=0;//max length
	CString strPrefix="Nuevo";//new element prefix
	CString str;//aux function
	while(nIndex!=-1&&nIndex>nIndexLast){

		//get string
		str=m_lcPart.GetItemText(nIndex,0);

		//this will be substituted by a rebuild reg-expression
		int i=std::string(str).find_last_not_of("0123456789");
		if(i>nLenMax){
			nLenMax=i;
			strPrefix=str.Left(i+1);//hold prefix
			lvInfo.psz=strPrefix;
			nOrder=0;//restart ordinal value
		}

		//this will be substituted by find reg-expression
		if(atoi(str.Right(str.GetLength()-i-1))>nOrder&&i==nLenMax){
			nOrder=atoi(str.Right(str.GetLength()-i-1));
		}

		//find memory & find next
		nIndexLast=nIndex;
		nIndex=m_lcPart.FindItem(&lvInfo,nIndex);

	}//find loop end

	//Build new element
	str.Format("%03i",nOrder+1);
	strPrefix+=str;

	//Inser new element
	nIndex=m_lcPart.InsertItem(0,strPrefix);
	m_bFlagNew=TRUE;//set new flag
	m_lcPart.SetFocus();
	m_bFlagHdlItemChanged1=FALSE;
	m_lcPart.EditLabel(nIndex);
	
}

void CDialogPlacePart::OnEndlabeleditListpart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//Local variables
	CString str;
	CString strQuery;

	//Cast to right structure
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Process name changing.
	if(!m_bFlagNew){
		if(pDispInfo->item.mask&LVIF_TEXT){
			str.Format("%i",pDispInfo->item.lParam);
			strQuery.Format("UPDATE tbPart SET nNamePart='"+CString(pDispInfo->item.pszText)+"',bTextBin=1,iIdLib=%i WHERE iIdPart="+str,m_iLibrary); 
			g_db.ExecuteSQL(strQuery);
			m_lcPart.SetItemText(pDispInfo->item.iItem,0,pDispInfo->item.pszText);
		}
		return;
	}

	m_bFlagNew=FALSE;//clear new flag

	//Connect to database
	if(!g_db.IsOpen()){
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);

		//Connect to database
		g_db.OpenEx(sConnect);
	}

	//Insert new element
	//strQuery.Format("INSERT INTO tbPart (nNamePart,bTextBin,iIdLib) VALUES ('"+m_lcPart.GetItemText(pDispInfo->item.iItem,0)+"',1,%i)",m_iLibrary);
	strQuery.Format("INSERT INTO tbPartView SELECT * FROM tbPartView WHERE nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')",m_iLibrary);	
	g_db.ExecuteSQL(strQuery);

	CRecordset rs(&g_db);
	rs.m_strFilter.Format("nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')",m_iLibrary);	
	rs.m_strSort="iIdPart DESC";
	rs.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

	rs.GetFieldValue("iIdPart",str);
	rs.Close();

	strQuery.Format("UPDATE tbPart SET nNamePart='"+CString(pDispInfo->item.pszText)+"',bTextBin=1,iIdLib=%i WHERE iIdPart="+str,m_iLibrary); 
	g_db.ExecuteSQL(strQuery);


	//Reload unit
	//LoadUnit(CString(pDispInfo->item.pszText));

	//Update part list
	UpdatePartList();

	//Select new item
	LVFINDINFO lvFindInfo;
	lvFindInfo.flags=LVFI_PARAM ;
	lvFindInfo.lParam=atoi(str);
	int nIndex=m_lcPart.FindItem(&lvFindInfo);
	if(nIndex>0){
		LVITEM lvItem;
		lvItem.mask=LVIF_STATE;
		lvItem.iItem=nIndex;
		lvItem.iSubItem=0;
		lvItem.state=LVIS_FOCUSED|LVIS_SELECTED;
		lvItem.stateMask=LVIS_FOCUSED|LVIS_SELECTED;
		m_lcPart.SetItemState(nIndex,&lvItem);
	}
	
	*pResult = 0;
}

void CDialogPlacePart::OnButtonDel( ){

	CString str;
	CString strQuery;

	int nIndex=m_cbLibrary.GetCurSel();
	m_cbLibrary.GetLBText(nIndex,m_sLibrary);


	//Query user for deletion
	POSITION pos = m_lcPart.GetFirstSelectedItemPosition();
	if (pos == NULL){
	   return;
	}
	else
	{
		int nItem=m_lcPart.GetNextSelectedItem(pos);
	   if(pos)
	   {
		   if(AfxMessageBox(IDS_MSG_CONFDELN,MB_YESNO|MB_ICONQUESTION,-1)!=IDYES){
				return;
		   }
	   }
	   else{
			str.FormatMessage(IDS_MSG_CONFDEL1,m_lcPart.GetItemText(nItem,0));
		   if(AfxMessageBox(str,MB_YESNO|MB_ICONQUESTION,-1)!=IDYES){
				return;
		   }

	   }
	}


	//CDatabase db;
	TCHAR sConnect[1024];

	g_GetConnectString(sConnect,1024);


	//Connect to database

	if (!m_db.OpenEx(sConnect))
	/*if (!db.OpenEx("DRIVER=Microsoft Access Driver (*.mdb);DBQ=BinDB.MDB;FIL=MS Access for Microsoft Access;UID=Admin;PWD=;"))*/
	{
		AfxMessageBox("No se puede conectar con la base de datos.");
		return;
	}


	//LOOP
	pos = m_lcPart.GetFirstSelectedItemPosition();
	if (pos == NULL){
	   return;
	}
	else
	{
	   while(pos)
	   {
		   int nItem=m_lcPart.GetNextSelectedItem(pos);

		   strQuery.Format("DELETE FROM tbPart WHERE iIdPart=%i",m_lcPart.GetItemData(nItem));
			m_db.ExecuteSQL(strQuery);
	   }
	}

	//Update part list
	UpdatePartList();

	//Closes connections
	m_db.Close();
}

int CDialogPlacePart::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Specialized creation code
	int iData=IDD;
	if(iData=IDD_PLACEPART2){
		m_pPPView->Create(NULL,"MyView",WS_CHILD|WS_VISIBLE|WS_BORDER,
			CRect(10,60,300,260),this,1000);
	}
	
	return 0;
}

void CDialogPlacePart::LoadUnit(LPCTSTR lpszUnit /*=NULL*/){

	//Local variables
	CString str;

	//If parameter is passed, use the parameter
	if(lpszUnit){
		str=lpszUnit;
	}
	else{
		//If no parameter, get first list control selected item
		POSITION pos = m_lcPart.GetFirstSelectedItemPosition();
		if (pos == NULL){
			//Return null if not item selected
			return;
		}
		int nItem=m_lcPart.GetNextSelectedItem(pos);
		str=m_lcPart.GetItemText(nItem,0);
	}

	if(m_pPPView->m_pShape){
		delete(m_pPPView->m_pShape);
		m_pPPView->m_pShape=NULL;
	}

	m_pPPView->m_pShape=new CShapeUnit();

	int nIndex=m_cbLibrary.GetCurSel();
	m_cbLibrary.GetLBText(nIndex,m_sLibrary);

	m_pPPView->m_pShape->LoadUnit(m_sLibrary+"."+str);

	m_pPPView->m_pShape->m_Rect-=(m_pPPView->m_pShape->m_Rect.TopLeft()-CPoint(10,10));

	m_pPPView->Invalidate();
}
void CDialogPlacePart::OnButtonNewlib() 
{
	// TODO: Add your control notification handler code here
	CNameDlg dlg;
	if(dlg.DoModal()==IDOK){

		if(!m_db.IsOpen()){
			TCHAR sConnect[1024];
			g_GetConnectString(sConnect,1024);
			//Connect to database
			m_db.OpenEx(sConnect);
		}

		m_db.ExecuteSQL("INSERT INTO tbLibrary (nNameLib) VALUES ('"+dlg.m_Name+"')");

		m_cbLibrary.ResetContent();

		CRecordset rsLib(&m_db);

		rsLib.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");

		CString strName;
		while(!rsLib.IsEOF()){
			CString str;
			rsLib.GetFieldValue("nNameLib",strName);
			int nIndex=m_cbLibrary.AddString(strName);
			rsLib.GetFieldValue("iIdLib",str);
			m_cbLibrary.SetItemData(nIndex,atoi(str));
			rsLib.MoveNext();
		}

		int nIndex=m_cbLibrary.FindString(-1,strName);
		m_cbLibrary.SetCurSel(nIndex);
		m_iLibrary=m_cbLibrary.GetItemData(nIndex);

		rsLib.Close();

		UpdatePartList();

		m_db.Close();
	}
}

void CDialogPlacePart::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	//CDatabase db;
	TCHAR sConnect[1024];

	g_GetConnectString(sConnect,1024);

	//Connect to database

	if (!m_db.OpenEx(sConnect))
	/*if (!db.OpenEx("DRIVER=Microsoft Access Driver (*.mdb);DBQ=BinDB.MDB;FIL=MS Access for Microsoft Access;UID=Admin;PWD=;"))*/
	{
		AfxMessageBox("No se puede conectar con la base de datos.");
		return;
	}

	UpdatePartList();

	m_db.Close();
}

void CDialogPlacePart::DoInsertNewLibrary(LPCTSTR lpszLibName, BOOL bCheckExists)
{
	if (!m_db.IsOpen()){
		TCHAR sConnect[1024];
		g_GetConnectString(sConnect, 1024);
		//Connect to database
		m_db.OpenEx(sConnect);
	}

	//Check if library exists
	if (bCheckExists){
		CRecordset rsLibExists(&m_db);
		CString strQuery;
		strQuery.Format("SELECT * FROM tbLibrary WHERE nNameLib = '%s' ORDER BY iIdLib DESC", lpszLibName);
		rsLibExists.Open(CRecordset::forwardOnly, strQuery);
		if (!rsLibExists.IsEOF())
		{
			CString str;
			rsLibExists.GetFieldValue("iIdLib", str);
			return;
		}
	}

	m_db.ExecuteSQL("INSERT INTO tbLibrary (nNameLib) VALUES ('" + CString(lpszLibName) + "')");

	m_cbLibrary.ResetContent();

	CRecordset rsLib(&m_db);

	rsLib.Open(CRecordset::forwardOnly, "SELECT * FROM tbLibrary");

	CString strName;
	while (!rsLib.IsEOF()){
		CString str;
		rsLib.GetFieldValue("nNameLib", strName);
		int nIndex = m_cbLibrary.AddString(strName);
		rsLib.GetFieldValue("iIdLib", str);
		m_cbLibrary.SetItemData(nIndex, atoi(str));
		rsLib.MoveNext();
	}

	int nIndex = m_cbLibrary.FindString(-1, strName);
	m_cbLibrary.SetCurSel(nIndex);
	m_iLibrary = m_cbLibrary.GetItemData(nIndex);

	rsLib.Close();

	UpdatePartList();

	m_db.Close();
}

void CDialogPlacePart::DoInsertPart(LPCTSTR lpszPartName)
{
	//Local variables
	CString str;
	CString strQuery;

	//Connect to database
	if (!g_db.IsOpen()){
		TCHAR sConnect[1024];

		g_GetConnectString(sConnect, 1024);

		//Connect to database
		g_db.OpenEx(sConnect);
	}

	//Insert new element
	//strQuery.Format("INSERT INTO tbPart (nNamePart,bTextBin,iIdLib) VALUES ('"+m_lcPart.GetItemText(pDispInfo->item.iItem,0)+"',1,%i)",m_iLibrary);
	strQuery.Format("INSERT INTO tbPartView SELECT * FROM tbPartView WHERE nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')", m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	CRecordset rs(&g_db);
	rs.m_strFilter.Format("nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')", m_iLibrary);
	rs.m_strSort = "iIdPart DESC";
	rs.Open(CRecordset::forwardOnly, "SELECT * FROM tbPart");

	rs.GetFieldValue("iIdPart", str);
	rs.Close();

	strQuery.Format("UPDATE tbPart SET nNamePart='" + CString(lpszPartName) + "',bTextBin=1,iIdLib=%i WHERE iIdPart=" + str, m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	//Closes connections
	m_db.Close();
}

void CDialogPlacePart::DoImportLibrary()
{
	std::string sFilter = "External library file (*.xml)|*.xml||"; //All Files (*.*)|*.*||;
	CStringArray saExtensions;
	saExtensions.Add("xml");


	CFileDialog fdialog(
		TRUE,		//save dialog box
		NULL,
		NULL,
		0,			//no flags
		sFilter.c_str(),
		NULL);

	if (fdialog.DoModal() == IDOK){

		//Get filename from dialog
		int offset = fdialog.m_ofn.nFileExtension;
		CString strFile = fdialog.m_ofn.lpstrFile;
		int nFilterIndex = fdialog.m_ofn.nFilterIndex;
		if ((!offset) || (fdialog.m_ofn.lpstrFile[offset] == 0)){

			strFile += "." + saExtensions[fdialog.m_ofn.nFilterIndex - 1];
			sFilter = saExtensions[fdialog.m_ofn.nFilterIndex - 1];
		}

		//Try to import library
		CImporter *pImporter = new CImporter();
		CObArray *pObArray = new CObArray();
		pImporter->ImportLibrary(strFile, pObArray);

		//Create new library
		CString strLib = strFile.Left(strFile.Find('.'));
		if (strLib.GetLength() == 0) strLib = strFile;
		DoInsertNewLibrary(strLib, TRUE);

		//Insert/update parts
		for (int i = 0; i<pObArray->GetSize(); i++){

			//Insert part
			CShapeUnit *pSh = (CShapeUnit *)pObArray->GetAt(i);
pSh->m_sUnitName = "Resistor";
			DoInsertPart(pSh->m_sUnitName);

			//Save parts
			//Create new method in CDocument to be called from here and serialize the parts
			CDraftDrawDoc *pDoc = theApp.GetActiveDocument();
			CString strFile = pSh->m_sUnitName;
			//Keep previous part edition status
			BOOL bFlagPartEdit = pDoc->m_bFlagPartEdit;
			//Set part edition mode to save the save
			pDoc->m_bFlagPartEdit = TRUE;
			CObArray* prevObArray = pDoc->m_pObArray;
			pDoc->m_pObArray = &pSh->m_obarrShapearr;
			pDoc->OnDatabaseSave(strLib + "." + strFile);
			pDoc->m_pObArray = prevObArray;
			//Restore flag
			pDoc->m_bFlagPartEdit = bFlagPartEdit;
		}

	}//End dialog result OK
}


void CDialogPlacePart::OnBnClickedButtonImplib()
{
	//Do import library
	DoImportLibrary();
}
