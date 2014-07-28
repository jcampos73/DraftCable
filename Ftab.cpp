////////////////////////////////////////////////////////////////
// 1999 Microsoft Systems Journal
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "stdafx.h"
#include "ftab.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CXOFFSET = 8;		// defined pitch of trapezoid slant
const int CXMARGIN = 2;		// left/right text margin
const int CYMARGIN = 1;		// top/bottom text margin
const int CYBORDER = 1;		// top border thickness

//////////////////
// Compute the the points, rect and region for a tab.
// Input x is starting x pos.
//
int CFolderTab::ComputeRgn(CDC& dc, int x)
{
	m_rgn.DeleteObject();

	CRect& rc = m_rect;
	rc.SetRectEmpty();

	// calculate desired text rectangle
	dc.DrawText(m_sText, &rc, DT_CALCRECT);
	rc.right  += 2*CXOFFSET + 2*CXMARGIN;						// add margins
	rc.bottom = rc.top + GetSystemMetrics(SM_CYHSCROLL);	// ht = scrollbar ht
	rc += CPoint(x,0);												// shift right

	// create trapezoid region
	CPoint pts[4];
	GetTrapezoid(rc, pts);
	m_rgn.CreatePolygonRgn(pts, 4, WINDING);

	return rc.Width();
}

//////////////////
// Given the boundint rect, compute trapezoid region.
// Note that the right and bottom edges not included in rect or
// trapezoid; these are normal rules of geometry. 
//
void CFolderTab::GetTrapezoid(const CRect& rc, CPoint* pts) const
{
	pts[0] = rc.TopLeft();
	pts[1] = CPoint(rc.left + CXOFFSET,   rc.bottom);
	pts[2] = CPoint(rc.right- CXOFFSET-1, rc.bottom);
	pts[3] = CPoint(rc.right-1, rc.top);
}


//////////////////
// Draw tab in normal or highlighted state
//
int CFolderTab::Draw(CDC& dc, CFont& font, BOOL bSelected)
{
	COLORREF bgColor = GetSysColor(bSelected ? COLOR_WINDOW     : COLOR_3DFACE);
	COLORREF fgColor = GetSysColor(bSelected ? COLOR_WINDOWTEXT : COLOR_BTNTEXT);

	CBrush brush(bgColor);					 // background brush
	dc.SetBkColor(bgColor);					 // text background
	dc.SetTextColor(fgColor);				 // text color = fg color

	CPen blackPen(PS_SOLID, 1, RGB(0,0,0));	// black
	CPen shadowPen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));

	// Fill trapezoid
	CPoint pts[4];
	CRect rc = m_rect;
	GetTrapezoid(rc, pts);
	CPen* pOldPen = dc.SelectObject(&blackPen);
	dc.FillRgn(&m_rgn, &brush);

	// Draw edges. This is requires two corrections:
	// 1) Trapezoid dimensions don't include the right and bottom edges,
	// so must use one pixel less on bottom (cybottom)
	// 2) the endpoint of LineTo is not included when drawing the line, so
	// must add one pixel (cytop)
	//
	pts[1].y--;			// correction #1: true bottom edge y-coord
	pts[2].y--;			// ...ditto
	pts[3].y--;			// correction #2:	extend final LineTo
	dc.MoveTo(pts[0]);						// upper left
	dc.LineTo(pts[1]);						// bottom left
	dc.SelectObject(&shadowPen);			// bottom line is shadow color
	dc.MoveTo(pts[1]);						// line is inside trapezoid bottom
	dc.LineTo(pts[2]);						// ...
	dc.SelectObject(&blackPen);			// upstroke is black
	dc.LineTo(pts[3]);						// y-1 to include endpoint
	if (!bSelected) {
		// if not highlighted, upstroke has a 3D shadow, one pixel inside
		pts[2].x--;		// offset left one pixel
		pts[3].x--;		// ...ditto
		dc.SelectObject(&shadowPen);
		dc.MoveTo(pts[2]);
		dc.LineTo(pts[3]);
	}
	dc.SelectObject(pOldPen);

	// draw text
	rc.DeflateRect(CXOFFSET + CXMARGIN, CYMARGIN);
	CFont* pOldFont = dc.SelectObject(&font);
	dc.DrawText(m_sText, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc.SelectObject(pOldFont);

	return m_rect.right;
}

//////////////////////////////////////////////////////////////////
// CFolderTabCtrl

IMPLEMENT_DYNAMIC(CFolderTabCtrl, CWnd)
BEGIN_MESSAGE_MAP(CFolderTabCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//16/12/2004
	ON_COMMAND_RANGE(1000, 1001, OnClickedRight )
END_MESSAGE_MAP()

CFolderTabCtrl::CFolderTabCtrl()
{
	m_iCurItem = 0;
//16/12/2004
m_iCurOffset=30;//button width
m_iCurItemOffset=0;
	m_dwFtabStyle = 0;
	m_nDesiredWidth = 0;
}

CFolderTabCtrl::~CFolderTabCtrl()
{
	while (!m_lsTabs.IsEmpty())
		delete (CFolderTab*)m_lsTabs.RemoveHead();
}

//////////////////
// Create folder tab control from static control.
// Destroys the static control. This is convenient for dialogs
//
BOOL CFolderTabCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	rc.bottom = rc.top + GetDesiredHeight();
	return Create(WS_CHILD|WS_VISIBLE, rc, pParent, nID);
}

//////////////////
// Create folder tab control.
//
BOOL CFolderTabCtrl::Create(DWORD dwStyle, const RECT& rc,
	CWnd* pParent, UINT nID, DWORD dwFtabStyle)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	m_dwFtabStyle = dwFtabStyle;

	static LPCTSTR lpClassName = _T("PDFolderTab");
	static BOOL bRegistered = FALSE; // registered?
	if (!bRegistered) {
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		if (!AfxRegisterClass(&wc)) {
			TRACE("*** CFolderTabCtrl::AfxRegisterClass failed!\n");
			return FALSE;
		}
		bRegistered = TRUE;
	}
	if (!CWnd::CreateEx(0, lpClassName, NULL, dwStyle, rc, pParent, nID))
		return FALSE;

//16/12/2004
//Create buttons
CButton *pBtn=new CButton();
pBtn->Create("",WS_VISIBLE|WS_CHILD|BS_ICON,CRect(15,0,30,15),this,1000);
HICON hicon=::LoadIcon(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDI_ARROW_RIGHT));
pBtn->SetIcon(hicon);
pBtn=new CButton();
pBtn->Create("",WS_VISIBLE|WS_CHILD|BS_ICON,CRect(0,0,15,15),this,1001);
hicon=::LoadIcon(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDI_ARROW_LEFT));
pBtn->SetIcon(hicon);

	// initialize fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = GetSystemMetrics(SM_CYHSCROLL)-CYMARGIN;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	m_fontNormal.CreateFontIndirect(&lf);
	lf.lfHeight -= 2;
	m_fontSelected.CreateFontIndirect(&lf);

	return TRUE;
}

//////////////////
// copy a font
//
static void CopyFont(CFont& dst, CFont& src)
{
	dst.DeleteObject();
	LOGFONT lf;
	VERIFY(src.GetLogFont(&lf));
	dst.CreateFontIndirect(&lf);
}

//////////////////
// Set normal, selected fonts
//
void CFolderTabCtrl::SetFonts(CFont& fontNormal, CFont& fontSelected)
{
	CopyFont(m_fontNormal, fontNormal);
	CopyFont(m_fontSelected, fontSelected);
}

//////////////////
// Paint function
//
void CFolderTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	CFolderTab* pCurTab = NULL;

	// draw all the normal (non-selected) tabs
	int n = GetItemCount();
	for (int i=0; i<n; i++) {
		CFolderTab* pTab = GetTab(i);
		ASSERT(pTab);
		if (i==m_iCurItem) {
			pCurTab = pTab;
		} else if (pTab->Draw(dc, m_fontNormal, FALSE) > rc.right)
			break;
	}
	// draw selected tab last so it will be "on top" of the others
	if (pCurTab)
		pCurTab->Draw(dc, m_fontSelected, TRUE);

	// draw border: line along the top edge, excluding seleted tab
	CRect rcCurTab(0,0,0,0);
	if (pCurTab)
		rcCurTab = pCurTab->GetRect();

	CPen blackPen(PS_SOLID, 1, RGB(0,0,0));	// black
	CPen* pOldPen = dc.SelectObject(&blackPen);
	dc.MoveTo(rcCurTab.right, rcCurTab.top);
	dc.LineTo(rc.right, rc.top);
	if (m_dwFtabStyle & FTS_FULLBORDER) {
		dc.MoveTo(rc.right-1, rc.top);
		dc.LineTo(rc.right-1, rc.bottom-1);
		dc.LineTo(rc.left,  rc.bottom-1);
		dc.LineTo(rc.left,  rc.top);
	} else {
		dc.MoveTo(rc.left, rc.top);
	}
	dc.LineTo(rcCurTab.TopLeft());
	dc.SelectObject(pOldPen);
}

//////////////////
// Handle mouse click: select new tab, if any. Notify parent, of course
//
void CFolderTabCtrl::OnLButtonDown(UINT nFlags, CPoint pt)
{
	int iTab = HitTest(pt);
	if (iTab>=0 && iTab!=m_iCurItem) {
		SelectItem(iTab);
		NMFOLDERTAB nm;
		nm.hwndFrom = m_hWnd;
		nm.idFrom = GetDlgCtrlID();
		nm.code = FTN_TABCHANGED;
		nm.iItem = iTab;
		nm.pItem = GetTab(iTab);
		CWnd* pParent = GetParent();
		pParent->SendMessage(WM_NOTIFY, nm.idFrom, (LPARAM)&nm);
	}
}

//////////////////
// Find which tab is under mouse, -1 if none
//
int CFolderTabCtrl::HitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);
	if (rc.PtInRect(pt)) {
		int n = GetItemCount();
		for (int i=0; i<n; i++) {
			if (GetTab(i)->HitTest(pt))
				return i;
		}
	}
	return -1;
}

//////////////////
// Select ith tab. Returns index selected
//
int CFolderTabCtrl::SelectItem(int iTab)
{
	if (iTab<0 || iTab>=GetItemCount())
		return -1;		// bad
	if (iTab==m_iCurItem)
		return iTab;	// already selected

	InvalidateTab(m_iCurItem);		// invalidate old tab (repaint)
	m_iCurItem = iTab;				// set new selected tab
	InvalidateTab(m_iCurItem);		// repaint new tab
	
	return m_iCurItem;
}

//////////////////
/// Invalidate a tab: invaldate its rect
//
void CFolderTabCtrl::InvalidateTab(int iTab, BOOL bErase)
{
	InvalidateRect(GetTab(iTab)->GetRect(), bErase);
}

BOOL CFolderTabCtrl::Load(UINT nIDRes)
{
	CString s;
	if (!s.LoadString(nIDRes))
		return FALSE;

	CString sTab;
	for (int i=0; AfxExtractSubString(sTab, s, i); i++) {
		AddItem(sTab);
	}
	RecomputeLayout();
	return TRUE;
}

int CFolderTabCtrl::AddItem(LPCTSTR lpszText)
{
	m_lsTabs.AddTail(new CFolderTab(lpszText));
	return m_lsTabs.GetCount() - 1;
}

BOOL CFolderTabCtrl::RemoveItem(int iPos)
{
	POSITION pos = m_lsTabs.FindIndex(iPos);
	if (pos) {
		CFolderTab* pTab = (CFolderTab*)m_lsTabs.GetAt(pos);
		m_lsTabs.RemoveAt(pos);
		delete pTab;
	}
	return pos!=NULL;
}

CFolderTab* CFolderTabCtrl::GetTab(int iPos)
{
	POSITION pos = m_lsTabs.FindIndex(iPos);
	return pos ? static_cast<CFolderTab*>(m_lsTabs.GetAt(pos)) : NULL;
}

void CFolderTabCtrl::RecomputeLayout()
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_fontNormal);
	//16/12/2004
	int x = m_iCurOffset;
	//int x = 0;
	int n = GetItemCount();
	CFolderTab* pTab;
	for (int i=0; i<n; i++) {
		pTab = GetTab(i);
		if (pTab) 
			x += pTab->ComputeRgn(dc, x) - CXOFFSET;
	}
	dc.SelectObject(pOldFont);

	if (pTab) {
		CRect rc = pTab->GetRect();
		m_nDesiredWidth = rc.right;
	}
}

//16/12/2004
void CFolderTabCtrl::OnClickedRight(UINT nID )
{

	//Control notification handler

	CClientDC dc(this);
	CFolderTab* pTab;

	if(nID==1000){
		if(m_iCurItemOffset<m_lsTabs.GetCount()-1)
		m_iCurItemOffset++;
	}
	else if(m_iCurItemOffset>0){
		m_iCurItemOffset--;
	}

	float fCurOffset=0.0;//button width
	int i;
	for(i=0;i<m_iCurItemOffset;i++){
		pTab = GetTab(i);
		if (pTab){ 
			//float fdata;
			//fdata*=2;
			fCurOffset += pTab->GetRect().Width();//pTab->ComputeRgn(dc, 0/*m_iCurOffset*/);
		}
	}

	for(i=0;i<m_iCurItemOffset;i++){
		float fdata=CXOFFSET;
		fCurOffset -=fdata;
	}

	m_iCurOffset=-fCurOffset+30;
	RecomputeLayout();
	Invalidate();
}

