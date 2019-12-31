#if !defined(AFX_LISTCTRLCABLE_H__C6B5F3B3_007A_4B39_A153_4DAD871AC54C__INCLUDED_)
#define AFX_LISTCTRLCABLE_H__C6B5F3B3_007A_4B39_A153_4DAD871AC54C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlCable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCable window

class CListCtrlCable : public CListCtrl
{
// Construction
public:
	CListCtrlCable();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlCable)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlCable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlCable)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLCABLE_H__C6B5F3B3_007A_4B39_A153_4DAD871AC54C__INCLUDED_)
