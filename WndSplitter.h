#if !defined(AFX_WNDSPLITTER_H__4AD92B0E_4505_474A_A233_0688DAFA604B__INCLUDED_)
#define AFX_WNDSPLITTER_H__4AD92B0E_4505_474A_A233_0688DAFA604B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndSplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndSplitter window

class CWndSplitter : public CSplitterWnd
{
// Construction
public:
	CWndSplitter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndSplitter)
	public:
	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndSplitter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndSplitter)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDSPLITTER_H__4AD92B0E_4505_474A_A233_0688DAFA604B__INCLUDED_)
