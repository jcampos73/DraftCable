#if !defined(AFX_DIALOGGRAPHDEN_H__109A2393_FEC8_4111_A6E8_31D398503896__INCLUDED_)
#define AFX_DIALOGGRAPHDEN_H__109A2393_FEC8_4111_A6E8_31D398503896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GraphDView.h"


// DialogGraphDen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGraphDen dialog

class CDialogGraphDen : public CDialog
{
// Construction
public:
	CDialogGraphDen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogGraphDen)
	enum { IDD = IDD_GRAPH_DEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGraphDView *m_pGDView;
	CShArray *m_pShArray;		//Pointer to shape array


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGraphDen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogGraphDen)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGRAPHDEN_H__109A2393_FEC8_4111_A6E8_31D398503896__INCLUDED_)
