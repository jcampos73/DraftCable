#if !defined(AFX_DIALOGTEXT_H__0F197CEE_5A64_4C06_9BFD_345A5D750A47__INCLUDED_)
#define AFX_DIALOGTEXT_H__0F197CEE_5A64_4C06_9BFD_345A5D750A47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogText dialog

class CDialogText : public CDialog
{
// Construction
public:
	int m_iSize;
	CDialogText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogText)
	enum { IDD = IDD_TEXT };
	BOOL	m_bVertical;
	CString	m_sAlias;
	CString	m_sFont;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogText)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnChangeFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEXT_H__0F197CEE_5A64_4C06_9BFD_345A5D750A47__INCLUDED_)
