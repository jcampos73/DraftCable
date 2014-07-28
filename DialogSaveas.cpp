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
	CDialog::OnOK();
}
