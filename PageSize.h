#if !defined(AFX_PAGESIZE_H__6B9C97B2_817A_4DE4_8B88_8AE9B09C3E59__INCLUDED_)
#define AFX_PAGESIZE_H__6B9C97B2_817A_4DE4_8B88_8AE9B09C3E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSize dialog

class CPageSize : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSize)

// Construction
public:
	int ChangeSize(int nSize);
	CSize m_szSize;
	CPageSize();
	~CPageSize();

// Dialog Data
	//{{AFX_DATA(CPageSize)
	enum { IDD = IDD_PAGESIZE };
	UINT	m_nHeight;
	UINT	m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSize)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioA3();
	afx_msg void OnRadioA4();
	afx_msg void OnRadioCustom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESIZE_H__6B9C97B2_817A_4DE4_8B88_8AE9B09C3E59__INCLUDED_)
