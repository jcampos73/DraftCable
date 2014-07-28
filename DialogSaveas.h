#if !defined(AFX_DIALOGSAVEAS_H__9342EEBC_869F_4CF5_A45C_3937881E5E57__INCLUDED_)
#define AFX_DIALOGSAVEAS_H__9342EEBC_869F_4CF5_A45C_3937881E5E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSaveas.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveas dialog

class CDialogSaveas : public CDialog
{
// Construction
public:
	CDialogSaveas(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSaveas)
	enum { IDD = IDD_SAVEAS };
	CComboBox	m_cbName;
	//}}AFX_DATA
	CString m_sName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSaveas)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSaveas)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSAVEAS_H__9342EEBC_869F_4CF5_A45C_3937881E5E57__INCLUDED_)
