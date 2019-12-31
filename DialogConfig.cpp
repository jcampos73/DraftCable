// DialogConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogConfig dialog


CDialogConfig::CDialogConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConfig)
	m_sPath = _T("");
	//}}AFX_DATA_INIT
}


void CDialogConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConfig)
	DDX_Text(pDX, IDC_EDIT1, m_sPath);
	DDV_MaxChars(pDX, m_sPath, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConfig, CDialog)
	//{{AFX_MSG_MAP(CDialogConfig)
	ON_BN_CLICKED(IDC_EXAM, OnExam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConfig message handlers

void CDialogConfig::OnExam() 
{
	// TODO: Add your control notification handler code here

	BROWSEINFO bi;
	TCHAR DisplayName[MAX_PATH];
	int iImage=0;

	bi.hwndOwner=AfxGetMainWnd()->m_hWnd;
	bi.pidlRoot=NULL;
	bi.pszDisplayName=DisplayName;
	bi.lpszTitle=NULL;
	bi.ulFlags=0;
	bi.lpfn=NULL;
	bi.iImage=iImage;


	LPITEMIDLIST pidl=SHBrowseForFolder(
		&bi
	); 

	if(pidl!=NULL){
		TCHAR Path[MAX_PATH];
		if(SHGetPathFromIDList(
			pidl,
			Path
			)){
			m_sPath=Path;
		}
	}

	UpdateData(FALSE);
}
