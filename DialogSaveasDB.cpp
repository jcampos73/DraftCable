// DialogSaveasDB.cpp : implementation file
//

#include "stdafx.h"
#include "UtilsCPP.h"
#include "DraftDraw.h"
#include "DialogSaveasDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveasDB dialog


CDialogSaveasDB::CDialogSaveasDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSaveasDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSaveasDB)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSaveasDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSaveasDB)
	DDX_Control(pDX, IDC_TREE1, m_tcTree);
	DDX_Text(pDX, IDC_EDIT1, m_Name1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSaveasDB, CDialog)
	//{{AFX_MSG_MAP(CDialogSaveasDB)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveasDB message handlers

void CDialogSaveasDB::AddTreeViewItems(HTREEITEM hitem /*= NULL*/)
{
	//Locals
	HANDLE hEnum=NULL;
	//HTREEITEM hEquipos,hHwd,hRadio;
	HTREEITEM hRoot,hHD;
	DWORD dwResult=NO_ERROR;
	//DWORD dwResultEnum;

	//Initialize tree control with root elements
	if(hitem==NULL){

		//Add root component
		hRoot=AddOneItem(NULL, "DB", 
			(HTREEITEM)TVI_LAST, iDB,TRUE);

		//1ST QUERY: QUERY LIBRARIES

		//Connect to database
		if(!g_db.IsOpen()){
			TCHAR sConnect[1024];
			g_GetConnectString(sConnect,1024);
			g_db.OpenEx(sConnect);
		}

		//Query libraries
		CRecordset rsLib(&g_db);
		rsLib.m_strSort="nNameLib ASC";
		rsLib.Open(CRecordset::forwardOnly,"SELECT * FROM tbLibrary");
		
		//Iterate libraries
		while(!rsLib.IsEOF()){
			//Get library
			CString str,strName;
			rsLib.GetFieldValue("nNameLib",strName);
			strName.ReleaseBuffer();
			rsLib.GetFieldValue("iIdLib",str);

			//Add item to tree
			hHD=AddOneItem(hRoot, strName.GetBuffer(0), 
				(HTREEITEM)TVI_LAST, iLibrary,TRUE,1);

			//2ND QUERY: QUERY PARTS

			//Query parts
			CRecordset rsPart(&g_db);
			rsPart.m_strFilter="iIdLib="+str;
			rsPart.m_strSort="nNamePart ASC";
			rsPart.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");

			//Iterate throw parts
			while(!rsPart.IsEOF()){
				//Get Part
				CString str;
				rsPart.GetFieldValue("nNamePart",strName);				
				rsPart.GetFieldValue("bRackType",str);

				//Check it it is normal part or rack
				if(atoi(str)){
					AddOneItem(hHD, strName.GetBuffer(0), 
								(HTREEITEM)TVI_LAST, iPartRack,TRUE,0);
				}
				else{
					AddOneItem(hHD, strName.GetBuffer(0), 
								(HTREEITEM)TVI_LAST, iPart,TRUE,0);
				}
				strName.ReleaseBuffer();

				rsPart.MoveNext();
			}
			rsPart.Close();

			rsLib.MoveNext();
		}//end while iterate libraries

		//Close recorset
		rsLib.Close();
		//Close database
		g_db.Close();

		//Select libraries at first visible items
		GetTreeCtrl().SelectSetFirstVisible( hHD );

		//This code shoul be modified to add right view HWND as parameter.
		//DONE
		/*
		CWnd *pWnd=GetParent();
		ASSERT(pWnd->IsKindOf( RUNTIME_CLASS( CSplitterWnd ) ) );
		pWnd=((CSplitterWnd *)pWnd)->GetPane(0,1);

		HTREEITEM hSheet=AddOneItem(hHD, "Hoja1", 
		(HTREEITEM)TVI_LAST, iSheet,TRUE,(DWORD)pWnd->m_hWnd);

		AddOneItem(hSheet, "Parts", 
		(HTREEITEM)TVI_LAST, iFolderClose,FALSE);

		hRadio=AddOneItem(hSheet, "Wires", 
		(HTREEITEM)TVI_LAST, iFolderClose,FALSE);

		GetTreeCtrl().SelectSetFirstVisible( hRadio );
		*/

	}

}

//Add one item to tree
//hParent	Parent item
//szText	Item text
//hInsAfter	insert fater this item
//iImage	index of image
//bChildren	true if has any children
//data		data associated with item
HTREEITEM CDialogSaveasDB::AddOneItem(HTREEITEM hParent, LPSTR szText, HTREEITEM hInsAfter, int iImage, BOOL bChildren, DWORD data/*=0*/)
{
	HTREEITEM hItem;       // return value
	TV_ITEM tvI;           // item structure
	TV_INSERTSTRUCT tvIns; // item insert structure


	// *** set tvI.mask member ***
	if(bChildren)			
	 tvI.mask = TVIF_TEXT | TVIF_IMAGE | 
				TVIF_SELECTEDIMAGE | 
				TVIF_PARAM | 
				TVIF_CHILDREN;//item has children
	else  // item has no children
	 tvI.mask = TVIF_TEXT | TVIF_IMAGE | 
				TVIF_SELECTEDIMAGE | 
				TVIF_PARAM;

	// set text, text length, image 
	tvI.pszText = szText;
	tvI.cchTextMax = lstrlen(szText);
	tvI.iImage = iImage;

	/*indexing temporaly disabled*/
	/*int index=netresArray.Add(data);
	tvI.lParam=index;*/
	tvI.lParam=data;/*0;*/

	// get type of icon to display
	if(bChildren)	 
	// selected a folder, OK to change bitmap
/*
	if(hParent != NULL)	
	  tvI.iSelectedImage = iFolderOpen;
	else  // selected the hard drive icon, 
		  //don't change bitmap!!
*/
	  tvI.iSelectedImage = iImage;
	else
	  tvI.iSelectedImage = iImage;
	tvI.cChildren= 1; // allow one more level down

	// do the insert
	tvIns.item = tvI;
	tvIns.hInsertAfter = hInsAfter;
	tvIns.hParent = hParent;
	hItem = GetTreeCtrl().InsertItem(&tvIns);
	return (hItem); // return (add error trap here)

}

CTreeCtrl& CDialogSaveasDB::GetTreeCtrl( ) const{

	CTreeCtrl& tc=(CTreeCtrl&)m_tcTree;
	return tc;
}

BOOL CDialogSaveasDB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Initialize tree view
	GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);
	AddTreeViewItems();

	//Parse filter string
	std::string str=m_ofn.lpstrFilter;
	//std::list<std::string> lstr;
	CString cstr=_DRAFTCABLE_DOC_FILTER_STR;

	
	//regex("...");
	//int idata=split(str,NULL,0);
	int idata=CDraftDrawDoc::Split(cstr,NULL,0);

	LPTSTR *sa=new LPTSTR[idata];
	int i;
	int currentIndex = 0;
	for(i=0;i<idata;i++){
		sa[i]=new TCHAR[255];
	}

	//Split filter string
	idata=CDraftDrawDoc::Split(cstr,sa,idata);

	for(i=0;i<idata;i++){
		CString filter=sa[i];
		LRESULT result=SendDlgItemMessage(IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)sa[i]);
		if(filter=="*.dd1"){
			currentIndex = result;
		}
	}
	SendDlgItemMessage(IDC_COMBO1,CB_SETCURSEL,currentIndex,0);

	for(i=0;i<idata;i++){
		delete(sa[i]);
	}
	delete sa;
	//lstr.clear();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Manage tree item selection change
void CDialogSaveasDB::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//Get the NM_TREEVIEW
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//Get the the item
	TV_ITEM item=pNMTreeView->itemNew;

	//Recover item text and data
	TCHAR buffer[1024];
	item.mask=TVIF_HANDLE|TVIF_TEXT|TVIF_PARAM;
	item.pszText=buffer;
	item.cchTextMax=1024;
	GetTreeCtrl().GetItem(&item);

	//If item has no data, then is not a library
	if(!item.lParam){

		GetDlgItem(IDC_EDIT1)->SetWindowText(item.pszText);

		HTREEITEM hItemCur=GetTreeCtrl().GetParentItem(item.hItem);
		TV_ITEM itemcur;
		itemcur.hItem=hItemCur;
		itemcur.mask=TVIF_HANDLE|TVIF_TEXT|TVIF_PARAM;
		itemcur.pszText=buffer;
		itemcur.cchTextMax=1024;
		GetTreeCtrl().GetItem(&itemcur);

		while(!itemcur.hItem||!itemcur.lParam){


			itemcur.hItem=GetTreeCtrl().GetParentItem(itemcur.hItem);
			GetTreeCtrl().GetItem(&itemcur);
		}

		if(itemcur.hItem){

			m_Lib=itemcur.pszText;
		}

	}
	else{

		m_Lib=item.pszText;
	}
	
	*pResult = 0;
}

//Extra validation
void CDialogSaveasDB::OnOK() 
{
	//Get data from controls
	UpdateData(TRUE);

	//Check is a library is selected
	if(m_Lib.IsEmpty()){
		AfxMessageBox("Debe seleccionar una librería de la base de datos.",MB_OK|MB_ICONINFORMATION,-1);
		return;

	}

	//Name is composed of library dot part name
	m_Name=m_Lib+"."+m_Name1;
	
	//Call base class
	CDialog::OnOK();
}

/*
using namespace boost; 

boost::regex e("\\x7c\\x2a\\x2e[^\\x29]+\\x7c");

unsigned tokenise(std::list<std::string>& l, std::string& s)
{
   return boost::regex_split(std::back_inserter(l), s, e);
}

int split(std::string s)
{

	std::list<std::string> l;

   do{



      unsigned result = tokenise(l, s);
	  std::cout << result << " tokens found" << std::endl;
      std::cout << "The remaining text is: \"" << s << "\"" << std::endl;
      while(l.size())
      {
         s = *(l.begin());
         l.pop_front();
         std::cout << s << std::endl;
      }
   }while(0);


   return 0;
}
*/