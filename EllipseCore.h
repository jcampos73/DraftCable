#ifndef _ELLIPSECORE_V00
#define _ELLIPSECORE_V00


#define PI 3.1415926535897932384626433832795

void RotateAxis(CPoint& P1,CPoint& P2,int b,float &angle);
void DrawEllipse(LPPOINT lpPoints, int nCount, CPoint P1, CPoint P2,int b,float angle);

void BoundRect(CPoint P1,CPoint P2,int b,CRect& bRect);

void PlaceSelectRect(CRect **lpArrayR,CRect *lpRect,int size);



#endif