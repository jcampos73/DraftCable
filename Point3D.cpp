#include "stdafx.h"
#include "Point3D.h"

/////////////////////////////////////////////////////////////////////////////
// Point3D
CPoint3D::CPoint3D()
{
	x = 0;
	y = 0;
	z = 0;
}


CPoint3D::CPoint3D(int x1, int y1, int z1)
{
	x = x1;
	y = y1;
	z = z1;
}

CPoint3D::~CPoint3D()
{
}

BOOL CPoint3D::operator==(CPoint3D point){

	if ((x != point.x) ||
		(y != point.y) ||
		(z != point.z)){


		return FALSE;
	}


	return TRUE;
}