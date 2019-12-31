// PageGrid.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PageGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageGrid property page

IMPLEMENT_DYNCREATE(CPageGrid, CPropertyPage)

CPageGrid::CPageGrid() : CPropertyPage(CPageGrid::IDD)
{
	//{{AFX_DATA_INIT(CPageGrid)
	m_nWidth = 0;
	//}}AFX_DATA_INIT
}

CPageGrid::~CPageGrid()
{
}

void CPageGrid::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGrid)
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxUInt(pDX, m_nWidth, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGrid, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGrid message handlers

BOOL CPageGrid::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_nWidth=(((CDraftDrawApp *)AfxGetApp())->GetActiveViewProp()).cx;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
