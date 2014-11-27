// PartPView.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PartPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartPView

IMPLEMENT_DYNCREATE(CPartPView, CView)

CPartPView::CPartPView()
{
	m_pShape=NULL;
}

CPartPView::~CPartPView()
{
	if(m_pShape){

		delete(m_pShape);
	}

}


BEGIN_MESSAGE_MAP(CPartPView, CView)
	//{{AFX_MSG_MAP(CPartPView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartPView drawing

void CPartPView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if(m_pShape){

		//MAP MODE
		int prev_mapm=pDC->SetMapMode(MM_ANISOTROPIC);

		CRect rect;
		GetClientRect(&rect);

		//Control for zoom - operation
		float scale_x=m_pShape->m_Rect.Width()+rect.Width();
		scale_x/=rect.Width();
		float scale_y=m_pShape->m_Rect.Height()+rect.Height();
		scale_y/=rect.Height();
		int scale=(scale_x>scale_y?scale_x:scale_y);

		CSize m_szWext=CSize(scale,scale);
		CSize m_szVPext=CSize(1,1);


		//Control for zoom + operation
		if(scale==1){


			scale_x=rect.Width();
			scale_x/=m_pShape->m_Rect.Width();
			scale_y=rect.Height();
			scale_y/=m_pShape->m_Rect.Height();
			scale=(scale_x>scale_y?scale_y:scale_x);


			m_szWext=CSize(1,1);
			m_szVPext=CSize(scale,scale);

			//Zoomed shape are sensible to offsets in origin:
			//shape goes out of preview screen
			m_pShape->m_Rect.OffsetRect(-m_pShape->m_Rect.TopLeft());

		}

		pDC->SetWindowExt(m_szWext);
		pDC->SetViewportExt(m_szVPext);



		m_pShape->OnDraw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPartPView diagnostics

#ifdef _DEBUG
void CPartPView::AssertValid() const
{
	CView::AssertValid();
}

void CPartPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPartPView message handlers


int CPartPView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// This method needs to be overridden to prevent an assertion
	// As described here
	// http://www.codeguru.com/cpp/w-d/dislog/article.php/c5007/Creating-a-View-on-a-Dialog.htm
	return MA_ACTIVATE;
}
