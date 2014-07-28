// TBEditView.h : interface of the CTBEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBEDITVIEW_H__5B96F45F_06A2_4038_A8CF_CF2BCC150046__INCLUDED_)
#define AFX_TBEDITVIEW_H__5B96F45F_06A2_4038_A8CF_CF2BCC150046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTBEditView : public CFormView
{
protected: // create from serialization only
	CTBEditView();
	DECLARE_DYNCREATE(CTBEditView)

public:
	//{{AFX_DATA(CTBEditView)
	enum{ IDD = IDD_TBEDIT_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CDraftDrawDoc* GetDocument();
	CBrush* m_pEditBkBrush;
	int m_nID;
	int m_nIndex;
	CString m_strID;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBEditView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTBEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bFlagIni;
	BOOL m_bModifiedFlag;
	int m_nCountMax1;
	int m_nCountMax2;
	int m_nCountTotalMax1;
	int m_nCountTotalMax2;
	int m_nItemSel;
	CList<WORD,WORD> m_listItemSel;

// Generated message map functions
protected:
	//{{AFX_MSG(CTBEditView)
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TBEditView.cpp
//inline CTBEditDoc* CTBEditView::GetDocument()
//   { return (CTBEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBEDITVIEW_H__5B96F45F_06A2_4038_A8CF_CF2BCC150046__INCLUDED_)
