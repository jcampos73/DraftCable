#include "stdafx.h"

#include "EllipseCore.h"

#include "math.h"


//------------------------------------------------------------------------
//FUNCTION: BoundRect
//Find out bounding rectangle of ellipse given both extremes of main axis
//and value of b parameter.
void BoundRect(CPoint P1,CPoint P2,int b,CRect& bRect){
	
CPoint vector=CPoint(P2.x-P1.x,P2.y-P1.y);	

float vector_mod=sqrt((double)(vector.x*vector.x+vector.y*vector.y));



float vector_x=vector.x*b/vector_mod;
float vector_y=vector.y*b/vector_mod;
	


CPoint point1=CPoint(P1.x-vector_y,P1.y+vector_x);
CPoint point2=CPoint(P2.x+vector_y,P2.y-vector_x);


bRect=CRect(point1,point2);

bRect.NormalizeRect();



	
	
}

//------------------------------------------------------------------------
//FUNCTION: DrawEllipse
//x=a*cos alfa;
//y=a*sen alfa;
void DrawEllipse(LPPOINT lpPoints, int nCount, CPoint P1, CPoint P2,int b,float angle){

	
CPoint point_org=CPoint((P1.x+P2.x)*.5,(P1.y+P2.y)*.5);

float alfa_inc=PI/nCount;

float a=.5*sqrt((double)((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y)));


float vectora_x=(P2.x-P1.x)/(2*a);
float vectora_y=(P2.y-P1.y)/(2*a);
float vectorb_x=vectora_y;
float vectorb_y=-vectora_x;


float alfa=angle*2*PI/360;

for(int i=0;i<nCount;i++){

	lpPoints[i].x=a*cos(alfa)*vectora_x+b*sin(alfa)*vectorb_x+point_org.x;
	lpPoints[i].y=a*cos(alfa)*vectora_y+b*sin(alfa)*vectorb_y+point_org.y;


	alfa+=alfa_inc;

}



}


//------------------------------------------------------------------------
//FUNCTION: RotateAxis
//Rotate ellipse axis a number o degrees keeps value of b axis.
void RotateAxis(CPoint& P1,CPoint& P2,int b,float &angle){

//previous calc
CPoint point_org=CPoint((P1.x+P2.x)*.5,(P1.y+P2.y)*.5);

float c=.5*sqrt((double)((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y)));	
//float a=sqrt(b*b+c*c);

float alfa=angle*2*PI/360;

/*
float fdata=fabs(c*sin(alfa));
if(fdata>b){

	b=c*sin(alfa);
}
*/


float alfa_p=atan(c*tan(alfa)/b);
angle=alfa_p*360/(2*PI);

b=sin(alfa)*c/sin(alfa_p);

float a=cos(alfa)*c/cos(alfa_p);
/*
float fdata1=b*b*sin(-alfa)*sin(-alfa);
float fdata2=cos(-alfa)*cos(-alfa);
float a=sqrt((c*c-fdata1)/fdata2);
*/

float vectora_x=(P2.x-P1.x)/(2*c);
float vectora_y=(P2.y-P1.y)/(2*c);
float vectorb_x=-vectora_y;
float vectorb_y=vectora_x;


	
	P1.x=-a*cos(alfa)*vectora_x-a*sin(alfa)*vectorb_x+point_org.x;
	P1.y=-a*cos(alfa)*vectora_y-a*sin(alfa)*vectorb_y+point_org.y;
	
	P2.x=a*cos(alfa)*vectora_x+a*sin(alfa)*vectorb_x+point_org.x;
	P2.y=a*cos(alfa)*vectora_y+a*sin(alfa)*vectorb_y+point_org.y;


	
}


void PlaceSelectRect(CRect **lpArrayR,CRect *lpRect,int size){//8 free position in array

	int l=size;
	CPoint l_2=CPoint(-l*.5,-l*.5);
	CSize sl=CSize(l,l);

	CPoint p0=lpRect->TopLeft()+l_2;
	CPoint p1=CPoint(lpRect->BottomRight().x,lpRect->TopLeft().y)+l_2;
	CPoint p2=lpRect->BottomRight()+l_2;
	CPoint p3=CPoint(lpRect->TopLeft().x,lpRect->BottomRight().y)+l_2;

	CPoint p4=CPoint(lpRect->CenterPoint().x,lpRect->TopLeft().y)+l_2;
	CPoint p5=CPoint(lpRect->BottomRight().x,lpRect->CenterPoint().y)+l_2;
	CPoint p6=CPoint(lpRect->CenterPoint().x,lpRect->BottomRight().y)+l_2;
	CPoint p7=CPoint(lpRect->TopLeft().x,lpRect->CenterPoint().y)+l_2;

	for(int i=0;i<7;i++){
	if(lpArrayR[i]){

		delete(lpArrayR[i]);
	}}

	lpArrayR[0]=new CRect(p0,sl);//NW
	lpArrayR[1]=new CRect(p1,sl);//NE
	lpArrayR[2]=new CRect(p2,sl);//SE
	lpArrayR[3]=new CRect(p3,sl);//SW
	lpArrayR[4]=new CRect(p4,sl);//N
	lpArrayR[5]=new CRect(p5,sl);//E
	lpArrayR[6]=new CRect(p6,sl);//S
	lpArrayR[7]=new CRect(p7,sl);//W
}


