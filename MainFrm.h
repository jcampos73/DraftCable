// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2A0C7336_CE61_4808_8F51_BE56B40A4839__INCLUDED_)
#define AFX_MAINFRM_H__2A0C7336_CE61_4808_8F51_BE56B40A4839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	// construction helpers
	void SetupMenus();

// Attributes
public:
	CView * FindView(CDocTemplate *pTemplate,BOOL bNotTree = FALSE,CRuntimeClass *pViewMatch =NULL);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndToolBarDrawing;
	// custom menu tests implementation in custmenu.cpp
	void AttachCustomMenu();
	CMenu m_menuPolygon;
public:
	CToolBar	m_wndToolBarPlace;
	HBITMAP m_hbCurrent;
	HBITMAP m_hbCurrentRack;
	HBITMAP m_hbDefault;
	CBitmap m_bmToolBar;
	CBitmap m_bmDefault;
	CBitmap m_bmToolBarRack;
	CBitmap m_bmDefaultRack;

// Generated message map functions
protected:
	CString m_strMode;
	CString m_strTool;
	CString m_strCoor;
	afx_msg void OnUpdateTool(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCoor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMode(CCmdUI *pCmdUI);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnFileNewComponente();
	//}}AFX_MSG
	afx_msg void OnUpdateBtEditPart(CCmdUI *pCmdUI);
	//afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnToolbarDropDown(NMHDR* pnmh, LRESULT* plRes);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2A0C7336_CE61_4808_8F51_BE56B40A4839__INCLUDED_)
