// DialogPartProp.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPartProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPartProp dialog


CDialogPartProp::CDialogPartProp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPartProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPartProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogPartProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPartProp)
	DDX_Control(pDX, IDC_DATAGRID1, m_grid);
	DDX_Control(pDX, IDC_ADODC1, m_adoDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPartProp, CDialog)
	//{{AFX_MSG_MAP(CDialogPartProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPartProp message handlers

BOOL CDialogPartProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Code to rebind controls
	TCHAR sConnect[1024];

	g_GetConnectString(sConnect,1024);


	m_adoDC.SetConnectionString(sConnect);
	CString str=m_adoDC.GetConnectionString();
	m_adoDC.Refresh();
	LPUNKNOWN lpUnk=m_grid.GetDataSource();
	m_grid.SetRefDataSource(m_adoDC.GetControlUnknown());
	lpUnk=m_grid.GetDataSource();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDialogPartProp, CDialog)
    //{{AFX_EVENTSINK_MAP(CDialogPartProp)
	ON_EVENT(CDialogPartProp, IDC_ADODC1, 211 /* Error */, OnErrorAdodc1, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDialogPartProp::OnErrorAdodc1(long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay) 
{
	// TODO: Add your control notification handler code here
	*fCancelDisplay=TRUE;
}
