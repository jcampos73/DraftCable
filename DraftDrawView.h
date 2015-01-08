// DraftDrawView.h : interface of the CDraftDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAFTDRAWVIEW_H__13E23870_2790_476C_9195_A85C808D178F__INCLUDED_)
#define AFX_DRAFTDRAWVIEW_H__13E23870_2790_476C_9195_A85C808D178F__INCLUDED_

#include "Shape.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define DRAFTDRAW_RECTMUL_DELETE	//deletes selecting rectangle when releasing left button
#define MAX_NUM_PEN_IN_VIEW 4		//Its used to draw with a thicker pen when we increment zoom
#define SCROLL_SIZE_PAGE 100		//Scroll size for page
#define SCROLL_SIZE_LINE 10			//Scroll size for line


class CDraftDrawView : public CFolderView
{
protected: // create from serialization only
	CDraftDrawView();
	DECLARE_DYNCREATE(CDraftDrawView)

// Attributes
public:
	BOOL IsPrinting(CDC *pDC);
	CDraftDrawDoc* GetDocument();

// Operations
public:
	void ClientToSchematic(LPPOINT lpPoint ) const;	//Transform a point in client coordinates to schematic coordinates
	void SnapToGrid(LPPOINT lpPoint );				//Snap to grid. Coordinates are schematic.
	void SnapToGridFloor(LPPOINT lpPoint );			//Snap to grid. Coordinates are schematic.
	void PrepareBuffer(CDraftDrawDoc* pDoc,CDC* pDC);
	void DrawGrid(CDC *pDC,LPRECT lpRect,BOOL inBuffer = FALSE);
	void DrawGrid(CDC *pDC,LPRECT lpRectOut,LPRECT lpRectIn);
	void DrawBorder(CDC *pDC);
	CPoint Snap(CPoint point);//Snap a point to the grid (if is ennabled)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftDrawView)
	public:
	virtual void OnDraw(CDC* pDC);  //Overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnInitialUpdate(); //Called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	virtual BOOL OnCommandShape(WPARAM wParam, LPARAM lParam);

// Implementation
public:
	BOOL m_bFlagIniUpdate;		//This flag is used becouse I don't why OnInitialUpdate function is called twice

	bool m_FlagBuffer1;			//Flag to create buffer on first call to ondraw
	CDC *pDC1;					//Buffer for drawing
	CBitmap *m_bmpCanvas1;		//This is the bitmap form de DC1 context
	CDC *pDC2;					//Secondary buffer for drawing. It is not used yet
	CBitmap *m_bmpCanvas2;		//This is the bitmap form de DC2 context. It is not used yet

	CPoint m_PointLBD0;				//Point of event left button down
	CPoint m_PointPrev;				//It is for frame erasing it is not used

	//Zoom control
	CSize m_szWext;					//Zoom control: window
	CSize m_szVPext;				//Zoom control: view port
	float m_xScale;					//m_xScale=function(m_szWext,m_szVPext)
	float m_yScale;					//m_yScale=function(m_szWext,m_szVPext)

	//06/12/2004
	//This member is used for updating the grip when scroll is performed.
	//Not currebtly used.
	CPoint m_ptScrollPrev;				//Previous position of scroll

	CPen arrPen[MAX_NUM_PEN_IN_VIEW];	//Its used to draw with a thicker pen when we increment zoom

	virtual ~CDraftDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//This array it is not used but will contain selected shape to keep track of them,
	//so as never forggeting to unselect them
	CObList m_ObListSel;			//Object array for storing selected shapes (not used yet)

	BOOL m_bFlagSelNet;				//This members are for showing context menu item
	BOOL m_bFlagEditTB;				//This members are for showing context menu item
	CString m_strEditID;			//Used to edit 'TB' for example.

	int m_SeparatorHeight;			//For drawing a horizontal separation line (used in a prototype) 
	CPen PenThick;					//Creates a thick pen (not currently used, was used for separator)

	//Grid control
	BOOL m_bGrid;					//Draw grid or not flag
	BOOL m_bGridTemp;				//Used to disable grid temporally on scroll (is not used)

	BOOL m_bSaveDC;					//04/06/2005: //Used to save drawing in a jpg, bmp, stc file.
	CRect m_RectMul;				//For selecting multiple shapes
	CRect m_RectDraw;				//for drawing operations
	CRect m_RectDraw2;				//It used for drawing background when moving a shape
	CRect m_RectPrev;				//to calculate update area
	BOOL m_bForceTransfer;			//Force transfer from secondary buffer

	CPoint m_ptConnectPrev;			//Previous connection point 

	//Functions
	void DoWorldTransform(CShape *pSh,HDC hdc);
	void GetScrollRect(CRect &rectUpdate);						//Calculate scroll rect using member m_ptScrollPrev
	void DoZoomIn(CPoint point);
	void DoZoomOut(CPoint point);
	CPoint GetConnectPrev(){ return m_ptConnectPrev;};			//Get previous connection point
	void SetConnectPrev(CPoint pt){ m_ptConnectPrev=pt;	};		//Set previous connection point
	void DeleteSelected(CObject *pOb);							//Delete from selected array
	void DoPreventShpOutSchem(CShape *pSh, CRect rcSheet);

	void _DoPanMove(CPoint point);
	void _DoMappingEngine(CPoint& point);
	BOOL _DoGetConnectionTmp(CShape *pSh, CPoint point, UINT nFlags,
		BOOL& bConnectionTmp,
		int& indexPin,
		CShapeContainer** pShContConnect
		);
	//Add a new shape to stack
	void _DoAddNewShapeToStack(UINT nFlags, CPoint point);
	BOOL _DoGetConnectionTmp(LPPOINT point, CShapeContainer** pShContConnect, CShape* pSh, BOOL bConnectionTmp);
	void _DoZoom(CShape* pSh);

	void GetSheetArea(CRect& rcSheet);

// Generated message map functions
protected:
	//{{AFX_MSG(CDraftDrawView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnSnapTogrid();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEscape();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOrderBottom();
	afx_msg void OnOrderTop();
	afx_msg void OnZoomAll();
	afx_msg void OnSnapTogrid2();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnViewGraphDen();
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnRotate();
	afx_msg void OnUpdateBtEditPart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtEditRack(CCmdUI *pCmdUI);
	afx_msg void OnSelectNet();
	//afx_msg void OnChangedTab(NMFOLDERTAB* nmtab, LRESULT* pRes);
	afx_msg void OnChangedTab(NMHDR* nmtab, LRESULT* pRes);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DraftDrawView.cpp
inline CDraftDrawDoc* CDraftDrawView::GetDocument()
   { return (CDraftDrawDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTDRAWVIEW_H__13E23870_2790_476C_9195_A85C808D178F__INCLUDED_)
