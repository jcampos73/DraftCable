// DraftDraw.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Globals.h"
#include "DraftDraw.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildFrmPP.h"
//#include "DraftDrawDoc.h"
#include "DraftDrawView.h"
#include "LeftView.h"
#include "TBEditView.h"

#include "htmlhelp.h"


BOOL CopyBitmap(CBitmap *pSrc,CBitmap *pDst);



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawApp

#define ID_FILE_MRU_FILE17 ID_FILE_MRU_FILE16 + 1
#define ID_FILE_MRU_FILE18 ID_FILE_MRU_FILE17 + 1
#define ID_FILE_MRU_FILE19 ID_FILE_MRU_FILE18 + 1
#define ID_FILE_MRU_FILE20 ID_FILE_MRU_FILE19 + 1

BEGIN_MESSAGE_MAP(CDraftDrawApp, CWinApp)
	//{{AFX_MSG_MAP(CDraftDrawApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW_RACK, OnFileNewRack)
	ON_COMMAND(ID_EDIT_RACK, OnEditRack)
	ON_COMMAND(ID_FILE_NEW_PATCHP, OnFileNewPatchP)
	ON_COMMAND(ID_FILE_NEW_SHEET, OnFileNewSheet)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE20, MyMRUFileHandler)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawApp construction

CDraftDrawApp::CDraftDrawApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDraftDrawApp object

CDraftDrawApp theApp;

CObArray g_aShBuffer;
int g_aShBufIndex=0;

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawApp initialization

BOOL CDraftDrawApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//HKEY_CURRENT_USER\Software\<company name>\<application name>\<section name>\<value name>
	SetRegistryKey(_T("CRIndustrial"));

	LoadStdProfileSettings(6);  // Load standard INI file options (including MRU)

	//Added 31/10/2004
	if(!LoadProfileSettings()){
		AfxMessageBox("No se ha podido verificar la información de Licencia!",MB_ICONSTOP|MB_OK,-1);
		return FALSE;
	}

	//Added 07/02/2005
	AfxOleInit();
	//21/04/2013
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	/*CMultiDocTemplate* pDocTemplate;*/
	pDocTemplate = new CMultiDocTemplate(
		IDR_DRAFTDTYPE,
		RUNTIME_CLASS(CDraftDrawDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	//added 20/06/2004
	//document templates
	//==================================================

	pMDTPatchP = new CMultiDocTemplate(
		IDR_PATCHPTYPE,
		RUNTIME_CLASS(CDraftDrawDoc),	
		RUNTIME_CLASS(CChildFramePP), 
		RUNTIME_CLASS(CTBEditView));
	AddDocTemplate(pMDTPatchP);

	//==================================================




AfxInitGlobal();
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// The main window has been initialized, so show and update it.
	pMainFrame->SetupMenus();
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
		//MAXIMIZE CHILD FRAME WINDOW
		//It has proved no use cs|=WS_MAXIMIZE in CChildFrame::PreCraeteWindow(),
		//so command is send here, between main frame ShowWindow and UpdateWindow.
		//Better than this solution is override CChildFrame::AcivateFrame() as descrived in
		//Knowledge Base/Visual C++
		//How to Create a Minimized/Maximized MDI Child Window
		//ID: Q142317
		//==================================================
		/*
		CWnd *pwnd=pMainFrame->GetWindow(GW_CHILD);
		pwnd=pwnd->GetWindow(GW_CHILD);
		pwnd->ShowWindow(SW_SHOWMAXIMIZED);
		*/
		//==================================================
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDraftDrawApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawApp message handlers


int CDraftDrawApp::GetActiveTool()
{
	CMDIFrameWnd *pFrame = 
				 (CMDIFrameWnd*)m_pMainWnd;

	if(!pFrame) return -1;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = 
				 (CMDIChildWnd *) pFrame->GetActiveFrame();

	if(!pChild) return -1;

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

	// Get the active view attached to the active MDI child
	// window.
	CView *pView = (CView *) pChild->GetActiveView();

	if(!pView) return -1;

	CDraftDrawDoc *pDoc=(CDraftDrawDoc *)pView->GetDocument();

	if(!pDoc) return -1;

	int iTool=pDoc->m_iToolSel;

	return iTool;
}

int CDraftDrawApp::GetActiveMode()
{
	CMDIFrameWnd *pFrame = 
				 (CMDIFrameWnd*)m_pMainWnd;

	if(!pFrame) return -1;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = 
				 (CMDIChildWnd *) pFrame->GetActiveFrame();

	if(!pChild) return -1;

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

	// Get the active view attached to the active MDI child
	// window.
	CView *pView = (CView *) pChild->GetActiveView();

	if(!pView) return -1;

	CDraftDrawDoc *pDoc=(CDraftDrawDoc *)pView->GetDocument();

	if(!pDoc) return -1;

	int iMode=pDoc->m_bFlagPartEdit;

	return iMode;
}

CDraftDrawDoc *CDraftDrawApp::GetActiveDocument()
{
	CMDIFrameWnd *pFrame = 
				 (CMDIFrameWnd*)m_pMainWnd;

	if(!pFrame) return NULL;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = 
				 (CMDIChildWnd *) pFrame->GetActiveFrame();

	if(!pChild) return NULL;

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

	// Get the active view attached to the active MDI child
	// window.
	CView *pView = (CView *) pChild->GetActiveView();

	if(!pView) return NULL;

	CDraftDrawDoc *pDoc=(CDraftDrawDoc *)pView->GetDocument();

	if(!pDoc) return NULL;

	return pDoc;
}

//Create a new rack document for current document
void CDraftDrawApp::OnFileNewRack(){

	//Get active document (we are going to open a new one, that is why is prev)
	CDraftDrawDoc *pDocPrev=GetActiveDocument();

	//Check if we have allready have a rack open
	if(pDocPrev->m_pDocRack){

		//Iterate through document views
		POSITION pos=pDocPrev->m_pDocRack->GetFirstViewPosition();
		CView *pview=pDocPrev->m_pDocRack->GetNextView(pos);

		// Get main window
		CMDIFrameWnd *pFrameWnd=(CMDIFrameWnd *)AfxGetMainWnd();
		CWnd *pWnd=pview->GetParent();
		//We try to find the active view
		while(pWnd->IsKindOf( RUNTIME_CLASS( CSplitterWnd ) ) ||pWnd->IsKindOf( RUNTIME_CLASS( CFolderFrame ) )){
			pWnd=pWnd->GetParent();
		}
		//Focus to MDI child window
		pFrameWnd->MDIActivate(pWnd);
		//If window is minimized, restore it
		if(pview->GetParent()->IsIconic()){
			pview->GetParent()->ShowWindow(SW_RESTORE);
		}
		//return, nothing to do else
		return;
	}

	//Send a message to open a new document
	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW);

	//Get a pointer to new document and set to rack type document
	CDraftDrawDoc *pDoc=GetActiveDocument();
	pDoc->m_iTypeDocument=_DOCTYPE_RACK;//default doc type is schematic

	//23/11/2004
	//Now check if we are editing a part to create its rack complementary part
	if(pDocPrev->m_bFlagPartEdit){

		//Create a new document of type part
		pDoc->OnNewDocumentPart();

		//Get the title of previous doc
		CString str=pDocPrev->GetTitle();

		//Get the rack counter part name
		//get libray name
		CString strLib = str.Left(str.Find('.'));
		//remove library name
		CString strFile = str.Right(str.GetLength() - str.Find('.') - 1);
		str = pDocPrev->GetRackOb(strFile, strLib);

		//If it exists open the document
		if(!str.IsEmpty()){
			//pDoc->OnOpenDocumentPart(str);
			pDoc->SetPathName1(strLib + "." + str);
			pDoc->SetTitle(strLib + "." + str);
			pDoc->OnDatabaseLoad();
		}
	}
	else{
		//Load a new rack using prev doc to identity rack counter part of each part
		pDoc->LoadRack(pDocPrev);
	}

	//Link documents: normal one and rack
	pDocPrev->m_pDocRack=pDoc;
	pDoc->m_pDocRack=pDocPrev;
}

int CDraftDrawApp::GetActiveDocType()
{

	CDraftDrawDoc *pDoc=GetActiveDocument();

	if(pDoc==NULL){
		return 0;
	}

	return pDoc->m_iTypeDocument;
}

void CDraftDrawApp::OnEditRack(){

	OnFileNewRack();
}

void CDraftDrawApp::OnUpdateBtEditPart(CCmdUI *pCmdUI){

	CMainFrame *pMFrame = 
				 (CMainFrame*)m_pMainWnd;

	//**************************************************************************
	//First implementation tryes to change whole toolbar bitmap
	//produces strange effects in:
	//-button background color
	//-disbled buttons
	//**************************************************************************
/*
	switch( GetActiveMode()){
	case 0:
		break;
	case 1:
		if(pMFrame->m_hbCurrent==NULL){
			pMFrame->m_wndToolBarPlace.SetBitmap(HBITMAP(pMFrame->m_bmToolBar));
			pMFrame->m_hbCurrent=HBITMAP(pMFrame->m_bmToolBar);
			pMFrame->m_wndToolBarPlace.Invalidate();
		}
		break;
	}
*/
	//**************************************************************************

	switch( GetActiveMode()){
	case 0:
		if(pMFrame->m_hbCurrent==HBITMAP(pMFrame->m_bmToolBar)){
			CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl();
			//myToolBar.MarkButton(ID_EDIT_PART,FALSE);
			myToolBar.CheckButton(ID_EDIT_PART,FALSE);

			CImageList* pilImages=myToolBar.GetImageList( );

			/*
			pilImages->Replace(9,CBitmap::FromHandle(pMFrame->m_hbDefault),NULL);
			*/

			//pilImages->Replace(8,&pMFrame->m_bmDefault,NULL);
			pMFrame->m_hbCurrent=HBITMAP(pMFrame->m_bmDefault);
			//pMFrame->m_wndToolBarPlace.Invalidate();
		}
		break;
	case 1:
		if(pMFrame->m_hbCurrent==NULL){
			CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl();
			//myToolBar.MarkButton(ID_EDIT_PART,TRUE);
			myToolBar.CheckButton(ID_EDIT_PART,TRUE);

			CImageList* pilImages=myToolBar.GetImageList( );

			/*
			IMAGEINFO ImageInfo;
			pilImages->GetImageInfo(9,&ImageInfo);
			pMFrame->m_hbDefault=ImageInfo.hbmImage;
			*/

			/*
			CopyBitmap(&pMFrame->m_bmToolBar,&pMFrame->m_bmDefault);
			*/


			//pilImages->Replace(8,&pMFrame->m_bmToolBar,NULL);
			pMFrame->m_hbCurrent=HBITMAP(pMFrame->m_bmToolBar);
			//pMFrame->m_wndToolBarPlace.Invalidate();
		}
		else if(pMFrame->m_hbCurrent==HBITMAP(pMFrame->m_bmDefault)){
			CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl();
			//myToolBar.MarkButton(ID_EDIT_PART,TRUE);
			myToolBar.CheckButton(ID_EDIT_PART,TRUE);

			CImageList* pilImages=myToolBar.GetImageList( );


			//pilImages->Replace(8,&pMFrame->m_bmToolBar,NULL);
			pMFrame->m_hbCurrent=HBITMAP(pMFrame->m_bmToolBar);
			//pMFrame->m_wndToolBarPlace.Invalidate();

		}
		break;
	}
}

void CDraftDrawApp::OnUpdateBtEditRack(CCmdUI *pCmdUI){

	CMainFrame *pMFrame = 
				 (CMainFrame*)m_pMainWnd;

	CDraftDrawDoc *pDDDoc=GetActiveDocument();

	switch(pDDDoc->m_iTypeDocument){
		case _DOCTYPE_SCHEMATIC:
			if(pMFrame->m_hbCurrentRack==HBITMAP(pMFrame->m_bmToolBarRack)){
				CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl();
				//myToolBar.MarkButton(ID_EDIT_RACK,FALSE);
				myToolBar.CheckButton(ID_EDIT_RACK,FALSE);

				CImageList* pilImages=myToolBar.GetImageList( );

				//pilImages->Replace(9,&pMFrame->m_bmDefaultRack,NULL);
				pMFrame->m_hbCurrentRack=HBITMAP(pMFrame->m_bmDefaultRack);
				//pMFrame->m_wndToolBarPlace.Invalidate();
			}
			break;
		case _DOCTYPE_RACK:
			if(pMFrame->m_hbCurrentRack==NULL){
				CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl();
				//myToolBar.MarkButton(ID_EDIT_RACK,TRUE);
				myToolBar.CheckButton(ID_EDIT_RACK,TRUE);

				CImageList* pilImages=myToolBar.GetImageList( );


				//pilImages->Replace(9,&pMFrame->m_bmToolBarRack,NULL);
				pMFrame->m_hbCurrentRack=HBITMAP(pMFrame->m_bmToolBarRack);
				//pMFrame->m_wndToolBarPlace.Invalidate();
			}
			else if(pMFrame->m_hbCurrentRack==HBITMAP(pMFrame->m_bmDefaultRack)){
				CToolBarCtrl& myToolBar=pMFrame->m_wndToolBarPlace.GetToolBarCtrl(); 
				//myToolBar.MarkButton(ID_EDIT_RACK,TRUE);
				myToolBar.CheckButton(ID_EDIT_RACK,TRUE);

				CImageList* pilImages=myToolBar.GetImageList( );


				//pilImages->Replace(9,&pMFrame->m_bmToolBarRack,NULL);
				pMFrame->m_hbCurrentRack=HBITMAP(pMFrame->m_bmToolBarRack);
				//pMFrame->m_wndToolBarPlace.Invalidate();
			}
			break;
	}
}

CSize CDraftDrawApp::GetActiveViewProp(){
/*
	CDraftDrawView *pDDView=(CDraftDrawView *)((CMainFrame *)AfxGetMainWnd())->FindView(pDocTemplate,TRUE);
	return pDDView->m_szGrid;
*/
	return GetActiveDocument()->m_szGrid;
}

void CDraftDrawApp::SetActiveViewProp(CSize szGrid){
/*
	CDraftDrawView *pDDView=(CDraftDrawView *)((CMainFrame *)AfxGetMainWnd())->FindView(pDocTemplate,TRUE);
	pDDView->m_szGrid=szGrid;
*/
	GetActiveDocument()->m_szGrid=szGrid;
}

//this funtion is auxiliar and should be removed from here
BOOL CopyBitmap(CBitmap *pSrc,CBitmap *pDst){

	BITMAP BitMap;
	pSrc->GetBitmap(&BitMap);
	DWORD nCount=pSrc->GetBitmapBits(0,BitMap.bmBits);

	LPVOID lpBuf=(LPVOID)new BYTE[nCount];

	pSrc->GetBitmapBits(nCount,lpBuf);

	BitMap.bmBits=lpBuf;
	pDst->CreateBitmapIndirect(&BitMap);


	return TRUE;
}

void CDraftDrawApp::OnFileNewPatchP(){

	pMDTPatchP->OpenDocumentFile(NULL);
}

void CDraftDrawApp::OnFileNewSheet(){

	CMainFrame *pMFrame = (CMainFrame*)m_pMainWnd;
	
	CChildFrame *pChldFrame=(CChildFrame *)pMFrame->MDIGetActive();

	


CDraftDrawDoc *pDDDoc=(CDraftDrawDoc *)pChldFrame->GetActiveView()->GetDocument();

pDDDoc->AddSheet();


	int iData=pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().GetItemCount();



pDDDoc->ChangeSheet(iData);


	CString str;
	str.Format("Hoja %i",iData+1);
	pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().AddItem(str);

/*
	//Hides active view
	pChldFrame->GetActiveView()->ShowWindow(SW_HIDE);

	//Create new view

	CFolderFrame *pwndFolderFrame=new CFolderFrame();

	

CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CDraftDrawDoc );
CObject* pObject = pRuntimeClass->CreateObject();
ASSERT( pObject->IsKindOf( RUNTIME_CLASS( CDraftDrawDoc ) ) );

	CDraftDrawDoc *pDDDoc=(CDraftDrawDoc *)pObject;
	pDDDoc->OnNewDocument();

	CCreateContext ccContext;
	ccContext.m_pCurrentDoc=pDDDoc;
	ccContext.m_pCurrentFrame=NULL;
	ccContext.m_pLastView=NULL;
	ccContext.m_pNewDocTemplate=NULL;
	ccContext.m_pNewViewClass=RUNTIME_CLASS(CDraftDrawView);

	pwndFolderFrame->Create(pChldFrame,
		   RUNTIME_CLASS(CDraftDrawView), &ccContext,
		   IDR_FOLDERTABS);
*/
	pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().RecomputeLayout();

	pChldFrame->m_wndFolderFrame.ShowControls(CFolderFrame::bestFit);

	pChldFrame->m_wndFolderFrame.GetFolderTabCtrl().SelectItem(iData);

	pChldFrame->GetActiveView()->Invalidate();
}

void CDraftDrawApp::OnEditUndo(){

	CDraftDrawDoc *pDDDoc=GetActiveDocument();

	if(pDDDoc->cmdDeque->m_nIndexCommQue>0||(GetActiveDocument()->cmdDeque->m_nIndexCommQue==0&&GetActiveDocument()->cmdDeque->m_bBackwards)){

		std::string sCommand;

		if(!pDDDoc->cmdDeque->m_bBackwards)
			pDDDoc->cmdDeque->m_nIndexCommQue--;

		sCommand=pDDDoc->cmdDeque->m_dqCommandsUndo.at(pDDDoc->cmdDeque->m_nIndexCommQue);

		BOOL bResult=m_OMParser.OMParse(sCommand)>0;

		pDDDoc->cmdDeque->m_nIndexCommQue--;
		pDDDoc->cmdDeque->m_bBackwards=true;

		while(pDDDoc->cmdDeque->m_nIndexCommQue>0&&bResult){

			sCommand=pDDDoc->cmdDeque->m_dqCommandsUndo.at(pDDDoc->cmdDeque->m_nIndexCommQue);
			bResult=m_OMParser.OMParse(sCommand)>0;
			pDDDoc->cmdDeque->m_nIndexCommQue--;
		}

		pDDDoc->UpdateAllViews(NULL,1);//update all views without a region (lHint=1)

		// Update title (to add '*').
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)m_pMainWnd;
		pFrame->OnUpdateFrameTitle(TRUE);

	}

}

void CDraftDrawApp::OnEditRedo(){

	CDraftDrawDoc *pDDDoc=GetActiveDocument();

	int idata=pDDDoc->cmdDeque->m_dqCommandsUndo.size();

	if(pDDDoc->cmdDeque->m_nIndexCommQue<idata){

		std::string sCommand;

		if(pDDDoc->cmdDeque->m_bBackwards)
			pDDDoc->cmdDeque->m_nIndexCommQue++;

		sCommand=pDDDoc->cmdDeque->m_dqCommands.at(pDDDoc->cmdDeque->m_nIndexCommQue);

		BOOL bResult=m_OMParser.OMParse(sCommand)>0;

		pDDDoc->cmdDeque->m_nIndexCommQue++;
		pDDDoc->cmdDeque->m_bBackwards=false;

		while(pDDDoc->cmdDeque->m_nIndexCommQue<idata&&bResult){

			sCommand=pDDDoc->cmdDeque->m_dqCommands.at(pDDDoc->cmdDeque->m_nIndexCommQue);
			bResult=m_OMParser.OMParse(sCommand)>0;
			pDDDoc->cmdDeque->m_nIndexCommQue++;
		}

		pDDDoc->UpdateAllViews(NULL,1);//update all views without a region (lHint=1)

		// Update title (to add '*').
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)m_pMainWnd;
		pFrame->OnUpdateFrameTitle(TRUE);


	}

}
//Used to disable certain buttons that are relate to aplicacion 
BOOL CDraftDrawApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pExtra==NULL){
		
	}
	else{
		switch(nID){
		case ID_EDIT_REDO:{
			CDraftDrawDoc *pDoc=GetActiveDocument();
			if(pDoc&&!pDoc->cmdDeque->m_dqCommands.empty()){
				int idata=GetActiveDocument()->cmdDeque->m_dqCommands.size();
				if(GetActiveDocument()->cmdDeque->m_nIndexCommQue<idata){
					return TRUE;
				}
			}
			return FALSE;
			}break;
		case ID_EDIT_UNDO:{
			CDraftDrawDoc *pDoc=GetActiveDocument();
			if(pDoc&&!pDoc->cmdDeque->m_dqCommandsUndo.empty()){
				if(GetActiveDocument()->cmdDeque->m_nIndexCommQue>0||(GetActiveDocument()->cmdDeque->m_nIndexCommQue==0&&GetActiveDocument()->cmdDeque->m_bBackwards)){
					return TRUE;
				}
			}
			return FALSE;
			}break;
		case ID_EDIT_RACK:{
			CDraftDrawDoc *pDoc=GetActiveDocument();
			if(pDoc){
				return TRUE;
			}
			return FALSE;
			}break;
		}
	}

	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDraftDrawApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//CWinApp::WinHelp(dwData, nCmd);

	switch (nCmd)
	{
		case HELP_CONTEXT: 
			::HtmlHelp(NULL, ".\\hlp\\DraftCable.chm",HH_HELP_CONTEXT, dwData );
			break;
		case HELP_FINDER: 
			::HtmlHelp(NULL, ".\\hlp\\DraftCable.chm",HH_DISPLAY_TOPIC, 0);
		         break;	
	}
}

int CDraftDrawApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	GdiplusShutdown(g_gdiplusToken);

	return CWinApp::ExitInstance();
}

void CDraftDrawApp::MyMRUFileHandler(UINT i)
{
	/*
	ASSERT_VALID(this);
	ENSURE(m_pRecentFileList != NULL);

	ENSURE_ARG(nID >= ID_FILE_MRU_FILE1);
	ENSURE_ARG(nID < ID_FILE_MRU_FILE1 + (UINT)m_pRecentFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

	TRACE(traceAppMsg, 0, _T("MRU: open file (%d) '%s'.\n"), (nIndex) + 1,
	(LPCTSTR)(*m_pRecentFileList)[nIndex]);

	g_bRemoveFromMRU = TRUE;
	if (OpenDocumentFile((*m_pRecentFileList)[nIndex]) == NULL)
	{
	if (g_bRemoveFromMRU)
	{
	m_pRecentFileList->Remove(nIndex);
	}
	}
	g_bRemoveFromMRU = TRUE;

	return TRUE;
	*/


	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList != NULL);

	ASSERT(i >= ID_FILE_MRU_FILE1);
	ASSERT(i < ID_FILE_MRU_FILE1 + (UINT)m_pRecentFileList->GetSize());

	CString strName, strCurDir, strMessage;
	int nIndex = i - ID_FILE_MRU_FILE1;
	ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

	strName.Format("MRU: open file (%d) '%s'.\n", (nIndex)+1, (LPCTSTR)(*m_pRecentFileList)[nIndex]);

	if (OpenDocumentFile((*m_pRecentFileList)[nIndex]) == NULL)
		m_pRecentFileList->Remove(nIndex);
	//AfxMessageBox(strName);


	GetActiveDocument()->UpdateAllViews(NULL, 1);

}
