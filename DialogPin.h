#if !defined(AFX_DIALOGPIN_H__5ABD352D_FBE0_433C_9845_4D29CCC1ED29__INCLUDED_)
#define AFX_DIALOGPIN_H__5ABD352D_FBE0_433C_9845_4D29CCC1ED29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPin dialog

class CDialogPin : public CDialog
{
// Construction
public:
	int m_TypePin;
	int m_iShape;
	char m_Str[256];
	CDialogPin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPin)
	enum { IDD = IDD_PIN };
	CComboBox	m_cbType;
	int		m_iShapeType;
	CString	m_stNumber;
	BOOL	m_bJack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPIN_H__5ABD352D_FBE0_433C_9845_4D29CCC1ED29__INCLUDED_)
