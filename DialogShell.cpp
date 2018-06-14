// DialogShell.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogShell.h"
#include "afxdialogex.h"


// CDialogShell dialog

IMPLEMENT_DYNAMIC(CDialogShell, CDialog)

CDialogShell::CDialogShell(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShell::IDD, pParent)
{

}

CDialogShell::~CDialogShell()
{
}

void CDialogShell::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHELL, m_ecShell);
}


BEGIN_MESSAGE_MAP(CDialogShell, CDialog)
END_MESSAGE_MAP()


// CDialogShell message handlers


void CDialogShell::OnOK()
{
	UpdateData(TRUE);

	CDraftDrawDoc *pDoc = theApp.GetActiveDocument();

	CString text;
	m_ecShell.GetWindowText(text);

	std::wstring sCommand((LPCTSTR)text);

	TCHAR buffer[255];
	BOOL bResult = theApp.m_OMParser.OMParse(sCommand, 0, buffer, 255) > 0;

	m_ecShell.SetWindowText(buffer);

	UpdateData(FALSE);

	//CDialog::OnOK();
}


BOOL CDialogShell::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ecShell.SetWindowText(_T("ActiveSheet.Shapes.Count"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
