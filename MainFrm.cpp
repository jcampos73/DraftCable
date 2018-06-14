// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DraftDraw.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//ON_WM_SETCURSOR()
	ON_COMMAND(ID_FILE_NEW_COMPONENTE, OnFileNewComponente)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	//Update commands
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, OnUpdateTool)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COOR, OnUpdateCoor)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnUpdateMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PART, OnUpdateBtEditPart)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MODE,
	ID_INDICATOR_COOR,
	ID_INDICATOR_TOOL,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_strTool="";

	m_hbCurrent=NULL;

	BOOL bResult=m_bmToolBar.LoadBitmap(IDB_BITMAP1);

	bResult=m_bmDefault.LoadBitmap(IDB_BITMAP2);

	m_hbCurrentRack=NULL;

	bResult=m_bmToolBarRack.LoadBitmap(IDB_TBRACK_SEL);

	bResult=m_bmDefaultRack.LoadBitmap(IDB_TBRACK);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBarDrawing.CreateEx(this, TBSTYLE_DROPDOWN, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarDrawing.LoadToolBar(IDR_DRAWING))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBarPlace.CreateEx(this, TBSTYLE_DROPDOWN, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarPlace.LoadToolBar(IDR_PLACE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//added to resize indicator tool
	//--------------------------------------------------
	int index=m_wndStatusBar.CommandToIndex(ID_INDICATOR_TOOL);

	CRect rect,rect1,rect2;
	GetWindowRect(&rect);
	m_wndStatusBar.GetItemRect(index,&rect1);


	rect2=CRect(
		CPoint(rect.Width()*0.40,0),
		CSize(rect.Width()*.25,rect1.Height())
		);


	

	if(!m_wndStatusBar.GetStatusBarCtrl().IsSimple()){

	/*
	int Parts[sizeof(indicators)/sizeof(UINT)];

	m_wndStatusBar.GetStatusBarCtrl().GetParts(sizeof(indicators)/sizeof(UINT),Parts);

	Parts[0]=100;
	Parts[1]=200;
	Parts[2]=250;
	Parts[3]=300;
	Parts[4]=350;
	m_wndStatusBar.GetStatusBarCtrl().SetParts(sizeof(indicators)/sizeof(UINT),Parts);
	m_wndStatusBar.GetStatusBarCtrl().GetParts(sizeof(indicators)/sizeof(UINT),Parts);
	*/

	UINT nID,nStyle;
	int cxWidth;

	m_wndStatusBar.GetPaneInfo(1,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(1,nID,nStyle,200);

	m_wndStatusBar.GetPaneInfo(2,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(2,nID,nStyle,200);

	m_wndStatusBar.GetPaneInfo(3,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(3,nID,nStyle,200);

/*
	m_wndStatusBar.SetPaneText(1,"pruebas");
*/

	}
	//--------------------------------------------------

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndToolBarDrawing.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBarDrawing);

	m_wndToolBarPlace.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBarPlace);



m_wndToolBar.SendMessage(TB_SETEXTENDEDSTYLE, 0,
                          TBSTYLE_EX_DRAWDDARROWS);

 

 int iButton = m_wndToolBar.SendMessage(TB_COMMANDTOINDEX,
                              ID_EDIT_UNDO);
 DWORD dwStyle = m_wndToolBar.GetButtonStyle(iButton);
 dwStyle |= TBSTYLE_DROPDOWN;
 m_wndToolBar.SetButtonStyle(iButton, dwStyle);

 iButton = m_wndToolBar.SendMessage(TB_COMMANDTOINDEX,
                              ID_EDIT_REDO);
 dwStyle = m_wndToolBar.GetButtonStyle(iButton);
 dwStyle |= TBSTYLE_DROPDOWN;
 m_wndToolBar.SetButtonStyle(iButton, dwStyle);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

void CMainFrame::SetupMenus()
{
	/*
	if ((GetSystemMetrics(SM_PENWINDOWS)) == NULL)
	{
		CMenu* pMenu = GetMenu();
		ASSERT(pMenu != NULL);
		pMenu->EnableMenuItem(IDM_TEST_PENEDIT_CODE, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(IDM_TEST_PENEDIT_TEMPLATE, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(IDM_TEST_PENEDIT_FEATURES, MF_DISABLED|MF_GRAYED);
	}
	*/
	// do not test for spin control until the user tries it
	// if the custom control DLL is not present, the test spin
	//  control menu item will be disabled in 'OnTestSpinEdit'.

	// custom menu tests
	VERIFY(m_menuPolygon.CreateMenu());
	AttachCustomMenu();
}

static UINT g_idMenuPolygon[] = {
	IDB_MENU_POLYGON_RECT,
	IDB_MENU_POLYGON_PARALEL,
	IDB_MENU_POLYGON_TRAP,
	IDB_MENU_POLYGON_ROMBO,
	IDB_MENU_POLYGON_TRIANG,
	IDB_MENU_POLYGON_PENT,
	IDB_MENU_POLYGON_HEX,
	IDB_MENU_POLYGON_OCT,
	IDB_MENU_POLYGON_CROSS,
	IDB_MENU_POLYGON_NGEAR,
	IDB_MENU_POLYGON_NSTAR,
	IDB_MENU_POLYGON_NPOLYGON,
	IDB_MENU_POLYGON_T,
	IDB_MENU_POLYGON_L,
	IDB_MENU_POLYGON_STRAIGHT,
	IDB_MENU_POLYGON_NCROSS
};
const int g_nMenuPolygon = sizeof(g_idMenuPolygon)/sizeof(g_idMenuPolygon[0]);
const int g_nMenuPolygonCols = 4; //4 column

// Call AttachCustomMenu once
//   it will replace the menu item with the ID  'IDM_TEST_CUSTOM_MENU'
//   with a color menu popup
// Replace the specified menu item with a color popup
void CMainFrame::AttachCustomMenu()
{
	// now add a few new menu items

	int nItemCol=g_nMenuPolygon/g_nMenuPolygonCols;
	for (int j=0;j<g_nMenuPolygonCols;j++)
	for (int iColor = 0; iColor < nItemCol; iColor++){

		//HBITMAP hbmp=::LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(g_idsMenuPolygon[iColor]));

		CBitmap *bmp=new CBitmap();

		bmp->LoadMappedBitmap(g_idMenuPolygon[iColor*g_nMenuPolygonCols+j]);


		UINT id=IDS_MENU_POLYGON_FIRST + iColor*g_nMenuPolygonCols+j +1;

		if(id==IDS_MENU_POLYGON_RECT){
			id=ID_BUTTON_RECT;
		}

		if(iColor==0&&j){

			VERIFY(m_menuPolygon.AppendMenu(MF_ENABLED | MF_MENUBARBREAK, id, bmp));
		}
		else{

			/*
			m_colorMenu.AppendColorMenuItem(IDS_COLOR_NAME_FIRST + iColor, colors[iColor]);
			*/
			VERIFY(m_menuPolygon.AppendMenu(MF_ENABLED, id, bmp));
		}

	}

	// Replace the specified menu item with a color popup
	//  (note: will only work once)

	CMenu* pMenuBar = GetMenu();
	ASSERT(pMenuBar != NULL);
	TCHAR szString[256];     // don't change the string

	pMenuBar->GetMenuString(ID_IMG_POLYGONPOPUP, szString, sizeof(szString),
		MF_BYCOMMAND);

	VERIFY(GetMenu()->ModifyMenu(ID_IMG_POLYGONPOPUP, MF_BYCOMMAND | MF_POPUP,
		(UINT)m_menuPolygon.m_hMenu, szString));



}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
/*	
	return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
*/
}

void CMainFrame::OnFileNewComponente() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_COMMAND,ID_FILE_NEW);
	SendMessage(WM_COMMAND,ID_EDIT_PART);
}

void CMainFrame::OnUpdateTool(CCmdUI *pCmdUI){

	CDraftDrawApp *theApp=(CDraftDrawApp *)AfxGetApp();


	switch(theApp->GetActiveTool()){

	case 0:
		m_strTool="Herramienta seleccionar";
		break;
	case 1:
		m_strTool="Herramienta insertar";
		break;
	default:
		m_strTool="";
		break;
	}

    pCmdUI->Enable();

	int index=m_wndStatusBar.CommandToIndex( ID_INDICATOR_TOOL);

	m_wndStatusBar.SetPaneText(index,m_strTool);

}

void CMainFrame::OnUpdateCoor(CCmdUI *pCmdUI){
	//FindView(theApp.pDocTemplate,TRUE);//skip left/tree view

	m_strCoor.Format(_T("(X,Y) = (%i,%i)"),g_iX,g_iY);

    pCmdUI->Enable();

	int index=m_wndStatusBar.CommandToIndex( ID_INDICATOR_COOR);

	m_wndStatusBar.SetPaneText(index,m_strCoor);
}

void CMainFrame::OnUpdateMode(CCmdUI *pCmdUI){

	CDraftDrawApp *theApp=(CDraftDrawApp *)AfxGetApp();

	if(theApp->GetActiveMode()){
		switch(theApp->GetActiveDocType()){

		case 0:
			m_strMode="MODO EDICIÓN DE PIEZA";
			break;
		case 1:
			m_strMode="MODO EDICIÓN DE CHASIS";
			break;
		default:
			m_strMode="";
			break;
		}
	}
	else{
		switch(theApp->GetActiveDocType()){

		case 0:
			m_strMode="MODO PLANO";
			break;
		case 1:
			m_strMode="MODO ALZADO DE RACK";
			break;
		default:
			m_strMode="";
			break;
		}
	}

    pCmdUI->Enable();

	int index=m_wndStatusBar.CommandToIndex( ID_INDICATOR_MODE);

	m_wndStatusBar.SetPaneText(index,m_strMode);
}

void CMainFrame::OnUpdateBtEditPart(CCmdUI *pCmdUI){

	int i=0;

/*
	switch( ((CDraftDrawApp *)AfxGetApp())->GetActiveMode()){
	case 0:
		break;
	case 1:
		if(m_hbCurrent==NULL){
			m_wndToolBarPlace.SetBitmap(HBITMAP(m_bmToolBar));
			m_hbCurrent=HBITMAP(m_bmToolBar);
		}
		break;
	}
*/
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

	//ATENTION!
	//06/07/2003
	//This message has been defered to by processed by main frame instead
	//of child frame
	if(nID==ID_INDICATOR_TOOL){

		return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	}
	
	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CView * CMainFrame::FindView(CDocTemplate *pTemplate,
	BOOL bNotTree /*= FALSE*/,
	CRuntimeClass *pViewMatch /*=NULL*/)
{
	if (!pTemplate)
		return NULL;

	POSITION pos = pTemplate->GetFirstDocPosition();
	if (pos){

		CDocument *pDoc = pTemplate->GetNextDoc(pos);
		POSITION viewPos = pDoc->GetFirstViewPosition();
		if (viewPos){

			CView *pView=pDoc->GetNextView(viewPos);

			if((bNotTree)&&(pView->IsKindOf( RUNTIME_CLASS( CTreeView ))) ){
				pView=pDoc->GetNextView(viewPos);
			}
			if(pViewMatch){

				if((viewPos)&&(!pView->IsKindOf(pViewMatch))){
					do{

						pView=pDoc->GetNextView(viewPos);
						if(pView->IsKindOf(pViewMatch)){
							return pView;
						}
						
					}while(viewPos);
				}
				return NULL;
			}

			return pView;
		}
	}
	return NULL;
}

//////////////////
// Handle TBN_DROPDOWN
// Default is to display the specified menu at the right place.
// You can override to generate dynamic menus
//
// Args:
//		- NMTOOLBAR struct from TBN_DROPDOWN
//		- command id of button
//		- point to display menu at
//
//void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
void CMainFrame::OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr)
{
	// load and display popup menu
	CMenu menu;
	menu.LoadMenu(IDR_EDITDROPDOWN);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);


	CDraftDrawDoc *pDDDoc=theApp.GetActiveDocument();
	pPopup->DeleteMenu(0,MF_BYPOSITION);
	//pPopup->AppendMenu(MF_STRING,ID_0_1,"Info");

	std::deque<std::wstring>::iterator it;
	std::deque<std::wstring> *pDeque;

	//if(pnmtb->iItem==ID_EDIT_REDO){
	if(pnmtb->idFrom==ID_EDIT_REDO){
		pDeque=&pDDDoc->cmdDeque->m_dqCommands;
	}
	else{
		pDeque=&pDDDoc->cmdDeque->m_dqCommandsUndo;
	}

	int nPos=0;
	int begin=pDDDoc->cmdDeque->m_nIndexCommQue-10;
	begin=begin<0?0:begin;
	std::size_t end=pDDDoc->cmdDeque->m_nIndexCommQue+10;
	end=end>pDDDoc->cmdDeque->m_dqCommands.size()?pDDDoc->cmdDeque->m_dqCommands.size():end;
	//for(it=pDeque->begin();it!=pDeque->end();++it){
	for(std::size_t i=begin;i<end;i++){

		//pPopup->AppendMenu(MF_STRING,ID_0_1,it->c_str());

		//if(pnmtb->iItem==ID_EDIT_REDO){
		if(pnmtb->idFrom==ID_EDIT_REDO){
			pPopup->AppendMenu(MF_STRING,ID_0_1,pDDDoc->cmdDeque->m_dqCommands[i].c_str());
		}
		else{
			pPopup->AppendMenu(MF_STRING,ID_0_1,pDDDoc->cmdDeque->m_dqCommandsUndo[i].c_str());
		}

		if(pDDDoc->cmdDeque->m_nIndexCommQue==i){
			pPopup->CheckMenuItem(nPos,MF_BYPOSITION|MF_CHECKED );
		}


		//i++;
		nPos++;
	}


	CRect rc;
	//m_wndToolBar.SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	m_wndToolBar.SendMessage(TB_GETRECT, pnmtb->idFrom, (LPARAM)&rc);
	m_wndToolBar.ClientToScreen(&rc);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
		rc.left, rc.bottom, this, &rc);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

	/*
	(!!!)27/03/2005
	This lines have been commented to allow compilation.
	If OLE is added in future uncomment them.
	See MFC ABR 2001:
	PRB: Problems Occur When Defining _AFX_NO_XXX
	Q140751

	Another solution is including MFC's private headers and uncomment lines now:

	#include <afxpriv.h>
	#include "../src/oleimpl2.h"
	#include "../src/afximpl.h"

	See MFC ABR 2001:
	HOWTO: Localize Stock ActiveX Property Pages With MFC Resources 
	Q200555

	*/
#ifndef _AFX_NO_OLE_SUPPORT
	/*
	// allow hook to set the title (used for OLE support)
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
		return;
	*/
#endif

	CMDIChildWnd* pActiveChild = NULL;
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle &&
	  (pActiveChild = MDIGetActive()) != NULL &&
	  (pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 &&
	  (pDocument != NULL ||
	   (pDocument = pActiveChild->GetActiveDocument()) != NULL))
	   //JC 27/03/2005
	   if(pDocument->IsModified()){
			UpdateFrameTitleForDocument(pDocument->GetTitle()+"*");
	   }
	   else
		UpdateFrameTitleForDocument(pDocument->GetTitle());
	else
	{
		LPCTSTR lpstrTitle = NULL;
		CString strTitle;

		if (pActiveChild != NULL)
		{
			strTitle = pActiveChild->GetTitle();
			if (!strTitle.IsEmpty())
				lpstrTitle = strTitle;
		}
		UpdateFrameTitleForDocument(lpstrTitle);
	}
}
