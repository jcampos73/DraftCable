#if !defined(AFX_DIALOGDEBUGSHPINFO_H__4F427680_9A3A_4AB3_BC70_BCD685B3C449__INCLUDED_)
#define AFX_DIALOGDEBUGSHPINFO_H__4F427680_9A3A_4AB3_BC70_BCD685B3C449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDebugShpInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDebugShpInfo dialog

class CDialogDebugShpInfo : public CDialog
{
// Construction
public:
	CDialogDebugShpInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDebugShpInfo)
	enum { IDD = IDD_DEBUG_SHPINFO };
	CString	m_sText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDebugShpInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDebugShpInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDEBUGSHPINFO_H__4F427680_9A3A_4AB3_BC70_BCD685B3C449__INCLUDED_)
