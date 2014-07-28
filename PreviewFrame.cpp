// PreviewFrame.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PreviewFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame

CPreviewFrame::CPreviewFrame()
{
}

CPreviewFrame::~CPreviewFrame()
{
}


BEGIN_MESSAGE_MAP(CPreviewFrame, CWnd)
	//{{AFX_MSG_MAP(CPreviewFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////
// Create window: register class, create view
//
BOOL CPreviewFrame::Create(CWnd* pParent, CRuntimeClass* pViewClass,
	CCreateContext* pcc, UINT nIDRes, int cxFolderTabCtrl, DWORD dwStyle)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	static LPCTSTR lpClassName = NULL;
	if (!lpClassName) {
		lpClassName = _T("PxlFolderFrame");
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = NULL; //CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		VERIFY(AfxRegisterClass(&wc));
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame message handlers
