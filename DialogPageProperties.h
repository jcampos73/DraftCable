#if !defined(AFX_DIALOGPAGEPROPERTIES_H__43DEA61D_6785_42C5_8034_D1B7E373F915__INCLUDED_)
#define AFX_DIALOGPAGEPROPERTIES_H__43DEA61D_6785_42C5_8034_D1B7E373F915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPageProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties

class CDialogPageProperties : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogPageProperties)

// Construction
public:
	CDialogPageProperties(UINT nIDCaption,CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDialogPageProperties(LPCTSTR pszCaption,CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPageProperties)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogPageProperties();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogPageProperties)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPAGEPROPERTIES_H__43DEA61D_6785_42C5_8034_D1B7E373F915__INCLUDED_)
