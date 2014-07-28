//{{AFX_INCLUDES()
#include "datagrid.h"
#include "adodc.h"
//}}AFX_INCLUDES
#if !defined(AFX_DIALOGPARTPROP_H__E2B96B11_3CA3_44CE_8329_7863A2FA05AA__INCLUDED_)
#define AFX_DIALOGPARTPROP_H__E2B96B11_3CA3_44CE_8329_7863A2FA05AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPartProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPartProp dialog

class CDialogPartProp : public CDialog
{
// Construction
public:
	CDialogPartProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPartProp)
	enum { IDD = IDD_PARTPROP };
	CDataGrid	m_grid;
	CAdodc	m_adoDC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPartProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPartProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnErrorAdodc1(long ErrorNumber, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* fCancelDisplay);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPARTPROP_H__E2B96B11_3CA3_44CE_8329_7863A2FA05AA__INCLUDED_)
