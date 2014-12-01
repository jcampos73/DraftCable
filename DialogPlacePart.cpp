// DialogPlacePart.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPlacePart.h"
#include "NameDlg.h"

#include "Importer.h"
#include "DraftDrawView.h"

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
	m_bFlagNew=FALSE;				//clear new flag
	m_bFlagHdlItemChanged=TRUE;		//enables hadling of item changed event
	m_bFlagHdlItemChanged1=TRUE;	//disables handling of item changed event for 1 time
	m_bDragging = FALSE;			//for dragging operations

	//Create part viewer
	if(iData=IDD_PLACEPART2){

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CPartPView );
		CObject* pObject = pRuntimeClass->CreateObject();
		ASSERT( pObject->IsKindOf( RUNTIME_CLASS( CPartPView ) ) );

		m_pPPView=(CPartPView *)pObject;
		m_pPPView->SetPlacePartDlg(this);
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
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDialogPlacePart::OnItemdblclickListpart)
ON_NOTIFY(NM_DBLCLK, IDC_LISTPART, &CDialogPlacePart::OnDblclkListpart)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTPART, &CDialogPlacePart::OnBegindragListpart)
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
		rsLib.GetFieldValue(_T("nNameLib"), str);
		int nIndex=m_cbLibrary.AddString(str);
		rsLib.GetFieldValue(_T("iIdLib"), str);
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
	CRect rect0, rect, rect1, rect2;
	m_pPPView->GetWindowRect(&rect0);
	m_cbLibrary.GetWindowRect(&rect);
	m_lcPart.GetWindowRect(&rect1);
	CWnd* pButton=GetDlgItem(IDC_BUTTON_NEWLIB);
	pButton->GetWindowRect(&rect2);
	ScreenToClient(&rect0);
	ScreenToClient(&rect);
	ScreenToClient(&rect1);
	ScreenToClient(&rect2);
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
		rs.GetFieldValue(_T("nNamePart"), str);
		m_lcPart.InsertItem(0,str);
		rs.GetFieldValue(_T("iIdPart"), str);
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

#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
		DoGetLibraryAndPart();
		CDraftDrawDoc *pDoc = theApp.GetActiveDocument();
		pDoc->SetShapeToPlace(m_sLibrary, m_sPart);
#endif

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
		int i = std::string(str).find_last_not_of(_T("0123456789"));
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
	strQuery.Format(_T("INSERT INTO tbPartView SELECT * FROM tbPartView WHERE nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')"), m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	CRecordset rs(&g_db);
	rs.m_strFilter.Format(_T("nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')"), m_iLibrary);
	rs.m_strSort = _T("iIdPart DESC");
	rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM tbPart"));

	rs.GetFieldValue(_T("iIdPart"), str);
	rs.Close();

	strQuery.Format(_T("UPDATE tbPart SET nNamePart='") + CString(pDispInfo->item.pszText) + _T("',bTextBin=1,iIdLib=%i WHERE iIdPart=") + str, m_iLibrary);
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
	/*if (!db.OpenEx(_T("DRIVER=Microsoft Access Driver (*.mdb);DBQ=BinDB.MDB;FIL=MS Access for Microsoft Access;UID=Admin;PWD=;")))*/
	{
		AfxMessageBox(_T("No se puede conectar con la base de datos."));
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

		m_db.ExecuteSQL(_T("INSERT INTO tbLibrary (nNameLib) VALUES ('") + dlg.m_Name + _T("')"));

		m_cbLibrary.ResetContent();

		CRecordset rsLib(&m_db);

		rsLib.Open(CRecordset::forwardOnly, _T("SELECT * FROM tbLibrary"));

		CString strName;
		while(!rsLib.IsEOF()){
			CString str;
			rsLib.GetFieldValue(_T("nNameLib"), strName);
			int nIndex=m_cbLibrary.AddString(strName);
			rsLib.GetFieldValue(_T("iIdLib"), str);
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
	/*if (!db.OpenEx(_T("DRIVER=Microsoft Access Driver (*.mdb);DBQ=BinDB.MDB;FIL=MS Access for Microsoft Access;UID=Admin;PWD=;")))*/
	{
		AfxMessageBox(_T("No se puede conectar con la base de datos."));
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
		strQuery.Format(_T("SELECT * FROM tbLibrary WHERE nNameLib = '%s' ORDER BY iIdLib DESC"), lpszLibName);
		rsLibExists.Open(CRecordset::forwardOnly, strQuery);
		if (!rsLibExists.IsEOF())
		{
			CString str;
			rsLibExists.GetFieldValue(_T("iIdLib"), str);
			return;
		}
	}

	m_db.ExecuteSQL(_T("INSERT INTO tbLibrary (nNameLib) VALUES ('") + CString(lpszLibName) + _T("')"));

	m_cbLibrary.ResetContent();

	CRecordset rsLib(&m_db);

	rsLib.Open(CRecordset::forwardOnly, _T("SELECT * FROM tbLibrary"));

	CString strName;
	while (!rsLib.IsEOF()){
		CString str;
		rsLib.GetFieldValue(_T("nNameLib"), strName);
		int nIndex = m_cbLibrary.AddString(strName);
		rsLib.GetFieldValue(_T("iIdLib"), str);
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

	//Delete existing shape
	strQuery.Format(_T("DELETE FROM tbPart WHERE nNamePart LIKE '") + CString(lpszPartName) + _T("' AND iIdLib=%i"), m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	//Insert new element
	//strQuery.Format("INSERT INTO tbPart (nNamePart,bTextBin,iIdLib) VALUES ('"+m_lcPart.GetItemText(pDispInfo->item.iItem,0)+"',1,%i)",m_iLibrary);
	strQuery.Format(_T("INSERT INTO tbPartView SELECT * FROM tbPartView WHERE nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')"), m_iLibrary);
	//strQuery.Format("INSERT INTO tbPart (nNamePart,bTextBin,iIdLib) SELECT nNamePart,bTextBin,iIdLib FROM tbPartView WHERE nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')", m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	CRecordset rs(&g_db);
	rs.m_strFilter.Format(_T("nNamePart='blank' AND iIdLib IN (SELECT iIdLib FROM tbLibrary WHERE nNameLib='Plantillas')"), m_iLibrary);
	rs.m_strSort = _T("iIdPart DESC");
	rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM tbPart"));

	rs.GetFieldValue(_T("iIdPart"), str);
	rs.Close();

	strQuery.Format(_T("UPDATE tbPart SET nNamePart='") + CString(lpszPartName) + _T("',bTextBin=1,iIdLib=%i WHERE iIdPart=") + str, m_iLibrary);
	g_db.ExecuteSQL(strQuery);

	//Closes connections
	g_db.Close();
}

void CDialogPlacePart::DoImportLibrary()
{
	CDraftDrawDoc *pDoc = theApp.GetActiveDocument();

	std::string sFilter = _T("External library file (*.xml)|*.xml||"); //All Files (*.*)|*.*||;
	CStringArray saExtensions;
	saExtensions.Add(_T("xml"));

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
		pImporter->ImportLibrary(strFile, pObArray, pDoc->m_szGrid);

		//Create new library
		CString strLib = strFile.Left(strFile.ReverseFind('.'));
		if (strLib.ReverseFind(_T('\\')) >= 0){
			strLib = strLib.Right(strLib.GetLength() - strLib.ReverseFind(_T('\\')) - 1);
		}
		if (strLib.GetLength() == 0) strLib = strFile;
		DoInsertNewLibrary(strLib, TRUE);

		//Iterate all shapes to bounding rect union
		CRect rectUnion;
		for (int i = 0; i < pObArray->GetSize(); i++){
			CShapeUnit *pSh = (CShapeUnit *)pObArray->GetAt(i);
			CRect rect;
			pSh->GetRectTemp(rect);
			//rectUnion.UnionRect(rectUnion, rect);
			if (rect.Width()>rectUnion.Width()){
				rectUnion = CRect(CPoint(0, 0), CSize(rect.Width(), rectUnion.Height()));
			}
			if (rect.Height()>rectUnion.Height()){
				rectUnion = CRect(CPoint(0, 0), CSize(rectUnion.Width(), rect.Height()));
			}
		}

		rectUnion.InflateRect(DCABLE_PADDINGX_DEFAULT << 1, DCABLE_PADDINGY_DEFAULT << 1);
		CSize size=rectUnion.Size();
		rectUnion = CRect(CPoint(0, 0), rectUnion.Size());

		//Insert/update parts
		for (int i = 0; i<pObArray->GetSize(); i++){

			//Insert part
			CShapeUnit *pSh = (CShapeUnit *)pObArray->GetAt(i);

			//Do insert in librarry
			DoInsertPart(pSh->m_sUnitName);

			//Get bounding rect
			CRect rectTemp;
			pSh->GetRectTemp(rectTemp);
			((CShapeFrmRect*)pSh->m_obarrShapearr[0])->m_Rect = rectUnion;
			CPoint offset = CPoint((rectUnion.Width() - rectTemp.Width()) >> 1, (rectUnion.Height() - rectTemp.Height()) >> 1);

			//Call a method to normalize ShapeUnit from bounding rect TopLeft to (0,0) or any other offset
			//pSh->NormalizeChildShapes(CPoint(DCABLE_PADDINGX_DEFAULT, DCABLE_PADDINGY_DEFAULT));
			pSh->NormalizeChildShapes(offset);

			//Save parts
			//Create new method in CDocument to be called from here and serialize the parts
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

void CDialogPlacePart::OnDblclkListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
	DoGetLibraryAndPart();
	CDraftDrawDoc *pDoc = theApp.GetActiveDocument();
	pDoc->SetShapeToPlace(m_sLibrary, m_sPart);
#endif

	*pResult = 0;
}


void CDialogPlacePart::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
	//While the mouse is moving, this routine is called.
	//This routine will redraw the drag image at the present
	// mouse location to display the dragging.
	//Also, while over a CListCtrl, this routine will highlight
	// the item we are hovering over.

	//// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)
	{
		//// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint(pt);
		ASSERT(pDropWnd); //make sure we have a window

		//// If we drag outside current window we need to adjust the highlights displayed
		BOOL bDiasableLstUdate_ = TRUE;
		if (bDiasableLstUdate_ == FALSE && pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY(pList->SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY(pList->RedrawItems(m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for (i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if (pDropWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)))
		{
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			if (bDiasableLstUdate_ == FALSE){
				// Turn off hilight for previous drop target
				pList->SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);
				// Redraw previous item
				pList->RedrawItems(m_nDropIndex, m_nDropIndex);

				// Get the item that is below cursor
				m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
				// Highlight it
				pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
				// Redraw item
				pList->RedrawItems(m_nDropIndex, m_nDropIndex);
				pList->UpdateWindow();
			}
		}
		else if (pDropWnd->IsKindOf(RUNTIME_CLASS(CPartPView)))
		{
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		else if (pDropWnd->IsKindOf(RUNTIME_CLASS(CDraftDrawView)))
		{
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}

	CDialog::OnMouseMove(nFlags, point);
#endif
}


void CDialogPlacePart::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
	//This routine is the end of the drag/drop operation.
	//When the button is released, we are to drop the item.
	//There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	//If we are in a drag and drop operation (otherwise we don't do anything)
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

		CPoint pt(point); //Get current mouse coordinates
		ClientToScreen(&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint(pt);
		ASSERT(pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf(RUNTIME_CLASS(CDraftDrawView)))
		{
			/*
			m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on
			DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
			*/
			DoGetLibraryAndPart();
			CDraftDrawDoc* pDoc = ((CDraftDrawView*)pDropWnd)->GetDocument();
			pDoc->SetShapeToPlace(m_sLibrary, m_sPart);
		}
	}
#endif
	CDialog::OnLButtonUp(nFlags, point);
}

int CDialogPlacePart::DoGetLibraryAndPart()
{
	POSITION pos = m_lcPart.GetFirstSelectedItemPosition();
	int nIndex = -1;
	if (pos){
		int nItem = m_lcPart.GetNextSelectedItem(pos);
		m_sPart = m_lcPart.GetItemText(nItem, 0);
		nIndex = m_cbLibrary.GetCurSel();
		m_cbLibrary.GetLBText(nIndex, m_sLibrary);
	}
	return nIndex;
}

void CDialogPlacePart::OnBegindragListpart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

#ifdef DCABLE_PLACEPART_DIALOG_NOT_MODAL
	//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well
	// as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//// Save the index of the item being dragged in m_nDragIndex
	////  This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	//// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image
	pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = m_lcPart.CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage); //make sure it was created
	//We will call delete later (in LButtonUp) to clean this up

	CBitmap bitmap;
	if (m_lcPart.GetSelectedCount() > 1) //more than 1 item in list is selected
		//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
		//bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);
		bitmap.LoadBitmap(IDB_BITMAP_BOX);
	else
		bitmap.LoadBitmap(IDB_BITMAP_BOX);

	m_pDragImage->Replace(0, &bitmap, &bitmap);

	//// Change the cursor to the drag image
	////	(still must perform DragMove() in OnMouseMove() to show it moving)
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

	//// Set dragging flag and others
	m_bDragging = TRUE;	//we are in a drag and drop operation
	m_nDropIndex = -1;	//we don't have a drop index yet
	m_pDragList = &m_lcPart; //make note of which list we are dragging from
	m_pDropWnd = &m_lcPart;	//at present the drag list is the drop list

	//// Capture all mouse messages
	SetCapture();
#endif

	*pResult = 0;
}
