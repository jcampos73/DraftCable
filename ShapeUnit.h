#if !defined(_SHAPEUNIT_H_INCLUDED)
#define _SHAPEUNIT_H_INCLUDED

#include <math.h>
#include "EllipseCore.h"
#include "Shape.h"


#define _DEFAULTMAXPLINE_DRAFTCABLE		512//128//!!!magic number

#define SHAPEUNIT_PINTYPE_UNIT		0x00000000
#define SHAPEUNIT_PINTYPE_WIRE		0x00000001
#define SHAPEUNIT_PINTYPE_MASK		0x000000ff
#define SHAPEUNIT_PINTYPE_JACK		0x00000100

#define SHAPECONTAINER_CONNECTING_PIN_UNIT	0
#define SHAPECONTAINER_CONNECTING_PIN_WIRE	1

extern char **TABLE_PIN_LABEL;

/////////////////////////////////////////////////////////////////////////////
// CShapeUnit object

class CShapeUnit : public CShapeContainer
{
// Construction
public:
	CShapeUnit(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	CShapeUnit(CShapeUnit& ShUnit);//Copy contructor
	BOOL CreateNStar(LPRECT lpRect,int nCount,float percDeep =25.0);
	BOOL CreateNGear(LPRECT lpRect,int nCount,float percDeep =25.0);
	BOOL CreateNCross(LPRECT lpRect,int nCount,float percDeep =25.0);
	BOOL Create(LPRECT lpRect,int nCount,int uiShapeType);
	DECLARE_SERIAL( CShapeUnit )

// Attributes
public:
	float m_Alfa;	//degress
	float m_Beta;	//degress
	float m_d;		//relative
	float m_a1;		//relative
	float m_b1;		//relative
	float m_a2;		//relative
	float m_b2;		//relative
	//06/03/2005
	BOOL m_bFlagPartEdit;			//Flag to indicate part edition document

	CString m_sUnitName;
	int m_uiPartnumber;
	int **m_pPoints;
	int **m_pPoints0;	//original array for rescale
	//24/03/2004 Moved to CShape
	//CRect m_Rect0;		//original rectangle for rescale
	int m_PointspCount;
	label **m_pLabels;
	int m_LabelsCount;

// Operations
public:
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& operator++( );
	void Resize();
	void SetupBuffers(LPRECT rect0);
	void ReleaseBuffers();
	virtual void GetRectTemp(CRect &rect);
	virtual void GetBoundRect(CRect &rect);
	virtual void NormalizeChildShapes(CPoint ptOffset = CPoint(0, 0));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

// Implementation
public:
	//This flag should be substituted by a callback function
	BOOL m_bFlagCBLButtonDblClk;
	callback_shp1 mf_cbPatchPEdit;
	//**************************************************************************

	BOOL m_bFlagFileOpen;
	CStdioFile funit;
	CPoint * m_pPBuffer;
	static CString CutString(CString &strtext);
	CObject *LoadUnit(LPCTSTR lpUnitName,CArchive *ar=NULL);
	void Serialize( CArchive& archive );
	void SerializeGbr(CGbrioFile &gbrfile);
	void SerializeDdw(CDdwioFile &ddwfile);
	void SerializeSvg(CSvgioFile &svgfile);
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	//virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL IsTB();
	virtual ~CShapeUnit();

protected:
	void DoDraw(CDC *pDC, CRect rect);
	virtual void GetData(CStringArray& saData){
		saData.Add(m_sUnitName);
	};
	void DoRotate(float fAngle);
	void DoRotate(float fAngle, CPoint ptPivot, BOOL bUsePivot = TRUE);
	void _DoCreatePolygon(int idata, int idata1);
	void _DoProcessUmlLabels(CRecordset *rsTemp, CMapStringToPtr* mapLabelTypeToKeysvalues);
	void _DoProcessDlgPartProp(CRecordset *rsTemp, CMapStringToPtr *mapLabelTypeToKeysvalues, BOOL &bFlagModified);
	void _DoProcessModifiedShapes(BOOL bFlagModified);

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
// CShapePin object

class CShapePin : public CShapeContainer
{
// Construction
public:
	CShapePin();
	CShapePin(UINT uiPinnumber,UINT uiPos=_DRAFTDRAW_SEL_RESIZING_RECT_S,DWORD dwStyle=0,cmddeque *cmddq =NULL);
	CShapePin(CShapePin& ShPin);//Copy constructor
	DECLARE_SERIAL( CShapePin )

// Attributes
public:
	enum{ PIN_UNIT = 0, PIN_WIRE = 1 };

// Operations
public:
	BOOL Disconnect(CPoint pointr,BOOL bDiscon =FALSE);
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& operator++( );
	//Return a pointer to a shape that can be connect, but doesn´t connect anaything.
	virtual BOOL PtInRect(LPPOINT point,CShapeContainer** lpSh);
	virtual void GetRectTemp(CRect &rect);
	virtual void NormalizeChildShapes(CPoint ptOffset = CPoint(0, 0));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_TypePin;			//Index to TABLE_PIN_LABEL
	BOOL m_bJack;			//If true pin is jack, if false is plug
	BOOL m_bFixed;			//If TRUE pin is member of a unit and canno be selected
	UINT m_uiPinnumber;		//1,2,3... Pin number is generated using this number.But must be linked to CShape::m_strIdent
							//to display properly property sheets.
	UINT m_uiPos;			//Can be North, South, East, West
	DWORD m_dwStyle;		//Unit Pin or Wire pin
	CSize m_szSize;			//Connection rectangle size
	CRect m_rectConect;		//Connection rectangle. Is recalculated in function OnDraw.
	CRect m_rectPin;		//All pin rectangle. Relative to m_Rect. 

	virtual void Serialize( CArchive& archive );
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void OnDraw(CDC* pDC);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual ~CShapePin();

	// Generated message map functions
protected:
	void RotatePin();
	void _DoCreateRectUnitPin(CPoint& point, CRect& rect1, CRect& rect2, CString& strlabel1, CString& strlabel2);
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeWire object

class CShapeWire : public CShapeContainer
{
// Construction
public:
	CShapeWire(LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	DECLARE_SERIAL( CShapeWire )

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
	UINT m_uiPinnumber;
	void RepositionPins();		//This function can be avoided if pins position does not
								//update on each redraw.
	virtual void Serialize( CArchive& archive );
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void CShapeWire::SerializeDdw(CDdwioFile &ddwfile);
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	//virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual ~CShapeWire();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeLabel object

class CShapeLabel : public CShape
{
// Construction
public:
	CShapeLabel(label *lpLbl =NULL,LPRECT lpRect=NULL,UINT nId=0,cmddeque *cmddq =NULL);
	DECLARE_SERIAL( CShapeLabel )
	BOOL Create(LPRECT lpRect,LPCTSTR lpszText,int nSize,BOOL bVer);

// Attributes
public:
	label m_Label;

// Operations
public:
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& operator++( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void Serialize( CArchive& archive );
	virtual void SerializeXml(CXMLArchive& archive);
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeSvg(CSvgioFile &svgfile);
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	//virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual ~CShapeLabel();
	void SetModified();

protected:
	BOOL m_bIni;
	//This flag is used when deserializing from XML formats, that don't store bounding rect
	//Default value is false
	BOOL m_bResizeOnFirstDraw;
	BOOL m_bOffsetAfterResize;
	virtual void GetData(CStringArray& saData){
		saData.Add(*m_Label.slabel);
		CString str;
		str.Format(_T("%i"), m_Label.iSize);
		saData.Add(str);
		str.Format(_T("%i"), m_Label.bver);
		saData.Add(str);
	};

	// Generated message map functions
protected:
	void RotateLabel();
	//Auto resize to fix the text
	void ResizeAuto(CDC *pDC);
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeFrmRect object

class CShapeFrmRect : public CShape
{
// Construction
public:
	CShapeFrmRect(LPRECT lpRect =NULL,UINT nId =0,cmddeque *cmddq =NULL);
	DECLARE_SERIAL( CShapeFrmRect );

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
	//virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeDdw(CDdwioFile &ddwfile);

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeFrmRect();

	// Generated message map functions
protected:
	CPen m_Pen;
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CShapeJunction object

class CShapeJunction : public CShapeContainer
{
// Construction
public:
	CShapeJunction();
	DECLARE_SERIAL(CShapeJunction)

// Attributes
public:

// Operations
public:
	virtual CShape& operator=( const CShape& Sh );
	virtual CShape& operator++( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShape)
	//}}AFX_VIRTUAL

// Implementation
public:
	//virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeDdw(CDdwioFile &ddwfile);

	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeJunction();

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
// CShapeBus object

class CShapeBus : public CShapePolyline
{
// Construction
public:
	CShapeBus(LPRECT lpRect =NULL,UINT nId =0,cmddeque *cmddq =NULL);
	DECLARE_SERIAL( CShapeBus )

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
	virtual void SerializeGbr(CGbrioFile &gbrfile);
	virtual void SerializeDdw(CDdwioFile &ddwfile);
	virtual void OnDraw(CDC* pDC);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CShapeBus();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShape)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	/*
	DECLARE_MESSAGE_MAP()
	*/
};

#endif