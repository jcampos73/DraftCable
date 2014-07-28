// CableEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "Globals.h"
/*#include "PruDialogCableEdit.h"*/
#include "CableEdit.h"

//Dialog
#include "DialogSaveas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCableEdit dialog


CCableEdit::CCableEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CCableEdit::IDD, pParent)
{
	m_bModified=FALSE;
	//{{AFX_DATA_INIT(CCableEdit)
	m_strFile = _T("");
	//}}AFX_DATA_INIT
}
CCableEdit::CCableEdit(LPCSTR lpcsCableFile, BOOL bConnector /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CCableEdit::IDD, pParent)
{
	m_bModified=FALSE;
	m_strFile=lpcsCableFile;
	m_bConnector=bConnector;
}

void CCableEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCableEdit)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Control(pDX, IDC_FILE, m_cbFile);
	DDX_Control(pDX, IDC_LIST1, m_lcCable);
	//DDX_Text(pDX, IDC_EDIT1, m_strFile);
	DDV_MaxChars(pDX, m_strFile, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCableEdit, CDialog)
	//{{AFX_MSG_MAP(CCableEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_FILE, OnSelendokFile)
	ON_CBN_EDITCHANGE(IDC_FILE, OnEditchangeFile)
	//}}AFX_MSG_MAP
	ON_WM_CHAR( )
	//ON_COMMAND_EX(ID_NEXT_FIELD,  OnNextPrevField)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCableEdit message handlers
BOOL CCableEdit::OnNextPrevField(UINT nCmdID){

	//not mapped

	return FALSE;
}

BOOL CCableEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_FlagEditing=false;

	// TODO: Add extra initialization here

	//Configures combo box control
	//-----------------------------------------------------------------
if(!m_bConnector){
	int nCount=AfxGetCableCount();
	LPTSTR *listCable=(LPTSTR *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nCount*sizeof(CString*));
	int i;
	for(i=0;i<nCount;i++){
		listCable[i]=new char[256];
	}		
	AfxGetCables(listCable,nCount,255);
	for(i=0;i<nCount;i++){
		if(listCable[i]){
			int nIndex=m_cbFile.AddString(listCable[i]);
			m_cbFile.SetItemData(nIndex,i);
			delete(listCable[i]);
		}
	}		
	if(!m_strFile.IsEmpty()){
		m_cbFile.SelectString(-1,m_strFile);
	}
	else{
		m_cbFile.SetCurSel( 0 );
	}
}
else{
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText("Conn. Number");
	int nCount=AfxGetConnectorCount();
	LPTSTR *listCable=(LPTSTR *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nCount*sizeof(CString*));
	int i;
	for(i=0;i<nCount;i++){
		listCable[i]=new char[256];
	}		
	AfxGetConnector(listCable,nCount,255);
	for(i=0;i<nCount;i++){
		if(listCable[i]){
			int nIndex=m_cbFile.AddString(listCable[i]);
			m_cbFile.SetItemData(nIndex,i);
			delete(listCable[i]);
		}
	}		
	if(!m_strFile.IsEmpty()){
		m_cbFile.SelectString(-1,m_strFile);
	}
	else{
		m_cbFile.SetCurSel( 0 );
	}
}
	//-----------------------------------------------------------------

	//Configures list control
	//-----------------------------------------------------------------
#define _COLDEFWIDTH_CABLEDIT		100
if(!m_bConnector){
	m_iColumn[0]=_COLDEFWIDTH_CABLEDIT;
	m_iColumn[1]=_COLDEFWIDTH_CABLEDIT;
	m_iColumn[2]=_COLDEFWIDTH_CABLEDIT;
	m_iColumn[3]=_COLDEFWIDTH_CABLEDIT;

	m_lcCable.InsertColumn(0,"PIN",LVCFMT_LEFT,m_iColumn[0],0);
	m_lcCable.InsertColumn(1,"REF.HILO",LVCFMT_LEFT,m_iColumn[1],1);
	m_lcCable.InsertColumn(2,"FUNCTION",LVCFMT_LEFT,m_iColumn[2],2);
	m_lcCable.InsertColumn(3,"PIN",LVCFMT_LEFT,m_iColumn[3],3);
}
else{
	m_iColumn[0]=_COLDEFWIDTH_CABLEDIT;
	m_iColumn[1]=_COLDEFWIDTH_CABLEDIT;

	m_lcCable.InsertColumn(0,"PIN",LVCFMT_LEFT,m_iColumn[0],0);
	m_lcCable.InsertColumn(1,"REF.HILO",LVCFMT_LEFT,m_iColumn[1],1);
}
	DWORD dwStyle = m_lcCable.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	/*m_lcUser.ModifyStyle(LVS_ICON, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);*/
	m_lcCable.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	//-----------------------------------------------------------------

	//Load cable file
	//-----------------------------------------------------------------
	LoadCable();
	//-----------------------------------------------------------------

	//load accelerator table
	//-----------------------------------------------------------------
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),
						   MAKEINTRESOURCE(IDR_MAINFRAME/*IDR_ACCELERATOR1*/));
	ASSERT(m_hAccel);
	//-----------------------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCableEdit::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	/*
	CEdit* pedit = m_lcCable.EditLabel(1);
	
	CRect rect;

	pedit->GetRect(&rect);

	rect.InflateRect(100,0);

	pedit->SetRect(&rect);//doesn't work
	*/

	if(!m_FlagEditing){


		//Construct edit control
		//-----------------------------------------------------------------

		//Check for empty registers
		//=================================================================
		TrimLast();
		//=================================================================

		int index=m_lcCable.GetSelectionMark();
		if(index<0){
			index=m_lcCable.GetItemCount();
			m_lcCable.InsertItem(index,"");
		}


		m_iIndexEd=index;

		m_peWire=new CEdit();
		CEdit* pedit = m_peWire;

		CRect rect;
		m_lcCable.GetItemRect(index,&rect,LVIR_BOUNDS);
		//-----------------------------------------------------------------

		//Flag
		m_FlagEditing=true;

	
#define _WIREEDIT_MULTILINE
		//-----------------------------------------------------------------
#ifdef _WIREEDIT_MULTILINE
		//Creates edit control
		rect.InflateRect(0,10);
		pedit->Create(ES_AUTOHSCROLL|    ES_MULTILINE|ES_LEFT|WS_BORDER|WS_CHILD,rect,&m_lcCable,1);
//debug
/*
UINT udata=pedit->GetLimitText( ) ;
pedit->SetLimitText(256) ;
DWORD dwdata=pedit->GetMargins( );
*/
		LONG lData=GetDialogBaseUnits();
		float scale=m_iColumn[0]/ LOWORD(lData)*4;

		//Prepare tab stops
		int TabStops [_COLNUMBER_CABLEDIT];

#define OFFSET1_CABLEDIT		1	//1st column offset
#define OFFSET_CABLEDIT			2	//Remain column offset
#define OFFSETR_CABLEDIT		2	//acumulative offset

		TabStops[0]=OFFSET1_CABLEDIT;
		for(int i=1;i<_COLNUMBER_CABLEDIT;i++){

			TabStops[i]=i*scale+i*OFFSETR_CABLEDIT+OFFSET_CABLEDIT;
		}

		//pedit->SetTabStops(scale);
		pedit->SetTabStops(_COLNUMBER_CABLEDIT,TabStops);
#else
		pedit->Create(ES_LEFT|WS_BORDER|WS_CHILD,rect,&m_lcCable,1);
		//pedit->SetTabStops( );//error, assertion failed!
#endif
		//-----------------------------------------------------------------

		//Prepares string for displaying
		//Chosen separator: TAB
		//-----------------------------------------------------------------

		CString strtext="\t";
		/*CString */strtext+=m_lcCable.GetItemText(index,0);
		strtext+="\t";//";";
		strtext+=m_lcCable.GetItemText(index,1);
		strtext+="\t";//";";
		strtext+=m_lcCable.GetItemText(index,2);
		strtext+="\t";//";";
		strtext+=m_lcCable.GetItemText(index,3);

		pedit->SetWindowText(strtext);
		//-----------------------------------------------------------------

		//Show window & set focus
		//-----------------------------------------------------------------
		pedit->ShowWindow(SW_SHOW);
		pedit->SetFocus();
		//-----------------------------------------------------------------


	}

	*pResult = 0;
}


BOOL CCableEdit::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CString strtext;

	DWORD pos;
	CString str;

	switch(LOWORD(wParam)){

	//Processes TAB messages when row edition is active
	case ID_NEXT_FIELD:
		
		if(m_FlagEditing){
			/*//beware produces a loop!
			m_peWire->PostMessage(m_MsgLast.message,
				m_MsgLast.wParam,
				m_MsgLast.lParam);
			*/
			pos=m_peWire->GetSel();
			m_peWire->GetWindowText(str);
			str.Insert(LOWORD(pos),'\t');
			m_peWire->SetWindowText(str);

			pos=LOWORD(pos)+1;;
			pos=MAKELONG(pos,pos);
			m_peWire->SetSel(pos);
			return TRUE;
		}
		
		break;

	//Processes CANCEL, ESCAPE when row edition is active
	case IDCANCEL:
	case IDCLOSE:
	case ID_CANCEL:
	case ID_ESCAPE:

		if(m_FlagEditing){

			m_peWire->DestroyWindow();

			delete(m_peWire);

			m_FlagEditing=false;
			return TRUE;
		}
		break;

	//Processes OK when row edition is active
	case IDOK:
	case ID_OK:
		if(m_FlagEditing){

			m_peWire->GetWindowText(strtext);

			int index=strtext.Find(';');
			int index1=strtext.Find('\t');
			if((index==0)||(index1==0)){
				strtext=strtext.Right(strtext.GetLength()-1);
			}

			m_lcCable.SetItemText(m_iIndexEd,0,CutString(strtext));
			m_lcCable.SetItemText(m_iIndexEd,1,CutString(strtext));
			m_lcCable.SetItemText(m_iIndexEd,2,CutString(strtext));
			m_lcCable.SetItemText(m_iIndexEd,3,CutString(strtext));

			m_peWire->DestroyWindow();

			delete(m_peWire);

			//Check for empty registers
			//=================================================================
			TrimLast();
			//=================================================================


			m_FlagEditing=false;
			m_bModified=TRUE;
			if(!m_bNew)
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return TRUE;
		}
		break;

	case IDC_BUTTON_NEW:
		m_cbFile.SetWindowText("");
		m_lcCable.DeleteAllItems();
		m_bNew=TRUE;
		break;

	case IDC_BUTTON_SAVE:
		SaveCable();
		break;

	case IDC_BUTTON_SAVEAS:
		SaveCable(TRUE);
		break;

	case IDC_BUTTON_EDIT:
		LRESULT Result;
		OnDblclkList1(NULL,&Result);
		break;

	//Paste in edit control when active
	case ID_EDIT_PASTE:
		if(m_FlagEditing){
			m_peWire->Paste();
		}
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

CString CCableEdit::CutString(CString &strtext)
{
	//Cut a rows into separate string in iteration manner
	//ALLOWED SEPARATORS: ';','TAB','SPACE'

	//FINDS FIRST SEPARATOR
	int index=strtext.Find(';');
	int index1=strtext.Find('\t');

	if((index1>=0)&&(index1<index)||(index<0)) index=index1;

#define DCABLE_SPCSEPAR_DISABLE
#ifndef DCABLE_SPCSEPAR_DISABLE
	index1=strtext.Find(' ');
	if((index1>=0)&&(index1<index)||(index<0)) index=index1;
#endif

	//DELETES REPEATED SEPARATORS
#define DCABLE_REPEATSEPAR_DISABLE
#ifndef DCABLE_REPEATSEPAR_DISABLE
	while((index<strtext.GetLength()-1)&&
		((strtext[index+1]==';')
		||(strtext[index+1]=='\t')
#ifndef DCABLE_SPCSEPAR_DISABLE
		||(strtext[index+1]==' ')
#endif
		)){

		strtext.Delete(index,1);
	}
#endif

	//CUT STRING
	if(index==-1){
		//simple row
		return strtext;
		strtext="";
	}
#ifndef DCABLE_REPEATSEPAR_DISABLE
	else if(index==0){
		//deletes initial separators
		strtext=strtext.Right(strtext.GetLength()-index-1);
		return CutString(strtext);
	}
#endif
	else{
		//cut left most string of a row
		CString strfield=strtext.Left(index);
		strtext=strtext.Right(strtext.GetLength()-index-1);
		return strfield;
	}

}


BOOL CCableEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Used by class CWinApp to translate window messages before they are dispatched
	//to theTranslateMessage andDispatchMessage Windows functions.


	if (pMsg->message >= WM_KEYFIRST && 
		pMsg->message <= WM_KEYLAST) {

		//Translate using accelerator table

		ASSERT(m_hAccel);

		//Only translate when row editor is active
		if(m_FlagEditing){

			int result;

			//Analyze msg for debug
			switch(pMsg->message){

			case WM_KEYDOWN:

			 m_MsgLast.message=pMsg->message;
			 m_MsgLast.wParam=pMsg->wParam;
			 m_MsgLast.lParam=pMsg->lParam;
			 break;

			}

			//Translate WM_KEYDOWN or WM_SYSKEYDOWN to appropiate WM_COMMAND or WM_SYSCOMMAND
			//and send command to appropiate window procedure.

			result=::TranslateAccelerator(m_hWnd, m_hAccel, 
										   pMsg);



			return result;

		}
	}
  	
	return CDialog::PreTranslateMessage(pMsg);
}

void CCableEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//this funtion was created to give subitem slect capabilities to
	//list. Must be included in a CListCtrl derived class!

	/*
	CRect rect;
	m_lcCable.GetClientRect(&rect);

	LONG lData=GetDialogBaseUnits();


	int scale_y=HIWORD(lData);

	int index=point.y/scale_y;
	*/

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCableEdit::OnSelendokFile() 
{
	// TODO: Add your control notification handler code here
	if(m_bModified){
		if(::AfxMessageBox("¿Desea guardar los cambios realizados?",MB_OKCANCEL|MB_ICONEXCLAMATION,-1)==IDOK){
			SaveCable(m_strFileLast);
		}
		else{
			CString str=m_strFileLast;
			int	nIndex=str.ReverseFind('\\');
			if(nIndex>=0){
				str=str.Right(str.GetLength()-nIndex-1);
			}
			int nItem=m_cbFile.FindStringExact(-1,str);
			if(nItem!=CB_ERR){
				m_cbFile.SetCurSel(nItem);
			}
			return;
		}
	}

	UpdateData(TRUE);

	LoadCable();
}

void CCableEdit::LoadCable()
{
	//m_cbFile.GetWindowText(m_strFile);
	int nIndex=m_cbFile.GetCurSel();
	m_cbFile.GetLBText(nIndex,m_strFile);
	if(m_strFile.IsEmpty()==FALSE){

		CString sName;
		if(!m_bConnector){
			sName=AfxGetCablePath();
		}
		else{
			sName=AfxGetConnectorPath();
		}
		sName+=m_strFile;


		m_lcCable.DeleteAllItems();

		CStdioFile stfile;
		stfile.Open(sName,CFile::modeRead);

		int i=0;


		CString strline;

		BOOL valid_data=stfile.ReadString(strline);

		while(valid_data){


		m_lcCable.InsertItem(i,CutString(strline));
		m_lcCable.SetItemText(i,1,CutString(strline));
		m_lcCable.SetItemText(i,2,CutString(strline));
		m_lcCable.SetItemText(i,3,CutString(strline));

//old code
/*
			int index=strline.Find(';');

			CString strfield=strline.Left(index);
			strline=strline.Right(strline.GetLength()-index-1);

			m_lcCable.InsertItem(i,strfield);

			int j=1;
			while(j<=3){

				index=strline.Find(';');
				
				if(index==-1){

					m_lcCable.SetItemText(i,j,strline);

				}
				else{
					CString strfield=strline.Left(index);
					strline=strline.Right(strline.GetLength()-index-1);

					m_lcCable.SetItemText(i,j,strfield);
				}

				j++;
			}
*/

			valid_data=stfile.ReadString(strline);
			i++;


		}

		stfile.Close();
		m_strFileLast=sName;
		m_bNew=FALSE;
		UpdateData(FALSE);
	}

}

void CCableEdit::OnEditchangeFile() 
{
	m_cbFile.GetWindowText(m_strFile);
	int nIndex=m_cbFile.FindString(-1,m_strFile);
	int nLen=m_strFile.GetLength();
	if(nLen<=m_nLen){
		m_nLen=nLen;
		return;
	}
		m_nLen=nLen;


	if(nIndex!=CB_ERR ){
		m_cbFile.SetCurSel(nIndex);
		CString str;
		m_cbFile.GetLBText(nIndex,str);
		int nLen1=str.GetLength();
		m_cbFile.SetEditSel(m_nLen,nLen1);
	}
}

void CCableEdit::SaveCable(BOOL bSaveas /*=FALSE*/)
{
	//m_cbFile.GetWindowText(m_strFile);
	int nIndex=m_cbFile.GetCurSel();

	if(!bSaveas){
		m_cbFile.GetLBText(nIndex,m_strFile);

	}
	else{
		CDialogSaveas dlgSaveas;
		if(dlgSaveas.DoModal()==IDOK){


			m_strFile=dlgSaveas.m_sName;
			nIndex=m_strFile.ReverseFind('.');

			if(nIndex<0){
				m_strFile+=".txt";

			}
			//Connect to database
			CDatabase db;
			TCHAR sConnect[1024];

			g_GetConnectString(sConnect,1024);

			db.OpenEx(sConnect);

			CRecordset rsConnector(&db);

			CString str;str.Format("nNameCon = '%s'",m_strFile);
			rsConnector.m_strFilter=str;

			rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbConnector");

			if(!rsConnector.IsEOF()){
				if(AfxMessageBox("File allready exists, overwrite?",MB_YESNOCANCEL,0)==IDNO){

					rsConnector.Close();
					db.Close();
					return;

				}

			}
			else{

				CString str;str.Format("INSERT INTO tbConnector (nNameCon) VALUES ('%s')",m_strFile);
				db.ExecuteSQL(str);

			}
			rsConnector.Close();
			db.Close();
		}
		else{
			return;
		}
	}

	nIndex=m_strFile.ReverseFind('.');

	if(nIndex<0){
		m_strFile+=".txt";

	}

	if(m_strFile.IsEmpty()==FALSE){

		CString sName;
		if(!m_bConnector){
			sName=AfxGetCablePath();
		}
		else{
			sName=AfxGetConnectorPath();
		}
		sName+=m_strFile;

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind=FindFirstFile(
		  sName,			// file name
		  &FindFileData		// data buffer
		);

		if((FindNextFile(
		  hFind,			// search handle 
		  &FindFileData		// data buffer
		  ))||(GetLastError()==ERROR_NO_MORE_FILES)){
			CString str;
			str.FormatMessage(IDS_MSG_CABLEXISTS,m_strFile);
			if(AfxMessageBox(str,MB_OK|MB_ICONQUESTION,-1)!=IDOK){
				return;
			}
		}

		CStdioFile stfile;
		stfile.Open(sName,CFile::modeWrite|CFile::modeCreate);


		CString strline;

		for(int i=0;i<m_lcCable.GetItemCount();i++){
			strline="";
			for(int j=0;j<m_lcCable.GetHeaderCtrl( )->GetItemCount( );j++){
				if(!strline.IsEmpty()){
					strline+=";";
				}
				strline+=m_lcCable.GetItemText(i,j);
			}
			strline+="\n";
			stfile.WriteString(strline);
		}
		//Add an empty line so that old versions of CTBEditView load wires well
		strline="\n";
		stfile.WriteString(strline);

		stfile.Close();

		m_bModified=FALSE;
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);

	}
}

void CCableEdit::SaveCable(CString strName)
{
	//m_cbFile.GetWindowText(m_strFile);
	m_strFile=strName;

	int nIndex=m_strFile.ReverseFind('.');

	if(nIndex<0){
		m_strFile+=".txt";

	}

	if(m_strFile.IsEmpty()==FALSE){

		CString sName="";
/*
		if(!m_bConnector){
			sName=AfxGetCablePath();
		}
		else{
			sName=AfxGetConnectorPath();
		}
*/
		sName+=m_strFile;
/*
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind=FindFirstFile(
		  sName,			// file name
		  &FindFileData		// data buffer
		);

		if((FindNextFile(
		  hFind,			// search handle 
		  &FindFileData		// data buffer
		  ))||(GetLastError()==ERROR_NO_MORE_FILES)){
			CString str;
			str.FormatMessage(IDS_MSG_CABLEXISTS,m_strFile);
			if(AfxMessageBox(str,MB_OK|MB_ICONQUESTION,-1)!=IDOK){
				return;
			}
		}
*/
		CStdioFile stfile;
		stfile.Open(sName,CFile::modeWrite|CFile::modeCreate);


		CString strline;

		for(int i=0;i<m_lcCable.GetItemCount();i++){
			strline="";
			for(int j=0;j<m_lcCable.GetHeaderCtrl( )->GetItemCount( );j++){
				if(!strline.IsEmpty()){
					strline+="; ";
				}
				strline+=m_lcCable.GetItemText(i,j);
			}
			strline+="\n";
			stfile.WriteString(strline);
		}
		//Add an empty line so that old versions of CTBEditView load wires well
		strline="\n";
		stfile.WriteString(strline);

		stfile.Close();

		m_bModified=FALSE;
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);

	}
}

void CCableEdit::TrimLast(){
		int index=m_lcCable.GetItemCount()-1;
		CString str="";
		BOOL bEmpty=TRUE;
		while((index>=0)&&(bEmpty)){
			bEmpty=TRUE;
			for(int j=0;j<m_lcCable.GetHeaderCtrl( )->GetItemCount( );j++){
				str=m_lcCable.GetItemText(index,j);
				str=str.SpanExcluding(" ");
				if(!str.IsEmpty()){
					bEmpty=FALSE;;
				}
			}
			if(bEmpty){
				m_lcCable.DeleteItem(index);
			}
			index--;
		};

}

void CCableEdit::OnOK() 
{
	/*
	HWND hwnd;
	GetDlgItem(IDC_NAME,&hwnd);
	int nIndex=::SendMessage(hwnd,CB_GETCURSEL,0,0);
	m_iType=::SendMessage(hwnd,CB_GETITEMDATA,nIndex,0);
	*/
	int nIndex=m_cbFile.GetCurSel();
	m_iType=m_cbFile.GetItemData(nIndex);
	CDialog::OnOK();
}