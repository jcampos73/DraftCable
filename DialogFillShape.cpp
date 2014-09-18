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

}

CDialogFillShape::~CDialogFillShape()
{
}

void CDialogFillShape::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CDialogFillShape::SetColor(COLORREF cr)
{
	m_crCurrent = cr;

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

BEGIN_MESSAGE_MAP(CDialogFillShape, CDialog)
	ON_WM_CTLCOLOR()
	ON_STN_DBLCLK(IDC_FRONT_COLOR, &CDialogFillShape::OnStnDblclickFrontColor)
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
