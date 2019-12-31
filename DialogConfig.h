#if !defined(AFX_DIALOGCONFIG_H__B80CB6AC_7870_4EA3_80AE_2AD3FFBDE68E__INCLUDED_)
#define AFX_DIALOGCONFIG_H__B80CB6AC_7870_4EA3_80AE_2AD3FFBDE68E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConfig dialog

class CDialogConfig : public CDialog
{
// Construction
public:
	CDialogConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogConfig)
	enum { IDD = IDD_CONFIG };
	CString	m_sPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConfig)
	afx_msg void OnExam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONFIG_H__B80CB6AC_7870_4EA3_80AE_2AD3FFBDE68E__INCLUDED_)
