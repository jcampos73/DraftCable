// DialogPatchPProperties.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPatchPProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPatchPProperties dialog


CDialogPatchPProperties::CDialogPatchPProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPatchPProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPatchPProperties)
	//}}AFX_DATA_INIT
}


void CDialogPatchPProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPatchPProperties)
	DDX_Control(pDX, IDC_NAME2, m_cbName2);
	DDX_Control(pDX, IDC_NAME1, m_cbName1);
	DDX_Control(pDX, IDC_LIST2, m_lcList2);
	DDX_Control(pDX, IDC_LIST1, m_lcList1);
	DDX_Control(pDX, IDC_FILE2, m_cbFile2);
	DDX_Control(pDX, IDC_FILE1, m_cbFile1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPatchPProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogPatchPProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPatchPProperties message handlers

BOOL CDialogPatchPProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Configures combo box control
	//-----------------------------------------------------------------

	int nCount=AfxGetCableCount();
	LPTSTR *listCable=(LPTSTR *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nCount*sizeof(CString*));
	int i;
	for(i=0;i<nCount;i++){
		listCable[i]=new char[256];
	}		
	AfxGetCables(listCable,nCount,255);
	for(i=0;i<nCount;i++){
		if(listCable[i]){
			int nIndex=m_cbFile1.AddString(listCable[i]);
			m_cbFile1.SetItemData(nIndex,i);
			nIndex=m_cbFile2.AddString(listCable[i]);
			m_cbFile2.SetItemData(nIndex,i);
			delete(listCable[i]);
		}
	}
	
	GlobalFree(listCable);
	//-----------------------------------------------------------------

	//Configures combo box control
	//-----------------------------------------------------------------
	std::map<std::string,int>::iterator it1=m_mapCableNum.begin();
	while(it1!=m_mapCableNum.end()){
		m_cbName1.AddString(it1->first.c_str());
		m_cbName2.AddString(it1->first.c_str());
		it1++;
	}

	//-----------------------------------------------------------------

	//List control
	//-----------------------------------------------------------------
	TCHAR buffer[MAX_PATH]="Número";
	LVCOLUMN lvColumn;
	lvColumn.pszText=buffer;
	lvColumn.mask=LVCF_ORDER|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvColumn.cx=50;
	lvColumn.iSubItem=0;
	lvColumn.iOrder=0;

	SendDlgItemMessage(IDC_LIST1,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);
	SendDlgItemMessage(IDC_LIST2,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);



	strcpy(buffer,"Cable Tipo");
	lvColumn.mask=LVCF_ORDER|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvColumn.cx=100;
	lvColumn.iSubItem=1;
	lvColumn.iOrder=1;

	/*
	SendDlgItemMessage(IDC_LIST1,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn);
	SendDlgItemMessage(IDC_LIST2,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn);
	*/

	m_lcList1.InsertColumn(1,"Cable Tipo",LVCFMT_LEFT,100,1);
	m_lcList2.InsertColumn(1,"Cable Tipo",LVCFMT_LEFT,100,1);

	//-----------------------------------------------------------------


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialogPatchPProperties::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch(LOWORD(wParam)){
	case ID_NEWTB1:
	case ID_NEWTB2:{
		UINT nId,nIdCB;
		CComboBox *pCombo;
		switch(LOWORD(wParam)){
		case ID_NEWTB1:
			nId=IDC_LIST1;
			nIdCB=IDC_NAME1;
			pCombo=&m_cbFile1;
			break;
		default:
			nId=IDC_LIST2;
			nIdCB=IDC_NAME2;
			pCombo=&m_cbFile2;
			break;
		}
		CString str;
		GetDlgItemText( nIdCB, str);
		if(str.IsEmpty()) break;
		int nIndex=pCombo->GetCurSel();
		if(nIndex==CB_ERR) break;
		//Get number of items in list ctrl
		int nCount=SendDlgItemMessage(nId,LVM_GETITEMCOUNT,0,0);
		//Insert item
		TCHAR buffer[MAX_PATH]="Item1";
		LVITEM lvItem;
		lvItem.mask=LVIF_TEXT|LVIF_PARAM;
		lvItem.pszText=strcpy(buffer,str);
		lvItem.lParam=m_cbFile1.GetItemData( nIndex );
		lvItem.iItem=nCount;
		lvItem.iSubItem=0;
		SendDlgItemMessage(nId,LVM_INSERTITEM,0,(LPARAM)&lvItem);
		lvItem.mask=LVIF_TEXT;
		m_cbFile1.GetLBText(nIndex,lvItem.pszText);
		lvItem.iItem=nCount;
		lvItem.iSubItem=1;
		SendDlgItemMessage(nId,LVM_SETITEM,0,(LPARAM)&lvItem);
		//m_lcList1.SetItemText(0,1,lvItem.pszText);
		}break;

	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CDialogPatchPProperties::OnOK() 
{
	// TODO: Add your command handler code here
	for(int i=0;i<2;i++){
		UINT nId;
		CStringArray *pstraCable;
		switch(i){
		case 0:
			nId=IDC_LIST1;
			m_pstraCable1=new CStringArray;
			pstraCable=m_pstraCable1;
			break;
		default:
			nId=IDC_LIST2;
			m_pstraCable2=new CStringArray;
			pstraCable=m_pstraCable2;
			break;
		}
		int nCount=SendDlgItemMessage(nId,LVM_GETITEMCOUNT,0,0);
		for(int j=0;j<nCount;j++){
			CString str;
			TCHAR buffer[MAX_PATH];
			LVITEM lvItem;
			lvItem.mask=LVIF_TEXT;
			lvItem.iSubItem=0;
			lvItem.pszText=buffer;
			lvItem.cchTextMax=MAX_PATH;
			SendDlgItemMessage(nId,LVM_GETITEMTEXT,j,(LPARAM)&lvItem);
			str=buffer;
			lvItem.iSubItem=1;
			SendDlgItemMessage(nId,LVM_GETITEMTEXT,j,(LPARAM)&lvItem);
			str+=";";
			str+=buffer;
			pstraCable->Add(str);
		}
	}

	CDialog::OnOK();
}
