// DialogNetList.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogNetList.h"

#include "DdxCustom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNetList dialog


CDialogNetList::CDialogNetList(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNetList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNetList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogNetList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNetList)
	DDX_Control(pDX, IDC_LIST1, m_lcList);
	DDX_Control(pDX, IDC_BUTTON_DETAILS, m_btnDetails);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_btnList);
	DDX_Control(pDX, IDC_BUTTON_ICONBIG, m_btnIconBig);
	//}}AFX_DATA_MAP
	//DDX_Text(pDX, IDC_EDIT_SHEET, m_dwaSheet);
}


BEGIN_MESSAGE_MAP(CDialogNetList, CDialog)
	//{{AFX_MSG_MAP(CDialogNetList)
	ON_BN_CLICKED(IDC_BUTTON_ICONBIG, OnButtonIconbig)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_BN_CLICKED(IDC_BUTTON_DETAILS, OnButtonDetails)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_SHEET, OnRadioSheet)
	ON_BN_CLICKED(IDC_RADIO_SHEETCUR, OnRadioSheetcur)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDialogNetList::OnNMDblclkList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNetList message handlers

BOOL CDialogNetList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	HICON hicon=::LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_LRGIC));
	HICON hiconprev=m_btnIconBig.SetIcon(hicon);
	hicon=::LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_LIST));
	hiconprev=m_btnList.SetIcon(hicon);	
	hicon=::LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_DTLS));
	hiconprev=m_btnDetails.SetIcon(hicon);	




#define BITMAP_WIDTH	32
#define BITMAP_HEIGHT	32
#define BITMAP_WIDTHSM	16
#define BITMAP_HEIGHTSM	16
#define NUM_BITMAPS		1

	m_ilIcons.Create(BITMAP_WIDTH,
	  BITMAP_HEIGHT,
	  ILC_MASK,   // list does not include masks
	  NUM_BITMAPS,
	  0 );   // list will not grow
	m_ilIconsSm.Create(BITMAP_WIDTHSM,
	  BITMAP_HEIGHTSM,
	  ILC_MASK,   // list does not include masks
	  NUM_BITMAPS,
	  0 );   // list will not grow



	// Load a Shell Large icon image
	SHFILEINFO shfi;
	SHGetFileInfo( ".doc", FILE_ATTRIBUTE_NORMAL, &shfi, sizeof( SHFILEINFO ), 
				   SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);



	HICON hIcon = shfi.hIcon;//::LoadIcon (AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_DOCWIRING));
	int iItem1=m_ilIcons.Add(hIcon);
	iItem1=m_ilIconsSm.Add(hIcon);
	hIcon = shfi.hIcon;
	//SHGetFileInfo is not able to get the icon for .htm files.
	//Could it work IShellFolder::GetAttributesOf ?
	SHGetFileInfo( ".xml", FILE_ATTRIBUTE_NORMAL, &shfi, sizeof( SHFILEINFO ), 
				   SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
	hIcon = shfi.hIcon;
	int iItem2=m_ilIcons.Add(hIcon);
	iItem2=m_ilIconsSm.Add(hIcon);

	m_lcList.SetImageList(&m_ilIcons,LVSIL_NORMAL   );

	m_lcList.InsertColumn(0,"Nombre",LVCFMT_LEFT,100,0);

	m_lcList.InsertItem(0,"Plantilla Standard.doc",iItem1);
	m_lcList.InsertItem(1,"Plantilla Standard.html",iItem2);
	m_lcList.SetItemState(1, LVIS_SELECTED, LVIS_SELECTED);
	m_lcList.SetSelectionMark(1);


	SetActiveWindow();
	m_btnIconBig.SetCheck(1);//SendDlgItemMessage(IDC_BUTTON_ICONBIG,BM_CLICK);
	SendDlgItemMessage(IDC_RADIO_ALL,BM_CLICK);


	CheckRelatedCtrls();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogNetList::OnButtonIconbig() 
{
	// TODO: Add your control notification handler code here


	BOOL bResult=m_lcList.ModifyStyle(LVS_LIST   ,LVS_ICON   );
	bResult|=m_lcList.ModifyStyle(LVS_REPORT ,LVS_ICON   );


	if(bResult){

		m_lcList.SetImageList(&m_ilIcons,LVSIL_NORMAL   );
	}

}

void CDialogNetList::OnButtonList() 
{
	// TODO: Add your control notification handler code here


	BOOL bResult=m_lcList.ModifyStyle(LVS_ICON   ,LVS_LIST   );

	if(bResult){

		m_lcList.SetImageList(&m_ilIconsSm,LVSIL_SMALL   );
	}

	m_lcList.ModifyStyle(LVS_REPORT ,LVS_LIST   );
}

void CDialogNetList::OnButtonDetails() 
{
	// TODO: Add your control notification handler code here


	BOOL bResult=m_lcList.ModifyStyle(LVS_ICON   ,LVS_REPORT   );

	if(bResult){

		m_lcList.SetImageList(&m_ilIconsSm,LVSIL_SMALL   );
	}

	m_lcList.ModifyStyle(LVS_LIST   ,LVS_REPORT   );
}

void CDialogNetList::OnRadioAll() 
{
	// TODO: Add your control notification handler code here
	CheckRelatedCtrls();	
}

void CDialogNetList::OnRadioSheet() 
{
	// TODO: Add your control notification handler code here
	CheckRelatedCtrls();	
}

void CDialogNetList::OnRadioSheetcur() 
{
	// TODO: Add your control notification handler code here
	CheckRelatedCtrls();
}

void CDialogNetList::CheckRelatedCtrls()
{

	SendDlgItemMessage(IDC_EDIT_SHEET,WM_ENABLE,SendDlgItemMessage(IDC_RADIO_SHEET,BM_GETCHECK)==BST_CHECKED);
}

void CDialogNetList::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos = m_lcList.GetFirstSelectedItemPosition();
	if (pos == NULL){
	   
		AfxMessageBox(_T("Debe seleccionar un tipo de documento"),MB_OK|MB_ICONEXCLAMATION,-1);
		return;
	}	
	else
	{
		int nItem = m_lcList.GetNextSelectedItem(pos);
		m_sFileName=m_lcList.GetItemText(nItem,0);
	}
	
	CDialog::OnOK();
}


void CDialogNetList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if(pNMItemActivate->iItem>=0){
		OnOK();
	}
}
