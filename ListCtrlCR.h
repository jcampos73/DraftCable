#if !defined(AFX_LISTCTRLCR_H__9F2FC1EE_A400_4A39_98A0_9827B6818975__INCLUDED_)
#define AFX_LISTCTRLCR_H__9F2FC1EE_A400_4A39_98A0_9827B6818975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlCR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCR window

class CListCtrlCR : public CListCtrl
{
// Construction
public:
	CListCtrlCR();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlCR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlCR();
protected:
	static int CALLBACK CompareProcAlphDesc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlCR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLCR_H__9F2FC1EE_A400_4A39_98A0_9827B6818975__INCLUDED_)
