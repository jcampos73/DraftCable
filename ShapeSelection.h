#if !defined(_SHAPESELECTION_H_INCLUDED)
#define _SHAPESELECTION_H_INCLUDED

//#include "stdafx.h"
#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////
// CShapeSelection object

class CShapeSelection : public CShapeRect
{
	// Construction
public:
	CShapeSelection(LPRECT lpRect = NULL, UINT nId = 0, cmddeque *cmddq = NULL);
	DECLARE_SERIAL(CShapeSelection)

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual void OnDraw(CDC* pDC);
	virtual ~CShapeSelection();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

#endif