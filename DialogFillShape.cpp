// DialogFillShape.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogFillShape.h"
#include "afxdialogex.h"


// CDialogFillShape dialog

IMPLEMENT_DYNAMIC(CDialogFillShape, CDialog)

CDialogFillShape::CDialogFillShape(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFillShape::IDD, pParent)
{
	m_crCurrent = RGB(255, 255, 255);
	m_crCurrentBgnd = RGB(0, 0, 0);
	m_blendCount = 0;
	m_blendPositions = NULL;
	m_blendFactors = NULL;
}

CDialogFillShape::~CDialogFillShape()
{
}

void CDialogFillShape::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_GRADIENT, m_ctrlSliderGradient);
}

void CDialogFillShape::SetColor(COLORREF cr)
{
	m_crCurrent = cr;

	//Text feed-back
	/*
	CString str = _T("");
	str.Format(_T("RGB(%d,%d,%d)"), GetRValue(m_crCurrent),
		GetGValue(m_crCurrent), GetBValue(m_crCurrent));
	GetDlgItem(IDC_COLOR_RGB)->SetWindowText(str);
	*/

	/*
	BYTE h, s, l;
	RGBtoHSL(cr, &h, &s, &l);
	str.Format(_T("HSL(%d,%d,%d)"), h, s, l);
	GetDlgItem(IDC_COLOR_HSL)->SetWindowText(str);
	*/

	if (m_brush.GetSafeHandle())
		m_brush.DeleteObject();
	m_brush.CreateSolidBrush(m_crCurrent);
	GetDlgItem(IDC_FRONT_COLOR)->Invalidate();
}

void CDialogFillShape::SetColorBgnd(COLORREF cr)
{
	m_crCurrentBgnd = cr;

	if (m_brushBgnd.GetSafeHandle())
		m_brushBgnd.DeleteObject();
	m_brushBgnd.CreateSolidBrush(m_crCurrentBgnd);
	GetDlgItem(IDC_BGND_COLOR)->Invalidate();
}

BEGIN_MESSAGE_MAP(CDialogFillShape, CDialog)
	ON_WM_CTLCOLOR()
	ON_STN_DBLCLK(IDC_FRONT_COLOR, &CDialogFillShape::OnStnDblclickFrontColor)
	ON_STN_DBLCLK(IDC_BGND_COLOR, &CDialogFillShape::OnStnDblclickBgndColor)
	ON_STN_CLICKED(IDC_STATIC_SWAP, &CDialogFillShape::OnStnBlclickSwapIcon)
	ON_BN_CLICKED(IDC_RADIO_FILL_NO, &CDialogFillShape::OnClickedRadioFillNo)
	ON_BN_CLICKED(IDC_RADIO_FILL_SOLID, &CDialogFillShape::OnClickedRadioFillSolid)
	ON_BN_CLICKED(IDC_RADIO_FILL_GRADIENT, &CDialogFillShape::OnClickedRadioFillGradient)
//	ON_COMMAND(ID_OK, &CDialogFillShape::OnOk)
END_MESSAGE_MAP()


// CDialogFillShape message handlers


HBRUSH CDialogFillShape::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		if (GetDlgItem(IDC_FRONT_COLOR)->m_hWnd == pWnd->m_hWnd)
		{
			if (m_brush.GetSafeHandle())
				hbr = m_brush;
		}
		else if (GetDlgItem(IDC_BGND_COLOR)->m_hWnd == pWnd->m_hWnd)
		{
			if (m_brushBgnd.GetSafeHandle())
				hbr = m_brushBgnd;
		}
		break;

	default:
		break;
	}

	return hbr;
}


void CDialogFillShape::OnStnDblclickFrontColor()
{
	CColorDialog dlg(m_crCurrent, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		SetColor(dlg.GetColor());
	}
}


BOOL CDialogFillShape::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_FRONT_COLOR)->SetWindowText("");
	GetDlgItem(IDC_BGND_COLOR)->SetWindowText("");

	SetColor(m_crCurrent);
	SetColorBgnd(m_crCurrentBgnd);

	//Initialize gradient slider
	_DoLoadSliders();

	ChangeFill(m_nFill);
	//m_nFill = DRAFTCABLE_FILL_NO;
	//GetDlgItem(IDC_RADIO_FILL_NO)->SendMessage(BM_SETCHECK, BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogFillShape::OnStnDblclickBgndColor()
{
	CColorDialog dlg(m_crCurrentBgnd, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		SetColorBgnd(dlg.GetColor());
	}
}

void CDialogFillShape::OnStnBlclickSwapIcon()
{
	_DoSwapColor();
}

void CDialogFillShape::_DoSwapColor()
{
	COLORREF current = m_crCurrent;
	SetColor(m_crCurrentBgnd);
	SetColorBgnd(current);
}

void CDialogFillShape::OnClickedRadioFillNo()
{
	ChangeFill(DRAFTCABLE_FILL_NO);
}


void CDialogFillShape::OnClickedRadioFillSolid()
{
	ChangeFill(DRAFTCABLE_FILL_SOLID);
}


void CDialogFillShape::OnClickedRadioFillGradient()
{
	ChangeFill(DRAFTCABLE_FILL_GRADIENT);
}

int CDialogFillShape::ChangeFill(Fill_Type nFillType)
{
	m_nFill = nFillType;

	switch (nFillType){
	case DRAFTCABLE_FILL_NO:
		GetDlgItem(IDC_RADIO_FILL_NO)->SendMessage(BM_SETCHECK, BST_CHECKED);
		GetDlgItem(IDC_RADIO_FILL_SOLID)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_FILL_GRADIENT)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		//Show-hide controls
		GetDlgItem(IDC_SLIDER_GRADIENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BGND_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SWAP)->ShowWindow(SW_HIDE);
		break;
	case DRAFTCABLE_FILL_SOLID:
		GetDlgItem(IDC_RADIO_FILL_NO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_FILL_SOLID)->SendMessage(BM_SETCHECK, BST_CHECKED);
		GetDlgItem(IDC_RADIO_FILL_GRADIENT)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		//Show-hide controls
		GetDlgItem(IDC_SLIDER_GRADIENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BGND_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SWAP)->ShowWindow(SW_HIDE);
		break;
	case DRAFTCABLE_FILL_GRADIENT:
		GetDlgItem(IDC_RADIO_FILL_NO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_FILL_SOLID)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_RADIO_FILL_GRADIENT)->SendMessage(BM_SETCHECK, BST_CHECKED);
		//Show-hide controls
		GetDlgItem(IDC_SLIDER_GRADIENT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BGND_COLOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SWAP)->ShowWindow(SW_SHOW);
		break;
	}

	return 0;
}

void CDialogFillShape::OnOK()
{
	UpdateData(TRUE);

	m_blendCount = 1;
	m_blendPositions = new float[1];
	m_blendFactors = new float[1];
	m_blendPositions[0] = m_ctrlSliderGradient.GetPos();
	m_blendPositions[0] /= (m_ctrlSliderGradient.GetRangeMax() - m_ctrlSliderGradient.GetRangeMin());
	m_blendFactors[0] = 1.0f;

	CDialog::OnOK();
}

//Implementation

void CDialogFillShape::_DoLoadSliders(){
	m_ctrlSliderGradient.SetRange(0, 10);
	m_ctrlSliderGradient.SetTicFreq(10);
	if (m_blendCount>0){
		_DoLoadSlider(m_blendPositions[0]);
	}
	else{
		_DoLoadSlider(0.5f);
	}
}

void CDialogFillShape::_DoLoadSlider(float position){
	m_ctrlSliderGradient.SetPos(position * (m_ctrlSliderGradient.GetRangeMax() - m_ctrlSliderGradient.GetRangeMin()));
}
