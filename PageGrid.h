#if !defined(AFX_PAGEGRID_H__E80E349A_D2C3_44F8_8B66_697B2159397C__INCLUDED_)
#define AFX_PAGEGRID_H__E80E349A_D2C3_44F8_8B66_697B2159397C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageGrid dialog

class CPageGrid : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGrid)

// Construction
public:
	CPageGrid();
	~CPageGrid();

// Dialog Data
	//{{AFX_DATA(CPageGrid)
	enum { IDD = IDD_PAGEGRID };
	UINT	m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGrid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGrid)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGRID_H__E80E349A_D2C3_44F8_8B66_697B2159397C__INCLUDED_)
