#if !defined(AFX_DIALOGSAVEASDB_H__06BB6C0C_4FC7_4E79_8308_31D66E234B42__INCLUDED_)
#define AFX_DIALOGSAVEASDB_H__06BB6C0C_4FC7_4E79_8308_31D66E234B42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSaveasDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSaveasDB dialog

class CDialogSaveasDB : public CDialog
{
// Construction
public:
	CDialogSaveasDB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSaveasDB)
	enum { IDD = IDD_SAVEASDB };
	CTreeCtrl	m_tcTree;
	CString	m_Name1;
	//}}AFX_DATA
	CString m_Lib;
	CString	m_Name;
	OPENFILENAME m_ofn;//initialize the appearance of a File Open or File Save As


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSaveasDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM AddOneItem(HTREEITEM hParent, LPSTR szText, HTREEITEM hInsAfter, int iImage, BOOL bChildren, DWORD data = 0);
	HTREEITEM hHD;
	HTREEITEM hRoot;
	void AddTreeViewItems(HTREEITEM hitem = NULL);
	CTreeCtrl& GetTreeCtrl( ) const;

	// Generated message map functions
	//{{AFX_MSG(CDialogSaveasDB)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSAVEASDB_H__06BB6C0C_4FC7_4E79_8308_31D66E234B42__INCLUDED_)
