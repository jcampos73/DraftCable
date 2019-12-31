#if !defined(AFX_CABLEEDIT_H__CC884ECA_3F7D_48E0_BE8B_27B47FBD8547__INCLUDED_)
#define AFX_CABLEEDIT_H__CC884ECA_3F7D_48E0_BE8B_27B47FBD8547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CableEdit.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CCableEdit dialog

class CCableEdit : public CDialog
{
// Construction
public:
#define _COLNUMBER_CABLEDIT		4
	//column width matrix
	int m_iColumn[_COLNUMBER_CABLEDIT];



	CCableEdit(CWnd* pParent = NULL);   // standard constructor
	CCableEdit(LPCTSTR lpcsCableFile,BOOL bConnector = FALSE,CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CCableEdit)
	enum { IDD = IDD_CABLEDIT };
	CString	m_strName;
	CListCtrl m_lcCable;
	CString	m_strFile;
	CComboBox m_cbFile;
	//}}AFX_DATA
	CString	m_strFileLast;
	int m_iType;
	int m_nColumn;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCableEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bNew;		//Used to disable SAVE button
	BOOL m_bConnector;	//Dialog behaviour is different if we are editing a cable or a connector.
	BOOL m_bModified;	//To prompt user for save.
	int m_nLen;
	//message track for debug
	MSG m_MsgLast;

	//handle of accelator table
	HACCEL m_hAccel;

	//variables for edition control
	bool m_FlagEditing;
	int m_iIndexEd;
	CEdit * m_peWire;

	CString CutString(CString &strtext);//support for fragmenting rows
	void TrimLast();
protected:
	void LoadCable();
	void SaveCable(BOOL bSaveas =FALSE);
	void SaveCable(CString strName);
	void DoPrepareCtrl();

	// Generated message map functions
	//{{AFX_MSG(CCableEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelendokFile();
	afx_msg void OnEditchangeFile();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg BOOL OnNextPrevField(UINT nCmdID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CABLEEDIT_H__CC884ECA_3F7D_48E0_BE8B_27B47FBD8547__INCLUDED_)
