#if !defined(AFX_POINT3D_H__INCLUDED_)
#define AFX_POINT3D_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Point3D.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPoint3D object

class CPoint3D
{
	// Construction
public:
	CPoint3D();
	CPoint3D(int x, int y, int z);

	// Attributes
public:
	int x;
	int y;
	int z;

	// Operations
public:
	BOOL operator==(CPoint3D point);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPoint3D)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CPoint3D();

	// Generated message map functions
protected:
};

#endif
