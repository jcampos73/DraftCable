// ChildFrmPP.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRMPP_H__CB56AA3B_3B46_4A50_A34A_97C029A3F601__INCLUDED_)
#define AFX_CHILDFRMPP_H__CB56AA3B_3B46_4A50_A34A_97C029A3F601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndSplitter.h"


class CDraftDrawView;

class CChildFramePP : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFramePP)
public:
	CChildFramePP();

// Attributes
protected:
	CWndSplitter m_wndSplitter;
	BOOL m_bActivated;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFramePP)
	public:
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFramePP();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRMPP_H__CB56AA3B_3B46_4A50_A34A_97C029A3F601__INCLUDED_)
