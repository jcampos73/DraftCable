#if !defined(AFX_PARTEDIT_H__7C95B5C3_4E18_4B24_B5FC_4003E83D2A9E__INCLUDED_)
#define AFX_PARTEDIT_H__7C95B5C3_4E18_4B24_B5FC_4003E83D2A9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartEdit.h : header file
//

#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////
// CPartEdit dialog

class CPartEdit : public CDialog
{
// Construction
public:
	CShape * m_pShape;
	CPartEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPartEdit)
	enum { IDD = IDD_PARTEDIT };
	CListCtrl	m_lcRegister;
	CComboBox	m_cbProperty;
	CString	m_strName;
	//}}AFX_DATA
	BOOL	m_bVertical;
	CString	m_sFont;
	int m_iSize;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPartEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEditlabel();
	void SetItemText(int nIndex, CShapePin *pShPin, int nPin);
	void DoLoadListCtrl(CString strIdent, int nTypePin);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTEDIT_H__7C95B5C3_4E18_4B24_B5FC_4003E83D2A9E__INCLUDED_)
