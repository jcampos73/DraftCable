#if !defined(AFX_NETALIAS_H__44A42CA4_06C1_44F1_B7E5_4AB58D14F92D__INCLUDED_)
#define AFX_NETALIAS_H__44A42CA4_06C1_44F1_B7E5_4AB58D14F92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetAlias.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetAlias dialog

class CNetAlias : public CDialog
{
// Construction
public:
	int m_iSize;
	CNetAlias(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetAlias)
	enum { IDD = IDD_NETALIAS };
	CComboBox	m_cbSize;
	CString	m_sAlias;
	BOOL	m_bVertical;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetAlias)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetAlias)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETALIAS_H__44A42CA4_06C1_44F1_B7E5_4AB58D14F92D__INCLUDED_)
