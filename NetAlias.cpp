// NetAlias.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "NetAlias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetAlias dialog


CNetAlias::CNetAlias(CWnd* pParent /*=NULL*/)
	: CDialog(CNetAlias::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetAlias)
	m_sAlias = _T("");
	m_bVertical = FALSE;
	//}}AFX_DATA_INIT
}


void CNetAlias::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetAlias)
	DDX_Control(pDX, IDC_SIZEFT, m_cbSize);
	DDX_Text(pDX, IDC_ALIAS, m_sAlias);
	DDV_MaxChars(pDX, m_sAlias, 255);
	DDX_Check(pDX, IDC_VERTICAL, m_bVertical);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetAlias, CDialog)
	//{{AFX_MSG_MAP(CNetAlias)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetAlias message handlers

BOOL CNetAlias::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbSize.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNetAlias::OnOK() 
{
	// TODO: Add extra validation here
	CString str;

	m_cbSize.GetWindowText(str);

	m_iSize=_wtoi(str);

	CDialog::OnOK();
}
