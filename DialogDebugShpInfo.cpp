// DialogDebugShpInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogDebugShpInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDebugShpInfo dialog


CDialogDebugShpInfo::CDialogDebugShpInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDebugShpInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDebugShpInfo)
	m_sText = _T("");
	//}}AFX_DATA_INIT
}


void CDialogDebugShpInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDebugShpInfo)
	DDX_Text(pDX, IDC_TEXT, m_sText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDebugShpInfo, CDialog)
	//{{AFX_MSG_MAP(CDialogDebugShpInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDebugShpInfo message handlers

BOOL CDialogDebugShpInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
