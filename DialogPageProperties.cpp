// DialogPageProperties.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "DialogPageProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties

IMPLEMENT_DYNAMIC(CDialogPageProperties, CPropertySheet)

CDialogPageProperties::CDialogPageProperties(UINT nIDCaption,CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CDialogPageProperties::CDialogPageProperties(LPCTSTR pszCaption,CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CDialogPageProperties::~CDialogPageProperties()
{
}


BEGIN_MESSAGE_MAP(CDialogPageProperties, CPropertySheet)
	//{{AFX_MSG_MAP(CDialogPageProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties message handlers
