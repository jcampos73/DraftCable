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
	m_szSize=CSize(-1,-1);
}

CPageSize::~CPageSize()
{
}

void CPageSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSize)
	DDX_Text(pDX, IDC_STATIC_CT_H, m_nHeight);
	DDV_MinMaxFloat(pDX, m_nHeight, 0.0, 10000.0);
	DDX_Text(pDX, IDC_STATIC_CT_W, m_nWidth);
	DDV_MinMaxFloat(pDX, m_nWidth, 0.0, 10000.0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSize, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSize)
	ON_BN_CLICKED(IDC_RADIO_A3, OnRadioA3)
	ON_BN_CLICKED(IDC_RADIO_A4, OnRadioA4)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_MILLIM, &CPageSize::OnClickedRadioMillim)
	ON_BN_CLICKED(IDC_RADIO_INCH, &CPageSize::OnClickedRadioInch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSize message handlers

BOOL CPageSize::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	ChangeUnits(MM);

	ChangeSize(DRAFTCABLE_SIZE_A4);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPageSize::ChangeSize(int nSize)
{
	if(nSize==DRAFTCABLE_SIZE_CUSTOM){
		if (m_szSize.cx < 0 || m_szSize.cy < 0){
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
		GetDlgItem(IDC_STATIC_CT_W)->SendMessage(WM_ENABLE, FALSE);
		GetDlgItem(IDC_STATIC_CT_H)->SendMessage(WM_ENABLE, FALSE);
		break;
	case DRAFTCABLE_SIZE_A3:
		m_szSize=CSize(420,297);
		GetDlgItem(IDC_RADIO_A4)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_A3)->SendMessage(BM_SETCHECK,BST_CHECKED);
		GetDlgItem(IDC_RADIO_CUSTOM)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_STATIC_CT_W)->SendMessage(WM_ENABLE, FALSE);
		GetDlgItem(IDC_STATIC_CT_H)->SendMessage(WM_ENABLE, FALSE);
		break;
	case DRAFTCABLE_SIZE_CUSTOM:
		GetDlgItem(IDC_RADIO_A4)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_A3)->SendMessage(BM_SETCHECK,BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_CUSTOM)->SendMessage(BM_SETCHECK,BST_CHECKED);
		GetDlgItem(IDC_STATIC_CT_W)->SendMessage(WM_ENABLE, TRUE);
		GetDlgItem(IDC_STATIC_CT_H)->SendMessage(WM_ENABLE, TRUE);
		break;
	}

	return 0;
}

int CPageSize::ChangeUnits(Units_Type units)
{
	CString str;

	UpdateData(TRUE);

	switch (units){
	case MM:
		GetDlgItem(IDC_RADIO_MILLIM)->SendMessage(BM_SETCHECK, BST_CHECKED);
		GetDlgItem(IDC_RADIO_INCH)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		SetDlgItemText(IDC_STATIC_A4, _T("millimeters"));
		SetDlgItemText(IDC_STATIC_A3, _T("millimeters"));
		SetDlgItemText(IDC_STATIC_CUSTOM, _T("millimeters"));

		str.Format(_T("%.1f"), A4_WIDTH_MM);
		SetDlgItemText(IDC_STATIC_A4_W, str);
		str.Format(_T("%.1f"), A4_HEIGHT_MM);
		SetDlgItemText(IDC_STATIC_A4_H, str);
		str.Format(_T("%.1f"), A3_WIDTH_MM);
		SetDlgItemText(IDC_STATIC_A3_W, str);
		str.Format(_T("%.1f"), A3_HEIGHT_MM);
		SetDlgItemText(IDC_STATIC_A3_H, str);
		str.Format(_T("%.1f"), m_nWidth * INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_CT_W, str);
		str.Format(_T("%.1f"), m_nHeight * INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_CT_H, str);
		break;
	case INCH:
		GetDlgItem(IDC_RADIO_MILLIM)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_INCH)->SendMessage(BM_SETCHECK, BST_CHECKED);
		SetDlgItemText(IDC_STATIC_A4, _T("inches"));
		SetDlgItemText(IDC_STATIC_A3, _T("inches"));
		SetDlgItemText(IDC_STATIC_CUSTOM, _T("inches"));

		str.Format(_T("%.1f"), A4_WIDTH_MM / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_A4_W, str);
		str.Format(_T("%.1f"), A4_HEIGHT_MM / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_A4_H, str);
		str.Format(_T("%.1f"), A3_WIDTH_MM / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_A3_W, str);
		str.Format(_T("%.1f"), A3_HEIGHT_MM / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_A3_H, str);
		str.Format(_T("%.1f"), m_nWidth / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_CT_W, str);
		str.Format(_T("%.1f"), m_nHeight / INCH_TO_MM);
		SetDlgItemText(IDC_STATIC_CT_H, str);
		break;
	}

	return 0;
}

void CPageSize::OnRadioA3() 
{
	ChangeSize(DRAFTCABLE_SIZE_A3);
}

void CPageSize::OnRadioA4() 
{
	ChangeSize(DRAFTCABLE_SIZE_A4);	
}

void CPageSize::OnRadioCustom() 
{
	UpdateData(TRUE);
	m_szSize = CSize(m_nWidth, m_nHeight);

	ChangeSize(DRAFTCABLE_SIZE_CUSTOM);	
}


void CPageSize::OnClickedRadioMillim()
{
	ChangeUnits(MM);
}


void CPageSize::OnClickedRadioInch()
{
	ChangeUnits(INCH);
}

float CPageSize::MMToPixel(HDC screen)
{
	int hSize = GetDeviceCaps(screen, HORZSIZE);
	int hRes = GetDeviceCaps(screen, HORZRES);
	float PixelsPerMM = (float)hRes / hSize;   // pixels per millimeter
	float PixelsPerInch = PixelsPerMM * INCH_TO_MM; //dpi

	return PixelsPerMM;
}

void CPageSize::OnOK()
{
	if (SendDlgItemMessage(IDC_RADIO_CUSTOM, BM_GETCHECK) == BST_CHECKED){
		UpdateData(TRUE);
		m_szSize = CSize(m_nWidth, m_nHeight);
		if (SendDlgItemMessage(IDC_RADIO_INCH, BM_GETCHECK) == BST_CHECKED){
			m_szSize = CSize(m_nWidth * INCH_TO_MM, m_nHeight * INCH_TO_MM);
		}
	}

	CPropertyPage::OnOK();
}
