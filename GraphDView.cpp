// GraphDView.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "GraphDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDView

IMPLEMENT_DYNCREATE(CGraphDView, CView)

CGraphDView::CGraphDView()
{
}

CGraphDView::~CGraphDView()
{
}


BEGIN_MESSAGE_MAP(CGraphDView, CView)
	//{{AFX_MSG_MAP(CGraphDView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDView drawing

void CGraphDView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	SIZE sz=m_pShArray->GetSize();
	CRect rect;
	GetClientRect(rect);
	int cell_width=rect.Width()/sz.cx;
	int cell_height=rect.Height()/sz.cy;

	int cell_y=0;
	for(int j=0;j<sz.cy;j++){
		int cell_x=0;
		for(int i=0;i<sz.cx;i++){

			CRect rect(cell_x,cell_y,cell_x+cell_width,cell_y+cell_height);
			int nCount=m_pShArray->GetCell(j,i).GetCount();
			if(nCount){

				COLORREF color=RGB(nCount*128/10,128-nCount*128/10,0);
				pDC->FillRect(rect,&CBrush(color));
				CString str;str.Format("%i",nCount);
				COLORREF prevColor=pDC->SetBkColor(color);
				pDC->DrawText(str,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				pDC->SetBkColor(prevColor);
			}
			else{

				pDC->Rectangle(rect);
			}
			cell_x+=cell_width;
		}
		cell_y+=cell_height;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CGraphDView diagnostics

#ifdef _DEBUG
void CGraphDView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphDView message handlers
