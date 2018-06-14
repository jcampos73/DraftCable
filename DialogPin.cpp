// DialogPin.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPin.h"

#include "Globals.h"
#include "ShapeUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDialogPin dialog


CDialogPin::CDialogPin(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPin)
	m_iShapeType = -1;
	m_stNumber = _T("");
	m_bJack = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogPin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPin)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE, m_iShapeType);
	DDX_CBString(pDX, IDC_COMBO_NUMBER, m_stNumber);
	DDV_MaxChars(pDX, m_stNumber, 10);
	DDX_Check(pDX, IDC_JACK, m_bJack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPin, CDialog)
	//{{AFX_MSG_MAP(CDialogPin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPin message handlers

BOOL CDialogPin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	HWND hWnd;
	GetDlgItem(IDC_COMBO_SHAPE,&hWnd);
	CWnd *pWnd=GetDlgItem(IDC_COMBO_NUMBER);
	pWnd->SetWindowText(_T("1"));


	strcpy(m_Str,"Unit pin");
	LPARAM lParam=(LPARAM) &m_Str;

	::SendMessage( 
		hWnd,        // handle to destination window 
		CB_ADDSTRING,       // message to send
		0,					// not used; must be zero
		lParam				// string to add (LPCTSTR)
	);

	strcpy(m_Str,"Wire pin");

	::SendMessage( 
		hWnd,        // handle to destination window 
		CB_ADDSTRING,       // message to send
		0,					// not used; must be zero
		lParam				// string to add (LPCTSTR)
	);

	WPARAM wParam=0;

	::SendMessage( 
		hWnd,        // handle to destination window 
		CB_SETCURSEL,       // message to send
		wParam,
		0
	);
	
	//Configures combo box control
	//-----------------------------------------------------------------
	int nCount=AfxGetConnectorCount();
	LPTSTR *listConnector=(LPTSTR *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nCount*sizeof(CString*));
	int i;
	for(i=0;i<nCount;i++){
		listConnector[i]=new TCHAR[256];
	}		
	AfxGetConnector(listConnector,nCount,255);
	for(i=0;i<nCount;i++){
		if(listConnector[i]){
			int nIndex=m_cbType.AddString(listConnector[i]);
			m_cbType.SetItemData(nIndex,i);
			delete(listConnector[i]);
		}
	}
	/*
	if(!m_strFile.IsEmpty()){
		m_cbFile.SelectString(-1,m_strFile);
	}
	else{
	*/
		m_cbType.SetCurSel( 0 );
	/*}*/
	//-----------------------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogPin::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_iShape=m_iShapeType|(m_bJack*SHAPEUNIT_PINTYPE_JACK);
	int nIndex=m_cbType.GetCurSel();
	m_TypePin=m_cbType.GetItemData(nIndex);
	CDialog::OnOK();
}
