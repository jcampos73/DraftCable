#if !defined(AFX_SHAPE_H__6C22E132_ED5F_4D1B_8CC7_BA7023668556__INCLUDED_)
#define AFX_SHAPE_H__6C22E132_ED5F_4D1B_8CC7_BA7023668556__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Shape.h : header file
//

#include "GbrioFile.h"
#include "SvgioFile.h"
#include "OMParser.h"		//Object model parser

#include "XMLArchive.h"

#define _DRAFTDRAW_MODE_SEL					0		//When we select a shape and we don't realease the left button
#define _DRAFTDRAW_MODE_DRW					1


#define _DRAFTDRAW_SEL_SIZING_RECT			0		//Normal draw
//#define _DRAFTDRAW_SEL_SIZING_LINE			1
#define _DRAFTDRAW_SEL_MOVING_RECT			2		//Place sizing handles
#define _DRAFTDRAW_SEL_RESIZING_RECT		3

//maximum number of markers to indicate selection
#define	_DRAFTDRAW_SEL_MARKERMAX			16
#define	_DRAFTDRAW_SEL_SIZE					12
#define	_DRAFTDRAW_SEL_SIZEREL				1

#define _DRAFTDRAW_SEL_RESIZING_RECT_NW		0
#define _DRAFTDRAW_SEL_RESIZING_RECT_NE		1
#define _DRAFTDRAW_SEL_RESIZING_RECT_SE		2
#define _DRAFTDRAW_SEL_RESIZING_RECT_SW		3
#define _DRAFTDRAW_SEL_RESIZING_RECT_N		4
#define _DRAFTDRAW_SEL_RESIZING_RECT_E		5
#define _DRAFTDRAW_SEL_RESIZING_RECT_S		6
#define _DRAFTDRAW_SEL_RESIZING_RECT_W		7

enum{ VERTEX_TRIANGLE = 3, VERTEX_SQUARE = 4, VERTEX_PENTAGON = 5, VERTEX_HEXAGON = 6, VERTEX_OCTOGON = 8 };

//This function protype s used to communicate with a parser to store shapes commans
//if WM_MOUSEMOVE produces any change a flag is activated, then
//WM_LBUTTONUP produces the command to be stored.
typedef int (*parserfunc)(std::string strComm,std::string strCommUndo);  // funcptr is synonym for "pointer to function returning int"

/////////////////////////////////////////////////////////////////////////////
// Struct DCCurveData_tag
//
// This structure defines a curve segment.
//
// La segunda estructura de puntos de control pPointCtrl0 almacena los puntos para el rescalado.
// Utilizando esta estructura y la pareja de miembros m_Rect/m_Rect0 de la shape donde esté incrustada
// la estructura de curva se puede rescalar esta última.
typedef struct DCCurveData_tag {
	unsigned int	nOrder;					// Order of the curve: 2->polyline,4->bezier
	// Number of control point >= order
	unsigned int	nPoint;					// Number of control points
	//Point structure is composed of two long,long values.
	LPPOINT		pPointCtrl;				// Array of control points
	LPPOINT		pPointCtrl0;				// Original array (for rescale)
	//Number of knots = number control points + order
	float		*pfKnots;					// Array of knots

	void DCCurveDataFromLine(POINT point1,POINT point2){
		nOrder=2;
		nPoint=2;
		pPointCtrl=new POINT[2];
		pPointCtrl0=new POINT[2];
		pPointCtrl[0]=point1;
		pPointCtrl[1]=point2;
		pPointCtrl0[0]=point1;
		pPointCtrl0[1]=point2;
	};

	void DCCurveDataFromBezier(POINT point1,POINT pointctrl1,POINT pointctrl2,POINT point2){
		nOrder=4;
		nPoint=4;
		pPointCtrl=new POINT[4];
		pPointCtrl0=new POINT[4];
		pPointCtrl[0]=point1;
		pPointCtrl[1]=pointctrl1;
		pPointCtrl[2]=pointctrl2;
		pPointCtrl[3]=point2;
		pPointCtrl0[0]=point1;
		pPointCtrl0[1]=pointctrl1;
		pPointCtrl0[2]=pointctrl2;
		pPointCtrl0[3]=point2;
	};

	~DCCurveData_tag(){
		delete(pPointCtrl);
		delete(pPointCtrl0);
	};

} DCCurveData_t;

/////////////////////////////////////////////////////////////////////////////
// CPoint3D object

class CPoint3D
{
// Construction
public:
	CPoint3D();
	CPoint3D(int x,int y,int z);

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
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPoint3D();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////
// CShape object

class CShape : public CObject
{
// Construction
public:
	CShape(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	CShape(CShape& Sh);//Copy constructor
	/*DECLARE_DYNCREATE( CShape )*/
	DECLARE_SERIAL( CShape )

// Attributes
public:
	//--------------------------------------------------------------------------
	//This angle has not been implemented becouse
	//memory buffer for painting movement does not work fine.
	float m_fAngle;				//Rotation angle of shape

	FLOAT m_eM11;				//Cosine of rotation angle
	FLOAT m_eM12;				//Sine of the rotation angle
	FLOAT m_eM21;				//Negative sine of the rotation angle
	FLOAT m_eM22;				//Cosine of rotation angle
	//--------------------------------------------------------------------------
	BOOL m_bConect;
	//Get Status of shape
	DWORD GetStatus(){return MAKELONG(m_Mode,m_TypeSelect);};
	//Set Status of shape
	void SetStatus(DWORD dwStatus){m_Mode=LOWORD(dwStatus);m_TypeSelect=HIWORD(dwStatus);};
	//void SetStatus(ddcStatus_t Status);
	virtual BOOL IsPoly(){return FALSE;};//Is a polyshape: polylines, polycurves, etc.

// Operations
public:
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& operator++( );
	virtual void Select(BOOL bSelect=TRUE);//select shape
	virtual BOOL IsSelected();//Is shape selected?
	virtual void Unselect();//unselect shape
	virtual void Move(int x,int y,BOOL bAbsolute/*=FALSE*/);//Move shape
	virtual void Move(int left,int top,int right,int bottom,BOOL bAbsolute=FALSE);//Move & Scale shape
	virtual void GetRectTemp(CRect &rect);//For drawing the part of the polyline that is not selected
	virtual void GetRectUpdatePlace(CRect &rect);//Rectangle to redraw when placing the shape
	virtual void Flip(float fAngle);//Angle of flip axis in degrees: 0.0 = horizontal, 90.0 = vertical, etc.
	virtual void Rotate(float fAngle);//+Angle->counter clockwise ; -Angle->counter clockwise
	BOOL IsEmpty( );
	void DoConntoId();
	void DoPasteConnections(std::map<int,int> *pmapIdtoId,mapShapeIdtoObj_t *pmapIdtoObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strIdent;//shape identificator, wire number for example
	CString m_strTypeIdent;//shape type identificator, wire type for example

	BOOL m_bNoResize;//this flag is set when shape does not to be resized
	BOOL m_bLineResize;//flag for line type shapes
	//07/12/2004
	BOOL m_bAllowsPartialDraw;//flag for shapes that are drawn in segments: polylines, buses.
	BOOL m_bDrawDashedRect;//Used by default when sizing a shape. Default value is TRUE

	//pointer to a child shape to stablish a conection
	//---------------------------------------------------------------------
	CShape *m_pshConn;
	CShape *m_pshChildConn;
	int m_uiConn;
	int m_uiChildConn;
	//---------------------------------------------------------------------

	/*parserfunc pfParser;*/
	cmddeque *pcmdDeque;

	virtual void Serialize( CArchive& archive );
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);

	HCURSOR * m_pCursorArray;
	int m_Mode;
	int m_TypeSelect;
	CRect *m_MarkerArray[_DRAFTDRAW_SEL_MARKERMAX];
	int m_VectorSelect;

	CPoint m_PointOrg;
	CPoint3D m_VectorProyect;
	CPoint m_Point;
	int m_Height;
	int m_Type;
	//Added 18/04/2004.
	//This member was added to write connections to file for Units and Wires.
	//Having and unique shape identifier (within document) may be useful.
	int m_uiShapeId;
	//Added 27/11/2004
	//For parsing commands.
	int m_uiShapeType;//=ddcShape,ddcShapeEllipse,ddcShapeRectangle...
	//05/12/2004
	BOOL m_bTransparent;	//If TRUE (default) shape is not filled.
	COLORREF m_crFill;		//Color to fill the shape.
	//18/09/2014
	COLORREF m_crFillBgnd;	//Color to fill the shape background (for gradients).
	//24/03/2005
	CString m_sVer;			//.dd1 file version
	float m_fVer;			//.dd1 file version as a decimal number

	void SetOrg(CPoint org);
	void SetProyect(CPoint3D point);
	void SetProyect(int x,int y,int z);

// Generated message map functions
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void OnDraw(CDC* pDC);
	virtual void OnRotate(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CRect rect);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)/*=0*/;
	virtual CShape *SetConnection(CShape *pSh=NULL,int index=0);
	virtual CObject *LoadUnit(LPCTSTR lpUnitName,CArchive *ar=NULL);
	virtual void GetSizingRect( LPRECT lpRect ) const;
	virtual void GetSizingRect( CRect &rect ) const;
	CRect m_Rect;
	CRect m_RectLast;
	//28/09/2005
	//CRect m_RectTemp;
	//15/05/2005
	CRect m_RectAux;
	CRect m_RectLastAux;

	virtual ~CShape();
	void DoWorldTransform(HDC hdc);

protected:
	//24/03/2004
	CRect m_Rect0;	
	virtual void GetData(CStringArray& saData){return;};
	//Do filling of shape: solid, gradient...
	virtual void DoFill(CDC* pDC, LPRECT lpRect =NULL);
	virtual void DoFill(CDC* pDC, void* gfxPath, CPoint point1, CPoint point2);
	//Split rectangle in two rectangles of same size, following vector
	static int SplitRect(CRect rect, LPPOINT vect, CRect(&arrRect)[2]);

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

typedef CShape* LPSHAPE;

/////////////////////////////////////////////////////////////////////////////
// CShapeRect object

class CShapeRect : public CShape
{
// Construction
public:
	CShapeRect(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapeRect)*/
	DECLARE_SERIAL( CShapeRect )

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
	BOOL m_bGroup;
	virtual void Serialize( CArchive& archive );
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeRect();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeEllipse object

class CShapeEllipse : public CShape
{
// Construction
public:
	CShapeEllipse(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapeEllipse)*/
	DECLARE_SERIAL( CShapeEllipse )

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
	void Serialize( CArchive& archive );
	void SerializeDdw(CDdwioFile &ddwfile);
	void SerializeGbr(CGbrioFile &gbrfile);
	virtual void OnDraw(CDC* pDC);
	//void OnMouseMove(UINT nFlags, CPoint point);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeEllipse();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};


/////////////////////////////////////////////////////////////////////////////
// CShapeLine object

class CShapeLine : public CShape
{
// Construction
public:
	CShapeLine(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapeLine)*/
	DECLARE_SERIAL( CShapeLine )

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
	virtual void Serialize( CArchive& archive );
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);
	virtual void OnDraw(CDC* pDC);
	//void OnMouseMove(UINT nFlags, CPoint point);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeLine();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeRule object

class CShapeRule : public CShape
{
// Construction
public:
	CShapeRule();
	DECLARE_DYNCREATE(CShapeRule)

// Attributes
public:
	double m_dfD;
	float m_fScale;
	int m_offset1;
	int m_offset2;
	int m_offset3;
	int m_offset4;
	float m_alfa;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnDraw(CDC* pDC);
	virtual ~CShapeRule();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeArc object

class CShapeArc : public CShape
{
// Construction
public:
	CShapeArc(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	DECLARE_SERIAL( CShapeArc )
	//Create a allipse arc from an array of points
	BOOL Create(LPPOINT lpPoint1, LPPOINT lpPoint2, BOOL bGdiplus = FALSE);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Operations
public:
	virtual void Serialize(CArchive& archive);
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	CPoint m_Point2;
	int b;
	virtual void OnDraw(CDC* pDC);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual ~CShapeArc();

// Implementation
protected:
	bool m_bGdiplus;
	//Start and end of ellipse arc
	float m_angleStart;
	float m_angleSweep;
	//Members used to rotate angle
	float m_Alfap;
	float m_Alfa;
	CPoint m_P1;
	CPoint m_P2;

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeContainer object

class CShapeContainer : public CShape
{
// Construction
public:
	CShapeContainer(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	CShapeContainer(CShapeContainer& ShCont);//Copy contructor
	/*DECLARE_DYNCREATE(CShapeContainer)*/
	DECLARE_SERIAL( CShapeContainer )

// Attributes
public:
	int m_ShapeNumber;	//Number of CShapes contained
	CObArray m_obarrShapearr;
	CIntArray m_iaParameter;
	CShape *GetParent();
	void SetParent(CShape *);
	POSITION GetFirstConnectedItemPosition() const;//Get first connected pin
	int GetNextConnectedItem(POSITION& pos) const;
	int GetIndex();
	CShape *GetFirstChainItem() const;//Get first element of chain
	void GetNextChainItem(CShape *& pSh,CShape *pShPrev) const;
	virtual BOOL IsPoly(){return FALSE;};//Is a polyshape: polylines, polycurves, etc.

// Operations
public:
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& CShapeContainer::operator++( );
	virtual CShape*& operator []( int nIndex );
	virtual CShape* operator []( int nIndex ) const;
	virtual void GetRectTemp(CRect &rect);
	virtual void GetRectUpdatePlace(CRect &rect);
	//Return a pointer to a shape that can be connect, but doesn´t connect anaything.
	virtual BOOL PtInRect(LPPOINT point,CShapeContainer** lpSh);
	//Check if a shape container is connected
	BOOL IsConnected();
	//Link two shapes.
	void LinkShapes(CShapeContainer* lpShCont1,
								CShapeContainer* lpShCont2);
	//Unlink two shapes.
	void UnlinkShapes(CShapeContainer* lpShCont1,
									CShapeContainer* lpShCont2);
	void DoConntoId(BOOL bOwn =FALSE,BOOL bChild =TRUE);
	void DoPasteConnections(std::map<int,int> *pmapIdtoId,mapShapeIdtoObj_t *pmapIdtoObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	CShape *m_pShParent;
	CFont m_fontFont1;
	//BOOL m_bConect;			//Shape is connected
	BOOL m_bConnectMake;	//Must be set to TRUE before calling OnLButtonDown to try to connect

	//data to define unit identifier
	BOOL	m_bVertical;
	CString	m_sFont;
	int m_iSize;

	virtual void Serialize( CArchive& archive );
	virtual void SerializeGbr(CGbrioFile &gbrfile)/*=0*/;
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void OnDraw(CDC* pDC);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual CShape *SetConnection(CShape *pSh=NULL,int index=0);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){CShape::OnKeyDown(nChar,nRepCnt,nFlags);};
	virtual void GetSizingRect( LPRECT lpRect ) const;
	virtual ~CShapeContainer();

protected:
	virtual void GetData(CStringArray& saData){return;};

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeBoard object

class CShapeBoard : public CShapeContainer
{
// Construction
public:
	CShapeBoard();

// Attributes
public:
	float m_Alfa;	//degress
	float m_Beta;	//degress
	float m_d;		//relative

	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SerializeGbr(CGbrioFile &gbrfile);
	virtual void OnDraw(CDC* pDC);
	//virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual ~CShapeBoard();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapePolyline object

class CShapePolyline : public CShapeContainer
{
// Construction
public:
	CShapePolyline(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapePolyline)*/
	DECLARE_SERIAL( CShapePolyline )
	//Create a polyline from an array of points
	BOOL Create(LPPOINT lpPoints, int nCount);
	//Create a rectangle
	BOOL Create(LPRECT lpRect);
	//Creates a polygon
	BOOL Create(LPRECT lpRect,int nCount);
	BOOL CreateNStar(LPRECT lpRect,int nCount,float percDeep =25.0);
	BOOL CreateNGear(LPRECT lpRect,int nCount,float percDeep =25.0);
	BOOL CreateNCross(LPRECT lpRect,int nCount,float percDeep =25.0);

// Attributes
public:
	CRect m_Rect0;
	CDWordArray m_dwarrPointarr0;
	CDWordArray m_dwarrPointarr;
	virtual BOOL IsPoly(){return TRUE;};//Is a polyshape: polylines, polycurves, etc.

// Operations
public:
	int GetPoints(LPPOINT lpPoints,int nCount);
	int GetSize();
	virtual void GetRectTemp(CRect &rect);
	//Get update rectangle when drawing a poly curve (when we place a segment, previous segment is changed)
	virtual void GetRectUpdatePlace(CRect &rect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Serialize( CArchive& archive );
	virtual void SerializeXml(CXMLArchive& archive);
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)/*=0*/;
	virtual ~CShapePolyline();

protected:
	BOOL m_bClosed;//Set to true if the polyline is closed
	virtual void GetData(CStringArray& saData){
		for(int i=0;i<m_dwarrPointarr.GetSize();i++){
			CString strAux;
			strAux.Format("%i",LOWORD(m_dwarrPointarr[i]));
			saData.Add(strAux);
			strAux.Format("%i",HIWORD(m_dwarrPointarr[i]));
			saData.Add(strAux);
		}
	};

	// Generated message map functions
protected:
	float CreatePointArray(LPRECT lpRect,LPPOINT ptArray,int nCount,float degOffset =0.0,BOOL bAlign =TRUE);
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeZoomRect object

class CShapeZoomRect : public CShape
{
// Construction
public:
	CShapeZoomRect();
	DECLARE_DYNCREATE(CShapeZoomRect)

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
	BOOL m_bGroup;

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeZoomRect();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapePicture object

class CShapePicture : public CShape
{
// Construction
public:
	CShapePicture(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapePicture)*/
	DECLARE_SERIAL( CShapePicture )

// Attributes
public:
	CString m_sPath;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	virtual void Serialize( CArchive& archive );
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bGroup;
	LPPICTURE gpPicture;

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	void ChooseFile(HWND hWnd);
	void LoadPictureFile(LPCTSTR szFile);
	virtual ~CShapePicture();

protected:
	virtual void GetData(CStringArray& saData){
		saData.Add(m_sPath);
	};

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeCurve object

class CShapeCurve : public CShapePolyline
{
// Construction
public:
	CShapeCurve(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapeCurve)*/
	DECLARE_SERIAL( CShapeCurve )

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//virtual void Serialize( CArchive& archive );
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeCurve();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapePolyArc object

class CShapePolyArc : public CShapePolyline
{
// Construction
public:
	CShapePolyArc(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	/*DECLARE_DYNCREATE(CShapeCurve)*/
	DECLARE_SERIAL( CShapePolyArc )

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//virtual void Serialize( CArchive& archive );
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapePolyArc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPE_H__6C22E132_ED5F_4D1B_8CC7_BA7023668556__INCLUDED_)
