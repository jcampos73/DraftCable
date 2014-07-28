// PageSize.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PageSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSize property page

#define DRAFTCABLE_SIZE_A4			0
#define DRAFTCABLE_SIZE_A3			1
#define DRAFTCABLE_SIZE_CUSTOM		7


IMPLEMENT_DYNCREATE(CPageSize, CPropertyPage)

CPageSize::CPageSize() : CPropertyPage(CPageSize::IDD)
{
	//{{AFX_DATA_INIT(CPageSize)
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT
	m_szSize=CSize(0,0);
}

CPageSize::~CPageSize()
{
}

void CPageSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSize)
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDV_MinMaxUInt(pDX, m_nHeight, 0, 10000);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxUInt(pDX, m_nWidth, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSize, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSize)
	ON_BN_CLICKED(IDC_RADIO_A3, OnRadioA3)
	ON_BN_CLICKED(IDC_RADIO_A4, OnRadioA4)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSize message handlers

BOOL CPageSize::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_RADIO_MILLIM)->SendMessage(BM_SETCHECK,BST_CHECKED);
	GetDlgItem(IDC_RADIO_INCH)->SendMessage(BM_SETCHECK,BST_UNCHECKED);

	ChangeSize(DRAFTCABLE_SIZE_CUSTOM);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPageSize::ChangeSize(int nSize)
{


	if(nSize==DRAFTCABLE_SIZE_CUSTOM){
		if(m_szSize==CSize(0,0)){
			m_szSize=CSize(297,210);
			nSize=DRAFTCABLE_SIZE_A4;
		}
		else if(m_szSize==CSize(297,210)){
			nSize=DRAFTCABLE_SIZE_A4;
		}
		else if(m_szSize==CSize(420,297)){
			nSize=DRAFTCABLE_SIZE_A3;
		}
	}


	switch(nSize){
	case DRAFTCABLE_SIZE_A4:
		m_szSize=CSize(297,210);
		GetDlgItem(IDC_RADIO_A4)->SendMessage(BM_SETCHECK,BST_CHECKED);
		GetDlgItem(IDC_RADIO_A3)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_CUSTOM)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		break;
	case DRAFTCABLE_SIZE_A3:
		m_szSize=CSize(420,297);
		GetDlgItem(IDC_RADIO_A4)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_A3)->SendMessage(BM_SETCHECK,BST_CHECKED);
		GetDlgItem(IDC_RADIO_CUSTOM)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		break;
	case DRAFTCABLE_SIZE_CUSTOM:
		GetDlgItem(IDC_RADIO_A4)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_A3)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_CUSTOM)->SendMessage(BM_SETCHECK,BST_CHECKED);
		break;
	}

	return 0;
}

void CPageSize::OnRadioA3() 
{
	// TODO: Add your control notification handler code here
	ChangeSize(DRAFTCABLE_SIZE_A3);
}

void CPageSize::OnRadioA4() 
{
	// TODO: Add your control notification handler code here
	ChangeSize(DRAFTCABLE_SIZE_A4);	
}

void CPageSize::OnRadioCustom() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_szSize=CSize(m_nWidth,m_nHeight);
	ChangeSize(DRAFTCABLE_SIZE_CUSTOM);	
}
