#if !defined(AFX_GRAPHDVIEW_H__841621EF_CE3C_4F9F_A779_634578818A7F__INCLUDED_)
#define AFX_GRAPHDVIEW_H__841621EF_CE3C_4F9F_A779_634578818A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphDView view

class CGraphDView : public CView
{
protected:
	CGraphDView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGraphDView)

// Attributes
public:
	CShArray *m_pShArray;		//Pointer to shape array

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGraphDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphDView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDVIEW_H__841621EF_CE3C_4F9F_A779_634578818A7F__INCLUDED_)
