// DialogText.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogText dialog


CDialogText::CDialogText(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogText)
	m_bVertical = FALSE;
	m_sAlias = _T("");
	m_sFont = _T("Arial,10");
	m_sName = _T("");
	//}}AFX_DATA_INIT
	m_iSize = 10;
}


void CDialogText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogText)
	DDX_Check(pDX, IDC_VERTICAL, m_bVertical);
	DDX_Text(pDX, IDC_ALIAS, m_sAlias);
	DDV_MaxChars(pDX, m_sAlias, 255);
	DDX_Text(pDX, IDC_FONT, m_sFont);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogText, CDialog)
	//{{AFX_MSG_MAP(CDialogText)
	ON_COMMAND(ID_OK, OnOk)
	ON_BN_CLICKED(ID_CHANGE_FONT, OnChangeFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogText message handlers

BOOL CDialogText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_ALIAS)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogText::OnOk() 
{
	// TODO: Add your command handler code here


	CDialog::OnOK();
}

void CDialogText::OnChangeFont() 
{
	// TODO: Add your control notification handler code here
/*
	CHOOSEFONT chfont;
	LOGFONT LogFont;
	TCHAR Style[1024];
	
	chfont.lStructSize=sizeof(CHOOSEFONT);
	chfont.hwndOwner=m_hWnd;
	//chfont.hDC;
	chfont.lpLogFont=&LogFont;
	//chfont.iPointSize;
	chfont.Flags=CF_EFFECTS;
	//chfont.rgbColors;
	//chfont.lCustData;
	//chfont.lpfnHook;
	//chfont.lpTemplateName;
	//chfont.hInstance;
	chfont.lpszStyle=Style;
	//chfont.nFontType;
	//chfont.nSizeMax;
	//chfont.nSizeMin;

	if(::ChooseFont(&chfont)){

	}
*/

	UpdateData(TRUE);

	CHOOSEFONT cf;            // common dialog box structure
	static LOGFONT lf;        // logical font structure

	lf.lfHeight=-13;
	lf.lfWeight=400;
	lf.lfOutPrecision=3;
	lf.lfClipPrecision=2;
	lf.lfQuality=1;
	lf.lfPitchAndFamily=34;
	strcpy(lf.lfFaceName,"Arial");

	static DWORD rgbCurrent;   // current text color
	//HFONT hfont, hfontPrev;
	//DWORD rgbPrev;

	// Initialize CHOOSEFONT
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof (CHOOSEFONT);
	cf.hwndOwner = m_hWnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = rgbCurrent;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;

	if (ChooseFont(&cf)==TRUE) {
		//hfont = CreateFontIndirect(cf.lpLogFont);
		//hfontPrev = SelectObject(hdc, hfont);
		//rgbCurrent= cf.rgbColors;
		//rgbPrev = SetTextColor(hdc, rgbCurrent);

		m_sFont.Format("%s, %i",lf.lfFaceName,cf.iPointSize/10);

		m_iSize=cf.iPointSize/10;

		UpdateData(FALSE);
	}


	
}
