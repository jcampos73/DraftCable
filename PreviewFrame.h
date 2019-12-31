#if !defined(AFX_PREVIEWFRAME_H__6E134B05_3B2D_4543_A296_F4821CBA935E__INCLUDED_)
#define AFX_PREVIEWFRAME_H__6E134B05_3B2D_4543_A296_F4821CBA935E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewFrame window
//Window to manage preview window in Place Part Dialog and SIC Dialog.

class CPreviewFrame : public CWnd
{
// Construction
public:
	CPreviewFrame();
	BOOL Create(CWnd* pParent, CRuntimeClass* pViewClass, CCreateContext* pcc,
		UINT nIDRes = 0,
		int cxFolderTabCtrl=-2,
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreviewFrame();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWFRAME_H__6E134B05_3B2D_4543_A296_F4821CBA935E__INCLUDED_)
