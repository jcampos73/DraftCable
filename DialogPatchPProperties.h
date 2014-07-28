#if !defined(AFX_DIALOGPATCHPPROPERTIES_H__CACF7166_5140_405B_91A2_AB7A3CDACBDD__INCLUDED_)
#define AFX_DIALOGPATCHPPROPERTIES_H__CACF7166_5140_405B_91A2_AB7A3CDACBDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPatchPProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPatchPProperties dialog

class CDialogPatchPProperties : public CDialog
{
// Construction
public:
	CDialogPatchPProperties(CWnd* pParent = NULL);   // standard constructor
	CStringArray *m_pstraCable1;
	CStringArray *m_pstraCable2;
	std::map<std::string,int> m_mapCableNum;

// Dialog Data
	//{{AFX_DATA(CDialogPatchPProperties)
	enum { IDD = IDD_PATCHPPROP };
	CComboBox	m_cbName2;
	CComboBox	m_cbName1;
	CListCtrl	m_lcList2;
	CListCtrl	m_lcList1;
	CComboBox	m_cbFile2;
	CComboBox	m_cbFile1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPatchPProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPatchPProperties)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPATCHPPROPERTIES_H__CACF7166_5140_405B_91A2_AB7A3CDACBDD__INCLUDED_)
