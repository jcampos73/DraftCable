#if !defined(AFX_DIALOGNETLIST_H__07BBBA22_7531_43CC_8AD6_389D31115ABB__INCLUDED_)
#define AFX_DIALOGNETLIST_H__07BBBA22_7531_43CC_8AD6_389D31115ABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogNetList.h : header file
//

#include "ButtonStates.h"


/////////////////////////////////////////////////////////////////////////////
// CDialogNetList dialog

class CDialogNetList : public CDialog
{
// Construction
public:
	CDialogNetList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogNetList)
	enum { IDD = IDD_NETLIST };
	CListCtrl	m_lcList;
	CButtonStates	m_btnDetails;
	CButtonStates	m_btnList;
	CButtonStates	m_btnIconBig;
	//}}AFX_DATA
	CImageList m_ilIcons;
	CImageList m_ilIconsSm;
	CDWordArray m_dwaSheet;
	CString m_sFileName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNetList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CheckRelatedCtrls();

	// Generated message map functions
	//{{AFX_MSG(CDialogNetList)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonIconbig();
	afx_msg void OnButtonList();
	afx_msg void OnButtonDetails();
	afx_msg void OnRadioAll();
	afx_msg void OnRadioSheet();
	afx_msg void OnRadioSheetcur();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNETLIST_H__07BBBA22_7531_43CC_8AD6_389D31115ABB__INCLUDED_)
