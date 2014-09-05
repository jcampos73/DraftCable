#if !defined(AFX_DIALOGPLACEPART_H__E0F582BE_B155_43D8_AFD0_355D4EA61288__INCLUDED_)
#define AFX_DIALOGPLACEPART_H__E0F582BE_B155_43D8_AFD0_355D4EA61288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "PartPView.h"


// DialogPlacePart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPlacePart dialog

class CDialogPlacePart : public CDialog
{
// Construction
public:
	CString m_sPart;
	//dwType =0 -> Standard place dialog
	//dwType =1 -> SIC dialog
	CDialogPlacePart(CWnd* pParent = NULL, DWORD dwType =0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPlacePart)
	enum { IDD = IDD_PLACEPART2 };
	CListCtrl	m_lcPart;
	CComboBox	m_cbLibrary;
	//}}AFX_DATA
	CPartPView *m_pPPView;
	int m_iLibrary;//id library
	CString m_sLibrary;//string library
	CDatabase m_db;
	DWORD m_dwType;
	BOOL m_bFlagNew;
	BOOL m_bFlagHdlItemChanged;//enables handling of item changed event
	BOOL m_bFlagHdlItemChanged1;//disables handling of item changed event for 1 time


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPlacePart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void UpdatePartList();
	void LoadUnit(LPCTSTR lpszUnit =NULL);
	void DoImportLibrary();
	// Generated message map functions
	//{{AFX_MSG(CDialogPlacePart)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemchangedListpart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelendokLibrary();
	afx_msg void OnButtonNew( );
	afx_msg void OnEndlabeleditListpart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDel( );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonNewlib();
	afx_msg void OnCheckAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPLACEPART_H__E0F582BE_B155_43D8_AFD0_355D4EA61288__INCLUDED_)
