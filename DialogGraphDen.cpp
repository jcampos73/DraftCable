// DialogGraphDen.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogGraphDen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGraphDen dialog


CDialogGraphDen::CDialogGraphDen(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGraphDen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGraphDen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CGraphDView );
	CObject* pObject = pRuntimeClass->CreateObject();
	ASSERT( pObject->IsKindOf( RUNTIME_CLASS( CGraphDView ) ) );


	m_pGDView=(CGraphDView *)pObject;

}


void CDialogGraphDen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGraphDen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGraphDen, CDialog)
	//{{AFX_MSG_MAP(CDialogGraphDen)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGraphDen message handlers

int CDialogGraphDen::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	CRect rect;
	GetClientRect(rect);

	m_pGDView->Create(NULL,_T("MyView"),WS_CHILD|WS_VISIBLE|WS_BORDER,rect,this,1000);
	m_pGDView->m_pShArray=m_pShArray;
	
	return 0;
}
