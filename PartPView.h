#if !defined(AFX_PARTPVIEW_H__95C0DFFB_2B02_4B53_9816_58F5CD825BD6__INCLUDED_)
#define AFX_PARTPVIEW_H__95C0DFFB_2B02_4B53_9816_58F5CD825BD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartPView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPartPView view

class CPartPView : public CView
{
protected:
	CPartPView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPartPView)

// Attributes
public:
	CShape *m_pShape;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartPView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPartPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPartPView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTPVIEW_H__95C0DFFB_2B02_4B53_9816_58F5CD825BD6__INCLUDED_)
