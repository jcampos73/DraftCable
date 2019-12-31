// ButtonStates.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonStates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonStates

CButtonStates::CButtonStates()
{
	m_uStyle = DFCS_BUTTONPUSH;
}

CButtonStates::~CButtonStates()
{
}


BEGIN_MESSAGE_MAP(CButtonStates, CButton)
	//{{AFX_MSG_MAP(CButtonStates)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE( BM_SETCHECK, OnSetCheck )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonStates message handlers

void CButtonStates::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
   //UINT uStyle = DFCS_BUTTONPUSH;
   int x=0,y=0;

   // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

   // If drawing selected, add the pushed style to DrawFrameControl.
   if (lpDrawItemStruct->itemState & ODS_SELECTED && !(m_uStyle & DFCS_PUSHED) ){
      m_uStyle |= DFCS_PUSHED;
		x=::GetSystemMetrics(SM_CXEDGE);
		y=::GetSystemMetrics(SM_CYEDGE);
   }

   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
      DFC_BUTTON, m_uStyle);
/*
   // Get the button's text.
   CString strText;
   GetWindowText(strText);

   // Draw the button text using the text color red.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255,0,0));
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
      &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
*/

   HICON hicon=GetIcon();

	::DrawState(
	  lpDrawItemStruct->hDC,                     // handle to device context
	  NULL,                  // handle to brush
	  NULL,  // callback function
	  (LPARAM)hicon,                // image information
	  0,                // more image information
	  x,                       // horizontal location
	  y,                       // vertical location
	  0,                      // image width
	  0,                      // image height
	  DST_ICON                 // image type and state
	);

}

void CButtonStates::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnLButtonUp(nFlags, point);
}

void CButtonStates::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int nIDFirstButton=GetDlgCtrlID();//IDC_BUTTON_ICONBIG;//GetDlgCtrlID();
	int nIDLastButton=GetDlgCtrlID();//IDC_BUTTON_DETAILS;//GetDlgCtrlID();
	int nIDCheckButton=GetDlgCtrlID();



	HWND hwnd=m_hWnd;

	for(int i=0;i<2;i++){


		HWND hwndcur=m_hWnd;

		do{

			hwndcur=::GetNextDlgGroupItem(
			  GetParent()->m_hWnd,       // handle to dialog box
			  hwndcur,       // handle to control
			  i==0   // direction flag
			  );

			int idata=::GetDlgCtrlID(hwndcur);

			if(idata<nIDFirstButton){
				nIDFirstButton=idata;
			}
			else if(idata>nIDLastButton){
				nIDLastButton=idata;
			}


		}while(hwnd!=hwndcur);

	}






	BOOL bResult=::CheckRadioButton(
	  GetParent()->m_hWnd,          // handle to dialog box
	  nIDFirstButton, // identifier of first button in group
	  nIDLastButton,  // identifier of last button in group
	  nIDCheckButton  // identifier of button to select
	);
	
	CButton::OnLButtonDown(nFlags, point);
}

LRESULT CButtonStates::OnSetCheck(WPARAM wParam, LPARAM lParam){


	SetCheck(wParam==BST_CHECKED);


	return 0;
}

void CButtonStates::SetCheck( int nCheck )
{

	UINT uStylePrev=m_uStyle;

	switch(nCheck){

	case 1:
		m_uStyle |= DFCS_PUSHED;

		break;

	case 0:
		m_uStyle &= ~DFCS_PUSHED;
		break;


	}

	if(m_uStyle!=uStylePrev){

		Invalidate();
	}

}
