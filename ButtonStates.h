#if !defined(AFX_BUTTONSTATES_H__F9021FB5_50A3_4A0D_9ED6_0FFE1C1142AF__INCLUDED_)
#define AFX_BUTTONSTATES_H__F9021FB5_50A3_4A0D_9ED6_0FFE1C1142AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonStates.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonStates window

class CButtonStates : public CButton
{
// Construction
public:
	CButtonStates();

// Attributes
public:

// Operations
public:
	void SetCheck( int nCheck );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonStates)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonStates();

	// Generated message map functions
protected:
	UINT m_uStyle;
	//{{AFX_MSG(CButtonStates)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONSTATES_H__F9021FB5_50A3_4A0D_9ED6_0FFE1C1142AF__INCLUDED_)
