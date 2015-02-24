// DialogSaveas.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogSaveas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveas dialog


CDialogSaveas::CDialogSaveas(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSaveas::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSaveas)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bConnector = FALSE;
}


void CDialogSaveas::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSaveas)
	DDX_Control(pDX, IDC_COMBO1, m_cbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSaveas, CDialog)
	//{{AFX_MSG_MAP(CDialogSaveas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveas message handlers

void CDialogSaveas::OnOK() 
{
	// TODO: Add extra validation here
	m_cbName.GetWindowText(m_sName);
	m_bTogCabCon = (SendDlgItemMessage(IDC_CHECK_TOGGLE_CAB_CON, BM_GETCHECK) == BST_CHECKED);
	CDialog::OnOK();
}


BOOL CDialogSaveas::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString lbl;
	if (m_bConnector)
	{
		lbl.FormatMessage(IDS_LBL_SAVEAS_CABLE);
		SetDlgItemText(IDC_CHECK_TOGGLE_CAB_CON, lbl);
	}
	else{
		lbl.FormatMessage(IDS_LBL_SAVEAS_CONNECTOR);
		SetDlgItemText(IDC_CHECK_TOGGLE_CAB_CON, lbl);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
